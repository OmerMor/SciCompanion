// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Skinable.h"

#include "MainFrm.h"

#if (!defined __EXT_MFC_NO_SHELL)
	#if (!defined __EXT_CONTROLS_SHELL_H)
		#include <ExtControlsShell.h>
	#endif
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogThemes

CDialogThemes::CDialogThemes()
	: CExtResizableDialog( CDialogThemes::IDD )
{
	//{{AFX_DATA_INIT(CDialogThemes)
	//}}AFX_DATA_INIT
}

CDialogThemes::~CDialogThemes()
{
}

bool CDialogThemes::CreateInsideBar(
	CExtControlBar * pBar
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	ASSERT( GetSafeHwnd() == NULL );
	ASSERT( pBar->GetSafeHwnd() != NULL );
	if( ! CExtResizableDialog::Create( IDD, pBar ) )
		return false;
	return true;
}

void CDialogThemes::InvokeMainFrameCommand( UINT nCmdID )
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
CFrameWnd * pFrame = GetParentFrame();
	ASSERT_VALID( pFrame );
CMainFrame * pMainFrame = DYNAMIC_DOWNCAST( CMainFrame, pFrame );
	if( pMainFrame == NULL )
	{
		pFrame = pFrame->GetParentFrame();
		ASSERT_VALID( pFrame );
		pMainFrame = STATIC_DOWNCAST( CMainFrame, pFrame );
	}
	pMainFrame->SendMessage( WM_COMMAND, WPARAM(nCmdID) );
}

void CDialogThemes::DoDataExchange( CDataExchange * pDX )
{
	CExtResizableDialog::DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CDialogThemes)
	DDX_Control(pDX, IDC_BUTTON_THEME_2005, m_wndButtonInstallTheme2005);
	DDX_Control(pDX, IDC_BUTTON_THEME_2003, m_wndButtonInstallTheme2003);
	DDX_Control(pDX, IDC_BUTTON_THEME_XP, m_wndButtonInstallThemeXP);
	DDX_Control(pDX, IDC_BUTTON_THEME_2000, m_wndButtonInstallTheme2000);
	DDX_Control(pDX, IDC_BUTTON_LOAD_THEME_BIN, m_wndButtonLoadThemeBIN);
	DDX_Control(pDX, IDC_BUTTON_LOAD_THEME_XML, m_wndButtonLoadThemeXML);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP( CDialogThemes, CExtResizableDialog )
	//{{AFX_MSG_MAP(CDialogThemes)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_THEME_XML, OnButtonLoadThemeXml)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_THEME_BIN, OnButtonLoadThemeBin)
	ON_BN_CLICKED(IDC_BUTTON_THEME_2005, OnButtonTheme2005)
	ON_BN_CLICKED(IDC_BUTTON_THEME_2003, OnButtonTheme2003)
	ON_BN_CLICKED(IDC_BUTTON_THEME_XP, OnButtonThemeXp)
	ON_BN_CLICKED(IDC_BUTTON_THEME_2000, OnButtonTheme2000)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDialogThemes::OnInitDialog() 
{
	if( ! CExtResizableDialog::OnInitDialog() )
		return FALSE;
	ShowSizeGrip( FALSE );
	m_wndButtonInstallTheme2005.m_bUseStdCommandNotification = true;
	m_wndButtonInstallTheme2003.m_bUseStdCommandNotification = true;
	m_wndButtonInstallThemeXP.m_bUseStdCommandNotification = true;
	m_wndButtonInstallTheme2000.m_bUseStdCommandNotification = true;
	m_wndButtonLoadThemeBIN.m_bUseStdCommandNotification = true;
	m_wndButtonLoadThemeXML.m_bUseStdCommandNotification = true;
	return TRUE;
}

void CDialogThemes::OnButtonLoadThemeXml() 
{
	InvokeMainFrameCommand( IDC_THEME_LOAD_SKIN_XML );
}

void CDialogThemes::OnButtonLoadThemeBin() 
{
	InvokeMainFrameCommand( IDC_THEME_LOAD_SKIN_BIN );
}

void CDialogThemes::OnButtonTheme2005() 
{
	InvokeMainFrameCommand( IDC_THEME_2005 );
}

void CDialogThemes::OnButtonTheme2003() 
{
	InvokeMainFrameCommand( IDC_THEME_2003 );
}

void CDialogThemes::OnButtonThemeXp() 
{
	InvokeMainFrameCommand( IDC_THEME_XP );
}

void CDialogThemes::OnButtonTheme2000() 
{
	InvokeMainFrameCommand( IDC_THEME_2000 );
}

/////////////////////////////////////////////////////////////////////////////
// CDialogInvokeModalDialogs

CDialogInvokeModalDialogs::CDialogInvokeModalDialogs()
	: CExtResizableDialog( CDialogInvokeModalDialogs::IDD )
{
	//{{AFX_DATA_INIT(CDialogInvokeModalDialogs)
	//}}AFX_DATA_INIT
}

CDialogInvokeModalDialogs::~CDialogInvokeModalDialogs()
{
}

bool CDialogInvokeModalDialogs::CreateInsideBar(
	CExtControlBar * pBar
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	ASSERT( GetSafeHwnd() == NULL );
	ASSERT( pBar->GetSafeHwnd() != NULL );
	if( ! CExtResizableDialog::Create( IDD, pBar ) )
		return false;
	return true;
}

