#include "stdafx.h"
#include "PicResource.h"
#include "PicDoc.h"
#include "SCIPicEditor.h"
#include "DontShowAgainDialog.h"
#include "PicDrawManager.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_picId = 0;

#define ISVALIDPALETTEINDEX(x) (x < 40)
#define MAKE_VALID_PALETTE_INDEX(x) (min((x), 39))
#define PALETTE(x) (pPalettes + x * 40)
#define CURPALETTE (pPalettes + bCurrentPalette * 40)

#define GET_PALDEX(s)               ((s)/PALETTE_SIZE)
#define GET_PALCOL(s)               ((s)%PALETTE_SIZE)

//
// Attempts to determine if this is a SCI0 or SCI1 pic
//
#define PIC_SCI0 0
#define PIC_SCI1 1
int SniffPicType(const BYTE *pdata, size_t cbData)
{
    int iRet = PIC_SCI0;
    if (cbData >= 2)
    {
        if ((pdata[0] == 0xFE) && (pdata[1] == 0x02))
        {
            iRet = PIC_SCI1;
        }
    }
    return iRet;
}


PicResource::PicResource()
{
    _iResourceNumber = -1;
    _iFileNumber = -1;
    _f256Color = false;
    _id = g_picId++;
}

struct AbsoluteCoords
{
    WORD x, y;
};
sci::istream & operator>>(sci::istream &stream, AbsoluteCoords &coords)
{
    BYTE bCoordinatePrefix;
    stream >> bCoordinatePrefix;
    BYTE bx, by;
    stream >> bx;
    stream >> by;
    coords.x = (WORD)bx;
    coords.y = (WORD)by;
    coords.x |= ( ((WORD)(bCoordinatePrefix & 0xf0)) << 4);
    coords.y |= ( ((WORD)(bCoordinatePrefix & 0x0f)) << 8);
    return stream;
}

struct RelativeCoords
{
    RelativeCoords(const AbsoluteCoords &absCoords) : xPrev(absCoords.x), yPrev(absCoords.y), x(0), y(0) {}
    void Next() { xPrev = x; yPrev = y; }
    WORD xPrev, yPrev;
    WORD x, y;
};
sci::istream & operator>>(sci::istream &stream, RelativeCoords &coords)
{
    BYTE bInput;
    stream >> bInput;
    coords.x = coords.xPrev;
    coords.y = coords.yPrev;
    if (bInput & 0x80)
    {
        coords.x -= ((bInput >> 4) & 0x7) ;
    }
    else
    {
        coords.x += ((bInput >> 4) & 0x7);
    }

    if (bInput & 0x08)
    {
        coords.y -= (bInput & 0x7);
    }
    else
    {
        coords.y += (bInput & 0x7);
    }
    return stream;
}


//
// Because we're dealing with unsigned bytes, we need to do some sepcial crap
//
WORD _CalcRelativeX(WORD xOld, BYTE bTemp)
{
    // Convert to unsigned
    __int8 cUnsigned = (char)bTemp;
    // Then sign-extend it.
    __int16 i16Temp = (__int16)cUnsigned;
    return ((__int16)xOld) + i16Temp;
}

BOOL PicResource::Serialize(sci::istream *pSerial) const
{
    return SerializeAllCommands(pSerial, _picCommands, _picCommands.size());
}

BOOL PicResource::PerformChecks() const
{
    // Perform a little validation.
    if (!theApp._fDontCheckPic)
    {
        DWORD fPalettePresent = 0;
        for (size_t i = 0; i < _picCommands.size(); i++)
        {
            const PicCommand &command = _picCommands[i];
            if (command.type == PicCommand::SetPalette)
            {
                fPalettePresent |= 1 << command.setPalette.bPaletteNumber;
            }
        }
        if (fPalettePresent != 0xf)
        {
            // Warn the user
            CDontShowAgainDialog dialog(TEXT("This pic is missing a Set Palette command for one or more of the four palettes.\nThis may lead to unpredictable results when the game draws the pic."),
                                        theApp._fDontCheckPic);
            dialog.DoModal();
        }
    }
    return TRUE; // Always save anyway...
}

