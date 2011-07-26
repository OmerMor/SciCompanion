
#include "stdafx.h"
#include "ViewResource.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// As long as these values are unsigned, there's no need to check for 0
#define CHECK_RECT(x,y)\
    ( x < sPIC_WIDTH && y < sPIC_HEIGHT )

#define BYTE_FROM_PALETTE_AND_OFFSET(num, offset)   ((BYTE) (((num) * PALETTE_SIZE) + (offset)))
#define BYTE_FROM_EGACOLOR(color)                   ((BYTE) ((color).color1 + ((color).color2 << 4)))

#define GET_PALETTE(palettes, number) ((palettes) + (number) * PALETTE_SIZE)

//
// This file contains code that is based on the ditherfill, pen and line-drawing code from SCISTudio3,
// by Brian Provinciano.
//

extern const EGACOLOR *g_defaultPalette = (EGACOLOR*)g_defaultPaletteInit;
extern SIZE_T cbDefaultPalette = sizeof(g_defaultPaletteInit);


extern WORD g_iPriorityBands[15] = { 43, 53, 64, 74, 85, 96, 106, 117, 127, 138, 149, 159, 170, 180, 190 };

//
// Given a y-coordinate, returns the priority at that position.
//
BYTE PriorityFromY(WORD y)
{
    BYTE bPriority = 0;
    for (bPriority = 0; bPriority < ARRAYSIZE(g_iPriorityBands); bPriority++)
    {
        if (y < g_iPriorityBands[bPriority])
            break;
    }
    return bPriority;
}

//
// Helpers for converting an arbitrary index to and from pattern info.
// The arbitrary index is used for the position of a square in a dialog that
// represents the different types of patterns.
//
void PatternInfoFromIndex(BYTE bIndex, PenStyle *pPenStyle)
{
    // Pattern size resets every 8.
    pPenStyle->bPatternSize = bIndex % 8;
    // fPattern alterates every 8, from solid->pattern->solid->pattern
    pPenStyle->fPattern = ((bIndex / 8) % 2);
    pPenStyle->bPatternNR = 0;
    // fRectangle is true for >= 16
    pPenStyle->fRectangle = (bIndex / 16);
}

BYTE IndexFromPatternInfo(const PenStyle *pPenStyle)
{
    BYTE bIndex = 0;
    if (pPenStyle->fRectangle)
    {
        bIndex += 16;
    }
    if (pPenStyle->fPattern)
    {
        bIndex += 8;
    }
    bIndex += pPenStyle->bPatternSize;
    return bIndex;
}

BOOL IsPenStyleEqual(const PenStyle *pPenStyle1, const PenStyle *pPenStyle2)
{
    return (  (pPenStyle1->bPatternNR == pPenStyle2->bPatternNR) &&
              (pPenStyle1->bPatternSize == pPenStyle2->bPatternSize) &&
              (pPenStyle1->fRectangle == pPenStyle2->fRectangle) &&
              (pPenStyle1->fPattern == pPenStyle2->fPattern) &&
              (pPenStyle1->fRandomNR == pPenStyle2->fRandomNR) );
}


BYTE c_SizeToOpcodeLine[] = { PIC_OP_RELATIVE_SHORT_LINES, PIC_OP_RELATIVE_MEDIUM_LINES, PIC_OP_RELATIVE_LONG_LINES };
BYTE c_SizeToOpcodePen[] =  { PIC_OP_RELATIVE_PATTERNS, PIC_OP_RELATIVE_MEDIUM_PATTERNS, PIC_OP_ABSOLUTE_PATTERNS };

BYTE OpcodeFromCommandTypeAndSize(BYTE bType, DRAWSIZE size)
{
    if (bType == PicCommand::Line)
    {
        return c_SizeToOpcodeLine[size];
    }
    else if (bType == PicCommand::Pattern)
    {
        return c_SizeToOpcodePen[size];
    }
    ASSERT(FALSE);
    return PIC_END;
}

DRAWSIZE DrawSizeFromCoords(WORD x1, WORD y1, WORD x2, WORD y2)
{
    DRAWSIZE dsReturn = DS_LARGE;
    __int16 dx = abs(((__int16)x1) - (__int16)x2);
    __int16 dy = abs(((__int16)y1) - (__int16)y2);

    if ((dx <= 6) && (dy <= 7))  // This keeps the byte below 0xf0
    {
        // Each coord is within 6 or 7 pixels of the previous
        dsReturn = DS_SMALL;
    }
    else if ((dx <= 0x7f) && (dy <= 0x6f))
    {
        // x is within 127 pixels (128 is ok for +ve, but who cares) and y is within 111
        dsReturn = DS_MEDIUM;
    }
    return dsReturn;
}



//
// Looks at the x,y of the next command, and sees which of three categories it falls
// into.  If the command is of a different type, it returns DS_LARGE.
//
DRAWSIZE DrawSizeFromNextCommand(PicCommand *pCommand, PicCommand *pNextCommand)
{
    DRAWSIZE dsReturn = DS_LARGE;
    if ((pCommand->type == PicCommand::Line) && (pNextCommand->type == PicCommand::Line) &&
        (pCommand->drawLine.xTo == pNextCommand->drawLine.xFrom) &&
        (pCommand->drawLine.yTo == pNextCommand->drawLine.yFrom))
    {
        dsReturn = DrawSizeFromCoords(pNextCommand->drawLine.xFrom,
                                      pNextCommand->drawLine.yFrom,
                                      pNextCommand->drawLine.xTo,
                                      pNextCommand->drawLine.yTo);
    }
    else if ((pCommand->type == PicCommand::Pattern) && (pNextCommand->type == PicCommand::Pattern))
    {
        dsReturn = DrawSizeFromCoords(pCommand->drawPattern.x,
                                      pCommand->drawPattern.y,
                                      pNextCommand->drawPattern.x,
                                      pNextCommand->drawPattern.y);
    }
    return dsReturn;
}




//
// Get name functions
//
void LineCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void PatternCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void FillCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void SetVisualCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void SetPriorityCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void SetControlCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void DisableVisualCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void DisablePriorityCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void DisableControlCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void SetPaletteCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void SetPaletteEntryCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void SetVisualCommandSCI1_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void SetPriorityBarsCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void DrawVisualBitmap_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void SetPaletteSCI1_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
void CircleCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);

DRAW_FUNCTION g_DrawFunctions[] =
{
    LineCommand_Draw,
    PatternCommand_Draw,
    FillCommand_Draw,
    SetVisualCommand_Draw,
    SetPriorityCommand_Draw,
    SetControlCommand_Draw,
    DisableVisualCommand_Draw,
    DisablePriorityCommand_Draw,
    DisableControlCommand_Draw,
    SetPaletteCommand_Draw,
    SetPaletteEntryCommand_Draw,
    SetVisualCommandSCI1_Draw,
    SetPriorityBarsCommand_Draw,
    DrawVisualBitmap_Draw,
    SetPaletteSCI1_Draw,
    CircleCommand_Draw,
};





BOOL _WriteColor(sci::istream *pSerial, EGACOLOR color)
{
    pSerial->WriteByte(color.color1 | (color.color2 << 4));
    return TRUE;
}



BOOL _WriteAbsCoordinate(sci::istream *pSerial, WORD x, WORD y)
{
    // Coordinate prefix | x / 256 | y / 256
    // Coord prefix byte is xxxxyyyy, where xxxx = x % 256, yyyy = y % 256.
    pSerial->WriteByte((BYTE) ( ((x >> 4) & 0x00f0) | (y >> 8))); // It is redundant to & y with 0x000f
    pSerial->WriteByte((BYTE) (x & 0x00ff));
    pSerial->WriteByte((BYTE) (y & 0x00ff));
    return TRUE;
}

BOOL _WriteMediumCoordinate(sci::istream *pSerial, WORD xOld, WORD yOld, WORD x, WORD y)
{
    // x | y
    BYTE byte;
    if (y < yOld)
    {
        byte = 0x80 | (BYTE)(yOld - y);
    }
    else
    {
        byte = (BYTE)(y - yOld);
    }
    ASSERT(byte < 0xf0); // Since 0xf0 signifies the next opcode.
    pSerial->WriteByte(byte);

    byte = (BYTE)(((__int16)x) - (__int16)xOld);
    return pSerial->WriteByte(byte);
}

BOOL _WriteSmallCoordinate(sci::istream *pSerial, WORD xOld, WORD yOld, WORD x, WORD y)
{
    // x,y
    // where xxxxyyyy, and leftmost bits of each indicate -ve or +ve
    BYTE byte = 0;
    if (x < xOld)
    {
        byte |= (((xOld - x) << 4) | 0x80);
    }
    else
    {
        byte |= ((x - xOld) << 4);
    }
    if (y < yOld)
    {
        byte |= ((yOld - y) | 0x08);
    }
    else
    {
        byte |= (y - yOld);
    }

    return pSerial->WriteByte(byte);
}

BOOL _WriteCoordinate(sci::istream *pSerial, DRAWSIZE ds, WORD xOld, WORD yOld, WORD x, WORD y)
{
    BOOL bRet = FALSE;
    switch (ds)
    {
    case DS_SMALL:
        bRet = _WriteSmallCoordinate(pSerial, xOld, yOld, x, y);
        break;
    case DS_MEDIUM:
        bRet = _WriteMediumCoordinate(pSerial, xOld, yOld, x, y);
        break;
    case DS_LARGE:
        bRet = _WriteAbsCoordinate(pSerial, x, y); // Don't need the old coord
        break;
    }
    ASSERT("Error writing coordinate: invalid drawsize");
    return bRet;
}


SERIALIZE_FUNCTION g_SerializeFunctions[] =
{
    LineCommand_Serialize,
    PatternCommand_Serialize,
    FillCommand_Serialize,
    SetVisualCommand_Serialize,
    SetPriorityCommand_Serialize,
    SetControlCommand_Serialize,
    DisableVisualCommand_Serialize,
    DisablePriorityCommand_Serialize,
    DisableControlCommand_Serialize,
    SetPaletteCommand_Serialize,
    SetPaletteEntryCommand_Serialize,
    SetVisualCommandSCI1_Serialize,
    SetPriorityBarsCommand_Serialize,
    DrawVisualBitmap_Serialize,
    SetPaletteSCI1_Serialize,
    CircleCommand_Serialize,
};

