// PageTabWindow.cpp : implementation file
//

#include "stdafx.h"

#if (!defined __EXT_MFC_NO_TAB_CTRL)

#include "profuis_controls.h"
#include "PageTabWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPageTabWindow::demo_items_data_t CPageTabWindow::g_ItemsData[] =
{
	{ _T("&DevStudio"),				true  },
		
	{ _T("Help &Index"),			true  },
	{ _T("Help &Search"),			false },
	{ _T("Index &Results"),			false },
	{ _T("&Contents"),				false },
	
	{ _T("R&eload"),				true  },
	{ _T("&Home"),					false },
	{ _T("&Browser"),				false },
	
	{ _T("Find Results &1"),		true  },
	{ _T("Find &Symbol Results"),	false },
	{ _T("Find Results &2"),		false },
	{ _T("Find In Files"),			false },
	
	{ _T("&Favorites"),				true  },
	{ _T("&Add To Favorites"),		false },
	
	{ _T("F&ull Screen"),			true  },
	
	{ _T("D&ynamic Help"),			true  },
	
	{ _T("Resource &View"),			true  },
	{ _T("Solution E&xplorer"),		false },
	{ _T("&Class View"),			false },
	
	{ _T("&Toolbox"),				true  },
	{ _T("Task &List"),				false },
	
	{ _T("&Open"),					true  },
	{ _T("Save &All"),				false },
};

/////////////////////////////////////////////////////////////////////////////
// CPageTabWindow dialog
/////////////////////////////////////////////////////////////////////////////

CPageTabWindow::CPageTabWindow(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageTabWindow::IDD, pParent)
	, m_nDemoImageListSize( sizeof( g_ItemsData ) / sizeof( g_ItemsData[0] ) )
	, m_nDemoImageListAddIndexInner( 0 )
	, m_nDemoImageListAddIndexOuter( 0 )
{
	//{{AFX_DATA_INIT(CPageTabWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	VERIFY(
		m_ImageList.Create(
		IDR_TAB_IMAGES,
		16,
		1,
		RGB(255,0,255)
		)
		);
	ASSERT( m_ImageList.GetImageCount() >= m_nDemoImageListSize );
}


void CPageTabWindow::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageTabWindow)
	DDX_Control(pDX, IDC_CHECK_SHOW_END, m_CheckShowEnd);
	DDX_Control(pDX, IDC_CHECK_SHOW_HOME, m_CheckShowHome);
	DDX_Control(pDX, IDC_TAB_STYLE_GROUP_BOX, m_wndGroupBoxStyle);
	DDX_Control(pDX, IDC_RADIO_STYLE_STANDARD, m_wndRadioStyleStandard);
	DDX_Control(pDX, IDC_RADIO_STYLE_ONENOTE, m_wndRadioStyleOneNote);
	DDX_Control(pDX, IDC_RADIO_STYLE_WHIDBEY, m_wndRadioStyleWhidbey);
	DDX_Control(pDX, IDC_RADIO_STYLE_FLAT, m_wndRadioStyleFlat);
	DDX_Control(pDX, IDC_RADIO_STYLE_BUTTONS, m_wndRadioStyleButtons);
	DDX_Control(pDX, IDC_CHECK_ITEM_DRAGGING, m_CheckItemDragging);
	DDX_Control(pDX, IDC_CHECK_ENABLE_TAB_LIST, m_CheckEnableTabList);
	DDX_Control(pDX, IDC_CHECK_SHOW_TAB_LIST, m_CheckShowTabList);
	DDX_Control(pDX, IDC_CHECK_FOCUS_ON_HOVER, m_CheckFocusOnHover);
	DDX_Control(pDX, IDC_CHECK_FULL_WIDTHS, m_CheckFullRowWidth);
	DDX_Control(pDX, IDC_CHECK_CENTER_TEXT, m_CheckCenterText);
	DDX_Control(pDX, IDC_CHECK_AUTOHIDE_SCROLL, m_CheckAutoHideScroll);
	DDX_Control(pDX, IDC_CHECK_ENABLE_HELP, m_CheckEnableHelp);
	DDX_Control(pDX, IDC_CHECK_ENABLE_CLOSE, m_CheckEnableClose);
	DDX_Control(pDX, IDC_CHECK_SHOW_HELP, m_CheckShowHelp);
	DDX_Control(pDX, IDC_CHECK_BOLD_SELECTION, m_CheckBoldSelection);
	DDX_Control(pDX, IDC_CHECK_SHOW_CLOSE, m_CheckShowClose);
	DDX_Control(pDX, IDC_CHECK_INVERT_VERT_FONT, m_CheckInvertFont);
	DDX_Control(pDX, IDC_CHECK_EQUAL_WIDTHS, m_CheckEqualWidth);
	DDX_Control(pDX, IDC_CHECK_SHOW_BORDERS, m_CheckShowBorders);
	DDX_Control(pDX, IDC_BUTTON_ADD_TAB_10, m_BtnAdd10);
	DDX_Control(pDX, IDC_BUTTON_ADD_TAB_1, m_BtnAdd1);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_TABS, m_BtnClear);
	//}}AFX_DATA_MAP

	DDX_Control( pDX, ID_TABCTRL_OUTER_TOP_BUTTONS, m_wndTabButtonsOuterTop );
	DDX_Control( pDX, ID_TABCTRL_OUTER_BOTTOM_BUTTONS, m_wndTabButtonsOuterBottom );
	DDX_Control( pDX, ID_TABCTRL_OUTER_LEFT_BUTTONS, m_wndTabButtonsOuterLeft );
	DDX_Control( pDX, ID_TABCTRL_OUTER_RIGHT_BUTTONS, m_wndTabButtonsOuterRight );

	DDX_Control( pDX, ID_TABCTRL_INNER_TOP_BUTTONS, m_wndTabButtonsInnerTop );
	DDX_Control( pDX, ID_TABCTRL_INNER_BOTTOM_BUTTONS, m_wndTabButtonsInnerBottom );
	DDX_Control( pDX, ID_TABCTRL_INNER_LEFT_BUTTONS, m_wndTabButtonsInnerLeft );
	DDX_Control( pDX, ID_TABCTRL_INNER_RIGHT_BUTTONS, m_wndTabButtonsInnerRight );

	DDX_Control( pDX, ID_TABCTRL_OUTER_TOP_FLAT, m_wndTabFlatOuterTop );
	DDX_Control( pDX, ID_TABCTRL_OUTER_BOTTOM_FLAT, m_wndTabFlatOuterBottom );
	DDX_Control( pDX, ID_TABCTRL_OUTER_LEFT_FLAT, m_wndTabFlatOuterLeft );
	DDX_Control( pDX, ID_TABCTRL_OUTER_RIGHT_FLAT, m_wndTabFlatOuterRight );

	DDX_Control( pDX, ID_TABCTRL_INNER_TOP_FLAT, m_wndTabFlatInnerTop );
	DDX_Control( pDX, ID_TABCTRL_INNER_BOTTOM_FLAT, m_wndTabFlatInnerBottom );
	DDX_Control( pDX, ID_TABCTRL_INNER_LEFT_FLAT, m_wndTabFlatInnerLeft );
	DDX_Control( pDX, ID_TABCTRL_INNER_RIGHT_FLAT, m_wndTabFlatInnerRight );

	DDX_Control( pDX, ID_TABCTRL_OUTER_TOP_WHIDBEY, m_wndTabWhidbeyOuterTop );
	DDX_Control( pDX, ID_TABCTRL_OUTER_BOTTOM_WHIDBEY, m_wndTabWhidbeyOuterBottom );
	DDX_Control( pDX, ID_TABCTRL_OUTER_LEFT_WHIDBEY, m_wndTabWhidbeyOuterLeft );
	DDX_Control( pDX, ID_TABCTRL_OUTER_RIGHT_WHIDBEY, m_wndTabWhidbeyOuterRight );

	DDX_Control( pDX, ID_TABCTRL_INNER_TOP_WHIDBEY, m_wndTabWhidbeyInnerTop );
	DDX_Control( pDX, ID_TABCTRL_INNER_BOTTOM_WHIDBEY, m_wndTabWhidbeyInnerBottom );
	DDX_Control( pDX, ID_TABCTRL_INNER_LEFT_WHIDBEY, m_wndTabWhidbeyInnerLeft );
	DDX_Control( pDX, ID_TABCTRL_INNER_RIGHT_WHIDBEY, m_wndTabWhidbeyInnerRight );

	DDX_Control( pDX, ID_TABCTRL_OUTER_TOP_ONENOTE, m_wndTabOneNoteOuterTop );
	DDX_Control( pDX, ID_TABCTRL_OUTER_BOTTOM_ONENOTE, m_wndTabOneNoteOuterBottom );
	DDX_Control( pDX, ID_TABCTRL_OUTER_LEFT_ONENOTE, m_wndTabOneNoteOuterLeft );
	DDX_Control( pDX, ID_TABCTRL_OUTER_RIGHT_ONENOTE, m_wndTabOneNoteOuterRight );

	DDX_Control( pDX, ID_TABCTRL_INNER_TOP_ONENOTE, m_wndTabOneNoteInnerTop );
	DDX_Control( pDX, ID_TABCTRL_INNER_BOTTOM_ONENOTE, m_wndTabOneNoteInnerBottom );
	DDX_Control( pDX, ID_TABCTRL_INNER_LEFT_ONENOTE, m_wndTabOneNoteInnerLeft );
	DDX_Control( pDX, ID_TABCTRL_INNER_RIGHT_ONENOTE, m_wndTabOneNoteInnerRight );

	DDX_Control( pDX, ID_TABCTRL_OUTER_TOP, m_wndTabOuterTop );
	DDX_Control( pDX, ID_TABCTRL_OUTER_BOTTOM, m_wndTabOuterBottom );
	DDX_Control( pDX, ID_TABCTRL_OUTER_LEFT, m_wndTabOuterLeft );
	DDX_Control( pDX, ID_TABCTRL_OUTER_RIGHT, m_wndTabOuterRight );

	DDX_Control( pDX, ID_TABCTRL_INNER_TOP, m_wndTabInnerTop );
	DDX_Control( pDX, ID_TABCTRL_INNER_BOTTOM, m_wndTabInnerBottom );
	DDX_Control( pDX, ID_TABCTRL_INNER_LEFT, m_wndTabInnerLeft );
	DDX_Control( pDX, ID_TABCTRL_INNER_RIGHT, m_wndTabInnerRight );
}


