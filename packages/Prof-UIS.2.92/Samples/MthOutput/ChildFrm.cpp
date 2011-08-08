// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "MthOutput.h"

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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
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

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( ! CExtNCW < CMDIChildWnd > :: PreCreateWindow( cs ) )
		return FALSE;

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

	m_DemoThread.m_wndDemoEventSource.m_pWndLogCtrl = &m_wndLogCtrl;
	if( 	!m_wndLogCtrl.Create(
				this,
				UINT(AFX_IDW_PANE_FIRST)
				)
		||	!m_DemoThread.CreateThread(
				&m_wndLogCtrl
				)
		)
	{
		ASSERT( FALSE );
		return -1;
	}

	return 0;
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CExtNCW < CMDIChildWnd > :: OnSetFocus( pOldWnd );
	if( m_wndLogCtrl.GetSafeHwnd() != NULL )
		m_wndLogCtrl.SetFocus();
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
	if( m_wndLogCtrl.GetSafeHwnd() != NULL )
	{
		if( m_wndLogCtrl.OnCmdMsg( nID,nCode,pExtra,pHandlerInfo ) )
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
	HWND hWndEventSource = m_DemoThread.m_wndDemoEventSource.GetSafeHwnd();
	if(		hWndEventSource != NULL
		&&	::IsWindow(hWndEventSource)
		)
	{
		m_DemoThread.m_wndDemoEventSource.PostMessage(
			WM_TIMER,
			ID_TIMER_DELAYED_DESTROY
			);
		Sleep( 1000 );
	}
	
	return CExtNCW < CMDIChildWnd > :: DestroyWindow();
}

