// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ResizableChildSheet.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC( CMainFrame, CMDIFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CMDIFrameWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_UI_LOOK_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_UI_LOOK_BAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_RESIZABLEBAR_PS1, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLEBAR_PS1, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_RESIZABLEBAR_PS2, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLEBAR_PS2, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_RESIZABLEBAR_PS3, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLEBAR_PS3, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_RESIZABLEBAR_PS4, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLEBAR_PS4, OnUpdateControlBarMenu)

	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPrepareMenu,
		OnExtMenuPrepare
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
{
	CExtControlBar::g_eResizablePanelDockingType =
		CExtControlBar::__RESIZABLE_DOCKING_TYPE_STUDIO_2005;

	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.showCmd = SW_HIDE;

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
}

CMainFrame::~CMainFrame()
{
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
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
	pApp;

	if( CExtNCW < CMDIFrameWnd > :: OnCreate(lpCreateStruct) == -1 )
	{
		ASSERT( FALSE );
		return -1;
	}

	VERIFY(
		g_CmdManager->ProfileWndAdd(
			__PROF_UIS_PROJECT_CMD_PROFILE_NAME,
			GetSafeHwnd()
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			__PROF_UIS_PROJECT_CMD_PROFILE_NAME,
			IDR_MAINFRAME
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			__PROF_UIS_PROJECT_CMD_PROFILE_NAME,
			IDR_MAINFRAME
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			__PROF_UIS_PROJECT_CMD_PROFILE_NAME,
			IDR_CHILDSTYPE
			)
		);
	
	m_wndMenuBar.SetMdiWindowPopupName( _T("Window") );

	if(	!m_wndMenuBar.Create( NULL, this, ID_VIEW_MENUBAR ) )
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }
	
	if(		(! m_wndToolBarUiLook.Create( NULL, this, ID_VIEW_UI_LOOK_BAR ) )
		||	(! m_wndToolBarUiLook.ThemeSwitcherInit() )
		)
	{
		TRACE0("Failed to create toolbar2\n");
		return -1;      // fail to create
	}

	if(		!m_wndToolBar.Create( NULL, this, AFX_IDW_TOOLBAR )
		||	!m_wndToolBar.LoadToolBar( IDR_MAINFRAME )
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if(		!m_wndStatusBar.Create(this)
		||	!m_wndStatusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT) )
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndResizableBarPS1.SetInitDesiredSizeVertical( CSize(150,200) );
	m_wndResizableBarPS1.SetInitDesiredSizeHorizontal( CSize(200,150) );
	if(	!m_wndResizableBarPS1.Create(
			NULL,
			this,
			ID_VIEW_RESIZABLEBAR_PS1
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBarPS1\n");
		return -1;		// fail to create
	}
	m_wndDockedPS1.AddPage( &m_wndPage1_PS1 );
	m_wndDockedPS1.AddPage( &m_wndPage2_PS1 );
	m_wndDockedPS1.AddPage( &m_wndPage3_PS1 );
	if( !m_wndDockedPS1.Create(&m_wndResizableBarPS1) )
	{
		TRACE0("Failed to create m_wndResizableBarPS1 window\n");
		return -1;
	}

	m_wndResizableBarPS2.SetInitDesiredSizeVertical( CSize(150,200) );
	m_wndResizableBarPS2.SetInitDesiredSizeHorizontal( CSize(200,150) );
	if(	!m_wndResizableBarPS2.Create(
			NULL,
			this,
			ID_VIEW_RESIZABLEBAR_PS2
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBarPS2\n");
		return -1;		// fail to create
	}
	m_wndDockedPS2.m_bInitTabSingleLine = true;
	m_wndDockedPS2.m_bInitTabButtons = true;
	m_wndDockedPS2.AddPage( &m_wndPage1_PS2 );
	m_wndDockedPS2.AddPage( &m_wndPage2_PS2 );
	m_wndDockedPS2.AddPage( &m_wndPage3_PS2 );
	if( !m_wndDockedPS2.Create(&m_wndResizableBarPS2) )
	{
		TRACE0("Failed to create m_wndResizableBarPS2 window\n");
		return -1;
	}

	m_wndResizableBarPS3.SetInitDesiredSizeVertical( CSize(150,200) );
	m_wndResizableBarPS3.SetInitDesiredSizeHorizontal( CSize(200,150) );
	if(	!m_wndResizableBarPS3.Create(
			NULL,
			this,
			ID_VIEW_RESIZABLEBAR_PS3
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBarPS3\n");
		return -1;		// fail to create
	}
	m_wndDockedPS3.m_bInitTabSingleLine = true;
	m_wndDockedPS3.m_bInitTabButtons = true;
	m_wndDockedPS3.m_bInitTabButtonsFlat = true;
	m_wndDockedPS3.AddPage( &m_wndPage1_PS3 );
	m_wndDockedPS3.AddPage( &m_wndPage2_PS3 );
	m_wndDockedPS3.AddPage( &m_wndPage3_PS3 );
	if( !m_wndDockedPS3.Create(&m_wndResizableBarPS3) )
	{
		TRACE0("Failed to create m_wndResizableBarPS3 window\n");
		return -1;
	}
	
	m_wndResizableBarPS4.SetInitDesiredSizeVertical( CSize(150,200) );
	m_wndResizableBarPS4.SetInitDesiredSizeHorizontal( CSize(200,150) );
	if(	!m_wndResizableBarPS4.Create(
			NULL,
			this,
			ID_VIEW_RESIZABLEBAR_PS4
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBarPS4\n");
		return -1;		// fail to create
	}
	m_wndDockedPS4.m_bInitTabSingleLine = true;
	m_wndDockedPS4.m_bInitTabButtons = true;
	m_wndDockedPS4.m_bInitTabButtonsFlat = true;
	m_wndDockedPS4.m_bInitTabSeparatorsFlat = true;
	m_wndDockedPS4.AddPage( &m_wndPage1_PS4 );
	m_wndDockedPS4.AddPage( &m_wndPage2_PS4 );
	m_wndDockedPS4.AddPage( &m_wndPage3_PS4 );
	if( !m_wndDockedPS4.Create(&m_wndResizableBarPS4) )
	{
		TRACE0("Failed to create m_wndResizableBarPS4 window\n");
		return -1;
	}

	m_wndMenuBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBarUiLook.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBarPS1.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBarPS2.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBarPS3.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBarPS4.EnableDocking( CBRS_ALIGN_ANY );

	// Enable control bars in the frame window to be redocable
	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
	if(	!CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{
		DockControlBar( &m_wndMenuBar );
		DockControlBar(&m_wndToolBarUiLook,AFX_IDW_DOCKBAR_RIGHT);
		DockControlBar( &m_wndToolBar );
		m_wndResizableBarPS1.DockControlBar( AFX_IDW_DOCKBAR_RIGHT,  1, this, false );
		m_wndResizableBarPS2.DockControlBar( AFX_IDW_DOCKBAR_BOTTOM, 2, this, false );
		m_wndResizableBarPS3.DockControlBar( AFX_IDW_DOCKBAR_RIGHT,  3, this, false );
		m_wndResizableBarPS4.DockControlBar( AFX_IDW_DOCKBAR_BOTTOM, 4, this, false );
	}

	RecalcLayout();

	return 0;
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
			pApp->m_pszProfileName,
			&m_dataFrameWP
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
	
	return CExtNCW < CMDIFrameWnd > :: DestroyWindow();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CExtNCW < CMDIFrameWnd > :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	CExtNCW < CMDIFrameWnd > :: ActivateFrame(nCmdShow);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CMDIFrameWnd > :: PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
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
	CExtNCW < CMDIFrameWnd > :: Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndToolBarUiLook.PreTranslateMessage( pMsg ) )
		return TRUE;
	if( m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	return CExtNCW < CMDIFrameWnd > :: PreTranslateMessage(pMsg);
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
	return CExtNCW < CMDIFrameWnd > :: OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

