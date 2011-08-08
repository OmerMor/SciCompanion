// PageShellListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageShellListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CPageShellListCtrl dialog

CPageShellListCtrl::CPageShellListCtrl(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageShellListCtrl::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageShellListCtrl)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

void CPageShellListCtrl::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageShellListCtrl)
	DDX_Control( pDX, IDC_CHECK_ROOT_FOLDER_STATE, m_checkRootFolder );
	DDX_Control( pDX, IDC_CHECK_FOLDER_BEFORE_OTHER, m_checkFolderBeforeOther );
	DDX_Control( pDX, IDC_CHECK_EDITING, m_checkEdit );
	DDX_Control( pDX, IDC_CHECK_VIEW_FILES, m_checkViewFiles );
	DDX_Control( pDX, IDC_CHECK_UP_FOLDER_STATE, m_checkUpFolder );
	DDX_Control( pDX, IDC_CHECK_SHOW_HIDDEN_STATE, m_checkShowHiddenState );
	DDX_Control( pDX, IDC_CHECK_WATCH_FILE_SYSTEM, m_checkWatchFileSystem );
	DDX_Control( pDX, IDC_COMBO_LIST_SORT, m_comboListSort );
	DDX_Control( pDX, ID_BOTTON_GO, m_buttonGo );
	DDX_Control( pDX, IDC_EDIT_PATH, m_editPath );
	DDX_Control( pDX, IDC_COMBO_LIST_TYPE, m_comboListType );
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_SHELL_LIST_CTRL, m_wndList );
}

BEGIN_MESSAGE_MAP(CPageShellListCtrl, CPageBase)
	//{{AFX_MSG_MAP(CPageShellListCtrl)
	ON_CBN_SELENDOK( IDC_COMBO_LIST_TYPE, OnSelendokComboListType )
	ON_BN_CLICKED( ID_BOTTON_GO, OnBottonGo )
	ON_CBN_SELENDOK( IDC_COMBO_LIST_SORT, OnSelendokComboListSort )
	ON_BN_CLICKED( IDC_CHECK_EDITING, OnCheckEditing )
	ON_BN_CLICKED( IDC_CHECK_VIEW_FILES, OnCheckViewFiles )
	ON_BN_CLICKED( IDC_CHECK_UP_FOLDER_STATE, OnCheckUpFolder )
	ON_BN_CLICKED( IDC_CHECK_FOLDER_BEFORE_OTHER, OnCheckFolderBeforeOther )
	ON_BN_CLICKED( IDC_CHECK_ROOT_FOLDER_STATE, OnCheckRootFolder )
	ON_BN_CLICKED( IDC_CHECK_SHOW_HIDDEN_STATE, OnCheckShowHidden )
	ON_BN_CLICKED( IDC_CHECK_WATCH_FILE_SYSTEM, OnCheckWatchFileSystem )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageShellListCtrl message handlers

BOOL CPageShellListCtrl::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	
	AddAnchor( IDC_SHELL_LIST_CTRL, __RDA_LT, __RDA_RB );
	AddAnchor( IDC_COMBO_LIST_TYPE, __RDA_RT );
	AddAnchor( IDC_EDIT_PATH, __RDA_LT, __RDA_RT );
	AddAnchor( ID_BOTTON_GO, __RDA_RT );
	AddAnchor( IDC_COMBO_LIST_SORT, __RDA_RT );
	AddAnchor( IDC_CHECK_EDITING, __RDA_RT );
	AddAnchor( IDC_CHECK_VIEW_FILES, __RDA_RT );
	AddAnchor( IDC_CHECK_UP_FOLDER_STATE, __RDA_RT );
	AddAnchor( IDC_CHECK_FOLDER_BEFORE_OTHER, __RDA_RT );
	AddAnchor( IDC_CHECK_ROOT_FOLDER_STATE, __RDA_RT );
	AddAnchor( IDC_CHECK_SHOW_HIDDEN_STATE, __RDA_RT );
	AddAnchor( IDC_CHECK_WATCH_FILE_SYSTEM, __RDA_RT );

	m_wndList.SetExtendedStyle( m_wndList.GetExtendedStyle() | LVS_EX_HEADERDRAGDROP );

