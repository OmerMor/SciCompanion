// AnimateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "AnimateDialog.h"
#include "ViewResource.h"

// CAnimateDialog dialog

CAnimateDialog::CAnimateDialog(CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(CAnimateDialog::IDD, pParent)
{
    _pView = NULL;
    _fDoubleBuffer = FALSE;
    _pbitmapDoubleBuf = NULL;
    _nCel = 0;

    _rectDialogSize.SetRectEmpty();
    _fInitialized = FALSE;
    _iZoom = 1;
    _ptOrigin = CPoint(0, 0);
    _rectPlacements.SetRectEmpty();
    _sizeWeDrawIn.SetSize(0, 0);
    _fAnimating = TRUE;
}

CAnimateDialog::~CAnimateDialog()
{
    if (_pView)
    {
        delete _pView;
    }

    if (_pbitmapDoubleBuf)
    {
        delete _pbitmapDoubleBuf;
    }
}

//
// Call this after changing the size of the animate control.
//
CSize CAnimateDialog::_RecalcSizeNeeded()
{
    CSize sizeMax(0, 0);
    if (_pView)
    {
        int cCels = _pView->GetCelCount(_nLoop);
        CRect rectPlacements(1000, 1000, -1000, -1000);
        for (int i = 0; i < cCels; i++)
        {
            CPoint point = _pView->GetPlacement(MAKE_INDEX(_nLoop, i));
            rectPlacements.left = min(rectPlacements.left, point.x);
            rectPlacements.right  = max(rectPlacements.right, point.x);
            rectPlacements.top = min(rectPlacements.top, point.y);
            rectPlacements.bottom = max(rectPlacements.bottom, point.y);

            CSize size = _pView->GetSize(MAKE_INDEX(_nLoop, i));
            sizeMax.cx = max(sizeMax.cx, size.cx);
            sizeMax.cy = max(sizeMax.cy, size.cy);
        }

        _rectPlacements = rectPlacements;
        //_size

        // Increase our "max rect" by all the placement possibilities.
        sizeMax.cx += rectPlacements.Width();
        sizeMax.cy += rectPlacements.Height();

        // Try to fit the thing intelligently in our space.
        _iZoom = min(_sizeAnimate.cx / sizeMax.cx, _sizeAnimate.cy / sizeMax.cy);
        _iZoom = max(_iZoom, 1);
    }
    sizeMax.cx *= _iZoom;
    sizeMax.cy *= _iZoom;
    return sizeMax;
}

//
// Called once at the beginning (or later, if the view data changes)
//
void CAnimateDialog::_AutoSize()
{
    if (_pView)
    {
        CSize sizeMax = _RecalcSizeNeeded();

        CRect rect;
        m_wndAnimate.GetClientRect(&rect);
        int dx = sizeMax.cx - rect.Width();
        int dy = sizeMax.cy - rect.Height();

        if ((dx > 0) || (dy > 0))
        {
            CRect rectDialog;
            GetClientRect(&rectDialog);
            if (dx > 0)
            {
                rectDialog.right += dx;
            }
            if (dy > 0)
            {
                rectDialog.left += dy;
            }
            MoveWindow(&rectDialog);
        }
    }    
}

void CAnimateDialog::SetView(const ViewResource *pView)
{
    _pView = static_cast<ViewResource*>(pView->Clone());
}

void CAnimateDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_ANIMATE, m_wndAnimate);
    DDX_Control(pDX, IDC_SLIDER, m_wndSlider);
    DDX_Control(pDX, IDC_BUTTONPLAY, m_wndButton);
    CRect rectOrig;
    m_wndAnimate.GetClientRect(&rectOrig);
    _sizeAnimate.cx = rectOrig.Width();
    _sizeAnimate.cy = rectOrig.Height();
    _sizeWeDrawIn = _RecalcSizeNeeded();

    GetClientRect(&_rectDialogSize);

    _AutoSize();

    SetWindowText(_strTitle);
    SetTimer(ANIMATE_TIMER, 200, NULL);

    _fInitialized = TRUE;

    m_wndSlider.SetRange(0, _pView->GetCelCount(_nLoop) - 1);
    _UpdateButton();

    DDX_Control(pDX, IDOK, m_wndOK);
}

void CAnimateDialog::_UpdateButton()
{
    // Update command ui doesn't work here, so set the text directly.
    m_wndButton.SetWindowText(_fAnimating ? TEXT("Pause") : TEXT("Play"));
}

