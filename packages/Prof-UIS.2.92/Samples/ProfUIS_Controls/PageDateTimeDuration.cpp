// PageDateTimeDuration.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageDateTimeDuration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if (!defined __EXT_MFC_NO_DURATIONWND && !defined __EXT_MFC_NO_DATETIMEWND )

/////////////////////////////////////////////////////////////////////////////
// CPageDateTimeDuration dialog

CPageDateTimeDuration::CPageDateTimeDuration(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageDateTimeDuration::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageDateTimeDuration)
	m_nTimeFormat = -1;
	m_nMode = -1;
	//}}AFX_DATA_INIT
}

void CPageDateTimeDuration::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDateTimeDuration)
	DDX_Control(pDX, IDC_CHECK_READONLY_DURATION, m_wndReadOnlyDuration);
	DDX_Control(pDX, IDC_CHECK_READONLY_DATETIME, m_wndReadOnlyDateTime);
	DDX_Control(pDX, IDC_CHECK_DISABLED_DURATION, m_wndDisabledDuration);
	DDX_Control(pDX, IDC_CHECK_DISABLED_DATETIME, m_wndDisabledDateTime);
	DDX_Control(pDX, IDC_CHECK_MODE_SECONDS, m_wndModeSeconds);
	DDX_Control(pDX, IDC_CHECK_MODE_MINUTES, m_wndModeMinutes);
	DDX_Control(pDX, IDC_CHECK_MODE_HOURS, m_wndModeHours);
	DDX_Control(pDX, IDC_CHECK_MODE_DAYS, m_wndModeDays);
	DDX_Control(pDX, IDC_STATIC_NOTIFICATIONS, m_wndLabelNotifications);
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS_DATETIME, m_wndContinuousDateTime);
	DDX_Control(pDX, IDC_CHECK_CONTINUOUS_DURATION, m_wndContinuousDuration);
	DDX_Control(pDX, IDC_STATIC_TIME_FORMAT, m_wndBoxTimeFormat);
	DDX_Control(pDX, IDC_STATIC_MODE_DATETIME, m_wndBoxModeDateTime);
	DDX_Control(pDX, IDC_STATIC_MODE_DURATION, m_wndBoxModeDuration);
	DDX_Control(pDX, IDC_CHECK_BTN_SPIN_DATETIME, m_wndBtnSpinDateTime);
	DDX_Control(pDX, IDC_CHECK_BTN_SPIN_DURATION, m_wndBtnSpinDuration);
	DDX_Control(pDX, IDC_CHECK_BTN_DROP_DOWN, m_wndBtnDropDown);
	DDX_Control(pDX, IDC_CHECK_BTN_SHOW_SELECTION_ALWAYS_DATETIME, m_wndBtnShowSelectionAlwaysDateTime);
	DDX_Control(pDX, IDC_CHECK_BTN_SHOW_SELECTION_ALWAYS_DURATION, m_wndBtnShowSelectionAlwaysDuration);
	DDX_Control(pDX, IDC_TEXT_COLOR_DATETIME, m_wndColorTextDateTime);
	DDX_Control(pDX, IDC_BACK_COLOR_DATETIME, m_wndColorBackDateTime);
	DDX_Control(pDX, IDC_ADJUST_SIZE_DATETIME, m_wndAdjustSizeDateTime);
	DDX_Control(pDX, IDC_TEXT_COLOR_DURATION, m_wndColorTextDuration);
	DDX_Control(pDX, IDC_BACK_COLOR_DURATION, m_wndColorBackDuration);
	DDX_Control(pDX, IDC_ADJUST_SIZE_DURATION, m_wndAdjustSizeDuration);
	DDX_Control(pDX, IDC_STATIC_DURATION, m_wndBoxDuration);
	DDX_Control(pDX, IDC_STATIC_DATE_TIME, m_wndBoxDateTime);
	DDX_Control(pDX, IDC_DURATION, m_wndDuration);
	DDX_Control(pDX, IDC_DATETIME, m_wndDateTime);
	DDX_Radio(pDX, IDC_TIME_FORMAT_AUTO, m_nTimeFormat);
	DDX_Radio(pDX, IDC_MODE_TIME, m_nMode);
	DDX_Control(pDX, IDC_TIME_FORMAT_AUTO, m_wndTimeFormatAuto);
	DDX_Control(pDX, IDC_TIME_FORMAT_H12, m_wndTimeFormatH12);
	DDX_Control(pDX, IDC_TIME_FORMAT_H24, m_wndTimeFormatH24);
	DDX_Control(pDX, IDC_MODE_TIME, m_wndModeTime);
	DDX_Control(pDX, IDC_MODE_DATE, m_wndModeDate);
	DDX_Control(pDX, IDC_MODE_DATE_TIME, m_wndModeDateTime);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_NOTIFICATIONS, m_wndNotifications);
}

BEGIN_MESSAGE_MAP(CPageDateTimeDuration, CPageBase)
	//{{AFX_MSG_MAP(CPageDateTimeDuration)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(
		CExtDurationWnd::g_nMsgChangingNotification,
		OnSelChangedDuration
	)
	ON_REGISTERED_MESSAGE(
		CExtDateTimeWnd::g_nMsgChangingNotification,
		OnSelChangedDateTime
	)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChanged
		)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDateTimeDuration message handlers

