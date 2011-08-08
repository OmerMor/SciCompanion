// PageCalendar.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageCalendar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if (!defined __EXT_MFC_NO_DATE_PICKER)

/////////////////////////////////////////////////////////////////////////////
// CPageCalendar dialog

CPageCalendar::CPageCalendar(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageCalendar::IDD, pParent)
	, m_bInitComplete( false )
	, m_bVerificationInProcess( false )
{
	//{{AFX_DATA_INIT(CPageCalendar)
	//}}AFX_DATA_INIT
}

void CPageCalendar::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageCalendar)
	DDX_Control(pDX, IDC_CALENDAR_ROWS_MIN, m_wndRowsMin);
	DDX_Control(pDX, IDC_CALENDAR_ROWS_MAX, m_wndRowsMax);
	DDX_Control(pDX, IDC_CALENDAR_COLUMNS_MAX, m_wndColumnsMax);
	DDX_Control(pDX, IDC_CALENDAR_COLUMNS_MIN, m_wndColumnsMin);
	DDX_Control(pDX, IDC_CALENDAR_ROWS_MIN_SPIN, m_spinRowsMin);
	DDX_Control(pDX, IDC_CALENDAR_ROWS_MAX_SPIN, m_spinRowsMax);
	DDX_Control(pDX, IDC_CALENDAR_COLUMNS_MIN_SPIN, m_spinColumnsMin);
	DDX_Control(pDX, IDC_CALENDAR_COLUMNS_MAX_SPIN, m_spinColumnsMax);
	DDX_Control(pDX, IDC_STATIC_NOTIFICATIONS, m_StaticNotifications);
	DDX_Control(pDX, IDC_STATIC_R_MIN, m_staticRMin);
	DDX_Control(pDX, IDC_STATIC_R_MAX, m_staticRMax);
	DDX_Control(pDX, IDC_STATIC_C_MIN, m_staticCMin);
	DDX_Control(pDX, IDC_STATIC_C_MAX, m_staticCMax);
	DDX_Control(pDX, IDC_CHECK_RESERVE_BORDERS, m_chkReserveBorders);
	DDX_Control(pDX, IDC_CHECK_NO_HDR_BK, m_chkNoHdrBk);
	DDX_Control(pDX, IDC_CHECK_HIDE_NON_MONTH_DAYS_INSIDE, m_chkHideNonMonthDaysInside);
	DDX_Control(pDX, IDC_CHECK_ENABLE_MULTIPLE_SELECT, m_chkEnableMultipleSelect);
	DDX_Control(pDX, IDC_CHECK_BOLD_WEEK, m_chkBoldWeek);
	DDX_Control(pDX, IDC_CHECK_BOLD_POPUP, m_chkBoldPopup);
	DDX_Control(pDX, IDC_CHECK_BOLD_HEADER, m_chkBoldHeader);
	DDX_Control(pDX, IDC_CHECK_BOLD_CELL, m_chkBoldCell);
	DDX_Control(pDX, IDC_CHECK_AUTO_DIMENSIONS_MODE, m_chkAutoDimensionsMode);
	DDX_Control(pDX, IDC_CHECK_SHOW_TODAY_HIGHLIGHT, m_chkShowTodayHighlight);
	DDX_Control(pDX, IDC_CHECK_SHOW_NON_MONTH_DAYS, m_chkShowNonMonthDays);
	DDX_Control(pDX, IDC_CHECK_SHOW_MONTH_LIST_SHADOW, m_chkShowMonthListShadow);
	DDX_Control(pDX, IDC_CHECK_SHOW_BTN_TODAY, m_chkShowBtnToday);
	DDX_Control(pDX, IDC_CHECK_SHOW_BTN_SEPARATOR, m_chkShowBtnSeparator);
	DDX_Control(pDX, IDC_CHECK_SHOW_BTN_NONE, m_chkShowBtnNone);
	DDX_Control(pDX, IDC_CHECK_SHOW_BORDERS, m_chkShowBorders);
	DDX_Control(pDX, IDC_CALENDAR, m_wndDatePicker);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_NOTIFICATIONS, m_wndNotifications);
}


BEGIN_MESSAGE_MAP(CPageCalendar, CPageBase)
	//{{AFX_MSG_MAP(CPageCalendar)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(
		CExtDatePickerWnd::g_nMsgSelectionNotification,
		OnMsgSelectionNotification
	)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageCalendar message handlers

