#if !defined(AFX_EXTPOPUPRICHEDITCTRL_H__B8A07E35_D826_4976_8BD0_088A56DA42D1__INCLUDED_)
#define AFX_EXTPOPUPRICHEDITCTRL_H__B8A07E35_D826_4976_8BD0_088A56DA42D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtPopupRichEditCtrl.h : header file
//

#include <afxrich.h>

class CExtDynamicRichEditView;

/////////////////////////////////////////////////////////////////////////////
// CExtDynamicRichEditDoc

class CExtDynamicRichEditDoc : public CRichEditDoc
{
	friend class CExtDynamicRichEditView;
protected:

	class CExtDynamicRichEditCntrItem : public CRichEditCntrItem
	{
	public:
		CExtDynamicRichEditCntrItem(
			REOBJECT * preo = NULL,
			CExtDynamicRichEditDoc * pContainer = NULL
			);
		~CExtDynamicRichEditCntrItem();
		CExtDynamicRichEditDoc * GetDocument()
		{
			ASSERT_VALID( this );
			return
				( (CExtDynamicRichEditDoc*) CRichEditCntrItem::GetDocument() );
		}
		CExtDynamicRichEditView * GetActiveView()
		{
			ASSERT_VALID( this );
			return
				( (CExtDynamicRichEditView*) CRichEditCntrItem::GetActiveView() );
		}
	};

public:
	DECLARE_DYNCREATE( CExtDynamicRichEditDoc );

	CExtDynamicRichEditDoc();

// Attributes
public:

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CExtDynamicRichEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

	virtual void OnIdle();
	virtual CRichEditCntrItem * CreateClientItem(
		REOBJECT * preo
		) const;

// Implementation
public:
	virtual ~CExtDynamicRichEditDoc();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CExtDynamicRichEditDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CExtDynamicRichEditView window

class CExtDynamicRichEditView : public CRichEditView
{
	friend class CExtDynamicRichEditDoc;
public:
	DECLARE_DYNCREATE( CExtDynamicRichEditView );
	CExtDynamicRichEditView();

// Attributes
public:
	CExtDynamicRichEditDoc * GetDocument();
protected:
	CSingleDocTemplate * m_pDocTemplate;

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CExtDynamicRichEditView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtDynamicRichEditView();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CExtDynamicRichEditView)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void _DoDestroyWindow(void);
};

/////////////////////////////////////////////////////////////////////////////
// CExtPopupRichEditView window

class CExtPopupRichEditView : public CExtDynamicRichEditView
{
// Construction
public:
	DECLARE_DYNCREATE( CExtPopupRichEditView );
	CExtPopupRichEditView();

// Attributes
public:
	COLORREF m_clrBackground, m_clrBorderLT, m_clrBorderRB;
	enum eShadowType_t
	{
	__EST_NONE		= 0,
	__EST_POPUP		= 1,
	__EST_WINHELP	= 2,
	};
	eShadowType_t m_eST;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtPopupRichEditView)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtPopupRichEditView();
	bool Activate(
		CExtResourceManager::CExtResourceMemoryBuffer & buffer,
		const RECT & rcItem,
		HWND hWndNotify,
		UINT nAlignType,
		int nAlignDistance = 30
		);
	void Deactivate();
private:
	HWND m_hWndNotify;
	CSize m_sizeLayout;
	void LayoutWnd();
	void SetupRTForPlain(
		CExtResourceManager::CExtResourceMemoryBuffer & buffer
		);
	static DWORD CALLBACK stat_StreamUploadCallback(
	   DWORD dwCookie,
	   LPBYTE bBuff,
	   LONG nCountIn,
	   LONG * p_nCountOut
	   );
protected:
	virtual int OnQueryShadowSize() const;
	virtual CRect OnQueryBorderSizes() const;
	virtual void OnPaintNcAreaParts(
		CDC & dc,
		CRect rcWnd,
		CRect rcClient,
		CRect rcBorders
		) const;

	// Generated message map functions
protected:
	//{{AFX_MSG(CExtPopupRichEditView)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnCancelMode();
#if _MFC_VER < 0x700
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
#endif
	afx_msg void OnSettingChange(UINT uFlags, __EXT_MFC_SAFE_LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg LRESULT OnDisplayChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThemeChanged( WPARAM wParam, LPARAM lParam );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTPOPUPRICHEDITCTRL_H__B8A07E35_D826_4976_8BD0_088A56DA42D1__INCLUDED_)
