// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ZoomScrollBar.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC( CMainFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP

	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_UI_LOOK_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_UI_LOOK_BAR, OnUpdateControlBarMenu)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_LOCATION,
	ID_ZOOM_VALUE_PANE,
	ID_ZOOM_SCROLL_BAR_PANE,
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

void CMainFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	CExtControlBar::DoFrameBarCheckUpdate( this, pCmdUI, true );
}

BOOL CMainFrame::OnBarCheck(UINT nID)
{
	return CExtControlBar::DoFrameBarCheckCmd( this, nID, true );
}

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

//	if( ! g_PaintManager.PaintManagerStateLoad(
//			pApp->m_pszRegistryKey,
//			pApp->m_pszProfileName,
//			pApp->m_pszProfileName
//			)
//		)
//	{
//		CExtPaintManagerSkin * pPM = new CExtPaintManagerSkin;
//		bool bLoaded = true;
//		if( ! pPM->m_Skin.SearchAndLoadSkinFile( _T("Aqua.Skin"), false ) )
//		{
//			if( ! pPM->m_Skin.SearchAndLoadSkinFile( _T("..\\..\\Skins\\Binary\\Aqua.Skin"), false ) )
//			{
//				bLoaded = false;
//				::AfxMessageBox( _T("Failed to load initial skin.") );
//				delete pPM;
//			}
//		}
//		if( bLoaded )
//			g_PaintManager.InstallPaintManager( pPM );
//	}

	CExtPopupMenuWnd::g_bMenuExpanding = false;
	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;
	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::RecalcLayout(BOOL bNotify)
{
	CExtNCW < CFrameWnd > :: RecalcLayout( bNotify );
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( CExtNCW < CFrameWnd > :: OnCreate( lpCreateStruct ) == -1 )
		return -1;

	m_wndStatusBar.m_bDrawPaneSeparatorsInsteadOfBorders = true;
	if(		(! m_wndStatusBar.Create( this ) )
		||	(! m_wndStatusBar.SetIndicators(
				indicators,
				sizeof(indicators)/sizeof(UINT)
				) )
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneWidth( 0, 10 );
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight( 25 );

	// create a view to occupy the client area of the frame
	if( ! m_wndView.Create(
			NULL,
			NULL,
			AFX_WS_DEFAULT_VIEW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
			CRect( 0, 0, 0, 0 ),
			this,
			AFX_IDW_PANE_FIRST,
			NULL
			)
		)
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
CString strTitleOld = GetTitle(), strTitleNew;
	strTitleNew.Format(
		_T("%s - [%d objects loaded]"),
		LPCTSTR( strTitleOld ),
		m_wndView.GetObjectCount()
		);
	SetTitle( LPCTSTR( strTitleNew ) );
	OnUpdateFrameTitle( FALSE );
	
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );

HICON hIcon = pApp->LoadIcon( IDR_MAINFRAME );
	ASSERT( hIcon != NULL );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
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

	if( ! m_wndMenuBar.Create(
			NULL,
			this,
			ID_VIEW_MENUBAR
			)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;
    }

	if(		(! m_wndToolBarUiLook.Create( NULL, this, ID_VIEW_UI_LOOK_BAR ) )
		||	(! m_wndToolBarUiLook.ThemeSwitcherInit() )
		)
	{
		TRACE0("Failed to create the m_wndToolBarUiLook toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBarUiLook.EnableDocking( CBRS_ALIGN_ANY );

	if( ! CExtControlBar::FrameEnableDocking( this ) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
	if( ! CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{
		DockControlBar( &m_wndToolBarUiLook, AFX_IDW_DOCKBAR_LEFT );
		RecalcLayout();
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CFrameWnd > :: PreCreateWindow( cs ) )
		return FALSE;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_LAYOUTRTL);
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW|CS_HREDRAW|CS_DBLCLKS );
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
	if( m_wndView.GetSafeHwnd() != NULL )
		m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if( m_wndView.GetSafeHwnd() != NULL )
	{
		if( m_wndView.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
			return TRUE;
	}
	// otherwise, do default handling
	return CExtNCW < CFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndToolBarUiLook.PreTranslateMessage( pMsg ) )
		return TRUE;
	if( m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	return CExtNCW < CFrameWnd > :: PreTranslateMessage(pMsg);
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

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CFrameWnd::OnGetMinMaxInfo( lpMMI );
	lpMMI->ptMinTrackSize.x = g_PaintManager->UiScalingDo( 700, CExtPaintManager::__EUIST_X );
	lpMMI->ptMinTrackSize.y = g_PaintManager->UiScalingDo( 300, CExtPaintManager::__EUIST_Y );
}


