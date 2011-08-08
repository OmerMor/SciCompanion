// IconListBox.cpp : implementation file
//

#include "stdafx.h"
#include "ProfUIS_Controls.h"
#include "IconListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxCustomPainted window

IMPLEMENT_DYNCREATE( CListBoxCustomPainted, CListBox );

CListBoxCustomPainted::CListBoxCustomPainted()
	: m_bShowSelAlways( true )
	, m_bCaseSensitiveSort( true )
	, m_bAutoDestroyListBox( false )
{
	PmBridge_Install( true );
}

CListBoxCustomPainted::~CListBoxCustomPainted()
{
	PmBridge_Uninstall();
}

void CListBoxCustomPainted::_RedrawLB()
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
	Invalidate();
	UpdateWindow();
}

HFONT CListBoxCustomPainted::OnLbQueryItemFont( INT nItemIndex )
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
	nItemIndex;
	return (HFONT) ::SendMessage( m_hWnd, WM_GETFONT, 0, 0 );
}

CSize CListBoxCustomPainted::OnLbQueryItemExtraSize( INT nItemIndex )
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
	nItemIndex;
	return CSize( 2, 0 );
}

bool CListBoxCustomPainted::OnLbQueryWindowFocusedState()
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
	if( ::GetFocus() == m_hWnd )
		return true;
	else
		return false;
}

bool CListBoxCustomPainted::OnLbEraseEntire( CDC & dc, CRect rcClient )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
	dc;
	rcClient;
	return false;
}

void CListBoxCustomPainted::PmBridge_OnPaintManagerChanged(
	CExtPaintManager * pGlobalPM
	)
{
	ASSERT_VALID( this );
	CExtPmBridge::PmBridge_OnPaintManagerChanged( pGlobalPM );
	if( GetSafeHwnd() == NULL )
		return;
	Invalidate();
}

BEGIN_MESSAGE_MAP( CListBoxCustomPainted, CListBox )
	//{{AFX_MSG_MAP(CListBoxCustomPainted)
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT( LBN_SELCANCEL, OnLbnReflectSelCancel )
	ON_CONTROL_REFLECT( LBN_KILLFOCUS, OnLbnReflectKillFocus )
	ON_CONTROL_REFLECT( LBN_SETFOCUS, OnLbnReflectSetFocus )
END_MESSAGE_MAP()

LRESULT CListBoxCustomPainted::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_ERASEBKGND )
		return TRUE;
	if( message == WM_PAINT )
	{
		CRect rcClient;
		GetClientRect( &rcClient );
		CPaintDC dcPaint( this );
		CExtMemoryDC dc( &dcPaint, &rcClient );
		CExtPaintManager * pPM = PmBridge_GetPM();
		ASSERT_VALID( pPM );
		if( ! OnLbEraseEntire( dc, rcClient ) )
			dc.FillSolidRect( &rcClient, pPM->GetColor( COLOR_WINDOW, this ) );
		HWND hWndParent = ::GetParent( m_hWnd );
		UINT nOwnDlgCtrlID = UINT( GetDlgCtrlID() );
		INT nItemIndex, nCurSel = INT( GetCurSel() ), nItemCount = INT( GetCount() ), nTopIndex = ((CListBox*)this)->GetTopIndex();
		for( nItemIndex = nTopIndex; nItemIndex < nItemCount; nItemIndex ++ )
		{
			DRAWITEMSTRUCT dis;
			::memset( &dis, 0, sizeof( DRAWITEMSTRUCT ) );
			dis.CtlType = ODT_LISTBOX;
			dis.CtlID = nOwnDlgCtrlID;
			dis.itemID = nItemIndex;
			dis.hDC = dc.GetSafeHdc();
			GetItemRect( nItemIndex, &dis.rcItem );
			dis.itemAction = ODA_DRAWENTIRE;
			dis.hwndItem = GetSafeHwnd();
			if( rcClient.bottom < dis.rcItem.top )
				break;
			if( GetSel( nItemIndex ) > 0 )
				dis.itemState |= ODS_SELECTED;
			if( nCurSel == nItemIndex )
				dis.itemState |= ODS_FOCUS;
			::SendMessage( hWndParent, WM_DRAWITEM, (WPARAM)nOwnDlgCtrlID, (LPARAM)&dis );
		}
		return 0;
	}
	if( message == WM_MOUSEWHEEL )
	{
		INT nDelta = INT( short(HIWORD(wParam)) );
		if( ( GetStyle() & WS_VSCROLL ) != 0 )
			SendMessage( WM_VSCROLL, MAKELONG( ( ( nDelta > 0 ) ? SB_LINEUP : SB_LINEDOWN ), 0 ) );
		return 0;
	}
	if( message == WM_HSCROLL || message == WM_VSCROLL )
	{
		Invalidate();
	}
	return CListBox::WindowProc( message, wParam, lParam );
}