//
// Use this for new ones
//
HRESULT PicResource::InitNew()
{
    _iFileNumber = -1;
    _iResourceNumber = -1;
    // Reserve space for about 166 commands.
    _picCommands.reserve(500 / 2);

    // Now Prepopulate the pic with palettes, pen styles/sizes and screen on/offs.
    // Otherwise, the SCI interpreter may use values from the previous picture.

    // First the palettes:
    for (BYTE b = 0; b < 4; b++)
    {
        _picCommands.push_back(PicCommand::CreateSetPalette(b, g_defaultPalette));
    }

    // We actually don't need to set the pen style - it will be set with
    // the first pen command.

    // Finally, turn off the screens.
    _picCommands.push_back(PicCommand::CreateDisableVisual());
    _picCommands.push_back(PicCommand::CreateDisablePriority());
    _picCommands.push_back(PicCommand::CreateDisableControl());
    return S_OK;
}



//
// And this for existing ones.
//
HRESULT PicResource::InitFromResource(const ResourceBlob *prd)
{
    ASSERT(prd->GetType() == RS_PIC);
    // See how many bytes long the stream is:
    int cBytes = prd->GetLength();
    // Because we are going to set the default size of our array to that.
    // A random sampling of SCI apps showed that the number of bytes of resource per pic command
    // was 2.95 to 4.75, with an average of around 4.  Here we'll assume 3 bytes per command, and we'll
    // set the growby size to that.  That might seem excessive, but all we really want to do is allocate
    // that much memory at first, without increasing the "current index" of the array (MFC doesn't allow
    // you to specify both an allocated size, and an array size).  We could, if we want, decrease the growby
    // size after our first allocation.
    _picCommands.reserve(cBytes / 2);

    // Just temporarily grab the bits from the resource.
    _iResourceNumber = prd->HasNumber() ? prd->GetNumber() : -1;
    _iFileNumber = prd->GetPackageHint();
    _strTitle = prd->GetName();
    sci::istream byteStream = prd->GetReadStream();
    //int iType = SniffPicType(???
    int iType = PIC_SCI0;
    _f256Color = (iType == PIC_SCI1);

    PatternInfo patterns;
    bool fDone = false;
    while (!fDone && byteStream.good())
    {
        BYTE bOpcode;
        byteStream >> bOpcode;
        if (iType == PIC_SCI0)
        {
            BYTE bColorCode;
            switch (bOpcode)
            {
            case PIC_OP_SET_COLOR:
                byteStream >> bColorCode;
                MAKE_VALID_PALETTE_INDEX(bColorCode);
                _picCommands.push_back(PicCommand::CreateSetVisual(GET_PALDEX(bColorCode), GET_PALCOL(bColorCode)));
                break;

            case PIC_OP_DISABLE_VISUAL:
                _picCommands.push_back(PicCommand::CreateDisableVisual());
                break;

            case PIC_OP_SET_PRIORITY:
                byteStream >> bColorCode;
                _picCommands.push_back(PicCommand::CreateSetPriority(bColorCode & 0xf));
                break;

            case PIC_OP_DISABLE_PRIORITY:
                _picCommands.push_back(PicCommand::CreateDisablePriority());
                break;

            case PIC_OP_RELATIVE_PATTERNS:
                _ReadRelativePatterns(byteStream, patterns);
                break;

            case PIC_OP_RELATIVE_MEDIUM_LINES:
                _ReadRelativeMediumLines(byteStream);
                break;

            case PIC_OP_RELATIVE_LONG_LINES:
                _ReadRelativeLongLines(byteStream);
                break;

            case PIC_OP_RELATIVE_SHORT_LINES:
                _ReadRelativeShortLines(byteStream);
                break;

            case PIC_OP_FILL:
                _ReadFill(byteStream);
                break;

            case PIC_OP_SET_PATTERN:
                byteStream >> patterns.bPatternCode;
                // There is no command of ours that is associated with this.
                // They are encoded into each CDrawPatternPicCommand.
                // Just make sure we keep track of these values.
                patterns.bPatternCode &= 0x37;
                patterns.bPatternSize = patterns.bPatternCode & 0x7;
                break;

            case PIC_OP_ABSOLUTE_PATTERNS:
                _ReadAbsolutePatterns(byteStream, patterns);
                break;

            case PIC_OP_SET_CONTROL:
                byteStream >> bColorCode;
                _picCommands.push_back(PicCommand::CreateSetControl(bColorCode));
                break;

            case PIC_OP_DISABLE_CONTROL:
                _picCommands.push_back(PicCommand::CreateDisableControl());
                break;

            case PIC_OP_RELATIVE_MEDIUM_PATTERNS:
                _ReadRelativeMediumPatterns(byteStream, patterns);
                break;

            case PIC_OP_OPX:
                {
                    BYTE bTemp;
                    byteStream >> bTemp;
                    if (byteStream.good())
                    {
                        switch (bTemp)
                        {
                        case 0x00: // PIC_OPX_SET_PALETTE_ENTRY
                            // Not available in SCI1
                            _ReadSetPaletteEntry(byteStream);
                            break;
                        case 0x01: // PIC_OPX_SET_PALETTE 
                            // SCI1: draw visual bitmap
                            _ReadSetPalette(byteStream);
                            break;
                        case 0x02: // PIC_OPX_MONO0 
                            byteStream.skip(41);
                            break;
                        case 0x03: // PIC_OPX_MONO1 
                        case 0x04: // PIC_OPX_MONO2 
                        case 0x05: // PIC_OPX_MONO3 
                            byteStream.skip(1);
                            break;
                        case 0x06: // SCI01 operations... not supported.
                        case 0x07:
                        case 0x08:
                            break;
                        }
                    }
                }
                break;

            case PIC_END:
                {
                    fDone = true;
                }
                break;
            }
        }
    }

    return byteStream.good() ? S_OK : E_FAIL;
}