GETNAME_FUNCTION g_GetNameFunctions[] =
{
    LineCommand_GetName,
    PatternCommand_GetName,
    FillCommand_GetName,
    SetVisualCommand_GetName,
    SetPriorityCommand_GetName,
    SetControlCommand_GetName,
    DisableVisualCommand_GetName,
    DisablePriorityCommand_GetName,
    DisableControlCommand_GetName,
    SetPaletteCommand_GetName,
    SetPaletteEntryCommand_GetName,
    SetVisualCommandSCI1_GetName,
    SetPriorityBarsCommand_GetName,
    DrawVisualBitmap_GetName,
    SetPaletteSCI1_GetName,
    CircleCommand_GetName,
};


void PicCommand::Draw(PicData *pData, SCIPicState &state) const
{
    g_DrawFunctions[type](this, pData, &state);
}


void PicCommand::GetName(TCHAR *pszBuf, size_t cchBuf) const
{
    g_GetNameFunctions[type](this, pszBuf, cchBuf);
}

BOOL Command_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    return g_SerializeFunctions[pCommand->type](pSerial, pCommand, pCommandPrev, pCommandNext, dsPrev, pds, pState);
}


//
// Given a bunch of pasted PICCOMMANDs', figure out the visual bounding rect that contains all the commands.
//
void PastedCommands_GetBounds(const PicCommand *pCommand, INT_PTR cCommands, sRECT *prc)
{
    prc->left = sPIC_MAXX;
    prc->right = 0;
    prc->top = sPIC_MAXY;
    prc->bottom = 0;

    const PicCommand *pCurrent = pCommand;
    for (INT_PTR i = 0; i < cCommands; i++)
    {
        switch (pCurrent->type)
        {
        case PicCommand::Line:
            prc->left = min(prc->left, pCurrent->drawLine.xFrom);
            prc->left = min(prc->left, pCurrent->drawLine.xTo);
            prc->right = max(prc->right, pCurrent->drawLine.xFrom);
            prc->right = max(prc->right, pCurrent->drawLine.xTo);
            prc->top = min(prc->top, pCurrent->drawLine.yFrom);
            prc->top = min(prc->top, pCurrent->drawLine.yTo);
            prc->bottom = max(prc->bottom, pCurrent->drawLine.yFrom);
            prc->bottom = max(prc->bottom, pCurrent->drawLine.yTo);
            break;

        case PicCommand::Pattern:
            prc->left = min(prc->left, pCurrent->drawPattern.x);
            prc->right = max(prc->right, pCurrent->drawPattern.x);
            prc->top = min(prc->top, pCurrent->drawPattern.y);
            prc->bottom = max(prc->bottom, pCurrent->drawPattern.y);
            break;

        case PicCommand::Fill:
            prc->left = min(prc->left, pCurrent->fill.x);
            prc->right = max(prc->right, pCurrent->fill.x);
            prc->top = min(prc->top, pCurrent->fill.y);
            prc->bottom = max(prc->bottom, pCurrent->fill.y);
            break;
        case PicCommand::Circle:
            prc->left = min(prc->left, pCurrent->circle.xFrom);
            prc->left = min(prc->left, pCurrent->circle.xTo);
            prc->right = max(prc->right, pCurrent->circle.xFrom);
            prc->right = max(prc->right, pCurrent->circle.xTo);
            prc->top = min(prc->top, pCurrent->circle.yFrom);
            prc->top = min(prc->top, pCurrent->circle.yTo);
            prc->bottom = max(prc->bottom, pCurrent->circle.yFrom);
            prc->bottom = max(prc->bottom, pCurrent->circle.yTo);
        default:
            // Not a coordinate command
            break;
        }
        pCurrent++;
    }

    // The rect bottom and right need to be "off the edge"
    prc->bottom += 1;
    prc->right += 1;

    // TODO: Give it a min size.
}


BOOL PastedCommands_ContainDrawCommands(const PicCommand *pCommands, INT_PTR cCommands)
{
    BOOL fContain = FALSE;
    for (INT_PTR i = 0; (!fContain) && (i < cCommands); i++)
    {
        switch (pCommands[i].type)
        {
        case PicCommand::Line:
        case PicCommand::Pattern:
        case PicCommand::Fill:
        case PicCommand::Circle:
            fContain = TRUE;
            break;
        }
    }
    return fContain;
}

//
// Returns FALSE if the coordiante is out of bounds.
//
BOOL Coord_Adjust(WORD *px, WORD *py, const PICCOMMAND_ADJUST *pAdjust)
{
    int x = (int)*px;
    int y = (int)*py;

    // Flip.
    if (pAdjust->fHFlip)
    {
        x = pAdjust->rcBounds.right - (x - pAdjust->rcBounds.left); 
    }
    if (pAdjust->fVFlip)
    {
        y = pAdjust->rcBounds.bottom - (y - pAdjust->rcBounds.top); 
    }

    int dx = x - pAdjust->rcBounds.left;
    int dy = y - pAdjust->rcBounds.top;
    if (RECTWIDTH(pAdjust->rcBounds) > 0)
    {
        dx = dx * RECTWIDTH(pAdjust->rcNew) / RECTWIDTH(pAdjust->rcBounds);
    }
    x = dx + pAdjust->rcNew.left;
    if (RECTHEIGHT(pAdjust->rcBounds) > 0)
    {
        dy = dy * RECTHEIGHT(pAdjust->rcNew) / RECTHEIGHT(pAdjust->rcBounds);
    }
    y = dy + pAdjust->rcNew.top;

    // Now rotate them.
    if (pAdjust->iAngle != 0)
    {
        // TODO: 
        // 1) Find the rotation center, based on the stretching done above (rcNew/rcBounds)
        double xCenter = (double)((pAdjust->rcNew.right + pAdjust->rcNew.left) / 2);
        double yCenter = (double)((pAdjust->rcNew.bottom + pAdjust->rcNew.top) / 2);
        double xOrig = ((double)x) - xCenter;
        double yOrig = ((double)y) - yCenter;
        double fAngle = ((double)pAdjust->iAngle) / 360.0 * 6.28;
        double xNew = (cos(fAngle) * xOrig - sin(fAngle) * yOrig) + xCenter;
        double yNew = (sin(fAngle) * xOrig + cos(fAngle) * yOrig) + yCenter;
        x = (int)xNew;
        y = (int)yNew;
    }

    BOOL fRet = ((x >= 0) && (y >= 0) && (x < sPIC_WIDTH) && (y < sPIC_HEIGHT));

    // Truncation/overflow is ok, since we've checked the bounds above.
    *px = (WORD)x;
    *py = (WORD)y;

    return fRet;
}

//
// Returns FALSE if the command is out of bounds.
//
BOOL Command_Adjust(PicCommand *pCommand, const PICCOMMAND_ADJUST *pAdjust)
{
    BOOL fRet = TRUE;
    __int16 dx = (__int16)(pAdjust->rcNew.left - pAdjust->rcBounds.left);
    __int16 dy = (__int16)(pAdjust->rcNew.top - pAdjust->rcBounds.top);
    switch (pCommand->type)
    {
    case PicCommand::Line:
        // REVIEW: could clip the line instead of completely removing it, when it goes out of bounds.
        // Would need to use bresenham algorithm.
        fRet = Coord_Adjust(&pCommand->drawLine.xFrom, &pCommand->drawLine.yFrom, pAdjust);
        if (fRet)
        {
            fRet = Coord_Adjust(&pCommand->drawLine.xTo, &pCommand->drawLine.yTo, pAdjust);
        }
        break;
    case PicCommand::Circle:
        // REVIEW: could clip the line instead of completely removing it, when it goes out of bounds.
        // Would need to use bresenham algorithm.
        fRet = Coord_Adjust(&pCommand->circle.xFrom, &pCommand->circle.yFrom, pAdjust);
        if (fRet)
        {
            fRet = Coord_Adjust(&pCommand->circle.xTo, &pCommand->circle.yTo, pAdjust);
        }
        break;
    case PicCommand::Pattern:
        fRet = Coord_Adjust(&pCommand->drawPattern.x, &pCommand->drawPattern.y, pAdjust);
        break;

    case PicCommand::Fill:
        fRet = Coord_Adjust(&pCommand->fill.x, &pCommand->fill.y, pAdjust);
        break;
    }
    return fRet;
}

//
// Adjusts the commands by the offset.
// Commands that are out of bounds are removed from pCommandsIn.  The new number of commands is *pcCommandsOut.
//
void PastedCommands_Adjust(std::vector<PicCommand> &commandsIn, const PICCOMMAND_ADJUST *pAdjust)
{
    for (size_t i = 0; i < commandsIn.size(); i++)
    {
        if (Command_Adjust(&commandsIn[i], pAdjust))
        {
            // We're good
        }
        else
        {
            // This was out of bounds - remove it.
            commandsIn.erase(&commandsIn[i]);
        }
    }
}

HGLOBAL CopiedCommands_AllocAndFillMemory(const PicCommand *pCommands, INT_PTR cCommands)
{
    sci::istream serial;
    BOOL fOk = TRUE;
    for (INT_PTR i = 0; fOk && (i < cCommands); i++)
    {
        fOk = pCommands->Serialize(&serial);
        pCommands++; // Go to next command
    }

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, serial.GetDataSize());
    if (hMem)
    {
        BYTE *pData = (BYTE*)GlobalLock(hMem);
        if (pData)
        {
            serial.seekg(0);
            serial.read_data(pData, serial.GetDataSize());
            GlobalUnlock(hMem);
        }
        else
        {
            // Didn't work
            GlobalFree(hMem);
            hMem = NULL;
        }
    }
    return hMem;
}


BOOL PtInSRect(sRECT *prc, WORD x, WORD y)
{
    return ( (((__int16)x) >= prc->left) && (((__int16)x) < prc->right) && (((__int16)y) >= prc->top) && (((__int16)y) < prc->bottom));
}

void Command_DrawWithOffset(const PicCommand *pCommandIn, PicData *pData, SCIPicState *pState, const PICCOMMAND_ADJUST *pAdjust)
{
    PicCommand commandAdjusted = *pCommandIn; // class copy, since we don't want to modify pCommandIn (should be const).
    if (Command_Adjust(&commandAdjusted, pAdjust))
    {
        g_DrawFunctions[pCommandIn->type](&commandAdjusted, pData, pState);
    }
}


//
// Writes the picture out to a file
// 
BOOL SerializeAllCommands(sci::istream *pSerial, const std::vector<PicCommand> &commands, size_t cCommands)
{
    BOOL bRet = TRUE;
    PicCommand commandPrev;
    DRAWSIZE ds = DS_SMALL; // Initial value doesn't matter.
    SerializedPicState picState = { 0 };
    for (size_t i = 0; bRet && (i < cCommands); i++)
    {
        const PicCommand &command = commands[i];
        bRet = Command_Serialize(pSerial, &command, (i == 0) ? NULL : &commandPrev, (i == (cCommands - 1)) ? NULL : &commands[i + 1], ds, &ds, &picState);
        commandPrev = command;
    }
    
    if (bRet)
    {
        // Terminate the command stream.
        bRet = pSerial->WriteByte(PIC_END);
    }
    return bRet;
}