BOOL CPageDateTimeDuration::OnInitDialog() 
{
	CPageBase::OnInitDialog();

	AddAnchor( IDC_NOTIFICATIONS, __RDA_LT, __RDA_RB );

	m_wndBoxDuration.SetStyle( CExtGroupBox::STYLE_CAPTION );
	m_wndBoxDateTime.SetStyle( CExtGroupBox::STYLE_CAPTION );
	m_wndBoxDuration.SetTextAlign( CExtGroupBox::ALIGN_CENTER );
	m_wndBoxDateTime.SetTextAlign( CExtGroupBox::ALIGN_CENTER );

	m_wndBoxTimeFormat.SetStyle( CExtGroupBox::STYLE_FLAT );
	m_wndBoxModeDateTime.SetStyle( CExtGroupBox::STYLE_FLAT );
	m_wndBoxModeDuration.SetStyle( CExtGroupBox::STYLE_FLAT );

	m_wndColorTextDateTime.m_lParamCookie = LPARAM(IDC_TEXT_COLOR_DATETIME);
	m_wndColorBackDateTime.m_lParamCookie = LPARAM(IDC_BACK_COLOR_DATETIME);
	m_wndColorTextDateTime.m_clrDefault = 
		m_wndColorTextDateTime.m_clrSelected = m_wndDateTime.GetTextColor();
	m_wndColorBackDateTime.m_clrDefault = 
		m_wndColorBackDateTime.m_clrSelected = m_wndDateTime.GetBkColor();
	
	m_wndColorTextDuration.m_lParamCookie = LPARAM(IDC_TEXT_COLOR_DURATION);
	m_wndColorBackDuration.m_lParamCookie = LPARAM(IDC_BACK_COLOR_DURATION);
	m_wndColorTextDuration.m_clrDefault = 
		m_wndColorTextDuration.m_clrSelected = m_wndDuration.GetTextColor();
	m_wndColorBackDuration.m_clrDefault = 
		m_wndColorBackDuration.m_clrSelected = m_wndDuration.GetBkColor();

	m_wndDuration.m_lParamCookie = LPARAM(IDC_DURATION);
	m_wndDateTime.m_lParamCookie = LPARAM(IDC_DATETIME);

	m_wndDateTime.SetDateTime( COleDateTime::GetCurrentTime() );

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_DISABLED_DATETIME, 
		!m_wndDateTime.IsWindowEnabled()
			? BST_CHECKED
			: BST_UNCHECKED		
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_DISABLED_DURATION, 
		!m_wndDuration.IsWindowEnabled()
			? BST_CHECKED
			: BST_UNCHECKED		
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_READONLY_DATETIME, 
		m_wndDuration.IsReadOnly()
			? BST_CHECKED
			: BST_UNCHECKED		
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_READONLY_DURATION, 
		m_wndDuration.IsReadOnly()
			? BST_CHECKED
			: BST_UNCHECKED		
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_CONTINUOUS_DATETIME, 
		m_wndDateTime.GetContinuousScrolling()
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_CONTINUOUS_DURATION, 
		m_wndDuration.GetContinuousScrolling()
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BTN_DROP_DOWN, 
		m_wndDateTime.IsDropDownButtonVisible()
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BTN_SPIN_DATETIME, 
		m_wndDateTime.IsSpinButtonVisible()
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BTN_SPIN_DURATION, 
		m_wndDuration.IsSpinButtonVisible()
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BTN_SHOW_SELECTION_ALWAYS_DURATION, 
		m_wndDateTime.NoHideSelectionGet()
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BTN_SHOW_SELECTION_ALWAYS_DATETIME, 
		m_wndDuration.NoHideSelectionGet()
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_MODE_DAYS, 
		m_wndDuration.IsItemVisible( CExtDurationWnd::day )
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_MODE_HOURS, 
		m_wndDuration.IsItemVisible( CExtDurationWnd::hour )
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_MODE_MINUTES, 
		m_wndDuration.IsItemVisible( CExtDurationWnd::minute )
		);
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_MODE_SECONDS, 
		m_wndDuration.IsItemVisible( CExtDurationWnd::second )
		);

	CExtDateTimeWnd::eMode_t eMode = m_wndDateTime.GetMode();
	switch( eMode ) 
	{
	case CExtDateTimeWnd::time:
		m_nMode = 0;
		break;
	case CExtDateTimeWnd::date:
		m_nMode = 1;
		break;
	case CExtDateTimeWnd::all:
		m_nMode = 2;
		break;
	}

	CExtDateTimeWnd::eTimeFormat_t eTimeFormat = m_wndDateTime.GetTimeFormat();
	switch( eTimeFormat ) 
	{
	case CExtDateTimeWnd::automatic:
		m_nTimeFormat = 0;
		break;
	case CExtDateTimeWnd::h24:
		m_nTimeFormat = 1;
		break;
	case CExtDateTimeWnd::h12:
		m_nTimeFormat = 2;
		break;
	}

	UpdateData( 0 );

	m_bInitComplete = true;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPageDateTimeDuration::OnSelChangedDuration( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
	lParam;
	const CExtDurationWnd::CHANGING_NOTIFICATION * pCN =
		CExtDurationWnd::CHANGING_NOTIFICATION::FromWPARAM( wParam );
	ASSERT( pCN != NULL );
	if( pCN->m_lParamCookie == LPARAM(IDC_DURATION) )
	{
		CString	strMsg, strMsgAll;
		strMsg.Format(
			_T("Duration: %s (old - %d sec, new - %d sec)"),
			pCN->m_bChangedFinally ? _T("changed") : _T("changing"),
			(LONG) pCN->m_dtSpanOld.GetTotalSeconds(),
			(LONG) pCN->m_dtSpanNew.GetTotalSeconds()
			);
		strMsg += _T("\r\n");

		m_wndNotifications.GetWindowText( strMsgAll );
		m_wndNotifications.SetWindowText( strMsgAll + strMsg );
		int nCharCount = strMsgAll.GetLength() + strMsg.GetLength();
		m_wndNotifications.SetSel(nCharCount-1,nCharCount-1);
	}
	return 0L;
}

LRESULT CPageDateTimeDuration::OnSelChangedDateTime( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
	lParam;
	const CExtDateTimeWnd::CHANGING_NOTIFICATION * pCN =
		CExtDateTimeWnd::CHANGING_NOTIFICATION::FromWPARAM( wParam );
	ASSERT( pCN != NULL );
	if( pCN->m_lParamCookie == LPARAM(IDC_DATETIME) )
	{
		CString	strMsg, strMsgAll;
		strMsg.Format(
			_T("DateTime: %s (old - %s, new - %s)"),
			pCN->m_bChangedFinally ? _T("changed") : _T("changing"),
			pCN->m_dtDateTimeOld.Format(),
			pCN->m_dtDateTimeNew.Format()
			);
		strMsg += _T("\r\n");

		m_wndNotifications.GetWindowText( strMsgAll );
		m_wndNotifications.SetWindowText( strMsgAll + strMsg );
		int nCharCount = strMsgAll.GetLength() + strMsg.GetLength();
		m_wndNotifications.SetSel(nCharCount-1,nCharCount-1);
	}
	return 0L;
}

LRESULT CPageDateTimeDuration::OnColorChanged(WPARAM wParam, LPARAM lParam)
{
	COLORREF clrColor = COLORREF(wParam);
	switch( lParam )
	{
	case IDC_TEXT_COLOR_DATETIME:
		m_wndDateTime.SetTextColor( clrColor );
		break;
	case IDC_BACK_COLOR_DATETIME:
		m_wndDateTime.SetBkColor( clrColor );
		break;
	case IDC_TEXT_COLOR_DURATION:
		m_wndDuration.SetTextColor( clrColor );
		break;
	case IDC_BACK_COLOR_DURATION:
		m_wndDuration.SetBkColor( clrColor );
		break;
	default:
		break;
	} // switch( lParam )
	return 0;
}

