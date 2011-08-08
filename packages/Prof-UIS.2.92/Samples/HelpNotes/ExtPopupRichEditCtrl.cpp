// ExtPopupRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HelpNotes.h"
#include "ExtPopupRichEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtDynamicRichEditDoc::CExtDynamicRichEditCntrItem

CExtDynamicRichEditDoc::CExtDynamicRichEditCntrItem::CExtDynamicRichEditCntrItem(
		REOBJECT * preo,
		CExtDynamicRichEditDoc * pContainer
		)
	: CRichEditCntrItem(
		preo, pContainer
		)
{
}

CExtDynamicRichEditDoc::CExtDynamicRichEditCntrItem::~CExtDynamicRichEditCntrItem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CExtDynamicRichEditDoc

IMPLEMENT_DYNCREATE( CExtDynamicRichEditDoc, CRichEditDoc )

BEGIN_MESSAGE_MAP(CExtDynamicRichEditDoc, CRichEditDoc)
	//{{AFX_MSG_MAP(CExtDynamicRichEditDoc)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI( ID_OLE_EDIT_LINKS, CRichEditDoc::OnUpdateEditLinksMenu )
	ON_COMMAND( ID_OLE_EDIT_LINKS, CRichEditDoc::OnEditLinks )
	ON_UPDATE_COMMAND_UI_RANGE( ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CRichEditDoc::OnUpdateObjectVerbMenu )
END_MESSAGE_MAP()

CExtDynamicRichEditDoc::CExtDynamicRichEditDoc()
{
}

CExtDynamicRichEditDoc::~CExtDynamicRichEditDoc()
{
}

BOOL CExtDynamicRichEditDoc::CanCloseFrame( CFrameWnd *  pFrame ) 
{
	ASSERT_VALID( this );
	pFrame;
	return TRUE;
}

CRichEditCntrItem * CExtDynamicRichEditDoc::CreateClientItem(
	REOBJECT * preo
	) const
{
	ASSERT_VALID( this );
	return new CExtDynamicRichEditCntrItem( preo, (CExtDynamicRichEditDoc*) this );
}

void CExtDynamicRichEditDoc::OnIdle()
{
	ASSERT_VALID( this );
}

BOOL CExtDynamicRichEditDoc::OnNewDocument()
{
	ASSERT_VALID( this );
	if( !CRichEditDoc::OnNewDocument() )
		return FALSE;
	CRichEditDoc::DeleteContents();
	return TRUE;
}

BOOL CExtDynamicRichEditDoc::SaveModified() 
{
	ASSERT_VALID( this );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CExtDynamicRichEditView

IMPLEMENT_DYNCREATE( CExtDynamicRichEditView, CRichEditView )

BEGIN_MESSAGE_MAP( CExtDynamicRichEditView, CRichEditView )
	//{{AFX_MSG_MAP(CExtDynamicRichEditView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CExtDynamicRichEditView::CExtDynamicRichEditView()
	: m_pDocTemplate( NULL )
{
}

CExtDynamicRichEditView::~CExtDynamicRichEditView()
{
}

void CExtDynamicRichEditView::_DoDestroyWindow()
{
	ASSERT_VALID( this );
//COleClientItem * pActiveItem =
//		GetDocument()->GetInPlaceActiveItem( this );
//	if(		pActiveItem != NULL
//		&&	pActiveItem->GetActiveView() == this
//		)
//	{
//		pActiveItem->Deactivate();
//		ASSERT( GetDocument()->GetInPlaceActiveItem(this) == NULL );
//	}
}

CExtDynamicRichEditDoc * CExtDynamicRichEditView::GetDocument()
{
	ASSERT_VALID( this );
	return ( (CExtDynamicRichEditDoc*) m_pDocument );
}

int CExtDynamicRichEditView::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	ASSERT_VALID( this );
	if( CRichEditView::OnCreate(lpCreateStruct) == -1 )
	{
		ASSERT_VALID( this );
		return -1;
	}
	if( m_pDocTemplate == NULL )
	{
		m_pDocTemplate =
			new CSingleDocTemplate(
				IDR_MAINFRAME,
				RUNTIME_CLASS( CExtDynamicRichEditDoc ),
				RUNTIME_CLASS( CFrameWnd ),
				RUNTIME_CLASS( CExtDynamicRichEditView )
				);
		ASSERT_VALID( m_pDocTemplate );
		m_pDocTemplate->SetContainerInfo(
			IDR_MAINFRAME
			);
		::AfxGetApp()->AddDocTemplate( m_pDocTemplate );
	} // if( m_pDocTemplate == NULL )
CDocument * pDocument =
		m_pDocTemplate->CreateNewDocument();
	ASSERT_VALID( pDocument );
	ASSERT( pDocument->GetDocTemplate() == m_pDocTemplate );
	pDocument->AddView( this );
	ASSERT( m_pDocument == pDocument );
	return 0;
}

