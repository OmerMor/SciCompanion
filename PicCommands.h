//
// Pic Command classes
//
#pragma once

#include "PicCommandsCommon.h"

// fwd decl
class ViewResource;

static const DWORD DRAW_ENABLE_VISUAL = 1;
static const DWORD DRAW_ENABLE_PRIORITY = 2;
static const DWORD DRAW_ENABLE_CONTROL = 4;
static const DWORD DRAW_ENABLE_ALL = 7;

#define sPIC_MAXX       319
#define sPIC_MAXY       189
#define BUFFEROFFSET(x, y) (sPIC_MAXY-(y))*sPIC_WIDTH+(x)
#define BUFFEROFFSET_NONSTD(cx, cy, x, y) (cy-(y))*cx+(x)

// Pic opcodes
#define PIC_OP_SET_COLOR                    0xf0
#define PIC_OP_DISABLE_VISUAL               0xf1
#define PIC_OP_SET_PRIORITY                 0xf2
#define PIC_OP_DISABLE_PRIORITY             0xf3
#define PIC_OP_RELATIVE_PATTERNS            0xf4
#define PIC_OP_RELATIVE_MEDIUM_LINES        0xf5
#define PIC_OP_RELATIVE_LONG_LINES          0xf6
#define PIC_OP_RELATIVE_SHORT_LINES         0xf7
#define PIC_OP_FILL                         0xf8
#define PIC_OP_SET_PATTERN                  0xf9
#define PIC_OP_ABSOLUTE_PATTERNS            0xfa
#define PIC_OP_SET_CONTROL                  0xfb
#define PIC_OP_DISABLE_CONTROL              0xfc
#define PIC_OP_RELATIVE_MEDIUM_PATTERNS     0xfd
#define PIC_OP_OPX                          0xfe
#define PIC_END                             0xff

#define PIC_OPX_SET_PALETTE_ENTRY           0x00
#define PIC_OPX_SET_PALETTE                 0x01

#define PIC_OPXSC1_DRAW_BITMAP              0x01
#define PIC_OPXSC1_SET_PALETTE              0x02
#define PIC_OPXSC1_SET_PRIORITY_BARS        0x04

static const int NumPriorityBars = 14;

typedef void (CALLBACK *PFNCLOSECOORDCALLBACK)(void *, CDC *, int, int, int, int);

//
// Keeps track of coordinates used in line commands
//
class CCoordinateTracker
{
public:
    CCoordinateTracker();
    CCoordinateTracker(const CCoordinateTracker& src)
    {
        _x = src._x;
        _y = src._y;
    }
    void AddCoord(WORD xFrom, WORD yFrom, WORD xTo, WORD yTo);
    void Reset();
    void DrawCoords(CPoint pt, CDC *pdc, PFNCLOSECOORDCALLBACK pfnCallback, void *pv) const;
    CCoordinateTracker& operator=( const CCoordinateTracker& src )
    {
        if (this != &src)
        {
            _x = src._x;
            _y = src._y;
        }
        return (*this);
    }

private:
    std::vector<WORD> _x;
    std::vector<WORD> _y;
};

//
// This class maintains the state of a picture, that PicCommands
// might want to change (e.g. current color, priority, etc...)
//
class SCIPicState
{
public:
    SCIPicState(BYTE bPaletteToUse);
    SCIPicState(const SCIPicState& src)
    {
        _Transfer(src);
    }
    SCIPicState();
    SCIPicState& operator=(const SCIPicState& src)
    {
        if (this != &src)
        {
            _Transfer(src);
        }
        return (*this);
    }
    void Reset(BYTE bPaletteToUse);

    DWORD dwDrawEnable;
    BYTE bPriorityValue;
    BYTE bControlValue;
    EGACOLOR color;

    // The following are just used for UI state.
    BYTE bPaletteNumber;// new: which palette the color belongs to
    BYTE bPaletteOffset;// new: which index in that palette that the color belongs to

    BYTE bPatternNR;
    BYTE bPatternSize;
    EGACOLOR pPalettes[160]; // 40 times 4 bytes

    BYTE bPaletteToDraw; // Global palette with which to draw
    BYTE rgLocked[40];   // Whether or not the colours in palette 0 are locked.

    CCoordinateTracker tracker;