BOOL CPageDateTimeDuration::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( !m_bInitComplete )
		return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	
	if( nCode == CN_COMMAND )
	{
		bool bChecked = IsDlgButtonChecked( nID ) ? true : false;
		switch( nID )
		{
		case IDC_CHECK_DISABLED_DATETIME:
			m_wndDateTime.EnableWindow( !bChecked );
			break;
		case IDC_CHECK_DISABLED_DURATION:
			m_wndDuration.EnableWindow( !bChecked );
			break;

		case IDC_CHECK_READONLY_DATETIME:
			m_wndDateTime.SetReadOnly( bChecked );
			break;
		case IDC_CHECK_READONLY_DURATION:
			m_wndDuration.SetReadOnly( bChecked );
			break;

		case IDC_CHECK_CONTINUOUS_DATETIME:
			m_wndDateTime.SetContinuousScrolling( bChecked );
			break;
		case IDC_CHECK_CONTINUOUS_DURATION:
			m_wndDuration.SetContinuousScrolling( bChecked );
			break;
			
		case IDC_CHECK_BTN_DROP_DOWN:
			m_wndDateTime.ShowDropDownButton( bChecked );
			break;
		case IDC_CHECK_BTN_SPIN_DATETIME:
			m_wndDateTime.ShowSpinButton( bChecked );
			break;
		case IDC_CHECK_BTN_SPIN_DURATION:
			m_wndDuration.ShowSpinButton( bChecked );
			break;
		case IDC_CHECK_BTN_SHOW_SELECTION_ALWAYS_DATETIME:
			m_wndDateTime.NoHideSelectionSet( bChecked );
			break;
		case IDC_CHECK_BTN_SHOW_SELECTION_ALWAYS_DURATION:
			m_wndDuration.NoHideSelectionSet( bChecked );
			break;

		case IDC_ADJUST_SIZE_DATETIME:   
			m_wndDateTime.AdjustSize();
			m_wndBoxDateTime.Invalidate();
			break;
		case IDC_ADJUST_SIZE_DURATION:
			m_wndDuration.AdjustSize();
			m_wndBoxDuration.Invalidate();
			break;
			
		case IDC_TIME_FORMAT_AUTO:
		case IDC_TIME_FORMAT_H24:
		case IDC_TIME_FORMAT_H12:
			{
				UpdateData();
				CExtDateTimeWnd::eTimeFormat_t eTimeFormat = CExtDateTimeWnd::automatic;
				switch( m_nTimeFormat ) 
				{
				case 0:
					eTimeFormat = CExtDateTimeWnd::automatic;
					break;
				case 1:
					eTimeFormat = CExtDateTimeWnd::h24;
					break;
				case 2:
					eTimeFormat = CExtDateTimeWnd::h12;
					break;
				}
				m_wndDateTime.SetTimeFormat( eTimeFormat );
			}
			break;
		
		case IDC_MODE_TIME:
		case IDC_MODE_DATE:
		case IDC_MODE_DATE_TIME:
			{
				UpdateData();
				CExtDateTimeWnd::eMode_t eMode = CExtDateTimeWnd::all;
				switch( m_nMode ) 
				{
				case 0:
					eMode = CExtDateTimeWnd::time;
					break;
				case 1:
					eMode = CExtDateTimeWnd::date;
					break;
				case 2:
					eMode = CExtDateTimeWnd::all;
					break;
				}
				m_wndDateTime.SetMode( eMode );
			}
			break;

		case IDC_CHECK_MODE_DAYS:
			m_wndDuration.SetShowItem( CExtDurationWnd::day, bChecked );
			break;
		case IDC_CHECK_MODE_HOURS:
			m_wndDuration.SetShowItem( CExtDurationWnd::hour, bChecked );
			break;
		case IDC_CHECK_MODE_MINUTES:
			m_wndDuration.SetShowItem( CExtDurationWnd::minute, bChecked );
			break;
		case IDC_CHECK_MODE_SECONDS:
			m_wndDuration.SetShowItem( CExtDurationWnd::second, bChecked );
			break;

		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		return TRUE;
	}
	
	return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

#endif // (!defined __EXT_MFC_NO_DURATIONWND && !defined __EXT_MFC_NO_DATETIMEWND )

#if (!defined __EXT_MFC_NO_DATE_BROWSER )

/////////////////////////////////////////////////////////////////////////////
// CLogCtrl

CLogCtrl::CLogCtrl()
	: m_dwHelperThreadID( ::GetCurrentThreadId() )
	, m_nLineCountMax( 32000 )
{
}

CLogCtrl::~CLogCtrl()
{
}


BEGIN_MESSAGE_MAP(CLogCtrl,CRichEditCtrl)
	//{{AFX_MSG_MAP(CLogCtrl)
	ON_WM_TIMER()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USR_WRITE_LOG_TEXT, On_WM_USR_WRITE_LOG_TEXT )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLogCtrl message handlers

BOOL CLogCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ! CRichEditCtrl :: PreCreateWindow(cs) )
		return FALSE;
	cs.style &= ~WS_BORDER;
	return TRUE;
}

void CLogCtrl::PreSubclassWindow() 
{
	CRichEditCtrl::PreSubclassWindow();
	_InitRichEditProps();
}

void CLogCtrl::OnTimer(__EXT_MFC_UINT_PTR nIDEvent) 
{
	if( nIDEvent == ID_TIMER_DELAYED_UPDATE )
	{
		if( _CanUpdate() )
		{
			KillTimer( ID_TIMER_DELAYED_UPDATE );
			Invalidate();
			UpdateWindow();
		}
		return;
	} // if( nIDEvent == ID_TIMER_DELAYED_UPDATE )
	
	CRichEditCtrl::OnTimer(nIDEvent);
}

void CLogCtrl::OnEditCopy() 
{
CHARRANGE _cr;
	GetSel( _cr );
	if( _cr.cpMin != _cr.cpMax )
		CRichEditCtrl::Copy();
}

void CLogCtrl::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
CHARRANGE _cr;
	GetSel( _cr );
	pCmdUI->Enable(
		( _cr.cpMin != _cr.cpMax ) ? TRUE : FALSE
		);
}

LRESULT CLogCtrl::On_WM_USR_WRITE_LOG_TEXT( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
	wParam;
_MthMessageDataFor_WM_USR_WRITE_LOG_TEXT & _data =
	_MthMessageDataFor_WM_USR_WRITE_LOG_TEXT::FromLPARAM( lParam );
	WriteText(
		_data.m_bBold,
		_data.m_clrText,
		_data.m_sText
		);
	return 0L;
}

void CLogCtrl::_InitRichEditProps()
{
PARAFORMAT _pf;
	_pf.cbSize = sizeof(PARAFORMAT);
	_pf.dwMask = PFM_TABSTOPS ;
	_pf.cTabCount = MAX_TAB_STOPS;
	for( int nTabIdx = 0 ; nTabIdx < _pf.cTabCount; nTabIdx++ )
		_pf.rgxTabs[nTabIdx] = (nTabIdx + 1) * 1440/5;
	CRichEditCtrl::SetParaFormat( _pf );

CHARFORMAT _cf;
	memset( &_cf, 0, sizeof(CHARFORMAT) );
	_cf.cbSize = sizeof(_cf);
	_cf.dwEffects = 0; // CFE_PROTECTED; 
	_cf.dwMask =
		CFM_FACE|CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT
		|CFM_SIZE|CFM_CHARSET|CFM_PROTECTED|CFM_COLOR;
	_cf.yHeight = 150;
	_cf.bCharSet = DEFAULT_CHARSET;
#if _MFC_VER < 0x700
	strcpy( _cf.szFaceName, "Courier New" );
#else
	__EXT_MFC_STRCPY( _cf.szFaceName, LF_FACESIZE, _T("Courier New") );
#endif
	_cf.crTextColor = RGB(0,0,0);
	CRichEditCtrl::SetDefaultCharFormat( _cf );
	CRichEditCtrl::SetWordCharFormat( _cf );
	
	CRichEditCtrl::SetBackgroundColor( FALSE, RGB(255,255,255) );
	
	CRichEditCtrl::SetSel( -1, -1 );
}

