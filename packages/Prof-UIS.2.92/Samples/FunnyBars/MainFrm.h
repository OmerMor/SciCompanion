// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__B6A8C1DC_B08A_4F30_9030_B374176D2549__INCLUDED_)
#define AFX_MAINFRM_H__B6A8C1DC_B08A_4F30_9030_B374176D2549__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFunnyToolButton

class CFunnyToolButton : public CExtBarButton
{
	CString m_strFunnyText;
	CSize m_sizeFunnyTextCalc, m_sizeFunnyTextWell;
	CExtCmdIcon m_iconCold;
	double m_lfHotPercent;
public:

	DECLARE_DYNAMIC( CFunnyToolButton );

	CFunnyToolButton(
		CExtToolControlBar * pBar,
		UINT nCmdID,
		LPCTSTR strFunnyText,
		double lfHotPercent
		);
	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz
		);

	virtual void PaintCompound(
		CDC & dc,
		bool bPaintParentChain,
		bool bPaintChildren,
		bool bPaintOneNearestChildrenLevelOnly
		);

	void UpdateHotPercent(
		double lfHotPercent,
		bool bForceEmptyIconCache
		)
	{
		ASSERT_VALID( this );
		if( m_lfHotPercent == lfHotPercent )
		{
			if( bForceEmptyIconCache )
				m_iconCold.Empty();
			return;
		}
		m_lfHotPercent = lfHotPercent;
		m_iconCold.Empty();
	}
}; // class CFunnyToolButton

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

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

public:  // control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
	CExtMenuControlBar    m_wndMenuBar;
	CExtToolControlBar    m_wndToolBarStd;
	CExtToolControlBar    m_wndToolBar256;
	CExtToolControlBar    m_wndToolBarTrue;
	CExtToolControlBar    m_wndToolBarTruePlusText;
	CExtToolControlBar    m_wndToolBarTrueHot;
	CExtToolControlBar    m_wndToolBarVista;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	//}}AFX_MSG
	afx_msg void OnAnyCommand();
	afx_msg void OnUpdateAnyCommand(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B6A8C1DC_B08A_4F30_9030_B374176D2549__INCLUDED_)
