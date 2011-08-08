#if !defined(AFX_PAGEBASE_H__D54A44F7_80A2_4569_AC2C_223797DB2D82__INCLUDED_)
#define AFX_PAGEBASE_H__D54A44F7_80A2_4569_AC2C_223797DB2D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageBase dialog

class CPageBase : public CExtResizableDialog
{
// Construction
public:
	DECLARE_DYNAMIC( CPageBase );
	CPageBase(UINT _IDD, CWnd* pParent = NULL);   // standard constructor
		
// Dialog Data
	//{{AFX_DATA(CPageBase)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	virtual void PageBase_PostInit();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageBase)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEBASE_H__D54A44F7_80A2_4569_AC2C_223797DB2D82__INCLUDED_)
