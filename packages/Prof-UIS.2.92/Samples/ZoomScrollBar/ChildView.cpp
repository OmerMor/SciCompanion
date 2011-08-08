// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "ZoomScrollBar.h"
#include "ChildView.h"
#include "MainFrm.h"
#include "Resources/resource.h"
#include "resource.h"

#if (! defined __VSSYM32_H__)
	#include <vssym32/vssym32.h>
#endif // (! defined __VSSYM32_H__)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

IMPLEMENT_DYNAMIC( CChildView, CExtScrollWnd );

const COLORREF CChildView::g_arrClrBk[ 4 ] =
{
	RGB(230,230,230),
	RGB(192,192,0),
	RGB(0,0,164),
	RGB(192,0,0),
};

const double CChildView::g_arrVZF[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ] =
{
	(__ZOOM_FACTOR_MULTIPLIER/15.0),
	(__ZOOM_FACTOR_MULTIPLIER/75.0),
	(__ZOOM_FACTOR_MULTIPLIER/150.0),
	(0.75),
};

const INT CChildView::g_arrRadiuses[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ] =
{
	2,
	3,
	5,
	9,
};

const INT CChildView::g_arrDwmGlowSizes[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ] =
{
	2,
	5,
	10,
	15,
};

CChildView::CChildView()
	: m_lfZoomFactor( 1.0 )
	, m_rcHelperZoomRectPadding( 31, 31, 31, 31 )
	, m_rcHelperDrawZoomRect( 0, 0, 0, 0 )
	, m_nHT( -1 )
	, m_eSTLT( CChildView::__ESTLT_BY_ZOOM )
	, m_eSTOT( CChildView::__ESTOT_ALPHA_BMP )
	, m_bEnableVistaTextRendering( g_PaintManager.m_bIsWinVistaOrLater && g_PaintManager.m_DWM.IsCompositionEnabled() )
	, m_bEnableHalftoning( g_PaintManager.m_bIsWinNT )
	, m_ptLastStatusLL( -1, -1 )
{
	m_bClientCB = true;
	m_wndZoomer.m_bEnabledTrackingToolTips = true;
	VERIFY(
		g_ResourceManager->LoadString(
			m_strEmptyLocationStatusText,
			ID_INDICATOR_LOCATION
			)
		);
	VERIFY(
		m_bmpHelperZoomRectHighlight.LoadBMP_Resource(
			MAKEINTRESOURCE(IDB_BITMAP_ZOOM_RECT_HIGHLIGHT)
			)
		);
	VERIFY(
		m_bmp.LoadBMP_Resource(
			MAKEINTRESOURCE( IDB_BITMAP_GERMANY )
			)
		);
CExtResourceManager::CExtResourceMemoryBuffer _buffer;
	if( g_ResourceManager->LoadResourceBuffer(
			_buffer,
			_T("dat"),
			IDR_DAT1
			)
		)
	{
		CString s;
		wchar_t _line[512];
		::memset( _line, 0, sizeof(_line) );
		INT nLineIndex = 0, nCharIndex, nCharCount = INT( _buffer.GetSize() );
		for( nCharIndex = 0; nCharIndex < nCharCount; nCharIndex ++ )
		{
			BYTE _byte1 = _buffer.GetBuffer()[nCharIndex++];
			if( nCharIndex >= nCharCount )
				break;
			BYTE _byte2 = _buffer.GetBuffer()[nCharIndex];
			wchar_t _wchr = wchar_t( (WORD(_byte2)<<8)|(WORD(_byte1)) );
			if( _wchr == 13 )
				continue;
			if( _wchr != 10 )
			{
				ASSERT( nLineIndex < (sizeof(_line)/sizeof(_line[0])) );
				_line[ nLineIndex ++ ] = _wchr;
				continue;
			} // if( _byte != 10 )
			s = _line;
			if( ! s.IsEmpty() )
			{
				GEODESIC_OBJECT _object;
				if( _object.ScanLine( s ) )
					m_arrObjects.Add( _object );
			} // if( ! s.IsEmpty() )
			nLineIndex = 0;
			::memset( _line, 0, sizeof(_line) );
		} // for( nCharIndex = 0; nCharIndex < nCharCount; nCharIndex ++ )
		s = _line;
		if( ! s.IsEmpty() )
		{
			GEODESIC_OBJECT _object;
			if( _object.ScanLine( s ) )
				m_arrObjects.Add( _object );
		} // if( ! s.IsEmpty() )
	}

static LPCTSTR g_arrObjectBmpResourceIDs[ 4 ] =
{
	MAKEINTRESOURCE(IDB_OBJECT0),
	MAKEINTRESOURCE(IDB_OBJECT1),
	MAKEINTRESOURCE(IDB_OBJECT2),
	MAKEINTRESOURCE(IDB_OBJECT3),
};
INT nBmpIndex, nBmpCount = INT( sizeof(g_arrObjectBmpResourceIDs)/sizeof(g_arrObjectBmpResourceIDs[0]) );
	for( nBmpIndex = 0; nBmpIndex < nBmpCount; nBmpIndex ++ )
	{
		VERIFY( m_arrObjectBmps[nBmpIndex].LoadBMP_Resource( g_arrObjectBmpResourceIDs[nBmpIndex] ) );
		m_arrObjectBmpSize[nBmpIndex] = m_arrObjectBmps[nBmpIndex].GetSize();
		m_arrObjectBmpOffsets[nBmpIndex].x = m_arrObjectBmpSize[nBmpIndex].cx / 2;
		m_arrObjectBmpOffsets[nBmpIndex].y = m_arrObjectBmpSize[nBmpIndex].cy / 2;
	} // for( nBmpIndex = 0; nBmpIndex < nBmpCount; nBmpIndex ++ )

}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP( CChildView, CExtScrollWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_COMMAND( ID_HIDE_ALL_TEXT_LABELS, OnHideAllTextLabels )
	ON_UPDATE_COMMAND_UI( ID_HIDE_ALL_TEXT_LABELS, OnUpdateHideAllTextLabels )
	ON_COMMAND( ID_SHOW_TEXT_LABELS_WITH_ZOOM, OnShowTextLabelsWithZooming )
	ON_UPDATE_COMMAND_UI( ID_SHOW_TEXT_LABELS_WITH_ZOOM, OnUpdateShowTextLabelsZooming )
	ON_COMMAND( ID_SHOW_ALL_TEXT_LABELS, OnShowAllTextLabels )
	ON_UPDATE_COMMAND_UI( ID_SHOW_ALL_TEXT_LABELS, OnUpdateShowAllTextLabels )
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_VISTA_TEXT_RENDERING, OnVistaTextRendering)
	ON_UPDATE_COMMAND_UI(ID_VISTA_TEXT_RENDERING, OnUpdateVistaTextRendering)
	ON_COMMAND(ID_HALFTONING, OnHalftoning)
	ON_UPDATE_COMMAND_UI(ID_HALFTONING, OnUpdateHalftoning)
	ON_COMMAND(ID_OBJECT_HIDE_ALL, OnObjectHideAll)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_HIDE_ALL, OnUpdateObjectHideAll)
	ON_COMMAND(ID_OBJECT_ELLIPSE_FLAT, OnObjectEllipseFlat)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ELLIPSE_FLAT, OnUpdateObjectEllipseFlat)
	ON_COMMAND(ID_OBJECT_ELLIPSE_CIRCLE, OnObjectEllipseCircle)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ELLIPSE_CIRCLE, OnUpdateObjectEllipseCircle)
	ON_COMMAND(ID_OBJECT_ELLIPSE_SOLID, OnObjectEllipseSolid)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ELLIPSE_SOLID, OnUpdateObjectEllipseSolid)
	ON_COMMAND(ID_OBJECT_ALPHA_BMP, OnObjectAlphaBmp)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_ALPHA_BMP, OnUpdateObjectAlphaBmp)
	ON_COMMAND(ID_SHOW_TRACKING_TOOLTIP_ON_ZOOM_SCROLLBAR, OnShowTrackingTooltipOnZoomScrollbar)
	ON_UPDATE_COMMAND_UI(ID_SHOW_TRACKING_TOOLTIP_ON_ZOOM_SCROLLBAR, OnUpdateShowTrackingTooltipOnZoomScrollbar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ! CExtWRB < CExtScrollWnd > :: PreCreateWindow( cs ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME);
	cs.style &= ~WS_BORDER;
	cs.lpszClass =
		::AfxRegisterWndClass(
		CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor( NULL, IDC_ARROW ),
		NULL, NULL
		);
	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CExtWRB < CExtScrollWnd > :: OnCreate( lpCreateStruct ) == -1 )
	{
		ASSERT( FALSE );
		return -1;
	}
	if( (GetExStyle()&WS_EX_LAYOUTRTL) != 0 )
		ModifyStyleEx( WS_EX_LAYOUTRTL, 0, SWP_FRAMECHANGED );

	m_wndScrollBarH.m_eSO = CExtScrollBar::__ESO_BOTTOM;
	m_wndScrollBarV.m_eSO = CExtScrollBar::__ESO_RIGHT;
	if( ! m_wndScrollBarV.Create(
			WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_RIGHTALIGN,
			CRect(0,0,0,0),
			this,
			1
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	if( ! m_wndScrollBarH.Create(
			WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_BOTTOMALIGN,
			CRect(0,0,0,0),
			this,
			2
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	m_wndScrollBarH.SyncReservedSpace( &m_wndScrollBarV );
	m_wndScrollBarV.SyncReservedSpace( &m_wndScrollBarH );

CMainFrame * pMainFrame = _GetMainFrame();
	ASSERT_VALID( pMainFrame );
	ASSERT( pMainFrame->GetSafeHwnd() != NULL );
	ASSERT( pMainFrame->m_wndStatusBar.GetSafeHwnd() != NULL );
	if( ! m_wndZoomer.Create(
			WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|SBS_HORZ,
			CRect( 0, 0, __ZOOM_EXTENT_PX, 20 ),
			&(pMainFrame->m_wndStatusBar),
			ID_ZOOM_SCROLL_BAR_PANE
			)
		)
	{
		ASSERT( FALSE );
		return false;
	}
	m_wndZoomer.m_eSO = CExtScrollBar::__ESO_BOTTOM;
SCROLLINFO _si = { sizeof(SCROLLINFO), SIF_ALL, 0, __ZOOM_EXTENT_PX, __ZOOM_EXTENT_PX/20, __ZOOM_EXTENT_PX/2, __ZOOM_EXTENT_PX/2 };
	VERIFY( m_wndZoomer.SetScrollInfo( &_si ) );
	m_wndZoomer.m_bEnabledToolTips         = true;
	m_wndZoomer.m_strTipTextForThumbButton = _T("Zoom");
	m_wndZoomer.m_strTipTextForUpButton    = _T("Zoom Out");
	m_wndZoomer.m_strTipTextForDownButton  = _T("Zoom In");
	_UpdateZoomerTT();
	pMainFrame->m_wndStatusBar.SetPaneControl(
		&m_wndZoomer,
		ID_ZOOM_SCROLL_BAR_PANE,
		false
		);
	pMainFrame->m_wndStatusBar.SetPaneWidth(
		pMainFrame->m_wndStatusBar.CommandToIndex( ID_ZOOM_SCROLL_BAR_PANE ),
		__ZOOM_EXTENT_PX
		);

	OnSwRecalcLayout( true );

	return 0;
}

CScrollBar* CChildView::GetScrollBarCtrl(int nBar) const
{
	ASSERT_VALID( this );
	if( m_hWnd == NULL || (! ::IsWindow(m_hWnd) ) )
		return NULL;
	ASSERT( nBar == SB_HORZ || nBar == SB_VERT );
	if( nBar == SB_HORZ )
	{
		if( m_wndScrollBarH.GetSafeHwnd() != NULL )
			return ( const_cast < CExtScrollBar * > ( &m_wndScrollBarH ) );
	} // if( nBar == SB_HORZ )
	else
	{
		if( m_wndScrollBarV.GetSafeHwnd() != NULL )
			return ( const_cast < CExtScrollBar * > ( &m_wndScrollBarV ) );
	} // else from if( nBar == SB_HORZ )
	return NULL;
}

void CChildView::PostNcDestroy() 
{
	m_rcHelperDrawZoomRect.SetRect( 0, 0, 0, 0 );
	m_nHT = -1;
	m_wndCoolTip.Hide();
	_StatusLL_Clear();
	m_bmpCache.Empty();
	m_ptLastStatusLL.x = m_ptLastStatusLL.y = -1;
	CExtWRB < CExtScrollWnd > :: PostNcDestroy();
}

void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_nHT = -1;
	m_wndCoolTip.Hide();
	_StatusLL_Clear();
	if( pScrollBar == (&m_wndZoomer) )
	{
		_SetZoomFromZoomer(
			( nSBCode == SB_THUMBTRACK ) ? true : false
			);
		return;
	} // if( pScrollBar == (&m_wndZoomer) )
	m_bmpCache.Empty();
	m_ptLastStatusLL.x = m_ptLastStatusLL.y = -1;
	CExtWRB < CExtScrollWnd > :: OnHScroll( nSBCode, nPos, pScrollBar );
}

void CChildView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_nHT = -1;
	m_wndCoolTip.Hide();
	_StatusLL_Clear();
	if( pScrollBar == (&m_wndZoomer) )
	{
		_SetZoomFromZoomer(
			( nSBCode == SB_THUMBTRACK ) ? true : false
			);
		return;
	} // if( pScrollBar == (&m_wndZoomer) )
	m_bmpCache.Empty();
	m_ptLastStatusLL.x = m_ptLastStatusLL.y = -1;
	CExtWRB < CExtScrollWnd > :: OnVScroll( nSBCode, nPos, pScrollBar );
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	m_bmpCache.Empty();
	m_ptLastStatusLL.x = m_ptLastStatusLL.y = -1;
	CExtWRB < CExtScrollWnd > :: OnSize( nType, cx, cy );
	OnSwRecalcLayout( true );
	OnSwUpdateWindow();
}

void CChildView::OnTimer(__EXT_MFC_UINT_PTR nIDEvent) 
{
	if( nIDEvent == 123 )
	{
		CPoint pt;
		if( ! ::GetCursorPos( &pt ) )
		{
			KillTimer( nIDEvent );
			_StatusLL_Clear();
			return;
		}
		HWND hWndFromPoint = ::WindowFromPoint( pt );
		if(		hWndFromPoint != m_hWnd
			&&	hWndFromPoint != m_wndCoolTip.m_hWnd
			)
		{
			KillTimer( nIDEvent );
			_StatusLL_Clear();
			return;
		}
// 		if(		( ! ::ScreenToClient( m_hWnd, &pt ) )
// 			||	( ! _StatusLL_Set( NULL, pt ) )
// 			)
// 		{
// 			KillTimer( nIDEvent );
// 			_StatusLL_Clear();
// 			return;
// 		}
		return;
	} // if( nIDEvent == 123 )
	CExtWRB < CExtScrollWnd > :: OnTimer(nIDEvent);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	CExtWRB < CExtScrollWnd > :: OnLButtonDown(nFlags, point);
	nFlags;
	_AdjustHT( point );
	if( m_nHT >= 0 )
	{
		const GEODESIC_OBJECT & _object = GetObjectAt( m_nHT );
		if( ! _object.m_strURL.IsEmpty() )
		{
			CExtHyperLinkButton::stat_HyperLinkOpen(
				LPCTSTR(_object.m_strURL)
				);
			::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
			return;
		}
	} // if( m_nHT >= 0 )
double lfMax = double( __ZOOM_EXTENT_PX - __ZOOM_EXTENT_PX/2 ) / double( __ZOOM_EXTENT_PX/2 + 1 );
	lfMax *= lfMax;
	lfMax *= __ZOOM_FACTOR_MULTIPLIER;
	lfMax += 1.0;
	if( m_lfZoomFactor > lfMax )
		return;
CRect rcAvailableRectTrackSpace = _AvailableRectTrackSpaceGet();
::SetCursor( ::AfxGetApp()->LoadCursor( IDC_EXT_ZOOM_PLUS ) );
	CRect rcTracked = _TrackZoomRect( point, rcAvailableRectTrackSpace, false );
::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
	if( rcTracked.IsRectEmpty() )
		return;
CSize sizeScrollOld = OnSwGetTotalSize();
CRect rcClient = OnSwGetClientRect();
	rcClient.OffsetRect( -rcClient.TopLeft() );
CSize _sizeClient = rcClient.Size();
double lfZoomChangingX = double( rcClient.Width() ) / double( rcTracked.Width() );
double lfZoomChangingY = double( rcClient.Height() ) / double( rcTracked.Height() );
double lfZoomChanging = min( lfZoomChangingX, lfZoomChangingY );
CPoint ptTrackedCenter = rcTracked.CenterPoint() + OnSwGetScrollPos();
	m_lfZoomFactor *= lfZoomChanging; 
	if( m_lfZoomFactor > lfMax )
		m_lfZoomFactor = lfMax;
	m_bmpCache.Empty();
	m_ptLastStatusLL.x = m_ptLastStatusLL.y = -1;
	OnSwUpdateScrollBars();
CSize sizeScrollNew = OnSwGetTotalSize();
CPoint ptScrollPosNew(
		( m_lfZoomFactor < 1.0 )
			? 0
			: ( ::MulDiv(
					ptTrackedCenter.x, // + _sizeClient.cx / 2,
					sizeScrollNew.cx,
					sizeScrollOld.cx
					)
				- _sizeClient.cx / 2
				),
		( m_lfZoomFactor < 1.0 )
			? 0
			: ( ::MulDiv(
					ptTrackedCenter.y, // + _sizeClient.cy / 2,
					sizeScrollNew.cy,
					sizeScrollOld.cy
					)
				- _sizeClient.cy / 2
				)
		);
	ptScrollPosNew.x = max( ptScrollPosNew.x, 0 );
	ptScrollPosNew.y = max( ptScrollPosNew.y, 0 );
	ptScrollPosNew.x = min( ptScrollPosNew.x, sizeScrollNew.cx );
	ptScrollPosNew.y = min( ptScrollPosNew.y, sizeScrollNew.cy );
	OnSwSetScrollPos( ptScrollPosNew );
	OnSwRecalcLayout( true );
	OnSwDoRedraw();
	UpdateWindow();
	// adjust zoomer finally
double lfPos = ::sqrt( ( ::sqrt( ( m_lfZoomFactor - 1.0 ) / __ZOOM_FACTOR_MULTIPLIER ) ) ) * double( __ZOOM_EXTENT_PX/2 + 1 ) + double( __ZOOM_EXTENT_PX/2 );;
INT nPos = UINT( lfPos );
	if( nPos < 0 )
		nPos = 0;
	else if( nPos > __ZOOM_EXTENT_PX )
		nPos = __ZOOM_EXTENT_PX;
	m_wndZoomer.SetScrollPos( nPos );
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CExtWRB < CExtScrollWnd > :: OnMouseMove(nFlags, point);
	_AdjustHT( point );
}

void CChildView::_AdjustHT( CPoint point )
{
	ASSERT_VALID( this );
CRect rcObject;
INT nHT = CChildView::_HitTestObject( point, &rcObject );
	if( m_nHT != nHT )
	{
		m_wndCoolTip.Hide();
		m_nHT = nHT;
		if( m_nHT >= 0 )
		{
			const GEODESIC_OBJECT & _object = GetObjectAt( m_nHT );
			CExtSafeString strTipText;
			strTipText.Format(
				_T("<html><small>Name:</small> %s<br><small>%s</small>%s%s<small>Longitude:</small> %lf<br><small>Latitude:</small> %lf</html>"),
				LPCTSTR( _object.m_strName ),
				_object.m_strURL.IsEmpty() ? _T("") : _T("URL: "),
				_object.m_strURL.IsEmpty() ? _T("") : LPCTSTR( _object.m_strURL ),
				_object.m_strURL.IsEmpty() ? _T("") : _T("<br>"),
				_object.m_gp.lfLongitude,
				_object.m_gp.lfLatitude
				);
			ClientToScreen( &rcObject );
			m_wndCoolTip.SetIcon( NULL );
			m_wndCoolTip.SetText( LPCTSTR(strTipText) );
			if( ! _object.m_bmp.IsEmpty() )
			{
				CExtCmdIcon _icon;
				_icon.m_bmpNormal = _object.m_bmp;
				m_wndCoolTip.SetIcon( _icon );
			} // if( ! _object.m_bmp.IsEmpty() )
			m_wndCoolTip.Show( this, rcObject );
			_StatusLL_Set( _object );
		} // if( m_nHT >= 0 )
		else
			_StatusLL_Set( NULL, point );
	} // if( m_nHT != nHT )
	else
	{
		if( m_nHT >= 0 )
		{
			const GEODESIC_OBJECT & _object = GetObjectAt( m_nHT );
			_StatusLL_Set( _object );
		} // if( m_nHT >= 0 )
		else
			_StatusLL_Set( NULL, point );
	} // else from if( m_nHT != nHT )
	SetTimer( 123, 100, NULL );
}

void CChildView::_StatusLL_Clear()
{
	ASSERT_VALID( this );
	if( GetSafeHwnd() == NULL )
		return;
CMainFrame * pMainFrame = _GetMainFrame();
	ASSERT_VALID( pMainFrame );
	if( pMainFrame->m_wndStatusBar.GetSafeHwnd() == NULL )
		return;
INT nPaneIndex = pMainFrame->m_wndStatusBar.CommandToIndex( ID_INDICATOR_LOCATION );
	if( nPaneIndex < 0 )
		return;
	pMainFrame->m_wndStatusBar.SetPaneText( nPaneIndex, LPCTSTR(m_strEmptyLocationStatusText) );
	pMainFrame->m_wndStatusBar.WidthFromContent( nPaneIndex );
}

bool CChildView::_StatusLL_Set(
	const CChildView::GEODESIC_OBJECT & _object
	)
{
	ASSERT_VALID( this );
	return
		_StatusLL_Set(
			LPCTSTR( _object.m_strName ),
			_object.m_gp.lfLongitude,
			_object.m_gp.lfLatitude
			);
}

bool CChildView::_StatusLL_Set(
	LPCTSTR strName,
	double lfLongitude,
	double lfLatitude
	)
{
	ASSERT_VALID( this );
	if( GetSafeHwnd() == NULL )
		return false;
CMainFrame * pMainFrame = _GetMainFrame();
	ASSERT_VALID( pMainFrame );
	if( pMainFrame->m_wndStatusBar.GetSafeHwnd() == NULL )
		return false;
INT nPaneIndex = pMainFrame->m_wndStatusBar.CommandToIndex( ID_INDICATOR_LOCATION );
	if( nPaneIndex < 0 )
		return false;
CString strPaneTextOld = pMainFrame->m_wndStatusBar.GetPaneText( nPaneIndex );
CString strPaneTextNew;
	if( strName != NULL )
		strPaneTextNew.Format(
			_T("<html><small>Name:</small> %s <small>Longitude:</small> %lf <small>Latitude:</small> %lf</html>"),
			strName,
			lfLongitude,
			lfLatitude
			);
	else
		strPaneTextNew.Format(
			_T("<html><small>Longitude:</small> %lf <small>Latitude:</small> %lf</html>"),
			lfLongitude,
			lfLatitude
			);
	if( strPaneTextNew == strPaneTextOld )
		return true;
	pMainFrame->m_wndStatusBar.SetPaneText( nPaneIndex, LPCTSTR(strPaneTextNew) );
	pMainFrame->m_wndStatusBar.WidthFromContent( nPaneIndex );
	return true;
}

bool CChildView::_StatusLL_Set(
	LPCTSTR strName,
	CPoint ptClient
	)
{
	ASSERT_VALID( this );
	if( GetSafeHwnd() == NULL )
		return false;
	if( m_bmp.IsEmpty() )
	{
		_StatusLL_Clear();
		return false;
	}
	if( m_ptLastStatusLL == ptClient )
		return true;
CPoint ptScrollPos = OnSwGetScrollPos();
CSize _sizeSrc = m_bmp.GetSize();
CSize _sizeDst(
		INT( double(_sizeSrc.cx) * m_lfZoomFactor ),
		INT( double(_sizeSrc.cy) * m_lfZoomFactor )
		);
CRect rcDst( 0, 0, _sizeDst.cx, _sizeDst.cy );
	rcDst.OffsetRect( - ptScrollPos );
	if( ! rcDst.PtInRect( ptClient  ) )
	{
		_StatusLL_Clear();
		return false;
	}
GEODESIC_POINT gp = ClientToGeodesic( ptClient + ptScrollPos );
	if( ! _StatusLL_Set( strName, gp.lfLongitude, gp.lfLatitude ) )
		return false;
	m_ptLastStatusLL = ptClient;
	return true;
}

INT CChildView::_HitTestObject(
	CPoint ptClient,
	RECT * pRectObject // = NULL
	) const
{
	ASSERT_VALID( this );
CRect rcClient = OnSwGetClientRect();
	if( ! rcClient.PtInRect( ptClient ) )
		return -1;
	rcClient.OffsetRect( -rcClient.TopLeft() );
CPoint ptScrollPos = OnSwGetScrollPos();
INT nObjectIndex, nObjectCount = GetObjectCount();
	for( nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex ++ )
	{
		const GEODESIC_OBJECT & _object = GetObjectAt( nObjectIndex );
		ASSERT( 0 <= _object.m_nLevel && _object.m_nLevel <= 3 );
		CPoint ptObject( GeodesicToClient( _object.m_gp ) );
		ptObject -= ptScrollPos;
		INT nRadius = g_arrRadiuses[_object.m_nLevel];
		CRect rcObject( ptObject, ptObject );
		rcObject.InflateRect( nRadius, nRadius );
		if( rcObject.PtInRect( ptClient ) )
		{
			if( pRectObject != NULL )
				::CopyRect( pRectObject, &rcObject );
			return nObjectIndex;
		} // if( rcObject.PtInRect( ptClient ) )
	} // for( nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex ++ )
	return -1;
}

void CChildView::_UpdateZoomerTT()
{
	ASSERT_VALID( this );
	if( ! m_wndZoomer.m_bEnabledTrackingToolTips )
	{
		m_wndZoomer.m_strTipTextForThumbButton = _T("Zoom");
		return;
	}
INT nZoomPaneValue = INT( m_lfZoomFactor * 100.0 );
	m_wndZoomer.m_strTipTextForThumbButton.Format(
		_T("Zoom %3d %%"),
		nZoomPaneValue
		);
}

void CChildView::_SetZoomFromZoomer(
	bool bTrackPos
	)
{
	ASSERT_VALID( this );
CPoint ptScrollPosOld = OnSwGetScrollPos();
CSize sizeScrollOld = OnSwGetTotalSize();
CRect rcClient = OnSwGetClientRect();
	rcClient.OffsetRect( -rcClient.TopLeft() );
CSize _sizeClient = rcClient.Size();
SCROLLINFO _scroll_info;
	::memset( &_scroll_info, 0, sizeof(SCROLLINFO) );
	_scroll_info.cbSize = sizeof(SCROLLINFO);
	if(	! m_wndZoomer.GetScrollInfo(
			&_scroll_info,
			SIF_ALL // SIF_POS|SIF_TRACKPOS
			)
		)
	{
		m_wndZoomer.GetScrollRange(
			&_scroll_info.nMin,
			&_scroll_info.nMax
			);
		_scroll_info.nPage = 0;
		_scroll_info.nTrackPos
			= _scroll_info.nPos
			= m_wndZoomer.GetScrollPos();
	}

INT nScrollPos = bTrackPos ? _scroll_info.nTrackPos : _scroll_info.nPos;
	if( nScrollPos < 0 )
		nScrollPos = 0;
	else if( nScrollPos > __ZOOM_EXTENT_PX )
		nScrollPos = __ZOOM_EXTENT_PX;
	nScrollPos -= __ZOOM_EXTENT_PX/2;
double lfZoomFactorOld = m_lfZoomFactor;
	m_lfZoomFactor = double( nScrollPos ) / double( __ZOOM_EXTENT_PX/2 + 1 );
	if( m_lfZoomFactor > 0.0 )
	{
		m_lfZoomFactor *= m_lfZoomFactor;
		m_lfZoomFactor *= m_lfZoomFactor;
		m_lfZoomFactor *= __ZOOM_FACTOR_MULTIPLIER;
	}
	else
		m_lfZoomFactor /= 1.5;
	m_lfZoomFactor += 1.0;
//	TRACE3( "m_lfZoomFactor = %lf,    _scroll_info.nTrackPos = %d,    _scroll_info.nPos = %d\r\n", m_lfZoomFactor, _scroll_info.nTrackPos, _scroll_info.nPos );
	if( lfZoomFactorOld == m_lfZoomFactor )
		return;
CSize sizeScrollNew = m_bmp.GetSize();
	sizeScrollNew.cx = INT( double(sizeScrollNew.cx) * m_lfZoomFactor );
	sizeScrollNew.cy = INT( double(sizeScrollNew.cy) * m_lfZoomFactor );
CPoint ptScrollPosNew(
		 ::MulDiv(
				ptScrollPosOld.x + _sizeClient.cx / 2,
				sizeScrollNew.cx,
				sizeScrollOld.cx
				)
			- _sizeClient.cx / 2
			,
		 ::MulDiv(
				ptScrollPosOld.y + _sizeClient.cy / 2,
				sizeScrollNew.cy,
				sizeScrollOld.cy
				)
			- _sizeClient.cy / 2
		);
	ptScrollPosNew.x = max( ptScrollPosNew.x, 0 );
	ptScrollPosNew.y = max( ptScrollPosNew.y, 0 );
	ptScrollPosNew.x = min( ptScrollPosNew.x, sizeScrollNew.cx );
	ptScrollPosNew.y = min( ptScrollPosNew.y, sizeScrollNew.cy );
	OnSwUpdateScrollBars();
	OnSwSetScrollPos( ptScrollPosNew );
	OnSwRecalcLayout( true );
	m_wndScrollBarH.Invalidate( TRUE );
	m_wndScrollBarV.Invalidate( TRUE );
	m_wndScrollBarH.UpdateWindow();
	m_wndScrollBarV.UpdateWindow();
	OnSwInvalidate( true );
	//OnSwUpdateWindow();

CMainFrame * pMainFrame = _GetMainFrame();
	ASSERT_VALID( pMainFrame );
INT nPaneIndex = pMainFrame->m_wndStatusBar.CommandToIndex( ID_ZOOM_VALUE_PANE );
	if( nPaneIndex > 0 )
	{
		INT nZoomPaneValue = INT( m_lfZoomFactor * 100.0 );
		CString strZoomPaneValue;
		strZoomPaneValue.Format( _T("<html><p><center>%3d<small>%%</small></center></p></html>"), nZoomPaneValue );
		pMainFrame->m_wndStatusBar.SetPaneText( nPaneIndex, LPCTSTR(strZoomPaneValue) );
		//pMainFrame->m_wndStatusBar.WidthFromContent( nPaneIndex );
	}
	_UpdateZoomerTT();
}

CMainFrame * CChildView::_GetMainFrame() const
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
CFrameWnd * pFrame = GetParentFrame();
	if( pFrame == NULL )
		return NULL;
CMainFrame * pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, pFrame );
	if( pMainFrame != NULL )
		return pMainFrame;
	pFrame = pFrame->GetParentFrame();
	pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, pFrame );
	return pMainFrame;
}