BOOL CPageCalendar::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	
//	AddAnchor( IDC_CHECK_RESERVE_BORDERS,		CSize( 0,  0) );
//	AddAnchor( IDC_CHECK_SHOW_BORDERS,			CSize( 0,  0) );

	AddAnchor( IDC_CHECK_SHOW_BTN_TODAY,		CSize( 0,  15) );
	AddAnchor( IDC_CHECK_SHOW_BTN_NONE,			CSize( 0,  15) );
	AddAnchor( IDC_CHECK_SHOW_BTN_SEPARATOR,	CSize( 0,  15) );

	AddAnchor( IDC_CHECK_BOLD_HEADER,			CSize( 0,  30) );
	AddAnchor( IDC_CHECK_BOLD_POPUP,			CSize( 0,  30) );
	AddAnchor( IDC_CHECK_BOLD_WEEK,				CSize( 0,  30) );
	AddAnchor( IDC_CHECK_BOLD_CELL,				CSize( 0,  30) );
	
	AddAnchor( IDC_CHECK_SHOW_NON_MONTH_DAYS,		CSize( 0,  45) );
	AddAnchor( IDC_CHECK_HIDE_NON_MONTH_DAYS_INSIDE,CSize( 0,  45) );

	AddAnchor( IDC_CHECK_NO_HDR_BK,				CSize( 0,  60) );

	AddAnchor( IDC_CHECK_SHOW_TODAY_HIGHLIGHT,	CSize( 0,  75) );

	AddAnchor( IDC_CHECK_SHOW_MONTH_LIST_SHADOW,CSize( 0,  90) );

	AddAnchor( IDC_CHECK_ENABLE_MULTIPLE_SELECT,CSize( 0,  100) );

	AddAnchor( IDC_CALENDAR, __RDA_LT, __RDA_RB );
	AddAnchor( IDC_NOTIFICATIONS, __RDA_LB, __RDA_RB );
	AddAnchor( IDC_STATIC_NOTIFICATIONS, __RDA_LB );

	m_wndDatePicker.m_lParamCookie = LPARAM(IDC_CALENDAR);

	//////////////////////////////////////////////////////////////////////////
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_BORDERS, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_BORDER_SHOW
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_BTN_NONE, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_BUTTON_NONE
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_BTN_TODAY, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_BUTTON_TODAY
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_BTN_SEPARATOR, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_BUTTON_SEPARATOR
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BOLD_HEADER, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_BOLD_HEADER
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BOLD_POPUP, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_BOLD_HEADER_POPUP
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BOLD_WEEK, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_BOLD_WEEK_DAYS
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BOLD_CELL, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_BOLD_DAY_CELLS
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_NON_MONTH_DAYS, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_SHOW_NON_MONTH_DAYS
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_HIDE_NON_MONTH_DAYS_INSIDE, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_HIDE_INNER_NON_MONTH_DAYS
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_RESERVE_BORDERS, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_BORDER_RESERVE
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_TODAY_HIGHLIGHT, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_SHOW_TODAY_HIGHLIGHT
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_MONTH_LIST_SHADOW, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_SHOW_MONTH_LIST_SHADOW
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_ENABLE_MULTIPLE_SELECT, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_MULTIPLE_SELECTION
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_NO_HDR_BK, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_NO_HEADER_BACKGROUND
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_AUTO_DIMENSIONS_MODE, 
		m_wndDatePicker.GetDatePickerStyle() & __EDPWS_AUTO_DIMENSION_MODE
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	GetDlgItem( IDC_CALENDAR_COLUMNS_MAX )->EnableWindow( IsDlgButtonChecked( IDC_CHECK_AUTO_DIMENSIONS_MODE ) );
	GetDlgItem( IDC_CALENDAR_ROWS_MAX )->EnableWindow( IsDlgButtonChecked( IDC_CHECK_AUTO_DIMENSIONS_MODE ) );
	GetDlgItem( IDC_CALENDAR_COLUMNS_MAX_SPIN )->EnableWindow( IsDlgButtonChecked( IDC_CHECK_AUTO_DIMENSIONS_MODE ) );
	GetDlgItem( IDC_CALENDAR_ROWS_MAX_SPIN )->EnableWindow( IsDlgButtonChecked( IDC_CHECK_AUTO_DIMENSIONS_MODE ) );
	m_spinColumnsMin.SetRange(0,10);
	m_spinColumnsMax.SetRange(0,10);
	m_spinRowsMin.SetRange(0,10);
	m_spinRowsMax.SetRange(0,10);

	GetDlgItem( IDC_CALENDAR_COLUMNS_MIN )->SetWindowText(_T("1"));
	GetDlgItem( IDC_CALENDAR_ROWS_MIN )->SetWindowText(_T("1"));

	GetDlgItem( IDC_CALENDAR_COLUMNS_MAX )->SetWindowText(_T("1"));
	GetDlgItem( IDC_CALENDAR_ROWS_MAX )->SetWindowText(_T("1"));
	
	m_bInitComplete = true;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

