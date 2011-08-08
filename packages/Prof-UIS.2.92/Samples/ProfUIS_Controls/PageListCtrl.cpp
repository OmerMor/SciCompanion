// PageListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageListCtrl.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ( ! defined __EXT_MFC_NO_LIST_VIEW_CTRL )

/////////////////////////////////////////////////////////////////////////////
// CPageListCtrl dialog

CPageListCtrl::CPageListCtrl(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageListCtrl::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageListCtrl)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

void CPageListCtrl::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageListCtrl)
	DDX_Control(pDX, IDC_CHECK_SHOW_HEADER_TOOL_TIPS, m_checkShowToolTipsInHeader);
	DDX_Control(pDX, IDC_CHECK_SHOW_HEADER_ICONS, m_checkShowHeaderIcons);
	DDX_Control(pDX, IDC_CHECK_SHOW_HEADER_BUTTONS, m_checkShowHeaderButtons);
	DDX_Control(pDX, IDC_HYPER_LINK_STATE_SAVE, m_hyperlinkStateSave);
	DDX_Control(pDX, IDC_HYPER_LINK_STATE_LOAD, m_hyperlinkStateLoad);
	DDX_Control(pDX, IDC_CHECK_COLUMN_AUTO_SIZING_BY_HEADER, m_checkColumnAutoSizingByHeader);
	DDX_Control(pDX, IDC_CHECK_COLUMN_AUTO_SIZING_BY_DATA, m_checkColumnAutoSizingByData);
	DDX_Control(pDX, IDC_CHECK_MULTI_COLUMN_SORTING, m_checkSortingByMultipleColumns);
	DDX_Control(pDX, IDC_CHECK_ENABLE_SORTING, m_checkSorting);
	DDX_Control(pDX, IDC_CHECK_ENABLE_COLUMN_DND, m_checkColumnDND);
	DDX_Control(pDX, IDC_COMBO_LIST_TYPE, m_comboListType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MY_LIST_CTRL, m_wndList);
}


BEGIN_MESSAGE_MAP(CPageListCtrl, CPageBase)
	//{{AFX_MSG_MAP(CPageListCtrl)
	ON_CBN_SELENDOK(IDC_COMBO_LIST_TYPE, OnSelendokComboListType)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_ENABLE_SORTING, OnCheckEnableSorting)
	ON_BN_CLICKED(IDC_CHECK_MULTI_COLUMN_SORTING, OnCheckMultiColumnSorting)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_COLUMN_DND, OnCheckEnableColumnDnd)
	ON_BN_CLICKED(IDC_CHECK_COLUMN_AUTO_SIZING_BY_DATA, OnCheckColumnAutoSizingByData)
	ON_BN_CLICKED(IDC_CHECK_COLUMN_AUTO_SIZING_BY_HEADER, OnCheckColumnAutoSizingByHeader)
	ON_BN_CLICKED(IDC_HYPER_LINK_STATE_LOAD, OnHyperLinkStateLoad)
	ON_BN_CLICKED(IDC_HYPER_LINK_STATE_SAVE, OnHyperLinkStateSave)
	ON_BN_CLICKED(IDC_CHECK_SHOW_HEADER_BUTTONS, OnCheckShowHeaderButtons)
	ON_BN_CLICKED(IDC_CHECK_SHOW_HEADER_ICONS, OnCheckShowHeaderIcons)
	ON_BN_CLICKED(IDC_CHECK_SHOW_HEADER_TOOL_TIPS, OnCheckShowHeaderToolTips)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( CExtHeaderCtrl::g_nMsgHeaderButtonClick, OnMsgHeaderButtonClick )
END_MESSAGE_MAP()

