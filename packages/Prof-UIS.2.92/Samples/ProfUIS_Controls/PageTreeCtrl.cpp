// PageTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CPageTreeCtrl dialog

CPageTreeCtrl::CPageTreeCtrl(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageTreeCtrl::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageTreeCtrl)
	//}}AFX_DATA_INIT
}

void CPageTreeCtrl::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageTreeCtrl)
	DDX_Control(pDX, IDC_CHECK_EXPANDED_ONLY, m_wndCheckBoxExpandedOnly);
	DDX_Control(pDX, IDC_CHECK_FULL_ROW_SELECT, m_wndFullRowSelectCheckBox);
	DDX_Control(pDX, IDC_CHECK_HAS_BUTTONS, m_wndHasButtonsCheckBox);
	DDX_Control(pDX, IDC_CHECK_HAS_LINES, m_wndHasLinesCheckBox);
	DDX_Control(pDX, IDC_CHECK_LINES_AT_ROOT, m_wndLineSatRoot);
	DDX_Control(pDX, IDC_CHECK_SHOW_SEL_ALWAYS, m_wndShowSelAlwaysCheckBox);
	DDX_Control(pDX, IDC_CHECK_SINGLE_EXPAND, m_wndSingleExpandCheckBox);
	DDX_Control(pDX, IDC_CHECK_TRACK_SELECT, m_wndTrackSelectCheckBox);
	DDX_Control(pDX, IDC_CHECK_HOVER_DISABLE_ITEMS, m_wndHoverDisableItemsCheckBox);
	DDX_Control(pDX, IDC_CHECK_HOVER_TREE_BOX_EX, m_wndHoverTreeBoxExCheckBox);
	DDX_Control(pDX, IDC_CHECK_HOVER_CHECK_BOX_EX, m_wndHoverCheckBoxExCheckBox);
	DDX_Control(pDX, IDC_CHECK_HOVER_RADIO_BOX_EX, m_wndHoverRadioBoxExCheckBox);
	DDX_Control(pDX, IDC_CHECK_MULTIPLE_SELECTION, m_wndMultipleSelectionCheckBox);
	DDX_Control(pDX, IDC_CHECK_FOCUS_DISABLE_ITEMS, m_wndFocusDisableItemsCheckBox);
	DDX_Control(pDX, IDC_CHECK_SELECT_DISABLE_ITEMS, m_wndSelectDisableItemsCheckBox);
	DDX_Control(pDX, IDC_CHECK_SELECT_COLLAPSED_ITEMS, m_wndSelectCollapsedItemsCheckBox);
	DDX_Control(pDX, IDC_CHECK_SUBTRACT_SELECTION, m_wndSubtractSelectionCheckBox);
	DDX_Control(pDX, IDC_CHECK_SOLID_OUTLINE, m_wndOutlineSolidCheckBox);
	DDX_Control(pDX, IDC_CHECK_TREE_BOX_BITMAP, m_wndTreeBoxBitmapCheckBox);
	DDX_Control(pDX, IDC_CHECK_TREE_BOX_SQUARE, m_wndTreeBoxSquareCheckBox);
	DDX_Control(pDX, IDC_CHECK_TREE_BOX_ROUND, m_wndTreeBoxRoundCheckBox);
	DDX_Control(pDX, IDC_CHECK_INFO_TIP, m_wndInfoTipCheckBox);
	DDX_Control(pDX, IDC_CHECK_EDIT_LABELS, m_wndEditLabelsCheckBox);
	DDX_Control(pDX, IDC_CHECK_READ_ONLY_LABELS, m_wndReadOnlyLabelsCheckBox);
	DDX_Control(pDX, IDC_CHECK_AUTO_DISABLE_CHILDREN_ITEMS, m_wndAutoDisableChildrenItems);
	DDX_Control(pDX, IDC_MYBUTTON2, m_buttonUp );
	DDX_Control(pDX, IDC_MYBUTTON1, m_buttonDown );
	DDX_Control(pDX, IDC_BUTTON_TREE_BOX_XP, m_buttonLoadTreeBoxXP );
	DDX_Control(pDX, IDC_BUTTON_TREE_BOX_VISTA_SET, m_buttonLoadTreeBoxVista );
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_TREE1, m_wndTree );
}


