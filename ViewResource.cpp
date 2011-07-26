
#include "stdafx.h"
#include "ViewResource.h"
#include "SCIPicEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

void RasterBitmap::_fill(size_t pos, BYTE bValue, int cAmount)
{
    fill_n(&_bits[pos], cAmount, bValue);
}

#define ISMIRROR(nLoop, mask) (0 != (((mask) >> (nLoop)) & 1))

#define GETPIXEL(p, x, y, cxActual) (*((p) + ((y) * (cxActual)) + (x)))
#define GNP_DONE 0      // pColor is not set
#define GNP_NEWLINE 1   // pColor is set
#define GNP_OK 2        // pColor is set
int GetNextPixel(const BYTE *pData, int &x, int &y, int cx, BYTE *pColor)
{
    int iResult = GNP_OK;
    if (x >= cx)
    {
        x = 0;
        if (y > 0)
        {
            y--;
            iResult = GNP_NEWLINE;
        }
        else
        {
            iResult = GNP_DONE;
        }
    }

    if (iResult != GNP_DONE)
    {
        *pColor = GETPIXEL(pData, x, y, CX_ACTUAL(cx));
    }

    ++x;
    return iResult;
}


ViewResource::CelInfo::CelInfo() : RasterBitmap()
{
    _bTransparent = 0x3;
    _xPlacement = 0;
    _yPlacement = 0;
}
ViewResource::CelInfo::CelInfo(const CelInfo &src) : RasterBitmap(src)
{
    _bTransparent = src._bTransparent;
    _xPlacement = src._xPlacement;
    _yPlacement = src._yPlacement;
}
ViewResource::CelInfo& ViewResource::CelInfo::operator=(const ViewResource::CelInfo& src)
{
    if (&src != this)
    {
        _bTransparent = src._bTransparent;
        _xPlacement = src._xPlacement;
        _yPlacement = src._yPlacement;
        RasterBitmap::operator=(src);
    }
    return *this;
}

bool ViewResource::CelInfo::read(sci::istream &byteStream)
{
    // Width and height
    byteStream >> width();
    byteStream >> height();
    if (byteStream.good())
    {
        // Ensure the size is reasonable (unsure if this is a real limit), so we don't go
        // allocating huge amounts of memory.
        _ClampSize(width(), height());
        // TODO - mark status "corrupted" if we had to adjust here.

        // x and y placement (right, down)
        WORD wPlacement;
        byteStream >> wPlacement;
        _xPlacement = (char)(wPlacement & 0x00ff);
        _yPlacement = (char)((wPlacement & 0xff00) >> 8);

        // Transparent colour
        byteStream >> _bTransparent;
        if (byteStream.good())
        {
            _read_imagedata(byteStream);
        }
    }
    return byteStream.good();
}

bool ViewResource::CelInfo::_read_imagedata(sci::istream &byteStream)
{
    // For some reason, the bitmap with needs to be rounded up to fit on a DWORD boundary.
    int cxActual = CX_ACTUAL(width());
    int cxRemainingOnThisLine = width();
    int cxLine = width();
    int cxPadding = cxActual - cxLine;

    // Bitmap buffer:
    int cBufferSizeRemaining = cxActual * (int)height();

    // The image starts from the bottom left, not the top left:
    int y = height() - 1;
    int x = 0;
    // Create our buffer, just filled with zeros for the time being.
    _blank_fill();
    BYTE *pImageDataCur = get_bits();
    BYTE color = 0;
    int cCount = 0;
    int cxRemainingForThisCommand = 0;
    while (byteStream.good() && (cBufferSizeRemaining > 0))
    {
        BYTE b;
        if (cxRemainingForThisCommand == 0)
        {
            // Read more data.
            byteStream >> b;
            color = (b & 0x0f);
            cCount = ((b & 0xf0) >> 4);
            cxRemainingForThisCommand = cCount;
        }

        ASSERT(y >= 0);

        int cAmountToFill;
        // Fill up to the end of this command.
        if (cxRemainingForThisCommand < cxRemainingOnThisLine)
        {
            cAmountToFill = cxRemainingForThisCommand;
            cxRemainingForThisCommand = 0;
            _fill(y * cxActual + x, color, cAmountToFill);
            x += cAmountToFill;
            cxRemainingOnThisLine -= cAmountToFill;
            cBufferSizeRemaining -= cAmountToFill;
        }
        else
        {
            // We are going to fill up to the end of the line, and pad the rest
            cAmountToFill = cxLine - x;
            _fill(y * cxActual + x, color, cAmountToFill);

            cBufferSizeRemaining -= cAmountToFill;
            // Pad:
            _fill(y * cxActual + x + cAmountToFill, _bTransparent, cxPadding);
            cBufferSizeRemaining -= cxPadding;

            // Move to the next line:
            x = 0;
            y -= 1;
            cxRemainingForThisCommand = cxRemainingForThisCommand - cxRemainingOnThisLine;
            cxRemainingOnThisLine = width();
        }        
    }

    return byteStream.good();
}

ViewResource::CelInfo ViewResource::CelInfo::InheritFromTemplate(const ViewResource::CelInfo &celTemplate)
{
    CelInfo cel;
    cel.transparent(celTemplate.transparent());
    cel.width(celTemplate.width());
    cel.height(celTemplate.height());
    cel.x_placement(celTemplate.x_placement());
    cel.y_placement(celTemplate.y_placement());
    return cel;
}

//
// CelInfo
//

//
// Realloc the bits in a CelInfo due to a change in size.
// Nothing changes if the size didn't change.
// fCopy indicates whether or not to copy over the old bits.
// Note: the pBits in pCel are deleted and reallocated.
// Ensure that they are valid or NULL
// fForce says whether or not to do it even if the size didn't change.
//
void RasterBitmap::ReallocBits(WORD cx, WORD cy, bool fCopy, bool fForce, bool fFill, BYTE bColor, RasterResizeFlags flags)
{
    // Validate sizes
    _ClampSize(cx, cy);
    if ((width() != cx) || (height() != cy) || fForce)
    {
        std::vector<BYTE> newBits;
        size_t cItems = CX_ACTUAL(cx) * cy;
        newBits.resize(cItems);
        if (fCopy || fFill)
        {
            if (fFill)
            {
                // Fill the new bitmap with the transparent colour
                newBits.assign(cItems, bColor);
            }
            if (fCopy)
            {
                BYTE *pBitsNew = &newBits[0];   // Access to raw data
                BYTE *pBits = get_bits();        // Access to old raw data

                // Copy from the old bitmap (y = 0 is at the bottom)
                int yEnd = min(cy, height());
                int cbToCopy = min(cx, width());
                int dy = cy - height();
                int xDestOffset = 0;
                int xSrcOffset = 0;

                if (Stretch == flags)
                {
                    // We're stretching.
                    for (int y = 0; y < cy; y++)
                    {
                        BYTE *pLineDest = pBitsNew + CX_ACTUAL(cx) * y;
                        // Careful of overflow (should be ok, views don't tend to be bigger than 256):
                        BYTE *pLineSrc = pBits + CX_ACTUAL(width()) * (y * height() / cy);
                        for (int x = 0; x < cx; x++)
                        {
                            *(pLineDest + x) = *(pLineSrc + x * width() / cx);
                        }
                    }
                }
                else
                {
                    // Normal case
                    if (AnchorBottomRight == flags)
                    {
                        // We anchor on the bottom right.
                        dy = 0;
                        int dx = cx - width();
                        if (dx > 0)
                        {
                            // Getting bigger.
                            xDestOffset = dx;
                        }
                        else
                        {
                            xSrcOffset = -dx;
                        }
                    } // Otherwise we anchor on the top left.
                    BYTE *pDest = (dy > 0) ? (pBitsNew + dy * CX_ACTUAL(cx)) : pBitsNew;
                    BYTE *pSrc = (dy < 0) ? (pBits + (-dy) * CX_ACTUAL(width())) : pBits;
                    for (int y = 0; y < yEnd; y++)
                    {
                        CopyMemory(pDest + y * CX_ACTUAL(cx) + xDestOffset, 
                                pSrc + y * CX_ACTUAL(width()) + xSrcOffset,
                                cbToCopy);
                    }
                }
            }
        }
        // Do the replacement.
        _bits.swap(newBits);
        width(cx);
        height(cy);
    }
    else
    {
        //ASSERT(fCopy == FALSE); // We could fix this - but at the moment, we aren't copying in this case.
    }
}

void ViewResource::CelInfo::copy_mirrored(const CelInfo &celOrig)
{
    // And now the bits
    int cxActual = CX_ACTUAL(width());
    const BYTE *pSrcBits = celOrig.get_bits();
    BYTE *pBits = get_bits();
    // Copy them over in reverse order
    for (int y = 0; y < celOrig.height(); y++)
    {
        // Start the original at the beginning of the line
        const BYTE *pLineOrig = pSrcBits + (y * cxActual);
        // ...and the mirror at the end.
        BYTE *pLineMirror = pBits + (y * cxActual) + (celOrig.width() - 1);
        for (int x = 0; x < celOrig.width(); x++)
        {
            *pLineMirror = *pLineOrig;
            ++pLineOrig;
            --pLineMirror;
        }
    }
}

void ViewResource::CelInfo::write_cel(sci::istream *pSerial) const
{
    // Preliminary data (size, placement, transparent colour)
    pSerial->WriteWord(width());
    pSerial->WriteWord(height());
    // Convert to BYTE before converting to WORD, to avoid sign extension problems.
    WORD wPlacement = (BYTE)_xPlacement;
    wPlacement |= (((WORD)(BYTE)_yPlacement) << 8);
    pSerial->WriteWord(wPlacement);
    pSerial->WriteByte(_bTransparent);

    // Now the image data
    // cxActual is how wide our bitmap data is (a boundary of 4 pixels - 32 bits)
    int cxActual = CX_ACTUAL(width());

    // Start at the bottom
    int y = height() - 1;
    int x = 0;

    BYTE cCount = 0;
    BYTE cColorPrev = 0xff; // invalid colour value
    
    // For some reason, image data always starts with a 0x00
    pSerial->WriteByte(0);

    int iResult = GetNextPixel(get_bits(), x, y, width(), &cColorPrev);
    cCount = 1;

    BYTE cColor;
    while (iResult != GNP_DONE)
    {
        // Look at the next pixel.
        iResult = GetNextPixel(get_bits(), x, y, width(), &cColor);
        if ((iResult == GNP_DONE) || (iResult == GNP_NEWLINE) || (cCount == 15) || (cColor != cColorPrev))
        {
            // (It is not valid to test cColor if iResult is GNP_DONE)
            ASSERT((iResult == GNP_DONE) || (cColor < 16));
            ASSERT(cCount < 16);

            // Write out our data
            BYTE b = ((cCount << 4) | cColorPrev);
            pSerial->WriteByte(b);
            cCount = 0;
        }
        cColorPrev = cColor;
        cCount++;
    }
}

HBITMAP ViewResource::CelInfo::get_bitmap(int cx, int cy, bool fAllowZoom) const
{
    HBITMAP hbm = NULL;
    CDC dc;
    if (dc.CreateCompatibleDC(NULL))
    {
        int cxBmp = 0;
        int cyBmp = 0;
        int iZoom = 0;
        BYTE bTransparent = 0;
        const BYTE *pBits = get_bits();
        
        ASSERT(_bTransparent < 16);
        // Figure out how much we can zoom.  If a dimension is 0, consider it 1. (avoid / by 0)
        iZoom = min((width() ? (cx / width()) : cx), (height() ? (cy / height()) : cy));
        iZoom = max(1, iZoom);
        iZoom = min(4, iZoom);

        SCIBitmapInfo bmi(cx, cy);
        BYTE *pBitsDest;
        hbm = CreateDIBSection((HDC)dc, &bmi, DIB_RGB_COLORS, (void**)&pBitsDest, NULL, 0);
        if (hbm)
        {
            // Fill with white.
            FillMemory(pBitsDest, CX_ACTUAL(cx) * cy, 0x0f);

            if (iZoom == 1)
            {
                // If zoom factor == 1, copy directly to the bmp
                int yOffset = height() - min(cy, height()); // yOffset -> if there's not enough room, copy the TOP of the view.
                for (int y = 0; y < min(cy, height()); y++)
                {
                    // Copy a scan line.
                    CopyMemory(pBitsDest + y * CX_ACTUAL(cx), pBits + (y + yOffset) * CX_ACTUAL(width()), min(cx, width()));
                }
            }
            else
            {
                // Otherwise, copy a magnified view.
                int cyBmpZoom = iZoom * height();
                int cxBmpZoom = iZoom * width();
                for (int y = 0; y < min(cy, height()); y++)
                {
                    // Copy iZoom scanlines.
                    const BYTE *pSrc = pBits + y * CX_ACTUAL(width());
                    for (int x = 0; x < min(cx, width()); x++)
                    {
                        BYTE b = *pSrc;
                        for (int yy = 0; yy < iZoom; yy++)
                        {
                            // For each row, set it at the left edge
                            BYTE *pDest = pBitsDest + ((y * iZoom) + yy) * CX_ACTUAL(cx) + x * iZoom; 
                            for (int xx = 0; xx < iZoom; xx++)
                            {
                                *pDest = b;
                                pDest++;
                            }

                        }
                        pSrc++; // Increment to the next column of the source
                    }
                }
            }
        }
    }
    return hbm;
}
void ViewResource::CelInfo::copy_bitmap_data(BYTE *pData, CSize sizeDebug) const
{
    ASSERT((width() == sizeDebug.cx) && (height() == sizeDebug.cy));
    memcpy(pData, get_bits(), datasize());
}

