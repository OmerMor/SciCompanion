// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProfUIS_Controls.h"
#include "MainDlg.h"

#include <intshcut.h>

#if (!defined __EXT_REGISTRY_H)
	#include <ExtRegistry.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_FAVORITES_FIRSTURL  40000
#define ID_FAVORITES_LASTURL   45000

typedef BOOL (CALLBACK* LPFNADDFAV)(HWND, TCHAR*, UINT, TCHAR*, UINT, LPITEMIDLIST);
typedef UINT (CALLBACK* LPFNORGFAV)(HWND, LPTSTR);

/////////////////////////////////////////////////////////////////////////////
// CSplashWnd

static bool g_bSplashWndClassRegistered = false;

CSplashWnd::CSplashWnd(
	CWnd * pWndParent,
	UINT nBitmapID
	)
	: m_nMaxTextLines( -1 )
	, m_nLineHeight( -1 )
	, m_clrText( RGB(255,255,255) )
	, m_rcText( 0, 0, 0, 0 )
{
	VERIFY( RegisterSplashWndClass() );
	VERIFY( Create( pWndParent, nBitmapID ) );
}

CSplashWnd::~CSplashWnd()
{
	if( GetSafeHwnd() != NULL )
		DestroyWindow();
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	__EXT_MFC_ON_WM_NCHITTEST()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSplashWnd message handlers

void CSplashWnd::AddTextLine(
	LPCTSTR sText // = NULL
	)
{
	ASSERT_VALID( this );
	m_arrTextLines.Add( (sText == NULL) ? _T("") : sText );
	if( GetSafeHwnd() == NULL )
		return;
	if( (GetStyle() & WS_VISIBLE) == 0 )
		return;
	Invalidate();
	UpdateWindow();
}

void CSplashWnd::ReplaceLastLine(
	LPCTSTR sText // = NULL
	)
{
	if( m_arrTextLines.GetSize() == 0 )
		return;
	m_arrTextLines.ElementAt( m_arrTextLines.GetSize() - 1 ) = (sText == NULL) ? _T("") : sText;
	if( GetSafeHwnd() == NULL )
		return;
	if( (GetStyle() & WS_VISIBLE) == 0 )
		return;
	Invalidate();
	UpdateWindow();
}

void CSplashWnd::ClearLines()
{
	ASSERT_VALID( this );
	if( m_arrTextLines.GetSize() == 0 )
		return;
	m_arrTextLines.RemoveAll();
	if( GetSafeHwnd() == NULL )
		return;
	if( (GetStyle() & WS_VISIBLE) == 0 )
		return;
	Invalidate();
	UpdateWindow();
}

bool CSplashWnd::Create(
	CWnd * pWndParent,
	UINT nBitmapID
	)
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() == NULL );
	ASSERT( m_bitmap.GetSafeHandle() == NULL );
	ASSERT( m_wndInvisibleParent.GetSafeHwnd() == NULL );
	if( ! RegisterSplashWndClass() )
	{
		ASSERT( FALSE );
		return false;
	}
	if( ! m_bitmap.LoadBitmap( nBitmapID ) )
	{
		ASSERT( FALSE );
		return false;
	}
BITMAP _bmpInfo;
	::memset( &_bmpInfo, 0, sizeof(BITMAP) );
	m_bitmap.GetBitmap( &_bmpInfo );
	m_sizeBitmap.cx = _bmpInfo.bmWidth;
	m_sizeBitmap.cy = _bmpInfo.bmHeight;
CExtPaintManager::monitor_parms_t _mp;
	CExtPaintManager::stat_GetMonitorParms( _mp, pWndParent );
CRect rcWnd( 0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy );
	rcWnd.OffsetRect(
		( _mp.m_rcWorkArea.Width() - m_sizeBitmap.cx ) / 2,
		( _mp.m_rcWorkArea.Height() - m_sizeBitmap.cy ) / 2
		);
	if( ! m_wndInvisibleParent.CreateEx( 0, _T("STATIC"), _T(""), WS_POPUP, 0,0,0,0, pWndParent->GetSafeHwnd(), (HMENU)NULL ) )
	{
		ASSERT( FALSE );
		m_bitmap.DeleteObject();
		return false;
	}
	if( ! CWnd::CreateEx(
			0, __SIMPLE_SPLASH_WINDOW_CLASS_NAME__, _T("Initizlizing ..."), WS_POPUP,
			rcWnd.left, rcWnd.top, m_sizeBitmap.cx, m_sizeBitmap.cy, m_wndInvisibleParent.GetSafeHwnd(), (HMENU)NULL
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

BOOL CSplashWnd::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}

void CSplashWnd::OnPaint() 
{
	ASSERT( m_bitmap.GetSafeHandle() != NULL );
CPaintDC dcPaint( this );
CExtMemoryDC dc( &dcPaint );
CDC dcImg;
	if( ! dcImg.CreateCompatibleDC( &dcPaint ) )
	{
		ASSERT( FALSE );
		return;
	}
CBitmap * pOldBmp = dcImg.SelectObject( &m_bitmap );
	dc.BitBlt(
		0, 0, m_sizeBitmap.cx, m_sizeBitmap.cy,
		&dcImg,
		0, 0,
		SRCCOPY
		);
	dcImg.SelectObject( pOldBmp );
	dcImg.DeleteDC();
int nLen, i, nCount = int( m_arrTextLines.GetSize() );
	if( nCount > 0 )
	{
		if( m_rcText.IsRectEmpty() )
		{
			GetClientRect( &m_rcText );
			m_rcText.DeflateRect(
				10,
				30,
				10,
				10
				);
		}
		if( m_font.GetSafeHandle() == 0 )
		{
			LOGFONT _lf;
			::memset( &_lf, 0, sizeof(LOGFONT) );
			g_PaintManager->m_FontNormal.GetLogFont( &_lf );
			__EXT_MFC_STRCPY( _lf.lfFaceName, LF_FACESIZE, _T("Arial") );
			_lf.lfWeight = 300;
			_lf.lfItalic = FALSE;
			_lf.lfUnderline = FALSE;
			_lf.lfStrikeOut = FALSE;
			_lf.lfWidth = 0;
			_lf.lfHeight = 12;
			VERIFY( m_font.CreateFontIndirect( &_lf ) );
		}
		int nOldBkMode = dc.SetBkMode( TRANSPARENT );
		COLORREF clrTextOld = dc.SetTextColor( m_clrText );
		CFont * pOldFont = dc.SelectObject( &m_font );
		if( m_nLineHeight < 0 )
		{
			TEXTMETRIC _tm;
			::memset( &_tm, 0, sizeof(TEXTMETRIC) );
			dc.GetTextMetrics( &_tm );
			m_nLineHeight = _tm.tmHeight;
		}
		if( m_nMaxTextLines < 0 )
		{
			m_nMaxTextLines = m_rcText.Height() / m_nLineHeight;
			if( m_nMaxTextLines < 0 )
				m_nMaxTextLines = 0;
		}
		if( nCount > m_nMaxTextLines )
		{
			m_arrTextLines.RemoveAt( 0, nCount - m_nMaxTextLines );
			nCount = m_nMaxTextLines;
		}
		int nY = m_rcText.top;
		for( i = 0; i < nCount; i++ )
		{
			LPCTSTR sText = m_arrTextLines[i];
			if( sText != NULL && ( nLen = int( _tcslen(sText) ) ) > 0 )
			{
				CRect rcText = m_rcText;
				rcText.OffsetRect(
					0,
					nY - rcText.top
					);
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					sText, nLen,
					&rcText,
					DT_SINGLELINE|DT_TOP|DT_RIGHT|DT_END_ELLIPSIS, 0
					);
			} // if( sText != NULL && ( nLen = int( _tcslen(sText) ) ) > 0 )
			nY += m_nLineHeight;
		} // for( i = 0; i < nCount; i++ )
		dc.SelectObject( pOldFont );
		dc.SetTextColor( clrTextOld );
		dc.SetBkMode( nOldBkMode );
	} // if( nCount > 0 )
}

void CSplashWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	bCalcValidRects;
	lpncsp;
}

UINT CSplashWnd::OnNcHitTest(CPoint point) 
{
	point;
	return HTCLIENT;
}

void CSplashWnd::OnClose() 
{
}

BOOL CSplashWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ( ! RegisterSplashWndClass() ) || ( ! CWnd::PreCreateWindow( cs ) ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	cs.lpszClass = __SIMPLE_SPLASH_WINDOW_CLASS_NAME__;
	return TRUE;
}

bool CSplashWnd::RegisterSplashWndClass()
{
	if( g_bSplashWndClassRegistered )
		return true;

WNDCLASS _wndClassInfo;
HINSTANCE hInst = AfxGetInstanceHandle();
	if( ! ::GetClassInfo( hInst, __SIMPLE_SPLASH_WINDOW_CLASS_NAME__, &_wndClassInfo ) )
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
		_wndClassInfo.lpszClassName = __SIMPLE_SPLASH_WINDOW_CLASS_NAME__;
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

BOOL CSplashWnd::DestroyWindow() 
{
	ShowWindow( SW_HIDE );
	CExtPopupMenuWnd::PassMsgLoop( false );
	CWnd::DestroyWindow();
	if( m_wndInvisibleParent.GetSafeHwnd() != NULL )
		m_wndInvisibleParent.DestroyWindow();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

IMPLEMENT_DYNAMIC( CMainDlg, CExtResizableDialog );

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CExtNCW < CExtResizableDialog > ( CMainDlg::IDD, pParent )
	, m_pPageNotAvailable( NULL )
#if (!defined __EXT_MFC_NO_TAB_CTRL)
	, m_pPageTabWindows( NULL )
#endif
	, m_pPageButtons( NULL )
	, m_pPageHyperLinks( NULL )
	, m_pPageAviLabel( NULL )
#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL && !defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL )
	, m_pPageTabContainers( NULL )
#endif
	, m_pPagePopupMenus( NULL )
	, m_pPageStatusBar( NULL )
	, m_pPageToolbars( NULL )
#if (!defined __EXT_MFC_NO_DATE_PICKER)
	, m_pPageCalendar( NULL )
#endif
#if (!defined __EXT_MFC_NO_DURATIONWND && !defined __EXT_MFC_NO_DATETIMEWND )
	, m_pPageDateTimeDuration( NULL )
#endif
#if (!defined __EXT_MFC_NO_DATE_BROWSER )
	, m_pPageDateBrowser( NULL )
#endif
#if (!defined __EXT_MFC_NO_GRIDWND)
	, m_pPageGrid( NULL )
	, m_pPageMaskedEdit( NULL )
#endif
	, m_pPageSystemNumberCurrencyEdit( NULL )
#if ( ! defined __EXT_MFC_NO_LIST_VIEW_CTRL )
	, m_pPageListCtrl( NULL )
#endif
#if ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )
	, m_pPageTreeCtrl( NULL )
#endif
#if ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )
	, m_pPageShellListCtrl( NULL )
#endif
#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )
	, m_pPageShellTreeCtrl( NULL )
#endif
#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL ) && ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )&& ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )
	, m_pPageShellBrowser( NULL )
#endif
#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )
	, m_pPageShellDialogFile( NULL )
#endif
#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )
	, m_pPageShellDialogBrowseFor( NULL )
#endif
	, m_pMalloc( NULL )
{
	//{{AFX_DATA_INIT(CMainDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bDialogInitComplete = false;
	SetAutoSubclassChildControls();
}

CMainDlg::~CMainDlg()
{
	if( m_pMalloc != NULL )
		m_pMalloc->Release();
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CExtNCW < CExtResizableDialog > :: DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CMainDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST, m_wndList);
	DDX_Control( pDX, IDC_MAIN_MENUBAR, m_wndMenuBar );
	DDX_Control( pDX, IDC_MAIN_TOOLBAR, m_wndToolBarUiLook );
}

BEGIN_MESSAGE_MAP( CMainDlg, CExtResizableDialog )
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_COMMAND(ID_FAVORITES_ADD, OnFavoritesAdd)
	ON_COMMAND(ID_FAVORITES_ORGANIZE, OnFavoritesOrganize)
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE( CExtPopupMenuWnd::g_nMsgPrepareOneMenuLevel, OnExtMenuPrepareLevel )
	ON_COMMAND_RANGE( ID_FAVORITES_FIRSTURL, ID_FAVORITES_LASTURL, OnFavoritesURL )

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CMainDlg::OnInitDialog()
{
	if( ! CExtNCW < CExtResizableDialog > :: OnInitDialog() )
	{
		ASSERT( FALSE );
		return FALSE;
	}
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
	
CSplashWnd _splash( this );

	_splash.AddTextLine( _T("Initializing command manager ...") );
	VERIFY( g_CmdManager->ProfileWndAdd( pApp->m_pszProfileName, GetSafeHwnd() ) );
	VERIFY( g_CmdManager->UpdateFromMenu( pApp->m_pszProfileName, IDR_MAIN_MENU ) );
	VERIFY( g_CmdManager->UpdateFromMenu( pApp->m_pszProfileName, IDR_POPUP_MENU ) );
	VERIFY( g_CmdManager->UpdateFromToolBar( pApp->m_pszProfileName, IDR_EDIT ) );

	_splash.AddTextLine( _T("Creating bars ...") );
	ASSERT( m_wndMenuBar.GetSafeHwnd() != NULL );
	if(	! m_wndMenuBar.LoadMenuBar( IDR_MAIN_MENU ) )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	m_wndMenuBar.SetBarStyle( m_wndMenuBar.GetBarStyle() & (~CBRS_GRIPPER) );
	
	ASSERT( m_wndToolBarUiLook.GetSafeHwnd() != NULL );
	if(	! m_wndToolBarUiLook.ThemeSwitcherInit() )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	m_wndToolBarUiLook.SetBarStyle( m_wndToolBarUiLook.GetBarStyle() & (~CBRS_GRIPPER) );
	
	_splash.AddTextLine( _T("Configuring main dialog window ...") );
static UINT g_statBasicCommands[] =
{
	ID_APP_ABOUT,
	ID_APP_EXIT,
	ID_OWNERDRAW_ITEM,
	ID_COLOR_CTRL_8X5,
	ID_COLOR_CTRL_8X2,
	ID_COLOR_CTRL_GRAYSCALE,
	ID_COLOR_CTRL_HLS,
	ID_DATE_PICKER_CTRL,
	ID_PALETTE_MENU,
	ID_FAVORITES_ADD,
	ID_FAVORITES_ORGANIZE,
	0, // end of list
};
	VERIFY( g_CmdManager->SetBasicCommands( pApp->m_pszProfileName, g_statBasicCommands ) );

	SetIcon( m_hIcon, TRUE );
	SetIcon( m_hIcon, FALSE );

	AddAnchor( IDC_LIST, __RDA_LT, __RDA_LB );
	AddAnchor( IDOK, __RDA_RB );
	AddAnchor( IDC_NEXT, __RDA_RB );
	AddAnchor( IDC_BACK, __RDA_RB );
	AddAnchor( IDC_STATIC_DELIVER_FRAME, __RDA_LB, __RDA_RB );
	AddAnchor( IDC_STATIC_PAGES_RECT, __RDA_LT, __RDA_RB );
	
	//////////////////////////////////////////////////////////////////////////
	
CWnd * pWndPageAlignment = GetDlgItem( IDC_STATIC_PAGES_RECT );
	ASSERT( pWndPageAlignment->GetSafeHwnd() != NULL );
CRect rcPages;
	pWndPageAlignment->GetWindowRect( &rcPages );
	ScreenToClient( &rcPages );

HINSTANCE hInstResource =
		::AfxFindResourceHandle(
			MAKEINTRESOURCE( IDR_MAINFRAME ),
			RT_ICON
			);
	ASSERT( hInstResource != NULL );

	_splash.AddTextLine( _T("Creating dialog pages ...") );
	//////////////////////////////////////////////////////////////////////////
	
	//_splash.AddTextLine( _T("... ") );
	m_pPageNotAvailable = new CPageNotAvailable;
	m_pPageNotAvailable->Create(
		CPageNotAvailable::IDD,
		this
		);
	
	//////////////////////////////////////////////////////////////////////////
	
	_splash.AddTextLine( _T("    ... buttons page") );
	m_pPageButtons = new CPageButtons;
	m_pPageButtons->Create(
		CPageButtons::IDD,
		this
		);
	
	//////////////////////////////////////////////////////////////////////////
	
	_splash.AddTextLine( _T("    ... hyper links page") );
	m_pPageHyperLinks = new CPageHyperLinks;
	m_pPageHyperLinks->Create(
		CPageHyperLinks::IDD,
		this
		);
	
	//////////////////////////////////////////////////////////////////////////
	
	_splash.AddTextLine( _T("    ... AVI label page") );
	m_pPageAviLabel = new CPageAviLabel;
	m_pPageAviLabel->Create(
		CPageAviLabel::IDD,
		this
		);

	//////////////////////////////////////////////////////////////////////////

#if (!defined __EXT_MFC_NO_TAB_CTRL)
	_splash.AddTextLine( _T("    ... tab controls page") );
	m_pPageTabWindows = new CPageTabWindow;
	m_pPageTabWindows->Create(
		CPageTabWindow::IDD,
		this
		);
#endif // (!defined __EXT_MFC_NO_TAB_CTRL)
	
	//////////////////////////////////////////////////////////////////////////
	
#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL && !defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL )
	_splash.AddTextLine( _T("    ... tab page containers page") );
	m_pPageTabContainers = new CPageTabContainers;
	m_pPageTabContainers->Create(
		CPageTabContainers::IDD,
		this
		);
#endif // #if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL && !defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL )
	
	//////////////////////////////////////////////////////////////////////////
	
	_splash.AddTextLine( _T("    ... popup menus page") );
	m_pPagePopupMenus = new CPagePopupMenus;
	m_pPagePopupMenus->Create(
		CPagePopupMenus::IDD,
		this
		);
	
	//////////////////////////////////////////////////////////////////////////
	
	_splash.AddTextLine( _T("    ... status bar page") );
	m_pPageStatusBar = new CPageStatusBar;
	m_pPageStatusBar->Create(
		CPageStatusBar::IDD,
		this
		);
	
	//////////////////////////////////////////////////////////////////////////
	
	_splash.AddTextLine( _T("    ... toolbars page") );
	m_pPageToolbars = new CPageToolbars;
	m_pPageToolbars->Create(
		CPageToolbars::IDD,
		this
		);
	
	//////////////////////////////////////////////////////////////////////////

#if (!defined __EXT_MFC_NO_GRIDWND)
	_splash.AddTextLine( _T("    ... masked edit page") );
	m_pPageMaskedEdit = new CPageMaskedEdit;
	m_pPageMaskedEdit->Create(
		CPageMaskedEdit::IDD,
		this
		);
#endif // #if (!defined __EXT_MFC_NO_GRIDWND)	

	//////////////////////////////////////////////////////////////////////////
	
	_splash.AddTextLine( _T("    ... system number and currency editors page") );
	m_pPageSystemNumberCurrencyEdit = new CPageSystemNumberCurrencyEditors;
	m_pPageSystemNumberCurrencyEdit->Create(
		CPageSystemNumberCurrencyEditors::IDD,
		this
		);

	//////////////////////////////////////////////////////////////////////////
	
#if (!defined __EXT_MFC_NO_DATE_PICKER)
	_splash.AddTextLine( _T("    ... calendar page") );
	m_pPageCalendar = new CPageCalendar;
	m_pPageCalendar->Create(
		CPageCalendar::IDD,
		this
		);
#endif // #if (!defined __EXT_MFC_NO_DATE_PICKER)	
	
	//////////////////////////////////////////////////////////////////////////

#if (!defined __EXT_MFC_NO_DURATIONWND && !defined __EXT_MFC_NO_DATETIMEWND )
	_splash.AddTextLine( _T("    ... date/time/duration page") );
	m_pPageDateTimeDuration = new CPageDateTimeDuration;
	m_pPageDateTimeDuration->Create(
		CPageDateTimeDuration::IDD,
		this
		);
#endif
	
	//////////////////////////////////////////////////////////////////////////
	
#if (!defined __EXT_MFC_NO_DATE_BROWSER )
	_splash.AddTextLine( _T("    ... date browser page") );
	m_pPageDateBrowser = new CPageDateBrowser;
	m_pPageDateBrowser->Create(
		CPageDateBrowser::IDD,
		this
		);
#endif
	
	//////////////////////////////////////////////////////////////////////////

#if (!defined __EXT_MFC_NO_GRIDWND)
	_splash.AddTextLine( _T("    ... grid page") );
	m_pPageGrid = new CPageGrid;
	m_pPageGrid->Create(
		CPageGrid::IDD,
		this
		);
#endif // #if (!defined __EXT_MFC_NO_GRIDWND)	

	//////////////////////////////////////////////////////////////////////////

#if ( ! defined __EXT_MFC_NO_LIST_VIEW_CTRL )
	_splash.AddTextLine( _T("    ... list view control page") );
	m_pPageListCtrl = new CPageListCtrl;
	m_pPageListCtrl->Create(
		CPageListCtrl::IDD,
		this
		);
#endif // ( ! defined __EXT_MFC_NO_LIST_VIEW_CTRL )

	//////////////////////////////////////////////////////////////////////////

#if ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )
	_splash.AddTextLine( _T("    ... tree view control page") );
	m_pPageTreeCtrl = new CPageTreeCtrl;
	m_pPageTreeCtrl->Create(
		CPageTreeCtrl::IDD,
		this
		);
#endif // ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )

	//////////////////////////////////////////////////////////////////////////

#if ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )
	_splash.AddTextLine( _T("    ... shell list view control page") );
	m_pPageShellListCtrl = new CPageShellListCtrl;
	m_pPageShellListCtrl->Create(
		CPageShellListCtrl::IDD,
		this
		);
#endif // ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )

	//////////////////////////////////////////////////////////////////////////

#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )
	_splash.AddTextLine( _T("    ... shell tree view control page") );
	m_pPageShellTreeCtrl = new CPageShellTreeCtrl;
	m_pPageShellTreeCtrl->Create(
		CPageShellTreeCtrl::IDD,
		this
		);
#endif // ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )

	//////////////////////////////////////////////////////////////////////////

#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL ) && ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )&& ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )
	_splash.AddTextLine( _T("    ... shell browser page") );
	m_pPageShellBrowser = new CPageShellBrowser;
	m_pPageShellBrowser->Create(
		CPageShellBrowser::IDD,
		this
		);
#endif ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL ) && ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )&& ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )

	//////////////////////////////////////////////////////////////////////////

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )
	_splash.AddTextLine( _T("    ... shell file dialog") );
	m_pPageShellDialogFile = new CPageShellDialogFile;
	m_pPageShellDialogFile->Create(
		CPageShellDialogFile::IDD,
		this
		);
#endif // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

	//////////////////////////////////////////////////////////////////////////

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )
	_splash.AddTextLine( _T("    ... shell browse for folder dialog") );
	m_pPageShellDialogBrowseFor = new CPageShellDialogBrowseFor;
	m_pPageShellDialogBrowseFor->Create(
		CPageShellDialogBrowseFor::IDD,
		this
		);
