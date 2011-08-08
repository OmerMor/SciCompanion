#if !defined(AFX_MYRESIZABLEPAGE2_H__A3DDD6C7_3CD7_423B_84CF_9C3473E78847__INCLUDED_)
#define AFX_MYRESIZABLEPAGE2_H__A3DDD6C7_3CD7_423B_84CF_9C3473E78847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyResizablePage2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyResizablePage2 dialog

class CMyResizablePage2 : public CExtResizablePropertyPage
{
	DECLARE_DYNCREATE(CMyResizablePage2)

// Construction
public:
	CMyResizablePage2();
	~CMyResizablePage2();

// Dialog Data
	//{{AFX_DATA(CMyResizablePage2)
	enum { IDD = IDD_DIALOG2 };
	CExtCheckBox	m_Check2;
	CExtCheckBox	m_Check1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMyResizablePage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMyResizablePage2)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYRESIZABLEPAGE2_H__A3DDD6C7_3CD7_423B_84CF_9C3473E78847__INCLUDED_)