BOOL CPageCalendar::OnCmdMsg(UINT nID, INT nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( !m_bInitComplete ){
		return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}

	if( nCode == EN_CHANGE && !m_bVerificationInProcess)
	{
		switch( nID )
		{
		case IDC_CALENDAR_ROWS_MIN:
			{
				INT nRowsMin = GetDlgItemInt( IDC_CALENDAR_ROWS_MIN );
				if( nRowsMin < 1 ){
					nRowsMin = 1;
				}
				CSize szCalendarDimensionsMin;
				CSize szCalendarDimensionsMax;
				m_wndDatePicker.DimGet( 
					&szCalendarDimensionsMin,
					&szCalendarDimensionsMax
					);
				szCalendarDimensionsMin.cy = nRowsMin;
				m_wndDatePicker.DimSet( 
					szCalendarDimensionsMin,
					szCalendarDimensionsMax
					);
				_VerifyCalendarDimensions();
			}
			break;

		case IDC_CALENDAR_COLUMNS_MIN:
			{
				INT nColumnsMin = GetDlgItemInt( IDC_CALENDAR_COLUMNS_MIN );
				if( nColumnsMin < 1 ){
					nColumnsMin = 1;
				}
				CSize szCalendarDimensionsMin;
				CSize szCalendarDimensionsMax;
				m_wndDatePicker.DimGet( 
					&szCalendarDimensionsMin,
					&szCalendarDimensionsMax
					);
				szCalendarDimensionsMin.cx = nColumnsMin;
				m_wndDatePicker.DimSet( 
					szCalendarDimensionsMin,
					szCalendarDimensionsMax
					);
				_VerifyCalendarDimensions();
			}
			break;

		case IDC_CALENDAR_ROWS_MAX:
			{
				INT nRowsMax = GetDlgItemInt( IDC_CALENDAR_ROWS_MAX );
				if( nRowsMax < 1 ){
					nRowsMax = 1;
				}
				CSize szCalendarDimensionsMin;
				CSize szCalendarDimensionsMax;
				m_wndDatePicker.DimGet( 
					&szCalendarDimensionsMin,
					&szCalendarDimensionsMax
					);
				szCalendarDimensionsMax.cy = nRowsMax;
				m_wndDatePicker.DimSet( 
					szCalendarDimensionsMin,
					szCalendarDimensionsMax
					);
				_VerifyCalendarDimensions();
			}
			break;
			
		case IDC_CALENDAR_COLUMNS_MAX:
			{
				INT nColumnsMax = GetDlgItemInt( IDC_CALENDAR_COLUMNS_MAX );
				if( nColumnsMax < 1 ){
					nColumnsMax = 1;
				}
				CSize szCalendarDimensionsMin;
				CSize szCalendarDimensionsMax;
				m_wndDatePicker.DimGet( 
					&szCalendarDimensionsMin,
					&szCalendarDimensionsMax
					);
				szCalendarDimensionsMax.cx = nColumnsMax;
				m_wndDatePicker.DimSet( 
					szCalendarDimensionsMin,
					szCalendarDimensionsMax
					);
				_VerifyCalendarDimensions();
			}
			break;
			
		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		m_wndDatePicker.UpdateDatePickerWnd();
		return TRUE;
	}

	if( nCode == CN_COMMAND )
	{
		switch( nID )
		{
		case IDC_CHECK_SHOW_BORDERS:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_BORDER_SHOW )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_BORDER_SHOW, 0 );
			break;

		case IDC_CHECK_SHOW_BTN_TODAY:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_BUTTON_TODAY )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_BUTTON_TODAY, 0 );
			break;

		case IDC_CHECK_SHOW_BTN_NONE:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_BUTTON_NONE )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_BUTTON_NONE, 0 );
			break;

		case IDC_CHECK_SHOW_BTN_SEPARATOR:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_BUTTON_SEPARATOR )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_BUTTON_SEPARATOR, 0 );
			break;
			
		case IDC_CHECK_BOLD_HEADER:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_BOLD_HEADER )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_BOLD_HEADER, 0 );
			break;
		case IDC_CHECK_BOLD_POPUP:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_BOLD_HEADER_POPUP )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_BOLD_HEADER_POPUP, 0 );
			break;
		case IDC_CHECK_BOLD_WEEK:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_BOLD_WEEK_DAYS )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_BOLD_WEEK_DAYS, 0 );
			break;
		case IDC_CHECK_BOLD_CELL:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_BOLD_DAY_CELLS )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_BOLD_DAY_CELLS, 0 );
			break;
			
		case IDC_CHECK_HIDE_NON_MONTH_DAYS_INSIDE:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_HIDE_INNER_NON_MONTH_DAYS )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_HIDE_INNER_NON_MONTH_DAYS, 0 );
			break;

		case IDC_CHECK_RESERVE_BORDERS:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_BORDER_RESERVE )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_BORDER_RESERVE, 0 );
			break;

		case IDC_CHECK_SHOW_NON_MONTH_DAYS:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_SHOW_NON_MONTH_DAYS )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_SHOW_NON_MONTH_DAYS, 0 );
			break;
			
		case IDC_CHECK_SHOW_TODAY_HIGHLIGHT:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_SHOW_TODAY_HIGHLIGHT )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_SHOW_TODAY_HIGHLIGHT, 0 );
			break;
			
		case IDC_CHECK_SHOW_MONTH_LIST_SHADOW:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_SHOW_MONTH_LIST_SHADOW )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_SHOW_MONTH_LIST_SHADOW, 0 );
			break;
			
		case IDC_CHECK_ENABLE_MULTIPLE_SELECT:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_MULTIPLE_SELECTION )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_MULTIPLE_SELECTION, 0 );
			break;

		case IDC_CHECK_NO_HDR_BK:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_NO_HEADER_BACKGROUND )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_NO_HEADER_BACKGROUND, 0 );
			break;
			
		case IDC_CHECK_AUTO_DIMENSIONS_MODE:
			IsDlgButtonChecked( nID )
				? m_wndDatePicker.ModifyDatePickerStyle( 0, __EDPWS_AUTO_DIMENSION_MODE )
				: m_wndDatePicker.ModifyDatePickerStyle( __EDPWS_AUTO_DIMENSION_MODE, 0 );
			GetDlgItem( IDC_CALENDAR_COLUMNS_MAX )->EnableWindow( IsDlgButtonChecked( nID ) );
			GetDlgItem( IDC_CALENDAR_ROWS_MAX )->EnableWindow( IsDlgButtonChecked( nID ) );
			GetDlgItem( IDC_CALENDAR_COLUMNS_MAX_SPIN )->EnableWindow( IsDlgButtonChecked( nID ) );
			GetDlgItem( IDC_CALENDAR_ROWS_MAX_SPIN )->EnableWindow( IsDlgButtonChecked( nID ) );
			break;

		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		m_wndDatePicker.UpdateDatePickerWnd();
		return TRUE;
	}
	return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//////////////////////////////////////////////////////////////////////////

