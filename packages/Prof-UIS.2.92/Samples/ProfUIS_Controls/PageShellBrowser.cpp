// PageShellBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageShellBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL ) && ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )&& ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )

/////////////////////////////////////////////////////////////////////////////
// CPageShellBrowser dialog

CPageShellBrowser::CPageShellBrowser(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageShellBrowser::IDD, pParent)
	, m_bInitComplete( false )
	, m_bHaldingSelectionInTree( false )
	, m_bHaldingSelectionInList( false )
	, m_bHaldingSelectionInCombo( false )
{
	//{{AFX_DATA_INIT(CPageShellBrowser)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

void CPageShellBrowser::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageShellBrowser)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_COMBO1, m_wndCombo );
	DDX_Control( pDX, IDC_SHELL_LIST_CTRL, m_wndList );
	DDX_Control( pDX, IDC_TREE1, m_wndTree );
}


BEGIN_MESSAGE_MAP(CPageShellBrowser, CPageBase)
	//{{AFX_MSG_MAP(CPageShellBrowser)
	ON_REGISTERED_MESSAGE( CExtShellListCtrl::g_nMsgShellLocationChanged, OnShellListLocationChanged )
	ON_REGISTERED_MESSAGE( CExtShellComboBox::g_nMsgShellLocationChanged, OnShellComboLocationChanged )
	ON_REGISTERED_MESSAGE( CExtTreeCtrl::g_nMsgTreeItemDelayedFocus, OnShellTreeDelayedItemFocus )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageShellBrowser message handlers

BOOL CPageShellBrowser::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	
//	VERIFY( m_wndList.BrowseToFolder( _T("C:\\") ) );
//	VERIFY( m_wndList.BrowseToFolder() );

	//m_wndTree.OutlineStyleSet(CExtTreeCtrl::outline_style_solid);
	//m_wndTree.ModifyStyle( 0, TVS_HASBUTTONS|TVS_LINESATROOT );
	m_wndTree.RefreshShellRoot();
	m_wndTree.FocusItem( m_wndTree.GetChildItem( TVI_ROOT ), true, false, true );

m_wndTree.ModifyStyle( 0, /*TVS_SINGLEEXPAND|*/ TVS_TRACKSELECT|TVS_EDITLABELS );
m_wndList.SetExtendedStyle( LVS_EX_UNDERLINEHOT|LVS_EX_UNDERLINECOLD );
m_wndList.ModifyStyle( 0, LVS_EDITLABELS );

INT nAnchorPercentForTree = 25;
	AddAnchor( IDC_TREE1, __RDA_LT, CSize(nAnchorPercentForTree,100) );
	AddAnchor( IDC_SHELL_LIST_CTRL, CSize(nAnchorPercentForTree,0), __RDA_RB );
	AddAnchor( IDC_COMBO1, __RDA_LT, __RDA_RT );
	
	m_bInitComplete = true;
	
	return TRUE;
}

LRESULT CPageShellBrowser::OnShellTreeDelayedItemFocus( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
CExtShellTreeCtrl * pST = (CExtShellTreeCtrl*)wParam;
	ASSERT( pST != NULL );
	if( pST->m_hWnd != m_wndTree.m_hWnd )
		return 0L;
	if( ( lParam & CExtTreeCtrl::__PARM_DELAYED_FOCUS_BEGIN_WAITING ) != 0 )
	{
		if( ( lParam & CExtTreeCtrl::__PARM_DELAYED_FOCUS_MOUSE_CLICK ) == 0 )
			return 0L;
	}
	if( m_bHaldingSelectionInList || m_bHaldingSelectionInCombo )
		return 0L;
	m_bHaldingSelectionInTree = true;
HTREEITEM htiSelect = m_wndTree.GetFocusedItem();
	ASSERT( htiSelect != NULL );
CExtTreeCtrl::TREEITEMINFO_t & _TII = m_wndTree.TreeItemInfoGet( htiSelect );
CExtShellItemData * pData = (CExtShellItemData*)_TII.m_lParam; //new CExtShellItemData;
	ASSERT( pData != NULL );
LPITEMIDLIST pidlSelect = (LPITEMIDLIST)pData->m_pidlAbsolute.GetPtr();
	ASSERT( pidlSelect != NULL );
	m_wndList.DelayFocusPIDL( pidlSelect );
	m_wndCombo.DelayFocusPIDL( pidlSelect );
	m_bHaldingSelectionInTree = false;
	return 0L;
}

LRESULT CPageShellBrowser::OnShellListLocationChanged( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
	lParam;
CExtShellListCtrl * pSL = (CExtShellListCtrl*)wParam;
	ASSERT( pSL != NULL );
	if( pSL->m_hWnd != m_wndList.m_hWnd )
		return 0L;
	if( m_bHaldingSelectionInTree || m_bHaldingSelectionInCombo )
		return 0L;
	m_bHaldingSelectionInList = true;
	//m_wndTree.FocusPath( m_wndList.GetCurrentFolderPath() );
	m_wndTree.DelayFocusPIDL( m_wndList.GetCurrentFolderPIDL() );
	m_wndCombo.DelayFocusPIDL( m_wndList.GetCurrentFolderPIDL() );
	m_bHaldingSelectionInList = false;
	return 0L;
}

LRESULT CPageShellBrowser::OnShellComboLocationChanged( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
	lParam;
CExtShellComboBox * pSL = (CExtShellComboBox*)wParam;
	ASSERT( pSL != NULL );
	if( pSL->m_hWnd != m_wndCombo.m_hWnd )
		return 0L;
	if( m_bHaldingSelectionInTree || m_bHaldingSelectionInList )
		return 0L;
	m_bHaldingSelectionInCombo = true;
	//m_wndTree.FocusPath( m_wndList.GetCurrentFolderPath() );
	m_wndTree.DelayFocusPIDL( m_wndCombo.GetCurrentFolderPIDL() );
	m_wndList.DelayFocusPIDL( m_wndCombo.GetCurrentFolderPIDL() );
	m_bHaldingSelectionInCombo = false;
	return 0L;
}

#endif // ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL ) && ( ! defined __EXT_MFC_NO_SHELL_TREE_VIEW_CTRL )&& ( ! defined __EXT_MFC_NO_SHELL_COMBO_BOX )
