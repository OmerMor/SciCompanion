// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__205C43B5_15CF_429F_8CA0_07C395066CE5__INCLUDED_)
#define AFX_MAINFRM_H__205C43B5_15CF_429F_8CA0_07C395066CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#if !defined(__EXT_TEMPL_H)
	#include <ExtTempl.h>
#endif

#include <../Src/ExtHook.h>
#include <../Src/ExtControlBarTabbedFeatures.h>

class CMainFrame : public CExtNCW < CMDIFrameWnd >
{
	DECLARE_DYNAMIC( CMainFrame );
public:
	CMainFrame();

// Attributes
protected:
	WINDOWPLACEMENT m_dataFrameWP;

public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


public:
	CExtMenuControlBar    m_wndMenuBar;
protected:  // control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
	CExtToolControlBar    m_wndToolBar;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;
	CExtControlBar m_wndResizableBarEmpty;
	CExtControlBar m_wndResizableBarTree;
	
	CExtNCSB < CExtWFF < CTreeCtrl > > m_wndDockedCtrlTree;
	CExtControlBar m_wndResizableBarEdit;
	CExtNCSB < CExtEdit > m_wndDockedCtrlEdit;
	CExtControlBar m_wndResizableBarCP;
	CExtWRB< CExtColorCtrl > m_wndDockedCtrlCP;
	CExtControlBar m_wndResizableBarDlg;
	CExtWRB< CExtResizableDialog > m_wndDockedResizableDialog;
	CExtLabel m_wndLabelInResizableDialog;
	CExtWRB< CExtEdit > m_wndEditInDockedDlg;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__205C43B5_15CF_429F_8CA0_07C395066CE5__INCLUDED_)