void CPageListCtrl::PageBase_PostInit()
{
	ASSERT_VALID( this );
	CPageBase::PageBase_PostInit();
CMainDlg * pMainDlg = (CMainDlg*)GetParent();
INT nInit = 0, nCount = pMainDlg->m_wndList.GetCount();
	VERIFY( m_ImageListNormal.Create( 32, 32, ILC_COLOR24|ILC_MASK, 0, 1 ) );
	VERIFY( m_ImageListSmall. Create( 16, 16, ILC_COLOR24|ILC_MASK, 0, 1 ) );
HINSTANCE hInstResource = ::AfxFindResourceHandle( MAKEINTRESOURCE( IDR_MAINFRAME ), RT_ICON );
	ASSERT( hInstResource != NULL );
	for( nInit = 0; nInit < nCount; nInit++ )
	{
		CExtCmdIcon * pIcon = pMainDlg->m_wndList.GetIconPtr( nInit );
		ASSERT( pIcon != NULL );
		ASSERT( ! pIcon->IsEmpty() );
		CExtBitmap _bmp = pIcon->m_bmpNormal;
		_bmp.Make32();
		ASSERT( ! _bmp.IsEmpty() );
		HICON hIcon = _bmp.CreateHICON( true );
		m_ImageListNormal.Add( hIcon );
		::DestroyIcon( hIcon );
		CExtBitmap::Filter _f( CExtBitmap::Filter::lanczos );
		_bmp.Scale( 16, 16, _f );
		ASSERT( ! _bmp.IsEmpty() );
		hIcon = _bmp.CreateHICON( false );
		m_ImageListSmall.Add( hIcon );
		::DestroyIcon( hIcon );
	}
	m_wndList.SetImageList( &m_ImageListNormal, LVSIL_NORMAL );
	m_wndList.SetImageList( &m_ImageListSmall, LVSIL_SMALL );
INT nColumnCount = m_wndList.GetHeaderCtrl().GetItemCount(), nColumnIndex;
INT nItemCount = 100, nItemIndex;
	for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex ++ )
	{
		CExtSafeString strItemText;
		strItemText.Format( _T("Item %02d"), nItemIndex );
		m_wndList.InsertItem( nItemIndex, strItemText );
		for( nColumnIndex = 0; nColumnIndex < nColumnCount; nColumnIndex ++ )
		{
			int nImageIndex = nItemIndex % nCount;
			CExtSafeString strSubItemText;
			CString strLBText;
			pMainDlg->m_wndList.GetText( nImageIndex, strLBText );
//strSubItemText.Format( _T("%d ---- Item %02d, SubItem %02d"), rand()%2, nItemIndex, nColumnIndex );
			if( nColumnIndex == 0 )
				strSubItemText = LPCTSTR(strLBText);
			else
				strSubItemText.Format( _T("%d"), rand()%5 );
			VERIFY(
				m_wndList.SetItem(
					nItemIndex,
					nColumnIndex, //int nSubItem,
					LVIF_TEXT|LVIF_IMAGE, // UINT nMask,
					strSubItemText, // 0L, // LPCTSTR lpszItem,
					nImageIndex, // int nImage,
					0L, // UINT nState,
					0L, // UINT nStateMask,
					0L // LPARAM lParam
					)
				);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPageListCtrl message handlers

BOOL CPageListCtrl::OnInitDialog() 
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	CPageBase::OnInitDialog();

INT nColumnCount = 5, nColumnIndex;
	for( nColumnIndex = 0; nColumnIndex < nColumnCount; nColumnIndex ++ )
	{
		CExtSafeString strColumnCaptionText;
		strColumnCaptionText.Format( _T("Column %02d"), nColumnIndex );
		m_wndList.InsertColumn( nColumnIndex, strColumnCaptionText, LVCFMT_LEFT, 150 );
	}

	m_hyperlinkStateLoad.m_bUseStdCommandNotification = true;
	m_hyperlinkStateSave.m_bUseStdCommandNotification = true;

	AddAnchor( IDC_MY_LIST_CTRL, __RDA_LT, __RDA_RB );
	AddAnchor( IDC_STATIC_LIST_TYPE, __RDA_RT );
	AddAnchor( IDC_COMBO_LIST_TYPE, __RDA_RT );
	AddAnchor( IDC_CHECK_MULTI_COLUMN_SORTING, __RDA_RT );
	AddAnchor( IDC_CHECK_ENABLE_SORTING, __RDA_RT );
	AddAnchor( IDC_CHECK_ENABLE_COLUMN_DND, __RDA_RT );
	AddAnchor( IDC_STATIC_COLUMN_AUTO_SIZING, __RDA_RT );
	AddAnchor( IDC_CHECK_COLUMN_AUTO_SIZING_BY_DATA, __RDA_RT );
	AddAnchor( IDC_CHECK_COLUMN_AUTO_SIZING_BY_HEADER, __RDA_RT );
	AddAnchor( IDC_CHECK_SHOW_HEADER_BUTTONS, __RDA_RT );
	AddAnchor( IDC_CHECK_SHOW_HEADER_ICONS, __RDA_RT );
	AddAnchor( IDC_CHECK_SHOW_HEADER_TOOL_TIPS, __RDA_RT );
	
	AddAnchor( IDC_STATIC_FRAME_1, __RDA_RT );
	AddAnchor( IDC_STATIC_FRAME_2, __RDA_RT );
	AddAnchor( IDC_STATIC_FRAME_3, __RDA_RT );
	AddAnchor( IDC_HYPER_LINK_STATE_LOAD, __RDA_RB );
	AddAnchor( IDC_HYPER_LINK_STATE_SAVE, __RDA_RB );
	
	m_wndList.StateLoad( pApp->m_pszRegistryKey, pApp->m_pszProfileName, _T("TestExtListCtrl") );
	m_comboListType.SetCurSel( INT( m_wndList.GetStyle() & LVS_TYPEMASK ) );

	OnCheckEnableSorting();
	OnCheckEnableColumnDnd();
	OnCheckColumnAutoSizingByHeader();
	OnCheckColumnAutoSizingByData();
	OnCheckShowHeaderButtons();
	OnCheckShowHeaderToolTips();

	m_bInitComplete = true;
	
	return TRUE;
}

void CPageListCtrl::OnDestroy() 
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	m_wndList.StateSave( pApp->m_pszRegistryKey, pApp->m_pszProfileName, _T("TestExtListCtrl") );
	CExtResizableDialog ::OnDestroy();
}

