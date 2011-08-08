#if !defined(AFX_MYRESIZABLEPAGE3_H__97AABDE1_2035_4ADF_889B_309F1E68E999__INCLUDED_)
#define AFX_MYRESIZABLEPAGE3_H__97AABDE1_2035_4ADF_889B_309F1E68E999__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyResizablePage3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyResizablePage3 dialog

class CMyResizablePage3 : public CExtResizablePropertyPage
{
	DECLARE_DYNCREATE(CMyResizablePage3)

// Construction
public:
	CMyResizablePage3();
	~CMyResizablePage3();

// Dialog Data
	//{{AFX_DATA(CMyResizablePage3)
	enum { IDD = IDD_DIALOG3 };
	//}}AFX_DATA

	CExtWFF< CMonthCalCtrl >	m_wndMonthCalCtrl;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMyResizablePage3)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMyResizablePage3)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYRESIZABLEPAGE3_H__97AABDE1_2035_4ADF_889B_309F1E68E999__INCLUDED_)
