// CMDITabsDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MDITabsDialogBar.h"
#include "ResourceDocument.h"
#include "SCIPicEditor.h"
#include "MDITabChildWnd.h"
#include "ScriptDocument.h"

// CMDITabsDialogBar

//
// Includes code from "Ownerdraw Tab Controls - Borders and All" on codeproject.com, by .dan.g.
//


BEGIN_MESSAGE_MAP(CMDITabsDialogBar, CTabCtrl)
    ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
    ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelChange)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CMDITabsDialogBar::AddTab(CFrameWnd *pFrame, MDITabType iType)
{
    // REVIEW: this is a bit of a hack.  The tab control uses its font to resize itself.
    // However, we use prof-uis paint manager's font to draw.  This font can change (and be deleted)
    // when system settings change.  But we don't know when to update it here.  It's fine for painting,
    // but bad for measuring.  So just set it again whenever we add an item.
    SetFont(&g_PaintManager->m_FontBoldBC);

    CString strTitle = pFrame->GetTitle();
    int cItems = GetItemCount();
    // Except for the first item, add at index 1 (game explorer tab always comes first)
    int iIndex = cItems > 0 ? 1 : 0;
    InsertItem(TCIF_TEXT | TCIF_PARAM, iIndex, strTitle, iType, (LPARAM)pFrame);
}

void CMDITabsDialogBar::RemoveTab(CFrameWnd *pFrame)
{
    // Remove it from the tab control
    int i = 0;
    for ( ; i < GetItemCount(); i++)
    {
        TCITEM tcitem = { 0 };
        tcitem.mask = TCIF_PARAM;
        if (GetItem(i, &tcitem))
        {
            if (tcitem.lParam == (LPARAM)pFrame)
            {
                break;
            }
        }
    }
    if (i < GetItemCount())
    {
        DeleteItem(i);
    }

    // Remove it from the travellog
    _travelLog.remove(pFrame);
    if (pFrame == _pActiveTab)
    {
        _pActiveTab = NULL;
    }
    // -> the newly active tab should get set in OnActivateTab
}

void CMDITabsDialogBar::OnActivateTab(CFrameWnd *pFrame)
{
    if (!_fNavigatingViaTravelLog)
    {
        // Remove it if its already in the list.
        _travelLog.remove(pFrame);
        // And add it back at the end.
        _travelLog.push_back(pFrame);
    }
    _pActiveTab = pFrame;
}

CFrameWnd *CMDITabsDialogBar::_GetNextPrev(bool fNext)
{
    CFrameWnd *pNext = NULL;
    if (_pActiveTab)
    {
        travellog_list::iterator it = find(_travelLog.begin(), _travelLog.end(), _pActiveTab);
        if (it != _travelLog.end())
        {
            if ((fNext ? (++it) : (--it)) != _travelLog.end())
            {
                pNext = *it;
            }
        }
    }
    return pNext;
}

bool CMDITabsDialogBar::CanGoBack()
{
    return (_GetNextPrev(false) != NULL);
}

bool CMDITabsDialogBar::CanGoForward()
{
    return (_GetNextPrev(true) != NULL);
}

void CMDITabsDialogBar::_GoBackForward(bool fForward)
{
    CFrameWnd *pBack = _GetNextPrev(fForward);
    if (pBack)
    {
        _fNavigatingViaTravelLog = true;
        CMDIChildWnd *pActive = static_cast<CMDIChildWnd*>(pBack);
        pActive->MDIActivate();
        _fNavigatingViaTravelLog = false;
    }
}

void CMDITabsDialogBar::GoBack()
{
    _GoBackForward(false);
}
void CMDITabsDialogBar::GoForward()
{
    _GoBackForward(true);
}


CScriptDocument *CMDITabsDialogBar::ActivateScript(ScriptId script)
{
    CScriptDocument *pSDRet = NULL;
    // Find a script that matches this, if any
    for (int i = 0 ; !pSDRet && i < GetItemCount(); i++)
    {
        CMDITabChildWnd *pActive;
        CScriptDocument *pSD = static_cast<CScriptDocument*>(_GetIfKindOf(i, RUNTIME_CLASS(CScriptDocument), &pActive));
        if (pSD && (pSD->GetScriptId() == script))
        {
            pSDRet = pSD;
            pActive->MDIActivate();
            pActive->SetFocus();
        }
    }
    return pSDRet;
}

