#if !defined(AFX_MYRESIZABLEPAGE1_H__1FE6E563_7E52_48BD_94D1_8596C231A61B__INCLUDED_)
#define AFX_MYRESIZABLEPAGE1_H__1FE6E563_7E52_48BD_94D1_8596C231A61B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyResizablePage1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyResizablePage1 dialog

class CMyResizablePage1 : public CExtResizablePropertyPage
{
	DECLARE_DYNCREATE(CMyResizablePage1)

// Construction
public:
	CMyResizablePage1();
	~CMyResizablePage1();

// Dialog Data
	//{{AFX_DATA(CMyResizablePage1)
	enum { IDD = IDD_DIALOG1 };
	CExtEdit	m_wndEdit;
	CExtComboBox	m_wndCombo;
	CExtButton	m_wndButton1;
	CExtButton	m_wndButton2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMyResizablePage1)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMyResizablePage1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYRESIZABLEPAGE1_H__1FE6E563_7E52_48BD_94D1_8596C231A61B__INCLUDED_)
