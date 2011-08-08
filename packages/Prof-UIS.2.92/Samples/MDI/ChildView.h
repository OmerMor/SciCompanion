// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__7627BE0E_E398_44DE_B281_3696A9A0C8BA__INCLUDED_)
#define AFX_CHILDVIEW_H__7627BE0E_E398_44DE_B281_3696A9A0C8BA__INCLUDED_

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
		m_wndColorCtrlCubeRGB_R,
		m_wndColorCtrlCubeRGB_G,
		m_wndColorCtrlHSL_BH,
		m_wndColorCtrlHSL_Panel,
		m_wndColorCtrlCubeRGB_B;
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

#endif // !defined(AFX_CHILDVIEW_H__7627BE0E_E398_44DE_B281_3696A9A0C8BA__INCLUDED_)