void CCoordinateTracker::AddCoord(WORD xFrom, WORD yFrom, WORD xTo, WORD yTo)
{
    _x.push_back(xFrom);
    _y.push_back(yFrom);
    _x.push_back(xTo);
    _y.push_back(yTo);
}

#define LIGHTUP_SENSITIVITY 20

//
// Call the callback with all the coordinates close to pt
//
void CCoordinateTracker::DrawCoords(CPoint pt, CDC *pDC, PFNCLOSECOORDCALLBACK pfnCallback, void *pv) const
{
    size_t cCount = _x.size();
    for (size_t i = 0; i < cCount; i += 2)
    {
        int x1 = (int)_x[i];
        int y1 = (int)_y[i];
        int x2 = (int)_x[i + 1];
        int y2 = (int)_y[i + 1];
        // If this is close to the cursor, then light it up
        if (((abs(pt.x - x1) < LIGHTUP_SENSITIVITY) && (abs(pt.y - y1) < LIGHTUP_SENSITIVITY)) ||
            ((abs(pt.x - x2) < LIGHTUP_SENSITIVITY) && (abs(pt.y - y2) < LIGHTUP_SENSITIVITY)))
        {
            (*pfnCallback)(pv, pDC, x1, y1, x2, y2);
        }
    }
}

CCoordinateTracker::CCoordinateTracker()
{
    _x.reserve(200);
    _y.reserve(200);
}

void CCoordinateTracker::Reset()
{
    _x.clear();
    _y.clear();
}

SCIPicState::SCIPicState(BYTE bPaletteToUse)
{
    Reset(bPaletteToUse);
}

SCIPicState::SCIPicState()
{
    SCIPicState(static_cast<BYTE>(0));
}

void SCIPicState::Reset(BYTE bPaletteToUse)
{
    dwDrawEnable = 0;
    bPriorityValue = 0;
    bControlValue = 0;
    color.color1 = 0;
    color.color2 = 0;
    bPaletteNumber = 0;
    bPaletteOffset = 0;
    bPatternNR = 0;
    bPatternSize = 0;
    bPaletteToDraw = bPaletteToUse;

    // Set all palettes to default:
    CopyMemory(GET_PALETTE(pPalettes, 0), g_defaultPalette, cbDefaultPalette);
    CopyMemory(GET_PALETTE(pPalettes, 1), g_defaultPalette, cbDefaultPalette);
    CopyMemory(GET_PALETTE(pPalettes, 2), g_defaultPalette, cbDefaultPalette);
    CopyMemory(GET_PALETTE(pPalettes, 3), g_defaultPalette, cbDefaultPalette);

    // The locked colours.
    ZeroMemory(rgLocked, sizeof(rgLocked));

    // SCI1 stuff
    ZeroMemory(rgPaletteSCI1, sizeof(rgPaletteSCI1));
    ZeroMemory(bPriorityLines, sizeof(bPriorityLines));

    tracker.Reset();
}

void InitPenStyle(PenStyle *pPenStyle)
{
    pPenStyle->fPattern = FALSE;
    pPenStyle->fRectangle = FALSE;
    pPenStyle->fRandomNR = TRUE;
    pPenStyle->bPatternSize = 0;
    pPenStyle->bPatternNR = 0;
}

inline void _PlotPixI(int p, PicData *pData, WORD x, WORD y, DWORD dwDrawEnable, EGACOLOR color, BYTE bPriorityValue, BYTE bControlValue)
{
    if ((pData->dwMapsToRedraw & DRAW_ENABLE_VISUAL) && (dwDrawEnable & DRAW_ENABLE_VISUAL))
    {
        pData->pdataVisual[p] = ((x^y) & 1)? color.color1 : color.color2;
    }
    if ((pData->dwMapsToRedraw & DRAW_ENABLE_PRIORITY) && (dwDrawEnable & DRAW_ENABLE_PRIORITY))
    {
        pData->pdataPriority[p] = bPriorityValue;
    }
    if ((pData->dwMapsToRedraw & DRAW_ENABLE_CONTROL) && (dwDrawEnable & DRAW_ENABLE_CONTROL))
    {
        pData->pdataControl[p] = bControlValue;
    }
}

inline void _PlotPix(PicData *pData, WORD x, WORD y, DWORD dwDrawEnable, EGACOLOR color, BYTE bPriorityValue, BYTE bControlValue)
{
    if( x >= 320 || y >= 190)
    {
        return;
    }

    int p = BUFFEROFFSET(x, y);
    //_PlotPixI(p, pData, x, y, dwDrawEnable, color, bPriorityValue, bControlValue);
    // Duplicate the code from _PlotPixI here for speed (perf increase of ~5%?)
    if ((pData->dwMapsToRedraw & DRAW_ENABLE_VISUAL) && (dwDrawEnable & DRAW_ENABLE_VISUAL))
    {
        pData->pdataVisual[p] = ((x^y) & 1)? color.color1 : color.color2;
    }
    if ((pData->dwMapsToRedraw & DRAW_ENABLE_PRIORITY) && (dwDrawEnable & DRAW_ENABLE_PRIORITY))
    {
        pData->pdataPriority[p] = bPriorityValue;
    }
    if ((pData->dwMapsToRedraw & DRAW_ENABLE_CONTROL) && (dwDrawEnable & DRAW_ENABLE_CONTROL))
    {
        pData->pdataControl[p] = bControlValue;
    }

    pData->pdataAux[p] |= (BYTE)dwDrawEnable;
}

void _PlotPixNonStd(int cx, int cy, PicData *pData, WORD x, WORD y, DWORD dwDrawEnable, EGACOLOR color, BYTE bPriorityValue, BYTE bControlValue)
{
    if( x >= cx || y >= cy)
    {
        return;
    }

    int p = BUFFEROFFSET_NONSTD(cx, cy, x, y);
    _PlotPixI(p, pData, x, y, dwDrawEnable, color, bPriorityValue, bControlValue);
}



#define LINEMACRO(pData, startx, starty, deltalinear, deltanonlinear, linearvar, nonlinearvar, \
                  linearend, nonlinearstart, linearmod, nonlinearmod) \
   x = (startx); y = (starty); \
   incrNE = ((deltalinear) > 0)? (deltalinear) : -(deltalinear); \
   incrNE <<= 1; \
   deltanonlinear <<= 1; \
   incrE = ((deltanonlinear) > 0) ? -(deltanonlinear) : (deltanonlinear);  \
   d = nonlinearstart-1;  \
   while (linearvar != (linearend)) { \
     _PlotPix(pData, x,y, dwDrawEnable, color, bPriorityValue, bControlValue); \
     linearvar += linearmod; \
     if ((d+=incrE) < 0) { \
       d += incrNE; \
       nonlinearvar += nonlinearmod; \
     }; \
   }; \
   _PlotPix(pData, x, y, dwDrawEnable, color, bPriorityValue, bControlValue);

void _DitherLine(PicData *pData, WORD xStart, WORD yStart, WORD xEnd, WORD yEnd, EGACOLOR color, BYTE bPriorityValue, BYTE bControlValue, DWORD dwDrawEnable)
{
    int dx, dy, incrE, incrNE, d, finalx, finaly;
    int x = (int)xStart;
    int y = (int)yStart;
    dx = (int)xEnd - (int)x;
    dy = (int)yEnd - (int)y;
    finalx = (int)xEnd;
    finaly = (int)yEnd;

    dx = abs(dx);
    dy = abs(dy);

    if (dx > dy) {
        if (finalx < x) {
            if (finaly < y) { /* llu == left-left-up */
                LINEMACRO(pData, x, y, dx, dy, x, y, finalx, dx, -1, -1);
            } else {         /* lld */
                LINEMACRO(pData, x, y, dx, dy, x, y, finalx, dx, -1, 1);
            }
        } else { /* x1 >= x */
            if (finaly < y) { /* rru */
                LINEMACRO(pData, x, y, dx, dy, x, y, finalx, dx, 1, -1);
            } else {         /* rrd */
                LINEMACRO(pData, x, y, dx, dy, x, y, finalx, dx, 1, 1);
            }
        }
    } else { /* dx <= dy */
        if (finaly < y) {
            if (finalx < x) { /* luu */
                LINEMACRO(pData, x, y, dy, dx, y, x, finaly, dy, -1, -1);
            } else {         /* ruu */
                LINEMACRO(pData, x, y, dy, dx, y, x, finaly, dy, -1, 1);
            }
        } else { /* y1 >= y */
            if (finalx < x) { /* ldd */
                LINEMACRO(pData, x, y, dy, dx, y, x, finaly, dy, 1, -1);
            } else {         /* rdd */
                LINEMACRO(pData, x, y, dy, dx, y, x, finaly, dy, 1, 1);
            }
        }
    }
}


BYTE circles[][30] = { /* bitmaps for circle patterns */
{0x80},
{0x4e, 0x40},
{0x73, 0xef, 0xbe, 0x70},
{0x38, 0x7c, 0xfe, 0xfe, 0xfe, 0x7c, 0x38, 0x00},
{0x1c, 0x1f, 0xcf, 0xfb, 0xfe, 0xff, 0xbf, 0xef, 0xf9, 0xfc, 0x1c},
{0x0e, 0x03, 0xf8, 0x7f, 0xc7, 0xfc, 0xff, 0xef, 0xfe, 0xff, 0xe7,
    0xfc, 0x7f, 0xc3, 0xf8, 0x1f, 0x00},
{0x0f, 0x80, 0xff, 0x87, 0xff, 0x1f, 0xfc, 0xff, 0xfb, 0xff, 0xef,
    0xff, 0xbf, 0xfe, 0xff, 0xf9, 0xff, 0xc7, 0xff, 0x0f, 0xf8,
    0x0f, 0x80},
{0x07, 0xc0, 0x1f, 0xf0, 0x3f, 0xf8, 0x7f, 0xfc, 0x7f, 0xfc, 0xff,
    0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0x7f,
    0xfc, 0x7f, 0xfc, 0x3f, 0xf8, 0x1f, 0xf0, 0x07, 0xc0}};