bool CLogCtrl::_CanUpdate()
{
	if( CExtControlBar::FindHwndNotSafeForMutualPaint(this) != NULL )
		return false;
	if( CExtPopupMenuWnd::g_bMenuWithShadows )
	{
		if( CExtPopupMenuWnd::IsMenuTracking() )
		{
			CExtPopupMenuWnd * pPopup = CExtPopupMenuSite::g_DefPopupMenuSite.GetInstance();
			ASSERT_VALID( pPopup );
			if( pPopup->_IsPopupWithShadows() )
				return false;
		}
		else if( CExtToolControlBar::g_bMenuTracking )
			return false;
	} // if( CExtPopupMenuWnd::g_bMenuWithShadows )
CExtPopupMenuTipWnd & _ATTW = CExtPopupMenuSite::g_DefPopupMenuSite.GetTip();
	if(		_ATTW.GetSafeHwnd() != NULL
		&&	(_ATTW.GetStyle()&WS_VISIBLE) != 0
		&&	_ATTW.m_ctrlShadow.GetSafeHwnd() == NULL
		&&	_ATTW.GetShadowSize() > 0
		)
		return false;
	return true;
}

void CLogCtrl::WriteText(
	bool bBold,
	COLORREF clrText,
	LPCTSTR sText
	)
{
	ASSERT( this != NULL );
	if( m_dwHelperThreadID != ::GetCurrentThreadId() )
	{
		_MthMessageDataFor_WM_USR_WRITE_LOG_TEXT _data(
			bBold,
			clrText,
			sText
			);
		SendMessage(
			WM_USR_WRITE_LOG_TEXT,
			WPARAM(0),
			_data
			);
		return;
	}
	
	ASSERT_VALID( this );
	if(		sText == NULL
		||	_tcslen( sText ) == 0
		)
		return;

	CRichEditCtrl::LineScroll( CRichEditCtrl::GetLineCount() );

bool bCanUpdate = _CanUpdate();
	SetRedraw( FALSE );

int nFirstLine1 = CRichEditCtrl::GetFirstVisibleLine();
CHARRANGE crOld1, crOld2;
	CRichEditCtrl::GetSel( crOld1 );
	CRichEditCtrl::SetSel( -1, -1 );
	CRichEditCtrl::GetSel( crOld2 );
bool bResetSel = true;
	if(		crOld1.cpMin == crOld2.cpMin
		&&	crOld1.cpMax == crOld2.cpMax
		)
		bResetSel = false;
	CRichEditCtrl::ReplaceSel( sText );

CHARFORMAT _cf;
	_cf.cbSize = sizeof(CHARFORMAT);
	_cf.dwMask = CFM_COLOR|CFM_BOLD;
	_cf.dwEffects = bBold ? CFE_BOLD : 0;
	_cf.crTextColor = clrText;
	CRichEditCtrl::SetSel( crOld2.cpMax, -1 );
	CRichEditCtrl::SetSelectionCharFormat( _cf );

	if( bResetSel )
	{
		CRichEditCtrl::SetSel( crOld1 );
		int nFirstLine2 = CRichEditCtrl::GetFirstVisibleLine();
		if( nFirstLine1 != nFirstLine2 )
			LineScroll( nFirstLine1-nFirstLine2, 0) ;
	} // if( bResetSel )

	if( m_nLineCountMax > 0 )
	{
		int nLineCount = CRichEditCtrl::GetLineCount();
		if( nLineCount > m_nLineCountMax )
		{
			int nEndRemovePos =
				CRichEditCtrl::LineIndex(
					nLineCount > m_nLineCountMax 
					);
			CHARRANGE crRestore;
			CRichEditCtrl::GetSel( crRestore );
			crRestore.cpMin -= nEndRemovePos;
			crRestore.cpMax -= nEndRemovePos;
			CRichEditCtrl::SetSel( 0, nEndRemovePos );
			CRichEditCtrl::ReplaceSel( _T("") );
			CRichEditCtrl::SetSel( crRestore );
		}
	} // if( m_nLineCountMax > 0 )

	if( ! bResetSel )
		CRichEditCtrl::SetSel( -1, -1 );

	SetRedraw( TRUE );
	if( bCanUpdate )
		Invalidate();
	else
		SetTimer(
			ID_TIMER_DELAYED_UPDATE,
			ELAPSE_TIMER_DELAYED_UPDATE,
			NULL
			);
}

//////////////////////////////////////////////////////////////////////////
// CLocalDateBrowser

LPCTSTR CLocalDateBrowser::stat_ViewModeToText( CExtDateBrowserWnd::e_view_mode_t _eVM )
{
	switch( _eVM )
	{
	case __EVM_CENTURY:
		return _T("century");
	case __EVM_YEAR_DECADE:
		return _T("year decade");
	case __EVM_ONE_YEAR:
		return _T("one year");
	case __EVM_MONTH:
		return _T("month");
	break;
#ifdef _DEBUG
	default:
		ASSERT( FALSE );
	break;
#endif // _DEBUG
	} // switch( _eVM )
	return _T("unknown");
}

void CLocalDateBrowser::WriteLog(
	LPCTSTR strHeaderText,
	LPCTSTR strDetailsText,
	COLORREF clr
	) const
{
	if(		( strHeaderText == NULL || _tcslen(strHeaderText) == 0 )
		&&	( strDetailsText == NULL || _tcslen(strDetailsText) == 0 )
		)
		return;
CPageDateBrowser * pDlg = DYNAMIC_DOWNCAST( CPageDateBrowser, GetParent() );
	if(		pDlg == NULL
		||	pDlg->m_wndNotifications.GetSafeHwnd() == NULL
		)
		return;
	WriteLog( pDlg->m_wndNotifications, strHeaderText, strDetailsText, clr );
}

void CLocalDateBrowser::WriteLog(
	CLogCtrl & wndLog,
	LPCTSTR strHeaderText,
	LPCTSTR strDetailsText,
	COLORREF clr
	) const
{
	if(		( strHeaderText == NULL || _tcslen(strHeaderText) == 0 )
		&&	( strDetailsText == NULL || _tcslen(strDetailsText) == 0 )
		)
		return;
COLORREF clrHeader      = CExtBitmap::stat_HLS_Adjust( clr, 0.0, -0.20, 0.0 );
COLORREF clrDescription = CExtBitmap::stat_HLS_Adjust( clr, 0.0, +0.20, 0.0 );
	if( wndLog.GetWindowTextLength() > 0 )
		wndLog.WriteText( false, RGB(0,0,0), _T("\r\n") );
	wndLog.WriteText( true, clrHeader, strHeaderText );
	wndLog.WriteText( false, RGB(0,0,0), _T("\r\n") );
	wndLog.WriteText( false, clrDescription, strDetailsText );
}

