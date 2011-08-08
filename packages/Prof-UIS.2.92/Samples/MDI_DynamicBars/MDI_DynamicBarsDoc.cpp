// MDI_DynamicBarsDoc.cpp : implementation of the CMDI_DynamicBarsDoc class
//

#include "stdafx.h"
#include "MDI_DynamicBars.h"

#include "MDI_DynamicBarsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsDoc

IMPLEMENT_DYNCREATE(CMDI_DynamicBarsDoc, CDocument)

BEGIN_MESSAGE_MAP(CMDI_DynamicBarsDoc, CDocument)
	//{{AFX_MSG_MAP(CMDI_DynamicBarsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsDoc construction/destruction

CMDI_DynamicBarsDoc::CMDI_DynamicBarsDoc()
{
	// TODO: add one-time construction code here

}

CMDI_DynamicBarsDoc::~CMDI_DynamicBarsDoc()
{
}

BOOL CMDI_DynamicBarsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsDoc serialization

void CMDI_DynamicBarsDoc::Serialize(CArchive& ar)
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
// CMDI_DynamicBarsDoc diagnostics

#ifdef _DEBUG
void CMDI_DynamicBarsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMDI_DynamicBarsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsDoc commands