#endif // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )

	//////////////////////////////////////////////////////////////////////////
	
	// TODO:

	// Color controls
	// Toolbox
	// Shortcut List	
	// Misc Control	(Font combobox, Browse Edit)

	//////////////////////////////////////////////////////////////////////////
	
	_splash.AddTextLine( _T("Registering page windows ...") );
	struct
	{
		LPCTSTR m_strResource;
		INT m_nWidth, m_nHeight;
		UINT m_nLoadImageFlags;
		CPageBase * m_pWndPage;
		LPCTSTR m_strText;
		bool m_bCreateInsideScrollContainer:1;
	} _iconInit[] =
	{
		{	MAKEINTRESOURCE( IDI_BUTTONS ),
			32, 32,
			0,
			m_pPageButtons,
			_T("Buttons"),
			false
		},
		{	MAKEINTRESOURCE( IDI_HYPER_LINKS ),
			32, 32,
			0,
			m_pPageHyperLinks,
			_T("Hyperlinks"),
			false
		},
		{	MAKEINTRESOURCE( IDI_AVI_LABEL ),
			32, 32,
			0,
			m_pPageAviLabel,
			_T("AVI label"),
			false
		},
		{	MAKEINTRESOURCE( IDI_LIST_VIEW_CTRL ),
			32, 32,
			0,
#if ( ! defined __EXT_MFC_NO_LIST_VIEW_CTRL )
			m_pPageListCtrl,
#else
			m_pPageNotAvailable,
#endif // ( ! defined __EXT_MFC_NO_LIST_VIEW_CTRL )
			_T("List View"),
			false
		},
		{	MAKEINTRESOURCE( IDI_TREE_VIEW_CTRL ),
			32, 32,
			0,
#if ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )
			m_pPageTreeCtrl,
#else
			m_pPageNotAvailable,
#endif // ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )
			_T("Tree View"),
			false
		},
		{	MAKEINTRESOURCE( IDI_TAB_WINDOW ),
			32, 32,
			0,
#if (!defined __EXT_MFC_NO_TAB_CTRL)
			m_pPageTabWindows,
#else
			m_pPageNotAvailable,
#endif // (!defined __EXT_MFC_NO_TAB_CTRL)
			_T("Tab Window"),
			false
		},
		{	MAKEINTRESOURCE( IDI_TAB_CONTAINER ),
			32, 32,
			0,
#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL && !defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL )
			m_pPageTabContainers,
#else
			m_pPageNotAvailable,
#endif // #if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL && !defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL )
			_T("Tab Containers"),
			false
		},
		{	MAKEINTRESOURCE( IDI_MASKED_EDIT ),
			32, 32,
			0,
#if (!defined __EXT_MFC_NO_GRIDWND)
			m_pPageMaskedEdit,
#else
			m_pPageNotAvailable,
#endif // (!defined __EXT_MFC_NO_GRIDWND)
			_T("Masked Edit"),
			false
		},
		{	MAKEINTRESOURCE( IDI_SYSTEM_NUMBER_CURRENCY_EDITORS ),
			32, 32,
			0,
			m_pPageSystemNumberCurrencyEdit,
			_T("System Number/Currency"),
			false
		},
		{	MAKEINTRESOURCE( IDI_MENU ),
			32, 32,
			0,
			m_pPagePopupMenus,
			_T("Popup Menus"),
			false
		},
		{	MAKEINTRESOURCE( IDI_STATUS_BAR ),
			32, 32,
			0,
			m_pPageStatusBar,
			_T("Status Bar"),
			false
		},
		{	MAKEINTRESOURCE( IDI_MENU_TOOLBAR ),
			32, 32,
			0,
			m_pPageToolbars,
			_T("Menubar && Toolbars"),
			false
		},
		{	MAKEINTRESOURCE( IDI_CALENDAR ),
			32, 32,
			0,
#if (!defined __EXT_MFC_NO_DATE_PICKER)
			m_pPageCalendar,
#else
			m_pPageNotAvailable,
#endif // #if (!defined __EXT_MFC_NO_DATE_PICKER)	
			_T("Calendar"),
			false
		},
		{	MAKEINTRESOURCE( IDI_DATE_TIME_DURATION ),
			32, 32,
			0,
#if (!defined __EXT_MFC_NO_DURATIONWND && !defined __EXT_MFC_NO_DATETIMEWND )
			m_pPageDateTimeDuration,
#else
			m_pPageNotAvailable,
#endif // (!defined __EXT_MFC_NO_DURATIONWND && !defined __EXT_MFC_NO_DATETIMEWND )
			_T("Date && Time"),
			false
		},
		{	MAKEINTRESOURCE( IDI_DATE_BROWSER ),
			32, 32,
			0,
#if (!defined __EXT_MFC_NO_DATE_BROWSER )
			m_pPageDateBrowser,
#else
			m_pPageNotAvailable,
#endif // (!defined __EXT_MFC_NO_DATE_BROWSER )
			_T("Date Browser"),
			false
		},
		{	MAKEINTRESOURCE( IDI_GRID ),
			32, 32,
			0,
#if (!defined __EXT_MFC_NO_GRIDWND )
			m_pPageGrid,
#else
			m_pPageNotAvailable,
#endif // (!defined __EXT_MFC_NO_GRIDWND )
			_T("Grid"),
			false
		},
		{	MAKEINTRESOURCE( IDI_SHELL_LIST_VIEW_CTRL ),
			32, 32,
			0,
#if ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )
			m_pPageShellListCtrl,
#else
			m_pPageNotAvailable,
#endif // ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )
			_T("Shell List View"),
			false
		},
		{	MAKEINTRESOURCE( IDI_SHELL_TREE_VIEW_CTRL ),
			32, 32,
			0,
#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )
			m_pPageShellTreeCtrl,
#else
			m_pPageNotAvailable,
#endif // ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )
			_T("Shell Tree View"),
			false
		},
		{	MAKEINTRESOURCE( IDI_SHELL_BROWSER ),
			32, 32,
			0,
#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL ) && ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )&& ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )
			m_pPageShellBrowser,
#else
			m_pPageNotAvailable,
#endif // ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL ) && ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )&& ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )
			_T("Shell Browser"),
			false
		},
		{	MAKEINTRESOURCE( IDI_SHELL_DIALOG_FILE ),
			32, 32,
			0,
#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )
			m_pPageShellDialogFile,
#else
			m_pPageNotAvailable,
#endif // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )
			_T("File Dialog"),
			false
		},
		{	MAKEINTRESOURCE( IDI_SHELL_DIALOG_BFF ),
			32, 32,
			0,
#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )
			m_pPageShellDialogBrowseFor,
#else
			m_pPageNotAvailable,
#endif // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )
			_T("Browse for Folder Dialog"),
			false
		},
	};