BEGIN_MESSAGE_MAP(CPageTreeCtrl, CPageBase)
	//{{AFX_MSG_MAP(CPageTreeCtrl)
	ON_NOTIFY( TVN_SELCHANGED, IDC_TREE1, OnSelChangedTree1 )
	ON_BN_CLICKED(IDC_MYBUTTON1, OnBnClickedMybutton1)
	ON_BN_CLICKED(IDC_MYBUTTON2, OnBnClickedMybutton2)
	ON_BN_CLICKED(IDC_CHECK_FULL_ROW_SELECT, OnBnClickedCheckFullRowSelect)
	ON_BN_CLICKED(IDC_CHECK_HAS_BUTTONS, OnBnClickedCheckHasButtons)
	ON_BN_CLICKED(IDC_CHECK_HAS_LINES, OnBnClickedCheckHasLines)
	ON_BN_CLICKED(IDC_CHECK_LINES_AT_ROOT, OnBnClickedCheckLineSatRoot)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SEL_ALWAYS, OnBnClickedCheckShowSelAlways)
	ON_BN_CLICKED(IDC_CHECK_SINGLE_EXPAND, OnBnClickedCheckSingleExpand)
	ON_BN_CLICKED(IDC_CHECK_TRACK_SELECT, OnBnClickedCheckTrackSelect)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_MULTIPLE_SELECTION, OnBnClickedCheckMultipleSelection)
	ON_BN_CLICKED(IDC_CHECK_FOCUS_DISABLE_ITEMS, OnBnClickedCheckFocusDisableItems)
	ON_BN_CLICKED(IDC_CHECK_SELECT_DISABLE_ITEMS, OnBnClickedCheckSelectDisableItems)
	ON_BN_CLICKED(IDC_CHECK_SELECT_COLLAPSED_ITEMS, OnBnClickedCheckSelectCollapsedItems)
	ON_BN_CLICKED(IDC_CHECK_SUBTRACT_SELECTION, OnBnClickedCheckSubtractSelection)
	ON_BN_CLICKED(IDC_CHECK_HOVER_DISABLE_ITEMS, OnBnClickedCheckHoverDisableItems)
	ON_BN_CLICKED(IDC_CHECK_HOVER_TREE_BOX_EX, OnBnClickedCheckHoverTreeBoxEx)
	ON_BN_CLICKED(IDC_CHECK_HOVER_CHECK_BOX_EX, OnBnClickedCheckHoverCheckBoxEx)
	ON_BN_CLICKED(IDC_CHECK_HOVER_RADIO_BOX_EX, OnBnClickedCheckHoverRadioBoxEx)
	ON_BN_CLICKED(IDC_BUTTON_TREE_BOX_XP, OnBnClickedButtonTreeBoxXp)
	ON_BN_CLICKED(IDC_BUTTON_TREE_BOX_VISTA_SET, OnBnClickedButtonTreeBoxVistaSet)
	ON_BN_CLICKED(IDC_CHECK_SOLID_OUTLINE, OnBnClickedCheckSolidOutline)
	ON_BN_CLICKED(IDC_CHECK_TREE_BOX_BITMAP, OnBnClickedCheckTreeBoxBitmap)
	ON_BN_CLICKED(IDC_CHECK_TREE_BOX_SQUARE, OnBnClickedCheckTreeBoxSquare)
	ON_BN_CLICKED(IDC_CHECK_TREE_BOX_ROUND, OnBnClickedCheckTreeBoxRound)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_AUTO_DISABLE_CHILDREN_ITEMS, OnBnClickedCheckAutoDisableChildrenItems)
	ON_BN_CLICKED(IDC_CHECK_INFO_TIP, OnBnClickedCheckInfoTip)
	ON_BN_CLICKED(IDC_CHECK_EDIT_LABELS, OnBnClickedCheckEditLabels)
	ON_BN_CLICKED(IDC_CHECK_READ_ONLY_LABELS, OnBnClickedCheckReadOnlyLabels)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, OnBegindragTree1)
	ON_NOTIFY(TVN_BEGINRDRAG, IDC_TREE1, OnBeginRdragTree1)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TREE1, OnGetdispinfoTree1)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, OnItemexpandingTree1)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE1, OnItemexpandedTree1)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE1, OnBeginlabeleditTree1)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, OnEndlabeleditTree1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageTreeCtrl message handlers

