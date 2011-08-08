// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
#define AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildFormView.h"

class CColorizedThemeOffice2003;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:
	CExtStatusControlBar  m_wndStatusBar;
	CColorizedThemeOffice2003 *m_pPaintManager;

private:
	// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;

	CBitmap m_bmpTrackerCache;
	CBrush m_brushTracker;
	CRect m_rcTrackerEntireArea;

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
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CExtMenuControlBar    m_wndMenuBar;
	CExtToolControlBar    m_wndToolBar;
	CExtToolControlBar    m_wndToolBarSeek;
	CExtToolControlBar    m_wndToolBarZoom;
	CExtToolControlBar    m_wndToolBarIed;

	CExtControlBar    m_wndResizableBar0;
	CExtControlBar    m_wndResizableBar1;
	CExtControlBar    m_wndResizableBar2;

	CChildFormView		  *m_pWndView;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnDrawControlBarTrackedArea(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};

class CColorizedThemeOffice2003 : public CExtPaintManagerOffice2003
{
public:
	DECLARE_SERIAL( CColorizedThemeOffice2003 );

	void SetThemeColors(
		COLORREF clrFillHint,
		COLORREF clrAccentHint
		)
	{
		if(		m_clrFillHint == clrFillHint
			&&	m_clrAccentHint == clrAccentHint
			)
			return;

		m_clrFillHint = clrFillHint;
		m_clrAccentHint = clrAccentHint;
		InitTranslatedColors();
		InitHelperBrushes();

		CMainFrame* pMainFrame = (STATIC_DOWNCAST(CMainFrame,AfxGetMainWnd()));
		ASSERT( pMainFrame != NULL );
		ASSERT_VALID( pMainFrame );
		pMainFrame->RedrawWindow(
			NULL,
			NULL,
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
			| RDW_FRAME | RDW_ALLCHILDREN
			);
		CExtControlBar::stat_RedrawFloatingFrames( pMainFrame );
		CExtControlBar::stat_RecalcBarMetrics( pMainFrame );
	}

	COLORREF m_clrFillHint;
	COLORREF m_clrAccentHint;

	CColorizedThemeOffice2003()
		: m_clrFillHint( RGB(196,196,196) )
		, m_clrAccentHint( RGB(160, 180, 225) )
	{
	}

protected:
	virtual bool OnQueryThemeColors(
		COLORREF *pclrFillHint,
		COLORREF *pclrAccentHint = NULL
		)
	{
		if( pclrFillHint != NULL )
			*pclrFillHint = m_clrFillHint;
		if( pclrAccentHint != NULL )
			*pclrAccentHint = m_clrAccentHint;
		return true;
	}

	virtual bool OnQueryUseThemeColors() const
	{
		return true;
	}

	virtual e_system_theme_t OnQuerySystemTheme() const
	{
		return ThemeUnknown;
	}

	virtual bool OnStateSerialize(
		CArchive & ar,
		bool bEnableThrowExceptions = false
		)
	{
		CExtPaintManagerOffice2003::OnStateSerialize( ar, bEnableThrowExceptions );
		if( ar.IsStoring() )
		{
			ar << DWORD(m_clrFillHint);
			ar << DWORD(m_clrAccentHint);
		}
		else
		{
			DWORD dwTmp;
			ar >> dwTmp;
			m_clrFillHint = COLORREF(dwTmp);
			ar >> dwTmp;
			m_clrAccentHint = COLORREF(dwTmp);
		}
		return true;
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