void PicResource::_ReadRelativePatterns(sci::istream &stream, PatternInfo &patterns)
{
    if (patterns.bPatternCode &  PATTERN_FLAG_USE_PATTERN)
    {
        BYTE bTemp;
        stream >> bTemp;
        patterns.bPatternNR = (bTemp >> 1) & 0x7f;
    }

    AbsoluteCoords absCoords;
    stream >> absCoords;
    _picCommands.push_back(PicCommand::CreatePattern(absCoords.x, absCoords.y, patterns.bPatternSize, patterns.bPatternNR, patterns.bPatternCode & PATTERN_FLAG_USE_PATTERN, patterns.bPatternCode & PATTERN_FLAG_RECTANGLE));

    BYTE bTemp;
    while (stream.peek(bTemp) && (bTemp < 0xf0))
    {
        if (patterns.bPatternCode & PATTERN_FLAG_USE_PATTERN)
        {
            BYTE bTemp;
            stream >> bTemp;
            patterns.bPatternNR = (bTemp >> 1) & 0x7f;
        }
        RelativeCoords relCoords(absCoords);
        stream >> relCoords;
        _picCommands.push_back(PicCommand::CreatePattern(relCoords.x, relCoords.y, patterns.bPatternSize, patterns.bPatternNR, patterns.bPatternCode & PATTERN_FLAG_USE_PATTERN, patterns.bPatternCode & PATTERN_FLAG_RECTANGLE));
        relCoords.Next();
    }
}

void PicResource::_ReadAbsolutePatterns(sci::istream &stream, PatternInfo &patterns)
{
    BYTE bTemp;
    while (stream.peek(bTemp) && (bTemp < 0xf0))
    {
        if (patterns.bPatternCode & PATTERN_FLAG_USE_PATTERN)
        {
            stream >> bTemp;
            patterns.bPatternNR = (bTemp >> 1) & 0x7f;
        }
        AbsoluteCoords absCoords;
        stream >> absCoords;
        _picCommands.push_back(PicCommand::CreatePattern(absCoords.x, absCoords.y, patterns.bPatternSize, patterns.bPatternNR, patterns.bPatternCode & PATTERN_FLAG_USE_PATTERN, patterns.bPatternCode & PATTERN_FLAG_RECTANGLE));
    }
}

