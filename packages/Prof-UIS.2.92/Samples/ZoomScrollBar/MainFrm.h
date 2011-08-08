// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
#define AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"

#if !defined(__EXT_TEMPL_H)
	#include <ExtTempl.h>
#endif

class CMainFrame : public CExtNCW < CFrameWnd >
{
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
	virtual void RecalcLayout(BOOL bNotify = TRUE);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	class CMyStatusBar : public CExtStatusControlBar
	{
	protected:
		virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
		{
			if( message == WM_HSCROLL || message == WM_VSCROLL )
				return GetParent()->GetDlgItem(AFX_IDW_PANE_FIRST)->SendMessage( message, wParam, lParam );
			return CExtStatusControlBar::WindowProc( message, wParam, lParam );
		}
	}; // class CMyStatusBar
	CMyStatusBar                      m_wndStatusBar;
	CExtMenuControlBar                m_wndMenuBar;
	CExtThemeSwitcherToolControlBar   m_wndToolBarUiLook;
	CChildView                        m_wndView;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