void CListBoxCustomPainted::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();
}

void CListBoxCustomPainted::PostNcDestroy()
{
	CListBox::PostNcDestroy();
	if( m_bAutoDestroyListBox )
		delete this;
}

void CListBoxCustomPainted::DrawItem( LPDRAWITEMSTRUCT pDIS ) 
{
	ASSERT_VALID( this );
	ASSERT( pDIS != NULL );
	ASSERT( pDIS->hDC != NULL );
	if( ((INT)pDIS->itemID) < 0 )
		return;
DWORD dwStyle = GetStyle();
	if(		( dwStyle & LBS_HASSTRINGS ) == 0
		&&	( dwStyle & (LBS_OWNERDRAWFIXED|LBS_OWNERDRAWVARIABLE) ) != 0
		)
		return;
INT nItemCount = INT( GetCount() ), nItemIndex = pDIS->itemID;
	if( nItemIndex < 0 || nItemIndex >= nItemCount )
		return;
CExtPaintManager * pPM = PmBridge_GetPM();
	ASSERT_VALID( pPM );
bool bWindowFocused = OnLbQueryWindowFocusedState();
bool bSelected = ( ( bWindowFocused || m_bShowSelAlways ) && ( pDIS->itemState & ODS_SELECTED ) != 0 ) ? true : false;
bool bFocused = ( bWindowFocused && ( pDIS->itemState & ODS_FOCUS ) != 0 ) ? true : false;
CDC dc;
	dc.Attach( pDIS->hDC );
	if( pDIS->itemAction & (ODA_DRAWENTIRE|ODA_SELECT) )
	{
		CRect rcErase( pDIS->rcItem );
		if( nItemIndex == ( nItemCount - 1 ) )
		{
			CRect rcClient;
			GetClientRect( &rcClient );
			if( rcErase.bottom < rcClient.bottom )
				rcErase.bottom = rcClient.bottom;
		}
		COLORREF clrBk = pPM->GetColor( ( ! IsWindowEnabled() ) ? COLOR_3DFACE : COLOR_WINDOW, this );
		dc.FillSolidRect( rcErase, clrBk );
	}
CRect rcDrawItem( pDIS->rcItem );
CSize sizeLbItemExtra = OnLbQueryItemExtraSize( nItemIndex );
	rcDrawItem.DeflateRect( sizeLbItemExtra );
	if( bSelected )
		dc.FillSolidRect( &rcDrawItem, pPM->GetColor( bWindowFocused ? COLOR_HIGHLIGHT : COLOR_3DFACE, this ) );
CString sText;
	GetText( nItemIndex, sText );
	if( ! sText.IsEmpty() )
	{
		HFONT hFont = OnLbQueryItemFont( nItemIndex );
		ASSERT( hFont != NULL );
		HGDIOBJ hOldFont = ::SelectObject( dc.m_hDC, hFont );
		COLORREF clrTextOld = dc.SetTextColor( pPM->GetColor( bSelected  ? ( bWindowFocused ?  COLOR_HIGHLIGHTTEXT : COLOR_BTNTEXT ) : COLOR_BTNTEXT, this ) );
		int nOldBkMode = dc.SetBkMode( TRANSPARENT );
		CRect rcText( rcDrawItem );
		UINT nFormat = DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_NOPREFIX;
		if( sText.Find( _T('\t') ) != -1 ) // do tabs expanding
			nFormat |= DT_EXPANDTABS;
		CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(sText), &rcText, nFormat, 0 );
		dc.SetBkMode( nOldBkMode );
		dc.SetTextColor( clrTextOld );
		::SelectObject( dc.m_hDC, hOldFont );

	}
	if( bFocused )
	{
		COLORREF clrTextOld = dc.SetTextColor( RGB(255,255,255) );
		COLORREF clrBkOld = dc.SetBkColor( RGB(0,0,0) );
		dc.DrawFocusRect( &rcDrawItem );
		dc.SetBkColor( clrBkOld );
		dc.SetTextColor( clrTextOld );
	}	
	dc.Detach();
}