int nInit;
	for( nInit = 0; nInit < (sizeof(_iconInit)/sizeof(_iconInit[0])) ; nInit++ )
	{
		_iconInit[nInit].m_pWndPage->ModifyStyle( 0, WS_TABSTOP );
		CExtCmdIcon icon;
		icon.AssignFromHICON(
			(HICON) ::LoadImage(
				hInstResource,
				_iconInit[nInit].m_strResource,
				IMAGE_ICON,
				_iconInit[nInit].m_nWidth,
				_iconInit[nInit].m_nHeight,
				_iconInit[nInit].m_nLoadImageFlags
				),
			false
			);
		ASSERT( ! icon.IsEmpty() );
		if(		_iconInit[nInit].m_bCreateInsideScrollContainer
			&&	_iconInit[nInit].m_pWndPage != m_pPageNotAvailable
			)
		{
			CExtScrollContainerWnd * pWndScrollContainer = new CExtScrollContainerWnd;
			pWndScrollContainer->m_bAutoDelete = true;
			pWndScrollContainer->m_bRedirectPreTranslateMessage = true;
			VERIFY( pWndScrollContainer->Create( this, rcPages ) );
			pWndScrollContainer->ModifyStyle( 0, WS_TABSTOP );
			_iconInit[nInit].m_pWndPage->SetParent( pWndScrollContainer );

			CRect rcAlign;
			_iconInit[nInit].m_pWndPage->GetWindowRect( &rcAlign );
			rcAlign.OffsetRect( -rcAlign.TopLeft() );
			_iconInit[nInit].m_pWndPage->MoveWindow( &rcAlign );

			_iconInit[nInit].m_pWndPage->ShowWindow( SW_SHOW );
			pWndScrollContainer->OnSwRecalcLayout( true );
			AddAnchor( pWndScrollContainer->GetSafeHwnd(), __RDA_LT, __RDA_RB );
			m_wndList.AddIcon( icon, _iconInit[nInit].m_strText, pWndScrollContainer->GetSafeHwnd() );
		}
		else
		{
			_iconInit[nInit].m_pWndPage->MoveWindow( &rcPages );
			AddAnchor( _iconInit[nInit].m_pWndPage->GetSafeHwnd(), __RDA_LT, __RDA_RB );
			m_wndList.AddIcon( icon, _iconInit[nInit].m_strText, _iconInit[nInit].m_pWndPage->GetSafeHwnd() );
		}
	} // for( nInit = 0; nInit < (sizeof(_iconInit)/sizeof(_iconInit[0])) ; nInit++ )

	for( nInit = 0; nInit < (sizeof(_iconInit)/sizeof(_iconInit[0])) ; nInit++ )
		_iconInit[nInit].m_pWndPage->PageBase_PostInit();

	_iconInit[0].m_pWndPage->ShowWindow( SW_SHOW );
	m_wndList.SetCurSel( 0 );
	
	// Get a pointer to the IMalloc interface