CRect CChildView::_AvailableRectTrackSpaceGet() const
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
CRect rcClient = OnSwGetClientRect();
	rcClient.OffsetRect( -rcClient.TopLeft() );
CRect rcMax( -OnSwGetScrollPos(), OnSwGetTotalSize() );
CRect rc(
		max( rcClient.left, rcMax.left ),
		max( rcClient.top, rcMax.top ),
		min( rcClient.right, rcMax.right ),
		min( rcClient.bottom, rcMax.bottom )
		);
	return rc;
}

HBRUSH CChildView::stat_GetHalftoneLightBrush(
	int nLighterSpec
	)
{
WORD _patternChashHalftone[8];
	if( nLighterSpec == 0 )
	{
		for( int nHTIdx = 0; nHTIdx < 8; nHTIdx++ )
			_patternChashHalftone[nHTIdx] =
				( (nHTIdx & 1) == 0 )
					? ( (WORD) 0 )
					: ( (WORD) (0x1111 << (nHTIdx & 0x03)) )
				;
	} // if( nLighterSpec == 0 )
	else if( nLighterSpec == 1 )
	{
		for( int nHTIdx = 0; nHTIdx < 8; nHTIdx++ )
			_patternChashHalftone[nHTIdx] =
				( (WORD) (0x1111 << (nHTIdx & 0x03)) )
				;
	} // else if( nLighterSpec == 1 )
	else
	{
		for( int nHTIdx = 0; nHTIdx < 8; nHTIdx++ )
			_patternChashHalftone[nHTIdx] =
				( (WORD) (0x5555 << (nHTIdx & 0x01)) )
				;
	} // else 
HBITMAP _bitmapChashHalftone =
		::CreateBitmap(
			8, 8, 1, 1,
			&_patternChashHalftone
			);
	if( _bitmapChashHalftone == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
HBRUSH hHalftoneBrush =
		::CreatePatternBrush( _bitmapChashHalftone );
	::DeleteObject( _bitmapChashHalftone );
	if( hHalftoneBrush == NULL )
	{
		ASSERT( FALSE );
		return NULL;
	}
	return hHalftoneBrush;
}

CRect CChildView::_TrackZoomRect(
	CPoint ptStartTracking,
	CRect rcAvailableRectTrackSpace,
	bool bUseHalftone // = false
	)
{
	ASSERT_VALID( this );
HWND hWndOwn = GetSafeHwnd();
	ASSERT( hWndOwn != NULL );
	if(		rcAvailableRectTrackSpace.left >= rcAvailableRectTrackSpace.right
		||	rcAvailableRectTrackSpace.top >= rcAvailableRectTrackSpace.bottom
		)
		return CRect( 0, 0, 0, 0 );
bool bStopFlag = false;
CRect rcResult( 0, 0, 0, 0 );
	rcResult.left = max( rcAvailableRectTrackSpace.left, ptStartTracking.x );
	rcResult.left = min( rcAvailableRectTrackSpace.right, rcResult.left );
	rcResult.top = max( rcAvailableRectTrackSpace.top, ptStartTracking.y );
	rcResult.top = min( rcAvailableRectTrackSpace.bottom, rcResult.top );
	rcResult.right = rcResult.left;
	rcResult.bottom = rcResult.top;
	SetFocus();
	CExtMouseCaptureSink::SetCapture( hWndOwn );
CBrush _brushHalftone;
	if( bUseHalftone )
	{
		_brushHalftone.Attach( stat_GetHalftoneLightBrush( 1 ) );
		ASSERT( _brushHalftone.GetSafeHandle() != NULL );
	} // if( bUseHalftone )
static const DWORD dwDCX = DCX_CACHE|DCX_LOCKWINDOWUPDATE|DCX_CLIPSIBLINGS|DCX_CLIPCHILDREN;
	for(	MSG msg; ::IsWindow( hWndOwn ) && ( ! bStopFlag ) ; )
	{ // message loop
		if( ! ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if(		( ! ::IsWindow( hWndOwn ) )
				||	bStopFlag
				)
				break;
			for(	LONG nIdleCounter = 0L;
					::AfxGetThread()->OnIdle( nIdleCounter );
					nIdleCounter ++
					);
			::WaitMessage();
			continue;
		} // if( ! ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		switch( msg.message )
		{
		case WM_KILLFOCUS:
			if( msg.hwnd == hWndOwn )
				bStopFlag = true;
		case WM_CANCELMODE:
		case WM_ACTIVATEAPP:
		case WM_SYSCOMMAND:
		case WM_SETTINGCHANGE:
		case WM_SYSCOLORCHANGE:
			bStopFlag = true;
		break;
		case WM_COMMAND:
			if(		(HIWORD(msg.wParam)) == 0
				||	(HIWORD(msg.wParam)) == 1
				)
				bStopFlag = true;
		break;
		case WM_CAPTURECHANGED:
			if( (HWND)msg.wParam != hWndOwn )
				bStopFlag = true;
		break;
		case WM_MOUSEWHEEL:
			if( msg.hwnd != hWndOwn )
				bStopFlag = true;
			else
			{
				::PeekMessage( &msg, NULL, msg.message, msg.message, PM_REMOVE );
				continue;
			} // else from if( msg.hwnd != hWndOwn )
		break;
		case WM_MOUSEMOVE:
			if( msg.hwnd != hWndOwn )
				bStopFlag = true;
			else
			{
				ASSERT_VALID( this );
				::PeekMessage( &msg, NULL, msg.message, msg.message, PM_REMOVE );
				CPoint point;
				point = DWORD( msg.lParam );
				_StatusLL_Set( NULL, point );
				CRect rcNew = rcResult;
				rcNew.right = max( rcAvailableRectTrackSpace.left, point.x );
				rcNew.right = min( rcAvailableRectTrackSpace.right, rcNew.right );
				rcNew.bottom = max( rcAvailableRectTrackSpace.top, point.y );
				rcNew.bottom = min( rcAvailableRectTrackSpace.bottom, rcNew.bottom );
				if( rcResult != rcNew )
				{
					CRect rcDraw = rcResult;
					rcDraw.NormalizeRect();
					if( ! rcDraw.IsRectEmpty() )
					{
						if( bUseHalftone )
						{
							CDC * pDC = GetDCEx( NULL, dwDCX );
							ASSERT( pDC->GetSafeHdc() != NULL );
							CBrush * pBrushOld = pDC->SelectObject( &_brushHalftone );
							pDC->PatBlt(
								rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(),
								PATINVERT
								);
							pDC->SelectObject( pBrushOld );
							ReleaseDC( pDC );
						} // if( bUseHalftone )
						else
						{
							m_rcHelperDrawZoomRect = rcDraw;
							m_rcHelperDrawZoomRect.OffsetRect( OnSwGetScrollPos() );
							OnSwDoRedraw();
							OnSwUpdateWindow();
						} // else from if( bUseHalftone )
					} // if( ! rcDraw.IsRectEmpty() )
					rcResult = rcNew;
					rcDraw = rcResult;
					rcDraw.NormalizeRect();
					if( ! rcDraw.IsRectEmpty() )
					{
						if( bUseHalftone )
						{
							CDC * pDC = GetDCEx( NULL, dwDCX );
							ASSERT( pDC->GetSafeHdc() != NULL );
							CBrush * pBrushOld = pDC->SelectObject( &_brushHalftone );
							pDC->PatBlt(
								rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(),
								PATINVERT
								);
							pDC->SelectObject( pBrushOld );
							ReleaseDC( pDC );
						} // if( bUseHalftone )
						else
						{
							m_rcHelperDrawZoomRect = rcDraw;
							m_rcHelperDrawZoomRect.OffsetRect( OnSwGetScrollPos() );
							OnSwDoRedraw();
							OnSwUpdateWindow();
						} // else from if( bUseHalftone )
					} // if( ! rcDraw.IsRectEmpty() )
				} // if( rcResult != rcNew )
				continue;
			} // else from if( msg.hwnd != hWndOwn )
		break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			bStopFlag = true;
			if( msg.hwnd == hWndOwn )
			{
				ASSERT_VALID( this );
				::PeekMessage( &msg, NULL, msg.message, msg.message, PM_REMOVE );
				continue;
			} // if( msg.hwnd == hWndOwn )
			break;
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_CONTEXTMENU:
		case WM_NCLBUTTONUP:
		case WM_NCLBUTTONDBLCLK:
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONUP:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONUP:
		case WM_NCMBUTTONDBLCLK:
		case WM_NCMBUTTONDOWN:
			bStopFlag = true;
		break;
		default:
			if(	   WM_KEYFIRST <= msg.message
				&& msg.message <= WM_KEYLAST
				)
			{
				Invalidate();
				if( msg.message == WM_KEYDOWN || msg.message == WM_KEYUP )
				{
					if( msg.wParam == VK_SHIFT || msg.wParam == VK_CONTROL )
					{
						::PeekMessage( &msg, NULL, msg.message, msg.message, PM_REMOVE );
						continue;
					} // if( msg.wParam == VK_SHIFT || msg.wParam == VK_CONTROL )
				} // if( msg.message == WM_KEYDOWN || msg.message == WM_KEYUP )
				bStopFlag = true;
			}
		break;
		} // switch( msg.message )
		if( bStopFlag )
		{
			rcResult.SetRect( 0, 0, 0, 0 );
			break;
		}
		if( ! ::AfxGetThread() -> PumpMessage() )
			break;
	} // message loop
	if( ! ::IsWindow( hWndOwn ) )
		return CRect( 0, 0, 0, 0 );
	_StatusLL_Clear();
	CExtMouseCaptureSink::ReleaseCapture();
	rcResult.NormalizeRect();
	m_rcHelperDrawZoomRect.SetRectEmpty();
	if( rcResult.IsRectEmpty() )
		rcResult.SetRect( 0, 0, 0, 0 );
	else
	{
		if( bUseHalftone )
		{
			CDC * pDC = GetDCEx( NULL, dwDCX );
			ASSERT( pDC->GetSafeHdc() != NULL );
			CBrush * pBrushOld = pDC->SelectObject( &_brushHalftone );
			pDC->PatBlt(
				rcResult.left, rcResult.top, rcResult.Width(), rcResult.Height(),
				PATINVERT
				);
			pDC->SelectObject( pBrushOld );
			ReleaseDC( pDC );
		} // if( bUseHalftone )
	} // else from if( ! rcResult.IsRectEmpty() )
	OnSwDoRedraw();
	OnSwUpdateWindow();
	return rcResult;
}

INT CChildView::GetObjectCount() const
{
	ASSERT_VALID( this );
INT nObjectCount = INT( m_arrObjects.GetSize() );
	return nObjectCount;
}

const CChildView::GEODESIC_OBJECT & CChildView::GetObjectAt( INT nObjectIndex ) const
{
	ASSERT_VALID( this );
	return ( const_cast < CChildView * > ( this ) ) -> GetObjectAt( nObjectIndex );
}

CChildView::GEODESIC_OBJECT & CChildView::GetObjectAt( INT nObjectIndex )
{
	ASSERT_VALID( this );
	ASSERT(
			0 <= nObjectIndex
		&&	nObjectIndex < INT( m_arrObjects.GetSize() )
		);
GEODESIC_OBJECT & _object = m_arrObjects.ElementAt( nObjectIndex );
	return _object;
}

CSize CChildView::OnSwGetTotalSize() const
{
CSize _size = m_bmp.GetSize();
	_size.cx = INT( double(_size.cx) * m_lfZoomFactor );
	_size.cy = INT( double(_size.cy) * m_lfZoomFactor );
	return _size;
}

void CChildView::OnSwSetScrollPos( POINT pt )
{
	m_bmpCache.Empty();
	m_ptLastStatusLL.x = m_ptLastStatusLL.y = -1;
	CExtWRB < CExtScrollWnd > :: OnSwSetScrollPos( pt );
}

bool CChildView::OnSwDoScrollBy(
	CSize sizeScroll,
	bool bDoScroll // = true
	)
{
	m_bmpCache.Empty();
	m_ptLastStatusLL.x = m_ptLastStatusLL.y = -1;
	CExtWRB < CExtScrollWnd > :: OnSwDoScrollBy( sizeScroll, bDoScroll );
	return true;
}

CRect CChildView::OnSwRecalcLayout(
	bool bDoLayout,
	LPCRECT pRectClientSrc
	)
{
CRect rc = CExtWRB < CExtScrollWnd > :: OnSwRecalcLayout( bDoLayout, pRectClientSrc ); 
	return rc;
}

void CChildView::OnSwPaintAreaBetweenScrollBarWindows(
	CDC & dc,
	const RECT & rcAreaBetweenScrollBarWindows
	)
{
	dc.FillSolidRect(
		&rcAreaBetweenScrollBarWindows,
		g_PaintManager->GetColor( COLOR_3DFACE )
		);
}

void CChildView::OnSwPaint( CDC & dc )
{
CRect rcClient;
	GetClientRect( &rcClient );
	if( ! m_bmpCache.IsEmpty() )
	{
		CSize _sizeBmp = m_bmpCache.GetSize();
		CSize _sizeClient = rcClient.Size();
		if( _sizeBmp != _sizeClient )
		{
			m_bmpCache.Empty();
			m_ptLastStatusLL.x = m_ptLastStatusLL.y = -1;
		}
	} // if( ! m_bmpCache.IsEmpty() )
	if( m_bmpCache.IsEmpty() )
	{
		CRect rcClipBox;
		dc.GetClipBox( &rcClipBox );
		COLORREF clrBk =
			g_PaintManager->GetColor(
				CExtPaintManager::CLR_3DFACE_OUT
				);
		CPoint ptVpRestore = dc.SetViewportOrg( 0, 0 );
//		if( ! g_PaintManager->PaintDocumentClientAreaBkgnd(
//				dc,
//				this
//				)
//			)
		if( ! g_PaintManager->PaintDockerBkgnd(
				true,
				dc,
				this
				)
			)
		{
			CPoint ptScrollPos = OnSwGetScrollPos();
			dc.FillSolidRect(
				&rcClipBox,
				clrBk
				);
		}
		dc.SetViewportOrg( ptVpRestore );
		if( ! m_bmp.IsEmpty() )
		{
			CSize _sizeSrc = m_bmp.GetSize();
			CSize _sizeDst(
				INT( double(_sizeSrc.cx) * m_lfZoomFactor ),
				INT( double(_sizeSrc.cy) * m_lfZoomFactor )
				);
			CRect rcSrc( 0, 0, _sizeSrc.cx, _sizeSrc.cy );
			CRect rcDst( 0, 0, _sizeDst.cx, _sizeDst.cy );
			int nOldStretchBltMode = ::GetStretchBltMode( dc.m_hDC );
			::SetStretchBltMode(
				dc.m_hDC,
				( m_bEnableHalftoning && g_PaintManager.m_bIsWinNT )
					? HALFTONE
					: COLORONCOLOR
				);
			m_bmp.Draw( dc.m_hDC, rcDst, rcSrc );
			::SetStretchBltMode(
				dc.m_hDC,
				nOldStretchBltMode
				);
			CPoint ptScrollPos = OnSwGetScrollPos();
			CRect rcRenderArea( 0, 0, min(rcClient.Width(),_sizeDst.cx), min(rcClient.Height(),_sizeDst.cy) );
			rcRenderArea.OffsetRect( ptScrollPos );
			GEODESIC_RECT grVisibleArea = ClientToGeodesic( rcRenderArea );
			INT nObjectIndex, nObjectCount = GetObjectCount();
			COLORREF arrClrShadeAccentLightOuter[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ];
			COLORREF arrClrShadeAccentDarkOuter[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ];
			COLORREF arrClrShadeAccentLightInner[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ];
			COLORREF arrClrShadeAccentDarkInner[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ];
			CBrush arrBrushes[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ];
			INT nColorIndex, nColorCount = INT( sizeof(arrBrushes)/sizeof(arrBrushes[0]) );
			for( nColorIndex = 0; nColorIndex < nColorCount; nColorIndex ++ )
			{
				arrBrushes[nColorIndex].CreateSolidBrush( g_arrClrBk[nColorIndex] );
				arrClrShadeAccentLightOuter[nColorIndex] = CExtBitmap::stat_HLS_Adjust( g_arrClrBk[nColorIndex], 0.0, 0.25, 0.0 );
				arrClrShadeAccentDarkOuter[nColorIndex] = CExtBitmap::stat_HLS_Adjust( g_arrClrBk[nColorIndex], 0.0, -0.35, 0.0 );
				if( g_arrRadiuses[nColorIndex >= 5] )
				{
					arrClrShadeAccentLightInner[nColorIndex] = CExtBitmap::stat_HLS_Adjust( g_arrClrBk[nColorIndex], 0.0, 0.15, 0.0 );
					arrClrShadeAccentDarkInner[nColorIndex] = CExtBitmap::stat_HLS_Adjust( g_arrClrBk[nColorIndex], 0.0, -0.15, 0.0 );
				}
				else
				{
					arrClrShadeAccentLightInner[nColorIndex] = COLORREF(-1L);
					arrClrShadeAccentDarkInner[nColorIndex] = COLORREF(-1L);
				}
			} // for( nColorIndex = 0; nColorIndex < nColorCount; nColorIndex ++ )
			if( m_eSTOT != __ESTOT_HIDE_ALL )
			{
				HGDIOBJ hOldPen = ::SelectObject( dc.m_hDC, ::GetStockObject( NULL_PEN ) );
				CBrush * pOldBrush = NULL;
				for( nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex ++ )
				{
					const GEODESIC_OBJECT & _object = GetObjectAt( nObjectIndex );
					ASSERT( 0 <= _object.m_nLevel && _object.m_nLevel <= 3 );
					if( ! grVisibleArea.PtInRect( _object.m_gp ) )
						continue;
					CPoint ptObject( GeodesicToClient( _object.m_gp ) );
					if( m_eSTOT == __ESTOT_ALPHA_BMP )
					{
						CExtBitmap & _bmp = m_arrObjectBmps[_object.m_nLevel];
						CPoint & _ptBmpOffset = m_arrObjectBmpOffsets[_object.m_nLevel];
						CSize & _sizeBmp = m_arrObjectBmpSize[_object.m_nLevel];
						CRect rcDst( CPoint( ptObject - _ptBmpOffset ), _sizeBmp );
						_bmp.AlphaBlend(
							dc.m_hDC,
							rcDst
							);
					} // if( m_eSTOT == __ESTOT_ALPHA_BMP )
					else
					{
						INT nRadius = g_arrRadiuses[_object.m_nLevel];
						if(		m_eSTOT == __ESTOT_ELLIPSE_FLAT
							||	m_eSTOT == __ESTOT_ELLIPSE_SOLID
							)
						{
							pOldBrush = dc.SelectObject( &(arrBrushes[_object.m_nLevel]) );
							dc.Ellipse(
								ptObject.x - nRadius,
								ptObject.y - nRadius,
								ptObject.x + nRadius + 1,
								ptObject.y + nRadius + 1
								);
							dc.SelectObject( pOldBrush );
						}
						if( m_eSTOT != __ESTOT_ELLIPSE_FLAT )
							CExtPaintManager::stat_PaintShadedEllipse(
								dc.m_hDC,
								ptObject,
								nRadius,
								nRadius,
								arrClrShadeAccentLightOuter[_object.m_nLevel],
								arrClrShadeAccentDarkOuter[_object.m_nLevel]
								);
						if( m_eSTOT == __ESTOT_ELLIPSE_SOLID )
						{
							if( arrClrShadeAccentLightInner[_object.m_nLevel] != COLORREF(-1L) )
							{
								ASSERT( arrClrShadeAccentDarkInner[_object.m_nLevel] != COLORREF(-1L) );
								CExtPaintManager::stat_PaintShadedEllipse(
									dc.m_hDC,
									ptObject,
									nRadius - 1,
									nRadius - 1,
									arrClrShadeAccentLightInner[_object.m_nLevel],
									arrClrShadeAccentDarkInner[_object.m_nLevel]
									);
							} // if( arrClrShadeAccentLightInner[_object.m_nLevel] != COLORREF(-1L) )
						}
					} // else from if( m_eSTOT == __ESTOT_ALPHA_BMP )
				} // for( nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex ++ )
				::SelectObject( dc.m_hDC, hOldPen );
			} // if( m_eSTOT != __ESTOT_HIDE_ALL )
			if(		(	m_lfZoomFactor >= (0.5)
					&&	m_eSTLT == CChildView::__ESTLT_BY_ZOOM
					)
				||		m_eSTLT == CChildView::__ESTLT_SHOW_ALL
				)
			{ // if show at least any text labels
				int nBkMode = dc.SetBkMode( TRANSPARENT );
				LOGFONT _lf = { 0 };
				g_PaintManager->m_FontNormal.GetLogFont( &_lf );
				CFont arrFonts[ sizeof(g_arrClrBk)/sizeof(g_arrClrBk[0]) ];
				INT nFontIndex, nFontCount = sizeof(arrFonts)/sizeof(arrFonts[0]);
				for( nFontIndex = 0; nFontIndex < nFontCount; nFontIndex ++ )
				{
					if( nFontIndex > 0 )
					{
						_lf.lfWidth  = ::MulDiv( _lf.lfWidth,  nFontIndex + 5, nFontIndex + 4 );
						_lf.lfHeight = ::MulDiv( _lf.lfHeight, nFontIndex + 5, nFontIndex + 4 );
						if( nFontIndex >= 3 )
							_lf.lfWeight = 900;
						else if( nFontIndex >= 2 )
							_lf.lfWeight = 500;
					}
					arrFonts[nFontIndex].CreateFontIndirect( &_lf );
				} // for( nFontIndex = 0; nFontIndex < nFontCount; nFontIndex ++ )
				bool bDwmMode = false;
				if(		m_bEnableVistaTextRendering
					&&	g_PaintManager.m_bIsWinVistaOrLater
					&&	g_PaintManager.m_DWM.IsCompositionEnabled()
					&&	g_PaintManager.m_pfnMsImgAlphaBlend != NULL
					&&	g_PaintManager.m_UxTheme.OpenThemeDataEx(
							m_hWnd,
							__EXT_UX_LPCWSTR( VSCLASS_WINDOW ),
							0
							) != NULL
					)
					bDwmMode = true;
				for( nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex ++ )
				{
					const GEODESIC_OBJECT & _object = GetObjectAt( nObjectIndex );
					ASSERT( 0 <= _object.m_nLevel && _object.m_nLevel <= 3 );
					if( _object.m_strName.IsEmpty() )
						continue;
					if( ! grVisibleArea.PtInRect( _object.m_gp ) )
						continue;
					CPoint ptObject( GeodesicToClient( _object.m_gp ) );
					if(		m_eSTLT == CChildView::__ESTLT_BY_ZOOM
						&&	m_lfZoomFactor < g_arrVZF[_object.m_nLevel]
						)
						continue;
					INT nRadius = g_arrRadiuses[_object.m_nLevel];
					CRect rcText(
						ptObject.x + nRadius,
						ptObject.y + nRadius,
						ptObject.x + nRadius + 1000,
						ptObject.y + nRadius + 1000
						);
					if( bDwmMode )
					{
						CExtUxTheme::__EXT_UX_DTTOPTS dtoMeasure = { sizeof(CExtUxTheme::__EXT_UX_DTTOPTS) };
						dtoMeasure.dwFlags = __EXT_UX_DTT_COMPOSITED | __EXT_UX_DTT_GLOWSIZE;
						dtoMeasure.iGlowSize = g_arrDwmGlowSizes[_object.m_nLevel];
						CRect rcTextMeasure;
						rcTextMeasure.SetRect( 0, 0, 0, 0 );
						CFont * pOldFont = dc.SelectObject( &(arrFonts[_object.m_nLevel]) );
						g_PaintManager.m_UxTheme.GetThemeTextExtent(
							dc.m_hDC,
							0,
							0,
							_object.m_strName,
							-1,
							DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOPREFIX|DT_CALCRECT,
							NULL,
							&rcTextMeasure
							);
						dc.SelectObject( pOldFont );
						rcText.right = rcText.left + rcTextMeasure.Width();
						rcText.bottom = rcText.top + rcTextMeasure.Height();

						rcText.InflateRect( g_arrDwmGlowSizes[_object.m_nLevel], g_arrDwmGlowSizes[_object.m_nLevel] );
						CExtMemoryDC dc32(
							&dc,
							&rcText,
							CExtMemoryDC::MDCOPT_TO_MEMORY
								| CExtMemoryDC::MDCOPT_FORCE_DIB
								| CExtMemoryDC::MDCOPT_DIB_NEGATIVE_HEIGHT
							);
						ASSERT( dc32.GetSafeHdc() != NULL );
						dc32.BitBlt(
							rcText.left,
							rcText.top,
							rcText.Width(),
							rcText.Height(),
							&dc,
							rcText.left,
							rcText.top,
							SRCCOPY
							);
						pOldFont = dc32.SelectObject( &(arrFonts[_object.m_nLevel]) );
						CExtUxTheme::__EXT_UX_DTTOPTS dto = { sizeof(CExtUxTheme::__EXT_UX_DTTOPTS) };
						dto.dwFlags = __EXT_UX_DTT_COMPOSITED | __EXT_UX_DTT_GLOWSIZE | __EXT_UX_DTT_TEXTCOLOR;
						dto.iGlowSize = g_arrDwmGlowSizes[_object.m_nLevel];
						dto.crText = RGB(0,0,0);
						g_PaintManager.m_UxTheme.DrawThemeTextEx(
							dc32.m_hDC,
							0,
							0,
							_object.m_strName,
							-1,
							DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX,
							rcText,
							&dto
							);
						if( _object.m_nLevel == 3 )
						{
							g_PaintManager.m_UxTheme.DrawThemeTextEx(
								dc32.m_hDC,
								0,
								0,
								_object.m_strName,
								-1,
								DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX,
								rcText,
								&dto
								);
						} // if( _object.m_nLevel == 3 )
						dc32.SelectObject( pOldFont );
					} // if( bDwmMode )
					else
					{
						CFont * pOldFont = dc.SelectObject( &(arrFonts[_object.m_nLevel]) );
						COLORREF clrText1 = RGB(255,255,255);
						COLORREF clrTextOld = dc.SetTextColor( clrText1 );
						rcText.OffsetRect( -1, -1 );
						CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(_object.m_strName), _object.m_strName.GetLength(), rcText, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOCLIP, 0 );
						rcText.OffsetRect( 1, 0 );
						CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(_object.m_strName), _object.m_strName.GetLength(), rcText, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOCLIP, 0 );
						rcText.OffsetRect( 1, 0 );
						CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(_object.m_strName), _object.m_strName.GetLength(), rcText, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOCLIP, 0 );
						rcText.OffsetRect( 0, 1 );
						CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(_object.m_strName), _object.m_strName.GetLength(), rcText, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOCLIP, 0 );
						rcText.OffsetRect( 0, 1 );
						CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(_object.m_strName), _object.m_strName.GetLength(), rcText, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOCLIP, 0 );
						rcText.OffsetRect( -1, 0 );
						CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(_object.m_strName), _object.m_strName.GetLength(), rcText, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOCLIP, 0 );
						rcText.OffsetRect( -1, 0 );
						CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(_object.m_strName), _object.m_strName.GetLength(), rcText, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOCLIP, 0 );
						rcText.OffsetRect( 0, -1 );
						CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(_object.m_strName), _object.m_strName.GetLength(), rcText, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOCLIP, 0 );
						rcText.OffsetRect( 1, 0 );
						COLORREF clrText2 = RGB(0,0,0);
						dc.SetTextColor( clrText2 );
						CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(_object.m_strName), _object.m_strName.GetLength(), rcText, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOCLIP, 0 );
						dc.SetTextColor( clrTextOld );
						dc.SelectObject( pOldFont );
					} // else from if( bDwmMode )
				} // for( nObjectIndex = 0; nObjectIndex < nObjectCount; nObjectIndex ++ )
				if( bDwmMode )
					g_PaintManager.m_UxTheme.CloseThemeData();
				dc.SetBkMode( nBkMode );
			} // if show at least any text labels
		} // if( ! m_bmp.IsEmpty() )
		ptVpRestore = dc.SetViewportOrg( 0, 0 );
		m_bmpCache.FromSurface( dc.m_hDC, rcClient );
		dc.SetViewportOrg( ptVpRestore );
	} // if( m_bmpCache.IsEmpty() )
	else
	{
		CPoint ptVpRestore = dc.SetViewportOrg( 0, 0 );
		m_bmpCache.Draw(
			dc.m_hDC,
			rcClient
			);
		dc.SetViewportOrg( ptVpRestore );
	} // else from if( m_bmpCache.IsEmpty() )
	if( ! m_rcHelperDrawZoomRect.IsRectEmpty() )
	{
		CRect rcDraw = m_rcHelperDrawZoomRect;
		rcDraw.InflateRect( &m_rcHelperZoomRectPadding );
		m_bmpHelperZoomRectHighlight.AlphaBlendSkinParts(
			dc.m_hDC,
			rcDraw,
			m_rcHelperZoomRectPadding,
			CExtBitmap::__EDM_STRETCH
			);
	}
}