CString CLocalDateBrowser::stat_FormatHitTestInfo(
	const CExtDateBrowserWnd::HITTESTINFO & _hitTest
	)
{
CString strHitTestDescription = _T("nowhere");
	switch( INT(_hitTest.m_nAreaFlags & __EHTIAF_INSIDE_CONTROL_MASK) )
	{
	case INT(__EHTIAF_ON_BUTTON_PREVIOUS):
		strHitTestDescription = _T("previous button");
	break;
	case INT(__EHTIAF_ON_BUTTON_NEXT):
		strHitTestDescription = _T("next button");
	break;
	case INT(__EHTIAF_ON_CAPTION):
		strHitTestDescription = _T("caption");
	break;
	case INT(__EHTIAF_ON_ITEM):
		strHitTestDescription = _T("palette item");
		if( ( _hitTest.m_nAreaFlags & __EHTIAF_ON_MONTH_HEADER_ITEM ) != 0 )
			strHitTestDescription += _T(" (header item)");
		if( _hitTest.m_time.GetStatus() == COleDateTime::valid )
		{
			strHitTestDescription += _T(", date is ");
			strHitTestDescription += _hitTest.m_time.Format( VAR_DATEVALUEONLY, LOCALE_USER_DEFAULT );
		}
	break;
	} // switch( INT(_hitTest.m_nAreaFlags & __EHTIAF_INSIDE_CONTROL_MASK) )
	if( ( _hitTest.m_nAreaFlags & __EHTIAF_OUTSIDE_AT_TOP ) != 0 )
			strHitTestDescription += _T(" (outside at top)");
	if( ( _hitTest.m_nAreaFlags & __EHTIAF_OUTSIDE_AT_BOTTOM ) != 0 )
			strHitTestDescription += _T(" (outside at bottom)");
	if( ( _hitTest.m_nAreaFlags & __EHTIAF_OUTSIDE_AT_LEFT ) != 0 )
			strHitTestDescription += _T(" (outside at left)");
	if( ( _hitTest.m_nAreaFlags & __EHTIAF_OUTSIDE_AT_RIGHT ) != 0 )
			strHitTestDescription += _T(" (outside at right)");
	return strHitTestDescription;
}

bool CLocalDateBrowser::OnDateBrowserHoverChanging(
	const CExtDateBrowserWnd::HITTESTINFO & _hitTestOld,
	const CExtDateBrowserWnd::HITTESTINFO & _hitTestNew
	)
{
	ASSERT_VALID( this );
bool bRetVal = CExtDateBrowserWnd::OnDateBrowserHoverChanging( _hitTestOld, _hitTestNew );
	if( m_bShowHoverEvents )
	{
		CString strDetalisText;
		CString strOldHitTest = stat_FormatHitTestInfo( _hitTestOld );
		CString strNewHitTest = stat_FormatHitTestInfo( _hitTestNew );
		strDetalisText.Format(
			_T("    old hit test = %s\r\n    new hit test = %s\r\n    return value = %s"),
			LPCTSTR(strOldHitTest),
			LPCTSTR(strNewHitTest),
			bRetVal ? _T("true") : _T("false")
			);
		WriteLog( _T("OnDateBrowserHoverChanging() query"), LPCTSTR(strDetalisText), RGB(172,172,172) );
	} // if( m_bShowHoverEvents )
	return bRetVal;
}

void CLocalDateBrowser::OnDateBrowserHoverChanged(
	const CExtDateBrowserWnd::HITTESTINFO & _hitTestOld,
	const CExtDateBrowserWnd::HITTESTINFO & _hitTestNew
	)
{
	ASSERT_VALID( this );
	CExtDateBrowserWnd::OnDateBrowserHoverChanged( _hitTestOld, _hitTestNew );
	if( m_bShowHoverEvents )
	{
		CString strDetalisText;
		CString strOldHitTest = stat_FormatHitTestInfo( _hitTestOld );
		CString strNewHitTest = stat_FormatHitTestInfo( _hitTestNew );
		strDetalisText.Format(
			_T("    old hit test = %s\r\n    new hit test = %s"),
			LPCTSTR(strOldHitTest),
			LPCTSTR(strNewHitTest)
			);
		WriteLog( _T("OnDateBrowserHoverChanged() notification"), LPCTSTR(strDetalisText), RGB(112,112,112) );
	} // if( m_bShowHoverEvents )
}

CSize CLocalDateBrowser::OnDateBrowserCalcSize(
	CDC & dcMeasure,
	const COleDateTime & _time
	) const
{
	ASSERT_VALID( this );
CSize _size = CExtDateBrowserWnd::OnDateBrowserCalcSize( dcMeasure, _time );
CString strDetalisText;
CString strTime = _time.Format( VAR_DATEVALUEONLY, LOCALE_USER_DEFAULT );
	strDetalisText.Format(
		_T("    date = %s\r\n    return value = CSize( cx = %d, cy = %d)"),
		LPCTSTR(strTime),
		INT(_size.cx),
		INT(_size.cy)
		);
	WriteLog( _T("OnCalcSize() query"), LPCTSTR(strDetalisText), RGB(192,0,0) );
	return _size;
}

bool CLocalDateBrowser::OnDateBrowserAnimateTimeChanging(
	const COleDateTime & _timeOld,
	const COleDateTime  & _timeNew
	) const
{
	ASSERT_VALID( this );
bool bRetVal = CExtDateBrowserWnd::OnDateBrowserAnimateTimeChanging( _timeOld, _timeNew );
CString strDetalisText;
CString strTimeOld = _timeOld.Format( VAR_DATEVALUEONLY, LOCALE_USER_DEFAULT );
CString strTimeNew = _timeNew.Format( VAR_DATEVALUEONLY, LOCALE_USER_DEFAULT );
	strDetalisText.Format(
		_T("    date old = %s    date new = %s\r\n    return value = %s"),
		LPCTSTR(strTimeOld),
		LPCTSTR(strTimeNew),
		bRetVal ? _T("true") : _T("false")
		);
	WriteLog( _T("OnAnimateTimeChanging() query"), LPCTSTR(strDetalisText), RGB(0,144,144) );
	return bRetVal;
}

bool CLocalDateBrowser::OnDateBrowserAnimateViewModeChanging(
	e_view_mode_t eVM_old,
	e_view_mode_t eVM_new
	) const
{
	ASSERT_VALID( this );
bool bRetVal = CExtDateBrowserWnd::OnDateBrowserAnimateViewModeChanging( eVM_old, eVM_new );
CString strDetalisText;
	strDetalisText.Format(
		_T("    old mode = %s    new mode = %s\r\n    return value = %s"),
		stat_ViewModeToText( eVM_old ),
		stat_ViewModeToText( eVM_new ),
		bRetVal ? _T("true") : _T("false")
		);
	WriteLog( _T("OnAnimateViewModeChanging() query"), LPCTSTR(strDetalisText), RGB(144,144,0) );
	return bRetVal;
}