void CDialogInvokeModalDialogs::DoDataExchange( CDataExchange * pDX )
{
	CExtResizableDialog::DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CDialogInvokeModalDialogs)
	DDX_Control(pDX, IDC_BUTTON_INVOKE_ICON_EDITOR_DIALOG, m_wndButtonInvokeDialogIconEditor);
	DDX_Control(pDX, IDC_BUTTON_INVOKE_COLOR_DIALOG, m_wndButtonInvokeDialogColor);
	DDX_Control(pDX, IDC_BUTTON_INVOKE_ABOUT_DIALOG, m_wndButtonInvokeDialogAbout);
	DDX_Control(pDX, IDC_BUTTON_INVOKE_FILE_DIALOG, m_wndButtonInvokeFileDialog);
	DDX_Control(pDX, IDC_BUTTON_INVOKE_BFF_DIALOG, m_wndButtonInvokeBffDialog);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP( CDialogInvokeModalDialogs, CExtResizableDialog )
	//{{AFX_MSG_MAP(CDialogInvokeModalDialogs)
	ON_BN_CLICKED(IDC_BUTTON_INVOKE_COLOR_DIALOG, OnButtonInvokeColorDialog)
	ON_BN_CLICKED(IDC_BUTTON_INVOKE_ICON_EDITOR_DIALOG, OnButtonInvokeIconEditorDialog)
	ON_BN_CLICKED(IDC_BUTTON_INVOKE_ABOUT_DIALOG, OnButtonInvokeAboutDialog)
	ON_BN_CLICKED(IDC_BUTTON_INVOKE_FILE_DIALOG, OnButtonInvokeFileDialog)
	ON_BN_CLICKED(IDC_BUTTON_INVOKE_BFF_DIALOG, OnButtonInvokeBffDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDialogInvokeModalDialogs::OnInitDialog() 
{
	if( ! CExtResizableDialog::OnInitDialog() )
		return FALSE;
	ShowSizeGrip( FALSE );
	m_wndButtonInvokeDialogIconEditor.m_bUseStdCommandNotification = true;
	m_wndButtonInvokeDialogColor.m_bUseStdCommandNotification = true;
	m_wndButtonInvokeDialogAbout.m_bUseStdCommandNotification = true;
	m_wndButtonInvokeFileDialog.m_bUseStdCommandNotification = true;
	m_wndButtonInvokeBffDialog.m_bUseStdCommandNotification = true;
	return TRUE;
}

void CDialogInvokeModalDialogs::OnButtonInvokeColorDialog() 
{
CExtColorDlg dlgColor;
	dlgColor.DoModal();
}

void CDialogInvokeModalDialogs::OnButtonInvokeIconEditorDialog() 
{
#if (defined __EXT_MFC_NO_ICONEDITDLG)
	::AfxMessageBox( _T("The CExtIconEditDlg class is not avaliable."), MB_ICONERROR );
#else // (defined __EXT_MFC_NO_ICONEDITDLG)
CExtIconEditDlg dlgIconEdit;
	dlgIconEdit.m_icon.AssignFromHICON(
		::AfxGetApp()->LoadIcon( IDR_MAINFRAME ),
		false
		);
	dlgIconEdit.DoModal();
#endif // else from (defined __EXT_MFC_NO_ICONEDITDLG)
}

void CDialogInvokeModalDialogs::OnButtonInvokeAboutDialog() 
{
	VERIFY( ProfUISAbout() );
}

void CDialogInvokeModalDialogs::OnButtonInvokeFileDialog() 
{
#if (defined __EXT_MFC_NO_SHELL_DIALOG_FILE)
	::AfxMessageBox( _T("The CExtShellDialogFile class is not avaliable."), MB_ICONERROR );
#else // (defined __EXT_MFC_NO_SHELL_DIALOG_FILE)
CExtShellDialogFile dlgShellFile;
	dlgShellFile.m_bEnableCollapsing = true;
	dlgShellFile.DoModal();
#endif // else from (defined __EXT_MFC_NO_SHELL_DIALOG_FILE)
}

void CDialogInvokeModalDialogs::OnButtonInvokeBffDialog() 
{
#if (defined __EXT_MFC_NO_SHELL_DIALOG_BFF)
	::AfxMessageBox( _T("The CExtShellDialogBrowseFor class is not avaliable."), MB_ICONERROR );
#else // (defined __EXT_MFC_NO_SHELL_DIALOG_BFF)
CExtShellDialogBrowseFor dlgShellBrowseFor;
	dlgShellBrowseFor.DoModal();
#endif // else from (defined __EXT_MFC_NO_SHELL_DIALOG_BFF)
}

/////////////////////////////////////////////////////////////////////////////
// CDialogInputFields

CDialogInputFields::CDialogInputFields()
	: CExtResizableDialog( CDialogInputFields::IDD )
{
	//{{AFX_DATA_INIT(CDialogInputFields)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

CDialogInputFields::~CDialogInputFields()
{
}

bool CDialogInputFields::CreateInsideBar(
	CExtControlBar * pBar
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	ASSERT( GetSafeHwnd() == NULL );
	ASSERT( pBar->GetSafeHwnd() != NULL );
	if( ! CExtResizableDialog::Create( IDD, pBar ) )
		return false;
	return true;
}

void CDialogInputFields::DoDataExchange( CDataExchange * pDX )
{
	CExtResizableDialog::DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CDialogInputFields)
	DDX_Control(pDX, IDC_IPADDRESS2, m_wndIPAddress2);
	DDX_Control(pDX, IDC_IPADDRESS1, m_wndIPAddress1);
	DDX_Control(pDX, IDC_SPIN6, m_wndSpin6);
	DDX_Control(pDX, IDC_SPIN5, m_wndSpin5);
	DDX_Control(pDX, IDC_SPIN4, m_wndSpin4);
	DDX_Control(pDX, IDC_SPIN3, m_wndSpin3);
	DDX_Control(pDX, IDC_EDIT6, m_wndEdit6);
	DDX_Control(pDX, IDC_EDIT5, m_wndEdit5);
	DDX_Control(pDX, IDC_EDIT4, m_wndEdit4);
	DDX_Control(pDX, IDC_EDIT3, m_wndEdit3);
	DDX_Control(pDX, IDC_EDIT2, m_wndEdit2);
	DDX_Control(pDX, IDC_EDIT1, m_wndEdit1);
	DDX_Control(pDX, IDC_COMBO4, m_wndCombo4);
	DDX_Control(pDX, IDC_COMBO3, m_wndCombo3);
	DDX_Control(pDX, IDC_COMBO2, m_wndCombo2);
	DDX_Control(pDX, IDC_COMBO1, m_wndCombo1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP( CDialogInputFields, CExtResizableDialog )
	//{{AFX_MSG_MAP(CDialogInputFields)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDialogInputFields::OnInitDialog() 
{
	if( ! CExtResizableDialog::OnInitDialog() )
		return FALSE;
	ShowSizeGrip( FALSE );
	return TRUE;
}

#if (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)

/////////////////////////////////////////////////////////////////////////////
// CDialogDateTimeDuration message handlers

CDialogDateTimeDuration::CDialogDateTimeDuration(CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(CDialogDateTimeDuration::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CDialogDateTimeDuration)
	m_nTimeFormat = -1;
	m_nMode = -1;
	//}}AFX_DATA_INIT
}

bool CDialogDateTimeDuration::CreateInsideBar(
	CExtControlBar * pBar
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	ASSERT( GetSafeHwnd() == NULL );
	ASSERT( pBar->GetSafeHwnd() != NULL );
	if( ! CExtResizableDialog::Create( IDD, pBar ) )
		return false;
	return true;
}


void CDialogDateTimeDuration::DoDataExchange(CDataExchange* pDX)
{
	CExtResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDateTimeDuration)
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
	DDX_Control(pDX, IDC_NOTIFICATIONS, m_wndNotifications);
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
}

BEGIN_MESSAGE_MAP(CDialogDateTimeDuration, CExtResizableDialog)
	//{{AFX_MSG_MAP(CDialogDateTimeDuration)
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

BOOL CDialogDateTimeDuration::OnInitDialog() 
{
	if( ! CExtResizableDialog::OnInitDialog() )
		return FALSE;

	ShowSizeGrip( FALSE );
	
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

LRESULT CDialogDateTimeDuration::OnSelChangedDuration( WPARAM wParam, LPARAM lParam )
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

LRESULT CDialogDateTimeDuration::OnSelChangedDateTime( WPARAM wParam, LPARAM lParam )
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

LRESULT CDialogDateTimeDuration::OnColorChanged(WPARAM wParam, LPARAM lParam)
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

BOOL CDialogDateTimeDuration::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( !m_bInitComplete )
		return CExtResizableDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	
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
			return CExtResizableDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		return TRUE;
	}
	
	return CExtResizableDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

#endif // (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)


/////////////////////////////////////////////////////////////////////////////
// CDialogButtons

CDialogButtons::CDialogButtons()
	: CExtResizableDialog( CDialogButtons::IDD )
{
	//{{AFX_DATA_INIT(CDialogButtons)
	//}}AFX_DATA_INIT
}

CDialogButtons::~CDialogButtons()
{
}

bool CDialogButtons::CreateInsideBar(
	CExtControlBar * pBar
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	ASSERT( GetSafeHwnd() == NULL );
	ASSERT( pBar->GetSafeHwnd() != NULL );
	if( ! CExtResizableDialog::Create( IDD, pBar ) )
		return false;
	return true;
}

void CDialogButtons::DoDataExchange( CDataExchange * pDX )
{
	CExtResizableDialog::DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CDialogButtons)
	DDX_Control( pDX, IDC_BUTTON_DISABLED_1, m_wndButtonDisabled1 );
	DDX_Control( pDX, IDC_BUTTON_NORMAL_1,   m_wndButtonNormal1 );
	DDX_Control( pDX, IDC_BUTTON_DEFAULT_1,  m_wndButtonDefault1 );
	DDX_Control( pDX, IDC_BUTTON_DISABLED_2, m_wndButtonDisabled2 );
	DDX_Control( pDX, IDC_BUTTON_NORMAL_2,   m_wndButtonNormal2 );
	DDX_Control( pDX, IDC_BUTTON_DEFAULT_2,  m_wndButtonDefault2 );
	DDX_Control( pDX, IDC_BUTTON_DISABLED_3, m_wndButtonDisabled3 );
	DDX_Control( pDX, IDC_BUTTON_NORMAL_3,   m_wndButtonNormal3 );
	DDX_Control( pDX, IDC_BUTTON_DEFAULT_3,  m_wndButtonDefault3 );
	DDX_Control( pDX, IDC_BUTTON_NORMAL_4,   m_wndButtonNormal4 );
	DDX_Control( pDX, IDC_BUTTON_DISABLED_4, m_wndButtonDefault4 );
	DDX_Control( pDX, IDC_BUTTON_DISABLED_5, m_wndButtonDisabled5 );
	DDX_Control( pDX, IDC_BUTTON_NORMAL_5,   m_wndButtonNormal5 );
	DDX_Control( pDX, IDC_BUTTON_DEFAULT_5,  m_wndButtonDefault5 );
	DDX_Control( pDX, IDC_BUTTON_DISABLED_6, m_wndButtonDisabled6 );
	DDX_Control( pDX, IDC_BUTTON_NORMAL_6,   m_wndButtonNormal6 );
	DDX_Control( pDX, IDC_BUTTON_DEFAULT_6,  m_wndButtonDefault6 );
	DDX_Control( pDX, IDC_BUTTON_CHECK_DIABLED,     m_wndCheckDisabled );
	DDX_Control( pDX, IDC_BUTTON_CHECK_THREE_STATE, m_wndCheckThreeState );
	DDX_Control( pDX, IDC_BUTTON_CHECK_TWO_STATE,   m_wndCheckTwoState );
	DDX_Control( pDX, IDC_BUTTON_RADIO_DISABLED,    m_wndRadioDisabled );
	DDX_Control( pDX, IDC_BUTTON_RADIO_2,           m_wndRadio2 );
	DDX_Control( pDX, IDC_BUTTON_RADIO_1,           m_wndRadio1 );
	DDX_Control( pDX, IDC_GROUP_BOX_1, m_wndGroupBox1 );
	DDX_Control( pDX, IDC_GROUP_BOX_2, m_wndGroupBox2 );
	DDX_Control( pDX, IDC_GROUP_BOX_3, m_wndGroupBox3 );
	DDX_Control( pDX, IDC_GROUP_BOX_4, m_wndGroupBox4 );
	DDX_Control( pDX, IDC_GROUP_BOX_5, m_wndGroupBox5 );
	DDX_Control( pDX, IDC_GROUP_BOX_6, m_wndGroupBox6 );
	DDX_Control( pDX, IDC_GROUP_BOX_7, m_wndGroupBox7 );
	DDX_Control( pDX, IDC_GROUP_BOX_8, m_wndGroupBox8 );
	DDX_Control( pDX, IDC_GROUP_BOX_9, m_wndGroupBox9 );
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP( CDialogButtons, CExtResizableDialog )
	//{{AFX_MSG_MAP(CDialogButtons)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDialogButtons::OnInitDialog() 
{
	if( ! CExtResizableDialog::OnInitDialog() )
		return FALSE;

	ShowSizeGrip( FALSE );

//	m_wndButtonDisabled1.SetIcon( GetSmallIconForButton() );
//	m_wndButtonNormal1.SetIcon( GetSmallIconForButton() );
//	m_wndButtonDefault1.SetIcon( GetSmallIconForButton() );
	
//	m_wndButtonDisabled2.SetIcon( GetSmallIconForButton() );
//	m_wndButtonNormal2.SetIcon( GetSmallIconForButton() );
//	m_wndButtonDefault2.SetIcon( GetSmallIconForButton() );
	VERIFY( m_wndButtonDisabled2.m_menu.LoadMenu( IDR_MENU_IN_BUTTON ) );
	VERIFY( m_wndButtonNormal2.m_menu.LoadMenu( IDR_MENU_IN_BUTTON ) );
	VERIFY( m_wndButtonDefault2.m_menu.LoadMenu( IDR_MENU_IN_BUTTON ) );
	
	m_wndButtonDisabled3.SetIcon( GetSmallIconForButton() );
	m_wndButtonNormal3.SetIcon( GetSmallIconForButton() );
	m_wndButtonDefault3.SetIcon( GetSmallIconForButton() );
	VERIFY( m_wndButtonDisabled3.m_menu.LoadMenu( IDR_MENU_IN_BUTTON ) );
	VERIFY( m_wndButtonNormal3.m_menu.LoadMenu( IDR_MENU_IN_BUTTON ) );
	VERIFY( m_wndButtonDefault3.m_menu.LoadMenu( IDR_MENU_IN_BUTTON ) );
	m_wndButtonDisabled3.SetSeparatedDropDown();
	m_wndButtonNormal3.SetSeparatedDropDown();
	m_wndButtonDefault3.SetSeparatedDropDown();

	m_wndButtonNormal4.m_strURL = _T("http://www.Prof-UIS.com");

	m_wndButtonDisabled5.m_clrSelected = RGB(192,0,0);
	m_wndButtonNormal5.m_clrSelected = RGB(0,192,0);
	m_wndButtonDefault5.m_clrSelected = RGB(0,0,192);

	m_wndButtonDisabled6.m_clrSelected = RGB(192,192,0);
	m_wndButtonNormal6.m_clrSelected = RGB(192,0,192);
	m_wndButtonDefault6.m_clrSelected = RGB(0,192,192);
	m_wndButtonDisabled6.SetSeparatedDropDown();
	m_wndButtonNormal6.SetSeparatedDropDown();
	m_wndButtonDefault6.SetSeparatedDropDown();

	m_wndCheckTwoState.SetCheck( 1 );
	m_wndCheckThreeState.SetCheck( 2 );
	m_wndCheckDisabled.SetCheck( 1 );
	m_wndRadio1.SetCheck( 1 );
	m_wndRadioDisabled.SetCheck( 1 );

	return TRUE;
}

void CDialogButtons::OnEditCopy() 
{
	::AfxMessageBox( _T("CDialogButtons::OnEditCopy()") );
}

void CDialogButtons::OnEditCut() 
{
	::AfxMessageBox( _T("CDialogButtons::OnEditCut()") );
}

void CDialogButtons::OnEditPaste() 
{
	::AfxMessageBox( _T("CDialogButtons::OnEditPaste()") );
}

/////////////////////////////////////////////////////////////////////////////
// CColorControlsView

CColorControlsView::CColorControlsView()
	: m_bSyncingValues( false )
	, m_bColorControlsVisible( false )
{
}

CColorControlsView::~CColorControlsView()
{
}

bool CColorControlsView::CreateInsideBar(
	CExtControlBar * pBar
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pBar );
	ASSERT( GetSafeHwnd() == NULL );
	ASSERT( pBar->GetSafeHwnd() != NULL );
	if( ! CWnd::Create(
			NULL,
			NULL,
			WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
			CRect( 0, 0, 0, 0 ),
			pBar,
			UINT(IDC_STATIC)
			)
		)
		return false;
	return true;
}

BEGIN_MESSAGE_MAP(CColorControlsView,CWnd )
	//{{AFX_MSG_MAP(CColorControlsView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CExtColorCtrl::g_nMsgColorChanged,OnColorChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorControlsView message handlers

BOOL CColorControlsView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ! CWnd::PreCreateWindow( cs ) )
		return FALSE;
	cs.lpszClass =
		::AfxRegisterWndClass(
			CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
			::LoadCursor( NULL, IDC_ARROW ),
			HBRUSH( COLOR_WINDOW + 1 ),
			NULL
			);
	return TRUE;
}