HRESULT hr = ::SHGetMalloc( &m_pMalloc );
	if( FAILED(hr) )
		return FALSE;

	m_bDialogInitComplete = true;

	_splash.AddTextLine( _T("Initialization complete.") );
	::Sleep( 500 );
	_splash.DestroyWindow();

	//////////////////////////////////////////////////////////////////////////
	
	EnableSaveRestore( _T("Dialog Positions"), _T("MainDlg") );
	CWnd::RepositionBars( 0, 0xFFFF, 0 );

// 	SetRedraw( FALSE );
// 	CExtPopupMenuWnd::PassMsgLoop( true );
// 	::SetActiveWindow( NULL );
// 	SetRedraw( TRUE );
// 	SetActiveWindow();
// 	SetFocus();
// 	RedrawWindow( NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_FRAME );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
	
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMainDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CExtNCW < CExtResizableDialog > :: OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////////////////////////

void CMainDlg::OnAppAbout() 
{
#ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
	VERIFY( ProfUISAbout() );
#endif // #ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
}

//////////////////////////////////////////////////////////////////////////

void CMainDlg::OnSize(UINT nType, int cx, int cy) 
{
	CExtNCW < CExtResizableDialog > :: OnSize( nType, cx, cy );

	if( nType != SIZE_MINIMIZED )
		CWnd::RepositionBars(0,0xFFFF,0);
}

//////////////////////////////////////////////////////////////////////////

void CMainDlg::OnSelchangeList() 
{
int nIndex = m_wndList.GetCurSel();
CListBoxMenuLike::ITEM_DATA * pItemData = m_wndList.m_arrItems[nIndex];
	for( int i = 0; i < m_wndList.m_arrItems.GetSize(); i++ )
	{
		if( nIndex != i )
			::ShowWindow( m_wndList.m_arrItems[i]->m_hWndPage, SW_HIDE );
	}
CWnd * pWndPageAlignment = GetDlgItem( IDC_STATIC_PAGES_RECT );
	ASSERT( pWndPageAlignment->GetSafeHwnd() != NULL );
CRect rcPages;
	pWndPageAlignment->GetWindowRect( &rcPages );
	ScreenToClient( &rcPages );
	::MoveWindow( pItemData->m_hWndPage, rcPages.left, rcPages.top, rcPages.Width(), rcPages.Height(), TRUE );
	::ShowWindow( pItemData->m_hWndPage, SW_SHOW );
}

//////////////////////////////////////////////////////////////////////////

void CMainDlg::OnBack() 
{
	int nIndex = m_wndList.GetCurSel();
	nIndex > 0
		? nIndex--
		: nIndex = m_wndList.GetCount()-1;
	m_wndList.SetCurSel( nIndex );
	OnSelchangeList();
}