void CAnimateDialog::OnPlay()
{
    _fAnimating = !_fAnimating;
    _UpdateButton();
}


BEGIN_MESSAGE_MAP(CAnimateDialog, CExtResizableDialog)
    ON_WM_SIZE()
    ON_WM_DRAWITEM()
    ON_WM_TIMER()
    ON_WM_HSCROLL()
    ON_COMMAND(IDC_BUTTONPLAY, OnPlay)
END_MESSAGE_MAP()

void CAnimateDialog::OnSize(UINT nType, int cx, int cy)
{
    CRect rectDialogSize;
    GetClientRect(&rectDialogSize);

    if (_fInitialized)
    {
        int dx = rectDialogSize.Width() - _rectDialogSize.Width();
        int dy = rectDialogSize.Height() - _rectDialogSize.Height();

        if (dx || dy)
        {
            CRect rectAnimateScreen;
            m_wndAnimate.GetWindowRect(&rectAnimateScreen);
            ScreenToClient(&rectAnimateScreen);
            rectAnimateScreen.right = rectAnimateScreen.left + rectAnimateScreen.Width() + dx;
            rectAnimateScreen.bottom = rectAnimateScreen.top + rectAnimateScreen.Height() + dy;
            m_wndAnimate.MoveWindow(&rectAnimateScreen, TRUE);

            _sizeAnimate.cx = rectAnimateScreen.Width();
            _sizeAnimate.cy = rectAnimateScreen.Height();

            _sizeWeDrawIn = _RecalcSizeNeeded();

            int rgid[] = { IDOK, IDC_BUTTONPLAY };
            for (int i = 0; i < ARRAYSIZE(rgid); i++)
            {
                CWnd *pOk = GetDlgItem(rgid[i]);
                if (pOk)
                {
                    CRect rectOkScreen;
                    pOk->GetWindowRect(&rectOkScreen);
                    ScreenToClient(&rectOkScreen);
                    rectOkScreen.OffsetRect(dx, dy);
                    pOk->MoveWindow(&rectOkScreen, TRUE);
                }
            }

            CWnd *pSlider = GetDlgItem(IDC_SLIDER);
            if (pSlider)
            {
                CRect rectScreen;
                pSlider->GetWindowRect(&rectScreen);
                ScreenToClient(&rectScreen);
                rectScreen.OffsetRect(0, dy);
                pSlider->SetWindowPos(NULL, rectScreen.left, rectScreen.top, rectScreen.Width() + dx, rectScreen.Height(), SWP_NOZORDER);
            }

            m_wndAnimate.Invalidate(FALSE);
        }
    }
    _rectDialogSize = rectDialogSize;

    __super::OnSize(nType, cx, cy);
}

void CAnimateDialog::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == ANIMATE_TIMER)
    {
        if (_pView && _fAnimating)
        {
            int cCels = _pView->GetCelCount(_nLoop);
            _nCel++;
            _nCel %= cCels;
            m_wndSlider.SetPos(_nCel);
            m_wndAnimate.Invalidate(FALSE);
        }
    }
    else
    {
        __super::OnTimer(nIDEvent);
    }
}


void CAnimateDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pWnd)
{
    switch (nSBCode)
    {
    case SB_THUMBTRACK:
        _nCel = nPos;
        _fAnimating = FALSE;
        _UpdateButton();
        m_wndAnimate.Invalidate(FALSE);
        break;

    case SB_LEFT:
    case SB_ENDSCROLL:
    case SB_LINELEFT:
    case SB_LINERIGHT:
    case SB_PAGELEFT:
    case SB_PAGERIGHT:
    case SB_RIGHT:
        _nCel = m_wndSlider.GetPos();
        _fAnimating = FALSE;
        _UpdateButton();
        m_wndAnimate.Invalidate(FALSE);
        break;

    case SB_THUMBPOSITION:
        // We update constantly, so no need to do anything here.
        break;
    }
}