CDocument *CMDITabsDialogBar::_GetIfKindOf(int i, const CRuntimeClass* pClass, CMDITabChildWnd **pActive) const
{
    CDocument *pDoc = NULL;
    TCITEM tcitem = { 0 };
    tcitem.mask = TCIF_PARAM;
    if (GetItem(i, &tcitem))
    {
        *pActive = reinterpret_cast<CMDITabChildWnd*>(tcitem.lParam);
        CDocument *pDocQ = (*pActive)->GetActiveDocument();
        if (pDocQ->IsKindOf(pClass))
        {
            pDoc = pDocQ;
        }
    }  
    return pDoc;
}

CResourceDocument *CMDITabsDialogBar::ActivateResourceDocument(ResourceType type, WORD wNum)
{
    CResourceDocument *pRDRet = NULL;
    // Find a script that matches this, if any
    for (int i = 0 ; !pRDRet && i < GetItemCount(); i++)
    {
        CMDITabChildWnd *pActive;
        CResourceDocument *pRD = static_cast<CResourceDocument*>(_GetIfKindOf(i, RUNTIME_CLASS(CResourceDocument), &pActive));
        if (pRD && (pRD->GetType() == type) && (pRD->GetNumber() == (int)wNum))
        {
            if (theApp._resourceRecency.IsResourceMostRecent(pRD))
            {
                pRDRet = pRD;
                pActive->MDIActivate();
                pActive->SetFocus();
            }
        }
    }
    return pRDRet;
}

int _GetBitmapIndex(MDITabType tabType)
{
    int i = 0;
    int tt = static_cast<int>(tabType);
    while (tt)
    {
        tt >>= 1;
        ++i;
    }
    return i;
}

// MDITabs message handlers

int CMDITabsDialogBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
    _fNavigatingViaTravelLog = false;

    const double c_rgHueAdjust[] =
    {
        0.0,    // none
        0.0,    // game
        0.4,    // view
        0.0,    // pic
        -1.0,   // script
        -0.8,    // vocab
        -0.3,    // font
        0.8,    // cursor
        0.3,    // text
        -0.6,   // sound
        0.0,    // room explorer
    };

    const double c_rgSatAdjust[] =
    {
        0.0,    // none
        -1.0,    // game
        0.0,    // view
        0.0,    // pic
        0.0,   // script
        0.0,    // vocab
        0.0,    // font
        0.0,    // cursor
        0.0,    // text
        0.0,   // sound
        0.0,    // room explorer
    };

    ASSERT(ARRAYSIZE(c_rgHueAdjust) == ARRAYSIZE(_tabBitmap));
    for (int i = 0; i < ARRAYSIZE(c_rgHueAdjust); ++i)
    {
        _tabBitmap[i].LoadBMP_Resource(MAKEINTRESOURCE(IDB_TABPIC));
        _tabBitmap[i].AdjustHLS(COLORREF(-1L), RGB(0, 0, 0), c_rgHueAdjust[i], 0.0, c_rgSatAdjust[i], NULL);

        _tabBitmapNS[i].LoadBMP_Resource(MAKEINTRESOURCE(IDB_TABPICNS));
        _tabBitmapNS[i].AdjustHLS(COLORREF(-1L), RGB(0, 0, 0), c_rgHueAdjust[i], 0.0, c_rgSatAdjust[i], NULL);
    }
   return 0;
}

void CMDITabsDialogBar::OnSelChange(NMHDR *pnmhdr, LRESULT *lResult)
{
    int iActive = GetCurSel();
    if (iActive != -1)
    {
        TCITEM tcitem;
        tcitem.mask = TCIF_PARAM;
        if (GetItem(iActive, &tcitem))
        {
            CMDIChildWnd *pActive = reinterpret_cast<CMDIChildWnd*>(tcitem.lParam);
            //pActive->ActivateFrame(); // This glitches maximized windows, so use MDIActivate
            pActive->MDIActivate();
        }
    }
}