void CListBoxCustomPainted::MeasureItem( LPMEASUREITEMSTRUCT pMIS ) 
{
	ASSERT_VALID( this );
	ASSERT( pMIS != NULL );
	ASSERT( ((INT)pMIS->itemID) >= 0 );
DWORD dwStyle = GetStyle();
	if(		( dwStyle & LBS_HASSTRINGS ) == 0
		&&	( dwStyle & (LBS_OWNERDRAWFIXED|LBS_OWNERDRAWVARIABLE) ) != 0
		)
		return;
	pMIS->itemWidth = pMIS->itemHeight = 16;
INT nItemIndex = INT( pMIS->itemID );
	if( nItemIndex < 0 || nItemIndex >= INT(GetCount()) )
		return;
HFONT hFont = OnLbQueryItemFont( nItemIndex );
	ASSERT( hFont != NULL );
CString sMeasure;
	GetText( nItemIndex, sMeasure );
	if( sMeasure.IsEmpty() )
		return;
CWindowDC dc( NULL );
CExtSafeString _sMeasure;
	_sMeasure = sMeasure;
CRect rcMeasure = CExtPaintManager::stat_CalcTextDimension( dc, * CFont::FromHandle(hFont), _sMeasure, DT_SINGLELINE|DT_LEFT|DT_NOPREFIX );
INT nWidth = rcMeasure.Width();
INT nHeight = rcMeasure.Height();
	pMIS->itemWidth = nWidth;
	pMIS->itemHeight = max( nHeight, (INT)pMIS->itemHeight );
CSize sizeLbItemExtra = OnLbQueryItemExtraSize( nItemIndex );
	pMIS->itemWidth += sizeLbItemExtra.cx * 2;
	pMIS->itemHeight += sizeLbItemExtra.cy * 2;
}

int CListBoxCustomPainted::CompareItem( LPCOMPAREITEMSTRUCT pCIS )
{
	ASSERT_VALID( this );
	ASSERT( pCIS != NULL );
DWORD dwStyle = GetStyle();
	if(		( dwStyle & LBS_HASSTRINGS ) == 0
		&&	( dwStyle & (LBS_OWNERDRAWFIXED|LBS_OWNERDRAWVARIABLE) ) != 0
		)
		return 0;
CString sText1, sText2;
	GetText( INT( pCIS->itemID1 ), sText1 );
	GetText( INT( pCIS->itemID2 ), sText2 );
INT nRetVal = m_bCaseSensitiveSort ? sText1.Compare( LPCTSTR(sText2) ) : sText1.CompareNoCase( LPCTSTR(sText2) );
	return nRetVal;
}

void CListBoxCustomPainted::DeleteItem( LPDELETEITEMSTRUCT pDIS )
{
	ASSERT_VALID( this );
	ASSERT( pDIS != NULL );
	CListBox::DeleteItem( pDIS );
}

int CListBoxCustomPainted::VKeyToItem( UINT nKey, UINT nIndex )
{
	ASSERT_VALID( this );
	return CListBox::VKeyToItem( nKey, nIndex );
}

int CListBoxCustomPainted::CharToItem( UINT nKey, UINT nIndex )
{
	ASSERT_VALID( this );
	return CListBox::CharToItem( nKey, nIndex );
}

void CListBoxCustomPainted::OnLbnReflectSelCancel() 
{
	ASSERT_VALID( this );
	_RedrawLB();
}

void CListBoxCustomPainted::OnLbnReflectKillFocus() 
{
	ASSERT_VALID( this );
	_RedrawLB();
}

void CListBoxCustomPainted::OnLbnReflectSetFocus() 
{
	ASSERT_VALID( this );
	_RedrawLB();
}

/////////////////////////////////////////////////////////////////////////////
// CListBoxMenuLike window

CListBoxMenuLike::CListBoxMenuLike()
{
}

CListBoxMenuLike::~CListBoxMenuLike()
{
	RemoveAllIcons();
}

