// PageShellTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageShellTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CPageShellTreeCtrl dialog

CPageShellTreeCtrl::CPageShellTreeCtrl(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageShellTreeCtrl::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageShellTreeCtrl)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

void CPageShellTreeCtrl::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageShellTreeCtrl)
	DDX_Control( pDX, IDC_BUTTON_GO, m_buttonGo );
	DDX_Control( pDX, IDC_MAKE_NEW_FOLDER_BUTTON, m_buttonMakeNewFolder );
	DDX_Control( pDX, IDC_EDIT_PATH, m_editPath );
	DDX_Control( pDX, IDC_INCLUDE_FILE_CHECK, m_checkIncludeFiles );
	DDX_Control( pDX, IDC_EDITING_LABELS_CHECK, m_checkEditLabels );
	DDX_Control( pDX, IDC_SHOW_HIDDEN_FILES_CHECK, m_checkShowHiddenItems );
	DDX_Control( pDX, IDC_WATCH_FILE_SYSTEM_CHECK, m_checkWatchFileSystem );
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_TREE1, m_wndTree );
}


BEGIN_MESSAGE_MAP(CPageShellTreeCtrl, CPageBase)
	//{{AFX_MSG_MAP(CPageShellTreeCtrl)
	ON_BN_CLICKED( IDC_BUTTON_GO, OnBnClickedButtonGo )
	ON_BN_CLICKED( IDC_MAKE_NEW_FOLDER_BUTTON, OnBnClickedMakeNewFolderButton )
	ON_BN_CLICKED( IDC_INCLUDE_FILE_CHECK, OnBnClickedShowFilesCheck )
	ON_BN_CLICKED( IDC_SHOW_HIDDEN_FILES_CHECK, OnBnClickedShowHiddenFilesCheck )
	ON_BN_CLICKED( IDC_EDITING_LABELS_CHECK, OnBnClickedEditingLabelsCheck )
	ON_BN_CLICKED( IDC_WATCH_FILE_SYSTEM_CHECK, OnBnClickedWatchFileSystemCheck )
	ON_NOTIFY( NM_RCLICK, IDC_TREE1, OnNMRclickTree1 )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageShellTreeCtrl message handlers

BOOL CPageShellTreeCtrl::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	
	m_wndTree.LineStyleSet(CExtTreeCtrl::line_style_solid);
	m_wndTree.ModifyStyle( 0, /*TVS_FULLROWSELECT|*/ TVS_HASBUTTONS|TVS_LINESATROOT );
	m_wndTree.ModifyStyle( 0, TVS_HASBUTTONS|TVS_LINESATROOT );
	m_wndTree.TreeModifyStyle( __EXT_TREE_MULTIPLE_SELECTION );

	m_wndTree.RefreshShellRoot();

	m_checkEditLabels.SetCheck(1);
	m_checkWatchFileSystem.SetCheck(1);
	OnBnClickedEditingLabelsCheck();

	AddAnchor( IDC_TREE1, __RDA_LT, __RDA_RB );
	AddAnchor( IDC_EDIT_PATH, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_BUTTON_GO, __RDA_RT );
	AddAnchor( IDC_MAKE_NEW_FOLDER_BUTTON, __RDA_RT );
	AddAnchor( IDC_INCLUDE_FILE_CHECK, __RDA_RT );
	AddAnchor( IDC_SHOW_HIDDEN_FILES_CHECK, __RDA_RT );
	AddAnchor( IDC_EDITING_LABELS_CHECK, __RDA_RT );
	AddAnchor( IDC_WATCH_FILE_SYSTEM_CHECK, __RDA_RT );
	
	m_bInitComplete = true;
	
	return TRUE;
}

void CPageShellTreeCtrl::OnBnClickedButtonGo()
{
CString strPath;
	m_editPath.GetWindowText( strPath );
	m_wndTree.FocusPath( LPCTSTR(strPath) );
	m_wndTree.SetFocus();
}

void CPageShellTreeCtrl::OnBnClickedMakeNewFolderButton()
{
CString strNewFolderName;
	m_editPath.GetWindowText( strNewFolderName );
	m_wndTree._CreateFolder( LPCTSTR(strNewFolderName) );
}

void CPageShellTreeCtrl::OnBnClickedShowFilesCheck()
{
	if( m_checkIncludeFiles.GetCheck() == BST_CHECKED )
		m_wndTree.IncludeFilesSet( true );

	if( m_checkIncludeFiles.GetCheck() == BST_UNCHECKED )
		m_wndTree.IncludeFilesSet( false );
}

void CPageShellTreeCtrl::OnBnClickedShowHiddenFilesCheck()
{
	if( m_checkShowHiddenItems.GetCheck() != 0 )
		m_wndTree.m_dwAttributeFilterAny |= SFGAO_HIDDEN;
	else
		m_wndTree.m_dwAttributeFilterAny &= ~(SFGAO_HIDDEN);
	m_wndTree.RescanContent();
}

void CPageShellTreeCtrl::OnBnClickedEditingLabelsCheck()
{
	if( m_checkEditLabels.GetCheck() == BST_CHECKED )
		if( ( m_wndTree.GetStyle() & TVS_EDITLABELS ) == 0 )
			m_wndTree.ModifyStyle( NULL, TVS_EDITLABELS );


	if( m_checkEditLabels.GetCheck() == BST_UNCHECKED )
		if( ( m_wndTree.GetStyle() & TVS_EDITLABELS ) != 0 )
			m_wndTree.ModifyStyle( TVS_EDITLABELS, NULL );

	m_wndTree.Invalidate();
}

void CPageShellTreeCtrl::OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	pNMHDR;
	(*pResult) = 0;
}

void CPageShellTreeCtrl::OnBnClickedWatchFileSystemCheck()
{
	if( m_checkWatchFileSystem.GetCheck() == BST_CHECKED )
		m_wndTree.WatchFileSystemSet( true );
	if( m_checkWatchFileSystem.GetCheck() == BST_UNCHECKED )
		m_wndTree.WatchFileSystemSet( false );
}

#endif // ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )
