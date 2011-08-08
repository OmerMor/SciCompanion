// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
#define AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"

#if (!defined __EXT_TEMPL_H)
	#include <ExtTempl.h>
#endif

#include "ExtPopupRichEditCtrl.h"

class CMainFrame : public CExtNCW < CFrameWnd >
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:
	CExtPopupRichEditView m_wndPopupRichEditView;

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

protected:  // control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
	CExtMenuControlBar    m_wndMenuBar;
	CExtToolControlBar    m_wndToolBar;
	CExtThemeSwitcherToolControlBar  m_wndToolBarUiLook;
	CChildView *          m_pWndView;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnItemCoveringNotification( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnPrepareMenu( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnAnyDeactivatePopup( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
