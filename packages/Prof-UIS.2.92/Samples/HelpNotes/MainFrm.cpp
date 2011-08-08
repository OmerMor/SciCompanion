// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "HelpNotes.h"

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

	ON_REGISTERED_MESSAGE( CExtPopupMenuWnd::g_nMsgItemCoveringNotification, OnItemCoveringNotification )
	ON_REGISTERED_MESSAGE( CExtPopupMenuWnd::g_nMsgPrepareDesktopBk, OnAnyDeactivatePopup )
	ON_REGISTERED_MESSAGE( CExtPopupMenuWnd::g_nMsgNotifyMenuExpanded, OnAnyDeactivatePopup )
	ON_REGISTERED_MESSAGE( CExtPopupMenuWnd::g_nMsgNotifyMenuClosed, OnAnyDeactivatePopup )

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
	: m_pWndView( NULL )
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

	CExtPopupMenuWnd::g_bMenuExpanding = false;
	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;
	CExtPopupMenuWnd::g_bMenuShowCoolTips = false;

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

CCreateContext _cc;
	_cc.m_pCurrentDoc = NULL;
	_cc.m_pCurrentFrame = this;
	_cc.m_pLastView = NULL;
	_cc.m_pNewDocTemplate = NULL;
	_cc.m_pNewViewClass = RUNTIME_CLASS( CChildView );
	m_pWndView = (CChildView*)CreateView( &_cc );
	if( m_pWndView == NULL )
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	ASSERT_KINDOF( CChildView, m_pWndView );
CChildView * pChildView =
		STATIC_DOWNCAST( CChildView, m_pWndView );
	SetActiveView( pChildView );
	m_pWndView->ModifyStyleEx( WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME, 0, SWP_FRAMECHANGED );
	
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
			NULL, // _T("Menu Bar"),
			this,
			ID_VIEW_MENUBAR,
			WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS
				|CBRS_TOP|CBRS_GRIPPER|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
			)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }

	if(		(! m_wndToolBar.Create(
				_T("ToolBar"),
				this,
				AFX_IDW_TOOLBAR,
				WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS
					|CBRS_TOP|CBRS_GRIPPER|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
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

UINT g_arrResetTextIDs[] =
{
	ID_FILE_NEW, ID_FILE_OPEN, ID_FILE_SAVE,
	ID_FILE_SEND_MAIL,
	ID_EDIT_UNDO, ID_EDIT_COPY, ID_EDIT_CUT,
	ID_EDIT_PASTE
};
	for( int i = 0; i < sizeof(g_arrResetTextIDs)/sizeof(g_arrResetTextIDs[0]); i++ )
	{
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
				pApp->m_pszProfileName,
				g_arrResetTextIDs[i]
				);
		ASSERT( pCmdItem != NULL );
		pCmdItem->m_sToolbarText = pCmdItem->m_sMenuText;
	}

	m_wndStatusBar.m_bDrawPaneSeparatorsInsteadOfBorders = true;
	if(		(! m_wndStatusBar.Create(this) )
		||	(! m_wndStatusBar.SetIndicators(
				indicators,
			sizeof(indicators)/sizeof(UINT)) )
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarUiLook.EnableDocking(CBRS_ALIGN_ANY);

	if( ! CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
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
		RecalcLayout();
	}

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
	if( m_pWndView->GetSafeHwnd() != NULL )
		m_pWndView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if( nCode == CN_COMMAND && pExtra == NULL )
		m_wndPopupRichEditView.Deactivate();
	if(		m_pWndView->GetSafeHwnd() != NULL
		&&	m_pWndView->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo )
		)
		return TRUE;
	return CExtNCW < CFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndToolBarUiLook.PreTranslateMessage( pMsg ) )
		return TRUE;
	if( m_wndMenuBar.TranslateMainFrameMessage( pMsg ) )
		return TRUE;
	return CExtNCW < CFrameWnd > ::PreTranslateMessage( pMsg );
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
	
	return CExtNCW < CFrameWnd > ::DestroyWindow();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CExtNCW < CFrameWnd > ::ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	CExtNCW < CFrameWnd > :: ActivateFrame( nCmdShow );
}

