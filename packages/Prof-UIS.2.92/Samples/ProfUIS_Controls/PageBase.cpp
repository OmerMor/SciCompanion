// PageBase.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageBase.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageBase dialog

IMPLEMENT_DYNAMIC( CPageBase, CExtResizableDialog );

CPageBase::CPageBase(UINT _IDD, CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(_IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPageBase::DoDataExchange(CDataExchange* pDX)
{
	CExtResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPageBase, CExtResizableDialog)
	//{{AFX_MSG_MAP(CPageBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPageBase::PageBase_PostInit()
{
	ASSERT_VALID( this );
}

/////////////////////////////////////////////////////////////////////////////
// CPageBase message handlers

BOOL CPageBase::OnInitDialog() 
{
	CExtResizableDialog::OnInitDialog();
	ShowSizeGrip( FALSE );
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CPageBase::PostNcDestroy() 
{
	CExtResizableDialog::PostNcDestroy();
	delete this;	
}
//////////////////////////////////////////////////////////////////////////
