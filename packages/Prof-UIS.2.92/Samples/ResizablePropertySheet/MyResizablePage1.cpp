// MyResizablePage1.cpp : implementation file
//

#include "stdafx.h"
#include "ResizablePropertySheet.h"
#include "MyResizablePage1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyResizablePage1 property page

IMPLEMENT_DYNCREATE(CMyResizablePage1, CExtResizablePropertyPage)

CMyResizablePage1::CMyResizablePage1()
	: CExtResizablePropertyPage( CMyResizablePage1::IDD )
{
	//{{AFX_DATA_INIT(CMyResizablePage1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyResizablePage1::~CMyResizablePage1()
{
}

void CMyResizablePage1::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyResizablePage1)
	DDX_Control(pDX, IDC_EDIT1, m_wndEdit);
	DDX_Control(pDX, IDC_COMBO1, m_wndCombo);
	DDX_Control(pDX, IDC_BUTTON1, m_wndButton1);
	DDX_Control(pDX, IDC_BUTTON2, m_wndButton2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyResizablePage1, CExtResizablePropertyPage)
	//{{AFX_MSG_MAP(CMyResizablePage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyResizablePage1 message handlers


BOOL CMyResizablePage1::OnInitDialog() 
{
	CExtResizablePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddAnchor( IDC_COMBO1, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_EDIT1, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_BUTTON1, __RDA_RB );
	AddAnchor( IDC_BUTTON2, __RDA_RB );
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMyResizablePage1::OnSetActive() 
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
			pSheet->SetWizardButtons( PSWIZB_NEXT );
	} // if( bRetVal )
	return bRetVal;
}

BOOL CMyResizablePage1::OnKillActive() 
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
			pSheet->SetWizardButtons( PSWIZB_NEXT|PSWIZB_BACK );
	} // if( bRetVal )
	return bRetVal;
}
