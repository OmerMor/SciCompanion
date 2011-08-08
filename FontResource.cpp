
#include "stdafx.h"
#include "FontResource.h"
#include "SCIPicEditor.h"

#define ROUND_UP_BYTE(cx) (((cx) + 7) / 8)

bool _IsLineEmpty(const BYTE *pBits, int cCount)
{
    bool fRet = true;
    while (cCount > 0)
    {
        if (*pBits != 0x0f) // White
        {
            fRet = false;
            break;
        }
        cCount--;
        pBits++;
    }
    return fRet;
}

CFontResource::CFontResource()
{
    _iResourceNumber = -1;
    _iPackageNumber = -1;
    _rgLetters.resize(128); // SCI0 fonts have 128 characters.
    _cChars = 0;
    _cyLine = 0;
}

HRESULT CFontResource::InitNew()
{
    HRESULT hr = S_OK;
    _cChars = (WORD)_rgLetters.size();
    CSize size(6, 8);
    for (DWORD i = 0; SUCCEEDED(hr) && (i < (DWORD)_rgLetters.size()); i++)
    {
        hr = SetSize(i, size, RasterBitmap::Normal) ? S_OK : E_OUTOFMEMORY;
    }
    _cyLine = 8;
    return hr;
}

BOOL CFontResource::_WriteLetter(sci::istream *pSerial, WORD iChar) const
{
    const LetterInfo &letter = _rgLetters[iChar];
    // The Font Resource format at
    // http://freesci.linuxgames.com/scihtml/x2345.html is incorrect.
    // Width comes before Height.
    BOOL fRet = pSerial->WriteByte(letter.bwidth());
    if (fRet)
    {
        fRet = pSerial->WriteByte(letter.bheight());
        if (fRet)
        {
            // This is how many bytes we store for each line (one byte per pixel)
            int cBytes = CX_ACTUAL(letter.width());
            int cx = (int)letter.width();

            // This is how many bytes we'll write out (one bit per pixel)
            int cBytesToWrite = ROUND_UP_BYTE(letter.bwidth());

            // Now go through each line
            for (int y = 0; fRet && (y < (int)letter.height()); y++)
            {
                // The resource is flipped vertically, so start reading from the bottom, while writing to the "top"
                const BYTE *pBitsLine = letter.get_bits() + CX_ACTUAL(cx) * ((int)letter.height() - y - 1);

                // Write a byte at a time.
                for (int i = 0; fRet && (i < cBytesToWrite); i++)
                {
                    BYTE bOut = 0;
                    int jEnd = 8 * (i + 1);
                    int iShift = 7;
                    for (int j = 8 * i; (j < jEnd) && (j < cx); j++, iShift--)
                    {
                        if (*(pBitsLine + j) == 0)
                        {
                            // If it's 0 (black), then consider it on.
                            bOut |= (1 << iShift);
                        }
                    }
                    // Write it.
                    fRet = pSerial->WriteByte(bOut);
                }
            }
        }
    }
    return fRet;
}


