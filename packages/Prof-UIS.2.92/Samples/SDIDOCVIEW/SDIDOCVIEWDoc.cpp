// SDIDOCVIEWDoc.cpp : implementation of the CSDIDOCVIEWDoc class
//

#include "stdafx.h"
#include "SDIDOCVIEW.h"

#include "SDIDOCVIEWDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWDoc

IMPLEMENT_DYNCREATE(CSDIDOCVIEWDoc, CDocument)

BEGIN_MESSAGE_MAP(CSDIDOCVIEWDoc, CDocument)
	//{{AFX_MSG_MAP(CSDIDOCVIEWDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWDoc construction/destruction

CSDIDOCVIEWDoc::CSDIDOCVIEWDoc()
{
	// TODO: add one-time construction code here

}

CSDIDOCVIEWDoc::~CSDIDOCVIEWDoc()
{
}

BOOL CSDIDOCVIEWDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWDoc serialization

void CSDIDOCVIEWDoc::Serialize(CArchive& ar)
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
// CSDIDOCVIEWDoc diagnostics

#ifdef _DEBUG
void CSDIDOCVIEWDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSDIDOCVIEWDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSDIDOCVIEWDoc commands
