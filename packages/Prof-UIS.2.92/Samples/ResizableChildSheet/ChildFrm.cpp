// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "ResizableChildSheet.h"

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
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CMDIChildWnd > :: PreCreateWindow(cs) )
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

void CChildFrame::Dump( CDumpContext & dc ) const
{
	CExtNCW < CMDIChildWnd > :: Dump(dc);
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

HINSTANCE hInstResource =
		AfxFindResourceHandle(
			MAKEINTRESOURCE( IDR_CHILDFRAME ),
			RT_GROUP_ICON
			);
	ASSERT( hInstResource != NULL );
HICON hIcon = (HICON)
		::LoadImage(
			hInstResource,
			MAKEINTRESOURCE( IDR_CHILDFRAME ),
			IMAGE_ICON,
			16,
			16,
			0
			);
	ASSERT( hIcon != NULL );
	SetIcon( hIcon, FALSE );
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	hIcon = pApp->LoadIcon( IDR_CHILDFRAME );
	ASSERT( hIcon != NULL );
	SetIcon( hIcon, TRUE );


	
	m_wndView.AddPage( &m_wndPage1 );
	m_wndView.AddPage( &m_wndPage2 );
	m_wndView.AddPage( &m_wndPage3 );
	if( !m_wndView.Create(this) )
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	m_wndView.SetDlgCtrlID( AFX_IDW_PANE_FIRST );
	RecalcLayout();
	m_wndView.SetActivePage( 2 );
	
	
	return 0;
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CExtNCW < CMDIChildWnd > :: OnSetFocus( pOldWnd );
	m_wndView.SetFocus();
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
	if( m_wndView.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
		return TRUE;
		// otherwise, do default handling
	return CExtNCW < CMDIChildWnd > :: OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CChildFrame::PreTranslateMessage(MSG* pMsg) 
{
CMainFrame * pMainFrame =
		STATIC_DOWNCAST( CMainFrame, AfxGetMainWnd() );
	if( pMainFrame->m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	return CExtNCW < CMDIChildWnd > :: PreTranslateMessage(pMsg);
}