//	VERIFY( m_wndList.FocusPath( _T("C:\\") ) );
	VERIFY( m_wndList.FocusPIDL() );
	m_checkUpFolder.SetCheck(1);
	m_checkViewFiles.SetCheck(1);
	m_checkFolderBeforeOther.SetCheck(1);
	m_checkRootFolder.SetCheck(1);
	m_checkWatchFileSystem.SetCheck(1);

	m_comboListType.SetCurSel( 2 );
	OnSelendokComboListType();
	m_comboListSort.SetCurSel( 0 );
	OnSelendokComboListSort();

	OnCheckUpFolder();
	OnCheckRootFolder();
	
	m_bInitComplete = true;
	
	return TRUE;
}

void CPageShellListCtrl::OnSelendokComboListType() 
{
int nSel = m_comboListType.GetCurSel();
	if( nSel < 0 )
		return;
	ASSERT( nSel < 4 );
DWORD dwArrStyles[4] = { LVS_SMALLICON, LVS_ICON, LVS_LIST, LVS_REPORT };
	m_wndList.ModifyStyle( LVS_TYPEMASK, dwArrStyles[nSel], SWP_FRAMECHANGED );
	
}

void CPageShellListCtrl::OnBottonGo() 
{
CString strPath;
	m_editPath.GetWindowText( strPath );
	if( ! m_wndList.FocusPath( LPCTSTR(strPath) ) )
		::Beep( 8000, 100 );

}

void CPageShellListCtrl::OnSelendokComboListSort() 
{
int nSel = m_comboListSort.GetCurSel();
	if( nSel < 0 )
		return;
	ASSERT( nSel < 3 );
DWORD dwArrStyles[3] = { 0, LVS_SORTASCENDING, LVS_SORTDESCENDING };
	m_wndList.ModifyStyle( LVS_SORTASCENDING|LVS_SORTDESCENDING, dwArrStyles[nSel], SWP_FRAMECHANGED );
	//m_wndList.SortItems()

// CExtSafeString s = m_wndList.GetCurrentFolderPath();
// 	m_wndList.BrowseToFolder( s );
}

void CPageShellListCtrl::OnCheckEditing() 
{
	if( m_checkEdit.GetCheck() )
		m_wndList.ModifyStyle( NULL, LVS_EDITLABELS );
	else
		m_wndList.ModifyStyle( LVS_EDITLABELS, NULL );
}

void CPageShellListCtrl::OnCheckViewFiles() 
{
	if( m_checkViewFiles.GetCheck() )
		m_wndList.IncludeFilesSet( true );
	else
		m_wndList.IncludeFilesSet( false );
}

void CPageShellListCtrl::OnCheckUpFolder() 
{
	if( m_checkUpFolder.GetCheck() )
		m_wndList.ShowParentFolderItemSet( true );
	else
		m_wndList.ShowParentFolderItemSet( false );
}

void CPageShellListCtrl::OnCheckFolderBeforeOther() 
{
	if( m_checkFolderBeforeOther.GetCheck() )
		m_wndList.FoldersBeforeOthersSet( true );
	else
		m_wndList.FoldersBeforeOthersSet( false );
}

void CPageShellListCtrl::OnCheckRootFolder()
{
	if( m_checkRootFolder.GetCheck() )
		m_wndList.ShowRootFolderItemSet( true );
	else
		m_wndList.ShowRootFolderItemSet( false );
}

void CPageShellListCtrl::OnCheckShowHidden()
{
	if( m_checkShowHiddenState.GetCheck() != 0 )
		m_wndList.m_dwAttributeFilterAny |= SFGAO_HIDDEN;
	else
		m_wndList.m_dwAttributeFilterAny &= ~(SFGAO_HIDDEN);
	m_wndList.RescanContent();
	
// 	if( m_checkShowHiddenState.GetCheck() )
// 		m_wndList.ShowHiddenStateSet( true );
// 	else
// 		m_wndList.ShowHiddenStateSet( false );
}

void CPageShellListCtrl::OnCheckWatchFileSystem()
{
	if( m_checkWatchFileSystem.GetCheck() )
		m_wndList.WatchFileSystemSet( true );
	else
		m_wndList.WatchFileSystemSet( false );
}

#endif // ( ! defined __EXT_MFC_NO_SHELL_LIST_VIEW_CTRL )
