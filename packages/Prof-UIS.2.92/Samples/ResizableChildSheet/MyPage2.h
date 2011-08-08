#if !defined(AFX_MYPAGE2_H__6F4C8DFB_CC25_4D26_B59E_E1A44DE7F276__INCLUDED_)
#define AFX_MYPAGE2_H__6F4C8DFB_CC25_4D26_B59E_E1A44DE7F276__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyPage2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyPage2 dialog

/////////////////////////////////////////////////////////////////////////////
// CMyPage2 dialog

class CMyPage2 : public CExtResizablePropertyPage
{
	DECLARE_DYNCREATE(CMyPage2)

// Construction
public:
	CMyPage2();
	~CMyPage2();

// Dialog Data
	//{{AFX_DATA(CMyPage2)
	enum { IDD = IDD_DIALOG_PAGE2 };
	CExtCheckBox	m_Check3;
	CExtCheckBox	m_Check2;
	CExtCheckBox	m_Check1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMyPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMyPage2)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButton1();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPAGE2_H__6F4C8DFB_CC25_4D26_B59E_E1A44DE7F276__INCLUDED_)