//////////////////////////////////////////////////////////////////////////

void CMainDlg::OnNext() 
{
	int nIndex = m_wndList.GetCurSel();
	nIndex < m_wndList.GetCount()-1
		? nIndex++
		: nIndex = 0;
	m_wndList.SetCurSel( nIndex );
	OnSelchangeList();
} 

//////////////////////////////////////////////////////////////////////////

LRESULT CMainDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT lResult = CExtNCW < CExtResizableDialog > :: WindowProc( message, wParam, lParam );
	if( message == WM_SETMESSAGESTRING )
	{
		if( m_pPageStatusBar )
		{
			m_pPageStatusBar->SendMessage( message,wParam,lParam );
		}
		return 0L;
	}
	return lResult;
}

//////////////////////////////////////////////////////////////////////////


#ifdef UNICODE

STDMETHODIMP CMainDlg::StrRetToStr( STRRET StrRet, LPTSTR * str, LPITEMIDLIST pidl )
{
	ASSERT( m_pMalloc != NULL );
HRESULT hr = S_OK;
int cch;
LPSTR strOffset;
	(*str) = NULL;
	switch( StrRet.uType )
	{
	case STRRET_WSTR: 
		cch = (int) ::wcslen( StrRet.pOleStr ) + 1;
		(*str) = (LPTSTR)m_pMalloc->Alloc( cch * sizeof(TCHAR) );
		if( (*str) != NULL )
			::lstrcpyn( *str, StrRet.pOleStr, cch );
		else
			hr = E_FAIL;
	break;
	case STRRET_OFFSET: 
		strOffset = ( ( (char *) pidl ) + StrRet.uOffset );
		cch = ::MultiByteToWideChar( CP_OEMCP, 0, strOffset, -1, NULL, 0 );
		(*str) = (LPTSTR)m_pMalloc->Alloc(cch * sizeof(TCHAR));
		if( (*str) != NULL )
			::MultiByteToWideChar( CP_OEMCP, 0, strOffset, -1, *str, cch );
		else
			hr = E_FAIL;
	break;
	case STRRET_CSTR: 
		cch = ::MultiByteToWideChar( CP_OEMCP, 0, StrRet.cStr, -1, NULL, 0 );
		(*str) = (LPTSTR)m_pMalloc->Alloc(cch * sizeof(TCHAR)); 
		if( (*str) != NULL )
			::MultiByteToWideChar( CP_OEMCP, 0, StrRet.cStr, -1, *str, cch );
		else
			hr = E_FAIL;
	break;
	} 
	return hr;
}

#else

STDMETHODIMP CMainDlg::StrRetToStr( STRRET StrRet, LPTSTR * str, LPITEMIDLIST pidl )
{
	ASSERT( m_pMalloc != NULL );
HRESULT hr = S_OK;
int cch;
LPSTR strOffset;
	(*str) = NULL;
	switch( StrRet.uType )
	{
	case STRRET_WSTR: 
		cch = ::WideCharToMultiByte( CP_ACP, 0, StrRet.pOleStr, -1, NULL, 0, NULL, NULL );
		(*str) = (LPTSTR) m_pMalloc->Alloc( cch * sizeof(TCHAR) ); 
		if( (*str) != NULL )
			::WideCharToMultiByte( CP_ACP, 0, StrRet.pOleStr, -1, *str, cch, NULL, NULL );
		else
			hr = E_FAIL;
	break;
	case STRRET_OFFSET: 
		strOffset = ( ( (char *) pidl ) + StrRet.uOffset );
		cch = (int) ::strlen( strOffset ) + 1;
		*str = (LPTSTR)m_pMalloc->Alloc( cch * sizeof(TCHAR) );
		if( (*str) != NULL )
			__EXT_MFC_STRCPY( *str, cch, strOffset );
		else
			hr = E_FAIL;
	break;
	case STRRET_CSTR: 
		cch = (int) ::strlen( StrRet.cStr ) + 1;
		*str = (LPTSTR)m_pMalloc->Alloc( cch * sizeof(TCHAR) );
		if (*str != NULL)
			__EXT_MFC_STRCPY( *str, cch, StrRet.cStr );
		else
			hr = E_FAIL;
	break;
	} 
	return hr;
}

#endif

STDMETHODIMP CMainDlg::ResolveChannel( IShellFolder * pFolder, LPCITEMIDLIST pidl, LPTSTR * lpszURL )
{
	(*lpszURL) = NULL;
IShellLink * pShellLink = NULL;
HRESULT hr = pFolder->GetUIObjectOf( NULL, 1, &pidl, IID_IShellLink, NULL, (LPVOID*)&pShellLink );
	if( SUCCEEDED( hr ) )
	{
		LPITEMIDLIST pidlChannel = NULL;
		hr = pShellLink->GetIDList( &pidlChannel );
		if( SUCCEEDED( hr ) )
		{
			IShellFolder * psfDesktop = NULL;
			hr = ::SHGetDesktopFolder( &psfDesktop );
			if( SUCCEEDED( hr ) )
			{
				STRRET strret;
				hr = psfDesktop->GetDisplayNameOf( pidlChannel, 0, &strret );
				if( SUCCEEDED( hr ) )
					StrRetToStr( strret, lpszURL, pidlChannel );
				psfDesktop->Release();
			}
		}
		pShellLink->Release();
	}
	return hr;
}

STDMETHODIMP CMainDlg::ResolveInternetShortcut( LPCTSTR lpszLinkFile, LPTSTR * lpszURL )
{
    (*lpszURL) = NULL;
IUniformResourceLocator * pUrlLink = NULL;
HRESULT hr = ::CoCreateInstance( CLSID_InternetShortcut, NULL, CLSCTX_INPROC_SERVER, IID_IUniformResourceLocator, (LPVOID*)&pUrlLink );
    if( SUCCEEDED( hr ) )
    {
        IPersistFile * pPersistFile = NULL;
        hr = pUrlLink->QueryInterface( IID_IPersistFile, (LPVOID*)&pPersistFile );
        if( SUCCEEDED( hr ) )
        {
            OLECHAR szOleCharLinkFile[ MAX_PATH ];
#ifndef _UNICODE
            ::MultiByteToWideChar( CP_ACP, 0, lpszLinkFile, -1, szOleCharLinkFile, sizeof(szOleCharLinkFile) );
#else
			__EXT_MFC_STRCPY( szOleCharLinkFile, MAX_PATH, lpszLinkFile );
#endif
            hr = pPersistFile->Load( szOleCharLinkFile, STGM_READ );
            if( SUCCEEDED( hr ) )
                hr = pUrlLink->GetURL( lpszURL );
            pPersistFile->Release();
        }
        pUrlLink->Release();
    }
    return hr;
} 

