// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1D26E132_198E_422B_85A7_A247E226CF8D__INCLUDED_)
#define AFX_MAINFRM_H__1D26E132_198E_422B_85A7_A247E226CF8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPage1.h"
#include "MyPage2.h"
#include "MyPage3.h"
#include "ExtChildResizablePropertySheet.h"

class CMainFrame : public CExtNCW < CMDIFrameWnd >
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

#ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
	
	class CMyMdiWindowsListDlg : public CExtMdiWindowsListDlg
	{
	public:
		CMyMdiWindowsListDlg(
				CMDIFrameWnd * pMDIFrameWnd,
				CWnd * pParent = NULL
				)
			: CExtMdiWindowsListDlg(
				pMDIFrameWnd,
				pParent
				)
		{
		}
		CMyMdiWindowsListDlg(
				HWND hMDIFrameWnd,
				CWnd * pParent = NULL
				)
			: CExtMdiWindowsListDlg(
				hMDIFrameWnd,
				pParent
				)
		{
		}
	protected:
		virtual void OnUpdateButtons()
		{
			CExtMdiWindowsListDlg::OnUpdateButtons();
			m_btnSave.ShowWindow(SW_HIDE);
		}
	}; // class CMyMdiWindowsListDlg : public CExtMdiWindowsListDlg
	
#endif // #ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
	
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

	CExtMenuControlBar    m_wndMenuBar;
	CExtThemeSwitcherToolControlBar  m_wndToolBarUiLook;
protected:  // control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
	CExtToolControlBar    m_wndToolBar;
	CExtControlBar m_wndResizableBarPS1, m_wndResizableBarPS2, m_wndResizableBarPS3, m_wndResizableBarPS4;
	CExtChildResizablePropertySheet m_wndDockedPS1, m_wndDockedPS2, m_wndDockedPS3, m_wndDockedPS4;
	CMyPage1 m_wndPage1_PS1, m_wndPage1_PS2, m_wndPage1_PS3, m_wndPage1_PS4;
	CMyPage2 m_wndPage2_PS1, m_wndPage2_PS2, m_wndPage2_PS3, m_wndPage2_PS4;
	CMyPage3 m_wndPage3_PS1, m_wndPage3_PS2, m_wndPage3_PS3, m_wndPage3_PS4;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1D26E132_198E_422B_85A7_A247E226CF8D__INCLUDED_)
