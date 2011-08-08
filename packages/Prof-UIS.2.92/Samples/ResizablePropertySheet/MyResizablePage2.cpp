// MyResizablePage2.cpp : implementation file
//

#include "stdafx.h"
#include "ResizablePropertySheet.h"
#include "MyResizablePage2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyResizablePage2 property page

IMPLEMENT_DYNCREATE(CMyResizablePage2, CExtResizablePropertyPage)

CMyResizablePage2::CMyResizablePage2() : CExtResizablePropertyPage(CMyResizablePage2::IDD)
{
	//{{AFX_DATA_INIT(CMyResizablePage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyResizablePage2::~CMyResizablePage2()
{
}

void CMyResizablePage2::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyResizablePage2)
	DDX_Control(pDX, IDC_CHECK2, m_Check2);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyResizablePage2, CExtResizablePropertyPage)
	//{{AFX_MSG_MAP(CMyResizablePage2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyResizablePage2 message handlers


BOOL CMyResizablePage2::OnInitDialog() 
{
	CExtResizablePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddAnchor( IDC_DATETIMEPICKER1, __RDA_LT, __RDA_RT );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