BOOL CPageTreeCtrl::OnInitDialog()
{
	CPageBase::OnInitDialog();
	
	//m_wndTree.OutlineColorSet( RGB(0,128,0) );
	//m_wndTree.ExpandCollapseButtonFillColorSet( RGB(200,165,134) );
	//m_wndTree.ExpandCollapseButtonSignColorSet( RGB(145,0,0) );
	m_wndTree.TreeBoxStyleSet(CExtTreeCtrl::tree_box_style_bitmap);
	//m_wndTree.TreeBoxStyleSet(CExtTreeCtrl::tree_box_style_round);
	//m_wndTree.OutlineStyleSet(CExtTreeCtrl::outline_style_solid);
	//m_wndTree.TreeBkColorSet( RGB(255, 123, 12) );

// 	// 	// 	HTREEITEM hTrRoot;
// 	// 	// 	HTREEITEM hTrAfterRoot;
// 	// 	// 	HTREEITEM hTrLeaf;
// 	// 	// 	CExtCmdIcon _icon;
// 	// 	// 		_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(IDB_BITMAP_VISTA3) );
// 	// 	// 		_icon.m_bmpNormal.Scale( 16, 16 );
// 	// 	// 		m_wndTree.TreeIconAdd( _icon );
//	//m_wndTree.OuterMarginCheckBoxLeftSet( 4 );
//	m_wndTree.OuterMarginCheckBoxRightSet( 4 );
//	//m_wndTree.OuterMarginIconLeftSet( 4 );
//	m_wndTree.OuterMarginIconRightSet( 4 );
//	//m_wndTree.OuterMarginLabelLeftSet( 4 );
//	m_wndTree.OuterMarginLabelRightSet( 4 );
//	//m_wndTree.OuterMarginControlLabelLeftSet( 4 );
//	m_wndTree.OuterMarginControlLabelRightSet( 4 );
//	m_wndTree.PaddingLabelLeftSet( 5 );
//	m_wndTree.PaddingLabelRightSet( 5 );

	m_wndTree.LoadWinVistaTreeBox();
//	m_wndTree.LoadWinXPTreeBox();

HTREEITEM htiControls, hti;

HTREEITEM htiTextCallBack = m_wndTree.InsertItem( LPSTR_TEXTCALLBACK, NULL, NULL );
	ASSERT( htiTextCallBack != NULL );
	htiTextCallBack;

	htiControls = m_wndTree.InsertItem( _T("Tree items demonstrating supported features"), NULL, NULL );

				hti = m_wndTree.InsertItem( _T("Edit control:   "), htiControls, NULL );
				m_wndTree.TreeItemInfoGet( hti ).m_eICT =  CExtTreeCtrl::__EICT_EDIT;
				m_wndTree.TreeItemInfoGet( hti ).m_strEditText = _T("control text");
				m_wndTree.TreeItemInfoGet( hti ).m_nControlWidth = 200;

				hti = m_wndTree.InsertItem( _T("Dropdown combobox control:   "), htiControls, NULL );
				m_wndTree.TreeItemInfoGet( hti ).m_eICT =  CExtTreeCtrl::__EICT_DROPDOWN_COMBOBOX;
				m_wndTree.TreeItemInfoGet( hti ).m_strEditText = _T("Line 1");
				m_wndTree.TreeItemInfoGet( hti ).m_nControlWidth = 200;
				m_wndTree.TreeItemInfoGet( hti ).ComboBoxItem_Add( new CExtTreeCtrl::COMBOBOXITEMDATA_t( _T("Line 1") ), false );
				m_wndTree.TreeItemInfoGet( hti ).ComboBoxItem_Add( new CExtTreeCtrl::COMBOBOXITEMDATA_t( _T("Line 2") ), false );
				m_wndTree.TreeItemInfoGet( hti ).ComboBoxItem_Add( new CExtTreeCtrl::COMBOBOXITEMDATA_t( _T("Line 3") ), false );

				hti = m_wndTree.InsertItem( _T("Droplist combobox control:   "), htiControls, NULL );
				m_wndTree.TreeItemInfoGet( hti ).m_eICT =  CExtTreeCtrl::__EICT_DROPLIST_COMBOBOX;
				m_wndTree.TreeItemInfoGet( hti ).m_strEditText = _T("Line 1");
				m_wndTree.TreeItemInfoGet( hti ).m_nControlWidth = 200;
				m_wndTree.TreeItemInfoGet( hti ).ComboBoxItem_Add( new CExtTreeCtrl::COMBOBOXITEMDATA_t( _T("Line 1") ), false );
				m_wndTree.TreeItemInfoGet( hti ).ComboBoxItem_Add( new CExtTreeCtrl::COMBOBOXITEMDATA_t( _T("Line 2") ), false );
				m_wndTree.TreeItemInfoGet( hti ).ComboBoxItem_Add( new CExtTreeCtrl::COMBOBOXITEMDATA_t( _T("Line 3") ), false );

				HTREEITEM htiProgressBars = m_wndTree.InsertItem( _T("Progress controls"), htiControls, NULL );

							hti = m_wndTree.InsertItem( _T("Progress bar with boxes:   "), htiProgressBars, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eICT =  CExtTreeCtrl::__EICT_PROGRESS_BAR_WITH_BOXES;
							m_wndTree.TreeItemInfoGet( hti ).m_nControlWidth = 200;
							m_listTreeItemsWithProgressControl.AddTail( hti );

							hti = m_wndTree.InsertItem( _T("Smooth progress bar:   "), htiProgressBars, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eICT =  CExtTreeCtrl::__EICT_PROGRESS_BAR_SMOOTH;
							m_wndTree.TreeItemInfoGet( hti ).m_nControlWidth = 200;
							m_listTreeItemsWithProgressControl.AddTail( hti );

							hti = m_wndTree.InsertItem( _T("Themed progress bar:   "), htiProgressBars, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eICT =  CExtTreeCtrl::__EICT_PROGRESS_BAR_THEMED;
							m_wndTree.TreeItemInfoGet( hti ).m_nControlWidth = 200;
							m_listTreeItemsWithProgressControl.AddTail( hti );

							hti = m_wndTree.InsertItem( _T("Endless progress bar:   "), htiProgressBars, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eICT =  CExtTreeCtrl::__EICT_PROGRESS_BAR_ENDLESS;
							m_wndTree.TreeItemInfoGet( hti ).m_nControlWidth = 200;
							m_listTreeItemsWithProgressControl.AddTail( hti );

				m_wndTree.Expand( htiProgressBars, TVE_EXPAND );

				HTREEITEM htiCheckBoxes = m_wndTree.InsertItem( _T("Check boxes"), htiControls, NULL );

							hti = m_wndTree.InsertItem( _T("Two-state check box"), htiCheckBoxes, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eCheckBoxType = CExtTreeCtrl::e_cbt_check_box;

							hti = m_wndTree.InsertItem( _T("Three-state check box"), htiCheckBoxes, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eCheckBoxType = CExtTreeCtrl::e_cbt_tri_state_check_box;

				m_wndTree.Expand( htiCheckBoxes, TVE_EXPAND );

				HTREEITEM htiRadio = m_wndTree.InsertItem( _T("Radio groups"), htiControls, NULL );
//m_wndTree.SetInsertMark( htiRadio, false );

							hti = m_wndTree.InsertItem( _T("GroupA-radio1"), htiRadio, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eCheckBoxType = CExtTreeCtrl::e_cbt_radio;
							m_wndTree.TreeItemInfoGet( hti ).m_strRadioGroupName = _T("A");
							m_wndTree.TreeItemInfoGet( hti ).m_nCheckBoxState = 1;

							hti = m_wndTree.InsertItem( _T("GroupA-radio2"), htiRadio, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eCheckBoxType = CExtTreeCtrl::e_cbt_radio;
							m_wndTree.TreeItemInfoGet( hti ).m_strRadioGroupName = _T("A");

							hti = m_wndTree.InsertItem( _T("GroupA-radio3 (disabled)"), htiRadio, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eCheckBoxType = CExtTreeCtrl::e_cbt_radio;
							m_wndTree.TreeItemInfoGet( hti ).m_strRadioGroupName = _T("A");
							m_wndTree.TreeItemInfoGet( hti ).m_bDisabled = true;

							hti = m_wndTree.InsertItem( _T("GroupA-radio4"), htiRadio, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eCheckBoxType = CExtTreeCtrl::e_cbt_radio;
							m_wndTree.TreeItemInfoGet( hti ).m_strRadioGroupName = _T("A");

							hti = m_wndTree.InsertItem( _T("GroupB-radio1"), htiRadio, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eCheckBoxType = CExtTreeCtrl::e_cbt_radio;
							m_wndTree.TreeItemInfoGet( hti ).m_strRadioGroupName = _T("B");
							m_wndTree.TreeItemInfoGet( hti ).m_nCheckBoxState = 1;

							hti = m_wndTree.InsertItem( _T("GroupB-radio2"), htiRadio, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eCheckBoxType = CExtTreeCtrl::e_cbt_radio;
							m_wndTree.TreeItemInfoGet( hti ).m_strRadioGroupName = _T("B");

							hti = m_wndTree.InsertItem( _T("GroupB-radio3 (disabled)"), htiRadio, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eCheckBoxType = CExtTreeCtrl::e_cbt_radio;
							m_wndTree.TreeItemInfoGet( hti ).m_strRadioGroupName = _T("B");
							m_wndTree.TreeItemInfoGet( hti ).m_bDisabled = true;

							hti = m_wndTree.InsertItem( _T("GroupB-radio4"), htiRadio, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_eCheckBoxType = CExtTreeCtrl::e_cbt_radio;
							m_wndTree.TreeItemInfoGet( hti ).m_strRadioGroupName = _T("B");

				m_wndTree.Expand( htiRadio, TVE_EXPAND );

				HTREEITEM htiFontEffects = m_wndTree.InsertItem( _T("Font effects"), htiControls, NULL );

							hti = m_wndTree.InsertItem( _T("Simple text"), htiFontEffects, NULL );

							hti = m_wndTree.InsertItem( _T("Bold text"), htiFontEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bBold = true;

							hti = m_wndTree.InsertItem( _T("Italic text"), htiFontEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bItalic = true;

							hti = m_wndTree.InsertItem( _T("Underlined text"), htiFontEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bUnderline = true;

							hti = m_wndTree.InsertItem( _T("Bold/italic text"), htiFontEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bBold = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_bItalic = true;

							hti = m_wndTree.InsertItem( _T("Bold/underlined text"), htiFontEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bBold = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_bUnderline = true;

							hti = m_wndTree.InsertItem( _T("Italic/underlined text"), htiFontEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bUnderline = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_bItalic = true;

							hti = m_wndTree.InsertItem( _T("Bold/italic/underlined text"), htiFontEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bBold = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_bItalic = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_bUnderline = true;

				m_wndTree.Expand( htiFontEffects, TVE_EXPAND );

				HTREEITEM htiColorEffects = m_wndTree.InsertItem( _T("Color effects"), htiControls, NULL );

							hti = m_wndTree.InsertItem( _T("Simple text"), htiColorEffects, NULL );

							hti = m_wndTree.InsertItem( _T("Bold text"), htiColorEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bBold = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal = RGB(255,0,0);
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextHover =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, +0.25 );
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextSelected =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, -0.25 );

							hti = m_wndTree.InsertItem( _T("Italic text"), htiColorEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bItalic = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal = RGB(0,255,0);
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextHover =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, +0.25 );
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextSelected =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, -0.25 );

							hti = m_wndTree.InsertItem( _T("Underlined text"), htiColorEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bUnderline = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal = RGB(0,0,255);
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextHover =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, +0.25 );
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextSelected =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, -0.25 );

							hti = m_wndTree.InsertItem( _T("Bold/italic text"), htiColorEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bBold = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_bItalic = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal = RGB(0,0,255);
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextHover =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, +0.25 );
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextSelected =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, -0.25 );

							hti = m_wndTree.InsertItem( _T("Bold/underlined text"), htiColorEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bBold = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_bUnderline = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal = RGB(255,255,0);
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextHover =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, +0.25 );
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextSelected =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, -0.25 );

							hti = m_wndTree.InsertItem( _T("Italic/underlined text"), htiColorEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bUnderline = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_bItalic = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal = RGB(0,255,255);
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextHover =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, +0.25 );
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextSelected =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, -0.25 );

							hti = m_wndTree.InsertItem( _T("Bold/italic/underlined text"), htiColorEffects, NULL );
		 					m_wndTree.TreeItemInfoGet( hti ).m_bBold = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_bItalic = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_bUnderline = true;
		 					m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal = RGB(192,192,192);
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextHover =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, +0.25 );
							m_wndTree.TreeItemInfoGet( hti ).m_clrTextSelected =
								CExtBitmap::stat_HLS_Adjust( m_wndTree.TreeItemInfoGet( hti ).m_clrTextNormal, 0.0, -0.25 );

				m_wndTree.Expand( htiColorEffects, TVE_EXPAND );

				CExtCmdIcon _icon;
				CExtBitmap::Filter _f( CExtBitmap::Filter::lanczos );
				_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(IDB_BITMAP_VISTA3) );
				_icon.m_bmpNormal.Scale( 16, 16, _f );
				m_wndTree.TreeIconAdd( _icon );
				_icon.m_bmpNormal.MakeMono( RGB(192,192,192) );
				m_wndTree.TreeIconAdd( _icon );
				_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(IDB_BITMAP_VISTA1) );
				_icon.m_bmpNormal.Scale( 16, 16, _f );
				m_wndTree.TreeIconAdd( _icon );
				_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(IDB_BITMAP_VISTA2) );
				_icon.m_bmpNormal.Scale( 16, 16, _f );
				m_wndTree.TreeIconAdd( _icon );
				_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(IDB_BITMAP_VISTA4) );
				_icon.m_bmpNormal.Scale( 16, 16, _f );
				m_wndTree.TreeIconAdd( _icon );
				_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(IDB_BITMAP_VISTA5) );
				_icon.m_bmpNormal.Scale( 16, 16, _f );
				m_wndTree.TreeIconAdd( _icon );
				_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(IDB_BITMAP_VISTA6) );
				_icon.m_bmpNormal.Scale( 16, 16, _f );
				m_wndTree.TreeIconAdd( _icon );
				_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(IDB_BITMAP_VISTA7) );
				_icon.m_bmpNormal.Scale( 16, 16, _f );
				m_wndTree.TreeIconAdd( _icon );
				_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(IDB_BITMAP_VISTA8) );
				_icon.m_bmpNormal.Scale( 16, 16, _f );
				m_wndTree.TreeIconAdd( _icon );
				HTREEITEM htiItemsWithIcons = m_wndTree.InsertItem( _T("Items with icons"), htiControls, NULL );
				m_wndTree.TreeItemInfoGet( htiItemsWithIcons ).m_nIconIndexCollapsed = 1;
				m_wndTree.TreeItemInfoGet( htiItemsWithIcons ).m_nIconIndexExpanded = 0;

							hti = m_wndTree.InsertItem( _T("Item 0"), htiItemsWithIcons, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexCollapsed = 2;
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexExpanded = 2;

							hti = m_wndTree.InsertItem( _T("Item 1"), htiItemsWithIcons, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexCollapsed = 3;
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexExpanded = 3;
							
							hti = m_wndTree.InsertItem( _T("Item 2"), htiItemsWithIcons, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexCollapsed = 4;
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexExpanded = 4;
							
							hti = m_wndTree.InsertItem( _T("Item 3"), htiItemsWithIcons, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexCollapsed = 5;
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexExpanded = 5;
							
							hti = m_wndTree.InsertItem( _T("Item 4"), htiItemsWithIcons, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexCollapsed = 6;
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexExpanded = 6;
							
							hti = m_wndTree.InsertItem( _T("Item 5"), htiItemsWithIcons, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexCollapsed = 7;
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexExpanded = 7;
							
							hti = m_wndTree.InsertItem( _T("Item 6 (disabled)"), htiItemsWithIcons, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexCollapsed = 8;
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexExpanded = 8;
							m_wndTree.TreeItemInfoGet( hti ).m_bDisabled = true;
							
							hti = m_wndTree.InsertItem( _T("Item 7"), htiItemsWithIcons, NULL );
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexCollapsed = 8;
							m_wndTree.TreeItemInfoGet( hti ).m_nIconIndexExpanded = 8;

				m_wndTree.Expand( htiItemsWithIcons, TVE_EXPAND );

		m_wndTree.Expand( htiControls, TVE_EXPAND );

	m_wndTree.TreeModifyStyle(
			__EXT_TREE_MULTIPLE_SELECTION
				//|__EXT_TREE_FOCUS_DISABLE_ITEMS
				//|__EXT_TREE_SELECT_DISABLE_ITEMS
				|__EXT_TREE_SELECT_COLLAPSED_ITEMS
				|__EXT_TREE_SUBTRACT_SELECTION
				|__EXT_TREE_HOVER_DISABLE_ITEMS
				|__EXT_TREE_AUTO_DISABLE_ITEMS_BY_PARENT
			);
	m_wndTree.TreeModifyStyleEx( 
		__EXT_TREE_EX_HOVER_TREE_BOXES 
			| __EXT_TREE_EX_HOVER_CHECK_BOXES
			| __EXT_TREE_EX_HOVER_RADIO_BUTTONS	
		);

	m_wndTree.ModifyStyle(
		0,
		TVS_INFOTIP //|TVS_EDITLABELS
		);

	m_wndHasButtonsCheckBox.SetCheck( 1 );
	m_wndHasLinesCheckBox.SetCheck( 0 );
	m_wndLineSatRoot.SetCheck( 1 );
	m_wndShowSelAlwaysCheckBox.SetCheck( 1 );
	m_wndHoverDisableItemsCheckBox.SetCheck( 1 );;
	m_wndHoverTreeBoxExCheckBox.SetCheck( 1 );
	m_wndHoverCheckBoxExCheckBox.SetCheck( 1 );
	m_wndHoverRadioBoxExCheckBox.SetCheck( 1 );
	m_wndMultipleSelectionCheckBox.SetCheck( 1 );
	m_wndFocusDisableItemsCheckBox.SetCheck( 0 );
	m_wndSelectDisableItemsCheckBox.SetCheck( 0 );
	m_wndSelectCollapsedItemsCheckBox.SetCheck( 1 );
	m_wndSubtractSelectionCheckBox.SetCheck( 1 );
	m_wndTreeBoxBitmapCheckBox.SetCheck( 1 );
	m_wndAutoDisableChildrenItems.SetCheck( 1 );
	m_wndEditLabelsCheckBox.SetCheck( 0 );
	m_wndInfoTipCheckBox.SetCheck( 1 );
	OnBnClickedCheckHasButtons();
	OnBnClickedCheckHasLines();
	OnBnClickedCheckLineSatRoot();
	OnBnClickedCheckShowSelAlways();
	OnBnClickedCheckSingleExpand();
	OnBnClickedCheckTrackSelect();

	AddAnchor( IDC_TREE1, __RDA_LT, __RDA_RB );
	AddAnchor( IDC_MYBUTTON1, __RDA_RT );
	AddAnchor( IDC_MYBUTTON2, __RDA_RT );
	AddAnchor( IDC_CHECK_EXPANDED_ONLY, __RDA_RT );
	AddAnchor( IDC_CHECK_FULL_ROW_SELECT, __RDA_RT );
	AddAnchor( IDC_CHECK_HAS_BUTTONS, __RDA_RT );
	AddAnchor( IDC_CHECK_HAS_LINES, __RDA_RT );
	AddAnchor( IDC_CHECK_LINES_AT_ROOT, __RDA_RT );
	AddAnchor( IDC_CHECK_SHOW_SEL_ALWAYS, __RDA_RT );
	AddAnchor( IDC_CHECK_SINGLE_EXPAND, __RDA_RT );
	AddAnchor( IDC_CHECK_TRACK_SELECT, __RDA_RT );
	AddAnchor( IDC_CHECK_MULTIPLE_SELECTION, __RDA_RT );
	AddAnchor( IDC_CHECK_FOCUS_DISABLE_ITEMS, __RDA_RT );
	AddAnchor( IDC_CHECK_SELECT_DISABLE_ITEMS, __RDA_RT );
	AddAnchor( IDC_CHECK_SELECT_COLLAPSED_ITEMS, __RDA_RT );
	AddAnchor( IDC_CHECK_SUBTRACT_SELECTION, __RDA_RT );
	AddAnchor( IDC_CHECK_HOVER_DISABLE_ITEMS, __RDA_RT );
	AddAnchor( IDC_CHECK_HOVER_TREE_BOX_EX, __RDA_RT );
	AddAnchor( IDC_CHECK_HOVER_CHECK_BOX_EX, __RDA_RT );
	AddAnchor( IDC_CHECK_HOVER_RADIO_BOX_EX, __RDA_RT );
	AddAnchor( IDC_BUTTON_TREE_BOX_XP, __RDA_RT );
	AddAnchor( IDC_BUTTON_TREE_BOX_VISTA_SET, __RDA_RT );
	AddAnchor( IDC_CHECK_SOLID_OUTLINE, __RDA_RT );
	AddAnchor( IDC_CHECK_TREE_BOX_BITMAP, __RDA_RT );
	AddAnchor( IDC_CHECK_TREE_BOX_SQUARE, __RDA_RT );
	AddAnchor( IDC_CHECK_TREE_BOX_ROUND, __RDA_RT );
	AddAnchor( IDC_CHECK_AUTO_DISABLE_CHILDREN_ITEMS, __RDA_RT );
	AddAnchor( IDC_CHECK_EDIT_LABELS, __RDA_RT );
	AddAnchor( IDC_CHECK_READ_ONLY_LABELS, __RDA_RT );
	AddAnchor( IDC_CHECK_INFO_TIP, __RDA_RT );

	RepositionBars( 0, 0xFFFF, 0 );

	SetTimer( 7, 250, NULL );

	m_bInitComplete = true;
	
	return TRUE;
}

void CPageTreeCtrl::OnSelChangedTree1( NMHDR * pNMHDR, LRESULT * p_lResult )
{
	pNMHDR;
	p_lResult;
}

void CPageTreeCtrl::OnBnClickedMybutton1()
{
bool bExpandedOnly = ( m_wndCheckBoxExpandedOnly.GetCheck() != 0 ) ? true : false;
HTREEITEM htiSelect = m_wndTree.GetSelectedItem();
	if( htiSelect == NULL )
		return;
HTREEITEM htiNext = m_wndTree.LinearItemGetNext( htiSelect, bExpandedOnly );
	if( htiNext == NULL )
		return;
	m_wndTree.FocusItem( htiNext, true, true );
}

void CPageTreeCtrl::OnBnClickedMybutton2()
{
bool bExpandedOnly = ( m_wndCheckBoxExpandedOnly.GetCheck() != 0 ) ? true : false;
HTREEITEM htiSelect = m_wndTree.GetSelectedItem();
	if( htiSelect == NULL )
		return;
HTREEITEM htiPrev = m_wndTree.LinearItemGetPrev( htiSelect, bExpandedOnly );
	if( htiPrev == NULL )
		return;
	m_wndTree.FocusItem( htiPrev, true, true );
}

void CPageTreeCtrl::OnBnClickedCheckFullRowSelect()
{
	ASSERT_VALID( this );
	if( m_wndFullRowSelectCheckBox.GetCheck() )
		m_wndTree.FullRowSelectionSet();
	else
		m_wndTree.FullRowSelectionSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckHasButtons()
{
	ASSERT_VALID( this );
	if( m_wndHasButtonsCheckBox.GetCheck() )
		m_wndTree.HasButtonsSet();
	else
		m_wndTree.HasButtonsSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckHasLines()
{
	ASSERT_VALID( this );
	if( m_wndHasLinesCheckBox.GetCheck() )
		m_wndTree.HasLinesSet();
	else
		m_wndTree.HasLinesSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckLineSatRoot()
{
	ASSERT_VALID( this );
	if( m_wndLineSatRoot.GetCheck() )
		m_wndTree.LinesAtRootSet();
	else
		m_wndTree.LinesAtRootSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckShowSelAlways()
{
	ASSERT_VALID( this );
	if( m_wndShowSelAlwaysCheckBox.GetCheck() )
		m_wndTree.ShowSelectionAlwaysSet();
	else
		m_wndTree.ShowSelectionAlwaysSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckSingleExpand()
{
	ASSERT_VALID( this );
	if( m_wndSingleExpandCheckBox.GetCheck() )
		m_wndTree.SingleExpandSet();
	else
		m_wndTree.SingleExpandSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckTrackSelect()
{
	ASSERT_VALID( this );
	if( m_wndTrackSelectCheckBox.GetCheck() )
		m_wndTree.TrackSelectionSet();
	else
		m_wndTree.TrackSelectionSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckMultipleSelection()
{
	ASSERT_VALID( this );
	if( m_wndMultipleSelectionCheckBox.GetCheck() )
		m_wndTree.MultipleSelectionSet();
	else
		m_wndTree.MultipleSelectionSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckFocusDisableItems()
{
	ASSERT_VALID( this );
	if( m_wndFocusDisableItemsCheckBox.GetCheck() )
		m_wndTree.FocusDisabledItemsSet();
	else
		m_wndTree.FocusDisabledItemsSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckSelectDisableItems()
{
	ASSERT_VALID( this );
	if( m_wndSelectDisableItemsCheckBox.GetCheck() )
		m_wndTree.SelectDisabledItemsSet();
	else
		m_wndTree.SelectDisabledItemsSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckSelectCollapsedItems()
{
	ASSERT_VALID( this );
	if( m_wndSelectCollapsedItemsCheckBox.GetCheck() )
		m_wndTree.SelectCollapsedItemsSet();
	else
		m_wndTree.SelectCollapsedItemsSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckSubtractSelection()
{
	ASSERT_VALID( this );
	if( m_wndSubtractSelectionCheckBox.GetCheck() )
		m_wndTree.SubtractSelectionSet();
	else
		m_wndTree.SubtractSelectionSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckHoverDisableItems()
{
	ASSERT_VALID( this );
	if( m_wndHoverDisableItemsCheckBox.GetCheck() )
		m_wndTree.HoverDisabledItemsSet();
	else
		m_wndTree.HoverDisabledItemsSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckHoverTreeBoxEx()
{
	ASSERT_VALID( this );
	if( m_wndHoverTreeBoxExCheckBox.GetCheck() )
		m_wndTree.HoverTreeBoxesSet();
	else
		m_wndTree.HoverTreeBoxesSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckHoverCheckBoxEx()
{
	ASSERT_VALID( this );
	if( m_wndHoverCheckBoxExCheckBox.GetCheck() )
		m_wndTree.HoverCheckBoxesSet();
	else
		m_wndTree.HoverCheckBoxesSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckHoverRadioBoxEx()
{
	ASSERT_VALID( this );
	if( m_wndHoverRadioBoxExCheckBox.GetCheck() )
		m_wndTree.HoverRadioBoxesSet();
	else
		m_wndTree.HoverRadioBoxesSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedButtonTreeBoxXp()
{
	m_wndTree.LoadWinXPTreeBox();
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedButtonTreeBoxVistaSet()
{
	m_wndTree.LoadWinVistaTreeBox();
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnSize(UINT nType, int cx, int cy)
{
	CExtResizableDialog::OnSize(nType, cx, cy);
	if( nType != SIZE_MINIMIZED )
		RepositionBars( 0, 0xFFFF, 0 );
}
void CPageTreeCtrl::OnBnClickedCheckSolidOutline()
{
	if( m_wndOutlineSolidCheckBox.GetCheck() )
		m_wndTree.LineStyleSet( CExtTreeCtrl::line_style_solid );
	else
		m_wndTree.LineStyleSet( CExtTreeCtrl::line_style_dots );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckTreeBoxBitmap()
{
	if( m_wndTreeBoxBitmapCheckBox.GetCheck() )
		m_wndTree.TreeBoxStyleSet( CExtTreeCtrl::tree_box_style_bitmap );
	m_wndTreeBoxRoundCheckBox.SetCheck( 0 );
	m_wndTreeBoxSquareCheckBox.SetCheck( 0 );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckTreeBoxSquare()
{
	if( m_wndTreeBoxSquareCheckBox.GetCheck() )
		m_wndTree.TreeBoxStyleSet( CExtTreeCtrl::tree_box_style_square );
	m_wndTreeBoxRoundCheckBox.SetCheck( 0 );
	m_wndTreeBoxBitmapCheckBox.SetCheck( 0 );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckTreeBoxRound()
{
	if( m_wndTreeBoxRoundCheckBox.GetCheck() )
		m_wndTree.TreeBoxStyleSet( CExtTreeCtrl::tree_box_style_round );
	m_wndTreeBoxBitmapCheckBox.SetCheck( 0 );
	m_wndTreeBoxSquareCheckBox.SetCheck( 0 );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnTimer( __EXT_MFC_UINT_PTR nIDEvent)
{
	if( nIDEvent == 7 )
	{
		if( m_wndTree.GetSafeHwnd() == NULL )
			return;
		POSITION pos = m_listTreeItemsWithProgressControl.GetHeadPosition();
		bool bAnyControlInvalidated = false;
		for( ; pos != NULL; )
		{
			HTREEITEM hti = m_listTreeItemsWithProgressControl.GetNext( pos );
			switch( m_wndTree.TreeItemInfoGet( hti ).m_eICT )
			{
			case CExtTreeCtrl::__EICT_PROGRESS_BAR_THEMED:
			case CExtTreeCtrl::__EICT_PROGRESS_BAR_SMOOTH:
			case CExtTreeCtrl::__EICT_PROGRESS_BAR_WITH_BOXES:
			case CExtTreeCtrl::__EICT_PROGRESS_BAR_ENDLESS:
			{
				m_wndTree.TreeItemInfoGet( hti ).m_nProgressPos ++;
				if( m_wndTree.TreeItemInfoGet( hti ).m_nProgressPos == m_wndTree.TreeItemInfoGet( hti ).m_nProgressMax )
					m_wndTree.TreeItemInfoGet( hti ).m_nProgressPos = m_wndTree.TreeItemInfoGet( hti ).m_nProgressMin;
				CRect rcItemControl;
				if( m_wndTree.TreeItemRectGet( hti, rcItemControl, CExtTreeCtrl::e_tirt_control ) )
				{
					m_wndTree.InvalidateRect( &rcItemControl );
					bAnyControlInvalidated = true;
				}
			}
			break;
			}
		}
//		if( bAnyControlInvalidated )
//			m_wndTree.UpdateWindow();
		return;
	}

	CExtResizableDialog::OnTimer(nIDEvent);
}

void CPageTreeCtrl::OnBnClickedCheckAutoDisableChildrenItems()
{
	ASSERT_VALID( this );
	if( m_wndAutoDisableChildrenItems.GetCheck() )
		m_wndTree.AutoDisableChildrenItemsSet();
	else
		m_wndTree.AutoDisableChildrenItemsSet( false );
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckEditLabels()
{
	ASSERT_VALID( this );
	if( m_wndEditLabelsCheckBox.GetCheck() )
	{
		if( ( m_wndTree.GetStyle() & TVS_EDITLABELS ) == 0 )
				m_wndTree.ModifyStyle( NULL, TVS_EDITLABELS );
	}
	else
	{
		if( ( m_wndTree.GetStyle() & TVS_EDITLABELS ) != 0 )
				m_wndTree.ModifyStyle( TVS_EDITLABELS, NULL );
	}
	m_wndTree.Invalidate();
}

void CPageTreeCtrl::OnBnClickedCheckReadOnlyLabels()
{
	m_wndTree.ReadOnlyLabelsSet( m_wndReadOnlyLabelsCheckBox.GetCheck() ? true : false );
}

void CPageTreeCtrl::OnBnClickedCheckInfoTip()
{
	ASSERT_VALID( this );
	if( m_wndInfoTipCheckBox.GetCheck() )
	{
		if( ( m_wndTree.GetStyle() & TVS_INFOTIP ) == 0 )
				m_wndTree.ModifyStyle( NULL, TVS_INFOTIP );
	}
	else
	{
		if( ( m_wndTree.GetStyle() & TVS_INFOTIP ) != 0 )
				m_wndTree.ModifyStyle( TVS_INFOTIP, NULL );
	}
	m_wndTree.Invalidate();
}

#endif // ( ! defined __EXT_MFC_NO_TREE_VIEW_CTRL )

void CPageTreeCtrl::OnBegindragTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	pNMHDR;
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	::AfxMessageBox( _T("CPageTreeCtrl::OnBegindragTree1") );
	*pResult = 0;
}

void CPageTreeCtrl::OnBeginRdragTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	pNMHDR;
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	::AfxMessageBox( _T("CPageTreeCtrl::OnBeginRdragTree1") );
	*pResult = 0;
}

void CPageTreeCtrl::OnGetdispinfoTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT_VALID( this );
TV_DISPINFO * pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	if(		( pTVDispInfo->item.mask & (TVIF_TEXT) ) != 0
		&&	pTVDispInfo->item.pszText != NULL
		&&	pTVDispInfo->item.cchTextMax > 0
		)
	{
		::memset( pTVDispInfo->item.pszText, 0, pTVDispInfo->item.cchTextMax * sizeof(TCHAR) );
		CString str;
		str.Format( _T("Item 0x%08X with externally queried text via the CPageTreeCtrl::OnGetdispinfoTree1() handler method"), pTVDispInfo->item.hItem );
		__EXT_MFC_STRNCPY( pTVDispInfo->item.pszText, pTVDispInfo->item.cchTextMax - 1, LPCTSTR(str), str.GetLength() );
	}
	*pResult = 0;
}

void CPageTreeCtrl::OnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT_VALID( this );
NM_TREEVIEW * pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	pNMTreeView;
	*pResult = 0;
}

void CPageTreeCtrl::OnItemexpandedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT_VALID( this );
NM_TREEVIEW * pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	pNMTreeView;
	*pResult = 0;
}

void CPageTreeCtrl::OnBeginlabeleditTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	pTVDispInfo;
	*pResult = 0;
}

void CPageTreeCtrl::OnEndlabeleditTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	pTVDispInfo;
	*pResult = 1;
}

