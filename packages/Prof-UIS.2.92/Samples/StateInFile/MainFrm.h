// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__B6A8C1DC_B08A_4F30_9030_B374176D2549__INCLUDED_)
#define AFX_MAINFRM_H__B6A8C1DC_B08A_4F30_9030_B374176D2549__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"

class CMainFrame : public CExtNCW < CFrameWnd >
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC( CMainFrame );

// Attributes
public:

private:
	// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
	CExtMenuControlBar    m_wndMenuBar;
	CExtToolControlBar    m_wndToolBar;
	CExtThemeSwitcherToolControlBar  m_wndToolBarUiLook;
	CExtControlBar    m_wndResizableBar0;
	CExtControlBar    m_wndResizableBar1;
	CChildView    m_wndView;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	//}}AFX_MSG
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B6A8C1DC_B08A_4F30_9030_B374176D2549__INCLUDED_)
