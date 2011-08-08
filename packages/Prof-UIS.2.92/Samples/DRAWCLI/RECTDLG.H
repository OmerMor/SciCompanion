// rectdlg.h : header file
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


/////////////////////////////////////////////////////////////////////////////
// CRectDlg dialog

class CRectDlg : public CPropertyPage
{
	void _UpdateControls();

// Construction
public:
	CRectDlg(CWnd* pParent = NULL); // standard constructor

	static COLORREF g_clrTransparent;
	COLORREF m_clrFill, m_clrOutline;

// Dialog Data
	//{{AFX_DATA(CRectDlg)
	enum { IDD = IDD_PROP_RECT };
	CSpinButtonCtrl m_SpinCtrl;
	BOOL    m_bNoFill;
	UINT    m_nPenSize;
	CExtColorButton	m_BtnColorOutline;
	CExtColorButton	m_BtnColorFill;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CRectDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnInternalUpdateControls(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
