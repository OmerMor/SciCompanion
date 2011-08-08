// MDIDOCVIEWView.cpp : implementation of the CMDIDOCVIEWView class
//

#include "stdafx.h"
#include "MDIDOCVIEW.h"

#include "MDIDOCVIEWDoc.h"
#include "MDIDOCVIEWView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWView

IMPLEMENT_DYNCREATE(CMDIDOCVIEWView, CView)

BEGIN_MESSAGE_MAP(CMDIDOCVIEWView, CView)
	//{{AFX_MSG_MAP(CMDIDOCVIEWView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWView construction/destruction

CMDIDOCVIEWView::CMDIDOCVIEWView()
{
	// TODO: add construction code here

}

CMDIDOCVIEWView::~CMDIDOCVIEWView()
{
}

BOOL CMDIDOCVIEWView::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CView::PreCreateWindow( cs ) )
		return FALSE;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME);
	cs.lpszClass = ::AfxRegisterWndClass( 0 );
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWView drawing

void CMDIDOCVIEWView::OnDraw(CDC* pDC)
{
	g_PaintManager->PaintDocumentClientAreaBkgnd( *pDC, this );
}

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWView printing

BOOL CMDIDOCVIEWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMDIDOCVIEWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMDIDOCVIEWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWView diagnostics

#ifdef _DEBUG
void CMDIDOCVIEWView::AssertValid() const
{
	CView::AssertValid();
}

void CMDIDOCVIEWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDIDOCVIEWDoc* CMDIDOCVIEWView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDIDOCVIEWDoc)));
	return (CMDIDOCVIEWDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWView message handlers