BEGIN_MESSAGE_MAP(CPageTabWindow, CPageBase)
	//{{AFX_MSG_MAP(CPageTabWindow)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_TABS, OnButtonClearTabs)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TAB_1, OnButtonAddTab1)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TAB_10, OnButtonAddTab10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageTabWindow message handlers

BOOL CPageTabWindow::OnInitDialog() 
{
	if( !CPageBase::OnInitDialog() ){
		ASSERT( FALSE );
		return FALSE;
	}

	if( m_ImageList.GetSafeHandle() == NULL ){
		ASSERT( FALSE );
		return FALSE;
	}
	
	AddAnchor( IDC_CHECK_SHOW_BORDERS,		CSize( 0,  0) );
	AddAnchor( IDC_CHECK_EQUAL_WIDTHS,		CSize( 0,  7) );
	AddAnchor( IDC_CHECK_FULL_WIDTHS,		CSize( 0,  14) );
	AddAnchor( IDC_CHECK_CENTER_TEXT,		CSize( 0,  21) );
	AddAnchor( IDC_CHECK_INVERT_VERT_FONT,	CSize( 0,  28) );
	AddAnchor( IDC_CHECK_BOLD_SELECTION,	CSize( 0,  35) );
	AddAnchor( IDC_CHECK_SHOW_CLOSE,		CSize( 0,  42) );
	AddAnchor( IDC_CHECK_ENABLE_CLOSE,		CSize( 0,  42) );
	AddAnchor( IDC_CHECK_SHOW_HELP,			CSize( 0,  49) );
	AddAnchor( IDC_CHECK_ENABLE_HELP,		CSize( 0,  49) );
	AddAnchor( IDC_CHECK_SHOW_TAB_LIST,		CSize( 0,  56) );
	AddAnchor( IDC_CHECK_ENABLE_TAB_LIST,	CSize( 0,  56) );
	AddAnchor( IDC_CHECK_AUTOHIDE_SCROLL,	CSize( 0,  63) );
	AddAnchor( IDC_CHECK_SHOW_HOME,			CSize( 0,  70) );
	AddAnchor( IDC_CHECK_SHOW_END,			CSize( 0,  77) );
	AddAnchor( IDC_CHECK_FOCUS_ON_HOVER,	CSize( 0,  84) );
	AddAnchor( IDC_CHECK_ITEM_DRAGGING,		CSize( 0,  100) );

	AddAnchor( IDC_BUTTON_CLEAR_TABS,		__RDA_RB );
	AddAnchor( IDC_BUTTON_ADD_TAB_1,		__RDA_LB );
	AddAnchor( IDC_BUTTON_ADD_TAB_10,		__RDA_LB );
	
	AddAnchor( IDC_RADIO_STYLE_STANDARD,	CSize( 50, 50), CSize( 50, 50) );
	AddAnchor( IDC_RADIO_STYLE_ONENOTE,		CSize( 50, 50), CSize( 50, 50) );
	AddAnchor( IDC_RADIO_STYLE_WHIDBEY,		CSize( 50, 50), CSize( 50, 50) );
	AddAnchor( IDC_RADIO_STYLE_FLAT,		CSize( 50, 50), CSize( 50, 50) );
	AddAnchor( IDC_RADIO_STYLE_BUTTONS,		CSize( 50, 50), CSize( 50, 50) );
	AddAnchor( IDC_TAB_STYLE_GROUP_BOX,		CSize( 50, 50), CSize( 50, 50) );

	AddAnchor( ID_TABCTRL_OUTER_TOP,			__RDA_LT, __RDA_RT );
	AddAnchor( ID_TABCTRL_OUTER_BOTTOM,			__RDA_LB, __RDA_RB );
	AddAnchor( ID_TABCTRL_OUTER_LEFT,			__RDA_LT, __RDA_LB );
	AddAnchor( ID_TABCTRL_OUTER_RIGHT,			__RDA_RT, __RDA_RB );
	
	AddAnchor( ID_TABCTRL_INNER_TOP,			__RDA_LT, __RDA_RT );
	AddAnchor( ID_TABCTRL_INNER_BOTTOM,			__RDA_LB, __RDA_RB );
	AddAnchor( ID_TABCTRL_INNER_LEFT,			__RDA_LT, __RDA_LB );
	AddAnchor( ID_TABCTRL_INNER_RIGHT,			__RDA_RT, __RDA_RB );

	AddAnchor( ID_TABCTRL_OUTER_TOP_ONENOTE,	__RDA_LT, __RDA_RT );
	AddAnchor( ID_TABCTRL_OUTER_BOTTOM_ONENOTE,	__RDA_LB, __RDA_RB );
	AddAnchor( ID_TABCTRL_OUTER_LEFT_ONENOTE,	__RDA_LT, __RDA_LB );
	AddAnchor( ID_TABCTRL_OUTER_RIGHT_ONENOTE,	__RDA_RT, __RDA_RB );
	
	AddAnchor( ID_TABCTRL_INNER_TOP_ONENOTE,	__RDA_LT, __RDA_RT );
	AddAnchor( ID_TABCTRL_INNER_BOTTOM_ONENOTE,	__RDA_LB, __RDA_RB );
	AddAnchor( ID_TABCTRL_INNER_LEFT_ONENOTE,	__RDA_LT, __RDA_LB );
	AddAnchor( ID_TABCTRL_INNER_RIGHT_ONENOTE,	__RDA_RT, __RDA_RB );

	AddAnchor( ID_TABCTRL_OUTER_TOP_WHIDBEY,	__RDA_LT, __RDA_RT );
	AddAnchor( ID_TABCTRL_OUTER_BOTTOM_WHIDBEY,	__RDA_LB, __RDA_RB );
	AddAnchor( ID_TABCTRL_OUTER_LEFT_WHIDBEY,	__RDA_LT, __RDA_LB );
	AddAnchor( ID_TABCTRL_OUTER_RIGHT_WHIDBEY,	__RDA_RT, __RDA_RB );
	
	AddAnchor( ID_TABCTRL_INNER_TOP_WHIDBEY,	__RDA_LT, __RDA_RT );
	AddAnchor( ID_TABCTRL_INNER_BOTTOM_WHIDBEY,	__RDA_LB, __RDA_RB );
	AddAnchor( ID_TABCTRL_INNER_LEFT_WHIDBEY,	__RDA_LT, __RDA_LB );
	AddAnchor( ID_TABCTRL_INNER_RIGHT_WHIDBEY,	__RDA_RT, __RDA_RB );

	AddAnchor( ID_TABCTRL_OUTER_TOP_FLAT,		__RDA_LT, __RDA_RT );
	AddAnchor( ID_TABCTRL_OUTER_BOTTOM_FLAT,	__RDA_LB, __RDA_RB );
	AddAnchor( ID_TABCTRL_OUTER_LEFT_FLAT,		__RDA_LT, __RDA_LB );
	AddAnchor( ID_TABCTRL_OUTER_RIGHT_FLAT,		__RDA_RT, __RDA_RB );
	
	AddAnchor( ID_TABCTRL_INNER_TOP_FLAT,		__RDA_LT, __RDA_RT );
	AddAnchor( ID_TABCTRL_INNER_BOTTOM_FLAT,	__RDA_LB, __RDA_RB );
	AddAnchor( ID_TABCTRL_INNER_LEFT_FLAT,		__RDA_LT, __RDA_LB );
	AddAnchor( ID_TABCTRL_INNER_RIGHT_FLAT,		__RDA_RT, __RDA_RB );

	AddAnchor( ID_TABCTRL_OUTER_TOP_BUTTONS,	__RDA_LT, __RDA_RT );
	AddAnchor( ID_TABCTRL_OUTER_BOTTOM_BUTTONS,	__RDA_LB, __RDA_RB );
	AddAnchor( ID_TABCTRL_OUTER_LEFT_BUTTONS,	__RDA_LT, __RDA_LB );
	AddAnchor( ID_TABCTRL_OUTER_RIGHT_BUTTONS,	__RDA_RT, __RDA_RB );
	
	AddAnchor( ID_TABCTRL_INNER_TOP_BUTTONS,	__RDA_LT, __RDA_RT );
	AddAnchor( ID_TABCTRL_INNER_BOTTOM_BUTTONS,	__RDA_LB, __RDA_RB );
	AddAnchor( ID_TABCTRL_INNER_LEFT_BUTTONS,	__RDA_LT, __RDA_LB );
	AddAnchor( ID_TABCTRL_INNER_RIGHT_BUTTONS,	__RDA_RT, __RDA_RB );

	m_wndTabButtonsOuterTop.ModifyTabWndStyle(		0, __ETWS_ORIENT_TOP	| __ETWS_GROUPED );
	m_wndTabButtonsOuterBottom.ModifyTabWndStyle(	0, __ETWS_ORIENT_BOTTOM	| __ETWS_GROUPED );
	m_wndTabButtonsOuterLeft.ModifyTabWndStyle(		0, __ETWS_ORIENT_LEFT	| __ETWS_GROUPED );
	m_wndTabButtonsOuterRight.ModifyTabWndStyle(	0, __ETWS_ORIENT_RIGHT	| __ETWS_GROUPED );
	m_wndTabButtonsInnerTop.ModifyTabWndStyle(		0, __ETWS_ORIENT_TOP );
	m_wndTabButtonsInnerBottom.ModifyTabWndStyle(	0, __ETWS_ORIENT_BOTTOM );
	m_wndTabButtonsInnerLeft.ModifyTabWndStyle(		0, __ETWS_ORIENT_LEFT );
	m_wndTabButtonsInnerRight.ModifyTabWndStyle(	0, __ETWS_ORIENT_RIGHT );

	m_wndTabFlatOuterTop.ModifyTabWndStyle(			0, __ETWS_ORIENT_TOP	| __ETWS_GROUPED );
	m_wndTabFlatOuterBottom.ModifyTabWndStyle(		0, __ETWS_ORIENT_BOTTOM	| __ETWS_GROUPED );
	m_wndTabFlatOuterLeft.ModifyTabWndStyle(		0, __ETWS_ORIENT_LEFT	| __ETWS_GROUPED );
	m_wndTabFlatOuterRight.ModifyTabWndStyle(		0, __ETWS_ORIENT_RIGHT	| __ETWS_GROUPED );
	m_wndTabFlatInnerTop.ModifyTabWndStyle(			0, __ETWS_ORIENT_TOP );
	m_wndTabFlatInnerBottom.ModifyTabWndStyle(		0, __ETWS_ORIENT_BOTTOM );
	m_wndTabFlatInnerLeft.ModifyTabWndStyle(		0, __ETWS_ORIENT_LEFT );
	m_wndTabFlatInnerRight.ModifyTabWndStyle(		0, __ETWS_ORIENT_RIGHT );

	m_wndTabWhidbeyOuterTop.ModifyTabWndStyle(		0, __ETWS_ORIENT_TOP	| __ETWS_GROUPED );
	m_wndTabWhidbeyOuterBottom.ModifyTabWndStyle(	0, __ETWS_ORIENT_BOTTOM	| __ETWS_GROUPED );
	m_wndTabWhidbeyOuterLeft.ModifyTabWndStyle(		0, __ETWS_ORIENT_LEFT	| __ETWS_GROUPED );
	m_wndTabWhidbeyOuterRight.ModifyTabWndStyle(	0, __ETWS_ORIENT_RIGHT	| __ETWS_GROUPED );
	m_wndTabWhidbeyInnerTop.ModifyTabWndStyle(		0, __ETWS_HIDE_ICONS | __ETWS_ORIENT_TOP );
	m_wndTabWhidbeyInnerBottom.ModifyTabWndStyle(	0, __ETWS_HIDE_ICONS | __ETWS_ORIENT_BOTTOM );
	m_wndTabWhidbeyInnerLeft.ModifyTabWndStyle(		0, __ETWS_HIDE_ICONS | __ETWS_ORIENT_LEFT );
	m_wndTabWhidbeyInnerRight.ModifyTabWndStyle(	0, __ETWS_HIDE_ICONS | __ETWS_ORIENT_RIGHT );

	m_wndTabOneNoteOuterTop.ModifyTabWndStyle(		0, __ETWS_ORIENT_TOP	| __ETWS_GROUPED );
	m_wndTabOneNoteOuterBottom.ModifyTabWndStyle(	0, __ETWS_ORIENT_BOTTOM	| __ETWS_GROUPED );
	m_wndTabOneNoteOuterLeft.ModifyTabWndStyle(		0, __ETWS_ORIENT_LEFT	| __ETWS_GROUPED );
	m_wndTabOneNoteOuterRight.ModifyTabWndStyle(	0, __ETWS_ORIENT_RIGHT	| __ETWS_GROUPED );
	m_wndTabOneNoteInnerTop.ModifyTabWndStyle(		0, __ETWS_ORIENT_TOP );
	m_wndTabOneNoteInnerBottom.ModifyTabWndStyle(	0, __ETWS_ORIENT_BOTTOM );
	m_wndTabOneNoteInnerLeft.ModifyTabWndStyle(		0, __ETWS_ORIENT_LEFT );
	m_wndTabOneNoteInnerRight.ModifyTabWndStyle(	0, __ETWS_ORIENT_RIGHT );

	m_wndTabOuterTop.ModifyTabWndStyle(				0, __ETWS_ORIENT_TOP	| __ETWS_GROUPED );
	m_wndTabOuterBottom.ModifyTabWndStyle(			0, __ETWS_ORIENT_BOTTOM	| __ETWS_GROUPED );
	m_wndTabOuterLeft.ModifyTabWndStyle(			0, __ETWS_ORIENT_LEFT	| __ETWS_GROUPED );
	m_wndTabOuterRight.ModifyTabWndStyle(			0, __ETWS_ORIENT_RIGHT	| __ETWS_GROUPED );
	m_wndTabInnerTop.ModifyTabWndStyle(				0, __ETWS_ORIENT_TOP );
	m_wndTabInnerBottom.ModifyTabWndStyle(			0, __ETWS_ORIENT_BOTTOM );
	m_wndTabInnerLeft.ModifyTabWndStyle(			0, __ETWS_ORIENT_LEFT );
	m_wndTabInnerRight.ModifyTabWndStyle(			0, __ETWS_ORIENT_RIGHT );

CWnd * pWndChild = GetWindow( GW_CHILD );
	for( ; pWndChild != NULL; pWndChild = pWndChild->GetWindow( GW_HWNDNEXT ) )
	{
		CExtTabWnd * pWndTab = DYNAMIC_DOWNCAST( CExtTabWnd, pWndChild );
		if( pWndTab != NULL )
			pWndTab->ModifyTabWndStyle( 0, __ETWS_CENTERED_TEXT | __ETWS_BOLD_SELECTION );
	} // for( ; pWndChild != NULL; pWndChild = pWndChild->GetWindow( GW_HWNDNEXT ) )
	
	//////////////////////////////////////////////////////////////////////////
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_EQUAL_WIDTHS, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_EQUAL_WIDTHS
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_FULL_WIDTHS, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_FULL_WIDTH
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_CENTER_TEXT, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_CENTERED_TEXT
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_INVERT_VERT_FONT, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_INVERT_VERT_FONT
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BOLD_SELECTION, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_BOLD_SELECTION
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_CLOSE, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_SHOW_BTN_CLOSE
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	GetDlgItem( IDC_CHECK_ENABLE_CLOSE )->EnableWindow( 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_SHOW_BTN_CLOSE 
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_ENABLE_CLOSE, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_ENABLED_BTN_CLOSE
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_HELP, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_SHOW_BTN_HELP
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	GetDlgItem( IDC_CHECK_ENABLE_HELP )->EnableWindow( 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_SHOW_BTN_HELP 
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_ENABLE_HELP, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_ENABLED_BTN_HELP
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_TAB_LIST, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_SHOW_BTN_TAB_LIST
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	GetDlgItem( IDC_CHECK_ENABLE_TAB_LIST )->EnableWindow( 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_SHOW_BTN_TAB_LIST 
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_ENABLE_TAB_LIST, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_ENABLED_BTN_TAB_LIST
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_AUTOHIDE_SCROLL, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_AUTOHIDE_SCROLL
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_FOCUS_ON_HOVER, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_HOVER_FOCUS
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_ITEM_DRAGGING, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_ITEM_DRAGGING
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_HOME, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_SHOW_BTN_SCROLL_HOME
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_END, 
		m_wndTabOuterTop.GetTabWndStyle() & __ETWS_SHOW_BTN_SCROLL_END
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	m_wndTabButtonsOuterTop.ShowWindow( SW_HIDE );
	m_wndTabButtonsOuterBottom.ShowWindow( SW_HIDE );
	m_wndTabButtonsOuterLeft.ShowWindow( SW_HIDE );
	m_wndTabButtonsOuterRight.ShowWindow( SW_HIDE );
	m_wndTabButtonsInnerTop.ShowWindow( SW_HIDE );
	m_wndTabButtonsInnerBottom.ShowWindow( SW_HIDE );
	m_wndTabButtonsInnerLeft.ShowWindow( SW_HIDE );
	m_wndTabButtonsInnerRight.ShowWindow( SW_HIDE );

	m_wndTabFlatOuterTop.ShowWindow( SW_HIDE );
	m_wndTabFlatOuterBottom.ShowWindow( SW_HIDE );
	m_wndTabFlatOuterLeft.ShowWindow( SW_HIDE );
	m_wndTabFlatOuterRight.ShowWindow( SW_HIDE );
	m_wndTabFlatInnerTop.ShowWindow( SW_HIDE );
	m_wndTabFlatInnerBottom.ShowWindow( SW_HIDE );
	m_wndTabFlatInnerLeft.ShowWindow( SW_HIDE );
	m_wndTabFlatInnerRight.ShowWindow( SW_HIDE );

	m_wndTabWhidbeyOuterTop.ShowWindow( SW_HIDE );
	m_wndTabWhidbeyOuterBottom.ShowWindow( SW_HIDE );
	m_wndTabWhidbeyOuterLeft.ShowWindow( SW_HIDE );
	m_wndTabWhidbeyOuterRight.ShowWindow( SW_HIDE );
	m_wndTabWhidbeyInnerTop.ShowWindow( SW_HIDE );
	m_wndTabWhidbeyInnerBottom.ShowWindow( SW_HIDE );
	m_wndTabWhidbeyInnerLeft.ShowWindow( SW_HIDE );
	m_wndTabWhidbeyInnerRight.ShowWindow( SW_HIDE );

	m_wndTabOneNoteOuterTop.ShowWindow( SW_HIDE );
	m_wndTabOneNoteOuterBottom.ShowWindow( SW_HIDE );
	m_wndTabOneNoteOuterLeft.ShowWindow( SW_HIDE );
	m_wndTabOneNoteOuterRight.ShowWindow( SW_HIDE );
	m_wndTabOneNoteInnerTop.ShowWindow( SW_HIDE );
	m_wndTabOneNoteInnerBottom.ShowWindow( SW_HIDE );
	m_wndTabOneNoteInnerLeft.ShowWindow( SW_HIDE );
	m_wndTabOneNoteInnerRight.ShowWindow( SW_HIDE );

	m_wndRadioStyleStandard.SetCheck( BST_CHECKED );

	OnButtonAddTab10();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

BOOL CPageTabWindow::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( nCode == CN_COMMAND )
	{
		DWORD dwTabWndStyleRemove = 0;
		DWORD dwTabWndStyleAdd = 0;
		
		switch( nID )
		{
		case IDC_RADIO_STYLE_STANDARD:
			m_wndTabButtonsOuterTop.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterRight.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerTop.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerRight.ShowWindow( SW_HIDE );

			m_wndTabFlatOuterTop.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterRight.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerTop.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerRight.ShowWindow( SW_HIDE );

			m_wndTabWhidbeyOuterTop.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterRight.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerTop.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerRight.ShowWindow( SW_HIDE );

			m_wndTabOneNoteOuterTop.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterRight.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerTop.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerRight.ShowWindow( SW_HIDE );

			m_wndTabOuterTop.ShowWindow( SW_SHOW );
			m_wndTabOuterBottom.ShowWindow( SW_SHOW );
			m_wndTabOuterLeft.ShowWindow( SW_SHOW );
			m_wndTabOuterRight.ShowWindow( SW_SHOW );
			m_wndTabInnerTop.ShowWindow( SW_SHOW );
			m_wndTabInnerBottom.ShowWindow( SW_SHOW );
			m_wndTabInnerLeft.ShowWindow( SW_SHOW );
			m_wndTabInnerRight.ShowWindow( SW_SHOW );

			return TRUE;

		case IDC_RADIO_STYLE_ONENOTE:
			m_wndTabButtonsOuterTop.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterRight.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerTop.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerRight.ShowWindow( SW_HIDE );

			m_wndTabFlatOuterTop.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterRight.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerTop.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerRight.ShowWindow( SW_HIDE );

			m_wndTabWhidbeyOuterTop.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterRight.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerTop.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerRight.ShowWindow( SW_HIDE );

			m_wndTabOuterTop.ShowWindow( SW_HIDE );
			m_wndTabOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabOuterRight.ShowWindow( SW_HIDE );
			m_wndTabInnerTop.ShowWindow( SW_HIDE );
			m_wndTabInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabInnerRight.ShowWindow( SW_HIDE );

			m_wndTabOneNoteOuterTop.ShowWindow( SW_SHOW );
			m_wndTabOneNoteOuterBottom.ShowWindow( SW_SHOW );
			m_wndTabOneNoteOuterLeft.ShowWindow( SW_SHOW );
			m_wndTabOneNoteOuterRight.ShowWindow( SW_SHOW );
			m_wndTabOneNoteInnerTop.ShowWindow( SW_SHOW );
			m_wndTabOneNoteInnerBottom.ShowWindow( SW_SHOW );
			m_wndTabOneNoteInnerLeft.ShowWindow( SW_SHOW );
			m_wndTabOneNoteInnerRight.ShowWindow( SW_SHOW );

			return TRUE;

		case IDC_RADIO_STYLE_WHIDBEY:
			m_wndTabButtonsOuterTop.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterRight.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerTop.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerRight.ShowWindow( SW_HIDE );

			m_wndTabFlatOuterTop.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterRight.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerTop.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerRight.ShowWindow( SW_HIDE );

			m_wndTabOuterTop.ShowWindow( SW_HIDE );
			m_wndTabOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabOuterRight.ShowWindow( SW_HIDE );
			m_wndTabInnerTop.ShowWindow( SW_HIDE );
			m_wndTabInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabInnerRight.ShowWindow( SW_HIDE );
		
			m_wndTabOneNoteOuterTop.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterRight.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerTop.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerRight.ShowWindow( SW_HIDE );

			m_wndTabWhidbeyOuterTop.ShowWindow( SW_SHOW );
			m_wndTabWhidbeyOuterBottom.ShowWindow( SW_SHOW );
			m_wndTabWhidbeyOuterLeft.ShowWindow( SW_SHOW );
			m_wndTabWhidbeyOuterRight.ShowWindow( SW_SHOW );
			m_wndTabWhidbeyInnerTop.ShowWindow( SW_SHOW );
			m_wndTabWhidbeyInnerBottom.ShowWindow( SW_SHOW );
			m_wndTabWhidbeyInnerLeft.ShowWindow( SW_SHOW );
			m_wndTabWhidbeyInnerRight.ShowWindow( SW_SHOW );

			return TRUE;

		case IDC_RADIO_STYLE_FLAT:
			m_wndTabButtonsOuterTop.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabButtonsOuterRight.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerTop.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabButtonsInnerRight.ShowWindow( SW_HIDE );

			m_wndTabWhidbeyOuterTop.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterRight.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerTop.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerRight.ShowWindow( SW_HIDE );

			m_wndTabOuterTop.ShowWindow( SW_HIDE );
			m_wndTabOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabOuterRight.ShowWindow( SW_HIDE );
			m_wndTabInnerTop.ShowWindow( SW_HIDE );
			m_wndTabInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabInnerRight.ShowWindow( SW_HIDE );
		
			m_wndTabOneNoteOuterTop.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterRight.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerTop.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerRight.ShowWindow( SW_HIDE );

			m_wndTabFlatOuterTop.ShowWindow( SW_SHOW );
			m_wndTabFlatOuterBottom.ShowWindow( SW_SHOW );
			m_wndTabFlatOuterLeft.ShowWindow( SW_SHOW );
			m_wndTabFlatOuterRight.ShowWindow( SW_SHOW );
			m_wndTabFlatInnerTop.ShowWindow( SW_SHOW );
			m_wndTabFlatInnerBottom.ShowWindow( SW_SHOW );
			m_wndTabFlatInnerLeft.ShowWindow( SW_SHOW );
			m_wndTabFlatInnerRight.ShowWindow( SW_SHOW );

			return TRUE;

		case IDC_RADIO_STYLE_BUTTONS:
			m_wndTabFlatOuterTop.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabFlatOuterRight.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerTop.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabFlatInnerRight.ShowWindow( SW_HIDE );

			m_wndTabWhidbeyOuterTop.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyOuterRight.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerTop.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabWhidbeyInnerRight.ShowWindow( SW_HIDE );

			m_wndTabOuterTop.ShowWindow( SW_HIDE );
			m_wndTabOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabOuterRight.ShowWindow( SW_HIDE );
			m_wndTabInnerTop.ShowWindow( SW_HIDE );
			m_wndTabInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabInnerRight.ShowWindow( SW_HIDE );
		
			m_wndTabOneNoteOuterTop.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterBottom.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterLeft.ShowWindow( SW_HIDE );
			m_wndTabOneNoteOuterRight.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerTop.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerBottom.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerLeft.ShowWindow( SW_HIDE );
			m_wndTabOneNoteInnerRight.ShowWindow( SW_HIDE );

			m_wndTabButtonsOuterTop.ShowWindow( SW_SHOW );
			m_wndTabButtonsOuterBottom.ShowWindow( SW_SHOW );
			m_wndTabButtonsOuterLeft.ShowWindow( SW_SHOW );
			m_wndTabButtonsOuterRight.ShowWindow( SW_SHOW );
			m_wndTabButtonsInnerTop.ShowWindow( SW_SHOW );
			m_wndTabButtonsInnerBottom.ShowWindow( SW_SHOW );
			m_wndTabButtonsInnerLeft.ShowWindow( SW_SHOW );
			m_wndTabButtonsInnerRight.ShowWindow( SW_SHOW );

			return TRUE;

		case IDC_CHECK_SHOW_BORDERS:
			{
				DWORD dwAddExBorderStyles = WS_EX_STATICEDGE;
				DWORD dwRemoveExBorderStyles = WS_EX_STATICEDGE;
				if( m_CheckShowBorders.GetCheck() )
					dwRemoveExBorderStyles = 0;
				else
					dwAddExBorderStyles = 0;

				m_wndTabOuterTop.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabOuterBottom.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabOuterLeft.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabOuterRight.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabInnerTop.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabInnerBottom.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabInnerLeft.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabInnerRight.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );

				m_wndTabOneNoteOuterTop.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabOneNoteOuterBottom.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabOneNoteOuterLeft.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabOneNoteOuterRight.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabOneNoteInnerTop.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabOneNoteInnerBottom.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabOneNoteInnerLeft.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabOneNoteInnerRight.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );

				m_wndTabWhidbeyOuterTop.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabWhidbeyOuterBottom.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabWhidbeyOuterLeft.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabWhidbeyOuterRight.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabWhidbeyInnerTop.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabWhidbeyInnerBottom.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabWhidbeyInnerLeft.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabWhidbeyInnerRight.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );

				m_wndTabFlatOuterTop.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabFlatOuterBottom.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabFlatOuterLeft.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabFlatOuterRight.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabFlatInnerTop.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabFlatInnerBottom.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabFlatInnerLeft.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabFlatInnerRight.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );

				m_wndTabButtonsOuterTop.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabButtonsOuterBottom.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabButtonsOuterLeft.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabButtonsOuterRight.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabButtonsInnerTop.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabButtonsInnerBottom.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabButtonsInnerLeft.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabButtonsInnerRight.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );

				UINT nSetWindowPosFlags =
					SWP_NOMOVE|SWP_NOSIZE
					|SWP_NOZORDER|SWP_NOOWNERZORDER
					|SWP_NOACTIVATE
					|SWP_FRAMECHANGED
					;
				m_wndTabOuterTop.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabOuterBottom.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabOuterLeft.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabOuterRight.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabInnerTop.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabInnerBottom.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabInnerLeft.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabInnerRight.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);

				m_wndTabOneNoteOuterTop.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabOneNoteOuterBottom.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabOneNoteOuterLeft.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabOneNoteOuterRight.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabOneNoteInnerTop.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabOneNoteInnerBottom.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabOneNoteInnerLeft.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabOneNoteInnerRight.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);

				m_wndTabWhidbeyOuterTop.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabWhidbeyOuterBottom.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabWhidbeyOuterLeft.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabWhidbeyOuterRight.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabWhidbeyInnerTop.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabWhidbeyInnerBottom.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabWhidbeyInnerLeft.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabWhidbeyInnerRight.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);

				m_wndTabFlatOuterTop.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabFlatOuterBottom.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabFlatOuterLeft.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabFlatOuterRight.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabFlatInnerTop.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabFlatInnerBottom.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabFlatInnerLeft.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabFlatInnerRight.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);

				m_wndTabButtonsOuterTop.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabButtonsOuterBottom.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabButtonsOuterLeft.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabButtonsOuterRight.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabButtonsInnerTop.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabButtonsInnerBottom.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabButtonsInnerLeft.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabButtonsInnerRight.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
			}
			break;
		case IDC_CHECK_EQUAL_WIDTHS:
			m_CheckEqualWidth.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_EQUAL_WIDTHS
				: dwTabWndStyleRemove = __ETWS_EQUAL_WIDTHS;
			break;
		case IDC_CHECK_FULL_WIDTHS:
			m_CheckFullRowWidth.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_FULL_WIDTH
				: dwTabWndStyleRemove = __ETWS_FULL_WIDTH;
			break;
		case IDC_CHECK_CENTER_TEXT:
			m_CheckCenterText.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_CENTERED_TEXT
				: dwTabWndStyleRemove = __ETWS_CENTERED_TEXT;
			break;
		case IDC_CHECK_INVERT_VERT_FONT:
			m_CheckInvertFont.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_INVERT_VERT_FONT
				: dwTabWndStyleRemove = __ETWS_INVERT_VERT_FONT;
			break;
		case IDC_CHECK_BOLD_SELECTION:
			m_CheckBoldSelection.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_BOLD_SELECTION
				: dwTabWndStyleRemove = __ETWS_BOLD_SELECTION;
			break;
		case IDC_CHECK_SHOW_CLOSE:
			m_CheckShowClose.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_SHOW_BTN_CLOSE
				: dwTabWndStyleRemove = __ETWS_SHOW_BTN_CLOSE;
			GetDlgItem( IDC_CHECK_ENABLE_CLOSE )->EnableWindow( IsDlgButtonChecked( nID ) );
			break;
		case IDC_CHECK_ENABLE_CLOSE:
			m_CheckEnableClose.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_ENABLED_BTN_CLOSE
				: dwTabWndStyleRemove = __ETWS_ENABLED_BTN_CLOSE;
			break;
		case IDC_CHECK_SHOW_HELP:
			m_CheckShowHelp.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_SHOW_BTN_HELP
				: dwTabWndStyleRemove = __ETWS_SHOW_BTN_HELP;
			GetDlgItem( IDC_CHECK_ENABLE_HELP )->EnableWindow( IsDlgButtonChecked( nID ) );
			break;
		case IDC_CHECK_ENABLE_HELP:
			m_CheckEnableHelp.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_ENABLED_BTN_HELP
				: dwTabWndStyleRemove = __ETWS_ENABLED_BTN_HELP;
			break;
		
		case IDC_CHECK_SHOW_TAB_LIST:
			m_CheckShowTabList.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_SHOW_BTN_TAB_LIST
				: dwTabWndStyleRemove = __ETWS_SHOW_BTN_TAB_LIST;
			GetDlgItem( IDC_CHECK_ENABLE_TAB_LIST )->EnableWindow( IsDlgButtonChecked( nID ) );
			break;
		case IDC_CHECK_ENABLE_TAB_LIST:
			m_CheckEnableTabList.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_ENABLED_BTN_TAB_LIST
				: dwTabWndStyleRemove = __ETWS_ENABLED_BTN_TAB_LIST;
			break;
		
		case IDC_CHECK_AUTOHIDE_SCROLL:
			m_CheckAutoHideScroll.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_AUTOHIDE_SCROLL
				: dwTabWndStyleRemove = __ETWS_AUTOHIDE_SCROLL;
			break;
		case IDC_CHECK_FOCUS_ON_HOVER:
			m_CheckFocusOnHover.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_HOVER_FOCUS
				: dwTabWndStyleRemove = __ETWS_HOVER_FOCUS;
			break;
		case IDC_CHECK_ITEM_DRAGGING:
			m_CheckItemDragging.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_ITEM_DRAGGING
				: dwTabWndStyleRemove = __ETWS_ITEM_DRAGGING;
			break;
		case IDC_CHECK_SHOW_HOME:
			m_CheckShowHome.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_SHOW_BTN_SCROLL_HOME
				: dwTabWndStyleRemove = __ETWS_SHOW_BTN_SCROLL_HOME;
			break;
		case IDC_CHECK_SHOW_END:
			m_CheckShowEnd.GetCheck() != 0 
				? dwTabWndStyleAdd = __ETWS_SHOW_BTN_SCROLL_END
				: dwTabWndStyleRemove = __ETWS_SHOW_BTN_SCROLL_END;
			break;
		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}

		m_wndTabOuterTop.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabOuterBottom.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabOuterLeft.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabOuterRight.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabInnerTop.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabInnerBottom.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabInnerLeft.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabInnerRight.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );

		m_wndTabOneNoteOuterTop.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabOneNoteOuterBottom.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabOneNoteOuterLeft.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabOneNoteOuterRight.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabOneNoteInnerTop.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabOneNoteInnerBottom.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabOneNoteInnerLeft.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabOneNoteInnerRight.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );

		m_wndTabWhidbeyOuterTop.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabWhidbeyOuterBottom.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabWhidbeyOuterLeft.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabWhidbeyOuterRight.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabWhidbeyInnerTop.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabWhidbeyInnerBottom.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabWhidbeyInnerLeft.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabWhidbeyInnerRight.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );

		m_wndTabFlatOuterTop.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabFlatOuterBottom.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabFlatOuterLeft.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabFlatOuterRight.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabFlatInnerTop.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabFlatInnerBottom.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabFlatInnerLeft.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabFlatInnerRight.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );

		m_wndTabButtonsOuterTop.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabButtonsOuterBottom.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabButtonsOuterLeft.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabButtonsOuterRight.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabButtonsInnerTop.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabButtonsInnerBottom.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabButtonsInnerLeft.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );
		m_wndTabButtonsInnerRight.ModifyTabWndStyle( dwTabWndStyleRemove, dwTabWndStyleAdd, true );

		return TRUE;
	}

	return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//////////////////////////////////////////////////////////////////////////

