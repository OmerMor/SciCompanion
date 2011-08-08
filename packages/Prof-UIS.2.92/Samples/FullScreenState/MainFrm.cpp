// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "FullScreenState.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// uncomment this to remove thin border if the main-frame in the fullscreen mode
// (please do not forget to remove the app state in the system registry folder
// HKEY_CURRENT_USER\Software\Foss\FullScreenState)
//#define __REMOVE_THIN_FULLSCREEN_BORDER

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC( CMainFrame, CMDIFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CMDIFrameWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_FULL_SCREEN, OnViewFullScreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULL_SCREEN, OnUpdateViewFullScreen)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP

	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR1, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR1, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR2, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR2, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR3, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR3, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_0, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_0, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_1, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_1, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_2, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_2, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_3, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_3, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_4, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_4, OnUpdateControlBarMenu)
	
	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_5, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_5, OnUpdateControlBarMenu)

	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPrepareMenu,
		OnExtMenuPrepare
	)

	ON_REGISTERED_MESSAGE(
		CExtControlBar::g_nMsgDrawTrackedArea,
		OnDrawControlBarTrackedArea
	)
	
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
	: m_rcTrackerEntireArea( 0, 0, 0, 0 )
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
	
	// window placement persistence
	::memset( &m_wpInitial, 0, sizeof(WINDOWPLACEMENT) );
	m_wpInitial.length = sizeof(WINDOWPLACEMENT);
	m_wpInitial.showCmd = SW_HIDE;

	CExtPopupMenuWnd::g_bMenuExpanding = false;
	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;

CBitmap _bmpPattern;
	if( _bmpPattern.LoadBitmap(IDB_BITMAP_TRACKER_PATTERN) )
	{
		VERIFY(
			m_brushTracker.CreatePatternBrush(
				&_bmpPattern
				)
			);
	} // if( _bmpPattern.LoadBitmap(IDB_BITMAP_TRACKER_PATTERN) )
#ifdef _DEBUG
	else
	{
		ASSERT( FALSE );
	} // else from if( _bmpPattern.LoadBitmap(IDB_BITMAP_TRACKER_PATTERN) )
#endif // _DEBUG
}

CMainFrame::~CMainFrame()
{
	if( m_bmpTrackerCache.GetSafeHandle() != NULL )
		m_bmpTrackerCache.DeleteObject();
}

void CMainFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
//	CFrameWnd::OnUpdateControlBarMenu( pCmdUI );
	CExtControlBar::DoFrameBarCheckUpdate(
		this,
		pCmdUI,
		false
		);
}

