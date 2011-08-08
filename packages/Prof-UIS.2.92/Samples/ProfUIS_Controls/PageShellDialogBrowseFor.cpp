// PageShellDialogBrowseFor.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageShellDialogBrowseFor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )

/////////////////////////////////////////////////////////////////////////////
// CPageShellDialogBrowseFor dialog

CPageShellDialogBrowseFor::CPageShellDialogBrowseFor(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageShellDialogBrowseFor::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageShellDialogBrowseFor)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

void CPageShellDialogBrowseFor::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageShellDialogBrowseFor)
	DDX_Control(pDX, IDC_CHECK_JUMP_TO_CURRENT_DIRECTORY_INITIALLY, m_checkJumpToCurrentDirectoryInitially);
	DDX_Control(pDX, IDC_CHECK_ALLOW_CURRENT_DIRECTORY_CHANGING, m_checkAllowCurrentDirectoryChanging);
	DDX_Control(pDX, IDC_CHECK_COMPARE_WITH_WINDOWS, m_checkCompareWithWindowsDialogs);
	DDX_Control(pDX, IDC_CHECK_ENABLE_SHELL_CONTEXT_MENUS_BFF, m_checkEnableShellContextMenusBFF);
	DDX_Control(pDX, IDC_CHECK_SHOW_HIDDEN_ITEMS_BFF, m_checkShowHiddenItemsBFF);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_CAPTION_FOR_NEW_FOLDER_BUTTON, m_editCustomCaptionForNewFolderButton);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_CAPTION_FOR_EDIT_LABEL, m_editCustomCaptionForEditLabel);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_CAPTION_FOR_CANCEL_BUTTON1, m_editCustomCaptionForCancelButton1);
	DDX_Control(pDX, IDC_STATIC_ROOT_FOR_BROWSE_FOR_DIALOG, m_labelEditRootInBrowseForDialog);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_CAPTION_FOR_OK_BUTTON, m_editCustomCaptionForOkButton);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_CAPTION_FOR_BROWSE_FOR_DIALOG, m_editCustomCaptionForBrowseForDialog);
	DDX_Control(pDX, IDC_CHECK_SHOW_NEW_FOLDER_BUTTON, m_checkShowNewFolderButton);
	DDX_Control(pDX, IDC_CHECK_SHOW_EDIT, m_checkShowEdit);
	DDX_Control(pDX, IDC_EDIT_TITLE_TEXT, m_editTitleText);
	DDX_Control(pDX, IDC_EDIT_BFF_INITIAL_FOLDER, m_editInitialFolderForBrowseDialog);
	DDX_Control(pDX, IDC_CHECK_SR_SL, m_checkSaveRestoreShellLocation);
	DDX_Control(pDX, IDC_CHECK_SR_WP, m_checkSaveRestoreWindowPosition);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_BFF_RESULT, m_editResultOfBrowseDialog);
}

BEGIN_MESSAGE_MAP(CPageShellDialogBrowseFor, CPageBase)
	//{{AFX_MSG_MAP(CPageShellDialogBrowseFor)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_WINDOWS_DIALOG, OnButtonShowWindowsDialog)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_PROF_UIS_DIALOG, OnButtonShowProfUisDialog)
	ON_BN_CLICKED(IDC_BUTTON_SET_CUSTOM_ROOT, OnButtonSetCustomRoot)
	ON_BN_CLICKED(IDC_BUTTON_RESET_CUSTOM_ROOT, OnButtonResetCustomRoot)
	ON_BN_CLICKED(IDC_CHECK_COMPARE_WITH_WINDOWS, OnCheckCompareWithWindowsDialogs)
	ON_MESSAGE( BFFM_VALIDATEFAILED, BrowseCallbackProc )
	ON_BN_CLICKED(IDC_CHECK_SR_SL, OnCheckSrSl)
	ON_BN_CLICKED(IDC_CHECK_JUMP_TO_CURRENT_DIRECTORY_INITIALLY, OnCheckJumpToCurrentDirectoryInitially)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageShellDialogBrowseFor message handlers

BOOL CPageShellDialogBrowseFor::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	
	m_editTitleText.SetCueBanner( _T("Empty text here means no title.") );
static LPCTSTR g_strEmptyCommentCueBanner = _T("Empty text here means use default caption.");
	m_editCustomCaptionForOkButton.SetCueBanner( g_strEmptyCommentCueBanner );
	m_editCustomCaptionForBrowseForDialog.SetCueBanner( g_strEmptyCommentCueBanner );
	m_labelEditRootInBrowseForDialog.SetCueBanner( _T("Default shell root is used.") ); 
	m_editCustomCaptionForCancelButton1.SetCueBanner( g_strEmptyCommentCueBanner );
	m_editCustomCaptionForNewFolderButton.SetCueBanner( g_strEmptyCommentCueBanner );
	m_editCustomCaptionForEditLabel.SetCueBanner( g_strEmptyCommentCueBanner );
	m_checkEnableShellContextMenusBFF.SetCheck( 1 );
	m_checkShowHiddenItemsBFF.SetCheck( 0 );
	m_checkSaveRestoreShellLocation.SetCheck( 1 );
	m_checkSaveRestoreWindowPosition.SetCheck( 1 );
	m_checkJumpToCurrentDirectoryInitially.SetCheck( 0 );
	m_checkAllowCurrentDirectoryChanging.SetCheck( 1 );
	
	OnCheckSrSl();
	m_bInitComplete = true;
	
	return TRUE;
}

