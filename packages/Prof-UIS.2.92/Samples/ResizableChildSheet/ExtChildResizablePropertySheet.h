#if !defined(AFX_EXTCHILDRESIZABLEPROPERTYSHEET_H__A1F4969D_B9D3_4048_9D23_1E3BA6009D08__INCLUDED_)
#define AFX_EXTCHILDRESIZABLEPROPERTYSHEET_H__A1F4969D_B9D3_4048_9D23_1E3BA6009D08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtChildResizablePropertySheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExtChildResizablePropertySheet

class CExtChildResizablePropertySheet : public CExtResizablePropertySheet
{
	DECLARE_DYNCREATE(CExtChildResizablePropertySheet)

	virtual void _TrickSyncActviveChild();
public:
	CExtChildResizablePropertySheet();

// Attributes
public:
	bool m_bInitTabSingleLine:1;
	bool m_bInitTabButtons:1;
	bool m_bInitTabButtonsFlat:1;
	bool m_bInitTabSeparatorsFlat:1;
private:
	bool m_bTabStylesInitialized:1;

// Operations
public:
	BOOL Create( CWnd * pWndParent );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtChildResizablePropertySheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExtChildResizablePropertySheet();

// Generated message map functions
protected:
	//{{AFX_MSG(CExtChildResizablePropertySheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTCHILDRESIZABLEPROPERTYSHEET_H__A1F4969D_B9D3_4048_9D23_1E3BA6009D08__INCLUDED_)