BOOL CMainFrame::OnBarCheck(UINT nID)
{
//	return CFrameWnd::OnBarCheck( nID );
	return
		CExtControlBar::DoFrameBarCheckCmd(
			this,
			nID,
			false
			);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( CExtNCW < CMDIFrameWnd > :: OnCreate( lpCreateStruct ) == -1 )
		return -1;
	
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
			IDR_MDITYPE
			)
		);

	m_wndMenuBar.SetMdiWindowPopupName( _T("Window") );
	if( !m_wndMenuBar.Create(
			NULL, // _T("Menu Bar"),
			this,
			ID_VIEW_MENUBAR
			)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }

	if( !m_wndToolBar.Create(
			_T("Standard"),
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

	if( !m_wndToolBar1.Create(
			_T("ToolBar 1"),
			this,
			IDR_TOOLBAR1
			)
		||
		!m_wndToolBar1.LoadToolBar(IDR_TOOLBAR1)
		)
	{
		TRACE0("Failed to create toolbar1\n");
		return -1;      // fail to create
	}
	if( !m_wndToolBar2.Create(
			NULL,
			this,
			IDR_TOOLBAR2
			)
		||
		!m_wndToolBar2.ThemeSwitcherInit()
		)
	{
		TRACE0("Failed to create toolbar2\n");
		return -1;      // fail to create
	}
	if( !m_wndToolBar3.Create(
			_T("ToolBar 3"),
			this,
			IDR_TOOLBAR3
			)
		||
		!m_wndToolBar3.LoadToolBar(IDR_TOOLBAR3)
		)
	{
		TRACE0("Failed to create toolbar3\n");
		return -1;      // fail to create
	}

	if(		!m_wndResizableBar0.Create(
				_T("Resizable Bar 0"),
				this,
				ID_VIEW_RESIZABLE_BAR_0
				)
		||	!m_wndResizableBar1.Create(
				_T("Resizable Bar 1"),
				this,
				ID_VIEW_RESIZABLE_BAR_1
				)
		||	!m_wndResizableBar2.Create(
				_T("Resizable Bar 2"),
				this,
				ID_VIEW_RESIZABLE_BAR_2
				)
		||	!m_wndResizableBar3.Create(
				_T("Resizable Bar 3"),
				this,
				ID_VIEW_RESIZABLE_BAR_3
				)
		||	!m_wndResizableBar4.Create(
				_T("Resizable Bar 4"),
				this,
				ID_VIEW_RESIZABLE_BAR_4
				)
		||	!m_wndResizableBar5.Create(
				_T("Resizable Bar 5"),
				this,
				ID_VIEW_RESIZABLE_BAR_5
				)
		)
	{
		TRACE0("Failed to create \"m_wndResizableBar\"s\n");
		return -1;		// fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	if( !m_wndMdiTabs.Create(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar3.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar0.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar3.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar4.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar5.EnableDocking(CBRS_ALIGN_ANY);

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
	
bool bInitialFullScreenMode =
		pApp->GetProfileInt(
			_T("Full-Screen-Settings"),
			_T("Full-Screen-Mode"),
			0
			)
		? true : false;

	ASSERT( ! CExtPopupMenuWnd::g_bFullScreenMode );
	if( bInitialFullScreenMode )
	{
		_DoViewFullScreen( false );
		ASSERT( CExtPopupMenuWnd::g_bFullScreenMode );
	} // if( bInitialFullScreenMode )
	else
	{
		VERIFY( _DoBarStateSerialization(bInitialFullScreenMode,false,true) );
	} // else from if( bInitialFullScreenMode )

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CExtNCW < CMDIFrameWnd > :: AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CExtNCW < CMDIFrameWnd > :: Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndToolBar2.PreTranslateMessage( pMsg ) )
		return TRUE;
	if( m_wndMenuBar.TranslateMainFrameMessage( pMsg ) )
		return TRUE;
	return CExtNCW < CMDIFrameWnd > :: PreTranslateMessage( pMsg );
}

BOOL CMainFrame::DestroyWindow() 
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

bool bFinalFullScreenMode =
		CExtPopupMenuWnd::g_bFullScreenMode ? true : false;
	VERIFY(
		pApp->WriteProfileInt(
			_T("Full-Screen-Settings"),
			_T("Full-Screen-Mode"),
			bFinalFullScreenMode ? 1 : 0
			)
		);
	VERIFY( _DoBarStateSerialization(bFinalFullScreenMode,true) );

	VERIFY(
		g_PaintManager.PaintManagerStateSave(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		);

	g_CmdManager->ProfileWndRemove( GetSafeHwnd() );
	
	return CExtNCW < CMDIFrameWnd > :: DestroyWindow();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
	if( m_wpInitial.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_wpInitial );
		CExtNCW < CMDIFrameWnd > :: ActivateFrame( m_wpInitial.showCmd );
		m_wpInitial.showCmd = SW_HIDE;
		return;
	}
	CExtNCW < CMDIFrameWnd > :: ActivateFrame( nCmdShow );
}


void CMainFrame::OnViewFullScreen() 
{
	_DoViewFullScreen( true );
}