void CChildView::OnHideAllTextLabels()
{
	if( m_bmp.IsEmpty() )
		return;
	m_eSTLT = CChildView::__ESTLT_HIDE_ALL;
	m_bmpCache.Empty();
	OnSwDoRedraw();
}

void CChildView::OnUpdateHideAllTextLabels( CCmdUI * pCmdUI )
{
	if( m_bmp.IsEmpty() )
	{
		pCmdUI->Enable( FALSE );
		pCmdUI->SetRadio( 0 );
		return;
	}
	pCmdUI->Enable();
	pCmdUI->SetRadio( ( m_eSTLT == CChildView::__ESTLT_HIDE_ALL ) ? 1 : 0 );
}

void CChildView::OnShowTextLabelsWithZooming()
{
	if( m_bmp.IsEmpty() )
		return;
	m_eSTLT = CChildView::__ESTLT_BY_ZOOM;
	m_bmpCache.Empty();
	OnSwDoRedraw();
}

void CChildView::OnUpdateShowTextLabelsZooming( CCmdUI * pCmdUI )
{
	if( m_bmp.IsEmpty() )
	{
		pCmdUI->Enable( FALSE );
		pCmdUI->SetRadio( 0 );
		return;
	}
	pCmdUI->Enable();
	pCmdUI->SetRadio( ( m_eSTLT == CChildView::__ESTLT_BY_ZOOM ) ? 1 : 0 );
}

