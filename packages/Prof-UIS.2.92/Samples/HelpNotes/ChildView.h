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

class CChildView : public CExtWS < CExtWA < CExtAFV < CFormView > > >
{
// Construction
public:
	CChildView();

	DECLARE_DYNCREATE( CChildView );

// Attributes
public:

// Operations
public:
	//{{AFX_DATA(CChildView)
	enum { IDD = IDD_DIALOG_CHILD_VIEW };
	CExtRadioButton		m_radioShadow1;
	CExtRadioButton		m_radioShadow2;
	CExtRadioButton		m_radioShadow3;
	CExtCheckBox		m_checkCustomBorderRbColor;
	CExtColorButton		m_buttonCustomBorderRbColor;
	CExtCheckBox		m_checkCustomBorderLtColor;
	CExtColorButton		m_buttonCustomBorderLtColor;
	CExtCheckBox		m_checkCustomBkColor;
	CExtColorButton		m_buttonCustomBkColor;
	CExtGroupBox		m_groupBox1;
	CExtGroupBox		m_groupBox2;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
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
	afx_msg void OnEditUndo();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnFilePrint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFileSave();
	afx_msg void OnFileSendMail();
	afx_msg void OnCheckCustomBackgroundColor();
	afx_msg void OnCheckCustomBorderLtColor();
	afx_msg void OnCheckCustomBorderRbColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__3C598774_58D7_4703_A198_95CF22226AFC__INCLUDED_)