void CMainFrame::_DoViewFullScreen( bool bSavePrevLayout )
{
	if( CExtPopupMenuWnd::g_bFullScreenMode )
	{
		CExtPopupMenuWnd::g_bFullScreenMode = false;
		if( bSavePrevLayout )
		{
			VERIFY( _DoBarStateSerialization(true,true) );
		}
		SetWindowPlacement( &m_wpSavedBeforeFullScreen );
		VERIFY( _DoBarStateSerialization(false,false) );
	} // if( CExtPopupMenuWnd::g_bFullScreenMode )
	else
	{
		m_wpSavedBeforeFullScreen.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement( &m_wpSavedBeforeFullScreen );

		CRect rcDesktop;
 		::GetWindowRect( ::GetDesktopWindow(), &rcDesktop );
// 		CExtPaintManager::monitor_parms_t _mp;
// 		CExtPaintManager::stat_GetMonitorParms( _mp, m_hWnd );
// 		rcDesktop = _mp.m_rcMonitor;

		CRect rcMdiClient;
		GetClientRect( &rcMdiClient );
		ClientToScreen( &rcMdiClient );
		CRect rcMdiFrame;
		GetWindowRect( &rcMdiFrame );
		rcMdiFrame.InflateRect(
			rcMdiClient.left-rcDesktop.left,
			rcMdiClient.top-rcDesktop.top,
			rcDesktop.right-rcMdiClient.right,
			rcDesktop.bottom-rcMdiClient.bottom
			);
		WINDOWPLACEMENT wpFullScreen;
		::memcpy( &wpFullScreen, &m_wpSavedBeforeFullScreen, sizeof(WINDOWPLACEMENT) );
		wpFullScreen.showCmd = SW_SHOWNORMAL;
#if (defined __REMOVE_THIN_FULLSCREEN_BORDER)
		CSize _sizeFrameBorder(
			::GetSystemMetrics(SM_CXFIXEDFRAME)-1,
			::GetSystemMetrics(SM_CYFIXEDFRAME)-1
			);
		rcMdiFrame.InflateRect( _sizeFrameBorder );
#endif // (defined __REMOVE_THIN_FULLSCREEN_BORDER)
		wpFullScreen.rcNormalPosition = rcMdiFrame;
		wpFullScreen.ptMaxPosition = rcMdiFrame.TopLeft();
		m_sizeFullScreenDesktopDifference =
			rcMdiFrame.Size() - rcDesktop.Size();
		CExtPopupMenuWnd::g_bFullScreenMode = true;
		if( bSavePrevLayout )
		{
			VERIFY( _DoBarStateSerialization(false,true) );
		}
		VERIFY( _DoBarStateSerialization(true,false) );
		SetWindowPlacement( &wpFullScreen );
 		//SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING|SWP_NOREDRAW );
 		//SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	} // else from if( CExtPopupMenuWnd::g_bFullScreenMode )
	RedrawWindow( NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN );
}

void CMainFrame::OnUpdateViewFullScreen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( CExtPopupMenuWnd::g_bFullScreenMode );
}

LRESULT CMainFrame::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_GETMINMAXINFO:
		if( CExtPopupMenuWnd::g_bFullScreenMode )
		{
			LPMINMAXINFO pMMI = (LPMINMAXINFO)lParam;
			ASSERT( pMMI != NULL );
			_DoFullScreenGetMinMaxInfo( pMMI );
			return 0;
		}
	break;
	}
LRESULT lResult = CExtNCW < CMDIFrameWnd > :: WindowProc( message, wParam, lParam );
	return lResult;
}

bool CMainFrame::_DoBarStateSerialization(
	bool bFullScreenState,
	bool bSaveState,
	bool bInitialLoad // = false
	)
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
CString sSpan = pApp->m_pszProfileName;
	sSpan += _T("\\");
	sSpan +=
		bFullScreenState
			? _T("BarPos-FullScreen")
			: _T("BarPos-Normal")
			;
	RecalcLayout();
bool bRetVal = false;
	if( bSaveState )
		bRetVal =
			CExtControlBar::ProfileBarStateSave(
				this,
				pApp->m_pszRegistryKey,
				pApp->m_pszProfileName,
				(LPCTSTR)sSpan
				);
	else
		bRetVal =
			CExtControlBar::ProfileBarStateLoad(
				this,
				pApp->m_pszRegistryKey,
				pApp->m_pszProfileName,
				(LPCTSTR)sSpan,
				&m_wpInitial
				);
	if( !bSaveState )
	{
		if( !bRetVal )
		{
			_DoInitialDock( bFullScreenState );
			bRetVal = true;
		} // if( !bRetVal )
		else if( (!bInitialLoad) && (!CExtPopupMenuWnd::g_bFullScreenMode) )
		{
			ActivateFrame( m_wpInitial.showCmd );
			RedrawWindow( NULL, NULL,
				RDW_INVALIDATE|RDW_UPDATENOW
					|RDW_ERASE|RDW_ERASENOW
					|RDW_FRAME|RDW_ALLCHILDREN
				);
		} // else if( (!bInitialLoad) && (!CExtPopupMenuWnd::g_bFullScreenMode) )
		if( !bInitialLoad )
			m_wpInitial.showCmd = SW_HIDE;
	} // if( !bSaveState )
	return bRetVal;
}

