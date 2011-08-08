// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MthOutput.h"

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

	ON_COMMAND_EX(IDR_TOOLBAR1, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR1, OnUpdateControlBarMenu)

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
	
	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_5, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_5, OnUpdateControlBarMenu)

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
			RUNTIME_CLASS(CExtPaintManagerNativeXP)
			);

	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;

	CExtPopupMenuWnd::g_bMenuExpanding = false;
	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;
	//CExtPopupMenuWnd::g_bMenuWithShadows = false;
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
	if( ! m_wndToolBarUiLook.Create(
			_T("ToolBar2 "),
			this,
			ID_VIEW_UI_LOOK_BAR
			)
		||
		! m_wndToolBarUiLook.ThemeSwitcherInit()
		)
	{
		TRACE0("Failed to create toolbar2\n");
		return -1;      // fail to create
	}

	m_DemoThread0.m_wndDemoEventSource.m_pWndLogCtrl = &m_wndLogCtrl0;
	m_DemoThread1.m_wndDemoEventSource.m_pWndLogCtrl = &m_wndLogCtrl1;
	m_DemoThread2.m_wndDemoEventSource.m_pWndLogCtrl = &m_wndLogCtrl2;
	m_DemoThread3.m_wndDemoEventSource.m_pWndLogCtrl = &m_wndLogCtrl3;
	m_DemoThread4.m_wndDemoEventSource.m_pWndLogCtrl = &m_wndLogCtrl4;
	m_DemoThread5.m_wndDemoEventSource.m_pWndLogCtrl = &m_wndLogCtrl5;
	if(		!m_wndResizableBar0.Create(
				_T("Resizable Bar 0"),
				this,
				ID_VIEW_RESIZABLE_BAR_0
				)
					||	!m_wndLogCtrl0.Create(
							&m_wndResizableBar0
							)
					||	!m_DemoThread0.CreateThread(
							&m_wndLogCtrl0
							)
		||	!m_wndResizableBar1.Create(
				_T("Resizable Bar 1"),
				this,
				ID_VIEW_RESIZABLE_BAR_1
				)
					||	!m_wndLogCtrl1.Create(
							&m_wndResizableBar1
							)
					||	!m_DemoThread1.CreateThread(
							&m_wndLogCtrl1
							)
		||	!m_wndResizableBar2.Create(
				_T("Resizable Bar 2"),
				this,
				ID_VIEW_RESIZABLE_BAR_2
				)
					||	!m_wndLogCtrl2.Create(
							&m_wndResizableBar2
							)
					||	!m_DemoThread2.CreateThread(
							&m_wndLogCtrl2
							)
		||	!m_wndResizableBar3.Create(
				_T("Resizable Bar 3"),
				this,
				ID_VIEW_RESIZABLE_BAR_3
				)
					||	!m_wndLogCtrl3.Create(
							&m_wndResizableBar3
							)
					||	!m_DemoThread3.CreateThread(
							&m_wndLogCtrl3
							)
		||	!m_wndResizableBar4.Create(
				_T("Resizable Bar 4"),
				this,
				ID_VIEW_RESIZABLE_BAR_4
				)
					||	!m_wndLogCtrl4.Create(
							&m_wndResizableBar4
							)
					||	!m_DemoThread4.CreateThread(
							&m_wndLogCtrl4
							)
		||	!m_wndResizableBar5.Create(
				_T("Resizable Bar 5"),
				this,
				ID_VIEW_RESIZABLE_BAR_5
				)
					||	!m_wndLogCtrl5.Create(
							&m_wndResizableBar5
							)
					||	!m_DemoThread5.CreateThread(
							&m_wndLogCtrl5
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

    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarUiLook.EnableDocking(CBRS_ALIGN_ANY);
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
	
	if(	!CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{
		CRect wrAlreadyDockedBar;
		DockControlBar(&m_wndMenuBar);
		DockControlBar(&m_wndToolBar);
		
		RecalcLayout();
		m_wndToolBar.GetWindowRect( &wrAlreadyDockedBar );
		wrAlreadyDockedBar.OffsetRect( 1, 0 );
		DockControlBar(&m_wndToolBar1,AFX_IDW_DOCKBAR_TOP,&wrAlreadyDockedBar);

		DockControlBar(&m_wndToolBarUiLook,AFX_IDW_DOCKBAR_RIGHT);
		
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
		m_wndResizableBar2.FloatControlBar( CPoint(300,100) );
		m_wndResizableBar2.DockControlBar(&m_wndResizableBar3,false,true);
		m_wndResizableBar2.GetParentFrame()->RecalcLayout();
		m_wndResizableBar3.DockControlBar(&m_wndResizableBar4,false,true);
		m_wndResizableBar2.GetParentFrame()->RecalcLayout();
		m_wndResizableBar0.DockControlBar(&m_wndResizableBar5,false,true);
		RecalcLayout();
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CMDIFrameWnd >::PreCreateWindow( cs ) )
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
	CExtNCW < CMDIFrameWnd > :: Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndToolBarUiLook.PreTranslateMessage( pMsg ) )
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
	
	return CExtNCW < CMDIFrameWnd > :: DestroyWindow();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CExtNCW < CMDIFrameWnd > :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	CExtNCW < CMDIFrameWnd > :: ActivateFrame( nCmdShow );
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

	if( nID == ID_EDIT_COPY )
	{
		if(		m_wndLogCtrl0.GetSafeHwnd() != NULL
			&&	m_wndLogCtrl0.IsWindowVisible()
			&&	GetFocus()->GetSafeHwnd() == m_wndLogCtrl0.GetSafeHwnd()
			&&	m_wndLogCtrl0.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo)
			)
			return TRUE;
		if(		m_wndLogCtrl1.GetSafeHwnd() != NULL
			&&	m_wndLogCtrl1.IsWindowVisible()
			&&	GetFocus()->GetSafeHwnd() == m_wndLogCtrl1.GetSafeHwnd()
			&&	m_wndLogCtrl1.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo)
			)
			return TRUE;
		if(		m_wndLogCtrl2.GetSafeHwnd() != NULL
			&&	m_wndLogCtrl2.IsWindowVisible()
			&&	GetFocus()->GetSafeHwnd() == m_wndLogCtrl2.GetSafeHwnd()
			&&	m_wndLogCtrl2.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo)
			)
			return TRUE;
		if(		m_wndLogCtrl3.GetSafeHwnd() != NULL
			&&	m_wndLogCtrl3.IsWindowVisible()
			&&	GetFocus()->GetSafeHwnd() == m_wndLogCtrl3.GetSafeHwnd()
			&&	m_wndLogCtrl3.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo)
			)
			return TRUE;
		if(		m_wndLogCtrl4.GetSafeHwnd() != NULL
			&&	m_wndLogCtrl4.IsWindowVisible()
			&&	GetFocus()->GetSafeHwnd() == m_wndLogCtrl4.GetSafeHwnd()
			&&	m_wndLogCtrl4.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo)
			)
			return TRUE;
		if(		m_wndLogCtrl5.GetSafeHwnd() != NULL
			&&	m_wndLogCtrl5.IsWindowVisible()
			&&	GetFocus()->GetSafeHwnd() == m_wndLogCtrl5.GetSafeHwnd()
			&&	m_wndLogCtrl5.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo)
			)
			return TRUE;
	} // if( nID == ID_EDIT_COPY )
	
	return CExtNCW < CMDIFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}
