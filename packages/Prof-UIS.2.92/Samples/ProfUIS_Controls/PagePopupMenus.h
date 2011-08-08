#if !defined(AFX_PAGEPOPUPMENUS_H__37F8A029_132C_4EEF_8BA8_FC82B64EFCEC__INCLUDED_)
#define AFX_PAGEPOPUPMENUS_H__37F8A029_132C_4EEF_8BA8_FC82B64EFCEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PagePopupMenus.h : header file
//
#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPagePopupMenus dialog

class CPagePopupMenus : public CPageBase
{
// Construction
public:
	CPagePopupMenus(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPagePopupMenus)
	enum { IDD = IDD_PAGE_POPUP_MENUS };
	CExtCheckBox	m_checkUseAcceleratedMenuScrolling;
	CExtEdit		m_editIncreasePercent;
	CExtEdit		m_editIncreaseStep;
	CExtEdit		m_editCountOfItems;
	CExtEdit		m_editMaxPixelsPerStep;
	CExtSpinWnd		m_spinIncreasePercent;
	CExtSpinWnd		m_spinIncreaseStep;
	CExtSpinWnd		m_spinCountOfItems;
	CExtSpinWnd		m_spinMaxPixelsPerStep;
	CExtLabel		m_labelIncreasePercent;
	CExtLabel		m_labelIncreaseStep;
	CExtLabel		m_labelCountOfItems;
	CExtLabel		m_labelMaxPixelsPerStep;
	CExtGroupBox	m_groupBoxLongSubMenuSettings;
	CExtLabel		m_Static1;
	CExtCheckBox	m_chkMenuWithShadows;
	CExtCheckBox	m_chkMenuAnimation;
	CExtCheckBox	m_chkPaletteWithCustomDrawnBackground;
	CExtGroupBox	m_groupBoxMenuAnimation;
	CExtComboBox	m_MenuAnimation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPagePopupMenus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;

	// Generated message map functions
	//{{AFX_MSG(CPagePopupMenus)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokMenuAnimation();
	afx_msg void OnCheckMenuAnimation();
	afx_msg void OnCheckMenuWithShadows();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCheckUseAcceleratedMenuScrolling();
	afx_msg void OnChangeEditIncreasePercent();
	afx_msg void OnChangeEditIncreaseStep();
	afx_msg void OnChangeEditMaxPixelsPerStep();
	//}}AFX_MSG
//				afx_msg LRESULT OnMsgTranslateMouseClickEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	LRESULT OnMsgSelectionNotification( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExtMenuDrawBackground(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawPopupLeftArea(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenuRarelyItem1();
	afx_msg void OnUpdateMenuRarelyItem1( CCmdUI * pCmdUI );
	afx_msg void OnMenuRarelyItem2();
	afx_msg void OnUpdateMenuRarelyItem2( CCmdUI * pCmdUI );
	afx_msg void OnMenuRarelyItem3();
	afx_msg void OnUpdateMenuRarelyItem3( CCmdUI * pCmdUI );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEPOPUPMENUS_H__37F8A029_132C_4EEF_8BA8_FC82B64EFCEC__INCLUDED_)
