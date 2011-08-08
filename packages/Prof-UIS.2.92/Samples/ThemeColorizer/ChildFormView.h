#if !defined(AFX_CHILDFORMVIEW_H__5A1BD4EF_2576_4480_BC1A_54B9357C8DC8__INCLUDED_)
#define AFX_CHILDFORMVIEW_H__5A1BD4EF_2576_4480_BC1A_54B9357C8DC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildFormView.h : header file
//

class CMainFrame;

/////////////////////////////////////////////////////////////////////////////
// CChildFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CChildFormView
	: public CExtWS < CExtAFV < CFormView >  >
{
protected:
	CChildFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChildFormView)

// Form Data
public:
	//{{AFX_DATA(CChildFormView)
	enum { IDD = IDD_DIALOG_CHILD_VIEW };
	CExtLabel		m_wndLabelColorTheme;
	CExtLabel		m_wndLabelSelType;
	CExtColorButton	m_wndColor2;
	CExtColorButton m_wndColor1;
	CExtGroupBox	m_wndGroupBox2;
	CExtGroupBox	m_wndGroupBox1;
	CExtComboBox	m_wndColorCtrlType;
	CExtComboBox	m_wndColorTheme;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CExtColorCtrl m_wndColorCtrl1;
	CExtColorCtrl m_wndColorCtrl2;

	friend class CMainFrame;

	enum e_ColorThemes_t
	{
		ThemeLunaBlue	= 1,
		ThemeLunaOlive	= 2,
		ThemeLunaSilver	= 3,
		ThemeStudio2005 = 4,
		ThemePumpkin	= 5,
		ThemeRose		= 6,
		ThemeEggplant	= 7,
		ThemeRainyDay	= 8,
		ThemeSpruce		= 9,
		ThemeLilac		= 10,
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFormView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CChildFormView)
	afx_msg void OnSelendokComboSelType();
	afx_msg void OnSelendokComboColorTheme();
	//}}AFX_MSG
	afx_msg LRESULT OnColorChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void CheckControls();
	bool m_bInitComplete;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFORMVIEW_H__5A1BD4EF_2576_4480_BC1A_54B9357C8DC8__INCLUDED_)
