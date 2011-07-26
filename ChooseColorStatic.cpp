// ChooseColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ChooseColorStatic.h"
#include "ChooseColorDialog.h"

// CChooseColorStatic

IMPLEMENT_DYNAMIC(CChooseColorStatic, CStatic)
CChooseColorStatic::CChooseColorStatic()
{
    _bHoverIndex = INVALID_COLORINDEX;
    _cRows = 0;
    _cColumns = 0;
    _fPrintIndex = FALSE;
    _fShowSelection = FALSE;
    _bSelectedColorIndex = 0;
    _bAuxSelectedColorIndex = 0;
    _fShowAuxSel = FALSE;
    _pPalette = NULL;
    _fShowIndices = FALSE;
    _fSelectionNumbers = FALSE;
}

CChooseColorStatic::~CChooseColorStatic()
{
}


BEGIN_MESSAGE_MAP(CChooseColorStatic, CStatic)
    ON_WM_DRAWITEM_REFLECT()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CChooseColorStatic::SetSelection(BYTE bIndex)
{
    _bSelectedColorIndex = bIndex;
    if (m_hWnd)
    {
        Invalidate(FALSE);
    }
}

void CChooseColorStatic::SetAuxSelection(BYTE bIndex)
{
    _bAuxSelectedColorIndex = bIndex;
    _fShowAuxSel = TRUE;
    if (m_hWnd)
    {
        Invalidate(FALSE);
    }
}

void CChooseColorStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    RECT rcClient;
    GetClientRect(&rcClient);
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    if (pDC)
    {

        CDC dcMem;
        if (dcMem.CreateCompatibleDC(pDC))
        {
            CBitmap bitmap;
            bitmap.CreateCompatibleBitmap(pDC, RECTWIDTH(rcClient), RECTHEIGHT(rcClient));
            if (dcMem.SelectObject(&bitmap))
            {
                // Now hand this memory DC off to the real drawing function.
                _DrawItem(&dcMem, RECTWIDTH(rcClient), RECTHEIGHT(rcClient));

                // Now blt back
                // Now we want to copy it back to the real dc.
                pDC->BitBlt(0, 0, RECTWIDTH(rcClient), RECTHEIGHT(rcClient), &dcMem, 0, 0, SRCCOPY);
            }
        }
    }
}


// CChooseColorStatic message handlers

void CChooseColorStatic::_DrawItem(CDC *pDC, int cx, int cy)
{
    // Call CExtLabel's background, which will paint the gradient (or whatever) background
    OnEraseBackground(*pDC, CRect(0, 0, cx, cy));
    //pDC->FillSolidRect(0, 0, cx, cy, g_PaintManager->GetColor(COLOR_3DFACE));

    if (_pPalette)
    {
        _DrawSelection(pDC);

        // Divide things up into _cRows rows and _cColumms columns.
        int iPaletteIndex = 0;

        for (iPaletteIndex = 0; iPaletteIndex < (_cRows * _cColumns); iPaletteIndex++)
        {
            ASSERT(iPaletteIndex < 256); // Since we cast to a BYTE
            // We use an int for this loop, because there could be up to 256 colors in this static.

            RECT rectSquare;
            _MapIndexToRect((BYTE)iPaletteIndex, &rectSquare);

            // Is this the current selected one?  Highlight it.

            // Figure out the right color - do color 1 for now.
            EGACOLOR egaColor = _pPalette[iPaletteIndex];
            RGBQUAD color1 = EGA_TO_RGBQUAD(egaColor.color1);
            RGBQUAD color2 = EGA_TO_RGBQUAD(egaColor.color2);

            CBitmap bm;
            if (CreateDCCompatiblePattern(color1, color2, pDC, &bm))
            {
                CBrush brushPat;
                if (brushPat.CreatePatternBrush(&bm))
                {
                    pDC->FillRect(&rectSquare, &brushPat);
                }
            }

            _DrawIndex(pDC, (BYTE)iPaletteIndex);

            if (_fSelectionNumbers && (!_fShowAuxSel || (_bSelectedColorIndex != _bAuxSelectedColorIndex)))
            {
                // Only show selection numbers when we're not showing auxiliary selection, or when we
                // are, and the two selections are different.
                _DrawTextAtIndex(pDC, _bSelectedColorIndex, TEXT("A"));
                if (_fShowAuxSel)
                {
                    _DrawTextAtIndex(pDC, _bAuxSelectedColorIndex, TEXT("B"));
                }
            }
        }

        _DrawHover(pDC);

    }
}

void CChooseColorStatic::_DrawHover(CDC *pDC)
{
    // Draw the hovered box.
    if (_bHoverIndex != INVALID_COLORINDEX)
    {
        RECT rectHover;
        _MapIndexToRect(_bHoverIndex, &rectHover);

        // Draw a box
        pDC->DrawFocusRect(&rectHover);
        // make it a little bigger.
        InflateRect(&rectHover, 1, 1);
        pDC->DrawFocusRect(&rectHover);
    }
}

