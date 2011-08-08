// MDIDOCVIEWDoc.cpp : implementation of the CMDIDOCVIEWDoc class
//

#include "stdafx.h"
#include "MDIDOCVIEW.h"

#include "MDIDOCVIEWDoc.h"

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