void CMDITabsDialogBar::OnUpdateTitles()
{
    // See comment in AddTab...
    SetFont(&g_PaintManager->m_FontBoldBC);

    // Determine the active child frame:
    CFrameWnd *pActive = NULL;
    CFrameWnd *pFrame = (CFrameWnd*)AfxGetMainWnd();
    if (pFrame)
    {
        pActive = pFrame->GetActiveFrame();
    }
    int iActive = -1;

    // Now go through and update titles, and selection.
    int i = 0;
    for ( ; i < GetItemCount(); i++)
    {
        TCITEM tcitem = { 0 };
        tcitem.mask = TCIF_PARAM;
        if (GetItem(i, &tcitem))
        {
            CFrameWnd *pFrame = (CFrameWnd*)tcitem.lParam;
            if (pFrame)
            {
                CString strTitle;
                pFrame->GetWindowText(strTitle);
                TCHAR szTitleCurrent[MAX_PATH];
                tcitem.mask = TCIF_TEXT;
                tcitem.pszText = szTitleCurrent;
                tcitem.cchTextMax = ARRAYSIZE(szTitleCurrent);
                if (GetItem(i, &tcitem))
                {
                    if (0 != strcmp(szTitleCurrent, strTitle))
                    {
                        StringCchCopy(szTitleCurrent, ARRAYSIZE(szTitleCurrent), strTitle);
                        tcitem.mask = TCIF_TEXT;
                        tcitem.pszText = szTitleCurrent;
                        SetItem(i, &tcitem);
                    }
                }

                if (pFrame == pActive)
                {
                    iActive = i;
                }
            }
        }
    }

    // Make sure the active one is selected.
    if (GetCurSel() != iActive)
    {
        SetCurSel(iActive);
    }

}


// CMDITabsDialogBar message handlers

//
// This makes the bar the entire width of the frame, and a fixed height.
//
LRESULT CMDITabsDialogBar::OnSizeParent(WPARAM wParam, LPARAM lParam)
{
    ASSERT_VALID(this);
    wParam; // unused
    AFX_SIZEPARENTPARAMS *pLayout = (AFX_SIZEPARENTPARAMS *) lParam;
    ASSERT(pLayout != NULL);
    CRect rcFrameRest = &pLayout->rect;
    DWORD dwWndStyle = GetStyle();
    if((dwWndStyle & WS_VISIBLE) == 0)
    {
        return 0;
    }
    int nHeight = 26;
    CRect rcOwnLayout(rcFrameRest);
    pLayout->rect.top += nHeight;
    rcOwnLayout.bottom = rcOwnLayout.top + nHeight;
    pLayout->sizeTotal.cy += nHeight;
    if (pLayout->hDWP != NULL)
    {
        ::AfxRepositionWindow(pLayout, m_hWnd, &rcOwnLayout);
    }
    return 0;
}

//
// Add a splash of colour
//
COLORREF _CombineBasedOnType(int iTabType, COLORREF clrSource)
{
    RGBQUAD source = { 0 };
    source.rgbBlue = GetBValue(clrSource);
    source.rgbGreen = GetGValue(clrSource);
    source.rgbRed = GetRValue(clrSource);

    COLORREF clr;
    switch (iTabType)
    {
    case TAB_PIC: // green
        {
            RGBQUAD tint = { 15, 255, 165 };
            clr = RGB_TO_COLORREF(_Combine(tint, source));
        }
        break;
    case TAB_TEXT: // cyan
        {
            RGBQUAD tint = { 255, 255, 155 };
            clr = RGB_TO_COLORREF(_Combine(tint, source));
        }
        break;
    case TAB_SOUND: // orange?
        {
            RGBQUAD tint = { 255, 0, 0 };
            clr = RGB_TO_COLORREF(_Combine(tint, source));
        }
        break;
    case TAB_VIEW: // blue
        {
            RGBQUAD tint = { 239, 132, 33 };
            clr = RGB_TO_COLORREF(_Combine(tint, source));
        }
        break;
    case TAB_FONT: // yellow 
        {
            RGBQUAD tint = { 0, 255, 255};
            clr = RGB_TO_COLORREF(_Combine(tint, source));
        }
        break;
    case TAB_VOCAB: // flesh
        {
            RGBQUAD tint = { 162, 206, 255 };
            clr = RGB_TO_COLORREF(_Combine(tint, source));
        }
        break;
    case TAB_SCRIPT: // red
        {
            RGBQUAD tint = { 25, 25, 255 };
            clr = RGB_TO_COLORREF(_Combine(tint, source));
        }
        break;
    case TAB_CURSOR: // pink
        {
            RGBQUAD tint = { 172, 172, 255 };
            clr = RGB_TO_COLORREF(_Combine(tint, source));
        }
        break;
    case TAB_GAME: // grey
        {
            RGBQUAD tint = { 128, 128, 128 };
            clr = RGB_TO_COLORREF(_Combine(tint, source));
        }
        break;
    default:
        clr = clrSource;
        break;
    }
    return clr;    
}

