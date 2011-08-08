#if !defined(__PAGE_SHELL_BROWSER_H)
#define __PAGE_SHELL_BROWSER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageShellBrowser.h : header file
//

#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL ) && ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )&& ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageShellBrowser dialog

class CPageShellBrowser : public CPageBase 
{
// Construction
public:
	CPageShellBrowser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageShellBrowser)
	enum { IDD = IDD_PAGE_SHELL_BROWSER };
	//}}AFX_DATA
	CExtShellComboBox m_wndCombo;
	CExtNCSB < CExtShellTreeCtrl >	m_wndTree;
	CExtNCSB < CExtShellListCtrl > m_wndList;
	bool m_bHaldingSelectionInTree:1, m_bHaldingSelectionInList:1, m_bHaldingSelectionInCombo:1;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageShellBrowser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;
	
	// Generated message map functions
	//{{AFX_MSG(CPageShellBrowser)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnShellTreeDelayedItemFocus( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnShellListLocationChanged( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnShellComboLocationChanged( WPARAM wParam, LPARAM lParam );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL ) && ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )&& ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PAGE_SHELL_BROWSER_H)
