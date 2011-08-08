// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RibbonBarMDI.h"

#include "MainFrm.h"

#if (!defined __RibbonIcons_H)
	#include "res/RibbonIcons.h"
#endif // (!defined __RibbonIcons_H)

#if (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)
	#if (!defined __BackstageViewResources_H)
		#include "res/BackstageViewResources.h"
	#endif // (!defined __BackstageViewResources_H)
#endif // (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)

#include <Resources/Resource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyUserDefinedRibbonOptionsPage

IMPLEMENT_DYNCREATE( CMyUserDefinedRibbonOptionsPage, CExtRibbonOptionsPage );

CMyUserDefinedRibbonOptionsPage::CMyUserDefinedRibbonOptionsPage()
	: CExtRibbonOptionsPage( IDD_MY_USER_DEFINED_OPTIONS_PAGE )
{
	//{{AFX_DATA_INIT(CMyUserDefinedRibbonOptionsPage)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

CMyUserDefinedRibbonOptionsPage::~CMyUserDefinedRibbonOptionsPage()
{
}

UINT CMyUserDefinedRibbonOptionsPage::RibbonOptionsPage_GetID() const
{
	ASSERT_VALID( this );
	return IDD_MY_USER_DEFINED_OPTIONS_PAGE;
}

bool CMyUserDefinedRibbonOptionsPage::RibbonOptionsPage_InitContent()
{
	ASSERT_VALID( this );
//CExtRibbonOptionsDialog * pDialogRibbonOptions =
//		RibbonOptionsPage_GetContainer();
//	if( pDialogRibbonOptions == NULL )
//		return false;
	return true;
}

void CMyUserDefinedRibbonOptionsPage::RibbonOptionsPage_Apply()
{
	ASSERT_VALID( this );
}

void CMyUserDefinedRibbonOptionsPage::RibbonOptionsPage_Cancel()
{
	ASSERT_VALID( this );
}

void CMyUserDefinedRibbonOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CExtRibbonOptionsPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyUserDefinedRibbonOptionsPage)
	DDX_Control( pDX, IDC_MY_STATIC, m_label );
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP( CMyUserDefinedRibbonOptionsPage, CExtRibbonOptionsPage )
	//{{AFX_MSG_MAP(CMyUserDefinedRibbonOptionsPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMyUserDefinedRibbonOptionsPage::OnInitDialog()
{
	if( ! CExtRibbonOptionsPage::OnInitDialog() )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	m_label.SetBkColor( m_clrPageBkgnd );
	m_label.SetTextColor( true, m_clrPageText );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMyBackstageView

#if (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)

CMyBackstageViewWnd::CMyBackstageViewWnd()
{
}

CMyBackstageViewWnd::~CMyBackstageViewWnd()
{
}

bool CMyBackstageViewWnd::OnBsvInitContent()
{
	_InitBackStageView_Resources();
	_InitBackStageView_Tree();
HTREEITEM htiRoot = RcgtdItemGetRoot();
	ASSERT( htiRoot != NULL );
CString strContent_CSS, strContent_HTML, strContent;
	OnRcgwGenerateContent( strContent_CSS, htiRoot, __BSV_CONTENT_TYPE_CSS );
	OnRcgwGenerateContent( strContent_HTML, htiRoot, __BSV_CONTENT_TYPE_HTML );
	strContent += strContent_CSS; 
	strContent += strContent_HTML;
	RcsTextSet( LPCTSTR(strContent), false );
//	afxDump << strContent; // for debugging only
	return true;
}

void CMyBackstageViewWnd::_InitBackStageView_Resources()
{
INT i;
CExtRichDocObjectBase & _OC = RcsLayoutGet()->GetObjectCountainerLocal(); // CExtRichContentLayout::stat_GetObjectCountainerGlobal();
CExtRichDocObjectImage * pObjPic = NULL;

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_HELP_LOW_RESOLUTION.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_HELP_LOW_RESOLUTION) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_ORANGE_DOT_LOW_RESOLUTION.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_ORANGE_DOT_LOW_RESOLUTION) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_CONTACT_US.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_CONTACT_US) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_OPTIONS_LOW_RESOLUTION.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_OPTIONS_LOW_RESOLUTION) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_CHECK_FOR_UPDATES.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_CHECK_FOR_UPDATES) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_SYSTEM_INFORMATION_TOOL.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_SYSTEM_INFORMATION_TOOL) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_CHANGE_PRODUCT_KEY.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_CHANGE_PRODUCT_KEY) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_SMALL_WARNING_ICON.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_SMALL_WARNING_ICON) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_ACTIVATE_PRODUCT.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_ACTIVATE_PRODUCT) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_PROTECT_DOCUMENT.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_PROTECT_DOCUMENT) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_CHECK_FOR_ISSUES.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_CHECK_FOR_ISSUES) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_MANAGE_VERSIONS.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_MANAGE_VERSIONS) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_CREATE.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_CREATE) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_TOP_LEVEL_CLOSE.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_TOP_LEVEL_CLOSE) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_TOP_LEVEL_EXIT.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_TOP_LEVEL_EXIT) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_TOP_LEVEL_OPEN.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_TOP_LEVEL_OPEN) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_TOP_LEVEL_OPTIONS.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_TOP_LEVEL_OPTIONS) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_TOP_LEVEL_SAVE.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_TOP_LEVEL_SAVE) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_TOP_LEVEL_SAVE_AS.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_TOP_LEVEL_SAVE_AS) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	for( i = 1; i <= 14; i ++ )
	{
		CString strPicName;
		strPicName.Format( _T("ID_NEW_DOC_BASIC_%d.bmp"), i );
		pObjPic = new CExtRichDocObjectImage( LPCTSTR(strPicName) );
		VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE((ID_NEW_DOC_BASIC_1-1+i)) ) );
		VERIFY( _OC.ObjectAdd( pObjPic ) );
	}
	for( i = 1; i <= 29; i ++ )
	{
		CString strPicName;
		strPicName.Format( _T("ID_NEW_DOC_ADDITIONAL_%d.bmp"), i );
		pObjPic = new CExtRichDocObjectImage( LPCTSTR(strPicName) );
		VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE((ID_NEW_DOC_ADDITIONAL_1-1+i)) ) );
		VERIFY( _OC.ObjectAdd( pObjPic ) );
	}

	pObjPic = new CExtRichDocObjectImage( _T("image_ID_APP_SCREEN_SHOT_FOR_BACKSTAGE_VIEW.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_APP_SCREEN_SHOT_FOR_BACKSTAGE_VIEW) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_SEND_USING_E_MAIL.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_SEND_USING_E_MAIL) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_SEND_AS_INTERNET_FAX.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_SEND_AS_INTERNET_FAX) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_CHANGE_FILE_TYPE.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_CHANGE_FILE_TYPE) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_CREATE_PDF_XPS_DOCUMENT.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_CREATE_PDF_XPS_DOCUMENT) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_SMALL_ORANGE_DOT.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_SMALL_ORANGE_DOT) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_PUBLISH_AS_BLOG_ENTRY.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_PUBLISH_AS_BLOG_ENTRY) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_SEND_AS_ATTACHMENT.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_SEND_AS_ATTACHMENT) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_SEND_AS_PDF.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_SEND_AS_PDF) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_SEND_AS_XPS.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_SEND_AS_XPS) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_INTERNER_FAX.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_INTERNER_FAX) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_CREATE_A_PDF_XPS.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_CREATE_A_PDF_XPS) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("icon_ID_BUTTON_PUBLISH_TO_BLOG.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_PUBLISH_TO_BLOG) ) );
	VERIFY( _OC.ObjectAdd( pObjPic ) );
}

void CMyBackstageViewWnd::_InitBackStageView_Tree()
{
INT i;
HTREEITEM htiDirectContent = NULL; htiDirectContent;
CExtRichGenItemData * pData_DirectContent = NULL; pData_DirectContent;
HTREEITEM htiSeparator = NULL; htiSeparator;
CExtRichGenItemData * pData_Separator = NULL; pData_Separator;
HTREEITEM htiCaption = NULL; htiCaption;
CExtRichGenItemData * pData_Caption = NULL; pData_Caption;
HTREEITEM htiGroup = NULL; htiGroup;
CExtRichGenItemData * pData_Group = NULL; pData_Group;
HTREEITEM htiTable = NULL; htiTable;
CExtRichGenItemData * pData_Table = NULL; pData_Table;
HTREEITEM htiTableRow = NULL; htiTableRow;
CExtRichGenItemData * pData_TableRow = NULL; pData_TableRow;
HTREEITEM htiTableCell = NULL; htiTableCell;
CExtRichGenItemData * pData_TableCell = NULL; pData_TableCell;
HTREEITEM htiScrollContainer = NULL; htiScrollContainer;
CExtRichGenItemData * pData_ScrollContainer = NULL; pData_ScrollContainer;

HTREEITEM htiRoot = RcgtdItemGetRoot();
	ASSERT( htiRoot != NULL );

HTREEITEM htiCommand_Save = RcgtdItemInsert( htiRoot );
	ASSERT( htiCommand_Save != NULL );
CExtRichGenItemData * pData_Save = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_Save );
	ASSERT( pData_Save != NULL );
	pData_Save->m_nCmdID = ID_FILE_SAVE;
	pData_Save->m_nItemType = __BSVIT_COMMAND_BUTTON_TOP_LEVEL;
	pData_Save->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_TOP_LEVEL_SAVE.bmp") );
	pData_Save->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Save") );
	RcgtdItemKeyTipSet( htiCommand_Save, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("S") ) ), false );

HTREEITEM htiCommand_SaveAs = RcgtdItemInsert( htiRoot );
	ASSERT( htiCommand_SaveAs != NULL );
CExtRichGenItemData * pData_SaveAs = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_SaveAs );
	ASSERT( pData_SaveAs != NULL );
	pData_SaveAs->m_nCmdID = ID_FILE_SAVE_AS;
	pData_SaveAs->m_nItemType = __BSVIT_COMMAND_BUTTON_TOP_LEVEL;
	pData_SaveAs->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_TOP_LEVEL_SAVE_AS.bmp") );
	pData_SaveAs->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Save As") );
	RcgtdItemKeyTipSet( htiCommand_SaveAs, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("A") ) ), false );

HTREEITEM htiCommand_Open = RcgtdItemInsert( htiRoot );
	ASSERT( htiCommand_Open != NULL );
CExtRichGenItemData * pData_Open = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_Open );
	ASSERT( pData_Open != NULL );
	pData_Open->m_nCmdID = ID_FB_OPEN;
	pData_Open->m_nItemType = __BSVIT_COMMAND_BUTTON_TOP_LEVEL;
	pData_Open->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_TOP_LEVEL_OPEN.bmp") );
	pData_Open->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Open") );
	RcgtdItemKeyTipSet( htiCommand_Open, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("O") ) ), false );

HTREEITEM htiCommand_Close = RcgtdItemInsert( htiRoot );
	ASSERT( htiCommand_Close != NULL );
CExtRichGenItemData * pData_Close = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_Close );
	ASSERT( pData_Close != NULL );
	pData_Close->m_nCmdID = ID_FILE_CLOSE;
	pData_Close->m_nItemType = __BSVIT_COMMAND_BUTTON_TOP_LEVEL;
	pData_Close->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_TOP_LEVEL_CLOSE.bmp") );
	pData_Close->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Close") );
	RcgtdItemKeyTipSet( htiCommand_Close, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("C") ) ), false );

HTREEITEM htiTabItem_Info = RcgtdItemInsert( htiRoot );
	ASSERT( htiTabItem_Info != NULL );
CExtRichGenItemData * pData_Info = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_Info );
	ASSERT( pData_Info != NULL );
	pData_Info->m_nCmdID = 39000;
	pData_Info->m_nItemType = __BSVIT_TAB_ITEM;
	pData_Info->m_strHtmlInner = _T("Info");
	RcgtdItemKeyTipSet( htiTabItem_Info, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("E") ) ), false );

			HTREEITEM htiTabPane_Info = RcgtdItemInsert( htiTabItem_Info );
			ASSERT( htiTabPane_Info != NULL );
			CExtRichGenItemData * pData_TabPaneInfo = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Info );
			pData_TabPaneInfo->m_nItemType = __BSVIT_TAB_PANE;
			pData_TabPaneInfo->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" ui-dock:left; width:68em; \" ") );

			//	HTREEITEM htiTabPane_Info_Left = htiTabPane_Info;
				HTREEITEM htiTabPane_Info_Left = RcgtdItemInsert( htiTabPane_Info );
				ASSERT( htiTabPane_Info_Left != NULL );
				CExtRichGenItemData * pData_TabPane_Info_Left = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Info_Left );
				pData_TabPane_Info_Left->m_nItemType = __BSVIT_LEFT_PANE_LIGHT;
				pData_TabPane_Info_Left->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" ui-dock:left; width:42em; \" ") );

					htiScrollContainer = RcgtdItemInsert( htiTabPane_Info_Left );
					ASSERT( htiScrollContainer != NULL );
					pData_ScrollContainer = (CExtRichGenItemData*)RcgtdItemDataGet( htiScrollContainer );
					ASSERT( pData_ScrollContainer != NULL );
					pData_ScrollContainer->m_nItemType = __BSVIT_SCROLL_CONTAINER_VERT;
					pData_ScrollContainer->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-top:1em; margin-bottom:1em; \" ") );

						htiGroup = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiGroup != NULL );
						pData_Group = (CExtRichGenItemData*)RcgtdItemDataGet( htiGroup );
						ASSERT( pData_Group != NULL );
						pData_Group->m_nItemType = __BSVIT_ELEMENT_GROUP_ATTENTION_HIGH;

							htiTable = RcgtdItemInsert( htiGroup );
							ASSERT( htiTable != NULL );
							pData_Table = (CExtRichGenItemData*)RcgtdItemDataGet( htiTable );
							ASSERT( pData_Table != NULL );
							pData_Table->m_nItemType = __BSVIT_TABLE;
						//	pData_Table->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" border-spacing:0.3em; \" ") );

								htiTableRow = RcgtdItemInsert( htiTable );
								ASSERT( htiTableRow != NULL );
								pData_TableRow = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableRow );
								ASSERT( pData_TableRow != NULL );
								pData_TableRow->m_nItemType = __BSVIT_TABLE_ROW;

									htiTableCell = RcgtdItemInsert( htiTableRow );
									ASSERT( htiTableCell != NULL );
									pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
									ASSERT( pData_TableCell != NULL );
									pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

										HTREEITEM htiCommand_ActivateProduct = RcgtdItemInsert( htiTableCell );
										ASSERT( htiCommand_ActivateProduct != NULL );
										CExtRichGenItemData * pData_ActivateProduct = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_ActivateProduct );
										ASSERT( pData_ActivateProduct != NULL );
										pData_ActivateProduct->m_nCmdID = 39100;
										pData_ActivateProduct->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE;
										pData_ActivateProduct->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_ACTIVATE_PRODUCT.bmp") );
										pData_ActivateProduct->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Activate<br>Product") );
										RcgtdItemKeyTipSet( htiCommand_ActivateProduct, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("Y1") ) ), false );

									htiTableCell = RcgtdItemInsert( htiTableRow );
									ASSERT( htiTableCell != NULL );
									pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
									ASSERT( pData_TableCell != NULL );
									pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

										htiCaption = RcgtdItemInsert( htiTableCell );
										ASSERT( htiCaption != NULL );
										pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
										ASSERT( pData_Caption != NULL );
										pData_Caption->m_nItemType = __BSVIT_CAPTION_ATTENTION_HIGH;
										pData_Caption->m_strHtmlInner = _T("Product Activation Required");
										pData_Caption->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-left:1em; margin-top:0em; \" ") );

										htiDirectContent = RcgtdItemInsert( htiTableCell );
										ASSERT( htiDirectContent != NULL );
										pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
										ASSERT( pData_DirectContent != NULL );
										pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
										pData_DirectContent->m_strHtmlInner =
											_T("<p width=\"350\" style=\" margin-left:1em; \" >Application (additional information)")
											_T(" requires a valid product license. Activate your product key now to obtain")
											_T(" a valid product license.</p>\r\n")
											;

						htiSeparator = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiSeparator != NULL );
						pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
						ASSERT( pData_Separator != NULL );
						pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

							htiGroup = RcgtdItemInsert( htiScrollContainer );
							ASSERT( htiGroup != NULL );
							pData_Group = (CExtRichGenItemData*)RcgtdItemDataGet( htiGroup );
							ASSERT( pData_Group != NULL );
							pData_Group->m_nItemType = __BSVIT_ELEMENT_GROUP;

								htiTable = RcgtdItemInsert( htiGroup );
								ASSERT( htiTable != NULL );
								pData_Table = (CExtRichGenItemData*)RcgtdItemDataGet( htiTable );
								ASSERT( pData_Table != NULL );
								pData_Table->m_nItemType = __BSVIT_TABLE;
							//	pData_Table->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" border-spacing:0.3em; \" ") );

									htiTableRow = RcgtdItemInsert( htiTable );
									ASSERT( htiTableRow != NULL );
									pData_TableRow = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableRow );
									ASSERT( pData_TableRow != NULL );
									pData_TableRow->m_nItemType = __BSVIT_TABLE_ROW;

										htiTableCell = RcgtdItemInsert( htiTableRow );
										ASSERT( htiTableCell != NULL );
										pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
										ASSERT( pData_TableCell != NULL );
										pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

											HTREEITEM htiCommand_Document = RcgtdItemInsert( htiTableCell );
											ASSERT( htiCommand_Document != NULL );
											CExtRichGenItemData * pData_Document = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_Document );
											ASSERT( pData_Document != NULL );
											pData_Document->m_nCmdID = 39101;
											pData_Document->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE_DD;
											pData_Document->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_PROTECT_DOCUMENT.bmp") );
											pData_Document->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Protect<br>Document") );
											RcgtdItemKeyTipSet( htiCommand_Document, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("P") ) ), false );

										htiTableCell = RcgtdItemInsert( htiTableRow );
										ASSERT( htiTableCell != NULL );
										pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
										ASSERT( pData_TableCell != NULL );
										pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

											htiCaption = RcgtdItemInsert( htiTableCell );
											ASSERT( htiCaption != NULL );
											pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
											ASSERT( pData_Caption != NULL );
											pData_Caption->m_nItemType = __BSVIT_CAPTION;
											pData_Caption->m_strHtmlInner = _T("Permissions");
											pData_Caption->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-left:1em; margin-top:0em; \" ") );

											htiDirectContent = RcgtdItemInsert( htiTableCell );
											ASSERT( htiDirectContent != NULL );
											pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
											ASSERT( pData_DirectContent != NULL );
											pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
											pData_DirectContent->m_strHtmlInner =
												_T("<p width=\"200\" style=\" margin-left:1em; \" >Anyone")
												_T(" can open, copy, and change any part of this document.</p>\r\n")
												;

						htiSeparator = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiSeparator != NULL );
						pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
						ASSERT( pData_Separator != NULL );
						pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

							htiGroup = RcgtdItemInsert( htiScrollContainer );
							ASSERT( htiGroup != NULL );
							pData_Group = (CExtRichGenItemData*)RcgtdItemDataGet( htiGroup );
							ASSERT( pData_Group != NULL );
							pData_Group->m_nItemType = __BSVIT_ELEMENT_GROUP;

								htiTable = RcgtdItemInsert( htiGroup );
								ASSERT( htiTable != NULL );
								pData_Table = (CExtRichGenItemData*)RcgtdItemDataGet( htiTable );
								ASSERT( pData_Table != NULL );
								pData_Table->m_nItemType = __BSVIT_TABLE;
							//	pData_Table->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" border-spacing:0.3em; \" ") );

									htiTableRow = RcgtdItemInsert( htiTable );
									ASSERT( htiTableRow != NULL );
									pData_TableRow = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableRow );
									ASSERT( pData_TableRow != NULL );
									pData_TableRow->m_nItemType = __BSVIT_TABLE_ROW;

										htiTableCell = RcgtdItemInsert( htiTableRow );
										ASSERT( htiTableCell != NULL );
										pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
										ASSERT( pData_TableCell != NULL );
										pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

											HTREEITEM htiCommand_CheckForIssues = RcgtdItemInsert( htiTableCell );
											ASSERT( htiCommand_CheckForIssues != NULL );
											CExtRichGenItemData * pData_CheckForIssues = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_CheckForIssues );
											ASSERT( pData_CheckForIssues != NULL );
											pData_CheckForIssues->m_nCmdID = 39102;
											pData_CheckForIssues->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE_DD;
											pData_CheckForIssues->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_CHECK_FOR_ISSUES.bmp") );
											pData_CheckForIssues->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Check for<br>Issues") );
											RcgtdItemKeyTipSet( htiCommand_CheckForIssues, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("I") ) ), false );

										htiTableCell = RcgtdItemInsert( htiTableRow );
										ASSERT( htiTableCell != NULL );
										pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
										ASSERT( pData_TableCell != NULL );
										pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

											htiCaption = RcgtdItemInsert( htiTableCell );
											ASSERT( htiCaption != NULL );
											pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
											ASSERT( pData_Caption != NULL );
											pData_Caption->m_nItemType = __BSVIT_CAPTION;
											pData_Caption->m_strHtmlInner = _T("Prepare for Sharing");
											pData_Caption->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-left:1em; margin-top:0em; \" ") );

											htiDirectContent = RcgtdItemInsert( htiTableCell );
											ASSERT( htiDirectContent != NULL );
											pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
											ASSERT( pData_DirectContent != NULL );
											pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
											pData_DirectContent->m_strHtmlInner =
												_T("<p width=\"200\" style=\" margin-left:1em; \" >Before sharing this file be aver it contains:</p>\r\n")
												_T("<ul style=\" list-style-type:square; \" >\r\n")
												_T("<li>Document properties, template name and author's name.</li>\r\n")
												_T("<li>Custom XML data.</li>\r\n")
												_T("</ul>\r\n")
												;

						htiSeparator = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiSeparator != NULL );
						pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
						ASSERT( pData_Separator != NULL );
						pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

							htiGroup = RcgtdItemInsert( htiScrollContainer );
							ASSERT( htiGroup != NULL );
							pData_Group = (CExtRichGenItemData*)RcgtdItemDataGet( htiGroup );
							ASSERT( pData_Group != NULL );
							pData_Group->m_nItemType = __BSVIT_ELEMENT_GROUP;

								htiTable = RcgtdItemInsert( htiGroup );
								ASSERT( htiTable != NULL );
								pData_Table = (CExtRichGenItemData*)RcgtdItemDataGet( htiTable );
								ASSERT( pData_Table != NULL );
								pData_Table->m_nItemType = __BSVIT_TABLE;
							//	pData_Table->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" border-spacing:0.3em; \" ") );

									htiTableRow = RcgtdItemInsert( htiTable );
									ASSERT( htiTableRow != NULL );
									pData_TableRow = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableRow );
									ASSERT( pData_TableRow != NULL );
									pData_TableRow->m_nItemType = __BSVIT_TABLE_ROW;

										htiTableCell = RcgtdItemInsert( htiTableRow );
										ASSERT( htiTableCell != NULL );
										pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
										ASSERT( pData_TableCell != NULL );
										pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

											HTREEITEM htiCommand_ManageVersions = RcgtdItemInsert( htiTableCell );
											ASSERT( htiCommand_ManageVersions != NULL );
											CExtRichGenItemData * pData_ManageVersions = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_ManageVersions );
											ASSERT( pData_ManageVersions != NULL );
											pData_ManageVersions->m_nCmdID = 39103;
											pData_ManageVersions->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE_DD;
											pData_ManageVersions->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_MANAGE_VERSIONS.bmp") );
											pData_ManageVersions->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Manage<br>Versions") );
											RcgtdItemKeyTipSet( htiCommand_ManageVersions, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("R") ) ), false );

										htiTableCell = RcgtdItemInsert( htiTableRow );
										ASSERT( htiTableCell != NULL );
										pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
										ASSERT( pData_TableCell != NULL );
										pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

											htiCaption = RcgtdItemInsert( htiTableCell );
											ASSERT( htiCaption != NULL );
											pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
											ASSERT( pData_Caption != NULL );
											pData_Caption->m_nItemType = __BSVIT_CAPTION;
											pData_Caption->m_strHtmlInner = _T("Versions");
											pData_Caption->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-left:1em; margin-top:0em; \" ") );

											htiDirectContent = RcgtdItemInsert( htiTableCell );
											ASSERT( htiDirectContent != NULL );
											pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
											ASSERT( pData_DirectContent != NULL );
											pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
											pData_DirectContent->m_strHtmlInner =
												_T("<p width=\"200\" style=\" margin-left:1em; \" >You can open or delete")
												_T(" a version file.</p>\r\n")
												_T("<ul style=\" list-style-type:square; \" >\r\n")
												_T("<li>There are no previous versions of this file.</li>\r\n")
												_T("</ul>\r\n")
												;

					htiDirectContent = RcgtdItemInsert( htiScrollContainer );
					ASSERT( htiDirectContent != NULL );
					pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
					ASSERT( pData_DirectContent != NULL );
					pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
					pData_DirectContent->m_strHtmlInner = _T("<p style=\" display:inline-block; height:3em; width:100%; \" ></p>");

				//	HTREEITEM htiTabPane_Info_Right = htiTabPane_Info;
				HTREEITEM htiTabPane_Info_Right = RcgtdItemInsert( htiTabPane_Info );
				ASSERT( htiTabPane_Info_Right != NULL );
				CExtRichGenItemData * pData_TabPane_Info_Right = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Info_Right );
				pData_TabPane_Info_Right->m_nItemType = __BSVIT_RIGHT_PANE_DARK;

					htiDirectContent = RcgtdItemInsert( htiTabPane_Info_Right );
					ASSERT( htiDirectContent != NULL );
					pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
					ASSERT( pData_DirectContent != NULL );
					pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
					pData_DirectContent->m_strHtmlInner =
						_T("<p style=\" margin-top:1em; margin-bottom:1em; text-align:center; font-style:italic; color:grey; \" ><small>Application screenshot should be here:</small><br/><img src=\"image_ID_APP_SCREEN_SHOT_FOR_BACKSTAGE_VIEW.bmp\"/></p>\r\n")
						;

					HTREEITEM htiCommand_DocumentProperties = RcgtdItemInsert( htiTabPane_Info_Right );
					ASSERT( htiCommand_DocumentProperties != NULL );
					CExtRichGenItemData * pDataDocumentProperties = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_DocumentProperties );
					ASSERT( pDataDocumentProperties != NULL );
					pDataDocumentProperties->m_nCmdID = 39104;
					pDataDocumentProperties->m_nItemType = __BSVIT_COMMAND_BUTTON_WIDE_FLAT;
//					pDataDocumentProperties->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Document Properties") );
					pDataDocumentProperties->m_strHtmlInner = _T("Document Properties");
					pDataDocumentProperties->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" display:inline-block; max-width:15em; margin:0px; text-align:center; verical-align:middle; \" ") );
					RcgtdItemKeyTipSet( htiCommand_DocumentProperties, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("QS") ) ), false );

					htiDirectContent = RcgtdItemInsert( htiTabPane_Info_Right );
					ASSERT( htiDirectContent != NULL );
					pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
					ASSERT( pData_DirectContent != NULL );
					pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
					pData_DirectContent->m_strHtmlInner =
						_T("<table style=\" width:100%; margin:0em 2em 0em 2em; \" >\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Size</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:grey; \" >Not saved yet</td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Pages</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:grey; \" >1</td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Words</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:grey; \" >0</td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Total Editing Time</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); \" >1 minute</td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Title</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:grey; \" ><i>no title</i></td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Tags</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:grey; \" ><i>no tags</i></td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Comments</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:grey; \" ><i>no comments</i></td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); \" colspan=2 >Related Dates:</td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Last Modified</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:grey; \" >Never</td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Creted</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); \" >Today 9:45AM</td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Last Printed</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:grey; \" >Never</td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); \" colspan=2 >Related People:</td>\r\n")
						_T("    </tr>\r\n")
						_T("    <tr>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:rgb(84,84,84); margin-left:1em; \" >Author</td>\r\n")
						_T("        <td style=\" margin-top:0.5em; color:grey; \" ><i>no author</i></td>\r\n")
						_T("    </tr>\r\n")
						_T("</table>\r\n")
						;

HTREEITEM htiTabItem_Recent = RcgtdItemInsert( htiRoot );
	ASSERT( htiTabItem_Recent != NULL );
CExtRichGenItemData * pData_Recent = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_Recent );
	ASSERT( pData_Recent != NULL );
	pData_Recent->m_nCmdID = 39001;
	pData_Recent->m_nItemType = __BSVIT_TAB_ITEM;
	pData_Recent->m_strHtmlInner = _T("Recent");
	RcgtdItemKeyTipSet( htiTabItem_Recent, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("R") ) ), false );

			HTREEITEM htiTabPane_Recent = RcgtdItemInsert( htiTabItem_Recent );
			ASSERT( htiTabPane_Recent != NULL );
			CExtRichGenItemData * pData_TabPaneRecent = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Recent );
			pData_TabPaneRecent->m_nItemType = __BSVIT_TAB_PANE;
			pData_TabPaneRecent->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" ui-dock:left; width:36em; height:90%; \" ") );

					htiCaption = RcgtdItemInsert( htiTabPane_Recent );
					ASSERT( htiCaption != NULL );
					pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
					ASSERT( pData_Caption != NULL );
					pData_Caption->m_nItemType = __BSVIT_CAPTION;
					pData_Caption->m_strHtmlInner = _T("Recent Documents");

					htiSeparator = RcgtdItemInsert( htiTabPane_Recent );
					ASSERT( htiSeparator != NULL );
					pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
					ASSERT( pData_Separator != NULL );
					pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

					htiScrollContainer = RcgtdItemInsert( htiTabPane_Recent );
					ASSERT( htiScrollContainer != NULL );
					pData_ScrollContainer = (CExtRichGenItemData*)RcgtdItemDataGet( htiScrollContainer );
					ASSERT( pData_ScrollContainer != NULL );
					pData_ScrollContainer->m_nItemType = __BSVIT_SCROLL_CONTAINER_VERT;
					pData_ScrollContainer->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-top:1em; margin-bottom:1em; \" ") );
							static LPCTSTR g_arrMruTemplateKeyTips[] =
							{
								_T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"),
							//	_T("Y1"), _T("Y2"), _T("Y3"), _T("Y4"), _T("Y5"), _T("Y6"), _T("Y7"), _T("Y8"), _T("Y9"),
							//	_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("I"),_T("J"),_T("K"),_T("L"),_T("M"),
							//	_T("N"),_T("O"),_T("P"),_T("Q"),_T("R"),_T("S"),_T("T"),_T("U"),_T("V"),_T("W"),_T("X"),_T("Y"),_T("Z"),
								_T("YA"),_T("YB"),_T("YC"),_T("YD"),_T("YE"),_T("YF"),_T("YG"),_T("YH"),_T("YI"),_T("YJ"),_T("YK"),_T("YL"),_T("YM"),
								_T("YN"),_T("YO"),_T("YP"),_T("YQ"),_T("YR"),_T("YS"),_T("YT"),_T("YU"),_T("YV"),_T("YW"),_T("YX"),_T("YY"),_T("YZ"),
								_T("ZA"),_T("ZB"),_T("ZC"),_T("ZD"),_T("ZE"),_T("ZF"),_T("ZG"),_T("ZH"),_T("ZI"),_T("ZJ"),_T("ZK"),_T("ZL"),_T("ZM"),
								_T("ZN"),_T("ZO"),_T("ZP"),_T("ZQ"),_T("ZR"),_T("ZS"),_T("ZT"),_T("ZU"),_T("ZV"),_T("ZW"),_T("ZX"),_T("ZY"),_T("ZZ"),
							//	_T(""),
							};
							INT nMruTemplateKeyTipIndex = 0, nMruTemplateKeyTipCount = sizeof(g_arrMruTemplateKeyTips) / sizeof(g_arrMruTemplateKeyTips[0]);
							for( int nMruFileIndex = 0; nMruFileIndex < 10; nMruFileIndex ++ )
							{
								HTREEITEM htiMruItem = RcgtdItemInsert( htiScrollContainer );
								ASSERT( htiMruItem != NULL );
								CExtRichGenItemData * pData_MruItem = (CExtRichGenItemData*)RcgtdItemDataGet( htiMruItem );
								ASSERT( pData_MruItem != NULL );
								pData_MruItem->m_nCmdID = 39200 + nMruFileIndex;
								pData_MruItem->m_nItemType = __BSVIT_MRU_FILE_ITEM;
								CString strCaption, strDescription;
								strCaption.Format( _T("Recent File %d"), nMruFileIndex + 1 );
								strDescription.Format( _T("c:\\Users\\User Name\\My Documents\\\\Folder%d\\File%d.dat"), nMruFileIndex + 1, nMruFileIndex + 1 );
								pData_MruItem->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_MANAGE_VERSIONS.bmp") );
								pData_MruItem->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, LPCTSTR(strCaption) );
								pData_MruItem->ParmAppend( __RCGI_PARM_FLAT_COMMAND_BUTTON_TEXT_AT_RIGHT_DESCRIPTION, LPCTSTR(strDescription) );
								if( nMruTemplateKeyTipIndex < nMruTemplateKeyTipCount )
								{
									RcgtdItemKeyTipSet(
										htiMruItem,
										new CExtCustomizeCmdKeyTip(
											__EXT_MFC_SAFE_LPCTSTR( g_arrMruTemplateKeyTips[nMruTemplateKeyTipIndex] )
											),
										false
										);
									nMruTemplateKeyTipIndex ++;
								} // if( nMruTemplateKeyTipIndex < nMruTemplateKeyTipCount )
							} // for( int nMruFileIndex = 0; nMruFileIndex < 10; nMruFileIndex ++ )

//					htiSeparator = RcgtdItemInsert( htiTabPane_Recent );
//					ASSERT( htiSeparator != NULL );
//					pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
//					ASSERT( pData_Separator != NULL );
//					pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

HTREEITEM htiTabItem_New = RcgtdItemInsert( htiRoot );
	ASSERT( htiTabItem_New != NULL );
CExtRichGenItemData * pData_New = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_New );
	ASSERT( pData_New != NULL );
	pData_New->m_nCmdID = 39002;
	pData_New->m_nItemType = __BSVIT_TAB_ITEM;
	pData_New->m_strHtmlInner = _T("New");
RcgwTreeItemSelectionDelay( htiTabItem_New ); // select this tab-item initially
RcgwTreeItemFocusDelay( htiTabItem_New ); // focus this tab-item initially
	RcgtdItemKeyTipSet( htiTabItem_New, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("N") ) ), false );

			HTREEITEM htiTabPane_New = RcgtdItemInsert( htiTabItem_New );
			ASSERT( htiTabPane_New != NULL );
			CExtRichGenItemData * pData_TabPaneNew = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_New );
			pData_TabPaneNew->m_nItemType = __BSVIT_TAB_PANE;

			//	HTREEITEM htiTabPane_New_Left = htiTabPane_New;
				HTREEITEM htiTabPane_New_Left = RcgtdItemInsert( htiTabPane_New );
				ASSERT( htiTabPane_New_Left != NULL );
				CExtRichGenItemData * pData_TabPane_New_Left = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_New_Left );
				pData_TabPane_New_Left->m_nItemType = __BSVIT_LEFT_PANE_LIGHT;
				pData_TabPane_New_Left->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" ui-dock:fill; width:100%; \" ") );

					htiCaption = RcgtdItemInsert( htiTabPane_New_Left );
					ASSERT( htiCaption != NULL );
					pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
					ASSERT( pData_Caption != NULL );
					pData_Caption->m_nItemType = __BSVIT_CAPTION;
					pData_Caption->m_strHtmlInner = _T("Available Templates");

					htiSeparator = RcgtdItemInsert( htiTabPane_New_Left );
					ASSERT( htiSeparator != NULL );
					pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
					ASSERT( pData_Separator != NULL );
					pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

					htiDirectContent = RcgtdItemInsert( htiTabPane_New_Left );
					ASSERT( htiDirectContent != NULL );
					pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
					ASSERT( pData_DirectContent != NULL );
					pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
					pData_DirectContent->m_strHtmlInner = _T("<div style=\" width:100%; height:95%; ui-sbh:hidden; overflow:scroll; \" >");

						htiCaption = RcgtdItemInsert( htiTabPane_New_Left );
						ASSERT( htiCaption != NULL );
						pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
						ASSERT( pData_Caption != NULL );
						pData_Caption->m_nItemType = __BSVIT_DIRECT_CONTENT;
						pData_Caption->m_strHtmlInner =
							_T("<p style=\" width:100%; margin:0px; padding:0.65em 1.25em 0.65em 1.25em; background-color:rgb(240,240,240); \" >Basic Templates</p>\r\n");

						static LPCTSTR g_arrNewTemplateKeyTips[] =
						{
							_T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"),
							_T("Y1"), _T("Y2"), _T("Y3"), _T("Y4"), _T("Y5"), _T("Y6"), _T("Y7"), _T("Y8"), _T("Y9"),
						//	_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("I"),_T("J"),_T("K"),_T("L"),_T("M"),
						//	_T("N"),_T("O"),_T("P"),_T("Q"),_T("R"),_T("S"),_T("T"),_T("U"),_T("V"),_T("W"),_T("X"),_T("Y"),_T("Z"),
							_T("YA"),_T("YB"),_T("YC"),_T("YD"),_T("YE"),_T("YF"),_T("YG"),_T("YH"),_T("YI"),_T("YJ"),_T("YK"),_T("YL"),_T("YM"),
							_T("YN"),_T("YO"),_T("YP"),_T("YQ"),_T("YR"),_T("YS"),_T("YT"),_T("YU"),_T("YV"),_T("YW"),_T("YX"),_T("YY"),_T("YZ"),
							_T("ZA"),_T("ZB"),_T("ZC"),_T("ZD"),_T("ZE"),_T("ZF"),_T("ZG"),_T("ZH"),_T("ZI"),_T("ZJ"),_T("ZK"),_T("ZL"),_T("ZM"),
							_T("ZN"),_T("ZO"),_T("ZP"),_T("ZQ"),_T("ZR"),_T("ZS"),_T("ZT"),_T("ZU"),_T("ZV"),_T("ZW"),_T("ZX"),_T("ZY"),_T("ZZ"),
						//	_T(""),
						};
						INT nNewTemplateKeyTipIndex = 0, nNewTemplateKeyTipCount = sizeof(g_arrNewTemplateKeyTips) / sizeof(g_arrNewTemplateKeyTips[0]);
						for( i = 1; i <= 14; i ++ )
						{
							CString strPicName;
							strPicName.Format( _T("ID_NEW_DOC_BASIC_%d.bmp"), i );
							HTREEITEM htiButton = RcgtdItemInsert( htiTabPane_New_Left );
							ASSERT( htiButton != NULL );
							CExtRichGenItemData * pData_Button = (CExtRichGenItemData*)RcgtdItemDataGet( htiButton );
							ASSERT( pData_Button != NULL );
							pData_Button->m_nCmdID = 39300 + i;
							pData_Button->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE_FLAT;
							pData_Button->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, LPCTSTR(strPicName) );
							CString strButtonCaption;
							strButtonCaption.Format( _T("Basic<br>Template %d"), i );
							pData_Button->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, LPCTSTR(strButtonCaption) );
							pData_Button->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" height:10em; \" ") );
							pData_Button->ParmAppend( __RCGI_PARM_IMAGE_AREA_TAG_PARAMETERS, _T(" style=\" min-height:60%; max-height:60%; \" ") );
							if( nNewTemplateKeyTipIndex < nNewTemplateKeyTipCount )
							{
								RcgtdItemKeyTipSet(
									htiButton,
									new CExtCustomizeCmdKeyTip(
										__EXT_MFC_SAFE_LPCTSTR( g_arrNewTemplateKeyTips[nNewTemplateKeyTipIndex] )
										),
									false
									);
								nNewTemplateKeyTipIndex ++;
							} // if( nNewTemplateKeyTipIndex < nNewTemplateKeyTipCount )
						} // for( i = 1; i <= 14; i ++ )

						htiCaption = RcgtdItemInsert( htiTabPane_New_Left );
						ASSERT( htiCaption != NULL );
						pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
						ASSERT( pData_Caption != NULL );
						pData_Caption->m_nItemType = __BSVIT_DIRECT_CONTENT;
						pData_Caption->m_strHtmlInner =
							_T("<p style=\" width:100%; margin:0px; padding:0.65em 1.25em 0.65em 1.25em; background-color:rgb(240,240,240); \" >Additional Templates</p>\r\n");

						for( i = 1; i <= 29; i ++ )
						{
							CString strPicName;
							strPicName.Format( _T("ID_NEW_DOC_ADDITIONAL_%d.bmp"), i );
							HTREEITEM htiButton = RcgtdItemInsert( htiTabPane_New_Left );
							ASSERT( htiButton != NULL );
							CExtRichGenItemData * pData_Button = (CExtRichGenItemData*)RcgtdItemDataGet( htiButton );
							ASSERT( pData_Button != NULL );
							pData_Button->m_nCmdID = 39400 + i;
							pData_Button->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE_FLAT;
							pData_Button->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, LPCTSTR(strPicName) );
							CString strButtonCaption;
							strButtonCaption.Format( _T("Additional<br>Template %d"), i );
							pData_Button->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, LPCTSTR(strButtonCaption) );
							pData_Button->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" height:10em; \" ") );
							pData_Button->ParmAppend( __RCGI_PARM_IMAGE_AREA_TAG_PARAMETERS, _T(" style=\" min-height:60%; max-height:60%; \" ") );
							if( nNewTemplateKeyTipIndex < nNewTemplateKeyTipCount )
							{
								RcgtdItemKeyTipSet(
									htiButton,
									new CExtCustomizeCmdKeyTip(
										__EXT_MFC_SAFE_LPCTSTR( g_arrNewTemplateKeyTips[nNewTemplateKeyTipIndex] )
										),
									false
									);
								nNewTemplateKeyTipIndex ++;
							} // if( nNewTemplateKeyTipIndex < nNewTemplateKeyTipCount )
						} // for( i = 1; i <= 29; i ++ )

					htiDirectContent = RcgtdItemInsert( htiTabPane_New_Left );
					ASSERT( htiDirectContent != NULL );
					pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
					ASSERT( pData_DirectContent != NULL );
					pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
					pData_DirectContent->m_strHtmlInner = _T("<p style=\" display:inline-block; height:3em; width:100%; \" ></p></div>");
//					pData_DirectContent->m_strHtmlInner = _T("</div>");

			//	HTREEITEM htiTabPane_New_Right = htiTabPane_New;
				HTREEITEM htiTabPane_New_Right = RcgtdItemInsert( htiTabPane_New );
				ASSERT( htiTabPane_New_Right != NULL );
				CExtRichGenItemData * pData_TabPane_New_Right = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_New_Right );
				pData_TabPane_New_Right->m_nItemType = __BSVIT_RIGHT_PANE_DARK;
				pData_TabPane_New_Right->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" ui-dock:right; width:20em; \" ") );

					htiDirectContent = RcgtdItemInsert( htiTabPane_New_Right );
					ASSERT( htiDirectContent != NULL );
					pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
					ASSERT( pData_DirectContent != NULL );
					pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
					pData_DirectContent->m_strHtmlInner = _T("<div style=\" height:3em; \" ></div>");

					HTREEITEM htiCommand_CreateNewDocument = RcgtdItemInsert( htiTabPane_New_Right );
					ASSERT( htiCommand_CreateNewDocument != NULL );
					CExtRichGenItemData * pData_CreateNewDocument = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_CreateNewDocument );
					ASSERT( pData_CreateNewDocument != NULL );
					pData_CreateNewDocument->m_nCmdID = 39105;
					pData_CreateNewDocument->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE;
					pData_CreateNewDocument->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_CREATE.bmp") );
					pData_CreateNewDocument->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Create") );
					RcgtdItemKeyTipSet( htiCommand_CreateNewDocument, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("N") ) ), false );



HTREEITEM htiTabItem_Print = RcgtdItemInsert( htiRoot );
	ASSERT( htiTabItem_Print != NULL );
CExtRichGenItemData * pData_Print = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_Print );
	ASSERT( pData_Print != NULL );
	pData_Print->m_nCmdID = ID_FILE_PRINT;
	pData_Print->m_nItemType = __BSVIT_TAB_ITEM;
	pData_Print->m_strHtmlInner = _T("Print");
	RcgtdItemKeyTipSet( htiTabItem_Print, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("P") ) ), false );

			HTREEITEM htiTabPane_Print = RcgtdItemInsert( htiTabItem_Print );
			ASSERT( htiTabPane_Print != NULL );
			CExtRichGenItemData * pData_TabPanePrint = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Print );
			pData_TabPanePrint->m_nItemType = __BSVIT_TAB_PANE;

					htiDirectContent = RcgtdItemInsert( htiTabPane_Print );
					ASSERT( htiDirectContent != NULL );
					pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
					ASSERT( pData_DirectContent != NULL );
					pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
					pData_DirectContent->m_strHtmlInner =
						_T("<p style=\" width:100%; margin-top:20em; text-align:center; font-style:italic; color:grey; \" ><small>Still under construction...</small></p>\r\n");



/*	
HTREEITEM htiTabItem_NestedTest = RcgtdItemInsert( htiRoot );
	ASSERT( htiTabItem_NestedTest != NULL );
CExtRichGenItemData * pData_NestedTest = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_NestedTest );
	ASSERT( pData_NestedTest != NULL );
	pData_NestedTest->m_nCmdID = 39004;
	pData_NestedTest->m_nItemType = __BSVIT_TAB_ITEM;
	pData_NestedTest->m_strHtmlInner = _T("NestedTest");
	RcgtdItemKeyTipSet( htiTabItem_NestedTest, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("D") ) ), false );

			HTREEITEM htiTabPane_NestedTest = RcgtdItemInsert( htiTabItem_NestedTest );
			ASSERT( htiTabPane_NestedTest != NULL );
			CExtRichGenItemData * pData_TabPaneNestedTest = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_NestedTest );
			pData_TabPaneNestedTest->m_nItemType = __BSVIT_TAB_PANE;
			pData_TabPaneNestedTest->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" width:75em; \" ") );

				HTREEITEM htiTabPane_Nested_Left = RcgtdItemInsert( htiTabPane_NestedTest );
				ASSERT( htiTabPane_Nested_Left != NULL );
				CExtRichGenItemData * pData_TabPane_Nested_Left = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Nested_Left );
				pData_TabPane_Nested_Left->m_nItemType = __BSVIT_LEFT_PANE_DARK;
				pData_TabPane_Nested_Left->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" display:ui-dock; ui-dock:left; width:15em; \" ") );

				HTREEITEM htiTabPane_Nested_Middle1 = RcgtdItemInsert( htiTabPane_NestedTest );
				ASSERT( htiTabPane_Nested_Middle1 != NULL );
				CExtRichGenItemData * pData_TabPane_Nested_Middle1 = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Nested_Middle1 );
				pData_TabPane_Nested_Middle1->m_nItemType = __BSVIT_MIDDLE_PANE_LIGHT;
				pData_TabPane_Nested_Middle1->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" display:ui-dock; ui-dock:left; width:15em; \" ") );

				HTREEITEM htiTabPane_Nested_Middle2 = RcgtdItemInsert( htiTabPane_NestedTest );
				ASSERT( htiTabPane_Nested_Middle2 != NULL );
				CExtRichGenItemData * pData_TabPane_Nested_Middle2 = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Nested_Middle2 );
				pData_TabPane_Nested_Middle2->m_nItemType = __BSVIT_MIDDLE_PANE_DARK;
				pData_TabPane_Nested_Middle2->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" display:ui-dock; ui-dock:left; width:15em; \" ") );

				HTREEITEM htiTabPane_Nested_Middle3 = RcgtdItemInsert( htiTabPane_NestedTest );
				ASSERT( htiTabPane_Nested_Middle3 != NULL );
				CExtRichGenItemData * pData_TabPane_Nested_Middle3 = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Nested_Middle3 );
				pData_TabPane_Nested_Middle3->m_nItemType = __BSVIT_MIDDLE_PANE_LIGHT;
				pData_TabPane_Nested_Middle3->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" display:ui-dock; ui-dock:left; width:15em; \" ") );

				HTREEITEM htiTabPane_Nested_Right = RcgtdItemInsert( htiTabPane_NestedTest );
				ASSERT( htiTabPane_Nested_Right != NULL );
				CExtRichGenItemData * pData_TabPane_Nested_Right = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Nested_Right );
				pData_TabPane_Nested_Right->m_nItemType = __BSVIT_RIGHT_PANE_DARK;
				pData_TabPane_Nested_Right->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" display:ui-dock; ui-dock:fill; width:100%; \" ") );

							htiCaption = RcgtdItemInsert( htiTabPane_Nested_Left );
							ASSERT( htiCaption != NULL );
							pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
							ASSERT( pData_Caption != NULL );
							pData_Caption->m_nItemType = __BSVIT_CAPTION;
							pData_Caption->m_strHtmlInner = _T("Level 1");
							htiSeparator = RcgtdItemInsert( htiTabPane_Nested_Left );
							ASSERT( htiSeparator != NULL );
							pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
							ASSERT( pData_Separator != NULL );
							pData_Separator->m_nItemType = __BSVIT_SEPARATOR;
							for( INT nNestedTestIndex1 = 0, nNestedTestCount1 = 5; nNestedTestIndex1 < nNestedTestCount1; nNestedTestIndex1 ++ )
							{
								HTREEITEM htiTabItem_Nested1 = RcgtdItemInsert( htiTabPane_Nested_Left );
								ASSERT( htiTabItem_Nested1 != NULL );
								CExtRichGenItemData * pData_TabItem_Nested1 = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_Nested1 );
								pData_TabItem_Nested1->m_nCmdID = 22000;
								pData_TabItem_Nested1->m_nItemType = __BSVIT_TAB_ITEM;
								pData_TabItem_Nested1->m_strHtmlInner.Format( _T("Level 1, Item %d"), nNestedTestIndex1 + 1);
if( nNestedTestIndex1 == 0 )
RcgwTreeItemSelectionDelay( htiTabItem_Nested1 ); // select this tab-item initially
								HTREEITEM htiTabPane_Nested1 = RcgtdItemInsert( htiTabItem_Nested1 );
								ASSERT( htiTabPane_Nested1 != NULL );
								CExtRichGenItemData * pData_TabPane_Nested1 = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Nested1 );
								pData_TabPane_Nested1->m_nItemType = __BSVIT_TAB_PANE;

											htiCaption = RcgtdItemInsert( htiTabItem_Nested1 );
											ASSERT( htiCaption != NULL );
											pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
											ASSERT( pData_Caption != NULL );
											pData_Caption->m_nItemType = __BSVIT_CAPTION;
											pData_Caption->m_strHtmlInner = _T("Level 2");
											htiSeparator = RcgtdItemInsert( htiTabItem_Nested1 );
											ASSERT( htiSeparator != NULL );
											pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
											ASSERT( pData_Separator != NULL );
											pData_Separator->m_nItemType = __BSVIT_SEPARATOR;
											for( INT nNestedTestIndex2 = 0, nNestedTestCount2 = 4; nNestedTestIndex2 < nNestedTestCount2; nNestedTestIndex2 ++ )
											{
												HTREEITEM htiTabItem_Nested2 = RcgtdItemInsert( htiTabItem_Nested1 );
												ASSERT( htiTabItem_Nested2 != NULL );
												CExtRichGenItemData * pData_TabItem_Nested2 = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_Nested2 );
												pData_TabItem_Nested2->m_nCmdID = 22000;
												pData_TabItem_Nested2->m_nItemType = __BSVIT_TAB_ITEM;
												pData_TabItem_Nested2->m_strHtmlInner.Format( _T("Level 2, Item %d"), nNestedTestIndex2 + 2);
if( nNestedTestIndex2 == 0 )
RcgwTreeItemSelectionDelay( htiTabItem_Nested2 ); // select this tab-item initially
												HTREEITEM htiTabPane_Nested2 = RcgtdItemInsert( htiTabItem_Nested2 );
												ASSERT( htiTabPane_Nested2 != NULL );
												CExtRichGenItemData * pData_TabPane_Nested2 = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Nested2 );
												pData_TabPane_Nested2->m_nItemType = __BSVIT_TAB_PANE;


											} // for( INT nNestedTestIndex2 = 0, nNestedTestCount2 = 4; nNestedTestIndex2 < nNestedTestCount2; nNestedTestIndex2 ++ )

							} // for( INT nNestedTestIndex1 = 0, nNestedTestCount1 = 5; nNestedTestIndex1 < nNestedTestCount1; nNestedTestIndex1 ++ )
*/


HTREEITEM htiTabItem_Share = RcgtdItemInsert( htiRoot );
	ASSERT( htiTabItem_Share != NULL );
CExtRichGenItemData * pData_Share = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_Share );
	ASSERT( pData_Share != NULL );
	pData_Share->m_nCmdID = 39005;
	pData_Share->m_nItemType = __BSVIT_TAB_ITEM;
	pData_Share->m_strHtmlInner = _T("Share");
	RcgtdItemKeyTipSet( htiTabItem_Share, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("H") ) ), false );

			HTREEITEM htiTabPane_Share = RcgtdItemInsert( htiTabItem_Share );
			ASSERT( htiTabPane_Share != NULL );
			CExtRichGenItemData * pData_TabPaneShare = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Share );
			pData_TabPaneShare->m_nItemType = __BSVIT_TAB_PANE;
			pData_TabPaneShare->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" ui-dock:left; width:77em; \" ") );

			//	HTREEITEM htiTabPane_Share_Left = htiTabPane_Share;
				HTREEITEM htiTabPane_Share_Left = RcgtdItemInsert( htiTabPane_Share );
				ASSERT( htiTabPane_Share_Left != NULL );
				CExtRichGenItemData * pData_TabPane_Share_Left = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Share_Left );
				pData_TabPane_Share_Left->m_nItemType = __BSVIT_LEFT_PANE_DARK;
				pData_TabPane_Share_Left->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" width:21em; \" ") );

					htiCaption = RcgtdItemInsert( htiTabPane_Share_Left );
					ASSERT( htiCaption != NULL );
					pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
					ASSERT( pData_Caption != NULL );
					pData_Caption->m_nItemType = __BSVIT_CAPTION;
					pData_Caption->m_strHtmlInner = _T("Send");
										htiSeparator = RcgtdItemInsert( htiTabPane_Share_Left );
										ASSERT( htiSeparator != NULL );
										pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
										ASSERT( pData_Separator != NULL );
										pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

										HTREEITEM htiTabItem_2ndLevel_SendUsingEMail = RcgtdItemInsert( htiTabPane_Share_Left );
										ASSERT( htiTabItem_2ndLevel_SendUsingEMail != NULL );
										CExtRichGenItemData * pData_2ndLevel_SendUsingEMail = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_2ndLevel_SendUsingEMail );
										ASSERT( pData_2ndLevel_SendUsingEMail != NULL );
										pData_2ndLevel_SendUsingEMail->m_nCmdID = 39020;
										pData_2ndLevel_SendUsingEMail->m_nItemType = __BSVIT_TAB_ITEM;
										pData_2ndLevel_SendUsingEMail->m_strHtmlInner =
											_T("<img style=\" position:relative; top:0.3em; \" src=\"icon_ID_BUTTON_SEND_USING_E_MAIL.bmp\" />&nbsp;&nbsp;&nbsp;Send Using E-mail");
RcgwTreeItemSelectionDelay( htiTabItem_2ndLevel_SendUsingEMail ); // select this tab-item initially
										RcgtdItemKeyTipSet( htiTabItem_2ndLevel_SendUsingEMail, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("E") ) ), false );
													HTREEITEM htiTabPane_2ndLevel_SendUsingEMail = RcgtdItemInsert( htiTabItem_2ndLevel_SendUsingEMail );
													ASSERT( htiTabPane_2ndLevel_SendUsingEMail != NULL );
													CExtRichGenItemData * pData_TabPane_2ndLevel_SendUsingEMail = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_2ndLevel_SendUsingEMail );
													pData_TabPane_2ndLevel_SendUsingEMail->m_nItemType = __BSVIT_TAB_PANE;
															htiCaption = RcgtdItemInsert( htiTabPane_2ndLevel_SendUsingEMail );
															ASSERT( htiCaption != NULL );
															pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
															ASSERT( pData_Caption != NULL );
															pData_Caption->m_nItemType = __BSVIT_CAPTION;
															pData_Caption->m_strHtmlInner = _T("Send Using E-mail");

															htiSeparator = RcgtdItemInsert( htiTabPane_2ndLevel_SendUsingEMail );
															ASSERT( htiSeparator != NULL );
															pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
															ASSERT( pData_Separator != NULL );
															pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

																				htiGroup = RcgtdItemInsert( htiTabPane_2ndLevel_SendUsingEMail );
																				ASSERT( htiGroup != NULL );
																				pData_Group = (CExtRichGenItemData*)RcgtdItemDataGet( htiGroup );
																				ASSERT( pData_Group != NULL );
																				pData_Group->m_nItemType = __BSVIT_ELEMENT_GROUP;

																										htiTable = RcgtdItemInsert( htiGroup );
																										ASSERT( htiTable != NULL );
																										pData_Table = (CExtRichGenItemData*)RcgtdItemDataGet( htiTable );
																										ASSERT( pData_Table != NULL );
																										pData_Table->m_nItemType = __BSVIT_TABLE;
																									//	pData_Table->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" border-spacing:0.3em; \" ") );

																											htiTableRow = RcgtdItemInsert( htiTable );
																											ASSERT( htiTableRow != NULL );
																											pData_TableRow = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableRow );
																											ASSERT( pData_TableRow != NULL );
																											pData_TableRow->m_nItemType = __BSVIT_TABLE_ROW;

																												htiTableCell = RcgtdItemInsert( htiTableRow );
																												ASSERT( htiTableCell != NULL );
																												pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
																												ASSERT( pData_TableCell != NULL );
																												pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

																													HTREEITEM htiCommand_SendAsAttachment = RcgtdItemInsert( htiTableCell );
																													ASSERT( htiCommand_SendAsAttachment != NULL );
																													CExtRichGenItemData * pData_SendAsAttachment = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_SendAsAttachment );
																													ASSERT( pData_SendAsAttachment != NULL );
																													pData_SendAsAttachment->m_nCmdID = 39106;
																													pData_SendAsAttachment->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE;
																													pData_SendAsAttachment->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_SEND_AS_ATTACHMENT.bmp") );
																													pData_SendAsAttachment->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Send as<br>Attachment") );
																													RcgtdItemKeyTipSet( htiCommand_SendAsAttachment, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("A") ) ), false );

																												htiTableCell = RcgtdItemInsert( htiTableRow );
																												ASSERT( htiTableCell != NULL );
																												pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
																												ASSERT( pData_TableCell != NULL );
																												pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

																													htiCaption = RcgtdItemInsert( htiTableCell );
																													ASSERT( htiCaption != NULL );
																													pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
																													ASSERT( pData_Caption != NULL );
																													pData_Caption->m_nItemType = __BSVIT_CAPTION;
																													pData_Caption->m_strHtmlInner = _T("Attach a copy of this document to an e-mail");
																													pData_Caption->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-left:1em; margin-top:0em; \" ") );

																													htiDirectContent = RcgtdItemInsert( htiTableCell );
																													ASSERT( htiDirectContent != NULL );
																													pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
																													ASSERT( pData_DirectContent != NULL );
																													pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
																													pData_DirectContent->m_strHtmlInner =
																														_T("<ul style=\" list-style-type:square; \" >\r\n")
																														_T("<li>Allows you to work on one copy of this document, while the e-mail recipients work on another</li>\r\n")
																														_T("</ul>\r\n")
																														;

															htiSeparator = RcgtdItemInsert( htiTabPane_2ndLevel_SendUsingEMail );
															ASSERT( htiSeparator != NULL );
															pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
															ASSERT( pData_Separator != NULL );
															pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

																				htiGroup = RcgtdItemInsert( htiTabPane_2ndLevel_SendUsingEMail );
																				ASSERT( htiGroup != NULL );
																				pData_Group = (CExtRichGenItemData*)RcgtdItemDataGet( htiGroup );
																				ASSERT( pData_Group != NULL );
																				pData_Group->m_nItemType = __BSVIT_ELEMENT_GROUP;

																										htiTable = RcgtdItemInsert( htiGroup );
																										ASSERT( htiTable != NULL );
																										pData_Table = (CExtRichGenItemData*)RcgtdItemDataGet( htiTable );
																										ASSERT( pData_Table != NULL );
																										pData_Table->m_nItemType = __BSVIT_TABLE;
																									//	pData_Table->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" border-spacing:0.3em; \" ") );

																											htiTableRow = RcgtdItemInsert( htiTable );
																											ASSERT( htiTableRow != NULL );
																											pData_TableRow = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableRow );
																											ASSERT( pData_TableRow != NULL );
																											pData_TableRow->m_nItemType = __BSVIT_TABLE_ROW;

																												htiTableCell = RcgtdItemInsert( htiTableRow );
																												ASSERT( htiTableCell != NULL );
																												pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
																												ASSERT( pData_TableCell != NULL );
																												pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

																													HTREEITEM htiCommand_SendAsPDF = RcgtdItemInsert( htiTableCell );
																													ASSERT( htiCommand_SendAsPDF != NULL );
																													CExtRichGenItemData * pData_SendAsPDF = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_SendAsPDF );
																													ASSERT( pData_SendAsPDF != NULL );
																													pData_SendAsPDF->m_nCmdID = 39107;
																													pData_SendAsPDF->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE;
																													pData_SendAsPDF->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_SEND_AS_PDF.bmp") );
																													pData_SendAsPDF->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Send as<br>PDF") );
																													RcgtdItemKeyTipSet( htiCommand_SendAsPDF, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("F") ) ), false );

																												htiTableCell = RcgtdItemInsert( htiTableRow );
																												ASSERT( htiTableCell != NULL );
																												pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
																												ASSERT( pData_TableCell != NULL );
																												pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

																													htiCaption = RcgtdItemInsert( htiTableCell );
																													ASSERT( htiCaption != NULL );
																													pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
																													ASSERT( pData_Caption != NULL );
																													pData_Caption->m_nItemType = __BSVIT_CAPTION;
																													pData_Caption->m_strHtmlInner = _T("Attach a PDF copy of this document to an e-mail");
																													pData_Caption->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-left:1em; margin-top:0em; \" ") );

																													htiDirectContent = RcgtdItemInsert( htiTableCell );
																													ASSERT( htiDirectContent != NULL );
																													pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
																													ASSERT( pData_DirectContent != NULL );
																													pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
																													pData_DirectContent->m_strHtmlInner =
																														_T("<ul style=\" list-style-type:square; \" >\r\n")
																														_T("<li>Document look the same on almost any computer</li>\r\n")
																														_T("<li>Preserve fonts, formatting and images accross platforms</li>\r\n")
																														_T("<li>Contents of PDF documents cannot be easily changed</li>\r\n")
																														_T("<li>Comply to an industry standard format</li>\r\n")
																														_T("</ul>\r\n")
																														;

															htiSeparator = RcgtdItemInsert( htiTabPane_2ndLevel_SendUsingEMail );
															ASSERT( htiSeparator != NULL );
															pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
															ASSERT( pData_Separator != NULL );
															pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

																				htiGroup = RcgtdItemInsert( htiTabPane_2ndLevel_SendUsingEMail );
																				ASSERT( htiGroup != NULL );
																				pData_Group = (CExtRichGenItemData*)RcgtdItemDataGet( htiGroup );
																				ASSERT( pData_Group != NULL );
																				pData_Group->m_nItemType = __BSVIT_ELEMENT_GROUP;

																										htiTable = RcgtdItemInsert( htiGroup );
																										ASSERT( htiTable != NULL );
																										pData_Table = (CExtRichGenItemData*)RcgtdItemDataGet( htiTable );
																										ASSERT( pData_Table != NULL );
																										pData_Table->m_nItemType = __BSVIT_TABLE;
																									//	pData_Table->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" border-spacing:0.3em; \" ") );

																											htiTableRow = RcgtdItemInsert( htiTable );
																											ASSERT( htiTableRow != NULL );
																											pData_TableRow = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableRow );
																											ASSERT( pData_TableRow != NULL );
																											pData_TableRow->m_nItemType = __BSVIT_TABLE_ROW;

																												htiTableCell = RcgtdItemInsert( htiTableRow );
																												ASSERT( htiTableCell != NULL );
																												pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
																												ASSERT( pData_TableCell != NULL );
																												pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

																													HTREEITEM htiCommand_SendAsXPS = RcgtdItemInsert( htiTableCell );
																													ASSERT( htiCommand_SendAsXPS != NULL );
																													CExtRichGenItemData * pData_SendAsXPS = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_SendAsXPS );
																													ASSERT( pData_SendAsXPS != NULL );
																													pData_SendAsXPS->m_nCmdID = 39108;
																													pData_SendAsXPS->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE;
																													pData_SendAsXPS->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_SEND_AS_XPS.bmp") );
																													pData_SendAsXPS->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Send as<br>XPS") );
																													RcgtdItemKeyTipSet( htiCommand_SendAsXPS, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("X") ) ), false );

																												htiTableCell = RcgtdItemInsert( htiTableRow );
																												ASSERT( htiTableCell != NULL );
																												pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
																												ASSERT( pData_TableCell != NULL );
																												pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

																													htiCaption = RcgtdItemInsert( htiTableCell );
																													ASSERT( htiCaption != NULL );
																													pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
																													ASSERT( pData_Caption != NULL );
																													pData_Caption->m_nItemType = __BSVIT_CAPTION;
																													pData_Caption->m_strHtmlInner = _T("Attach a XPS copy of this document to an e-mail");
																													pData_Caption->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-left:1em; margin-top:0em; \" ") );

																													htiDirectContent = RcgtdItemInsert( htiTableCell );
																													ASSERT( htiDirectContent != NULL );
																													pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
																													ASSERT( pData_DirectContent != NULL );
																													pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
																													pData_DirectContent->m_strHtmlInner =
																														_T("<ul style=\" list-style-type:square; \" >\r\n")
																														_T("<li>Document look the same on almost any computer</li>\r\n")
																														_T("<li>Preserve fonts, formatting and images accross platforms</li>\r\n")
																														_T("<li>Contents of XPS documents cannot be easily changed</li>\r\n")
																														_T("<li>Comply to an industry standard format</li>\r\n")
																														_T("</ul>")
																														;

										HTREEITEM htiTabItem_2ndLevel_SendAsInternetFax = RcgtdItemInsert( htiTabPane_Share_Left );
										ASSERT( htiTabItem_2ndLevel_SendAsInternetFax != NULL );
										CExtRichGenItemData * pData_2ndLevel_SendAsInternetFax = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_2ndLevel_SendAsInternetFax );
										ASSERT( pData_2ndLevel_SendAsInternetFax != NULL );
										pData_2ndLevel_SendAsInternetFax->m_nCmdID = 39021;
										pData_2ndLevel_SendAsInternetFax->m_nItemType = __BSVIT_TAB_ITEM;
										pData_2ndLevel_SendAsInternetFax->m_strHtmlInner =
											_T("<img style=\" position:relative; top:0.3em; \" src=\"icon_ID_BUTTON_SEND_AS_INTERNET_FAX.bmp\" />&nbsp;&nbsp;&nbsp;Send as Internet Fax");
										RcgtdItemKeyTipSet( htiTabItem_2ndLevel_SendAsInternetFax, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("IF") ) ), false );
													HTREEITEM htiTabPane_2ndLevel_SendAsInternetFax = RcgtdItemInsert( htiTabItem_2ndLevel_SendAsInternetFax );
													ASSERT( htiTabPane_2ndLevel_SendAsInternetFax != NULL );
													CExtRichGenItemData * pData_TabPane_2ndLevel_SendAsInternetFax = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_2ndLevel_SendAsInternetFax );
													pData_TabPane_2ndLevel_SendAsInternetFax->m_nItemType = __BSVIT_TAB_PANE;
															htiCaption = RcgtdItemInsert( htiTabPane_2ndLevel_SendAsInternetFax );
															ASSERT( htiCaption != NULL );
															pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
															ASSERT( pData_Caption != NULL );
															pData_Caption->m_nItemType = __BSVIT_CAPTION;
															pData_Caption->m_strHtmlInner = _T("Send as Internet Fax");

															htiSeparator = RcgtdItemInsert( htiTabPane_2ndLevel_SendAsInternetFax );
															ASSERT( htiSeparator != NULL );
															pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
															ASSERT( pData_Separator != NULL );
															pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

																	htiDirectContent = RcgtdItemInsert( htiTabPane_2ndLevel_SendAsInternetFax );
																	ASSERT( htiDirectContent != NULL );
																	pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
																	ASSERT( pData_DirectContent != NULL );
																	pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
																	pData_DirectContent->m_strHtmlInner =
																		_T("<p style=\" width:100%; margin-right:5em; \" >Send a fax without using a fax machine.</p>\r\n")
																		_T("<p style=\" height:2em; \" ></p>\r\n")
																		_T("<p style=\" width:100%; margin-right:5em; \" >You need to sign up for a fax provider in order to successfully send a fax. ")
																		_T("You will be prompted to sign up if you have not already done so.</p>\r\n")
																		_T("<p style=\" height:2em; \" ></p>\r\n")
																		;

																	HTREEITEM htiCommand_InternetFax = RcgtdItemInsert( htiTabPane_2ndLevel_SendAsInternetFax );
																	ASSERT( htiCommand_InternetFax != NULL );
																	CExtRichGenItemData * pData_InternetFax = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_InternetFax );
																	ASSERT( pData_InternetFax != NULL );
																	pData_InternetFax->m_nCmdID = 39109;
																	pData_InternetFax->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE;
																	pData_InternetFax->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_INTERNER_FAX.bmp") );
																	pData_InternetFax->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Internet<br>Fax") );
																	RcgtdItemKeyTipSet( htiCommand_InternetFax, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("A") ) ), false );

					htiCaption = RcgtdItemInsert( htiTabPane_Share_Left );
					ASSERT( htiCaption != NULL );
					pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
					ASSERT( pData_Caption != NULL );
					pData_Caption->m_nItemType = __BSVIT_CAPTION;
					pData_Caption->m_strHtmlInner = _T("File Types");
										htiSeparator = RcgtdItemInsert( htiTabPane_Share_Left );
										ASSERT( htiSeparator != NULL );
										pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
										ASSERT( pData_Separator != NULL );
										pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

										HTREEITEM htiTabItem_2ndLevel_ChangeFileType = RcgtdItemInsert( htiTabPane_Share_Left );
										ASSERT( htiTabItem_2ndLevel_ChangeFileType != NULL );
										CExtRichGenItemData * pData_2ndLevel_ChangeFileType = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_2ndLevel_ChangeFileType );
										ASSERT( pData_2ndLevel_ChangeFileType != NULL );
										pData_2ndLevel_ChangeFileType->m_nCmdID = 39022;
										pData_2ndLevel_ChangeFileType->m_nItemType = __BSVIT_TAB_ITEM;
										pData_2ndLevel_ChangeFileType->m_strHtmlInner =
											_T("<img style=\" position:relative; top:0.3em; \" src=\"icon_ID_BUTTON_CHANGE_FILE_TYPE.bmp\" />&nbsp;&nbsp;&nbsp;Change File Type");
										RcgtdItemKeyTipSet( htiTabItem_2ndLevel_ChangeFileType, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("C") ) ), false );
													HTREEITEM htiTabPane_2ndLevel_ChangeFileType = RcgtdItemInsert( htiTabItem_2ndLevel_ChangeFileType );
													ASSERT( htiTabPane_2ndLevel_ChangeFileType != NULL );
													CExtRichGenItemData * pData_TabPane_2ndLevel_ChangeFileType = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_2ndLevel_ChangeFileType );
													pData_TabPane_2ndLevel_ChangeFileType->m_nItemType = __BSVIT_TAB_PANE;
															htiCaption = RcgtdItemInsert( htiTabPane_2ndLevel_ChangeFileType );
															ASSERT( htiCaption != NULL );
															pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
															ASSERT( pData_Caption != NULL );
															pData_Caption->m_nItemType = __BSVIT_CAPTION;
															pData_Caption->m_strHtmlInner = _T("Save File");

															htiSeparator = RcgtdItemInsert( htiTabPane_2ndLevel_ChangeFileType );
															ASSERT( htiSeparator != NULL );
															pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
															ASSERT( pData_Separator != NULL );
															pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

																	htiDirectContent = RcgtdItemInsert( htiTabPane_2ndLevel_ChangeFileType );
																	ASSERT( htiDirectContent != NULL );
																	pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
																	ASSERT( pData_DirectContent != NULL );
																	pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
																	pData_DirectContent->m_strHtmlInner =
																		_T("<p style=\" width:100%; margin-top:20em; text-align:center; font-style:italic; color:grey; \" ><small>Still under construction...</small></p>\r\n");

										HTREEITEM htiTabItem_2ndLevel_CreatePdfXpsDocument = RcgtdItemInsert( htiTabPane_Share_Left );
										ASSERT( htiTabItem_2ndLevel_CreatePdfXpsDocument != NULL );
										CExtRichGenItemData * pData_2ndLevel_CreatePdfXpsDocument = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_2ndLevel_CreatePdfXpsDocument );
										ASSERT( pData_2ndLevel_CreatePdfXpsDocument != NULL );
										pData_2ndLevel_CreatePdfXpsDocument->m_nCmdID = 39023;
										pData_2ndLevel_CreatePdfXpsDocument->m_nItemType = __BSVIT_TAB_ITEM;
										pData_2ndLevel_CreatePdfXpsDocument->m_strHtmlInner =
											_T("<img style=\" position:relative; top:0.3em; \" src=\"icon_ID_BUTTON_CREATE_PDF_XPS_DOCUMENT.bmp\" />&nbsp;&nbsp;&nbsp;Create PDF/XPS Document");
										RcgtdItemKeyTipSet( htiTabItem_2ndLevel_CreatePdfXpsDocument, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("P") ) ), false );
													HTREEITEM htiTabPane_2ndLevel_CreatePdfXpsDocument = RcgtdItemInsert( htiTabItem_2ndLevel_CreatePdfXpsDocument );
													ASSERT( htiTabPane_2ndLevel_CreatePdfXpsDocument != NULL );
													CExtRichGenItemData * pData_TabPane_2ndLevel_CreatePdfXpsDocument = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_2ndLevel_CreatePdfXpsDocument );
													pData_TabPane_2ndLevel_CreatePdfXpsDocument->m_nItemType = __BSVIT_TAB_PANE;
															htiCaption = RcgtdItemInsert( htiTabPane_2ndLevel_CreatePdfXpsDocument );
															ASSERT( htiCaption != NULL );
															pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
															ASSERT( pData_Caption != NULL );
															pData_Caption->m_nItemType = __BSVIT_CAPTION;
															pData_Caption->m_strHtmlInner = _T("Create PDF/XPS Document");

															htiSeparator = RcgtdItemInsert( htiTabPane_2ndLevel_CreatePdfXpsDocument );
															ASSERT( htiSeparator != NULL );
															pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
															ASSERT( pData_Separator != NULL );
															pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

																	htiDirectContent = RcgtdItemInsert( htiTabPane_2ndLevel_CreatePdfXpsDocument );
																	ASSERT( htiDirectContent != NULL );
																	pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
																	ASSERT( pData_DirectContent != NULL );
																	pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
																	pData_DirectContent->m_strHtmlInner =
																		_T("<p style=\" width:100%; margin-right:5em; \" >Create an new PDF or XPS document using the contents of this document.</p>\r\n")
																		_T("<p style=\" height:2em; \" ></p>\r\n")
																		_T("<p style=\" width:100%; margin-right:5em; \" >PDF and XPS documents provide the follwing capabilities:</p>\r\n")
																		_T("<p style=\" height:2em; \" ></p>\r\n")
																		_T("<ul style=\" list-style-type:square; \" >\r\n")
																		_T("<li>Document look the same on almost any computer</li>\r\n")
																		_T("<li>Preserve fonts, formatting and images accross platforms</li>\r\n")
																		_T("<li>Contents of PDF and XPS documents cannot be easily changed</li>\r\n")
																		_T("<li>Comply to an industry standard format</li>\r\n")
																		_T("</ul>")
																		_T("<p style=\" height:2em; \" ></p>\r\n")
																		_T("<p style=\" width:100%; margin-right:5em; \" >Free PDF and XPS viewers are available on the web.</p>\r\n")
																		_T("<p style=\" height:1em; \" ></p>\r\n")
																		;

																	HTREEITEM htiCommand_CreatePdfXps = RcgtdItemInsert( htiTabPane_2ndLevel_CreatePdfXpsDocument );
																	ASSERT( htiCommand_CreatePdfXps != NULL );
																	CExtRichGenItemData * pData_CreatePdfXps = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_CreatePdfXps );
																	ASSERT( pData_CreatePdfXps != NULL );
																	pData_CreatePdfXps->m_nCmdID = 39110;
																	pData_CreatePdfXps->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE;
																	pData_CreatePdfXps->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_CREATE_A_PDF_XPS.bmp") );
																	pData_CreatePdfXps->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Create a<br>PDF/XPS") );
																	RcgtdItemKeyTipSet( htiCommand_CreatePdfXps, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("A") ) ), false );

					htiCaption = RcgtdItemInsert( htiTabPane_Share_Left );
					ASSERT( htiCaption != NULL );
					pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
					ASSERT( pData_Caption != NULL );
					pData_Caption->m_nItemType = __BSVIT_CAPTION;
					pData_Caption->m_strHtmlInner = _T("Special Locations");
										htiSeparator = RcgtdItemInsert( htiTabPane_Share_Left );
										ASSERT( htiSeparator != NULL );
										pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
										ASSERT( pData_Separator != NULL );
										pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

										HTREEITEM htiTabItem_2ndLevel_SaveToSharePoint = RcgtdItemInsert( htiTabPane_Share_Left );
										ASSERT( htiTabItem_2ndLevel_SaveToSharePoint != NULL );
										CExtRichGenItemData * pData_2ndLevel_SaveToSharePoint = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_2ndLevel_SaveToSharePoint );
										ASSERT( pData_2ndLevel_SaveToSharePoint != NULL );
										pData_2ndLevel_SaveToSharePoint->m_nCmdID = 39024;
										pData_2ndLevel_SaveToSharePoint->m_nItemType = __BSVIT_TAB_ITEM;
										pData_2ndLevel_SaveToSharePoint->m_strHtmlInner =
											_T("<img style=\" position:relative; top:0.3em; \" src=\"icon_ID_BUTTON_SMALL_ORANGE_DOT.bmp\" />&nbsp;&nbsp;&nbsp;Save to SharePoint");
										RcgtdItemKeyTipSet( htiTabItem_2ndLevel_SaveToSharePoint, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("S") ) ), false );
													HTREEITEM htiTabPane_2ndLevel_SaveToSharePoint = RcgtdItemInsert( htiTabItem_2ndLevel_SaveToSharePoint );
													ASSERT( htiTabPane_2ndLevel_SaveToSharePoint != NULL );
													CExtRichGenItemData * pData_TabPane_2ndLevel_SaveToSharePoint = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_2ndLevel_SaveToSharePoint );
													pData_TabPane_2ndLevel_SaveToSharePoint->m_nItemType = __BSVIT_TAB_PANE;
															htiCaption = RcgtdItemInsert( htiTabPane_2ndLevel_SaveToSharePoint );
															ASSERT( htiCaption != NULL );
															pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
															ASSERT( pData_Caption != NULL );
															pData_Caption->m_nItemType = __BSVIT_CAPTION;
															pData_Caption->m_strHtmlInner = _T("Save to SharePoint");

															htiSeparator = RcgtdItemInsert( htiTabPane_2ndLevel_SaveToSharePoint );
															ASSERT( htiSeparator != NULL );
															pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
															ASSERT( pData_Separator != NULL );
															pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

																	htiDirectContent = RcgtdItemInsert( htiTabPane_2ndLevel_SaveToSharePoint );
																	ASSERT( htiDirectContent != NULL );
																	pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
																	ASSERT( pData_DirectContent != NULL );
																	pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
																	pData_DirectContent->m_strHtmlInner =
																		_T("<p style=\" width:100%; margin-top:20em; text-align:center; font-style:italic; color:grey; \" ><small>Still under construction...</small></p>\r\n");

										HTREEITEM htiTabItem_2ndLevel_PublishAsBlogEntry = RcgtdItemInsert( htiTabPane_Share_Left );
										ASSERT( htiTabItem_2ndLevel_PublishAsBlogEntry != NULL );
										CExtRichGenItemData * pData_2ndLevel_PublishAsBlogEntry = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_2ndLevel_PublishAsBlogEntry );
										ASSERT( pData_2ndLevel_PublishAsBlogEntry != NULL );
										pData_2ndLevel_PublishAsBlogEntry->m_nCmdID = 39025;
										pData_2ndLevel_PublishAsBlogEntry->m_nItemType = __BSVIT_TAB_ITEM;
										pData_2ndLevel_PublishAsBlogEntry->m_strHtmlInner =
											_T("<img style=\" position:relative; top:0.3em; \" src=\"icon_ID_BUTTON_PUBLISH_AS_BLOG_ENTRY.bmp\" />&nbsp;&nbsp;&nbsp;Publish as Blog Entry");
										RcgtdItemKeyTipSet( htiTabItem_2ndLevel_PublishAsBlogEntry, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("B") ) ), false );
													HTREEITEM htiTabPane_2ndLevel_PublishAsBlogEntry = RcgtdItemInsert( htiTabItem_2ndLevel_PublishAsBlogEntry );
													ASSERT( htiTabPane_2ndLevel_PublishAsBlogEntry != NULL );
													CExtRichGenItemData * pData_TabPane_2ndLevel_PublishAsBlogEntry = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_2ndLevel_PublishAsBlogEntry );
													pData_TabPane_2ndLevel_PublishAsBlogEntry->m_nItemType = __BSVIT_TAB_PANE;
															htiCaption = RcgtdItemInsert( htiTabPane_2ndLevel_PublishAsBlogEntry );
															ASSERT( htiCaption != NULL );
															pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
															ASSERT( pData_Caption != NULL );
															pData_Caption->m_nItemType = __BSVIT_CAPTION;
															pData_Caption->m_strHtmlInner = _T("Publish as Blog Entry");

															htiSeparator = RcgtdItemInsert( htiTabPane_2ndLevel_PublishAsBlogEntry );
															ASSERT( htiSeparator != NULL );
															pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
															ASSERT( pData_Separator != NULL );
															pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

																	htiDirectContent = RcgtdItemInsert( htiTabPane_2ndLevel_PublishAsBlogEntry );
																	ASSERT( htiDirectContent != NULL );
																	pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
																	ASSERT( pData_DirectContent != NULL );
																	pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
																	pData_DirectContent->m_strHtmlInner =
																		_T("<p style=\" width:100%; margin-right:5em; \" >Create a new blog post using your current document. ")
																		_T("Blogging is great for publishing content to the Web or to your company's network.</p>\r\n")
																		_T("<p style=\" height:2em; \" ></p>\r\n")
																		_T("<p style=\" width:100%; margin-right:5em; \" >Several popular blogging sites are supported:</p>\r\n")
																		_T("<p style=\" height:2em; \" ></p>\r\n")
																		_T("<ul style=\" list-style-type:square; \" >\r\n")
																		_T("<li>Blog provider 1</li>\r\n")
																		_T("<li>Blog provider 2</li>\r\n")
																		_T("<li>Blog provider 3</li>\r\n")
																		_T("<li>Blog provider 4</li>\r\n")
																		_T("<li>Blog provider 5</li>\r\n")
																		_T("</ul>")
																		_T("<p style=\" height:2em; \" ></p>\r\n")
																		_T("<p style=\" width:100%; margin-right:5em; \" >You are prompted to register your blog account ")
																		_T("if this is the first time you have published a blog post. You may add additional blog accounts ")
																		_T("or change existing blog accounts at any time.</p>\r\n")
																		_T("<p style=\" height:1em; \" ></p>\r\n")
																		;

																	HTREEITEM htiCommand_PublishToEntry = RcgtdItemInsert( htiTabPane_2ndLevel_PublishAsBlogEntry );
																	ASSERT( htiCommand_PublishToEntry != NULL );
																	CExtRichGenItemData * pData_PublishToEntry = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_PublishToEntry );
																	ASSERT( pData_PublishToEntry != NULL );
																	pData_PublishToEntry->m_nCmdID = 39111;
																	pData_PublishToEntry->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE;
																	pData_PublishToEntry->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_PUBLISH_TO_BLOG.bmp") );
																	pData_PublishToEntry->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Publish to<br>Blog") );
																	RcgtdItemKeyTipSet( htiCommand_PublishToEntry, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("A") ) ), false );

			//	HTREEITEM htiTabPane_Share_Right = htiTabPane_Share;
				HTREEITEM htiTabPane_Share_Right = RcgtdItemInsert( htiTabPane_Share );
				ASSERT( htiTabPane_Share_Right != NULL );
				CExtRichGenItemData * pData_TabPane_Share_Right = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Share_Right );
				pData_TabPane_Share_Right->m_nItemType = __BSVIT_RIGHT_PANE_LIGHT;



HTREEITEM htiTabItem_Application = RcgtdItemInsert( htiRoot );
	ASSERT( htiTabItem_Application != NULL );
CExtRichGenItemData * pData_Application = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabItem_Application );
	ASSERT( pData_Application != NULL );
	pData_Application->m_nCmdID = 39006;
	pData_Application->m_nItemType = __BSVIT_TAB_ITEM;
	pData_Application->m_strHtmlInner = _T("App");
	RcgtdItemKeyTipSet( htiTabItem_Application, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("L") ) ), false );

			HTREEITEM htiTabPane_Application = RcgtdItemInsert( htiTabItem_Application );
			ASSERT( htiTabPane_Application != NULL );
			CExtRichGenItemData * pData_TabPaneApplication = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Application );
			pData_TabPaneApplication->m_nItemType = __BSVIT_TAB_PANE;
			pData_TabPaneApplication->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" ui-dock:left; width:87em; \" ") );

			//	HTREEITEM htiTabPane_Application_Left = htiTabPane_Application;
				HTREEITEM htiTabPane_Application_Left = RcgtdItemInsert( htiTabPane_Application );
				ASSERT( htiTabPane_Application_Left != NULL );
				CExtRichGenItemData * pData_TabPane_Application_Left = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Application_Left );
				pData_TabPane_Application_Left->m_nItemType = __BSVIT_LEFT_PANE_LIGHT;
				pData_TabPane_Application_Left->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" width:35em; \" ") );

					htiScrollContainer = RcgtdItemInsert( htiTabPane_Application_Left );
					ASSERT( htiScrollContainer != NULL );
					pData_ScrollContainer = (CExtRichGenItemData*)RcgtdItemDataGet( htiScrollContainer );
					ASSERT( pData_ScrollContainer != NULL );
					pData_ScrollContainer->m_nItemType = __BSVIT_SCROLL_CONTAINER_VERT;
					pData_ScrollContainer->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-top:1em; margin-bottom:1em; \" ") );

						htiCaption = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiCaption != NULL );
						pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
						ASSERT( pData_Caption != NULL );
						pData_Caption->m_nItemType = __BSVIT_CAPTION;
						pData_Caption->m_strHtmlInner = _T("Help &amp; Training");

						htiSeparator = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiSeparator != NULL );
						pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
						ASSERT( pData_Separator != NULL );
						pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

						HTREEITEM htiCommand_ApplicationHelp = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiCommand_ApplicationHelp != NULL );
						CExtRichGenItemData * pData_ApplicationHelp = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_ApplicationHelp );
						ASSERT( pData_ApplicationHelp != NULL );
						pData_ApplicationHelp->m_nCmdID = 39112;
						pData_ApplicationHelp->m_nItemType = __BSVIT_COMMAND_BUTTON_WIDE_FLAT;
						pData_ApplicationHelp->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_HELP_LOW_RESOLUTION.bmp") );
						pData_ApplicationHelp->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Application Help") );
						pData_ApplicationHelp->ParmAppend( __RCGI_PARM_FLAT_COMMAND_BUTTON_TEXT_AT_RIGHT_DESCRIPTION, _T("Get help using application.") );
						RcgtdItemKeyTipSet( htiCommand_ApplicationHelp, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("H") ) ), false );

						HTREEITEM htiCommand_GettingStarted = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiCommand_GettingStarted != NULL );
						CExtRichGenItemData * pData_GettingStarted = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_GettingStarted );
						ASSERT( pData_GettingStarted != NULL );
						pData_GettingStarted->m_nCmdID = 39113;
						pData_GettingStarted->m_nItemType = __BSVIT_COMMAND_BUTTON_WIDE_FLAT;
						pData_GettingStarted->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_ORANGE_DOT_LOW_RESOLUTION.bmp") );
						pData_GettingStarted->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Getting Started") );
						pData_GettingStarted->ParmAppend( __RCGI_PARM_FLAT_COMMAND_BUTTON_TEXT_AT_RIGHT_DESCRIPTION, _T("See what\'s new and find resources to help you<br>learn the basics quickly.") );
						RcgtdItemKeyTipSet( htiCommand_GettingStarted, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("S") ) ), false );

						HTREEITEM htiCommand_ApplicationOnLine = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiCommand_ApplicationOnLine != NULL );
						CExtRichGenItemData * pData_ApplicationOnLine = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_ApplicationOnLine );
						ASSERT( pData_ApplicationOnLine != NULL );
						pData_ApplicationOnLine->m_nCmdID = 39114;
						pData_ApplicationOnLine->m_nItemType = __BSVIT_COMMAND_BUTTON_WIDE_FLAT;
						pData_ApplicationOnLine->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_ORANGE_DOT_LOW_RESOLUTION.bmp") );
						pData_ApplicationOnLine->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Application Online") );
						pData_ApplicationOnLine->ParmAppend( __RCGI_PARM_FLAT_COMMAND_BUTTON_TEXT_AT_RIGHT_DESCRIPTION, _T("Get free product updates, images, templates,<br>help, and online services.") );
						RcgtdItemKeyTipSet( htiCommand_ApplicationOnLine, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("G") ) ), false );

						HTREEITEM htiCommand_ContactUs = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiCommand_ContactUs != NULL );
						CExtRichGenItemData * pData_ContactUs = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_ContactUs );
						ASSERT( pData_ContactUs != NULL );
						pData_ContactUs->m_nCmdID = 39115;
						pData_ContactUs->m_nItemType = __BSVIT_COMMAND_BUTTON_WIDE_FLAT;
						pData_ContactUs->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_CONTACT_US.bmp") );
						pData_ContactUs->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Contact Us") );
						pData_ContactUs->ParmAppend( __RCGI_PARM_FLAT_COMMAND_BUTTON_TEXT_AT_RIGHT_DESCRIPTION, _T("Let us know if you need help or how we can make<br>application better.") );
						RcgtdItemKeyTipSet( htiCommand_ContactUs, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("C") ) ), false );

						htiCaption = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiCaption != NULL );
						pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
						ASSERT( pData_Caption != NULL );
						pData_Caption->m_nItemType = __BSVIT_CAPTION;
						pData_Caption->m_strHtmlInner = _T("Tools for Working With Office");

						htiSeparator = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiSeparator != NULL );
						pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
						ASSERT( pData_Separator != NULL );
						pData_Separator->m_nItemType = __BSVIT_SEPARATOR;

						HTREEITEM htiCommand_ApplicationOptions = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiCommand_ApplicationOptions != NULL );
						CExtRichGenItemData * pData_ApplicationOptions = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_ApplicationOptions );
						ASSERT( pData_ApplicationOptions != NULL );
						pData_ApplicationOptions->m_nCmdID = 39116;
						pData_ApplicationOptions->m_nItemType = __BSVIT_COMMAND_BUTTON_WIDE_FLAT;
						pData_ApplicationOptions->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_OPTIONS_LOW_RESOLUTION.bmp") );
						pData_ApplicationOptions->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Options") );
						pData_ApplicationOptions->ParmAppend( __RCGI_PARM_FLAT_COMMAND_BUTTON_TEXT_AT_RIGHT_DESCRIPTION, _T("Customize language, display, and other program<br>settings.") );
						RcgtdItemKeyTipSet( htiCommand_ApplicationOptions, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("O") ) ), false );

						HTREEITEM htiCommand_CheckForUpdates = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiCommand_CheckForUpdates != NULL );
						CExtRichGenItemData * pData_CheckForUpdates = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_CheckForUpdates );
						ASSERT( pData_CheckForUpdates != NULL );
						pData_CheckForUpdates->m_nCmdID = 39117;
						pData_CheckForUpdates->m_nItemType = __BSVIT_COMMAND_BUTTON_WIDE_FLAT;
						pData_CheckForUpdates->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_CHECK_FOR_UPDATES.bmp") );
						pData_CheckForUpdates->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Check for Updates") );
						pData_CheckForUpdates->ParmAppend( __RCGI_PARM_FLAT_COMMAND_BUTTON_TEXT_AT_RIGHT_DESCRIPTION, _T("Get the latest updates available<br>for application.") );
						RcgtdItemKeyTipSet( htiCommand_CheckForUpdates, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("U") ) ), false );

						HTREEITEM htiCommand_SystemInformationTool = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiCommand_SystemInformationTool != NULL );
						CExtRichGenItemData * pData_SystemInformationTool = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_SystemInformationTool );
						ASSERT( pData_SystemInformationTool != NULL );
						pData_SystemInformationTool->m_nCmdID = 39118;
						pData_SystemInformationTool->m_nItemType = __BSVIT_COMMAND_BUTTON_WIDE_FLAT;
						pData_SystemInformationTool->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_SYSTEM_INFORMATION_TOOL.bmp") );
						pData_SystemInformationTool->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("System Information Tool") );
						pData_SystemInformationTool->ParmAppend( __RCGI_PARM_FLAT_COMMAND_BUTTON_TEXT_AT_RIGHT_DESCRIPTION, _T("Get the summary of your system including<br>hardware resources, components, and software<br>environment.") );
						RcgtdItemKeyTipSet( htiCommand_SystemInformationTool, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("I") ) ), false );

						htiDirectContent = RcgtdItemInsert( htiScrollContainer );
						ASSERT( htiDirectContent != NULL );
						pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
						ASSERT( pData_DirectContent != NULL );
						pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
						pData_DirectContent->m_strHtmlInner = _T("<p style=\" display:inline-block; height:3em; width:100%; \" ></p>");

			//	HTREEITEM htiTabPane_Application_Right = htiTabPane_Application;
				HTREEITEM htiTabPane_Application_Right = RcgtdItemInsert( htiTabPane_Application );
				ASSERT( htiTabPane_Application_Right != NULL );
				CExtRichGenItemData * pData_TabPane_Application_Right = (CExtRichGenItemData*)RcgtdItemDataGet( htiTabPane_Application_Right );
				pData_TabPane_Application_Right->m_nItemType = __BSVIT_RIGHT_PANE_DARK;

					htiGroup = RcgtdItemInsert( htiTabPane_Application_Right );
					ASSERT( htiGroup != NULL );
					pData_Group = (CExtRichGenItemData*)RcgtdItemDataGet( htiGroup );
					ASSERT( pData_Group != NULL );
					pData_Group->m_nItemType = __BSVIT_ELEMENT_GROUP_ATTENTION_LOW;

						htiTable = RcgtdItemInsert( htiGroup );
						ASSERT( htiTable != NULL );
						pData_Table = (CExtRichGenItemData*)RcgtdItemDataGet( htiTable );
						ASSERT( pData_Table != NULL );
						pData_Table->m_nItemType = __BSVIT_TABLE;
						pData_Table->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" border-spacing:0.3em; \" ") );

							htiTableRow = RcgtdItemInsert( htiTable );
							ASSERT( htiTableRow != NULL );
							pData_TableRow = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableRow );
							ASSERT( pData_TableRow != NULL );
							pData_TableRow->m_nItemType = __BSVIT_TABLE_ROW;

								htiTableCell = RcgtdItemInsert( htiTableRow );
								ASSERT( htiTableCell != NULL );
								pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
								ASSERT( pData_TableCell != NULL );
								pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

									HTREEITEM htiCommand_ChangeProductKey = RcgtdItemInsert( htiTableCell );
									ASSERT( htiCommand_ChangeProductKey != NULL );
									CExtRichGenItemData * pData_ChangeProductKey = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_ChangeProductKey );
									ASSERT( pData_ChangeProductKey != NULL );
									pData_ChangeProductKey->m_nCmdID = 39119;
									pData_ChangeProductKey->m_nItemType = __BSVIT_COMMAND_BUTTON_SQUARE;
									pData_ChangeProductKey->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_CHANGE_PRODUCT_KEY.bmp") );
									pData_ChangeProductKey->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Change<br>Product Key") );
									RcgtdItemKeyTipSet( htiCommand_ChangeProductKey, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("Y1") ) ), false );

								htiTableCell = RcgtdItemInsert( htiTableRow );
								ASSERT( htiTableCell != NULL );
								pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
								ASSERT( pData_TableCell != NULL );
								pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

									htiCaption = RcgtdItemInsert( htiTableCell );
									ASSERT( htiCaption != NULL );
									pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
									ASSERT( pData_Caption != NULL );
									pData_Caption->m_nItemType = __BSVIT_CAPTION_ATTENTION_LOW;
									pData_Caption->m_strHtmlInner = _T("Product Activation Required");
									pData_Caption->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin-left:0.3em; \" ") );

									{ // BLOCK: for inner table
										HTREEITEM htiTable = NULL; htiTable;
										CExtRichGenItemData * pData_Table = NULL; pData_Table;
										htiTable = RcgtdItemInsert( htiTableCell );
										ASSERT( htiTable != NULL );
										pData_Table = (CExtRichGenItemData*)RcgtdItemDataGet( htiTable );
										ASSERT( pData_Table != NULL );
										pData_Table->m_nItemType = __BSVIT_TABLE;
										pData_Table->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" border-spacing:0.3em; \" ") );

											htiTableRow = RcgtdItemInsert( htiTable );
											ASSERT( htiTableRow != NULL );
											pData_TableRow = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableRow );
											ASSERT( pData_TableRow != NULL );
											pData_TableRow->m_nItemType = __BSVIT_TABLE_ROW;

												htiTableCell = RcgtdItemInsert( htiTableRow );
												ASSERT( htiTableCell != NULL );
												pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
												ASSERT( pData_TableCell != NULL );
												pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

													htiDirectContent = RcgtdItemInsert( htiTableCell );
													ASSERT( htiDirectContent != NULL );
													pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
													ASSERT( pData_DirectContent != NULL );
													pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
													pData_DirectContent->m_strHtmlInner = _T("<img src=\"icon_ID_BUTTON_CHANGE_PRODUCT_KEY.bmp\"/>");

												htiTableCell = RcgtdItemInsert( htiTableRow );
												ASSERT( htiTableCell != NULL );
												pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
												ASSERT( pData_TableCell != NULL );
												pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;

													htiDirectContent = RcgtdItemInsert( htiTableCell );
													ASSERT( htiDirectContent != NULL );
													pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
													ASSERT( pData_DirectContent != NULL );
													pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
													pData_DirectContent->m_strHtmlInner =
														_T("<p><b>Application Name (and, of course, application icon)</b></p>\r\n")
														_T("<p width=\"400\" style=\" margin-left:2em; \" >Some text description. Some text description. Some text description. Some text description. ")
														_T("   Some text description. Some text description. Some text description. Some text description. ")
														_T("   Some text description. Some text description. Some text description. Some text description. ")
														_T("   Some text description. Some text description. Some text description. Some text description. </p>\r\n")
														;

											htiTableRow = RcgtdItemInsert( htiTable );
											ASSERT( htiTableRow != NULL );
											pData_TableRow = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableRow );
											ASSERT( pData_TableRow != NULL );
											pData_TableRow->m_nItemType = __BSVIT_TABLE_ROW;

												htiTableCell = RcgtdItemInsert( htiTableRow );
												ASSERT( htiTableCell != NULL );
												pData_TableCell = (CExtRichGenItemData*)RcgtdItemDataGet( htiTableCell );
												ASSERT( pData_TableCell != NULL );
												pData_TableCell->m_nItemType = __BSVIT_TABLE_CELL;
												pData_TableCell->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" colspan=\"2\" ") );

													htiDirectContent = RcgtdItemInsert( htiTableCell );
													ASSERT( htiDirectContent != NULL );
													pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
													ASSERT( pData_DirectContent != NULL );
													pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
													pData_DirectContent->m_strHtmlInner =
														_T("<p width=\"70%\" >")
														_T("<img src=\"icon_ID_BUTTON_SMALL_WARNING_ICON.bmp\" style=\" float:left; border-width:0.3em; ui-draw-method:center; \" />")
														_T("Connect to your corporate network or contact your system administrator for futher assistance.")
														_T("</p>\r\n")
														;
									} // BLOCK: for inner table

					htiGroup = RcgtdItemInsert( htiTabPane_Application_Right );
					ASSERT( htiGroup != NULL );
					pData_Group = (CExtRichGenItemData*)RcgtdItemDataGet( htiGroup );
					ASSERT( pData_Group != NULL );
					pData_Group->m_nItemType = __BSVIT_ELEMENT_GROUP;

						htiCaption = RcgtdItemInsert( htiGroup );
						ASSERT( htiCaption != NULL );
						pData_Caption = (CExtRichGenItemData*)RcgtdItemDataGet( htiCaption );
						ASSERT( pData_Caption != NULL );
						pData_Caption->m_nItemType = __BSVIT_CAPTION;
						pData_Caption->m_strHtmlInner = _T("About Application (additional information)");

						htiSeparator = RcgtdItemInsert( htiGroup );
						ASSERT( htiSeparator != NULL );
						pData_Separator = (CExtRichGenItemData*)RcgtdItemDataGet( htiSeparator );
						ASSERT( pData_Separator != NULL );
						pData_Separator->m_nItemType = __BSVIT_SEPARATOR;
						pData_Separator->ParmAppend( __RCGI_PARM_TAG_PARAMETERS, _T(" style=\" margin:5px; height:12px \" ") );

						htiDirectContent = RcgtdItemInsert( htiGroup );
						ASSERT( htiDirectContent != NULL );
						pData_DirectContent = (CExtRichGenItemData*)RcgtdItemDataGet( htiDirectContent );
						ASSERT( pData_DirectContent != NULL );
						pData_DirectContent->m_nItemType = __BSVIT_DIRECT_CONTENT;
						pData_DirectContent->m_strHtmlInner =
							_T("<p style=\" margin-top:0.4em; \" >Application version: XXXX.XXXX.XXXX.XXXX (XX-bit)</p>\r\n")
							_T("<p style=\" margin-top:0.4em; \" ><a href=\"http://www.prof-uis.com\">Additional Version and Copyright information.</a></p>\r\n")
							_T("<p style=\" margin-top:0.4em; \" >Part of the XXXX XXXX XXXX XXXX (xxxx xxxx xxxx xxxx) product.</p>\r\n")
							_T("<p style=\" margin-top:0.4em; \" >&copy;201X Company Name. All rights reserved.</p>\r\n")
							_T("<p style=\" margin-top:0.4em; \" ><a href=\"http://www.prof-uis.com\">Company Product Support Services.</a></p>\r\n")
							_T("<p style=\" margin-top:0.4em; \" >Product ID: 1111-2222-3333-4444-5555-6666-7777</p>\r\n")
							_T("<p style=\" margin-top:0.4em; \" ><a href=\"http://www.prof-uis.com\">Company Software License Terms.</a></p>\r\n")
							;

HTREEITEM htiCommand_Options = RcgtdItemInsert( htiRoot );
	ASSERT( htiCommand_Options != NULL );
CExtRichGenItemData * pData_Options = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_Options );
	ASSERT( pData_Options != NULL );
	pData_Options->m_nCmdID = IDD_EXT_RIBBON_OPTIONS_DIALOG;
	pData_Options->m_nItemType = __BSVIT_COMMAND_BUTTON_TOP_LEVEL;
	pData_Options->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_TOP_LEVEL_OPTIONS.bmp") );
	pData_Options->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Options") );
	RcgtdItemKeyTipSet( htiCommand_Options, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("I") ) ), false );

HTREEITEM htiCommand_Exit = RcgtdItemInsert( htiRoot );
	ASSERT( htiCommand_Exit != NULL );
CExtRichGenItemData * pData_Exit = (CExtRichGenItemData*)RcgtdItemDataGet( htiCommand_Exit );
	ASSERT( pData_Exit != NULL );
	pData_Exit->m_nCmdID = ID_APP_EXIT;
	pData_Exit->m_nItemType = __BSVIT_COMMAND_BUTTON_TOP_LEVEL;
	pData_Exit->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_ICON_IMAGE, _T("icon_ID_BUTTON_TOP_LEVEL_EXIT.bmp") );
	pData_Exit->ParmAppend( __RCGI_PARM_COMMAND_BUTTON_CAPTION, _T("Exit") );
	RcgtdItemKeyTipSet( htiCommand_Exit, new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("X") ) ), false );
}

void CMyBackstageViewWnd::OnBsvItemInvokeDropDownMenu( HTREEITEM hti, CExtRichGenTreeData::e_item_invoke_type eIIT )
{
	ASSERT_VALID( this );
	ASSERT( hti != NULL );
	ASSERT( RcgtdItemIsPresent( hti ) );
	if(	!	(	eIIT == CExtRichGenTreeData::__E_RCTD_IIT_PROGRAMMATIC
			||	eIIT == CExtRichGenTreeData::__E_RCTD_IIT_MOUSE_LBUTTON_DOWN
			||	eIIT == CExtRichGenTreeData::__E_RCTD_IIT_KEY_VK_SPACE_DOWN
			||	eIIT == CExtRichGenTreeData::__E_RCTD_IIT_KEY_VK_RETURN_DOWN
			||	eIIT == CExtRichGenTreeData::__E_RCTD_IIT_KEY_TIP
			)
		)
		return;
	CExtPopupMenuWnd::CancelMenuTracking();
	RcgwTreeItemFocus( hti );
CExtRichContentLayout * pRCL = RcsLayoutGet();
	ASSERT( pRCL != NULL );
CExtSafeString strID;
	strID.Format( _T("%p"), LPVOID(hti) );
CExtRichContentItem * pRCI = pRCL->ElementByUiBindingSrcID( LPCTSTR(strID) );
	ASSERT( pRCI != NULL );
CRect rcExclude = pRCI->CalcEffectiveRect();
	ClientToScreen( &rcExclude );
CPoint ptTrack = rcExclude.CenterPoint();
HWND hWndOwn = GetSafeHwnd();
HWND hWndTraget = GetOwner()->GetSafeHwnd();
	if( hWndTraget == NULL )
		hWndTraget = hWndOwn;
CExtPopupMenuWnd * pPopup =
		CExtPopupMenuWnd::InstantiatePopupMenu(
			hWndTraget,
			RUNTIME_CLASS(CExtPopupMenuWnd),
			this
			);
	if( ! pPopup->CreatePopupMenu( GetSafeHwnd() ) )
	{
		delete pPopup;
		return;
	}
	pPopup->m_hWndNotifyMenuClosed = hWndOwn;
DWORD dwTrackFlags = TPMX_TOPALIGN|TPMX_OWNERDRAW_FIXED|TPMX_RIBBON_MODE;
	if( eIIT == CExtRichGenTreeData::__E_RCTD_IIT_KEY_TIP )
	{
		OnRcgtdKeyTipTrackingStop();
		dwTrackFlags |= TPMX_RIBBON_KEYTIPS;
	}
	else if(
			eIIT == CExtRichGenTreeData::__E_RCTD_IIT_KEY_VK_SPACE_DOWN
		||	eIIT == CExtRichGenTreeData::__E_RCTD_IIT_KEY_VK_RETURN_DOWN
		)
		dwTrackFlags |= TPMX_SELECT_ANY;
CExtCmdIcon _icon;
	_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_BUTTON_SMALL_ORANGE_DOT) );
	for( int i = 0; i < 5; i ++ )
	{
		CExtSafeString strMenuItemText, strExtendedText;
		strMenuItemText.Format( _T("Menu command %d"), i + 1 );
		strExtendedText.Format( _T("<html><p style=\"margin-left:1em;\">Some description text for<br><i>menu command %d</i> goes here.</p></html>"), i + 1 );
		VERIFY( pPopup->ItemInsertCommand( 42000 + 1, -1, strMenuItemText ) );
		CExtPopupMenuWnd::MENUITEMDATA & _mii = pPopup->ItemGetInfo( pPopup->ItemGetCount() - 1 );
		_mii.SetPopupIcon( _icon );
		CExtCustomizeCmdKeyTip * pCCKT = _mii.KeyTipGetInfo( true );
		ASSERT( pCCKT != NULL );
		pCCKT->KeyCodeAdd( DWORD('1') + DWORD(i) );
		_mii.SetExtendedText( LPCTSTR(strExtendedText) );
		_mii.SetBold();
	}
	pPopup->_SyncItems();
	if( ! pPopup->TrackPopupMenu(
			dwTrackFlags,
			ptTrack.x,
			ptTrack.y,
			&rcExclude
			)
		)
		return;
}

#endif // (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)

/////////////////////////////////////////////////////////////////////////////
// CMyRibbonBar

CMyRibbonBar::CMyRibbonBar()
{
	::GetLocaleInfo(
		::GetSystemDefaultLCID(),
		LOCALE_SDECIMAL,
		m_sDecimalSeparator.GetBuffer( 64 ),
		63
		);
	m_sDecimalSeparator.ReleaseBuffer();
	m_sDecimalSeparator.TrimLeft( _T(" \r\n\t") );
	m_sDecimalSeparator.TrimRight( _T(" \r\n\t") );
	if( m_sDecimalSeparator.GetLength() == 0 )
		m_sDecimalSeparator = _T('.');

	m_hIconFontTT = AfxGetApp()->LoadIcon(IDI_ICON_FONT_TT);
	ASSERT( m_hIconFontTT != NULL );
	m_hIconFontPix = AfxGetApp()->LoadIcon(IDI_ICON_FONT_PIX);
	ASSERT( m_hIconFontPix != NULL );

	m_sCurrentFontFaceName = _T("Arial");
	m_lfCurrentFontSize = 12.0f;

	m_clrFontColor = RGB(255,0,0);
	m_clrTextHighlightColor = RGB(255,255,0);

#if (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)
	CExtRibbonBar::m_pBackstageViewWnd = &m_BSV;
#endif // (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)
}

CMyRibbonBar::~CMyRibbonBar()
{
	::DestroyIcon( m_hIconFontTT );
	::DestroyIcon( m_hIconFontPix );
}

int CALLBACK CMyRibbonBar::stat_FillFontListBoxProc(
	ENUMLOGFONTEX * pEnumLogFontEx,
	NEWTEXTMETRICEX * pNewTextMetriCEx, 
	int nFontType,
	CListBox * pListBox
	)
{
	pNewTextMetriCEx;
	nFontType;
bool bAt =
		( pEnumLogFontEx->elfLogFont.lfFaceName[0] == _T('@') )
			? true : false;
	if( bAt )
		return 1; // current version hides tool fonts
LPCTSTR sFaceName =
		bAt
			? ((LPCTSTR)&(pEnumLogFontEx->elfLogFont.lfFaceName))+1
			: pEnumLogFontEx->elfLogFont.lfFaceName
			;
    if( pListBox->FindStringExact( 0, sFaceName ) == CB_ERR )
	{
		int nItemIdx = pListBox->AddString( sFaceName );
		DWORD dwItemData =
				( bAt ? 1L : 0L )
			|	DWORD( pEnumLogFontEx->elfLogFont.lfPitchAndFamily ) << 1
			|	DWORD( pEnumLogFontEx->elfLogFont.lfOutPrecision ) << 9
			|	DWORD( pEnumLogFontEx->elfLogFont.lfCharSet) << 17
			;
		pListBox->SetItemData( nItemIdx, dwItemData );
	} // if( pListBox->FindStringExact( 0, sFaceName ) == CB_ERR )
    return 1;
}

void CMyRibbonBar::OnRibbonGalleryInitContent(
	CExtRibbonGalleryWnd & wndRG,
	CExtRibbonGalleryPopupMenuWnd * pGalleryPopup,
	CExtRibbonButtonGallery * pRibbonGalleryTBB
	)
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
	ASSERT_VALID( (&wndRG) );
	ASSERT( wndRG.GetSafeHwnd() != NULL );
#ifdef _DEBUG
	if( pGalleryPopup != NULL )
	{
		ASSERT_VALID( pGalleryPopup );
		ASSERT( pGalleryPopup->GetSafeHwnd() != NULL );
	}
#endif // _DEBUG
UINT nCmdID = 0;
	if( pRibbonGalleryTBB != NULL )
	{
		ASSERT_VALID( pRibbonGalleryTBB );
		nCmdID = pRibbonGalleryTBB->GetCmdID();
	} // if( pRibbonGalleryTBB != NULL )
	else
	{
		ASSERT( pGalleryPopup != NULL );
		CExtCustomizeCmdTreeNode * pNode = pGalleryPopup->_CmdNodeGet();
		ASSERT_VALID( pNode );
		nCmdID = pNode->GetCmdID();
	}
	switch( nCmdID )
	{
	case ID_QUICK_STYLES_BIG:
		{
			wndRG.ModifyToolBoxWndStyle( __TBWS_ZERO_HEIGHT_CAPTIONS );
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup =
				wndRG.ItemInsert( NULL, _T("Gallery group"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup != NULL );
			CExtBitmap _bmp;
			VERIFY( _bmp.LoadBMP_Resource( MAKEINTRESOURCE(ID_RIBBON_GALLERY_STYLES) ) );
			CSize _sizeBmp = _bmp.GetSize();
			static const INT nItemWidth = 64;
			CRect rcItem( 0, 0, nItemWidth, _sizeBmp.cy );
			INT nItemIndex, nItemCount = _sizeBmp.cx / nItemWidth;
			for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			{
				CExtCmdIcon _icon;
				_icon.m_bmpNormal.FromBitmap( _bmp, &rcItem );
				rcItem.OffsetRect( nItemWidth, 0 );
				CExtSafeString str;
				str.Format( _T("Style %d"), nItemIndex + 1 );
				CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryItem =
					wndRG.ItemInsert( pTBCI_GalleryGroup, str, &_icon );
				ASSERT( pTBCI_GalleryItem != NULL );
				if( nItemIndex == 0 )
					pTBCI_GalleryItem->ModifyItemStyle( __TBWI_SELECTED );
			} // for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			pTBCI_GalleryGroup->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			wndRG.ItemExpand( pTBCI_GalleryGroup );
			wndRG.ItemSetActive( pTBCI_GalleryGroup );
			wndRG.UpdateToolBoxWnd( true );
		}
		break; // case ID_QUICK_STYLES_BIG
	case ID_FONT_UNDERLINE:
		{
			wndRG.ModifyToolBoxWndStyle( __TBWS_ZERO_HEIGHT_CAPTIONS );
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup =
				wndRG.ItemInsert( NULL, _T("Gallery group"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup != NULL );
			CExtBitmap _bmp;
			VERIFY( _bmp.LoadBMP_Resource( MAKEINTRESOURCE(ID_RIBBON_GALLERY_UNDERLINES) ) );
			CSize _sizeBmp = _bmp.GetSize();
			static const INT nItemHeight = 26;
			CRect rcItem( 0, 0, _sizeBmp.cx, nItemHeight );
			INT nItemIndex, nItemCount = _sizeBmp.cy / nItemHeight;
			for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			{
				CExtCmdIcon _icon;
				_icon.m_bmpNormal.FromBitmap( _bmp, &rcItem );
				rcItem.OffsetRect( 0, nItemHeight );
				CExtSafeString str;
				str.Format( _T("Underline Style %d"), nItemIndex + 1 );
				CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryItem =
					wndRG.ItemInsert( pTBCI_GalleryGroup, str, &_icon );
				ASSERT( pTBCI_GalleryItem != NULL );
				if( nItemIndex == 0 )
					pTBCI_GalleryItem->ModifyItemStyle( __TBWI_SELECTED );
			} // for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			pTBCI_GalleryGroup->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			wndRG.ItemExpand( pTBCI_GalleryGroup );
			wndRG.ItemSetActive( pTBCI_GalleryGroup );
			wndRG.UpdateToolBoxWnd( true );
		}
		break; // case ID_FONT_UNDERLINE
	case ID_PARAGRAPH_BULLETS:
		{
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup0 =
				wndRG.ItemInsert( NULL, _T("Recently Used Bullets"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup0 != NULL );
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup1 =
				wndRG.ItemInsert( NULL, _T("Bullet Library"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup1 != NULL );
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup2 =
				wndRG.ItemInsert( NULL, _T("Document Bullets"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup2 != NULL );
			CExtBitmap _bmp;
			VERIFY( _bmp.LoadBMP_Resource( MAKEINTRESOURCE(ID_RIBBON_GALLERY_BULLETS) ) );
			CSize _sizeBmp = _bmp.GetSize();
			static const INT nItemWidth = 40;
			CRect rcItem( 0, 0, nItemWidth, _sizeBmp.cy );
			INT nItemIndex, nItemCount = _sizeBmp.cx / nItemWidth;
			for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			{
				CExtCmdIcon _icon;
				_icon.m_bmpNormal.FromBitmap( _bmp, &rcItem );
				rcItem.OffsetRect( nItemWidth, 0 );
				CExtSafeString str;
				if( nItemIndex == 0 )
					str = _T("None");
				else
					str.Format( _T("Bullet %d"), nItemIndex + 1 );

				CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryItem =
					wndRG.ItemInsert( pTBCI_GalleryGroup1, str, &_icon );
				ASSERT( pTBCI_GalleryItem != NULL );
				if( nItemIndex == 0 )
					pTBCI_GalleryItem->ModifyItemStyle( __TBWI_SELECTED );

				if( nItemIndex == 1 )
				{
					pTBCI_GalleryItem =
						wndRG.ItemInsert( pTBCI_GalleryGroup0, str, &_icon );
					ASSERT( pTBCI_GalleryItem != NULL );

					pTBCI_GalleryItem =
						wndRG.ItemInsert( pTBCI_GalleryGroup2, str, &_icon );
					ASSERT( pTBCI_GalleryItem != NULL );
				}
			} // for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			pTBCI_GalleryGroup0->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			pTBCI_GalleryGroup1->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			pTBCI_GalleryGroup2->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			wndRG.ItemExpand( pTBCI_GalleryGroup0 );
			wndRG.ItemExpand( pTBCI_GalleryGroup1 );
			wndRG.ItemExpand( pTBCI_GalleryGroup2 );
			wndRG.ItemSetActive( pTBCI_GalleryGroup1 );
			wndRG.UpdateToolBoxWnd( true );
		}
		break; // case ID_PARAGRAPH_BULLETS
	case ID_PARAGRAPH_NUMBERING:
		{
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup0 =
				wndRG.ItemInsert( NULL, _T("Recently Used Number Formats"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup0 != NULL );
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup1 =
				wndRG.ItemInsert( NULL, _T("Numbering Library"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup1 != NULL );
			CExtBitmap _bmp;
			VERIFY( _bmp.LoadBMP_Resource( MAKEINTRESOURCE(ID_RIBBON_GALLERY_NUMBERING) ) );
			CSize _sizeBmp = _bmp.GetSize();
			static const INT nItemWidth = 76;
			CRect rcItem( 0, 0, nItemWidth, _sizeBmp.cy );
			INT nItemIndex, nItemCount = _sizeBmp.cx / nItemWidth;
			for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			{
				CExtCmdIcon _icon;
				_icon.m_bmpNormal.FromBitmap( _bmp, &rcItem );
				rcItem.OffsetRect( nItemWidth, 0 );
				CExtSafeString str;
				if( nItemIndex == 0 )
					str = _T("None");
				else
					str.Format( _T("Number Format %d"), nItemIndex + 1 );

				CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryItem =
					wndRG.ItemInsert( pTBCI_GalleryGroup1, str, &_icon );
				ASSERT( pTBCI_GalleryItem != NULL );
				if( nItemIndex == 0 )
					pTBCI_GalleryItem->ModifyItemStyle( __TBWI_SELECTED );

				if( nItemIndex == 1 )
				{
					pTBCI_GalleryItem =
						wndRG.ItemInsert( pTBCI_GalleryGroup0, str, &_icon );
					ASSERT( pTBCI_GalleryItem != NULL );
				}
			} // for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			pTBCI_GalleryGroup0->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			pTBCI_GalleryGroup1->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			wndRG.ItemExpand( pTBCI_GalleryGroup0 );
			wndRG.ItemExpand( pTBCI_GalleryGroup1 );
			wndRG.ItemSetActive( pTBCI_GalleryGroup1 );
			wndRG.UpdateToolBoxWnd( true );
		}
		break; // case ID_PARAGRAPH_NUMBERING
	case ID_PARAGRAPH_MULTILEVEL_LIST:
		{
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup0 =
				wndRG.ItemInsert( NULL, _T("Current List"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup0 != NULL );
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup1 =
				wndRG.ItemInsert( NULL, _T("My Lists"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup1 != NULL );
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup2 =
				wndRG.ItemInsert( NULL, _T("Lists in Current Document"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup2 != NULL );
			CExtBitmap _bmp;
			VERIFY( _bmp.LoadBMP_Resource( MAKEINTRESOURCE(ID_RIBBON_GALLERY_MULTILEVEL_LIST) ) );
			CSize _sizeBmp = _bmp.GetSize();
			static const INT nItemWidth = 76;
			CRect rcItem( 0, 0, nItemWidth, _sizeBmp.cy );
			INT nItemIndex, nItemCount = _sizeBmp.cx / nItemWidth;
			for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			{
				CExtCmdIcon _icon;
				_icon.m_bmpNormal.FromBitmap( _bmp, &rcItem );
				rcItem.OffsetRect( nItemWidth, 0 );
				CExtSafeString str;
				if( nItemIndex == 0 )
					str = _T("None");
				else
					str.Format( _T("List Format %d"), nItemIndex + 1 );

				CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryItem =
					wndRG.ItemInsert( pTBCI_GalleryGroup1, str, &_icon );
				ASSERT( pTBCI_GalleryItem != NULL );
				if( nItemIndex == 0 )
					pTBCI_GalleryItem->ModifyItemStyle( __TBWI_SELECTED );

				if( nItemIndex == 1 )
				{
					pTBCI_GalleryItem =
						wndRG.ItemInsert( pTBCI_GalleryGroup0, str, &_icon );
					ASSERT( pTBCI_GalleryItem != NULL );

					pTBCI_GalleryItem =
						wndRG.ItemInsert( pTBCI_GalleryGroup2, str, &_icon );
					ASSERT( pTBCI_GalleryItem != NULL );
				}
			} // for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			pTBCI_GalleryGroup0->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			pTBCI_GalleryGroup1->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			pTBCI_GalleryGroup2->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			wndRG.ItemExpand( pTBCI_GalleryGroup0 );
			wndRG.ItemExpand( pTBCI_GalleryGroup1 );
			wndRG.ItemExpand( pTBCI_GalleryGroup2 );
			wndRG.ItemSetActive( pTBCI_GalleryGroup1 );
			wndRG.UpdateToolBoxWnd( true );
		}
		break; // case ID_PARAGRAPH_MULTILEVEL_LIST
	case ID_CHANGE_LIST_LEVEL:
		{
			wndRG.ModifyToolBoxWndStyle( __TBWS_ZERO_HEIGHT_CAPTIONS );
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup =
				wndRG.ItemInsert( NULL, _T("Gallery group"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup != NULL );
			CExtBitmap _bmp;
			VERIFY( _bmp.LoadBMP_Resource( MAKEINTRESOURCE(ID_RIBBON_GALLERY_CHANGE_LIST_LEVEL) ) );
			CSize _sizeBmp = _bmp.GetSize();
			static const INT nItemHeight = 30;
			CRect rcItem( 0, 0, _sizeBmp.cx, nItemHeight );
			INT nItemIndex, nItemCount = _sizeBmp.cy / nItemHeight;
			for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			{
				CExtCmdIcon _icon;
				_icon.m_bmpNormal.FromBitmap( _bmp, &rcItem );
				rcItem.OffsetRect( 0, nItemHeight );
				CExtSafeString str;
				str.Format( _T("Level %d"), nItemIndex + 1 );
				CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryItem =
					wndRG.ItemInsert( pTBCI_GalleryGroup, str, &_icon );
				ASSERT( pTBCI_GalleryItem != NULL );
				if( nItemIndex == 0 )
					pTBCI_GalleryItem->ModifyItemStyle( __TBWI_SELECTED );
			} // for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			pTBCI_GalleryGroup->ModifyItemStyle( 0, __TBWI_LISTBOX_VIEW );
			wndRG.ItemExpand( pTBCI_GalleryGroup );
			wndRG.ItemSetActive( pTBCI_GalleryGroup );
			wndRG.UpdateToolBoxWnd( true );
		}
		break; // case ID_CHANGE_LIST_LEVEL
	default:
		if( (pGalleryPopup->TrackFlagsGet()&TPMX_RIBBON_FILE_MENU) )
		{
			CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryGroup =
				wndRG.ItemInsert( NULL, _T("Recent Documents"), (HICON) NULL );
			ASSERT( pTBCI_GalleryGroup != NULL );
			INT nItemIndex, nItemCount = 5;
			for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			{
				CExtSafeString str;
				str.Format( _T(" %d  Recent Document Name"), nItemIndex + 1 );
				CExtToolBoxWnd::TOOLBOX_ITEM_DATA * pTBCI_GalleryItem =
					wndRG.ItemInsert( pTBCI_GalleryGroup, str, (HICON)NULL );
				ASSERT( pTBCI_GalleryItem != NULL );
				pTBCI_GalleryItem;
			} // for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++ )
			wndRG.ItemExpand( pTBCI_GalleryGroup );
			wndRG.ItemSetActive( pTBCI_GalleryGroup );
			wndRG.UpdateToolBoxWnd( true );
		} // if( (pGalleryPopup->TrackFlagsGet()&TPMX_RIBBON_FILE_MENU) )
		break; // default
	} // switch( nCmdID )
}

void CMyRibbonBar::OnRibbonOptionsDialogInitPageRTCs(
	CExtRibbonOptionsDialog & dlgRibbonOptions
	)
{
	ASSERT_VALID( this );
	CExtRibbonBar::OnRibbonOptionsDialogInitPageRTCs( dlgRibbonOptions );
	dlgRibbonOptions.m_arrPageRTCs.Add(
		RUNTIME_CLASS( CMyUserDefinedRibbonOptionsPage )
		);
}

bool CMyRibbonBar::OnColorItemValueGet(
	CExtBarColorButton * pTBB,
	CExtCustomizeCmdTreeNode * pNode,
	COLORREF & clr,
	bool bSelected
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pNode );
CExtRibbonPage * pRibbonPage = GetMainRibbonPage();
	if( pRibbonPage == NULL )
	{
		UINT nCmdID = pNode->GetCmdID(false);
		switch( nCmdID )
		{
		case ID_FONT_CHAR_COLOR:
			if( bSelected )
				clr = m_clrFontColor;
			else
				clr = RGB(255,0,0);
			break;
		case ID_FONT_TEXT_HIGHLIGHT_COLOR:
			if( bSelected )
				clr = m_clrTextHighlightColor;
			else
				clr = RGB(255,255,0);
			break;
		default:
			return
				CExtRibbonBar::OnColorItemValueGet(
					pTBB,
					pNode,
					clr,
					bSelected
					);
		}
		return true;
	}
	else
		return
			pRibbonPage->OnColorItemValueGet(
				pTBB,
				pNode,
				clr,
				bSelected
				);
}

bool CMyRibbonBar::OnColorItemValueSet(
	CExtBarColorButton * pTBB,
	CExtCustomizeCmdTreeNode * pNode,
	COLORREF & clr,
	bool bSelected
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pNode );
CExtRibbonPage * pRibbonPage = GetMainRibbonPage();
	if( pRibbonPage == NULL )
	{
		UINT nCmdID = pNode->GetCmdID(false);
		switch( nCmdID )
		{
		case ID_FONT_CHAR_COLOR:
			if( clr == COLORREF(-1L) )
				m_clrFontColor = RGB(255,0,0);
			else
				m_clrFontColor = clr;
			break;
		case ID_FONT_TEXT_HIGHLIGHT_COLOR:
			if( clr == COLORREF(-1L) )
				m_clrTextHighlightColor = RGB(255,255,0);
			else
				m_clrTextHighlightColor = clr;
			break;
		default:
			return
				CExtRibbonBar::OnColorItemValueSet(
					pTBB,
					pNode,
					clr,
					bSelected
					);
		}
		CExtRibbonBar::RedrawCommandItems( nCmdID );
		return true;
	}
	else
		return
			pRibbonPage->OnColorItemValueSet(
				pTBB,
				pNode,
				clr,
				bSelected
				);
}

bool CMyRibbonBar::OnColorItemCustom(
	CExtCustomizeCmdTreeNode * pNode
	)
{
	ASSERT_VALID( this );
	ASSERT_VALID( pNode );
CExtRibbonPage * pRibbonPage = GetMainRibbonPage();
	if( pRibbonPage == NULL )
	{
		CExtColorDlg dlgColor;
		UINT nCmdID = pNode->GetCmdID(false);
		switch( nCmdID )
		{
		case ID_FONT_CHAR_COLOR:
			dlgColor.m_clrInit = dlgColor.m_clrNew = m_clrFontColor;
			break;
		case ID_FONT_TEXT_HIGHLIGHT_COLOR:
			dlgColor.m_clrInit = dlgColor.m_clrNew = m_clrTextHighlightColor;
			break;
		default:
			return CExtRibbonBar::OnColorItemCustom( pNode );
		}
		if( dlgColor.DoModal() != IDOK )
			return false;
		switch( nCmdID )
		{
		case ID_FONT_CHAR_COLOR:
			m_clrFontColor = dlgColor.m_clrNew;
			break;
		case ID_FONT_TEXT_HIGHLIGHT_COLOR:
			m_clrTextHighlightColor = dlgColor.m_clrNew;
			break;
		}
		CExtRibbonBar::RedrawCommandItems( nCmdID );
		return true;
	}
	else
		return pRibbonPage->OnColorItemCustom( pNode );
}

CExtBarButton * CMyRibbonBar::OnCreateToolbarButton(
	CExtToolControlBar * pBar,
	CExtCustomizeCmdTreeNode * pNodeI,
	CExtCustomizeCmdTreeNode * pNodeC
	)
{
	ASSERT_VALID( this );
CExtBarButton * pTBB =  CExtRibbonBar::OnCreateToolbarButton( pBar, pNodeI, pNodeC );
	if( pTBB != NULL )
	{
		CExtBarColorButton * pColorTBB = DYNAMIC_DOWNCAST( CExtBarColorButton, pTBB );
		if( pColorTBB != NULL )
		{
			switch( pColorTBB->GetCmdID( false ) )
			{
			case ID_FONT_CHAR_COLOR:
				pColorTBB->IconTypeSet( CExtBarColorButton::__DIT_CHAR_2007 );
			break;
			case ID_FONT_TEXT_HIGHLIGHT_COLOR:
				pColorTBB->IconTypeSet( CExtBarColorButton::__DIT_FILL_2007 );
			break;
			}
		}
	}
	return pTBB;
}

bool CMyRibbonBar::OnPopupListBoxGetStyles(
	CExtBarButton * pTBB,
	CExtCustomizeCmdTreeNode * pNode,
	DWORD & dwListBoxStyles
	) const
{
	ASSERT_VALID( this );
#ifdef _DEBUG
	if( pTBB != NULL )
	{
		ASSERT_VALID( pTBB );
		ASSERT_VALID( pTBB->GetBar() );
	}
#endif // _DEBUG
	ASSERT_VALID( pNode );
UINT nCmdID = pNode->GetCmdID(false);
	if( nCmdID == ID_SE_FONT_LIST )
	{
		dwListBoxStyles |= LBS_SORT;
		return true;
	} // if( nCmdID == ID_SE_FONT_LIST )
	return
		CExtRibbonBar::OnPopupListBoxGetStyles(
			pTBB,
			pNode,
			dwListBoxStyles
			);
}

static void stat_FormatFontSize(
	CExtSafeString & sText,
	LPCTSTR sDecimalSeparator,
	float fFontSize
	)
{
	ASSERT( sDecimalSeparator != NULL );
float fFL = (float)floor(fFontSize);
	if( fFL == fFontSize )
		sText.Format( _T("%.0f"), fFontSize );
	else
	{
//		double fDiv = double(fFontSize) - double(fFL);
//		fDiv *= 10.0;
//		fDiv = ceil(fDiv);
//		int nDiv = (int)fDiv;
//		sText.Format(
//			_T("%.0f%s%d"),
//			fFL,
//			sDecimalSeparator,
//			nDiv
//			);
		sText.Format( _T("%.1f"), fFontSize );
		sText.Replace( __EXT_MFC_SAFE_LPCTSTR( _T(".") ), __EXT_MFC_SAFE_LPCTSTR( sDecimalSeparator ) );
	}
}

bool CMyRibbonBar::OnPopupListBoxInitContent(
	CExtBarButton * pTBB,
	CExtCustomizeCmdTreeNode * pNode,
	CListBox & wndListBox
	)
{
#ifdef _DEBUG
	if( pTBB != NULL )
	{
		ASSERT_VALID( pTBB );
		ASSERT_VALID( pTBB->GetBar() );
	}
#endif // _DEBUG
	ASSERT_VALID( pNode );
	ASSERT_VALID( (&wndListBox) );
	pTBB;
UINT nCmdID = pNode->GetCmdID(false);
	switch( nCmdID )
	{
	case ID_SE_FONT_LIST:
		{
			CClientDC dc( this );
			LOGFONT lf;
			::memset( &lf, 0, sizeof(LOGFONT) );
			lf.lfCharSet = DEFAULT_CHARSET;
			::EnumFontFamiliesEx(
				dc.m_hDC,
				&lf,
				(FONTENUMPROC)stat_FillFontListBoxProc,
				(LPARAM)&wndListBox,
				0
				);
			LPCTSTR sFieldText = (LPCTSTR)pNode->m_sDefInplaceEditBuffer;
			if( sFieldText != NULL && _tcslen(sFieldText) > 0 )
				wndListBox.SelectString( -1, sFieldText );
			return true;
		} // case ID_SE_FONT_LIST
	case ID_SE_SIZE:
		{
			static const float arr_sizes[] = {
				8.0f, 9.0f, 10.0f, 10.5f, 11.0f, 12.0f, 14.0f, 16.0f,
				18.0f, 20.0f, 22.0f, 24.0f, 26.0f, 28.0f, 36.0f, 48.0f, 72.0f
			};
			for(	int nItemIdx = 0;
					nItemIdx < sizeof(arr_sizes)/sizeof(arr_sizes[0]);
					nItemIdx++
					)
			{
				CExtSafeString sText;
				float fFontSize = arr_sizes[nItemIdx];
				ASSERT( __SO_FONT_SIZE_MIN <= fFontSize );
				ASSERT( fFontSize <= __SO_FONT_SIZE_MAX );
				stat_FormatFontSize(
					sText,
					m_sDecimalSeparator,
					fFontSize
					);
				wndListBox.AddString( (LPCTSTR)sText );
				wndListBox.SetItemData( nItemIdx, (DWORD)fFontSize );
			}

			CExtSafeString sFieldText;
				stat_FormatFontSize(
					sFieldText,
					m_sDecimalSeparator,
					m_lfCurrentFontSize
					);
			if( !sFieldText.IsEmpty() )
				wndListBox.SelectString( -1, sFieldText );
			return true;
		} // case ID_SE_FONT_LIST
	} // switch( nCmdID )
	return false;
}

CSize CMyRibbonBar::OnPopupListBoxMeasureTrackSize(
	const CExtBarButton * pTBB,
	const CExtCustomizeCmdTreeNode * pNode
	) const
{
#ifdef _DEBUG
	if( pTBB != NULL )
	{
		ASSERT_VALID( pTBB );
		ASSERT_VALID( pTBB->GetBar() );
	}
#endif // _DEBUG
	ASSERT_VALID( pNode );
	pTBB;
UINT nCmdID = pNode->GetCmdID(false);
	switch( nCmdID )
	{
	case ID_SE_FONT_LIST:
			return CSize( 300, 400 );
	} // switch( nCmdID )
	return CSize( 0, 0 ); // this is not our business, let the bar button compute it
}

bool CMyRibbonBar::OnPopupListBoxItemDraw(
	CExtBarButton * pTBB,
	CExtCustomizeCmdTreeNode * pNode,
	CListBox & wndListBox,
	LPDRAWITEMSTRUCT pDIS
	)
{
	pTBB;
#ifdef _DEBUG
	if( pTBB != NULL )
	{
		ASSERT_VALID( pTBB );
		ASSERT_VALID( pTBB->GetBar() );
	}
#endif // _DEBUG
	ASSERT_VALID( pNode );
	ASSERT_VALID( (&wndListBox) );
	ASSERT( pDIS != NULL && pDIS->hDC != NULL );
	wndListBox;
UINT nCmdID = pNode->GetCmdID(false);
	switch( nCmdID )
	{
	case ID_SE_FONT_LIST:
		{
			ASSERT( (wndListBox.GetStyle() & LBS_HASSTRINGS) != 0 );
			if( ((INT)pDIS->itemID) < 0 || ((INT)pDIS->itemID) >= wndListBox.GetCount() )
				return true;

			CString sText;
			wndListBox.GetText( (INT)pDIS->itemID, sText );
			ASSERT( !sText.IsEmpty() );
			DWORD dwItemData = DWORD( wndListBox.GetItemData( (INT)pDIS->itemID ) );
			bool bSymbols =
				( (( ((DWORD)(dwItemData)) >> 17)&0x0FF) == SYMBOL_CHARSET )
				? true : false;

			HFONT hDefFont = (HFONT)g_PaintManager->m_FontNormal.GetSafeHandle();
			if( hDefFont == NULL )
				hDefFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
			if( hDefFont == NULL )
				hDefFont = (HFONT)::GetStockObject(SYSTEM_FONT);
			ASSERT( hDefFont != NULL );
			LOGFONT lf;
			::memset( &lf, 0, sizeof(LOGFONT) );
			if(	::GetObject(
					(HGDIOBJ)hDefFont,
					sizeof(LOGFONT),
					&lf
					) != sizeof(LOGFONT)
				)
			{
				ASSERT( FALSE );
				return true;
			}
			__EXT_MFC_STRNCPY(
				lf.lfFaceName,
				LF_FACESIZE,
				LPCTSTR(sText),
				LF_FACESIZE
				);
			lf.lfHeight = ::MulDiv( lf.lfHeight, 13, 8 );
			lf.lfOutPrecision	= BYTE( (dwItemData >> 9) & 0x0FF);
			lf.lfCharSet		= BYTE( (dwItemData >> 17) & 0x0FF);
			lf.lfQuality		= PROOF_QUALITY;
			CFont font;
			if( !font.CreateFontIndirect( &lf ) )
			{
				ASSERT( FALSE );
				return true;
			}

			CDC dc;
			dc.Attach( pDIS->hDC );
			dc.FillSolidRect(
				&pDIS->rcItem,
				g_PaintManager->GetColor(
					( pDIS->itemState & ODS_SELECTED )
						? COLOR_HIGHLIGHT
						: COLOR_WINDOW
						,
					this
					)
				);

			CRect rcText( pDIS->rcItem );
			//rcText.DeflateRect( OnPopupListBoxCalcItemExtraSizes(pTBB) );
			ASSERT( m_hIconFontTT != NULL );
			ASSERT( m_hIconFontPix != NULL );

			dc.DrawIcon(
				rcText.TopLeft(),
				(dwItemData & 2) ? m_hIconFontPix : m_hIconFontTT
				);
			rcText.left += 18;

			COLORREF clrTextOld =
				dc.SetTextColor(
					g_PaintManager->GetColor(
						( pDIS->itemState & ODS_SELECTED )
							? COLOR_HIGHLIGHTTEXT
							: COLOR_BTNTEXT
							,
						this
						)
					);
			int nOldBkMode = dc.SetBkMode( TRANSPARENT );

			HGDIOBJ hOldFont = dc.SelectObject( (HGDIOBJ)hDefFont );

			if( bSymbols )
			{
				CRect rcTextMeasure( 0, 0, 0, 0 );
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					LPCTSTR(sText),
					&rcTextMeasure,
					DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOPREFIX|DT_CALCRECT, 0
					);
				CExtRichContentLayout::stat_DrawText(
					dc.m_hDC,
					LPCTSTR(sText),
					&rcText,
					DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_NOPREFIX, 0
					);
				rcText.left += rcTextMeasure.Width() + 4;
			} // if( bSymbols )

			dc.SelectObject( &font );
			CExtRichContentLayout::stat_DrawText(
				dc.m_hDC,
				LPCTSTR(sText),
				&rcText,
				DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_NOPREFIX, 0
				);
			dc.SetBkMode( nOldBkMode );
			dc.SetTextColor( clrTextOld );
			dc.SelectObject( (HGDIOBJ)hOldFont );

			if( pDIS->itemState & ODS_SELECTED )
			{
				COLORREF clrTextOld = 
					dc.SetTextColor( 
						::GetSysColor( COLOR_HIGHLIGHTTEXT ) 
						);
				COLORREF clrBkOld = dc.SetBkColor( RGB(0,0,0) );
				dc.DrawFocusRect( &pDIS->rcItem );
				dc.SetBkColor( clrBkOld );
				dc.SetTextColor( clrTextOld );
			}	

			dc.Detach();
			return true;
		} // case ID_SE_FONT_LIST
	} // switch( nCmdID )
	return false; // this is not our business, let the bar button paint it
}

bool CMyRibbonBar::OnPopupListBoxItemMeasure(
	CExtBarButton * pTBB,
	CExtCustomizeCmdTreeNode * pNode,
	CListBox & wndListBox,
	LPMEASUREITEMSTRUCT pMIS
	)
{
	pTBB;
#ifdef _DEBUG
	if( pTBB != NULL )
	{
		ASSERT_VALID( pTBB );
		ASSERT_VALID( pTBB->GetBar() );
	}
#endif // _DEBUG
	ASSERT_VALID( pNode );
	ASSERT_VALID( (&wndListBox) );
	ASSERT( pMIS != NULL );
	wndListBox;
UINT nCmdID = pNode->GetCmdID(false);
	switch( nCmdID )
	{
	case ID_SE_FONT_LIST:
		{
			ASSERT( (wndListBox.GetStyle() & LBS_HASSTRINGS) != 0 );
			if( ((INT)pMIS->itemID) < 0 || ((INT)pMIS->itemID) >= wndListBox.GetCount() )
				return true;

			CString sText;
			wndListBox.GetText( (INT)pMIS->itemID, sText );
			ASSERT( !sText.IsEmpty() );
			DWORD dwItemData = DWORD( wndListBox.GetItemData( (INT)pMIS->itemID ) );

			HFONT hDefFont = (HFONT)g_PaintManager->m_FontNormal.GetSafeHandle();
			if( hDefFont == NULL )
				hDefFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
			if( hDefFont == NULL )
				hDefFont = (HFONT)::GetStockObject(SYSTEM_FONT);
			ASSERT( hDefFont != NULL );
			LOGFONT lf;
			::memset( &lf, 0, sizeof(LOGFONT) );
			if(	::GetObject(
					(HGDIOBJ)hDefFont,
					sizeof(LOGFONT),
					&lf
					) != sizeof(LOGFONT)
				)
			{
				ASSERT( FALSE );
				return true;
			}
			__EXT_MFC_STRNCPY(
				lf.lfFaceName,
				LF_FACESIZE,
				LPCTSTR(sText),
				LF_FACESIZE
				);
			lf.lfHeight = ::MulDiv( lf.lfHeight, 13, 8 );
			lf.lfOutPrecision	= BYTE( (dwItemData >> 9) & 0x0FF);
			lf.lfCharSet		= BYTE( (dwItemData >> 17) & 0x0FF);
			lf.lfQuality		= PROOF_QUALITY;
			CFont font;
			if( !font.CreateFontIndirect( &lf ) )
			{
				ASSERT( FALSE );
				return true;
			}

			//CSize sizeExtra = OnPopupListBoxCalcItemExtraSizes( pTBB );
			//sizeExtra.cx *= 2;
			//sizeExtra.cy *= 2;
			CClientDC dc( this );

			HGDIOBJ hOldFont = dc.SelectObject( (HGDIOBJ)hDefFont );
			CRect rcText( 0, 0, 0, 0 );
			CExtRichContentLayout::stat_DrawText(
				dc.m_hDC,
				LPCTSTR(sText),
				&rcText,
				DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOPREFIX|DT_CALCRECT, 0
				);
			CSize sizeBtn( rcText.Size() );
			dc.SelectObject( &font );
			rcText.SetRect( 0, 0, 0, 0 );
			CExtRichContentLayout::stat_DrawText(
				dc.m_hDC,
				LPCTSTR(sText),
				&rcText,
				DT_SINGLELINE|DT_LEFT|DT_TOP|DT_NOPREFIX|DT_CALCRECT, 0
				);
			sizeBtn.cx = max( sizeBtn.cx, rcText.Width() );
			sizeBtn.cy = max( sizeBtn.cy, rcText.Height() );
			dc.SelectObject( hOldFont );

			sizeBtn.cx += 18; // icon
			sizeBtn.cy = max( 16, sizeBtn.cy ); // icon
			//sizeBtn += sizeExtra;

			pMIS->itemWidth = sizeBtn.cx;
			pMIS->itemHeight = sizeBtn.cy;
			return true;
		} // case ID_SE_FONT_LIST
	} // switch( nCmdID )
	return false; // this is not our business, let the bar button measure it
}

bool CMyRibbonBar::OnTextFieldVerify(
	CExtBarTextFieldButton * pTextFieldTBB,
	CExtCustomizeCmdTreeNode * pNode,
	__EXT_MFC_SAFE_LPCTSTR sTextOld,
	__EXT_MFC_SAFE_LPCTSTR sTextNew
	)
{
	ASSERT_VALID( this );
#ifdef _DEBUG
	if( pTextFieldTBB != NULL )
	{
		ASSERT_VALID( pTextFieldTBB );
		ASSERT_VALID( pTextFieldTBB->GetBar() );
	}
#endif // _DEBUG
	ASSERT_VALID( pNode );
	ASSERT( sTextOld != NULL );
	ASSERT( sTextNew != NULL );
	pTextFieldTBB;
	sTextOld;

UINT nCmdID = pNode->GetCmdID(false);
	if( nCmdID == ID_SE_SIZE )
	{
		int nTextLenNew = int( _tcslen( sTextNew ) );
		//if( nTextLenNew == 0 )
		//	return false;
		int nSepLen = m_sDecimalSeparator.GetLength();
		ASSERT( nSepLen > 0 );
		bool bSeparatorPassed = false;
		for( int nChrIdx = 0; nChrIdx < nTextLenNew; nChrIdx++ )
		{
			TCHAR chr = sTextNew[nChrIdx];
			if( _istdigit(chr) )
				continue;
			if( bSeparatorPassed )
				return false;
			if( _tcsncmp(
					((LPCTSTR)sTextNew) + nChrIdx,
					m_sDecimalSeparator,
					nSepLen
					)
				)
				return false;
			bSeparatorPassed = true;
			nChrIdx += nSepLen - 1;
		} // for( int nChrIdx = 0; nChrIdx < nTextLenNew; nChrIdx++ )
		return true;
	} // if( nCmdID == ID_SE_SIZE )

	return true;
}

void CMyRibbonBar::OnTextFieldInplaceTextGet(
	const CExtBarTextFieldButton * pTextFieldTBB,
	const CExtCustomizeCmdTreeNode * pNode,
	CExtSafeString & sTextFieldBuffer
	)
{
#ifdef _DEBUG
	if( pTextFieldTBB != NULL )
	{
		ASSERT_VALID( pTextFieldTBB );
		ASSERT_VALID( pTextFieldTBB->GetBar() );
	}
#endif // _DEBUG
	ASSERT_VALID( pNode );
	pTextFieldTBB;
UINT nCmdID = pNode->GetCmdID(false);
	switch( nCmdID )
	{
	case ID_SE_FONT_LIST:
		sTextFieldBuffer = m_sCurrentFontFaceName;
		return;
	case ID_SE_SIZE:
		{
			float fFontSize = m_lfCurrentFontSize;
			ASSERT( __SO_FONT_SIZE_MIN <= fFontSize );
			ASSERT( fFontSize <= __SO_FONT_SIZE_MAX );
			stat_FormatFontSize(
				sTextFieldBuffer,
				m_sDecimalSeparator,
				fFontSize
				);
			return;
		} // case ID_SE_FONT_LIST
	} // switch( nCmdID )
}

void CMyRibbonBar::OnTextFieldInplaceTextSet(
	CExtBarTextFieldButton * pTextFieldTBB,
	CExtCustomizeCmdTreeNode * pNode,
	CExtSafeString & sTextFieldBuffer,
	__EXT_MFC_SAFE_LPCTSTR sTextNew
	)
{
	ASSERT_VALID( this );
#ifdef _DEBUG
	if( pTextFieldTBB != NULL )
	{
		ASSERT_VALID( pTextFieldTBB );
		ASSERT_VALID( pTextFieldTBB->GetBar() );
	}
#endif // _DEBUG
	ASSERT_VALID( pNode );
	ASSERT( sTextNew != NULL );
	pTextFieldTBB;
CString sTextAdjusted;
	sTextAdjusted = LPCTSTR(sTextNew);
UINT nCmdID = pNode->GetCmdID(false);
	switch( nCmdID )
	{
	case ID_SE_FONT_LIST:
		sTextFieldBuffer = LPCTSTR(sTextAdjusted);
 		m_sCurrentFontFaceName =  LPCTSTR(sTextAdjusted);
		CExtRibbonBar::RedrawCommandItems( nCmdID );
		break;
	case ID_SE_SIZE:
		{
			sTextAdjusted.Replace(
				LPCTSTR(m_sDecimalSeparator),
				_T(".")
				);
			sTextAdjusted.Replace( _T(" "), _T("") );
			TCHAR * pEndPtr = NULL;
			float fFontSize =
				(float) _tcstod( (LPCTSTR)sTextAdjusted, &pEndPtr );
			m_lfCurrentFontSize = fFontSize;
			ASSERT( __SO_FONT_SIZE_MIN <= fFontSize );
			ASSERT( fFontSize <= __SO_FONT_SIZE_MAX );
			stat_FormatFontSize(
				sTextFieldBuffer,
				m_sDecimalSeparator,
				fFontSize
				);
			CExtRibbonBar::RedrawCommandItems( nCmdID );
			break;
		} // case ID_SE_FONT_LIST
	} // switch( nCmdID )
CExtPopupMenuWnd * pPopup =
		CExtPopupMenuWnd::GetTrackingMenu();
	if( pPopup->GetSafeHwnd() != NULL )
		pPopup->PostMessage( WM_CANCELMODE );
	CExtRibbonBar::OnTextFieldInplaceTextSet(
		pTextFieldTBB,
		pNode,
		sTextFieldBuffer,
		sTextNew
		);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC( CMainFrame, CMDIFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CMDIFrameWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP

//	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
//	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_UI_LOOK_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_UI_LOOK_BAR, OnUpdateControlBarMenu)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	ID_INDICATOR_ZOOM
};

void CMainFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	CExtControlBar::DoFrameBarCheckUpdate( this, pCmdUI, true );
}

BOOL CMainFrame::OnBarCheck(UINT nID)
{
	return CExtControlBar::DoFrameBarCheckCmd( this, nID, true );
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
	: m_pRibbonNode( NULL )
{
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
		g_PaintManager.InstallPaintManager(
			RUNTIME_CLASS(CExtPaintManagerOffice2010_R1)
			);

//							if( ! g_PaintManager.PaintManagerStateLoad(
//									pApp->m_pszRegistryKey,
//									pApp->m_pszProfileName,
//									pApp->m_pszProfileName
//									)
//								)
//							{
//								CExtPaintManagerSkin * pPM = new CExtPaintManagerSkin;
//								bool bLoaded = true;
//								if( ! pPM->m_Skin.SearchAndLoadSkinFile( _T("BlackDiamond.Skin"), false ) )
//								{
//									if( ! pPM->m_Skin.SearchAndLoadSkinFile( _T("..\\..\\Skins\\Binary\\Aqua.Skin"), false ) )
//									{
//										bLoaded = false;
//										::AfxMessageBox( _T("Failed to load initial skin.") );
//										delete pPM;
//									}
//								}
//								if( bLoaded )
//									g_PaintManager.InstallPaintManager( pPM );
//							}

	CExtPopupMenuWnd::g_bMenuExpanding = false;
	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;

	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;
}

CMainFrame::~CMainFrame()
{
	if( m_pRibbonNode != NULL )
		delete m_pRibbonNode;
}

void CMainFrame::RecalcLayout(BOOL bNotify)
{
	CExtNCW < CMDIFrameWnd > :: RecalcLayout( bNotify );
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( CExtNCW < CMDIFrameWnd > :: OnCreate( lpCreateStruct ) == -1 )
		return -1;

CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );

HICON hIcon = pApp->LoadIcon( IDR_MAINFRAME );
	ASSERT( hIcon != NULL );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	g_CmdManager->ProfileSetup(
		pApp->m_pszProfileName,
		GetSafeHwnd()
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_MAINFRAME
			)
		);

	if(	! m_wndRibbonBar.Create(
			NULL,
			this
			)
		)
	{
		TRACE0("Failed to create the m_wndRibbonBar toolbar\n");
		return -1;      // fail to create
	}
//m_wndRibbonBar.ModifyStyleEx( 0, WS_EX_LAYOUTRTL, SWP_FRAMECHANGED );
//m_wndView.ModifyStyleEx( 0, WS_EX_LAYOUTRTL, SWP_FRAMECHANGED );
//g_ResourceManager->SetLangLayout( LAYOUT_RTL );
	_InitRibbonBar();

	m_wndToolBarUiLook.m_bEnableOffice2007r3 = true;
	if(		(! m_wndToolBarUiLook.Create( NULL, this, ID_VIEW_UI_LOOK_BAR ) )
		||	(! m_wndToolBarUiLook.ThemeSwitcherInit() )
		)
	{
		TRACE0("Failed to create the m_wndToolBarUiLook toolbar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.m_bDrawPaneSeparatorsInsteadOfBorders = true;
	if(		(! m_wndStatusBar.Create( this ) )
		||	(! m_wndStatusBar.SetIndicators(
				indicators,
				sizeof(indicators)/sizeof(UINT)
				) )
		||	(! m_wndZoomScrollBar.Create(
				WS_CHILD|WS_VISIBLE|SBS_HORZ,
				CRect(0,0,0,21),
				&m_wndStatusBar,
				ID_INDICATOR_ZOOM
				)
			)
		||	(!	m_wndStatusBar.SetPaneControl(
				&m_wndZoomScrollBar,
				ID_INDICATOR_ZOOM,
				true
				)
			)
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight( 23 );
INT nZoomPaneIndex = m_wndStatusBar.CommandToIndex( ID_INDICATOR_ZOOM );
	m_wndStatusBar.SetPaneWidth( nZoomPaneIndex, 120 );
	m_wndZoomScrollBar.SetScrollRange( 0, 100 );
	m_wndZoomScrollBar.SetScrollPos( 50 );
	m_wndZoomScrollBar.m_bEnabledToolTips         = true;
	m_wndZoomScrollBar.m_strTipTextForThumbButton = _T("Zoom");
	m_wndZoomScrollBar.m_strTipTextForUpButton    = _T("Zoom Out");
	m_wndZoomScrollBar.m_strTipTextForDownButton  = _T("Zoom In");
	m_wndZoomScrollBar.m_eSO = CExtScrollBar::__ESO_BOTTOM;

	m_wndToolBarUiLook.EnableDocking( CBRS_ALIGN_ANY );

	if( ! CExtControlBar::FrameEnableDocking( this ) )
	{
		ASSERT( FALSE );
		return -1;
	}

	_AdjustRTL();
	
	m_wndRibbonBar.CustomizeStateLoad(
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		pApp->m_pszProfileName
		);

	if( ! CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{
		DockControlBar( &m_wndToolBarUiLook, AFX_IDW_DOCKBAR_RIGHT );
		RecalcLayout();
	}

	if( m_wndMdiTabs.Create( this ) )
		m_wndMdiTabs.OrientationSet( __ETWS_ORIENT_BOTTOM );

#if (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)
	m_wndRibbonBar.BackstageView_EnsureVisible( false );
#endif // (defined __USE_BACKSTAGEVIEW_IN_THIS_SAMPLE_APP__)

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CMDIFrameWnd > :: PreCreateWindow( cs ) )
		return FALSE;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_LAYOUT_RTL);
	cs.lpszClass =
		::AfxRegisterWndClass(
			CS_BYTEALIGNCLIENT,
			::LoadCursor( NULL, IDC_ARROW ),
			NULL,
			NULL
			);
	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
//cs.dwExStyle |= WS_EX_LAYOUTRTL;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CExtNCW < CMDIFrameWnd > :: AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CExtNCW < CMDIFrameWnd > :: Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if( nID == ID_RIBBON_MINIMIZE_RESTORE )
	{
		if( nCode == CN_UPDATE_COMMAND_UI )
		{
			((CCmdUI *)pExtra)->Enable();
			((CCmdUI *)pExtra)->SetCheck( m_wndRibbonBar.RibbonPage_ExpandedModeGet() );
		}
		else if( pExtra == NULL )
		{
			m_wndRibbonBar.RibbonPage_ExpandedModeSet( ! m_wndRibbonBar.RibbonPage_ExpandedModeGet(), true );
		}
		return TRUE;
	}

#if (!defined __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG)
	if( nCode == CN_COMMAND || nCode == CN_UPDATE_COMMAND_UI )
	{
		if( nID == ID_WINDOWS_LIST )
		{
			if( pExtra == NULL && nCode == CN_COMMAND )
			{
				CMyMdiWindowsListDlg dlgMdiWindowsList( this );
				dlgMdiWindowsList.DoModal();
			}
			else if( nCode == CN_UPDATE_COMMAND_UI )
				((CCmdUI*)pExtra)->Enable();
			return TRUE;
		}
	}
#endif // (!defined __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG)

//	// TEST FOR INDETERIMINATE BUTTON STATE IN RIBBONs
//	if(		nID == ID_EDIT_PASTE
//		||	nID == ID_EDIT_COPY
//		||	nID == ID_EDIT_FIND
//		||	nID == ID_EDIT_REPLACE
//		||	nID == ID_FONT_BOLD
//		||	nID == ID_PB_NO_BORDER
//		)
//	{
//		if( nCode == CN_UPDATE_COMMAND_UI )
//		{
//			((CCmdUI*)pExtra)->Enable();
//			((CCmdUI*)pExtra)->SetCheck( 2 );
//		}
//		return TRUE;
//	}

	if( nID == ID_RTL )
	{
		if( nCode == CN_UPDATE_COMMAND_UI )
		{
			((CCmdUI *)pExtra)->Enable( g_PaintManager.m_bIsWinNT );
			if( g_PaintManager.m_bIsWinNT )
				((CCmdUI *)pExtra)->SetCheck(
					( (GetExStyle()&WS_EX_LAYOUT_RTL) != 0 ) ? 1 : 0
					);
		}
		else if( pExtra == NULL && g_PaintManager.m_bIsWinNT )
		{
			CExtNcFrameImpl::m_bNcFrameImpl_IsEnabled = false;
			if( (GetExStyle()&WS_EX_LAYOUT_RTL) != 0 )
				ModifyStyleEx( WS_EX_LAYOUT_RTL, 0, SWP_FRAMECHANGED );
			else
				ModifyStyleEx( 0, WS_EX_LAYOUT_RTL, SWP_FRAMECHANGED );
			CExtNcFrameImpl::m_bNcFrameImpl_IsEnabled = true;
			_AdjustRTL();
		}
		return TRUE;
	}

	if( nID == IDD_EXT_RIBBON_OPTIONS_DIALOG )
		return m_wndRibbonBar.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
	if(		nID == ID_FILE_PRINT
//		||	nID == 39024 // "Save to Share Point" second level tab page in backstage view
//		||	nID == ID_FILE_SAVE
//		||	nID == 39100 // "Activate Product" button in backstage view
//		||	nID == 39101 // "Protect Document" button in backstage view
		)
		return FALSE;
//	if( nID == ID_FB_OPEN && nCode == CN_COMMAND && pExtra == NULL )
//		return TRUE;

//	if( nID == ID_EDIT_COPY && nCode == CN_COMMAND )
//	{ // correct
//		if( pExtra == NULL )
//		{
//			int x = 0;
//			x;
//			AfxMessageBox("Edit Copy");
//		}
//		return TRUE; // this should be invoked even if pExtra != NULL
//	} // correct

	// otherwise, do default handling
//	return CExtNCW < CMDIFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
	CExtNCW < CMDIFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
	return TRUE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndToolBarUiLook.PreTranslateMessage( pMsg ) )
		return TRUE;
	if( m_wndRibbonBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	return CExtNCW < CMDIFrameWnd > :: PreTranslateMessage(pMsg);
}

BOOL CMainFrame::DestroyWindow() 
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	m_wndRibbonBar.CustomizeStateSave(
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		pApp->m_pszProfileName
		);
	VERIFY(
		CExtControlBar::ProfileBarStateSave(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
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
	
	return CExtNCW < CMDIFrameWnd > :: DestroyWindow();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
bool bDwmMode = NcFrameImpl_IsDwmCaptionReplacement();
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		if( bDwmMode )
			NcFrameImpl_NcLock( true );
		SetWindowPlacement( &m_dataFrameWP );
		CExtNCW < CMDIFrameWnd > :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		if( bDwmMode )
		{
			NcFrameImpl_NcLock( false );
			NcFrameImpl_RecalcNcFrame();
		}
		return;
	}
	CExtNCW < CMDIFrameWnd > :: ActivateFrame( nCmdShow );
	if( bDwmMode )
		NcFrameImpl_RecalcNcFrame();
}

void CMainFrame::_AdjustRTL()
{
	ASSERT_VALID( this );
	if( (GetExStyle()&WS_EX_LAYOUT_RTL) != 0 )
	{
		g_ResourceManager->SetLangLayout( LAYOUT_RTL );
		CExtNcFrameImpl::m_dwNcFrameImpl_StyleExInitial |= WS_EX_LAYOUT_RTL;
		m_wndRibbonBar.ModifyStyleEx( 0, WS_EX_LAYOUTRTL, 0 );
	}
	else
	{
		g_ResourceManager->SetLangLayout( LAYOUT_LTR );
		CExtNcFrameImpl::m_dwNcFrameImpl_StyleExInitial &= ~(WS_EX_LAYOUT_RTL);
		m_wndRibbonBar.ModifyStyleEx( WS_EX_LAYOUTRTL, 0, 0 );
	}
	m_wndRibbonBar.Ribbon_UpdateLayout();
	CExtNcFrameImpl::NcFrameImpl_RecalcNcFrame();
	CExtNcFrameImpl::NcFrameImpl_SetupRgn();
	if( IsWindowVisible() )
		RedrawWindow( NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN );
}

void CMainFrame::_InitRibbonBar()
{
	ASSERT_VALID( this );
	ASSERT( m_pRibbonNode == NULL );
	m_pRibbonNode = new CExtRibbonNode;
CExtRibbonNodeTabPageCollection * pRibbonNodeTabPageCollection =
		new CExtRibbonNodeTabPageCollection;
	m_pRibbonNode->InsertNode( NULL, pRibbonNodeTabPageCollection );
			pRibbonNodeTabPageCollection->InsertNode( NULL, _InitRibbonPage_Home() );
			pRibbonNodeTabPageCollection->InsertNode( NULL, _InitRibbonPage_Insert() );
			pRibbonNodeTabPageCollection->InsertNode( NULL, _InitRibbonPage_PageLayout() );
			pRibbonNodeTabPageCollection->InsertNode( NULL, _InitRibbonPage_References() );
			pRibbonNodeTabPageCollection->InsertNode( NULL, _InitRibbonPage_Mailings() );
			pRibbonNodeTabPageCollection->InsertNode( NULL, _InitRibbonPage_Review() );
			pRibbonNodeTabPageCollection->InsertNode( NULL, _InitRibbonPage_View() );
	pRibbonNodeTabPageCollection->PageSelectionSet( 0 );
//INT nPageIndex, nPageCount = pRibbonNodeTabPageCollection->GetNodeCount();
//	for( nPageIndex = 0; nPageIndex < nPageCount; nPageIndex ++ )
//	{
//		CExtRibbonNodeTabPage * pRibbonNodeTabPage =
//			STATIC_DOWNCAST(
//				CExtRibbonNodeTabPage,
//				pRibbonNodeTabPageCollection->ElementAt( nPageIndex )
//				);
//		ASSERT_VALID( pRibbonNodeTabPage );
//		pRibbonNodeTabPage->ModifyFlags( __ECTN_TBB_HIDDEN );
//	} // for( nPageIndex = 0; nPageIndex < nPageCount; nPageIndex ++ )

CExtRibbonNodeRightButtonsCollection * pRibbonNodeRightButtonsCollection =
		_InitRibbon_RightButtonsCollection();
	m_pRibbonNode->InsertNode( NULL, pRibbonNodeRightButtonsCollection );
CExtRibbonNodeQuickAccessButtonsCollection * pRibbonNodeQuickAccessButtonsCollection =
		_InitRibbon_QuickAccessButtonsCollection();
	m_pRibbonNode->InsertNode( NULL, pRibbonNodeQuickAccessButtonsCollection );

CExtRibbonNodeFile * pRibbonNodeFile = _InitRibbon_FileMenu();
	m_pRibbonNode->InsertNode( NULL, pRibbonNodeFile );

	m_wndRibbonBar.SetButtons( m_pRibbonNode );
	VERIFY( m_wndRibbonBar.MenuInfoGetDefault()->AccelTableLoad( IDR_MAINFRAME ) );

bool bSmallFileButtonIcon = g_PaintManager->Ribbon_IsSystemMenuIconPresent() ? true : false;
LPCTSTR strFileButtonIconResID = bSmallFileButtonIcon ? (MAKEINTRESOURCE(IDB_BITMAP_UI_LOGO_ICON)) : (MAKEINTRESOURCE(IDB_BITMAP_SMALL_ICON));
	VERIFY( m_wndRibbonBar.Ribbon_FileButtonGet() -> m_iconForLargeMode.m_bmpNormal.LoadBMP_Resource( strFileButtonIconResID ) );
}

void CMainFrame::NcFrameImpl_SyncThemeSettings( CExtPaintManager * pPM, bool bPostNotification )
{
	CExtNCW < CMDIFrameWnd > :: NcFrameImpl_SyncThemeSettings( pPM, bPostNotification );
	if( ( ! bPostNotification ) && m_wndRibbonBar.GetSafeHwnd() != NULL )
	{
		bool bSmallFileButtonIcon = pPM->Ribbon_IsSystemMenuIconPresent() ? true : false;
		LPCTSTR strFileButtonIconResID = bSmallFileButtonIcon ? (MAKEINTRESOURCE(IDB_BITMAP_UI_LOGO_ICON)) : (MAKEINTRESOURCE(IDB_BITMAP_SMALL_ICON));
		VERIFY( m_wndRibbonBar.Ribbon_FileButtonGet() -> m_iconForLargeMode.m_bmpNormal.LoadBMP_Resource( strFileButtonIconResID ) );
	}
}

CExtRibbonNodeFile * CMainFrame::_InitRibbon_FileMenu()
{
CExtRibbonNodeFile * pRibbonNodeFile = new CExtRibbonNodeFile;
	pRibbonNodeFile->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('F') ) ), false );
	pRibbonNodeFile->m_keyTipOptionsButton.KeyCodeAdd( DWORD('T') );
	pRibbonNodeFile->m_keyTipExitButton.KeyCodeAdd( DWORD('X') );
							CExtCustomizeCmdScreenTip * pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Application Button") );
							pCmdScreenTip->TextMainSet( _T("Click here to open, save or print,\nand to see everything else you can\ndo with the document.") );
							pCmdScreenTip->BmpMainGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_FILE_DLB_TIP_IMAGE) );
							pCmdScreenTip->BmpMainGet().Make32();
							pCmdScreenTip->BmpMainGet().AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pRibbonNodeFile->CmdScreenTipAttach( pCmdScreenTip );
//pRibbonNodeFile->m_nTpmxAdditionalFlags &= ~(TPMX_RIBBON_OPTIONS_BUTTON);

CExtRibbonNode * pNodeFileNew =
		new CExtRibbonNode(
			ID_FB_NEW,
			ID_FB_NEW,
			NULL,
			__ECTN_BIG_ACCENT_IN_MENU,
			NULL,
			_T("&New...")
			);
	pNodeFileNew->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('N') ) ), false );
	VERIFY(
		pNodeFileNew->m_iconBig.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_FB_NEW )
			)
		);
	pNodeFileNew->m_iconBig.m_bmpNormal.Make32();
	pNodeFileNew->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("New") );
							pNodeFileNew->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeFile->InsertNode( NULL, pNodeFileNew );

CExtRibbonNode * pNodeFileOpen =
		new CExtRibbonNode(
			ID_FB_OPEN,
			ID_FB_OPEN,
			NULL,
			__ECTN_BIG_ACCENT_IN_MENU,
			NULL,
			_T("&Open...")
			);
	pNodeFileOpen->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('O') ) ), false );
	VERIFY(
		pNodeFileOpen->m_iconBig.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_FB_OPEN )
			)
		);
	pNodeFileOpen->m_iconBig.m_bmpNormal.Make32();
	pNodeFileOpen->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Open") );
							pNodeFileOpen->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeFile->InsertNode( NULL, pNodeFileOpen );

CExtRibbonNode * pNodeFileSave =
		new CExtRibbonNode(
			ID_FB_SAVE,
			ID_FB_SAVE,
			NULL,
			__ECTN_BIG_ACCENT_IN_MENU,
			NULL,
			_T("&Save")
			);
	pNodeFileSave->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('S') ) ), false );
	VERIFY(
		pNodeFileSave->m_iconBig.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_FB_SAVE )
			)
		);
	pNodeFileSave->m_iconBig.m_bmpNormal.Make32();
	pNodeFileSave->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Save") );
							pNodeFileSave->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeFile->InsertNode( NULL, pNodeFileSave );

CExtRibbonNode * pNodeFileSaveAs =
		new CExtRibbonNode(
			ID_FB_SAVE_AS,
			ID_FB_SAVE_AS,
			NULL,
			__ECTN_BIG_ACCENT_IN_MENU|__ECTN_TBB_SEPARATED_DROPDOWN,
			NULL,
			_T("Save &As...")
			);
	pNodeFileSaveAs->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('A') ) ), false );
	pNodeFileSaveAs->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('F') ) ), false, false );
	pNodeFileSaveAs->SetTextMenuCaption( _T("Save As Other Format") );
	VERIFY(
		pNodeFileSaveAs->m_iconBig.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_FB_SAVE_AS )
			)
		);
	pNodeFileSaveAs->m_iconBig.m_bmpNormal.Make32();
	pNodeFileSaveAs->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Save As") );
							pNodeFileSaveAs->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeFile->InsertNode( NULL, pNodeFileSaveAs );

				CExtRibbonNode * pNodeFileSaveAsFormat1 =
					new CExtRibbonNode(
						ID_FB_SAVE_AS_FROMAT_ICON,
						ID_FB_SAVE_AS_FROMAT_ICON,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("Save Document As Format 1")
						);
				pNodeFileSaveAsFormat1->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('1') ) ), false );
				pNodeFileSaveAsFormat1->SetTextMenuExtended(
					_T("Save a copy of that will be fully compatible\nwith Format 1")
					);
				VERIFY(
					pNodeFileSaveAsFormat1->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_SAVE_AS_FROMAT_ICON )
						)
					);
				pNodeFileSaveAsFormat1->m_iconBig.m_bmpNormal.Make32();
				pNodeFileSaveAsFormat1->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileSaveAs->InsertNode( NULL, pNodeFileSaveAsFormat1 );

				CExtRibbonNode * pNodeFileSaveAsFormat2 =
					new CExtRibbonNode(
						ID_FB_SAVE_AS_FROMAT_ICON,
						ID_FB_SAVE_AS_FROMAT_ICON,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("Save Document As Format 2")
						);
				pNodeFileSaveAsFormat2->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('2') ) ), false );
				pNodeFileSaveAsFormat2->SetTextMenuExtended(
					_T("Publish a copy of this document\nas Format 2")
					);
				VERIFY(
					pNodeFileSaveAsFormat2->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_SAVE_AS_FROMAT_ICON )
						)
					);
				pNodeFileSaveAsFormat2->m_iconBig.m_bmpNormal.Make32();
				pNodeFileSaveAsFormat2->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileSaveAs->InsertNode( NULL, pNodeFileSaveAsFormat2 );

				CExtRibbonNode * pNodeFileSaveAsFormat3 =
					new CExtRibbonNode(
						ID_FB_SAVE_AS_FROMAT_ICON,
						ID_FB_SAVE_AS_FROMAT_ICON,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("Save Document As Format 3")
						);
				pNodeFileSaveAsFormat3->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('3') ) ), false );
				pNodeFileSaveAsFormat3->SetTextMenuExtended(
					_T("Save a copy of that will be fully compatible\nwith Format 3")
					);
				VERIFY(
					pNodeFileSaveAsFormat3->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_SAVE_AS_FROMAT_ICON )
						)
					);
				pNodeFileSaveAsFormat3->m_iconBig.m_bmpNormal.Make32();
				pNodeFileSaveAsFormat3->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileSaveAs->InsertNode( NULL, pNodeFileSaveAsFormat3 );

				CExtRibbonNode * pNodeFileSaveAsFormat4 =
					new CExtRibbonNode(
						ID_FB_SAVE_AS_FROMAT_ICON,
						ID_FB_SAVE_AS_FROMAT_ICON,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("Save Document As Format 4")
						);
				pNodeFileSaveAsFormat4->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('4') ) ), false );
				pNodeFileSaveAsFormat4->SetTextMenuExtended(
					_T("Publish a copy of this document\nas Format 4")
					);
				VERIFY(
					pNodeFileSaveAsFormat4->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_SAVE_AS_FROMAT_ICON )
						)
					);
				pNodeFileSaveAsFormat4->m_iconBig.m_bmpNormal.Make32();
				pNodeFileSaveAsFormat4->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileSaveAs->InsertNode( NULL, pNodeFileSaveAsFormat4 );

CExtRibbonNode * pNodeFileFinish =
		new CExtRibbonNode(
			ID_FB_FINISH,
			ID_FB_FINISH,
			NULL,
			__ECTN_BIG_ACCENT_IN_MENU|__ECTN_GROUP_START,
			NULL,
			_T("&Finish")
			);
	pNodeFileFinish->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('I') ) ), false );
	pNodeFileFinish->SetTextMenuCaption( _T("Before You Share") );
	VERIFY(
		pNodeFileFinish->m_iconBig.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_FB_FINISH )
			)
		);
	pNodeFileFinish->m_iconBig.m_bmpNormal.Make32();
	pNodeFileFinish->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonNodeFile->InsertNode( NULL, pNodeFileFinish );

				CExtRibbonNode * pNodeFileFinishInspectDocument =
					new CExtRibbonNode(
						ID_FB_FINISH_INSPECT_DOCUMENT,
						ID_FB_FINISH_INSPECT_DOCUMENT,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("&Inspect Document...")
						);
				pNodeFileFinishInspectDocument->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('I') ) ), false );
				pNodeFileFinishInspectDocument->SetTextMenuExtended(
					_T("Helps check the document for\ninapropriate or private information.")
					);
				VERIFY(
					pNodeFileFinishInspectDocument->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_FINISH_INSPECT_DOCUMENT )
						)
					);
				pNodeFileFinishInspectDocument->m_iconBig.m_bmpNormal.Make32();
				pNodeFileFinishInspectDocument->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileFinish->InsertNode( NULL, pNodeFileFinishInspectDocument );

				CExtRibbonNode * pNodeFileFinishRestrictPermission =
					new CExtRibbonNode(
						ID_FB_FINISH_RESTRICT_PERMISSION,
						ID_FB_FINISH_RESTRICT_PERMISSION,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("Restrict &Permission")
						);
				pNodeFileFinishRestrictPermission->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('R') ) ), false );
				pNodeFileFinishRestrictPermission->SetTextMenuExtended(
					_T("Grant access to specific users by verifying\ntheir login credentials.")
					);
				VERIFY(
					pNodeFileFinishRestrictPermission->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_FINISH_RESTRICT_PERMISSION )
						)
					);
				pNodeFileFinishRestrictPermission->m_iconBig.m_bmpNormal.Make32();
				pNodeFileFinishRestrictPermission->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileFinish->InsertNode( NULL, pNodeFileFinishRestrictPermission );

							CExtRibbonNode * pNodeFileFinishUnrestrictedAccess =
								new CExtRibbonNode(
									ID_FB_FINISH_RESTRICT_PERMISSION_UNRESTRICTED_ACCESS,
									ID_FB_FINISH_RESTRICT_PERMISSION_UNRESTRICTED_ACCESS,
									NULL,
									0,
									NULL,
									_T("&Unrestricted Access")
									);
							pNodeFileFinishUnrestrictedAccess->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('U') ) ), false );
							pNodeFileFinishRestrictPermission->InsertNode( NULL, pNodeFileFinishUnrestrictedAccess );

							CExtRibbonNode * pNodeFileFinishDoNotDistribute =
								new CExtRibbonNode(
									ID_FB_FINISH_RESTRICT_PERMISSION_DO_NOT_DISTRIBUTE,
									ID_FB_FINISH_RESTRICT_PERMISSION_DO_NOT_DISTRIBUTE,
									NULL,
									0,
									NULL,
									_T("Do &Not Distribute...")
									);
							pNodeFileFinishRestrictPermission->InsertNode( NULL, pNodeFileFinishDoNotDistribute );

							CExtRibbonNode * pNodeFileFinishRestrictPermissionAs =
								new CExtRibbonNode(
									ID_FB_FINISH_RESTRICT_PERMISSION_AS,
									ID_FB_FINISH_RESTRICT_PERMISSION_AS,
									NULL,
									__ECTN_GROUP_START,
									NULL,
									_T("&Restrict Permission As...")
									);
							pNodeFileFinishRestrictPermissionAs->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('R') ) ), false );
							pNodeFileFinishRestrictPermission->InsertNode( NULL, pNodeFileFinishRestrictPermissionAs );

				CExtRibbonNode * pNodeFileFinishAddDigitalSignature =
					new CExtRibbonNode(
						ID_FB_FINISH_ADD_DIGITAL_SIGNATURE,
						ID_FB_FINISH_ADD_DIGITAL_SIGNATURE,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("Add a Digital &Signature...")
						);
				pNodeFileFinishAddDigitalSignature->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('S') ) ), false );
				pNodeFileFinishAddDigitalSignature->SetTextMenuExtended(
					_T("Add a signature that is not visible within\nthe content of the document.")
					);
				VERIFY(
					pNodeFileFinishAddDigitalSignature->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_FINISH_ADD_DIGITAL_SIGNATURE )
						)
					);
				pNodeFileFinishAddDigitalSignature->m_iconBig.m_bmpNormal.Make32();
				pNodeFileFinishAddDigitalSignature->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileFinish->InsertNode( NULL, pNodeFileFinishAddDigitalSignature );

				CExtRibbonNode * pNodeFileFinishCompatibilityChecker =
					new CExtRibbonNode(
						ID_FB_FINISH_COMPATIBILITY_CHECKER,
						ID_FB_FINISH_COMPATIBILITY_CHECKER,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("&Compatibility Checker...")
						);
				pNodeFileFinishCompatibilityChecker->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('C') ) ), false );
				pNodeFileFinishCompatibilityChecker->SetTextMenuExtended(
					_T("Check the document for features\nunsupported by previous version.")
					);
				VERIFY(
					pNodeFileFinishCompatibilityChecker->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_FINISH_COMPATIBILITY_CHECKER )
						)
					);
				pNodeFileFinishCompatibilityChecker->m_iconBig.m_bmpNormal.Make32();
				pNodeFileFinishCompatibilityChecker->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileFinish->InsertNode( NULL, pNodeFileFinishCompatibilityChecker );

				CExtRibbonNode * pNodeFileFinishMarkAsFinal =
					new CExtRibbonNode(
						ID_FB_FINISH_MARK_AS_FINAL,
						ID_FB_FINISH_MARK_AS_FINAL,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("Mark As &Final")
						);
				pNodeFileFinishMarkAsFinal->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('M') ) ), false );
				pNodeFileFinishMarkAsFinal->SetTextMenuExtended(
					_T("Let readers know your document is final\nand make it read only.")
					);
				VERIFY(
					pNodeFileFinishMarkAsFinal->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_FINISH_MARK_AS_FINAL )
						)
					);
				pNodeFileFinishMarkAsFinal->m_iconBig.m_bmpNormal.Make32();
				pNodeFileFinishMarkAsFinal->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileFinish->InsertNode( NULL, pNodeFileFinishMarkAsFinal );

CExtRibbonNode * pNodeFileShare =
		new CExtRibbonNode(
			ID_FB_SHARE,
			ID_FB_SHARE,
			NULL,
			__ECTN_BIG_ACCENT_IN_MENU,
			NULL,
			_T("S&hare")
			);
	pNodeFileShare->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
	pNodeFileShare->SetTextMenuCaption( _T("Share Your Document") );
	VERIFY(
		pNodeFileShare->m_iconBig.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_FB_SHARE )
			)
		);
	pNodeFileShare->m_iconBig.m_bmpNormal.Make32();
	pNodeFileShare->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonNodeFile->InsertNode( NULL, pNodeFileShare );

				CExtRibbonNode * pNodeFileShareEmail =
					new CExtRibbonNode(
						ID_FB_SHARE_EMAIL,
						ID_FB_SHARE_EMAIL,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("&Email...")
						);
				pNodeFileShareEmail->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('E') ) ), false );
				pNodeFileShareEmail->SetTextMenuExtended(
					_T("Email a copy of your document as an\nattachment.")
					);
				VERIFY(
					pNodeFileShareEmail->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_SHARE_EMAIL )
						)
					);
				pNodeFileShareEmail->m_iconBig.m_bmpNormal.Make32();
				pNodeFileShareEmail->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileShare->InsertNode( NULL, pNodeFileShareEmail );

				CExtRibbonNode * pNodeFileShareInternetFax =
					new CExtRibbonNode(
						ID_FB_SHARE_INET_FAX,
						ID_FB_SHARE_INET_FAX,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("Internet Fa&x...")
						);
				pNodeFileShareInternetFax->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('F') ) ), false );
				pNodeFileShareInternetFax->SetTextMenuExtended(
					_T("Use an internet fax service to fax your\ndocument.")
					);
				VERIFY(
					pNodeFileShareInternetFax->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_SHARE_INET_FAX )
						)
					);
				pNodeFileShareInternetFax->m_iconBig.m_bmpNormal.Make32();
				pNodeFileShareInternetFax->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileShare->InsertNode( NULL, pNodeFileShareInternetFax );

				CExtRibbonNode * pNodeFileShareDocumentManagementServer =
					new CExtRibbonNode(
						ID_FB_SHARE_DMS,
						ID_FB_SHARE_DMS,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("&Document Management Server...")
						);
				pNodeFileShareDocumentManagementServer->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('D') ) ), false );
				pNodeFileShareDocumentManagementServer->SetTextMenuExtended(
					_T("Share this document by saving it to an\ndocument management server.")
					);
				VERIFY(
					pNodeFileShareDocumentManagementServer->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_SHARE_DMS )
						)
					);
				pNodeFileShareDocumentManagementServer->m_iconBig.m_bmpNormal.Make32();
				pNodeFileShareDocumentManagementServer->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileShare->InsertNode( NULL, pNodeFileShareDocumentManagementServer );

				CExtRibbonNode * pNodeFileShareCreateDocumentWorkspace =
					new CExtRibbonNode(
						ID_FB_SHARE_CREATE_DOC_WKS,
						ID_FB_SHARE_CREATE_DOC_WKS,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU,
						NULL,
						_T("&Create Document Workspace...")
						);
				pNodeFileShareCreateDocumentWorkspace->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('W') ) ), false );
				pNodeFileShareCreateDocumentWorkspace->SetTextMenuExtended(
					_T("Create new site of this document and\nkeep your local copy synchronized.")
					);
				VERIFY(
					pNodeFileShareCreateDocumentWorkspace->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_SHARE_CREATE_DOC_WKS )
						)
					);
				pNodeFileShareCreateDocumentWorkspace->m_iconBig.m_bmpNormal.Make32();
				pNodeFileShareCreateDocumentWorkspace->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFileShare->InsertNode( NULL, pNodeFileShareCreateDocumentWorkspace );

CExtRibbonNode * pNodeFilePrint =
		new CExtRibbonNode(
			ID_FB_PRINT,
			ID_FB_PRINT,
			NULL,
			__ECTN_BIG_ACCENT_IN_MENU|__ECTN_TBB_SEPARATED_DROPDOWN,
			NULL,
			_T("&Print...")
			);
	pNodeFilePrint->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('P') ) ), false );
	pNodeFilePrint->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('W') ) ), false, false );
	pNodeFilePrint->SetTextMenuCaption( _T("Printing Your Document") );
	VERIFY(
		pNodeFilePrint->m_iconBig.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_FB_PRINT )
			)
		);
	pNodeFilePrint->m_iconBig.m_bmpNormal.Make32();
	pNodeFilePrint->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonNodeFile->InsertNode( NULL, pNodeFilePrint );

				CExtRibbonNode * pNodeFilePrintPrint =
					new CExtRibbonNode(
						ID_FB_PRINT,
						ID_FB_PRINT,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU|__ECTN_BIG_ACCENT_IN_MENU,
						NULL,
						_T("&Quick Print")
						);
				pNodeFilePrintPrint->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('Q') ) ), false );
				VERIFY(
					pNodeFilePrintPrint->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_PRINT )
						)
					);
				pNodeFilePrintPrint->m_iconBig.m_bmpNormal.Make32();
				pNodeFilePrintPrint->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFilePrint->InsertNode( NULL, pNodeFilePrintPrint );

				CExtRibbonNode * pNodeFilePringPreview =
					new CExtRibbonNode(
						ID_FB_PRINT_PREVIEW,
						ID_FB_PRINT_PREVIEW,
						NULL,
						__ECTN_BIG_ACCENT_IN_MENU|__ECTN_BOLD_IN_MENU|__ECTN_BIG_ACCENT_IN_MENU,
						NULL,
						_T("Print Pre&view")
						);
				pNodeFilePringPreview->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('V') ) ), false );
				VERIFY(
					pNodeFilePringPreview->m_iconBig.m_bmpNormal.LoadBMP_Resource(
						MAKEINTRESOURCE( ID_FB_PRINT_PREVIEW )
						)
					);
				pNodeFilePringPreview->m_iconBig.m_bmpNormal.Make32();
				pNodeFilePringPreview->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
				pNodeFilePrint->InsertNode( NULL, pNodeFilePringPreview );


CExtRibbonNode * pNodeFileClose =
		new CExtRibbonNode(
			ID_FB_CLOSE,
			ID_FB_CLOSE,
			NULL,
			__ECTN_BIG_ACCENT_IN_MENU|__ECTN_GROUP_START,
			NULL,
			_T("&Close")
			);
	pNodeFileClose->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('C') ) ), false );
	VERIFY(
		pNodeFileClose->m_iconBig.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_FB_CLOSE )
			)
		);
	pNodeFileClose->m_iconBig.m_bmpNormal.Make32();
	pNodeFileClose->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Close") );
							pNodeFileClose->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeFile->InsertNode( NULL, pNodeFileClose );

	return pRibbonNodeFile;
}

CExtRibbonNodeRightButtonsCollection * CMainFrame::_InitRibbon_RightButtonsCollection()
{
CExtRibbonNodeRightButtonsCollection * pRibbonNodeRightButtonsCollection =
		new CExtRibbonNodeRightButtonsCollection;

CExtRibbonNode * pNodeRTL =
		new CExtRibbonNode(
			ID_RTL,
			ID_RTL,
			NULL,
			0L,
			_T("RTL")
			);
	VERIFY(
		pNodeRTL->m_iconSmall.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( IDB_RTL )
			)
		);
	pNodeRTL->m_iconSmall.m_bmpNormal.Make32( false );
	pNodeRTL->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), BYTE(0) );
	pRibbonNodeRightButtonsCollection->InsertNode(
		NULL,
		pNodeRTL
		);

CExtRibbonNode * pNodeAppAbout =
		new CExtRibbonNode(
			ID_APP_ABOUT,
			ID_APP_ABOUT,
			NULL,
			0L,
			_T("About")
			);
	VERIFY(
		pNodeAppAbout->m_iconSmall.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_APP_ABOUT_16x16 )
			)
		);
	pNodeAppAbout->m_iconSmall.m_bmpNormal.PreMultiplyRGBChannels( false );
							CExtCustomizeCmdScreenTip * pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("About") );
							pNodeAppAbout->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeRightButtonsCollection->InsertNode(
		NULL,
		pNodeAppAbout
		);

CExtRibbonNodeMdiRightButtons * pNodeMDI =
		new CExtRibbonNodeMdiRightButtons;
	pRibbonNodeRightButtonsCollection->InsertNode(
		NULL,
		pNodeMDI
		);

	return pRibbonNodeRightButtonsCollection;
}

CExtRibbonNodeQuickAccessButtonsCollection * CMainFrame::_InitRibbon_QuickAccessButtonsCollection()
{
CExtRibbonNodeQuickAccessButtonsCollection * pRibbonNodeQuickAccessButtonsCollection =
		new CExtRibbonNodeQuickAccessButtonsCollection;

CExtRibbonNode * pNodeQA_FileSave =
		new CExtRibbonNode(
			ID_QA_FILE_SAVE,
			ID_QA_FILE_SAVE,
			NULL,
			0L,
			_T("Save")
			);
	VERIFY(
		pNodeQA_FileSave->m_iconSmall.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_QA_FILE_SAVE )
			)
		);
	pNodeQA_FileSave->m_iconSmall.m_bmpNormal.Make32();
	pNodeQA_FileSave->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							CExtCustomizeCmdScreenTip * pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Save") );
							pNodeQA_FileSave->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeQuickAccessButtonsCollection->InsertNode(
		NULL,
		pNodeQA_FileSave
		);

CExtRibbonNode * pNodeQA_EditUndo =
		new CExtRibbonNode(
			ID_QA_EDIT_UNDO,
			ID_QA_EDIT_UNDO,
			NULL,
			0L,
			_T("Undo")
			);
	VERIFY(
		pNodeQA_EditUndo->m_iconSmall.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_QA_EDIT_UNDO )
			)
		);
	pNodeQA_EditUndo->m_iconSmall.m_bmpNormal.Make32();
	pNodeQA_EditUndo->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Undo") );
							pNodeQA_EditUndo->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeQuickAccessButtonsCollection->InsertNode(
		NULL,
		pNodeQA_EditUndo
		);

CExtRibbonNode * pNodeQA_EditRedo =
		new CExtRibbonNode(
			ID_QA_EDIT_REDO,
			ID_QA_EDIT_REDO,
			NULL,
			0L,
			_T("Redo")
			);
	VERIFY(
		pNodeQA_EditRedo->m_iconSmall.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_QA_EDIT_REDO )
			)
		);
	pNodeQA_EditRedo->m_iconSmall.m_bmpNormal.Make32();
	pNodeQA_EditRedo->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Redo") );
							pNodeQA_EditRedo->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeQuickAccessButtonsCollection->InsertNode(
		NULL,
		pNodeQA_EditRedo
		);

CExtRibbonNode * pNodeQA_FilePrint =
		new CExtRibbonNode(
			ID_QA_FILE_PRINT,
			ID_QA_FILE_PRINT,
			NULL,
			0L,
			_T("Print")
			);
	VERIFY(
		pNodeQA_FilePrint->m_iconSmall.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_QA_FILE_PRINT )
			)
		);
	pNodeQA_FilePrint->m_iconSmall.m_bmpNormal.Make32();
	pNodeQA_FilePrint->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Print") );
							pNodeQA_FilePrint->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeQuickAccessButtonsCollection->InsertNode(
		NULL,
		pNodeQA_FilePrint
		);

CExtRibbonNode * pNodeQA_AddDigitalSignature =
		new CExtRibbonNode(
			ID_QA_ADD_DIGITAL_SIGNATURE,
			ID_QA_ADD_DIGITAL_SIGNATURE,
			NULL,
			0L,
			_T("Add a Digital Signature")
			);
	VERIFY(
		pNodeQA_AddDigitalSignature->m_iconSmall.m_bmpNormal.LoadBMP_Resource(
			MAKEINTRESOURCE( ID_QA_ADD_DIGITAL_SIGNATURE )
			)
		);
	pNodeQA_AddDigitalSignature->m_iconSmall.m_bmpNormal.Make32();
	pNodeQA_AddDigitalSignature->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Add digital signature") );
							pNodeQA_AddDigitalSignature->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNodeQuickAccessButtonsCollection->InsertNode(
		NULL,
		pNodeQA_AddDigitalSignature
		);

	return pRibbonNodeQuickAccessButtonsCollection;
}

CExtRibbonNode * CMainFrame::_InitRibbonPage_Home()
{
CExtRibbonNodeTabPage * pRibbonNodeTabPage = new CExtRibbonNodeTabPage( 0L, NULL, _T("Home") );
	pRibbonNodeTabPage->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Home_Clipboard() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Home_Font() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Home_Paragraph() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Home_Style() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Home_Find() );
	return pRibbonNodeTabPage;
}

CExtRibbonNode * CMainFrame::_InitRibbonPage_Insert()
{
CExtRibbonNodeTabPage * pRibbonNodeTabPage = new CExtRibbonNodeTabPage( 0L, NULL, _T("Insert") );
	pRibbonNodeTabPage->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('N') ) ), false );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Insert_Pages() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Insert_Table() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Insert_Illustrations() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Insert_Links() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Insert_HeaderAndFooter() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Insert_Text() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Insert_Symbols() );
	return pRibbonNodeTabPage;
}

CExtRibbonNode * CMainFrame::_InitRibbonPage_PageLayout()
{
CExtRibbonNodeTabPage * pRibbonNodeTabPage = new CExtRibbonNodeTabPage( 0L, NULL, _T("Page Layout") );
	pRibbonNodeTabPage->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('P') ) ), false );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_PageLayout_Themes() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_PageLayout_PageSetup() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_PageLayout_PageBackground() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_PageLayout_Paragraph() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_PageLayout_Arrange() );
	return pRibbonNodeTabPage;
}

CExtRibbonNode * CMainFrame::_InitRibbonPage_References()
{
CExtRibbonNodeTabPage * pRibbonNodeTabPage = new CExtRibbonNodeTabPage( 0L, NULL, _T("References") );
	pRibbonNodeTabPage->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('S') ) ), false );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_References_TableOfContents() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_References_FootNotes() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_References_CitationsAndBibliography() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_References_Captions() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_References_Index() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_References_TableOfAuthorities() );
	return pRibbonNodeTabPage;
}

CExtRibbonNode * CMainFrame::_InitRibbonPage_Mailings()
{
CExtRibbonNodeTabPage * pRibbonNodeTabPage = new CExtRibbonNodeTabPage( 0L, NULL, _T("Mailings") );
	pRibbonNodeTabPage->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('M') ) ), false );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Mailings_Create() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Mailings_StartMailMerge() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Mailings_WriteAndInsertFields() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Mailings_PreviewResults() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Mailings_Finish() );
	return pRibbonNodeTabPage;
}

CExtRibbonNode * CMainFrame::_InitRibbonPage_Review()
{
CExtRibbonNodeTabPage * pRibbonNodeTabPage = new CExtRibbonNodeTabPage( 0L, NULL, _T("Review") );
	pRibbonNodeTabPage->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('R') ) ), false );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Review_Proofing() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Review_Comments() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Review_Tracking() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Review_Changes() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Review_Compare() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_Review_Protect() );
	return pRibbonNodeTabPage;
}

CExtRibbonNode * CMainFrame::_InitRibbonPage_View()
{
CExtRibbonNodeTabPage * pRibbonNodeTabPage = new CExtRibbonNodeTabPage( 0L, NULL, _T("View") );
	pRibbonNodeTabPage->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('W') ) ), false );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_View_DocumentViews() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_View_ShowHide() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_View_Zoom() );
	pRibbonNodeTabPage->InsertNode( NULL, _InitRibbonNode_View_Window() );
	return pRibbonNodeTabPage;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Home_Clipboard()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_CLIPBOARD );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZC") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_PASTE_BIG) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							CExtCustomizeCmdScreenTip * pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Clipboard") );
							pRibbonGroup->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->SetTextInToolbar( _T("Clipboard") );

CExtRibbonNodeDialogLauncher * pRibbonDLB =
		new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_CLIPBOARD );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Clipboard") );
							pCmdScreenTip->TextMainSet( _T("Show the Clipboard\nTask Pane.") );
							pCmdScreenTip->BmpMainGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_CLIPBOARD_DLB_TIP_IMAGE) );
							pCmdScreenTip->BmpMainGet().Make32();
							pCmdScreenTip->BmpMainGet().AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pRibbonDLB->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonDLB->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("FO") ) ), false );
	pRibbonGroup->InsertNode( NULL, pRibbonDLB );

//	pRibbonGroup->InsertNode( NULL, new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_CLIPBOARD ) );
//	pRibbonGroup->InsertNode( NULL, new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_CLIPBOARD ) );
//	pRibbonGroup->InsertNode( NULL, new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_CLIPBOARD ) );

CExtRibbonNode * pNodePaste =
		new CExtRibbonNode( ID_EDIT_PASTE, 0, NULL, 0, _T("Paste") );
	pNodePaste->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('V') ) ), false );
	pNodePaste->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodePaste->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_PASTE_BIG) ) );
	pNodePaste->m_iconBig.m_bmpNormal.Make32();
	pNodePaste->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Paste") );
							pCmdScreenTip->TextMainSet( _T("Paste the contents\nof the Clipboard.") );
							pCmdScreenTip->BmpMainGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_PASTE_BIG) );
							pCmdScreenTip->BmpMainGet().Make32();
							pCmdScreenTip->BmpMainGet().AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip->CaptionSecondarySet( _T("Press F1 for more help.") );
							pCmdScreenTip->BmpSecondaryGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_APP_ABOUT_16x16) );
							pCmdScreenTip->BmpSecondaryGet().PreMultiplyRGBChannels( false );
							pNodePaste->CmdScreenTipAttach( pCmdScreenTip, true );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Paste") );
							pCmdScreenTip->TextMainSet( _T("Click here for more options such as\npasting only the values or\nformatting.") );
							pCmdScreenTip->BmpMainGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_PASTE_BIG) );
							pCmdScreenTip->BmpMainGet().Make32();
							pCmdScreenTip->BmpMainGet().AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip->CaptionSecondarySet( _T("Press F1 for more help.") );
							pCmdScreenTip->BmpSecondaryGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_APP_ABOUT_16x16) );
							pCmdScreenTip->BmpSecondaryGet().PreMultiplyRGBChannels( false );
							pNodePaste->CmdScreenTipAttach( pCmdScreenTip, false );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePaste
		);

// 	g_CmdManager->CmdAllocPtr(
// 		g_CmdManager->ProfileNameFromWnd( m_hWnd ),
// 		ID_EDIT_PASTE )->m_sMenuText = "12";

pNodePaste->ModifyFlags( __ECTN_TBB_SEPARATED_DROPDOWN );
			CExtCustomizeCmdTreeNode * pNodePaste_Paste =
				new CExtCustomizeCmdTreeNode( ID_EDIT_PASTE, ID_EDIT_PASTE, NULL, 0, _T("Paste"), _T("&Paste") );
			pNodePaste_Paste->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('P') ) ), false );
			pNodePaste->InsertNode(
				NULL,
				pNodePaste_Paste
				);

			CExtCustomizeCmdTreeNode * pNodePaste_Special =
				new CExtCustomizeCmdTreeNode( ID_EDIT_PASTE_SPECIAL, ID_EDIT_PASTE_SPECIAL, NULL, 0, _T("Paste &Special...") );
			pNodePaste_Special->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('S') ) ), false );
			pNodePaste->InsertNode(
				NULL,
				pNodePaste_Special
				);

			CExtCustomizeCmdTreeNode * pNodePaste_Hyperlink =
				new CExtCustomizeCmdTreeNode( ID_EDIT_PASTE_AS_HYPERLINK, ID_EDIT_PASTE_AS_HYPERLINK, NULL, 0, _T("Paste as &Hyperlink") );
			pNodePaste_Hyperlink->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
			pNodePaste->InsertNode(
				NULL,
				pNodePaste_Hyperlink
				);

//	pRibbonGroup->InsertNode(
//		NULL,
//		new CExtRibbonNode // separator
//		);

CExtRibbonNode * pNodeCopy =
		new CExtRibbonNode( ID_EDIT_COPY, 0, NULL, 0, _T("Copy") );
	pNodeCopy->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('C') ) ), false );
//pNodeCopy->ModifyFlags( __ECTN_TBB_UNDO_REDO|__ECTN_TBB_SEPARATED_DROPDOWN );
	pNodeCopy->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeCopy->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_COPY_SMALL) ) );
	pNodeCopy->m_iconSmall.m_bmpNormal.Make32();
	pNodeCopy->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Copy") );
							pCmdScreenTip->TextMainSet( _T("Copy the selection and\nput it on the Clipboard.") );
							pNodeCopy->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeCopy
		);

CExtRibbonNode * pNodeCut =
		new CExtRibbonNode( ID_EDIT_CUT, 0, NULL, 0, _T("Cut") );
	pNodeCut->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('X') ) ), false );
	pNodeCut->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeCut->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_CUT_SMALL) ) );
	pNodeCut->m_iconSmall.m_bmpNormal.Make32();
	pNodeCut->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Cut") );
							pCmdScreenTip->TextMainSet( _T("Cut the selection from the document\nand put it on the Clipboard.") );
							pNodeCut->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeCut
		);

CExtRibbonNode * pNodeClearFormatting =
		new CExtRibbonNode( ID_EDIT_CLEAR, 0, NULL, 0, _T("Clear Formatting") );
	pNodeClearFormatting->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("FP") ) ), false );
	pNodeClearFormatting->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeClearFormatting->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_CLEAR_SMALL) ) );
	pNodeClearFormatting->m_iconSmall.m_bmpNormal.Make32();
	pNodeClearFormatting->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Clear Formatting") );
							pCmdScreenTip->TextMainSet( _T("Copy the formatting from one place\nand apply it to an other.\n\nUse this button to apply\nthe same formatting to multiple\nplaces in the document.") );
							pCmdScreenTip->CaptionSecondarySet( _T("Press F1 for more help.") );
							pCmdScreenTip->BmpSecondaryGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_APP_ABOUT_16x16) );
							pCmdScreenTip->BmpSecondaryGet().PreMultiplyRGBChannels( false );
							pNodeClearFormatting->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeClearFormatting
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Home_Font()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeToolGroup( ID_RIBBON_PAGE_FONT );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZF") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_GROUP_SMALL) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Font") );
							CExtCustomizeCmdScreenTip * pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Font") );
							pRibbonGroup->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->RibbonILE_SetCollapsed( __EXT_RIBBON_ILE_MAX - 5 );

CExtRibbonNodeDialogLauncher * pRibbonDLB =
		new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_FONT );
	pRibbonDLB->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("FN") ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Font") );
							pCmdScreenTip->TextMainSet( _T("Show the Font dialog box.") );
							pCmdScreenTip->BmpMainGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_DLB_TIP_IMAGE) );
							pRibbonDLB->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode( NULL, pRibbonDLB );

CExtRibbonNodeGroup * pRibbonNode_FontNameAndSize =
		new CExtRibbonNodeToolGroup( 141 );
	pRibbonGroup->InsertNode( NULL, pRibbonNode_FontNameAndSize );
CArray < DWORD, DWORD > & arrRule_FontNameAndSize =
		pRibbonNode_FontNameAndSize->RibbonILE_RuleArrayGet();
	arrRule_FontNameAndSize.RemoveAll();
	arrRule_FontNameAndSize.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MAX,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			false
			)
		);
	arrRule_FontNameAndSize.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MIN, // __EXT_RIBBON_ILE_MAX - 4,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			true
			)
		);

CExtCustomizeCmdTreeNode * pNodeFontName =
		new CExtRibbonNode( ID_SE_FONT_LIST );
	pNodeFontName->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("FF") ) ), false );
	pNodeFontName->ModifyFlags( __ECTN_TBB_TEXT_FIELD|__ECTN_TBB_COMBO );
INT nFontNameFieldWidth = g_PaintManager.GetPM()->UiScalingDo( 143, CExtPaintManager::__EUIST_X );
	pNodeFontName->TextFieldWidthSet( nFontNameFieldWidth );
	pNodeFontName->DropDownWidthSet( nFontNameFieldWidth );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Font") );
							pCmdScreenTip->TextMainSet( _T("Change then font face.") );
							pNodeFontName->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_FontNameAndSize->InsertNode(
		NULL,
		pNodeFontName
		);
CExtCustomizeCmdTreeNode * pNodeFontSize =
		new CExtRibbonNode( ID_SE_SIZE );
	pNodeFontSize->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("FS") ) ), false );
	pNodeFontSize->ModifyFlags( __ECTN_TBB_TEXT_FIELD|__ECTN_TBB_COMBO );
INT nFontSizeFieldWidth = g_PaintManager.GetPM()->UiScalingDo( 34, CExtPaintManager::__EUIST_X );
INT nFontSizeDropDownWidth = g_PaintManager.GetPM()->UiScalingDo( 44, CExtPaintManager::__EUIST_X );
	pNodeFontSize->TextFieldWidthSet( nFontSizeFieldWidth );
	pNodeFontSize->DropDownWidthSet( nFontSizeDropDownWidth );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Font Size") );
							pCmdScreenTip->TextMainSet( _T("Change then font size.") );
							pNodeFontSize->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_FontNameAndSize->InsertNode(
		NULL,
		pNodeFontSize
		);

CExtRibbonNodeGroup * pRibbonNode_FontSizeUpAndDown =
		new CExtRibbonNodeToolGroup( 135 );
	pRibbonGroup->InsertNode( NULL, pRibbonNode_FontSizeUpAndDown );

CExtRibbonNode * pNodeFontMakeLarger =
		new CExtRibbonNodeToolButton( ID_FONT_MAKE_LARGER );
	pNodeFontMakeLarger->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("FG") ) ), false );
	pNodeFontMakeLarger->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontMakeLarger->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_MAKE_LARGER) ) );
	pNodeFontMakeLarger->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontMakeLarger->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Grow Font") );
							pCmdScreenTip->TextMainSet( _T("Increase the font size.") );
							pNodeFontMakeLarger->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_FontSizeUpAndDown->InsertNode(
		NULL,
		pNodeFontMakeLarger
		);

CExtRibbonNode * pNodeFontMakeSmaller =
		new CExtRibbonNodeToolButton( ID_FONT_MAKE_SMALLER );
	pNodeFontMakeSmaller->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("FK") ) ), false );
	pNodeFontMakeSmaller->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontMakeSmaller->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_MAKE_SMALLER) ) );
	pNodeFontMakeSmaller->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontMakeSmaller->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Shrink Font") );
							pCmdScreenTip->TextMainSet( _T("Decrease the font size.") );
							pNodeFontMakeSmaller->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_FontSizeUpAndDown->InsertNode(
		NULL,
		pNodeFontMakeSmaller
		);

CExtRibbonNodeGroup * pRibbonNode_GroupClearFormatting =
		new CExtRibbonNodeToolGroup( 135 );
CArray < DWORD, DWORD > & arrRule_ClearFormatting =
		pRibbonNode_GroupClearFormatting->RibbonILE_RuleArrayGet();
	arrRule_ClearFormatting.RemoveAll();
	arrRule_ClearFormatting.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MAX,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			true
			)
		);
	arrRule_ClearFormatting.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MIN, // __EXT_RIBBON_ILE_MAX - 4,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			false
			)
		);
	pRibbonGroup->InsertNode( NULL, pRibbonNode_GroupClearFormatting );

CExtRibbonNode * pRibbonNode_ClearFormatting =
		new CExtRibbonNodeToolButton( ID_FONT_CLEAR_FORMATTING );
	pRibbonNode_ClearFormatting->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('E') ) ), false );
	VERIFY( pRibbonNode_ClearFormatting->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_CLEAR_FORMATTING) ) );
	pRibbonNode_ClearFormatting->m_iconSmall.m_bmpNormal.Make32();
	pRibbonNode_ClearFormatting->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Clear Formatting") );
							pCmdScreenTip->TextMainSet( _T("Clear all the formatting from the\nselection, leaving only the plain\ntext.") );
							pCmdScreenTip->CaptionSecondarySet( _T("Press F1 for more help.") );
							pCmdScreenTip->BmpSecondaryGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_APP_ABOUT_16x16) );
							pCmdScreenTip->BmpSecondaryGet().PreMultiplyRGBChannels( false );
							pRibbonNode_ClearFormatting->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_GroupClearFormatting->InsertNode(
		NULL,
		pRibbonNode_ClearFormatting
		);

// 	pRibbonGroup->InsertNode(
// 		NULL,
// 		new CExtRibbonNode // separator
// 		);

CExtRibbonNodeGroup * pRibbonNode_FontAttributes1 =
		new CExtRibbonNodeToolGroup( 136 );
CArray < DWORD, DWORD > & arrRule_FontAttributes1 =
		pRibbonNode_FontAttributes1->RibbonILE_RuleArrayGet();
	arrRule_FontAttributes1.RemoveAll();
	arrRule_FontAttributes1.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MAX,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			false
			)
		);
	arrRule_FontAttributes1.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MIN, // __EXT_RIBBON_ILE_MAX - 4,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			true
			)
		);
	pRibbonGroup->InsertNode( NULL, pRibbonNode_FontAttributes1 );

CExtRibbonNode * pNodeFontBold =
		new CExtRibbonNodeToolButton( ID_FONT_BOLD );
	pNodeFontBold->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('1') ) ), false );
	pNodeFontBold->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontBold->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_BOLD) ) );
	pNodeFontBold->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontBold->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Bold") );
							pCmdScreenTip->TextMainSet( _T("Make the selected text bold.") );
							pNodeFontBold->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_FontAttributes1->InsertNode(
		NULL,
		pNodeFontBold
		);

CExtRibbonNode * pNodeFontItalic =
		new CExtRibbonNodeToolButton( ID_FONT_ITALIC );
	pNodeFontItalic->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('2') ) ), false );
	pNodeFontItalic->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontItalic->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_ITALIC) ) );
	pNodeFontItalic->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontItalic->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Italic") );
							pCmdScreenTip->TextMainSet( _T("Italicize the selected text.") );
							pNodeFontItalic->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_FontAttributes1->InsertNode(
		NULL,
		pNodeFontItalic
		);

CExtRibbonNodeGallery * pNodeFontUnderline =
		new CExtRibbonNodeGallery( ID_FONT_UNDERLINE );
	pNodeFontUnderline->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('3') ) ), false );
	pNodeFontUnderline->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontUnderline->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_UNDERLINE) ) );
	pNodeFontUnderline->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontUnderline->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Underline") );
							pCmdScreenTip->TextMainSet( _T("Underline the selected text.") );
							pNodeFontUnderline->CmdScreenTipAttach( pCmdScreenTip );
							pNodeFontUnderline->CmdScreenTipSet( pCmdScreenTip, true, false );
	pRibbonNode_FontAttributes1->InsertNode(
		NULL,
		pNodeFontUnderline
		);
CArray < DWORD, DWORD > & arrRuleFontUnderline = pNodeFontUnderline->RibbonILE_RuleArrayGet();
	arrRuleFontUnderline.RemoveAll();
	arrRuleFontUnderline.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MAX,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			false
			)
		);
	pNodeFontUnderline->ModifyFlags( __ECTN_TBB_SEPARATED_DROPDOWN );
	pNodeFontUnderline->m_nTpmxAdditionalFlags |= TPMX_RIBBON_RESIZING|TPMX_RIBBON_RESIZING_VERTICAL_ONLY;
	pNodeFontUnderline->m_sizePopupGalleryControl.cx = g_PaintManager.GetPM()->UiScalingDo( 160, CExtPaintManager::__EUIST_X );
	pNodeFontUnderline->m_sizePopupGalleryControlMin.cy = g_PaintManager.GetPM()->UiScalingDo( 35, CExtPaintManager::__EUIST_Y ) + 12;
	pNodeFontUnderline->m_sizePopupGalleryControl.cy = ( pNodeFontUnderline->m_sizePopupGalleryControlMin.cy - 12 ) * 8 + 2;
	pNodeFontUnderline->m_sizePopupGalleryControlMin.cx
		= pNodeFontUnderline->m_sizePopupGalleryControlMax.cx
		= pNodeFontUnderline->m_sizePopupGalleryControl.cx;

			CExtRibbonNode * pNodeFontMoreUnderlines =
				new CExtRibbonNode( ID_FONT_MORE_UNDERLINES, ID_FONT_MORE_UNDERLINES, NULL, 0, _T("&More Underlines...") );
			pNodeFontMoreUnderlines->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('M') ) ), false );
			pNodeFontUnderline->InsertNode(
				NULL,
				pNodeFontMoreUnderlines
				);

			CExtRibbonNode * pNodeFontUnderlineColor =
				new CExtRibbonNode( ID_FONT_UNDERLINE_COLOR, ID_FONT_UNDERLINE_COLOR, NULL, 0, _T("Underline &Color") );
			pNodeFontUnderlineColor->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('U') ) ), false );
			VERIFY( pNodeFontUnderlineColor->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_UNDERLINE_COLOR) ) );
			pNodeFontUnderlineColor->m_iconSmall.m_bmpNormal.Make32();
			pNodeFontUnderlineColor->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pNodeFontUnderlineColor->ModifyFlags( __ECTN_TBB_COLOR|__ECTN_TBB_MCB_CUSTOM|__ECTN_TBB_MCB_DEFAULT );
			pNodeFontUnderline->InsertNode(
				NULL,
				pNodeFontUnderlineColor
				);

CExtRibbonNode * pNodeFontStrikethrough =
		new CExtRibbonNodeToolButton( ID_FONT_STRIKETHROUGH );
	pNodeFontStrikethrough->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('4') ) ), false );
	pNodeFontStrikethrough->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontStrikethrough->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_STRIKETHROUGH) ) );
	pNodeFontStrikethrough->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontStrikethrough->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Strikethrough") );
							pCmdScreenTip->TextMainSet( _T("Draw a line through the middle of\nthe selected text.") );
							pNodeFontStrikethrough->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_FontAttributes1->InsertNode(
		NULL,
		pNodeFontStrikethrough
		);

CExtRibbonNodeGroup * pRibbonNode_FontAttributes2 =
		new CExtRibbonNodeToolGroup( 135 );
	pRibbonGroup->InsertNode( NULL, pRibbonNode_FontAttributes2 );
	
CExtRibbonNode * pNodeFontSubscript =
		new CExtRibbonNodeToolButton( ID_FONT_SUBSCRIPT );
	pNodeFontSubscript->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('5') ) ), false );
	pNodeFontSubscript->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontSubscript->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_SUBSCRIPT) ) );
	pNodeFontSubscript->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontSubscript->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Subscript") );
							pCmdScreenTip->TextMainSet( _T("Create small letters below the text\nbaseline.") );
							pNodeFontSubscript->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_FontAttributes2->InsertNode(
		NULL,
		pNodeFontSubscript
		);

CExtRibbonNode * pNodeFontSuperscript =
		new CExtRibbonNodeToolButton( ID_FONT_SUPERSCRIPT );
	pNodeFontSuperscript->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('6') ) ), false );
	pNodeFontSuperscript->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontSuperscript->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_SUPERSCRIPT) ) );
	pNodeFontSuperscript->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontSuperscript->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Superscript") );
							pCmdScreenTip->TextMainSet( _T("Create small letters above the line\nof text.\n\nTo create a footnote click Insert\nFootnote on the References tab.") );
							pNodeFontSuperscript->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_FontAttributes2->InsertNode(
		NULL,
		pNodeFontSuperscript
		);

CExtRibbonNode * pNodeFontChangeCase =
		new CExtRibbonNodeToolButton( ID_FONT_CHANGE_CASE );
	pNodeFontChangeCase->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('7') ) ), false );
	pNodeFontChangeCase->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontChangeCase->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_CHANGE_CASE) ) );
	pNodeFontChangeCase->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontChangeCase->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Change Case") );
							pCmdScreenTip->TextMainSet( _T("Change all the selected text to\nUPPERCASE, lowercase, or other\ncommon capitalizations.") );
							pCmdScreenTip->CaptionSecondarySet( _T("Press F1 for more help.") );
							pCmdScreenTip->BmpSecondaryGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_APP_ABOUT_16x16) );
							pCmdScreenTip->BmpSecondaryGet().PreMultiplyRGBChannels( false );
							pNodeFontChangeCase->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_FontAttributes2->InsertNode(
		NULL,
		pNodeFontChangeCase
		);
	pNodeFontChangeCase->ModifyFlags( __ECTN_DYNAMIC_POPUP );
			pNodeFontChangeCase->InsertNode(
				NULL,
				new CExtCustomizeCmdTreeNode( ID_FONT_CASE_SENTENCE, ID_FONT_CASE_SENTENCE,
					NULL, 0, _T("&Sentence case.") )
				);
			pNodeFontChangeCase->InsertNode(
				NULL,
				new CExtCustomizeCmdTreeNode( ID_FONT_CASE_LOWER, ID_FONT_CASE_LOWER,
					NULL, 0, _T("&lowercase") )
				);
			pNodeFontChangeCase->InsertNode(
				NULL,
				new CExtRibbonNode( ID_FONT_CASE_UPPPER, ID_FONT_CASE_UPPPER,
					NULL, 0, _T("&UPPERCASE") )
				);
			pNodeFontChangeCase->InsertNode(
				NULL,
				new CExtCustomizeCmdTreeNode( ID_FONT_CASE_CAPITALIZE_EACH_WORD, ID_FONT_CASE_CAPITALIZE_EACH_WORD,
					NULL, 0, _T("&Capitalize Each Word") )
				);
			pNodeFontChangeCase->InsertNode(
				NULL,
				new CExtCustomizeCmdTreeNode( ID_FONT_CASE_TOGGLE, ID_FONT_CASE_TOGGLE,
					NULL, 0, _T("&tOGGLE cASE") )
				);

CExtRibbonNodeGroup * pRibbonNode_Color =
		new CExtRibbonNodeToolGroup( 137 );
	pRibbonGroup->InsertNode( NULL, pRibbonNode_Color );

CExtRibbonNode * pNodeFontCharColor =
		new CExtRibbonNodeToolButton( ID_FONT_CHAR_COLOR );

	pNodeFontCharColor->SetLParam( ID_FONT_CHAR_COLOR );
	
	pNodeFontCharColor->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('I') ) ), false );
	pNodeFontCharColor->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontCharColor->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_CHAR_COLOR) ) );
	pNodeFontCharColor->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontCharColor->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pNodeFontCharColor->ModifyFlags( __ECTN_TBB_COLOR|__ECTN_TBB_MCB_CUSTOM|__ECTN_TBB_MCB_DEFAULT );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Text Color") );
							pCmdScreenTip->TextMainSet( _T("Change the text color.") );
							pCmdScreenTip->CaptionSecondarySet( _T("Press F1 for more help.") );
							pCmdScreenTip->BmpSecondaryGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_APP_ABOUT_16x16) );
							pCmdScreenTip->BmpSecondaryGet().PreMultiplyRGBChannels( false );
							pNodeFontCharColor->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_Color->InsertNode(
		NULL,
		pNodeFontCharColor
		);

CExtRibbonNode * pNodeFontTextHighlightColor =
		new CExtRibbonNodeToolButton( ID_FONT_TEXT_HIGHLIGHT_COLOR );

	pNodeFontTextHighlightColor->SetLParam( ID_FONT_TEXT_HIGHLIGHT_COLOR );

	pNodeFontTextHighlightColor->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("FC") ) ), false );
	pNodeFontTextHighlightColor->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFontTextHighlightColor->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FONT_TEXT_HIGHLIGHT_COLOR) ) );
	pNodeFontTextHighlightColor->m_iconSmall.m_bmpNormal.Make32();
	pNodeFontTextHighlightColor->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pNodeFontTextHighlightColor->ModifyFlags( __ECTN_TBB_COLOR|__ECTN_TBB_MCB_CUSTOM|__ECTN_TBB_MCB_DEFAULT|__ECTN_TBB_SEPARATED_DROPDOWN );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Text Highlight Color") );
							pCmdScreenTip->TextMainSet( _T("Make text look like it was marked\nwith a highlighter pen.") );
							pNodeFontTextHighlightColor->CmdScreenTipAttach( pCmdScreenTip );
							pNodeFontTextHighlightColor->CmdScreenTipSet( pCmdScreenTip, true, false );
	pRibbonNode_Color->InsertNode(
		NULL,
		pNodeFontTextHighlightColor
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Home_Paragraph()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeToolGroup( ID_RIBBON_PAGE_PARAGRAPH );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZP") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_GROUP_SMALL) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Paragraph") );
							CExtCustomizeCmdScreenTip * pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Paragraph") );
							pRibbonGroup->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->RibbonILE_SetCollapsed( __EXT_RIBBON_ILE_MAX - 4 );

CExtRibbonNodeDialogLauncher * pRibbonDLB =
		new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_PARAGRAPH );
	pRibbonDLB->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("PG") ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Paragraph") );
							pCmdScreenTip->TextMainSet( _T("Show the Paragraph dialog box.") );
							pCmdScreenTip->BmpMainGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_DLB_TIP_IMAGE) );
							pRibbonDLB->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode( NULL, pRibbonDLB );

CExtRibbonNodeGroup * pRibbonNode_ParagraphAttributes1 =
		new CExtRibbonNodeToolGroup( 138 );
	pRibbonGroup->InsertNode( NULL, pRibbonNode_ParagraphAttributes1 );

CExtRibbonNodeGallery * pNodeParagraphBullets =
		new CExtRibbonNodeGallery( ID_PARAGRAPH_BULLETS );
	pNodeParagraphBullets->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('U') ) ), false );
	pNodeParagraphBullets->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphBullets->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_BULLETS) ) );
	pNodeParagraphBullets->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphBullets->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Bullets") );
							pCmdScreenTip->TextMainSet( _T("Start a buletted list.\n\nClick the arrow to choose different\nbullet styles.") );
							pNodeParagraphBullets->CmdScreenTipAttach( pCmdScreenTip );
							pNodeParagraphBullets->CmdScreenTipSet( pCmdScreenTip, true, false );
	pRibbonNode_ParagraphAttributes1->InsertNode(
		NULL,
		pNodeParagraphBullets
		);
CArray < DWORD, DWORD > & arrRuleParagraphBullets = pNodeParagraphBullets->RibbonILE_RuleArrayGet();
	arrRuleParagraphBullets.RemoveAll();
	arrRuleParagraphBullets.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MAX,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			false
			)
		);
	pNodeParagraphBullets->ModifyFlags( __ECTN_TBB_SEPARATED_DROPDOWN );
	pNodeParagraphBullets->m_nTpmxAdditionalFlags |= TPMX_RIBBON_RESIZING;
	pNodeParagraphBullets->m_sizePopupGalleryControl.cx = g_PaintManager.GetPM()->UiScalingDo( 310, CExtPaintManager::__EUIST_X );
	pNodeParagraphBullets->m_sizePopupGalleryControl.cy = g_PaintManager.GetPM()->UiScalingDo( 275, CExtPaintManager::__EUIST_Y );
	pNodeParagraphBullets->m_sizePopupGalleryControlMin.cx = g_PaintManager.GetPM()->UiScalingDo( 120, CExtPaintManager::__EUIST_X );
	pNodeParagraphBullets->m_sizePopupGalleryControlMin.cy = g_PaintManager.GetPM()->UiScalingDo( 120, CExtPaintManager::__EUIST_Y );

			CExtRibbonNodeGallery * pNodeChangeListLevel;

			pNodeChangeListLevel =
				new CExtRibbonNodeGallery( ID_CHANGE_LIST_LEVEL, NULL, 0, _T("&Change List Level"), _T("&Change List Level") );
			pNodeChangeListLevel->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('C') ) ), false );
			pNodeParagraphBullets->InsertNode(
				NULL,
				pNodeChangeListLevel
				);
			VERIFY( pNodeChangeListLevel->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGE_LIST_LEVEL) ) );
			pNodeChangeListLevel->m_iconSmall.m_bmpNormal.Make32();
			pNodeChangeListLevel->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pNodeChangeListLevel->m_sizePopupGalleryControl.cx
				= pNodeChangeListLevel->m_sizePopupGalleryControlMin.cx
				= pNodeChangeListLevel->m_sizePopupGalleryControlMax.cx
				= g_PaintManager.GetPM()->UiScalingDo( 210, CExtPaintManager::__EUIST_X );
			pNodeChangeListLevel->m_sizePopupGalleryControl.cy
				= pNodeChangeListLevel->m_sizePopupGalleryControlMin.cy
				= pNodeChangeListLevel->m_sizePopupGalleryControlMax.cy
				= g_PaintManager.GetPM()->UiScalingDo( 355, CExtPaintManager::__EUIST_Y );

			CExtRibbonNode * pNodeDefineNewBullet =
				new CExtRibbonNode( ID_DEFINE_NEW_BULLET, ID_DEFINE_NEW_BULLET, NULL, 0, _T("&Define New Bullet...") );
			pNodeDefineNewBullet->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('D') ) ), false );
			pNodeParagraphBullets->InsertNode(
				NULL,
				pNodeDefineNewBullet
				);

CExtRibbonNodeGallery * pNodeParagraphNumbering =
		new CExtRibbonNodeGallery( ID_PARAGRAPH_NUMBERING );
	pNodeParagraphNumbering->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('N') ) ), false );
	pNodeParagraphNumbering->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphNumbering->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_NUMBERING) ) );
	pNodeParagraphNumbering->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphNumbering->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Numbering") );
							pCmdScreenTip->TextMainSet( _T("Start a numbered list.\n\nClick the arrow to choose different\nnumbering formats.") );
							pNodeParagraphNumbering->CmdScreenTipAttach( pCmdScreenTip );
							pNodeParagraphNumbering->CmdScreenTipSet( pCmdScreenTip, true, false );
	pRibbonNode_ParagraphAttributes1->InsertNode(
		NULL,
		pNodeParagraphNumbering
		);
CArray < DWORD, DWORD > & arrRuleParagraphNumbering = pNodeParagraphNumbering->RibbonILE_RuleArrayGet();
	arrRuleParagraphNumbering.RemoveAll();
	arrRuleParagraphNumbering.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MAX,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			false
			)
		);
	pNodeParagraphNumbering->ModifyFlags( __ECTN_TBB_SEPARATED_DROPDOWN );
	pNodeParagraphNumbering->m_nTpmxAdditionalFlags |= TPMX_RIBBON_RESIZING;
	pNodeParagraphNumbering->m_sizePopupGalleryControl.cx = g_PaintManager.GetPM()->UiScalingDo( 270, CExtPaintManager::__EUIST_X );
	pNodeParagraphNumbering->m_sizePopupGalleryControl.cy = g_PaintManager.GetPM()->UiScalingDo( 390, CExtPaintManager::__EUIST_Y );
	pNodeParagraphNumbering->m_sizePopupGalleryControlMin.cx = g_PaintManager.GetPM()->UiScalingDo( 220, CExtPaintManager::__EUIST_X );
	pNodeParagraphNumbering->m_sizePopupGalleryControlMin.cy = g_PaintManager.GetPM()->UiScalingDo( 220, CExtPaintManager::__EUIST_Y );

			pNodeChangeListLevel =
				new CExtRibbonNodeGallery( ID_CHANGE_LIST_LEVEL, NULL, 0, _T("&Change List Level"), _T("&Change List Level") );
			pNodeChangeListLevel->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('C') ) ), false );
			pNodeParagraphNumbering->InsertNode(
				NULL,
				pNodeChangeListLevel
				);
			VERIFY( pNodeChangeListLevel->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGE_LIST_LEVEL) ) );
			pNodeChangeListLevel->m_iconSmall.m_bmpNormal.Make32();
			pNodeChangeListLevel->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pNodeChangeListLevel->m_sizePopupGalleryControl.cx
				= pNodeChangeListLevel->m_sizePopupGalleryControlMin.cx
				= pNodeChangeListLevel->m_sizePopupGalleryControlMax.cx
				= g_PaintManager.GetPM()->UiScalingDo( 210, CExtPaintManager::__EUIST_X );
			pNodeChangeListLevel->m_sizePopupGalleryControl.cy
				= pNodeChangeListLevel->m_sizePopupGalleryControlMin.cy
				= pNodeChangeListLevel->m_sizePopupGalleryControlMax.cy
				= g_PaintManager.GetPM()->UiScalingDo( 355, CExtPaintManager::__EUIST_Y );

			CExtRibbonNode * pNodeDefineNewNumberFormat =
				new CExtRibbonNode( ID_DEFINE_NEW_BULLET, ID_DEFINE_NEW_BULLET, NULL, 0, _T("&Define New Number Format...") );
			pNodeDefineNewNumberFormat->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('D') ) ), false );
			pNodeParagraphNumbering->InsertNode(
				NULL,
				pNodeDefineNewNumberFormat
				);

			CExtRibbonNode * pNodeSetNumeringValue =
				new CExtRibbonNode( ID_SET_NUMBERING_VALUE, ID_SET_NUMBERING_VALUE, NULL, 0, _T("Set &Numering Value...") );
			pNodeSetNumeringValue->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('N') ) ), false );
			pNodeParagraphNumbering->InsertNode(
				NULL,
				pNodeSetNumeringValue
				);
			VERIFY( pNodeSetNumeringValue->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_SET_NUMBERING_VALUE) ) );
			pNodeSetNumeringValue->m_iconSmall.m_bmpNormal.Make32();
			pNodeSetNumeringValue->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );

CExtRibbonNodeGallery * pNodeParagraphMultilevelList =
		new CExtRibbonNodeGallery( ID_PARAGRAPH_MULTILEVEL_LIST );
	pNodeParagraphMultilevelList->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('M') ) ), false );
	pNodeParagraphMultilevelList->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphMultilevelList->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_MULTILEVEL_LIST) ) );
	pNodeParagraphMultilevelList->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphMultilevelList->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Numbering") );
							pCmdScreenTip->TextMainSet( _T("Start a multilevel list.\n\nClick the arrow to choose different\nmultilevel list styles.") );
							pNodeParagraphMultilevelList->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes1->InsertNode(
		NULL,
		pNodeParagraphMultilevelList
		);
CArray < DWORD, DWORD > & arrRuleParagraphMultilevelList = pNodeParagraphMultilevelList->RibbonILE_RuleArrayGet();
	arrRuleParagraphMultilevelList.RemoveAll();
	arrRuleParagraphMultilevelList.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MAX,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			false
			)
		);
//	pNodeParagraphMultilevelList->ModifyFlags( __ECTN_TBB_SEPARATED_DROPDOWN );
	pNodeParagraphMultilevelList->m_nTpmxAdditionalFlags |= TPMX_RIBBON_RESIZING;
	pNodeParagraphMultilevelList->m_sizePopupGalleryControl.cx = g_PaintManager.GetPM()->UiScalingDo( 270, CExtPaintManager::__EUIST_X );
	pNodeParagraphMultilevelList->m_sizePopupGalleryControl.cy = g_PaintManager.GetPM()->UiScalingDo( 500, CExtPaintManager::__EUIST_Y );
	pNodeParagraphMultilevelList->m_sizePopupGalleryControlMin.cx = g_PaintManager.GetPM()->UiScalingDo( 220, CExtPaintManager::__EUIST_X );
	pNodeParagraphMultilevelList->m_sizePopupGalleryControlMin.cy = g_PaintManager.GetPM()->UiScalingDo( 220, CExtPaintManager::__EUIST_Y );

			pNodeChangeListLevel =
				new CExtRibbonNodeGallery( ID_CHANGE_LIST_LEVEL, NULL, 0, _T("&Change List Level"), _T("&Change List Level") );
			pNodeChangeListLevel->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('C') ) ), false );
			pNodeParagraphMultilevelList->InsertNode(
				NULL,
				pNodeChangeListLevel
				);
			VERIFY( pNodeChangeListLevel->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGE_LIST_LEVEL) ) );
			pNodeChangeListLevel->m_iconSmall.m_bmpNormal.Make32();
			pNodeChangeListLevel->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pNodeChangeListLevel->m_sizePopupGalleryControl.cx
				= pNodeChangeListLevel->m_sizePopupGalleryControlMin.cx
				= pNodeChangeListLevel->m_sizePopupGalleryControlMax.cx
				= g_PaintManager.GetPM()->UiScalingDo( 210, CExtPaintManager::__EUIST_X );
			pNodeChangeListLevel->m_sizePopupGalleryControl.cy
				= pNodeChangeListLevel->m_sizePopupGalleryControlMin.cy
				= pNodeChangeListLevel->m_sizePopupGalleryControlMax.cy
				= g_PaintManager.GetPM()->UiScalingDo( 355, CExtPaintManager::__EUIST_Y );

			CExtRibbonNode * pNodeDefineNewMultilevelList =
				new CExtRibbonNode( ID_DEFINE_NEW_MULTILEVEL_LIST, ID_DEFINE_NEW_MULTILEVEL_LIST, NULL, 0, _T("&Define New Multilevel List...") );
			pNodeDefineNewMultilevelList->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('D') ) ), false );
			pNodeParagraphMultilevelList->InsertNode(
				NULL,
				pNodeDefineNewMultilevelList
				);

			CExtRibbonNode * pNodeDefineNewListStyle =
				new CExtRibbonNode( ID_DEFINE_NEW_LIST_STYLE, ID_DEFINE_NEW_LIST_STYLE, NULL, 0, _T("Define New &List Style...") );
			pNodeDefineNewListStyle->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('L') ) ), false );
			pNodeParagraphMultilevelList->InsertNode(
				NULL,
				pNodeDefineNewListStyle
				);


CExtRibbonNodeGroup * pRibbonNode_ParagraphAttributes2 =
		new CExtRibbonNodeToolGroup( 139 );
CArray < DWORD, DWORD > & arrRule_ParagraphAttributes2 =
		pRibbonNode_ParagraphAttributes2->RibbonILE_RuleArrayGet();
	arrRule_ParagraphAttributes2.RemoveAll();
	arrRule_ParagraphAttributes2.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MAX,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			false
			)
		);
	arrRule_ParagraphAttributes2.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MIN, // __EXT_RIBBON_ILE_MAX - 4,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			true
			)
		);
	pRibbonGroup->InsertNode( NULL, pRibbonNode_ParagraphAttributes2 );

CExtRibbonNode * pNodeParagraphIncreaseIndent =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_INCREASE_INDENT );
	pNodeParagraphIncreaseIndent->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AI") ) ), false );
	pNodeParagraphIncreaseIndent->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphIncreaseIndent->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_INCREASE_INDENT) ) );
	pNodeParagraphIncreaseIndent->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphIncreaseIndent->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Increase Indent") );
							pCmdScreenTip->TextMainSet( _T("Increases the indent level of the\nparagraph.") );
							pNodeParagraphIncreaseIndent->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes2->InsertNode(
		NULL,
		pNodeParagraphIncreaseIndent
		);
	
CExtRibbonNode * pNodeParagraphDecreaseIndent =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_DECREASE_INDENT );
	pNodeParagraphDecreaseIndent->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AO") ) ), false );
	pNodeParagraphDecreaseIndent->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphDecreaseIndent->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_DECREASE_INDENT) ) );
	pNodeParagraphDecreaseIndent->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphDecreaseIndent->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Decrease Indent") );
							pCmdScreenTip->TextMainSet( _T("Decreases the indent level of the\nparagraph.") );
							pNodeParagraphDecreaseIndent->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes2->InsertNode(
		NULL,
		pNodeParagraphDecreaseIndent
		);

CExtRibbonNodeGroup * pRibbonNode_ParagraphAttributes3 =
		new CExtRibbonNodeToolGroup( 139 );
CArray < DWORD, DWORD > & arrRule_ParagraphAttributes3 =
		pRibbonNode_ParagraphAttributes3->RibbonILE_RuleArrayGet();
	arrRule_ParagraphAttributes3.RemoveAll();
	arrRule_ParagraphAttributes3.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MAX,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			true
			)
		);
	arrRule_ParagraphAttributes3.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MIN, // __EXT_RIBBON_ILE_MAX - 4,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			false
			)
		);
	pRibbonGroup->InsertNode( NULL, pRibbonNode_ParagraphAttributes3 );

CExtRibbonNode * pNodeParagraphLineSpacing =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_LINE_SPACING );
	pNodeParagraphLineSpacing->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('K') ) ), false );
	VERIFY( pNodeParagraphLineSpacing->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_LINE_SPACING) ) );
	pNodeParagraphLineSpacing->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphLineSpacing->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Line spacing") );
							pCmdScreenTip->TextMainSet( _T("Change the spacing between lines\nof text.\n\nYou can also customize the amount\nof space added before and after\nparagraphs.") );
							pNodeParagraphLineSpacing->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes3->InsertNode(
		NULL,
		pNodeParagraphLineSpacing
		);
			pNodeParagraphLineSpacing->InsertNode(
				NULL,
				new CExtCustomizeCmdTreeNode( ID_PARAGRAPH_LINE_SPACING_10, ID_PARAGRAPH_LINE_SPACING_10, NULL, 0, _T("1.0"), _T("1.0") )
				);
			pNodeParagraphLineSpacing->InsertNode(
				NULL,
				new CExtCustomizeCmdTreeNode( ID_PARAGRAPH_LINE_SPACING_15, ID_PARAGRAPH_LINE_SPACING_10, NULL, 0, _T("1.5"), _T("1.5") )
				);
			pNodeParagraphLineSpacing->InsertNode(
				NULL,
				new CExtCustomizeCmdTreeNode( ID_PARAGRAPH_LINE_SPACING_20, ID_PARAGRAPH_LINE_SPACING_10, NULL, 0, _T("2.0"), _T("2.0") )
				);
			pNodeParagraphLineSpacing->InsertNode(
				NULL,
				new CExtCustomizeCmdTreeNode( ID_PARAGRAPH_LINE_SPACING_25, ID_PARAGRAPH_LINE_SPACING_10, NULL, 0, _T("2.5"), _T("2.5") )
				);
			pNodeParagraphLineSpacing->InsertNode(
				NULL,
				new CExtCustomizeCmdTreeNode( ID_PARAGRAPH_LINE_SPACING_30, ID_PARAGRAPH_LINE_SPACING_10, NULL, 0, _T("3.0"), _T("3.0") )
				);
			pNodeParagraphLineSpacing->InsertNode(
				NULL,
				new CExtCustomizeCmdTreeNode( ID_PARAGRAPH_LINE_SPACING_MORE, ID_PARAGRAPH_LINE_SPACING_MORE, NULL, 0, _T("More..."), _T("More...") )
				);
			CExtCustomizeCmdTreeNode * pNodeLineSpacing_Before =
				new CExtCustomizeCmdTreeNode( ID_PARAGRAPH_LINE_SPACING_SPACE_BEFORE, ID_PARAGRAPH_LINE_SPACING_SPACE_BEFORE, NULL, __ECTN_GROUP_START, _T("Space &Before"), _T("Space &Before") );
			pNodeLineSpacing_Before->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('B') ) ), false );
			pNodeParagraphLineSpacing->InsertNode(
				NULL,
				pNodeLineSpacing_Before
				);
			CExtCustomizeCmdTreeNode * pNodeLineSpacing_After =
				new CExtCustomizeCmdTreeNode( ID_PARAGRAPH_LINE_SPACING_NO_SPACE_AFTER, ID_PARAGRAPH_LINE_SPACING_NO_SPACE_AFTER, NULL, 0, _T("No Space &After"), _T("No Space &After") );
			pNodeLineSpacing_After->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('A') ) ), false );
			pNodeParagraphLineSpacing->InsertNode(
				NULL,
				pNodeLineSpacing_After
				);

CExtRibbonNodeGroup * pRibbonNode_ParagraphAttributes4 =
		new CExtRibbonNodeToolGroup( 140 );
CArray < DWORD, DWORD > & arrRule_ParagraphAttributes4 =
		pRibbonNode_ParagraphAttributes4->RibbonILE_RuleArrayGet();
	arrRule_ParagraphAttributes4.RemoveAll();
	arrRule_ParagraphAttributes4.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MAX,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			false
			)
		);
	arrRule_ParagraphAttributes4.Add(
		__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
			__EXT_RIBBON_ILE_MIN, // __EXT_RIBBON_ILE_MAX - 4,
			__EXT_RIBBON_ILV_SIMPLE_SMALL,
			true
			)
		);
	pRibbonGroup->InsertNode( NULL, pRibbonNode_ParagraphAttributes4 );

CExtRibbonNode * pNodeParagraphAlignLeft =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_ALIGN_LEFT );
	pNodeParagraphAlignLeft->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AL") ) ), false );
	pNodeParagraphAlignLeft->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphAlignLeft->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_ALIGN_LEFT) ) );
	pNodeParagraphAlignLeft->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphAlignLeft->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Align Text Left") );
							pCmdScreenTip->TextMainSet( _T("Align text to the left.") );
							pNodeParagraphAlignLeft->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes4->InsertNode(
		NULL,
		pNodeParagraphAlignLeft
		);
	
CExtRibbonNode * pNodeParagraphAlignCenter =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_ALIGN_CENTER );
	pNodeParagraphAlignCenter->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AC") ) ), false );
	pNodeParagraphAlignCenter->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphAlignCenter->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_ALIGN_CENTER) ) );
	pNodeParagraphAlignCenter->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphAlignCenter->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Center") );
							pCmdScreenTip->TextMainSet( _T("Center text.") );
							pNodeParagraphAlignCenter->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes4->InsertNode(
		NULL,
		pNodeParagraphAlignCenter
		);

CExtRibbonNode * pNodeParagraphAlignRight =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_ALIGN_RIGHT );
	pNodeParagraphAlignRight->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AR") ) ), false );
	pNodeParagraphAlignRight->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphAlignRight->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_ALIGN_RIGHT) ) );
	pNodeParagraphAlignRight->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphAlignRight->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Align Text Right") );
							pCmdScreenTip->TextMainSet( _T("Align text to the right.") );
							pNodeParagraphAlignRight->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes4->InsertNode(
		NULL,
		pNodeParagraphAlignRight
		);

CExtRibbonNode * pNodeParagraphAlignJustify =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_ALIGN_JUSTIFY );
	pNodeParagraphAlignJustify->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AJ") ) ), false );
	pNodeParagraphAlignJustify->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphAlignJustify->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_ALIGN_JUSTIFY) ) );
	pNodeParagraphAlignJustify->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphAlignJustify->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Justify") );
							pCmdScreenTip->TextMainSet( _T("Align text to both the left and right\nmargins, adding extra space\nbetween words as necessary.\n\nThis creates a clean look along the\nleft and right side of the page.") );
							pNodeParagraphAlignJustify->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes4->InsertNode(
		NULL,
		pNodeParagraphAlignJustify
		);

CExtRibbonNodeGroup * pRibbonNode_ParagraphAttributes5 =
		new CExtRibbonNodeToolGroup( 139 );
CArray < DWORD, DWORD > & arrRule_ParagraphAttributes5 =
		pRibbonNode_ParagraphAttributes5->RibbonILE_RuleArrayGet();
	arrRule_ParagraphAttributes5.RemoveAll();
	pRibbonGroup->InsertNode(
		NULL,
		pRibbonNode_ParagraphAttributes5
		);

CExtRibbonNode * pNodeParagraphShading =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_SHADING );
	pNodeParagraphShading->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
	pNodeParagraphShading->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphShading->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_SHADING) ) );
	pNodeParagraphShading->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphShading->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
//	pNodeParagraphShading->ModifyFlags( __ECTN_TBB_COLOR|__ECTN_TBB_MCB_CUSTOM|__ECTN_TBB_MCB_DEFAULT );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Shading") );
							pCmdScreenTip->TextMainSet( _T("Color the background behind the\nselected text of paragraph.") );
							pCmdScreenTip->CaptionSecondarySet( _T("Press F1 for more help.") );
							pCmdScreenTip->BmpSecondaryGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_APP_ABOUT_16x16) );
							pCmdScreenTip->BmpSecondaryGet().PreMultiplyRGBChannels( false );
							pNodeParagraphShading->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes5->InsertNode(
		NULL,
		pNodeParagraphShading
		);

CExtRibbonNode * pNodeParagraphBorder =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_BORDER );
	pNodeParagraphBorder->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('B') ) ), false );
	pNodeParagraphBorder->SetCmdID( ID_PB_OUTSIDE_BORDERS, true );
	pNodeParagraphBorder->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphBorder->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_BORDER) ) );
	pNodeParagraphBorder->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphBorder->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Borders") );
							pCmdScreenTip->TextMainSet( _T("Customize the borders of the\nselected text.") );
							pCmdScreenTip->CaptionSecondarySet( _T("Press F1 for more help.") );
							pCmdScreenTip->BmpSecondaryGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_APP_ABOUT_16x16) );
							pCmdScreenTip->BmpSecondaryGet().PreMultiplyRGBChannels( false );
							pNodeParagraphBorder->CmdScreenTipAttach( pCmdScreenTip );
							pNodeParagraphBorder->CmdScreenTipSet( pCmdScreenTip, true, false );
	pRibbonNode_ParagraphAttributes5->InsertNode(
		NULL,
		pNodeParagraphBorder
		);
	pNodeParagraphBorder->ModifyFlags( __ECTN_TBB_SEPARATED_DROPDOWN|__ECTN_TBB_AUTOCHANGE_ID );
	CExtCustomizeCmdTreeNode * pTempNode123 = NULL;
	CExtCmdIcon _iconTmp123;

			VERIFY( _iconTmp123.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PB_NO_BORDER) ) );
			_iconTmp123.m_bmpNormal.Make32();
			_iconTmp123.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pTempNode123 = new CExtCustomizeCmdTreeNode(
				ID_PB_NO_BORDER, ID_PB_NO_BORDER, NULL, 0,
				_T("&No Border"), _T("&No Border"), _T(""), 0L, &_iconTmp123 );
			pTempNode123->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('N') ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("No Border.") );
							pTempNode123->CmdScreenTipAttach( pCmdScreenTip );
							pTempNode123->CmdScreenTipSet( pCmdScreenTip, true, false );
			pNodeParagraphBorder->InsertNode( NULL, pTempNode123 );

			VERIFY( _iconTmp123.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PB_ALL_BORDERS) ) );
			_iconTmp123.m_bmpNormal.Make32();
			_iconTmp123.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pTempNode123 = new CExtCustomizeCmdTreeNode(
				ID_PB_ALL_BORDERS, ID_PB_ALL_BORDERS, NULL, 0,
				_T("&All Borders"), _T("&All Borders"), _T(""), 0L, &_iconTmp123 );
			pTempNode123->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('A') ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("All Borders.") );
							pTempNode123->CmdScreenTipAttach( pCmdScreenTip );
							pTempNode123->CmdScreenTipSet( pCmdScreenTip, true, false );
			pNodeParagraphBorder->InsertNode( NULL, pTempNode123 );

			VERIFY( _iconTmp123.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PB_OUTSIDE_BORDERS) ) );
			_iconTmp123.m_bmpNormal.Make32();
			_iconTmp123.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pTempNode123 = new CExtCustomizeCmdTreeNode(
				ID_PB_OUTSIDE_BORDERS, ID_PB_OUTSIDE_BORDERS, NULL, 0,
				_T("&Outside Borders"), _T("&Outside Borders"), _T(""), 0L, &_iconTmp123 );
			pTempNode123->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('O') ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Outside Borders.") );
							pTempNode123->CmdScreenTipAttach( pCmdScreenTip );
							pTempNode123->CmdScreenTipSet( pCmdScreenTip, true, false );
			pNodeParagraphBorder->InsertNode( NULL, pTempNode123 );

			VERIFY( _iconTmp123.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PB_INSIDE_BORDERS) ) );
			_iconTmp123.m_bmpNormal.Make32();
			_iconTmp123.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pTempNode123 = new CExtCustomizeCmdTreeNode(
				ID_PB_INSIDE_BORDERS, ID_PB_INSIDE_BORDERS, NULL, 0,
				_T("&Inside Borders"), _T("&Inside Borders"), _T(""), 0L, &_iconTmp123 );
			pTempNode123->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('I') ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Inside Borders.") );
							pTempNode123->CmdScreenTipAttach( pCmdScreenTip );
							pTempNode123->CmdScreenTipSet( pCmdScreenTip, true, false );
			pNodeParagraphBorder->InsertNode( NULL, pTempNode123 );

			VERIFY( _iconTmp123.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PB_TOP_BORDER) ) );
			_iconTmp123.m_bmpNormal.Make32();
			_iconTmp123.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pTempNode123 = new CExtCustomizeCmdTreeNode(
				ID_PB_TOP_BORDER, ID_PB_TOP_BORDER, NULL, __ECTN_GROUP_START,
				_T("To&p Border"), _T("To&p Border"), _T(""), 0L, &_iconTmp123 );
			pTempNode123->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('P') ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Top Border.") );
							pTempNode123->CmdScreenTipAttach( pCmdScreenTip );
							pTempNode123->CmdScreenTipSet( pCmdScreenTip, true, false );
			pNodeParagraphBorder->InsertNode( NULL, pTempNode123 );

			VERIFY( _iconTmp123.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PB_BOTTOM_BORDER) ) );
			_iconTmp123.m_bmpNormal.Make32();
			_iconTmp123.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pTempNode123 = new CExtCustomizeCmdTreeNode(
				ID_PB_BOTTOM_BORDER, ID_PB_BOTTOM_BORDER, NULL, 0,
				_T("&Bottom Border"), _T("&Bottom Border"), _T(""), 0L, &_iconTmp123 );
			pTempNode123->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('B') ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Bottom Border.") );
							pTempNode123->CmdScreenTipAttach( pCmdScreenTip );
							pTempNode123->CmdScreenTipSet( pCmdScreenTip, true, false );
			pNodeParagraphBorder->InsertNode( NULL, pTempNode123 );

			VERIFY( _iconTmp123.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PB_LEFT_BORDER) ) );
			_iconTmp123.m_bmpNormal.Make32();
			_iconTmp123.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pTempNode123 = new CExtCustomizeCmdTreeNode(
				ID_PB_LEFT_BORDER, ID_PB_LEFT_BORDER, NULL, 0,
				_T("&Left Border"), _T("&Left Border"), _T(""), 0L, &_iconTmp123 );
			pTempNode123->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('L') ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Left Border.") );
							pTempNode123->CmdScreenTipAttach( pCmdScreenTip );
							pTempNode123->CmdScreenTipSet( pCmdScreenTip, true, false );
			pNodeParagraphBorder->InsertNode( NULL, pTempNode123 );

			VERIFY( _iconTmp123.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PB_RIGHT_BORDER) ) );
			_iconTmp123.m_bmpNormal.Make32();
			_iconTmp123.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pTempNode123 = new CExtCustomizeCmdTreeNode(
				ID_PB_RIGHT_BORDER, ID_PB_RIGHT_BORDER, NULL, 0,
				_T("&Right Border"), _T("&Right Border"), _T(""), 0L, &_iconTmp123 );
			pTempNode123->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('R') ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Right Border.") );
							pTempNode123->CmdScreenTipAttach( pCmdScreenTip );
							pTempNode123->CmdScreenTipSet( pCmdScreenTip, true, false );
			pNodeParagraphBorder->InsertNode( NULL, pTempNode123 );

			VERIFY( _iconTmp123.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PB_INSIDE_BORDER_H) ) );
			_iconTmp123.m_bmpNormal.Make32();
			_iconTmp123.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pTempNode123 = new CExtCustomizeCmdTreeNode(
				ID_PB_INSIDE_BORDER_H, ID_PB_INSIDE_BORDER_H, NULL, __ECTN_GROUP_START,
				_T("Inside &Horizontal Border"), _T("Inside &Horizontal Border"), _T(""), 0L, &_iconTmp123 );
			pTempNode123->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Inside Horizontal Border.") );
							pTempNode123->CmdScreenTipAttach( pCmdScreenTip );
							pTempNode123->CmdScreenTipSet( pCmdScreenTip, true, false );
			pNodeParagraphBorder->InsertNode( NULL, pTempNode123 );

			VERIFY( _iconTmp123.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PB_INSIDE_BORDER_V) ) );
			_iconTmp123.m_bmpNormal.Make32();
			_iconTmp123.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pTempNode123 = new CExtCustomizeCmdTreeNode(
				ID_PB_INSIDE_BORDER_V, ID_PB_INSIDE_BORDER_V, NULL, 0,
				_T("Inside &Vertical Border"), _T("Inside &Vertical Border"), _T(""), 0L, &_iconTmp123 );
			pTempNode123->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('V') ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Inside Vertical Border.") );
							pTempNode123->CmdScreenTipAttach( pCmdScreenTip );
							pTempNode123->CmdScreenTipSet( pCmdScreenTip, true, false );
			pNodeParagraphBorder->InsertNode( NULL, pTempNode123 );

CExtRibbonNode * pNodeParagraphSort =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_SORT );
	pNodeParagraphSort->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("SO") ) ), false );
	pNodeParagraphSort->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphSort->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_SORT) ) );
	pNodeParagraphSort->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphSort->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Sort") );
							pCmdScreenTip->TextMainSet( _T("Alphabetize the selected text or\nsort numerical data.") );
							pCmdScreenTip->CaptionSecondarySet( _T("Press F1 for more help.") );
							pCmdScreenTip->BmpSecondaryGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_APP_ABOUT_16x16) );
							pCmdScreenTip->BmpSecondaryGet().PreMultiplyRGBChannels( false );
							pNodeParagraphSort->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes5->InsertNode(
		NULL,
		pNodeParagraphSort
		);

CExtRibbonNode * pNodeParagraphMarks =
		new CExtRibbonNodeToolButton( ID_PARAGRAPH_MARKS );
	pNodeParagraphMarks->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('8') ) ), false );
	pNodeParagraphMarks->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeParagraphMarks->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_MARKS) ) );
	pNodeParagraphMarks->m_iconSmall.m_bmpNormal.Make32();
	pNodeParagraphMarks->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Show paragraph marks and other hidden formatting symbols.") );
							pNodeParagraphMarks->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonNode_ParagraphAttributes5->InsertNode(
		NULL,
		pNodeParagraphMarks
		);

// CExtRibbonNode * pNodeParagraphxx =
// 		new CExtRibbonNodeToolButton( ID_PARAGRAPH_XX );
// 	pNodeParagraphxx->RibbonILE_RuleArrayGet().RemoveAll();
// 	VERIFY( pNodeParagraphxx->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARAGRAPH_XX) ) );
// 	pNodeParagraphxx->m_iconSmall.m_bmpNormal.Make32();
// 	pNodeParagraphxx->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
// 	pRibbonGroup->InsertNode(
// 		NULL,
// 		pNodeParagraphxx
// 		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Home_Style()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_STYLE );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZS") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_QUICK_STYLES_BIG) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							CExtCustomizeCmdScreenTip * pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Style") );
							pRibbonGroup->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->SetTextInToolbar( _T("Style") );
	//pRibbonGroup->RibbonILE_SetCollapsed( __EXT_RIBBON_ILE_MAX - 6 );

CExtRibbonNodeDialogLauncher * pRibbonDLB =
		new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_STYLE );
	pRibbonDLB->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("FY") ) ), false );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Styles") );
							pCmdScreenTip->TextMainSet( _T("Show the Styles window.") );
							pCmdScreenTip->BmpMainGet().LoadBMP_Resource( MAKEINTRESOURCE(ID_STYLE_DLB_TIP_IMAGE) );
							pRibbonDLB->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode( NULL, pRibbonDLB );

CExtRibbonNodeGallery * pNodeQuickStyles =
		new CExtRibbonNodeGallery( ID_QUICK_STYLES_BIG, NULL, 0, _T("Quick\nStyles") );
	pNodeQuickStyles->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('L') ) ), false );
	pNodeQuickStyles->m_nTpmxAdditionalFlags |= TPMX_RIBBON_RESIZING;
	pNodeQuickStyles->m_sizePopupGalleryControl.cy = g_PaintManager.GetPM()->UiScalingDo( 58*4, CExtPaintManager::__EUIST_Y );
	pNodeQuickStyles->RibbonILE_RuleRemoveEntriesByILV( true, true, false );
CArray < DWORD, DWORD > & arrRule_QuickStyles = pNodeQuickStyles->RibbonILE_RuleArrayGet();
INT nQuickIndex, nQuickCount = 5;
	for( nQuickIndex = 0; nQuickIndex < nQuickCount; nQuickIndex ++ )
	{
		arrRule_QuickStyles.InsertAt(
			0,
			__EXT_RIBBON_MAKE_RULE_ARRAY_ENTRY(
				( __EXT_RIBBON_ILE_MIN + 3 + nQuickIndex ),
				( ( 76 * ( nQuickIndex + 2 ) ) + 16 ),
				false
				)
			);
	} // for( nQuickIndex = 0; nQuickIndex < nQuickCount; nQuickIndex ++ )
	VERIFY( pNodeQuickStyles->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_QUICK_STYLES_BIG) ) );
	pNodeQuickStyles->m_iconBig.m_bmpNormal.Make32();
	pNodeQuickStyles->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Quick Styles") );
							pCmdScreenTip->TextMainSet( _T("Format titles, quotes, and other\ntext using this gallery of styles.") );
							pNodeQuickStyles->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeQuickStyles
		);

CExtRibbonNode * pNodeSaveSelAsNewQuickStyle =
		new CExtRibbonNode( ID_SAVE_SELECTION_AS_NEW_QUICK_STYLE, 0, NULL, 0, _T("Save Selection as a New &Quick Style") );
	pNodeSaveSelAsNewQuickStyle->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('Q') ) ), false );
	pNodeSaveSelAsNewQuickStyle->ModifyFlags( __ECTN_GROUP_START );
	pNodeQuickStyles->InsertNode(
		NULL,
		pNodeSaveSelAsNewQuickStyle
		);
CExtRibbonNode * pNodeClearFormatting =
		new CExtRibbonNode( ID_CLEAR_FORMATTING, 0, NULL, 0, _T("&Clear Formatting") );
	pNodeClearFormatting->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('C') ) ), false );
	pNodeQuickStyles->InsertNode(
		NULL,
		pNodeClearFormatting
		);
CExtRibbonNode * pNodeApplyStyles =
		new CExtRibbonNode( ID_APPLY_STYLES, 0, NULL, 0, _T("&Apply Styles...") );
	pNodeApplyStyles->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('A') ) ), false );
	pNodeQuickStyles->InsertNode(
		NULL,
		pNodeApplyStyles
		);

CExtRibbonNode * pNodeChangeStyles =
		new CExtRibbonNode( ID_CHANGE_STYLES_BIG, 0, NULL, 0, _T("Change\nStyles") );
	pNodeChangeStyles->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('G') ) ), false );
	pNodeChangeStyles->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeChangeStyles->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGE_STYLES_BIG) ) );
	pNodeChangeStyles->m_iconBig.m_bmpNormal.Make32();
	pNodeChangeStyles->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Change Styles") );
							pCmdScreenTip->TextMainSet( _T("Change the set of styles, colors,\nand fonts used in this document.") );
							pNodeChangeStyles->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeChangeStyles
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Home_Find()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_EDIT_FIND_BIG );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZN") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_FIND_BIG) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							CExtCustomizeCmdScreenTip * pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Find") );
							pRibbonGroup->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->SetTextInToolbar( _T("Find") );
	//pRibbonGroup->RibbonILE_SetCollapsed( __EXT_RIBBON_ILE_MAX - 6 );

CExtRibbonNode * pNodeFind =
		new CExtRibbonNode( ID_EDIT_PASTE, 0, NULL, 0, _T("Find") );
	pNodeFind->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("FD") ) ), false );
	pNodeFind->RibbonILE_RuleArrayGet().RemoveAll();
	VERIFY( pNodeFind->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_FIND_BIG) ) );
	pNodeFind->m_iconBig.m_bmpNormal.Make32();
	pNodeFind->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Find") );
							pCmdScreenTip->TextMainSet( _T("Find text in the document.") );
							pNodeFind->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeFind
		);

	pRibbonGroup->InsertNode(
		NULL,
		new CExtRibbonNode // separator
		);

CExtRibbonNode * pNodeReplace =
		new CExtRibbonNode( ID_EDIT_REPLACE_SMALL, 0, NULL, 0, _T("Replace") );
	pNodeReplace->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('R') ) ), false );
	pNodeReplace->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeReplace->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_REPLACE_SMALL) ) );
	pNodeReplace->m_iconSmall.m_bmpNormal.Make32();
	pNodeReplace->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Replace") );
							pCmdScreenTip->TextMainSet( _T("Replace text in the document") );
							pNodeReplace->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeReplace
		);

CExtRibbonNode * pNodeGoTo =
		new CExtRibbonNode( ID_EDIT_GOTO_SMALL, 0, NULL, 0, _T("Go To") );
	pNodeGoTo->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('T') ) ), false );
	pNodeGoTo->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeGoTo->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_GOTO_SMALL) ) );
	pNodeGoTo->m_iconSmall.m_bmpNormal.Make32();
	pNodeGoTo->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->TextMainSet( _T("Go To ...") );
							pNodeGoTo->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeGoTo
		);

CExtRibbonNode * pNodeSelect =
		new CExtRibbonNode( ID_EDIT_SELECT_SMALL, 0, NULL, 0, _T("Select") );
	pNodeSelect->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("SL") ) ), false );
	pNodeSelect->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeSelect->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_EDIT_SELECT_SMALL) ) );
	pNodeSelect->m_iconSmall.m_bmpNormal.Make32();
	pNodeSelect->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
							pCmdScreenTip = new CExtCustomizeCmdScreenTip;
							pCmdScreenTip->CaptionMainSet( _T("Select") );
							pCmdScreenTip->TextMainSet( _T("Select text or objects in the\ndocument.\n\nUse Select Object to allow you to\nselect objects that have been\npositioned behind the text.") );
							pNodeSelect->CmdScreenTipAttach( pCmdScreenTip );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSelect
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Insert_Pages()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_INSERT_PAGES );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZP") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COVER_PAGE_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COVER_PAGE_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Pages") );
	//pRibbonGroup->RibbonILE_SetCollapsed( __EXT_RIBBON_ILE_MAX - 6 );

CExtRibbonNode * pNodeCoverPage =
		new CExtRibbonNode( ID_COVER_PAGE_LARGE, 0, NULL, 0, _T("Cover\nPage") );
	pNodeCoverPage->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('V') ) ), false );
	pNodeCoverPage->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeCoverPage->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COVER_PAGE_LARGE) ) );
	pNodeCoverPage->m_iconBig.m_bmpNormal.Make32();
	pNodeCoverPage->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeCoverPage->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COVER_PAGE_SMALL) ) );
	pNodeCoverPage->m_iconSmall.m_bmpNormal.Make32();
	pNodeCoverPage->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeCoverPage
		);

CExtRibbonNode * pNodeBlankPage =
		new CExtRibbonNode( ID_BLANK_PAGE_LARGE, 0, NULL, 0, _T("Blank\nPage") );
	pNodeBlankPage->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("NP") ) ), false );
	pNodeBlankPage->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeBlankPage->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_BLANK_PAGE_LARGE) ) );
	pNodeBlankPage->m_iconBig.m_bmpNormal.Make32();
	pNodeBlankPage->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeBlankPage->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_BLANK_PAGE_SMALL) ) );
	pNodeBlankPage->m_iconSmall.m_bmpNormal.Make32();
	pNodeBlankPage->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeBlankPage
		);

CExtRibbonNode * pNodePageBreak =
		new CExtRibbonNode( ID_PAGE_BREAK_LARGE, 0, NULL, 0, _T("Page\nBreak") );
	pNodePageBreak->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('B') ) ), false );
	pNodePageBreak->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodePageBreak->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PAGE_BREAK_LARGE) ) );
	pNodePageBreak->m_iconBig.m_bmpNormal.Make32();
	pNodePageBreak->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodePageBreak->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PAGE_BREAK_SMALL) ) );
	pNodePageBreak->m_iconSmall.m_bmpNormal.Make32();
	pNodePageBreak->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePageBreak
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Insert_Table()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_INSERT_TABLE );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZA") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_TABLE_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Table") );
	//pRibbonGroup->RibbonILE_SetCollapsed( __EXT_RIBBON_ILE_MAX - 6 );
	pRibbonGroup->RibbonILE_SetCollapsedNever();

CExtRibbonNode * pNodeTable =
		new CExtRibbonNode( ID_INSERT_TABLE_LARGE, 0, NULL, 0, _T("Table") );
	pNodeTable->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('T') ) ), false );
	pNodeTable->RibbonILE_RuleRemoveSmallILV();
	pNodeTable->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeTable->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_TABLE_LARGE) ) );
	pNodeTable->m_iconBig.m_bmpNormal.Make32();
	pNodeTable->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeTable
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Insert_Illustrations()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_INSERT_ILLUSTRATIONS );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZI") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_PICTURE_FROM_FILE_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_PICTURE_FROM_FILE_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Illustrations") );

CExtRibbonNode * pNodePictureFromFile =
		new CExtRibbonNode( ID_INSERT_PICTURE_FROM_FILE_LARGE, 0, NULL, 0, _T("Picture\nfrom File") );
	pNodePictureFromFile->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('P') ) ), false );
	pNodePictureFromFile->RibbonILE_RuleRemoveSmallILV();
	pNodePictureFromFile->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodePictureFromFile->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_PICTURE_FROM_FILE_LARGE) ) );
	pNodePictureFromFile->m_iconBig.m_bmpNormal.Make32();
	pNodePictureFromFile->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodePictureFromFile->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_PICTURE_FROM_FILE_SMALL) ) );
	pNodePictureFromFile->m_iconSmall.m_bmpNormal.Make32();
	pNodePictureFromFile->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePictureFromFile
		);

CExtRibbonNode * pNodeIgxGraphic =
		new CExtRibbonNode( ID_INSERT_IGX_GRAPHIC_LARGE, 0, NULL, 0, _T("IGX\nGraphic") );
	pNodeIgxGraphic->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('M') ) ), false );
	pNodeIgxGraphic->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeIgxGraphic->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_IGX_GRAPHIC_LARGE) ) );
	pNodeIgxGraphic->m_iconBig.m_bmpNormal.Make32();
	pNodeIgxGraphic->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeIgxGraphic->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_IGX_GRAPHIC_SMALL) ) );
	pNodeIgxGraphic->m_iconSmall.m_bmpNormal.Make32();
	pNodeIgxGraphic->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeIgxGraphic
		);

CExtRibbonNode * pNodeChart =
		new CExtRibbonNode( ID_INSERT_CHART_LARGE, 0, NULL, 0, _T("Chart") );
	pNodeChart->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('C') ) ), false );
	pNodeChart->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeChart->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_CHART_LARGE) ) );
	pNodeChart->m_iconBig.m_bmpNormal.Make32();
	pNodeChart->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeChart->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_CHART_SMALL) ) );
	pNodeChart->m_iconSmall.m_bmpNormal.Make32();
	pNodeChart->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeChart
		);

CExtRibbonNode * pNodeShapes =
		new CExtRibbonNode( ID_INSERT_SHAPES_LARGE, 0, NULL, 0, _T("Shapes") );
	pNodeShapes->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("SH") ) ), false );
	pNodeShapes->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeShapes->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_SHAPES_LARGE) ) );
	pNodeShapes->m_iconBig.m_bmpNormal.Make32();
	pNodeShapes->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeShapes->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_SHAPES_SMALL) ) );
	pNodeShapes->m_iconSmall.m_bmpNormal.Make32();
	pNodeShapes->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeShapes
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Insert_Links()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_INSERT_LINKS );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZL") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_HYPERLINK_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_HYPERLINK_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Links") );

CExtRibbonNode * pNodeHyperlink =
		new CExtRibbonNode( ID_INSERT_HYPERLINK_LARGE, 0, NULL, 0, _T("Hyperlink") );
	pNodeHyperlink->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('I') ) ), false );
	pNodeHyperlink->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeHyperlink->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_HYPERLINK_LARGE) ) );
	pNodeHyperlink->m_iconBig.m_bmpNormal.Make32();
	pNodeHyperlink->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeHyperlink->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_HYPERLINK_SMALL) ) );
	pNodeHyperlink->m_iconSmall.m_bmpNormal.Make32();
	pNodeHyperlink->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeHyperlink
		);

CExtRibbonNode * pNodeBookmark =
		new CExtRibbonNode( ID_INSERT_BOOKMARK_LARGE, 0, NULL, 0, _T("Bookmark") );
	pNodeBookmark->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('K') ) ), false );
	pNodeBookmark->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeBookmark->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_BOOKMARK_LARGE) ) );
	pNodeBookmark->m_iconBig.m_bmpNormal.Make32();
	pNodeBookmark->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeBookmark->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_BOOKMARK_SMALL) ) );
	pNodeBookmark->m_iconSmall.m_bmpNormal.Make32();
	pNodeBookmark->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeBookmark
		);

CExtRibbonNode * pNodeCrossReference =
		new CExtRibbonNode( ID_INSERT_CROSS_REFERENCE_LARGE, 0, NULL, 0, _T("Cross-reference") );
	pNodeCrossReference->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("RF") ) ), false );
	pNodeCrossReference->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeCrossReference->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_CROSS_REFERENCE_LARGE) ) );
	pNodeCrossReference->m_iconBig.m_bmpNormal.Make32();
	pNodeCrossReference->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeCrossReference->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_CROSS_REFERENCE_SMALL) ) );
	pNodeCrossReference->m_iconSmall.m_bmpNormal.Make32();
	pNodeCrossReference->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeCrossReference
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Insert_HeaderAndFooter()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_INSERT_HEADER_AND_FOOTER );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZH") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_HEADER_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_HEADER_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Header &&\nFooter") );

CExtRibbonNode * pNodeHeader =
		new CExtRibbonNode( ID_INSERT_HEADER_LARGE, 0, NULL, 0, _T("Header") );
	pNodeHeader->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
	pNodeHeader->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeHeader->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_HEADER_LARGE) ) );
	pNodeHeader->m_iconBig.m_bmpNormal.Make32();
	pNodeHeader->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeHeader->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_HEADER_SMALL) ) );
	pNodeHeader->m_iconSmall.m_bmpNormal.Make32();
	pNodeHeader->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeHeader
		);

CExtRibbonNode * pNodeFooter =
		new CExtRibbonNode( ID_INSERT_FOOTER_LARGE, 0, NULL, 0, _T("Footer") );
	pNodeFooter->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('O') ) ), false );
	pNodeFooter->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeFooter->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_FOOTER_LARGE) ) );
	pNodeFooter->m_iconBig.m_bmpNormal.Make32();
	pNodeFooter->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeFooter->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_FOOTER_SMALL) ) );
	pNodeFooter->m_iconSmall.m_bmpNormal.Make32();
	pNodeFooter->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeFooter
		);

CExtRibbonNode * pNodePageNumber =
		new CExtRibbonNode( ID_INSERT_PAGE_NUMBER_LARGE, 0, NULL, 0, _T("Page\nNumber") );
	pNodePageNumber->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("NU") ) ), false );
	pNodePageNumber->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodePageNumber->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_PAGE_NUMBER_LARGE) ) );
	pNodePageNumber->m_iconBig.m_bmpNormal.Make32();
	pNodePageNumber->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodePageNumber->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_PAGE_NUMBER_SMALL) ) );
	pNodePageNumber->m_iconSmall.m_bmpNormal.Make32();
	pNodePageNumber->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePageNumber
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Insert_Text()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_INSERT_TEXT );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZT") ) ), false );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_WORD_ART_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Text") );

CExtRibbonNode * pNodeTextBox =
		new CExtRibbonNode( ID_INSERT_TEXT_BOX_LARGE, 0, NULL, 0, _T("Text\nBox") );
	pNodeTextBox->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('X') ) ), false );
	pNodeTextBox->RibbonILE_RuleRemoveSmallILV();
	pNodeTextBox->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeTextBox->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_TEXT_BOX_LARGE) ) );
	pNodeTextBox->m_iconBig.m_bmpNormal.Make32();
	pNodeTextBox->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeTextBox
		);

CExtRibbonNode * pNodeQuickParts =
		new CExtRibbonNode( ID_INSERT_QUICK_PARTS_LARGE, 0, NULL, 0, _T("Quick\nParts") );
	pNodeQuickParts->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('Q') ) ), false );
	pNodeQuickParts->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeQuickParts->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_QUICK_PARTS_LARGE) ) );
	pNodeQuickParts->m_iconBig.m_bmpNormal.Make32();
	pNodeQuickParts->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeQuickParts->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_QUICK_PARTS_SMALL) ) );
	pNodeQuickParts->m_iconSmall.m_bmpNormal.Make32();
	pNodeQuickParts->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeQuickParts
		);

CExtRibbonNode * pNodeWordArt =
		new CExtRibbonNode( ID_INSERT_WORD_ART_LARGE, 0, NULL, 0, _T("WordArt") );
	pNodeWordArt->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('W') ) ), false );
	pNodeWordArt->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeWordArt->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_WORD_ART_LARGE) ) );
	pNodeWordArt->m_iconBig.m_bmpNormal.Make32();
	pNodeWordArt->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeWordArt->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_WORD_ART_SMALL) ) );
	pNodeWordArt->m_iconSmall.m_bmpNormal.Make32();
	pNodeWordArt->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeWordArt
		);

CExtRibbonNode * pNodeDropCap =
		new CExtRibbonNode( ID_INSERT_DROP_CAP_LARGE, 0, NULL, 0, _T("Drop Cap") );
	pNodeDropCap->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("RC") ) ), false );
	pNodeDropCap->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeDropCap->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_DROP_CAP_LARGE) ) );
	pNodeDropCap->m_iconBig.m_bmpNormal.Make32();
	pNodeDropCap->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeDropCap->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_DROP_CAP_SMALL) ) );
	pNodeDropCap->m_iconSmall.m_bmpNormal.Make32();
	pNodeDropCap->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeDropCap
		);

CExtRibbonNode * pNodeSignatureLine =
		new CExtRibbonNode( ID_INSERT_SIGNATURE_LINE_SMALL, 0, NULL, 0, _T("Signature Line") );
	pNodeSignatureLine->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('G') ) ), false );
	pNodeSignatureLine->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeSignatureLine->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_SIGNATURE_LINE_SMALL) ) );
	pNodeSignatureLine->m_iconSmall.m_bmpNormal.Make32();
	pNodeSignatureLine->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSignatureLine
		);

CExtRibbonNode * pNodeDateAndTime =
		new CExtRibbonNode( ID_INSERT_DATE_AND_TIME_SMALL, 0, NULL, 0, _T("Date && Time") );
	pNodeDateAndTime->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('D') ) ), false );
	pNodeDateAndTime->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeDateAndTime->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_DATE_AND_TIME_SMALL) ) );
	pNodeDateAndTime->m_iconSmall.m_bmpNormal.Make32();
	pNodeDateAndTime->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeDateAndTime
		);

CExtRibbonNode * pNodeObject =
		new CExtRibbonNode( ID_INSERT_OBJECT_SMALL, 0, NULL, 0, _T("Insert Object") );
	pNodeObject->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('J') ) ), false );
	pNodeObject->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeObject->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_OBJECT_SMALL) ) );
	pNodeObject->m_iconSmall.m_bmpNormal.Make32();
	pNodeObject->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeObject
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Insert_Symbols()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_INSERT_SYMBOLS );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZS") ) ), false );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_RED_DOT) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Symbols") );

CExtRibbonNode * pNodeEquation =
		new CExtRibbonNode( ID_INSERT_DROP_CAP_LARGE, 0, NULL, 0, _T("Equation") );
	pNodeEquation->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('E') ) ), false );
	pNodeEquation->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeEquation->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_EQUATION_LARGE) ) );
	pNodeEquation->m_iconBig.m_bmpNormal.Make32();
	pNodeEquation->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeEquation->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_EQUATION_SMALL) ) );
	pNodeEquation->m_iconSmall.m_bmpNormal.Make32();
	pNodeEquation->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeEquation
		);

CExtRibbonNode * pNodeSymbol =
		new CExtRibbonNode( ID_INSERT_DROP_CAP_LARGE, 0, NULL, 0, _T("Symbol") );
	pNodeSymbol->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('U') ) ), false );
	pNodeSymbol->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeSymbol->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_SYMBOL_LARGE) ) );
	pNodeSymbol->m_iconBig.m_bmpNormal.Make32();
	pNodeSymbol->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeSymbol->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_SYMBOL_SMALL) ) );
	pNodeSymbol->m_iconSmall.m_bmpNormal.Make32();
	pNodeSymbol->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSymbol
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_PageLayout_Themes()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_PL_THEMES );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZT") ) ), false );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_THEME_COLORS_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Themes") );

CExtRibbonNode * pNodeThemes =
		new CExtRibbonNode( ID_THEMES_LARGE, 0, NULL, 0, _T("Themes") );
	pNodeThemes->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("TH") ) ), false );
	pNodeThemes->RibbonILE_RuleRemoveSmallILV();
	pNodeThemes->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeThemes->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_THEMES_LARGE) ) );
	pNodeThemes->m_iconBig.m_bmpNormal.Make32();
	pNodeThemes->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeThemes
		);

CExtRibbonNode * pNodeColors =
		new CExtRibbonNode( ID_THEME_COLORS_SMALL, 0, NULL, 0, _T("Colors") );
	pNodeColors->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("TC") ) ), false );
	pNodeColors->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeColors->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_THEME_COLORS_SMALL) ) );
	pNodeColors->m_iconSmall.m_bmpNormal.Make32();
	pNodeColors->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeColors
		);

CExtRibbonNode * pNodeFonts =
		new CExtRibbonNode( ID_THEME_FONTS_SMALL, 0, NULL, 0, _T("Fonts") );
	pNodeFonts->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("TF") ) ), false );
	pNodeFonts->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeFonts->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_THEME_FONTS_SMALL) ) );
	pNodeFonts->m_iconSmall.m_bmpNormal.Make32();
	pNodeFonts->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeFonts
		);

CExtRibbonNode * pNodeEffects =
		new CExtRibbonNode( ID_THEME_EFFECTS_SMALL, 0, NULL, 0, _T("Effects") );
	pNodeEffects->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("TE") ) ), false );
	pNodeEffects->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeEffects->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_THEME_EFFECTS_SMALL) ) );
	pNodeEffects->m_iconSmall.m_bmpNormal.Make32();
	pNodeEffects->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeEffects
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_PageLayout_PageSetup()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_PL_PAGE_SETUP );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZS") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_SIZE_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_SIZE_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Page\nSetup") );

CExtRibbonNodeDialogLauncher * pRibbonDLB =
		new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_PL_PAGE_SETUP );
	pRibbonDLB->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("SP") ) ), false );
	pRibbonGroup->InsertNode( NULL, pRibbonDLB );

CExtRibbonNode * pNodeMarginsBox =
		new CExtRibbonNode( ID_MARGINS_LARGE, 0, NULL, 0, _T("Margins") );
	pNodeMarginsBox->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('M') ) ), false );
	pNodeMarginsBox->RibbonILE_RuleRemoveSmallILV();
	pNodeMarginsBox->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeMarginsBox->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MARGINS_LARGE) ) );
	pNodeMarginsBox->m_iconBig.m_bmpNormal.Make32();
	pNodeMarginsBox->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeMarginsBox
		);

CExtRibbonNode * pNodeOrientation =
		new CExtRibbonNode( ID_ORIENTATION_LARGE, 0, NULL, 0, _T("Orientation") );
	pNodeOrientation->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('O') ) ), false );
	pNodeOrientation->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeOrientation->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ORIENTATION_LARGE) ) );
	pNodeOrientation->m_iconBig.m_bmpNormal.Make32();
	pNodeOrientation->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeOrientation->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ORIENTATION_SMALL) ) );
	pNodeOrientation->m_iconSmall.m_bmpNormal.Make32();
	pNodeOrientation->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeOrientation
		);

CExtRibbonNode * pNodeSize =
		new CExtRibbonNode( ID_SIZE_LARGE, 0, NULL, 0, _T("Size") );
	pNodeSize->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("SZ") ) ), false );
	pNodeSize->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeSize->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_SIZE_LARGE) ) );
	pNodeSize->m_iconBig.m_bmpNormal.Make32();
	pNodeSize->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeSize->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_SIZE_SMALL) ) );
	pNodeSize->m_iconSmall.m_bmpNormal.Make32();
	pNodeSize->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSize
		);

CExtRibbonNode * pNodeColumns =
		new CExtRibbonNode( ID_COLUMNS_LARGE, 0, NULL, 0, _T("Columns") );
	pNodeColumns->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("J") ) ), false );
	pNodeColumns->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeColumns->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COLUMNS_LARGE) ) );
	pNodeColumns->m_iconBig.m_bmpNormal.Make32();
	pNodeColumns->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeColumns->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COLUMNS_SMALL) ) );
	pNodeColumns->m_iconSmall.m_bmpNormal.Make32();
	pNodeColumns->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeColumns
		);

CExtRibbonNode * pNodeBreaks =
		new CExtRibbonNode( ID_PAGE_BREAKS_SMALL, 0, NULL, 0, _T("Breaks") );
	pNodeBreaks->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('B') ) ), false );
	pNodeBreaks->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeBreaks->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PAGE_BREAKS_SMALL) ) );
	pNodeBreaks->m_iconSmall.m_bmpNormal.Make32();
	pNodeBreaks->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeBreaks
		);

CExtRibbonNode * pNodeLineNumbers =
		new CExtRibbonNode( ID_PAGE_LINE_NUMBERS_SMALL, 0, NULL, 0, _T("Line Numbers") );
	pNodeLineNumbers->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("LN") ) ), false );
	pNodeLineNumbers->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeLineNumbers->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PAGE_LINE_NUMBERS_SMALL) ) );
	pNodeLineNumbers->m_iconSmall.m_bmpNormal.Make32();
	pNodeLineNumbers->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeLineNumbers
		);

CExtRibbonNode * pNodeHyphenation =
		new CExtRibbonNode( ID_PAGE_HYPHENATION_SMALL, 0, NULL, 0, _T("Hyphenation") );
	pNodeHyphenation->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
	pNodeHyphenation->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeHyphenation->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PAGE_HYPHENATION_SMALL) ) );
	pNodeHyphenation->m_iconSmall.m_bmpNormal.Make32();
	pNodeHyphenation->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeHyphenation
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_PageLayout_PageBackground()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_PL_PAGE_BACKGROUND );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZB") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PGBK_BORDERS_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PGBK_BORDERS_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Page\nBackground") );

CExtRibbonNode * pNodeWatermark =
		new CExtRibbonNode( ID_PGBK_WATERMARK_LARGE, 0, NULL, 0, _T("Watermark") );
	pNodeWatermark->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("PW") ) ), false );
	pNodeWatermark->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeWatermark->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PGBK_WATERMARK_LARGE) ) );
	pNodeWatermark->m_iconBig.m_bmpNormal.Make32();
	pNodeWatermark->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeWatermark->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PGBK_WATERMARK_SMALL) ) );
	pNodeWatermark->m_iconSmall.m_bmpNormal.Make32();
	pNodeWatermark->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeWatermark
		);

CExtRibbonNode * pNodePageColor =
		new CExtRibbonNode( ID_PGBK_COLOR_LARGE, 0, NULL, 0, _T("Page\nColor") );
	pNodePageColor->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("PC") ) ), false );
	pNodePageColor->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodePageColor->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PGBK_COLOR_LARGE) ) );
	pNodePageColor->m_iconBig.m_bmpNormal.Make32();
	pNodePageColor->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodePageColor->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PGBK_COLOR_SMALL) ) );
	pNodePageColor->m_iconSmall.m_bmpNormal.Make32();
	pNodePageColor->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePageColor
		);

CExtRibbonNode * pNodePageBorders =
		new CExtRibbonNode( ID_PGBK_BORDERS_LARGE, 0, NULL, 0, _T("Page\nBorders") );
	pNodePageBorders->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("PB") ) ), false );
	pNodePageBorders->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodePageBorders->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PGBK_BORDERS_LARGE) ) );
	pNodePageBorders->m_iconBig.m_bmpNormal.Make32();
	pNodePageBorders->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodePageBorders->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PGBK_BORDERS_SMALL) ) );
	pNodePageBorders->m_iconSmall.m_bmpNormal.Make32();
	pNodePageBorders->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePageBorders
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_PageLayout_Paragraph()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_PL_PARAGRAPH );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZG") ) ), false );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARA_INDENT1_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Paragraph") );

CExtRibbonNodeDialogLauncher * pRibbonDLB =
		new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_PL_PARAGRAPH );
	pRibbonDLB->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("PG") ) ), false );
	pRibbonGroup->InsertNode( NULL, pRibbonDLB );

CExtRibbonNodeLabel * pNodeIndentLabel =
		new CExtRibbonNodeLabel( 123, NULL, 0, _T("Indent") );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeIndentLabel
		);
CExtRibbonNode * pNodeIndent1 =
		new CExtRibbonNode( ID_PARA_INDENT1_SMALL, 0, NULL, 0, _T("Left") );
	pNodeIndent1->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("IL") ) ), false );
	pNodeIndent1->RibbonILE_RuleRemoveSmallILV();
	pNodeIndent1->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeIndent1->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARA_INDENT1_SMALL) ) );
	pNodeIndent1->m_iconSmall.m_bmpNormal.Make32();
	pNodeIndent1->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeIndent1
		);
CExtRibbonNode * pNodeIndent2 =
		new CExtRibbonNode( ID_PARA_INDENT2_SMALL, 0, NULL, 0, _T("Right") );
	pNodeIndent2->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("IR") ) ), false );
	pNodeIndent2->RibbonILE_RuleRemoveSmallILV();
	pNodeIndent2->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeIndent2->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARA_INDENT2_SMALL) ) );
	pNodeIndent2->m_iconSmall.m_bmpNormal.Make32();
	pNodeIndent2->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeIndent2
		);

	pRibbonGroup->InsertNode(
		NULL,
		new CExtRibbonNode // separator
		);

CExtRibbonNodeLabel * pNodeSpacingLabel =
		new CExtRibbonNodeLabel( 123, NULL, 0, _T("Spacing") );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSpacingLabel
		);
CExtRibbonNode * pNodeSpacing1 =
		new CExtRibbonNode( ID_PARA_SPACING1_SMALL, 0, NULL, 0, _T("Before") );
	pNodeSpacing1->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("SB") ) ), false );
	pNodeSpacing1->RibbonILE_RuleRemoveSmallILV();
	pNodeSpacing1->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeSpacing1->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARA_SPACING1_SMALL) ) );
	pNodeSpacing1->m_iconSmall.m_bmpNormal.Make32();
	pNodeSpacing1->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSpacing1
		);
CExtRibbonNode * pNodeSpacing2 =
		new CExtRibbonNode( ID_PARA_SPACING2_SMALL, 0, NULL, 0, _T("After") );
	pNodeSpacing2->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("SA") ) ), false );
	pNodeSpacing2->RibbonILE_RuleRemoveSmallILV();
	pNodeSpacing2->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeSpacing2->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PARA_SPACING2_SMALL) ) );
	pNodeSpacing2->m_iconSmall.m_bmpNormal.Make32();
	pNodeSpacing2->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSpacing2
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_PageLayout_Arrange()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_PL_ARRANGE );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZA") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_BTF_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_BTF_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Arrange") );

CExtRibbonNodeDialogLauncher * pRibbonDLB =
		new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_PL_ARRANGE );
	pRibbonDLB->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("PA") ) ), false );
	pRibbonGroup->InsertNode( NULL, pRibbonDLB );

CExtRibbonNode * pNodePositionBox =
		new CExtRibbonNode( ID_ARRANGE_POSITION_LARGE, 0, NULL, 0, _T("Position") );
	pNodePositionBox->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("PO") ) ), false );
	pNodePositionBox->RibbonILE_RuleRemoveSmallILV();
	pNodePositionBox->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodePositionBox->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_POSITION_LARGE) ) );
	pNodePositionBox->m_iconBig.m_bmpNormal.Make32();
	pNodePositionBox->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePositionBox
		);

CExtRibbonNode * pNodeBringToFront =
		new CExtRibbonNode( ID_ARRANGE_BTF_LARGE, 0, NULL, 0, _T("Bring to\nFront") );
	pNodeBringToFront->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AF") ) ), false );
	pNodeBringToFront->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeBringToFront->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_BTF_LARGE) ) );
	pNodeBringToFront->m_iconBig.m_bmpNormal.Make32();
	pNodeBringToFront->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeBringToFront->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_BTF_SMALL) ) );
	pNodeBringToFront->m_iconSmall.m_bmpNormal.Make32();
	pNodeBringToFront->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeBringToFront
		);

CExtRibbonNode * pNodeSendToBack =
		new CExtRibbonNode( ID_ARRANGE_STB_LARGE, 0, NULL, 0, _T("Send to\nBack") );
	pNodeSendToBack->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AE") ) ), false );
	pNodeSendToBack->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeSendToBack->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_STB_LARGE) ) );
	pNodeSendToBack->m_iconBig.m_bmpNormal.Make32();
	pNodeSendToBack->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeSendToBack->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_STB_SMALL) ) );
	pNodeSendToBack->m_iconSmall.m_bmpNormal.Make32();
	pNodeSendToBack->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSendToBack
		);

CExtRibbonNode * pNodeTextWrapping =
		new CExtRibbonNode( ID_ARRANGE_TW_LARGE, 0, NULL, 0, _T("Text\nWrapping") );
	pNodeTextWrapping->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("TW") ) ), false );
	pNodeTextWrapping->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeTextWrapping->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_TW_LARGE) ) );
	pNodeTextWrapping->m_iconBig.m_bmpNormal.Make32();
	pNodeTextWrapping->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeTextWrapping->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_TW_SMALL) ) );
	pNodeTextWrapping->m_iconSmall.m_bmpNormal.Make32();
	pNodeTextWrapping->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeTextWrapping
		);

CExtRibbonNode * pNodeAlign =
		new CExtRibbonNode( ID_ARRANGE_ALIGN_LARGE, 0, NULL, 0, _T("Align") );
	pNodeAlign->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AA") ) ), false );
	VERIFY( pNodeAlign->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_ALIGN_LARGE) ) );
	pNodeAlign->m_iconBig.m_bmpNormal.Make32();
	pNodeAlign->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeAlign->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_ALIGN_SMALL) ) );
	pNodeAlign->m_iconSmall.m_bmpNormal.Make32();
	pNodeAlign->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeAlign
		);

CExtRibbonNode * pNodeGroup =
		new CExtRibbonNode( ID_ARRANGE_GROUP_LARGE, 0, NULL, 0, _T("Group") );
	VERIFY( pNodeGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_GROUP_LARGE) ) );
	pNodeGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AG") ) ), false );
	pNodeGroup->m_iconBig.m_bmpNormal.Make32();
	pNodeGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_GROUP_SMALL) ) );
	pNodeGroup->m_iconSmall.m_bmpNormal.Make32();
	pNodeGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeGroup
		);

CExtRibbonNode * pNodeRotate =
		new CExtRibbonNode( ID_ARRANGE_ROTATE_LARGE, 0, NULL, 0, _T("Rotate") );
	pNodeRotate->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("AY") ) ), false );
	VERIFY( pNodeRotate->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_ROTATE_LARGE) ) );
	pNodeRotate->m_iconBig.m_bmpNormal.Make32();
	pNodeRotate->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeRotate->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ARRANGE_ROTATE_SMALL) ) );
	pNodeRotate->m_iconSmall.m_bmpNormal.Make32();
	pNodeRotate->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeRotate
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_References_TableOfContents()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REF_TOC );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZC") ) ), false );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_RED_DOT) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Table of\nContents") );

CExtRibbonNode * pNodeTableOfContents =
		new CExtRibbonNode( ID_TOC_TABLE_LARGE, 0, NULL, 0, _T("Table of\nContents") );
	pNodeTableOfContents->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('T') ) ), false );
	pNodeTableOfContents->RibbonILE_RuleRemoveSmallILV();
	pNodeTableOfContents->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeTableOfContents->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TOC_TABLE_LARGE) ) );
	pNodeTableOfContents->m_iconBig.m_bmpNormal.Make32();
	pNodeTableOfContents->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeTableOfContents
		);

CExtRibbonNode * pNodeAddText =
		new CExtRibbonNode( ID_TOC_ADD_TEXT_SMALL, 0, NULL, 0, _T("Add Text") );
	pNodeAddText->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('A') ) ), false );
	pNodeAddText->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeAddText->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TOC_ADD_TEXT_SMALL) ) );
	pNodeAddText->m_iconSmall.m_bmpNormal.Make32();
	pNodeAddText->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeAddText
		);

CExtRibbonNode * pNodeUpdateTable =
		new CExtRibbonNode( ID_TOC_UPDATE_TABLE_SMALL, 0, NULL, 0, _T("Update Table") );
	pNodeUpdateTable->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('U') ) ), false );
	pNodeUpdateTable->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeUpdateTable->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TOC_UPDATE_TABLE_SMALL) ) );
	pNodeUpdateTable->m_iconSmall.m_bmpNormal.Make32();
	pNodeUpdateTable->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeUpdateTable
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_References_FootNotes()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REF_FOOTNOTES );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZF") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FN_INSERT_FOOTNOTE_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FN_INSERT_FOOTNOTE_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Footnotes") );

CExtRibbonNodeDialogLauncher * pRibbonDLB =
		new CExtRibbonNodeDialogLauncher( ID_RIBBON_PAGE_REF_FOOTNOTES );
	pRibbonDLB->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('Q') ) ), false );
	pRibbonGroup->InsertNode( NULL, pRibbonDLB );

CExtRibbonNode * pNodeInsertFootnote =
		new CExtRibbonNode( ID_FN_INSERT_FOOTNOTE_LARGE, 0, NULL, 0, _T("Insert\nFootnote") );
	pNodeInsertFootnote->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('F') ) ), false );
	pNodeInsertFootnote->RibbonILE_RuleRemoveSmallILV();
	pNodeInsertFootnote->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeInsertFootnote->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FN_INSERT_FOOTNOTE_LARGE) ) );
	pNodeInsertFootnote->m_iconBig.m_bmpNormal.Make32();
	pNodeInsertFootnote->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeInsertFootnote->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FN_INSERT_FOOTNOTE_SMALL) ) );
	pNodeInsertFootnote->m_iconSmall.m_bmpNormal.Make32();
	pNodeInsertFootnote->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeInsertFootnote
		);

CExtRibbonNode * pNodeInsertEndnote =
		new CExtRibbonNode( ID_FN_INSERT_ENDNOTE_SMALL, 0, NULL, 0, _T("Insert Endnote") );
	pNodeInsertEndnote->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('E') ) ), false );
	pNodeInsertEndnote->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeInsertEndnote->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FN_INSERT_ENDNOTE_SMALL) ) );
	pNodeInsertEndnote->m_iconSmall.m_bmpNormal.Make32();
	pNodeInsertEndnote->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeInsertEndnote
		);

CExtRibbonNode * pNodeNextFootnote =
		new CExtRibbonNode( ID_FN_NEXT_FOOTNOTE_SMALL, 0, NULL, 0, _T("Next Footnote") );
	pNodeNextFootnote->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('O') ) ), false );
	pNodeNextFootnote->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeNextFootnote->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FN_NEXT_FOOTNOTE_SMALL) ) );
	pNodeNextFootnote->m_iconSmall.m_bmpNormal.Make32();
	pNodeNextFootnote->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeNextFootnote
		);

CExtRibbonNode * pNodeShowNotes =
		new CExtRibbonNode( ID_FN_SHOW_NOTES_SMALL, 0, NULL, 0, _T("Show Notes") );
	pNodeShowNotes->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
	pNodeShowNotes->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeShowNotes->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_FN_SHOW_NOTES_SMALL) ) );
	pNodeShowNotes->m_iconSmall.m_bmpNormal.Make32();
	pNodeShowNotes->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeShowNotes
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_References_CitationsAndBibliography()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REF_CB );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZB") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CB_INSTERT_CITATION_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CB_INSTERT_CITATION_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Citations &&\nBibliography") );

CExtRibbonNode * pNodeInsertCitation =
		new CExtRibbonNode( ID_CB_INSTERT_CITATION_LARGE, 0, NULL, 0, _T("Insert\nCitation") );
	pNodeInsertCitation->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('C') ) ), false );
	pNodeInsertCitation->RibbonILE_RuleRemoveSmallILV();
	pNodeInsertCitation->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeInsertCitation->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CB_INSTERT_CITATION_LARGE) ) );
	pNodeInsertCitation->m_iconBig.m_bmpNormal.Make32();
	pNodeInsertCitation->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeInsertCitation->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CB_INSTERT_CITATION_SMALL) ) );
	pNodeInsertCitation->m_iconSmall.m_bmpNormal.Make32();
	pNodeInsertCitation->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeInsertCitation
		);

CExtRibbonNode * pNodeManageSources =
		new CExtRibbonNode( ID_CB_MANAGE_SOURCES_SMALL, 0, NULL, 0, _T("Manage Sources") );
	pNodeManageSources->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('M') ) ), false );
	pNodeManageSources->RibbonILE_RuleRemoveSmallILV();
	pNodeManageSources->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeManageSources->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CB_MANAGE_SOURCES_SMALL) ) );
	pNodeManageSources->m_iconSmall.m_bmpNormal.Make32();
	pNodeManageSources->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeManageSources
		);

CExtRibbonNode * pNodeStyle =
		new CExtRibbonNode( ID_CB_STYLE_SMALL, 0, NULL, 0, _T("Style") );
	pNodeStyle->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('L') ) ), false );
	pNodeStyle->RibbonILE_RuleRemoveSmallILV();
	pNodeStyle->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeStyle->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CB_STYLE_SMALL) ) );
	pNodeStyle->m_iconSmall.m_bmpNormal.Make32();
	pNodeStyle->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeStyle
		);

CExtRibbonNode * pNodeInsertBibliography =
		new CExtRibbonNode( ID_CB_INSERT_BIBLIOGRAPHY_SMALL, 0, NULL, 0, _T("Insert Bibliography") );
	pNodeInsertBibliography->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('B') ) ), false );
	pNodeInsertBibliography->RibbonILE_RuleRemoveSmallILV();
	pNodeInsertBibliography->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeInsertBibliography->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CB_INSERT_BIBLIOGRAPHY_SMALL) ) );
	pNodeInsertBibliography->m_iconSmall.m_bmpNormal.Make32();
	pNodeInsertBibliography->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeInsertBibliography
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_References_Captions()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REF_CAPTIONS );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZP") ) ), false );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_CAPTIONS_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Captions") );

CExtRibbonNode * pNodeInsertCaption =
		new CExtRibbonNode( ID_INSERT_CAPTION_LARGE, 0, NULL, 0, _T("Insert\nCaption") );
	pNodeInsertCaption->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('P') ) ), false );
	pNodeInsertCaption->RibbonILE_RuleRemoveSmallILV();
	pNodeInsertCaption->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeInsertCaption->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_CAPTION_LARGE) ) );
	pNodeInsertCaption->m_iconBig.m_bmpNormal.Make32();
	pNodeInsertCaption->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeInsertCaption
		);

CExtRibbonNode * pNodeInsertTableOfFigures =
		new CExtRibbonNode( ID_INSERT_TOF_SMALL, 0, NULL, 0, _T("Insert Table of Figures") );
	pNodeInsertTableOfFigures->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('G') ) ), false );
	pNodeInsertTableOfFigures->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeInsertTableOfFigures->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INSERT_TOF_SMALL) ) );
	pNodeInsertTableOfFigures->m_iconSmall.m_bmpNormal.Make32();
	pNodeInsertTableOfFigures->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeInsertTableOfFigures
		);

CExtRibbonNode * pNodeUpdateTable =
		new CExtRibbonNode( ID_CAPT_UPDATE_TABLE_SMALL, 0, NULL, 0, _T("Update Table") );
	pNodeUpdateTable->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('V') ) ), false );
	pNodeUpdateTable->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeUpdateTable->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CAPT_UPDATE_TABLE_SMALL) ) );
	pNodeUpdateTable->m_iconSmall.m_bmpNormal.Make32();
	pNodeUpdateTable->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeUpdateTable
		);

CExtRibbonNode * pNodeCrossReference =
		new CExtRibbonNode( ID_CAPT_CROSS_REF_SMALL, 0, NULL, 0, _T("Cross-reference") );
	pNodeCrossReference->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("RF") ) ), false );
	pNodeCrossReference->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeCrossReference->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CAPT_CROSS_REF_SMALL) ) );
	pNodeCrossReference->m_iconSmall.m_bmpNormal.Make32();
	pNodeCrossReference->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeCrossReference
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_References_Index()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REF_INDEX );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZI") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INDEX_MARK_ENTRY_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INDEX_MARK_ENTRY_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Index") );

CExtRibbonNode * pNodeMarkEntry =
		new CExtRibbonNode( ID_INDEX_MARK_ENTRY_LARGE, 0, NULL, 0, _T("Mark\nEntry") );
	pNodeMarkEntry->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('N') ) ), false );
	pNodeMarkEntry->RibbonILE_RuleRemoveSmallILV();
	pNodeMarkEntry->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeMarkEntry->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INDEX_MARK_ENTRY_LARGE) ) );
	pNodeMarkEntry->m_iconBig.m_bmpNormal.Make32();
	pNodeMarkEntry->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeMarkEntry->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INDEX_MARK_ENTRY_SMALL) ) );
	pNodeMarkEntry->m_iconSmall.m_bmpNormal.Make32();
	pNodeMarkEntry->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeMarkEntry
		);

CExtRibbonNode * pNodeInsertIndex =
		new CExtRibbonNode( ID_INDEX_INSERT_SMALL, 0, NULL, 0, _T("Insert Index") );
	pNodeInsertIndex->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('X') ) ), false );
	pNodeInsertIndex->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeInsertIndex->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INDEX_INSERT_SMALL) ) );
	pNodeInsertIndex->m_iconSmall.m_bmpNormal.Make32();
	pNodeInsertIndex->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeInsertIndex
		);

CExtRibbonNode * pNodeUpdateIndex =
		new CExtRibbonNode( ID_INDEX_UPDATE_SMALL, 0, NULL, 0, _T("Update Index") );
	pNodeUpdateIndex->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('D') ) ), false );
	pNodeUpdateIndex->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeUpdateIndex->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_INDEX_UPDATE_SMALL) ) );
	pNodeUpdateIndex->m_iconSmall.m_bmpNormal.Make32();
	pNodeUpdateIndex->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeUpdateIndex
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_References_TableOfAuthorities()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REF_TOA );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZA") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TA_MARK_CITATION_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TA_MARK_CITATION_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Table of\nAuthorities") );

CExtRibbonNode * pNodeMarkCitation =
		new CExtRibbonNode( ID_TA_MARK_CITATION_LARGE, 0, NULL, 0, _T("Mark\nCitation") );
	pNodeMarkCitation->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('I') ) ), false );
	pNodeMarkCitation->RibbonILE_RuleRemoveSmallILV();
	pNodeMarkCitation->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeMarkCitation->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TA_MARK_CITATION_LARGE) ) );
	pNodeMarkCitation->m_iconBig.m_bmpNormal.Make32();
	pNodeMarkCitation->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeMarkCitation->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TA_MARK_CITATION_SMALL) ) );
	pNodeMarkCitation->m_iconSmall.m_bmpNormal.Make32();
	pNodeMarkCitation->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeMarkCitation
		);

CExtRibbonNode * pNodeInsertTableOfAuthorities =
		new CExtRibbonNode( ID_TA_INSERT_SMALL, 0, NULL, 0, _T("Insert Table of\nAuthorities") );
	pNodeInsertTableOfAuthorities->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("RT") ) ), false );
	pNodeInsertTableOfAuthorities->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeInsertTableOfAuthorities->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TA_INSERT_SMALL) ) );
	pNodeInsertTableOfAuthorities->m_iconSmall.m_bmpNormal.Make32();
	pNodeInsertTableOfAuthorities->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeInsertTableOfAuthorities
		);

CExtRibbonNode * pNodeUpdateTable =
		new CExtRibbonNode( ID_TA_UPDATE_SMALL, 0, NULL, 0, _T("Update Table") );
	pNodeUpdateTable->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("RU") ) ), false );
	pNodeUpdateTable->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeUpdateTable->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TA_UPDATE_SMALL) ) );
	pNodeUpdateTable->m_iconSmall.m_bmpNormal.Make32();
	pNodeUpdateTable->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeUpdateTable
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Mailings_Create()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_MAIL_CREATE );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZC") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_CREATE_ENVELOPES_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_CREATE_ENVELOPES_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Create") );

CExtRibbonNode * pNodeEnvelopes =
		new CExtRibbonNode( ID_MAILINGS_CREATE_ENVELOPES_LARGE, 0, NULL, 0, _T("Envelopes") );
	pNodeEnvelopes->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('E') ) ), false );
	pNodeEnvelopes->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeEnvelopes->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_CREATE_ENVELOPES_LARGE) ) );
	pNodeEnvelopes->m_iconBig.m_bmpNormal.Make32();
	pNodeEnvelopes->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeEnvelopes->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_CREATE_ENVELOPES_SMALL) ) );
	pNodeEnvelopes->m_iconSmall.m_bmpNormal.Make32();
	pNodeEnvelopes->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeEnvelopes
		);

CExtRibbonNode * pNodeLabels =
		new CExtRibbonNode( ID_MAILINGS_CREATE_LABELS_LARGE, 0, NULL, 0, _T("Labels") );
	pNodeLabels->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('L') ) ), false );
	pNodeLabels->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeLabels->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_CREATE_LABELS_LARGE) ) );
	pNodeLabels->m_iconBig.m_bmpNormal.Make32();
	pNodeLabels->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeLabels->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_CREATE_LABELS_SMALL) ) );
	pNodeLabels->m_iconSmall.m_bmpNormal.Make32();
	pNodeLabels->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeLabels
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Mailings_StartMailMerge()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_MAIL_SMM );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZS") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_START_MAIL_MERGE_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_START_MAIL_MERGE_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Start Mail\nMerge") );

CExtRibbonNode * pNodeStartMailMerge =
		new CExtRibbonNode( ID_MAILINGS_CREATE_LABELS_LARGE, 0, NULL, 0, _T("Start Mail\nMerge") );
	pNodeStartMailMerge->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('S') ) ), false );
	pNodeStartMailMerge->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeStartMailMerge->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_START_MAIL_MERGE_LARGE) ) );
	pNodeStartMailMerge->m_iconBig.m_bmpNormal.Make32();
	pNodeStartMailMerge->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeStartMailMerge->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_START_MAIL_MERGE_SMALL) ) );
	pNodeStartMailMerge->m_iconSmall.m_bmpNormal.Make32();
	pNodeStartMailMerge->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeStartMailMerge
		);

CExtRibbonNode * pNodeSelectRecipients =
		new CExtRibbonNode( ID_MAILINGS_CREATE_LABELS_LARGE, 0, NULL, 0, _T("Select\nRecipients") );
	pNodeSelectRecipients->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('R') ) ), false );
	pNodeSelectRecipients->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeSelectRecipients->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_SELECT_RECIPIENTS_LARGE) ) );
	pNodeSelectRecipients->m_iconBig.m_bmpNormal.Make32();
	pNodeSelectRecipients->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeSelectRecipients->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_SELECT_RECIPIENTS_SMALL) ) );
	pNodeSelectRecipients->m_iconSmall.m_bmpNormal.Make32();
	pNodeSelectRecipients->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSelectRecipients
		);

CExtRibbonNode * pNodeEditRecipientList =
		new CExtRibbonNode( ID_MAILINGS_CREATE_LABELS_LARGE, 0, NULL, 0, _T("Edit\nRecipient List") );
	pNodeEditRecipientList->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('D') ) ), false );
	pNodeEditRecipientList->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeEditRecipientList->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_EDIT_RECIPIENT_LIST_LARGE) ) );
	pNodeEditRecipientList->m_iconBig.m_bmpNormal.Make32();
	pNodeEditRecipientList->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeEditRecipientList->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_EDIT_RECIPIENT_LIST_SMALL) ) );
	pNodeEditRecipientList->m_iconSmall.m_bmpNormal.Make32();
	pNodeEditRecipientList->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeEditRecipientList
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Mailings_WriteAndInsertFields()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_MAIL_WIF );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZW") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_HIGHLIGHT_MERGE_FIELDS_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_HIGHLIGHT_MERGE_FIELDS_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Write &&\nInsert Fields") );

CExtRibbonNode * pNodeHighlightMergeFields =
		new CExtRibbonNode( ID_MAILINGS_HIGHLIGHT_MERGE_FIELDS_LARGE, 0, NULL, 0, _T("Highlight\nMerge Fields") );
	pNodeHighlightMergeFields->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
	pNodeHighlightMergeFields->RibbonILE_RuleRemoveSmallILV();
	pNodeHighlightMergeFields->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeHighlightMergeFields->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_HIGHLIGHT_MERGE_FIELDS_LARGE) ) );
	pNodeHighlightMergeFields->m_iconBig.m_bmpNormal.Make32();
	pNodeHighlightMergeFields->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeHighlightMergeFields->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_HIGHLIGHT_MERGE_FIELDS_SMALL) ) );
	pNodeHighlightMergeFields->m_iconSmall.m_bmpNormal.Make32();
	pNodeHighlightMergeFields->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeHighlightMergeFields
		);

	pRibbonGroup->InsertNode(
		NULL,
		new CExtRibbonNode // separator
		);

CExtRibbonNode * pNodeAddressBlock =
		new CExtRibbonNode( ID_MAILINGS_ADDRESS_BLOCK_LARGE, 0, NULL, 0, _T("Address\nBlock") );
	pNodeAddressBlock->RibbonILE_RuleRemoveSmallILV();
	pNodeAddressBlock->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('A') ) ), false );
	VERIFY( pNodeAddressBlock->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_ADDRESS_BLOCK_LARGE) ) );
	pNodeAddressBlock->m_iconBig.m_bmpNormal.Make32();
	pNodeAddressBlock->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeAddressBlock->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_ADDRESS_BLOCK_SMALL) ) );
	pNodeAddressBlock->m_iconSmall.m_bmpNormal.Make32();
	pNodeAddressBlock->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeAddressBlock
		);

CExtRibbonNode * pNodeGreetingLine =
		new CExtRibbonNode( ID_MAILINGS_GREETING_LINE_LARGE, 0, NULL, 0, _T("Greeting\nLine") );
	pNodeGreetingLine->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('G') ) ), false );
	pNodeGreetingLine->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeGreetingLine->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_GREETING_LINE_LARGE) ) );
	pNodeGreetingLine->m_iconBig.m_bmpNormal.Make32();
	pNodeGreetingLine->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeGreetingLine->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_GREETING_LINE_SMALL) ) );
	pNodeGreetingLine->m_iconSmall.m_bmpNormal.Make32();
	pNodeGreetingLine->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeGreetingLine
		);

CExtRibbonNode * pNodeInsertMergeField =
		new CExtRibbonNode( ID_MAILINGS_INSERT_MERGE_FIELD_LARGE, 0, NULL, 0, _T("Insert Merge\nField") );
	pNodeInsertMergeField->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('I') ) ), false );
	pNodeInsertMergeField->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeInsertMergeField->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_INSERT_MERGE_FIELD_LARGE) ) );
	pNodeInsertMergeField->m_iconBig.m_bmpNormal.Make32();
	pNodeInsertMergeField->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeInsertMergeField->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_INSERT_MERGE_FIELD_SMALL) ) );
	pNodeInsertMergeField->m_iconSmall.m_bmpNormal.Make32();
	pNodeInsertMergeField->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeInsertMergeField
		);

CExtRibbonNode * pNodeRules =
		new CExtRibbonNode( ID_MAILINGS_RULES, 0, NULL, 0, _T("Rules") );
	pNodeRules->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('U') ) ), false );
	pNodeRules->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeRules->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_RED_DOT) ) );
	pNodeRules->m_iconSmall.m_bmpNormal.Make32();
	pNodeRules->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeRules
		);

CExtRibbonNode * pNodeMatchFields =
		new CExtRibbonNode( ID_MAILINGS_MATCH_FIELDS_SMALL, 0, NULL, 0, _T("Match Fields") );
	pNodeMatchFields->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('T') ) ), false );
	pNodeMatchFields->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeMatchFields->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_MATCH_FIELDS_SMALL) ) );
	pNodeMatchFields->m_iconSmall.m_bmpNormal.Make32();
	pNodeMatchFields->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeMatchFields
		);

CExtRibbonNode * pNodeUpdateLabels =
		new CExtRibbonNode( ID_MAILINGS_UPDATE_LABELS_SMALL, 0, NULL, 0, _T("Update Labels") );
	pNodeUpdateLabels->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('B') ) ), false );
	pNodeUpdateLabels->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeUpdateLabels->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_UPDATE_LABELS_SMALL) ) );
	pNodeUpdateLabels->m_iconSmall.m_bmpNormal.Make32();
	pNodeUpdateLabels->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeUpdateLabels
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Mailings_PreviewResults()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_MAIL_PREVIEW_RESULTS );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZP") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_PREVIEW_RESULTS_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_PREVIEW_RESULTS_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Preview\nResults") );

CExtRibbonNode * pNodePreviewResults =
		new CExtRibbonNode( ID_MAILINGS_HIGHLIGHT_MERGE_FIELDS_LARGE, 0, NULL, 0, _T("Preview\nResults") );
	pNodePreviewResults->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('P') ) ), false );
	pNodePreviewResults->RibbonILE_RuleRemoveSmallILV();
	pNodePreviewResults->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodePreviewResults->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_PREVIEW_RESULTS_LARGE) ) );
	pNodePreviewResults->m_iconBig.m_bmpNormal.Make32();
	pNodePreviewResults->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodePreviewResults->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_PREVIEW_RESULTS_SMALL) ) );
	pNodePreviewResults->m_iconSmall.m_bmpNormal.Make32();
	pNodePreviewResults->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePreviewResults
		);

	pRibbonGroup->InsertNode(
		NULL,
		new CExtRibbonNode // separator
		);

CExtRibbonNodeGroup * pRibbonNode_ToolGroupNavigator =
		new CExtRibbonNodeToolGroup( 123 );
	pRibbonGroup->InsertNode(
		NULL,
		pRibbonNode_ToolGroupNavigator
		);

CExtRibbonNodeGroup * pRibbonNode_ToolGroupNavigatorForward =
		new CExtRibbonNodeToolGroup( 123 );
	pRibbonNode_ToolGroupNavigator->InsertNode(
		NULL,
		pRibbonNode_ToolGroupNavigatorForward
		);
			CExtRibbonNode * pNodeFirstRecord =
				new CExtRibbonNodeToolButton(  ID_MAILINGS_FIRST_RECORD_SMALL );
			pNodeFirstRecord->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('Q') ) ), false );
			pNodeFirstRecord->RibbonILE_RuleArrayGet().RemoveAll();
			VERIFY( pNodeFirstRecord->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE( ID_MAILINGS_FIRST_RECORD_SMALL) ) );
			pNodeFirstRecord->m_iconSmall.m_bmpNormal.Make32();
			pNodeFirstRecord->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pRibbonNode_ToolGroupNavigatorForward->InsertNode(
				NULL,
				pNodeFirstRecord
				);
			CExtRibbonNode * pNodePrevRecord =
				new CExtRibbonNodeToolButton(  ID_MAILINGS_PREV_RECORD_SMALL );
			pNodePrevRecord->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('M') ) ), false );
			pNodePrevRecord->RibbonILE_RuleArrayGet().RemoveAll();
			VERIFY( pNodePrevRecord->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE( ID_MAILINGS_PREV_RECORD_SMALL) ) );
			pNodePrevRecord->m_iconSmall.m_bmpNormal.Make32();
			pNodePrevRecord->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pRibbonNode_ToolGroupNavigatorForward->InsertNode(
				NULL,
				pNodePrevRecord
				);

CExtRibbonNodeGroup * pRibbonNode_ToolGroupNavigatorMiddle =
		new CExtRibbonNodeToolGroup( 123 );
	pRibbonNode_ToolGroupNavigator->InsertNode(
		NULL,
		pRibbonNode_ToolGroupNavigatorMiddle
		);
			CExtCustomizeCmdTreeNode * pNodeGoToRecord =
				new CExtRibbonNode( ID_MAILINGS_GOTO_RECORD );
			pNodeGoToRecord->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('W') ) ), false );
			pNodeGoToRecord->ModifyFlags( __ECTN_TBB_TEXT_FIELD );
			pNodeGoToRecord->TextFieldWidthSet( 34 );
			pRibbonNode_ToolGroupNavigatorMiddle->InsertNode(
				NULL,
				pNodeGoToRecord
				);

CExtRibbonNodeGroup * pRibbonNode_ToolGroupNavigatorBackward =
		new CExtRibbonNodeToolGroup( 123 );
	pRibbonNode_ToolGroupNavigator->InsertNode(
		NULL,
		pRibbonNode_ToolGroupNavigatorBackward
		);
			CExtRibbonNode * pNodeNextRecord =
				new CExtRibbonNodeToolButton(  ID_MAILINGS_NEXT_RECORD_SMALL );
			pNodeNextRecord->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('X') ) ), false );
			pNodeNextRecord->RibbonILE_RuleArrayGet().RemoveAll();
			VERIFY( pNodeNextRecord->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE( ID_MAILINGS_NEXT_RECORD_SMALL) ) );
			pNodeNextRecord->m_iconSmall.m_bmpNormal.Make32();
			pNodeNextRecord->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pRibbonNode_ToolGroupNavigatorBackward->InsertNode(
				NULL,
				pNodeNextRecord
				);
			CExtRibbonNode * pNodeLastRecord =
				new CExtRibbonNodeToolButton(  ID_MAILINGS_LAST_RECORD_SMALL );
			pNodeLastRecord->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('V') ) ), false );
			pNodeLastRecord->RibbonILE_RuleArrayGet().RemoveAll();
			VERIFY( pNodeLastRecord->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE( ID_MAILINGS_LAST_RECORD_SMALL) ) );
			pNodeLastRecord->m_iconSmall.m_bmpNormal.Make32();
			pNodeLastRecord->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
			pRibbonNode_ToolGroupNavigatorBackward->InsertNode(
				NULL,
				pNodeLastRecord
				);

CExtRibbonNode * pNodeFindRecipient =
		new CExtRibbonNode( ID_MAILINGS_FIND_RECIPIENT_SMALL, 0, NULL, 0, _T("Find Recipient") );
	pNodeFindRecipient->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('J') ) ), false );
	pNodeFindRecipient->RibbonILE_RuleRemoveSmallILV();
	pNodeFindRecipient->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeFindRecipient->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_FIND_RECIPIENT_SMALL) ) );
	pNodeFindRecipient->m_iconSmall.m_bmpNormal.Make32();
	pNodeFindRecipient->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeFindRecipient
		);

CExtRibbonNode * pNodeAutoCheckForErrors =
		new CExtRibbonNode( ID_MAILINGS_AUTO_CHECK_FOR_ERRORS_SMALL, 0, NULL, 0, _T("Auto Check for Errors") );
	pNodeAutoCheckForErrors->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('K') ) ), false );
	pNodeAutoCheckForErrors->RibbonILE_RuleRemoveSmallILV();
	pNodeAutoCheckForErrors->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeAutoCheckForErrors->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_AUTO_CHECK_FOR_ERRORS_SMALL) ) );
	pNodeAutoCheckForErrors->m_iconSmall.m_bmpNormal.Make32();
	pNodeAutoCheckForErrors->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeAutoCheckForErrors
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Mailings_Finish()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_MAIL_FINISH );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZF") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_FINISH_AND_MERGE_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Finish") );

CExtRibbonNode * pNodeFinishAndMerge =
		new CExtRibbonNode( ID_MAILINGS_HIGHLIGHT_MERGE_FIELDS_LARGE, 0, NULL, 0, _T("Finish &&\nMerge") );
	pNodeFinishAndMerge->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('F') ) ), false );
	pNodeFinishAndMerge->RibbonILE_RuleRemoveSmallILV();
	pNodeFinishAndMerge->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeFinishAndMerge->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_MAILINGS_FINISH_AND_MERGE_LARGE) ) );
	pNodeFinishAndMerge->m_iconBig.m_bmpNormal.Make32();
	pNodeFinishAndMerge->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeFinishAndMerge
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Review_Proofing()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REVIEW_PROOFING );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZP") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_SPELLING_AND_GRAMMAR_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_SPELLING_AND_GRAMMAR_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Spelling &&\nGrammar") );

CExtRibbonNode * pNodeSpellingAndGrammar =
		new CExtRibbonNode( ID_MARGINS_LARGE, 0, NULL, 0, _T("Spelling &&\nGrammar") );
	pNodeSpellingAndGrammar->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('S') ) ), false );
	pNodeSpellingAndGrammar->RibbonILE_RuleRemoveSmallILV();
	pNodeSpellingAndGrammar->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeSpellingAndGrammar->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_SPELLING_AND_GRAMMAR_LARGE) ) );
	pNodeSpellingAndGrammar->m_iconBig.m_bmpNormal.Make32();
	pNodeSpellingAndGrammar->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeSpellingAndGrammar->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_SPELLING_AND_GRAMMAR_SMALL) ) );
	pNodeSpellingAndGrammar->m_iconSmall.m_bmpNormal.Make32();
	pNodeSpellingAndGrammar->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSpellingAndGrammar
		);

CExtRibbonNode * pNodeResearch =
		new CExtRibbonNode( ID_ORIENTATION_LARGE, 0, NULL, 0, _T("Research") );
	pNodeResearch->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('R') ) ), false );
	pNodeResearch->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeResearch->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PROOFING_RESEARCH_LARGE) ) );
	pNodeResearch->m_iconBig.m_bmpNormal.Make32();
	pNodeResearch->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeResearch->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PROOFING_RESEARCH_SMALL) ) );
	pNodeResearch->m_iconSmall.m_bmpNormal.Make32();
	pNodeResearch->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeResearch
		);

CExtRibbonNode * pNodeThesaurus =
		new CExtRibbonNode( ID_SIZE_LARGE, 0, NULL, 0, _T("Thesaurus") );
	pNodeThesaurus->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('E') ) ), false );
	pNodeThesaurus->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeThesaurus->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PROOFING_THESAURUS_LARGE) ) );
	pNodeThesaurus->m_iconBig.m_bmpNormal.Make32();
	pNodeThesaurus->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeThesaurus->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PROOFING_THESAURUS_SMALL) ) );
	pNodeThesaurus->m_iconSmall.m_bmpNormal.Make32();
	pNodeThesaurus->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeThesaurus
		);

CExtRibbonNode * pNodeTranslate =
		new CExtRibbonNode( ID_COLUMNS_LARGE, 0, NULL, 0, _T("Translate") );
	pNodeTranslate->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('L') ) ), false );
	pNodeTranslate->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeTranslate->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PROOFING_TRANSLATE_LARGE) ) );
	pNodeTranslate->m_iconBig.m_bmpNormal.Make32();
	pNodeTranslate->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeTranslate->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PROOFING_TRANSLATE_SMALL) ) );
	pNodeTranslate->m_iconSmall.m_bmpNormal.Make32();
	pNodeTranslate->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeTranslate
		);

CExtRibbonNode * pNodeTranslationToolTip =
		new CExtRibbonNode( ID_TRANSLATION_TOOL_TIP, 0, NULL, 0, _T("Translation Tool Tip") );
	pNodeTranslationToolTip->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("PT") ) ), false );
	pNodeTranslationToolTip->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeTranslationToolTip->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ORANGE_DOT) ) );
	pNodeTranslationToolTip->m_iconSmall.m_bmpNormal.Make32();
	pNodeTranslationToolTip->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeTranslationToolTip
		);

CExtRibbonNode * pNodeSetLanguage =
		new CExtRibbonNode( ID_PROOFING_SET_LANGUAGE_SMALL, 0, NULL, 0, _T("Set Language") );
	pNodeSetLanguage->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('U') ) ), false );
	pNodeSetLanguage->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeSetLanguage->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PROOFING_SET_LANGUAGE_SMALL) ) );
	pNodeSetLanguage->m_iconSmall.m_bmpNormal.Make32();
	pNodeSetLanguage->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSetLanguage
		);

CExtRibbonNode * pNodeWordCount =
		new CExtRibbonNode( ID_PAGE_HYPHENATION_SMALL, 0, NULL, 0, _T("WordCount") );
	pNodeWordCount->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('W') ) ), false );
	pNodeWordCount->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeWordCount->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PROOFING_WORD_COUNT_SMALL) ) );
	pNodeWordCount->m_iconSmall.m_bmpNormal.Make32();
	pNodeWordCount->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeWordCount
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Review_Comments()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REVIEW_COMMENTS );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZC") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMMENT_NEW_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMMENT_NEW_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Comments") );

CExtRibbonNode * pNodeNewComment =
		new CExtRibbonNode( ID_COMMENT_NEW_LARGE, 0, NULL, 0, _T("New\nComment") );
	pNodeNewComment->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('C') ) ), false );
	pNodeNewComment->RibbonILE_RuleRemoveSmallILV();
	pNodeNewComment->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeNewComment->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMMENT_NEW_LARGE) ) );
	pNodeNewComment->m_iconBig.m_bmpNormal.Make32();
	pNodeNewComment->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeNewComment->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMMENT_NEW_SMALL) ) );
	pNodeNewComment->m_iconSmall.m_bmpNormal.Make32();
	pNodeNewComment->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeNewComment
		);

CExtRibbonNode * pNodeDelete =
		new CExtRibbonNode( ID_COMMENT_DELETE_LARGE, 0, NULL, 0, _T("Delete") );
	pNodeDelete->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('D') ) ), false );
	VERIFY( pNodeDelete->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMMENT_DELETE_LARGE) ) );
	pNodeDelete->m_iconBig.m_bmpNormal.Make32();
	pNodeDelete->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeDelete->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMMENT_DELETE_SMALL) ) );
	pNodeDelete->m_iconSmall.m_bmpNormal.Make32();
	pNodeDelete->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeDelete
		);

CExtRibbonNode * pNodePrevious =
		new CExtRibbonNode( ID_COMMENT_PREV_LARGE, 0, NULL, 0, _T("Previous") );
	pNodePrevious->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('V') ) ), false );
	VERIFY( pNodePrevious->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMMENT_PREV_LARGE) ) );
	pNodePrevious->m_iconBig.m_bmpNormal.Make32();
	pNodePrevious->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodePrevious->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMMENT_PREV_SMALL) ) );
	pNodePrevious->m_iconSmall.m_bmpNormal.Make32();
	pNodePrevious->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePrevious
		);

CExtRibbonNode * pNodeNext =
		new CExtRibbonNode( ID_COMMENT_NEXT_LARGE, 0, NULL, 0, _T("Next") );
	pNodeNext->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('N') ) ), false );
	VERIFY( pNodeNext->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMMENT_NEXT_LARGE) ) );
	pNodeNext->m_iconBig.m_bmpNormal.Make32();
	pNodeNext->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeNext->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMMENT_NEXT_SMALL) ) );
	pNodeNext->m_iconSmall.m_bmpNormal.Make32();
	pNodeNext->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeNext
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Review_Tracking()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REVIEW_TRACKING );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZT") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TRACKING_TRACK_CHANGES_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TRACKING_TRACK_CHANGES_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Tracking") );

CExtRibbonNode * pNodeTrackChanges =
		new CExtRibbonNode( ID_TRACKING_TRACK_CHANGES_LARGE, 0, NULL, 0, _T("Track\nChanges") );
	pNodeTrackChanges->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('G') ) ), false );
	pNodeTrackChanges->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeTrackChanges->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TRACKING_TRACK_CHANGES_LARGE) ) );
	pNodeTrackChanges->m_iconBig.m_bmpNormal.Make32();
	pNodeTrackChanges->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeTrackChanges->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TRACKING_TRACK_CHANGES_SMALL) ) );
	pNodeTrackChanges->m_iconSmall.m_bmpNormal.Make32();
	pNodeTrackChanges->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeTrackChanges
		);

CExtRibbonNode * pNodeBalloons =
		new CExtRibbonNode( ID_TRACKING_BALLOONS_LARGE, 0, NULL, 0, _T("Balloons") );
	pNodeBalloons->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("TB") ) ), false );
	pNodeBalloons->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeBalloons->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TRACKING_BALLOONS_LARGE) ) );
	pNodeBalloons->m_iconBig.m_bmpNormal.Make32();
	pNodeBalloons->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeBalloons->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TRACKING_BALLOONS_SMALL) ) );
	pNodeBalloons->m_iconSmall.m_bmpNormal.Make32();
	pNodeBalloons->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeBalloons
		);

CExtRibbonNode * pNodeMarkup =
		new CExtRibbonNode( ID_TRACKING_MARKUP_SMALL, 0, NULL, 0, _T("Markup") );
	pNodeMarkup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("TD") ) ), false );
	pNodeMarkup->RibbonILE_RuleRemoveSmallILV();
	pNodeMarkup->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeMarkup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TRACKING_MARKUP_SMALL) ) );
	pNodeMarkup->m_iconSmall.m_bmpNormal.Make32();
	pNodeMarkup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeMarkup
		);

CExtRibbonNode * pNodeShowMarkup =
		new CExtRibbonNode( ID_TRACKING_SHOW_MARKUP_SMALL, 0, NULL, 0, _T("Show Markup") );
	pNodeShowMarkup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("TM") ) ), false );
	pNodeShowMarkup->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeShowMarkup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TRACKING_SHOW_MARKUP_SMALL) ) );
	pNodeShowMarkup->m_iconSmall.m_bmpNormal.Make32();
	pNodeShowMarkup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeShowMarkup
		);

CExtRibbonNode * pNodeReviewingPane =
		new CExtRibbonNode( ID_TRACKING_REVIEWING_PANE_SMALL, 0, NULL, 0, _T("Next") );
	pNodeReviewingPane->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("TP") ) ), false );
	pNodeReviewingPane->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeReviewingPane->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_TRACKING_REVIEWING_PANE_SMALL) ) );
	pNodeReviewingPane->m_iconSmall.m_bmpNormal.Make32();
	pNodeReviewingPane->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeReviewingPane
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Review_Changes()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REVIEW_CHANGES );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZH") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGES_REJECT_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGES_REJECT_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Changes") );

CExtRibbonNode * pNodeAccept =
		new CExtRibbonNode( ID_CHANGES_ACCEPT_LARGE, 0, NULL, 0, _T("Accept") );
	pNodeAccept->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('A') ) ), false );
	pNodeAccept->RibbonILE_RuleRemoveSmallILV();
	pNodeAccept->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeAccept->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGES_ACCEPT_LARGE) ) );
	pNodeAccept->m_iconBig.m_bmpNormal.Make32();
	pNodeAccept->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeAccept
		);

CExtRibbonNode * pNodeReject =
		new CExtRibbonNode( ID_CHANGES_REJECT_LARGE, 0, NULL, 0, _T("Reject") );
	pNodeReject->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('R') ) ), false );
	VERIFY( pNodeReject->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGES_REJECT_LARGE) ) );
	pNodeReject->m_iconBig.m_bmpNormal.Make32();
	pNodeReject->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeReject->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGES_REJECT_SMALL) ) );
	pNodeReject->m_iconSmall.m_bmpNormal.Make32();
	pNodeReject->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeReject
		);

CExtRibbonNode * pNodePrevious =
		new CExtRibbonNode( ID_CHANGES_PREV_SMALL, 0, NULL, 0, _T("Previous") );
	pNodePrevious->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('F') ) ), false );
	pNodePrevious->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodePrevious->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGES_PREV_SMALL) ) );
	pNodePrevious->m_iconSmall.m_bmpNormal.Make32();
	pNodePrevious->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePrevious
		);

CExtRibbonNode * pNodeNext =
		new CExtRibbonNode( ID_CHANGES_NEXT_SMALL, 0, NULL, 0, _T("Next") );
	pNodeNext->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
	pNodeNext->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeNext->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_CHANGES_NEXT_SMALL) ) );
	pNodeNext->m_iconSmall.m_bmpNormal.Make32();
	pNodeNext->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeNext
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Review_Compare()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REVIEW_COMPARE );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZD") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMPARE_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pRibbonGroup->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMPARE_SMALL) ) );
	pRibbonGroup->m_iconSmall.m_bmpNormal.Make32();
	pRibbonGroup->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Compare") );

CExtRibbonNode * pNodeCompare =
		new CExtRibbonNode( ID_COMPARE_LARGE, 0, NULL, 0, _T("Compare") );
	pNodeCompare->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('M') ) ), false );
	pNodeCompare->RibbonILE_RuleRemoveSmallILV();
	pNodeCompare->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeCompare->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMPARE_LARGE) ) );
	pNodeCompare->m_iconBig.m_bmpNormal.Make32();
	pNodeCompare->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeCompare->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_COMPARE_SMALL) ) );
	pNodeCompare->m_iconSmall.m_bmpNormal.Make32();
	pNodeCompare->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeCompare
		);

CExtRibbonNode * pNodeShowSourceDocuments =
		new CExtRibbonNode( ID_SHOW_SRC_DOCS_LARGE, 0, NULL, 0, _T("Show Source\nDocuments") );
	pNodeShowSourceDocuments->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('O') ) ), false );
	pNodeShowSourceDocuments->RibbonILE_RuleRemoveSmallILV();
	pNodeShowSourceDocuments->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeShowSourceDocuments->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_SHOW_SRC_DOCS_LARGE) ) );
	pNodeShowSourceDocuments->m_iconBig.m_bmpNormal.Make32();
	pNodeShowSourceDocuments->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeShowSourceDocuments
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_Review_Protect()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_REVIEW_PROTECT );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZR") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PROTECT_DOC_LARGE) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Protect") );

CExtRibbonNode * pNodeProtectDocument =
		new CExtRibbonNode( ID_PROTECT_DOC_LARGE, 0, NULL, 0, _T("Protect\nDocument") );
	pNodeProtectDocument->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("PR") ) ), false );
	pNodeProtectDocument->RibbonILE_RuleRemoveSmallILV();
	pNodeProtectDocument->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeProtectDocument->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_PROTECT_DOC_LARGE) ) );
	pNodeProtectDocument->m_iconBig.m_bmpNormal.Make32();
	pNodeProtectDocument->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeProtectDocument
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_View_DocumentViews()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_VIEW_DV );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZV") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_BLUE_DOWN_ARROW) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Document\nViews") );

CExtRibbonNode * pNodePrintLayout =
		new CExtRibbonNode( ID_DV_PRINT_LAYOUT_LARGE, 0, NULL, 0, _T("Print\nLayout") );
	pNodePrintLayout->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('P') ) ), false );
	pNodePrintLayout->RibbonILE_RuleRemoveSmallILV();
	pNodePrintLayout->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodePrintLayout->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_DV_PRINT_LAYOUT_LARGE) ) );
	pNodePrintLayout->m_iconBig.m_bmpNormal.Make32();
	pNodePrintLayout->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodePrintLayout
		);

CExtRibbonNode * pNodeFullScreenReading =
		new CExtRibbonNode( ID_DV_FULL_SCREEN_LARGE, 0, NULL, 0, _T("Full Screen\nReading") );
	pNodeFullScreenReading->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('F') ) ), false );
	pNodeFullScreenReading->RibbonILE_RuleRemoveSmallILV();
	pNodeFullScreenReading->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeFullScreenReading->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_DV_FULL_SCREEN_LARGE) ) );
	pNodeFullScreenReading->m_iconBig.m_bmpNormal.Make32();
	pNodeFullScreenReading->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeFullScreenReading
		);

CExtRibbonNode * pNodeWebLayout =
		new CExtRibbonNode( ID_DV_WEB_LAYOUT_LARGE, 0, NULL, 0, _T("Web\nLayout") );
	pNodeWebLayout->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('L') ) ), false );
	pNodeWebLayout->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeWebLayout->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_DV_WEB_LAYOUT_LARGE) ) );
	pNodeWebLayout->m_iconBig.m_bmpNormal.Make32();
	pNodeWebLayout->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeWebLayout->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_DV_WEB_LAYOUT_SMALL) ) );
	pNodeWebLayout->m_iconSmall.m_bmpNormal.Make32();
	pNodeWebLayout->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeWebLayout
		);

CExtRibbonNode * pNodeOutline =
		new CExtRibbonNode( ID_DV_OUTLINE_LARGE, 0, NULL, 0, _T("Outline") );
	pNodeOutline->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('U') ) ), false );
	pNodeOutline->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeOutline->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_DV_OUTLINE_LARGE) ) );
	pNodeOutline->m_iconBig.m_bmpNormal.Make32();
	pNodeOutline->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeOutline->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_DV_OUTLINE_SMALL) ) );
	pNodeOutline->m_iconSmall.m_bmpNormal.Make32();
	pNodeOutline->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeOutline
		);

CExtRibbonNode * pNodeDraft =
		new CExtRibbonNode( ID_DV_DRAFT_LARGE, 0, NULL, 0, _T("Draft") );
	pNodeDraft->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('E') ) ), false );
	pNodeDraft->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeDraft->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_DV_DRAFT_LARGE) ) );
	pNodeDraft->m_iconBig.m_bmpNormal.Make32();
	pNodeDraft->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeDraft->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_DV_DRAFT_SMALL) ) );
	pNodeDraft->m_iconSmall.m_bmpNormal.Make32();
	pNodeDraft->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeDraft
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_View_ShowHide()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_VIEW_SHOW_HIDE );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZS") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_BLUE_DOWN_ARROW) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Show/Hide") );

CExtRibbonNode * pNodeRuler =
		new CExtRibbonNode( ID_SHOW_HIDE_RULER, 0, NULL, 0, _T("Ruler") );
	pNodeRuler->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('R') ) ), false );
	pNodeRuler->RibbonILE_RuleRemoveSmallILV();
	pNodeRuler->RibbonILE_RuleRemoveLargeILV();
	pNodeRuler->ModifyFlags( __ECTN_TBB_CHECK_BOX );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeRuler
		);

CExtRibbonNode * pNodeGridlines =
		new CExtRibbonNode( ID_SHOW_HIDE_GRIDLINES, 0, NULL, 0, _T("Gridlines") );
	pNodeGridlines->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('G') ) ), false );
	pNodeGridlines->RibbonILE_RuleRemoveSmallILV();
	pNodeGridlines->RibbonILE_RuleRemoveLargeILV();
	pNodeGridlines->ModifyFlags( __ECTN_TBB_CHECK_BOX );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeGridlines
		);

CExtRibbonNode * pNodeProperties =
		new CExtRibbonNode( ID_SHOW_HIDE_PROPERTIES, 0, NULL, 0, _T("Properties") );
	pNodeProperties->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('D') ) ), false );
	pNodeProperties->RibbonILE_RuleRemoveSmallILV();
	pNodeProperties->RibbonILE_RuleRemoveLargeILV();
	pNodeProperties->ModifyFlags( __ECTN_TBB_CHECK_BOX );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeProperties
		);

CExtRibbonNode * pNodeDocumentMap =
		new CExtRibbonNode( ID_SHOW_HIDE_DOCUMENT_MAP, 0, NULL, 0, _T("Document Map") );
	pNodeDocumentMap->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("VM") ) ), false );
	pNodeDocumentMap->RibbonILE_RuleRemoveSmallILV();
	pNodeDocumentMap->RibbonILE_RuleRemoveLargeILV();
	pNodeDocumentMap->ModifyFlags( __ECTN_TBB_CHECK_BOX );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeDocumentMap
		);

CExtRibbonNode * pNodeThumbnails =
		new CExtRibbonNode( ID_SHOW_HIDE_THUMBNAILS, 0, NULL, 0, _T("Thumbnails") );
	pNodeThumbnails->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('H') ) ), false );
	pNodeThumbnails->RibbonILE_RuleRemoveSmallILV();
	pNodeThumbnails->RibbonILE_RuleRemoveLargeILV();
	pNodeThumbnails->ModifyFlags( __ECTN_TBB_CHECK_BOX );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeThumbnails
		);

CExtRibbonNode * pNodeDocumentActionBar =
		new CExtRibbonNode( ID_SHOW_HIDE_DOCUMENT_ACTION_BAR, 0, NULL, 0, _T("Document Action Bar") );
	pNodeDocumentActionBar->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("VA") ) ), false );
	pNodeDocumentActionBar->RibbonILE_RuleRemoveSmallILV();
	pNodeDocumentActionBar->RibbonILE_RuleRemoveLargeILV();
	pNodeDocumentActionBar->ModifyFlags( __ECTN_TBB_CHECK_BOX );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeDocumentActionBar
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_View_Zoom()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_VIEW_ZOOM );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZZ") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_BLUE_DOWN_ARROW) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Zoom") );

CExtRibbonNode * pNodeZoom =
		new CExtRibbonNode( ID_ZOOM_LARGE, 0, NULL, 0, _T("Zoom") );
	pNodeZoom->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('Q') ) ), false );
	pNodeZoom->RibbonILE_RuleRemoveSmallILV();
	pNodeZoom->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeZoom->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_ZOOM_LARGE) ) );
	pNodeZoom->m_iconBig.m_bmpNormal.Make32();
	pNodeZoom->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeZoom
		);

	return pRibbonGroup;
}

CExtRibbonNode * CMainFrame::_InitRibbonNode_View_Window()
{
CExtRibbonNodeGroup * pRibbonGroup =
		new CExtRibbonNodeGroup( ID_RIBBON_PAGE_VIEW_WINDOW );
	pRibbonGroup->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("ZW") ) ), false );
	VERIFY( pRibbonGroup->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_BLUE_DOWN_ARROW) ) );
	pRibbonGroup->m_iconBig.m_bmpNormal.Make32();
	pRibbonGroup->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->SetTextInToolbar( _T("Window") );

CExtRibbonNode * pNodeNewWindow =
		new CExtRibbonNode( ID_WINDOW_NEW_LARGE, 0, NULL, 0, _T("New\nWindow") );
	pNodeNewWindow->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('N') ) ), false );
	pNodeNewWindow->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeNewWindow->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_WINDOW_NEW_LARGE) ) );
	pNodeNewWindow->m_iconBig.m_bmpNormal.Make32();
	pNodeNewWindow->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeNewWindow->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_WINDOW_NEW_SMALL) ) );
	pNodeNewWindow->m_iconSmall.m_bmpNormal.Make32();
	pNodeNewWindow->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeNewWindow
		);

CExtRibbonNode * pNodeArrangeAll =
		new CExtRibbonNode( ID_WINDOW_ARRANGE_ALL_LARGE, 0, NULL, 0, _T("Arrange\nAll") );
	pNodeArrangeAll->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('A') ) ), false );
	pNodeArrangeAll->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeArrangeAll->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_WINDOW_ARRANGE_ALL_LARGE) ) );
	pNodeArrangeAll->m_iconBig.m_bmpNormal.Make32();
	pNodeArrangeAll->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeArrangeAll->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_WINDOW_ARRANGE_ALL_SMALL) ) );
	pNodeArrangeAll->m_iconSmall.m_bmpNormal.Make32();
	pNodeArrangeAll->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeArrangeAll
		);

CExtRibbonNode * pNodeSplit =
		new CExtRibbonNode( ID_WINDOW_SPLIT_LARGE, 0, NULL, 0, _T("Split") );
	pNodeSplit->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('S') ) ), false );
	pNodeSplit->RibbonILE_RuleRemoveSmallILV();
	VERIFY( pNodeSplit->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_WINDOW_SPLIT_LARGE) ) );
	pNodeSplit->m_iconBig.m_bmpNormal.Make32();
	pNodeSplit->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	VERIFY( pNodeSplit->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_WINDOW_SPLIT_SMALL) ) );
	pNodeSplit->m_iconSmall.m_bmpNormal.Make32();
	pNodeSplit->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSplit
		);

	pRibbonGroup->InsertNode(
		NULL,
		new CExtRibbonNode // separator
		);

CExtRibbonNode * pNodeCompareSideBySide =
		new CExtRibbonNode( ID_WINDOW_CLOSE_SIDE_BY_SIDE_SMALL, 0, NULL, 0, _T("Compare Side by Side") );
	pNodeCompareSideBySide->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('B') ) ), false );
	pNodeCompareSideBySide->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeCompareSideBySide->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_WINDOW_CLOSE_SIDE_BY_SIDE_SMALL) ) );
	pNodeCompareSideBySide->m_iconSmall.m_bmpNormal.Make32();
	pNodeCompareSideBySide->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeCompareSideBySide
		);

CExtRibbonNode * pNodeSynchronousScrolling =
		new CExtRibbonNode( ID_WINDOW_SYNC_SCROLLING_SMALL, 0, NULL, 0, _T("Synchronous Scrolling") );
	pNodeSynchronousScrolling->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_LPCTSTR( _T("VS") ) ), false );
	pNodeSynchronousScrolling->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeSynchronousScrolling->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_WINDOW_SYNC_SCROLLING_SMALL) ) );
	pNodeSynchronousScrolling->m_iconSmall.m_bmpNormal.Make32();
	pNodeSynchronousScrolling->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSynchronousScrolling
		);

CExtRibbonNode * pNodeResetWindowPosition =
		new CExtRibbonNode( ID_WINDOW_RESET_POSITION_SMALL, 0, NULL, 0, _T("Reset Window Position") );
	pNodeResetWindowPosition->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('T') ) ), false );
	pNodeResetWindowPosition->RibbonILE_RuleRemoveLargeILV();
	VERIFY( pNodeResetWindowPosition->m_iconSmall.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_WINDOW_RESET_POSITION_SMALL) ) );
	pNodeResetWindowPosition->m_iconSmall.m_bmpNormal.Make32();
	pNodeResetWindowPosition->m_iconSmall.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeResetWindowPosition
		);

	pRibbonGroup->InsertNode(
		NULL,
		new CExtRibbonNode // separator
		);

CExtRibbonNode * pNodeSwitchWindows =
		new CExtRibbonNode( ID_WINDOW_SWITCH_LARGE, 0, NULL, 0, _T("Switch\nWindows") );
	pNodeSwitchWindows->CmdKeyTipSet( new CExtCustomizeCmdKeyTip( __EXT_MFC_SAFE_TCHAR( _T('W') ) ), false );
	pNodeSwitchWindows->RibbonILE_RuleRemoveSmallILV();
	pNodeSwitchWindows->RibbonILE_RuleRemoveNormalILV();
	VERIFY( pNodeSwitchWindows->m_iconBig.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE(ID_WINDOW_SWITCH_LARGE) ) );
	pNodeSwitchWindows->m_iconBig.m_bmpNormal.Make32();
	pNodeSwitchWindows->m_iconBig.m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), 0 );
	pRibbonGroup->InsertNode(
		NULL,
		pNodeSwitchWindows
		);

	return pRibbonGroup;
}

