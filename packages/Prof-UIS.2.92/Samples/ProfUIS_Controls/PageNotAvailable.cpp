// PageNotAvailable.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageNotAvailable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageNotAvailable dialog


CPageNotAvailable::CPageNotAvailable(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageNotAvailable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageNotAvailable)
	//}}AFX_DATA_INIT
}


void CPageNotAvailable::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageNotAvailable)
	DDX_Control(pDX, IDC_STATIC_NOT_AVAILABLE, m_wndLabelNotAvailable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageNotAvailable, CPageBase)
	//{{AFX_MSG_MAP(CPageNotAvailable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageNotAvailable message handlers

BOOL CPageNotAvailable::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	return TRUE;
}
