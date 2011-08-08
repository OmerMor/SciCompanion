#if !defined(__PAGE_LIST_CTRL_H)
#define __PAGE_LIST_CTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageListCtrl.h : header file
//

#if ( ! defined __EXT_MFC_NO_LIST_VIEW_CTRL )

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageListCtrl dialog

class CPageListCtrl : public CPageBase 
{
// Construction
public:
	CPageListCtrl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageListCtrl)
	enum { IDD = IDD_PAGE_LIST_CTRL };
	CExtCheckBox	m_checkShowToolTipsInHeader;
	CExtCheckBox	m_checkShowHeaderIcons;
	CExtCheckBox	m_checkShowHeaderButtons;
	CExtHyperLinkButton	m_hyperlinkStateSave;
	CExtHyperLinkButton	m_hyperlinkStateLoad;
	CExtCheckBox	m_checkColumnAutoSizingByHeader;
	CExtCheckBox	m_checkColumnAutoSizingByData;
	CExtCheckBox	m_checkSortingByMultipleColumns;
	CExtCheckBox	m_checkSorting;
	CExtCheckBox	m_checkColumnDND;
	CExtComboBox	m_comboListType;
	//}}AFX_DATA
	CExtNCSB < CExtListCtrl >	m_wndList;
//	CExtListCtrl	m_wndList;

	CExtImageList m_ImageListNormal, m_ImageListSmall;

	virtual void PageBase_PostInit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageListCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;
	
	// Generated message map functions
	//{{AFX_MSG(CPageListCtrl)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokComboListType();
	afx_msg void OnDestroy();
	afx_msg void OnCheckEnableSorting();
	afx_msg void OnCheckMultiColumnSorting();
	afx_msg void OnCheckEnableColumnDnd();
	afx_msg void OnCheckColumnAutoSizingByData();
	afx_msg void OnCheckColumnAutoSizingByHeader();
	afx_msg void OnHyperLinkStateLoad();
	afx_msg void OnHyperLinkStateSave();
	afx_msg void OnCheckShowHeaderButtons();
	afx_msg void OnCheckShowHeaderIcons();
	afx_msg void OnCheckShowHeaderToolTips();
	//}}AFX_MSG
	afx_msg LRESULT OnMsgHeaderButtonClick( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

#endif // ( ! defined __EXT_MFC_NO_LIST_VIEW_CTRL )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PAGE_LIST_CTRL_H)