//
// REVIEW: we're still off by one here, in some cases (due to placement)
//
void CAnimateDialog::_OnDraw(CDC *pDC, LPRECT prc)
{
    // First fill with the background color.
    BYTE bColor = _pView->GetTransparentColor(MAKE_INDEX(_nLoop, _nCel));
    //COLORREF color = EGA_TO_COLORREF(15 - bColor);
    COLORREF color = RGB(255, 192, 128);
    CBrush brush;
    brush.CreateSolidBrush(color);
    pDC->FillRect(prc, &brush);


    if (_pView)
    {
        DWORD dwIndex = MAKE_INDEX(_nLoop, _nCel);
        CSize size = _pView->GetSize(dwIndex);
        size_t cbViewData = CX_ACTUAL(size.cx) * size.cy;
        if (cbViewData > 0)
        {
            CPoint ptPlacement = _pView->GetPlacement(dwIndex);

            scoped_array<BYTE> viewData(new BYTE[cbViewData]);
            _pView->CopyBitmapData(dwIndex, viewData.get(), size);

            SCIBitmapInfo bmi(size.cx, size.cy);
            CSize sizeDest(size.cx * _iZoom, size.cy * _iZoom);

            // Put view at the bottom and centered.
            CPoint pointAt((RECTWIDTH(*prc) - _sizeWeDrawIn.cx) / 2, RECTHEIGHT(*prc) - _sizeWeDrawIn.cy);

            CBrush brush;
            brush.CreateSolidBrush(RGB(255, 255, 255));
            CRect rectFoo(pointAt.x, pointAt.y, pointAt.x + _sizeWeDrawIn.cx, pointAt.y + _sizeWeDrawIn.cy);
            ::FillRect((HDC)*pDC, &rectFoo, brush);

            // Now look at the placement we got for this item, and consider it against
            // the rect of *all* placements.  The above x and y is good for placement 0,0
            CPoint pointOffset = ptPlacement - _rectPlacements.TopLeft();
            pointOffset.x *= _iZoom;
            pointOffset.y *= _iZoom;
            pointAt.Offset(pointOffset);

            // Adjust it a little more, since each frame is of a different height.  We need to
            // shove it to the bottom of where the highest one would paint.
            int cyHighestFrame = _sizeWeDrawIn.cy - _rectPlacements.Height() * _iZoom;
            ASSERT(cyHighestFrame >= sizeDest.cy);
            int dy = cyHighestFrame - sizeDest.cy;
            pointAt.y += dy;

            int cxWidestFrame = _sizeWeDrawIn.cx - _rectPlacements.Width() * _iZoom;
            ASSERT(cxWidestFrame >= sizeDest.cx);
            int dx = (cxWidestFrame - sizeDest.cx) / 2; // since x is centered
            pointAt.x += dx;

            StretchDIBits((HDC)*pDC, pointAt.x, pointAt.y, sizeDest.cx, sizeDest.cy, 0, 0, size.cx, size.cy, viewData.get(), &bmi, DIB_RGB_COLORS, SRCCOPY);
        }
    }
}

void CAnimateDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    if (nIDCtl == IDC_ANIMATE)
    {
        LPRECT prc = &lpDrawItemStruct->rcItem;
        CDC dc;
        dc.Attach(lpDrawItemStruct->hDC);

        _GenerateDoubleBuffer(&dc, prc);

        CDC dcMem;
        if (dcMem.CreateCompatibleDC(&dc))
        {
            if (_pbitmapDoubleBuf)
            {
                HGDIOBJ hOldBmp = dcMem.SelectObject(_pbitmapDoubleBuf);

                // Now we're ready to draw.
                _OnDraw(&dcMem, prc);

                // Copy into the final buffer:
                dc.StretchBlt(0, 0, RECTWIDTH(*prc), RECTHEIGHT(*prc), &dcMem, 0, 0, RECTWIDTH(*prc), RECTHEIGHT(*prc), SRCCOPY);

                dcMem.SelectObject(hOldBmp);
            }
        }


        dc.Detach();
    }
    else
    {
        __super::OnDrawItem(nIDCtl, lpDrawItemStruct);
    }
}

void CAnimateDialog::_GenerateDoubleBuffer(CDC *pDC, LPRECT prc)
{
    if (!_fDoubleBuffer || (_sizeDoubleBuf != _sizeAnimate))
    {
        _sizeDoubleBuf = _sizeAnimate;
        if (_pbitmapDoubleBuf)
        {
            delete _pbitmapDoubleBuf;
        }

        _pbitmapDoubleBuf = new CBitmap();
        if (_pbitmapDoubleBuf)
        {
            _fDoubleBuffer = _pbitmapDoubleBuf->CreateCompatibleBitmap(pDC, _sizeDoubleBuf.cx, _sizeDoubleBuf.cy);
        }
    }
}

// CAnimateDialog message handlers
