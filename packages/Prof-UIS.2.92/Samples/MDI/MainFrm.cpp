// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MDI.h"

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
	
	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_TA, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_TA, OnUpdateControlBarMenu)
	
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

//	m_bNcFrameImpl_IsDwmBased = false;
}

CMainFrame::~CMainFrame()
{
}

static UINT g_statBasicCommands[] =
{
	ID_APP_ABOUT,
	ID_APP_EXIT,
	ID_FILE_NEW,
	ID_VIEW_MENUBAR,
	ID_VIEW_TOOLBAR,
	ID_VIEW_UI_LOOK_BAR,
	ID_VIEW_RESIZABLE_BAR_0,
	ID_VIEW_RESIZABLE_BAR_1,
	ID_VIEW_RESIZABLE_BAR_2,
	ID_VIEW_RESIZABLE_BAR_3,
	ID_VIEW_RESIZABLE_BAR_4,
	ID_VIEW_RESIZABLE_BAR_TA,
	ID_VIEW_STATUS_BAR,
	ID_EDIT_COPY,
	ID_EDIT_CUT,
	ID_EDIT_PASTE,
	ID_WINDOW_CASCADE,
	ID_WINDOW_TILE_HORZ,
	0, // end of list
};

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
	if( (! m_wndMenuBar.Create(
			NULL, // _T("Menu Bar"),
			this,
			ID_VIEW_MENUBAR
			) )
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
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
		return -1;      // fail to create
	}

	if(		(! m_wndToolBarUiLook.Create( NULL, this, ID_VIEW_UI_LOOK_BAR ) )
		||	(! m_wndToolBarUiLook.ThemeSwitcherInit() )
		)
	{
		TRACE0("Failed to create toolbar2\n");
		return -1;      // fail to create
	}

	if(	!m_wndResizableBar0.Create(
			_T("Resizable Bar 0"),
			this,
			ID_VIEW_RESIZABLE_BAR_0
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar0\n");
		return -1;		// fail to create
	}
	if( !m_wndInBarEdit.Create(
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL
				|ES_MULTILINE|ES_LEFT|ES_WANTRETURN,
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

	if(	!m_wndResizableBar1.Create(
			_T("Resizable Bar 1"),
			this,
			ID_VIEW_RESIZABLE_BAR_1
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar1\n");
		return -1;		// fail to create
	}
	if( !m_wndInBarListBox.Create(
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL
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

	if(	!m_wndResizableBar2.Create(
			_T("Resizable Bar 2"),
			this,
			ID_VIEW_RESIZABLE_BAR_2
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar2\n");
		return -1;		// fail to create
	}
	if( !m_wndInBarColorPicker.Create(
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

	if(	!m_wndResizableBar3.Create(
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

	if(	!m_wndResizableBar4.Create(
			_T("Resizable Bar 4"),
			this,
			ID_VIEW_RESIZABLE_BAR_4
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar4\n");
		return -1;		// fail to create
	}

	if(	!m_wndResizableBarTA.Create(
			_T("Task Area"),
			this,
			ID_VIEW_RESIZABLE_BAR_TA
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBarTA\n");
		return -1;		// fail to create
	}

	if(		(! m_wndStatusBar.Create(this) )
		||	(! m_wndStatusBar.SetIndicators(
				indicators,
				sizeof(indicators)/sizeof(UINT) ) )
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	for( INT iLbStrNo = 0; iLbStrNo < 50; iLbStrNo++ )
	{
		CString s;
		s.Format( _T("String %02d"), iLbStrNo );
		
		m_wndInBarListBox.AddString( LPCTSTR(s) );
		
		s += _T("\r\n");
		m_wndInBarEdit.SetSel( -1, -1 );
		m_wndInBarEdit.ReplaceSel( s );
	}
	m_wndInBarEdit.SetSel( 0, 0 );

    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarUiLook.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar0.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar2.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar3.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar4.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBarTA.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);

	if( ! CExtControlBar::FrameEnableDocking(this) )
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

	if(	! CExtControlBar::ProfileBarStateLoad(
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
		m_wndResizableBar0.DockControlBarInnerOuter( AFX_IDW_DOCKBAR_LEFT, true, this, true );
		m_wndResizableBar1.DockControlBarLTRB( 75, &m_wndResizableBar0, AFX_IDW_DOCKBAR_BOTTOM, true );
		m_wndResizableBar3.DockControlBarInnerOuter( AFX_IDW_DOCKBAR_BOTTOM, true, this, true );
		m_wndResizableBar2.DockControlBarLTRB( 30, &m_wndResizableBar3, AFX_IDW_DOCKBAR_LEFT, true );
		m_wndResizableBar4.DockControlBarInnerOuter( AFX_IDW_DOCKBAR_RIGHT, true, this, true );
		m_wndResizableBarTA.DockControlBarInnerOuter( AFX_IDW_DOCKBAR_RIGHT, true, this, true );
		RecalcLayout();
	}

	g_CmdManager->SerializeState(
		pApp->m_pszProfileName,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		false
		);

CRect rcWndTA;
	m_wndResizableBarTA.GetWindowRect( &rcWndTA );
CSize _sizeTA = rcWndTA.Size();
	m_wndResizableBarTA.SetInitDesiredSizeFloating( _sizeTA );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CMDIFrameWnd > :: PreCreateWindow( cs ) )
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

void CMainFrame::RecalcLayout(BOOL bNotify)
{
	CExtNCW < CMDIFrameWnd > :: RecalcLayout( bNotify );
}

LRESULT CMainFrame::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
LRESULT lResult = CExtNCW < CMDIFrameWnd > :: WindowProc( message, wParam, lParam );
	switch( message )
	{
	case WM_SIZE:
		if( wParam != SIZE_MINIMIZED )
			CExtControlBar::stat_CompressFrameLayout( this );
	break;
	}
	return lResult;
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
#if (!defined __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG)
	if( nCode == CN_COMMAND )
	{
		if( nID == ID_WINDOWS_LIST )
		{
			CMyMdiWindowsListDlg dlgMdiWindowsList( this );
			dlgMdiWindowsList.DoModal();
			return TRUE;
		}
	}
#endif // (!defined __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG)
	return CExtNCW < CMDIFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}