void CPageShellDialogBrowseFor::OnButtonShowWindowsDialog() 
{
BROWSEINFO _bi;
	::memset( &_bi, 0, sizeof(BROWSEINFO) );
TCHAR SelectFolder[MAX_PATH];
	_bi.hwndOwner = m_hWndFocus;
	if( m_pidlCustomRoot != NULL )
		_bi.pidlRoot = m_pidlCustomRoot;
CString _strTextTitle;
	m_editTitleText.GetWindowText( _strTextTitle );
	if( ! _strTextTitle.IsEmpty() )
		_bi.lpszTitle = _strTextTitle;
	if( m_checkShowEdit.GetCheck() == 1 )
		_bi.ulFlags = BIF_EDITBOX/*|BIF_VALIDATE*/;
	_bi.pszDisplayName = SelectFolder;
CExtPIDL pidl;
	pidl.Attach( (ITEMIDLIST*)(LPITEMIDLIST)::SHBrowseForFolder( &_bi ) );
	if( ! pidl.IsEmpty() )
	{
		CExtSafeString str = pidl.GetPath();
		if( str.IsEmpty() )
			str = SelectFolder;
		m_editResultOfBrowseDialog.SetWindowText( str );
		pidl.Empty();
	}
	else
		m_editResultOfBrowseDialog.SetWindowText( __CANCEL_BUTTON_PRESSED_EVENT_RESULT_DESCRIPTION );
}

void CPageShellDialogBrowseFor::OnButtonShowProfUisDialog() 
{
CString strCustomDialogCaption;
	m_editCustomCaptionForBrowseForDialog.GetWindowText( strCustomDialogCaption );
CString strCustomOkButtonCaption;
	m_editCustomCaptionForOkButton.GetWindowText( strCustomOkButtonCaption );
CString strTextTitle;
	m_editTitleText.GetWindowText( strTextTitle );
CString strCustomCancelButtonCaption;
	m_editCustomCaptionForCancelButton1.GetWindowText( strCustomCancelButtonCaption );
CString strCustomNewFolderButtonCaption;
	m_editCustomCaptionForNewFolderButton.GetWindowText( strCustomNewFolderButtonCaption );
CString strCustomEditLabelCaption;
	m_editCustomCaptionForEditLabel.GetWindowText( strCustomEditLabelCaption );
CExtShellDialogBrowseFor dlgShellBrowseFor( this );
	if( ! strCustomDialogCaption.IsEmpty() )
		dlgShellBrowseFor.m_strCustomDialogCaption = LPCTSTR(strCustomDialogCaption);
	if( ! strCustomOkButtonCaption.IsEmpty() )
		dlgShellBrowseFor.m_strCustomOkButtonCaption = LPCTSTR(strCustomOkButtonCaption);
	if( ! strCustomCancelButtonCaption.IsEmpty() )
		dlgShellBrowseFor.m_strCustomCancelButtonCaption = LPCTSTR(strCustomCancelButtonCaption);
	if( ! strCustomNewFolderButtonCaption.IsEmpty() )
		dlgShellBrowseFor.m_strCustomNewFolderButtonCaption = LPCTSTR(strCustomNewFolderButtonCaption);
	if( ! strCustomEditLabelCaption.IsEmpty() )
		dlgShellBrowseFor.m_strCustomEditLabelCaption = LPCTSTR(strCustomEditLabelCaption);
	if( ! strTextTitle.IsEmpty() )
		dlgShellBrowseFor.m_strLabelAtTop = LPCTSTR(strTextTitle);
	if( m_checkShowHiddenItemsBFF.GetCheck() != 0 )
		dlgShellBrowseFor.m_wndShellTree.m_dwAttributeFilterAny |= SFGAO_HIDDEN;
	else
		dlgShellBrowseFor.m_wndShellTree.m_dwAttributeFilterAllAbsent |= SFGAO_HIDDEN;
	dlgShellBrowseFor.m_wndShellTree.ShowShellContextMenusSet( ( m_checkEnableShellContextMenusBFF.GetCheck() != 0 ) ? true : false );
	dlgShellBrowseFor.m_bShowMakeNewFolderButton = ( m_checkShowNewFolderButton.GetCheck() == 1 ) ? true : false;
	dlgShellBrowseFor.m_bShowFolderEdit = ( m_checkShowEdit.GetCheck() == 1 ) ? true : false;
	if( m_pidlCustomRoot != NULL )
		dlgShellBrowseFor.m_pidlRoot = m_pidlCustomRoot;
CString strInitialFolder;
	m_editInitialFolderForBrowseDialog.GetWindowText( strInitialFolder );
	strInitialFolder.TrimLeft( _T(" \r\n\t") );
	strInitialFolder.TrimRight( _T(" \r\n\t") );
	if( ! strInitialFolder.IsEmpty() )
		dlgShellBrowseFor.m_pidlResult.FromFolder( LPCTSTR(strInitialFolder), NULL );

	dlgShellBrowseFor.m_bCurrentDirectorySetInitially = ( m_checkJumpToCurrentDirectoryInitially.GetCheck() != 0 ) ? true : false;
	dlgShellBrowseFor.m_bCurrentDirectorySetOnWalk = ( m_checkAllowCurrentDirectoryChanging.GetCheck() != 0 ) ? true : false;
	dlgShellBrowseFor.m_bSaveRestoreShellLocation = ( m_checkSaveRestoreShellLocation.GetCheck() != 0 ) ? true : false;
	dlgShellBrowseFor.m_bSaveRestoreWindowPosition = ( m_checkSaveRestoreWindowPosition.GetCheck() != 0 ) ? true : false;

	if( dlgShellBrowseFor.DoModal() != IDOK )
		m_editResultOfBrowseDialog.SetWindowText( __CANCEL_BUTTON_PRESSED_EVENT_RESULT_DESCRIPTION );
	else if( ! dlgShellBrowseFor.m_pidlResult.IsEmpty() )
	{
		CExtSafeString str = dlgShellBrowseFor.m_pidlResult.GetPath();
		if( str.IsEmpty() )
			str = dlgShellBrowseFor.m_strDisplayNameResult;
		m_editResultOfBrowseDialog.SetWindowText( str );
	}
	else
		m_editResultOfBrowseDialog.SetWindowText( __CANCEL_BUTTON_PRESSED_EVENT_RESULT_DESCRIPTION );
}