void CColorControlsView::OnPaint() 
{
CPaintDC dcPaint( this );
	CExtPaintManager::stat_ExcludeChildAreas(
		dcPaint.GetSafeHdc(),
		GetSafeHwnd()
		);
CExtMemoryDC dc( &dcPaint );
	if( ! g_PaintManager->PaintDockerBkgnd(true,dc,this) )
	{
		CRect rcClient;
		GetClientRect( &rcClient );
		dc.FillSolidRect(
			&rcClient,
			g_PaintManager->GetColor( COLOR_3DFACE )
			);
	}
}


BOOL CColorControlsView::OnEraseBkgnd(CDC* pDC) 
{
	pDC;
	return TRUE;
}


int CColorControlsView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	VERIFY(
		m_wndColorCtrlRoller.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD|WS_VISIBLE,
			CRect(10,10,230,200),
			this,
			0x1FF
			)
		);
	m_wndColorCtrlRoller = CExtColorCtrl::MODE_HSL_ROLLER;

	VERIFY(
		m_wndColorCtrlCubeRGB.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD|WS_VISIBLE,
			CRect(10,210,230,410),
			this,
			0x200
			)
		);
	m_wndColorCtrlCubeRGB = CExtColorCtrl::MODE_RGB_CUBE;

	VERIFY(
		m_wndColorCtrlCubeCMY.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD|WS_VISIBLE,
			CRect(240,210,460,410),
			this,
			0x201
			)
		);
	m_wndColorCtrlCubeCMY = CExtColorCtrl::MODE_CMY_CUBE;
	
	VERIFY(
		m_wndColorCtrlHSL.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD|WS_VISIBLE,
			CRect(240,10,460,200),
			this,
			0x202
			)
		);
	m_wndColorCtrlHSL = CExtColorCtrl::MODE_HSL_PANEL;

	VERIFY(
		m_wndColorCtrlMixerRGB.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD|WS_VISIBLE,
			CRect(470,210,690,410),
			this,
			0x203
			)
		);
	m_wndColorCtrlMixerRGB = CExtColorCtrl::MODE_RGB_MIX;
	
	VERIFY(
		m_wndColorCtrlMixerCMY.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD|WS_VISIBLE,
			CRect(470,10,690,200),
			this,
			0x204
			)
		);
	m_wndColorCtrlMixerCMY = CExtColorCtrl::MODE_CMY_MIX;