void CFontResource::_RenderLetter(CDC *pDC, LetterInfo *pLetter, TCHAR ch, BOOL fCalc, WORD *pwMaxCharHeight, int *piAbove, int *piBelow) const
{
    BOOL fRet = TRUE;
    BOOL fWritingFont = FALSE;
    CRect rect(0, 0, 0, 0);
    if (pDC->DrawText(&ch, 1, &rect, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE))
    {
        // REVIEW: this doesn't work so well for italic fonts.  DrawText "underreports" the width, and
        // so we don't give enough space to render the character.  However, if we add the extra space (say even
        // a pixel) the letters end up to far from each other.
        // We could offer a trimming function as some sort of solution.
        if ((rect.Width() <= 127) && (rect.Height() <= 127))
        {
            if (fCalc)
            {
                *pwMaxCharHeight = max(*pwMaxCharHeight, rect.Height());
            }

            // Make a bitmap.
            SCIBitmapInfo bmi(rect.Width(), rect.Height());    
            BYTE *pBits;
            HBITMAP hDib = CreateDIBSection((HDC)*pDC, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
            if (hDib)
            {
                // Set the whole bitmap to white.
                memset(pBits, 0x0F, CX_ACTUAL(rect.Width()) * rect.Height());
                HGDIOBJ hOldBmp = pDC->SelectObject(hDib);
                // Black is our 'on'.
                COLORREF colorTextOld = pDC->SetTextColor(RGB(0, 0, 0));
                COLORREF colorOld = pDC->SetBkColor(RGB(255, 255, 255));
                int iBkMode = pDC->SetBkMode(OPAQUE);
                if (pDC->DrawText(&ch, 1, &rect, DT_NOPREFIX | DT_SINGLELINE))
                {
                    // Now it's time to suck the pixels out.  The Font Resource format at
                    // http://freesci.linuxgames.com/scihtml/x2345.html is incorrect.
                    // Width comes before Height.
                    if (fCalc)
                    {
                        *piAbove = 127; // We'll work down from here.
                        *piBelow = 127;

                        // Count how many empty lines there are above and below.
                        int iAbove = 0;
                        for (int y = (rect.Height() - 1); y >= 0; y--)
                        {
                            BYTE *pLine = ((BYTE*)pBits) + y * CX_ACTUAL(rect.Width());
                            if (_IsLineEmpty(pLine, rect.Width()))
                            {
                                iAbove++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        int iBelow = 0;
                        for (int y = 0; y < rect.Height(); y++)
                        {
                            BYTE *pLine = ((BYTE*)pBits) + y * CX_ACTUAL(rect.Width());
                            if (_IsLineEmpty(pLine, rect.Width()))
                            {
                                iBelow++;
                            }
                            else
                            {
                                break;
                            }
                        }
                        //ASSERT(iAbove != 0); // REVIEW: was this important???
                        *piBelow = min(*piBelow, iBelow);
                        *piAbove = min(*piAbove, iAbove);
                    }
                    else
                    {
                        // Write the font.
                        // But first, adjust the rect for iAbove and iBelow.  Note, these are switched around, because
                        // the letter is printed upside down.
                        rect.top += *piBelow;
                        rect.bottom -= *piAbove;
                        // If it still has some height, write it (otherwise, we'll write an empty char later).
                        if (rect.Height() > 0)
                        {
                            fWritingFont = TRUE;
                            pLetter->set_size((WORD)rect.Width(), (WORD)rect.Height());
                            for (int y = (rect.bottom - 1); fRet && y >= rect.top; y--)
                            {
                                CopyMemory(pLetter->get_bits() + (y - rect.top) * CX_ACTUAL(pLetter->width()),
                                            pBits + y * CX_ACTUAL(pLetter->width()),
                                            CX_ACTUAL(pLetter->width()));
                            }
                        }
                    }
                }
                pDC->SetBkMode(iBkMode);
                pDC->SetBkColor(colorOld);
                pDC->SetTextColor(colorTextOld);
                pDC->SelectObject(hOldBmp);
                DeleteObject(hDib);
            }
        }
    }

    if (!fWritingFont && !fCalc)
    {
        // For some reason, this character could not be drawn (invisible, etc...).
        // In it's place, we'll have a 1x1 character.
        pLetter->create_degenerate(0);
    }
}

static const int CharTableOffset = 6;

BOOL CFontResource::Serialize(sci::istream *pSerial) const
{
    // First prepare the header.
    BOOL fRet = pSerial->WriteWord(0);
    if (fRet)
    {
        // The number of characters
        fRet = pSerial->WriteWord(_cChars);
        if (fRet)
        {
            // The height of each line
            fRet = pSerial->WriteWord(_cyLine);
        }
    }

    if (fRet)
    {
        WORD *rgwOffsets = new WORD[_cChars];
        if (rgwOffsets)
        {
            // Write some dumb data to take up space.
            fRet = pSerial->WriteBytes((BYTE*)rgwOffsets, _cChars * 2);
            if (fRet)
            {
                int iAbove = 127;
                int iBelow = 127;

                
                for (WORD i = 0; fRet && (i < _cChars); i++)
                {
                    rgwOffsets[i] = (WORD)(pSerial->GetPosition());
                    fRet = _WriteLetter(pSerial, i);
                }

                // Now copy the offset table directly into the sci::istream;
                if (fRet)
                {
                    CopyMemory(pSerial->GetInternalPointer() + CharTableOffset, rgwOffsets, sizeof(WORD) * _cChars);
                }
            }
            delete [] rgwOffsets;
        }
    }

    return fRet;
}


HRESULT CFontResource::InitFromResource(const ResourceBlob *prd)
{
    ASSERT(prd->GetType() == RS_FONT);
    _iPackageNumber = prd->GetPackageHint();
    _iResourceNumber = prd->HasNumber() ? prd->GetNumber() : -1;

    sci::istream byteStream = prd->GetReadStream();

    WORD wDummy;
    byteStream >> wDummy;   // The resource starts with a word (unknown use)
    byteStream >> _cChars;  // Number of chars (128)
    byteStream >> _cyLine;  // The line height
    // Some validation
    // TODO: report a status error 
    _cChars = min((WORD)_rgLetters.size(), _cChars);
    // 1 - 128 seems reasonable for line height
    _cyLine = max(1, _cyLine);
    _cyLine = min(128, _cyLine);

    HRESULT hr = E_FAIL;
    if (byteStream.good())
    {
        WORD i = 0;
        for (; i < _cChars; i++)
        {
            WORD wOffset;
            byteStream >> wOffset;
            sci::istream byteStreamLetter(byteStream);
            byteStreamLetter.seekg(wOffset);
            if (byteStreamLetter.good())
            {
                _ReadLetter(byteStreamLetter, _rgLetters[i]);
            }
        }
        hr = (i == _cChars) ? S_OK : E_FAIL;
    }
    return hr;
}

//
// Fills in LetterInfo*
//
void CFontResource::_ReadLetter(sci::istream &byteStream, LetterInfo &letter)
{
    DWORD dwOriginalPos = byteStream.tellg();

    BYTE cx;
    byteStream >> cx;
    BYTE cy;
    byteStream >> cy;
    if (byteStream.good())
    {
        // Allocate enough room for the font character
        // We'll use one byte per pixel, like we do for views, even though the 
        // font is stored as a 1-bit-per-pixel resource.
        letter.set_size(cx, cy);
        // Calcualte how many bytes to read from the resource, for each line:
        int cBytesToReadPerLine = (((int)letter.width()) + 7) / 8;

        // Now transfer from the resource into our datastructure.
        for (int y = 0; byteStream.good() && (y < (int)letter.height()); y++)
        {
            int cBitsRemainingThisLine = letter.width();

            // Start of the line:
            BYTE *pLineDest = letter.get_bits() + ((int)letter.height() - 1 - y) * CX_ACTUAL((int)letter.width());
            // Figure out how many bytes to read.  A byte holds up to 8 pixel values.

            for (int iByte = 0; byteStream.good() && (iByte < cBytesToReadPerLine); iByte++)
            {
                BYTE bValue;
                byteStream >> bValue;
                // Read a byte.  Now convert it into up to
                for (int iBit = 7; (iBit >= 0) && cBitsRemainingThisLine; iBit--, cBitsRemainingThisLine--)
                {
                    // Black (0x00) for on, and white (0x0f) for off
                    *pLineDest = ((bValue >> iBit) & 1) ? 0x00 : 0x0f;
                    pLineDest++;
                }
            }
        }
    }
}


HRESULT CFontResource::InitFromLOGFONT(const LOGFONT *pLogFont)
{
    HRESULT hr = E_FAIL;
    _cChars = (WORD)_rgLetters.size();
    WORD wMaxCharHeight = 0;
    CDC dc;
    if (dc.CreateCompatibleDC(NULL))
    {
        CFont font;
        if (font.CreateFontIndirect(pLogFont))
        {
            int iAboveMin = 127;
            int iBelowMin = 127;
            HGDIOBJ hFontOld = dc.SelectObject(&font);
            // Figure out how much space above and below the letters we can remove.
            for (int i = 0; i < _cChars; i++)
            {
                ASSERT((char)i != '0');
                int iAbove, iBelow;
                _RenderLetter(&dc, NULL, (char)i, TRUE, &wMaxCharHeight, &iAbove, &iBelow);
                iAboveMin = min(iAboveMin, iAbove);
                iBelowMin = min(iBelowMin, iBelow);
            }
            wMaxCharHeight -= iAboveMin + iBelowMin;
            // TODO: work out a better calculation here:
            _cyLine = wMaxCharHeight; 

            for (int i = 0; i < _cChars; i++)
            {
                _RenderLetter(&dc, &_rgLetters[i], (char)i, FALSE, &wMaxCharHeight, &iAboveMin, &iBelowMin);
            }

            hr = S_OK;
            dc.SelectObject(hFontOld);
        }
    }

    return hr;
}

HBITMAP CFontResource::GetBitmap(DWORD dwIndex, int cx, int cy) const
{
    return GetFastBitmap(dwIndex, cx, cy);
}

HBITMAP CFontResource::GetFastBitmap(DWORD dwIndex, int cx, int cy) const
{
    HBITMAP hbm = NULL;

    CDC dc;
    if (dc.CreateCompatibleDC(NULL))
    {
        int cxBmp = 0;
        int cyBmp = 0;
        int iZoom = 0;
        BYTE bTransparent = 0;
        const BYTE *pBits = NULL;
        if (dwIndex < _rgLetters.size())
        {
            pBits = _rgLetters[dwIndex].get_bits();
            cxBmp = _rgLetters[dwIndex].width();
            cyBmp = _rgLetters[dwIndex].height();
            // Figure out how much we can zoom.
            iZoom = min((cx / cxBmp), (cy / cyBmp));
            iZoom = max(1, iZoom);
            iZoom = min(4, iZoom);
        }

        SCIBitmapInfo bmi(cx, cy);
        BYTE *pBitsDest;
        hbm = CreateDIBSection((HDC)dc, &bmi, DIB_RGB_COLORS, (void**)&pBitsDest, NULL, 0);
        if (hbm)
        {
            // Fonts should have a white background in the empty space surrounding the actual letter,
            // so fill with white.
            memset(pBitsDest, 0x0f, CX_ACTUAL(cx) * cy);

            if (iZoom == 1)
            {
                // If zoom factor == 1, copy directly to the bmp
                int yOffset = cyBmp - min(cy, cyBmp); // yOffset -> if there's not enough room, copy the TOP of the view.
                for (int y = 0; y < min(cy, cyBmp); y++)
                {
                    // Copy a scan line.
                    CopyMemory(pBitsDest + y * CX_ACTUAL(cx), pBits + (y + yOffset) * CX_ACTUAL(cxBmp), min(cx, cxBmp));
                }
            }
            else
            {
                // Otherwise, copy a magnified view.
                int cyBmpZoom = iZoom * cyBmp;
                int cxBmpZoom = iZoom * cxBmp;
                for (int y = 0; y < min(cy, cyBmp); y++)
                {
                    // Copy iZoom scanlines.
                    const BYTE *pSrc = pBits + y * CX_ACTUAL(cxBmp);
                    for (int x = 0; x < min(cx, cxBmp); x++)
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

void CFontResource::CopyBitmapData(DWORD dwIndex, BYTE *pData, CSize sizeDebug) const
{
    if (dwIndex < (WORD)_rgLetters.size())
    {
        ASSERT(sizeDebug.cx == _rgLetters[dwIndex].width());
        ASSERT(sizeDebug.cy == _rgLetters[dwIndex].height());
        CopyMemory(pData, _rgLetters[dwIndex].get_bits(), _rgLetters[dwIndex].datasize());
    }
}

CSize CFontResource::GetSize(DWORD dwIndex) const
{
    CSize size(0);
    if (dwIndex < (WORD)_rgLetters.size())
    {
        size.cx = _rgLetters[dwIndex].width();
        size.cy = _rgLetters[dwIndex].height();
    }
    return size;
}

void CFontResource::SetBitmapData(DWORD dwIndex, const BYTE *pData)
{
    if (dwIndex < (DWORD)_cChars)
    {
        int cb = (int)_rgLetters[dwIndex].datasize();
        BYTE *pOurLetterData = _rgLetters[dwIndex].get_bits();
        CopyMemory(pOurLetterData, pData, cb);

        // Verify the colours.
        for (int i = 0; i < cb; i++)
        {
            switch(pOurLetterData[i])
            {
            case 0x00:
            case 0x0f:
                break;
            default:
                // If it's not one of those 3, make it white
                pOurLetterData[i] = 0x0f;
                break;
            }
        }
    }
}

void CFontResource::_ValidateSize(CSize *pSize)
{
    pSize->cx = min(pSize->cx, 127);
    pSize->cy = min(pSize->cy, 127);
    pSize->cx = max(pSize->cx, 1);
    pSize->cy = max(pSize->cy, 1);
}

BOOL CFontResource::SetSize(DWORD dwIndex, CSize size, RasterBitmap::RasterResizeFlags dwFlags)
{
    // width, height must be <= 127
    BOOL fRet = FALSE;
    if (dwIndex < (DWORD)_cChars)
    {
        _ValidateSize(&size);
        fRet = TRUE;
        _rgLetters[dwIndex].ReallocBits((WORD)size.cx, (WORD)size.cy, true, false, true, 0xf, dwFlags); // white
    }
    return fRet;

}