//
// LoopInfo
//
ViewResource::LoopInfo::LoopInfo() : _wDummy(0), _fMirror(false), _bMirrorOf(-1)
{
}
ViewResource::LoopInfo::LoopInfo(const CelInfo &src) : _wDummy(0), _fMirror(false), _bMirrorOf(-1)
{
    _cels.push_back(src);
}
ViewResource::LoopInfo::LoopInfo(const LoopInfo &src)
{
    _wDummy = src._wDummy;
    _fMirror = src._fMirror;
    _bMirrorOf = src._bMirrorOf;
    _cels = src._cels;
}
ViewResource::LoopInfo& ViewResource::LoopInfo::operator=(const LoopInfo& src)
{
    if (&src != this)
    {
        _wDummy = src._wDummy;
        _fMirror = src._fMirror;
        _bMirrorOf = src._bMirrorOf;
        _cels = src._cels;
    }
    return *this;
}

bool ViewResource::LoopInfo::read(sci::istream &byteStream)
{
    ASSERT(_cels.empty());
    WORD nCels;
    byteStream >> nCels;
    bool fZeroCels = (nCels == 0);
    if (fZeroCels)
    {
        nCels = 1;
        // Handle corrupt views (e.g. LSL2, view 225, loop 3) by
        // creating a dummy loop with 1 1x1 cel
    }
    if (byteStream.good())
    {
        _cels.assign(nCels, CelInfo()); // Just a bunch of empty ones.
    }
    byteStream >> _wDummy; // Skip 2 bytes (function unknown) - store just in case
    
    if (fZeroCels)
    {
        _cels[0].create_degenerate(_cels[0].transparent());
    }
    else
    {
        for (WORD i = 0; i < nCels; i++)
        {
            WORD nOffset;
            byteStream >> nOffset;
            if (byteStream.good())
            {
                sci::istream byteStreamCel(byteStream);
                byteStreamCel.seekg(nOffset);
                _cels[i].read(byteStreamCel);
            }
        }
    }
    ASSERT(!byteStream.good() || (_cels.size() == nCels)); // Ensure cel count is right.
    return byteStream.good();
}

int ViewResource::LoopInfo::size() const
{
    return (int)_cels.size();
}

ViewResource::CelInfo &ViewResource::LoopInfo::operator[](int iIndex)
{
    return _cels[iIndex];
}
const ViewResource::CelInfo &ViewResource::LoopInfo::operator[](int iIndex) const
{
    return _cels[iIndex];
}
void ViewResource::LoopInfo::insert_cel(int nCel, const CelInfo &cel)
{
    _cels.insert(&_cels[nCel], cel);
}
void ViewResource::LoopInfo::remove_cel(int nCel)
{
    _cels.erase(&_cels[nCel]);
}
void ViewResource::LoopInfo::move_from_to(int celFrom, int celTo)
{
    // Assumes the cel indices are valid
    // (celTo can point to one after)
    CelInfo celTemp = _cels[celFrom];
    _cels.insert(&_cels[celTo], celTemp);
    // Now remove the from, which could be before or after
    if (celFrom > celTo)
    {
        _cels.erase(&_cels[celFrom + 1]);
    }
    else
    {
        _cels.erase(&_cels[celFrom]);
    }
}

#define UPDATE_FROM_MIRROR 0xFF
//
// Make this loop be a mirror of nOriginal. loopOrig is the LoopInfo for nOriginal.
// if nOriginal is UPDATE_FROM_MIRROR, then this loop is actually the source, and it being updated
// from its mirror.
//
void ViewResource::LoopInfo::mirror_from(BYTE nOriginal, const LoopInfo &loopOrig)
{
    // Reallocate all cels.
    _cels.clear();
    if (nOriginal != UPDATE_FROM_MIRROR)
    {
        _bMirrorOf = nOriginal;
        _fMirror = true;
    }
    for (int i = 0; i < loopOrig.size(); i++)
    {
        // Make new empty cels, and for each one, do a "sync mirror state" if the original.
        _cels.push_back(ViewResource::CelInfo());
        ASSERT(_cels.size() == (i + 1));
        _SyncCelMirrorState(loopOrig[i], _cels[i]);
    }
}
void ViewResource::LoopInfo::write_loop(sci::istream *pSerial) const
{
    // # of cels
    int cCels = (int)_cels.size();
    pSerial->WriteWord((WORD)cCels);
    // 2 unknown bytes
    pSerial->WriteWord(_wDummy);

    // Cel offsets:
    WORD wOffsets = 0;
    // Just need to take up some space for the time being.
    WORD *rgOffsetsDummy = new WORD[cCels];
    if (rgOffsetsDummy)
    {
        // Stash this address for later;
        wOffsets = (WORD)pSerial->GetPosition();
        ZeroMemory(rgOffsetsDummy, sizeof(WORD) * cCels);
        pSerial->WriteBytes((BYTE*)rgOffsetsDummy, sizeof(WORD) * cCels);
        delete [] rgOffsetsDummy;
    }

    for (int i = 0; i < cCels; i++)
    {
        // Set the offset:
        // We need to recalc this each time (Based on iIndex), since the pData
        // in the serializer can change (be re-alloced).
        WORD *pOffsets = (WORD*)(pSerial->GetInternalPointer() + wOffsets);
        pOffsets[i] = ((WORD)(pSerial->GetPosition()));

        // Write the cel:
        _cels[i].write_cel(pSerial);
    }
}