COLORREF clrInit = RGB(196,128,64);
	m_wndColorCtrlCubeRGB.SetupColor(clrInit,false);
	m_wndColorCtrlCubeCMY.SetupColor(clrInit,false);
	m_wndColorCtrlMixerRGB.SetupColor(clrInit,false);
	m_wndColorCtrlMixerCMY.SetupColor(clrInit,false);
	m_wndColorCtrlHSL.SetupColor(clrInit,false);

	return 0;
}

LRESULT CColorControlsView::OnColorChanged(WPARAM wParam,LPARAM lParam)
{
	if( m_bSyncingValues )
		return 0;
	m_bSyncingValues = true;
CExtColorCtrl::selection_colors_t * pClrs =
		(CExtColorCtrl::selection_colors_t *)lParam;
	ASSERT( pClrs != NULL );
COLORREF clrNew = pClrs->m_clrNew;
HWND hWndColorCtrlFrom = (HWND)wParam;
	ASSERT( ::IsWindow(hWndColorCtrlFrom) );
	if( hWndColorCtrlFrom != m_wndColorCtrlRoller.GetSafeHwnd() )
		m_wndColorCtrlRoller.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlCubeRGB.GetSafeHwnd() )
		m_wndColorCtrlCubeRGB.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlCubeCMY.GetSafeHwnd() )
		m_wndColorCtrlCubeCMY.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlMixerRGB.GetSafeHwnd() )
		m_wndColorCtrlMixerRGB.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlMixerCMY.GetSafeHwnd() )
		m_wndColorCtrlMixerCMY.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlHSL.GetSafeHwnd() )
		m_wndColorCtrlHSL.SetupColor(clrNew,false);
	m_bSyncingValues = false;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC( CMainFrame, CMDIFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CMDIFrameWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDC_THEME_2000, OnTheme2000)
	ON_UPDATE_COMMAND_UI(IDC_THEME_2000, OnUpdateTheme2000)
	ON_COMMAND(IDC_THEME_XP, OnThemeXp)
	ON_UPDATE_COMMAND_UI(IDC_THEME_XP, OnUpdateThemeXp)
	ON_COMMAND(IDC_THEME_2003, OnTheme2003)
	ON_UPDATE_COMMAND_UI(IDC_THEME_2003, OnUpdateTheme2003)
	ON_COMMAND(IDC_THEME_2005, OnTheme2005)
	ON_UPDATE_COMMAND_UI(IDC_THEME_2005, OnUpdateTheme2005)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(IDC_THEME_LOAD_SKIN_XML, OnThemeLoadSkinXml)
	ON_COMMAND(IDC_THEME_LOAD_SKIN_BIN, OnThemeLoadSkinBin)
	ON_COMMAND(ID_COMBO_BOX_IN_TOOLBAR, OnComboBoxInToolbar)
	//}}AFX_MSG_MAP
	ON_COMMAND_EX( ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_MENUBAR, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_TOOLBAR_THEMES, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_TOOLBAR_THEMES, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_TOOLBAR_SKINS, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_TOOLBAR_SKINS, OnUpdateControlBarMenu )

	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPrepareMenu,
		OnExtMenuPrepare
		)
	
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
	: m_pBarButtons( NULL )
	, m_pBarInputFields( NULL )
