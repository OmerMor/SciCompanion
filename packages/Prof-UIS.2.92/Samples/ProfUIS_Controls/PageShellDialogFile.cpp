// PageShellDialogFile.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageShellDialogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

/////////////////////////////////////////////////////////////////////////////
// CPageShellDialogFile dialog

CPageShellDialogFile::CPageShellDialogFile(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageShellDialogFile::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageShellDialogFile)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

void CPageShellDialogFile::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageShellDialogFile)
	DDX_Control(pDX, IDC_CHECK_JUMP_TO_CURRENT_DIRECTORY_INITIALLY, m_checkJumpToCurrentDirectoryInitially);
	DDX_Control(pDX, IDC_CHECK_ALLOW_CURRENT_DIRECTORY_CHANGING, m_checkAllowCurrentDirectoryChanging);
	DDX_Control(pDX, IDC_CHECK_COMPARE_WITH_WINDOWS, m_checkCompareWithWindowsDialogs);
	DDX_Control(pDX, IDC_CHECK_SHOW_TREE, m_checkShowTree);
	DDX_Control(pDX, IDC_CHECK_USE_SPLITTER, m_checkUseSplitter);
	DDX_Control(pDX, IDC_CHECK_COLLAPSING_ANIMATION, m_checkCollapsingAnimation);
	DDX_Control(pDX, IDC_CHECK_SHOW_ROOT_FOLDER, m_checkShowRootFolder);
	DDX_Control(pDX, IDC_CHECK_SHOW_PARENT_FOLDER, m_checkShowParentFolder);
	DDX_Control(pDX, IDC_CHECK_ENABLE_SHELL_CONTEXT_MENUS_FD, m_checkEnableShellContextMenusFD);
	DDX_Control(pDX, IDC_CHECK_INITIALLY_COLLAPSED, m_checkInitiallyCollapsed);
	DDX_Control(pDX, IDC_CHECK_ENABLE_COLLAPSING, m_checkEnableCollapsing);
	DDX_Control(pDX, IDC_CHECK_SHOW_HIDDEN_ITEMS_FD, m_checkShowHiddenItemsFD);
	DDX_Control(pDX, IDC_CHECK_RESOLVE_LINKS, m_checkResolveLinks);
	DDX_Control(pDX, IDC_CHECK_WRITE_ACCESS, m_checkWriteAccess);
	DDX_Control(pDX, IDC_CHECK_SHOW_FT_COMBO, m_checkShowFTCombo);
	DDX_Control(pDX, IDC_CHECK_READ_ACCESS, m_checkReadAccess);
	DDX_Control(pDX, IDC_CHECK_PROMPT_OVERWRITE, m_checkPromptOverwrite);
	DDX_Control(pDX, IDC_CHECK_FNE_SHOW, m_checkFNEShow);
	DDX_Control(pDX, IDC_CHECK_FNE_READ_ONLY, m_checkFNEReadOnly);
	DDX_Control(pDX, IDC_CHECK_FILES_MUST_EXIST, m_checkFilesMustExist);
	DDX_Control(pDX, IDC_CHECK_PATH_MUST_EXIST, m_checkPathMustExist);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_CAPTION_FOR_FILE_TYPE_LABEL, m_editCustomCaptionForFileTypeLabel);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_CAPTION_FOR_FILE_NAME_LABEL, m_editCustomCaptionForFileNameLabel);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_CAPTION_FOR_CANCEL_BUTTON2, m_editCustomCaptionForCancelButton2);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_CAPTION_FOR_OPEN_SAVE_BUTTON, m_editCustomCaptionForOpenSaveButton);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_CAPTION_FOR_FILE_DIALOG, m_editCustomCaptionForFileDialog);
	DDX_Control(pDX, IDC_COMBO_FILE_DIALOG_TYPE, m_comboFileDialogType);
	DDX_Control(pDX, IDC_EDIT_FD_INITIAL_FOLDER, m_editInitialFolderForFileDialog);
	DDX_Control(pDX, IDC_EDIT_FD_FT_INIT_IDX, m_editInitialFileTypeIndex);
	DDX_Control(pDX, IDC_SPIN_FD_FT_INIT_IDX, m_spinInitialFileTypeIndex);
	DDX_Control(pDX, IDC_EDIT_FD_INITIAL_FILE_NAMES, m_editInitialFileNames );
	DDX_Control(pDX, IDC_CHECK_SR_SL, m_checkSaveRestoreShellLocation);
	DDX_Control(pDX, IDC_CHECK_SR_WP, m_checkSaveRestoreWindowPosition);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_FD_FILE_FILTER, m_editFileDialogFilter);
	DDX_Control(pDX, IDC_EDIT_FD_RESULT, m_editResultOfFileDialog);
}