void CExtDynamicRichEditView::OnDestroy()
{
	ASSERT_VALID( this );
	CRichEditView::OnDestroy();
	_DoDestroyWindow();
	if( m_pDocument != NULL )
	{
		m_pDocument->RemoveView( this );
		ASSERT( m_pDocument == NULL );
	} // if( m_pDocument != NULL )
}

void CExtDynamicRichEditView::PostNcDestroy() 
{
}

BOOL CExtDynamicRichEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	ASSERT_VALID( this );
	cs.lpszClass = NULL;
LONG nStylesSaved =	cs.style & (WS_POPUP|WS_CHILD);
	cs.style &= ~(WS_POPUP);
	cs.style |= WS_CHILD;
	if( ! CRichEditView::PreCreateWindow(cs) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	cs.style &= ~(WS_POPUP|WS_CHILD|WS_BORDER|WS_DLGFRAME);
	cs.style |= nStylesSaved;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME|WS_EX_WINDOWEDGE);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CExtPopupRichEditView

IMPLEMENT_DYNCREATE( CExtPopupRichEditView, CExtDynamicRichEditView );

CExtPopupRichEditView::CExtPopupRichEditView()
	: m_hWndNotify( NULL )
	, m_sizeLayout( 0, 0 )
	, m_clrBackground( COLORREF(-1L) )
	, m_clrBorderLT( COLORREF(-1L) )
	, m_clrBorderRB( COLORREF(-1L) )
	, m_eST( CExtPopupRichEditView::__EST_POPUP )
{
}

CExtPopupRichEditView::~CExtPopupRichEditView()
{
}

BEGIN_MESSAGE_MAP( CExtPopupRichEditView, CExtDynamicRichEditView )
	//{{AFX_MSG_MAP(CExtPopupRichEditView)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_CANCELMODE()
	ON_WM_ACTIVATEAPP()
	//}}AFX_MSG_MAP
	ON_WM_SYSCOLORCHANGE()
	__EXT_MFC_SAFE_ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_MESSAGE(__ExtMfc_WM_THEMECHANGED, OnThemeChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtPopupRichEditView message handlers

void CExtPopupRichEditView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	ASSERT_VALID( this );
//	CExtDynamicRichEditView::OnNcCalcSize(bCalcValidRects, lpncsp);
	bCalcValidRects;
	if( (GetStyle()&WS_VISIBLE) == 0 )
		return;
int nShadowSize = OnQueryShadowSize();
CRect rcBorderSizes = OnQueryBorderSizes();
	lpncsp->rgrc[0].left += rcBorderSizes.left;
	lpncsp->rgrc[0].top += rcBorderSizes.top;
	lpncsp->rgrc[0].right -= rcBorderSizes.right + nShadowSize;
	lpncsp->rgrc[0].bottom -= rcBorderSizes.bottom + nShadowSize;
}

void CExtPopupRichEditView::OnNcPaint() 
{
	ASSERT_VALID( this );
int nShadowSize = OnQueryShadowSize();
CWindowDC dc( this );
CRect rcClient, rcBar;
	GetClientRect( rcClient );
	ClientToScreen( rcClient );
	GetWindowRect( rcBar );
	rcClient.OffsetRect( -rcBar.TopLeft() );
	rcBar.OffsetRect( -rcBar.TopLeft() );
	dc.ExcludeClipRect(rcClient);
CRect rcWnd = rcBar, rcBorders = rcBar;
	rcBorders.DeflateRect(
		0,
		0,
		nShadowSize,
		nShadowSize
		);
CExtMemoryDC mdc(
		&dc,
		&rcBar,
		CExtMemoryDC::MDCOPT_RTL_COMPATIBILITY
			|CExtMemoryDC::MDCOPT_TO_MEMORY
			| CExtMemoryDC::MDCOPT_FILL_BITS
		);
	mdc.FillSolidRect(
		&rcBorders,
		( m_clrBackground == COLORREF(-1L) )
			? ( ::GetSysColor( COLOR_INFOBK ) )
			: m_clrBackground
		);
	OnPaintNcAreaParts(
		mdc,
		rcWnd,
		rcClient,
		rcBorders
		);
}

