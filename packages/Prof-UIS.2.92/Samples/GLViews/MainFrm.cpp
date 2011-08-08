// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GLViews.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static struct
{
	GLfloat
		m_fLocalTranslationX,
		m_fLocalTranslationY,
		m_fLocalTranslationZ,
		m_fLocalAngleRadsPitch,
		m_fLocalAngleRadsYaw,
		m_fLocalAngleRadsRoll;
} g_arrInitCameraData[ GL_VIEWS_CAMERA_COUNT ] =
{
	{     0.00f,   0.00f,   0.04f,       -0.0f,  -0.0f,  0.0f    },    //  0
	{     0.20f,   0.20f,   0.20f,        0.7f,  -0.4f,  0.0f    },    //  1
	{    -0.10f,  -0.30f,  -0.10f,       -1.6f,   0.4f,  0.0f    },    //  2
	{    -0.20f,   0.00f,   0.20f,       -0.0f,   0.5f,  0.0f    },    //  3
	{     0.00f,  -0.20f,  -0.20f,       -0.8f,  -0.0f,  0.0f    },    //  4
	{     0.40f,   0.00f,   0.00f,       -0.0f,  -1.1f,  0.0f    },    //  5
	{     0.31f,  -0.15f,   0.00f,       -0.3f,  -1.2f,  0.0f    },    //  6
	{    -0.15f,   0.31f,   0.00f,        1.2f,   0.6f,  0.0f    },    //  7
	{     0.28f,  -0.09f,   0.24f,       -1.4f,  -1.5f,  0.0f    },    //  8
	{    -0.09f,   0.31f,  -0.05f,        1.7f,  -0.0f,  0.0f    },    //  9
};

/////////////////////////////////////////////////////////////////////////////
// CSimpleSplashWnd

static bool g_bSplashWndClassRegistered = false;

CSimpleSplashWnd::CSimpleSplashWnd()
	: m_sStatusText( _T("Initializing ...") )
{
	VERIFY( RegisterSplashWndClass() );
}

CSimpleSplashWnd::CSimpleSplashWnd(
	CWnd * pWndParent,
	UINT nBitmapID
	)
	: m_sStatusText( _T("Initializing ...") )
{
	VERIFY( Create( pWndParent, nBitmapID ) );
}

CSimpleSplashWnd::~CSimpleSplashWnd()
{
}


BEGIN_MESSAGE_MAP(CSimpleSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSimpleSplashWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	__EXT_MFC_ON_WM_NCHITTEST()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSimpleSplashWnd message handlers

void CSimpleSplashWnd::SetStatusText(
	LPCTSTR sStatusText
	)
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
LPCTSTR sText = (sStatusText == NULL) ? _T("") : sStatusText;
	if( m_sStatusText == sText )
		return;
	m_sStatusText = sText;
	if( (GetStyle() & WS_VISIBLE) == 0 )
		return;
	Invalidate();
	UpdateWindow();
}

bool CSimpleSplashWnd::Create(
	CWnd * pWndParent,
	UINT nBitmapID
	)
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() == NULL );
	ASSERT( m_bitmap.GetSafeHandle() == NULL );
	if( ! RegisterSplashWndClass() )
	{
		ASSERT( FALSE );
		return false;
	}

	if( ! m_bitmap.LoadBitmap(nBitmapID) )
	{
		ASSERT( FALSE );
		return false;
	}
BITMAP _bmpInfo;
	::memset( &_bmpInfo, 0, sizeof(BITMAP) );
	m_bitmap.GetBitmap( &_bmpInfo );
	m_sizeBitmap.cx = _bmpInfo.bmWidth;
	m_sizeBitmap.cy = _bmpInfo.bmHeight;
CRect rcDesktop;
	VERIFY( ::SystemParametersInfo( SPI_GETWORKAREA, 0, PVOID(&rcDesktop), 0 ) );
CRect rcWnd( 0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy );
	rcWnd.OffsetRect(
		( rcDesktop.Width() - m_sizeBitmap.cx ) / 2,
		( rcDesktop.Height() - m_sizeBitmap.cy ) / 2
		);
	if( ! m_wndInvisibleParent.CreateEx( 0, _T("STATIC"), _T(""), WS_POPUP, 0,0,0,0, pWndParent->GetSafeHwnd(), (HMENU)NULL ) )
	{
		ASSERT( FALSE );
		m_bitmap.DeleteObject();
		return false;
	}
	if( ! CWnd::CreateEx(
			0, GL_VIEWS_SPLASHWND_WNDCLASS, _T("GLViews initizlizing ..."), WS_POPUP,
			rcWnd.left, rcWnd.top, m_sizeBitmap.cx, m_sizeBitmap.cy,
			m_wndInvisibleParent.GetSafeHwnd(), (HMENU)NULL
			)
		)
	{
		ASSERT( FALSE );
		m_bitmap.DeleteObject();
		m_wndInvisibleParent.DestroyWindow();
		return false;
	}
	VERIFY(
		::SetWindowPos(
			m_hWnd, HWND_TOP, 0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW|SWP_NOACTIVATE|SWP_NOOWNERZORDER
			)
		);
	CExtPopupMenuWnd::PassMsgLoop( false );
	return true;
}

BOOL CSimpleSplashWnd::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}

void CSimpleSplashWnd::OnPaint() 
{
	ASSERT( m_bitmap.GetSafeHandle() != NULL );
CPaintDC dcPaint( this );
CDC dcMem;
	if( !dcMem.CreateCompatibleDC( &dcPaint ) )
	{
		ASSERT( FALSE );
		return;
	}
CBitmap * pOldBmp = dcMem.SelectObject( &m_bitmap );
	dcPaint.BitBlt(
		0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy,
		&dcMem,
		0, 0,
		SRCCOPY
		);
	if( !m_sStatusText.IsEmpty() )
	{
		CRect rcText;
		GetClientRect( &rcText );
		rcText.bottom -= 6;
		rcText.top = rcText.bottom - 48;
		rcText.DeflateRect( 16, 0 );
		int nOldBkMode = dcPaint.SetBkMode( TRANSPARENT );
		COLORREF clrTextOld = dcPaint.SetTextColor( RGB(0,0,0) );
		CFont * pOldFont = dcPaint.SelectObject( &g_PaintManager->m_FontNormal );
		CExtRichContentLayout::stat_DrawText(
			dcPaint.m_hDC,
			LPCTSTR(m_sStatusText),
			&rcText,
			DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS, 0
			);
		dcPaint.SelectObject( pOldFont );
		dcPaint.SetTextColor( clrTextOld );
		dcPaint.SetBkMode( nOldBkMode );
	} // if( !m_sStatusText.IsEmpty() )
	dcMem.SelectObject( pOldBmp );
}

void CSimpleSplashWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	bCalcValidRects;
	lpncsp;
}

UINT CSimpleSplashWnd::OnNcHitTest(CPoint point) 
{
	point;
	return HTCLIENT;
}

void CSimpleSplashWnd::OnClose() 
{
}

BOOL CSimpleSplashWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ( !RegisterSplashWndClass() )
		|| ( !CWnd::PreCreateWindow(cs) )
		)
	{
		ASSERT( FALSE );
		return FALSE;
	}

	cs.lpszClass = GL_VIEWS_SPLASHWND_WNDCLASS;

	return TRUE;
}

bool CSimpleSplashWnd::RegisterSplashWndClass()
{
	if( g_bSplashWndClassRegistered )
		return true;

WNDCLASS _wndClassInfo;
HINSTANCE hInst = AfxGetInstanceHandle();
	if( ! ::GetClassInfo(
			hInst,
			GL_VIEWS_SPLASHWND_WNDCLASS,
			&_wndClassInfo
			)
		)
	{
		// otherwise we need to register a new class
		_wndClassInfo.style =
				CS_GLOBALCLASS
				//|CS_DBLCLKS
				|CS_HREDRAW|CS_VREDRAW
				|CS_NOCLOSE
				|CS_SAVEBITS
				;
		_wndClassInfo.lpfnWndProc = ::DefWindowProc;
		_wndClassInfo.cbClsExtra = _wndClassInfo.cbWndExtra = 0;
		_wndClassInfo.hInstance = hInst;
		_wndClassInfo.hIcon = NULL;
		_wndClassInfo.hCursor =
				::LoadCursor(
					NULL, //hInst,
					IDC_WAIT
					)
				;
		ASSERT( _wndClassInfo.hCursor != NULL );
		_wndClassInfo.hbrBackground = NULL; 
		_wndClassInfo.lpszMenuName = NULL;
		_wndClassInfo.lpszClassName = GL_VIEWS_SPLASHWND_WNDCLASS;
		if( !::AfxRegisterClass( &_wndClassInfo ) )
		{
			ASSERT( FALSE );
			//AfxThrowResourceException();
			return false;
		}
	}

	g_bSplashWndClassRegistered = true;
	return true;
}

BOOL CSimpleSplashWnd::DestroyWindow() 
{
	ShowWindow( SW_HIDE );
	CExtPopupMenuWnd::PassMsgLoop( false );
	CWnd::DestroyWindow();
	if( m_wndInvisibleParent.GetSafeHwnd() != NULL )
		m_wndInvisibleParent.DestroyWindow();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSimpleHtmlCtrl

CSimpleHtmlCtrl::CSimpleHtmlCtrl()
	: m_pBrowser( NULL )
{
}

CSimpleHtmlCtrl::~CSimpleHtmlCtrl()
{
}

BEGIN_MESSAGE_MAP(CSimpleHtmlCtrl, CWnd)
	//{{AFX_MSG_MAP(CSimpleHtmlCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleHtmlCtrl message handlers

bool CSimpleHtmlCtrl::Create( CWnd * pWndParent )
{
UINT nID = pWndParent->GetDlgCtrlID();
CString sWindowName;
	pWndParent->GetWindowText( sWindowName );
	if(	! CreateControl(
			CLSID_WebBrowser,
			LPCTSTR(sWindowName),
			WS_VISIBLE|WS_CHILD,
			CRect(0,0,0,0),
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

void CSimpleHtmlCtrl::NavigateURL( LPCTSTR lpszURL )
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

void CSimpleHtmlCtrl::NavigateResourceID(
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
// CMainFrame

IMPLEMENT_DYNAMIC( CMainFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_ANIMATE, OnAnimate)
	ON_UPDATE_COMMAND_UI(ID_ANIMATE, OnUpdateAnimate)
	ON_COMMAND(ID_SHOW_CAMERAS, OnShowCameras)
	ON_UPDATE_COMMAND_UI(ID_SHOW_CAMERAS, OnUpdateShowCameras)
	ON_COMMAND(ID_SHOW_MIRROR, OnShowMirror)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MIRROR, OnUpdateShowMirror)
	ON_COMMAND(ID_SHOW_AVI_PLAYER, OnShowAviPlayer)
	ON_UPDATE_COMMAND_UI(ID_SHOW_AVI_PLAYER, OnUpdateShowAviPlayer)
	ON_COMMAND(ID_RESET_CAMERAS, OnResetCameras)
	ON_COMMAND(ID_SHOW_OUTER_OBJECTS, OnShowOuterObjects)
	ON_UPDATE_COMMAND_UI(ID_SHOW_OUTER_OBJECTS, OnUpdateShowOuterObjects)
	ON_COMMAND(ID_SHOW_TEXT_OBJECTS, OnShowTextObjects)
	ON_UPDATE_COMMAND_UI(ID_SHOW_TEXT_OBJECTS, OnUpdateShowTextObjects)
	ON_COMMAND(ID_SHOW_CUBE_OBJECTS, OnShowCubeObjects)
	ON_UPDATE_COMMAND_UI(ID_SHOW_CUBE_OBJECTS, OnUpdateShowCubeObjects)
	ON_COMMAND(ID_SHOW_PLANET_OBJECTS, OnShowPlanetObjects)
	ON_UPDATE_COMMAND_UI(ID_SHOW_PLANET_OBJECTS, OnUpdateShowPlanetObjects)
	//}}AFX_MSG_MAP

	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_BAR_CAM0, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_BAR_CAM0, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_BAR_CAM1, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_BAR_CAM1, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_BAR_TREE, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_BAR_TREE, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_BAR_HELP, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_BAR_HELP, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_UI_LOOK_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_UI_LOOK_BAR, OnUpdateControlBarMenu)

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
	: m_wndViewCamMain( IDC_CURSOR_GL_GREEN )
	, m_wndViewCam0( IDC_CURSOR_GL_RED )
	, m_wndViewCam1( IDC_CURSOR_GL_BLUE )
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
			RUNTIME_CLASS(CExtPaintManagerOffice2010_R1)
			);
	
	CExtPopupMenuWnd::g_bMenuExpanding = false;
	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;
	if( ! g_PaintManager.m_bIsWin2000orLater )
		CExtPopupMenuWnd::g_bMenuWithShadows = false;

	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	CExtControlBar::DoFrameBarCheckUpdate(
		this,
		pCmdUI,
		false
		);
}

BOOL CMainFrame::OnBarCheck(UINT nID)
{
	return
		CExtControlBar::DoFrameBarCheckCmd(
			this,
			nID,
			false
			);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( CExtNCW < CFrameWnd > :: OnCreate( lpCreateStruct ) == -1 )
	{
		ASSERT( FALSE );
		return -1;
	}

CSimpleSplashWnd _splash( this, IDB_BITMAP_SPLASH );

	_splash.SetStatusText(
		_T("Initializing the Command Manager ...")
		);

HINSTANCE hInst = ::AfxGetInstanceHandle();
HICON hIcon =
		::LoadIcon(
			hInst,
			MAKEINTRESOURCE(IDR_MAINFRAME)
			);
	ASSERT( hIcon != NULL );
	SetIcon( hIcon, TRUE );
	hIcon = (HICON)
		::LoadImage(
			hInst,
			MAKEINTRESOURCE(IDR_MAINFRAME),
			IMAGE_ICON,
			16,
			16,
			0
			);
	ASSERT( hIcon != NULL );
	SetIcon( hIcon, FALSE );

CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

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
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_MENU_CAM_ROTATE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_MENU_CAM_TRANSLATE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_TOOLBAR_INVIEW
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_TOOLBAR_HELPER_ICONS
			)
		);