    // SCI1-specific: (we re-use color)
    RGBQUAD rgPaletteSCI1[256];
    BYTE bPriorityLines[NumPriorityBars];

private:
    void _Transfer(const SCIPicState& src)
    {
        dwDrawEnable = src.dwDrawEnable;        
        bPriorityValue = src.bPriorityValue;        
        bControlValue = src.bControlValue;        
        color = src.color;        
        bPaletteNumber = src.bPaletteNumber;        
        bPaletteOffset = src.bPaletteOffset;        
        bPatternNR = src.bPatternNR;        
        bPatternSize = src.bPatternSize;        
        CopyMemory(pPalettes, src.pPalettes, sizeof(pPalettes));
        bPaletteToDraw = src.bPaletteToDraw;        
        CopyMemory(rgLocked, src.rgLocked, sizeof(rgLocked));
        tracker = src.tracker;
        CopyMemory(rgPaletteSCI1, src.rgPaletteSCI1, sizeof(rgPaletteSCI1));
        CopyMemory(bPriorityLines, src.bPriorityLines, sizeof(bPriorityLines));
    }
};



struct PicData
{
    DWORD dwMapsToRedraw;
    BYTE *pdataVisual;
    BYTE *pdataPriority;
    BYTE *pdataControl;
    BYTE *pdataAux;
};

//
// This is specifically for serialization
//
struct SerializedPicState
{
    BOOL fInited;
    BYTE bPatternSize;
    BYTE wFlags;
};

struct sRECT
{
    __int16 left;
    __int16 top;
    __int16 right;
    __int16 bottom;
};

BOOL PtInSRect(sRECT *prc, WORD x, WORD y);

//
// COMMAND structures
//
struct LINECOMMAND
{
    WORD xFrom;
    WORD yFrom;
    WORD xTo;
    WORD yTo;
};

struct CIRCLECOMMAND
{
    WORD xFrom;
    WORD yFrom;
    WORD xTo;
    WORD yTo;
};

struct PATTERNCOMMAND
{
    WORD x;
    WORD y;
    BYTE bPatternSize;
    BYTE bPatternNR;
    BYTE wFlags; // 
};

struct FILLCOMMAND
{
    WORD x;
    WORD y;
};

struct SETVISUALCOMMAND
{
    BYTE bPaletteNumber;
    BYTE bPaletteIndex;
};

struct SETPRIORITYCOMMAND
{
    BYTE bPriorityValue;
};

struct SETCONTROLCOMMAND
{
    BYTE bControlValue;
};

struct SETPALETTECOMMAND
{
    // So as not to bloat all commands, we allocate the palette here,
    // instead of having an embedded array.
    EGACOLOR *pPalette;
    BYTE bPaletteNumber;
};

struct SETPALETTEENTRYCOMMAND
{
    EGACOLOR color;
    BYTE bPaletteNumber;
    BYTE bOffset;
};

struct SETVISUALCOMMAND_SCI1
{
    BYTE bPaletteIndex;
};

struct SETPRIORITYBARSCOMMAND_SCI1
{
    BYTE *pPriorityLines;
};

struct DRAWVISUALBITMAPCOMMAND_SCI1
{
    BYTE bX; // Why is this only 1 byte?
    BYTE bY;
    WORD cx;
    WORD cy;
    BYTE bTransparentColor;
    BOOL bMirrored;
    BYTE *pData; // Bitmap data (length = cy * CX_ACTUAL(cx))
};


#define SCI1_PALETTE_LENGTH 256

struct SETPALETTECOMMAND_SCI1
{
    RGBQUAD *pPalette; // 256
};

//
// PicCommand
// This class represents the commands that make up an SCI picture.
//
// Due to the need for efficiency in manipulating thousands of commands in a single
// picture, these objects generally don't get allocated.  e.g. we have arrays of PicCommand, not
// PicCommand*, as the memory allocations hurt performance.
//
// So the PicCommand class contains a union of all the possible data required for a
// command.  However, the setPalette command is unique in that it requires much more
// data (a whole palette).  So we allocate a palette in this object (palette commands
// are infrequent, so perf is not an issue).  Since we have an allocated object here, we
// need a destructor, copy constructor, and an = operator overload.
//
class PicCommand
{
public:

