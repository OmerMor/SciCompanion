#if !defined(AFX_PAGECALENDAR_H__6D42983E_1D78_4C83_9120_9B93EBDCEFB1__INCLUDED_)
#define AFX_PAGECALENDAR_H__6D42983E_1D78_4C83_9120_9B93EBDCEFB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageCalendar.h : header file
//

#if (!defined __EXT_MFC_NO_DATE_PICKER)

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageCalendar dialog

class CPageCalendar : public CPageBase 
{
// Construction
public:
	CPageCalendar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageCalendar)
	enum { IDD = IDD_PAGE_CALENDAR };
	CExtEdit	m_wndRowsMin;
	CExtEdit	m_wndRowsMax;
	CExtEdit	m_wndColumnsMax;
	CExtEdit	m_wndColumnsMin;
	CExtSpinWnd m_spinRowsMin;
	CExtSpinWnd	m_spinRowsMax;
	CExtSpinWnd	m_spinColumnsMin;
	CExtSpinWnd	m_spinColumnsMax;
	CExtLabel	m_StaticNotifications;
	CExtLabel	m_staticRMin;
	CExtLabel	m_staticRMax;
	CExtLabel	m_staticCMin;
	CExtLabel	m_staticCMax;
	CExtCheckBox	m_chkReserveBorders;
	CExtCheckBox	m_chkNoHdrBk;
	CExtCheckBox	m_chkHideNonMonthDaysInside;
	CExtCheckBox	m_chkEnableMultipleSelect;
	CExtCheckBox	m_chkBoldWeek;
	CExtCheckBox	m_chkBoldPopup;
	CExtCheckBox	m_chkBoldHeader;
	CExtCheckBox	m_chkBoldCell;
	CExtCheckBox	m_chkAutoDimensionsMode;
	CExtCheckBox	m_chkShowTodayHighlight;
	CExtCheckBox	m_chkShowNonMonthDays;
	CExtCheckBox	m_chkShowMonthListShadow;
	CExtCheckBox	m_chkShowBtnToday;
	CExtCheckBox	m_chkShowBtnSeparator;
	CExtCheckBox	m_chkShowBtnNone;
	CExtCheckBox	m_chkShowBorders;
	CExtDatePickerWnd m_wndDatePicker;
	//}}AFX_DATA
	CExtNCSB < CExtFlatEdit > m_wndNotifications;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageCalendar)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void _VerifyCalendarDimensions();
	bool m_bInitComplete:1;
	bool m_bVerificationInProcess:1;
	
	// Generated message map functions
	//{{AFX_MSG(CPageCalendar)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	LRESULT OnMsgSelectionNotification( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

#endif // #if (!defined __EXT_MFC_NO_DATE_PICKER)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGECALENDAR_H__6D42983E_1D78_4C83_9120_9B93EBDCEFB1__INCLUDED_)