void CChildView::OnShowAllTextLabels()
{
	if( m_bmp.IsEmpty() )
		return;
	m_eSTLT = CChildView::__ESTLT_SHOW_ALL;
	m_bmpCache.Empty();
	OnSwDoRedraw();
}

void CChildView::OnUpdateShowAllTextLabels( CCmdUI * pCmdUI )
{
	if( m_bmp.IsEmpty() )
	{
		pCmdUI->Enable( FALSE );
		pCmdUI->SetRadio( 0 );
		return;
	}
	pCmdUI->Enable();
	pCmdUI->SetRadio( ( m_eSTLT == CChildView::__ESTLT_SHOW_ALL ) ? 1 : 0 );
}

BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
//	return CExtScrollWnd ::OnSetCursor(pWnd, nHitTest, message);
	pWnd;
	nHitTest;
	message;
	if( m_nHT >= 0 )
	{
		const GEODESIC_OBJECT & _object = GetObjectAt( m_nHT );
		if( ! _object.m_strURL.IsEmpty() )
		{
			::SetCursor( ::AfxGetApp()->LoadCursor( IDC_EXT_HAND ) );
			return TRUE;
		}
	}
	::SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
	return TRUE;
}

void CChildView::OnVistaTextRendering() 
{
	if( g_PaintManager.m_bIsWinVistaOrLater && g_PaintManager.m_DWM.IsCompositionEnabled() )
	{
		m_bEnableVistaTextRendering = ( ! m_bEnableVistaTextRendering );
		m_bmpCache.Empty();
		OnSwDoRedraw();
	}
}

