// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ColorButton.h"

CExtBitmap g_bitmapHighlight;
CExtBitmap g_bitmapHighlightX;

// CColorPickerButton

IMPLEMENT_DYNAMIC(CColorPickerButton, CButton)
CColorPickerButton::CColorPickerButton()
{
    _pFactory = NULL;
    if (g_bitmapHighlight.IsEmpty())
    {
        g_bitmapHighlight.LoadBMP_Resource(MAKEINTRESOURCE(IDB_VPCHIGHLIGHT));
        g_bitmapHighlightX.LoadBMP_Resource(MAKEINTRESOURCE(IDB_VPCHIGHLIGHTX));
    }
}

CColorPickerButton::~CColorPickerButton()
{
}


BEGIN_MESSAGE_MAP(CColorPickerButton, CButton)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CColorPickerButton message handlers


void CColorPickerButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    __super::OnLButtonDown(nFlags, point);
    if (_pFactory)
    {
        RECT rc;
        GetWindowRect(&rc);
        _pFactory->InvokeDialog(GetDlgCtrlID(), &rc);
    }
    int x= 0;
}



void CColorPickerButton::SetColorAndState(EGACOLOR color, BOOL bOn)
{
    if ((_color.color1 != color.color1) || (_color.color2 != color.color2) || (_bOn != bOn))
    {
        _color = color; _bOn = bOn;
        Invalidate(FALSE);
    }
}

RGBQUAD _Desaturate(RGBQUAD color)
{
    RGBQUAD colorRet;
    colorRet.rgbBlue = (BYTE)((((WORD)color.rgbBlue) + 256) / 3);
    colorRet.rgbRed = (BYTE)((((WORD)color.rgbRed) + 256) / 3);
    colorRet.rgbGreen = (BYTE)((((WORD)color.rgbGreen) + 256) / 3);
    return colorRet;
}

RGBQUAD _Redden(RGBQUAD color)
{
    RGBQUAD colorRet;
    colorRet.rgbBlue = (BYTE)(((WORD)color.rgbBlue / 2));
    colorRet.rgbGreen = (BYTE)(((WORD)color.rgbGreen / 2));
    colorRet.rgbRed = (BYTE)(((WORD)color.rgbRed) * 3);
    if (colorRet.rgbRed < 200)
    {
        colorRet.rgbRed = 200;
    }
    return colorRet;
}

RGBQUAD _Combine(RGBQUAD color1, RGBQUAD color2)
{
    RGBQUAD colorRet;
    colorRet.rgbBlue = (BYTE)((((WORD)color1.rgbBlue) + ((WORD)color2.rgbBlue)) / 2);
    colorRet.rgbRed = (BYTE)((((WORD)color1.rgbRed) + ((WORD)color2.rgbRed)) / 2);
    colorRet.rgbGreen = (BYTE)((((WORD)color1.rgbGreen) + ((WORD)color2.rgbGreen)) / 2);
    return colorRet;
}

RGBQUAD _Darker(RGBQUAD color)
{
    RGBQUAD colorRet;
    colorRet.rgbBlue = (color.rgbBlue) / 2;
    colorRet.rgbRed = (color.rgbRed) / 2;
    colorRet.rgbGreen = (color.rgbGreen) / 2;
    return colorRet;
}

RGBQUAD _Lighter(RGBQUAD color)
{
    RGBQUAD colorRet;
    colorRet.rgbBlue = (((color.rgbBlue)) +  ((255 - color.rgbBlue) / 2));
    colorRet.rgbRed = (((color.rgbRed)) + ((255 - color.rgbRed) / 2));
    colorRet.rgbGreen = (((color.rgbGreen)) +  ((255 - color.rgbGreen) / 2));
    return colorRet;
}

COLORREF _GetTextColor(RGBQUAD color)
{
    DWORD dwAvg = ((DWORD)color.rgbBlue) + ((DWORD)color.rgbGreen) + ((DWORD)color.rgbRed);
    dwAvg /= 3;
    if (dwAvg < 200)
    {
        return RGB(220, 255, 220);
    }
    else
    {
        return RGB(110, 128, 110);
    }
}

BOOL CreateDCCompatiblePattern(RGBQUAD color1, RGBQUAD color2, CDC *pDC, CBitmap *pbm)
{
    RGBQUAD HatchBits[16] = { color1, color1, color2, color2, color1, color1, color2, color2, 
                              color2, color2, color1, color1, color2, color2, color1, color1 };
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biWidth = 4;
    bmi.bmiHeader.biHeight = 4;
    bmi.bmiHeader.biPlanes = 1;

    CDC dcMem;
    BOOL bRet = dcMem.CreateCompatibleDC(pDC);
    if (bRet)
    {
        bRet = pbm->CreateCompatibleBitmap(pDC, 4, 4);
        if (bRet)
        {
            HGDIOBJ hObj = dcMem.SelectObject(pbm);
            bRet = StretchDIBits((HDC)dcMem, 0, 0, 4, 4, 0, 0, 4, 4, HatchBits, &bmi, DIB_RGB_COLORS, SRCCOPY);
            dcMem.SelectObject(hObj);
        }        
    }
    return bRet;
}