CExtCmdItem * pCmd =
		g_CmdManager->CmdGetPtr( pApp->m_pszProfileName, ID_BTN_MENU_ROTATION );
	ASSERT( pCmd != NULL );
	pCmd->m_sMenuText = _T("Camera &Rotation");
	pCmd =
		g_CmdManager->CmdGetPtr( pApp->m_pszProfileName, ID_BTN_MENU_TRANSLATION );
	ASSERT( pCmd != NULL );
	pCmd->m_sMenuText = _T("Camera &Translation");

	// create a view to occupy the client area of the frame
	_splash.SetStatusText(
		_T("Creating the user interface: main view ...")
		);
	if(	!m_wndViewCamMain.Create(
			NULL,
			NULL,
			WS_CHILD|WS_VISIBLE,
			CRect( 0, 0, 0, 0 ),
			this,
			AFX_IDW_PANE_FIRST,
			NULL
			)
		)
	{
		ASSERT( FALSE );
		TRACE0("Failed to create view window\n");
		return -1;
	}

	_splash.SetStatusText(
		_T("Creating the user interface: bars ...")
		);
	if( ! m_wndMenuBar.Create(
			NULL, // _T("Menu Bar"),
			this,
			ID_VIEW_MENUBAR
			)
		)
    {
		ASSERT( FALSE );
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }

	if(		(! m_wndToolBarUiLook.Create( NULL, this, ID_VIEW_UI_LOOK_BAR ) )
		||	(! m_wndToolBarUiLook.ThemeSwitcherInit() )
		)
    {
		ASSERT( FALSE );
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }

	_splash.SetStatusText(
		_T("Creating the user interface: dockable view 0 ...")
		);
	if(	!m_wndResizableBar0.Create(
			NULL,
			this,
			ID_VIEW_BAR_CAM0
			)
		)
	{
		ASSERT( FALSE );
		TRACE0("Failed to create m_wndResizableBar0\n");
		return -1;		// fail to create
	}
	if(	!m_wndViewCam0.Create(
			NULL,
			NULL,
			WS_CHILD|WS_VISIBLE,
			CRect( 0, 0, 0, 0 ),
			&m_wndResizableBar0,
			ID_VIEW_BAR_CAM0,
			NULL
			)
		)
	{
		ASSERT( FALSE );
		return -1;		// fail to create
	}

	_splash.SetStatusText(
		_T("Creating the user interface: dockable view 1 ...")
		);
	if(	!m_wndResizableBar1.Create(
			NULL,
			this,
			ID_VIEW_BAR_CAM1
			)
		)
	{
		ASSERT( FALSE );
		TRACE0("Failed to create m_wndResizableBar1\n");
		return -1;		// fail to create
	}
	if(	!m_wndViewCam1.Create(
			NULL,
			NULL,
			WS_CHILD|WS_VISIBLE,
			CRect( 0, 0, 0, 0 ),
			&m_wndResizableBar1,
			ID_VIEW_BAR_CAM1,
			NULL
			)
		)
	{
		ASSERT( FALSE );
		return -1;		// fail to create
	}

	_splash.SetStatusText(
		_T("Creating the user interface: hierarchy view ...")
		);
	if(	!m_wndResizableBar2.Create(
			NULL,
			this,
			ID_VIEW_BAR_TREE
			)
		)
	{
		ASSERT( FALSE );
		TRACE0("Failed to create m_wndResizableBar2\n");
		return -1;		// fail to create
	}
	if(	!m_wndObjectHierarchyTree.Create(
			WS_CHILD|WS_VISIBLE
				|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS
				|TVS_INFOTIP|TVS_TRACKSELECT
				|TVS_NONEVENHEIGHT|TVS_SHOWSELALWAYS,
			CRect( 0, 0, 0, 0 ),
			&m_wndResizableBar2,
			ID_VIEW_BAR_TREE
			)
		)
	{
		ASSERT( FALSE );
		return -1;		// fail to create
	}
	m_wndObjectHierarchyTree.ModifyStyleEx( 0, WS_EX_STATICEDGE, SWP_FRAMECHANGED );
	VERIFY(
		m_ilObjectHierarchyTree.Create(
			IDB_BITMAP_TREE_IMG_LIST,
			16,
			1,
			RGB(255,0,255)
			)
		);
	m_wndObjectHierarchyTree.SetImageList(
		&m_ilObjectHierarchyTree,
		TVSIL_NORMAL
		);

	_splash.SetStatusText(
		_T("Creating the user interface: help view ...")
		);
	if(	!m_wndResizableBar3.Create(
			NULL,
			this,
			ID_VIEW_BAR_HELP
			)
		)
	{
		ASSERT( FALSE );
		TRACE0("Failed to create m_wndResizableBar3\n");
		return -1;		// fail to create
	}
	if(	!m_wndHelpHtmlCtrl.Create(
			&m_wndResizableBar3
			)
		|| !( (new CSimpleThinFrame)->
				CreateDynamicThinFrame(
					&m_wndHelpHtmlCtrl
					)
				)
		)
	{
		ASSERT( FALSE );
		return -1;		// fail to create
	}
	m_wndHelpHtmlCtrl.NavigateResourceID(
		ID_WEB_BROWSER_PAGE
		);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	_splash.SetStatusText(
		_T("Configuring control bars ...")
		);

	m_wndResizableBar0.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar3.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarUiLook.EnableDocking(CBRS_ALIGN_ANY);

	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( !CExtControlBar::FrameInjectAutoHideAreas(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	_splash.SetStatusText(
		_T("Loading last UI state ...")
		);
	if( ! CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{
		_splash.SetStatusText(
			_T("Docking control bars ...")
			);
		
		DockControlBar( &m_wndToolBarUiLook, AFX_IDW_DOCKBAR_RIGHT );
		RecalcLayout();
		
		CSize _sizeDesired( 200, 200 );
		m_wndResizableBar0.SetInitDesiredSizeFloating(   _sizeDesired );
		m_wndResizableBar0.SetInitDesiredSizeHorizontal( _sizeDesired );
		m_wndResizableBar0.SetInitDesiredSizeVertical(   _sizeDesired );
		m_wndResizableBar1.SetInitDesiredSizeFloating(   _sizeDesired );
		m_wndResizableBar1.SetInitDesiredSizeHorizontal( _sizeDesired );
		m_wndResizableBar1.SetInitDesiredSizeVertical(   _sizeDesired );
		m_wndResizableBar2.SetInitDesiredSizeFloating(   _sizeDesired );
		m_wndResizableBar2.SetInitDesiredSizeHorizontal( _sizeDesired );
		m_wndResizableBar2.SetInitDesiredSizeVertical(   _sizeDesired );
		m_wndResizableBar3.SetInitDesiredSizeFloating(   _sizeDesired );
		m_wndResizableBar3.SetInitDesiredSizeHorizontal( _sizeDesired );
		m_wndResizableBar3.SetInitDesiredSizeVertical(   _sizeDesired );

		m_wndResizableBar1.DockControlBar( AFX_IDW_DOCKBAR_LEFT, 1, this, false );
		m_wndResizableBar1.DockControlBar( &m_wndResizableBar0, true, true, this, false );
		m_wndResizableBar2.DockControlBar( AFX_IDW_DOCKBAR_RIGHT, 1, this, false );
		m_wndResizableBar3.DockControlBar( AFX_IDW_DOCKBAR_BOTTOM, 2, this, false );
		
		RecalcLayout();
	}

	_splash.SetStatusText(
		_T("Initializing 3D objects ...")
		);

	ASSERT_VALID( m_wndViewCamMain.m_wndGlPanel.GetView3D() );
	ASSERT_VALID( m_wndViewCam0.m_wndGlPanel.GetView3D() );
	ASSERT_VALID( m_wndViewCam1.m_wndGlPanel.GetView3D() );
	m_wndViewCamMain.m_wndGlPanel.GetView3D()->m_strViewName = _T("main view (green cursor)");
	m_wndViewCam0.m_wndGlPanel.GetView3D()->m_strViewName = _T("view 0 (red cursor)");
	m_wndViewCam1.m_wndGlPanel.GetView3D()->m_strViewName = _T("view 1 (blue cursor)");

C3DObject * pObjRoot3D = the3DPipe.GetRoot();
	ASSERT_VALID( pObjRoot3D );
	pObjRoot3D->AddChild( new C3DOuterScene );

C3DMirror * pObjWndMirror = new C3DMirror;
	pObjRoot3D->AddChild( pObjWndMirror );

C3DAviPlayer * pObjWndAviPlayer = new C3DAviPlayer;
	pObjRoot3D->AddChild( pObjWndAviPlayer );

C3DModifier * pObjCenterCubeModifier = new C3DModifier;
C3DCube * pObjCubeCenter = new C3DCube( GL_VIEWS_NONAME, true );
	pObjCenterCubeModifier->AddChild( pObjCubeCenter );
	pObjRoot3D->AddChild( pObjCenterCubeModifier );
C3DCube * pObjCubeLeaf = new C3DCube;
	pObjCubeCenter->AddChild( pObjCubeLeaf );
	pObjCubeLeaf->m_v3LocalTranslation += _v3t( 0.0f, 0.0f, C3DCube::g_fTranslateCubeVal );
	pObjCubeLeaf = new C3DCube;
	pObjCubeCenter->AddChild( pObjCubeLeaf );
	pObjCubeLeaf->m_v3LocalTranslation += _v3t( 0.0f, 0.0f, -C3DCube::g_fTranslateCubeVal );
	pObjCubeLeaf = new C3DCube;
	pObjCubeCenter->AddChild( pObjCubeLeaf );
	pObjCubeLeaf->m_v3LocalTranslation += _v3t( 0.0f, C3DCube::g_fTranslateCubeVal, 0.0f );
	pObjCubeLeaf = new C3DCube;
	pObjCubeCenter->AddChild( pObjCubeLeaf );
	pObjCubeLeaf->m_v3LocalTranslation += _v3t( 0.0f, -C3DCube::g_fTranslateCubeVal, 0.0f );
	pObjCubeLeaf = new C3DCube;
	pObjCubeCenter->AddChild( pObjCubeLeaf );
	pObjCubeLeaf->m_v3LocalTranslation += _v3t( C3DCube::g_fTranslateCubeVal, 0.0f, 0.0f );
	pObjCubeLeaf = new C3DCube;
	pObjCubeCenter->AddChild( pObjCubeLeaf );
	pObjCubeLeaf->m_v3LocalTranslation += _v3t( -C3DCube::g_fTranslateCubeVal, 0.0f, 0.0f );

C3DModifier * pObjModifierForEarth = new C3DModifier;
C3DPlanet * pObjPlanetEarth =
		new C3DPlanet(
			GL_VIEWS_PLANET_EARTH,
			GL_VIEWS_TEXTURE_IDX_EARTH,
			C3DPlanet::g_nPlanetSphereStacks,
			C3DPlanet::g_fPlanetRadiusEarth,
			C3DPlanet::g_fAnglePlayStepPitchEarth,
			C3DPlanet::g_fAnglePlayStepYawEarth,
			C3DPlanet::g_fAnglePlayStepRollEarth
			);
	pObjPlanetEarth->m_v3LocalTranslation += _v3t( 0.0f, C3DCube::g_fTranslateCubeVal*4.0f, 0.0f );
	pObjModifierForEarth->AddChild( pObjPlanetEarth );
	pObjCubeCenter->AddChild( pObjModifierForEarth );

C3DModifier * pObjModifierForMoon = new C3DModifier;
C3DPlanet * pObjPlanetMoon =
		new C3DPlanet(
			GL_VIEWS_PLANET_MOON,
			GL_VIEWS_TEXTURE_IDX_MOON,
			C3DPlanet::g_nPlanetSphereStacks,
			C3DPlanet::g_fPlanetRadiusMoon,
			C3DPlanet::g_fAnglePlayStepPitchMoon,
			C3DPlanet::g_fAnglePlayStepYawMoon,
			C3DPlanet::g_fAnglePlayStepRollMoon
			);
	pObjPlanetMoon->m_v3LocalTranslation += _v3t( 0.0f, C3DCube::g_fTranslateCubeVal, 0.0f );
	pObjModifierForMoon->AddChild( pObjPlanetMoon );
	pObjPlanetEarth->AddChild( pObjModifierForMoon );

static TCHAR sTextProfUIS[] = _T("Professional User Interface Suite");
C3DModifier * pObjTextGroupProfUIS = new C3DModifier( sTextProfUIS );
	pObjCubeCenter->AddChild( pObjTextGroupProfUIS );
static const int nLenTextProfUIS = sizeof( sTextProfUIS ) / sizeof( TCHAR ) - 1;
_q4t _qTextProfUISRotAdjust2( _ntr::get_pi() / 2.0f, 0.0, -1.0, 0.0 );
	_qTextProfUISRotAdjust2 *= _q4t( _ntr::get_pi() / 2.0f, 0.0, 0.0, 1.0 );
int nChar;
	for( nChar = 0; nChar < nLenTextProfUIS; nChar++ )
	{
		if( sTextProfUIS[nChar] == _T(' ') )
			continue;
		TCHAR sText[2] = { sTextProfUIS[nChar], _T('\0') };
		C3DText * pObjTextLetter =
			new C3DText(
				sText,
				RGB( 16, 128, 16 ),
				_ntr::d2r( 0.0f ),
				_ntr::d2r( 0.0f ),
				_ntr::d2r( 2.0f )
				);
		pObjTextLetter->m_v3LocalTranslation +=
			_v3t( C3DCube::g_fTranslateCubeVal*2.0f, 0.0f, 0.0f );
		_q4t _qTextProfUISRotAdjust1(
			0.12f * float(nChar), 0.0f, 0.0f, 1.0f );
		pObjTextLetter->m_quatLocalOrientation = _qTextProfUISRotAdjust2;
		C3DModifier * pObjModifier = new C3DModifier;
		pObjModifier->m_quatLocalOrientation = _qTextProfUISRotAdjust1;
		pObjTextGroupProfUIS->AddChild( pObjModifier );
		pObjModifier->AddChild( pObjTextLetter );
	}

static TCHAR sTextWWW[] = _T("http://www.prof-uis.com");
C3DModifier * pObjTextGroupWWW = new C3DModifier( sTextWWW );
	pObjCubeCenter->AddChild( pObjTextGroupWWW );
static const int nLenTextWWW = sizeof( sTextWWW ) / sizeof( TCHAR ) - 1;
_q4t _qTextWWWRotAdjust2( _ntr::get_pi() / 2.0f, 0.0, -1.0, 0.0 );
	_qTextWWWRotAdjust2 *= _q4t( _ntr::get_pi() / 2.0f, 0.0, 0.0, 1.0 );
	for( nChar = 0; nChar < nLenTextWWW; nChar++ )
	{
		if( sTextWWW[nChar] == _T(' ') )
			continue;
		TCHAR sText[2] = { sTextWWW[nChar], _T('\0') };
		C3DText * pObjTextLetter =
			new C3DText(
				sText,
				RGB( 16, 16, 128 ),
				_ntr::d2r( 0.0f ),
				_ntr::d2r( 2.0f ),
				_ntr::d2r( 0.0f )
				);
		pObjTextLetter->m_v3LocalTranslation +=
			_v3t( C3DCube::g_fTranslateCubeVal*2.2f, 0.0f, 0.0f );
		_q4t _qTextWWWRotAdjust1(
			0.13f * float(nChar), 0.0f, 0.0f, 1.0f );
		pObjTextLetter->m_quatLocalOrientation = _qTextWWWRotAdjust2;
		C3DModifier * pObjModifier = new C3DModifier;
		pObjModifier->m_quatLocalOrientation = _qTextWWWRotAdjust1;
		pObjTextGroupWWW->AddChild( pObjModifier );
		pObjModifier->AddChild( pObjTextLetter );
	}

	_splash.SetStatusText(
		_T("Initializing cameras ...")
		);
	for( int nCamIdx = 0; nCamIdx < GL_VIEWS_CAMERA_COUNT; nCamIdx++ )
	{
		CString sCamName;
		sCamName.Format( GL_VIEWS_CAMERA_NAME_FMT, nCamIdx );
		C3DCamera * pCam =
			new C3DCamera(
				ID_SELCAM0 + nCamIdx,
				(LPCTSTR)sCamName
				);
		the3DPipe.AddCamera( pCam );
	}
	_ResetCameras( false );
	SerializeCameraState(
		GL_VIEWS_CAMERA_STATE,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		false
		);
	SyncCameraFovValue();

	_splash.SetStatusText(
		_T("Initializing the Object Hierarchy View ...")
		);
	the3DPipe.GetRoot()->WalkTree(
		C3DObject::EWTQ_ADD_TO_TREE,
		NULL,
		NULL,
		&m_wndObjectHierarchyTree,
		NULL
		);
	m_wndObjectHierarchyTree.Expand(  // expand root
		the3DPipe.GetRoot()->GetHTREEITEM(),
		TVE_EXPAND
		);

	_splash.SetStatusText(
		_T("Initializing the 3D Redering Pipe Thread ...")
		);
	if(		(!pApp->SetThreadPriority(THREAD_PRIORITY_HIGHEST))
		||	(!the3DPipe.CreateThread(CREATE_SUSPENDED))
		||	(!the3DPipe.SetThreadPriority(THREAD_PRIORITY_HIGHEST))
		||	the3DPipe.ResumeThread() == 0xFFFFFFFF
		)
	{
		ASSERT( FALSE );
		return -1;
	}

	_splash.DestroyWindow();

	PostMessage( GL_VIEWS_WM_RENDER_INITIAL );

	return 0;
}

void CMainFrame::_ResetCameras( bool bLockViews )
{
	ASSERT_VALID( this );
	if( bLockViews )
	{
		int nViewCount = the3DPipe.GetViewCount();
		for( int nViewIdx = 0; nViewIdx < nViewCount; nViewIdx++ )		
		{
			C3DView * pView3D = the3DPipe.GetView( nViewIdx );
			ASSERT_VALID( pView3D );
			pView3D->Lock();
		}
	}
int nCamCount = the3DPipe.GetCameraCount();
	ASSERT( nCamCount == GL_VIEWS_CAMERA_COUNT );
	for( int nCamIdx = 0; nCamIdx < nCamCount; nCamIdx++ )
	{
		C3DCamera * pCam = the3DPipe.GetCamera( nCamIdx );
		ASSERT_VALID( pCam );
		pCam->m_fNearPlane = GL_VIEWS_CAMERA_DEF_NEAR_PLANE;
		pCam->m_fFarPlane = GL_VIEWS_CAMERA_DEF_FAR_PLANE;
		pCam->m_nFovIndex = GL_VIEWS_FOV_DEF_INDEX;
		ASSERT( 0 <= pCam->m_nFovIndex && pCam->m_nFovIndex < GL_VIEWS_FOV_COUNT );
		pCam->LocalLoadIdentity();
		pCam->m_v3LocalTranslation =
			_v3t(
				g_arrInitCameraData[nCamIdx].m_fLocalTranslationX,
				g_arrInitCameraData[nCamIdx].m_fLocalTranslationY,
				g_arrInitCameraData[nCamIdx].m_fLocalTranslationZ
				);
		pCam->LocalAdjustOrientation(
			g_arrInitCameraData[nCamIdx].m_fLocalAngleRadsPitch,
			g_arrInitCameraData[nCamIdx].m_fLocalAngleRadsYaw,
			g_arrInitCameraData[nCamIdx].m_fLocalAngleRadsRoll
			);
	} // for( int nCamIdx = 0; nCamIdx < nCamCount; nCamIdx++ )
	if( bLockViews )
	{
		int nViewCount = the3DPipe.GetViewCount();
		for( int nViewIdx = 0; nViewIdx < nViewCount; nViewIdx++ )		
		{
			C3DView * pView3D = the3DPipe.GetView( nViewIdx );
			ASSERT_VALID( pView3D );
			pView3D->Unlock();
		}
	}
	SyncCameraFovValue();
}

static CString productsection2regkeypath(
	LPCTSTR sProfileName,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	)
{
	ASSERT( sProfileName != NULL );
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );

	// use the same version numbers with Prof-UIS
DWORD dwVersion = g_CmdManager.GetVersionDWORD( false );
	ASSERT( dwVersion != 0 );
CString strVer;
	strVer.Format(
		_T("%d%d%d%d"),
		INT( (dwVersion>>24)&0x0FF ),
		INT( (dwVersion>>16)&0x0FF ),
		INT( (dwVersion>>8)&0x0FF ),
		INT( dwVersion&0x0FF )
		);

CString s;
	s.Format(
		_T("Software\\%s\\%s\\%s\\fmt-spec-%s"),
		sSectionNameCompany,
		sSectionNameProduct,
		sProfileName,
		strVer
		);
	s.Replace('\r','_');
	s.Replace('\t','_');
	s.Replace('\n','_');
	s.Replace('?','_');
	s.Replace('*','_');
	s.Replace('.','_');
	s.Replace(',','_');
	return s;
}

static bool fileobj_to_registry(
	CFile & _file,
	LPCTSTR sProfileName,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	)
{
	ASSERT( sProfileName != NULL );
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
CString sRegKeyPath =
		productsection2regkeypath(
			sProfileName,
			sSectionNameCompany,
			sSectionNameProduct
			);

	return CExtCmdManager::FileObjToRegistry( _file, (LPCTSTR)sRegKeyPath );
}

static bool fileobj_from_registry(
	CFile & _file,
	LPCTSTR sProfileName,
	LPCTSTR sSectionNameCompany, // under HKEY_CURRENT_USER\Software
	LPCTSTR sSectionNameProduct // under HKEY_CURRENT_USER\Software\%sSectionNameCompany%
	)
{
	ASSERT( sProfileName != NULL );
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
	ASSERT( _file.GetLength() == 0 );
CString sRegKeyPath =
		productsection2regkeypath(
			sProfileName,
			sSectionNameCompany,
			sSectionNameProduct
			);
	return CExtCmdManager::FileObjFromRegistry( _file, (LPCTSTR)sRegKeyPath );
}

void CMainFrame::SerializeCameraState(
	LPCTSTR sProfileName,
	LPCTSTR sSectionNameCompany,
	LPCTSTR sSectionNameProduct,
	bool bSave
	)
{
	ASSERT_VALID( this );
	ASSERT( sProfileName != NULL );
	ASSERT( sSectionNameCompany != NULL );
	ASSERT( sSectionNameProduct != NULL );
	try
	{
		CMemFile _file;
		if( bSave )
		{
			{ // BLOCK: CArchive usage
				CArchive ar(
					&_file,
					CArchive::store
					);
				SerializeCameraState( ar );
				ar.Flush();
			} // BLOCK: CArchive usage

			// ... write _file to registty
			_file.Seek( 0, CFile::begin );
			if( !fileobj_to_registry(
					_file,
					sProfileName,
					sSectionNameCompany,
					sSectionNameProduct
					)
				)
				return;

		} // if( bSave )
		else
		{
			// ... read _file from registty
			if( !fileobj_from_registry(
					_file,
					sProfileName,
					sSectionNameCompany,
					sSectionNameProduct
					)
				)
			{
				int nCamIndexMain = GL_VIEWS_CAMERA_DEFIDX_MAIN,
					nCamIndex0 = GL_VIEWS_CAMERA_DEFIDX_0,
					nCamIndex1 = GL_VIEWS_CAMERA_DEFIDX_1;
				m_wndViewCamMain.m_wndToolbar.m_wndCameraSelCombo.SetCurSel( nCamIndexMain );
				m_wndViewCam0.m_wndToolbar.m_wndCameraSelCombo.SetCurSel( nCamIndex0 );
				m_wndViewCam1.m_wndToolbar.m_wndCameraSelCombo.SetCurSel( nCamIndex1 );
				return;
			}
			_file.Seek( 0, CFile::begin );

			CArchive ar(
				&_file,
				CArchive::load
				);
			SerializeCameraState( ar );
		} // else from if( bSave )
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )
}

void CMainFrame::SerializeCameraState(
	CArchive & ar
	)
{
	ASSERT_VALID( this );
int nCamIndexMain = GL_VIEWS_CAMERA_DEFIDX_MAIN,
	nCamIndex0 = GL_VIEWS_CAMERA_DEFIDX_0,
	nCamIndex1 = GL_VIEWS_CAMERA_DEFIDX_1;
	try
	{
		if( ar.IsStoring() )
		{
			nCamIndexMain = m_wndViewCamMain.m_wndToolbar.m_wndCameraSelCombo.GetCurSel();
			nCamIndex0 = m_wndViewCam0.m_wndToolbar.m_wndCameraSelCombo.GetCurSel();
			nCamIndex1 = m_wndViewCam1.m_wndToolbar.m_wndCameraSelCombo.GetCurSel();
			ASSERT( 0 <= nCamIndexMain && nCamIndexMain < GL_VIEWS_CAMERA_COUNT );
			ASSERT( 0 <= nCamIndex0 && nCamIndex0 < GL_VIEWS_CAMERA_COUNT );
			ASSERT( 0 <= nCamIndex1 && nCamIndex1 < GL_VIEWS_CAMERA_COUNT );
			ar << DWORD(nCamIndexMain);
			ar << DWORD(nCamIndex0);
			ar << DWORD(nCamIndex1);
		} // if( ar.IsStoring() )
		else
		{
			DWORD dwTmp;
			ar >> dwTmp; nCamIndexMain = int(dwTmp);
			ar >> dwTmp; nCamIndex0 = int(dwTmp);
			ar >> dwTmp; nCamIndex1 = int(dwTmp);
			ASSERT( 0 <= nCamIndexMain && nCamIndexMain < GL_VIEWS_CAMERA_COUNT );
			ASSERT( 0 <= nCamIndex0 && nCamIndex0 < GL_VIEWS_CAMERA_COUNT );
			ASSERT( 0 <= nCamIndex1 && nCamIndex1 < GL_VIEWS_CAMERA_COUNT );
		} // else from if( ar.IsStoring() )

		int nCamCount = the3DPipe.GetCameraCount();
		ASSERT( nCamCount == GL_VIEWS_CAMERA_COUNT );
		for( int nCamIdx = 0; nCamIdx < nCamCount; nCamIdx++ )
		{
			C3DCamera * pCam = the3DPipe.GetCamera( nCamIdx );
			ASSERT_VALID( pCam );
			pCam->SerializeState( ar );
		} // for( int nCamIdx = 0; nCamIdx < nCamCount; nCamIdx++ )
	} // try
	catch( CException * pXept )
	{
		pXept->Delete();
		ASSERT( FALSE );
	} // catch( CException * pXept )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )

	if( !ar.IsStoring() )
	{
		if( 0 > nCamIndexMain || nCamIndexMain >= GL_VIEWS_CAMERA_COUNT )
			nCamIndexMain = 8;
		if( 0 > nCamIndex0 || nCamIndex0 >= GL_VIEWS_CAMERA_COUNT )
			nCamIndex0 = 9;
		if( 0 > nCamIndex1 || nCamIndex1 >= GL_VIEWS_CAMERA_COUNT )
			nCamIndex1 = 5;
		m_wndViewCamMain.m_wndToolbar.m_wndCameraSelCombo.SetCurSel( nCamIndexMain );
		m_wndViewCam0.m_wndToolbar.m_wndCameraSelCombo.SetCurSel( nCamIndex0 );
		m_wndViewCam1.m_wndToolbar.m_wndCameraSelCombo.SetCurSel( nCamIndex1 );
	} // if( !ar.IsStoring() )
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
	m_wndViewCamMain.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	switch( nID )
	{
	case ID_SELCAM0:
	case ID_SELCAM1:
	case ID_SELCAM2:
	case ID_SELCAM3:
	case ID_SELCAM4:
	case ID_SELCAM5:
	case ID_SELCAM6:
	case ID_SELCAM7:
	case ID_SELCAM8:
	case ID_SELCAM9:
	case ID_ROTATE_ABOUT_X0:
	case ID_ROTATE_ABOUT_X1:
	case ID_ROTATE_ABOUT_Y0:
	case ID_ROTATE_ABOUT_Y1:
	case ID_ROTATE_ABOUT_Z0:
	case ID_ROTATE_ABOUT_Z1:
	case ID_TRANSLATE_X0:
	case ID_TRANSLATE_X1:
	case ID_TRANSLATE_Y0:
	case ID_TRANSLATE_Y1:
	case ID_TRANSLATE_Z0:
	case ID_TRANSLATE_Z1:
	case ID_CAM_FOV_INC:
	case ID_CAM_FOV_DEC:
		{
			HWND hWndFocus = ::GetFocus();
			if( hWndFocus != NULL )
			{
				if(		m_wndViewCam0.m_hWnd != NULL
					&&	::IsWindowVisible( m_wndViewCam0.m_hWnd )
					&&	::IsChild( m_wndViewCam0.m_hWnd, hWndFocus )
					)
				{
					if( m_wndViewCam0.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) )
						return TRUE;
				}
				if(		m_wndViewCam1.m_hWnd != NULL
					&&	::IsWindowVisible( m_wndViewCam1.m_hWnd )
					&&	::IsChild( m_wndViewCam1.m_hWnd, hWndFocus )
					)
				{
					if( m_wndViewCam1.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) )
						return TRUE;
				}
			} // if( hWndFocus != NULL )
		}
		break;
	} // switch( nID )

	if( m_wndViewCamMain.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) )
		return TRUE;

	// otherwise, do default handling
	return CExtNCW < CFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == GL_VIEWS_WM_RENDER_INITIAL )
	{
		the3DPipe.Render();
		return TRUE;
	}
	if( m_wndToolBarUiLook.PreTranslateMessage( pMsg ) )
		return TRUE;
	if( m_wndMenuBar.TranslateMainFrameMessage( pMsg ) )
		return TRUE;
	return CExtNCW < CFrameWnd > :: PreTranslateMessage( pMsg );
}