void CMainFrame::_DoInitialDock(
	bool bFullScreenState
	)
{
	if( bFullScreenState )
	{
		CRect wrAlreadyDockedBar;
		DockControlBar(&m_wndMenuBar);
		DockControlBar(&m_wndToolBar);
		DockControlBar(&m_wndToolBar1,AFX_IDW_DOCKBAR_LEFT);
		DockControlBar(&m_wndToolBar2,AFX_IDW_DOCKBAR_RIGHT);
		RecalcLayout();
		FloatControlBar(&m_wndToolBar3,CPoint(400,50));

		RecalcLayout();
		static CSize
			_sizeInitH( 250, 100 ),
			_sizeInitV( 100, 250 ),
			_sizeInitF( 250, 250 );
		m_wndResizableBar0.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar0.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar0.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar1.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar1.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar1.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar2.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar2.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar2.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar3.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar3.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar3.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar4.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar4.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar4.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar5.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar5.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar5.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar0.DockControlBar(AFX_IDW_DOCKBAR_RIGHT, 1,this);
		m_wndResizableBar1.DockControlBar(AFX_IDW_DOCKBAR_BOTTOM,2,this);
		m_wndResizableBar2.DockControlBar(AFX_IDW_DOCKBAR_RIGHT, 3,this);
		m_wndResizableBar3.DockControlBar(AFX_IDW_DOCKBAR_BOTTOM,4,this);
		m_wndResizableBar4.DockControlBar(AFX_IDW_DOCKBAR_RIGHT, 5,this);
		m_wndResizableBar5.DockControlBar(AFX_IDW_DOCKBAR_BOTTOM,6,this);

		ShowControlBar(&m_wndStatusBar,FALSE,TRUE);
		ShowControlBar(&m_wndMenuBar,FALSE,TRUE);
		ShowControlBar(&m_wndToolBar,FALSE,TRUE);
		ShowControlBar(&m_wndToolBar1,FALSE,TRUE);
		ShowControlBar(&m_wndToolBar2,FALSE,TRUE);
		ShowControlBar(&m_wndResizableBar0,FALSE,TRUE);
		ShowControlBar(&m_wndResizableBar1,FALSE,TRUE);
		ShowControlBar(&m_wndResizableBar2,FALSE,TRUE);
		ShowControlBar(&m_wndResizableBar3,FALSE,TRUE);
		ShowControlBar(&m_wndResizableBar4,FALSE,TRUE);
		ShowControlBar(&m_wndResizableBar5,FALSE,TRUE);
		RecalcLayout();
	} // if( bFullScreenState )
	else
	{
		CRect wrAlreadyDockedBar;
		DockControlBar(&m_wndMenuBar);
		DockControlBar(&m_wndToolBar);
		
		RecalcLayout();
		m_wndToolBar.GetWindowRect( &wrAlreadyDockedBar );
		wrAlreadyDockedBar.OffsetRect( 1, 0 );
		DockControlBar(&m_wndToolBar1,AFX_IDW_DOCKBAR_TOP,&wrAlreadyDockedBar);

		DockControlBar(&m_wndToolBar2,AFX_IDW_DOCKBAR_RIGHT);
		
		RecalcLayout();
		m_wndToolBar1.GetWindowRect( &wrAlreadyDockedBar );
		wrAlreadyDockedBar.OffsetRect( 1, 0 );
		DockControlBar(&m_wndToolBar3,AFX_IDW_DOCKBAR_TOP,&wrAlreadyDockedBar);

		RecalcLayout();
		static CSize
			_sizeInitH( 250, 100 ),
			_sizeInitV( 100, 250 ),
			_sizeInitF( 250, 250 );
		m_wndResizableBar0.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar0.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar0.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar1.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar1.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar1.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar2.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar2.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar2.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar3.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar3.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar3.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar4.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar4.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar4.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar5.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar5.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar5.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar0.DockControlBar(AFX_IDW_DOCKBAR_LEFT,1,this,false);
		m_wndResizableBar0.DockControlBar(&m_wndResizableBar1,true,true);
		m_wndResizableBar2.DockControlBar(AFX_IDW_DOCKBAR_RIGHT,1,this,false);
		m_wndResizableBar2.DockControlBar(&m_wndResizableBar3,false,true);
		m_wndResizableBar2.GetParentFrame()->RecalcLayout();
		m_wndResizableBar3.DockControlBar(&m_wndResizableBar4,false,true);
		m_wndResizableBar2.GetParentFrame()->RecalcLayout();
		m_wndResizableBar0.DockControlBar(&m_wndResizableBar5,false,true);
		RecalcLayout();
	} // else from if( bFullScreenState )
}

