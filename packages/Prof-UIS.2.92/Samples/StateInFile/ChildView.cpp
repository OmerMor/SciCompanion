// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "StateInFile.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ! CWnd::PreCreateWindow( cs ) )
		return FALSE;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME);
	cs.style &= ~WS_BORDER;
	cs.lpszClass = ::AfxRegisterWndClass( CS_HREDRAW|CS_VREDRAW, ::LoadCursor( NULL, IDC_ARROW ) );
	return TRUE;
}

void CChildView::OnPaint() 
{
CRect rcClient;
	GetClientRect( &rcClient );
CPaintDC dcPaint( this );
CExtMemoryDC dc( &dcPaint, &rcClient );
	g_PaintManager->PaintDocumentClientAreaBkgnd( dc, this );
}