void CPageListCtrl::OnSelendokComboListType() 
{
INT nSel = m_comboListType.GetCurSel();
	ASSERT( 0 <= nSel && nSel <= 3 );
	m_wndList.ModifyStyle( LVS_TYPEMASK, 0 );
	m_wndList.ModifyStyle( 0, nSel );
}

#endif // ( ! defined __EXT_MFC_NO_LIST_VIEW_CTRL )

void CPageListCtrl::OnCheckEnableSorting()
{
bool bEnableSorting = ( m_checkSorting.GetCheck() != 0 ) ? true : false;
	m_checkSortingByMultipleColumns.EnableWindow( bEnableSorting ? TRUE : FALSE );
bool bMultiColumnSorting = false;	
	if( bEnableSorting )
		bMultiColumnSorting = ( m_checkSortingByMultipleColumns.GetCheck() != 0 ) ? true : false;
	m_wndList.m_bSortEnabled = bEnableSorting;
	m_wndList.m_bSortEnabledMultiple = bMultiColumnSorting;
}

void CPageListCtrl::OnCheckMultiColumnSorting()
{
	OnCheckEnableSorting();
}

void CPageListCtrl::OnCheckEnableColumnDnd()
{
bool bEnableColumnDND = ( m_checkColumnDND.GetCheck() != 0 ) ? true : false;
DWORD dwExtendedListStyle = m_wndList.GetExtendedStyle();
	if( bEnableColumnDND )
		dwExtendedListStyle |= (LVS_EX_HEADERDRAGDROP);
	else
		dwExtendedListStyle &= ~(LVS_EX_HEADERDRAGDROP);
	m_wndList.SetExtendedStyle(  dwExtendedListStyle );
}

void CPageListCtrl::OnCheckColumnAutoSizingByData() 
{
	m_wndList.m_bColumnAutoSizingByData = ( m_checkColumnAutoSizingByData.GetCheck() != 0 ) ? true : false;
}

void CPageListCtrl::OnCheckColumnAutoSizingByHeader() 
{
	m_wndList.m_bColumnAutoSizingByHeader = ( m_checkColumnAutoSizingByHeader.GetCheck() != 0 ) ? true : false;
}