    //
    // command types
    // 
    // These serve as indexes into function pointer arrays
    enum CommandType
    {
        Line =           0x00000000,
        Pattern =        0x00000001,
        Fill =           0x00000002,
        SetVisual =      0x00000003,
        SetPriority =    0x00000004,
        SetControl =     0x00000005,
        DisableVisual =  0x00000006,
        DisablePriority =0x00000007,
        DisableControl = 0x00000008,
        SetPalette =     0x00000009,
        SetPaletteEntry =0x0000000a,
        SetVisualSCI1 =  0x0000000b,
        SetPriorityBars =0x0000000c,
        DrawBitmap =     0x0000000d,
        SetPaletteSCI1 = 0x0000000e,
        Circle =         0x0000000f,
        CommandTypeMax = 0x0000000f,
        None =           0xffffffff, // Just for convenience
    };

    PicCommand()
    {
        ZeroMemory(this, sizeof(PicCommand));
        type = None;
    }
    // Copy constructor
    PicCommand(const PicCommand& src)
    {
        CopyMemory(this, &src, sizeof(PicCommand));
        if (src.type == SetPalette)
        {
            // Reallocate the palette
            setPalette.pPalette = new EGACOLOR[PALETTE_SIZE];
            if (setPalette.pPalette)
            {
                CopyMemory(setPalette.pPalette, src.setPalette.pPalette, sizeof(EGACOLOR) * PALETTE_SIZE);
            }
        }
    }
    // For deserialization from clipboard:
    BOOL Initialize(sci::istream &byteStream)
    {
        _CleanUp();
        ZeroMemory(this, sizeof(PicCommand));
        byteStream >> *this;
        if (byteStream.good())
        {
            ASSERT(type <= CommandTypeMax); // That we have a valid command!
            if (type == SetPalette)
            {
                // Allocate the palette
                setPalette.pPalette = new EGACOLOR[PALETTE_SIZE];
                if (setPalette.pPalette)
                {
                    byteStream.read_data((BYTE*)setPalette.pPalette, sizeof(EGACOLOR) * PALETTE_SIZE);
                }
                else
                {
                    byteStream.skip(sizeof(EGACOLOR) * PALETTE_SIZE);
                }
            }            
        }
        return byteStream.good();
    }
    // Serialization for clipboard
    BOOL Serialize(sci::istream *pSerial) const
    {
        BOOL fRet = pSerial->WriteBytes((BYTE*)this, sizeof(PicCommand));
        if (fRet)
        {
            if (type == SetPalette)
            {
                fRet = FALSE;
                if (setPalette.pPalette)
                {
                    fRet = pSerial->WriteBytes((BYTE*)setPalette.pPalette, sizeof(EGACOLOR) * PALETTE_SIZE);
                }
            }
        }
        return fRet;
    }
    ~PicCommand()
    {
        _CleanUp();
    }

    PicCommand& operator=( const PicCommand& src )
	{
        if (this != &src)
        {
            _CleanUp(); // Delete any existing data.
            CopyMemory(this, &src, sizeof(PicCommand));
            if (src.type == SetPalette)
            {
                // Reallocate the palette
                setPalette.pPalette = new EGACOLOR[PALETTE_SIZE];
                if (setPalette.pPalette)
                {
                    CopyMemory(setPalette.pPalette, src.setPalette.pPalette, sizeof(EGACOLOR) * PALETTE_SIZE);
                }
            }
        }
		return(*this);
	}

    CommandType type;
    union 
    {
        LINECOMMAND drawLine;
        CIRCLECOMMAND circle;
        PATTERNCOMMAND drawPattern;
        FILLCOMMAND fill;
        SETVISUALCOMMAND setVisual;
        SETPRIORITYCOMMAND setPriority;
        SETCONTROLCOMMAND setControl;
        SETPALETTECOMMAND setPalette;
        SETPALETTEENTRYCOMMAND setPaletteEntry;
        SETVISUALCOMMAND_SCI1 setVisualSCI1;
        SETPRIORITYBARSCOMMAND_SCI1 setPriorityBars;
        DRAWVISUALBITMAPCOMMAND_SCI1 drawVisualBitmap;
        SETPALETTECOMMAND_SCI1 setPaletteSCI1;
    };

