#if !defined(__DLG_H)
#define __DLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPageWELCOME window

class CPageWELCOME : public CExtResizablePropertyPage
{
public:
	CPageWELCOME();
	virtual ~CPageWELCOME();

	//{{AFX_DATA(CPageWELCOME)
	enum { IDD = IDD_PAGE_WELCOME };
	//}}AFX_DATA
	CExtRichContentScrollWnd m_wndRCS;

	//{{AFX_VIRTUAL(CPageWELCOME)
	protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CPageWELCOME)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; /// class CPageWELCOME

/////////////////////////////////////////////////////////////////////////////
// CTextEditor window

class CTextEditor : public CExtNCSB < CExtEdit >
{
public:
	CTextEditor();
	virtual ~CTextEditor();
	//{{AFX_VIRTUAL(CTextEditor)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam ) ;
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CTextEditor)
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	//}}AFX_MSG
	afx_msg LRESULT OnPaste( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CPageHTML window

class CPageHTML : public CExtResizablePropertyPage
{
public:
	CPageHTML();
	virtual ~CPageHTML();

	//{{AFX_DATA(CPageHTML)
	enum { IDD = IDD_PAGE_HTML };
	CExtComboBox m_comboScollBarsType;
	CExtComboBox m_comboTextOrientation;
	CExtHyperLinkButton m_hyperlinkReLayout;
	//}}AFX_DATA
public:
	CExtRichContentScrollWnd m_wndRCS;
	CTextEditor m_wndEditHTML;

	//{{AFX_VIRTUAL(CPageHTML)
	protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CPageHTML)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelEndOkScrollBarsType();
	afx_msg void OnReLayout();
	//}}AFX_MSG
	afx_msg void OnChangeEditHTML();
	DECLARE_MESSAGE_MAP()
}; /// class CPageHTML

class CDialogCssEditor : public CExtNCW < CExtResizableDialog >
{
protected:
	bool m_bEditingNewCSS:1;
	bool _Validate( bool bSave );
public:
	CString m_strHREF, m_strInitialHREF, m_strCSS, m_strInitialCSS;

	CDialogCssEditor(
		bool bEditingNewCSS,
		LPCTSTR strHREF = NULL,
		LPCTSTR strCSS = NULL
		);
	virtual ~CDialogCssEditor();

	//{{AFX_DATA(CDialogCssEditor)
	enum { IDD = IDD_DIALOG_CSS_EDITOR };
	//}}AFX_DATA
	CExtEdit m_editHREF;
	CExtNCSB < CExtEdit > m_editCSS;
public:

	//{{AFX_VIRTUAL(CDialogCssEditor)
	protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CDialogCssEditor)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnChangeEditors();
	DECLARE_MESSAGE_MAP()
}; /// class CDialogCssEditor

/////////////////////////////////////////////////////////////////////////////
// CPageGC window

class CPageGC : public CExtResizablePropertyPage
{
public:
	CPageGC();
	virtual ~CPageGC();

	//{{AFX_DATA(CPageGC)
	enum { IDD = IDD_PAGE_GC };
	//}}AFX_DATA
public:
	CExtNCSB < CExtListCtrl > m_wndLV;
	CExtHyperLinkButton m_wndHyperLinkAddImage;
	CExtHyperLinkButton m_wndHyperLinkAddCSS;
	CExtHyperLinkButton m_wndHyperLinkEditSelectedCSS;
	CExtHyperLinkButton m_wndHyperLinkDeleteSelected;

	//{{AFX_VIRTUAL(CPageGC)
	protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CPageGC)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnAddImage();
	afx_msg void OnAddCSS();
	afx_msg void OnEditSelectedCSS();
	afx_msg void OnDeleteSelected();
	DECLARE_MESSAGE_MAP()
	void _CheckListViewSelection();
	void _ReInsertObjectsIntoListView();
}; /// class CPageGC

/////////////////////////////////////////////////////////////////////////////
// CDrawTextSurfaceWnd window

class CDrawTextSurfaceWnd : public CExtButton
{
public:
	UINT m_nDTF, m_nExtDTF;
	CRect m_rcCustomRect;
	CDrawTextSurfaceWnd();
	virtual ~CDrawTextSurfaceWnd();
	virtual void _RenderImpl( // non-buffered paint
		CDC & dc,
		bool bTransparent = false,
		bool bCombinedContent = false
		);
	virtual CRect _GetLayoutRect();
	virtual const CExtAnimationParameters *
		AnimationClient_OnQueryAnimationParameters(
			INT eAPT // __EAPT_*** animation type
			) const;
}; /// CDrawTextSurfaceWnd 