void CPageListCtrl::OnHyperLinkStateLoad() 
{
CString strPathName;

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile(
		NULL,
		CExtShellDialogFile::__EFDT_OPEN_SINGLE
		);
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("List control state files (*.list)|*.list|")
		_T("All files (*.*)|*.*|")
		_T("|")
		);
	dlgShellFile.m_nInitialFileTypeIndex = 0;
	dlgShellFile.m_bFilesMustExist = true;
	dlgShellFile.m_strCustomDialogCaption = _T("Open list control state file");
	if( dlgShellFile.DoModal() != IDOK )
		return;
	ASSERT( dlgShellFile.m_arrRetValNames.GetSize() == 1 );
	strPathName = LPCTSTR(dlgShellFile.m_arrRetValNames[0]);

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog dlgFile(
		TRUE,
		_T("list"),
		NULL,
		OFN_FILEMUSTEXIST
			|OFN_PATHMUSTEXIST
			|OFN_HIDEREADONLY
			|OFN_LONGNAMES
			|OFN_NOCHANGEDIR
			,
		_T("List control state files (*.list)|*.list|")
		_T("All files (*.*)|*.*|")
		_T("|")
		,
		this
		);
	dlgFile.m_ofn.lpstrTitle = _T("Open list control state file");
	if( dlgFile.DoModal() != IDOK )
		return;
	strPathName = dlgFile.GetPathName();
	ASSERT( ! strPathName.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )
	
	try
	{
		CFile _file(
			LPCTSTR(strPathName),
			CFile::modeRead|CFile::typeBinary
			);
		CArchive ar( &_file, CArchive::load );
		m_wndList.StateSerialize( ar );
		return;
	} // try
	catch( CException * pException )
	{
		pException->Delete();
		ASSERT( FALSE );
	} // catch( CException * pException )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )
	::AfxMessageBox(
		_T("Failed to load list control state file."),
		MB_OK|MB_ICONERROR
		);
}

void CPageListCtrl::OnHyperLinkStateSave() 
{
CString strPathName;

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile(
		NULL,
		CExtShellDialogFile::__EFDT_SAVE_AS
		);
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("List control state files (*.list)|*.list|")
		_T("All files (*.*)|*.*|")
		_T("|")
		);
	dlgShellFile.m_nInitialFileTypeIndex = 0;
	dlgShellFile.m_bPathMustExist = true;
	dlgShellFile.m_strCustomDialogCaption = _T("Save list control state file");
	if( dlgShellFile.DoModal() != IDOK )
		return;
	ASSERT( dlgShellFile.m_arrRetValNames.GetSize() == 1 );
	strPathName = LPCTSTR(dlgShellFile.m_arrRetValNames[0]);

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog dlgFile(
		FALSE,
		_T("list"),
		NULL,
		OFN_FILEMUSTEXIST
			|OFN_HIDEREADONLY
			|OFN_LONGNAMES
			|OFN_NOCHANGEDIR
			|OFN_OVERWRITEPROMPT
			,
		_T("List control state files (*.list)|*.list|")
		_T("All files (*.*)|*.*|")
		_T("|")
		,
		this
		);
	dlgFile.m_ofn.lpstrTitle = _T("Save list control state file");
	if( dlgFile.DoModal() != IDOK )
		return;
	strPathName = dlgFile.GetPathName();
	ASSERT( ! strPathName.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

	try
	{
		CFile _file(
			LPCTSTR(strPathName),
			CFile::modeCreate|CFile::modeWrite|CFile::typeBinary
			);
		CArchive ar( &_file, CArchive::store );
		m_wndList.StateSerialize( ar );
		m_wndList.RedrawWindow( NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW|RDW_ALLCHILDREN );
		return;
	} // try
	catch( CException * pException )
	{
		pException->Delete();
		ASSERT( FALSE );
	} // catch( CException * pException )
	catch( ... )
	{
		ASSERT( FALSE );
	} // catch( ... )
	::AfxMessageBox(
		_T("Failed to save list control state file."),
		MB_OK|MB_ICONERROR
		);
}