BYTE junq[32] = { /* random-looking fill pattern */
0x20, 0x94, 0x02, 0x24, 0x90, 0x82, 0xa4, 0xa2, 0x82, 0x09, 0x0a, 0x22,
0x12, 0x10, 0x42, 0x14, 0x91, 0x4a, 0x91, 0x11, 0x08, 0x12, 0x25, 0x10,
0x22, 0xa8, 0x14, 0x24, 0x00, 0x50, 0x24, 0x04};
BYTE junqindex[128] = { /* starting points for junq fill */
0x00, 0x18, 0x30, 0xc4, 0xdc, 0x65, 0xeb, 0x48,
0x60, 0xbd, 0x89, 0x05, 0x0a, 0xf4, 0x7d, 0x7d,
0x85, 0xb0, 0x8e, 0x95, 0x1f, 0x22, 0x0d, 0xdf,
0x2a, 0x78, 0xd5, 0x73, 0x1c, 0xb4, 0x40, 0xa1,
0xb9, 0x3c, 0xca, 0x58, 0x92, 0x34, 0xcc, 0xce,
0xd7, 0x42, 0x90, 0x0f, 0x8b, 0x7f, 0x32, 0xed,
0x5c, 0x9d, 0xc8, 0x99, 0xad, 0x4e, 0x56, 0xa6,
0xf7, 0x68, 0xb7, 0x25, 0x82, 0x37, 0x3a, 0x51,
0x69, 0x26, 0x38, 0x52, 0x9e, 0x9a, 0x4f, 0xa7,
0x43, 0x10, 0x80, 0xee, 0x3d, 0x59, 0x35, 0xcf,
0x79, 0x74, 0xb5, 0xa2, 0xb1, 0x96, 0x23, 0xe0,
0xbe, 0x05, 0xf5, 0x6e, 0x19, 0xc5, 0x66, 0x49,
0xf0, 0xd1, 0x54, 0xa9, 0x70, 0x4b, 0xa4, 0xe2,
0xe6, 0xe5, 0xab, 0xe4, 0xd2, 0xaa, 0x4c, 0xe3,
0x06, 0x6f, 0xc6, 0x4a, 0xa4, 0x75, 0x97, 0xe1
};

//
// fPattern:        pattern or solid?
// fRectangle:      rect or circle?
//
void _DrawPattern(PicData *pData, WORD x, WORD y, EGACOLOR color, BYTE bPriorityValue, BYTE bControlValue, DWORD dwDrawEnable, BOOL fPattern, BYTE bPatternSize, BYTE bPatternNR, BOOL fRectangle)
{
    WORD wSize = (WORD)bPatternSize;

    // Fix up x and y
    if (x < wSize)
    {
        x = wSize;
    }
    if ((x + wSize) > 319)
    {
        x = 319 - wSize;
    }
    if (y < wSize)
    {
        y = wSize;
    }
    if ((y + wSize) > 189)
    {
        y = 189 - wSize;
    }

    if (bPatternNR < ARRAYSIZE(junqindex))
    {
        BYTE junqbit = junqindex[bPatternNR];
        if (fRectangle)
        {
            WORD k, l;
            for (l = y - wSize; l <= y + wSize; l++)
            {
                for (k = x - wSize; k <= (x + wSize + 1); k++)
                {
                    if (fPattern)
                    {
                        if ( (junq[junqbit>>3] >> (7-(junqbit & 7))) & 1)
                        {
                            _PlotPix(pData, k, l, dwDrawEnable, color, bPriorityValue, bControlValue);
                        }
                        junqbit++;
                        if (junqbit == 0xff)
                        {
                            junqbit = 0;
                        }
                    }
                    else
                    {
                        _PlotPix(pData, k, l, dwDrawEnable, color, bPriorityValue, bControlValue);
                    }
                }
            }
        }
        else
        {
            WORD k, l;
            // Circle
            int circlebit = 0;
            for (l = y - wSize; l <= y + wSize; l++)
            {
                for (k = x- wSize; k <= x + wSize + 1; k++)
                {
                    if ((circles[bPatternSize][circlebit>>3] >> (7-(circlebit & 7))) & 1)
                    {
                        if (fPattern)
                        {
                            if ((junq[junqbit>>3] >> (7-(junqbit & 7))) & 1)
                            {
                                _PlotPix(pData, k, l, dwDrawEnable, color, bPriorityValue, bControlValue);
                            }
                            junqbit++;
                            if (junqbit == 0xff)
                            {
                                junqbit = 0;
                            }
                        }
                        else
                        {
                            _PlotPix(pData, k, l, dwDrawEnable, color, bPriorityValue, bControlValue);
                        }
                    }
                    circlebit++;
                }
            }
        }
    }
    else
    {
        REPORTERROR(TEXT("bPatternNR was too high"));
    }
}




//
// REVIEW: This sucks - I had to make a full copy of this function, just to allow it to draw to something smaller
// than 320 x 190.  (don't want to pass extra params to _PlotPix due to perf)
//
void DrawPatternInRect(int cx, int cy, PicData *pData, WORD x, WORD y, EGACOLOR color, BYTE bPriorityValue, BYTE bControlValue, DWORD dwDrawEnable, const PenStyle *pPenStyle)
{
    WORD wSize = (WORD)pPenStyle->bPatternSize;

    // Fix up x and y
    if (x < wSize)
    {
        x = wSize;
    }
    if ((x + wSize) > (cx - 1))
    {
        x = (cx - 1) - wSize;
    }
    if (y < wSize)
    {
        y = wSize;
    }
    if ((y + wSize) > (cy - 1))
    {
        y = (cy - 1) - wSize;
    }

    if (pPenStyle->bPatternNR < ARRAYSIZE(junqindex))
    {
        BYTE junqbit = junqindex[pPenStyle->bPatternNR];
        if (pPenStyle->fRectangle)
        {
            WORD k, l;
            for (l = y - wSize; l <= y + wSize; l++)
            {
                for (k = x - wSize; k <= (x + wSize + 1); k++)
                {
                    if (pPenStyle->fPattern)
                    {
                        if ( (junq[junqbit>>3] >> (7-(junqbit & 7))) & 1)
                        {
                            _PlotPixNonStd(cx, cy, pData, k, l, dwDrawEnable, color, bPriorityValue, bControlValue);
                        }
                        junqbit++;
                        if (junqbit == 0xff)
                        {
                            junqbit = 0;
                        }
                    }
                    else
                    {
                        _PlotPixNonStd(cx, cy, pData, k, l, dwDrawEnable, color, bPriorityValue, bControlValue);
                    }
                }
            }
        }
        else
        {
            WORD k, l;
            // Circle
            int circlebit = 0;
            for (l = y - wSize; l <= y + wSize; l++)
            {
                for (k = x- wSize; k <= x + wSize + 1; k++)
                {
                    if ((circles[pPenStyle->bPatternSize][circlebit>>3] >> (7-(circlebit & 7))) & 1)
                    {
                        if (pPenStyle->fPattern)
                        {
                            if ((junq[junqbit>>3] >> (7-(junqbit & 7))) & 1)
                            {
                                _PlotPixNonStd(cx, cy, pData, k, l, dwDrawEnable, color, bPriorityValue, bControlValue);
                            }
                            junqbit++;
                            if (junqbit == 0xff)
                            {
                                junqbit = 0;
                            }
                        }
                        else
                        {
                            _PlotPixNonStd(cx, cy, pData, k, l, dwDrawEnable, color, bPriorityValue, bControlValue);
                        }
                    }
                    circlebit++;
                }
            }
        }
    }
    else
    {
        REPORTERROR(TEXT("bPatternNR was too high"));
    }
}

DWORD MakeViewResourceIndexFromCelAndLoop(const ViewResource *pvr, int nLoop, int nCel)
{
    nLoop = nLoop % pvr->GetLoopCount();
    if (nLoop < 0)
    {
        nLoop = pvr->GetLoopCount() + nLoop;
        ASSERT(nLoop > 0);
    }
    nCel = nCel % pvr->GetCelCount(nLoop);
    if (nCel < 0)
    {
        nCel = pvr->GetCelCount(nLoop) + nCel;
        ASSERT(nCel > 0);
    }
    return MAKE_INDEX(nLoop, nCel);
}

//
// Give a view, and its position, returns the point that is at the
// center of the view.
//
CPoint FindCenterOfView(WORD xEgo, WORD yEgo, const ViewResource *pvr, int nLoop, int nCel)
{
    DWORD dwIndex = MakeViewResourceIndexFromCelAndLoop(pvr, nLoop, nCel);
    CSize size = pvr->GetSize(dwIndex);
    CPoint pointOrigin = pvr->GetPlacement(dwIndex);
    int xiEgo = (int)xEgo + pointOrigin.x;
    int yiEgo = (int)yEgo + pointOrigin.y;

    return CPoint(xiEgo, yiEgo - size.cy / 2);
}

#define YSTEP 2 // We may want to allow the user to change this.
CRect GetViewBoundsRect(WORD xEgo, WORD yEgo, const ViewResource *pvr, int nLoop, int nCel)
{
    DWORD dwIndex = MakeViewResourceIndexFromCelAndLoop(pvr, nLoop, nCel);
    CSize size = pvr->GetSize(dwIndex);
    CPoint pointOrigin = pvr->GetPlacement(dwIndex);
    CRect rect;
    rect.left = xEgo - pointOrigin.x - (size.cx / 2);
    rect.right = rect.left + size.cx;
    rect.bottom = yEgo + 1;
    rect.top = rect.bottom - YSTEP;
    return rect;
}

//
// Determines if an object bounded by rect, can be there, based on the control mask.
// rect is inclusive (e.g. includes both left and right columns, and top and bottom lines)
//
bool CanBeHere(const BYTE *pdataControl, const CRect &rect, WORD wControlMask)
{
    CRect rcClipped = rect;
    rcClipped.left = max(rcClipped.left, 0);
    rcClipped.top = max(rcClipped.top, 0);
    rcClipped.bottom = min(rcClipped.bottom, sPIC_HEIGHT - 1);
    rcClipped.right = min(rcClipped.right, sPIC_WIDTH - 1);
    for (int y = rcClipped.top; y <= rcClipped.bottom; y++)
    {
        for (int x = rcClipped.left; x <= rcClipped.right; x++)
        {
            int p = BUFFEROFFSET(x, y);
            ASSERT(pdataControl[p] <= 15);
            if (wControlMask & (1 << (WORD)pdataControl[p]))
            {
                return false;
            }
        }
    }
    return true;
}