    // Create functions
    static PicCommand CreateLine(WORD xFrom, WORD yFrom, WORD xTo, WORD yTo)
    {
        PicCommand command;
        command._CreateLine(xFrom, yFrom, xTo, yTo);
        return command;
    }
    static PicCommand CreatePattern(WORD x, WORD y, BYTE bPatternSize, BYTE bPatternNR, BOOL fPattern, BOOL fRectangle)
    {
        PicCommand command;
        command._CreatePattern(x, y, bPatternSize, bPatternNR, fPattern, fRectangle);
        return command;
    }
    static PicCommand CreateFill(WORD x, WORD y)
    {
        PicCommand command;
        command._CreateFill(x, y);
        return command;
    }
    static PicCommand CreateSetVisual(BYTE bPaletteNumber, BYTE bPaletteIndex)
    {
        PicCommand command;
        command._CreateSetVisual(bPaletteNumber, bPaletteIndex);
        return command;
    }
    static PicCommand CreateSetPriority(BYTE bPriorityValue)
    {
        PicCommand command;
        command._CreateSetPriority(bPriorityValue);
        return command;
    }
    static PicCommand CreateSetControl(BYTE bControlValue)
    {
        PicCommand command;
        command._CreateSetControl(bControlValue);
        return command;
    }
    static PicCommand CreateDisableVisual()
    {
        PicCommand command;
        command._CreateDisableVisual();
        return command;
    }
    static PicCommand CreateDisablePriority()
    {
        PicCommand command;
        command._CreateDisablePriority();
        return command;
    }
    static PicCommand CreateDisableControl()
    {
        PicCommand command;
        command._CreateDisableControl();
        return command;
    }
    static PicCommand CreateSetPalette(BYTE bPaletteNumber, const EGACOLOR *pPalette)
    {
        PicCommand command;
        command._CreateSetPalette(bPaletteNumber, pPalette);
        return command;
    }
    static PicCommand CreateSetPaletteEntry(BYTE bPaletteNumber, BYTE bOffset, EGACOLOR color)
    {
        PicCommand command;
        command._CreateSetPaletteEntry(bPaletteNumber, bOffset, color);
        return command;
    }
    void CreateSetVisualSCI1(BYTE bColor);
    void CreateSetPriorityBars(const BYTE *pBars);
    void CreateSetPaletteSCI1(const BYTE *pColors);
    void CreateDrawVisualBitmap(BYTE bX, BYTE bY, WORD cx, WORD cy, BYTE bTransparent, const BYTE *pData, WORD wLength);
    void CreateCircle(WORD xFrom, WORD yFrom, WORD xTo, WORD yTo);

    // Drawing functions, etc...
    void Draw(PicData *pData, SCIPicState &state) const;
    void GetName(TCHAR *pszBuf, size_t cchBuf) const;

private:
    void _CreateLine(WORD xFrom, WORD yFrom, WORD xTo, WORD yTo);
    void _CreatePattern(WORD x, WORD y, BYTE bPatternSize, BYTE bPatternNR, BOOL fPattern, BOOL fRectangle);
    void _CreateFill(WORD x, WORD y);
    void _CreateSetVisual(BYTE bPaletteNumber, BYTE bPaletteIndex);
    void _CreateSetPriority(BYTE bPriorityValue);
    void _CreateSetControl(BYTE bControlValue);
    void _CreateDisableVisual();
    void _CreateDisablePriority();
    void _CreateDisableControl();
    void _CreateSetPalette(BYTE bPaletteNumber, const EGACOLOR *pPalette);
    void _CreateSetPaletteEntry(BYTE bPaletteNumber, BYTE bOffset, EGACOLOR color);
    void _CreateSetVisualSCI1(BYTE bColor);
    void _CreateSetPriorityBars(const BYTE *pBars);
    void _CreateSetPaletteSCI1(const BYTE *pColors);
    void _CreateDrawVisualBitmap(BYTE bX, BYTE bY, WORD cx, WORD cy, BYTE bTransparent, const BYTE *pData, WORD wLength);
    void _CreateCircle(WORD xFrom, WORD yFrom, WORD xTo, WORD yTo);

    BOOL _IsEmpty()
    {
        // Check some things that would indicate this command is empty
        return (type == None) && (setPalette.pPalette == NULL);
    }

    void _CleanUp()
    {
        // Clean up any allocated data
        switch (type)
        {
        case SetPalette:
            delete [] setPalette.pPalette;
            setPalette.pPalette = NULL;
            break;
        case SetPaletteSCI1:
            delete [] setPaletteSCI1.pPalette;
            setPaletteSCI1.pPalette = NULL;
            break;
        case SetPriorityBars:
            delete [] setPriorityBars.pPriorityLines;
            setPriorityBars.pPriorityLines = NULL;
            break;
        case DrawBitmap:
            delete [] drawVisualBitmap.pData;
            drawVisualBitmap.pData = NULL;
            break;
        }
        type = None; // Set command type to none
    }

};