/////////////////////////////////////////////////////////////////////////////
// CPageDT window

class CPageDT : public CExtResizablePropertyPage
{
public:
	CPageDT();
	virtual ~CPageDT();

	CExtRichContentLayout::e_layout_orientation_t m_eUserLO;

	//{{AFX_DATA(CPageDT)
	enum { IDD = IDD_PAGE_DT };
	CExtSpinWnd				m_spinCustomRectBottom;
	CExtSpinWnd				m_spinCustomRectTop;
	CExtSpinWnd				m_spinCustomRectRight;
	CExtSpinWnd				m_spinCustomRectLeft;
	CExtCheckBox			m_checkUseCustomRect;
	CExtHyperLinkButton		m_wndHyperLinkUseCurrentRect;
	CExtCheckBox			m_checkExtMultilineVerticalAlignment;
	CExtCheckBox			m_checkExtCalcEffectiveRect;
	CExtCheckBox			m_checkWin32TextOffset;
	CExtCheckBox			m_checkProfUisText;
	CExtCheckBox			m_checkWin32TextCompare;
	CExtComboBox			m_comboVerticalAlignment;
	CExtComboBox			m_comboHorizontalAlignment;
 	CExtButton				m_buttonCancel;
 	CExtButton				m_buttonOk;
	CExtComboBox			m_comboTextFormat;
	CExtComboBox			m_comboTextOrientation;
	CExtGroupBox			m_groupBoxVistarTextRenderingOptions;
	CExtCheckBox			m_checkBoxUseVistaTextAPIs;
	CExtEdit				m_editVistaTextBorderSize;
	CExtSpinWnd				m_spinVistaTextBorderSize;
	CExtColorButton			m_buttonVistaTextBorderColor;
	CExtEdit				m_editVistaTextGlowSize;
	CExtSpinWnd				m_spinVistaTextGlowSize;
	CExtComboBox			m_comboVistaShadowType;
	CExtColorButton			m_buttonVistaTextShadowColor;
	CExtEdit				m_editVistaTextShadowOffsetX;
	CExtSpinWnd				m_spinVistaTextShadowOffsetX;
	CExtEdit				m_editVistaTextShadowOffsetY;
	CExtSpinWnd				m_spinVistaTextShadowOffsetY;
	CExtComboBox			m_comboBackgroundType;
	//}}AFX_DATA
	CDrawTextSurfaceWnd		m_wndDrawTextSurface;
	CTextEditor	m_wndDrawTextEdit;

	//{{AFX_VIRTUAL(CPageDT)
	protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	//}}AFX_VIRTUAL

public:
	CExtUxTheme::__EXT_UX_DTTOPTS m_DTO;
protected:
	bool m_bInitComplete:1;
	//{{AFX_MSG(CPageDT)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditDrawText();
	//}}AFX_MSG
	afx_msg void OnRescanCustomRect();
	afx_msg void OnButtonGetCurrentRect();
	afx_msg void OnRescanDtFlags();
	afx_msg LRESULT OnColorChangedFinally( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
}; /// class CPageDT

/////////////////////////////////////////////////////////////////////////////
// CPageAC window

class CPageAC : public CExtResizablePropertyPage
{
public:
	CPageAC();
	virtual ~CPageAC();

	//{{AFX_DATA(CPageAC)
	enum { IDD = IDD_PAGE_AC };
	CExtComboBox m_comboScollBarsType;
	//}}AFX_DATA

	CExtRichContentScrollWnd m_wndRCS;
	CExtButton m_buttonPush;
	CExtEdit m_edit;
	CExtEditSystemNumber m_editSystemNumber;
	CExtEditSystemCurrency m_editSystemCurrency;
//	CExtDateTimeWnd m_editDateTime;
//	CExtDurationWnd m_editDuration;
//	CExtLLEditWnd m_editLL;
	CExtColorButton m_buttonColor;
	CExtComboBox m_comboBox;
//	CExtImageEditWnd m_imageEditor;
//	CExtNCSB < CExtShellTreeCtrl > m_shellTree;

	//{{AFX_VIRTUAL(CPageAC)
	protected:
	virtual void DoDataExchange( CDataExchange * pDX );
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CPageAC)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelEndOkScrollBarsType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; /// class CPageAC


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DLG_H)