void CPageTabWindow::OnButtonClearTabs() 
{
	m_BtnClear.EnableWindow( FALSE );

	if( ! m_BtnAdd1.IsWindowEnabled() )
	{
		m_BtnAdd1.EnableWindow( TRUE );
		m_BtnAdd10.EnableWindow( TRUE );
	}

	m_wndTabOuterTop.ItemRemoveAll(true);
	m_wndTabOuterBottom.ItemRemoveAll(true);
	m_wndTabOuterLeft.ItemRemoveAll(true);
	m_wndTabOuterRight.ItemRemoveAll(true);
	m_wndTabInnerTop.ItemRemoveAll(true);
	m_wndTabInnerBottom.ItemRemoveAll(true);
	m_wndTabInnerLeft.ItemRemoveAll(true);
	m_wndTabInnerRight.ItemRemoveAll(true);

	m_wndTabOneNoteOuterTop.ItemRemoveAll(true);
	m_wndTabOneNoteOuterBottom.ItemRemoveAll(true);
	m_wndTabOneNoteOuterLeft.ItemRemoveAll(true);
	m_wndTabOneNoteOuterRight.ItemRemoveAll(true);
	m_wndTabOneNoteInnerTop.ItemRemoveAll(true);
	m_wndTabOneNoteInnerBottom.ItemRemoveAll(true);
	m_wndTabOneNoteInnerLeft.ItemRemoveAll(true);
	m_wndTabOneNoteInnerRight.ItemRemoveAll(true);

	m_wndTabWhidbeyOuterTop.ItemRemoveAll(true);
	m_wndTabWhidbeyOuterBottom.ItemRemoveAll(true);
	m_wndTabWhidbeyOuterLeft.ItemRemoveAll(true);
	m_wndTabWhidbeyOuterRight.ItemRemoveAll(true);
	m_wndTabWhidbeyInnerTop.ItemRemoveAll(true);
	m_wndTabWhidbeyInnerBottom.ItemRemoveAll(true);
	m_wndTabWhidbeyInnerLeft.ItemRemoveAll(true);
	m_wndTabWhidbeyInnerRight.ItemRemoveAll(true);

	m_wndTabFlatOuterTop.ItemRemoveAll(true);
	m_wndTabFlatOuterBottom.ItemRemoveAll(true);
	m_wndTabFlatOuterLeft.ItemRemoveAll(true);
	m_wndTabFlatOuterRight.ItemRemoveAll(true);
	m_wndTabFlatInnerTop.ItemRemoveAll(true);
	m_wndTabFlatInnerBottom.ItemRemoveAll(true);
	m_wndTabFlatInnerLeft.ItemRemoveAll(true);
	m_wndTabFlatInnerRight.ItemRemoveAll(true);

	m_wndTabButtonsOuterTop.ItemRemoveAll(true);
	m_wndTabButtonsOuterBottom.ItemRemoveAll(true);
	m_wndTabButtonsOuterLeft.ItemRemoveAll(true);
	m_wndTabButtonsOuterRight.ItemRemoveAll(true);
	m_wndTabButtonsInnerTop.ItemRemoveAll(true);
	m_wndTabButtonsInnerBottom.ItemRemoveAll(true);
	m_wndTabButtonsInnerLeft.ItemRemoveAll(true);
	m_wndTabButtonsInnerRight.ItemRemoveAll(true);
}