void _DrawGradient(CDC &dc, const CRect &rect, COLORREF c1, COLORREF c2)
{
    TRIVERTEX vertices[2];
    vertices[0].Alpha = 0;
    vertices[0].Red = GetRValue(c1) << 8;
    vertices[0].Green = GetGValue(c1) << 8;
    vertices[0].Blue = GetBValue(c1) << 8;
    vertices[0].x = rect.left;
    vertices[0].y = rect.top;
    vertices[1].Alpha = 0;
    vertices[1].Red = GetRValue(c2) << 8;
    vertices[1].Green = GetGValue(c2) << 8;
    vertices[1].Blue = GetBValue(c2) << 8;
    vertices[1].x = rect.right;
    vertices[1].y = rect.bottom;
    GRADIENT_RECT rc;
    rc.UpperLeft = 0;
    rc.LowerRight = 1;
    dc.GradientFill(vertices, ARRAYSIZE(vertices), &rc, 1, GRADIENT_FILL_RECT_V);
}

void CMDITabsDialogBar::DrawItem(LPDRAWITEMSTRUCT pdis)
{
    CRect rc = pdis->rcItem;
    int nTabIndex = pdis->itemID;
    bool fSelected = (nTabIndex == GetCurSel());
    ODA_DRAWENTIRE;
	char label[64];
	TCITEM tci;
	tci.mask = TCIF_TEXT | TCIF_PARAM;
	tci.pszText = label;
	tci.cchTextMax = ARRAYSIZE(label);
	if (GetItem(nTabIndex, &tci))
    {
        CMDITabChildWnd *pActive = reinterpret_cast<CMDITabChildWnd*>(tci.lParam);

        CDC dc;
        dc.Attach(pdis->hDC);

        int iIndex = _GetBitmapIndex(pActive->GetTabType());
        CExtBitmap &bitmapToUse = fSelected ? _tabBitmap[iIndex] : _tabBitmapNS[iIndex];
        CRect rcSrc(CPoint(0, 0), bitmapToUse.GetSize());
        //CRect rcPadding(2, 2, 2, 2);
        CRect rcPadding(0, 0, 0, 0);
        bitmapToUse.AlphaBlendSkinParts(pdis->hDC, rc, rcSrc, rcPadding, /*__EXT_BMP_FLAG_PREMULTIPLIED_RGB_CHANNELS |*/ CExtBitmap::__EDM_STRETCH, true, true, 0xFF);

        // Use a different font decoration depending on if this is the most recent version of this item.
        bool fNotMostRecent = false;
        CDocument *pDocAny = pActive->GetActiveDocument();
        if (pDocAny && pDocAny->IsKindOf(RUNTIME_CLASS(CResourceDocument)))
        {
            CResourceDocument *pDoc = static_cast<CResourceDocument*>(pDocAny);
            fNotMostRecent = !theApp._resourceRecency.IsResourceMostRecent(pDoc);
        }
        if (fNotMostRecent)
        {
            // Draw a diagonal line across the thing.
            dc.MoveTo(rc.TopLeft());
            dc.LineTo(rc.BottomRight());
        }

        rc.OffsetRect(4, 1); // indent
        COLORREF crText = RGB(0, 0, 0); // g_PaintManager->PAINTPUSHBUTTONDATA::m_clrForceTextNormal; // green in release, black in debug!
        int nOldText = dc.SetTextColor(crText);
        int nOldBk = dc.SetBkMode(TRANSPARENT);
        dc.DrawText(tci.pszText, -1, &rc, DT_SINGLELINE);
        dc.SetBkMode(nOldBk);
        dc.SetTextColor(nOldText);

        dc.Detach();
    }
}


