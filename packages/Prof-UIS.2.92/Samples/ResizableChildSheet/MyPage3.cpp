// MyPage3.cpp : implementation file
//

#include "stdafx.h"
#include "ResizableChildSheet.h"
#include "MyPage3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPage3 property page

IMPLEMENT_DYNCREATE(CMyPage3, CExtResizablePropertyPage)

CMyPage3::CMyPage3() : CExtResizablePropertyPage(CMyPage3::IDD)
{
	//{{AFX_DATA_INIT(CMyPage3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyPage3::~CMyPage3()
{
}

void CMyPage3::DoDataExchange(CDataExchange* pDX)
{
	CExtResizablePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPage3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPage3, CExtResizablePropertyPage)
	//{{AFX_MSG_MAP(CMyPage3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPage3 message handlers

BOOL CMyPage3::OnInitDialog() 
{
	CExtResizablePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddAnchor( IDC_MONTHCALENDAR1, __RDA_LT, __RDA_RB );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
