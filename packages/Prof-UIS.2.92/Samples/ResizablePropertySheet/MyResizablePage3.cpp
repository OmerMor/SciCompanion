// MyResizablePage3.cpp : implementation file
//

#include "stdafx.h"
#include "ResizablePropertySheet.h"
#include "MyResizablePage3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyResizablePage3 property page

IMPLEMENT_DYNCREATE(CMyResizablePage3, CExtResizablePropertyPage)

CMyResizablePage3::CMyResizablePage3() : CExtResizablePropertyPage(CMyResizablePage3::IDD)
{
	//{{AFX_DATA_INIT(CMyResizablePage3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyResizablePage3::~CMyResizablePage3()
{
}

void CMyResizablePage3::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyResizablePage3)
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_wndMonthCalCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyResizablePage3, CExtResizablePropertyPage)
	//{{AFX_MSG_MAP(CMyResizablePage3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyResizablePage3 message handlers


BOOL CMyResizablePage3::OnInitDialog() 
{
	CExtResizablePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddAnchor( IDC_MONTHCALENDAR1, __RDA_LT, __RDA_RB );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMyResizablePage3::OnSetActive() 
{
BOOL bRetVal = CExtResizablePropertyPage::OnSetActive();
	if( bRetVal )
	{
		CExtResizablePropertySheet * pSheet =
			STATIC_DOWNCAST(
				CExtResizablePropertySheet,
				GetParent()
				);
		if( pSheet->IsWizard() )
			pSheet->SetWizardButtons( PSWIZB_BACK|PSWIZB_FINISH );
	} // if( bRetVal )
	return bRetVal;
}

BOOL CMyResizablePage3::OnKillActive() 
{
BOOL bRetVal = CExtResizablePropertyPage::OnKillActive();
	if( bRetVal )
	{
		CExtResizablePropertySheet * pSheet =
			STATIC_DOWNCAST(
				CExtResizablePropertySheet,
				GetParent()
				);
		if( pSheet->IsWizard() )
			pSheet->SetWizardButtons( PSWIZB_BACK|PSWIZB_NEXT );
	} // if( bRetVal )
	return bRetVal;
}