bool CLocalDateBrowser::OnDateBrowserTimeChanging(
	const COleDateTime & _timeOld,
	const COleDateTime & _timeNew
	)
{
	ASSERT_VALID( this );
bool bRetVal = CExtDateBrowserWnd::OnDateBrowserTimeChanging( _timeOld, _timeNew );
CString strDetalisText;
CString strTimeOld = _timeOld.Format( VAR_DATEVALUEONLY, LOCALE_USER_DEFAULT );
CString strTimeNew = _timeNew.Format( VAR_DATEVALUEONLY, LOCALE_USER_DEFAULT );
	strDetalisText.Format(
		_T("    date old = %s    date new = %s\r\n    return value = %s"),
		LPCTSTR(strTimeOld),
		LPCTSTR(strTimeNew),
		bRetVal ? _T("true") : _T("false")
		);
	WriteLog( _T("OnTimeChanging() query"), LPCTSTR(strDetalisText), RGB(0,0,192) );
	return bRetVal;
}

void CLocalDateBrowser::OnDateBrowserTimeChanged(
	const COleDateTime & _timeOld,
	const COleDateTime & _timeNew
	)
{
	ASSERT_VALID( this );
	CExtDateBrowserWnd::OnDateBrowserTimeChanged( _timeOld, _timeNew );
CString strDetalisText;
CString strTimeOld = _timeOld.Format( VAR_DATEVALUEONLY, LOCALE_USER_DEFAULT );
CString strTimeNew = _timeNew.Format( VAR_DATEVALUEONLY, LOCALE_USER_DEFAULT );
	strDetalisText.Format(
		_T("    date old = %s    date new = %s"),
		LPCTSTR(strTimeOld),
		LPCTSTR(strTimeNew)
		);
	WriteLog( _T("OnTimeChanged() notification"), LPCTSTR(strDetalisText), RGB(0,0,144) );
}

bool CLocalDateBrowser::OnDateBrowserViewModeChanging(
	CExtDateBrowserWnd::e_view_mode_t eVM_old,
	CExtDateBrowserWnd::e_view_mode_t eVM_new
	)
{
	ASSERT_VALID( this );
bool bRetVal = CExtDateBrowserWnd::OnDateBrowserViewModeChanging( eVM_old, eVM_new );
CString strDetalisText;
	strDetalisText.Format(
		_T("    old mode = %s    new mode = %s\r\n    return value = %s"),
		stat_ViewModeToText( eVM_old ),
		stat_ViewModeToText( eVM_new ),
		bRetVal ? _T("true") : _T("false")
		);
	WriteLog( _T("OnViewModeChanging() query"), LPCTSTR(strDetalisText), RGB(0,192,0) );
	return bRetVal;
}

void CLocalDateBrowser::OnDateBrowserViewModeChanged(
	CExtDateBrowserWnd::e_view_mode_t eVM_old,
	CExtDateBrowserWnd::e_view_mode_t eVM_new
	)
{
	ASSERT_VALID( this );
	CExtDateBrowserWnd::OnDateBrowserViewModeChanged( eVM_old, eVM_new );
CString strDetalisText;
	strDetalisText.Format(
		_T("    old mode = %s    new mode = %s"),
		stat_ViewModeToText( eVM_old ),
		stat_ViewModeToText( eVM_new )
		);
	WriteLog( _T("OnViewModeChanged() notification"), LPCTSTR(strDetalisText), RGB(0,144,0) );
}

/////////////////////////////////////////////////////////////////////////////
// CPageDateBrowser dialog

IMPLEMENT_DYNAMIC( CPageDateBrowser, CPageBase );

CPageDateBrowser::CPageDateBrowser(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageDateBrowser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageDateBrowser)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

void CPageDateBrowser::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDateBrowser)
	DDX_Control(pDX, IDC_CHECK_SHOW_HOVER_EVENTS, m_wndCheckShowHoverEventsInLogView);
	DDX_Control(pDX, IDC_CHECK_SHOW_NON_YEAR_DECADE_ITEMS, m_checkShowNonYearDecadeItems);
	DDX_Control(pDX, IDC_CHECK_SHOW_NON_CENTURY_ITEMS, m_checkShowNonCenturyItems);
	DDX_Control(pDX, IDC_CHECK_SHOW_OUT_OF_RANGE_ITEMS, m_checkShowOutOfRangeItems);
	DDX_Control(pDX, IDC_CHECK_SHOW_NON_MONTH_DAYS, m_checkShowNonMonthDays);
	DDX_Control(pDX, IDC_COMBO_LAST_YEAR, m_comboLastYear);
	DDX_Control(pDX, IDC_COMBO_FIRST_YEAR, m_comboFirstYear);
	DDX_Control(pDX, IDC_COMBO_FIRST_DAY_OF_WEEK, m_comboFirstDayOfWeek);
	DDX_Control(pDX, IDC_COMBO_DATE_BROWSER_THEME, m_comboDateBrowserTheme);
	DDX_Control(pDX, IDC_GROUP_BOX_AP, m_gropBoxAnimationParameters);
	DDX_Control(pDX, IDC_EDIT_AP_ZOOM_STEP_TIME, m_editZoomStepTime);
	DDX_Control(pDX, IDC_EDIT_AP_ZOOM_STEP_COUNT, m_editZoomStepCount);
	DDX_Control(pDX, IDC_EDIT_AP_SCROLL_STEP_TIME, m_editScrollStepTime);
	DDX_Control(pDX, IDC_EDIT_AP_SCROLL_STEP_COUNT, m_editScrollStepCount);
	DDX_Control(pDX, IDC_SPIN_AP_ZOOM_STEP_TIME, m_spinZoomStepTime);
	DDX_Control(pDX, IDC_SPIN_AP_ZOOM_STEP_COUNT, m_spinZoomStepCount);
	DDX_Control(pDX, IDC_SPIN_AP_SCROLL_STEP_TIME, m_spinScrollStepTime);
	DDX_Control(pDX, IDC_SPIN_AP_SCROLL_STEP_COUNT, m_spinScrollStepCount);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_NOTIFICATIONS, m_wndNotifications);
	DDX_Control( pDX, IDC_CUSTOM1, m_wndDateBrowser );
}

BEGIN_MESSAGE_MAP(CPageDateBrowser, CDialog)
	//{{AFX_MSG_MAP(CPageDateBrowser)
	ON_BN_CLICKED(IDC_BUTTON_CONTENT_TO_SIZE, OnButtonContentToSize)
	ON_BN_CLICKED(IDC_BUTTON_AP_ZOOM_RESET, OnButtonApZoomReset)
	ON_BN_CLICKED(IDC_BUTTON_AP_SCROLL_RESET, OnButtonApScrollReset)
	ON_CBN_SELENDOK(IDC_COMBO_DATE_BROWSER_THEME, OnSelendokComboDateBrowserTheme)
	ON_EN_CHANGE(IDC_EDIT_AP_SCROLL_STEP_COUNT, OnChangeEditApScrollStepCount)
	ON_EN_CHANGE(IDC_EDIT_AP_SCROLL_STEP_TIME, OnChangeEditApScrollStepTime)
	ON_EN_CHANGE(IDC_EDIT_AP_ZOOM_STEP_COUNT, OnChangeEditApZoomStepCount)
	ON_EN_CHANGE(IDC_EDIT_AP_ZOOM_STEP_TIME, OnChangeEditApZoomStepTime)
	ON_CBN_SELENDOK(IDC_COMBO_FIRST_DAY_OF_WEEK, OnSelendokComboFirstDayOfWeek)
	ON_CBN_SELENDOK(IDC_COMBO_FIRST_YEAR, OnSelendokComboFirstYear)
	ON_CBN_SELENDOK(IDC_COMBO_LAST_YEAR, OnSelendokComboLastYear)
	ON_BN_CLICKED(IDC_CHECK_SHOW_OUT_OF_RANGE_ITEMS, OnCheckShowOutOfRangeItems)
	ON_BN_CLICKED(IDC_CHECK_SHOW_NON_MONTH_DAYS, OnCheckShowNonMonthDays)
	ON_BN_CLICKED(IDC_CHECK_SHOW_NON_CENTURY_ITEMS, OnCheckShowNonCenturyItems)
	ON_BN_CLICKED(IDC_CHECK_SHOW_NON_YEAR_DECADE_ITEMS, OnCheckShowNonYearDecadeItems)
	ON_BN_CLICKED(IDC_CHECK_SHOW_HOVER_EVENTS, OnCheckShowHoverEvents)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDateBrowser message handlers