//
// ViewResouce
//

ViewResource::ViewResource()
{
    _dwDummy = 0;
    _iResourceNumber = -1;
    _iPackageNumber = -1;
}
bool ViewResource::_ValidIndex(DWORD dwIndex) const
{
    return _ValidIndex(DWORD_TO_LOOP(dwIndex), DWORD_TO_CEL(dwIndex));
}
bool ViewResource::_ValidIndex(int nLoop, int nCel) const
{
    if (nLoop < (int)_loops.size())
    {
        if (nCel < _loops[nLoop].size())
        {
            return true;
        }
    }
    return false;
}

ViewResource::CelInfo &ViewResource::_GetCel(DWORD dwIndex)
{
    return _loops[DWORD_TO_LOOP(dwIndex)][DWORD_TO_CEL(dwIndex)];
}
const ViewResource::CelInfo &ViewResource::_GetCel(DWORD dwIndex) const
{
    return _loops[DWORD_TO_LOOP(dwIndex)][DWORD_TO_CEL(dwIndex)];
}

ViewResource::CelInfo &ViewResource::_GetCel(int nLoop, int nCel)
{
    return _loops[nLoop][nCel];
}


/******************************************************/
// Given a loop and a deleted loop, this returns the new loop
// index.
int _NewLoopNumber(int nLoop, int nDeletedLoop)
{
    return (nLoop < nDeletedLoop) ? nLoop : nLoop - 1;
}

void ViewResource::_SyncCelMirrorState(const CelInfo &celOrig, CelInfo &celMirror)
{
    ASSERT(celOrig.transparent() < 16);
    celMirror.transparent(celOrig.transparent());
    celMirror.x_placement(-celOrig.x_placement()); // Note that we invert x here!  It's a mirror!
    celMirror.y_placement(celOrig.y_placement());

    if ((celMirror.width() != celOrig.width()) || (celMirror.height() != celOrig.height()))
    {
        celMirror.ReallocBits(celOrig.width(), celOrig.height(), false, false, false, 0, RasterBitmap::Normal);
    }
    celMirror.copy_mirrored(celOrig);
}
void ViewResource::_UpdateMirrorsWholeLoop(int nLoop)
{
    _UpdateMirrors(nLoop, -1);
}
void ViewResource::_UpdateMirrors(DWORD dwIndex)
{
    _UpdateMirrors(DWORD_TO_LOOP(dwIndex), DWORD_TO_CEL(dwIndex));
}
// -1 means update the entire loop
void ViewResource::_UpdateMirrors(int nLoop, int nCel)
{
    int nLoopIgnore = -1;

    // Are we a mirror of anything?  If so, update the source.
    if (_loops[nLoop].mirror())
    {
        BYTE bSource = _loops[nLoop].mirror_of();
        if (nCel == -1)
        {
            // Replace the entire loop.
            _loops[bSource].mirror_from(UPDATE_FROM_MIRROR, _loops[nLoop]);
        }
        else
        {
            _SyncCelMirrorState(_loops[nLoop][nCel], _loops[bSource][nCel]);
        }
        // Now, is anyone else (but us) a mirror of this source?  See comment below.
        nLoopIgnore = nLoop;
        nLoop = bSource;
    }

    // Is anything a mirror of nLoop?  nLoop can either be the loop passed in (in the
    // case when we ourselves we're not a mirror), or can be the source loop (if we we're
    // a mirror).
    for (int i = 0; i < (int)_loops.size(); i++)
    {
        if ((i != nLoop) && (i != nLoopIgnore)) // If it's not us...
        {
            if (_loops[i].mirror() && (_loops[i].mirror_of() == (BYTE)nLoop))
            {
                if (nCel == -1)
                {
                    _loops[i].mirror_from((BYTE)nLoop, _loops[nLoop]);
                }
                else
                {
                    _SyncCelMirrorState(_loops[nLoop][nCel], _loops[i][nCel]);
                }
            }
        }
    }
}