BEGIN_MESSAGE_MAP(CPageShellDialogFile, CPageBase)
	//{{AFX_MSG_MAP(CPageShellDialogFile)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_WINDOWS_DIALOG, OnButtonShowWindowsDialog)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_PROF_UIS_DIALOG, OnButtonShowProfUisDialog)
	ON_BN_CLICKED(IDC_CHECK_COMPARE_WITH_WINDOWS, OnCheckCompareWithWindowsDialogs)
	ON_BN_CLICKED(IDC_CHECK_SR_SL, OnCheckSrSl)
	ON_BN_CLICKED(IDC_CHECK_JUMP_TO_CURRENT_DIRECTORY_INITIALLY, OnCheckJumpToCurrentDirectoryInitially)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageShellDialogFile message handlers

BOOL CPageShellDialogFile::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	
	m_comboFileDialogType.SetCurSel( 0 );
static LPCTSTR g_strEmptyCommentCueBanner = _T("Empty text here means use default caption.");
	m_editCustomCaptionForFileDialog.SetCueBanner( g_strEmptyCommentCueBanner );
	m_editCustomCaptionForOpenSaveButton.SetCueBanner( g_strEmptyCommentCueBanner );
	m_editCustomCaptionForCancelButton2.SetCueBanner( g_strEmptyCommentCueBanner );
	m_editCustomCaptionForFileTypeLabel.SetCueBanner( g_strEmptyCommentCueBanner );
	m_editCustomCaptionForFileNameLabel.SetCueBanner( g_strEmptyCommentCueBanner );
	m_editFileDialogFilter.SetWindowText( 
		_T("Portable Network Graphics (*.png;*.mng)|*.png; *.mng|")
		_T("JPG Files (*.jpg;*.jp2)|*.jpg; *.jp2|")
		_T("GIF Files (*.gif)|*.gif|")
		_T("PCX Files (*.pcx)|*.pcx|")
		_T("Windows bitmaps (*.bmp)|*.bmp|")
		_T("Windows icons (*.ico)|*.ico|")
		_T("All Files (*.*)|*.*|")
		_T("|")
		);
	m_checkFNEShow.SetCheck( 1 );
	m_checkFNEReadOnly.SetCheck( 0 );
	m_checkShowFTCombo.SetCheck( 1 );
	m_checkFilesMustExist.SetCheck( 0 );
	m_checkPathMustExist.SetCheck( 0 );
	m_checkReadAccess.SetCheck( 0 );
	m_checkWriteAccess.SetCheck( 0 );
	m_checkPromptOverwrite.SetCheck( 1 ); // used in save dialogs only
	m_checkShowHiddenItemsFD.SetCheck( 0 );
	m_checkResolveLinks.SetCheck( 1 );
	m_checkEnableShellContextMenusFD.SetCheck( 1 );
	m_checkEnableCollapsing.SetCheck( 1 );
	m_checkUseSplitter.SetCheck( 1 );
	m_checkShowTree.SetCheck( 1 );
	m_checkSaveRestoreShellLocation.SetCheck( 1 );
	m_checkSaveRestoreWindowPosition.SetCheck( 1 );
	m_checkJumpToCurrentDirectoryInitially.SetCheck( 0 );
	m_checkAllowCurrentDirectoryChanging.SetCheck( 1 );
	
	OnCheckSrSl();
	m_bInitComplete = true;
	
	return TRUE;
}