void CColorPickerButton::_OnDraw(HDC hdc, RECT *prc, EGACOLOR color)
{
    CDC *pDC = CDC::FromHandle(hdc);
    if (pDC)
    {
        // Figure out the right color - do color 1 for now.
        RGBQUAD color1 = EGA_TO_RGBQUAD(color.color1);
        RGBQUAD color2 = EGA_TO_RGBQUAD(color.color2);

        CBitmap bm;
        if (CreateDCCompatiblePattern(color1, color2, pDC, &bm))
        {
            CBrush brushPat;
            if (brushPat.CreatePatternBrush(&bm))
            {
                CRect rcShrunk(prc->left + 1, prc->top + 1, prc->right - 1, prc->bottom - 1);
                pDC->FillRect(&rcShrunk, &brushPat);
            }
        }
       
        //int cx = RECTWIDTH(*prc) / 2;
        //int cy = RECTHEIGHT(*prc) / 2;
        //RECT rectOverlay = { prc->left + cx, prc->top + cy, prc->right, prc->bottom };
        int iModePrev = pDC->SetBkMode(TRANSPARENT);
        RGBQUAD colorCombine = _Combine(color1, color2);
        RGBQUAD colorTL = _Lighter(colorCombine);
        RGBQUAD colorBR = _Darker(colorCombine);

        // Draw an x maybe
        if (!_bOn)
        {
            /*
            RGBQUAD colorRedder = _Redden(colorCombine);
            CPen penX(PS_SOLID, 1, RGB_TO_COLORREF(colorRedder));
            HGDIOBJ hgdiObjOld = pDC->SelectObject(&penX);
            pDC->MoveTo(prc->left, prc->bottom);
            pDC->LineTo(prc->right, prc->top);
            pDC->MoveTo(prc->left, prc->top);
            pDC->LineTo(prc->right, prc->bottom);
            pDC->SelectObject(hgdiObjOld);*/
        }

        /*
        // Do highlights.
        CPen penTL(PS_SOLID, 2, RGB_TO_COLORREF(colorTL));
        HGDIOBJ hgdiObj = pDC->SelectObject(&penTL);
        pDC->MoveTo(prc->left + 1, prc->bottom - 1);
        pDC->LineTo(prc->left + 1, prc->top + 1);
        pDC->LineTo(prc->right - 1, prc->top + 1);
        CPen penBR(PS_SOLID, 2, RGB_TO_COLORREF(colorBR));
        pDC->SelectObject(&penBR);
        pDC->LineTo(prc->right -1 , prc->bottom - 1);
        pDC->LineTo(prc->left + 1, prc->bottom - 1);
        pDC->SelectObject(hgdiObj);*/

        // Draw some text.
        int iColorPrev = pDC->SetTextColor(_GetTextColor(colorCombine));
        CString text;
        GetWindowText(text);
        pDC->DrawText(text, prc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SetTextColor(iColorPrev);

        CRect rcPadding(0, 0, 0, 0);
        if (_bOn)
        {
            CRect rcSrc(CPoint(0, 0), g_bitmapHighlight.GetSize());
            g_bitmapHighlight.AlphaBlendSkinParts(hdc, *prc, rcSrc, rcPadding, /*__EXT_BMP_FLAG_PREMULTIPLIED_RGB_CHANNELS |*/ CExtBitmap::__EDM_STRETCH, true, true, 0xFF);
        }
        else
        {
            CRect rcSrc(CPoint(0, 0), g_bitmapHighlightX.GetSize());
            g_bitmapHighlightX.AlphaBlendSkinParts(hdc, *prc, rcSrc, rcPadding, /*__EXT_BMP_FLAG_PREMULTIPLIED_RGB_CHANNELS |*/ CExtBitmap::__EDM_STRETCH, true, true, 0xFF);
        }

        pDC->SetBkMode(iModePrev);
    }
}


// CColorPickerButton message handlers

void CColorPickerButton::DrawItem(LPDRAWITEMSTRUCT pdis)
{
    _OnDraw(pdis->hDC, &pdis->rcItem, _color);
}





//
// CBrushButton
//

IMPLEMENT_DYNAMIC(CBrushButton, CColorPickerButton)
CBrushButton::CBrushButton()
{
}

CBrushButton::~CBrushButton()
{
}

void CBrushButton::SetPenStyle(const PenStyle *pPenStyle)
{
    if (!IsPenStyleEqual(pPenStyle, &_penStyle))
    {
        _penStyle = *pPenStyle;
        Invalidate(FALSE);
    }
}


void CBrushButton::_OnDraw(HDC hdc, RECT *prc, EGACOLOR color)
{
    // Fill with the background color.
    CDC *pDC = CDC::FromHandle(hdc);
    if (pDC)
    {
        // Ask the pattern to draw itself
        RECT rcLocal = *prc;

        // Black border.
        pDC->FillSolidRect(rcLocal.left, rcLocal.top, RECTWIDTH(rcLocal), RECTHEIGHT(rcLocal), RGB(0, 0, 0));

        // Pen style is always black.
        color.color1 = 0x0;
        color.color2 = 0x0;

        InflateRect(&rcLocal, -1, -1); // Shink for the borders.

        int cMin = 16;

        SCIBitmapInfo bmi(cMin, cMin);
        scoped_array<BYTE> dataBrush(new BYTE[cMin * cMin]);
        // Fill with white.
        memset(dataBrush.get(), 0x0f, cMin * cMin);

        // Fake a PicData to draw into, with a visual brush.
        PicData data = { DRAW_ENABLE_VISUAL, dataBrush.get(), NULL, NULL, NULL };

        DrawPatternInRect(cMin, cMin, &data, 8, 8, color, 0, 0, DRAW_ENABLE_VISUAL, &_penStyle);

        int nOldMode = pDC->SetStretchBltMode(HALFTONE);
        StretchDIBits((HDC)*pDC, rcLocal.left, rcLocal.top, RECTWIDTH(rcLocal), RECTHEIGHT(rcLocal),
            0, 0, cMin, cMin, dataBrush.get(), &bmi, DIB_RGB_COLORS, SRCCOPY);                    
        pDC->SetStretchBltMode(nOldMode);
    }
}
