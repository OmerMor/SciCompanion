// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__3C598774_58D7_4703_A198_95CF22226AFC__INCLUDED_)
#define AFX_CHILDVIEW_H__3C598774_58D7_4703_A198_95CF22226AFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

protected:
	CExtColorCtrl
		m_wndColorCtrlCubeRGB,
		m_wndColorCtrlCubeCMY,
		m_wndColorCtrlMixerRGB,
		m_wndColorCtrlMixerCMY,
		m_wndColorCtrlHSL;
	bool m_bSyncingValues;
	bool m_bColorControlsVisible;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewColorControls();
	afx_msg void OnUpdateViewColorControls(CCmdUI* pCmdUI);
	//}}AFX_MSG
	LRESULT OnColorChanged(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__3C598774_58D7_4703_A198_95CF22226AFC__INCLUDED_)
