// rectdlg.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include "drawcli.h"
#include "rectdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define __LOCALMSG_InternalUpdateControls (WM_USER+333)

/////////////////////////////////////////////////////////////////////////////
// CRectDlg dialog

COLORREF CRectDlg::g_clrTransparent = COLORREF(-1);

CRectDlg::CRectDlg(CWnd* /*pParent = NULL*/)
	: CPropertyPage(CRectDlg::IDD)
{
	//{{AFX_DATA_INIT(CRectDlg)
	m_bNoFill = FALSE;
	m_nPenSize = 0;
	//}}AFX_DATA_INIT
	m_clrFill = m_clrOutline = g_clrTransparent;
}

void CRectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRectDlg)
	DDX_Control(pDX, IDC_SPIN1, m_SpinCtrl);
	DDX_Check(pDX, IDC_NOFILL, m_bNoFill);
	DDX_Text(pDX, IDC_WEIGHT, m_nPenSize);
	DDV_MinMaxUInt(pDX, m_nPenSize, 0, 100);
	DDX_Control(pDX, IDC_BUTTON_COLOR_OUTLINE, m_BtnColorOutline);
	DDX_Control(pDX, IDC_BUTTON_COLOR_FILL, m_BtnColorFill);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRectDlg, CDialog)
	//{{AFX_MSG_MAP(CRectDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_NOFILL, _UpdateControls)
	ON_EN_CHANGE(IDC_WEIGHT, _UpdateControls)
	ON_MESSAGE(
		__LOCALMSG_InternalUpdateControls,
		OnInternalUpdateControls
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRectDlg message handlers

BOOL CRectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SpinCtrl.SetRange(0, 100);
	m_SpinCtrl.SetBase(10);
	m_SpinCtrl.SetPos( /*1*/ m_nPenSize );

	m_BtnColorOutline.m_bEnableBtnColorDefault
		= m_BtnColorFill.m_bEnableBtnColorDefault
		= false;
	m_BtnColorOutline.m_clrDefault
		= m_BtnColorOutline.m_clrSelected
		= m_clrOutline;
	m_BtnColorFill.m_clrDefault
		= m_BtnColorFill.m_clrSelected
		= m_clrFill;

	PostMessage( __LOCALMSG_InternalUpdateControls );

	return TRUE;
}

LRESULT CRectDlg::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;
	m_clrFill = m_BtnColorFill.m_clrSelected;
	m_clrOutline = m_BtnColorOutline.m_clrSelected;
	PostMessage( __LOCALMSG_InternalUpdateControls );
	return 0;
}

LRESULT CRectDlg::OnInternalUpdateControls(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;
	UpdateData();
	if( m_BtnColorOutline.GetSafeHwnd() != NULL )
		m_BtnColorOutline.EnableWindow( m_nPenSize != 0 );
	if( m_BtnColorFill.GetSafeHwnd() != NULL )
		m_BtnColorFill.EnableWindow( !m_bNoFill );
	return 0;
}

void CRectDlg::_UpdateControls()
{
	PostMessage( __LOCALMSG_InternalUpdateControls );
}