//
// IViewResource
//
int ViewResource::GetMirrorOf(int nLoop) const
{
    int iMirror = -1;
    if (nLoop < (int)_loops.size())
    {
        if (_loops[nLoop].mirror())
        {
            iMirror = (int)_loops[nLoop].mirror_of();
        }
    }
    return iMirror;
}
HRESULT ViewResource::InsertCel(int nLoop, int nCel, BOOL fBefore)
{
    HRESULT hr = E_INVALIDARG;
    if (_ValidIndex(nLoop, nCel))
    {
        LoopInfo &loop = _loops[nLoop];
        if (loop.size() >= 128)
        {
            // "too many cels".  SCI Companion doesn't support more than 128.
            hr = E_INVALIDARG;
        }
        else
        {
            hr = S_OK;
            int nCelTemplate = nCel;
            if (!fBefore)
            {
                nCel++;
            }

            // Before doing anything, prepare a new cel, based on the attributes of the template cel
            const ViewResource::CelInfo &celTemplate = loop[nCelTemplate];
            // We can just copy if we're duping new cels.. otherwise, we "inherit"
            CelInfo newCel = theApp._fDupeNewCels ? celTemplate : CelInfo::InheritFromTemplate(celTemplate);
            if (!theApp._fDupeNewCels)
            {
                // Continue our inheritation!  Fill with transparent colour at least.
                newCel.ReallocBits(celTemplate.width(), celTemplate.height(), false, true, true, celTemplate.transparent(), RasterBitmap::Normal);
            }

            loop.insert_cel(nCelTemplate, newCel);
            _UpdateMirrorsWholeLoop(nLoop);
        }
        
    }
    return hr;
}
HRESULT ViewResource::InsertLoop(int nLoop, BOOL fBefore)
{
    HRESULT hr = E_INVALIDARG;
    if (nLoop < (int)_loops.size())
    {
        CelInfo &celTemplate = _loops[nLoop][0]; // First cel of previous loop.
        if (!fBefore)
        {
            nLoop++;
        }
        // Make a new loop, with one cel that is a template of the previous loops first cel.
        _loops.insert(&_loops[nLoop], LoopInfo(celTemplate));
        hr = S_OK;
    }
    return hr;
}
HRESULT ViewResource::RemoveLoop(int nLoopRemoved)
{
    HRESULT hr = E_INVALIDARG;
    if ((nLoopRemoved < (int)_loops.size()) && (_loops.size() > 1)) // Can only delete a loop if there is more than one.
    {
        // Now we need to update all the mirror references.
        // Anyone who was a mirror of nLoop is now no longer a mirror.
        // Anyone who was a mirror of some other loop, now needs that number adjusted.
        for (int i = 0; i < (int)_loops.size(); i++)
        {
            if (_loops[i].mirror())
            {
                int nMirrorOf = (int)_loops[i].mirror_of();
                if (nMirrorOf == nLoopRemoved)
                {
                    _loops[i].mirror(false); // No longer a mirror.
                }
                else
                {
                    _loops[i].mirror_of((BYTE)_NewLoopNumber(nMirrorOf, nLoopRemoved)); // Mirror # adjusted.
                }
            }
        }

        _loops.erase(&_loops[nLoopRemoved]);
        hr = S_OK;
    }
    return hr;
}
HRESULT ViewResource::MakeMirrorOf(int nLoop, int nOriginal)
{
    int cLoops = (int)_loops.size();
    if ((nLoop < cLoops) && (nOriginal < cLoops))
    {
        LoopInfo &loop = _loops[nLoop];
        if (nOriginal == -1)
        {
            // This is all there is to do.
            loop.mirror(false);
        }
        else
        {
            // Ensure that the thing we're making nLoop a mirror of, is not itself a mirror.
            // This would be a bug in our code.
            ASSERT(!_loops[nOriginal].mirror());
            loop.mirror_from((BYTE)nOriginal, _loops[nOriginal]);
        }
    }
    return S_OK;
}

bool ViewResource::_IsLastLoop(int nLoop)
{
    bool fRet = (_loops.size() == 1); // 1 loop left
    if (!fRet)
    {
        // 2 loops left, but we're a mirror, so it's essentially the last loop
        fRet = (_loops.size() == 2) && _loops[nLoop].mirror();
    }
    if (!fRet)
    {
        // One last possibility.  All other loops are mirrors of ourself.
        size_t i = 0;
        for (; i < _loops.size(); ++i)
        {
            if (nLoop != (int)i)
            {
                if (!_loops[i].mirror() || (nLoop != _loops[i].mirror_of()))
                {
                    break; // Nope...
                }
            }
        }
        fRet = (i == _loops.size()); // true, if loop completed.
    }
    return fRet;
}

HRESULT ViewResource::RemoveCel(DWORD dwIndex)
{
    HRESULT hr = E_INVALIDARG;
    if (_ValidIndex(dwIndex))
    {
        int nLoop = DWORD_TO_LOOP(dwIndex);
        LoopInfo &loop = _loops[nLoop];
        if ((loop.size() == 1) && _IsLastLoop(nLoop))
        {
            hr = E_FAIL; // Can't allow deleting.  Last cel of last loop.
        }
        else
        {
            loop.remove_cel(DWORD_TO_CEL(dwIndex));
            if (loop.size() < 1)
            {
                // That was the last cel in the loop... remove the loop.
                // And leave any mirrors as they were (e.g. don't delete the last cel in the mirrors)
                RemoveLoop(DWORD_TO_LOOP(dwIndex));
            }
            else
            {
                // Update the mirrors
                _UpdateMirrorsWholeLoop(DWORD_TO_LOOP(dwIndex));
            }
            hr = S_OK;
        }
    }
    return hr;
}