LRESULT CPageShellDialogBrowseFor::BrowseCallbackProc( WPARAM wParam, LPARAM lParam )
{
	wParam;
	lParam;
	return 0;
}

void CPageShellDialogBrowseFor::OnButtonSetCustomRoot() 
{
BROWSEINFO _bi;
	::memset( &_bi, 0, sizeof(BROWSEINFO) );
	_bi.hwndOwner = m_hWndFocus;
	_bi.lpszTitle = _T("Select custom root");
	m_pidlCustomRoot.Empty();
	m_pidlCustomRoot.Attach( (ITEMIDLIST*)(LPITEMIDLIST)::SHBrowseForFolder( &_bi ) );

	m_strRootCurrent = _T("");
	if( ! m_pidlCustomRoot.IsRoot() )
	{
		m_strRootCurrent = LPCTSTR(m_pidlCustomRoot.GetPath());
		if( m_strRootCurrent.IsEmpty() )
			m_strRootCurrent = LPCTSTR(m_pidlCustomRoot.GetDisplayNameOf());
	}
	m_labelEditRootInBrowseForDialog.SetWindowText( LPCTSTR(m_strRootCurrent) ); 
}

void CPageShellDialogBrowseFor::OnButtonResetCustomRoot() 
{
	m_pidlCustomRoot.Empty();

	m_strRootCurrent = _T("");
	m_labelEditRootInBrowseForDialog.SetWindowText( LPCTSTR(m_strRootCurrent) ); 
}

void CPageShellDialogBrowseFor::OnCheckCompareWithWindowsDialogs() 
{
UINT nShowCmd = ( m_checkCompareWithWindowsDialogs.GetCheck() != 0 ) ? SW_SHOW : SW_HIDE;
	GetDlgItem( IDC_BUTTON_SHOW_WINDOWS_DIALOG ) -> ShowWindow( nShowCmd );
}


void CPageShellDialogBrowseFor::OnCheckSrSl() 
{
BOOL bEnable = ( m_checkSaveRestoreShellLocation.GetCheck() != 0 ) ? FALSE : TRUE;
	m_checkJumpToCurrentDirectoryInitially.EnableWindow( bEnable );
	m_editInitialFolderForBrowseDialog.EnableWindow( bEnable );
}

void CPageShellDialogBrowseFor::OnCheckJumpToCurrentDirectoryInitially() 
{
BOOL bEnable = ( m_checkJumpToCurrentDirectoryInitially.GetCheck() != 0 ) ? FALSE : TRUE;
	m_checkSaveRestoreShellLocation.EnableWindow( bEnable );
	m_editInitialFolderForBrowseDialog.EnableWindow( bEnable );
}

#endif // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_BFF )