void PicResource::_ReadRelativeMediumPatterns(sci::istream &stream, PatternInfo &patterns)
{
    BYTE bTemp;
    if (patterns.bPatternCode & PATTERN_FLAG_USE_PATTERN)
    {
        stream >> bTemp;
        patterns.bPatternNR = (bTemp >> 1) & 0x7f;
    }
    AbsoluteCoords absCoords;
    stream >> absCoords;
    _picCommands.push_back(PicCommand::CreatePattern(absCoords.x, absCoords.y, patterns.bPatternSize, patterns.bPatternNR, patterns.bPatternCode & PATTERN_FLAG_USE_PATTERN, patterns.bPatternCode & PATTERN_FLAG_RECTANGLE));

    while (stream.peek(bTemp) && (bTemp < 0xf0))
    {
        if (patterns.bPatternCode & PATTERN_FLAG_USE_PATTERN)
        {
            stream >> bTemp;
            patterns.bPatternNR = (bTemp >> 1) & 0x7f;
        }
        stream >> bTemp;
        WORD x, y;
        if (bTemp & 0x80)
        {
            y = absCoords.y - (bTemp & 0x7f);
        }
        else
        {
            y = absCoords.y + bTemp;
        }
        stream >> bTemp;
        x = _CalcRelativeX(absCoords.x, bTemp);
        _picCommands.push_back(PicCommand::CreatePattern(x, y, patterns.bPatternSize, patterns.bPatternNR, patterns.bPatternCode & PATTERN_FLAG_USE_PATTERN, patterns.bPatternCode & PATTERN_FLAG_RECTANGLE));
        // There is a bug in the SCI spec at
        // http://freesci.linuxgames.com/scihtml/x2396.html
        // Where xOld and yOld are not updated here.
        absCoords.x = x;
        absCoords.y = y;
    }
}

void PicResource::_ReadRelativeMediumLines(sci::istream &stream)
{
    AbsoluteCoords absCoords;
    stream >> absCoords;
    BYTE bInput;
    while (stream.peek(bInput) && (bInput < 0xf0))
    {
        BYTE bTemp;
        stream >> bTemp;
        WORD x, y;
        if (bTemp & 0x80)
        {
            y = absCoords.y - (bTemp & 0x7f);
        }
        else
        {
            y = absCoords.y + bTemp;
        }
        stream >> bTemp;
        x = _CalcRelativeX(absCoords.x, bTemp);
        _picCommands.push_back(PicCommand::CreateLine(absCoords.x, absCoords.y, x, y));
        absCoords.x = x;
        absCoords.y = y;
    }
}

void PicResource::_ReadRelativeLongLines(sci::istream &stream)
{
    AbsoluteCoords absCoordsOne;
    stream >> absCoordsOne;
    BYTE bInput;
    while (stream.peek(bInput) && (bInput < 0xf0))
    {
        AbsoluteCoords absCoordsTwo;
        stream >> absCoordsTwo;
        _picCommands.push_back(PicCommand::CreateLine(absCoordsOne.x, absCoordsOne.y, absCoordsTwo.x, absCoordsTwo.y));
        absCoordsOne.x = absCoordsTwo.x;
        absCoordsOne.y = absCoordsTwo.y;
    }
}

void PicResource::_ReadRelativeShortLines(sci::istream &stream)
{
    AbsoluteCoords absCoords;
    stream >> absCoords;
    RelativeCoords relCoords(absCoords);
    BYTE bInput;
    while (stream.peek(bInput) && (bInput < 0xf0))
    {
        stream >> relCoords;
        _picCommands.push_back(PicCommand::CreateLine(relCoords.xPrev, relCoords.yPrev, relCoords.x, relCoords.y));
        relCoords.Next();
    }
}

void PicResource::_ReadFill(sci::istream &stream)
{
    BYTE bTemp;
    while (stream.peek(bTemp) && (bTemp < 0xf0))
    {
        AbsoluteCoords absCoords;
        stream >> absCoords;
        _picCommands.push_back(PicCommand::CreateFill(absCoords.x, absCoords.y));
    }
}

void PicResource::_ReadSetPaletteEntry(sci::istream &stream)
{
    BYTE bTemp;
    while (stream.peek(bTemp) && (bTemp < 0xf0))
    {
        BYTE bIndex, bColor;
        stream >> bIndex;
        stream >> bColor;
        BYTE bPaletteNum = bIndex / 40;
        BYTE bOffset = bIndex % 40;
        if (bPaletteNum < 4)
        {
            EGACOLOR color;
            // High bits are color1
            color.color1 = bColor >> 4;
            // Low bits are color 2
            color.color2 = bColor & 0xf;
            _picCommands.push_back(PicCommand::CreateSetPaletteEntry(bPaletteNum, bOffset, color));
        }
        else
        {
            REPORTERROR("Bad palette number.");
        }
    }
}

