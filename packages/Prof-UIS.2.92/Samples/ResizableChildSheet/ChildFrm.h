// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__B6B0AE2D_15EA_42DF_BE9F_A0F677C63844__INCLUDED_)
#define AFX_CHILDFRM_H__B6B0AE2D_15EA_42DF_BE9F_A0F677C63844__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPage1.h"
#include "MyPage2.h"
#include "MyPage3.h"
#include "ExtChildResizablePropertySheet.h"

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

class CChildFrame : public CExtNCW < CMDIChildWnd >
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	// view for the client area of the frame.
	CMyPage1 m_wndPage1;
	CMyPage2 m_wndPage2;
	CMyPage3 m_wndPage3;
	CExtChildResizablePropertySheet m_wndView;
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__B6B0AE2D_15EA_42DF_BE9F_A0F677C63844__INCLUDED_)