BOOL CMainFrame::DestroyWindow() 
{
	m_wndViewCamMain.m_wndGlPanel.TimerAnimationEnable( false );

	the3DPipe.ShutdownAndWaitFor();
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	SerializeCameraState(
		GL_VIEWS_CAMERA_STATE,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		true
		);

	VERIFY(
		CExtControlBar::ProfileBarStateSave(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
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

void CMainFrame::OnAnimate() 
{
	ASSERT_VALID( this );
	m_wndViewCamMain.m_wndGlPanel.TimerAnimationEnable(
		!the3DPipe.m_bTimerAnimationEnabled
		);
}
void CMainFrame::OnUpdateAnimate(CCmdUI* pCmdUI) 
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
	pCmdUI->Enable();
	pCmdUI->SetCheck(
		the3DPipe.m_bTimerAnimationEnabled ? TRUE : FALSE
		);
}

void CMainFrame::OnShowCameras() 
{
	ASSERT_VALID( this );
	C3DCamera::g_bRenderCameraAxes =
		!C3DCamera::g_bRenderCameraAxes;
	the3DPipe.Render();
}
void CMainFrame::OnUpdateShowCameras(CCmdUI* pCmdUI) 
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
	pCmdUI->Enable();
	pCmdUI->SetCheck(
		C3DCamera::g_bRenderCameraAxes ? TRUE : FALSE
		);
}

void CMainFrame::OnShowMirror() 
{
	ASSERT_VALID( this );
	C3DMirror::g_bRenderMirrors =
		!C3DMirror::g_bRenderMirrors;
	the3DPipe.Render();
}
void CMainFrame::OnUpdateShowMirror(CCmdUI* pCmdUI) 
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
	pCmdUI->Enable();
	pCmdUI->SetCheck(
		C3DMirror::g_bRenderMirrors ? TRUE : FALSE
		);
}

