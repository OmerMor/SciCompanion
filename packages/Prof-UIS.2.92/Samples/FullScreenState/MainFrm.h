// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F714E2DF_477E_45FB_99F4_0BF9B1494D80__INCLUDED_)
#define AFX_MAINFRM_H__F714E2DF_477E_45FB_99F4_0BF9B1494D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CExtNCW < CMDIFrameWnd >
{
	DECLARE_DYNAMIC(CMainFrame);
	CBitmap m_bmpTrackerCache;
	CBrush m_brushTracker;
	CRect m_rcTrackerEntireArea;
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
public:

private:
	// window placement persistence
	WINDOWPLACEMENT m_wpInitial;
	// saved window placement before going full-screen mode
	WINDOWPLACEMENT m_wpSavedBeforeFullScreen;
	// persistent size for OnGetMinMaxInfo() in full-screen mode
	CSize m_sizeFullScreenDesktopDifference;

	bool _DoBarStateSerialization(
		bool bFullScreenState,
		bool bSaveState,
		bool bInitialLoad = false
		);
	void _DoInitialDock(
		bool bFullScreenState
		);
	void _DoViewFullScreen( bool bSavePrevLayout );
	void _DoFullScreenGetMinMaxInfo( LPMINMAXINFO lpMMI );

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
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
public: // for child frame
	CExtMenuControlBar    m_wndMenuBar;
protected:
	CExtToolControlBar    m_wndToolBar;
	CExtToolControlBar    m_wndToolBar1;
	CExtThemeSwitcherToolControlBar m_wndToolBar2;
	CExtToolControlBar    m_wndToolBar3;
	CExtControlBar    m_wndResizableBar0;
	CExtControlBar    m_wndResizableBar1;
	CExtControlBar    m_wndResizableBar2;
	CExtControlBar    m_wndResizableBar3;
	CExtControlBar    m_wndResizableBar4;
	CExtControlBar    m_wndResizableBar5;
#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	CExtTabMdiWnd m_wndMdiTabs;
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewFullScreen();
	afx_msg void OnUpdateViewFullScreen(CCmdUI* pCmdUI);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawControlBarTrackedArea(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F714E2DF_477E_45FB_99F4_0BF9B1494D80__INCLUDED_)