void PicResource::_ReadSetPalette(sci::istream &stream)
{
    BYTE bPaletteNum;
    stream >> bPaletteNum;
    EGACOLOR palette[40];
    EGACOLOR *pPalette = palette;
    for (int i = 0; i < ARRAYSIZE(palette); i++)
    {
        BYTE bColor;
        stream >> bColor;
        EGACOLOR colorToSet;
        colorToSet.color1 = (bColor >> 4);
        colorToSet.color2 = (bColor & 0xf);
        *pPalette = colorToSet;
        pPalette++;
    }
    _picCommands.push_back(PicCommand::CreateSetPalette(bPaletteNum, palette));
}

/*
HRESULT PicResource::_ReadSetVisualSCI1(CPicByteStream &data)
{
    BYTE bColorCode;
    HRESULT hr = data.GetInput(&bColorCode);
    if (SUCCEEDED(hr))
    {
        PicCommand command;
        command.CreateSetVisualSCI1(bColorCode);
        _picCommands.push_back(command);
    }
    return hr;
}

HRESULT PicResource::_ReadSetPriorityBars(CPicByteStream &data)
{
    BYTE rgbBars[NumPriorityBars];
    HRESULT hr = data.GetInput(rgbBars, sizeof(rgbBars));
    if (SUCCEEDED(hr))
    {
        PicCommand command;
        command.CreateSetPriorityBars(rgbBars);
        _picCommands.push_back(command);
    }
    return hr;
}

HRESULT PicResource::_ReadSetPaletteSCI1(CPicByteStream &data)
{
    HRESULT hr = data.SkipInput(0x104); // 256 bytes of 0x00-0xff, followed by 4 bytes of 00
    if (SUCCEEDED(hr))
    {
        // There's more to this here, but for now we'll ignore.  The unused by of each COLORREF is
        // an "importance byte"... if 0, then the colour is "important" (forces colour to be used in game palette).
        BYTE rgColors[256 * 4];
        hr = data.GetInput(rgColors, sizeof(rgColors));
        if (SUCCEEDED(hr))
        {
            PicCommand command;
            command.CreateSetPaletteSCI1(rgColors);
            _picCommands.push_back(command);
        }
    }
    return hr;
}

//
// REVIEW:
// In general, these functions should fail outright (fatally), if they didn't read their
// full amount of data (because future commands will then be corrupted).
//
HRESULT PicResource::_ReadDrawVisualBitmap(CPicByteStream &data)
{
    HRESULT hr = data.SkipInput(1); // First byte is 0.
    if (SUCCEEDED(hr))
    {
        // x, y
        BYTE bx, by;
        hr = data.GetInput(&bx);
        if (SUCCEEDED(hr))
        {
            hr = data.GetInput(&by);
        }

        // length of data
        WORD cbEncoded;
        if (SUCCEEDED(hr))
        {
            hr = data.GetInput((BYTE*)&cbEncoded, sizeof(cbEncoded));
        }

        // width, height
        WORD cx, cy;
        if (SUCCEEDED(hr))
        {
            hr = data.GetInput((BYTE*)&cx, sizeof(cx));
            if (SUCCEEDED(hr))
            {
                hr = data.GetInput((BYTE*)&cy, sizeof(cy));
                if (SUCCEEDED(hr))
                {
                    hr = data.SkipInput(sizeof(WORD)); // Unknown WORD
                }
            }
        }

        BYTE bTransparent;
        BYTE bMirroring;
        if (SUCCEEDED(hr))
        {
            hr = data.GetInput(&bTransparent);
            if (SUCCEEDED(hr))
            {
                hr = data.GetInput(&bMirroring);
            }
        }

        if (SUCCEEDED(hr))
        {
            // We already read 8 bytes since we read encoding, so subtract 8
            cbEncoded -= 8;
            std::vector<BYTE> bitmapData;
            bitmapData.resize(cbEncoded);
            hr = data.GetInput(&bitmapData[0], cbEncoded);
            if (SUCCEEDED(hr))
            {
                PicCommand command;
                command.CreateDrawVisualBitmap(bx, by, cx, cy, bTransparent, &bitmapData[0], cbEncoded);
                _picCommands.push_back(command);
            }
        }
    }
    return hr;
}
*/