bool CExtPopupRichEditView::Activate(
	CExtResourceManager::CExtResourceMemoryBuffer & buffer,
	const RECT & rcItem,
	HWND hWndNotify,
	UINT nAlignType,
	int nAlignDistance // = 30
	)
{
	ASSERT_VALID( this );
	ASSERT( hWndNotify != NULL && ::IsWindow( hWndNotify ) );
	ASSERT(
			nAlignType == AFX_IDW_DOCKBAR_TOP
		||	nAlignType == AFX_IDW_DOCKBAR_BOTTOM
		||	nAlignType == AFX_IDW_DOCKBAR_LEFT
		||	nAlignType == AFX_IDW_DOCKBAR_RIGHT
		);
	ASSERT( nAlignDistance >= 0 );
	Deactivate();
	if( buffer.IsEmpty() )
		return true;
	if(	! CreateEx(
			WS_EX_TOPMOST,
			_T("RICHEDIT"),
			NULL,
			WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN
				| ( g_PaintManager.m_bIsWin2000orLater
						? (WS_EX_NOINHERITLAYOUT)
						: 0
					)
				|ES_LEFT|ES_MULTILINE|ES_READONLY|ES_WANTRETURN
				,
			0, 0, 0, 0,
			hWndNotify,
			(HMENU)NULL
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}

CRichEditCtrl & wndRichEditCtrl = GetRichEditCtrl();

CHARFORMAT cfDefault;
	::memset( &cfDefault, 0, sizeof(CHARFORMAT) );
	cfDefault.cbSize = sizeof(cfDefault);
	cfDefault.dwEffects = CFE_PROTECTED; 
	cfDefault.dwMask =
		CFM_BOLD|CFM_FACE|CFM_SIZE|CFM_CHARSET|CFM_PROTECTED|CFM_COLOR;
	cfDefault.yHeight = 200;
	cfDefault.bCharSet = DEFAULT_CHARSET;
#if _MFC_VER < 0x700
	strcpy(
		cfDefault.szFaceName,
		"Tahoma"
		); 
#else
	__EXT_MFC_STRCPY(
		cfDefault.szFaceName,
		LF_FACESIZE, 
		_T("Tahoma")
		); 
#endif
	cfDefault.crTextColor = ::GetSysColor( COLOR_INFOTEXT );
	wndRichEditCtrl.SetDefaultCharFormat( cfDefault );
	wndRichEditCtrl.SetWordCharFormat( cfDefault );
	if( m_clrBackground == COLORREF(-1L) )
		wndRichEditCtrl.SetBackgroundColor( FALSE, ::GetSysColor( COLOR_INFOBK ) );
	else
		wndRichEditCtrl.SetBackgroundColor( FALSE, m_clrBackground );
	
	m_hWndNotify = hWndNotify;
CRect _rcItem = rcItem;
	if( (::GetWindowLong(hWndNotify,GWL_EXSTYLE)&WS_EX_LAYOUTRTL) != 0 )
		__EXT_MFC_SwapLeftRight( _rcItem );
CRect rcWndPreCalc = _rcItem;
	::ClientToScreen( hWndNotify, ((LPPOINT)(&rcWndPreCalc)) );
	::ClientToScreen( hWndNotify, ((LPPOINT)(&rcWndPreCalc))+1 );
CRect rcWndItem = rcWndPreCalc;
CSize sizeItem = rcWndItem.Size();
	SetupRTForPlain( buffer );
	rcWndPreCalc.right = rcWndPreCalc.left + m_sizeLayout.cx;
	rcWndPreCalc.bottom = rcWndPreCalc.top + m_sizeLayout.cy;
int nShadowSize = OnQueryShadowSize();
CRect rcBorderSizes = OnQueryBorderSizes();
	rcWndPreCalc.right += rcBorderSizes.left + rcBorderSizes.right + nShadowSize;
	rcWndPreCalc.bottom += rcBorderSizes.top + rcBorderSizes.bottom + nShadowSize;
CRect rcSet = rcWndPreCalc;
	rcSet.OffsetRect( -rcSet.TopLeft() );
CRect rcWndFinal = rcWndPreCalc;
CExtPaintManager::monitor_parms_t _mp;
	CExtPaintManager::stat_GetMonitorParms( _mp, rcWndPreCalc );
CRect rcWorkArea = _mp.m_rcWorkArea; // _mp.m_rcMonitor
bool bRecalcToMaxDistance = false, bStopFlag = false;
UINT nAlignTypeRecalc = nAlignType;
	for( int nStep = 0; nStep < 4 && (!bStopFlag); nStep++ )
	{
		rcWndPreCalc = rcWndFinal;
		if( nStep == 3 )
		{
			bRecalcToMaxDistance = true;
			break;
		}
		switch( nAlignTypeRecalc )
		{
		case AFX_IDW_DOCKBAR_TOP:
			rcWndPreCalc.OffsetRect( 0, -rcWndPreCalc.Height() - nAlignDistance );
			if( rcWndPreCalc.top >= rcWorkArea.top )
			{
				bStopFlag = true;
				break;
			}
			switch( nStep )
			{
			case 0:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_BOTTOM;
			break;
			case 1:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_LEFT;
			break;
			case 2:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_BOTTOM;
			break;
			}; // switch( nStep )
		break;
		case AFX_IDW_DOCKBAR_BOTTOM:
			rcWndPreCalc.OffsetRect( 0, sizeItem.cy + nAlignDistance );
			if( rcWndPreCalc.bottom < rcWorkArea.bottom )
			{
				bStopFlag = true;
				break;
			}
			switch( nStep )
			{
			case 0:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_TOP;
			break;
			case 1:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_LEFT;
			break;
			case 2:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_RIGHT;
			break;
			}; // switch( nStep )
		break;
		case AFX_IDW_DOCKBAR_LEFT:
			rcWndPreCalc.OffsetRect( -rcWndPreCalc.Width() - nAlignDistance, 0 );
			if( rcWndPreCalc.left >= rcWorkArea.left )
			{
				bStopFlag = true;
				break;
			}
			switch( nStep )
			{
			case 0:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_RIGHT;
			break;
			case 1:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_TOP;
			break;
			case 2:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_RIGHT;
			break;
			}; // switch( nStep )
		break;
		case AFX_IDW_DOCKBAR_RIGHT:
			rcWndPreCalc.OffsetRect( sizeItem.cx + nAlignDistance, 0 );
			if( rcWndPreCalc.right < rcWorkArea.right )
			{
				bStopFlag = true;
				break;
			}
			switch( nStep )
			{
			case 0:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_LEFT;
			break;
			case 1:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_TOP;
			break;
			case 2:
				nAlignTypeRecalc = AFX_IDW_DOCKBAR_BOTTOM;
			break;
			}; // switch( nStep )
		break;
#ifdef _DEBUG
		default:
			{
				ASSERT( FALSE );
			}
		break;
#endif // _DEBUG
		} // switch( nAlignTypeRecalc )
	} // for( int nStep = 0; nStep < 4 && (!bStopFlag); nStep++ )
	if( bRecalcToMaxDistance )
	{
		CRect rcMaxDistance(
			rcWndItem.left - rcWorkArea.left,
			rcWndItem.top - rcWorkArea.top,
			rcWndItem.right - rcWndPreCalc.right,
			rcWndItem.bottom - rcWndPreCalc.bottom
			);
		if(		nAlignType == AFX_IDW_DOCKBAR_TOP
			||	nAlignType == AFX_IDW_DOCKBAR_BOTTOM
			)
		{
			if( rcMaxDistance.top > rcMaxDistance.bottom )
			{
				rcWndPreCalc.OffsetRect( 0, -rcWndPreCalc.Height() - nAlignDistance );
				if( rcWndPreCalc.top < rcWorkArea.top )
					rcWndPreCalc.OffsetRect( 0, rcWorkArea.top - rcWndPreCalc.top );
			}
			else
			{
				rcWndPreCalc.OffsetRect( 0, sizeItem.cy + nAlignDistance );
				if( rcWndPreCalc.bottom > rcWorkArea.bottom )
					rcWndPreCalc.OffsetRect( 0, rcWorkArea.bottom - rcWndPreCalc.bottom );
			}
		}
		else
		{
			if( rcMaxDistance.left > rcMaxDistance.right )
			{
				rcWndPreCalc.OffsetRect( -rcWndPreCalc.Width() - nAlignDistance, 0 );
				if( rcWndPreCalc.left < rcWorkArea.left )
					rcWndPreCalc.OffsetRect( rcWorkArea.left - rcWndPreCalc.left, 0 );
			}
			else
			{
				rcWndPreCalc.OffsetRect( sizeItem.cx + nAlignDistance, 0 );
				if( rcWndPreCalc.right > rcWorkArea.right )
					rcWndPreCalc.OffsetRect( rcWorkArea.right - rcWndPreCalc.right, 0 );
			}
		}
	} // if( bRecalcToMaxDistance )
	if( rcWndPreCalc.right > rcWorkArea.right )
		rcWndPreCalc.OffsetRect( rcWorkArea.right - rcWndPreCalc.right, 0 );
	if( rcWndPreCalc.left < rcWorkArea.left )
		rcWndPreCalc.OffsetRect( rcWorkArea.left - rcWndPreCalc.left, 0 );
	if( rcWndPreCalc.bottom > rcWorkArea.bottom )
		rcWndPreCalc.OffsetRect( 0, rcWorkArea.bottom - rcWndPreCalc.bottom );
	if( rcWndPreCalc.top < rcWorkArea.top )
		rcWndPreCalc.OffsetRect( 0, rcWorkArea.top - rcWndPreCalc.top );
	
	rcWndFinal = rcWndPreCalc;

	theApp.m_hWndLastPopup = m_hWnd;
	SetWindowPos(
		&CWnd::wndTopMost,
		rcWndFinal.left,
		rcWndFinal.top,
		rcWndFinal.Width(),
		rcWndFinal.Height(),
		SWP_NOOWNERZORDER|SWP_NOACTIVATE
			|SWP_SHOWWINDOW|SWP_FRAMECHANGED
		);
	wndRichEditCtrl.SetRect( &rcSet );
	SetWindowPos(
		NULL,
		0,
		0,
		0,
		0,
		SWP_NOOWNERZORDER|SWP_NOZORDER
			|SWP_NOACTIVATE|SWP_FRAMECHANGED
			|SWP_NOMOVE|SWP_NOSIZE
		);
	UpdateWindow();

	return true;
}

void CExtPopupRichEditView::Deactivate()
{
	ASSERT_VALID( this );
	if( GetSafeHwnd() != NULL )
	{
		ShowWindow( SW_HIDE	);
		ASSERT( !IsWindowVisible() );
		CRect rcWndOld;
		GetWindowRect( &rcWndOld );
		CWnd * pWndTLP = GetTopLevelParent();
		if( pWndTLP != NULL )
		{
			ASSERT_VALID( pWndTLP );
			if( pWndTLP->IsWindowVisible() )
			{
				CRect rcTLP;
				pWndTLP->GetWindowRect( &rcTLP );
				if( !rcTLP.IsRectEmpty() )
				{
					CRect rcIntersection;
					if(	rcIntersection.IntersectRect(
							&rcTLP,
							&rcWndOld
							)
						)
					{
						pWndTLP->RedrawWindow(
							NULL,
							NULL,
							RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW
							);
					}
				} // if( !rcTLP.IsRectEmpty() )
			} // if( pWndTLP->IsWindowVisible() )
		} // if( pWndTLP != NULL )
		CWnd * pWnd = CWnd::FromHandlePermanent( m_hWndNotify );
		if( pWnd != NULL )
		{
			CFrameWnd * pDockSite =
				DYNAMIC_DOWNCAST( CFrameWnd, pWnd );
			if(		pDockSite != NULL
				&&	(! pDockSite->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
				)
				CExtControlBar::stat_RedrawFloatingFrames( pDockSite );
		}
		DestroyWindow();
		CExtPaintManager::stat_PassPaintMessages();
	}
	m_hWndNotify = NULL;
}

void CExtPopupRichEditView::LayoutWnd()
{
	ASSERT_VALID( this );
CRichEditCtrl & wndRichEditCtrl = GetRichEditCtrl();
	wndRichEditCtrl.SetEventMask( ENM_REQUESTRESIZE );
	m_sizeLayout.cx = m_sizeLayout.cy = 0;
int nFirst = 0;
int nLast = ::GetSystemMetrics( SM_CXFULLSCREEN );
int nMax = 0;
	do
	{
		int cx = ( nFirst + nLast ) / 2;
		CRect rc( 0, 0, cx, 1 );
		CRect rcMove = rc;
		MoveWindow( &rc );
		wndRichEditCtrl.SetRect( &rc );
		wndRichEditCtrl.RequestResize();
		if( nMax == 0 )
			nMax = m_sizeLayout.cy;
		if( m_sizeLayout.cy > nMax )
			nFirst = cx + 1;
		else
			nLast = cx - 1;
	}
	while( nFirst < nLast );
	//m_sizeLayout.cx += 4;
}

BOOL CExtPopupRichEditView::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	ASSERT_VALID( this );
	if( message == WM_NOTIFY )
	{
		REQRESIZE * pRequestResizeData = (REQRESIZE*)lParam;
		if( pRequestResizeData->nmhdr.code == EN_REQUESTRESIZE )
		{
			m_sizeLayout.cx = pRequestResizeData->rc.right - pRequestResizeData->rc.left;
			m_sizeLayout.cy = pRequestResizeData->rc.bottom - pRequestResizeData->rc.top;
			return TRUE;
		} // if( pRequestResizeData->nmhdr.code == EN_REQUESTRESIZE )
	} // if( message == WM_NOTIFY )
	return CExtDynamicRichEditView::OnChildNotify(message, wParam, lParam, pLResult);
}

void CExtPopupRichEditView::SetupRTForPlain(
	CExtResourceManager::CExtResourceMemoryBuffer & buffer
	)
{
	ASSERT_VALID( this );
	try
	{
		CMemFile f( buffer.GetBuffer(), buffer.GetSize() );
		CArchive ar(
			&f,
			CArchive::load
			);
		CExtDynamicRichEditDoc * pDoc = GetDocument();
		ar.m_pDocument = pDoc;
		pDoc->Serialize(ar);
		f.Close();
		ar.Close();
	} // try
	catch( CException * pException )
	{
		ASSERT( FALSE );
		pException->Delete();
	} // catch( CException * pException )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )

	LayoutWnd();
}

DWORD CALLBACK CExtPopupRichEditView::stat_StreamUploadCallback(
   DWORD dwCookie,
   LPBYTE bBuff,
   LONG nCountIn,
   LONG * p_nCountOut
   )
{
LPCTSTR strText = (LPCTSTR)dwCookie;
	(*p_nCountOut) = LONG( _tcslen( strText ) ) * LONG( sizeof(TCHAR) );
	if( (*p_nCountOut) > nCountIn )
		(*p_nCountOut) = nCountIn;
	::memcpy( bBuff, strText, (*p_nCountOut) );
	strText += (*p_nCountOut) / sizeof(TCHAR);
	return ( *strText != '\0' ) ? (DWORD)strText : NULL;
}

LRESULT CExtPopupRichEditView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if( message == WM_MOUSEACTIVATE )
		return MA_NOACTIVATEANDEAT;
	if( __EXT_MFC_WM_MOUSEFIRST <= message && message <= __EXT_MFC_WM_MOUSELAST )
		return 0;
	if( WM_KEYFIRST <= message && message <= WM_KEYLAST )
		return 0;
	return CExtDynamicRichEditView::WindowProc(message, wParam, lParam);
}