void CChooseColorStatic::_DrawSelection(CDC *pDC)
{
    if (_fShowSelection)
    {
        for (int i = 0; i < (_fShowAuxSel ? 2 : 1); i++)
        {
            BYTE bSelectedIndex = (i == 0) ? _bSelectedColorIndex : _bAuxSelectedColorIndex;
            RECT rectSquare;
            _MapIndexToRect(bSelectedIndex, &rectSquare);

            RECT rectHighlight = rectSquare;
            InflateRect(&rectHighlight, 2, 2);
            CBrush brushSolid;
            brushSolid.CreateSolidBrush(RGB(0, 0, 0));
            pDC->FillRect(&rectHighlight, &brushSolid);

            InflateRect(&rectHighlight, -1, -1);

            CBrush brushSolidWhite;
            brushSolidWhite.CreateSolidBrush(RGB(255, 255, 255));
            pDC->FillRect(&rectHighlight, &brushSolidWhite);
        }
        
    }
}


COLORREF _GetGoodTextColor(RGBQUAD color)
{
    DWORD dwAvg = ((DWORD)color.rgbBlue) + ((DWORD)color.rgbGreen) + ((DWORD)color.rgbRed);
    dwAvg /= 3;
    if (dwAvg < 170)
    {
        return RGB(220, 255, 220);
    }
    else
    {
        return RGB(110, 128, 110);
    }
}

//
// Draw a number in this square
//
void CChooseColorStatic::_DrawIndex(CDC *pDC, BYTE bIndex)
{
    if (_fShowIndices)
    {
        TCHAR szBuf[3];
        StringCchPrintf(szBuf, ARRAYSIZE(szBuf), TEXT("%02d"), bIndex);
        _DrawTextAtIndex(pDC, bIndex, szBuf);
    }
}

void CChooseColorStatic::_DrawTextAtIndex(CDC *pDC, BYTE bIndex, PCTSTR psz)
{
    RECT rectSquare;
    _MapIndexToRect(bIndex, &rectSquare);

    int iBkMode = pDC->SetBkMode(TRANSPARENT);

    // Note: we're just using color1 for sampling here.  Currently the "drawindex"
    // feature is only used for non-dithered colours (e.g. priority and control)
    COLORREF colorOld = pDC->SetTextColor(_GetGoodTextColor(EGA_TO_RGBQUAD(_pPalette[bIndex].color1)));

    pDC->DrawText(psz, -1, &rectSquare, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    pDC->SetBkMode(iBkMode);
    pDC->SetTextColor(colorOld);
}


// Space around each square.
#define PALETTES_INSET 2


//
// Returns a number between 0 and ((_cRows * _cColumns) - 1), or INVALID_COLORINDEX
// if the point did not lie on a square.
//
BYTE CChooseColorStatic::_MapPointToIndex(CPoint point)
{
    BYTE bRet = INVALID_COLORINDEX;

    RECT rc;
    GetClientRect(&rc); // Adjust a bit.
    InflateRect(&rc, -PALETTES_INSET, -PALETTES_INSET);

    if ((_cColumns > 0) && (_cRows > 0))
    {
        int cxSquare = RECTWIDTH(rc) / _cColumns;
        int cySquare = RECTHEIGHT(rc) / _cRows;

        int x = point.x - PALETTES_INSET;
        int y = point.y - PALETTES_INSET;
        int iColumn = x / cxSquare;
        int iRow = y / cySquare;
        if ((iColumn < _cColumns) && (iRow < _cRows))
        {
            bRet = (BYTE)((iRow * _cColumns) + iColumn);
        }
    }
    return bRet;
}

void CChooseColorStatic::_MapRowColToRect(int iRow, int iColumn, RECT *prcOut)
{
    RECT rc;
    GetClientRect(&rc);
    InflateRect(&rc, -PALETTES_INSET, -PALETTES_INSET); // Adjust a bit.
    int cxSquare = RECTWIDTH(rc) / _cColumns;
    int cySquare = RECTHEIGHT(rc) / _cRows;
    prcOut->left = iColumn * cxSquare + PALETTES_INSET;
    prcOut->top = iRow * cySquare + PALETTES_INSET;
    prcOut->right = (iColumn + 1) * cxSquare;
    prcOut->bottom = (iRow + 1) * cySquare;
}


void CChooseColorStatic::_MapIndexToRect(BYTE bIndex, RECT *prc)
{
    if (bIndex < (_cColumns * _cRows))
    {
        int iRow = bIndex / _cColumns;
        int iColumn = bIndex % _cColumns;
        _MapRowColToRect(iRow, iColumn, prc);
    }
    else
    {
        // make it zero
        ZeroMemory(prc, sizeof(*prc));
    }
}

void CChooseColorStatic::_OnButtonDown(UINT nFlags, CPoint point, BOOL fLeft)
{
    int x =0;
    if (_pCallback)
    {
        BYTE bColorIndex = _MapPointToIndex(point);
        if (bColorIndex != INVALID_COLORINDEX)
        {
            _pCallback->OnColorClick(bColorIndex, this->GetDlgCtrlID(), fLeft);
        }
    }
}


void CChooseColorStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
    _OnButtonDown(nFlags, point, TRUE);
}