void CChildView::OnUpdateVistaTextRendering(CCmdUI* pCmdUI) 
{
	if( g_PaintManager.m_bIsWinVistaOrLater && g_PaintManager.m_DWM.IsCompositionEnabled() )
	{
		pCmdUI->Enable();
		pCmdUI->SetCheck( m_bEnableVistaTextRendering ? 1 : 0 );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CChildView::OnHalftoning() 
{
	if( g_PaintManager.m_bIsWinNT )
	{
		m_bEnableHalftoning = ( ! m_bEnableHalftoning );
		m_bmpCache.Empty();
		OnSwDoRedraw();
	}
}

void CChildView::OnUpdateHalftoning(CCmdUI* pCmdUI) 
{
	if( g_PaintManager.m_bIsWinNT )
	{
		pCmdUI->Enable();
		pCmdUI->SetCheck( m_bEnableHalftoning ? 1 : 0 );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CChildView::OnObjectHideAll() 
{
	if( m_eSTOT == __ESTOT_HIDE_ALL )
		return;
	m_eSTOT = __ESTOT_HIDE_ALL;
	m_bmpCache.Empty();
	OnSwDoRedraw();
}

void CChildView::OnUpdateObjectHideAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio( ( m_eSTOT == __ESTOT_HIDE_ALL ) ? TRUE : FALSE );
}

void CChildView::OnObjectEllipseFlat() 
{
	if( m_eSTOT == __ESTOT_ELLIPSE_FLAT )
		return;
	m_eSTOT = __ESTOT_ELLIPSE_FLAT;
	m_bmpCache.Empty();
	OnSwDoRedraw();
}

void CChildView::OnUpdateObjectEllipseFlat(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio( ( m_eSTOT == __ESTOT_ELLIPSE_FLAT ) ? TRUE : FALSE );
}

void CChildView::OnObjectEllipseCircle() 
{
	if( m_eSTOT == __ESTOT_ELLIPSE_CIRCLE )
		return;
	m_eSTOT = __ESTOT_ELLIPSE_CIRCLE;
	m_bmpCache.Empty();
	OnSwDoRedraw();
}

void CChildView::OnUpdateObjectEllipseCircle(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio( ( m_eSTOT == __ESTOT_ELLIPSE_CIRCLE ) ? TRUE : FALSE );
}

void CChildView::OnObjectEllipseSolid() 
{
	if( m_eSTOT == __ESTOT_ELLIPSE_SOLID )
		return;
	m_eSTOT = __ESTOT_ELLIPSE_SOLID;
	m_bmpCache.Empty();
	OnSwDoRedraw();
}

void CChildView::OnUpdateObjectEllipseSolid(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio( ( m_eSTOT == __ESTOT_ELLIPSE_SOLID ) ? TRUE : FALSE );
}

void CChildView::OnObjectAlphaBmp() 
{
	if( m_eSTOT == __ESTOT_ALPHA_BMP )
		return;
	m_eSTOT = __ESTOT_ALPHA_BMP;
	m_bmpCache.Empty();
	OnSwDoRedraw();
}

void CChildView::OnUpdateObjectAlphaBmp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio( ( m_eSTOT == __ESTOT_ALPHA_BMP ) ? TRUE : FALSE );
}


void CChildView::OnShowTrackingTooltipOnZoomScrollbar() 
{
	m_wndZoomer.m_bEnabledTrackingToolTips = (! m_wndZoomer.m_bEnabledTrackingToolTips );
	_UpdateZoomerTT();
}

void CChildView::OnUpdateShowTrackingTooltipOnZoomScrollbar(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck( m_wndZoomer.m_bEnabledTrackingToolTips ? 1 : 0 );
}