BOOL HitTestView(WORD xCursor, WORD yCursor, WORD xEgo, WORD yEgo, const ViewResource *pvr, int nLoop, int nCel)
{
    DWORD dwIndex = MakeViewResourceIndexFromCelAndLoop(pvr, nLoop, nCel);
    CSize size = pvr->GetSize(dwIndex);
    CPoint pointOrigin = pvr->GetPlacement(dwIndex);
    int xiEgo = (int)xEgo + pointOrigin.x;
    int yiEgo = (int)yEgo + pointOrigin.y;
    if ((xiEgo >= 0) && (yiEgo >= 0))
    {
        return HitTestEgoBox(xCursor, yCursor, (WORD)xiEgo, (WORD)yiEgo, (WORD)size.cx, (WORD)size.cy);
    }
    else
    {
        return FALSE;
    }
}

void GetViewRect(CRect *prc, WORD xEgo, WORD yEgo, const ViewResource *pvr, int nLoop, int nCel)
{
    DWORD dwIndex = MakeViewResourceIndexFromCelAndLoop(pvr, nLoop, nCel);
    CSize size = pvr->GetSize(dwIndex);
    CPoint pointOrigin = pvr->GetPlacement(dwIndex);
    int xiEgo = (int)xEgo + pointOrigin.x;
    int yiEgo = (int)yEgo + pointOrigin.y;

    prc->top = yiEgo - size.cy;
    prc->bottom = yiEgo;
    prc->left = xiEgo - size.cx / 2;
    prc->right = xiEgo + size.cx / 2;
}


//
// y is at the bottom of ego, x is in the middle (just like the game)
//
BOOL HitTestEgoBox(WORD xCursor, WORD yCursor, WORD xEgo, WORD yEgo, WORD cx, WORD cy)
{
    WORD cxHalf = cx / 2;

    if (yCursor > yEgo)
    {
        return FALSE;
    }

    WORD ySafeTop;
    if (yEgo < cy)
    {
        ySafeTop = 0;
    }
    else
    {
        ySafeTop = yEgo - cy;
    }
    if (yCursor < ySafeTop)
    {
        return FALSE;
    }

    if (xCursor > (cxHalf + xEgo))
    {
        return FALSE;
    }

    WORD xSafeLeft;
    if (xEgo < cxHalf)
    {
        xSafeLeft = 0;
    }
    else
    {
        xSafeLeft = xEgo - cxHalf;
    }
    if (xCursor < xSafeLeft)
    {
        return FALSE;
    }

    return TRUE;
}


//
// Draws a view (represented by pvr) onto a pic (represented by pdataDisplay and pdataPriority)
//
void DrawViewWithPriority(BYTE *pdataDisplay, const BYTE *pdataPriority, BYTE bEgoPriority, WORD xIn, WORD yIn, const ViewResource *pvr, int nLoop, int nCel, BOOL fShowOutline)
{
    DWORD dwIndex = MakeViewResourceIndexFromCelAndLoop(pvr, nLoop, nCel);
    CPoint pointOrigin = pvr->GetPlacement(dwIndex);
    CSize size = pvr->GetSize(dwIndex);
    BYTE bTransparent = pvr->GetTransparentColor(dwIndex);

    yIn += (WORD)pointOrigin.y;
    xIn += (WORD)pointOrigin.x;
    
    int xLeft = xIn - (size.cx / 2);
    int yBottom = yIn;
    int yTop = yIn - size.cy + 1;
    int xRight = xLeft + size.cx;

    scoped_array<BYTE> data(new BYTE[CX_ACTUAL(size.cx) * size.cy]);
    pvr->CopyBitmapData(dwIndex, data.get(), size);
    int yView = 0;
    for (int yPic = yTop; yPic <= yBottom; yPic++, yView++)
    {
        if ((yPic < sPIC_HEIGHT) && (yPic >= 0))
        {
            int xViewLeft = 0;
            if (xLeft < 0)
            {
                // If we're off the left of the pic, don't copy from the beginning of the view's row
                xViewLeft = -xLeft;
            }
            int xViewRight = size.cx;
            if (xRight >= sPIC_WIDTH)
            {
                xViewRight -= (xRight - sPIC_WIDTH);
            }

            int xFirstPixel = -1;
            int xLastPixel = -1;
            int xPic = max(0, xLeft);
            for (int xView = xViewLeft; xView < xViewRight; xView++, xPic++)
            {
                int pPicOffset = BUFFEROFFSET(xPic, yPic);
                // We can draw something.
                int pViewOffset = BUFFEROFFSET_NONSTD(CX_ACTUAL(size.cx), size.cy, xView, yView + 1);
                BYTE bView = *(data.get() + pViewOffset);
                if (bView != bTransparent)
                {
                    BOOL fShowing = ((*(pdataPriority + pPicOffset)) <= bEgoPriority);
                    if (fShowing)
                    {
                        // Copy a pixel!
                        *(pdataDisplay + pPicOffset) = bView;
                        xLastPixel = -1;
                    }
                    else
                    {
                        if (fShowOutline)
                        {
                            xLastPixel = xPic;
                            if (xFirstPixel == -1)
                            {
                                xFirstPixel = xPic;
                            }
                        }
                    }
                }
            }
            if (fShowOutline)
            {
                if ((xFirstPixel >= 0) && (xFirstPixel < sPIC_WIDTH))
                {
                    int pPicOffset = BUFFEROFFSET(xFirstPixel, yPic);
                    *(pdataDisplay + pPicOffset) = bEgoPriority;
                }
                if ((xLastPixel >= 0) && (xLastPixel < sPIC_WIDTH))
                {
                    int pPicOffset = BUFFEROFFSET(xLastPixel, yPic);
                    *(pdataDisplay + pPicOffset) = bEgoPriority;
                }

            }
        }
    }
}


void DrawBoxWithPriority(BYTE *pdataDisplay, const BYTE *pdataPriority, BYTE bEgoPriority, WORD xIn, WORD yIn, WORD cx, WORD cy)
{
    WORD cxHalf = cx / 2;
    WORD xLeft;
    if (xIn < cxHalf)
    {
        xLeft = 0;
    }
    else
    {
        xLeft = xIn - cxHalf;
    }
    WORD xRight = xIn + cxHalf;
    if (xRight >= 319)
    {
        xRight = 319;
    }

    WORD yBottom = yIn;
    if (yBottom > 189)
    {
        yBottom = 189;
    }
    WORD ySafeTop;
    if (yIn < cy)
    {
        ySafeTop = 0;
    }
    else
    {
        ySafeTop = yIn - cy;
    }

    for (WORD x = xLeft; x <= xRight; x++)
    {
        for (WORD y = ySafeTop; y <= yBottom; y++)
        {
            int p = BUFFEROFFSET(x, y);
            if ( (*(pdataPriority + p)) <= bEgoPriority)
            {
                // Then draw it.  Avoid using _PlotPix, since we need an aux map for that.
                if ( ((x - xLeft) < 2) || ((xRight - x) < 2) || ((y - ySafeTop) < 2) || ((yBottom - y) < 2))
                {
                    pdataDisplay[p] = 0x0f; // White
                }
                else
                {
                    pdataDisplay[p] = bEgoPriority; // the priority colour
                }

            }
        }
    }
}






#define sPIC_SIZE       60800U
#define MAX_BUFF ((__int32)sPIC_SIZE)
#define EMPTY ((__int32)-1)

// Big global buffer.
// We need to take care to serialize access to this.
// It shouldn't be a problem if all drawing is done sequentially, and on the foreground thread.
sPOINT g_buf[MAX_BUFF+1];
sPOINT g_pEmpty={EMPTY,EMPTY};

#define GET_PIX_VISUAL(x, y) (*((pdata->pdataVisual) + BUFFEROFFSET(x, y)))
#define GET_PIX_AUX(x, y) (*((pdata->pdataAux) +  BUFFEROFFSET(x, y)))

//
// BUG in FILL_BOUNDS and DitherFill:
// when you fill, normally white is empty, even if the aux screen is marked there.
// when you fill with a half white color, you need to REALLY mark the aux screen, such that even a white
// spot it considered filled.
//
// Questionable whether we need to fix this or not, it only affects really large areas.
//

#define FILL_BOUNDS(fx, fy) ((dwDrawEnable & GET_PIX_AUX((fx), (fy))) && \
                  !((dwDrawEnable & DRAW_ENABLE_VISUAL) && (GET_PIX_VISUAL((fx), (fy))==15)))


inline BOOL qstore(__int16 x,__int16 y, __int32 *prpos)
{
   if ((*prpos) == MAX_BUFF)
   {
       return FALSE;
   }
   g_buf[*prpos].x = x;
   g_buf[*prpos].y = y;
   (*prpos)++;
   return TRUE;
}


inline sPOINT qretrieve(__int32 *prpos)
{
   if (!*prpos)
   {
       return g_pEmpty;
   }

   (*prpos)--;
   return g_buf[*prpos];
}


#define OK_TO_FILL(x,y) ( CHECK_RECT((x),(y)) && !FILL_BOUNDS((x),(y)) )

int g_commands = 0;


#define MOVING_RIGHT 0x00000001
#define MOVING_LEFT  0x00000002


CRITICAL_SECTION g_csDither = { 0 };
BOOL g_fDitherCritSecInited = FALSE;

void InitDitherCritSec()
{
    ASSERT(!g_fDitherCritSecInited);
    InitializeCriticalSection(&g_csDither);
    g_fDitherCritSecInited = TRUE;
}

void DeleteDitherCritSec()
{
    if (g_fDitherCritSecInited)
    {
        DeleteCriticalSection(&g_csDither);
        g_fDitherCritSecInited = FALSE;
    }
}

void _DitherFill(PicData *pdata, WORD x, WORD y, EGACOLOR color, BYTE bPriorityValue, BYTE bControlValue, DWORD dwDrawEnable)
{
    if (!g_fDitherCritSecInited)
    {
        return;
    }

    // Guard against someone doing a fill with pure white, since this algorithm will hang in that case.
    // Hero's quest does this, when some pictures are drawn with some palettes.
    if ((color.color1 == 0xf) && (color.color2 == 0xf))
    {
        return;
    }

    // Guard access to the buffers we use for dithering.
    CGuard guard(&g_csDither);

    g_commands =0;

    sPOINT p;
    __int32 rpos, spos;

    if (!CHECK_RECT(x,y))
    {
        return;
    }

    if(FILL_BOUNDS(x,y))
    {
        return;
    }

    __int16 x1, y1;

    rpos = spos = 0;

    if(!qstore(x,y, &rpos))
    {
        return;
    }

    for (;;)
    {
        p = qretrieve(&rpos);
        x1 = p.x;
        y1 = p.y;

        if ((x1 == EMPTY) || (y1 == EMPTY))
        {
            return;
        }
        else
        {
            if (OK_TO_FILL(x1,y1))
            {
                // PERF: maybe inline this call?
                _PlotPix(pdata, (WORD)x1, (WORD)y1, dwDrawEnable, color, bPriorityValue, bControlValue);

                // PERF: Tried removing OK_TO_FILL here, but it made it worse
                // (It's technically uncessary)
                if ((y1 != 0) && OK_TO_FILL(x1, y1-1))
                {
                    if(!qstore(x1, y1-1, &rpos)) break;
                }
                if ((x1 != 0) && OK_TO_FILL(x1-1, y1))
                {  
                    if(!qstore(x1-1, y1, &rpos)) break;
                }
                if ((x1 != sPIC_MAXX) && OK_TO_FILL(x1+1, y1))
                { 
                    if(!qstore(x1+1, y1, &rpos)) break;;
                }
                if ((y1 != sPIC_MAXY) && OK_TO_FILL(x1, y1+1))
                {
                    if(!qstore(x1, y1+1, &rpos)) break;;
                }

                g_commands += 4;
            }

        }

    }
}