void CChooseColorStatic::OnRButtonDown(UINT nFlags, CPoint point)
{
    _OnButtonDown(nFlags, point, FALSE);
}


void CChooseColorStatic::OnMouseMove(UINT nFlags, CPoint point)
{
    BOOL bHoverIndex = _MapPointToIndex(point);
    if (bHoverIndex != _bHoverIndex)
    {
        _bHoverIndex = bHoverIndex;
        _pCallback->OnColorHover(_bHoverIndex);
        RedrawWindow();
        //Invalidate(FALSE);
    }
}


void CChooseColorStatic::OnPaletteUpdated()
{
    // Someone changed the palette from under us.
    Invalidate(FALSE);
}




//
// CChooseBrushStatic
//
IMPLEMENT_DYNAMIC(CChooseBrushStatic, CChooseColorStatic)
CChooseBrushStatic::CChooseBrushStatic()
{
    _bHoverIndex = INVALID_COLORINDEX;
    _cRows = 0;
    _cColumns = 0;
    _fPrintIndex = FALSE;
    _fShowSelection = FALSE;
    _bSelectedColorIndex = 0;
    _pPalette = NULL;
}

CChooseBrushStatic::~CChooseBrushStatic()
{
}

BEGIN_MESSAGE_MAP(CChooseBrushStatic, CChooseColorStatic)
    ON_WM_DRAWITEM_REFLECT()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


#define PATTERN_PAINT_SIZE 16
#define PATTERN_BMP_SIZE (PATTERN_PAINT_SIZE * PATTERN_PAINT_SIZE)

RGBQUAD RGBQUADFromCOLORREF(COLORREF color)
{
    RGBQUAD quad;
    quad.rgbBlue = GetBValue(color);
    quad.rgbRed = GetRValue(color);
    quad.rgbGreen = GetGValue(color);
    quad.rgbReserved = 0;
    return quad;
}

void CChooseBrushStatic::_DrawItem(CDC *pDC, int cx, int cy)
{
    // Fill in the background
    pDC->FillSolidRect(0, 0, cx, cy, g_PaintManager->GetColor(COLOR_3DFACE));

    _DrawSelection(pDC);

    // Create a bitmap large enough for one brush.
    EGACOLOR color = { 0x00, 0x00 }; // Black

    BYTE dataBrush[PATTERN_BMP_SIZE];
    // Fake a PicData to draw into, with a visual brush.
    PicData data = { DRAW_ENABLE_VISUAL, (BYTE*)&dataBrush, NULL, NULL, NULL };

    for (BYTE bPaletteIndex = 0; bPaletteIndex < (_cRows * _cColumns); bPaletteIndex++)
    {
        // Fill the bitmap with white each time.
        memset(&dataBrush, 0x0f, sizeof(dataBrush)); // Fill with white

        // Figure out what we want to draw (growing larger from left to right)
        // solid circles
        // pattern circles
        // solid squares
        // pattern squares

        PenStyle penStyle;
        PatternInfoFromIndex(bPaletteIndex, &penStyle);

        // Ask the pattern to draw itself
        
        DrawPatternInRect(PATTERN_PAINT_SIZE, PATTERN_PAINT_SIZE, &data, 8, 8, color, 0, 0, DRAW_ENABLE_VISUAL, &penStyle);

        // Now draw into the DC.
        RECT rectSquare;
        _MapIndexToRect(bPaletteIndex, &rectSquare);
        
        SCIBitmapInfo bmi(PATTERN_PAINT_SIZE, PATTERN_PAINT_SIZE);
        // Adjust the colors a little for the current scheme...
        bmi._colors[0xe] = RGBQUADFromCOLORREF(g_PaintManager->GetColor(COLOR_3DFACE));
        bmi._colors[-1] = RGBQUADFromCOLORREF(g_PaintManager->GetColor(COLOR_BTNTEXT));
        StretchDIBits((HDC)*pDC, rectSquare.left, rectSquare.top, RECTWIDTH(rectSquare), RECTHEIGHT(rectSquare),
            0, 0, PATTERN_PAINT_SIZE, PATTERN_PAINT_SIZE, &dataBrush, &bmi, DIB_RGB_COLORS, SRCCOPY);                    
    }

    _DrawHover(pDC);
}