PicResource::~PicResource()
{
}

EGACOLOR g_egaColorChooserPalette[] = 
{
    { 0, 0 },
    { 1, 1 },
    { 2, 2 },
    { 3, 3 },
    { 4, 4 },
    { 5, 5 },
    { 6, 6 },
    { 7, 7 },
    { 8, 8 },
    { 9, 9 },
    { 10, 10 },
    { 11, 11 },
    { 12, 12 },
    { 13, 13 },
    { 14, 14 },
    { 15, 15 },
};

/////////////////////////////B//////G/////R///////
RGBQUAD g_egaColors[] = { { 0x00, 0x00, 0x00, 0x00} ,
                          { 0xA0, 0x00, 0x00, 0x00} ,
                          { 0x00, 0xA0, 0x00, 0x00} ,
                          { 0xA0, 0xA0, 0x00, 0x00} ,
                          { 0x00, 0x00, 0xA0, 0x00} ,
                          { 0xA0, 0x00, 0xA0, 0x00} ,
                          { 0x00, 0x50, 0xA0, 0x00} ,
                          { 0xA0, 0xA0, 0xA0, 0x00} ,
                          { 0x50, 0x50, 0x50, 0x00} ,
                          { 0xff, 0x50, 0x50, 0x00} ,
                          { 0x50, 0xff, 0x00, 0x00} ,
                          { 0xff, 0xff, 0x50, 0x00} ,
                          { 0x50, 0x50, 0xff, 0x00} ,
                          { 0xff, 0x50, 0xff, 0x00} ,
                          { 0x50, 0xff, 0xff, 0x00} ,
                          { 0xff, 0xff, 0xff, 0x00} ,
};

COLORREF g_egaColorsCR[] = { RGB(0x00, 0x00, 0x00) ,
                            RGB(0x00, 0x00, 0xA0),
                            RGB(0x00, 0xA0, 0x00),
                            RGB(0x00, 0xA0, 0xA0),
                            RGB(0xA0, 0x00, 0x00),
                            RGB(0xA0, 0x00, 0xA0),
                            RGB(0xA0, 0x50, 0x00),
                            RGB(0xA0, 0xA0, 0xA0),
                            RGB(0x50, 0x50, 0x50),
                            RGB(0x50, 0x50, 0xff),
                            RGB(0x50, 0xff, 0x00),
                            RGB(0x50, 0xff, 0xff),
                            RGB(0xff, 0x50, 0x50),
                            RGB(0xff, 0x50, 0xff),
                            RGB(0xff, 0xff, 0x50),
                            RGB(0xff, 0xff, 0xff),
};


SCIBitmapInfo::SCIBitmapInfo(int cx, int cy)
{
    bmiHeader.biSize = sizeof(bmiHeader);
    bmiHeader.biWidth = cx;
    bmiHeader.biHeight = cy;
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 8; // That's right, 8bpp, though we only use 4.  Waste of space, but makes it easy to draw on.
    bmiHeader.biCompression = BI_RGB;
    bmiHeader.biSizeImage = 0; // 0 is ok for BI_RGB
    bmiHeader.biXPelsPerMeter = 0; // ???
    bmiHeader.biYPelsPerMeter = 0; // ??? 
    bmiHeader.biClrUsed = 16;
    bmiHeader.biClrImportant = 16;
    CopyMemory(&bmiColors, g_egaColors, sizeof(g_egaColors));
}

SCIBitmapInfoEx::SCIBitmapInfoEx(int cx, int cy)
{
    bmiHeader.biSize = sizeof(bmiHeader);
    bmiHeader.biWidth = cx;
    bmiHeader.biHeight = cy;
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 8;
    bmiHeader.biCompression = BI_RGB;
    bmiHeader.biSizeImage = 0; // 0 is ok for BI_RGB
    bmiHeader.biXPelsPerMeter = 0;
    bmiHeader.biYPelsPerMeter = 0;
    bmiHeader.biClrUsed = 256;
    bmiHeader.biClrImportant = 256;
    for (int i = 0; i < ARRAYSIZE(_colors); i += 16)
    {
        CopyMemory(&bmiColors + i, g_egaColors, sizeof(g_egaColors));
    }
}

