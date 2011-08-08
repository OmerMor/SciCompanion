#if !defined(AFX_MYPAGE3_H__99F16960_A788_4CA7_9CDB_168C03337647__INCLUDED_)
#define AFX_MYPAGE3_H__99F16960_A788_4CA7_9CDB_168C03337647__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyPage3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyPage3 dialog

class CMyPage3 : public CExtResizablePropertyPage
{
	DECLARE_DYNCREATE(CMyPage3)

// Construction
public:
	CMyPage3();
	~CMyPage3();

// Dialog Data
	//{{AFX_DATA(CMyPage3)
	enum { IDD = IDD_DIALOG_PAGE3 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMyPage3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMyPage3)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPAGE3_H__99F16960_A788_4CA7_9CDB_168C03337647__INCLUDED_)