//
// DRAW LINE
//
void PicCommand::_CreateLine(WORD xFrom, WORD yFrom, WORD xTo, WORD yTo)
{
    ASSERT(_IsEmpty());
    type = Line;
    drawLine.xFrom = xFrom;
    drawLine.yFrom = yFrom;
    drawLine.xTo = xTo;
    drawLine.yTo = yTo;
}

void LineCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    LineCommand_DrawOnly(pCommand, pData, pState);
    pState->tracker.AddCoord(pCommand->drawLine.xFrom, pCommand->drawLine.yFrom, pCommand->drawLine.xTo, pCommand->drawLine.yTo);
}

//
// This doesn't update the picstate.
//
void LineCommand_DrawOnly(const PicCommand *pCommand, PicData *pData, const SCIPicState *pState)
{
    _DitherLine(pData,
                pCommand->drawLine.xFrom, pCommand->drawLine.yFrom, pCommand->drawLine.xTo, pCommand->drawLine.yTo,
                pState->color, pState->bPriorityValue, pState->bControlValue, pState->dwDrawEnable);
}

//
// Override for drawing lines on the buffers, w/o the line actually being part of the pic.
//
void LineCommand_DrawOverride(PicCommand *pCommand, PicData *pData, EGACOLOR color, BYTE bPriorityValue, BYTE bControlValue, DWORD dwDrawEnable)
{
    _DitherLine(pData,
                pCommand->drawLine.xFrom, pCommand->drawLine.yFrom, pCommand->drawLine.xTo, pCommand->drawLine.yTo,
                color, bPriorityValue, bControlValue, dwDrawEnable);
}


void LineCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Line: %dx%d - %dx%d", pCommand->drawLine.xFrom, pCommand->drawLine.yFrom, pCommand->drawLine.xTo, pCommand->drawLine.yTo);
}

void LineCommand_TODO(PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    *pszBuf = 0;
}

BOOL LineCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    // This is complex.
    // To draw a line.
    // Get the current DRAWSIZE for this line.  Check if the this one is equal to or smaller
    // than the previous.  If so, add to it.  Otherwise, start a new command.
    DRAWSIZE ds = DrawSizeFromCoords(pCommand->drawLine.xFrom, pCommand->drawLine.yFrom,
                                     pCommand->drawLine.xTo, pCommand->drawLine.yTo);

    // We need to start a new command if the previous one was a different kind of command (or none)
    BOOL bStartNew = (!pCommandPrev) || (pCommand->type != pCommandPrev->type);

    // Or if the line ended on a different than this one begins.
    if (!bStartNew)
    {
        bStartNew = ((pCommand->drawLine.xFrom != pCommandPrev->drawLine.xTo) ||
                    (pCommand->drawLine.yFrom != pCommandPrev->drawLine.yTo));
    }

    if (!bStartNew)
    {
        // It was the same kind of command.  Maybe we can keep adding to it.  We can do so
        // if it our DRAWSIZE is less than or equal to the previous DRAWSIZE.
        // But how can we be sure of the previous DRAWSIZE?  It might expost itself as smaller than
        // it really was, since the thing before it could have been bigger.  So it needs to be
        // passed in as a param.
        bStartNew = (ds > dsPrev);
    }

    BOOL bRet;
    if (bStartNew)
    {
        // Start a new command.
        pSerial->WriteByte(OpcodeFromCommandTypeAndSize(PicCommand::Line, ds));
        
        _WriteAbsCoordinate(pSerial, pCommand->drawLine.xFrom, pCommand->drawLine.yFrom);
        bRet = _WriteCoordinate(pSerial, ds, pCommand->drawLine.xFrom, pCommand->drawLine.yFrom, pCommand->drawLine.xTo, pCommand->drawLine.yTo);
        *pds = ds; // Pass out the DRAWSIZE we used.
    }
    else
    {
        // Continue from the old command.
        bRet = _WriteCoordinate(pSerial, dsPrev, pCommand->drawLine.xFrom, pCommand->drawLine.yFrom, pCommand->drawLine.xTo, pCommand->drawLine.yTo);
        *pds = dsPrev;
    }

    return TRUE;
}







//
// PATTERN
//
void PicCommand::_CreatePattern(WORD x, WORD y, BYTE bPatternSize, BYTE bPatternNR, BOOL fPattern, BOOL fRectangle)
{
    ASSERT(_IsEmpty());
    ASSERT(bPatternNR < 128);

    type = Pattern;
    drawPattern.x = x;
    drawPattern.y = y;
    drawPattern.bPatternSize = bPatternSize;
    drawPattern.bPatternNR = bPatternNR;

    // Even though the SCI pic resource specifies that PIC_OP_SET_PATTERN is a separate command, we integrate
    // it into each draw-pattern command.  The avoids the user falling into the trap of drawing a pattern
    // with a particular fSolid state, and then going back earlier in the pic and changing it (if that is
    // allowed, this confuses the rendering engine (it may crash, hang, or draw incorrectly) - so this editor will not allow that).
    drawPattern.wFlags = 0;
    if (fRectangle)
    {
        drawPattern.wFlags |= PATTERN_FLAG_RECTANGLE;
    }

    if (fPattern)
    {
        drawPattern.wFlags |= PATTERN_FLAG_USE_PATTERN;        
    }
}

// This version doesn't update the pState.  It is used for previewing the pen tool.
void PatternCommand_Draw_DrawOnly(const PicCommand *pCommand, PicData *pData, const SCIPicState *pState)
{
    _DrawPattern(pData,
                 pCommand->drawPattern.x, pCommand->drawPattern.y,
                 pState->color, pState->bPriorityValue, pState->bControlValue, pState->dwDrawEnable,
                 pCommand->drawPattern.wFlags & PATTERN_FLAG_USE_PATTERN,
                 pCommand->drawPattern.bPatternSize,
                 pCommand->drawPattern.bPatternNR,
                 pCommand->drawPattern.wFlags & PATTERN_FLAG_RECTANGLE);
}

// This version *only* updates state.
void PatternCommand_Draw_StateOnly(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    // Update some state variables (even though they are part of the command, the view may want to know)
    pState->bPatternSize = pCommand->drawPattern.bPatternSize;
    //pState->bPatternCode = pCommand->drawPattern.wFlags;
    pState->bPatternNR = pState->bPatternNR;
}

void PatternCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    PatternCommand_Draw_DrawOnly(pCommand, pData, pState);
    PatternCommand_Draw_StateOnly(pCommand, pData, pState);
}

void PatternCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Pen: %dx%d %s-%s(%d)", pCommand->drawPattern.x, pCommand->drawPattern.y,
                    pCommand->drawPattern.wFlags & PATTERN_FLAG_USE_PATTERN ? TEXT("Pat") : TEXT("Sld"),
                    pCommand->drawPattern.wFlags & PATTERN_FLAG_RECTANGLE ? TEXT("Rect") : TEXT("Circ"),    
                    pCommand->drawPattern.bPatternSize);
}

BOOL PatternCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    // We need to start a new command if the previous one was a different kind of command;
    BOOL bStartNew = (!pCommandPrev) || (pCommand->type != pCommandPrev->type);

    if (!bStartNew)
    {
        // Also, we need to make sure that the previous pattern size and stuff, is equal to ours
        if ((pCommandPrev->drawPattern.wFlags != pCommand->drawPattern.wFlags) ||
            (pCommandPrev->drawPattern.bPatternSize != pCommand->drawPattern.bPatternSize))
        {
            bStartNew = TRUE;
        }
    }

    if (!bStartNew)
    {
        // We can be able to continue from the last pattern command, if the previous DRAWSIZE is more
        // than the current (or undefined)
        DRAWSIZE ds = DrawSizeFromCoords(pCommandPrev->drawPattern.x, pCommandPrev->drawPattern.y,
                                         pCommand->drawPattern.x, pCommand->drawPattern.y);
        bStartNew = (ds > dsPrev);
    }

    BOOL bRet;
    if (bStartNew)
    {
        // Start a new command.
        // We need to figure out what the next command is though, before doing this.
        // Because we have no idea of the DRAWSIZE to use.
        DRAWSIZE dsThis;
        if (!pCommandNext || (pCommand->type != pCommandNext->type))
        {
            dsThis = DS_LARGE; // Well, we're just one point.
        }
        else
        {
            dsThis = DrawSizeFromCoords(pCommand->drawPattern.x, pCommand->drawPattern.y,
                                        pCommandNext->drawPattern.x, pCommandNext->drawPattern.y);
        }

        // Start the command.  First we'll set the pattern.
        // We can optimize this by realizing what the current pattern size is
        if (!pState->fInited ||
            (pState->wFlags != pCommand->drawPattern.wFlags) ||
            (pState->bPatternSize != pCommand->drawPattern.bPatternSize))
        {

            pState->fInited = TRUE;
            pState->wFlags = pCommand->drawPattern.wFlags;
            pState->bPatternSize = pCommand->drawPattern.bPatternSize;
            pSerial->WriteByte(PIC_OP_SET_PATTERN);
            pSerial->WriteByte((pCommand->drawPattern.wFlags) | (pCommand->drawPattern.bPatternSize));
        }

        // Write the opcode
        pSerial->WriteByte(OpcodeFromCommandTypeAndSize(PicCommand::Pattern, dsThis));

        // If we set the use pattern flag, we also need to write a byte here.
        if (pCommand->drawPattern.wFlags & PATTERN_FLAG_USE_PATTERN)
        {
            // Write the NR byte
            pSerial->WriteByte(pCommand->drawPattern.bPatternNR);
        }

        *pds = dsThis;
        // And we always start with an absolute coord, despite our DRAWSIZE
        bRet = _WriteAbsCoordinate(pSerial, pCommand->drawPattern.x, pCommand->drawPattern.y);
    }
    else
    {
        // Continue from the last one.

        if (pCommand->drawPattern.wFlags & PATTERN_FLAG_USE_PATTERN)
        {
            // Write the NR byte
            pSerial->WriteByte(pCommand->drawPattern.bPatternNR);
        }

        bRet = _WriteCoordinate(pSerial, dsPrev, pCommandPrev->drawPattern.x, pCommandPrev->drawPattern.y,
                                pCommand->drawPattern.x, pCommand->drawPattern.y);
    }
    return bRet;
}