#if (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)
	, m_pBarDateTimeDuration( NULL )
#endif // (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)
	, m_pBarColorControls( NULL )
	, m_pBarInvokeModalDialogs( NULL )
	, m_pBarThemes( NULL )
{
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.showCmd = SW_HIDE;

CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
	if( ! g_PaintManager.PaintManagerStateLoad(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		)
	{
		CExtPaintManagerSkin * pPM = new CExtPaintManagerSkin;
		bool bLoaded = true;
		if( ! pPM->m_Skin.SearchAndLoadSkinFile( _T("BlackDiamond.Skin"), false ) )
		{
			if( ! pPM->m_Skin.SearchAndLoadSkinFile( _T("..\\..\\Skins\\Binary\\BlackDiamond.Skin"), false ) )
			{
				bLoaded = false;
				::AfxMessageBox( _T("Failed to load initial skin.") );
				delete pPM;
			}
		}
		if( bLoaded )
			g_PaintManager.InstallPaintManager( pPM );
	}

//	m_bNcFrameImpl_IsDwmBased = false;
}

CMainFrame::~CMainFrame()
{
}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
void CMainFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
//	CFrameWnd::OnUpdateControlBarMenu( pCmdUI );
	CExtControlBar::DoFrameBarCheckUpdate(
		this,
		pCmdUI,
		false
		);
}

BOOL CMainFrame::OnBarCheck(UINT nID)
{
//	return CFrameWnd::OnBarCheck( nID );
	return
		CExtControlBar::DoFrameBarCheckCmd(
			this,
			nID,
			false
			);
}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	if( CExtNCW < CMDIFrameWnd > :: OnCreate(lpCreateStruct) == -1 )
	{
		ASSERT( FALSE );
		return -1;
	}

	VERIFY(
		g_CmdManager->ProfileSetup(
			pApp->m_pszProfileName,
			GetSafeHwnd()
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_MAINFRAME
			)
		);

	m_wndMenuBar.SetMdiWindowPopupName( _T("Window") );
	if(	! m_wndMenuBar.Create(
			NULL,
			this,
			ID_VIEW_MENUBAR
			)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }
	
	if(		(! m_wndToolBarStandard.Create(
				NULL,
				this,
				AFX_IDW_TOOLBAR
				) )
		||	(! m_wndToolBarStandard.LoadToolBar( IDR_MAINFRAME ) )
		||	(! m_wndComboInToolBar.Create(
				WS_CHILD|WS_VISIBLE
					|WS_CLIPCHILDREN|WS_CLIPSIBLINGS
					|CBS_DROPDOWN|CBS_HASSTRINGS,
				CRect( 0, 0, 120, 400 ),
				&m_wndToolBarStandard,
				ID_COMBO_BOX_IN_TOOLBAR
				) )
		)
	{
		TRACE0("Failed to create standard toolbar\n");
		return -1;      // fail to create
	}
int nComboButtonIndexInToolBar =
		m_wndToolBarStandard.CommandToIndex( ID_COMBO_BOX_IN_TOOLBAR );
	ASSERT( nComboButtonIndexInToolBar >= 0 );
	m_wndToolBarStandard.SetButtonCtrl(
		nComboButtonIndexInToolBar,
		&m_wndComboInToolBar
		);
	m_wndComboInToolBar.SetFont(
		CFont::FromHandle(
			(HFONT) ::GetStockObject(DEFAULT_GUI_FONT)
			)
		);
	m_wndComboInToolBar.AddString( _T("combobox") );
	m_wndComboInToolBar.AddString( _T("1") );
	m_wndComboInToolBar.AddString( _T("2") );
	m_wndComboInToolBar.AddString( _T("3") );
	m_wndComboInToolBar.AddString( _T("4") );
	m_wndComboInToolBar.AddString( _T("5") );
	m_wndComboInToolBar.SetCurSel( 0 );

static const UINT g_arrButtonsForToolBarThemes[]=
{
	IDC_THEME_2000,
	IDC_THEME_XP,
	IDC_THEME_2003,
	IDC_THEME_2005,
};
	if(		(! m_wndToolBarThemes.Create(
				NULL,
				this,
				ID_VIEW_TOOLBAR_THEMES
				) )
		||	(! m_wndToolBarThemes.SetButtons(
				g_arrButtonsForToolBarThemes,
				sizeof(g_arrButtonsForToolBarThemes) / sizeof(g_arrButtonsForToolBarThemes[0])
				) )
		)
	{
		TRACE0("Failed to create themes toolbar\n");
		return -1;      // fail to create
	}

