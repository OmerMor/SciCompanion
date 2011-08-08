// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "MDIDOCVIEW.h"

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CMDIChildWnd > :: PreCreateWindow( cs ) )
		return FALSE;
	cs.style =
		WS_CHILD | WS_VISIBLE
			| WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
			| FWS_ADDTOTITLE | WS_THICKFRAME
			| WS_MINIMIZEBOX | WS_MAXIMIZEBOX
			| WS_CLIPCHILDREN | WS_CLIPSIBLINGS
			;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME);
	return TRUE;
}

void CChildFrame::ActivateFrame(int nCmdShow)
{
//	nCmdShow = SW_SHOWMAXIMIZED;
	CExtNCW < CMDIChildWnd > :: ActivateFrame( nCmdShow );
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

BOOL CChildFrame::PreTranslateMessage(MSG* pMsg) 
{
CMainFrame * pMainFrame =
		STATIC_DOWNCAST( CMainFrame, GetParentFrame() );
	ASSERT_VALID( pMainFrame );
	if( pMainFrame->m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	return CExtNCW < CMDIChildWnd > :: PreTranslateMessage( pMsg );
}

