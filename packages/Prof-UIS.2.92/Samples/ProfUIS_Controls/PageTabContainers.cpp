// PageTabContainers.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageTabContainers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL && !defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CPageTabContainers dialog

CPageTabContainers::CPageTabContainers(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageTabContainers::IDD, pParent)
	, m_bInitComplete( false )
	, m_wndListBox0( true )
	, m_wndListBox1( true )
	, m_wndEditCtrl0( true )
	, m_wndEditCtrl1( true )
	, m_wndTreeCtrl0( true )
	, m_wndTreeCtrl1( true )
{
	//{{AFX_DATA_INIT(CPageTabContainers)
	m_nShift = 4;
	m_nSize = 16;
	//}}AFX_DATA_INIT
}


void CPageTabContainers::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageTabContainers)
	DDX_Control(pDX, IDC_EDIT_SIZE, m_wndSize);
	DDX_Control(pDX, IDC_EDIT_SHIFT, m_wndShift);
	DDX_Control(pDX, IDC_CHECK_SHOW_HOME, m_CheckShowHome);
	DDX_Control(pDX, IDC_CHECK_SHOW_END, m_CheckShowEnd);
	DDX_Control(pDX, IDC_CHECK_ITEM_DRAGGING, m_chkItemDragging);
	DDX_Control(pDX, IDC_CHECK_SHOW_TAB_LIST, m_chkShowTabList);
	DDX_Control(pDX, IDC_CHECK_ENABLE_TAB_LIST, m_chkEnableTabList);
	DDX_Control(pDX, IDC_STATIC_SIZE, m_StaticSize);
	DDX_Control(pDX, IDC_STATIC_SHIFT, m_StaticShift);
	DDX_Control(pDX, IDC_STATIC_ORIENTATION, m_StaticOrientation);
	DDX_Control(pDX, IDC_SPIN_SIZE, m_SpinSize);
	DDX_Control(pDX, IDC_SPIN_SHIFT, m_SpinShift);
	DDX_Control(pDX, IDC_COMBO_ORIENTATION, m_Orientation);
	DDX_Control(pDX, IDC_CHECK_SHOW_HELP, m_chkShowHelp);
	DDX_Control(pDX, IDC_CHECK_SHOW_CLOSE, m_chkShowClose);
	DDX_Control(pDX, IDC_CHECK_SHOW_BORDERS, m_chkShowBorders);
	DDX_Control(pDX, IDC_CHECK_INVERT_VERT_FONT, m_chkInvertVertFont);
	DDX_Control(pDX, IDC_CHECK_INCLINE_BEFORE, m_chkInclineBefore);
	DDX_Control(pDX, IDC_CHECK_INCLINE_AFTER, m_chkInclineAfter);
	DDX_Control(pDX, IDC_CHECK_FULL_WIDTHS, m_chkFullWidths);
	DDX_Control(pDX, IDC_CHECK_FOCUS_ON_HOVER, m_chkFocusOnHover);
	DDX_Control(pDX, IDC_CHECK_EQUAL_WIDTHS, m_chkEqualWidths);
	DDX_Control(pDX, IDC_CHECK_ENABLE_HELP, m_chkEnableHelp);
	DDX_Control(pDX, IDC_CHECK_ENABLE_CLOSE, m_chkEnableClose);
	DDX_Control(pDX, IDC_CHECK_CENTER_TEXT, m_chkCenterText);
	DDX_Control(pDX, IDC_CHECK_BOLD_SELECTION, m_chkBoldSelection);
	DDX_Control(pDX, IDC_CHECK_AUTOHIDE_SCROLL, m_chkAutoHideScroll);
	DDX_Text(pDX, IDC_EDIT_SHIFT, m_nShift);
	DDV_MinMaxInt(pDX, m_nShift, 0, 25);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_nSize);
	DDV_MinMaxInt(pDX, m_nSize, 13, 40);
	//}}AFX_DATA_MAP
	DDX_Control( pDX, ID_TABCTRL_FLAT, m_wndTabPageContainerFlat );
	DDX_Control( pDX, ID_TABCTRL, m_wndTabPageContainer );
}