static const UINT g_arrButtonsForToolBarSkins[]=
{
	IDC_THEME_LOAD_SKIN_XML,
	IDC_THEME_LOAD_SKIN_BIN,
};
	if(		(! m_wndToolBarSkins.Create(
				NULL,
				this,
				ID_VIEW_TOOLBAR_SKINS
				) )
		||	(! m_wndToolBarSkins.SetButtons(
				g_arrButtonsForToolBarSkins,
				sizeof(g_arrButtonsForToolBarSkins) / sizeof(g_arrButtonsForToolBarSkins[0])
				) )
		)
	{
		TRACE0("Failed to create skins toolbar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.m_bDrawPaneSeparatorsInsteadOfBorders = true;
	if(		(! m_wndStatusBar.Create(this) )
		||	(! m_wndStatusBar.SetIndicators(
				indicators,
				sizeof(indicators) / sizeof(UINT)
				) )
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

static const UINT g_arrButtonsToSetToolBarText[]=
{
	IDC_THEME_2000,
	IDC_THEME_XP,
	IDC_THEME_2003,
	IDC_THEME_2005,
	IDC_THEME_LOAD_SKIN_XML,
	IDC_THEME_LOAD_SKIN_BIN,
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
	ID_APP_ABOUT,
};
INT nCommandIndex, nCommandCount = sizeof(g_arrButtonsToSetToolBarText) / sizeof(g_arrButtonsToSetToolBarText[0]);
	for( nCommandIndex = 0; nCommandIndex < nCommandCount; nCommandIndex++ )
	{
		UINT nCmdID = g_arrButtonsToSetToolBarText[nCommandIndex];
		CExtCmdItem * pCmdItem = g_CmdManager->CmdGetPtr( pApp->m_pszProfileName, nCmdID );
		ASSERT( pCmdItem != NULL );
		pCmdItem->m_sToolbarText = pCmdItem->m_sMenuText;
	} // for( nCommandIndex = 0; nCommandIndex < nCommandCount; nCommandIndex++ )

	m_wndMenuBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBarStandard.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBarThemes.EnableDocking( CBRS_ALIGN_ANY );
	m_wndToolBarSkins.EnableDocking( CBRS_ALIGN_ANY );

	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}

#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( !CExtControlBar::FrameInjectAutoHideAreas(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	
static UINT statBasicCommands[] =
{
	ID_FILE_NEW, 
	ID_FILE_OPEN, 
	ID_FILE_SAVE,
	ID_EDIT_COPY,
	ID_EDIT_CUT,
	ID_EDIT_PASTE,
	ID_APP_EXIT,
	ID_APP_ABOUT,
	ID_VIEW_TOOLBAR, 
	ID_WINDOW_TILE_HORZ,
	ID_WINDOW_CASCADE,
	ID_WINDOW_ARRANGE,
	IDC_THEME_2000,
	IDC_THEME_XP,
	IDC_THEME_2003,
	IDC_THEME_2005,
	0 // end of commands list
}; // statBasicCommands array
	VERIFY(
		g_CmdManager->SetBasicCommands(
			pApp->m_pszProfileName,
			statBasicCommands
			)
		);

#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	if( ! m_wndMdiTabs.Create(
			this,
			CRect(0,0,0,0),
			UINT(IDC_STATIC),
			WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
				(	__ETWS_MDI_DEFAULT|__ETWS_HIDE_ICONS|__ETWS_ITEM_DRAGGING
						|__ETWS_SHOW_BTN_SCROLL_HOME
							|__ETWS_ENABLED_BTN_SCROLL_HOME
						|__ETWS_SHOW_BTN_SCROLL_END
							|__ETWS_ENABLED_BTN_SCROLL_END
						|__ETWS_SHOW_BTN_HELP
							|__ETWS_ENABLED_BTN_HELP
						|__ETWS_SHOW_BTN_CLOSE
							|__ETWS_ENABLED_BTN_CLOSE
						|__ETWS_SHOW_BTN_TAB_LIST
							|__ETWS_ENABLED_BTN_TAB_LIST
				) & (~(__ETWS_AUTOHIDE_SCROLL))
			)
		)
	{
		ASSERT( FALSE );
		return -1;
	}
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

	// install dynamic bar site for this frame window
	CExtDynamicBarSite::m_nBarListInMenusCmdID = ID_DYNAMIC_BARS_LIST;
	CExtDynamicBarSite::Install( this );

	// allocate and initialize one persistent bar
HICON hIcon = (HICON)
		::LoadImage(
			::AfxGetInstanceHandle(),
			MAKEINTRESOURCE(IDR_MAINFRAME),
			IMAGE_ICON,
			16,
			16,
			0
			);
	ASSERT( hIcon != NULL );
CExtCmdIcon icon;
	icon.AssignFromHICON( hIcon, false );

	m_pBarButtons =
		CExtDynamicBarSite::BarAlloc(
			_T("Buttons"),
			icon,
			0,
			NULL,
			true
			);
	if( m_pBarButtons == NULL )
	{
		ASSERT( FALSE );
		return -1;
	}
	m_pBarButtons->m_nMdiMenuResourceID =
		IDR_MAINFRAME;
	m_pBarButtons->m_nMdiAccelTableResourceID =
		IDR_MAINFRAME;
