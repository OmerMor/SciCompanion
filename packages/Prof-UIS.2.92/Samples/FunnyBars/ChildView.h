// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__33CAE65F_75FA_4040_ADF4_513C2DF610BC__INCLUDED_)
#define AFX_CHILDVIEW_H__33CAE65F_75FA_4040_ADF4_513C2DF610BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView 
	: public CExtNCSB < CExtWS < CExtAFV < CFormView > > >
{
// Construction
public:
	CChildView();

	DECLARE_DYNCREATE( CChildView );

// Attributes
public:

// Operations
protected:
	LOGFONT m_lf;
	bool m_bInitializingControls:1;
	void _ConrolsData_2_UiSettings();
	void _UpdateHotBar( bool bForceEmptyIconCache );
public:

	//{{AFX_DATA(CChildView)
	enum { IDD = IDD_CHILD_FORM };
	CExtLabel		m_wndLabelFontWidth;
	CExtLabel		m_wndLabelFontHeight;
	CExtLabel		m_wndLabelFontFaceName;
	CExtGroupBox	m_wndBoxFont;
	CExtGroupBox	m_wndBoxBrightness;
	CExtSliderWnd	m_wndSliderHTBB;
	CExtEdit		m_EditFontWidth;
	CExtEdit		m_EditFontHeight;
	CExtComboBox	m_ComboFontFaceName;
	CExtCheckBox	m_BtnFontUnderline;
	CExtCheckBox	m_BtnFontStrikeout;
	CExtCheckBox	m_BtnFontItalic;
	CExtCheckBox	m_BtnFontBold;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg void OnCheckFontBold();
	afx_msg void OnCheckFontItalic();
	afx_msg void OnCheckFontUnderline();
	afx_msg void OnCheckFontStrikeout();
	afx_msg void OnChangeEditFontWidth();
	afx_msg void OnChangeEditFontHeight();
	afx_msg void OnSelchangeComboFontFaceName();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__33CAE65F_75FA_4040_ADF4_513C2DF610BC__INCLUDED_)