STDMETHODIMP CMainDlg::ResolveLink( LPCTSTR lpszLinkFile, LPTSTR * lpszURL )
{
    (*lpszURL) = NULL;
IShellLink * pShellLink = NULL;
HRESULT hr = ::CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pShellLink ); 
    if( SUCCEEDED( hr ) )
    {
        IPersistFile * pPersistFile = NULL;
        hr = pShellLink->QueryInterface( IID_IPersistFile, (LPVOID*)&pPersistFile );
        if( SUCCEEDED( hr ) )
        {
            OLECHAR szOleCharLinkFile[ MAX_PATH ];
#ifndef _UNICODE
            ::MultiByteToWideChar( CP_ACP, 0, lpszLinkFile, -1, szOleCharLinkFile, sizeof(szOleCharLinkFile) );
#else
			__EXT_MFC_STRCPY( szOleCharLinkFile, MAX_PATH, lpszLinkFile );
#endif
            hr = pPersistFile->Load( szOleCharLinkFile, STGM_READ );
            if( SUCCEEDED( hr ) )
            {
                WIN32_FIND_DATA wfd;      
                hr = pShellLink->Resolve( m_hWnd, SLR_NO_UI ); 
                if( NOERROR == hr )
                {
                    *lpszURL = (LPTSTR)m_pMalloc->Alloc( MAX_PATH );
                    hr = pShellLink->GetPath( *lpszURL, MAX_PATH - 1,  (WIN32_FIND_DATA*)&wfd, SLGP_UNCPRIORITY );
                }
            }
            pPersistFile->Release();
        }
        pShellLink->Release();
    }
    return hr;
}

void CMainDlg::_PrepareFavoritesList( IShellFolder * pFolder, UINT * pnMenuID, CExtPopupMenuWnd * pPopup, INT * pnPos )
{
	if(		pFolder == NULL 
		||	pnMenuID == NULL 
		||	pPopup == NULL 
		||	pnPos == NULL 
		)
		return;
IEnumIDList * pItems = NULL;
LPITEMIDLIST pidlNext = NULL;
STRRET strRetName;
STRRET strRetFile;
LPTSTR lpszName = NULL;
LPTSTR lpszFileName = NULL;
LPTSTR lpszURL = NULL;
HRESULT hr = pFolder->EnumObjects( GetSafeHwnd(), SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &pItems );
	while( NOERROR == hr )
	{
		hr = pItems->Next( 1, &pidlNext, NULL );
		if( NOERROR == hr )
		{
			bool bFolder = false;
			ULONG rgfInOut = 0; 
			pFolder->GetAttributesOf( 1, const_cast < LPCITEMIDLIST * > ( &pidlNext ), &rgfInOut );
			if( ( rgfInOut & SFGAO_FOLDER ) != 0 )
				bFolder = ( ( rgfInOut & SFGAO_FOLDER ) != 0 ) ? true : false;
			pFolder->GetDisplayNameOf( pidlNext, SHGDN_NORMAL, &strRetName );
			pFolder->GetDisplayNameOf( pidlNext, SHGDN_FORPARSING, &strRetFile );
			StrRetToStr( strRetName, &lpszName, pidlNext );
			StrRetToStr( strRetFile, &lpszFileName, pidlNext );
			CExtSafeString sName( lpszName );
			if( sName.GetLength() > 40 )
			{
				sName = sName.Left( 40 );
				sName += __EXT_MFC_SAFE_LPCTSTR( _T("...") );
			}
			HICON hIcon = NULL;
			HICON hIconLarge = NULL;
			IExtractIcon * pExtractIcon = NULL;
			if( pFolder->GetUIObjectOf( NULL, 1, const_cast<LPCITEMIDLIST *>(&pidlNext), IID_IExtractIcon, NULL, (void**)&pExtractIcon ) == S_OK ) 
			{
				TCHAR szBuf[MAX_PATH] = _T("");
				INT index = 0;
				UINT flags = 0;
				HRESULT hr = pExtractIcon->GetIconLocation( GIL_FORSHELL, szBuf, MAX_PATH, &index, &flags );
				if( hr == S_OK )
				{
					if( pExtractIcon->Extract( szBuf, index, &hIconLarge, &hIcon, (16 << 16) + 32 ) != S_OK ) 
					{
						hIcon = NULL;
						hIconLarge = NULL;
					}					
				}
				pExtractIcon->Release();
			}
			SHFILEINFO fileInfo = {0};
			::SHGetFileInfo( lpszFileName, NULL, &fileInfo, sizeof(fileInfo), SHGFI_TYPENAME );
			if( bFolder && _tcscmp( fileInfo.szTypeName, _T("Channel Shortcut") ) != 0 )
			{
				CExtPopupMenuWnd * pNewPopup = new CExtPopupMenuWnd();
				ASSERT( pNewPopup != NULL );
				VERIFY( pNewPopup->CreatePopupMenu( GetSafeHwnd() ) );
				pPopup->ItemInsertSpecPopup( pNewPopup, (*pnPos)++, sName, hIcon );
				IShellFolder * pSubFolder = NULL;
				HRESULT hrSub = pFolder->BindToObject( pidlNext, NULL, IID_IShellFolder, (LPVOID*)&pSubFolder );
				if( SUCCEEDED( hrSub ) )
				{		
					pFolder->GetDisplayNameOf( pidlNext, SHGDN_FORPARSING, &strRetName );
					LPTSTR lpszFolderName = NULL;
					StrRetToStr( strRetName, &lpszFolderName, pidlNext );
					pNewPopup->ItemInsertCommand( ID_FAVORITES_SHORTCUT_LIST, -1, lpszFolderName );
					if( lpszFolderName  != NULL )
						m_pMalloc->Free( lpszFolderName );
					pSubFolder->Release();
				}
			}
			else
			{
				UINT nMenuID = (*pnMenuID)++;
				pPopup->ItemInsertCommand( nMenuID, -1, sName, NULL, hIcon, false, 0, GetSafeHwnd() );
				CString sChannelShortcut, sInternetShortcut;
				CExtRegistry reg;
				if( reg.Open( HKEY_CLASSES_ROOT, _T("ChannelShortcut"), KEY_READ ) )
					reg.LoadString( _T(""), sChannelShortcut.GetBuffer( 100 ), 100 );
				if( reg.Open( HKEY_CLASSES_ROOT, _T("InternetShortcut"), KEY_READ ) )
					reg.LoadString( _T(""), sInternetShortcut.GetBuffer(100), 100 );
				if( 0 == sChannelShortcut.CompareNoCase( fileInfo.szTypeName ) ) // HKEY_CLASSES_ROOT\ChannelShortcut {f3aa0dc0-9cc8-11d0-a599-00c04fd64434}
					ResolveChannel( pFolder, pidlNext, &lpszURL );
				else if( 0 == sInternetShortcut.CompareNoCase( fileInfo.szTypeName ) ) // HKEY_CLASSES_ROOT\InternetShortcut {FBF23B40-E3F0-101B-8488-00AA003E56F8}
					ResolveInternetShortcut(lpszFileName, &lpszURL);
				else
					ResolveLink( lpszFileName, &lpszURL );
				LPTSTR lpszURL_Final = lpszURL != NULL ? lpszURL : lpszFileName;
				if( lpszURL_Final != NULL )
				{
					CString sURL;
					if( m_mapURLs.Lookup( nMenuID, sURL ) )
						ASSERT( 0 == sURL.CompareNoCase(lpszURL_Final) );
					else
						m_mapURLs.SetAt( nMenuID, lpszURL_Final );
					if( lpszURL != NULL )
						m_pMalloc->Free( lpszURL );
				}
			}
			if( hIcon != NULL )
			{
				::DestroyIcon( hIcon );
				hIcon = NULL;
			}
			if( hIconLarge != NULL )
			{
				::DestroyIcon( hIconLarge );
				hIconLarge = NULL;
			}
			if( lpszName != NULL )
				m_pMalloc->Free( lpszName );
			if( lpszFileName  != NULL )
				m_pMalloc->Free( lpszFileName );
			if( pidlNext != NULL )
				m_pMalloc->Free( pidlNext );
		}
	}
	if( pItems != NULL )
		pItems->Release();
}