void CPageShellDialogFile::OnButtonShowWindowsDialog() 
{
CExtShellDialogFile::e_file_dialog_type_t _eFDT =
		(CExtShellDialogFile::e_file_dialog_type_t)
			m_comboFileDialogType.GetCurSel();
	ASSERT( CExtShellDialogFile::__EFDT_MIN_VALUE <= _eFDT && _eFDT <= CExtShellDialogFile::__EFDT_MAX_VALUE );
CString strCustomDialogCaption;
	m_editCustomCaptionForFileDialog.GetWindowText( strCustomDialogCaption );
CString strFilter;
	m_editFileDialogFilter.GetWindowText( strFilter );
CFileDialog dlgFile(
		(	_eFDT <= CExtShellDialogFile::__EFDT_OPEN_SINGLE
		||	_eFDT <= CExtShellDialogFile::__EFDT_OPEN_MULTIPLE
		)
			? TRUE : FALSE
			,
		_T("jpg"),
		strCustomDialogCaption.IsEmpty() ? NULL : LPCTSTR(strCustomDialogCaption),
		0
			| ( ( m_checkShowHiddenItemsFD.GetCheck() != 0 ) ? 0 : OFN_HIDEREADONLY )
			| ( ( m_checkPromptOverwrite.GetCheck() != 0 ) ? OFN_OVERWRITEPROMPT : 0 )
			,
		strFilter.IsEmpty()
			?	_T("All Files (*.*)|*.*|")
				_T("|") 
			:	strFilter
		,
		this
		);
	dlgFile.m_ofn.nFilterIndex = 1;
	dlgFile.m_ofn.Flags |= OFN_EXPLORER;
	if( m_checkFilesMustExist.GetCheck() != 0 )
		dlgFile.m_ofn.Flags |= OFN_FILEMUSTEXIST;
	if( m_checkPathMustExist.GetCheck() != 0 )
		dlgFile.m_ofn.Flags |= OFN_PATHMUSTEXIST;
	if( m_checkPromptOverwrite.GetCheck() != 0 ) // used in save dialogs only
		dlgFile.m_ofn.Flags |= OFN_OVERWRITEPROMPT;
CString strInitialFolder;
	m_editInitialFolderForFileDialog.GetWindowText( strInitialFolder );
	strInitialFolder.TrimLeft( _T(" \r\n\t") );
	strInitialFolder.TrimRight( _T(" \r\n\t") );
	if( ! strInitialFolder.IsEmpty() )
		dlgFile.m_ofn.lpstrInitialDir = LPTSTR(LPCTSTR(strInitialFolder));

//CString _strInitialFileNames;
// 	m_editInitialFileNames.GetWindowText( _strInitialFileNames );
// 	_strInitialFileNames.TrimLeft( _T(" \r\n\t") );
// 	_strInitialFileNames.TrimRight( _T(" \r\n\t") );
// 	dlgFile.m_ofn.lpstrFile = LPTSTR(LPCTSTR(_strInitialFileNames));

INT nRetVal = (INT)dlgFile.DoModal();
	if( nRetVal == IDOK )
		m_editResultOfFileDialog.SetWindowText( dlgFile.m_ofn.lpstrFile );
	else
		m_editResultOfFileDialog.SetWindowText( __CANCEL_BUTTON_PRESSED_EVENT_RESULT_DESCRIPTION );
}