void CMainFrame::OnShowAviPlayer() 
{
	ASSERT_VALID( this );
	C3DAviPlayer::g_bRenderAviPlayers =
		!C3DAviPlayer::g_bRenderAviPlayers;
	the3DPipe.Render();
}
void CMainFrame::OnUpdateShowAviPlayer(CCmdUI* pCmdUI) 
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
	pCmdUI->Enable();
	pCmdUI->SetCheck(
		C3DAviPlayer::g_bRenderAviPlayers ? TRUE : FALSE
		);
}

void CMainFrame::OnShowOuterObjects() 
{
	ASSERT_VALID( this );
	C3DOuterScene::g_bRenderOuterScene =
		!C3DOuterScene::g_bRenderOuterScene;
	the3DPipe.Render();
}
void CMainFrame::OnUpdateShowOuterObjects(CCmdUI* pCmdUI) 
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
	pCmdUI->Enable();
	pCmdUI->SetCheck(
		C3DOuterScene::g_bRenderOuterScene ? TRUE : FALSE
		);
}

void CMainFrame::OnShowTextObjects() 
{
	ASSERT_VALID( this );
	C3DText::g_bRenderTextObjects =
		!C3DText::g_bRenderTextObjects;
	the3DPipe.Render();
}
void CMainFrame::OnUpdateShowTextObjects(CCmdUI* pCmdUI) 
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
	pCmdUI->Enable();
	pCmdUI->SetCheck(
		C3DText::g_bRenderTextObjects ? TRUE : FALSE
		);
}