void InitPenStyle(PenStyle *pPenStyle);
BOOL IsPenStyleEqual(const PenStyle *pPenStyle1, const PenStyle *pPenStyle2);
void PatternInfoFromIndex(BYTE bIndex, PenStyle *pPenStyle);
BYTE IndexFromPatternInfo(const PenStyle *pPenStyle);

//
// Specifies the 3 types of x,y encoding for drawing commands
//
enum DRAWSIZE
{
    DS_SMALL     = 0,   // x,y in one byte  (max 8,8)
    DS_MEDIUM    = 1,   // x,y in two bytes (max 128,111)
    DS_LARGE     = 2,   // x,y in three bytes (max 319, 189)
};



//
// Draw functions
//
void LineCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void LineCommand_DrawOnly(const PicCommand *pCommand, PicData *pData, const SCIPicState *pState);
void PatternCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void PatternCommand_Draw_DrawOnly(const PicCommand *pCommand, PicData *pData, const SCIPicState *pState);
void PatternCommand_Draw_StateOnly(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void FillCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void SetVisualCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void SetPriorityCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void SetControlCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void DisableVisualCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void DisablePriorityCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void DisableControlCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void SetPaletteCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void SetPaletteEntryCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void SetVisualCommandSCI1_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void SetPriorityBarsCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void SetPaletteSCI1_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void DrawVisualBitmap_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void CircleCommand_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void CircleCommand_DrawOnly(const PicCommand *pCommand, PicData *pData, const SCIPicState *pState);

//
// Fancy pants stuff
//
COLORREF Command_GetColor(const PicCommand *pCommand);


//
// Serialize functions
//
// pCommandPrev and pCommandNext are optional parameters.
// dsPrev is only valid if there is a pCommandPrev of the same type.
//
BOOL LineCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL PatternCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL FillCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL SetVisualCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL SetPriorityCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL SetControlCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL DisableVisualCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL DisablePriorityCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL DisableControlCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL SetPaletteCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL SetPaletteEntryCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL SetVisualCommandSCI1_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL SetPriorityBarsCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL SetPaletteSCI1_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL DrawVisualBitmap_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);
BOOL CircleCommand_Serialize(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);

BOOL SerializeAllCommands(sci::istream *pSerial, const std::vector<PicCommand> &commands, size_t cCommands);

//
// Special one for post-processing in the image.
//
void LineCommand_DrawOverride(PicCommand *pCommand, PicData *pData, EGACOLOR color, BYTE bPriorityValue, BYTE bControlValue, DWORD dwDrawEnable);
void LineCommand_DrawEx(int cx, int cy, PicCommand *pCommand, BYTE *pData, EGACOLOR color);

// Serialize functions (to be added later)

