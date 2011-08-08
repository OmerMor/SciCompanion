#if !defined(AFX_PAGEDATETIMEDURATION_H__B15ECEDA_5E82_4F10_9A40_F6232C2EB49E__INCLUDED_)
#define AFX_PAGEDATETIMEDURATION_H__B15ECEDA_5E82_4F10_9A40_F6232C2EB49E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageDateTimeDuration.h : header file
//

#include "PageBase.h"

#if (!defined __EXT_MFC_NO_DURATIONWND && !defined __EXT_MFC_NO_DATETIMEWND )

/////////////////////////////////////////////////////////////////////////////
// CPageDateTimeDuration dialog

class CPageDateTimeDuration : public CPageBase
{
// Construction
public:
	CPageDateTimeDuration(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageDateTimeDuration)
	enum { IDD = IDD_PAGE_DATE_TIME_DURATION };
	CExtCheckBox	m_wndDisabledDuration;
	CExtCheckBox	m_wndDisabledDateTime;
	CExtCheckBox	m_wndReadOnlyDuration;
	CExtCheckBox	m_wndReadOnlyDateTime;
	CExtCheckBox	m_wndModeSeconds;
	CExtCheckBox	m_wndModeMinutes;
	CExtCheckBox	m_wndModeHours;
	CExtCheckBox	m_wndModeDays;
	CExtLabel		m_wndLabelNotifications;
	CExtCheckBox	m_wndContinuousDateTime;
	CExtCheckBox	m_wndContinuousDuration;
	CExtGroupBox	m_wndBoxTimeFormat;
	CExtGroupBox	m_wndBoxModeDateTime;
	CExtGroupBox	m_wndBoxModeDuration;
	CExtCheckBox	m_wndBtnSpinDateTime;
	CExtCheckBox	m_wndBtnSpinDuration;
	CExtCheckBox	m_wndBtnDropDown;
	CExtCheckBox	m_wndBtnShowSelectionAlwaysDateTime;
	CExtCheckBox	m_wndBtnShowSelectionAlwaysDuration;
	CExtColorButton m_wndColorTextDateTime;
	CExtColorButton	m_wndColorBackDateTime;
	CExtButton	m_wndAdjustSizeDateTime;
	CExtColorButton m_wndColorTextDuration;
	CExtColorButton	m_wndColorBackDuration;
	CExtButton	m_wndAdjustSizeDuration;
	CExtGroupBox	m_wndBoxDuration;
	CExtGroupBox	m_wndBoxDateTime;
	CExtDurationWnd m_wndDuration;
	CExtDateTimeWnd m_wndDateTime;
	CExtRadioButton m_wndTimeFormatAuto;
	CExtRadioButton m_wndTimeFormatH12;
	CExtRadioButton m_wndTimeFormatH24;
	CExtRadioButton m_wndModeTime;
	CExtRadioButton m_wndModeDate;
	CExtRadioButton m_wndModeDateTime;
	int		m_nTimeFormat;
	int		m_nMode;
	//}}AFX_DATA
	CExtNCSB < CExtFlatEdit > m_wndNotifications;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageDateTimeDuration)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;

	// Generated message map functions
	//{{AFX_MSG(CPageDateTimeDuration)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnSelChangedDuration( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSelChangedDateTime( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // (!defined __EXT_MFC_NO_DURATIONWND && !defined __EXT_MFC_NO_DATETIMEWND )

#if (!defined __EXT_MFC_NO_DATE_BROWSER )

/////////////////////////////////////////////////////////////////////////////
// CLogCtrl window

#define ID_TIMER_DELAYED_UPDATE 1
#define ELAPSE_TIMER_DELAYED_UPDATE 50

#define ID_TIMER_DELAYED_DESTROY 2

#define ID_TIMER_EVENT_SOURCE 3
#define ELAPSE_TIMER_EVENT_SOURCE 50

#define WM_USR_WRITE_LOG_TEXT (WM_USER + 0x1234)

class CLogCtrl : public CRichEditCtrl
{
	DWORD m_dwHelperThreadID;
	
	struct _MthMessageDataFor_WM_USR_WRITE_LOG_TEXT
	{
		bool m_bBold:1;
		COLORREF m_clrText;
		LPCTSTR m_sText;
		_MthMessageDataFor_WM_USR_WRITE_LOG_TEXT(
			bool bBold,
			COLORREF clrText,
			LPCTSTR sText
			)
			: m_bBold( bBold )
			, m_clrText( clrText )
			, m_sText( sText )
		{
		}
		operator LPARAM () const
		{
			return reinterpret_cast < LPARAM > ( this );
		}
		static _MthMessageDataFor_WM_USR_WRITE_LOG_TEXT &
			FromLPARAM(
				LPARAM lParam
				)
		{
			_MthMessageDataFor_WM_USR_WRITE_LOG_TEXT * pData =
				reinterpret_cast < _MthMessageDataFor_WM_USR_WRITE_LOG_TEXT * > ( lParam );
			ASSERT( pData != NULL );
			return * pData;
		}
	}; // _MthMessageDataFor_WM_USR_WRITE_LOG_TEXT

// Construction
public:
	CLogCtrl();

// Attributes
public:
	LONG m_nLineCountMax;

// Operations
public:
	BOOL Create(
		CWnd * pWndParent,
		UINT nID = UINT(IDC_STATIC),
		CRect rect = CRect(0,0,0,0),
		DWORD dwWindowStyles =
			WS_CHILD|WS_VISIBLE
				|WS_HSCROLL|WS_VSCROLL
				|ES_AUTOHSCROLL|ES_AUTOVSCROLL
				|ES_LEFT|ES_MULTILINE|ES_NOHIDESEL
				|ES_READONLY
		)
	{
		m_dwHelperThreadID = ::GetCurrentThreadId();
		return
			CRichEditCtrl::Create(
				dwWindowStyles,
				rect,
				pWndParent,
				nID
				);
	}

