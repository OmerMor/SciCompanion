// MyPage2.cpp : implementation file
//

#include "stdafx.h"
#include "ResizableChildSheet.h"
#include "MyPage2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPage2 property page

IMPLEMENT_DYNCREATE(CMyPage2, CExtResizablePropertyPage)

CMyPage2::CMyPage2() : CExtResizablePropertyPage(CMyPage2::IDD)
{
	//{{AFX_DATA_INIT(CMyPage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyPage2::~CMyPage2()
{
}

void CMyPage2::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPage2)
	DDX_Control(pDX, IDC_CHECK3, m_Check3);
	DDX_Control(pDX, IDC_CHECK2, m_Check2);
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPage2, CExtResizablePropertyPage)
	//{{AFX_MSG_MAP(CMyPage2)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDC_BUTTON1, OnButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPage2 message handlers

BOOL CMyPage2::OnInitDialog() 
{
	CExtResizablePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddAnchor( IDC_CHECK1, __RDA_RT );
	AddAnchor( IDC_CHECK2, __RDA_RT );
	AddAnchor( IDC_CHECK3, __RDA_RT );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyPage2::OnOK()
{
	::AfxMessageBox( _T("CMyPage2::OnOK()") );
}

void CMyPage2::OnCancel()
{
	::AfxMessageBox( _T("CMyPage2::OnCancel()") );
}

void CMyPage2::OnButton1()
{
	::AfxMessageBox( _T("CMyPage2::OnButton1()") );
}