void CMainFrame::OnShowCubeObjects() 
{
	ASSERT_VALID( this );
	C3DCube::g_bRenderCubeObjects =
		!C3DCube::g_bRenderCubeObjects;
	the3DPipe.Render();
}
void CMainFrame::OnUpdateShowCubeObjects(CCmdUI* pCmdUI) 
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
	pCmdUI->Enable();
	pCmdUI->SetCheck(
		C3DCube::g_bRenderCubeObjects ? TRUE : FALSE
		);
}

void CMainFrame::OnShowPlanetObjects() 
{
	ASSERT_VALID( this );
	C3DPlanet::g_bRenderPlanetObjects =
		!C3DPlanet::g_bRenderPlanetObjects;
	the3DPipe.Render();
}
void CMainFrame::OnUpdateShowPlanetObjects(CCmdUI* pCmdUI) 
{
	ASSERT_VALID( this );
	ASSERT( pCmdUI != NULL );
	pCmdUI->Enable();
	pCmdUI->SetCheck(
		C3DPlanet::g_bRenderPlanetObjects ? TRUE : FALSE
		);
}

void CMainFrame::OnResetCameras() 
{
	ASSERT_VALID( this );
CString sMsg;
	sMsg.Format(
		_T("This command will reset position, orientation ")
		_T("and FOV parameters for all %d cameras. ")
		_T("Whish you see cameras in initial state?"),
		GL_VIEWS_CAMERA_COUNT
		);
	if( ::AfxMessageBox(
			(LPCTSTR)sMsg,
			MB_YESNO|MB_ICONQUESTION
			) != IDYES
		)
		return;
	_ResetCameras( true );
	SyncCameraFovValue();
	the3DPipe.Render();
}