// General functions
void Command_Draw(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
void Command_DrawF(PicCommand *pCommand, DWORD *pData, SCIPicState *pState);

struct PICCOMMAND_ADJUST
{
    RECT rcBounds; // Original bounds
    RECT rcNew;
    BOOL fVFlip;    // Flip vertical
    BOOL fHFlip;    // Flip horizontal
    int iAngle;
};

void PastedCommands_GetBounds(const PicCommand *pCommand, INT_PTR cCommands, sRECT *prc);
void Command_DrawWithOffset(const PicCommand *pCommandIn, PicData *pData, SCIPicState *pState, const PICCOMMAND_ADJUST *pAdjust);
void PastedCommands_Adjust(std::vector<PicCommand> &commandsIn, const PICCOMMAND_ADJUST *pAdjust);
HGLOBAL CopiedCommands_AllocAndFillMemory(const PicCommand *pCommands, INT_PTR cCommands);
BOOL PastedCommands_ContainDrawCommands(const PicCommand *pCommands, INT_PTR cCommands);

typedef void (*DRAW_FUNCTION)(const PicCommand *pCommand, PicData *pData, SCIPicState *pState);
typedef void (*GETNAME_FUNCTION)(const PicCommand *pCommand, TCHAR *pszBuf, size_t cchBuf);
typedef BOOL (*SERIALIZE_FUNCTION)(sci::istream *pSerial, const PicCommand *pCommand, const PicCommand *pCommandPrev, const PicCommand *pCommandNext, DRAWSIZE dsPrev, DRAWSIZE *pds, SerializedPicState *pState);

const BYTE g_defaultPaletteInit[] = 
{ (0), (0x11), (0x22), (0x33), (0x44), (0x55), (0x66), (0x77),
      (0x88), (0x99), (0xaa), (0xbb), (0xcc), (0xdd), (0xee), (0x88),
      (0x88), (0x01), (0x02), (0x03), (0x04), (0x05), (0x06), (0x88),
      (0x88), (0xf9), (0xfa), (0xfb), (0xfc), (0xfd), (0xfe), (0xff),
      (0x08), (0x91), (0x2a), (0x3b), (0x4c), (0x5d), (0x6e), (0x88) };

extern const EGACOLOR *g_defaultPalette;

void InitDitherCritSec();
void DeleteDitherCritSec();

//
// For the fake ego feature.
//
void DrawBoxWithPriority(BYTE *pdataDisplay, const BYTE *pdataPriority, BYTE bEgoPriority, WORD x, WORD y, WORD cx, WORD cy);
void DrawViewWithPriority(BYTE *pdataDisplay, const BYTE *pdataPriority, BYTE bEgoPriority, WORD xIn, WORD yIn, const ViewResource *pvr, int nLoop, int nCel, BOOL fShowOutline = FALSE); 
BOOL HitTestEgoBox(WORD xCursor, WORD yCursor, WORD xEgo, WORD yEgo, WORD cx, WORD cy);
BOOL HitTestView(WORD xCursor, WORD yCursor, WORD xEgo, WORD yEgo, const ViewResource *pvr, int nLoop, int nCel);
void GetViewRect(CRect *prc, WORD xEgo, WORD yEgo, const ViewResource *pvr, int nLoop, int nCel);
CPoint FindCenterOfView(WORD xEgo, WORD yEgo, const ViewResource *pvr, int nLoop, int nCel);
CRect GetViewBoundsRect(WORD xEgo, WORD yEgo, const ViewResource *pvr, int nLoop, int nCel);
bool CanBeHere(const BYTE *pdataPriority, const CRect &rect, WORD wControlMask = 0x8000); 
void DrawPatternInRect(int cx, int cy, PicData *pData, WORD x, WORD y, EGACOLOR color, BYTE bPriorityValue, BYTE bControlValue, DWORD dwDrawEnable, const PenStyle *pPenStyle);

//
// Each command shall have the following:
// A way to draw a particular screen onto a hdc.
// void DrawCommand(HDC hdc, int iScreen, SCIPicState *pState)
//
// A way to draw itself into a piece of bitmap data
// void DrawCommand(BYTE *pdataVisual, BYTE *pdataPriority, BYTE *pdataControl, BYTE *pdataAux, SCIPicState *pState)
//
// A way to create itself from data.
// static HRESULT FromBuffer(BYTE *pPicData, PicCommand *pPrevious, **ppCommand);
// 
// A method that returns its type
// BYTE GetType()
//
// A way to serialize itself.
// **pNext is the next guy to call serialize on.
// If this method is called, the command knows it needs to place a new opcode
// HRESULT Serialize(BYTE *pPicData, PicCommand *pPrevious, PicCommand **pNext)
//
// if this one is called (signature varies with type of command), then it knows it
// doesn't need to start a new opcode
// HRESULT SerializeWithHint(BYTE *pPicData, WORD xPreviousTo, WORD yPreviousTo, PicCommand **pNext)
//
// Of course, we can't call SerializeWithHint unless we first call this, to ensure the same opcode:
// void UseSameOpcode(WORD xTo, WORD yTo, BYTE *pOpcode)  // opcode is one of 3 things
// 
// If we see that the next command is not of the right type, then just

//


//
// Scenario 1:
// Loading from a databuffer:
// - We look at the opcode, and then instantiate the command base off of it.  We'll do the 
//   reading, and then construct the command based on the results.  i.e.
//   CDrawLinePicCommand(WORD xFrom, WORD yFrom, WORD xTo, WORD yTo);
// - Then, as we find multiple instances of the command, we will keep instantiating those.
//
// Scenario 2:
// Drawing
// We'll keep calling DrawCommand sequentially, for all the pic commands.
//
// Scenario 3:
// Persisting to buffer.
// We start with the first one, and ask it to persist itself.  It knows to persist its
// opcode, if the previous command is not of the same type.
//
//