BOOL CPageDateBrowser::OnInitDialog()
{
	CPageBase::OnInitDialog();

	m_gropBoxAnimationParameters.SetStyle( CExtGroupBox::STYLE_CAPTION );

	OnButtonApZoomReset();

	OnButtonApScrollReset();
	m_comboFirstDayOfWeek.SetCurSel( 0 );
	m_comboFirstYear.SelectString( -1, _T("1900") );
	m_comboLastYear.SelectString( -1, _T("2099") );

	m_comboDateBrowserTheme.SetCurSel( 0 );
	//m_comboDateBrowserTheme.SetCurSel( 1 );
	//SetTheme( _T("Skin") );

	m_checkShowNonCenturyItems.SetCheck( 1 );
	m_checkShowNonYearDecadeItems.SetCheck( 1 );
	m_checkShowNonMonthDays.SetCheck( 1 );
	m_checkShowOutOfRangeItems.SetCheck( 0 );
	
	m_spinScrollStepCount.SetRange( 0, 50 );
	m_spinScrollStepTime.SetRange( 0, 50 );
	m_spinZoomStepCount.SetRange( 0, 50 );
	m_spinZoomStepTime.SetRange( 0, 50 );

	AddAnchor( IDC_NOTIFICATIONS, __RDA_LT, __RDA_RB );

	return TRUE; 
}

void CPageDateBrowser::OnButtonContentToSize() 
{
CSize _size = m_wndDateBrowser.CalcSize();
CRect rc;
	m_wndDateBrowser.GetWindowRect( &rc );
	if( rc.Size() == _size )
	{
		::AfxMessageBox( _T("Size is OK.") );
		return;
	}
	ScreenToClient( &rc );
	rc.right = rc.left + _size.cx;
	rc.bottom = rc.top + _size.cy;
	m_wndDateBrowser.MoveWindow( &rc );
}

void CPageDateBrowser::OnButtonApZoomReset() 
{
	SetZoomAnimation( __EXT_DATE_BROWSER_ANIMATION_ZOOM_STEP_COUNT, __EXT_DATE_BROWSER_ANIMATION_ZOOM_STEP_TIME );
}

void CPageDateBrowser::OnButtonApScrollReset() 
{
	SetScrollAnimation( __EXT_DATE_BROWSER_ANIMATION_SCROLL_STEP_COUNT, __EXT_DATE_BROWSER_ANIMATION_SCROLL_STEP_TIME );
}

void CPageDateBrowser::OnSelendokComboDateBrowserTheme() 
{
	if( m_comboDateBrowserTheme.GetSafeHwnd() == NULL )
		return;
CString s;
	m_comboDateBrowserTheme.GetWindowText( s );
	SetTheme( s );
}

void CPageDateBrowser::OnChangeEditApScrollStepCount() 
{
	if( m_editScrollStepCount.GetSafeHwnd() == NULL )
		return;
CString s;
	m_editScrollStepCount.GetWindowText( s );
	if( ! s.IsEmpty() )
	{
		if( _ttol(s) > 50 )
			s = _T("50");
		SetScrollAnimation( _ttol(s), -1 );
	}
}

void CPageDateBrowser::OnChangeEditApScrollStepTime() 
{
	if( m_editScrollStepTime.GetSafeHwnd() == NULL )
		return;
CString s;
	m_editScrollStepTime.GetWindowText( s );
	if( ! s.IsEmpty() )
	{
		if( _ttol(s) > 50 )
			s = _T("50");
		SetScrollAnimation( -1, _ttol(s) );
	}
}

void CPageDateBrowser::OnChangeEditApZoomStepCount() 
{
	if( m_editZoomStepCount.GetSafeHwnd() == NULL )
		return;
CString s;
	m_editZoomStepCount.GetWindowText( s );
	if( ! s.IsEmpty() )
	{
		if( _ttol(s) > 50 )
			s = _T("50");
		SetZoomAnimation( _ttol(s), -1 );
	}
}

void CPageDateBrowser::OnChangeEditApZoomStepTime() 
{
	if( m_editZoomStepTime.GetSafeHwnd() == NULL )
		return;
CString s;
	m_editZoomStepTime.GetWindowText( s );
	if( ! s.IsEmpty() )
	{
		if( _ttol(s) > 50 )
			s = _T("50");
		SetZoomAnimation( -1, _ttol(s) );
	}
}

void CPageDateBrowser::SetZoomAnimation( INT _stepCount, INT _stepTime )
{
CString s;

	if( _stepCount >= 0 )
	{
		s.Format( _T("%d"), _stepCount );
		m_editZoomStepCount.SetWindowText( s );
	}
	
	if( _stepTime >= 0 )
	{
		s.Format( _T("%d"), _stepTime );
		m_editZoomStepTime.SetWindowText( s );
	}

	if( _stepCount >= 0 )
		m_wndDateBrowser.m_nAnimationStepCountZoom = _stepCount;
	
	if( _stepTime >= 0 )
		m_wndDateBrowser.m_nAnimationStepTimeZoom = _stepTime;

	m_wndDateBrowser.Invalidate();
}

void CPageDateBrowser::SetScrollAnimation( INT _stepCount, INT _stepTime )
{
CString s;

	if( _stepCount >= 0 )
	{
		s.Format( _T("%d"), _stepCount );
		m_editScrollStepCount.SetWindowText( s );
	}
	
	if( _stepTime >= 0 )
	{
		s.Format( _T("%d"), _stepTime );
		m_editScrollStepTime.SetWindowText( s );
	}

	if( _stepCount >= 0 )
		m_wndDateBrowser.m_nAnimationStepCountScroll = _stepCount;
	
	if( _stepTime >= 0 )
		m_wndDateBrowser.m_nAnimationStepTimeScroll = _stepTime;

	m_wndDateBrowser.Invalidate();
}

