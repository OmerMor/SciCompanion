// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "FixedSizePanels.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoHtmlCtrl

IMPLEMENT_DYNAMIC( CDemoHtmlCtrl, CWnd );

CDemoHtmlCtrl::CDemoHtmlCtrl()
	: m_pBrowser( NULL )
	, m_bReflectParentSizing( false )
{
}

CDemoHtmlCtrl::~CDemoHtmlCtrl()
{
}


BEGIN_MESSAGE_MAP(CDemoHtmlCtrl, CWnd)
	//{{AFX_MSG_MAP(CDemoHtmlCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDemoHtmlCtrl message handlers

bool CDemoHtmlCtrl::Create(
		CWnd * pWndParent,
		CRect rectInit // = CRect(0,0,0,0)
		)
{
UINT nID = pWndParent->GetDlgCtrlID();
CString sWindowName;
	pWndParent->GetWindowText( sWindowName );
	if(	!CreateControl(
			CLSID_WebBrowser,
			LPCTSTR(sWindowName),
			WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
			rectInit,
			pWndParent,
			nID
			)
		)
	{ // if IWebBrowser2 is not created successfully
		ASSERT( FALSE );
		DestroyWindow();
		return false;
	} // if IWebBrowser2 is not created successfully

LPUNKNOWN lpUnk = GetControlUnknown();
	ASSERT( lpUnk != NULL );
HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_pBrowser);
	if( FAILED(hr) )
	{
		ASSERT( FALSE );
		DestroyWindow();
		return false;
	}

	return true;
}

void CDemoHtmlCtrl::GoBack()
{
	ASSERT_VALID( this );
	ASSERT( m_pBrowser != NULL );
	m_pBrowser->GoBack();
}

void CDemoHtmlCtrl::GoForward()
{
	ASSERT_VALID( this );
	ASSERT( m_pBrowser != NULL );
	m_pBrowser->GoForward();
}

void CDemoHtmlCtrl::Refresh()
{
	ASSERT_VALID( this );
	ASSERT( m_pBrowser != NULL );
	m_pBrowser->Refresh();
}

void CDemoHtmlCtrl::NavigateURL( LPCTSTR lpszURL )
{
	ASSERT_VALID( this );
	ASSERT( m_pBrowser != NULL );

COleSafeArray vPostData;

COleVariant vURL( lpszURL, VT_BSTR );
COleVariant vHeaders( LPCTSTR(NULL), VT_BSTR);
COleVariant vTargetFrameName( LPCTSTR(NULL), VT_BSTR );
COleVariant vFlags( 0L, VT_I4 );

	m_pBrowser->Navigate2(
		vURL,
		vFlags,
		vTargetFrameName,
		vPostData,
		vHeaders
		);
}

void CDemoHtmlCtrl::NavigateResourceID(
	UINT nResourceID
	)
{
	ASSERT_VALID( this );
	ASSERT( m_pBrowser != NULL );

CString sAppName;
	::GetModuleFileName(
		NULL,
		sAppName.GetBuffer( _MAX_PATH + 1 ),
		_MAX_PATH
		);
	sAppName.ReleaseBuffer();
CString sResourseID;
	sResourseID.Format( _T("%d"), INT(nResourceID) );
CString sNavigatePath;
	sNavigatePath =
		_T("res://")
		+ sAppName
		+ _T("/") //+ _T(".exe/")
		+ sResourseID
		;
	NavigateURL( sNavigatePath );
}

/////////////////////////////////////////////////////////////////////////////
// CPanelEmpty

IMPLEMENT_DYNAMIC(CPanelEmpty, CExtPanelControlBar)

CPanelEmpty::CPanelEmpty()
{
}

CPanelEmpty::~CPanelEmpty()
{
}

CSize CPanelEmpty::OnQueryFixedBarNewSize(
	CExtPanelControlBar::e_fixed_bar_size_query_t eWhichSizeNeedToKnow
	) const
{
	ASSERT_VALID( this );
	switch( eWhichSizeNeedToKnow )
	{
	case CExtPanelControlBar::__FSQ_DOCKED_H:
		return CSize(150,50);
	case CExtPanelControlBar::__FSQ_DOCKED_V:
		return CSize(50,150);
	default:
		return CSize(150,150);
	} // switch( eWhichSizeNeedToKnow )
}

BEGIN_MESSAGE_MAP(CPanelEmpty, CExtPanelControlBar)
	//{{AFX_MSG_MAP(CPanelEmpty)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelCustomDrawn

IMPLEMENT_DYNAMIC(CPanelCustomDrawn, CPanelEmpty)

CPanelCustomDrawn::CPanelCustomDrawn()
{
}

CPanelCustomDrawn::~CPanelCustomDrawn()
{
}

BEGIN_MESSAGE_MAP(CPanelCustomDrawn, CPanelEmpty)
	//{{AFX_MSG_MAP(CPanelCustomDrawn)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPanelCustomDrawn::OnPaint()
{
	ASSERT_VALID( this );

CPaintDC dcPaint( this );
CExtMemoryDC dc( &dcPaint );
CRect rcClient;
	GetClientRect( &rcClient );
	ASSERT( GetWindow( GW_CHILD ) == NULL );
	// fill gradient & draw text
	dc.FillSolidRect(
		&rcClient,
		g_PaintManager->GetColor(
			CExtPaintManager::CLR_3DFACE_OUT,
			this
			)
		);
	rcClient.DeflateRect( 2, 2 );
	COLORREF clrFc = g_PaintManager->GetColor( COLOR_3DFACE, this );
	COLORREF clrDk = g_PaintManager->GetColor( COLOR_3DDKSHADOW, this );
	COLORREF clrHi = g_PaintManager->GetColor( COLOR_3DHILIGHT, this );
	COLORREF clrLo = g_PaintManager->GetColor( COLOR_3DSHADOW, this );
	dc.Draw3dRect( &rcClient, clrFc, clrDk );
	rcClient.DeflateRect( 1, 1 );
	dc.Draw3dRect( &rcClient, clrHi, clrLo );
	rcClient.DeflateRect( 1, 1 );
	dc.Draw3dRect( &rcClient, clrLo, clrHi );
	rcClient.DeflateRect( 1, 1 );
	dc.Draw3dRect( &rcClient, clrDk, clrFc );
	rcClient.DeflateRect( 2, 2 );
	UINT nDockBarID = GetSafeDockBarDlgCtrlID();
	CExtPaintManager::stat_PaintGradientRect(
		dc,
		rcClient,
		RGB( 128, 128, 255 ),
		RGB( 0, 0, 0 ),
		(nDockBarID==AFX_IDW_DOCKBAR_LEFT || nDockBarID==AFX_IDW_DOCKBAR_RIGHT)
			? true : false
		);
	
CRect rcDrawText( rcClient );
	rcDrawText.DeflateRect( 5, 5 );
CRect rcDrawTextShadow( rcDrawText );
	rcDrawTextShadow.OffsetRect( 1, 1 );
CString sText;
CFont * pFont = &g_PaintManager->m_FontNormal;
CExtRichContentLayout::e_layout_orientation_t eLO = CExtRichContentLayout::__ELOT_NORMAL;
	switch( nDockBarID )
	{
	case AFX_IDW_DOCKBAR_TOP:
		sText = _T("<html>Docked at <b>top</b></html>");
	break;
	case AFX_IDW_DOCKBAR_BOTTOM:
		sText = _T("<html>Docked at <b>bottom</b></html>");
	break;
	case AFX_IDW_DOCKBAR_LEFT:
		eLO = CExtRichContentLayout::__ELOT_90_CW;
		sText = _T("<html>Docked at <b>left</b></html>");
	break;
	case AFX_IDW_DOCKBAR_RIGHT:
		eLO = CExtRichContentLayout::__ELOT_90_CW;
		sText = _T("<html>Docked at <b>right</b></html>");
	break;
	default:
		ASSERT( nDockBarID == AFX_IDW_DOCKBAR_FLOAT );
		sText = _T("<html><b>Floating</b></html>");
	break;
	} // switch( nDockBarID )
	CFont * pOldFont = dc.SelectObject( pFont );
	int nOldBkMode = dc.SetBkMode( TRANSPARENT );
	COLORREF clrOldTextColor = dc.SetTextColor( RGB(128,128,128) );
	CExtRichContentLayout::stat_DrawText( CExtRichContentLayout::__ELFMT_AUTO_DETECT, eLO, dc.m_hDC, LPCTSTR(sText), &rcDrawTextShadow, DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOCLIP, 0 );
	dc.SetTextColor( RGB(255,255,255) );
	CExtRichContentLayout::stat_DrawText( CExtRichContentLayout::__ELFMT_AUTO_DETECT, eLO, dc.m_hDC, LPCTSTR(sText), &rcDrawText, DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOCLIP, 0 );
	dc.SetTextColor( clrOldTextColor );
	dc.SetBkMode( nOldBkMode );
	dc.SelectObject( pOldFont );
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame::CColorStatic

CMainFrame::CColorStatic::CColorStatic()
	: m_clrIn( RGB(0,0,0) )
	, m_clrOut( RGB(255,255,255) )
{
}

LRESULT CMainFrame::CColorStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == WM_PAINT )
	{
		CPaintDC dc( this );
		CRect rcClient;
		GetClientRect( &rcClient );
		dc.FillRect( &rcClient, &g_PaintManager->m_brushLightDefault );
		COLORREF clrLight		= g_PaintManager->GetColor( COLOR_3DHILIGHT, this );
		COLORREF clrDkShadow	= g_PaintManager->GetColor( COLOR_3DDKSHADOW, this );
		COLORREF clrFace		= g_PaintManager->GetColor( COLOR_3DFACE, this );
		COLORREF clrShadow		= g_PaintManager->GetColor( COLOR_3DSHADOW, this );
		dc.Draw3dRect( &rcClient, clrShadow, clrLight );
		rcClient.DeflateRect( 1, 1 );
		dc.Draw3dRect( &rcClient, clrDkShadow, clrFace );
		rcClient.DeflateRect( 1, 1 );
		
		CRect rcButtonIn;
		GetWindow(GW_HWNDNEXT)->GetWindowRect( &rcButtonIn );
		rcButtonIn.OffsetRect( -rcButtonIn.TopLeft() );
		rcButtonIn.InflateRect( 1, 1 );
		rcButtonIn.OffsetRect( rcClient.TopLeft() );
		
		CRect rcButtonOut( rcButtonIn );
		rcButtonOut.OffsetRect( rcButtonOut.Width()/2, rcButtonOut.Height()/2 );
		CRect rcUnion( rcButtonIn.left, rcButtonIn.top, rcButtonOut.right, rcButtonOut.bottom );
		CPoint ptOffset(
			( rcClient.Width() - rcUnion.Width() ) / 2 + 1,
			( rcClient.Height() - rcUnion.Height() ) / 2 + 1
			);
		rcButtonIn.OffsetRect( ptOffset );
		rcButtonOut.OffsetRect( ptOffset );
		
		dc.FillSolidRect( &rcButtonOut, m_clrOut );
		dc.Draw3dRect( &rcButtonOut, clrFace, clrDkShadow );
		rcButtonOut.DeflateRect( 1, 1 );
		dc.Draw3dRect( &rcButtonOut, clrLight, clrShadow );
		rcButtonOut.DeflateRect( 1, 1 );
		dc.Draw3dRect( &rcButtonOut, clrFace, clrFace );
		
		dc.FillSolidRect( &rcButtonIn, m_clrIn );
		dc.Draw3dRect( &rcButtonIn, clrFace, clrDkShadow );
		rcButtonIn.DeflateRect( 1, 1 );
		dc.Draw3dRect( &rcButtonIn, clrLight, clrShadow );
		rcButtonIn.DeflateRect( 1, 1 );
		dc.Draw3dRect( &rcButtonIn, clrFace, clrFace );
		
		return 0;
	} // if( message == WM_PAINT )
	
	if( message == WM_ERASEBKGND )
	{
		return (!0);
	} // if( message == WM_ERASEBKGND )

	return CStatic::WindowProc(message,wParam,lParam);
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame::CDynamicButton