STDMETHODIMP CMainDlg::IDLFromPath( CExtSafeString strPath, LPITEMIDLIST * ppidl )
{
LPMALLOC pMalloc = NULL;
IShellFolder * psfDeskTop  = NULL;
    if( FAILED( ::SHGetMalloc( &pMalloc ) ) )
        return S_FALSE;
    if( FAILED( ::SHGetDesktopFolder( &psfDeskTop ) ) )
    {
        if( pMalloc != NULL )
            pMalloc->Release();
        return S_FALSE;
    }
OLECHAR szOleCharPath[ MAX_PATH ];
#ifndef _UNICODE
    ::MultiByteToWideChar( CP_ACP, 0, strPath, -1, szOleCharPath, sizeof(szOleCharPath) );
#else
	__EXT_MFC_STRCPY( szOleCharPath, MAX_PATH, strPath );
#endif
HRESULT hRes = psfDeskTop->ParseDisplayName( NULL, NULL, szOleCharPath, NULL, ppidl, NULL );
    if( pMalloc != NULL )
        pMalloc->Release();
    if( psfDeskTop != NULL ) 
        psfDeskTop->Release();
    return hRes;
}

LRESULT CMainDlg::OnExtMenuPrepareLevel(WPARAM wParam, LPARAM lParam)
{
	lParam;
CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
		reinterpret_cast
		< CExtPopupMenuWnd::MsgPrepareMenuData_t * >
		( wParam );
	ASSERT( pData != NULL );
CExtPopupMenuWnd * pPopup = pData->m_pPopup;
	ASSERT( pPopup != NULL );
CExtPopupMenuWnd * pPopupParent = pPopup->GetParentMenuWnd();
INT nItemPos = pPopup->ItemFindPosForCmdID( ID_FAVORITES_SHORTCUT_LIST );
	if( nItemPos >= 0 )
	{
		CExtPopupMenuWnd::MENUITEMDATA & mi = pPopup->ItemGetInfo( nItemPos );
		LPITEMIDLIST pidlRootFolder = NULL;
		HRESULT hRes = S_OK;
		if( pPopupParent == NULL )
			hRes = ::SHGetSpecialFolderLocation( NULL, CSIDL_FAVORITES, &pidlRootFolder );
		else
			hRes = IDLFromPath( mi.GetText(), &pidlRootFolder );
		pPopup->ItemRemove( nItemPos );
		if( pPopupParent == NULL )
			m_mapURLs.RemoveAll();
		if( SUCCEEDED( hRes ) )
		{
			IShellFolder * psfDesktop = NULL;
			IShellFolder * psfRootFolder = NULL;
			hRes = ::SHGetDesktopFolder( &psfDesktop );
			if( SUCCEEDED( hRes ) )
			{
				hRes = psfDesktop->BindToObject( pidlRootFolder, NULL, IID_IShellFolder, (LPVOID*)&psfRootFolder );
				psfDesktop->Release();
				if( SUCCEEDED( hRes ) )
				{
					UINT uMenuID = UINT( ID_FAVORITES_FIRSTURL + m_mapURLs.GetCount() );
					_PrepareFavoritesList( psfRootFolder, &uMenuID, pPopup, &nItemPos );
					if( pPopup->ItemGetCount() == 0 )
					{
						pPopup->ItemInsertCommand( ID_FAVORITES_LASTURL + 1, 0, _T("(Empty)") );
						CExtPopupMenuWnd::MENUITEMDATA & mi = pPopup->ItemGetInfo( 0 );
						mi.Enable( false );
					}
				}
			}
			ASSERT( m_pMalloc );
			m_pMalloc->Free( pidlRootFolder );
			psfRootFolder->Release();
		}
	}
	return 1;
}

void CMainDlg::OnFavoritesAdd() 
{
HMODULE hModule = (HMODULE)::LoadLibrary(_T("shdocvw.dll"));
    if( hModule )
    {
		LPFNADDFAV lpfnDoAddToFavDlg = (LPFNADDFAV)GetProcAddress( hModule, "DoAddToFavDlg" );
		if( lpfnDoAddToFavDlg )
		{
			TCHAR szPath[MAX_PATH] = _T("");
			LPITEMIDLIST pidlFavorites = NULL;
			LPMALLOC pMalloc = NULL;
			::SHGetMalloc( &pMalloc );
			if(		::SHGetSpecialFolderPath( NULL, szPath, CSIDL_FAVORITES, TRUE ) 
				&&	SUCCEEDED( ::SHGetSpecialFolderLocation( NULL, CSIDL_FAVORITES, &pidlFavorites ) )
				)
			{
				CString sTitle = _T("Professional GUI tools and solutions for MFC & .NET desktop applications");
				CString sURL = _T("http://www.prof-uis.com/");
				BOOL bOK = 
					lpfnDoAddToFavDlg(
						GetSafeHwnd(), 
						sURL.GetBuffer( sURL.GetLength() ),
						sURL.GetLength(),
						sTitle.GetBuffer( sTitle.GetLength() ),
						sTitle.GetLength(),
						pidlFavorites
						);
				if( bOK )
				{
					// shortcut is added
				}
			}
			if( pidlFavorites )
				pMalloc->Free( pidlFavorites );
			pMalloc->Release();
		}
		::FreeLibrary( hModule );
    }
}

void CMainDlg::OnFavoritesOrganize() 
{
HMODULE hModule = ::LoadLibrary(_T("shdocvw.dll"));
    if( hModule )
    {
		LPFNORGFAV lpfnOrgFav = (LPFNORGFAV)::GetProcAddress( hModule, "DoOrganizeFavDlg" );
		if( lpfnOrgFav )
		{
			TCHAR wszPathName[_MAX_PATH] = _T("");
			HRESULT hResult = ::SHGetSpecialFolderPath( GetSafeHwnd(), wszPathName, CSIDL_FAVORITES, TRUE );
			if( SUCCEEDED( hResult ) )
			{
				if( lpfnOrgFav != NULL)
					lpfnOrgFav( GetSafeHwnd(), wszPathName );
			}
		}
		::FreeLibrary( hModule );
	}
}

void CMainDlg::OnFavoritesURL( UINT nID )
{
	if( m_mapURLs.GetCount() > 0 )
	{
		COleVariant vtEmpty;
		CString sURL;
		if( m_mapURLs.Lookup( nID, sURL ) )
		{
			// AfxMessageBox( sURL );
			// m_wndBrowser.NavigateURL( sURL );
		}
		else
			ASSERT( FALSE );
	}
}