void CMainFrame::_DoFullScreenGetMinMaxInfo( LPMINMAXINFO lpMMI )
{
	ASSERT( CExtPopupMenuWnd::g_bFullScreenMode );

CRect rcDesktop;
	::GetWindowRect( ::GetDesktopWindow(), &rcDesktop );
// 	CExtPaintManager::monitor_parms_t _mp;
// 	CExtPaintManager::stat_GetMonitorParms( _mp, m_hWnd );
// 	rcDesktop = _mp.m_rcMonitor;

#if (defined __REMOVE_THIN_FULLSCREEN_BORDER)
CSize _sizeFrameBorder(
		::GetSystemMetrics(SM_CXFIXEDFRAME)-1,
		::GetSystemMetrics(SM_CYFIXEDFRAME)-1
		);
#endif // (defined __REMOVE_THIN_FULLSCREEN_BORDER)
CSize _size =
		rcDesktop.Size()
		+ m_sizeFullScreenDesktopDifference
#if (defined __REMOVE_THIN_FULLSCREEN_BORDER)
		+ _sizeFrameBorder + _sizeFrameBorder
#endif // (defined __REMOVE_THIN_FULLSCREEN_BORDER)
		;
	lpMMI->ptMaxSize.x = _size.cx;
	lpMMI->ptMaxSize.y = _size.cy;
	lpMMI->ptMaxTrackSize.x = _size.cx;
	lpMMI->ptMaxTrackSize.y = _size.cy;
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if( CExtPopupMenuWnd::g_bFullScreenMode )
	{
		switch( nID )
		{
		case SC_SIZE:
		case SC_MOVE:
		case SC_MAXIMIZE:
			{
				CMenu * pSysMenu =
					GetSystemMenu( FALSE );
				if( pSysMenu == NULL )
					return;
				CString strMenuText;
				pSysMenu->GetMenuString( nID, strMenuText, MF_BYCOMMAND );
				strMenuText.Replace( _T("&"), _T("") );
				CString strMsg;
				strMsg.Format(
					_T("The \"%s\" system command is not available in the Full-Screen mode."),
					strMenuText
					);
				::AfxMessageBox( strMsg, MB_OK|MB_ICONINFORMATION );
			}
			return;
		} // switch( nID )
	} // if( CExtPopupMenuWnd::g_bFullScreenMode )
	CExtNCW < CMDIFrameWnd > :: OnSysCommand( nID, lParam );
}

LRESULT CMainFrame::OnExtMenuPrepare(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;
	
	//////////////////////////////////////////////////////////////////////////
	// Add "Windows..." command
	//////////////////////////////////////////////////////////////////////////
	
	CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
		reinterpret_cast
		< CExtPopupMenuWnd::MsgPrepareMenuData_t * >
		( wParam );
	ASSERT( pData != NULL );
	CExtPopupMenuWnd * pPopup = pData->m_pPopup;
	ASSERT( pPopup != NULL );
	
	INT nItemPos;
	INT nNewPos = -1;
	
	nItemPos = pPopup->ItemFindPosForCmdID( __ID_MDIWND_DLGWINDOWS );
	if( nItemPos > 0 )
	{
		// "More Windows..." command found
		pPopup->ItemRemove( nItemPos );
		nNewPos = nItemPos;
	}
	else
	{
		int nMaxCmdID = 0;
		for( int nCmdID = __ID_MDIWNDLIST_FIRST; nCmdID <= __ID_MDIWNDLIST_LAST; nCmdID++ ){
			nItemPos = pPopup->ItemFindPosForCmdID( nCmdID );
			if( nItemPos > 0 ){
				if( nCmdID > nMaxCmdID ){
					nMaxCmdID = nCmdID;
					nNewPos = nItemPos;
				}
			}
		}
		if( nNewPos > 0 ){
			pPopup->ItemInsert(
				(UINT)CExtPopupMenuWnd::TYPE_SEPARATOR,
				++nNewPos
				);
			nNewPos++;
		}
	}
	if( nNewPos > 0 )
	{
		UINT nCmdID = ID_WINDOWS_LIST;
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( this->GetSafeHwnd() ),
			nCmdID
			);
		if( pCmdItem == NULL ){
			pCmdItem = 
				g_CmdManager->CmdAllocPtr( 
				g_CmdManager->ProfileNameFromWnd( this->GetSafeHwnd() ), 
				nCmdID 
				);
		}
		ASSERT( pCmdItem != NULL );
		if( pCmdItem != NULL )
		{
			CExtSafeString sMoreWindows(_T("Windows..."));
			CExtSafeString sManageWindows(_T("Manages the currently open windows"));
			pCmdItem->m_sMenuText = sMoreWindows;
			pCmdItem->m_sToolbarText = pCmdItem->m_sMenuText;
			pCmdItem->m_sTipTool = sManageWindows;
			pCmdItem->m_sTipStatus = pCmdItem->m_sTipTool;
			pCmdItem->StateSetBasic( true );
			
			pPopup->ItemInsert(
				nCmdID,
				nNewPos,
				sMoreWindows,
				NULL,
				m_hWnd
				);
		}
	}
	
	return 1;
}