BEGIN_MESSAGE_MAP( CListBoxMenuLike, CListBoxCustomPainted )
	//{{AFX_MSG_MAP(CListBoxMenuLike)
	ON_CONTROL_REFLECT(LBN_SELCANCEL, OnSelcancel)
	ON_CONTROL_REFLECT(LBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(LBN_SETFOCUS, OnSetfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxMenuLike message handlers

void CListBoxMenuLike::DrawItem( LPDRAWITEMSTRUCT pDIS ) 
{
	ASSERT_VALID( this );
	ASSERT( pDIS != NULL );
	ASSERT( pDIS->hDC != NULL );
	if( ((INT)pDIS->itemID) < 0 )
		return;
CDC * pDC = CDC::FromHandle( pDIS->hDC );
CRect rcItem = pDIS->rcItem;
	if( ( pDIS->itemState & ( ODS_SELECTED | ODS_FOCUS ) ) != 0 )
	{
		CRect rcErase( rcItem );
		if( INT(pDIS->itemID) == (GetCount()-1) )
		{
			CRect rcClient;
			GetClientRect( &rcClient );
			if( rcErase.bottom < rcClient.bottom )
				rcErase.bottom = rcClient.bottom;
		}
		pDC->FillSolidRect( rcErase, g_PaintManager->GetColor( COLOR_WINDOW, (CObject*)this ) );
		CRect rcBottomArea( rcErase );
		rcBottomArea.top = rcItem.bottom;
		CExtPaintManager::PAINTMENUITEMDATA _pmid(
			NULL,
			rcBottomArea,
			rcBottomArea,
			GetIconAreaWidth(),
			_T(""),
			_T(""),
			NULL,
			false,
			false,
			false,
			false,
			false,
			true,
			false,
			false,
			false,
			0,
			NULL
			);
		g_PaintManager->PaintMenuItem( *pDC, _pmid );
	}
INT nIconAreaWidth = GetIconAreaWidth();
	m_arrItems[ pDIS->itemID ] -> DrawItem( nIconAreaWidth, pDIS );
	if(		( pDIS->itemState & ODS_SELECTED ) != 0
		&&	OnLbQueryWindowFocusedState()
		)
	{
		CRect rcFocus = rcItem;
		rcFocus.left += nIconAreaWidth;
		rcFocus.DeflateRect( 3, 3 );
		pDC->DrawFocusRect( &rcFocus );
	}
}

void CListBoxMenuLike::MeasureItem( LPMEASUREITEMSTRUCT pMIS ) 
{
	ASSERT_VALID( this );
	ASSERT( pMIS != NULL );
	ASSERT( ((INT)pMIS->itemID) >= 0 );
CRect rcClient;
	GetClientRect( &rcClient );
CSize _sizeClientArea = rcClient.Size();
	m_arrItems[ pMIS->itemID ] -> MeasureItem( pMIS, _sizeClientArea );
}

void CListBoxMenuLike::OnSelcancel() 
{
	_Invalidate();
}

void CListBoxMenuLike::OnKillfocus() 
{
	_Invalidate();
}

void CListBoxMenuLike::OnSetfocus() 
{
	_Invalidate();
}

void CListBoxMenuLike::ITEM_DATA::DrawItem(INT nIconAreaWidth, LPDRAWITEMSTRUCT pDIS)
{
CRect rcItem = pDIS->rcItem;
CDC dcTmp;
	dcTmp.Attach( pDIS->hDC );
CExtMemoryDC dc( &dcTmp, &rcItem );
bool bSelected = (pDIS->itemState & ODS_SELECTED) != 0;
	CExtPaintManager::PAINTMENUITEMDATA _pmid(
		NULL,
		rcItem,
		rcItem,
		nIconAreaWidth,
		LPCTSTR(m_strText), // _T(""),
		_T(""),
		&m_icon,
		false,
		bSelected,
		false,
		false,
		false,
		true,
		false,
		false,
		false,
		0,
		NULL
		);
	g_PaintManager->PaintMenuItem( dc, _pmid );
	dc.__Flush();
	dcTmp.Detach();
}

void CListBoxMenuLike::ITEM_DATA::MeasureItem(
	LPMEASUREITEMSTRUCT pMIS,
	CSize _sizeClientArea
	) 
{
CSize _sizeIcon = GetSize();
	pMIS->itemWidth = _sizeClientArea.cx;
	pMIS->itemHeight = _sizeIcon.cy;
CWindowDC dcDesktop( NULL );
CRect rcText( 0, 0, _sizeClientArea.cx, 0 );
CFont * pOldFont = dcDesktop.SelectObject( &g_PaintManager->m_FontNormal );
UINT nTextHeight = (UINT) CExtRichContentLayout::stat_DrawText( dcDesktop.m_hDC, LPCTSTR(m_strText), &rcText, DT_LEFT|DT_TOP|DT_CALCRECT, 0 );
	dcDesktop.SelectObject( pOldFont );
	pMIS->itemHeight = max( nTextHeight, pMIS->itemHeight );
	// vertical gaps
	pMIS->itemHeight += 8;
	rcText = CalcTextRect( CRect( 0, 0, pMIS->itemWidth, pMIS->itemHeight ), _sizeIcon );
}

bool CListBoxMenuLike::OnLbEraseEntire( CDC & dc, CRect rcClient )
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
CExtPaintManager::PAINTMENUITEMDATA _pmid(
		NULL,
		rcClient,
		rcClient,
		GetIconAreaWidth(),
		_T(""),
		_T(""),
		NULL,
		false,
		false,
		false,
		false,
		false,
		true,
		false,
		false,
		false,
		0,
		NULL
		);
	g_PaintManager->PaintMenuItem( dc, _pmid );
	return false;
}

