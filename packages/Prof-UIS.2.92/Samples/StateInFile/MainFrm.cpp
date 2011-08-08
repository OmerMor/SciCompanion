// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "StateInFile.h"

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

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC( CMainFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP

	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_UI_LOOK_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_UI_LOOK_BAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_0, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_0, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_1, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_1, OnUpdateControlBarMenu)

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
	CExtControlBar::DoFrameBarCheckUpdate( this, pCmdUI, true );
}

BOOL CMainFrame::OnBarCheck(UINT nID)
{
	return CExtControlBar::DoFrameBarCheckCmd( this, nID, true );
}

static UINT g_statBasicCommands[] =
{
	ID_APP_ABOUT,
	ID_APP_EXIT,
//	ID_VIEW_MENUBAR,
	ID_VIEW_TOOLBAR,
	ID_VIEW_UI_LOOK_BAR,
//	ID_VIEW_RESIZABLE_BAR_0,
//	ID_VIEW_RESIZABLE_BAR_1,
	ID_VIEW_STATUS_BAR,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
	ID_EDIT_COPY,
	ID_EDIT_CUT,
	ID_EDIT_PASTE,
	0, // end of list
};

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( CExtNCW < CFrameWnd > :: OnCreate( lpCreateStruct ) == -1 )
		return -1;
	// create a view to occupy the client area of the frame
	if(	! m_wndView.Create(
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
			NULL, // _T("Menu Bar"),
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
		TRACE0("Failed to create the m_wndToolBar toolbar\n");
		return -1;      // fail to create
	}

	if(		(! m_wndToolBarUiLook.Create( NULL, this, ID_VIEW_UI_LOOK_BAR ) )
		||	(! m_wndToolBarUiLook.ThemeSwitcherInit() )
		)
	{
		TRACE0("Failed to create the m_wndToolBarUiLook toolbar\n");
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

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBarUiLook.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBar0.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBar1.EnableDocking( CBRS_ALIGN_ANY );
	
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
		m_wndResizableBar0.DockControlBar( AFX_IDW_DOCKBAR_LEFT, 1 );
		m_wndResizableBar1.DockControlBar( AFX_IDW_DOCKBAR_BOTTOM, 2 );
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
	if( m_wndView.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
		return TRUE;
	// otherwise, do default handling
	return CExtNCW < CFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
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

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndToolBarUiLook.PreTranslateMessage( pMsg ) )
		return TRUE;
	if( m_wndMenuBar.TranslateMainFrameMessage( pMsg ) )
		return TRUE;
	return CExtNCW < CFrameWnd > :: PreTranslateMessage( pMsg );
}

void CMainFrame::OnFileOpen() 
{
CString strPathName;

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile(
		NULL,
		CExtShellDialogFile::__EFDT_OPEN_SINGLE
		);
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("UI state files (*.UI state)|*.UI state|All files (*.*)|*.*||")
		);
	dlgShellFile.m_nInitialFileTypeIndex = 0;
	dlgShellFile.m_bFilesMustExist = true;
	dlgShellFile.m_strCustomDialogCaption = _T("Open UI state from file");
	dlgShellFile.m_strEditorTextInitial = _T(".\\Default.UI state");
	if( dlgShellFile.DoModal() != IDOK )
		return;
	ASSERT( dlgShellFile.m_arrRetValNames.GetSize() == 1 );
	strPathName = LPCTSTR(dlgShellFile.m_arrRetValNames[0]);

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog dlgFile(
		TRUE,
		_T("UI state"),
		_T(".\\Default.UI state"),
		OFN_ENABLESIZING|OFN_EXPLORER
			|OFN_LONGNAMES|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST
			|OFN_NOCHANGEDIR|OFN_NODEREFERENCELINKS
			,
		_T("UI state files (*.UI state)|*.UI state|All files (*.*)|*.*||"),
		this
		);
	dlgFile.m_ofn.lpstrTitle = _T("Open UI state from file");
	if( dlgFile.DoModal() != IDOK )
		return;
	strPathName = dlgFile.m_ofn.lpstrFile;
	ASSERT( ! strPathName.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

	try
	{
		CFile _file(
			strPathName,
			CFile::modeRead|CFile::shareExclusive
			);
		CArchive ar( &_file, CArchive::load);

		CWinApp * pApp = ::AfxGetApp();
		ASSERT( pApp != NULL );
		ASSERT( pApp->m_pszRegistryKey != NULL );
		ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
		ASSERT( pApp->m_pszProfileName != NULL );
		ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

		ShowWindow( SW_HIDE );
		VERIFY(
			CExtControlBar::ProfileBarStateSerialize(
				ar,
				this,
				&m_dataFrameWP
				)
			);
		VERIFY(
			g_CmdManager->SerializeState(
				pApp->m_pszProfileName,
				ar
				)
			);
		ActivateFrame( SW_SHOW );
	} // try
	catch( CException * pXept )
	{
		CString sErrorDescription;
		VERIFY(
			pXept->GetErrorMessage(
				sErrorDescription.GetBuffer(8192), 8192
				)
			);
		sErrorDescription.ReleaseBuffer();
		ASSERT( !sErrorDescription.IsEmpty() );
		pXept->Delete();
		ActivateFrame( SW_SHOW );
		::ProfUISMsgBox( GetSafeHwnd(), LPCTSTR(sErrorDescription), _T("State load failed"), MB_ICONERROR );
	} // catch( CException * pXept )
	catch( ... )
	{
		ActivateFrame( SW_SHOW );
		::ProfUISMsgBox( GetSafeHwnd(), _T("Unknown exception caught"), _T("State load failed"), MB_ICONERROR );
	} // catch( ... )
}

void CMainFrame::OnFileSave() 
{
CString strPathName;

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile(
		NULL,
		CExtShellDialogFile::__EFDT_SAVE_AS
		);
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("UI state files (*.UI state)|*.UI state|All files (*.*)|*.*||")
		);
	dlgShellFile.m_nInitialFileTypeIndex = 0;
	dlgShellFile.m_bPathMustExist = true;
	dlgShellFile.m_strCustomDialogCaption = _T("Save UI state to file");
	dlgShellFile.m_strEditorTextInitial = _T(".\\Default.UI state");
	if( dlgShellFile.DoModal() != IDOK )
		return;
	ASSERT( dlgShellFile.m_arrRetValNames.GetSize() == 1 );
	strPathName = LPCTSTR(dlgShellFile.m_arrRetValNames[0]);

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog dlgFile(
		FALSE,
		_T("UI state"),
		_T(".\\Default.UI state"),
		OFN_ENABLESIZING|OFN_EXPLORER
			|OFN_LONGNAMES|OFN_PATHMUSTEXIST
			|OFN_NOCHANGEDIR|OFN_NODEREFERENCELINKS
			,
		_T("UI state files (*.UI state)|*.UI state|All files (*.*)|*.*||"),
		this
		);
	dlgFile.m_ofn.lpstrTitle = _T("Save UI state to file");
	if( dlgFile.DoModal() != IDOK )
		return;
	strPathName = dlgFile.m_ofn.lpstrFile;
	ASSERT( ! strPathName.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

	try
	{
		CFile _file(
			strPathName,
			CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive
			);
		CArchive ar( &_file, CArchive::store);

		CWinApp * pApp = ::AfxGetApp();
		ASSERT( pApp != NULL );
		ASSERT( pApp->m_pszRegistryKey != NULL );
		ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
		ASSERT( pApp->m_pszProfileName != NULL );
		ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

		VERIFY(
			CExtControlBar::ProfileBarStateSerialize(
				ar,
				this
				)
			);
		VERIFY(
			g_CmdManager->SerializeState(
				pApp->m_pszProfileName,
				ar
				)
			);
	} // try
	catch( CException * pXept )
	{
		CString sErrorDescription;
		VERIFY(
			pXept->GetErrorMessage(
				sErrorDescription.GetBuffer(8192), 8192
				)
			);
		sErrorDescription.ReleaseBuffer();
		ASSERT( !sErrorDescription.IsEmpty() );
		pXept->Delete();
		::ProfUISMsgBox( GetSafeHwnd(), LPCTSTR(sErrorDescription), _T("State save failed"), MB_ICONERROR );
	} // catch( CException * pXept )
	catch( ... )
	{
		::ProfUISMsgBox( GetSafeHwnd(), _T("Unknown exception caught"), _T("State save failed"), MB_ICONERROR );
	} // catch( ... )
}

