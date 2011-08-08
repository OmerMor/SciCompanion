// MDI_DynamicBarsView.cpp : implementation of the CMDI_DynamicBarsView class
//

#include "stdafx.h"
#include "MDI_DynamicBars.h"

#include "MDI_DynamicBarsDoc.h"
#include "MDI_DynamicBarsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsView

IMPLEMENT_DYNCREATE(CMDI_DynamicBarsView, CView)

BEGIN_MESSAGE_MAP(CMDI_DynamicBarsView, CView)
	//{{AFX_MSG_MAP(CMDI_DynamicBarsView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsView construction/destruction

CMDI_DynamicBarsView::CMDI_DynamicBarsView()
{
	// TODO: add construction code here

}

CMDI_DynamicBarsView::~CMDI_DynamicBarsView()
{
}

BOOL CMDI_DynamicBarsView::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CView::PreCreateWindow( cs ) )
		return FALSE;
	cs.lpszClass = ::AfxRegisterWndClass( 0, ::LoadCursor( NULL, IDC_ARROW ) );
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsView drawing

void CMDI_DynamicBarsView::OnDraw(CDC* pDC)
{
	g_PaintManager->PaintDocumentClientAreaBkgnd( *pDC, this );
}

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsView printing

BOOL CMDI_DynamicBarsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMDI_DynamicBarsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMDI_DynamicBarsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsView diagnostics

#ifdef _DEBUG
void CMDI_DynamicBarsView::AssertValid() const
{
	CView::AssertValid();
}

void CMDI_DynamicBarsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDI_DynamicBarsDoc* CMDI_DynamicBarsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDI_DynamicBarsDoc)));
	return (CMDI_DynamicBarsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsView message handlers
