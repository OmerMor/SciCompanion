// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SDI.h"

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
	//}}AFX_MSG_MAP

	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_UI_LOOK_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_UI_LOOK_BAR, OnUpdateControlBarMenu)

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

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
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
	: m_wndInBarEdit( true, true )
	, m_wndInBarListBox( true, true )
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

static UINT g_statBasicCommands[] =
{
	ID_APP_ABOUT,
	ID_APP_EXIT,
	ID_VIEW_MENUBAR,
	ID_VIEW_TOOLBAR,
	ID_VIEW_UI_LOOK_BAR,
	ID_VIEW_RESIZABLE_BAR_0,
	ID_VIEW_RESIZABLE_BAR_1,
	ID_VIEW_RESIZABLE_BAR_2,
	ID_VIEW_RESIZABLE_BAR_3,
	ID_VIEW_RESIZABLE_BAR_4,
	ID_VIEW_STATUS_BAR,
	ID_EDIT_COPY,
	ID_EDIT_CUT,
	ID_EDIT_PASTE,
	0, // end of list
};

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

// Uncomment the following like for making all the tabbed bar containers based on
// multi-row tab control:
// CExtControlBar::g_dwTabContainerTabsStyle = __ETWS_MULTI_ROW_COLUMN; //|__ETWS_EQUAL_WIDTHS|__ETWS_FULL_WIDTH;


	if( CExtNCW < CFrameWnd > :: OnCreate( lpCreateStruct ) == -1 )
		return -1;

	// create a view to occupy the client area of the frame
	if( ! m_wndView.Create(
			NULL,
			NULL,
			AFX_WS_DEFAULT_VIEW,
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

	if(		(! m_wndToolBar.Create(
				_T("ToolBar"),
				this,
				AFX_IDW_TOOLBAR
				) )
		||	(! m_wndToolBar.LoadToolBar( IDR_MAINFRAME ) )
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	if(		(! m_wndToolBarUiLook.Create( NULL, this, ID_VIEW_UI_LOOK_BAR ) )
		||	(! m_wndToolBarUiLook.ThemeSwitcherInit() )
		)
	{
		TRACE0("Failed to create the m_wndToolBarUiLook toolbar\n");
		return -1;      // fail to create
	}

	if(	! m_wndResizableBar0.Create(
			_T("Resizable Bar 0"),
			this,
			ID_VIEW_RESIZABLE_BAR_0
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar0\n");
		return -1;		// fail to create
	}
	if( ! m_wndInBarEdit.Create(
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_MULTILINE|ES_LEFT,
			CRect(0,0,0,0),
			&m_wndResizableBar0,
			m_wndResizableBar0.GetDlgCtrlID()
			)
		)
	{
		TRACE0("Failed to create m_wndInBarEdit\n");
		return -1;		// fail to create
	}
	m_wndInBarEdit.SetFont(
		CFont::FromHandle(
			(HFONT)::GetStockObject(DEFAULT_GUI_FONT)
			)
		);

	if(	! m_wndResizableBar1.Create(
			_T("Resizable Bar 1"),
			this,
			ID_VIEW_RESIZABLE_BAR_1
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar1\n");
		return -1;		// fail to create
	}
	if( ! m_wndInBarListBox.Create(
			WS_CHILD|WS_VISIBLE|WS_VSCROLL
				|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT
				|LBS_EXTENDEDSEL|LBS_MULTIPLESEL
				,
			CRect(0,0,0,0),
			&m_wndResizableBar1,
			m_wndResizableBar1.GetDlgCtrlID()
			)
		)
	{
		TRACE0("Failed to create m_wndInBarListBox\n");
		return -1;		// fail to create
	}
	m_wndInBarListBox.SetFont(
		CFont::FromHandle(
			(HFONT)::GetStockObject(DEFAULT_GUI_FONT)
			)
		);
	for( INT iLbStrNo = 0; iLbStrNo < 50; iLbStrNo++ )
	{
		CString s;
		s.Format( _T("String %02d"), iLbStrNo );
		m_wndInBarListBox.AddString( LPCTSTR(s) );
	}

	if(	! m_wndResizableBar2.Create(
			_T("Resizable Bar 2"),
			this,
			ID_VIEW_RESIZABLE_BAR_2
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar2\n");
		return -1;		// fail to create
	}
	if( ! m_wndInBarColorPicker.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD|WS_VISIBLE,
			CRect(0,0,0,0),
			&m_wndResizableBar2,
			m_wndResizableBar2.GetDlgCtrlID()
			)
		)
	{
		TRACE0("Failed to create m_wndInBarColorPicker\n");
		return -1;		// fail to create
	}

	if(	! m_wndResizableBar3.Create(
			_T("Resizable Bar 3"),
			this,
			ID_VIEW_RESIZABLE_BAR_3
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar3\n");
		return -1;		// fail to create
	}
	if( !m_wndInBarDlg.Create(
			IDD_IN_BAR_DLG,
			&m_wndResizableBar3
			)
		)
	{
		TRACE0("Failed to create m_wndInBarDlg\n");
		return -1;		// fail to create
	}
	m_wndInBarDlg.ShowSizeGrip( FALSE );

	if(	! m_wndResizableBar4.Create(
			_T("Resizable Bar 4"),
			this,
			ID_VIEW_RESIZABLE_BAR_4
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar4\n");
		return -1;		// fail to create
	}

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

    m_wndMenuBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBarUiLook.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBar0.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBar1.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBar2.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBar3.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBar4.EnableDocking( CBRS_ALIGN_ANY );

	if( ! CExtControlBar::FrameEnableDocking( this ) )
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
	g_CmdManager->SerializeState(
		pApp->m_pszProfileName,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		false
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
		DockControlBar( &m_wndMenuBar );
		DockControlBar( &m_wndToolBar );
		DockControlBar( &m_wndToolBarUiLook, AFX_IDW_DOCKBAR_RIGHT );
		m_wndResizableBar0.DockControlBar( AFX_IDW_DOCKBAR_RIGHT, 1, this, false );
		m_wndResizableBar1.DockControlBar( AFX_IDW_DOCKBAR_LEFT, 1, this, false );
		m_wndResizableBar2.DockControlBar( AFX_IDW_DOCKBAR_TOP, 2, this, false );
		m_wndResizableBar2.DockControlBar( &m_wndResizableBar3, true, false, this, true );
		m_wndResizableBar4.DockControlBar( AFX_IDW_DOCKBAR_BOTTOM, 2, this, false );
		RecalcLayout();
	}

//CMenu * pSysMenu = GetSystemMenu( FALSE );
//	if( pSysMenu != NULL )
//		pSysMenu->EnableMenuItem(
//			SC_CLOSE,
//			MF_DISABLED|MF_GRAYED|MF_BYCOMMAND
//			);

//	m_wndMenuBar.FlashCaption( 65535, 250, RGB(255,255,0), RGB(0,128,0) );
//	m_wndToolBar.FlashCaption( 65535, 250, RGB(255,255,0), RGB(0,128,0) );
//	m_wndResizableBar0.FlashCaption( 65535, 375 );
//	m_wndResizableBar1.FlashCaption( 65535, 500, RGB(224,224,224), RGB(132,0,132) );
//	m_wndResizableBar2.FlashCaption( 65535, 250, RGB(255,255,0), RGB(0,128,0) );
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
	if(		::GetFocus() == m_wndInBarEdit.GetSafeHwnd()
		&&	m_wndInBarEdit.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo )
		)
		return TRUE;
	if( m_wndView.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
		return TRUE;
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