void CPageDateBrowser::SetTheme( LPCTSTR _theme )
{
	if( _tcscmp( _theme, _T("Simple")) == 0 )
		m_wndDateBrowser.PaintStyleSet( CExtDateBrowserWnd::__EPST_SIMPLE );
	else if( _tcscmp( _theme, _T("Skin")) == 0 )
		m_wndDateBrowser.PaintStyleSet( CExtDateBrowserWnd::__EPST_SKIN );
	else if( _tcscmp( _theme, _T("PaintManager")) == 0 )
		m_wndDateBrowser.PaintStyleSet( CExtDateBrowserWnd::__EPST_PAINT_MANAGER );

	m_wndDateBrowser.Invalidate();
}

void CPageDateBrowser::OnSelendokComboFirstDayOfWeek() 
{
CString s;
	m_comboFirstDayOfWeek.GetWindowText( s );
	SetFirstDayOfWeek( s );
}

void CPageDateBrowser::OnSelendokComboFirstYear() 
{
INT nSelFirst = m_comboFirstYear.GetCurSel();
INT nSelLast = m_comboLastYear.GetCurSel();
	if( nSelFirst > nSelLast )
	{
//		nSelFirst = nSelLast;
//		m_comboFirstYear.SetCurSel( nSelFirst );
		nSelLast = nSelFirst;
		m_comboLastYear.SetCurSel( nSelLast );
	}
CString sFirst, sLast;
	m_comboFirstYear.GetLBText( nSelFirst, sFirst );
	m_comboLastYear.GetLBText( nSelLast, sLast );
COleDateTime _firstYear, _lastYear, _curr = m_wndDateBrowser.TimeGet();
	_firstYear.SetDateTime( _ttol(sFirst), 1, 1, 0, 0, 0 );
	_lastYear.SetDateTime( _ttol(sLast), 12, 31, 23, 59, 59 );
	m_wndDateBrowser.TimeSet( _firstYear, -1 );
	m_wndDateBrowser.TimeSet( _lastYear, 1 );
	if( _curr < _firstYear )
		m_wndDateBrowser.TimeSet( _firstYear );
	else if( _curr > _lastYear )
		m_wndDateBrowser.TimeSet( _lastYear );
	m_wndDateBrowser.Invalidate();
}

void CPageDateBrowser::OnSelendokComboLastYear() 
{
INT nSelFirst = m_comboFirstYear.GetCurSel();
INT nSelLast = m_comboLastYear.GetCurSel();
	if( nSelFirst > nSelLast )
	{
		nSelFirst = nSelLast;
		m_comboFirstYear.SetCurSel( nSelFirst );
//		nSelLast = nSelFirst;
//		m_comboLastYear.SetCurSel( nSelLast );
	}
CString sFirst, sLast;
	m_comboFirstYear.GetLBText( nSelFirst, sFirst );
	m_comboLastYear.GetLBText( nSelLast, sLast );
COleDateTime _firstYear, _lastYear, _curr = m_wndDateBrowser.TimeGet();
	_firstYear.SetDateTime( _ttol(sFirst), 1, 1, 0, 0, 0 );
	_lastYear.SetDateTime( _ttol(sLast), 12, 31, 23, 59, 59 );
	m_wndDateBrowser.TimeSet( _firstYear, -1 );
	m_wndDateBrowser.TimeSet( _lastYear, 1 );
	if( _curr < _firstYear )
		m_wndDateBrowser.TimeSet( _firstYear );
	else if( _curr > _lastYear )
		m_wndDateBrowser.TimeSet( _lastYear );
	m_wndDateBrowser.Invalidate();
}

void CPageDateBrowser::SetFirstDayOfWeek( LPCTSTR _day )
{
	if( _tcscmp( _day, _T("Use Windows settings")) == 0 )
		m_wndDateBrowser.m_nFirstDayOfWeek = -1;
	else if( _tcscmp( _day, _T("Sunday")) == 0 )
		m_wndDateBrowser.m_nFirstDayOfWeek = 0;
	else if( _tcscmp( _day, _T("Monday")) == 0 )
		m_wndDateBrowser.m_nFirstDayOfWeek = 1;
	else if( _tcscmp( _day, _T("Tuesday")) == 0 )
		m_wndDateBrowser.m_nFirstDayOfWeek = 2;
	else if( _tcscmp( _day, _T("Wednesday")) == 0 )
		m_wndDateBrowser.m_nFirstDayOfWeek = 3;
	else if( _tcscmp( _day, _T("Thursday")) == 0 )
		m_wndDateBrowser.m_nFirstDayOfWeek = 4;
	else if( _tcscmp( _day, _T("Friday")) == 0 )
		m_wndDateBrowser.m_nFirstDayOfWeek = 5;
	else if( _tcscmp( _day, _T("Saturday")) == 0 )
		m_wndDateBrowser.m_nFirstDayOfWeek = 6;

	m_wndDateBrowser.Invalidate();
}

void CPageDateBrowser::OnCheckShowOutOfRangeItems() 
{
	if( m_checkShowOutOfRangeItems.GetCheck() != 0 )
		m_wndDateBrowser.m_bShowOutOfRangeItems = true;
	else
		m_wndDateBrowser.m_bShowOutOfRangeItems = false;

	m_wndDateBrowser.Invalidate();
}

void CPageDateBrowser::OnCheckShowNonMonthDays()
{
	if( m_checkShowNonMonthDays.GetCheck() != 0 )
	{
		m_wndDateBrowser.m_bShowDaysAfterMonth = true;
		m_wndDateBrowser.m_bShowDaysBeforeMonth = true;
	}
	else
	{
		m_wndDateBrowser.m_bShowDaysAfterMonth = false;
		m_wndDateBrowser.m_bShowDaysBeforeMonth = false;
	}
	
	m_wndDateBrowser.Invalidate();
}

void CPageDateBrowser::OnCheckShowNonCenturyItems() 
{
	if( m_checkShowNonCenturyItems.GetCheck() != 0 )
	{
		m_wndDateBrowser.m_bShowItemsAfterCentury = true;
		m_wndDateBrowser.m_bShowItemsBeforeCentury = true;
	}
	else
	{
		m_wndDateBrowser.m_bShowItemsAfterCentury = false;
		m_wndDateBrowser.m_bShowItemsBeforeCentury = false;
	}
	
	m_wndDateBrowser.Invalidate();
}

void CPageDateBrowser::OnCheckShowNonYearDecadeItems() 
{
	if( m_checkShowNonYearDecadeItems.GetCheck() != 0 )
	{
		m_wndDateBrowser.m_bShowItemsAfterYearDecade = true;
		m_wndDateBrowser.m_bShowItemsBeforeYearDecade = true;
	}
	else
	{
		m_wndDateBrowser.m_bShowItemsAfterYearDecade = false;
		m_wndDateBrowser.m_bShowItemsBeforeYearDecade = false;
	}
	
	m_wndDateBrowser.Invalidate();
}


void CPageDateBrowser::OnCheckShowHoverEvents() 
{
	m_wndDateBrowser.m_bShowHoverEvents = m_wndCheckShowHoverEventsInLogView.GetCheck() ? true : false;
	
}

#endif  // (!defined __EXT_MFC_NO_DATE_BROWSER )