void CMDITabsDialogBar::DrawItemBorder(LPDRAWITEMSTRUCT lpdis)
{
	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
    BOOL bBackTabs = FALSE;

	CRect rItem(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 

	if (bSelected || bBackTabs)
	{
		rItem.bottom += bSelected ? -1 : 1;
		// edges
        pDC->FillRect(CRect(rItem.left, rItem.top, rItem.left + 1, rItem.bottom), &g_PaintManager->m_brushLightestDefault);
		pDC->FillRect(CRect(rItem.left, rItem.top, rItem.right, rItem.top + 1), &g_PaintManager->m_brushLightestDefault);
		pDC->FillRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), &g_PaintManager->m_brushDarkestDefault);
	}
	else // draw simple dividers
	{
		pDC->FillRect(CRect(rItem.left - 1, rItem.top, rItem.left, rItem.bottom), &g_PaintManager->m_brushDarkestDefault);
		pDC->FillRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), &g_PaintManager->m_brushDarkestDefault);
	}
}

void CMDITabsDialogBar::DrawMainBorder(LPDRAWITEMSTRUCT lpdis)
{
    CRect rBorder(lpdis->rcItem);
    CDC* pDC = CDC::FromHandle(lpdis->hDC); 
    pDC->FillSolidRect(&rBorder, g_PaintManager->GetColor(COLOR_BTNFACE));
}



void CMDITabsDialogBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// prepare dc
    HGDIOBJ hOldFont = dc.SelectObject(g_PaintManager->m_FontBoldBC);

	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_TAB;
	dis.CtlID = GetDlgCtrlID();
	dis.hwndItem = GetSafeHwnd();
	dis.hDC = dc.GetSafeHdc();
	dis.itemAction = ODA_DRAWENTIRE;

	// draw the rest of the border
	CRect rClient, rPage;
	GetClientRect(&dis.rcItem);
	rPage = dis.rcItem;
	AdjustRect(FALSE, rPage);
	dis.rcItem.top = rPage.top - 2;

	DrawMainBorder(&dis);

	// paint the tabs first and then the borders
	int nTab = GetItemCount();
	int nSel = GetCurSel();

	if (!nTab) // no pages added
		return;

	while (nTab--)
	{
		if (nTab != nSel)
		{
			dis.itemID = nTab;
			dis.itemState = 0;

			VERIFY(GetItemRect(nTab, &dis.rcItem));
			DrawItem(&dis);
			//DrawItemBorder(&dis);
		}
	}

	// now selected tab
    if (nSel != -1)
    {
	    dis.itemID = nSel;
	    dis.itemState = ODS_SELECTED;

	    VERIFY(GetItemRect(nSel, &dis.rcItem));
	    DrawItem(&dis);
	    //DrawItemBorder(&dis);    
    }
    dc.SelectObject(hOldFont);
}

BOOL CMDITabsDialogBar::OnEraseBkgnd(CDC *pDC)
{
	CRect rClient, rTab, rTotalTab, rBkgnd, rEdge;
	int nTab, nTabHeight = 0;

	__super::OnEraseBkgnd(pDC);

	// calc total tab width
	GetClientRect(rClient);
	nTab = GetItemCount();
	rTotalTab.SetRectEmpty();

	while (nTab--)
	{
		GetItemRect(nTab, rTab);
		rTotalTab.UnionRect(rTab, rTotalTab);
	}

	nTabHeight = rTotalTab.Height();

	// add a bit
	rTotalTab.InflateRect(2, 3);
	rEdge = rTotalTab;

	// then if background color is set, paint the visible background
	// area of the tabs in the bkgnd color
	// note: the mfc code for drawing the tabs makes all sorts of assumptions
	// about the background color of the tab control being the same as the page
	// color - in some places the background color shows thru' the pages!!
	// so we must only paint the background color where we need to, which is that
	// portion of the tab area not excluded by the tabs themselves
    CBrush *pBrush = &g_PaintManager->m_brushLighterDefault;
	
	// full width of tab ctrl above top of tabs
	rBkgnd = rClient;
	rBkgnd.bottom = rTotalTab.top + 3;
    pDC->FillRect(&rBkgnd, pBrush);
	
	// width of tab ctrl visible bkgnd including bottom pixel of tabs to left of tabs
	rBkgnd = rClient;
	rBkgnd.right = 2;
	rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);
    pDC->FillRect(&rBkgnd, pBrush);
	
	// to right of tabs
	rBkgnd = rClient;
	rBkgnd.left += rTotalTab.Width() - 2;
	rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);
    pDC->FillRect(&rBkgnd, pBrush);

	return TRUE;
}