LRESULT CMainFrame::OnItemCoveringNotification( WPARAM wParam, LPARAM lParam )
{
	lParam;
CExtPopupMenuWnd::ITEMCOVERINGNOTIFICATON * pICN =
		CExtPopupMenuWnd::ITEMCOVERINGNOTIFICATON::
			FromWPARAM( wParam );
	ASSERT( pICN != NULL );
	pICN->m_bSuspendTips = false;
	if(		pICN->m_eICN == CExtPopupMenuWnd::ITEMCOVERINGNOTIFICATON::__EICN_DELAY
		||	pICN->m_eICN == CExtPopupMenuWnd::ITEMCOVERINGNOTIFICATON::__EICN_CANCEL
		)
	{
		m_wndPopupRichEditView.Deactivate();
		return 0;
	}
	pICN->m_bSuspendTips = true;
	ASSERT( pICN->m_eICN == CExtPopupMenuWnd::ITEMCOVERINGNOTIFICATON::__EICN_SET );
CRect rcItem( 0, 0, 0, 0 );
UINT nAlignType = AFX_IDW_DOCKBAR_RIGHT;
UINT nCmdID = 0;
	if( pICN->m_pPopup != NULL )
	{
		ASSERT( pICN->m_pTBB == NULL );
		ASSERT_VALID( pICN->m_pPopup );
		CExtPopupMenuWnd::MENUITEMDATA & _mii =
			pICN->m_pPopup->ItemGetInfo(
				pICN->m_nPopupItemIndex
				);
		if(		_mii.IsSeparator()
			||	_mii.IsPopup()
			||	CExtCmdManager::IsSystemCommand( _mii.GetCmdID() )
			)
		{
			m_wndPopupRichEditView.Deactivate();
			return 0;
		}
		pICN->m_pPopup->_GetItemRect(
			pICN->m_nPopupItemIndex,
			rcItem
			);
		pICN->m_pPopup->ClientToScreen( &rcItem );
		if( _mii.IsToolButton() )
		{
			CRect rcPopup;
			pICN->m_pPopup->GetWindowRect( &rcPopup );
			rcItem.left = rcPopup.left;
			rcItem.right = rcPopup.right;
		}
		nCmdID = _mii.GetCmdID();
	} // if( pICN->m_pPopup != NULL )
	else
	{
		ASSERT_VALID( pICN->m_pTBB );
		ASSERT_VALID( pICN->m_pTBB->GetBar() );
		if(		pICN->m_pTBB->IsAbleToTrackMenu()
			||	pICN->m_pTBB->IsSeparator()
			)
		{
			m_wndPopupRichEditView.Deactivate();
			return 0;
		}
		rcItem = pICN->m_pTBB->Rect();
		pICN->m_pTBB->GetBar()->ClientToScreen( &rcItem );
		nCmdID = pICN->m_pTBB->GetCmdID( false );
		if( pICN->m_pTBB->GetBar()->IsFloating() )
			nAlignType = AFX_IDW_DOCKBAR_BOTTOM;
		else
		{
			nAlignType = (UINT)
				pICN->m_pTBB->GetBar()->GetParent()->GetDlgCtrlID();
			switch( nAlignType )
			{
			case AFX_IDW_DOCKBAR_TOP:
				nAlignType = AFX_IDW_DOCKBAR_BOTTOM;
			break;
			case AFX_IDW_DOCKBAR_BOTTOM:
				nAlignType = AFX_IDW_DOCKBAR_TOP;
			break;
			case AFX_IDW_DOCKBAR_LEFT:
				nAlignType = AFX_IDW_DOCKBAR_RIGHT;
			break;
			case AFX_IDW_DOCKBAR_RIGHT:
				nAlignType = AFX_IDW_DOCKBAR_LEFT;
			break;
#ifdef _DEBUG
			default:
				{
					ASSERT( FALSE );
				}
			break;
#endif // _DEBUG
			} // switch( nAlignType )
		}
	} // else from if( pICN->m_pPopup != NULL )
	ScreenToClient( &rcItem );

CExtResourceManager::CExtResourceMemoryBuffer buffer;
	if(	! g_ResourceManager->LoadResourceBuffer(
			buffer,
			_T("RTF"),
			nCmdID
			)
		)
	{
		return 0;
	}
	ASSERT_VALID( m_pWndView );
	m_wndPopupRichEditView.m_clrBackground =
		( m_pWndView->m_checkCustomBkColor.GetCheck() != 0 )
			? m_pWndView->m_buttonCustomBkColor.m_clrSelected
			: COLORREF(-1L)
			;
	m_wndPopupRichEditView.m_clrBorderLT =
		( m_pWndView->m_checkCustomBorderLtColor.GetCheck() != 0 )
			? m_pWndView->m_buttonCustomBorderLtColor.m_clrSelected
			: COLORREF(-1L)
			;
	m_wndPopupRichEditView.m_clrBorderRB =
		( m_pWndView->m_checkCustomBorderRbColor.GetCheck() != 0 )
			? m_pWndView->m_buttonCustomBorderRbColor.m_clrSelected
			: COLORREF(-1L)
			;
	if( m_pWndView->m_radioShadow2.GetCheck() != 0 )
		m_wndPopupRichEditView.m_eST = CExtPopupRichEditView::__EST_POPUP;
	else if( m_pWndView->m_radioShadow3.GetCheck() != 0 )
		m_wndPopupRichEditView.m_eST = CExtPopupRichEditView::__EST_WINHELP;
	else
		m_wndPopupRichEditView.m_eST = CExtPopupRichEditView::__EST_NONE;
	m_wndPopupRichEditView.Activate(
		buffer,
		rcItem,
		m_hWnd,
		nAlignType,
		8
		);
	
	return 0;
}

LRESULT CMainFrame::OnAnyDeactivatePopup( WPARAM wParam, LPARAM lParam )
{
	wParam;
	lParam;
	m_wndPopupRichEditView.Deactivate();
	return 0;
}
