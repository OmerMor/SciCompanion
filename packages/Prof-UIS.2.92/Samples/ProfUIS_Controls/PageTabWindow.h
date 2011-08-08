#if !defined(AFX_PAGETABWINDOW_H__6360D695_F300_4D8C_AE56_2ED554879B7C__INCLUDED_)
#define AFX_PAGETABWINDOW_H__6360D695_F300_4D8C_AE56_2ED554879B7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageTabWindow.h : header file
//

#if (!defined __EXT_MFC_NO_TAB_CTRL)

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageTabWindow dialog

class CPageTabWindow : public CPageBase
{
// Construction
public:
	CPageTabWindow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageTabWindow)
	enum { IDD = IDD_PAGE_TAB_WINDOW };
	CExtCheckBox	m_CheckShowHome;
	CExtCheckBox	m_CheckShowEnd;
	CExtGroupBox	m_wndGroupBoxStyle;
	CExtRadioButton m_wndRadioStyleStandard;
	CExtRadioButton m_wndRadioStyleOneNote;
	CExtRadioButton m_wndRadioStyleWhidbey;
	CExtRadioButton m_wndRadioStyleFlat;
	CExtRadioButton m_wndRadioStyleButtons;
	CExtCheckBox	m_CheckItemDragging;
	CExtCheckBox	m_CheckEnableTabList;
	CExtCheckBox	m_CheckShowTabList;
	CExtCheckBox	m_CheckFocusOnHover;
	CExtCheckBox	m_CheckFullRowWidth;
	CExtCheckBox	m_CheckCenterText;
	CExtCheckBox	m_CheckAutoHideScroll;
	CExtCheckBox	m_CheckEnableHelp;
	CExtCheckBox	m_CheckEnableClose;
	CExtCheckBox	m_CheckShowHelp;
	CExtCheckBox	m_CheckBoldSelection;
	CExtCheckBox	m_CheckShowClose;
	CExtCheckBox	m_CheckInvertFont;
	CExtCheckBox	m_CheckEqualWidth;
	CExtCheckBox	m_CheckShowBorders;
	CExtButton		m_BtnAdd10;
	CExtButton		m_BtnAdd1;
	CExtButton		m_BtnClear;
	//}}AFX_DATA
	
	CExtTabWnd
		m_wndTabOuterTop,
		m_wndTabOuterBottom,
		m_wndTabOuterLeft,
		m_wndTabOuterRight,
		m_wndTabInnerTop,
		m_wndTabInnerBottom,
		m_wndTabInnerLeft,
		m_wndTabInnerRight;

	CExtTabOneNoteWnd
		m_wndTabOneNoteOuterTop,
		m_wndTabOneNoteOuterBottom,
		m_wndTabOneNoteOuterLeft,
		m_wndTabOneNoteOuterRight,
		m_wndTabOneNoteInnerTop,
		m_wndTabOneNoteInnerBottom,
		m_wndTabOneNoteInnerLeft,
		m_wndTabOneNoteInnerRight;

	CExtTabWhidbeyWnd
		m_wndTabWhidbeyOuterTop,
		m_wndTabWhidbeyOuterBottom,
		m_wndTabWhidbeyOuterLeft,
		m_wndTabWhidbeyOuterRight,
		m_wndTabWhidbeyInnerTop,
		m_wndTabWhidbeyInnerBottom,
		m_wndTabWhidbeyInnerLeft,
		m_wndTabWhidbeyInnerRight;

	CExtTabFlatWnd
		m_wndTabFlatOuterTop,
		m_wndTabFlatOuterBottom,
		m_wndTabFlatOuterLeft,
		m_wndTabFlatOuterRight,
		m_wndTabFlatInnerTop,
		m_wndTabFlatInnerBottom,
		m_wndTabFlatInnerLeft,
		m_wndTabFlatInnerRight;

	CExtTabButtonsWnd
		m_wndTabButtonsOuterTop,
		m_wndTabButtonsOuterBottom,
		m_wndTabButtonsOuterLeft,
		m_wndTabButtonsOuterRight,
		m_wndTabButtonsInnerTop,
		m_wndTabButtonsInnerBottom,
		m_wndTabButtonsInnerLeft,
		m_wndTabButtonsInnerRight;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageTabWindow)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageTabWindow)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonClearTabs();
	afx_msg void OnButtonAddTab1();
	afx_msg void OnButtonAddTab10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	struct demo_items_data_t
	{
		LPCTSTR m_sItemText;
		bool m_bGroupStart:1;
	};
	static demo_items_data_t g_ItemsData[];
	
	CImageList m_ImageList;
	int m_nDemoImageListSize;
	int m_nDemoImageListAddIndexInner;
	int m_nDemoImageListAddIndexOuter;
	
	void _DoAdd1( bool bUpdateTabWnd );
};

#endif // (!defined __EXT_MFC_NO_TAB_CTRL)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGETABWINDOW_H__6360D695_F300_4D8C_AE56_2ED554879B7C__INCLUDED_)
