// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "RibbonBarMDI.h"
#include "ChildView.h"
#include "MainFrm.h"

#if (!defined __RibbonIcons_H)
	#include "res/RibbonIcons.h"
#endif // (!defined __RibbonIcons_H)

#if !defined(__EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWDoc

IMPLEMENT_DYNCREATE(CMDIDOCVIEWDoc, CDocument)

BEGIN_MESSAGE_MAP(CMDIDOCVIEWDoc, CDocument)
	//{{AFX_MSG_MAP(CMDIDOCVIEWDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWDoc construction/destruction

CMDIDOCVIEWDoc::CMDIDOCVIEWDoc()
{
	// TODO: add one-time construction code here

}

CMDIDOCVIEWDoc::~CMDIDOCVIEWDoc()
{
}

BOOL CMDIDOCVIEWDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWDoc serialization

void CMDIDOCVIEWDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWDoc diagnostics

#ifdef _DEBUG
void CMDIDOCVIEWDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMDIDOCVIEWDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWDoc commands




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