int CExtPopupRichEditView::OnQueryShadowSize() const
{
	ASSERT_VALID( this );
	if( m_eST == __EST_POPUP )
	{
		int nMenuShadowSize = g_PaintManager->GetMenuShadowSize();
		return nMenuShadowSize;
	}
	if( m_eST == __EST_WINHELP )
		return 6;
	return 0;
}

CRect CExtPopupRichEditView::OnQueryBorderSizes() const
{
	ASSERT_VALID( this );
	return CRect( 7, 4, 7, 4 );
}

void CExtPopupRichEditView::OnPaintNcAreaParts(
	CDC & dc,
	CRect rcWnd,
	CRect rcClient,
	CRect rcBorders
	) const
{
	ASSERT_VALID( this );
	rcWnd;
	rcClient;
	if( m_eST == __EST_POPUP )
	{
		int nShadowSize = OnQueryShadowSize();
		CExtWndShadow _shadow;
		_shadow.Paint(
			NULL,
			dc,
			rcBorders,
			CRect(0,0,0,0),
			CRect(0,0,0,0),
			(UINT)nShadowSize,
			CExtWndShadow::DEF_BRIGHTNESS_MIN,
			CExtWndShadow::DEF_BRIGHTNESS_MAX,
			false
			);
	}
	else if( m_eST == __EST_WINHELP )
	{
		int nShadowSize = OnQueryShadowSize();
		CBitmap	bitmapShadow;
		CBrush brushShadow;
		static int g_arrBmpPattern[] =
			{ 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };
		bitmapShadow.CreateBitmap( 8, 8, 1, 1, g_arrBmpPattern );
		brushShadow.CreatePatternBrush( &bitmapShadow );
		dc.SetBkColor( RGB(255,255,255) );
		brushShadow.UnrealizeObject();
		CBrush * pbrOld = (CBrush *)
				dc.SelectObject( &brushShadow );
			dc.PatBlt(
			rcBorders.left + nShadowSize,
			rcBorders.bottom,
			rcBorders.Width(),
			nShadowSize,
			0xA000C9
			);
		dc.PatBlt(
			rcBorders.right,
			rcBorders.top + nShadowSize,
			nShadowSize,
			rcBorders.Height(),
			0xA000C9
			);
		dc.SelectObject( pbrOld );
	}
COLORREF clrBorderLT =
		( m_clrBorderLT == COLORREF(-1L) )
			? ( ::GetSysColor( COLOR_3DFACE ) )
			: m_clrBorderLT
		;
COLORREF clrBorderRB =
		( m_clrBorderRB == COLORREF(-1L) )
			? ( ::GetSysColor( COLOR_3DDKSHADOW ) )
			: m_clrBorderRB
		;
	dc.Draw3dRect(
		&rcBorders,
		clrBorderLT,
		clrBorderRB
		);
}