void CMainFrame::CDynamicButton::_OnClick(
	bool bSelectAny,
	bool bSeparatedDropDownClicked
	)
{
	ASSERT_VALID( this );
	bSelectAny;
	bSeparatedDropDownClicked;
CFrameWnd * pFrame = GetParentFrame();
	ASSERT_VALID( pFrame );
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
	{
		pFrame = pFrame->GetParentFrame();
		ASSERT_VALID( pFrame );
	}
	pFrame->SetFocus();
	ASSERT_KINDOF( CMainFrame, pFrame );
	if( ((CMainFrame*)pFrame)->m_hWndSelectedToolButton != NULL )
	{
		::InvalidateRect(
			((CMainFrame*)pFrame)->m_hWndSelectedToolButton,
			NULL,
			FALSE
			);
	}
	((CMainFrame*)pFrame)->m_hWndSelectedToolButton = m_hWnd;
	Invalidate();
	pFrame->SendMessage(
		WM_COMMAND,
		(WPARAM)GetDlgCtrlID(),
		(LPARAM)m_hWnd
		);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame::CDynamicPaletteButton

CMainFrame::CDynamicPaletteButton::CDynamicPaletteButton( COLORREF clr )
	: m_clr( clr )
{
}

LRESULT CMainFrame::CDynamicPaletteButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(		message == WM_LBUTTONDOWN
		||	message == WM_RBUTTONDOWN
		)
	{
		CFrameWnd * pFrame = GetParentFrame();
		ASSERT_VALID( pFrame );
		if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
		{
			pFrame = pFrame->GetParentFrame();
			ASSERT_VALID( pFrame );
		}
		pFrame->SetFocus();
		ASSERT_KINDOF( CMainFrame, pFrame );
		Invalidate();
		WPARAM wParam = (WPARAM)GetDlgCtrlID();
		if( message == WM_RBUTTONDOWN )
			wParam += 160;
		pFrame->SendMessage(
			WM_COMMAND,
			wParam,
			(LPARAM)m_hWnd
			);
		return 0L;
	}
	if(		message == WM_LBUTTONUP
		||	message == WM_RBUTTONUP
		)
	{
		return 0L;
	}
	return CDynamicButton::WindowProc(message,wParam,lParam);
}

