
#include "stdafx.h"
#include "CursorResource.h"
#include "SCIPicEditor.h"


// The spec listed here is wrong as of 2006/4/23:
// http://freesci.linuxgames.com/scihtml/x2213.html
//
// Here is the way it actually works: (left is "mask", right is "black/white")
// 10 Transparent 
// 01 White 
// 00 Black
// 11 White 



CCursorResource::CCursorResource()
{
    _iResourceNumber = -1;
    _iPackageNumber = -1;

    FillMemory(_rgData, sizeof(_rgData), 0x03); // Transparent
    _xPlacement = 0;
    _yPlacement = 0;
}

BOOL CCursorResource::Serialize(sci::istream *pSerial) const
{
    BOOL fRet = pSerial->WriteWord(0);
    if (fRet)
    {
        // The hot spot
        // REVIEW: some cursors have 480 and 320 in the first two spots.  Do we need to preserve that?
        fRet = pSerial->WriteWord(_yPlacement == 8 ? 0x00ff : 0x0000);
    }

    if (fRet)
    {
        WORD rgwMask[16];
        ZeroMemory(rgwMask, sizeof(rgwMask));
        WORD rgwWhiteOrBlack[16];
        ZeroMemory(rgwWhiteOrBlack, sizeof(rgwWhiteOrBlack));
        for (int iRow = 0; iRow < 16; iRow++)
        {
            for (int iCol = 0; iCol < 16; iCol++)
            {
                int iIndex = 16 * (15 - iRow) + iCol;
                switch(_rgData[iIndex])
                {
                case 0x0f: // White
                    rgwWhiteOrBlack[iRow] |= (1 << (15 - iCol));
                    break;

                case 0x00: // Black
                    // Nothing to set, it's already 0
                    break;

                default:   // Transparent
                    ASSERT(_rgData[iIndex] == CursorTransparent);
                    // Set the bit to indicate it's transparrent
                    rgwMask[iRow] |= (1 << (15 - iCol));
                    break;
                }
            }
        }
        fRet = pSerial->WriteBytes((BYTE*)rgwMask, sizeof(rgwMask));
        if (fRet)
        {
            fRet = pSerial->WriteBytes((BYTE*)rgwWhiteOrBlack, sizeof(rgwWhiteOrBlack));
        }
    }
    return fRet;
}


HRESULT CCursorResource::InitFromResource(const ResourceBlob *prd)
{
    _iPackageNumber = prd->GetPackageHint();
    _iResourceNumber = prd->HasNumber() ? prd->GetNumber() : -1;

    sci::istream byteStream = prd->GetReadStream();
    WORD wxHot;
    WORD wyHot;
    byteStream >> wxHot;
    byteStream >> wyHot;
    // Only two choices of hot spot in SCI0, based on the upper byte of 'y'
    _xPlacement = (0x00ff & wyHot) ? 8 : 0;
    _yPlacement = _xPlacement;

    // Now read the masks
    for (int iRow = 0; byteStream.good() && (iRow < 16); iRow++)
    {
        WORD w16bits;
        byteStream >> w16bits;
        for (int i = 0; i < 16; i++)
        {
            int iIndex = 16 * (15 - iRow) + i;
            // Just set as an indicator, 0x00 or 0xff
            _rgData[iIndex] = (w16bits & (0x1 << (15 - i))) ? 0xff : 0x00;
        }
    }

    // Now the white/black values
    for (int iRow = 0; byteStream.good() && (iRow < 16); iRow++)
    {
        WORD w16bits;
        byteStream >> w16bits;
        for (int i = 0; i < 16; i++)
        {
            int iIndex = 16 * (15 - iRow) + i;
            BOOL bSet = (w16bits & (0x1 << (15 - i))) != 0;
            if (_rgData[iIndex] == 0x00)
            {
                _rgData[iIndex] = bSet ? 0x0f : 0x00;
            }
            else
            {
                _rgData[iIndex] = bSet ? 0x0f : CursorTransparent;
            }
        }
    }
    return S_OK;
}

HBITMAP CCursorResource::GetBitmap(DWORD dwIndex, int cx, int cy) const
{
    return GetFastBitmap(dwIndex, cx, cy);
}

HBITMAP CCursorResource::GetFastBitmap(DWORD dwIndex, int cx, int cy) const
{
    HBITMAP hbm = NULL;
    CDC dc;
    if (dc.CreateCompatibleDC(NULL))
    {
        int cxBmp = 16;
        int cyBmp = 16;
        int iZoom = 0;
        BYTE bTransparent = 0;
        const BYTE *pBits = _rgData;
        // Figure out how much we can zoom.
        iZoom = min((cx / cxBmp), (cy / cyBmp));
        iZoom = max(1, iZoom);
        iZoom = min(4, iZoom);

        // REVIEW : this code is similar to font resource or view resource.... maybe we can combine
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
                int yOffset = cyBmp - min(cy, cyBmp); // yOffset -> if there's not enough room, copy the TOP of the resource.
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

void CCursorResource::CopyBitmapData(DWORD dwIndex, BYTE *pData, CSize sizeDebug) const
{
    ASSERT(sizeDebug.cx == 16);
    ASSERT(sizeDebug.cy == 16);
    CopyMemory(pData, _rgData, sizeof(_rgData));
}

CSize CCursorResource::GetSize(DWORD dwIndex) const
{
    return CSize(16, 16);
}


void CCursorResource::SetBitmapData(DWORD dwIndex, const BYTE *pData)
{
    CopyMemory(_rgData, pData, sizeof(_rgData));
    // Verify the colours.
    for (int i = 0; i < ARRAYSIZE(_rgData); i++)
    {
        switch(_rgData[i])
        {
        case 0x00:
        case 0x0f:
        case CursorTransparent:
            break;
        default:
            // If it's not one of those 3, make it transparent (this allows invert colours to work, for example)
            _rgData[i] = CursorTransparent;
            break;
        }
    }
}


BOOL CCursorResource::SetSize(DWORD dwIndex, CSize size, RasterBitmap::RasterResizeFlags dwFlags)
{
    // This doesn't affect us.
    return FALSE;
}

void CCursorResource::SetPlacement(DWORD dwIndex, CPoint pt)
{
    // For SCI0, hotspot can only be at 0, 0 or 8, 8
    if ((pt.x < 4) && (pt.y < 4))
    {
        _xPlacement = 0;
    }
    else
    {
        _xPlacement = 8;
    }
    _yPlacement = _xPlacement;
}

            

