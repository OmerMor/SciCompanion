// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MDI_DynamicBars.h"

#include "MainFrm.h"

#if (!defined __EXT_MFC_NO_SHELL)
	#if (!defined __EXT_CONTROLS_SHELL_H)
		#include <ExtControlsShell.h>
	#endif
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define __DEMO_BARS_COUNT_ALL 20 // 40
#define __DEMO_BARS_COUNT_INITIALLY_DOCKED 14 // 28

IMPLEMENT_SERIAL( CSimpleControlBar, CExtDynamicControlBar, VERSIONABLE_SCHEMA|1 );

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC( CMainFrame, CMDIFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CMDIFrameWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_PERSISTENCE_LOAD_FROM_FILE_RESIZABLE, OnPersistenceLoadFromFile_Resizable)
	ON_COMMAND(ID_PERSISTENCE_SAVE_TO_FILE_RESIZABLE, OnPersistenceSaveToFile_Resizable)
	ON_COMMAND(ID_PERSISTENCE_LOAD_FROM_FILE_FIXEDSIZED, OnPersistenceLoadFromFile_Fixedsized)
	ON_COMMAND(ID_PERSISTENCE_SAVE_TO_FILE_FIXEDSIZED, OnPersistenceSaveToFile_Fixedsized)
	//}}AFX_MSG_MAP

	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_UI_LOOK_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_UI_LOOK_BAR, OnUpdateControlBarMenu)

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
	: m_pBarPersistent( NULL )
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
			RUNTIME_CLASS(CExtPaintManagerStudio2005)
			);

//	CExtPopupMenuWnd::g_bMenuExpanding = false;
//	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;

	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.showCmd = SW_HIDE;
}