BEGIN_MESSAGE_MAP(CPageTabContainers, CPageBase)
	//{{AFX_MSG_MAP(CPageTabContainers)
	ON_CBN_SELENDOK(IDC_COMBO_ORIENTATION, OnSelendokComboOrientation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageTabContainers message handlers

BOOL CPageTabContainers::OnInitDialog() 
{
	CPageBase::OnInitDialog();

	m_wndTabPageContainer.ModifyStyle( 0, WS_TABSTOP );
	m_wndTabPageContainer.ModifyStyleEx( 0, WS_EX_CONTROLPARENT );
m_wndTabPageContainer.CloseOnTabsSet();

	m_wndTabPageContainerFlat.ModifyStyle( 0, WS_TABSTOP );
	m_wndTabPageContainerFlat.ModifyStyleEx( 0, WS_EX_CONTROLPARENT );

	AddAnchor( ID_TABCTRL_FLAT,	__RDA_LT, CSize(100,50) );
	AddAnchor( ID_TABCTRL,		CSize(0,50), __RDA_RB );

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
	
	//////////////////////////////////////////////////////////////////////////
	
	HICON hIconTree = (HICON)::LoadImage( 
		::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_TREE), 
		IMAGE_ICON, 
		16, 16, 0
		);
	HICON hIconEdit = (HICON)::LoadImage( 
		::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_EDIT), 
		IMAGE_ICON, 
		16, 16, 0
		);
	HICON hIconColor = (HICON)::LoadImage( 
		::AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDI_COLOR), 
		IMAGE_ICON, 
		16, 16, 0
		);
	
	if( !m_wndColorCtrl0.Create(
		_T("BUTTON"),
		_T(""),
		WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0),
		&m_wndTabPageContainerFlat,
		UINT(IDC_STATIC)))
	{
		TRACE0("Failed to create m_wndColorCtrl0\n");
		return -1;		// fail to create
	}

	m_wndTabPageDialog0.SetAutoSubclassChildControls();
	if( ! m_wndTabPageDialog0.Create( IDD_DIALOG_FOR_TAB_PAGE, &m_wndTabPageContainerFlat ) )
	{
		TRACE0("Failed to create m_wndTabPageDialog0\n");
		return -1;		// fail to create
	}
	m_wndTabPageDialog0.ModifyStyle( 0, WS_TABSTOP|DS_CONTROL );
	m_wndTabPageDialog0.ModifyStyleEx( 0, WS_EX_CONTROLPARENT );
	m_wndTabPageDialog0.ShowSizeGrip( FALSE );
	m_wndTabPageDialog0.AddAnchor( IDC_EDIT_IN_TAB_PAGE_DIALOG1, __RDA_LT, __RDA_RT );
	m_wndTabPageDialog0.AddAnchor( IDC_EDIT_IN_TAB_PAGE_DIALOG2, __RDA_LT, __RDA_RT );

	InitListBoxCtrl(&m_wndListBox0, &m_wndTabPageContainerFlat);
	InitEditCtrl(&m_wndEditCtrl0, &m_wndTabPageContainerFlat);
	InitTreeCtrl(&m_wndTreeCtrl0, &m_wndTabPageContainerFlat);
	
	VERIFY( m_wndTabPageContainerFlat.PageInsert( &m_wndColorCtrl0, _T("Color Ctrl"), hIconColor ) );
	VERIFY( m_wndTabPageContainerFlat.PageInsert( &m_wndListBox0, _T("List Box") ) );
	VERIFY( m_wndTabPageContainerFlat.PageInsert( &m_wndTreeCtrl0, _T("Tree Ctrl"), hIconTree ) );
	VERIFY( m_wndTabPageContainerFlat.PageInsert( &m_wndEditCtrl0, _T("Edit Ctrl"), hIconEdit ) );
	VERIFY( m_wndTabPageContainerFlat.PageInsert( &m_wndTabPageDialog0, _T("Dialog") ) );
	
	//////////////////////////////////////////////////////////////////////////

	if( !m_wndColorCtrl1.Create(
		_T("BUTTON"),
		_T(""),
		WS_CHILD|WS_VISIBLE,
		CRect(0,0,0,0),
		&m_wndTabPageContainer,
		UINT(IDC_STATIC)))
	{
		TRACE0("Failed to create m_wndColorCtrl1\n");
		return -1;		// fail to create
	}
	
	m_wndTabPageDialog1.SetAutoSubclassChildControls();
	if( ! m_wndTabPageDialog1.Create( IDD_DIALOG_FOR_TAB_PAGE, &m_wndTabPageContainer ) )
	{
		TRACE0("Failed to create m_wndTabPageDialog1\n");
		return -1;		// fail to create
	}
	m_wndTabPageDialog1.ModifyStyle( 0, WS_TABSTOP|DS_CONTROL );
	m_wndTabPageDialog1.ModifyStyleEx( 0, WS_EX_CONTROLPARENT );
	m_wndTabPageDialog1.ShowSizeGrip( FALSE );
	m_wndTabPageDialog1.AddAnchor( IDC_EDIT_IN_TAB_PAGE_DIALOG1, __RDA_LT, __RDA_RT );
	m_wndTabPageDialog1.AddAnchor( IDC_EDIT_IN_TAB_PAGE_DIALOG2, __RDA_LT, __RDA_RT );
	
	InitListBoxCtrl(&m_wndListBox1, &m_wndTabPageContainer);
	InitEditCtrl(&m_wndEditCtrl1, &m_wndTabPageContainer);
	InitTreeCtrl(&m_wndTreeCtrl1, &m_wndTabPageContainer);
	
	VERIFY( m_wndTabPageContainer.PageInsert( &m_wndColorCtrl1, _T("Color Ctrl"), hIconColor ) );
	VERIFY( m_wndTabPageContainer.PageInsert( &m_wndListBox1, _T("List Box") ) );
	VERIFY( m_wndTabPageContainer.PageInsert( &m_wndTreeCtrl1, _T("Tree Ctrl"), hIconTree ) );
	VERIFY( m_wndTabPageContainer.PageInsert( &m_wndEditCtrl1, _T("Edit Ctrl"), hIconEdit ) );
	VERIFY( m_wndTabPageContainer.PageInsert( &m_wndTabPageDialog1, _T("Dialog") ) );
	
	//////////////////////////////////////////////////////////////////////////

	m_wndTabPageContainerFlat.CenterTextSet( true );
	m_wndTabPageContainer.CenterTextSet( true );
	
	m_wndTabPageContainerFlat.SelectionBoldSet( true );
	m_wndTabPageContainer.SelectionBoldSet( true );
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_INCLINE_AFTER, 
		m_wndTabPageContainerFlat.ItemsHasInclineAfterGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_INCLINE_BEFORE, 
		m_wndTabPageContainerFlat.ItemsHasInclineBeforeGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_EQUAL_WIDTHS, 
		m_wndTabPageContainerFlat.EqualWidthsGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_FULL_WIDTHS, 
		m_wndTabPageContainerFlat.FullWidthGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_CENTER_TEXT, 
		m_wndTabPageContainerFlat.CenterTextGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_INVERT_VERT_FONT, 
		m_wndTabPageContainerFlat.InvertVertFontGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_BOLD_SELECTION, 
		m_wndTabPageContainerFlat.SelectionBoldGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_CLOSE, 
		m_wndTabPageContainerFlat.ShowBtnCloseGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	GetDlgItem( IDC_CHECK_ENABLE_CLOSE )->EnableWindow( 
		m_wndTabPageContainerFlat.ShowBtnCloseGet()
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_ENABLE_CLOSE, 
		m_wndTabPageContainerFlat.EnabledBtnCloseGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_HELP, 
		m_wndTabPageContainerFlat.ShowBtnHelpGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	GetDlgItem( IDC_CHECK_ENABLE_HELP )->EnableWindow( 
		m_wndTabPageContainerFlat.ShowBtnHelpGet()
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_ENABLE_HELP, 
		m_wndTabPageContainerFlat.EnabledBtnHelpGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_TAB_LIST, 
		m_wndTabPageContainerFlat.ShowBtnTabListGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	GetDlgItem( IDC_CHECK_ENABLE_TAB_LIST )->EnableWindow( 
		m_wndTabPageContainerFlat.ShowBtnTabListGet()
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_ENABLE_TAB_LIST, 
		m_wndTabPageContainerFlat.EnabledBtnTabListGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_AUTOHIDE_SCROLL, 
		m_wndTabPageContainerFlat.AutoHideScrollGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_FOCUS_ON_HOVER, 
		m_wndTabPageContainerFlat.HoverFocusGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_ITEM_DRAGGING, 
		m_wndTabPageContainerFlat.ItemDraggingGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);
	
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_HOME, 
		m_wndTabPageContainerFlat.ShowBtnScrollHomeGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_END, 
		m_wndTabPageContainerFlat.ShowBtnScrollEndGet()
		? BST_CHECKED
		: BST_UNCHECKED		
		);

	m_Orientation.SelectString( 0, _T("Bottom") );
	
	m_wndTabPageContainerFlat.SetSize( m_nSize );
	m_wndTabPageContainerFlat.SetShift( m_nShift );
	m_SpinShift.SetRange(0,25);
	m_SpinSize.SetRange(13,40);
	
	UpdateData(0);
	
	m_bInitComplete = true;
	
	// sets Show Borders style
	::CheckDlgButton( 
		this->GetSafeHwnd(), 
		IDC_CHECK_SHOW_BORDERS, 
		BST_CHECKED
		);
	SendMessage(
		WM_COMMAND,
		MAKEWPARAM(IDC_CHECK_SHOW_BORDERS,BN_CLICKED),
		(LPARAM)m_hWnd
		);
	
//	m_wndTabPageContainerFlat.GetSafeTabWindow()->ModifyTabWndStyleEx( 0, __ETWS_EX_CLOSE_ON_TABS );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

BOOL CPageTabContainers::InitListBoxCtrl(CListBox *pListBox, CExtTabPageContainerWnd *pTabsContainer)
{
	if( ! pListBox->Create(
			WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|WS_TABSTOP|LBS_NOINTEGRALHEIGHT,
			CRect(0,0,0,0), 
			pTabsContainer,
			UINT(IDC_STATIC)
			)
		)
	{
		TRACE0("Failed to create ListBox\n");
		return FALSE;		// fail to create
	}

	pListBox->SetFont(
		CFont::FromHandle(
		(HFONT)::GetStockObject(DEFAULT_GUI_FONT)
		)
		);

	pListBox->AddString(_T("--------------------Configuration: TabCtrl - Win32 Debug--------------------"));
	pListBox->AddString(_T("Compiling resources..."));
	pListBox->AddString(_T("Compiling..."));
	pListBox->AddString(_T("StdAfx.cpp"));
	pListBox->AddString(_T("Prof-UIS multiple monitor support:"));
	pListBox->AddString(_T("     built-in"));
	pListBox->AddString(_T(" Automatically linking with ProfUIS library"));
	pListBox->AddString(_T("     (Professional User Interface Suite)"));
	pListBox->AddString(_T("Compiling..."));
	pListBox->AddString(_T("ExtTabCtrl.cpp"));
	pListBox->AddString(_T("TabCtrl.cpp"));
	pListBox->AddString(_T("TabCtrlDlg.cpp"));
	pListBox->AddString(_T("FlatTabCtrlPage.cpp"));
	pListBox->AddString(_T("ExtFlatTabCtrl.cpp"));
	pListBox->AddString(_T("Generating Code..."));
	pListBox->AddString(_T("Linking..."));
	pListBox->AddString(_T(""));		
	pListBox->AddString(_T("TabCtrl.exe - 0 error(s), 0 warning(s)"));
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CPageTabContainers::InitEditCtrl(CEdit *pEditCtrl, CExtTabPageContainerWnd *pTabsContainer)
{
	if( ! pEditCtrl->Create(
			WS_CHILD|WS_VISIBLE|WS_TABSTOP
				|WS_VSCROLL
				|ES_MULTILINE|ES_LEFT
				|ES_NOHIDESEL|ES_WANTRETURN
				|ES_AUTOVSCROLL
				,
			CRect(0,0,0,0),
			pTabsContainer,
			UINT(IDC_STATIC)
			)
		)
	{
		TRACE0("Failed to create EditCtrl\n");
		return FALSE;		// fail to create
	}
	pEditCtrl->SetFont(
		CFont::FromHandle(
		(HFONT)::GetStockObject(DEFAULT_GUI_FONT)
		)
		);
	pEditCtrl->SetWindowText( _T("  Type text here...") );
	return TRUE;	
}

//////////////////////////////////////////////////////////////////////////

BOOL CPageTabContainers::InitTreeCtrl(CTreeCtrl *pTreeCtrl, CExtTabPageContainerWnd *pTabsContainer)
{
	if( ! pTreeCtrl->Create(
			WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|WS_TABSTOP
				|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT
				|TVS_INFOTIP|TVS_DISABLEDRAGDROP
				|TVS_SINGLEEXPAND|TVS_SHOWSELALWAYS
				,
			CRect(0,0,0,0),
			pTabsContainer,
			UINT(IDC_STATIC)
			)
		)
	{
		TRACE0("Failed to create TreeCtrl\n");
		return FALSE;		// fail to create
	}
//pTreeCtrl->EnableWindow( FALSE );
	pTreeCtrl->SetFont(
		CFont::FromHandle(
		(HFONT)::GetStockObject(DEFAULT_GUI_FONT)
		)
		);
	for( int nTreeIdx0=0; nTreeIdx0<10; nTreeIdx0++ )
	{
		CString sTreeItemText;
		sTreeItemText.Format( _T("Item 1-%d"), nTreeIdx0+1 );
		HTREEITEM htiAtLevel0 =
			pTreeCtrl->InsertItem(
			LPCTSTR(sTreeItemText)
			);
		ASSERT( htiAtLevel0 != NULL );
		for( int nTreeIdx1=0; nTreeIdx1<5; nTreeIdx1++ )
		{
			sTreeItemText.Format( _T("Item 2-%d"), nTreeIdx1+1 );
			HTREEITEM htiAtLevel1 =
				pTreeCtrl->InsertItem(
				LPCTSTR(sTreeItemText),
				htiAtLevel0
				);
			ASSERT( htiAtLevel1 != NULL );
			for( int nTreeIdx2=0; nTreeIdx2<3; nTreeIdx2++ )
			{
				sTreeItemText.Format( _T("Item 3-%d"), nTreeIdx2+1 );
				HTREEITEM htiAtLevel2 =
					pTreeCtrl->InsertItem(
					LPCTSTR(sTreeItemText),
					htiAtLevel1
					);
				ASSERT( htiAtLevel2 != NULL );
				htiAtLevel2;
			} // for( int nTreeIdx2=0; nTreeIdx2<5; nTreeIdx2++ )
		} // for( int nTreeIdx1=0; nTreeIdx1<5; nTreeIdx1++ )
	} // for( int nTreeIdx0=0; nTreeIdx0<10; nTreeIdx0++ )
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CPageTabContainers::OnSelendokComboOrientation() 
{
	CString sOrientation;
	m_Orientation.GetWindowText(sOrientation);	
	if(sOrientation=="Top"){
		m_wndTabPageContainerFlat.OrientationSet(__ETWS_ORIENT_TOP);
		m_wndTabPageContainer.OrientationSet(__ETWS_ORIENT_TOP);
	}
	if(sOrientation=="Bottom"){
		m_wndTabPageContainerFlat.OrientationSet(__ETWS_ORIENT_BOTTOM);
		m_wndTabPageContainer.OrientationSet(__ETWS_ORIENT_BOTTOM);
	}
	if(sOrientation=="Left"){
		m_wndTabPageContainerFlat.OrientationSet(__ETWS_ORIENT_LEFT);
		m_wndTabPageContainer.OrientationSet(__ETWS_ORIENT_LEFT);
	}
	if(sOrientation=="Right"){
		m_wndTabPageContainerFlat.OrientationSet(__ETWS_ORIENT_RIGHT);
		m_wndTabPageContainer.OrientationSet(__ETWS_ORIENT_RIGHT);
	}
}

//////////////////////////////////////////////////////////////////////////

BOOL CPageTabContainers::OnCmdMsg(UINT nID, INT nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( !m_bInitComplete ){
		return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	
	if( nCode == EN_CHANGE )
	{
		UpdateData();
		switch( nID )
		{
		case IDC_EDIT_SHIFT:
			m_wndTabPageContainerFlat.SetShift( m_nShift );
			break;
		case IDC_EDIT_SIZE:
			m_wndTabPageContainerFlat.SetSize( m_nSize );
			break;
			
		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		return TRUE;
	}
	
	if( nCode == CN_COMMAND )
	{
		bool bDlgButtonChecked = ( IsDlgButtonChecked( nID ) == BST_CHECKED );
		switch( nID )
		{
		case IDC_CHECK_AUTOHIDE_SCROLL:
			m_wndTabPageContainerFlat.AutoHideScrollSet( bDlgButtonChecked );
			m_wndTabPageContainer.AutoHideScrollSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_BOLD_SELECTION:
			m_wndTabPageContainerFlat.SelectionBoldSet( bDlgButtonChecked );
			m_wndTabPageContainer.SelectionBoldSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_CENTER_TEXT:
			m_wndTabPageContainerFlat.CenterTextSet( bDlgButtonChecked );
			m_wndTabPageContainer.CenterTextSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_SHOW_CLOSE:
			GetDlgItem( IDC_CHECK_ENABLE_CLOSE )->EnableWindow( bDlgButtonChecked );
			m_wndTabPageContainerFlat.ShowBtnCloseSet( bDlgButtonChecked );
			m_wndTabPageContainer.ShowBtnCloseSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_ENABLE_CLOSE:
			m_wndTabPageContainerFlat.EnabledBtnCloseSet( bDlgButtonChecked );
			m_wndTabPageContainer.EnabledBtnCloseSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_SHOW_HELP:
			GetDlgItem( IDC_CHECK_ENABLE_HELP )->EnableWindow( bDlgButtonChecked );
			m_wndTabPageContainerFlat.ShowBtnHelpSet( bDlgButtonChecked );
			m_wndTabPageContainer.ShowBtnHelpSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_ENABLE_HELP:
			m_wndTabPageContainerFlat.EnabledBtnHelpSet( bDlgButtonChecked );
			m_wndTabPageContainer.EnabledBtnHelpSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_SHOW_TAB_LIST:
			GetDlgItem( IDC_CHECK_ENABLE_TAB_LIST )->EnableWindow( bDlgButtonChecked );
			m_wndTabPageContainerFlat.ShowBtnTabListSet( bDlgButtonChecked );
			m_wndTabPageContainer.ShowBtnTabListSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_ENABLE_TAB_LIST:
			m_wndTabPageContainerFlat.EnabledBtnTabListSet( bDlgButtonChecked );
			m_wndTabPageContainer.EnabledBtnTabListSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_EQUAL_WIDTHS:
			m_wndTabPageContainerFlat.EqualWidthsSet( bDlgButtonChecked );
			m_wndTabPageContainer.EqualWidthsSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_FOCUS_ON_HOVER:
			m_wndTabPageContainerFlat.HoverFocusSet( bDlgButtonChecked );
			m_wndTabPageContainer.HoverFocusSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_ITEM_DRAGGING:
			m_wndTabPageContainerFlat.ItemDraggingSet( bDlgButtonChecked );
			m_wndTabPageContainer.ItemDraggingSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_SHOW_HOME:
			m_wndTabPageContainerFlat.ShowBtnScrollHomeSet( bDlgButtonChecked );
			m_wndTabPageContainer.ShowBtnScrollHomeSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_SHOW_END:
			m_wndTabPageContainerFlat.ShowBtnScrollEndSet( bDlgButtonChecked );
			m_wndTabPageContainer.ShowBtnScrollEndSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_FULL_WIDTHS:
			m_wndTabPageContainerFlat.FullWidthSet( bDlgButtonChecked );
			m_wndTabPageContainer.FullWidthSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_INVERT_VERT_FONT:
			m_wndTabPageContainerFlat.InvertVertFontSet( bDlgButtonChecked );
			m_wndTabPageContainer.InvertVertFontSet( bDlgButtonChecked );
			break;
		case IDC_CHECK_SHOW_BORDERS:
			{
				DWORD dwAddExBorderStyles = WS_EX_STATICEDGE;
				DWORD dwRemoveExBorderStyles = WS_EX_STATICEDGE;
				bDlgButtonChecked 
					? dwRemoveExBorderStyles = 0
					: dwAddExBorderStyles = 0;
				m_wndTabPageContainerFlat.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				m_wndTabPageContainer.ModifyStyleEx( dwRemoveExBorderStyles, dwAddExBorderStyles );
				UINT nSetWindowPosFlags =
					SWP_NOMOVE|SWP_NOSIZE
					|SWP_NOZORDER|SWP_NOOWNERZORDER
					|SWP_NOACTIVATE
					|SWP_FRAMECHANGED
					;
				m_wndTabPageContainerFlat.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
				m_wndTabPageContainer.SetWindowPos(NULL,0,0,0,0,nSetWindowPosFlags);
			}
			break;
		case IDC_CHECK_INCLINE_AFTER:
			m_wndTabPageContainerFlat.ItemsHasInclineAfterSet( !m_wndTabPageContainerFlat.ItemsHasInclineAfterGet() );
			break;
		case IDC_CHECK_INCLINE_BEFORE:
			m_wndTabPageContainerFlat.ItemsHasInclineBeforeSet( !m_wndTabPageContainerFlat.ItemsHasInclineBeforeGet() );
			break;

		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		return TRUE;
	}
	
	return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

#endif // #if (!defined __EXT_MFC_NO_TAB_PAGECONTAINER_CTRL && !defined __EXT_MFC_NO_TAB_PAGECONTAINER_FLAT_CTRL )
