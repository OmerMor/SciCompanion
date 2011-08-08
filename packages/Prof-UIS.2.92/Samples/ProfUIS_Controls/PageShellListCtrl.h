#if !defined(__PAGE_SHELL_LIST_CTRL_H)
#define __PAGE_SHELL_LIST_CTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageShellListCtrl.h : header file
//

#if ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageShellListCtrl dialog

class CPageShellListCtrl : public CPageBase 
{
// Construction
public:
	CPageShellListCtrl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageShellListCtrl)
	enum { IDD = IDD_PAGE_SHELL_LIST_CTRL };
	CExtCheckBox		m_checkRootFolder;
	CExtCheckBox		m_checkFolderBeforeOther;
	CExtCheckBox		m_checkEdit;
	CExtCheckBox		m_checkViewFiles;
	CExtCheckBox		m_checkUpFolder;
	CExtCheckBox		m_checkShowHiddenState;
	CExtCheckBox		m_checkWatchFileSystem;
	CExtComboBox		m_comboListSort;
	CExtButton			m_buttonGo;
	CExtEdit			m_editPath;
	CExtComboBox		m_comboListType;
	//}}AFX_DATA
	CExtNCSB < CExtShellListCtrl >	m_wndList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageShellListCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;
	
	// Generated message map functions
	//{{AFX_MSG(CPageShellListCtrl)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokComboListType();
	afx_msg void OnBottonGo();
	afx_msg void OnSelendokComboListSort();
	afx_msg void OnCheckEditing();
	afx_msg void OnCheckUpFolder();
	afx_msg void OnCheckViewFiles();
	afx_msg void OnCheckFolderBeforeOther();
	afx_msg void OnCheckRootFolder();
	afx_msg void OnCheckShowHidden();
	afx_msg void OnCheckWatchFileSystem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EXT_MFC_NO_SHELL_LIST_VIEW_CTRL)
