#if !defined(__PAGE_SHELL_TREE_CTRL_H)
#define __PAGE_SHELL_TREE_CTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageShellTreeCtrl.h : header file
//

#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageShellTreeCtrl dialog

class CPageShellTreeCtrl : public CPageBase 
{
// Construction
public:
	CPageShellTreeCtrl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageShellTreeCtrl)
	enum { IDD = IDD_PAGE_SHELL_TREE_CTRL };
	CExtButton m_buttonGo;
	CExtButton m_buttonMakeNewFolder;
	CExtEdit   m_editPath;
	CExtCheckBox m_checkIncludeFiles;
	CExtCheckBox m_checkEditLabels;
	CExtCheckBox m_checkShowHiddenItems;
	CExtCheckBox m_checkWatchFileSystem;
	//}}AFX_DATA
	CExtNCSB < CExtShellTreeCtrl > m_wndTree;
//	CExtShellTreeCtrl m_wndTree;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageShellTreeCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;
	
	// Generated message map functions
	//{{AFX_MSG(CPageShellTreeCtrl)
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonGo();
	afx_msg void OnBnClickedMakeNewFolderButton();
	afx_msg void OnBnClickedShowFilesCheck();
	afx_msg void OnBnClickedShowHiddenFilesCheck();
	afx_msg void OnBnClickedEditingLabelsCheck();
	afx_msg void OnBnClickedWatchFileSystemCheck();
	afx_msg void OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EXT_MFC_NO_SHELL_TREE_VIEW_CTRL)