//
// FILL
//
void PicCommand::_CreateFill(WORD x, WORD y)
{
    ASSERT(_IsEmpty());
    type = Fill;
    fill.x = x;
    fill.y = y;
}

void FillCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    _DitherFill(pData, pCommand->fill.x, pCommand->fill.y, pState->color, pState->bPriorityValue, pState->bControlValue, pState->dwDrawEnable);
}

void FillCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Fill: %dx%d", pCommand->fill.x, pCommand->fill.y);    
}

BOOL FillCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    if (!pCommandPrev || (pCommand->type != pCommandPrev->type))
    {
        // The last command was of a different type, so write the opcode.
        pSerial->WriteByte(PIC_OP_FILL);
    }

    // Then the write the absolute coordinate.
    return _WriteAbsCoordinate(pSerial, pCommand->fill.x, pCommand->fill.y);
}


//
// VISUAL SET
//
void PicCommand::_CreateSetVisual(BYTE bPaletteNumber, BYTE bPaletteIndex)
{
    ASSERT(_IsEmpty());
    type = SetVisual;
    setVisual.bPaletteIndex = bPaletteIndex;
    setVisual.bPaletteNumber = bPaletteNumber;
}

void SetVisualCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    // Obey SCI game behaviour of ignoring bPalette if not zero
    BOOL bPaletteToUse = pCommand->setVisual.bPaletteNumber;
    if (bPaletteToUse == 0)
    {
        bPaletteToUse = pState->bPaletteToDraw;
    }
    EGACOLOR *pPalette = GET_PALETTE(pState->pPalettes, bPaletteToUse);

    // Update the SCIPicState
    pState->bPaletteNumber = pCommand->setVisual.bPaletteNumber; // This matters for UI state -> note, it is not necessarily bPaletteToUse.
    // bPaletteToUse represents the palette to use when drawing stuff right now, but the palette number
    // encoded in the command, might be different.
    pState->bPaletteOffset = pCommand->setVisual.bPaletteIndex;  // This matters for UI state
    // Set the color we are actually going to draw, given current picstate:
    if (pState->rgLocked[pState->bPaletteOffset])
    {
        // Palette 0 has this index "locked".  So use palette 0 for drawing for this colour.
        pState->color = pState->pPalettes[pCommand->setVisual.bPaletteIndex];
    }
    else
    {
        // The "normal" case
        pState->color = pPalette[pCommand->setVisual.bPaletteIndex];
    }
    pState->dwDrawEnable |= DRAW_ENABLE_VISUAL;
}

void SetVisualCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Set Visual: %d,%d", pCommand->setVisual.bPaletteNumber, pCommand->setVisual.bPaletteIndex);
}

BOOL SetVisualCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    pSerial->WriteByte(PIC_OP_SET_COLOR);
    return pSerial->WriteByte(BYTE_FROM_PALETTE_AND_OFFSET(pCommand->setVisual.bPaletteNumber, pCommand->setVisual.bPaletteIndex));
}



void PicCommand::_CreateSetPriority(BYTE bPriorityValue)
{
    ASSERT(_IsEmpty());
    type = SetPriority;
    setPriority.bPriorityValue = bPriorityValue;
}

void SetPriorityCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    pState->bPriorityValue = pCommand->setPriority.bPriorityValue;
    pState->dwDrawEnable |= DRAW_ENABLE_PRIORITY;
}

void SetPriorityCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Set Priority: %d", pCommand->setPriority.bPriorityValue);
}

BOOL SetPriorityCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    pSerial->WriteByte(PIC_OP_SET_PRIORITY);
    return pSerial->WriteByte(pCommand->setPriority.bPriorityValue);
}




void PicCommand::_CreateSetControl(BYTE bControlValue)
{
    ASSERT(_IsEmpty());
    type = SetControl;
    setControl.bControlValue = bControlValue;
}

void SetControlCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    pState->bControlValue = pCommand->setControl.bControlValue;
    pState->dwDrawEnable |= DRAW_ENABLE_CONTROL;
}

void SetControlCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Set Control: %d", pCommand->setControl.bControlValue); 
}

BOOL SetControlCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    pSerial->WriteByte(PIC_OP_SET_CONTROL);
    return pSerial->WriteByte(pCommand->setControl.bControlValue);
}




void PicCommand::_CreateDisableVisual()
{
    ASSERT(_IsEmpty());
    type = DisableVisual;
}

void DisableVisualCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    pState->dwDrawEnable &= ~DRAW_ENABLE_VISUAL;
}

void DisableVisualCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Disable Visual");
}

BOOL DisableVisualCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    return pSerial->WriteByte(PIC_OP_DISABLE_VISUAL);
}





void PicCommand::_CreateDisablePriority()
{
    ASSERT(_IsEmpty());
    type = DisablePriority;
}

void DisablePriorityCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    pState->dwDrawEnable &= ~DRAW_ENABLE_PRIORITY;
}


void DisablePriorityCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Disable Priority");
}

BOOL DisablePriorityCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    return pSerial->WriteByte(PIC_OP_DISABLE_PRIORITY);
}




void PicCommand::_CreateDisableControl()
{
    ASSERT(_IsEmpty());
    type = DisableControl;
}

void DisableControlCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    pState->dwDrawEnable &= ~DRAW_ENABLE_CONTROL;
}

void DisableControlCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Disable Control");
}

BOOL DisableControlCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    return pSerial->WriteByte(PIC_OP_DISABLE_CONTROL);
}


//
// PALETTES
//
void PicCommand::_CreateSetPalette(BYTE bPaletteNumber, const EGACOLOR *pPalette)
{
    ASSERT(_IsEmpty());
    type = SetPalette;
    setPalette.bPaletteNumber = bPaletteNumber;
    setPalette.pPalette = new EGACOLOR[PALETTE_SIZE];
    ASSERT(sizeof(BYTE) == sizeof(EGACOLOR));
    if (setPalette.pPalette)
    {
        // If we're experiencing memory stress, then we won't set the palette
        // here, and the pic will be corrupted. Oh well.
        CopyMemory(setPalette.pPalette, pPalette, PALETTE_SIZE);
    }
}

void SetPaletteCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    EGACOLOR *pCurrentPalette = GET_PALETTE(pState->pPalettes, pCommand->setPalette.bPaletteNumber);
    if (pCommand->setPalette.pPalette)
    {
        // Could have failed due to memory stress
        CopyMemory(pCurrentPalette, pCommand->setPalette.pPalette, PALETTE_SIZE);

        if (pCommand->setPalette.bPaletteNumber == pState->bPaletteNumber)
        {
            // It's possible that the current color being used was changed by the palette modification
            // just made.
            pState->color = pCurrentPalette[pState->bPaletteOffset];
        }
    }
}

void SetPaletteCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Set palette: %d", pCommand->setPalette.bPaletteNumber);
}

BOOL SetPaletteCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    // PIC_OP_OPX | PIC_OPX_SET_PALETTE | bPaletteNum | pPalette
    pSerial->WriteByte(PIC_OP_OPX);
    pSerial->WriteByte(PIC_OPX_SET_PALETTE);

    pSerial->WriteByte(pCommand->setPalette.bPaletteNumber);
    return pSerial->WriteBytes((BYTE*)pCommand->setPalette.pPalette, 40);
}


void PicCommand::_CreateSetPaletteEntry(BYTE bPaletteNumber, BYTE bOffset, EGACOLOR color)
{
    type = SetPaletteEntry;
    ASSERT(bPaletteNumber < 4);
    setPaletteEntry.bPaletteNumber = bPaletteNumber;
    ASSERT(bOffset < PALETTE_SIZE);
    setPaletteEntry.bOffset = bOffset;
    setPaletteEntry.color = color;
}

void SetPaletteEntryCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    EGACOLOR *pPalette = GET_PALETTE(pState->pPalettes, pCommand->setPaletteEntry.bPaletteNumber);
    pPalette[pCommand->setPaletteEntry.bOffset] = pCommand->setPaletteEntry.color;

    if (pCommand->setPaletteEntry.bPaletteNumber == pState->bPaletteNumber)
    {
        // It's possible that the current color being used was changed by the palette modification
        // just made.
        // ACTUALLY - SCI DOES NOT DO THIS.
        /*
        if (pCommand->setPaletteEntry.bOffset == pState->bPaletteOffset)
        {
            // Yup.  Update the color variable in SCIPicState.
            pState->color = pPalette[pState->bPaletteOffset];
        }*/
    }

    if (pCommand->setPaletteEntry.bPaletteNumber == 0)
    {
        // Lock this color in palette 0.
        pState->rgLocked[pCommand->setPaletteEntry.bOffset] = 0xff;
    }
}

void SetPaletteEntryCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Set palette entry: %d,%d", pCommand->setPaletteEntry.bPaletteNumber, pCommand->setPaletteEntry.bOffset);
}

BOOL SetPaletteEntryCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    // PIC_OP_OPX | PIC_OPX_SET_PALETTE_ENTRY | [ (bPaletteNum,offset), color] * n
    if (!pCommandPrev || (pCommandPrev->type != pCommand->type))
    {
        pSerial->WriteByte(PIC_OP_OPX);
        pSerial->WriteByte(PIC_OPX_SET_PALETTE_ENTRY);
        // The optimization here is that we don't write the opcodes if the last command was the same
    }
    pSerial->WriteByte(BYTE_FROM_PALETTE_AND_OFFSET(pCommand->setPaletteEntry.bPaletteNumber, pCommand->setPaletteEntry.bOffset));
    return pSerial->WriteByte(BYTE_FROM_EGACOLOR(pCommand->setPaletteEntry.color));
}




void PicCommand::CreateSetVisualSCI1(BYTE bColor)
{
    ASSERT(_IsEmpty());
    type = SetVisualSCI1;
    setVisualSCI1.bPaletteIndex = bColor;
}