void CPageListCtrl::OnCheckShowHeaderButtons() 
{
CExtBitmap bmpIconItem, bmpIconButton;
bool bShowHeaderButtons = ( m_checkShowHeaderButtons.GetCheck() != 0 ) ? true : false;
bool bShowHeaderIcons = ( m_checkShowHeaderIcons.GetCheck() != 0 ) ? true : false;
	if( bShowHeaderButtons )
	{
		HICON hIcon = ::AfxGetApp()->LoadIcon( IDI_NOTEPAD );
		VERIFY( bmpIconButton.AssignFromHICON( hIcon ) );
		CExtBitmap::Filter _f( CExtBitmap::Filter::lanczos );
		bmpIconButton.Scale( 14, 14, _f );
		::DestroyIcon( hIcon );
	}
	if( bShowHeaderIcons )
	{
		HICON hIcon = ::AfxGetApp()->LoadIcon( IDR_MAINFRAME );
		VERIFY( bmpIconItem.AssignFromHICON( hIcon ) );
		CExtBitmap::Filter _f( CExtBitmap::Filter::lanczos );
		bmpIconItem.Scale( 14, 14, _f );
		::DestroyIcon( hIcon );
	}
CExtHeaderCtrl & wndHeader = m_wndList.GetHeaderCtrl();
	ASSERT( wndHeader.GetSafeHwnd() != NULL );
INT nColNo, nColCount = INT( wndHeader.GetItemCount() );
	for( nColNo = 0; nColNo < nColCount; nColNo ++ )
	{
		CExtHeaderCtrl::EXTENDED_ITEM_DATA & _eii = wndHeader.ExtendedItemDataGet( nColNo );
		HD_ITEM _item;
		::memset( &_item, 0, sizeof(HD_ITEM) );
		_item.mask = UINT(HDI_TEXT);
		TCHAR strText[ 1024 ];
		_item.cchTextMax = sizeof( strText ) / sizeof( strText[0] );
		_item.pszText = strText;
		wndHeader.GetItem( nColNo, &_item );
		_eii.m_strTipTextItem.Format( _T("Tooltip for \"%s\" header item"), LPCTSTR(strText) );
		if( bShowHeaderButtons )
		{
			_eii.m_iconButton.m_bmpNormal = bmpIconButton;
			_eii.m_iconButton.m_bmpNormal.Scale( 12, 12 );
			_eii.m_strTipTextButton.Format( _T("Button inside \"%s\" header item"), LPCTSTR(strText) );
		}
		else
		{
			_eii.m_iconButton.Empty();
			_eii.m_strTipTextButton.Empty();
		}
		if( bShowHeaderIcons )
			_eii.m_iconItem.m_bmpNormal = bmpIconItem;
		else
			_eii.m_iconItem.m_bmpNormal.Empty();
	}
	m_wndList.RedrawWindow( NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW|RDW_ALLCHILDREN );
}

void CPageListCtrl::OnCheckShowHeaderIcons() 
{
	OnCheckShowHeaderButtons();	
}

void CPageListCtrl::OnCheckShowHeaderToolTips()
{
CExtHeaderCtrl & wndHeader = m_wndList.GetHeaderCtrl();
	ASSERT( wndHeader.GetSafeHwnd() != NULL );
	wndHeader.m_bShowTipsOverHeader = ( m_checkShowToolTipsInHeader.GetCheck() != 0 ) ? true : false;
}

LRESULT CPageListCtrl::OnMsgHeaderButtonClick( WPARAM wParam, LPARAM lParam )
{
	lParam;
CExtHeaderCtrl & wndHeader = m_wndList.GetHeaderCtrl();
	ASSERT( wndHeader.GetSafeHwnd() != NULL );
HD_ITEM _item;
	::memset( &_item, 0, sizeof(HD_ITEM) );
	_item.mask = UINT(HDI_TEXT);
TCHAR strText[ 1024 ];
	_item.cchTextMax = sizeof( strText ) / sizeof( strText[0] );
	_item.pszText = strText;
	wndHeader.GetItem( int(wParam), &_item );
CString str;
	str.Format( _T("Button on \"%s\" header item is clicked"), LPCTSTR(strText) );
	::AfxMessageBox( LPCTSTR(str), MB_OK|MB_ICONINFORMATION );
	return 0L;
}