CMainFrame::~CMainFrame()
{
}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
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
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	if( CExtNCW < CMDIFrameWnd > :: OnCreate( lpCreateStruct ) == -1 )
	{
		ASSERT( FALSE );
		return -1;
	}

	VERIFY(
		g_CmdManager->ProfileWndAdd(
			pApp->m_pszProfileName,
			GetSafeHwnd()
			)
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
			IDR_MDIDOCTYPE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_DYNAMIC_BAR
			)
		);
	
	m_wndMenuBar.SetMdiWindowPopupName( _T("Window") );
	if(	! m_wndMenuBar.Create(
			NULL,
			this,
			ID_VIEW_MENUBAR
			)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }
	
	if(		(! m_wndToolBar.Create( NULL, this, AFX_IDW_TOOLBAR ) )
		||	(! m_wndToolBar.LoadToolBar( IDR_MAINFRAME ) )
		)
	{
		TRACE0("Failed to create m_wndToolBar toolbar\n");
		return -1;      // fail to create
	}

	if(		(! m_wndToolBarUiLook.Create( NULL, this, ID_VIEW_UI_LOOK_BAR ) )
		||	(! m_wndToolBarUiLook.ThemeSwitcherInit() )
		)
	{
		TRACE0("Failed to create m_wndToolBarUiLook toolbar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.m_bHideTextOnDisabledPanes = false;
	m_wndStatusBar.m_bDrawPaneSeparatorsInsteadOfBorders = true;
	if(		(! m_wndStatusBar.Create(this) )
		||	(! m_wndStatusBar.SetIndicators(
				indicators,
				sizeof(indicators) / sizeof(UINT)
				) )
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndMenuBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBarUiLook.EnableDocking( CBRS_ALIGN_ANY );

	if( ! CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( ! CExtControlBar::FrameInjectAutoHideAreas(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	
static UINT statBasicCommands[] =
{
	ID_FILE_NEW, 
	ID_FILE_OPEN, 
	ID_FILE_SAVE,
	ID_APP_EXIT,
	ID_APP_ABOUT,
	ID_FILE_PRINT,
	ID_FILE_PRINT_SETUP, 
	ID_EDIT_COPY, 
	ID_EDIT_CUT, 
	ID_EDIT_PASTE, 
	ID_EDIT_UNDO, 
	ID_VIEW_TOOLBAR, 
	ID_WINDOW_TILE_HORZ,
	ID_VIEW_UI_LOOK_BAR,
	ID_PERSISTENCE_LOAD_FROM_FILE_RESIZABLE,
	ID_PERSISTENCE_SAVE_TO_FILE_RESIZABLE,
	0 // end of commands list
}; // statBasicCommands array
	VERIFY(
		g_CmdManager->SetBasicCommands(
			pApp->m_pszProfileName,
			statBasicCommands
			)
		);

#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	if( ! m_wndMdiTabs.Create(
			this,
			CRect(0,0,0,0),
			UINT(__EXT_MFC_IDC_STATIC),
			WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
			__ETWS_MDI_DEFAULT|__ETWS_HIDE_ICONS|__ETWS_ITEM_DRAGGING
			)
		)
	{
		ASSERT( FALSE );
		return -1;
	}
//	m_wndMdiTabs.ModifyTabWndStyleEx( 0, __ETWS_EX_CLOSE_ON_TABS, false );
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

	// install dynamic bar site for this frame window
	CExtDynamicBarSite::m_nBarListInMenusCmdID = ID_DYNAMIC_BARS_LIST;
	CExtDynamicBarSite::Install( this );

	// allocate and initialize one persistent bar
HICON hIcon = (HICON)
		::LoadImage(
			::AfxGetInstanceHandle(),
			MAKEINTRESOURCE(IDR_MAINFRAME),
			IMAGE_ICON,
			16,
			16,
			0
			);
	ASSERT( hIcon != NULL );
CExtCmdIcon icon;
	icon.AssignFromHICON( hIcon, false );
	m_pBarPersistent =
		CExtDynamicBarSite::BarAlloc(
			_T("Persistent Bar"),
			icon,
			0,
			RUNTIME_CLASS(CSimpleControlBar),
			true // this bar is persistent
			);
	ASSERT( m_pBarPersistent != NULL );
	m_pBarPersistent->m_nMdiMenuResourceID =
		IDR_DYNAMIC_BAR;
	m_pBarPersistent->m_nMdiAccelTableResourceID =
		IDR_MAINFRAME;
UINT nCmdID = (UINT)m_pBarPersistent->GetDlgCtrlID();
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			nCmdID
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->StateSetBasic();
CSimpleDynamicEdit * pEdit = new CSimpleDynamicEdit;
	pEdit->m_brush.CreateSolidBrush(
			( STATIC_DOWNCAST(
				CSimpleControlBar,
				m_pBarPersistent
				)
			) -> m_clrEditorBackground
		);
	VERIFY(
		pEdit->Create(
			m_pBarPersistent->m_hWnd,
			_T("\r\n")
			_T("This is persistent bar.\r\n")
			_T("\r\n")
			_T("Persistent bars are not re-created\r\n")
			_T("when the CExtDynamicBarSite::StateLoad()\r\n")
			_T("method is invoked.\r\n")
			_T("\r\n")
			)
		);
static const CSize _sizeDesiredDocked( 100, 100 );
static const CSize _sizeDesiredFloating( 200, 150 );
	m_pBarPersistent->SetInitDesiredSizeHorizontal( _sizeDesiredDocked );
	m_pBarPersistent->SetInitDesiredSizeVertical( _sizeDesiredDocked );
	m_pBarPersistent->SetInitDesiredSizeFloating( _sizeDesiredFloating );
	ShowControlBar( m_pBarPersistent, TRUE, TRUE );
	
	// allocate and initialize dynamic bars
	if( ! CExtDynamicBarSite::StateLoad(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		)
	{ // if all bars were not loaded from registry, then allocate them
		HICON hIcon = (HICON)
			::LoadImage(
				::AfxGetInstanceHandle(),
				MAKEINTRESOURCE(IDR_MAINFRAME),
				IMAGE_ICON,
				16,
				16,
				0
				);
		ASSERT( hIcon != NULL );
		CExtCmdIcon icon;
		icon.AssignFromHICON( hIcon, false );
		int i;
		for( i = 0; i < __DEMO_BARS_COUNT_ALL; i++ )
		{
			CExtSafeString strBarCaption;
			strBarCaption.Format(
				_T("Dynamic Bar %d"),
				i
				);
			m_arrAllDynamicBars.Add(
				CExtDynamicBarSite::BarAlloc(
					strBarCaption,
					icon,
					0,
					RUNTIME_CLASS(CSimpleControlBar)
					)
				);
			ASSERT( m_arrAllDynamicBars[i] != NULL );
			m_arrAllDynamicBars[i]->m_nMdiMenuResourceID =
				IDR_DYNAMIC_BAR;
			m_arrAllDynamicBars[i]->m_nMdiAccelTableResourceID =
				IDR_MAINFRAME;
		} // for( i = 0; i < __DEMO_BARS_COUNT_ALL; i++ )
	} // if all bars were not loaded from registry, then allocate them
	else
	{ // else just get them
		CExtDynamicBarSite::BarEnumGetArray( m_arrAllDynamicBars, false, true );
	} // else just get them
int i;
	for( i = 0; i < m_arrAllDynamicBars.GetSize(); i++ )
	{ // create edit controls inside all dynamic bars
		UINT nCmdID = (UINT)m_arrAllDynamicBars[i]->GetDlgCtrlID();
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
				pApp->m_pszProfileName,
				nCmdID
				);
		ASSERT( pCmdItem != NULL );
		pCmdItem->StateSetBasic();
		CString strEditorText;
		strEditorText.Format(
			_T("\r\n")
			_T("Edit control inside dynamic resizable control bar \"%s\"\r\n")
			_T("\r\n")
			_T("\r\n")
			_T("Please try to invoke:\r\n")
			_T("\r\n")
			_T("- \"Options\" menus, marked with a down arrow in captions of dynamic control bars\r\n")
			_T("\r\n")
			_T("- context menus over the MDI tab window (available in Prof-UIS and Prof-UIS Trial only)\r\n")
			_T("\r\n")
			_T("- system menus of MDI child frames\r\n")
			_T("\r\n")
			_T("- \"View | Dynamic Resizable Bars\" menu\r\n")
			_T("\r\n")
			_T("\r\n")
			,
			LPCTSTR( m_arrAllDynamicBars[i]->m_strCaptionText )
			);
		CSimpleDynamicEdit * pEdit = new CSimpleDynamicEdit;
		pEdit->m_brush.CreateSolidBrush(
				( STATIC_DOWNCAST(
					CSimpleControlBar,
					m_arrAllDynamicBars[i]
					)
				) -> m_clrEditorBackground
			);
		VERIFY(
			pEdit->Create(
				m_arrAllDynamicBars[i]->m_hWnd,
				LPCTSTR( strEditorText )
				)
			);
		// dynamic bars are initially invisible
		static const CSize _sizeDesiredDocked( 80, 60 );
		static const CSize _sizeDesiredFloating( 200, 150 );
		m_arrAllDynamicBars[i]->SetInitDesiredSizeHorizontal( _sizeDesiredDocked );
		m_arrAllDynamicBars[i]->SetInitDesiredSizeVertical( _sizeDesiredDocked );
		m_arrAllDynamicBars[i]->SetInitDesiredSizeFloating( _sizeDesiredFloating );
		ShowControlBar( m_arrAllDynamicBars[i], TRUE, TRUE );
	} // create edit controls inside all dynamic bars

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
		int i, j;
		for( i = 0, j = 0; i < m_arrAllDynamicBars.GetSize(); j++ )
		{
			static const UINT g_ArrDockBarIDs[] =
			{
				AFX_IDW_DOCKBAR_TOP,
				AFX_IDW_DOCKBAR_BOTTOM,
				AFX_IDW_DOCKBAR_LEFT,
				AFX_IDW_DOCKBAR_RIGHT,
			};
			UINT nDocBarID =
				g_ArrDockBarIDs[
					j % (sizeof(g_ArrDockBarIDs)/sizeof(g_ArrDockBarIDs[0]))
					];
			bool bDockIntoDocument = false;
			if( nDocBarID == AFX_IDW_DOCKBAR_FLOAT )
			{
				nDocBarID = AFX_IDW_DOCKBAR_LEFT;
				bDockIntoDocument = true;
			}
			m_arrAllDynamicBars[i]->
				DockControlBarInnerOuter(
					nDocBarID,
					true,
					this,
					false
					);
			if( i >= __DEMO_BARS_COUNT_INITIALLY_DOCKED )
			{
				m_arrAllDynamicBars[i]->BarStateSet(
					CExtDynamicControlBar::__EDBS_DOCUMENT,
					true
					);
			} // if( i >= __DEMO_BARS_COUNT_INITIALLY_DOCKED )
			i++;
			if( i < m_arrAllDynamicBars.GetSize() )
			{
				m_arrAllDynamicBars[i]->
					DockControlBarLTRB(
						m_arrAllDynamicBars[i-1],
						(	nDocBarID == AFX_IDW_DOCKBAR_LEFT
						||	nDocBarID == AFX_IDW_DOCKBAR_RIGHT
						)
							? AFX_IDW_DOCKBAR_BOTTOM
							: AFX_IDW_DOCKBAR_RIGHT
							,
						false
						);
				if( i >= __DEMO_BARS_COUNT_INITIALLY_DOCKED )
				{
					m_arrAllDynamicBars[i]->BarStateSet(
						CExtDynamicControlBar::__EDBS_DOCUMENT,
						true
						);
				} // if( i >= __DEMO_BARS_COUNT_INITIALLY_DOCKED )
				i++;
			} // if( i < m_arrAllDynamicBars.GetSize() )
		} // for( i = 0, j = 0; i < m_arrAllDynamicBars.GetSize(); j++ )
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		m_pBarPersistent->BarStateSet(
			CExtDynamicControlBar::__EDBS_AUTO_HIDDEN,
			true
			);
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		RecalcLayout();
	}

	g_CmdManager->SerializeState(
		pApp->m_pszProfileName,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		false
		);

	PostMessage( WM_COMMAND, ID_WINDOW_CASCADE );

//	m_pBarPersistent->FlashCaption( 65535, 250, RGB(255,255,0), RGB(0,128,0) );

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
	pApp;

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
		CExtDynamicBarSite::StateSave(
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
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CExtNCW < CMDIFrameWnd > :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	CExtNCW < CMDIFrameWnd > :: ActivateFrame( nCmdShow );
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CMDIFrameWnd > :: PreCreateWindow( cs ) )
		return FALSE;
	cs.style |= WS_MAXIMIZE;
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
	if( CExtDynamicBarSite::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
		return TRUE;
	if( nCode == CN_COMMAND )
	{
#ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
		if( nID == ID_WINDOWS_LIST )
		{
			CExtMdiWindowsListDlg dlgMdiWindowsList( this );
			dlgMdiWindowsList.DoModal();
			return TRUE;
		}
#endif // #ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
	}
	return CExtNCW < CMDIFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void CMainFrame::OnPersistenceLoadFromFile_Resizable() 
{
CString strPathName;

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile(
		NULL,
		CExtShellDialogFile::__EFDT_OPEN_SINGLE
		);
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("Resizable bar state files (*.RBS)|*.RBS|All files (*.*)|*.*||")
		);
	dlgShellFile.m_nInitialFileTypeIndex = 0;
	dlgShellFile.m_bFilesMustExist = true;
	dlgShellFile.m_strCustomDialogCaption = _T("Open resizable bar state from file");
	dlgShellFile.m_strEditorTextInitial = _T(".\\Default.RBS");
	if( dlgShellFile.DoModal() != IDOK )
		return;
	ASSERT( dlgShellFile.m_arrRetValNames.GetSize() == 1 );
	strPathName = LPCTSTR(dlgShellFile.m_arrRetValNames[0]);

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog dlgFile(
		TRUE,
		_T("Resizable bar state"),
		_T(".\\Default.RBS"),
		OFN_ENABLESIZING|OFN_EXPLORER
			|OFN_LONGNAMES|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST
			|OFN_NOCHANGEDIR|OFN_NODEREFERENCELINKS
			,
		_T("Resizable bar state files (*.RBS)|*.RBS|All files (*.*)|*.*||"),
		this
		);
	dlgFile.m_ofn.lpstrTitle = _T("Open resizable bar state from file");
	if( dlgFile.DoModal() != IDOK )
		return;
	strPathName = dlgFile.m_ofn.lpstrFile;
	ASSERT( ! strPathName.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

	try
	{
		// clean own array of dynamic bars
		m_arrAllDynamicBars.RemoveAll();

		// init data stream
		CFile _file(
			strPathName,
			CFile::modeRead
				|CFile::typeBinary
			);
		CArchive ar( &_file, CArchive::load );

		// load fully dynamic bars
		CExtDynamicBarSite::StateSerialize( ar );

		// query own array of all the fully dynamic bars
		CExtDynamicBarSite::BarEnumGetArray( m_arrAllDynamicBars, false, true );

		// re-initialize contained child windows in dynamic bars
		CWinApp * pApp = ::AfxGetApp();
		ASSERT( pApp != NULL );
		ASSERT( pApp->m_pszRegistryKey != NULL );
		ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
		ASSERT( pApp->m_pszProfileName != NULL );
		ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
		int i;
		for( i = 0; i < m_arrAllDynamicBars.GetSize(); i++ )
		{ // create edit controls inside all dynamic bars
			UINT nCmdID = (UINT)m_arrAllDynamicBars[i]->GetDlgCtrlID();
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					pApp->m_pszProfileName,
					nCmdID
					);
			ASSERT( pCmdItem != NULL );
			pCmdItem->StateSetBasic();
			CString strEditorText;
			strEditorText.Format(
				_T("\r\n")
				_T("Edit control inside dynamic resizable control bar \"%s\"\r\n")
				_T("\r\n")
				_T("\r\n")
				_T("Please try to invoke:\r\n")
				_T("\r\n")
				_T("- \"Options\" menus, marked with a down arrow in captions of dynamic control bars\r\n")
				_T("\r\n")
				_T("- context menus over the MDI tab window (available in Prof-UIS and Prof-UIS Trial only)\r\n")
				_T("\r\n")
				_T("- system menus of MDI child frames\r\n")
				_T("\r\n")
				_T("- \"View | Dynamic Resizable Bars\" menu\r\n")
				_T("\r\n")
				_T("\r\n")
				,
				LPCTSTR( m_arrAllDynamicBars[i]->m_strCaptionText )
				);
			CSimpleDynamicEdit * pEdit = new CSimpleDynamicEdit;
			pEdit->m_brush.CreateSolidBrush(
					( STATIC_DOWNCAST(
						CSimpleControlBar,
						m_arrAllDynamicBars[i]
						)
					) -> m_clrEditorBackground
				);
			VERIFY(
				pEdit->Create(
					m_arrAllDynamicBars[i]->m_hWnd,
					LPCTSTR( strEditorText )
					)
				);
			// dynamic bars are initially invisible
			static const CSize _sizeDesiredDocked( 80, 60 );
			static const CSize _sizeDesiredFloating( 200, 150 );
			m_arrAllDynamicBars[i]->SetInitDesiredSizeHorizontal( _sizeDesiredDocked );
			m_arrAllDynamicBars[i]->SetInitDesiredSizeVertical( _sizeDesiredDocked );
			m_arrAllDynamicBars[i]->SetInitDesiredSizeFloating( _sizeDesiredFloating );
			ShowControlBar( m_arrAllDynamicBars[i], TRUE, TRUE );
		} // create edit controls inside all dynamic bars

		// load docking state of all the resizable bars only
		if( ! CExtControlBar::ProfileBarStateSerialize(
				ar,
				this,
				NULL,
				false,
				true
				)
			)
		{
			ASSERT( FALSE );
			::AfxThrowUserException();
		}

		return;
	} // try
	catch( CException * pException )
	{
		pException;
	} // catch( CException * pException )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )
	CExtDynamicBarSite::BarEnumGetArray( m_arrAllDynamicBars, false, true );
	::AfxMessageBox( _T("Failed to load file"), MB_OK|MB_ICONERROR );
}

void CMainFrame::OnPersistenceSaveToFile_Resizable() 
{
CString strPathName;

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile(
		NULL,
		CExtShellDialogFile::__EFDT_SAVE_AS
		);
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("Resizable bar state files (*.RBS)|*.RBS|All files (*.*)|*.*||")
		);
	dlgShellFile.m_nInitialFileTypeIndex = 0;
	dlgShellFile.m_bPathMustExist = true;
	dlgShellFile.m_strCustomDialogCaption = _T("Save resizable bar state to file");
	dlgShellFile.m_strEditorTextInitial = _T(".\\Default.RBS");
	if( dlgShellFile.DoModal() != IDOK )
		return;
	ASSERT( dlgShellFile.m_arrRetValNames.GetSize() == 1 );
	strPathName = LPCTSTR(dlgShellFile.m_arrRetValNames[0]);

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog dlgFile(
		FALSE,
		_T("Resizable bar state"),
		_T(".\\Default.RBS"),
		OFN_ENABLESIZING|OFN_EXPLORER
			|OFN_LONGNAMES|OFN_PATHMUSTEXIST
			|OFN_NOCHANGEDIR|OFN_NODEREFERENCELINKS
			,
		_T("Resizable bar state files (*.RBS)|*.RBS|All files (*.*)|*.*||"),
		this
		);
	dlgFile.m_ofn.lpstrTitle = _T("Save resizable bar state to file");
	if( dlgFile.DoModal() != IDOK )
		return;
	strPathName = dlgFile.m_ofn.lpstrFile;
	ASSERT( ! strPathName.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

	try
	{
		// initialize data stream
		CFile _file(
			strPathName,
			CFile::modeCreate
				|CFile::modeWrite
				|CFile::typeBinary
			);
		CArchive ar( &_file, CArchive::store );

		// save state of dynamic bars
		CExtDynamicBarSite::StateSerialize( ar );

		// save docking state of all the resizable bars only
		if( ! CExtControlBar::ProfileBarStateSerialize(
				ar,
				this,
				NULL,
				false,
				true
				)
			)
		{
			ASSERT( FALSE );
			::AfxThrowUserException();
		}
		return;
	} // try
	catch( CException * pException )
	{
		pException;
	} // catch( CException * pException )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )
	::AfxMessageBox( _T("Failed to save file"), MB_OK|MB_ICONERROR );
}

void CMainFrame::OnPersistenceLoadFromFile_Fixedsized() 
{
CString strPathName;

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile(
		NULL,
		CExtShellDialogFile::__EFDT_OPEN_SINGLE
		);
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("Fixedsized bar state files (*.FBS)|*.FBS|All files (*.*)|*.*||")
		);
	dlgShellFile.m_nInitialFileTypeIndex = 0;
	dlgShellFile.m_bFilesMustExist = true;
	dlgShellFile.m_strCustomDialogCaption = _T("Open fixedsized bar state from file");
	dlgShellFile.m_strEditorTextInitial = _T(".\\Default.FBS");
	if( dlgShellFile.DoModal() != IDOK )
		return;
	ASSERT( dlgShellFile.m_arrRetValNames.GetSize() == 1 );
	strPathName = LPCTSTR(dlgShellFile.m_arrRetValNames[0]);

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog dlgFile(
		TRUE,
		_T("Fixedsized bar state"),
		_T(".\\Default.FBS"),
		OFN_ENABLESIZING|OFN_EXPLORER
			|OFN_LONGNAMES|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST
			|OFN_NOCHANGEDIR|OFN_NODEREFERENCELINKS
			,
		_T("Fixedsized bar state files (*.FBS)|*.FBS|All files (*.*)|*.*||"),
		this
		);
	dlgFile.m_ofn.lpstrTitle = _T("Open fixedsized bar state from file");
	if( dlgFile.DoModal() != IDOK )
		return;
	strPathName = dlgFile.m_ofn.lpstrFile;
	ASSERT( ! strPathName.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

	try
	{
		// init data stream
		CFile _file(
			strPathName,
			CFile::modeRead
				|CFile::typeBinary
			);
		CArchive ar( &_file, CArchive::load );

		// load docking state of all the fixedsized bars only
		if( ! CExtControlBar::ProfileBarStateSerialize(
				ar,
				this,
				NULL,
				true,
				false
				)
			)
		{
			ASSERT( FALSE );
			::AfxThrowUserException();
		}

		return;
	} // try
	catch( CException * pException )
	{
		pException;
	} // catch( CException * pException )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )
	::AfxMessageBox( _T("Failed to load file"), MB_OK|MB_ICONERROR );
}

void CMainFrame::OnPersistenceSaveToFile_Fixedsized() 
{
CString strPathName;

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile(
		NULL,
		CExtShellDialogFile::__EFDT_SAVE_AS
		);
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("Fixedsized bar state files (*.FBS)|*.FBS|All files (*.*)|*.*||")
		);
	dlgShellFile.m_nInitialFileTypeIndex = 0;
	dlgShellFile.m_bPathMustExist = true;
	dlgShellFile.m_strCustomDialogCaption = _T("Save fixedsized bar state to file");
	dlgShellFile.m_strEditorTextInitial = _T(".\\Default.FBS");
	if( dlgShellFile.DoModal() != IDOK )
		return;
	ASSERT( dlgShellFile.m_arrRetValNames.GetSize() == 1 );
	strPathName = LPCTSTR(dlgShellFile.m_arrRetValNames[0]);

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog dlgFile(
		FALSE,
		_T("Fixedsized bar state"),
		_T(".\\Default.FBS"),
		OFN_ENABLESIZING|OFN_EXPLORER
			|OFN_LONGNAMES|OFN_PATHMUSTEXIST
			|OFN_NOCHANGEDIR|OFN_NODEREFERENCELINKS
			,
		_T("Fixedsized bar state files (*.FBS)|*.FBS|All files (*.*)|*.*||"),
		this
		);
	dlgFile.m_ofn.lpstrTitle = _T("Save fixedsized bar state to file");
	if( dlgFile.DoModal() != IDOK )
		return;
	strPathName = dlgFile.m_ofn.lpstrFile;
	ASSERT( ! strPathName.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

	try
	{
		// initialize data stream
		CFile _file(
			strPathName,
			CFile::modeCreate
				|CFile::modeWrite
				|CFile::typeBinary
			);
		CArchive ar( &_file, CArchive::store );

		// save docking state of all the resizable bars only
		if( ! CExtControlBar::ProfileBarStateSerialize(
				ar,
				this,
				NULL,
				true,
				false
				)
			)
		{
			ASSERT( FALSE );
			::AfxThrowUserException();
		}
		return;
	} // try
	catch( CException * pException )
	{
		pException;
	} // catch( CException * pException )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )
	::AfxMessageBox( _T("Failed to save file"), MB_OK|MB_ICONERROR );
}