//
// IEditRasterResource
//
void ViewResource::SetBitmapData(DWORD dwIndex, const BYTE *pData)
{
    if (_ValidIndex(dwIndex))
    {
        _GetCel(dwIndex).SetData(pData);
        _UpdateMirrors(dwIndex);
    }
}

BOOL ViewResource::SetSize(DWORD dwIndex, CSize size, RasterBitmap::RasterResizeFlags flags)
{
    BOOL fRet = FALSE;
    if (_ValidIndex(dwIndex))
    {
        ViewResource::CelInfo &cel = _GetCel(dwIndex);
        if ((cel.width() != size.cx) || (cel.height() != size.cy))
        {
            cel.ReallocBits((WORD)size.cx, (WORD)size.cy, true, false, true, cel.transparent(), flags);
            _UpdateMirrors(dwIndex);
        }
        fRet = TRUE;
    }
    return fRet;
}

void ViewResource::SetTransparentColor(DWORD dwIndex, BYTE bTransparent)
{
    if (_ValidIndex(dwIndex))
    {
        _GetCel(dwIndex).transparent(bTransparent);
        _UpdateMirrors(dwIndex); // A little heavyweight
    }
}

void ViewResource::SetPlacement(DWORD dwIndex, CPoint pt)
{
    if (_ValidIndex(dwIndex))
    {
        _GetCel(dwIndex).x_placement((BYTE)pt.x);
        _GetCel(dwIndex).y_placement((BYTE)pt.y);
        _UpdateMirrors(dwIndex); // A little heavyweight
    }
}

void ViewResource::MoveFromTo(DWORD dwIndexFrom, DWORD dwIndexTo)
{
    int nLoopFrom = DWORD_TO_LOOP(dwIndexFrom);
    int nLoop = DWORD_TO_LOOP(dwIndexTo);
    int nCelFrom = DWORD_TO_CEL(dwIndexFrom);
    int nCelTo = DWORD_TO_CEL(dwIndexTo);
    // Must be from the same loop
    // Note: celTo can be one past the end.
    if (_ValidIndex(dwIndexFrom) && (nLoop == nLoopFrom) && (nCelTo <= _loops[nLoop].size()))
    {
        if (nCelFrom != nCelTo)
        {
            _loops[nLoop].move_from_to(nCelFrom, nCelTo);
            // A little heavy weight, but oh well... alternately, we could check all mirrors and repeat what we did above
            _UpdateMirrorsWholeLoop(nLoop);
        }
    }
}

BOOL ViewResource::Serialize(sci::istream *pSerial) const
{
    int cLoops = (int)_loops.size();
    // The wMirrorMask is used only during serialization.  Update it now.
    WORD wMirrorMask = 0;
    for (int i = 0; i < cLoops; i++)
    {
        if (_loops[i].mirror())
        {
            wMirrorMask |= (1 << i);
        }
    }

    // # of loops
    BOOL fRet = pSerial->WriteWord((WORD)cLoops);
    if (fRet)
    {
        // Mirror mask:
        fRet = pSerial->WriteWord((WORD)wMirrorMask);
        if (fRet)
        {
            // 4 bytes of unknown use
            fRet = pSerial->WriteBytes((BYTE*)&_dwDummy, sizeof(_dwDummy));
        }
    }

    // Loop offsets:
    WORD wOffsets = 0;
    if (fRet)
    {
        // Just need to take up some space for the time being.
        fRet = FALSE;
        scoped_array<WORD> rgOffsetsDummy(new WORD[cLoops]);
        if (rgOffsetsDummy.get())
        {
            // Stash this address for later;
            wOffsets = (WORD)(pSerial->GetPosition());
            ZeroMemory(rgOffsetsDummy.get(), sizeof(WORD) * cLoops);
            fRet = pSerial->WriteBytes((BYTE*)rgOffsetsDummy.get(), sizeof(WORD) * cLoops);
        }
    }

    if (fRet)
    {
        // First the unmirrored ones:
        for (int i = 0; fRet && (i < cLoops); i++)
        {
            if (!_loops[i].mirror())
            {
                // Set the offset:
                // (Recalc this each time, since pSerial->pData can change)
                WORD *pOffsets = (WORD*)(pSerial->GetInternalPointer() + wOffsets);
                pOffsets[i] = ((WORD)(pSerial->GetPosition()));
                
                // Check if this is a mirrored loop, and if so, ignore it (write the same offset as
                // the one which mirrors it.
                _loops[i].write_loop(pSerial);
            }
        }

        // Then their mirrors:
        WORD *pOffsets = (WORD*)(pSerial->GetInternalPointer() + wOffsets);
        for (int i = 0; i < cLoops; i++)
        {
            const LoopInfo &fooDebug = _loops[i];
            if (_loops[i].mirror())
            {
                // Set the offset - use the same offset of the cel of which we are a mirror.
                ASSERT(_loops[i].mirror_of() < cLoops);
                pOffsets[i] = pOffsets[_loops[i].mirror_of()];
                ASSERT(pOffsets[i]);
                // That's it, we're done.
            }
        }
    }

    return fRet;    
}

