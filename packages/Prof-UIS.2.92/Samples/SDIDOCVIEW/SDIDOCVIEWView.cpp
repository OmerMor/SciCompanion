// SDIDOCVIEWView.cpp : implementation of the CSDIDOCVIEWView class
//

#include "stdafx.h"
#include "SDIDOCVIEW.h"

#include "SDIDOCVIEWDoc.h"
#include "SDIDOCVIEWView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWView

IMPLEMENT_DYNCREATE(CSDIDOCVIEWView, CView)

BEGIN_MESSAGE_MAP(CSDIDOCVIEWView, CView)
	//{{AFX_MSG_MAP(CSDIDOCVIEWView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWView construction/destruction

CSDIDOCVIEWView::CSDIDOCVIEWView()
{
	// TODO: add construction code here

}

CSDIDOCVIEWView::~CSDIDOCVIEWView()
{
}

BOOL CSDIDOCVIEWView::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CView::PreCreateWindow( cs ) )
		return FALSE;
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME);
	cs.lpszClass = ::AfxRegisterWndClass( 0, ::LoadCursor( NULL, IDC_ARROW ) );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWView drawing

void CSDIDOCVIEWView::OnDraw(CDC* pDC)
{
	g_PaintManager->PaintDocumentClientAreaBkgnd( *pDC, this );
}

/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWView printing

BOOL CSDIDOCVIEWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSDIDOCVIEWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSDIDOCVIEWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWView diagnostics

#ifdef _DEBUG
void CSDIDOCVIEWView::AssertValid() const
{
	CView::AssertValid();
}

void CSDIDOCVIEWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIDOCVIEWDoc* CSDIDOCVIEWView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIDOCVIEWDoc)));
	return (CSDIDOCVIEWDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWView message handlers