UINT nCmdID = (UINT)m_pBarButtons->GetDlgCtrlID();
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			nCmdID
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->StateSetBasic();
	if( ! m_dlgButtons.CreateInsideBar( m_pBarButtons ) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
#if (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)
	m_pBarDateTimeDuration =
		CExtDynamicBarSite::BarAlloc(
			_T("Date/Time/Duration"),
			icon,
			0,
			NULL,
			true
			);
	if( m_pBarDateTimeDuration == NULL )
	{
		ASSERT( FALSE );
		return -1;
	}
	m_pBarDateTimeDuration->m_nMdiMenuResourceID =
		IDR_MAINFRAME;
	m_pBarDateTimeDuration->m_nMdiAccelTableResourceID =
		IDR_MAINFRAME;
	nCmdID = (UINT)m_pBarDateTimeDuration->GetDlgCtrlID();
	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			nCmdID
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->StateSetBasic();
	if( ! m_dlgDateTimeDuration.CreateInsideBar( m_pBarDateTimeDuration ) )
	{
		ASSERT( FALSE );
		return -1;
	}
#endif // (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)

	m_pBarThemes =
		CExtDynamicBarSite::BarAlloc(
			_T("Themes"),
			icon,
			0,
			NULL,
			true
			);
	if( m_pBarThemes == NULL )
	{
		ASSERT( FALSE );
		return -1;
	}
	m_pBarThemes->m_nMdiMenuResourceID =
		IDR_MAINFRAME;
	m_pBarThemes->m_nMdiAccelTableResourceID =
		IDR_MAINFRAME;
	nCmdID = (UINT)m_pBarThemes->GetDlgCtrlID();
	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			nCmdID
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->StateSetBasic();
	if( ! m_dlgThemes.CreateInsideBar( m_pBarThemes ) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
	m_pBarInvokeModalDialogs =
		CExtDynamicBarSite::BarAlloc(
			_T("Modal Dialogs"),
			icon,
			0,
			NULL,
			true
			);
	if( m_pBarInvokeModalDialogs == NULL )
	{
		ASSERT( FALSE );
		return -1;
	}
	m_pBarInvokeModalDialogs->m_nMdiMenuResourceID =
		IDR_MAINFRAME;
	m_pBarInvokeModalDialogs->m_nMdiAccelTableResourceID =
		IDR_MAINFRAME;
	nCmdID = (UINT)m_pBarInvokeModalDialogs->GetDlgCtrlID();
	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			nCmdID
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->StateSetBasic();
	if( ! m_dlgInvokeModalDialogs.CreateInsideBar( m_pBarInvokeModalDialogs ) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
	m_pBarInputFields =
		CExtDynamicBarSite::BarAlloc(
			_T("Input Fields"),
			icon,
			0,
			NULL,
			true
			);
	if( m_pBarInputFields == NULL )
	{
		ASSERT( FALSE );
		return -1;
	}
	m_pBarInputFields->m_nMdiMenuResourceID =
		IDR_MAINFRAME;
	m_pBarInputFields->m_nMdiAccelTableResourceID =
		IDR_MAINFRAME;
	nCmdID = (UINT)m_pBarInputFields->GetDlgCtrlID();
	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			nCmdID
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->StateSetBasic();
	if( ! m_dlgInputFields.CreateInsideBar( m_pBarInputFields ) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
	m_pBarColorControls =
		CExtDynamicBarSite::BarAlloc(
			_T("Color Controls"),
			icon,
			0,
			NULL,
			true
			);
	if( m_pBarColorControls == NULL )
	{
		ASSERT( FALSE );
		return -1;
	}
	m_pBarColorControls->m_nMdiMenuResourceID =
		IDR_MAINFRAME;
	m_pBarColorControls->m_nMdiAccelTableResourceID =
		IDR_MAINFRAME;
	nCmdID = (UINT)m_pBarColorControls->GetDlgCtrlID();
	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName,
			nCmdID
			);
	ASSERT( pCmdItem != NULL );
	pCmdItem->StateSetBasic();
	if( ! m_viewColorControls.CreateInsideBar( m_pBarColorControls ) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
	CExtDynamicBarSite::StateLoad(
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		pApp->m_pszProfileName
		);
	if(	! CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{
		DockControlBar( &m_wndMenuBar );
		DockControlBar( &m_wndToolBarStandard );
		DockControlBar( &m_wndToolBarThemes );
		DockControlBar( &m_wndToolBarSkins );
		m_pBarColorControls->BarStateSet(
			CExtDynamicControlBar::__EDBS_DOCUMENT,
			true
			);
#if (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)
		m_pBarDateTimeDuration->BarStateSet(
			CExtDynamicControlBar::__EDBS_DOCUMENT,
			true
			);
#endif // (!defined __EXT_MFC_NO_DURATIONWND ) && (!defined __EXT_MFC_NO_DATETIMEWND)
		m_pBarInputFields->BarStateSet(
			CExtDynamicControlBar::__EDBS_DOCUMENT,
			true
			);
		m_pBarButtons->BarStateSet(
			CExtDynamicControlBar::__EDBS_DOCUMENT,
			true
			);
		m_pBarInvokeModalDialogs->DockControlBarInnerOuter(
			AFX_IDW_DOCKBAR_RIGHT,
			true,
			this,
			false
			);
		ShowControlBar( m_pBarThemes, TRUE, TRUE );
		m_pBarThemes->DockControlBarLTRB(
			m_pBarInvokeModalDialogs,
			AFX_IDW_DOCKBAR_BOTTOM,
			true
			);
	}

	g_CmdManager->SerializeState(
		pApp->m_pszProfileName,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		false
		);

//m_wndToolBarStandard.FlashCaption( 65535, 250, RGB(128,0,0), RGB(255,255,64) );
//m_pBarButtons->FlashCaption( 65535, 250, RGB(128,0,0), RGB(255,255,64) );
//m_pBarInvokeModalDialogs->FlashCaption( 65535, 250, RGB(128,0,0), RGB(255,255,64) );

	return 0;
}

BOOL CMainFrame::DestroyWindow() 
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
	pApp;

	VERIFY(
		CExtControlBar::ProfileBarStateSave(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		);
	VERIFY(
		CExtDynamicBarSite::StateSave(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		);
	VERIFY(
		g_CmdManager->SerializeState(
			pApp->m_pszProfileName,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			true
			)
		);
	VERIFY(
		g_PaintManager.PaintManagerStateSave(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		);

	g_CmdManager->ProfileWndRemove( GetSafeHwnd() );
	m_wndMenuBar.RemoveAllWndHooks();
	
	return CExtNCW < CMDIFrameWnd > :: DestroyWindow();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CExtNCW < CMDIFrameWnd > :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	CExtNCW < CMDIFrameWnd > :: ActivateFrame(nCmdShow);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CMDIFrameWnd > :: PreCreateWindow(cs) )
		return FALSE;
	//cs.style |= WS_MAXIMIZE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CExtNCW < CMDIFrameWnd > :: AssertValid();
}

void CMainFrame::Dump( CDumpContext & dc ) const
{
	CExtNCW < CMDIFrameWnd > :: Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	return CExtNCW < CMDIFrameWnd > :: PreTranslateMessage(pMsg);
}


LRESULT CMainFrame::OnExtMenuPrepare(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;
	
	//////////////////////////////////////////////////////////////////////////
	// Add "Windows..." command
	//////////////////////////////////////////////////////////////////////////
	
	CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
		reinterpret_cast
		< CExtPopupMenuWnd::MsgPrepareMenuData_t * >
		( wParam );
	ASSERT( pData != NULL );
	CExtPopupMenuWnd * pPopup = pData->m_pPopup;
	ASSERT( pPopup != NULL );
	
	INT nItemPos;
	INT nNewPos = -1;
	
	nItemPos = pPopup->ItemFindPosForCmdID( __ID_MDIWND_DLGWINDOWS );
	if( nItemPos > 0 )
	{
		// "More Windows..." command found
		pPopup->ItemRemove( nItemPos );
		nNewPos = nItemPos;
	}
	else
	{
		int nMaxCmdID = 0;
		for( int nCmdID = __ID_MDIWNDLIST_FIRST; nCmdID <= __ID_MDIWNDLIST_LAST; nCmdID++ ){
			nItemPos = pPopup->ItemFindPosForCmdID( nCmdID );
			if( nItemPos > 0 ){
				if( nCmdID > nMaxCmdID ){
					nMaxCmdID = nCmdID;
					nNewPos = nItemPos;
				}
			}
		}
		if( nNewPos > 0 ){
			pPopup->ItemInsert(
				(UINT)CExtPopupMenuWnd::TYPE_SEPARATOR,
				++nNewPos
				);
			nNewPos++;
		}
	}
	if( nNewPos > 0 )
	{
		UINT nCmdID = ID_WINDOWS_LIST;
		CExtCmdItem * pCmdItem =
			g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd( this->GetSafeHwnd() ),
			nCmdID
			);
		if( pCmdItem == NULL ){
			pCmdItem = 
				g_CmdManager->CmdAllocPtr( 
				g_CmdManager->ProfileNameFromWnd( this->GetSafeHwnd() ), 
				nCmdID 
				);
		}
		ASSERT( pCmdItem != NULL );
		if( pCmdItem != NULL )
		{
			CExtSafeString sMoreWindows(_T("Windows..."));
			CExtSafeString sManageWindows(_T("Manages the currently open windows"));
			pCmdItem->m_sMenuText = sMoreWindows;
			pCmdItem->m_sToolbarText = pCmdItem->m_sMenuText;
			pCmdItem->m_sTipTool = sManageWindows;
			pCmdItem->m_sTipStatus = pCmdItem->m_sTipTool;
			pCmdItem->StateSetBasic( true );
			
			pPopup->ItemInsert(
				nCmdID,
				nNewPos,
				sMoreWindows,
				NULL,
				m_hWnd
				);
		}
	}
	return 1;
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( CExtDynamicBarSite::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
		return TRUE;
	if( nCode == CN_COMMAND )
	{
#ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
		if( nID == ID_WINDOWS_LIST )
		{
			CExtMdiWindowsListDlg dlgMdiWindowsList( this );
			dlgMdiWindowsList.DoModal();
			return TRUE;
		}
#endif // #ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
	}
	return CExtNCW < CMDIFrameWnd > :: OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnFileNew() 
{
	::AfxMessageBox( _T("CMainFrame::OnFileNew()") );
}

void CMainFrame::OnFileOpen() 
{
	::AfxMessageBox( _T("CMainFrame::OnFileOpen()") );
}

void CMainFrame::OnFileSave() 
{
	::AfxMessageBox( _T("CMainFrame::OnFileSave()") );
}

void CMainFrame::OnEditCopy() 
{
	::AfxMessageBox( _T("CMainFrame::OnThemeLoadSkinBin()") );
}

void CMainFrame::OnEditCut() 
{
	::AfxMessageBox( _T("CMainFrame::OnEditCopy()") );
}

void CMainFrame::OnEditPaste() 
{
	::AfxMessageBox( _T("CMainFrame::OnEditPaste()") );
}

void CMainFrame::OnComboBoxInToolbar() 
{
	::AfxMessageBox( _T("CMainFrame::OnComboBoxInToolbar()") );
}

void CMainFrame::OnTheme2000() 
{
	if(		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManager) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) ) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerSkin) ) )
		)
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManager )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
		| RDW_FRAME | RDW_ALLCHILDREN
		);
	CExtControlBar::stat_RedrawFloatingFrames( this );
	CExtControlBar::stat_RecalcBarMetrics( this );
}