//
// IResourceBase
//
HBITMAP ViewResource::GetBitmap(DWORD dwIndex, int cx, int cy) const
{
    return _GetBitmapI(dwIndex, cx, cy, true);
}
HBITMAP ViewResource::GetFastBitmap(DWORD dwIndex, int cx, int cy) const
{
    return _GetBitmapI(dwIndex, cx, cy, false);
}
void ViewResource::CopyBitmapData(DWORD dwIndex, BYTE *pData, CSize sizeDebug) const
{
    if (_ValidIndex(dwIndex))
    {
        _GetCel(dwIndex).copy_bitmap_data(pData, sizeDebug);
    }
}
CSize ViewResource::GetSize(DWORD dwIndex) const
{
    if (_ValidIndex(dwIndex))
    {
        return _GetCel(dwIndex).get_size();
    }
    return CSize(0);
}
BYTE ViewResource::GetTransparentColor(DWORD dwIndex) const
{
    if (_ValidIndex(dwIndex))
    {
        return _GetCel(dwIndex).transparent();
    }
    else
    {
        return 0;
    }
}
CPoint ViewResource::GetPlacement(DWORD dwIndex) const
{
    if (_ValidIndex(dwIndex))
    {
        return _GetCel(dwIndex).get_placement();
    }
    else
    {
        return CPoint(0);
    }
}

HBITMAP ViewResource::_GetBitmapI(DWORD dwIndex, int cx, int cy, BOOL fAllowZoom) const
{
    if (_ValidIndex(dwIndex))
    {
        return _GetCel(dwIndex).get_bitmap(cx, cy, fAllowZoom == TRUE);
    }
    else
    {
        return NULL;
    }
}

// this for existing ones.
HRESULT ViewResource::InitFromResource(const ResourceBlob *prd)
{
    ASSERT(prd->GetType() == RS_VIEW);
    ASSERT(_loops.empty());
    _iPackageNumber = prd->GetPackageHint();
    _iResourceNumber = prd->HasNumber() ? prd->GetNumber() : -1;

    sci::istream byteStream = prd->GetReadStream();
    WORD nLoops;
    WORD wMirrorMask = 0;
    byteStream >> nLoops;
    if (byteStream.good())
    {
        _loops.assign(nLoops, LoopInfo()); // Just empty ones for now
    }
    byteStream >> wMirrorMask;
    byteStream.skip(4); // Skip 4 bytes (function unknown)
    
    scoped_array<WORD> rgOffsets(new WORD[nLoops]);
    for (WORD i = 0; byteStream.good() && (i < nLoops); i++)
    {
        byteStream >> rgOffsets[i]; // Read offset.
        if (!ISMIRROR(i, wMirrorMask))
        {
            // Make a copy of the stream so we don't lose our position in this one.
            sci::istream streamLoop(byteStream);
            streamLoop.seekg(rgOffsets[i]);
            _loops[i].read(streamLoop);
        }
    }

    if (byteStream.good())
    {
        // Now that we got the data from all "originals", repeat again for mirrors.
        for (WORD iMirror = 0; iMirror < nLoops; iMirror++)
        {
            if (ISMIRROR(iMirror, wMirrorMask))
            {
                ASSERT(rgOffsets[iMirror]); // Ensure we read an offset.
                // Find out of which "original" it is a mirror.
                for (WORD iOrig = 0; iOrig < nLoops; iOrig++)
                {
                    if (!ISMIRROR(iOrig, wMirrorMask))
                    {
                        if (rgOffsets[iOrig] == rgOffsets[iMirror])
                        {
                            _loops[iMirror].mirror_from((BYTE)iOrig, _loops[iOrig]);
                            break;
                        }
                    }
                }
            }
        }
    }
    return byteStream.good() ? S_OK : E_FAIL;
}
// This for new ones
HRESULT ViewResource::InitNew()
{
    ASSERT(_loops.empty());
    // Make a view with one loop that has one cel.
    CelInfo celTemplate;
    // 16x16 filled with transparent colour
    celTemplate.ReallocBits(16, 16, false, true, true, celTemplate.transparent(), RasterBitmap::Normal);
    _loops.push_back(LoopInfo(celTemplate));
    return S_OK;
}