//////////////////////////////////////////////////////////////////////////

void CPageTabWindow::OnButtonAddTab1() 
{
	_DoAdd1(true);
}

//////////////////////////////////////////////////////////////////////////

void CPageTabWindow::OnButtonAddTab10() 
{
	for( INT i=0; i < 10; i++ ){
		bool bUpdateTabWnd = ( i == 9 ) ? true : false;
		_DoAdd1(bUpdateTabWnd);
	}
}

//////////////////////////////////////////////////////////////////////////

void CPageTabWindow::_DoAdd1( bool bUpdateTabWnd )
{
	ASSERT( m_ImageList.GetSafeHandle()	!= NULL );
	ASSERT( m_nDemoImageListSize > 0 );

	if( ! m_BtnClear.IsWindowEnabled() )
		m_BtnClear.EnableWindow( TRUE );

	// insert 1 item into inner (non-grouped) tab controls
	ASSERT( m_nDemoImageListAddIndexInner <= m_nDemoImageListSize );
	if( m_nDemoImageListAddIndexInner == (m_nDemoImageListSize-1) )
		m_nDemoImageListAddIndexInner = 0;

LPCTSTR strTabItemText = g_ItemsData[m_nDemoImageListAddIndexInner].m_sItemText;
	ASSERT( strTabItemText != NULL );

HICON hIconInner = m_ImageList.ExtractIcon( m_nDemoImageListAddIndexInner );
	ASSERT( hIconInner != NULL );

	m_wndTabInnerTop.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabInnerBottom.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabInnerLeft.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabInnerRight.ItemInsert(
		strTabItemText,
		hIconInner
		);

	m_wndTabOneNoteInnerTop.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabOneNoteInnerBottom.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabOneNoteInnerLeft.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabOneNoteInnerRight.ItemInsert(
		strTabItemText,
		hIconInner
		);

	m_wndTabWhidbeyInnerTop.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabWhidbeyInnerBottom.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabWhidbeyInnerLeft.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabWhidbeyInnerRight.ItemInsert(
		strTabItemText,
		hIconInner
		);

	m_wndTabFlatInnerTop.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabFlatInnerBottom.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabFlatInnerLeft.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabFlatInnerRight.ItemInsert(
		strTabItemText,
		hIconInner
		);

	m_wndTabButtonsInnerTop.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabButtonsInnerBottom.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabButtonsInnerLeft.ItemInsert(
		strTabItemText,
		hIconInner
		);
	m_wndTabButtonsInnerRight.ItemInsert(
		strTabItemText,
		hIconInner
		);

	::DestroyIcon( hIconInner );

	m_nDemoImageListAddIndexInner++;

	// insert 1 group into outer (grouped) tab controls

	ASSERT( m_nDemoImageListAddIndexOuter <= m_nDemoImageListSize );

	if( m_nDemoImageListAddIndexOuter == (m_nDemoImageListSize-1) )
		m_nDemoImageListAddIndexOuter = 0;

	for( INT nAddedToOuter = 0; true ; nAddedToOuter++ )
	{
		bool bGroupStart = 
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_bGroupStart;
		if( nAddedToOuter != 0 && bGroupStart )
			break;
		DWORD dwItemStyle = 0;
		if( bGroupStart )
			dwItemStyle |= __ETWI_GROUP_START | __ETWI_IN_GROUP_ACTIVE;
		HICON hIconOuter = m_ImageList.ExtractIcon( m_nDemoImageListAddIndexOuter );
		ASSERT( hIconOuter != NULL );
		ASSERT( g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText != NULL );

		m_wndTabOuterTop.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabOuterBottom.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabOuterLeft.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabOuterRight.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);

		m_wndTabOneNoteOuterTop.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabOneNoteOuterBottom.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabOneNoteOuterLeft.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabOneNoteOuterRight.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);

		m_wndTabWhidbeyOuterTop.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabWhidbeyOuterBottom.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabWhidbeyOuterLeft.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabWhidbeyOuterRight.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);

		m_wndTabFlatOuterTop.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabFlatOuterBottom.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabFlatOuterLeft.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabFlatOuterRight.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);

		m_wndTabButtonsOuterTop.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabButtonsOuterBottom.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabButtonsOuterLeft.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);
		m_wndTabButtonsOuterRight.ItemInsert(
			g_ItemsData[m_nDemoImageListAddIndexOuter].m_sItemText,
			hIconOuter,
			true,
			dwItemStyle
			);

		::DestroyIcon( hIconOuter );

		if( m_nDemoImageListAddIndexOuter == (m_nDemoImageListSize-1) )
			m_nDemoImageListAddIndexOuter = 0;
		else
			m_nDemoImageListAddIndexOuter++;
	} // for( INT nAddedToOuter = 0; true ; nAddedToOuter++ )

	// update or delay-update all tab controls
	m_wndTabInnerTop.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabInnerBottom.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabInnerLeft.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabInnerRight.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOuterTop.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOuterBottom.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOuterLeft.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOuterRight.UpdateTabWnd( bUpdateTabWnd );

	m_wndTabOneNoteInnerTop.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOneNoteInnerBottom.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOneNoteInnerLeft.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOneNoteInnerRight.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOneNoteOuterTop.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOneNoteOuterBottom.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOneNoteOuterLeft.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabOneNoteOuterRight.UpdateTabWnd( bUpdateTabWnd );

	m_wndTabWhidbeyInnerTop.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabWhidbeyInnerBottom.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabWhidbeyInnerLeft.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabWhidbeyInnerRight.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabWhidbeyOuterTop.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabWhidbeyOuterBottom.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabWhidbeyOuterLeft.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabWhidbeyOuterRight.UpdateTabWnd( bUpdateTabWnd );

	m_wndTabFlatInnerTop.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabFlatInnerBottom.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabFlatInnerLeft.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabFlatInnerRight.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabFlatOuterTop.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabFlatOuterBottom.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabFlatOuterLeft.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabFlatOuterRight.UpdateTabWnd( bUpdateTabWnd );

	m_wndTabButtonsInnerTop.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabButtonsInnerBottom.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabButtonsInnerLeft.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabButtonsInnerRight.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabButtonsOuterTop.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabButtonsOuterBottom.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabButtonsOuterLeft.UpdateTabWnd( bUpdateTabWnd );
	m_wndTabButtonsOuterRight.UpdateTabWnd( bUpdateTabWnd );

	if( m_wndTabInnerTop.ItemGetCount() >= 100 )
	{
		if( m_BtnAdd1.IsWindowEnabled() )
		{
			m_BtnAdd1.EnableWindow( FALSE );
			m_BtnAdd10.EnableWindow( FALSE );
		}
	}
}

#endif // (!defined __EXT_MFC_NO_TAB_CTRL)