void CMainFrame::CDynamicPaletteButton::_OnClick(
	bool bSelectAny,
	bool bSeparatedDropDownClicked
	)
{
	ASSERT_VALID( this );
	bSelectAny;
	bSeparatedDropDownClicked;
CFrameWnd * pFrame = GetParentFrame();
	ASSERT_VALID( pFrame );
	if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
	{
		pFrame = pFrame->GetParentFrame();
		ASSERT_VALID( pFrame );
	}
	pFrame->SetFocus();
	ASSERT_KINDOF( CMainFrame, pFrame );
	Invalidate();
	pFrame->SendMessage(
		WM_COMMAND,
		(WPARAM)GetDlgCtrlID(),
		(LPARAM)m_hWnd
		);
}

void CMainFrame::CDynamicPaletteButton::_RenderImpl( // non-buffered paint
	CDC & dc,
	bool bTransparent, // = false
	bool bCombinedContent // = false
	)
{
	ASSERT_VALID( this );
	bTransparent;
	bCombinedContent;
CRect rectClient;
	GetClientRect( &rectClient );
bool bPushed = (m_bPushed || GetCheck()) ? true : false;
bool bHover =
		( m_bMouseOver
		&& !CExtPopupMenuWnd::IsMenuTracking() )
			? true : false;
	dc.FillSolidRect( &rectClient, m_clr );

COLORREF clrLight		= g_PaintManager->GetColor( COLOR_3DHILIGHT, this );
COLORREF clrDkShadow	= g_PaintManager->GetColor( COLOR_3DDKSHADOW, this );
	if( bPushed || bHover )
	{
COLORREF clrFace		= g_PaintManager->GetColor( COLOR_3DFACE, this );
COLORREF clrShadow		= g_PaintManager->GetColor( COLOR_3DSHADOW, this );
		dc.Draw3dRect(
			&rectClient,
			clrShadow,
			clrLight
			);
		rectClient.DeflateRect( 1, 1 );
		dc.Draw3dRect(
			&rectClient,
			clrDkShadow,
			clrFace
			);
	}
	else
	{
		dc.Draw3dRect(
			&rectClient,
			clrDkShadow,
			clrLight
			);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC( CMainFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP

	ON_COMMAND_EX( ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_MENUBAR, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_UI_LOOK_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_UI_LOOK_BAR, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_PANEL_EMPTY, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_PANEL_EMPTY, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_PANEL_CUSTOM_DRAWN, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_PANEL_CUSTOM_DRAWN, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_PANEL_EDIT, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_PANEL_EDIT, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_PANEL_DIALOG, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_PANEL_DIALOG, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_PANEL_TOOLS, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_PANEL_TOOLS, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_PANEL_PALETTE, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_PANEL_PALETTE, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_PANEL_RESIZABLE1, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_PANEL_RESIZABLE1, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_PANEL_RESIZABLE2, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_PANEL_RESIZABLE2, OnUpdateControlBarMenu )

	ON_COMMAND_RANGE( IDC_BUTTON1, IDC_BUTTON16, OnClickedToolButton )
	ON_COMMAND_RANGE( IDC_BUTTON17, IDC_BUTTON176, OnClickedPaletteButtonL )
	ON_COMMAND_RANGE( (IDC_BUTTON17+160), (IDC_BUTTON176+160), OnClickedPaletteButtonR )

END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	if( ! g_PaintManager.PaintManagerStateLoad(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		)
		g_PaintManager.InstallPaintManager(
			RUNTIME_CLASS(CExtPaintManagerOffice2007_R2_LunaBlue)
			);

	m_hWndSelectedToolButton = NULL;

	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;
}

CMainFrame::~CMainFrame()
{
}

static UINT g_statBasicCommands[] =
{
	ID_APP_ABOUT,
	ID_APP_EXIT,
	ID_VIEW_MENUBAR,
	ID_VIEW_TOOLBAR,
	ID_VIEW_UI_LOOK_BAR,
	ID_VIEW_STATUS_BAR,
	ID_VIEW_PANEL_EMPTY,
	ID_VIEW_PANEL_CUSTOM_DRAWN,
	ID_VIEW_PANEL_EDIT,
	ID_VIEW_PANEL_DIALOG,
	ID_VIEW_PANEL_TOOLS,
	ID_VIEW_PANEL_PALETTE,
	ID_VIEW_PANEL_RESIZABLE1,
	ID_VIEW_PANEL_RESIZABLE2,
	0, // end of list
};

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( CExtNCW < CFrameWnd > :: OnCreate( lpCreateStruct ) == -1 )
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
HICON hIcon = pApp->LoadIcon( IDR_MAINFRAME );
	ASSERT( hIcon != NULL );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	ASSERT( pApp->m_pszProfileName != NULL );
	g_CmdManager->ProfileSetup(
		pApp->m_pszProfileName,
		GetSafeHwnd()
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_MAINFRAME
			)
		);

	if( !m_wndMenuBar.Create(
			NULL,
			this,
			ID_VIEW_MENUBAR
			)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }

	if( !m_wndToolBar.Create(
			_T("ToolBar"),
			this,
			AFX_IDW_TOOLBAR
			)
		||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if(		(! m_wndToolBarUiLook.Create( NULL, this, ID_VIEW_UI_LOOK_BAR ) )
		||	(! m_wndToolBarUiLook.ThemeSwitcherInit() )
		)
	{
		TRACE0("Failed to create toolbar2\n");
		return -1;      // fail to create
	}

	if(		!m_wndPanelEmpty.Create(
				_T("Empty Panel"),
				this,
				ID_VIEW_PANEL_EMPTY,
				WS_CHILD|WS_VISIBLE
					|CBRS_RIGHT|CBRS_GRIPPER|CBRS_TOOLTIPS
					|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
					|CBRS_HIDE_INPLACE
				)
		||	!m_wndPanelCustomDrawn.Create(
				_T("Custom-drawn Panel"),
				this,
				ID_VIEW_PANEL_CUSTOM_DRAWN,
				WS_CHILD|WS_VISIBLE
					|CBRS_RIGHT|CBRS_GRIPPER|CBRS_TOOLTIPS
					|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
					|CBRS_HIDE_INPLACE
				)
		||	!m_wndPanelEdit.Create(
				_T("Panel With Edit Control"),
				this,
				ID_VIEW_PANEL_EDIT,
				WS_CHILD|WS_VISIBLE
					|CBRS_LEFT|CBRS_GRIPPER|CBRS_TOOLTIPS
					|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
					|CBRS_HIDE_INPLACE
				)
					||	!m_wndEdit.Create(
							WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL
								|ES_MULTILINE|ES_WANTRETURN
								|ES_AUTOHSCROLL|ES_AUTOVSCROLL
								,
							CRect(0,0,160,80),
							&m_wndPanelEdit,
							UINT(IDC_STATIC)
							)
		||	!m_wndPanelDialog.Create(
				_T("Panel With Dialog"),
				this,
				ID_VIEW_PANEL_DIALOG,
				WS_CHILD|WS_VISIBLE
					|CBRS_LEFT|CBRS_GRIPPER|CBRS_TOOLTIPS
					|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
					|CBRS_HIDE_INPLACE
				)
					||	!m_dlgForPanelDialog.Create(
							IDD_DIALOG_FOR_PANEL,
							&m_wndPanelDialog
							)
		||	!m_wndPanelTools.Create(
				_T("Tools Panel"),
				this,
				ID_VIEW_PANEL_TOOLS,
				WS_CHILD|WS_VISIBLE
					|CBRS_RIGHT|CBRS_GRIPPER|CBRS_TOOLTIPS
					|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
					|CBRS_HIDE_INPLACE
				)
					||	!m_dlgForPanelTools.Create(
							IDD_DIALOG_TOOLS,
							&m_wndPanelTools
							)
		||	!m_wndPanelPalette.Create(
				_T("Palette Panel"),
				this,
				ID_VIEW_PANEL_PALETTE,
				WS_CHILD|WS_VISIBLE
					|CBRS_BOTTOM|CBRS_GRIPPER|CBRS_TOOLTIPS
					|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
					|CBRS_HIDE_INPLACE
				)
					||	!m_dlgForPanelPalette.Create(
							IDD_DIALOG_PALETTE,
							&m_wndPanelPalette
							)
		||	!m_wndResizableBar1.Create(
				_T("Resizable panel 1 (in fixed mode)"),
				this,
				ID_VIEW_PANEL_RESIZABLE1
				)
			||	!m_wndHtmlCtrl1.Create(
					&m_wndResizableBar1,
					CRect( 0, 0, 150, 150 )
					)
		||	!m_wndResizableBar2.Create(
				_T("Resizable panel 2 (in fixed mode)"),
				this,
				ID_VIEW_PANEL_RESIZABLE2
				)
			||	!m_wndHtmlCtrl2.Create(
					&m_wndResizableBar2,
					CRect( 0, 0, 150, 150 )
					)
		)
	{
		TRACE0("Failed to create bars/panels\n");
		return -1;      // fail to create
	}
	m_wndToolsPanelLabel.SubclassDlgItem( IDC_STATIC_BOTTOM_LABEL, &m_dlgForPanelTools );
	m_wndDialogPanelLabel.SubclassDlgItem( IDC_STATIC_TEST, &m_dlgForPanelDialog );
	m_wndDialogPanelCheck1.SubclassDlgItem( IDC_CHECK1, &m_dlgForPanelDialog );
	m_wndDialogPanelCheck2.SubclassDlgItem( IDC_CHECK2, &m_dlgForPanelDialog );
	m_wndHtmlCtrl1.NavigateResourceID( IDR_HTML_DEMO_PAGE1 );
	m_wndHtmlCtrl2.NavigateResourceID( IDR_HTML_DEMO_PAGE2 );
	m_wndEdit.ModifyStyleEx(0,WS_EX_CLIENTEDGE,SWP_FRAMECHANGED);
	m_wndEdit.SetFont(
		CFont::FromHandle(
			(HFONT)::GetStockObject(DEFAULT_GUI_FONT)
			)
		);
	m_wndEdit.SetWindowText( _T("Fixed size panel\r\nwith edit control") );
	m_dlgForPanelPalette.ShowSizeGrip( FALSE );
	m_dlgForPanelDialog.ShowSizeGrip( FALSE );
	m_dlgForPanelTools.ShowSizeGrip( FALSE );
CImageList _ilTools;
	VERIFY( _ilTools.Create( IDB_BITMAP_TOOLS, 16, 1, RGB(255,0,255) ) );
	if( m_dlgForPanelTools.m_wndGripper.GetSafeHwnd() != NULL )
		m_dlgForPanelTools.m_wndGripper.SetWindowPos( &CWnd::wndBottom, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOSENDCHANGING|SWP_HIDEWINDOW );
HWND hWndToolBtn = ::GetWindow( m_dlgForPanelTools.m_hWnd, GW_CHILD );
int iToolBtn;
	for( iToolBtn=0; iToolBtn < 16; iToolBtn++, hWndToolBtn = ::GetWindow(hWndToolBtn,GW_HWNDNEXT) )
	{
		ASSERT( hWndToolBtn != NULL );
		ASSERT( ::IsWindow(hWndToolBtn) );
		if( iToolBtn == 0 )
		{
			m_hWndSelectedToolButton = hWndToolBtn; 
			::InvalidateRect(
				m_hWndSelectedToolButton,
				NULL,
				FALSE
				);
		}
		
		if( CWnd::FromHandlePermanent( hWndToolBtn ) != NULL )
			continue;

		CDynamicButton * pBtn =
			new CDynamicButton;
		pBtn->SubclassWindow( hWndToolBtn );
		pBtn->SetFlat( TRUE );
		pBtn->SetIcon( _ilTools.ExtractIcon( iToolBtn ) );
		CExtSafeString sToolTip;
		sToolTip.Format(
			_T("Tool button %d"),
			iToolBtn + 1
			);
		pBtn->SetTooltipText( sToolTip );
	}
	hWndToolBtn = ::GetWindow( m_dlgForPanelPalette.m_hWnd, GW_CHILD );
	ASSERT( hWndToolBtn != NULL );
	ASSERT( ::IsWindow(hWndToolBtn) );
	
	// skip resizing gripper
	hWndToolBtn = ::GetWindow(hWndToolBtn,GW_HWNDNEXT);
	ASSERT( hWndToolBtn != NULL );
	ASSERT( ::IsWindow(hWndToolBtn) );

	m_wndColorStatic.SubclassWindow( hWndToolBtn );
	hWndToolBtn = ::GetWindow(hWndToolBtn,GW_HWNDNEXT);
	for( iToolBtn=0; iToolBtn < 160; iToolBtn++, hWndToolBtn = ::GetWindow(hWndToolBtn,GW_HWNDNEXT) )
	{
		ASSERT( hWndToolBtn != NULL );
		ASSERT( ::IsWindow(hWndToolBtn) );

		if( CWnd::FromHandlePermanent( hWndToolBtn ) != NULL )
			continue;

		COLORREF clr;
		if( iToolBtn < 32 )
		{
			double fColorSrc = double(iToolBtn)/32.0;
			clr = 
				CExtBitmap::stat_HLStoRGB(
					0.0,
					sqrt( fColorSrc ),
					0.0
					);
		} // if( iToolBtn < 32 )
		else
		{
			int iColorSrc = iToolBtn-32;
			int iSat = iColorSrc/32;
			ASSERT( iSat >= 0 && iSat <= 3 );
			double fSat = 0.15;
			if( iSat == 1 )
				fSat = 0.3;
			else if( iSat == 2 )
				fSat = 0.5;
			else if( iSat == 3 )
				fSat = 1.0;

			iColorSrc = iColorSrc % 32;
			double fColorSrc = double(iColorSrc)/32.0;
			clr = 
				CExtBitmap::stat_HLStoRGB(
					fColorSrc,
					0.5,
					fSat
					);
		} // else from if( iToolBtn < 32 )
		CDynamicPaletteButton * pBtn =
			new CDynamicPaletteButton( clr );
		pBtn->SubclassWindow( hWndToolBtn );
		CExtSafeString sToolTip;
		sToolTip.Format(
			_T("Palette button %d - #%02X%02X%02X"),
			iToolBtn + 1,
			int( GetRValue(clr) ),
			int( GetGValue(clr) ),
			int( GetBValue(clr) )
			);
		pBtn->SetTooltipText( sToolTip );
	}

	if(		!m_wndStatusBar.Create(this)
		||	!m_wndStatusBar.SetIndicators(
				indicators,
				sizeof(indicators) / sizeof(UINT)
			)
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarUiLook.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPanelEmpty.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPanelCustomDrawn.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPanelEdit.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPanelDialog.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPanelTools.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	m_wndPanelPalette.EnableDocking(CBRS_ALIGN_BOTTOM);
	m_wndResizableBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar2.EnableDocking(CBRS_ALIGN_ANY);

	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
	VERIFY(
		g_CmdManager->SetBasicCommands(
			pApp->m_pszProfileName,
			g_statBasicCommands
			)
		);

	if( !CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{
		DockControlBar( &m_wndMenuBar );
		DockControlBar( &m_wndToolBar );
		RecalcLayout();

		CRect wrAlreadyDockedBar;
		
		DockControlBar( &m_wndPanelCustomDrawn );
		m_wndPanelCustomDrawn.GetWindowRect( &wrAlreadyDockedBar );
		wrAlreadyDockedBar.OffsetRect( 0, 1 );
		DockControlBar( &m_wndPanelEmpty, AFX_IDW_DOCKBAR_RIGHT, &wrAlreadyDockedBar );
		
		DockControlBar( &m_wndPanelEdit );
		m_wndPanelEdit.GetWindowRect( &wrAlreadyDockedBar );
		wrAlreadyDockedBar.OffsetRect( 0, 1 );
		DockControlBar( &m_wndPanelDialog, AFX_IDW_DOCKBAR_LEFT, &wrAlreadyDockedBar );
		
		DockControlBar( &m_wndPanelTools );
		DockControlBar( &m_wndPanelPalette );
		DockControlBar( &m_wndResizableBar1, AFX_IDW_DOCKBAR_LEFT );
		DockControlBar( &m_wndResizableBar2, AFX_IDW_DOCKBAR_RIGHT );

		DockControlBar( &m_wndToolBarUiLook, AFX_IDW_DOCKBAR_RIGHT );
		
		ShowControlBar( &m_wndPanelEmpty, FALSE, TRUE );
		ShowControlBar( &m_wndPanelEdit, FALSE, TRUE );
		ShowControlBar( &m_wndPanelDialog, FALSE, TRUE );
		ShowControlBar( &m_wndResizableBar2, FALSE, TRUE );

		RecalcLayout();
	}
	g_CmdManager->SerializeState(
		pApp->m_pszProfileName,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		false
		);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CFrameWnd > :: PreCreateWindow( cs ) )
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CExtNCW < CFrameWnd > :: AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CExtNCW < CFrameWnd > :: Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	pOldWnd;
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CExtNCW < CFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}


BOOL CMainFrame::PreTranslateMessage( MSG * pMsg ) 
{
	if( m_wndToolBarUiLook.PreTranslateMessage( pMsg ) )
		return TRUE;
	if( m_wndMenuBar.TranslateMainFrameMessage( pMsg ) )
		return TRUE;
	return CExtNCW < CFrameWnd > :: PreTranslateMessage( pMsg );
}

BOOL CMainFrame::DestroyWindow() 
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	VERIFY(
		CExtControlBar::ProfileBarStateSave(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		);
	VERIFY(
		g_CmdManager->SerializeState(
			pApp->m_pszProfileName,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			true
			)
		);
	VERIFY(
		g_PaintManager.PaintManagerStateSave(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		);

	g_CmdManager->ProfileWndRemove( GetSafeHwnd() );
	
	return CExtNCW < CFrameWnd > :: DestroyWindow();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CExtNCW < CFrameWnd > :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	
	CExtNCW < CFrameWnd > :: ActivateFrame( nCmdShow );
}

void CMainFrame::OnClickedToolButton( UINT nID )
{
	ASSERT( IDC_BUTTON1 <= nID && nID <= IDC_BUTTON16 );
CString sMsg;
	sMsg.Format(
		_T("Tool button %d clicked"),
		nID - IDC_BUTTON1 + 1
		);
	::AfxMessageBox( sMsg );
}

void CMainFrame::OnClickedPaletteButtonL( UINT nID )
{
	ASSERT( IDC_BUTTON17 <= nID && nID <= IDC_BUTTON176 );
CDynamicPaletteButton * pBtn = (CDynamicPaletteButton *)
		CWnd::FromHandlePermanent(
			::GetDlgItem( m_dlgForPanelPalette.m_hWnd, nID ) 
			);
	ASSERT_VALID( pBtn );
	m_wndColorStatic.m_clrIn = pBtn->m_clr;
	m_wndColorStatic.Invalidate();
CString sMsg;
	sMsg.Format(
		_T("Palette button %d clicked (left mouse button)\nColor is #%02X%02X%02X"),
		nID - IDC_BUTTON17 + 1,
		int( GetRValue(pBtn->m_clr) ),
		int( GetGValue(pBtn->m_clr) ),
		int( GetBValue(pBtn->m_clr) )
		);
	::AfxMessageBox( sMsg );
}

void CMainFrame::OnClickedPaletteButtonR( UINT nID )
{
	nID -= 160;
	ASSERT( IDC_BUTTON17 <= nID && nID <= IDC_BUTTON176 );
CDynamicPaletteButton * pBtn = (CDynamicPaletteButton *)
		CWnd::FromHandlePermanent(
			::GetDlgItem( m_dlgForPanelPalette.m_hWnd, nID ) 
			);
	ASSERT_VALID( pBtn );
	m_wndColorStatic.m_clrOut = pBtn->m_clr;
	m_wndColorStatic.Invalidate();
CString sMsg;
	sMsg.Format(
		_T("Palette button %d clicked (right mouse button)\nColor is #%02X%02X%02X"),
		nID - IDC_BUTTON17 + 1,
		int( GetRValue(pBtn->m_clr) ),
		int( GetGValue(pBtn->m_clr) ),
		int( GetBValue(pBtn->m_clr) )
		);
	::AfxMessageBox( sMsg );
}