void SetVisualCommandSCI1_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    // TODO - this is just temporary, so we can see the effect.
    pState->bPaletteNumber = 0;
    pState->bPaletteOffset = 15;
    pState->color.color1 = 0xf;
    pState->color.color2 = 0xf;
    pState->dwDrawEnable |= DRAW_ENABLE_VISUAL;
}

void SetVisualCommandSCI1_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Set visual: %02x", pCommand->setVisualSCI1.bPaletteIndex);
}

BOOL SetVisualCommandSCI1_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    return FALSE;
}




void PicCommand::CreateSetPriorityBars(const BYTE *pBars)
{
    ASSERT(_IsEmpty());
    type = SetPriorityBars;
    setPriorityBars.pPriorityLines = new BYTE[NumPriorityBars];
    if (setPriorityBars.pPriorityLines)
    {
        CopyMemory(setPriorityBars.pPriorityLines, pBars, NumPriorityBars * sizeof(*setPriorityBars.pPriorityLines));
    }
}

void SetPriorityBarsCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    // TODO
}

void SetPriorityBarsCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Set priority bars");
}

BOOL SetPriorityBarsCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    return FALSE;
}



void PicCommand::CreateSetPaletteSCI1(const BYTE *pColors)
{
    ASSERT(_IsEmpty());
    type = SetPaletteSCI1;
    setPaletteSCI1.pPalette = new RGBQUAD[SCI1_PALETTE_LENGTH];
    if (setPaletteSCI1.pPalette)
    {
        for (int i = 0; i < 1024; i += 4)
        {
            setPaletteSCI1.pPalette[i / 4].rgbRed = pColors[i + 1];
            setPaletteSCI1.pPalette[i / 4].rgbGreen = pColors[i + 2];
            setPaletteSCI1.pPalette[i / 4].rgbBlue = pColors[i + 3];
        }
    }
}

void SetPaletteSCI1_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    if (pCommand->setPaletteSCI1.pPalette)
    {
        // Copy it into the real palette.
        CopyMemory(pState->rgPaletteSCI1, pCommand->setPaletteSCI1.pPalette, sizeof(pState->rgPaletteSCI1));
    }
}

void SetPaletteSCI1_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Set 256 palette");
}

BOOL SetPaletteSCI1_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    return FALSE;
}



//
// REVIEW: This algorithm doesn't seem to work (or else our decompression is wrong)
//
void PicCommand::CreateDrawVisualBitmap(BYTE bX, BYTE bY, WORD cx, WORD cy, BYTE bTransparent, const BYTE *pData, WORD wLength)
{
    ASSERT(_IsEmpty());
    type = DrawBitmap;
    drawVisualBitmap.bMirrored = FALSE; // TODO
    drawVisualBitmap.bTransparentColor = bTransparent;
    drawVisualBitmap.bX = bX;
    drawVisualBitmap.bY = bY;
    drawVisualBitmap.cx = cx;
    drawVisualBitmap.cy = cy;
    drawVisualBitmap.pData = NULL;

    // Only allocate if the size is within the limits.
    if ((cx <= 320) && (cy <= 190))
    {
        int cbBitmap = CX_ACTUAL(cx) * cy;
        drawVisualBitmap.pData = new BYTE[cbBitmap];
        if (drawVisualBitmap.pData)
        {
            int cxRemainingOnLine = cx;
            BYTE *pStart = drawVisualBitmap.pData;
            BYTE *pCurrent = drawVisualBitmap.pData;
            int cLine = 0;
            BOOL fBail = FALSE;

            // Interpret the bytes and fill our bitmap
            while (!fBail && (wLength != 0))
            {
                BYTE operation = (*pData) & 0xC0; // Bits 6 and 7 determine the operation.

                // The count is either in bits 0-5, or 0-6, depending on if bit 6 is used
                // (bit 6 is used if bit 7 is set)
                int count = (int) ((*pData & 0x80) ? ((*pData) & 0x3F) : ((*pData) & 0x7F));

                pData++; wLength--;
                if (wLength != 0)
                {
                    int iPositionAlongLine = (int)cx - cxRemainingOnLine;
                    switch(operation)
                    {
                    case 0xC0:
                        // Draw count bytes with the background color.
                        // I have no idea what background color is - maybe transparent color?
                        if (pStart + cbBitmap > pCurrent + count)
                        {
                            FillMemory(pCurrent, count, bTransparent);
                        }
                        else
                        {
                            fBail = TRUE; // We are overwriting our bitmap boundary... more data than necessary!
                        }
                        break;
                    case 0x80:
                        if (wLength != 0)
                        {
                            BYTE bColor = (*pData);
                            if (pStart + cbBitmap > pCurrent + count)
                            {
                                FillMemory(pCurrent, count, bColor);
                                // Draw count bytes of bColor
                                pData++; wLength--;
                            }
                            else
                            {
                                fBail = TRUE;
                            }
                        }
                        else
                        {
                            fBail = TRUE;
                        }
                        break;
                    case 0x00:
                    case 0x40:
                        if (wLength >= count)
                        {
                            // Just copy *pData to the real pData.
                            if (pStart + cbBitmap > pCurrent + count)
                            {
                                CopyMemory(pCurrent, pData, count);
                                pData += count; wLength -= count;
                            }
                            else
                            {
                                fBail = TRUE;
                            }
                        }
                        else
                        {
                            fBail = TRUE;
                        }
                        break;
                    default:
                        ASSERT(FALSE); // Not possible to get here.
                        break;
                    }
                    pCurrent += count;
                }
                else
                {
                    // We had a command that didn't end on a boundary.  Bail.
                    break;
                }
            }
            //ASSERT(wLength == 0);
        }
    }

}

void DrawVisualBitmap_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    if (pCommand->drawVisualBitmap.pData)
    {
        // Optimization
        if ((pCommand->drawVisualBitmap.cx == 320) && (pCommand->drawVisualBitmap.cy == 190))
        {
            // Just bit copy everything
            CopyMemory(pData->pdataVisual, pCommand->drawVisualBitmap.pData, CX_ACTUAL(320) * 190);
        }
        else
        {
            // Copy line by line.
        }
    }
}

void DrawVisualBitmap_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Bitmap (%dx%d)", pCommand->drawVisualBitmap.cx, pCommand->drawVisualBitmap.cy);
}

BOOL DrawVisualBitmap_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    return FALSE;
}


//
// Circle command
//
void PicCommand::CreateCircle(WORD xFrom, WORD yFrom, WORD xTo, WORD yTo)
{
    ASSERT(_IsEmpty());
    type = Circle;
    circle.xFrom = xFrom;
    circle.yFrom = yFrom;
    circle.xTo = xTo;
    circle.yTo = yTo;
}

void CircleCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState)
{
    CircleCommand_DrawOnly(pCommand, pData, pState);
}

//
// Helper that constructs a series of line commands to simulate a circle.
//
void _CircleHelper(const PicCommand *pCommand, std::vector<PicCommand> &temp)
{
    float x1 = static_cast<float>(pCommand->circle.xFrom);
    float x2 = static_cast<float>(pCommand->circle.xTo);
    float y1 = static_cast<float>(pCommand->circle.yFrom);
    float y2 = static_cast<float>(pCommand->circle.yTo);
    float PI = 3.1515926538f;
    float t, a, b, tinc, centx, centy;
    a = abs(0.5f * (x2 - x1));
    b = abs(0.5f * (y2 - y1));
    tinc = PI * 2.0f / (a + b);
    temp.reserve(static_cast<size_t>((a + b)) + 1);
    centx = ((x1 + x2) + 0.5f) * 0.5f;
    centy = ((y1 + y2) + 0.5f) * 0.5f;

    float xOrig = centx + a;
    float yOrig = centy;
    float xFrom = xOrig;
    float yFrom = yOrig;
    for(t = 0; t < PI * 2.0f; t += tinc)
    {
        float xTo = centx + a * cos(t);
        float yTo = centy - b * sin(t);
        temp.push_back(PicCommand::CreateLine(static_cast<WORD>(xFrom), static_cast<WORD>(yFrom), static_cast<WORD>(xTo), static_cast<WORD>(yTo)));
        xFrom = xTo;
        yFrom = yTo;
    }
    // Close it off...
    temp.push_back(PicCommand::CreateLine(static_cast<WORD>(xFrom), static_cast<WORD>(yFrom), static_cast<WORD>(xOrig), static_cast<WORD>(yOrig)));
}

void CircleCommand_DrawOnly(const PicCommand *pCommand, PicData *pData, const SCIPicState *pState)
{
    std::vector<PicCommand> temp;
    _CircleHelper(pCommand, temp);
    size_t cTemps = temp.size();
    for (size_t i = 0; i < cTemps; i++)
    {
        PicCommand &command = temp[i];
        LineCommand_DrawOnly(&command, pData, pState);
    }
}

void CircleCommand_GetName(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf)
{
    StringCchPrintf(pszBuf, cchBuf, "Circle: %dx%d - %dx%d", pCommand->circle.xFrom, pCommand->circle.yFrom, pCommand->circle.xTo, pCommand->circle.yTo);
}
BOOL CircleCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState)
{
    std::vector<PicCommand> temp;
    _CircleHelper(pCommand, temp);

    PicCommand commandPrev;
    BOOL bRet = TRUE;
    size_t cTemps = temp.size();
    *pds = dsPrev;
    for (size_t i = 0; bRet && (i < cTemps); i++)
    {
        const PicCommand &command = temp[i];
        bRet = Command_Serialize(pSerial, &command, (i == 0) ? NULL : &commandPrev, (i == (cTemps - 1)) ? pCommandNext : &temp[i + 1], *pds, pds, pState);
        commandPrev = command;
    }
    return bRet;
}






//
// Choose a nice color for commands.
//
COLORREF Command_GetColor(const PicCommand *pCommand)
{
    // By default, things are black.
    COLORREF color;
    switch (pCommand->type)
    {
    case PicCommand::SetPalette:
    case PicCommand::SetPaletteEntry:
        color = RGB(180, 0, 0);
        break;
    case PicCommand::SetVisual:
    case PicCommand::SetPriority:
    case PicCommand::SetControl:
        color = RGB(0, 0, 180);
        break;
    case PicCommand::DisableVisual:
    case PicCommand::DisablePriority:
    case PicCommand::DisableControl:
        color = RGB(128, 128, 128);
        break;

    case PicCommand::Fill:
        color = RGB(180, 0, 180);
        break;

    case PicCommand::Line:
        color = RGB(0, 180, 0);
        break;

    default:
        color = RGB(0, 0, 0);
        break;
    }
    return color;
}