LRESULT CMainFrame::OnDrawControlBarTrackedArea(WPARAM wParam, LPARAM lParam)
{
	lParam;
CExtControlBar::MsgDrawTrackedAreaData_t * pMsgDrawTrackedAreaData =
		(CExtControlBar::MsgDrawTrackedAreaData_t *)wParam;
	ASSERT( pMsgDrawTrackedAreaData != NULL );
	ASSERT_VALID( (&(pMsgDrawTrackedAreaData->m_wndBar)) );
	ASSERT( pMsgDrawTrackedAreaData->m_wndBar.GetSafeHwnd() != NULL );
	ASSERT( pMsgDrawTrackedAreaData->m_dcDraw.GetSafeHdc() != NULL );
	pMsgDrawTrackedAreaData->m_bMessageHandled = true;
	if( pMsgDrawTrackedAreaData->m_bErasingPrevious )
	{
		if( m_bmpTrackerCache.GetSafeHandle() != NULL )
		{
			CDC dcSrc;
			if( dcSrc.CreateCompatibleDC( &(pMsgDrawTrackedAreaData->m_dcDraw) ) )
			{
				CBitmap * pOldBitmap = dcSrc.SelectObject( &m_bmpTrackerCache );
				pMsgDrawTrackedAreaData->m_dcDraw.BitBlt(
					m_rcTrackerEntireArea.left,
					m_rcTrackerEntireArea.top,
					m_rcTrackerEntireArea.Width(),
					m_rcTrackerEntireArea.Height(),
					&dcSrc,
					0,
					0,
					SRCCOPY
					);
				dcSrc.SelectObject( pOldBitmap );
				dcSrc.DeleteDC();
			} // if( dcSrc.CreateCompatibleDC( &(pMsgDrawTrackedAreaData->m_dcDraw) ) )
			m_bmpTrackerCache.DeleteObject(); // no longer needed
		} // if( m_bmpTrackerCache.GetSafeHandle() != NULL )
	} // if( pMsgDrawTrackedAreaData->m_bErasingPrevious )
	else
	{
		m_rcTrackerEntireArea = pMsgDrawTrackedAreaData->m_rcDrawArea;
		if( m_bmpTrackerCache.GetSafeHandle() != NULL )
			m_bmpTrackerCache.DeleteObject();
		CDC dcDst;
		if( dcDst.CreateCompatibleDC( &(pMsgDrawTrackedAreaData->m_dcDraw) ) )
		{
			if( m_bmpTrackerCache.CreateCompatibleBitmap(
					&(pMsgDrawTrackedAreaData->m_dcDraw),
					m_rcTrackerEntireArea.Width(),
					m_rcTrackerEntireArea.Height()
					)
				)
			{
				CBitmap * pOldBitmap = dcDst.SelectObject( &m_bmpTrackerCache );
				dcDst.BitBlt(
					0,
					0,
					m_rcTrackerEntireArea.Width(),
					m_rcTrackerEntireArea.Height(),
					&(pMsgDrawTrackedAreaData->m_dcDraw),
					m_rcTrackerEntireArea.left,
					m_rcTrackerEntireArea.top,
					SRCCOPY
					);
				dcDst.SelectObject( pOldBitmap );
			} // if( m_bmpTrackerCache.CreateCompatibleBitmap( ...
			dcDst.DeleteDC();
		} // if( dcDst.CreateCompatibleDC( &(pMsgDrawTrackedAreaData->m_dcDraw) ) )
		HBRUSH hBrush = (HBRUSH)
			m_brushTracker.GetSafeHandle();
		if( hBrush == NULL )
			hBrush = (HBRUSH)
			::GetStockObject( WHITE_BRUSH );
		pMsgDrawTrackedAreaData->Draw(
			hBrush,
			PATCOPY
			);
	} // else from if( pMsgDrawTrackedAreaData->m_bErasingPrevious )
	return 0L;
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( nCode == CN_COMMAND )
	{
		if( nID == ID_WINDOWS_LIST )
		{
			CMyMdiWindowsListDlg dlgMdiWindowsList( this );
			dlgMdiWindowsList.DoModal();
			return TRUE;
		}
	}
	return CExtNCW < CMDIFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}