void CExtPopupRichEditView::OnCancelMode() 
{
	CExtDynamicRichEditView::OnCancelMode();
	Deactivate();
}

#if _MFC_VER < 0x700
void CExtPopupRichEditView::OnActivateApp(BOOL bActive, HTASK hTask) 
#else
void CExtPopupRichEditView::OnActivateApp(BOOL bActive, DWORD hTask) 
#endif
{
	CExtDynamicRichEditView::OnActivateApp(bActive, hTask);
	if( ! bActive )
		SendMessage( WM_CANCELMODE );
}

void CExtPopupRichEditView::OnSysColorChange() 
{
	ASSERT_VALID( this );
	CExtDynamicRichEditView::OnSysColorChange();
	SendMessage( WM_CANCELMODE );
}

LRESULT CExtPopupRichEditView::OnDisplayChange( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
LRESULT lResult = CExtDynamicRichEditView::OnDisplayChange( wParam, lParam );
	SendMessage( WM_CANCELMODE );
	return lResult;
}

LRESULT CExtPopupRichEditView::OnThemeChanged( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
LRESULT lResult = Default();
	g_PaintManager.OnThemeChanged( this, wParam, lParam );
	g_CmdManager.OnThemeChanged( g_PaintManager.GetPM(), this, wParam, lParam );
	SendMessage( WM_CANCELMODE );
	return lResult;
}

void CExtPopupRichEditView::OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection) 
{
	ASSERT_VALID( this );
	CExtDynamicRichEditView::OnSettingChange(uFlags, lpszSection);
	g_PaintManager.OnSettingChange( this, uFlags, lpszSection );
	g_CmdManager.OnSettingChange( g_PaintManager.GetPM(), this, uFlags, lpszSection );
	SendMessage( WM_CANCELMODE );
}