	void WriteText(
		bool bBold,
		COLORREF clrText,
		LPCTSTR sText
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogCtrl();

protected:
	void _InitRichEditProps();
	bool _CanUpdate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogCtrl)
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT On_WM_USR_WRITE_LOG_TEXT( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
// CLocalDateBrowser

class CLocalDateBrowser : public CExtDateBrowserWnd
{
protected:
	static LPCTSTR stat_ViewModeToText( e_view_mode_t _eVM );
	static CString stat_FormatHitTestInfo(
		const HITTESTINFO & _hitTest
		);
	void WriteLog(
		LPCTSTR strHeaderText,
		LPCTSTR strDetailsText,
		COLORREF clr
		) const;
	void WriteLog(
		CLogCtrl & wndLog,
		LPCTSTR strHeaderText,
		LPCTSTR strDetailsText,
		COLORREF clr
		) const;
public:
	bool m_bShowHoverEvents:1;
	CLocalDateBrowser()
		: m_bShowHoverEvents( false )
	{
	}
	virtual bool OnDateBrowserHoverChanging(
		const HITTESTINFO & _hitTestOld,
		const HITTESTINFO & _hitTestNew
		);
	virtual void OnDateBrowserHoverChanged(
		const HITTESTINFO & _hitTestOld,
		const HITTESTINFO & _hitTestNew
		);
	virtual CSize OnDateBrowserCalcSize(
		CDC & dcMeasure,
		const COleDateTime & _time
		) const;
	virtual bool OnDateBrowserAnimateTimeChanging(
		const COleDateTime & _timeOld,
		const COleDateTime  & _timeNew
		) const;
	virtual bool OnDateBrowserAnimateViewModeChanging(
		e_view_mode_t eVM_old,
		e_view_mode_t eVM_new
		) const;
	virtual bool OnDateBrowserTimeChanging(
		const COleDateTime & _timeOld,
		const COleDateTime & _timeNew
		);
	virtual void OnDateBrowserTimeChanged(
		const COleDateTime & _timeOld,
		const COleDateTime & _timeNew
		);
	virtual bool OnDateBrowserViewModeChanging(
		e_view_mode_t eVM_old,
		e_view_mode_t eVM_new
		);
	virtual void OnDateBrowserViewModeChanged(
		e_view_mode_t eVM_old,
		e_view_mode_t eVM_new
		);
};

/////////////////////////////////////////////////////////////////////////////
// CPageDateBrowser dialog

class CPageDateBrowser : public CPageBase
{
// Construction
public:
	DECLARE_DYNAMIC( CPageDateBrowser );
	CPageDateBrowser(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPageDateBrowser)
	enum { IDD = IDD_PAGE_DATE_BROWSER };
	CExtCheckBox	m_wndCheckShowHoverEventsInLogView;
	CExtCheckBox	m_checkShowNonYearDecadeItems;
	CExtCheckBox	m_checkShowNonCenturyItems;
	CExtCheckBox	m_checkShowOutOfRangeItems;
	CExtCheckBox	m_checkShowNonMonthDays;
	CExtComboBox	m_comboLastYear;
	CExtComboBox	m_comboFirstYear;
	CExtComboBox	m_comboFirstDayOfWeek;
	CExtComboBox	m_comboDateBrowserTheme;
	CExtGroupBox	m_gropBoxAnimationParameters;
	CExtEdit		m_editZoomStepTime;
	CExtEdit		m_editZoomStepCount;
	CExtEdit		m_editScrollStepTime;
	CExtEdit		m_editScrollStepCount;
	CExtSpinWnd		m_spinZoomStepTime;
	CExtSpinWnd		m_spinZoomStepCount;
	CExtSpinWnd		m_spinScrollStepTime;
	CExtSpinWnd		m_spinScrollStepCount;
	//}}AFX_DATA
	CExtNCSB < CLogCtrl > m_wndNotifications;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageDateBrowser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	void SetZoomAnimation( INT _stepCount, INT _stepTime );
	void SetScrollAnimation( INT _stepCount, INT _stepTime );
	void SetTheme( LPCTSTR _theme );
	void SetFirstDayOfWeek( LPCTSTR _day );

// Implementation
protected:
	HICON m_hIcon;
	CLocalDateBrowser m_wndDateBrowser;

	// Generated message map functions
	//{{AFX_MSG(CPageDateBrowser)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonContentToSize();
	afx_msg void OnButtonApZoomReset();
	afx_msg void OnButtonApScrollReset();
	afx_msg void OnSelendokComboDateBrowserTheme();
	afx_msg void OnChangeEditApScrollStepCount();
	afx_msg void OnChangeEditApScrollStepTime();
	afx_msg void OnChangeEditApZoomStepCount();
	afx_msg void OnChangeEditApZoomStepTime();
	afx_msg void OnSelendokComboFirstDayOfWeek();
	afx_msg void OnSelendokComboFirstYear();
	afx_msg void OnSelendokComboLastYear();
	afx_msg void OnCheckShowOutOfRangeItems();
	afx_msg void OnCheckShowNonMonthDays();
	afx_msg void OnCheckShowNonCenturyItems();
	afx_msg void OnCheckShowNonYearDecadeItems();
	afx_msg void OnCheckShowHoverEvents();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif  // (!defined __EXT_MFC_NO_DATE_BROWSER )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEDATETIMEDURATION_H__B15ECEDA_5E82_4F10_9A40_F6232C2EB49E__INCLUDED_)