void CPageCalendar::_VerifyCalendarDimensions()
{
	m_bVerificationInProcess = true;

	CSize szCalendarDimensionsMin;
	CSize szCalendarDimensionsMax;
	m_wndDatePicker.DimGet( 
		&szCalendarDimensionsMin,
		&szCalendarDimensionsMax
	);
				
	SetDlgItemInt( IDC_CALENDAR_ROWS_MIN, szCalendarDimensionsMin.cy );
	SetDlgItemInt( IDC_CALENDAR_COLUMNS_MIN, szCalendarDimensionsMin.cx );

	SetDlgItemInt( IDC_CALENDAR_ROWS_MAX, szCalendarDimensionsMax.cy );
	SetDlgItemInt( IDC_CALENDAR_COLUMNS_MAX, szCalendarDimensionsMax.cx );

	m_bVerificationInProcess = false;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CPageCalendar::OnMsgSelectionNotification( WPARAM wParam, LPARAM lParam )
{
	lParam;
	const CExtDatePickerWnd::SELECTION_NOTIFICATION * pSN =
		CExtDatePickerWnd::SELECTION_NOTIFICATION::FromWPARAM( wParam );
	if( pSN->m_lParamCookie == LPARAM(IDC_CALENDAR) )
	{
		CString	strMsg, strMsgAll,
			strDtBegin( pSN->m_dtBegin.Format() ),
			strDtEnd( pSN->m_dtEnd.Format() );
		strMsg.Format(
			_T("Selection %s:\t%s"),
			pSN->m_bFinalSelectionChanging ? _T("changed") : _T("changing"),
			//pSN->m_lParamCookie,
			strDtBegin
			);
		if( pSN->m_dtBegin != pSN->m_dtEnd ){
			strMsg += _T(" - ");
			strMsg += strDtEnd;
		}
		strMsg += _T("\r\n");

		m_wndNotifications.GetWindowText( strMsgAll );
		m_wndNotifications.SetWindowText( strMsgAll + strMsg );
		int nCharCount = strMsgAll.GetLength() + strMsg.GetLength();
		m_wndNotifications.SetSel(nCharCount-1,nCharCount-1);
	}
	return 0L;
}

#endif // #if (!defined __EXT_MFC_NO_DATE_PICKER)
