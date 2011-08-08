#if !defined(AFX_PAGENOTAVAILABLE_H__DA7E6031_FB21_4EAA_835A_E91E1B35A377__INCLUDED_)
#define AFX_PAGENOTAVAILABLE_H__DA7E6031_FB21_4EAA_835A_E91E1B35A377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageNotAvailable.h : header file
//
#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageNotAvailable dialog

class CPageNotAvailable : public CPageBase
{
// Construction
public:
	CPageNotAvailable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageNotAvailable)
	enum { IDD = IDD_PAGE_NOT_AVAILABLE };
	CExtLabel	m_wndLabelNotAvailable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageNotAvailable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageNotAvailable)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGENOTAVAILABLE_H__DA7E6031_FB21_4EAA_835A_E91E1B35A377__INCLUDED_)