struct SCI1BitmapInfo : public BITMAPINFO
{
    SCI1BitmapInfo(int cx, int cy, RGBQUAD *rg256Palette)
    {
        bmiHeader.biSize = sizeof(bmiHeader);
        bmiHeader.biWidth = cx;
        bmiHeader.biHeight = cy;
        bmiHeader.biPlanes = 1;
        bmiHeader.biBitCount = 8; // That's right, 8bpp, though we only use 4.  Waste of space, but makes it easy to draw on.
        bmiHeader.biCompression = BI_RGB;
        bmiHeader.biSizeImage = 0; // 0 is ok for BI_RGB
        bmiHeader.biXPelsPerMeter = 0; // ???
        bmiHeader.biYPelsPerMeter = 0; // ??? 
        bmiHeader.biClrUsed = 256;
        bmiHeader.biClrImportant = 256;
        CopyMemory(&bmiColors, rg256Palette, sizeof(_colors));
    }
    RGBQUAD _colors[256];
};


BITMAPINFO *PicResource::GetSCIBMI() const
{
    if (_f256Color)
    {
        return new SCI1BitmapInfo(sPIC_WIDTH, sPIC_HEIGHT, NULL);
    }
    else
    {
        return new SCIBitmapInfo(sPIC_WIDTH, sPIC_HEIGHT);
    }
}

const PicCommand &PicResource::GetCommandAt(INT_PTR iIndex) const
{
    return _picCommands[iIndex];
}

//
// Caller needs to free HBITMAP
//
HBITMAP PicResource::GetVisualBitmap(int cx, int cy) const
{
    return PicDrawManager(this).CreateVisualBitmap(cx, cy);
}

HBITMAP PicResource::GetPriorityBitmap(int cx, int cy) const
{
    return PicDrawManager(this).CreatePriorityBitmap(cx, cy);
}

HBITMAP PicResource::GetControlBitmap(int cx, int cy) const
{
    return PicDrawManager(this).CreateControlBitmap(cx, cy);
}


INT_PTR PicResource::GetMemorySize() const
{
    INT_PTR iNumberCommands = _picCommands.size();
    return ((iNumberCommands * sizeof(PicCommand)) + sizeof(PicResource));
}

BOOL _GetOppositeCommandType(PicCommand *pCommand, PicCommand::CommandType *pType)
{
    BOOL fRet = FALSE;
    switch(pCommand->type)
    {
    case PicCommand::SetVisual:
        *pType = PicCommand::DisableVisual;
        fRet = TRUE;
        break;
    case PicCommand::SetPriority:
        *pType = PicCommand::DisablePriority;
        fRet = TRUE;
        break;
    case PicCommand::SetControl:
        *pType = PicCommand::DisableControl;
        fRet = TRUE;
        break;
    case PicCommand::DisableVisual:
        *pType = PicCommand::SetVisual;
        fRet = TRUE;
        break;
    case PicCommand::DisablePriority:
        *pType = PicCommand::SetPriority;
        fRet = TRUE;
        break;
    case PicCommand::DisableControl:
        *pType = PicCommand::SetControl;
        fRet = TRUE;
        break;
    }
    return fRet;
}

void PicResource::_InsertCommands(INT_PTR iStart, INT_PTR cCount, PicCommand *pCommands)
{
    INT_PTR iSize = _picCommands.size();
    if (iStart == -1)
    {
        iStart = iSize; // at the end
    }

    if ((iStart <= iSize) && (iStart >= 0))
    {
        _picCommands.insert(&_picCommands[iStart], pCommands, pCommands + cCount);
    }
}

void PicResource::InsertCommands(INT_PTR iStart, INT_PTR cCount, PicCommand *pCommands)
{
    // Special case:
    // If we are inserting a toggle visual/priority/control, then if the previous command
    // is the opposite, just delete it.
    PicCommand::CommandType type;
    if ((cCount == 1) &&
        (iStart > 0) &&
        _GetOppositeCommandType(pCommands, &type) &&
        (_picCommands[iStart - 1].type == type))

    {
        // delete instead.
        RemoveCommand(iStart - 1);
    }
    else
    {
        _InsertCommands(iStart, cCount, pCommands);
    }
}

