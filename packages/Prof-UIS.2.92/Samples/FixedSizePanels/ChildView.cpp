// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "FixedSizePanels.h"
#include "ChildView.h"

#if !defined(__EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

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
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ! CWnd::PreCreateWindow( cs ) )
		return FALSE;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME);
	cs.style &= ~(WS_BORDER|WS_THICKFRAME);
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);
	return TRUE;
}

void CChildView::OnPaint() 
{
CPaintDC dcPaint( this );
CExtMemoryDC dc( &dcPaint );
CRect rcClient;
	GetClientRect( &rcClient );
	if( ! g_PaintManager->PaintDocumentClientAreaBkgnd( dc, this ) )
		dc.FillSolidRect(
			&rcClient,
			g_PaintManager->GetColor( COLOR_WINDOW, this )
			);
}


BOOL CChildView::OnEraseBkgnd( CDC * pDC ) 
{
	pDC;
	return TRUE;
}


