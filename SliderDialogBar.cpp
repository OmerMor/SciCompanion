// SliderDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "PicDoc.h"
#include "PicResource.h"
#include "SliderDialogBar.h"


// CPositionSlider
CPositionSlider::CPositionSlider()
{
    _pDoc = NULL;
    _lastPos = 0;
}

CPositionSlider::~CPositionSlider()
{
}

BEGIN_MESSAGE_MAP(CPositionSlider, CExtSliderWnd)
    ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
    ON_WM_HSCROLL_REFLECT()
END_MESSAGE_MAP()

// CPositionSlider message handlers


//
// This makes the bar the entire width of the frame, and a fixed height.
//
LRESULT CPositionSlider::OnSizeParent(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);
    wParam; // unused
    AFX_SIZEPARENTPARAMS *pLayout = (AFX_SIZEPARENTPARAMS *) lParam;
    ASSERT(pLayout != NULL);
    CRect rcFrameRest = &pLayout->rect;
    DWORD dwWndStyle = CSliderCtrl::GetStyle(); // CExtSliderWnd has a GetStyle that is completely different.
    if((dwWndStyle & WS_VISIBLE) == 0)
    {
        return 0;
    }
    int nHeight = 28;
    CRect rcOwnLayout(rcFrameRest);
    pLayout->rect.bottom -= nHeight;
    rcOwnLayout.top = rcOwnLayout.bottom - nHeight;
    pLayout->sizeTotal.cy += nHeight;
    if (pLayout->hDWP != NULL)
    {
        ::AfxRepositionWindow(pLayout, m_hWnd, &rcOwnLayout);
    }
    return 0;
}


void CPositionSlider::HScroll(UINT nSBCode, UINT nPos)
{
    switch (nSBCode)
    {
    case SB_THUMBTRACK:
        // Set this pos on the document.
        if (_pDoc)
        {
            _pDoc->SeekToPos((INT_PTR)nPos);
            _lastPos = nPos;
        }
        break;

    case SB_LEFT:
    case SB_ENDSCROLL:
    case SB_LINELEFT:
    case SB_LINERIGHT:
    case SB_PAGELEFT:
    case SB_PAGERIGHT:
    case SB_RIGHT:
        {
            // Set this pos on the document.
            int iPos = GetPos();
            if (_pDoc)
            {
                _pDoc->SeekToPos((INT_PTR)iPos);
            }
        }
        break;



    case SB_THUMBPOSITION:
        // We update constantly, so no need to do anything here.
        break;
    }
}

void CPositionSlider::SetDocument(CDocument *pDoc)
{
    _pDoc = static_cast<CPicDoc*>(pDoc);
    if (_pDoc)
    {
        _pDoc->AddNonViewClient(this);
    }
}


void CPositionSlider::UpdateNonView(LPARAM lHint)
{
    // Foo bar
    int x =0;
    ASSERT(_pDoc);
    PicDrawManager &pdm = _pDoc->GetDrawManager();
    if ((lHint == 0) || (lHint & (VIEWUPDATEHINT_NEWPIC | VIEWUPDATEHINT_EDITPICINVALID)))
    {
        INT_PTR cCommands = pdm.GetPic()->GetCommandCount();
        SetRange(0, (int)cCommands);
        SetPageSize((int) (cCommands / 20));
        SetLineSize(1);
        lHint |= VIEWUPDATEHINT_EDITPICPOSCHANGE; // New pic, so we'll update pos changed too.
    }

    if (lHint & VIEWUPDATEHINT_EDITPICPOSCHANGE)
    {
        INT_PTR cCommands = pdm.GetPic()->GetCommandCount();
        INT_PTR iPos = pdm.GetPos();
        if (iPos == -1)
        {
            iPos = cCommands;
        }
        SetPos((int)iPos);
    }
}