//
// Remove a particular command.
// iCommandIndex may be -1, to indicate the last command
//
void PicResource::RemoveCommand(INT_PTR iCommandIndex)
{
    RemoveCommandRange(iCommandIndex, iCommandIndex);
}

//
// iEnd may be -1 (to indicate the end)
//
void PicResource::RemoveCommandRange(INT_PTR iStart, INT_PTR iEnd)
{
    if (iStart == -1)
    {
        iStart = _picCommands.size() - 1;
    }

    if (iEnd == -1)
    {
        iEnd = _picCommands.size() - 1;
    }

    if ((iStart >= 0) && (iStart < (INT_PTR)_picCommands.size()))
    {
        INT_PTR iHowMany = iEnd - iStart + 1;
        _picCommands.erase(&_picCommands[iStart], &_picCommands[iStart + iHowMany]);
    }
}


HRESULT PicResource::CopyRangeToClipboard(INT_PTR iStart, INT_PTR iEnd) const
{
    HRESULT hr = E_FAIL;
    if ((iStart >= 0) && (iEnd < (INT_PTR)_picCommands.size()))
    {
        UINT uFormat = theApp.GetCommandClipboardFormat();
        if (uFormat)
        {
            if (OpenClipboard(theApp.GetMainWnd()->GetSafeHwnd()))
            {
                HANDLE hClip = NULL;
                HGLOBAL hGlobal = CopiedCommands_AllocAndFillMemory(&_picCommands[0] + iStart, iEnd - iStart + 1);
                if (hGlobal)
                {
                    // Empty the clipboard - otherwise, it could have both commands and a bitmap on it,
                    // and the piceditor will prefer the bitmap (tracing image).
                    EmptyClipboard();

                    hClip = SetClipboardData(uFormat, hGlobal);
                    if (hClip == NULL)
                    {
                        GlobalFree(hGlobal);
                    }
                    else
                    {
                        // Clipboard has taken ownership
                        hr = S_OK;
                    }
                }
                CloseClipboard();
            }
        }
    }
    return hr;
}

//
// Draw pic to position iPos (-1 means entire thing)
//
void PicResource::Draw(PicData &data, SCIPicState &state, INT_PTR iPos) const
{
    if (iPos == -1)
    {
        iPos = _picCommands.size();
    }
    iPos = min((INT_PTR)_picCommands.size(), iPos);
    for (INT_PTR i = 0; i < iPos; i++)
    {
        const PicCommand &command = _picCommands[i];
        command.Draw(&data, state);
    }
}

void PicResource::UpdatePicState(SCIPicState &state, INT_PTR iDrawPos) const
{
    if (iDrawPos == -1)
    {
        iDrawPos = _picCommands.size();
    }
    iDrawPos = min((INT_PTR)_picCommands.size(), iDrawPos);
    for (INT_PTR i = 0; i < iDrawPos; i++)
    {
        const PicCommand &command = _picCommands[i];
        switch(command.type)
        {
        case PicCommand::SetVisual:
        case PicCommand::SetPriority:
        case PicCommand::SetControl:
        case PicCommand::DisableVisual:
        case PicCommand::DisablePriority:
        case PicCommand::DisableControl:
        case PicCommand::SetPalette:
        case PicCommand::SetPaletteEntry:
            // These commands affect picture state.
            command.Draw(NULL, state);
            break;

        case PicCommand::Pattern:
            // This one affects state too.  But tell it not to draw.
            PatternCommand_Draw_StateOnly(&command, NULL, &state);

        default:
            // The others do not affect state.
            break;
        }
    }
}

INT_PTR PicResource::GetLastChangedSpot(PicData &data, SCIPicState &state, int x, int y) const
{
    // Now process all our pic commands!
    BYTE bColor = 0xf; // White, for visual.  Note - this won't work well for dithered colors.  We need to expand our radius
    INT_PTR iLastTimeItChanged = -1; // Never changed.
    size_t iPos = _picCommands.size();
    for (size_t i = 0; i < iPos; i++)
    {
        _picCommands[i].Draw(&data, state);
        if (data.pdataVisual[BUFFEROFFSET(x, y)] != bColor)
        {
            iLastTimeItChanged = i;
            bColor = data.pdataVisual[BUFFEROFFSET(x, y)];
        }
    }
    return iLastTimeItChanged;
}


