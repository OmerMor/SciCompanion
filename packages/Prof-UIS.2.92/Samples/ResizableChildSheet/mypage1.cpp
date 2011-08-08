// mypage1.cpp : implementation file
//

#include "stdafx.h"
#include "ResizableChildSheet.h"
#include "mypage1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPage1 property page

IMPLEMENT_DYNCREATE(CMyPage1, CExtResizablePropertyPage)

CMyPage1::CMyPage1() : CExtResizablePropertyPage(CMyPage1::IDD)
{
	//{{AFX_DATA_INIT(CMyPage1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyPage1::~CMyPage1()
{
}

void CMyPage1::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPage1)
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPage1, CExtResizablePropertyPage)
	//{{AFX_MSG_MAP(CMyPage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPage1 message handlers

BOOL CMyPage1::OnInitDialog() 
{
	CExtResizablePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddAnchor( IDC_EDIT1, __RDA_LT, __RDA_RT );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

