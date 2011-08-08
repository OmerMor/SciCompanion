#if !defined(__PAGE_TREE_CTRL_H)
#define __PAGE_TREE_CTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageTreeCtrl.h : header file
//

#if ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageTreeCtrl dialog

class CPageTreeCtrl : public CPageBase 
{
	CList < HTREEITEM, HTREEITEM > m_listTreeItemsWithProgressControl;
// Construction
public:
	CPageTreeCtrl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageTreeCtrl)
	enum { IDD = IDD_PAGE_TREE_CTRL };
	CExtCheckBox m_wndCheckBoxExpandedOnly;
	CExtCheckBox m_wndFullRowSelectCheckBox;
	CExtCheckBox m_wndHasButtonsCheckBox;
	CExtCheckBox m_wndHasLinesCheckBox;
	CExtCheckBox m_wndLineSatRoot;
	CExtCheckBox m_wndShowSelAlwaysCheckBox;
	CExtCheckBox m_wndSingleExpandCheckBox;
	CExtCheckBox m_wndTrackSelectCheckBox;
	CExtCheckBox m_wndHoverDisableItemsCheckBox;
	CExtCheckBox m_wndHoverTreeBoxExCheckBox;
	CExtCheckBox m_wndHoverCheckBoxExCheckBox;
	CExtCheckBox m_wndHoverRadioBoxExCheckBox;
	CExtCheckBox m_wndMultipleSelectionCheckBox;
	CExtCheckBox m_wndFocusDisableItemsCheckBox;
	CExtCheckBox m_wndSelectDisableItemsCheckBox;
	CExtCheckBox m_wndSelectCollapsedItemsCheckBox;
	CExtCheckBox m_wndSubtractSelectionCheckBox;
	CExtCheckBox m_wndOutlineSolidCheckBox;
	CExtCheckBox m_wndTreeBoxBitmapCheckBox;
	CExtCheckBox m_wndTreeBoxSquareCheckBox;
	CExtCheckBox m_wndTreeBoxRoundCheckBox;
	CExtCheckBox m_wndAutoDisableChildrenItems;
	CExtCheckBox m_wndEditLabelsCheckBox;
	CExtCheckBox m_wndReadOnlyLabelsCheckBox;
	CExtCheckBox m_wndInfoTipCheckBox;
	CExtButton m_buttonOK;
	CExtButton m_buttonCancel;
	CExtButton m_buttonUp;
	CExtButton m_buttonDown;
	CExtButton m_buttonLoadTreeBoxXP;
	CExtButton m_buttonLoadTreeBoxVista;
	//}}AFX_DATA
	CExtNCSB < CExtTreeCtrl > m_wndTree;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageTreeCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;
	
	// Generated message map functions
	//{{AFX_MSG(CPageTreeCtrl)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangedTree1( NMHDR * pNMHDR, LRESULT * p_lResult );
	afx_msg void OnBnClickedMybutton1();
	afx_msg void OnBnClickedMybutton2();
	afx_msg void OnBnClickedCheckFullRowSelect();
	afx_msg void OnBnClickedCheckHasButtons();
	afx_msg void OnBnClickedCheckHasLines();
	afx_msg void OnBnClickedCheckLineSatRoot();
	afx_msg void OnBnClickedCheckShowSelAlways();
	afx_msg void OnBnClickedCheckSingleExpand();
	afx_msg void OnBnClickedCheckTrackSelect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCheckMultipleSelection();
	afx_msg void OnBnClickedCheckFocusDisableItems();
	afx_msg void OnBnClickedCheckSelectDisableItems();
	afx_msg void OnBnClickedCheckSelectCollapsedItems();
	afx_msg void OnBnClickedCheckSubtractSelection();
	afx_msg void OnBnClickedCheckHoverDisableItems();
	afx_msg void OnBnClickedCheckHoverTreeBoxEx();
	afx_msg void OnBnClickedCheckHoverCheckBoxEx();
	afx_msg void OnBnClickedCheckHoverRadioBoxEx();
	afx_msg void OnBnClickedButtonTreeBoxXp();
	afx_msg void OnBnClickedButtonTreeBoxVistaSet();
	afx_msg void OnBnClickedCheckSolidOutline();
	afx_msg void OnBnClickedCheckTreeBoxBitmap();
	afx_msg void OnBnClickedCheckTreeBoxSquare();
	afx_msg void OnBnClickedCheckTreeBoxRound();
	afx_msg void OnTimer( __EXT_MFC_UINT_PTR nIDEvent );
	afx_msg void OnBnClickedCheckAutoDisableChildrenItems();
	afx_msg void OnBnClickedCheckInfoTip();
	afx_msg void OnBnClickedCheckEditLabels();
	afx_msg void OnBnClickedCheckReadOnlyLabels();
	afx_msg void OnBegindragTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginRdragTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditTree1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PAGE_TREE_CTRL_H)