void CMainFrame::SyncCameraFovValue( int nCamIdx, int nFovIdx )
{
	ASSERT_VALID( this );
	ASSERT( 0 <= nCamIdx && nCamIdx < GL_VIEWS_CAMERA_COUNT );
	ASSERT( 0 <= nFovIdx && nFovIdx < GL_VIEWS_FOV_COUNT );
	if( m_wndViewCamMain.m_wndToolbar.m_wndCameraSelCombo.GetCurSel() == nCamIdx )
		m_wndViewCamMain.m_wndToolbar.m_wndCameraFovCombo.SetCurSel( nFovIdx );
	if( m_wndViewCam0.m_wndToolbar.m_wndCameraSelCombo.GetCurSel() == nCamIdx )
		m_wndViewCam0.m_wndToolbar.m_wndCameraFovCombo.SetCurSel( nFovIdx );
	if( m_wndViewCam1.m_wndToolbar.m_wndCameraSelCombo.GetCurSel() == nCamIdx )
		m_wndViewCam1.m_wndToolbar.m_wndCameraFovCombo.SetCurSel( nFovIdx );
}

void CMainFrame::SyncCameraFovValue()
{
	the3DPipe.ObjectWriteAccessSet( true );
		int nCamCount = the3DPipe.GetCameraCount();
		ASSERT( nCamCount == GL_VIEWS_CAMERA_COUNT );
		for( int nCamIdx = 0; nCamIdx < nCamCount; nCamIdx++ )
		{
			C3DCamera * pCam = the3DPipe.GetCamera( nCamIdx );
			ASSERT_VALID( pCam );
			SyncCameraFovValue( nCamIdx, pCam->m_nFovIndex );
		} // for( int nCamIdx = 0; nCamIdx < nCamCount; nCamIdx++ )
	the3DPipe.ObjectWriteAccessSet( false );
}

