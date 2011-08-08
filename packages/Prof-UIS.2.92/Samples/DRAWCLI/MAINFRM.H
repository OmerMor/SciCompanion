// mainfrm.h : interface of the CMainFrame class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

//#define __SAMPLE_APP_USES_STD_MENU_LINE

class CMainFrame
	: public CExtNCW < CMDIFrameWnd >
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	, public CExtCustomizeSite
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
{
	DECLARE_DYNAMIC( CMainFrame );

public:
	// in standard toolbar
#if (defined __EXT_MFC_NO_CUSTOMIZE)
	CExtBarColorButton * m_pBtnColorFillS, * m_pBtnColorOutlineS;
#endif // (defined __EXT_MFC_NO_CUSTOMIZE)
	// in palette toolbar
	CExtBarColorButton * m_pBtnColorFillP, * m_pBtnColorOutlineP;

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
			
#if (!defined __SAMPLE_APP_USES_STD_MENU_LINE)
			CMainFrame * pFrame = 
				static_cast < CMainFrame * > (
					CWnd::FromHandlePermanent( m_hMDIFrameWnd )
					);
			if ( ! pFrame->m_wndMenuBar.IsDisplayMdiDocumentButtons() )
			{
				m_btnCascade.ShowWindow(SW_HIDE);
				m_btnTileVert.ShowWindow(SW_HIDE);
				m_btnTileHorz.ShowWindow(SW_HIDE);
				m_btnMinimize.ShowWindow(SW_HIDE);
			}
			else
			{
#endif // (!defined __SAMPLE_APP_USES_STD_MENU_LINE)
				m_btnCascade.ShowWindow(SW_SHOW);
				m_btnTileVert.ShowWindow(SW_SHOW);
				m_btnTileHorz.ShowWindow(SW_SHOW);
				m_btnMinimize.ShowWindow(SW_SHOW);
#if (!defined __SAMPLE_APP_USES_STD_MENU_LINE)
			}
#endif // (!defined __SAMPLE_APP_USES_STD_MENU_LINE)
			//m_btnSave.ShowWindow(SW_HIDE);
		}
	}; // class CMyMdiWindowsListDlg : public CExtMdiWindowsListDlg

#endif // #ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	

// Attributes
public:

	// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;

// Operations
public:

// Implementation
public:
	virtual ~CMainFrame();
	virtual void RecalcLayout(BOOL bNotify = TRUE);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	// control bar embedded members
#if (!defined __SAMPLE_APP_USES_STD_MENU_LINE)
	CExtMenuControlBar    m_wndMenuBar;
#endif // (!defined __SAMPLE_APP_USES_STD_MENU_LINE)
	CExtStatusControlBar	m_wndStatusBar;
	CExtToolControlBar		m_wndToolBarStandard;
	CExtToolControlBar		m_wndToolBarCustom;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;
	CExtToolControlBar		m_wndPalette;

#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	CExtTabMdiOneNoteWnd	m_wndMdiTabs;
//	CExtTabMdiWhidbeyWnd	m_wndMdiTabs;
//	CExtTabMdiFlatWnd		m_wndMdiTabs;
//	CExtTabMdiButtonsWnd	m_wndMdiTabs;
//	CExtTabMdiWnd			m_wndMdiTabs;
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual bool OnColorItemValueGet(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		COLORREF & clr,
		bool bSelected
		);
	virtual bool OnColorItemValueSet(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		COLORREF & clr,
		bool bSelected
		);
	virtual CExtBarButton * OnCreateToolbarButton(
		CExtToolControlBar * pBar,
		CExtCustomizeCmdTreeNode * pNodeI,
		CExtCustomizeCmdTreeNode * pNodeC
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL
	
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMditabsTop();
	afx_msg void OnUpdateMditabsTop(CCmdUI* pCmdUI);
	afx_msg void OnMditabsBottom();
	afx_msg void OnUpdateMditabsBottom(CCmdUI* pCmdUI);
	afx_msg void OnMditabsLeft();
	afx_msg void OnUpdateMditabsLeft(CCmdUI* pCmdUI);
	afx_msg void OnMditabsRight();
	afx_msg void OnUpdateMditabsRight(CCmdUI* pCmdUI);
	afx_msg void OnMditabsShowClose();
	afx_msg void OnUpdateMditabsShowClose(CCmdUI* pCmdUI);
	afx_msg void OnMditabsShowCtxHelp();
	afx_msg void OnUpdateMditabsShowCtxHelp(CCmdUI* pCmdUI);
	afx_msg void OnContextHelp();
	afx_msg void OnUpdateContextHelp(CCmdUI* pCmdUI);
	afx_msg void OnCustomize();
	afx_msg void OnUpdateCustomize(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
/*
	afx_msg LRESULT OnMsgTranslateMouseClickEvent(WPARAM wParam, LPARAM lParam);
*/
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDrawInPlaceFrame construction/destruction

class CDrawInPlaceFrame : public COleIPFrameWnd
{
	DECLARE_DYNCREATE( CDrawInPlaceFrame );
public:
	CDrawInPlaceFrame();

// Attributes
public:
	CExtSafeString m_strSerializableProfileName, m_strCmdProfileName;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawInPlaceFrame)
	public:
	virtual BOOL OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawInPlaceFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	COleDropTarget	m_dropTarget;
	COleResizeBar   m_wndResizeBar;

public:
	// in standard toolbar
	CExtBarColorButton * m_pBtnColorFillS, * m_pBtnColorOutlineS;
	// in palette toolbar
	CExtBarColorButton * m_pBtnColorFillP, * m_pBtnColorOutlineP;

	// control bar embedded members
	CExtToolControlBar		m_wndToolBarStandard;
	CExtToolControlBar		m_wndToolBarCustom;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;
	CExtToolControlBar		m_wndPalette;

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawInPlaceFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