void CPageShellDialogFile::OnButtonShowProfUisDialog() 
{
CExtShellDialogFile::e_file_dialog_type_t _eFDT =
		(CExtShellDialogFile::e_file_dialog_type_t)
			m_comboFileDialogType.GetCurSel();
	ASSERT( CExtShellDialogFile::__EFDT_MIN_VALUE <= _eFDT && _eFDT <= CExtShellDialogFile::__EFDT_MAX_VALUE );
CString strCustomDialogCaption;
	m_editCustomCaptionForFileDialog.GetWindowText( strCustomDialogCaption );
CString strCustomOpenSaveButtonCaption;
	m_editCustomCaptionForOpenSaveButton.GetWindowText( strCustomOpenSaveButtonCaption );
CString strCustomCancelButtonCaption;
	m_editCustomCaptionForCancelButton2.GetWindowText( strCustomCancelButtonCaption );
CString strCustomFileNameLabelCaption;
	m_editCustomCaptionForFileNameLabel.GetWindowText( strCustomFileNameLabelCaption );
CString strCustomFileTypeLabelCaption;
	m_editCustomCaptionForFileTypeLabel.GetWindowText( strCustomFileTypeLabelCaption );
CExtShellDialogFile dlgShellFile( this, _eFDT );
	////// dlgShellFile.m_bSaveRestoreInterriorOptions = false;
	if( ! strCustomDialogCaption.IsEmpty() )
		dlgShellFile.m_strCustomDialogCaption = LPCTSTR(strCustomDialogCaption);
	if( ! strCustomOpenSaveButtonCaption.IsEmpty() )
		dlgShellFile.m_strCustomOpenSaveButtonCaption = LPCTSTR(strCustomOpenSaveButtonCaption);
	if( ! strCustomCancelButtonCaption.IsEmpty() )
		dlgShellFile.m_strCustomCancelButtonCaption = LPCTSTR(strCustomCancelButtonCaption);
	if( ! strCustomFileNameLabelCaption.IsEmpty() )
		dlgShellFile.m_strCustomFileNameLabelCaption = LPCTSTR(strCustomFileNameLabelCaption);
	if( ! strCustomFileTypeLabelCaption.IsEmpty() )
		dlgShellFile.m_strCustomFileTypeLabelCaption = LPCTSTR(strCustomFileTypeLabelCaption);
CString strFilter;
	m_editFileDialogFilter.GetWindowText( strFilter );
	if( strFilter.IsEmpty() )
		dlgShellFile.m_comboFileTypes.SetFilter(
			_T("All Files (*.*)|*.*|")
			_T("|")
			);
	else
		dlgShellFile.m_comboFileTypes.SetFilter( LPCTSTR(strFilter) );
	dlgShellFile.m_bShowFileNamesEdit = ( m_checkFNEShow.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bReadOnlyFileNamesEdit = ( m_checkFNEReadOnly.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bShowFileTypesComboBox = ( m_checkShowFTCombo.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bFilesMustExist = ( m_checkFilesMustExist.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bPathMustExist = ( m_checkPathMustExist.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bCheckReadAccess = ( m_checkReadAccess.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bCheckWriteAccess = ( m_checkWriteAccess.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bPromptOverwrite = ( m_checkPromptOverwrite.GetCheck() != 0 ) ? true : false; // used in save dialogs only
	dlgShellFile.m_bResolveLinks = ( m_checkResolveLinks.GetCheck() != 0 ) ? true : false;
	if( m_checkShowHiddenItemsFD.GetCheck() != 0 )
	{
		dlgShellFile.m_wndShellTree.m_dwAttributeFilterAny |= SFGAO_HIDDEN;
		dlgShellFile.m_wndShellList.m_dwAttributeFilterAny |= SFGAO_HIDDEN;
	}
	else
	{
		dlgShellFile.m_wndShellTree.m_dwAttributeFilterAllAbsent |= SFGAO_HIDDEN;
		dlgShellFile.m_wndShellList.m_dwAttributeFilterAllAbsent |= SFGAO_HIDDEN;
	}
	dlgShellFile.m_bUseSplitter = ( m_checkUseSplitter.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bShowTree = ( m_checkShowTree.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bEnableCollapsing = ( m_checkEnableCollapsing.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bCollapseInitially = ( m_checkInitiallyCollapsed.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bEnableCollapsingAnimation = ( m_checkCollapsingAnimation.GetCheck() != 0 ) ? true : false;
	if( m_checkEnableShellContextMenusFD.GetCheck() != 0 )
	{
		dlgShellFile.m_wndShellList.ShowShellContextMenusSet( true );
		dlgShellFile.m_wndShellTree.ShowShellContextMenusSet( true );
	}
	else
	{
		dlgShellFile.m_wndShellList.ShowShellContextMenusSet( false );
		dlgShellFile.m_wndShellTree.ShowShellContextMenusSet( false );
	}
	dlgShellFile.m_wndShellList.ShowRootFolderItemSet( ( m_checkShowRootFolder.GetCheck() != 0 ) ? true : false );
	dlgShellFile.m_wndShellList.ShowParentFolderItemSet( ( m_checkShowParentFolder.GetCheck() != 0 ) ? true : false );
CString strInitialFolder;
	m_editInitialFolderForFileDialog.GetWindowText( strInitialFolder );
	strInitialFolder.TrimLeft( _T(" \r\n\t") );
	strInitialFolder.TrimRight( _T(" \r\n\t") );
	if( ! strInitialFolder.IsEmpty() )
	{
		// dlgShellFile.m_strRetValFolder = LPCTSTR(strInitialFolder);
		dlgShellFile.m_pidlRetValFolder.FromFolder( LPCTSTR(strInitialFolder), NULL );
	}
CString strInitialFileTypeIndex;
	m_editInitialFileTypeIndex.GetWindowText( strInitialFileTypeIndex );
	dlgShellFile.m_nInitialFileTypeIndex = _ttoi( LPCTSTR(strInitialFileTypeIndex) );
//	dlgShellFile.m_nInitialFileTypeIndex = dlgShellFile.m_comboFileTypes.FindFilterIndexByExtension( _T("*.pcx") );
CString _strInitialFileNames;
	m_editInitialFileNames.GetWindowText( _strInitialFileNames );
	_strInitialFileNames.TrimLeft( _T(" \r\n\t") );
	_strInitialFileNames.TrimRight( _T(" \r\n\t") );
	dlgShellFile.m_strEditorTextInitial = LPCTSTR(_strInitialFileNames);
	
	dlgShellFile.m_bCurrentDirectorySetInitially = ( m_checkJumpToCurrentDirectoryInitially.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bCurrentDirectorySetOnWalk = ( m_checkAllowCurrentDirectoryChanging.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bSaveRestoreShellLocation = ( m_checkSaveRestoreShellLocation.GetCheck() != 0 ) ? true : false;
	dlgShellFile.m_bSaveRestoreWindowPosition = ( m_checkSaveRestoreWindowPosition.GetCheck() != 0 ) ? true : false;

	
	// VERIFY( dlgShellFile.m_pidlRoot.GetSpecialFolderLocation( m_hWnd, CSIDL_DRIVES ) ); // custom shell root test


INT nRetVal = (INT)dlgShellFile.DoModal();
	if( nRetVal == IDOK )
	{
//		m_editResultOfFileDialog.SetWindowText( dlgShellFile.m_strRetValNames );

// 			CExtSafeString str;
// 			INT nIndex, nCount = INT( dlgShellFile.m_arrRetVal.GetSize() );
// 			for( nIndex = 0; nIndex < nCount; nIndex ++ )
// 			{
// 				CExtSafeString strTmp = dlgShellFile.m_arrRetVal.GetObjAt( nIndex ) -> GetPath();
// 				if( nIndex > 0 )
// 					str += _T(" ");
// 				str += _T("\"");
// 				str += LPCTSTR(strTmp);
// 				str += _T("\"");
// 			}
// 			m_editResultOfFileDialog.SetWindowText( LPCTSTR(str) );

		CExtSafeString str;
		INT nIndex, nCount = INT( dlgShellFile.m_arrRetValNames.GetSize() );
		for( nIndex = 0; nIndex < nCount; nIndex ++ )
		{
			CExtSafeString & strTmp = dlgShellFile.m_arrRetValNames.ElementAt( nIndex );
			if( nIndex > 0 )
				str += _T(" ");
			str += _T("\"");
			str += LPCTSTR(strTmp);
			str += _T("\"");
		}
		m_editResultOfFileDialog.SetWindowText( LPCTSTR(str) );
	
	}
	else
		m_editResultOfFileDialog.SetWindowText( __CANCEL_BUTTON_PRESSED_EVENT_RESULT_DESCRIPTION );
}

void CPageShellDialogFile::OnCheckCompareWithWindowsDialogs() 
{
UINT nShowCmd = ( m_checkCompareWithWindowsDialogs.GetCheck() != 0 ) ? SW_SHOW : SW_HIDE;
	GetDlgItem( IDC_BUTTON_SHOW_WINDOWS_DIALOG ) -> ShowWindow( nShowCmd );
}

void CPageShellDialogFile::OnCheckSrSl() 
{
BOOL bEnable = ( m_checkSaveRestoreShellLocation.GetCheck() != 0 ) ? FALSE : TRUE;
	m_checkJumpToCurrentDirectoryInitially.EnableWindow( bEnable );
	m_editInitialFolderForFileDialog.EnableWindow( bEnable );
}

void CPageShellDialogFile::OnCheckJumpToCurrentDirectoryInitially() 
{
BOOL bEnable = ( m_checkJumpToCurrentDirectoryInitially.GetCheck() != 0 ) ? FALSE : TRUE;
	m_checkSaveRestoreShellLocation.EnableWindow( bEnable );
	m_editInitialFolderForFileDialog.EnableWindow( bEnable );
}

#endif // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