void CMainFrame::OnUpdateTheme2000(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		(	g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManager) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) ) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerSkin) ) )
		)
		? TRUE
		: FALSE
		);
}

void CMainFrame::OnThemeXp() 
{
	if(		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerOffice2003) ) )
		)
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManagerXP )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
		| RDW_FRAME | RDW_ALLCHILDREN
		);
	CExtControlBar::stat_RedrawFloatingFrames( this );
	CExtControlBar::stat_RecalcBarMetrics( this );
}

void CMainFrame::OnUpdateThemeXp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		(	g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerXP) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerOffice2003) ) )
		)	? TRUE
		: FALSE
		);
}

void CMainFrame::OnTheme2003() 
{
	if(		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerOffice2003) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerStudio2005) ) )
		)
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManagerOffice2003 )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
		| RDW_FRAME | RDW_ALLCHILDREN
		);
	CExtControlBar::stat_RedrawFloatingFrames( this );
	CExtControlBar::stat_RecalcBarMetrics( this );
}

void CMainFrame::OnUpdateTheme2003(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		(	g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerOffice2003) )
		&&	(! g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerStudio2005) ) )
		)
			? TRUE
			: FALSE
		);
}

void CMainFrame::OnTheme2005() 
{
	if( g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerStudio2005) ) )
		return;
	VERIFY(
		g_PaintManager.InstallPaintManager( new CExtPaintManagerStudio2005 )
		);
	RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
		| RDW_FRAME | RDW_ALLCHILDREN
		);
	CExtControlBar::stat_RedrawFloatingFrames( this );
	CExtControlBar::stat_RecalcBarMetrics( this );
}

void CMainFrame::OnUpdateTheme2005(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetRadio(
		g_PaintManager->IsKindOf( RUNTIME_CLASS(CExtPaintManagerStudio2005) )
		? TRUE
		: FALSE
		);
}

void CMainFrame::OnThemeLoadSkinXml() 
{
CString strPathName;

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile(
		NULL,
		CExtShellDialogFile::__EFDT_OPEN_SINGLE
		);
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("XML files (*.XML)|*.XML|")
		_T("All files (*.*)|*.*|")
		_T("|")
		);
	dlgShellFile.m_nInitialFileTypeIndex = 0;
	dlgShellFile.m_bFilesMustExist = true;
	dlgShellFile.m_strCustomDialogCaption = _T("Load XML skin");
	if( dlgShellFile.DoModal() != IDOK )
		return;
	ASSERT( dlgShellFile.m_arrRetValNames.GetSize() == 1 );
	strPathName = LPCTSTR(dlgShellFile.m_arrRetValNames[0]);

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog _dlg(
		TRUE,
		_T("XML"),
		NULL,
		OFN_FILEMUSTEXIST
			|OFN_PATHMUSTEXIST
			|OFN_HIDEREADONLY
			|OFN_LONGNAMES
			|OFN_NOCHANGEDIR
			,
		_T("XML files (*.XML)|*.XML|")
		_T("All files (*.*)|*.*|")
		_T("|")
		,
		this
		);
TCHAR strCurrDir[_MAX_PATH];
	::GetCurrentDirectory( _MAX_PATH, strCurrDir );
	_dlg.m_ofn.lpstrInitialDir = strCurrDir;
	_dlg.m_ofn.lpstrTitle = _T("Load XML skin");
	if( _dlg.DoModal() != IDOK )
		return;
	strPathName = _dlg.GetPathName();
	ASSERT( ! strPathName.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtPaintManagerSkin * pPM = new CExtPaintManagerSkin;
	if( ! pPM->m_Skin.Load( LPCTSTR(strPathName) ) )
	{
		::AfxMessageBox( _T("Failed to load skin.") );
		delete pPM;
		return;
	}
	g_PaintManager.InstallPaintManager( pPM );
//	VERIFY( pPM->m_Skin.SerializeFile( _T("Test.Skin"), true, false ) );
}

void CMainFrame::OnThemeLoadSkinBin() 
{
CString strPathName;

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile(
		NULL,
		CExtShellDialogFile::__EFDT_OPEN_SINGLE
		);
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("Skin files (*.skin)|*.skin|")
		_T("All files (*.*)|*.*|")
		_T("|")
		);
	dlgShellFile.m_nInitialFileTypeIndex = 0;
	dlgShellFile.m_bFilesMustExist = true;
	dlgShellFile.m_strCustomDialogCaption = _T("Load binary skin");
	if( dlgShellFile.DoModal() != IDOK )
		return;
	ASSERT( dlgShellFile.m_arrRetValNames.GetSize() == 1 );
	strPathName = LPCTSTR(dlgShellFile.m_arrRetValNames[0]);

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog _dlg(
		TRUE,
		_T("skin"),
		NULL,
		OFN_FILEMUSTEXIST
			|OFN_PATHMUSTEXIST
			|OFN_HIDEREADONLY
			|OFN_LONGNAMES
			|OFN_NOCHANGEDIR
			,
		_T("Skin files (*.skin)|*.skin|")
		_T("All files (*.*)|*.*|")
		_T("|")
		,
		this
		);
TCHAR strCurrDir[_MAX_PATH];
	::GetCurrentDirectory( _MAX_PATH, strCurrDir );
	_dlg.m_ofn.lpstrInitialDir = strCurrDir;
	_dlg.m_ofn.lpstrTitle = _T("Load binary skin");
	if( _dlg.DoModal() != IDOK )
		return;
	strPathName = _dlg.GetPathName();
	ASSERT( ! strPathName.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtPaintManagerSkin * pPM = new CExtPaintManagerSkin;
	if( ! pPM->m_Skin.SerializeFile( LPCTSTR(strPathName), false, false ) )
	{
		::AfxMessageBox( _T("Failed to load skin.") );
		delete pPM;
		return;
	}
	g_PaintManager.InstallPaintManager( pPM );
}

