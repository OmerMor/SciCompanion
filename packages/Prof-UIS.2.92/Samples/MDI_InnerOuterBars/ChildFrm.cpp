// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "MDI_InnerOuterBars.h"

#include "ChildFrm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE( CChildFrame, CMDIChildWnd )

BEGIN_MESSAGE_MAP( CChildFrame, CMDIChildWnd )
	//{{AFX_MSG_MAP(CChildFrame)
		ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
		ON_WM_SETFOCUS()
		ON_WM_CREATE()
	//}}AFX_MSG_MAP

	ON_COMMAND_EX(ID_VIEW_CHILDFRAME_BAR_0, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_CHILDFRAME_BAR_0, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_CHILDFRAME_BAR_1, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_CHILDFRAME_BAR_1, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_CHILDFRAME_BAR_2, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_CHILDFRAME_BAR_2, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR_CHILD0, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_CHILD0, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR_CHILD1, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_CHILD1, OnUpdateControlBarMenu)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
	: m_sProfileName( _T("Default MDI child profile") )
{
	// TODO: add member initialization code here
	
	m_hChildFrameIcon = NULL;
HINSTANCE hInstResource =
		AfxFindResourceHandle(
			MAKEINTRESOURCE(IDR_MDITYPE),
			RT_GROUP_ICON
			);
	ASSERT( hInstResource != NULL );
	if( hInstResource != NULL )
	{
		m_hChildFrameIcon =
			(HICON)::LoadImage(
				hInstResource,
				MAKEINTRESOURCE(IDR_MDITYPE),
				IMAGE_ICON,
				16,
				16,
				0
				);
		ASSERT( m_hChildFrameIcon != NULL );
	}
}

CChildFrame::~CChildFrame()
{

	if( m_hChildFrameIcon != NULL )
	{
		::DestroyIcon( m_hChildFrameIcon );
	}

}

void CChildFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	CExtControlBar::DoFrameBarCheckUpdate( this, pCmdUI, true );
}

BOOL CChildFrame::OnBarCheck(UINT nID)
{
	return CExtControlBar::DoFrameBarCheckCmd( this, nID, true );
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( ! CExtNCW < CMDIChildWnd > :: PreCreateWindow( cs ) )
		return FALSE;

	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CExtNCW < CMDIChildWnd > :: AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CExtNCW < CMDIChildWnd > :: Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
void CChildFrame::OnFileClose() 
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.

	SendMessage(WM_CLOSE);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CExtNCW < CMDIChildWnd > :: OnCreate( lpCreateStruct ) == -1 )
		return -1;

	ASSERT( m_hChildFrameIcon != NULL );
	SetIcon( m_hChildFrameIcon, FALSE );
	SetIcon( m_hChildFrameIcon, TRUE );

	// create a view to occupy the client area of the frame
	if( ! m_wndView.Create(
			NULL,
			NULL,
			AFX_WS_DEFAULT_VIEW,
			CRect(0, 0, 0, 0),
			this,
			AFX_IDW_PANE_FIRST,
			NULL
			)
		)
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if(	!m_wndToolbar0.Create(
			_T("Child Toolbar 0"),
			this,
			IDR_TOOLBAR_CHILD0
			)
		|| !m_wndToolbar0.LoadToolBar(IDR_TOOLBAR_CHILD0)
		)
	{
		TRACE0("Failed to create m_wndToolbar0\n");
		return -1;		// fail to create
	}

	if(	!m_wndToolbar1.Create(
			_T("Child Toolbar 1"),
			this,
			IDR_TOOLBAR_CHILD1
			)
		|| !m_wndToolbar1.LoadToolBar(IDR_TOOLBAR_CHILD1)
		)
	{
		TRACE0("Failed to create m_wndToolbar1\n");
		return -1;		// fail to create
	}

	if(	!m_wndResizableBar0.Create(
			_T("Resizable Bar 0"),
			this,
			ID_VIEW_CHILDFRAME_BAR_0
			)
		|| !m_wndListBox0.Create(
			WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT,
			CRect( 0, 0, 0, 0 ),
			&m_wndResizableBar0,
			UINT( IDC_STATIC )
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar0\n");
		return -1;		// fail to create
	}
	m_wndListBox0.SetFont( CFont::FromHandle( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) ) );
	m_wndListBox0.AddString( _T("CChildFrame::m_wndListBox0") );

	if(	!m_wndResizableBar1.Create(
			_T("Resizable Bar 1"),
			this,
			ID_VIEW_CHILDFRAME_BAR_1
			)
		|| !m_wndListBox1.Create(
			WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT,
			CRect( 0, 0, 0, 0 ),
			&m_wndResizableBar1,
			UINT( IDC_STATIC )
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar1\n");
		return -1;		// fail to create
	}
	m_wndListBox1.SetFont( CFont::FromHandle( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) ) );
	m_wndListBox1.AddString( _T("CChildFrame::m_wndListBox1") );

	if(	!m_wndResizableBar2.Create(
			_T("Resizable Bar 2"),
			this,
			ID_VIEW_CHILDFRAME_BAR_2
			)
		|| !m_wndListBox2.Create(
			WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT,
			CRect( 0, 0, 0, 0 ),
			&m_wndResizableBar2,
			UINT( IDC_STATIC )
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar2\n");
		return -1;		// fail to create
	}
	m_wndListBox2.SetFont( CFont::FromHandle( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) ) );
	m_wndListBox2.AddString( _T("CChildFrame::m_wndListBox2") );

	m_wndToolbar0.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolbar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar0.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar2.EnableDocking(CBRS_ALIGN_ANY);

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


CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );

	if( ! CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			LPCTSTR( m_sProfileName )
			)
		)
	{
		DockControlBar( &m_wndToolbar0, AFX_IDW_DOCKBAR_TOP );
		DockControlBar( &m_wndToolbar1, AFX_IDW_DOCKBAR_LEFT );
		m_wndResizableBar0.DockControlBar( AFX_IDW_DOCKBAR_TOP, 1, this, false );
		m_wndResizableBar0.DockControlBar( &m_wndResizableBar1, true, false, this, false );
		m_wndResizableBar2.DockControlBar( AFX_IDW_DOCKBAR_BOTTOM, 1, this, true );
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
		m_wndResizableBar2.AutoHideModeSet( true, false, false, true );
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	}
	DelayRecalcLayout();

	return 0;
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CExtNCW < CMDIChildWnd > :: OnSetFocus( pOldWnd );
	if( m_wndView.GetSafeHwnd() != NULL )
		m_wndView.SetFocus();
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
	if( m_wndView.GetSafeHwnd() != NULL )
	{
		if( m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo ) )
			return TRUE;
	}
	
	// otherwise, do default handling
	return CExtNCW < CMDIChildWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}


BOOL CChildFrame::PreTranslateMessage(MSG* pMsg) 
{
CMainFrame * pMainFrame =
		DYNAMIC_DOWNCAST(
			CMainFrame,
			::AfxGetMainWnd()
			);
	ASSERT_VALID( pMainFrame );
	if( pMainFrame->m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	
	return CExtNCW < CMDIChildWnd > :: PreTranslateMessage( pMsg );
}

BOOL CChildFrame::DestroyWindow() 
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );

	VERIFY(
		CExtControlBar::ProfileBarStateSave(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			LPCTSTR( m_sProfileName )
			)
		);
	g_CmdManager->ProfileWndRemove( GetSafeHwnd() );
	
	return CExtNCW < CMDIChildWnd > :: DestroyWindow();
}

