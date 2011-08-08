#if !defined(AFX_MYPAGE1_H__AC5B07D7_D138_4AC7_B0F9_400DE1F4FCA0__INCLUDED_)
#define AFX_MYPAGE1_H__AC5B07D7_D138_4AC7_B0F9_400DE1F4FCA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mypage1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyPage1 dialog

class CMyPage1 : public CExtResizablePropertyPage
{
	DECLARE_DYNCREATE(CMyPage1)

// Construction
public:
	CMyPage1();
	~CMyPage1();

// Dialog Data
	//{{AFX_DATA(CMyPage1)
	enum { IDD = IDD_DIALOG_PAGE1 };
	CExtLabel	m_Static1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMyPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMyPage1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPAGE1_H__AC5B07D7_D138_4AC7_B0F9_400DE1F4FCA0__INCLUDED_)
