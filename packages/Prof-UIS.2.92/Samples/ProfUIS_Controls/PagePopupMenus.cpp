// PagePopupMenus.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PagePopupMenus.h"
#include <Resources/Resource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPagePopupMenus dialog


CPagePopupMenus::CPagePopupMenus(CWnd* pParent /*=NULL*/)
	: CPageBase(CPagePopupMenus::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPagePopupMenus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPagePopupMenus::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPagePopupMenus)
	DDX_Control(pDX, IDC_CHECK_USE_ACCELERATED_MENU_SCROLLING, m_checkUseAcceleratedMenuScrolling);
	DDX_Control(pDX, IDC_EDIT_INCREASE_PERCENT, m_editIncreasePercent);
	DDX_Control(pDX, IDC_EDIT_INCREASE_STEP, m_editIncreaseStep);
	DDX_Control(pDX, IDC_EDIT_LONG_COUNT_OF_ITEMS, m_editCountOfItems);
	DDX_Control(pDX, IDC_EDIT_MAX_PIXELS_PER_STEP, m_editMaxPixelsPerStep);
	DDX_Control(pDX, IDC_SPIN_INCREASE_PERCENT, m_spinIncreasePercent);
	DDX_Control(pDX, IDC_SPIN_INCREASE_STEP, m_spinIncreaseStep);
	DDX_Control(pDX, IDC_SPIN_LONG_COUNT_OF_ITEMS, m_spinCountOfItems);
	DDX_Control(pDX, IDC_SPIN_MAX_PIXELS_PER_STEP, m_spinMaxPixelsPerStep);
	DDX_Control(pDX, IDC_STATIC_INCREASE_PERCENT, m_labelIncreasePercent);
	DDX_Control(pDX, IDC_STATIC_INCREASE_STEP, m_labelIncreaseStep);
	DDX_Control(pDX, IDC_STATIC_LONG_COUNT_OF_ITEMS, m_labelCountOfItems);
	DDX_Control(pDX, IDC_STATIC_MAX_PIXELS_PER_STEP, m_labelMaxPixelsPerStep);
	DDX_Control(pDX, IDC_STATIC_LONG_SUB_MENU_SETTINGS, m_groupBoxLongSubMenuSettings);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_CHECK_MENU_WITH_SHADOWS, m_chkMenuWithShadows);
	DDX_Control(pDX, IDC_CHECK_MENU_ANIMATION, m_chkMenuAnimation);
	DDX_Control(pDX, IDC_CHECK_PALETTE_WITH_CUSTOM_DRAWN_BACKGROUND, m_chkPaletteWithCustomDrawnBackground);
	DDX_Control(pDX, IDC_STATIC_MENU_ANIMATION, m_groupBoxMenuAnimation);
	DDX_Control(pDX, IDC_MENU_ANIMATION, m_MenuAnimation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPagePopupMenus, CPageBase)
	//{{AFX_MSG_MAP(CPagePopupMenus)
	ON_CBN_SELENDOK(IDC_MENU_ANIMATION, OnSelendokMenuAnimation)
	ON_BN_CLICKED(IDC_CHECK_MENU_ANIMATION, OnCheckMenuAnimation)
	ON_BN_CLICKED(IDC_CHECK_MENU_WITH_SHADOWS, OnCheckMenuWithShadows)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_CHECK_USE_ACCELERATED_MENU_SCROLLING, OnCheckUseAcceleratedMenuScrolling)
	ON_EN_CHANGE(IDC_EDIT_INCREASE_PERCENT, OnChangeEditIncreasePercent)
	ON_EN_CHANGE(IDC_EDIT_INCREASE_STEP, OnChangeEditIncreaseStep)
	ON_EN_CHANGE(IDC_EDIT_MAX_PIXELS_PER_STEP, OnChangeEditMaxPixelsPerStep)
	//}}AFX_MSG_MAP
//				ON_REGISTERED_MESSAGE(
//					CExtPopupMenuWnd::g_nMsgTranslateMouseClickEvent,
//					OnMsgTranslateMouseClickEvent
//					)
	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPrepareMenu,
		OnExtMenuPrepare
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPopupDrawBackground,
		OnExtMenuDrawBackground
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPopupDrawItem,
		OnDrawPopupMenuItem
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupMenuWnd::g_nMsgPopupDrawLeftArea,
		OnDrawPopupLeftArea
		)
#if (!defined __EXT_MFC_NO_DATE_PICKER)
	ON_REGISTERED_MESSAGE(CExtDatePickerWnd::g_nMsgSelectionNotification, OnMsgSelectionNotification)
#endif // #if (!defined __EXT_MFC_NO_DATE_PICKER)	
	
	// color popup menu
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyCustColor,
		OnColorSelectCustom
		)

	ON_COMMAND( IDC_MENU_ITEM_RARELY1, OnMenuRarelyItem1 )
	ON_UPDATE_COMMAND_UI( IDC_MENU_ITEM_RARELY1, OnUpdateMenuRarelyItem1 )

	ON_COMMAND( IDC_MENU_ITEM_RARELY2, OnMenuRarelyItem2 )
	ON_UPDATE_COMMAND_UI( IDC_MENU_ITEM_RARELY2, OnUpdateMenuRarelyItem2 )

	ON_COMMAND( IDC_MENU_ITEM_RARELY3, OnMenuRarelyItem3 )
	ON_UPDATE_COMMAND_UI( IDC_MENU_ITEM_RARELY3, OnUpdateMenuRarelyItem3 )

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPagePopupMenus message handlers

BOOL CPagePopupMenus::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	
	m_Static1.SetFontBold( true );

	m_chkPaletteWithCustomDrawnBackground.SetCheck( 1 );
	
	AddAnchor( IDC_STATIC1, CSize(50,0), CSize(50,0) );

	m_groupBoxMenuAnimation.SetStyle( CExtGroupBox::STYLE_FLAT );
	m_groupBoxLongSubMenuSettings.SetStyle( CExtGroupBox::STYLE_FLAT );
	
	m_chkMenuWithShadows.SetCheck( BST_CHECKED );
	OnCheckMenuWithShadows();
	m_chkMenuAnimation.SetCheck( BST_CHECKED );
	OnCheckMenuAnimation();

	int nIndex = -1, nInitSel = -1;
	nIndex = m_MenuAnimation.AddString( _T("(none)") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_NONE );

	nIndex = m_MenuAnimation.AddString( _T("Random select") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_RANDOM);

	nIndex = m_MenuAnimation.AddString( _T("Roll") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_ROLL);

	nIndex = m_MenuAnimation.AddString( _T("Slide") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_SLIDE);

	nInitSel = nIndex = m_MenuAnimation.AddString( _T("Fade") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_FADE);

	nIndex = m_MenuAnimation.AddString( _T("Roll and stretch") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_ROLL_AND_STRETCH);

	nIndex = m_MenuAnimation.AddString( _T("Slide and stretch") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_SLIDE_AND_STRETCH);

	nIndex = m_MenuAnimation.AddString( _T("Noise") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_NOISE);

	nIndex = m_MenuAnimation.AddString( _T("Boxes") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_BOXES);

	nIndex = m_MenuAnimation.AddString( _T("Circles") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_CIRCLES);

	nIndex = m_MenuAnimation.AddString( _T("Holes") );
	m_MenuAnimation.SetItemData( nIndex, CExtPopupMenuWnd::__AT_HOLES);

	m_editCountOfItems.SetWindowText( _T("100") );
CString strTmp;
	strTmp.Format( _T("%d"), CExtPopupMenuWnd::g_nAcceleratedIncreasePercent );
	m_editIncreasePercent.SetWindowText( LPCTSTR(strTmp) );
	strTmp.Format( _T("%d"), CExtPopupMenuWnd::g_nAcceleratedIncreaseStep );
	m_editIncreaseStep.SetWindowText( LPCTSTR(strTmp) );
	strTmp.Format( _T("%d"), CExtPopupMenuWnd::g_nAcceleratedPixelsPerStepMax );
	m_editMaxPixelsPerStep.SetWindowText( LPCTSTR(strTmp) );

	m_MenuAnimation.SetCurSel( nInitSel );

	m_bInitComplete = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPagePopupMenus::OnSelendokMenuAnimation() 
{
	int nCurSel = m_MenuAnimation.GetCurSel();
	CExtPopupMenuWnd::g_DefAnimationType =
		(CExtPopupMenuWnd::e_animation_type_t)m_MenuAnimation.GetItemData( nCurSel );
}

void CPagePopupMenus::OnCheckMenuAnimation() 
{
	CExtPopupMenuWnd::g_bMenuExpandAnimation =
		m_chkMenuAnimation.GetCheck() ?
		true : false;
}

void CPagePopupMenus::OnCheckMenuWithShadows() 
{
	CExtPopupMenuWnd::g_bMenuWithShadows =
		m_chkMenuWithShadows.GetCheck() ?
		true : false;
}

void CPagePopupMenus::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	pWnd;
	
	CMenu _menu;
	if( _menu.GetSafeHmenu() == NULL )
	{
		if( !_menu.LoadMenu(IDR_POPUP_MENU) )
		{
			ASSERT( FALSE );
			return;
		}
	}
CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
	pPopup->SetLeftAreaWidth( 30 );
	pPopup->UpdateFromMenu(
		GetSafeHwnd(),
		&_menu
		);

// MAKING "About" menu item default
INT nDefaultItemPos =
		pPopup->ItemFindPosForCmdID(ID_APP_ABOUT);
	if( nDefaultItemPos >= 0 )
		pPopup->ItemDefaultSet(nDefaultItemPos);

//							// dynamic example 1: make the About command checked
//						CExtPopupMenuWnd::MENUITEMDATA & _miiAbout = pPopup->ItemGetInfo( nDefaultItemPos );
//							_miiAbout.SetNoCmdUI( true );
//							_miiAbout.Check( true );
//
//							// dynamic example 2: new check box item at the top
//						static bool g_bCheckedDynamicMenuItem = true;
//							pPopup->ItemInsertCommand( 4000, 0, _T("The dynamic check box menu item."), NULL, NULL, false, g_bCheckedDynamicMenuItem ? 1 : 0 );
//							g_bCheckedDynamicMenuItem = ! g_bCheckedDynamicMenuItem;

INT nReplacePos =
		pPopup->ItemFindPosForCmdID( ID_COLOR_CTRL_8X5 );
	if( nReplacePos >= 0 )
	{
		CExtPopupColorMenuWnd * pColorPopup = new CExtPopupColorMenuWnd;
		pColorPopup->m_lParamCookie = LPARAM( ID_COLOR_CTRL_8X5 );
		pColorPopup->m_hWndNotifyColorChanged = GetSafeHwnd();
		pColorPopup->m_clrDefault = COLORREF(-1);
		//pColorPopup->m_clrInitial =	;
		VERIFY(
			pPopup->ItemInsertSpecPopup(
				pColorPopup,
				nReplacePos + 1,
				pPopup->ItemGetText(nReplacePos),
				pPopup->ItemGetIcon(nReplacePos)
				)
			);
		pPopup->ItemSetDisplayed( nReplacePos + 1, true );
		VERIFY( pPopup->ItemRemove(nReplacePos) );
//		// set check mark on popup item
//		pPopup->ItemGetInfo(nReplacePos).Check( true );
	} // if( nReplacePos >= 0 )
	nReplacePos =
		pPopup->ItemFindPosForCmdID( ID_COLOR_CTRL_8X2 );
	if( nReplacePos >= 0 )
	{
		CExtPopupColorMenuWnd * pColorPopup = new CExtPopupColorMenuWnd;
		pColorPopup->m_lParamCookie = LPARAM( ID_COLOR_CTRL_8X2 );
		pColorPopup->m_hWndNotifyColorChanged = GetSafeHwnd();
		pColorPopup->m_clrDefault = COLORREF(-1);
		pColorPopup->SetColors8x2();
		VERIFY(
			pPopup->ItemInsertSpecPopup(
				pColorPopup,
				nReplacePos + 1,
				pPopup->ItemGetText(nReplacePos),
				pPopup->ItemGetIcon(nReplacePos)
				)
			);
		pPopup->ItemSetDisplayed( nReplacePos + 1, true );
		VERIFY( pPopup->ItemRemove(nReplacePos) );
	} // if( nReplacePos >= 0 )
	nReplacePos =
		pPopup->ItemFindPosForCmdID( ID_COLOR_CTRL_GRAYSCALE );
	if( nReplacePos >= 0 )
	{
		CExtPopupColorMenuWnd * pColorPopup = new CExtPopupColorMenuWnd;
		pColorPopup->m_lParamCookie = LPARAM( ID_COLOR_CTRL_GRAYSCALE );
		pColorPopup->m_hWndNotifyColorChanged = GetSafeHwnd();
		pColorPopup->m_clrDefault = COLORREF(-1);
		pColorPopup->RemoveAllColors();
		int nGrayColors = 16;
		pColorPopup->SetColorsInRow( nGrayColors );
		pColorPopup->m_sizeColorItemCell.cy *= 2;
		for( int nColorIndex = 0; nColorIndex < nGrayColors; nColorIndex++ )
		{
			double f = double( nColorIndex ) / double( nGrayColors - 1 );
			COLORREF clr = CExtBitmap::stat_HLStoRGB( 0.0, f, 0.0 );
			int n = GetRValue(clr);
			CExtSafeString str;
			str.Format(
				_T("#%02X%02X%02X"),
				n,
				n,
				n
				);
			pColorPopup->AddColor(
				new CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY(
					0,
					clr,
					str
					)
				);
		}
		VERIFY(
			pPopup->ItemInsertSpecPopup(
				pColorPopup,
				nReplacePos + 1,
				pPopup->ItemGetText(nReplacePos),
				pPopup->ItemGetIcon(nReplacePos)
				)
			);
		pPopup->ItemSetDisplayed( nReplacePos + 1, true );
		VERIFY( pPopup->ItemRemove(nReplacePos) );
	} // if( nReplacePos >= 0 )
	nReplacePos =
		pPopup->ItemFindPosForCmdID( ID_COLOR_CTRL_HLS );
	if( nReplacePos >= 0 )
	{
		CExtPopupColorMenuWnd * pColorPopup = new CExtPopupColorMenuWnd;
		pColorPopup->m_lParamCookie = LPARAM( ID_COLOR_CTRL_HLS );
		pColorPopup->m_hWndNotifyColorChanged = GetSafeHwnd();
		pColorPopup->m_clrDefault = COLORREF(-1);
		pColorPopup->RemoveAllColors();
		int nStepsHue = 12;
		int nStepsSat = 8;
		pColorPopup->m_sizeColorItemCell.cx = 14;
		pColorPopup->m_sizeColorItemCell.cy = 12;
		pColorPopup->m_sizeColorItemSpaces.cx = 1;
		pColorPopup->m_sizeColorItemSpaces.cy = 1;
		pColorPopup->SetColorsInRow( nStepsHue );
		for( int nSat = 0; nSat < nStepsSat; nSat++ )
		{
			double fSat = 1.0 - double( nSat ) / double( nStepsSat );
			for( int nHue = 0; nHue < nStepsHue; nHue++ )
			{
				double fHue = double( nHue ) / double( nStepsHue );
				COLORREF clr = CExtBitmap::stat_HLStoRGB( fHue, 0.5, fSat );
				CExtSafeString str;
				str.Format(
					_T("#%02X%02X%02X"),
					GetRValue(clr),
					GetGValue(clr),
					GetBValue(clr)
					);
				pColorPopup->AddColor(
					new CExtPopupColorMenuWnd::COLORREF_TABLE_ENTRY(
						0,
						clr,
						str
						)
					);
			}
		}
		VERIFY(
			pPopup->ItemInsertSpecPopup(
				pColorPopup,
				nReplacePos + 1,
				pPopup->ItemGetText(nReplacePos),
				pPopup->ItemGetIcon(nReplacePos)
				)
			);
		pPopup->ItemSetDisplayed( nReplacePos + 1, true );
		VERIFY( pPopup->ItemRemove(nReplacePos) );
	} // if( nReplacePos >= 0 )
	
#if (!defined __EXT_MFC_NO_DATE_PICKER)
	nReplacePos =
		pPopup->ItemFindPosForCmdID(ID_DATE_PICKER_CTRL);
	
	if( nReplacePos >= 0 )
	{
		CExtPopupControlMenuWnd::g_bControlMenuWithShadows = true;
		CExtPopupDatePickerMenuWnd * pDatePickerPopup = 
			new CExtPopupDatePickerMenuWnd(
			0L,
			CSize(1,1),
			WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
			__EDPWS_DEFAULT|__EDPWS_MULTIPLE_SELECTION
			);

		VERIFY(
			pPopup->ItemInsertSpecPopup(
			pDatePickerPopup,
			nReplacePos + 1,
			pPopup->ItemGetText(nReplacePos),
			pPopup->ItemGetIcon(nReplacePos)
			)
			);
		pPopup->ItemSetDisplayed( nReplacePos + 1, true );
		VERIFY( pPopup->ItemRemove(nReplacePos) );
	} // if( nReplacePos >= 0 )
#endif // #if (!defined __EXT_MFC_NO_DATE_PICKER)	

	if( ! pPopup->ItemInsert(
			UINT(CExtPopupMenuWnd::TYPE_POPUP),
			-1,
			_T("Long sub-menu")
			)
		)
	{
		ASSERT( FALSE );
		delete pPopup;
	}
CExtPopupMenuWnd * pLongSubMenu =
		pPopup->ItemGetPopup(
			pPopup->ItemGetCount() - 1
			);
	ASSERT_VALID( pLongSubMenu );
CString strCountOfItems;
	m_editCountOfItems.GetWindowText( strCountOfItems );
INT nIongIndex, nLongCount;
	nLongCount = INT( _ttol( LPCTSTR( strCountOfItems ) ) );
	if( nLongCount < 0 )
	{
		nLongCount = 0;
		strCountOfItems.Format( _T("%d"), nLongCount );
		m_editCountOfItems.SetWindowText( LPCTSTR(strCountOfItems) );
	} // if( nLongCount < 0 )
	else if( nLongCount > 400 )
	{
		nLongCount = 400;
		strCountOfItems.Format( _T("%d"), nLongCount );
		m_editCountOfItems.SetWindowText( LPCTSTR(strCountOfItems) );
	} // else if( nLongCount > 400 )
	for( nIongIndex = 0; nIongIndex < nLongCount; nIongIndex++ )
	{
		UINT nCmdID = UINT(nIongIndex)+UINT(14*1000);
		CString strLongCommandName;
		strLongCommandName.Format(
			_T("Item %d"),
			nIongIndex + 1
			);
		pLongSubMenu->ItemInsertCommand(
			nCmdID,
			-1,
			LPCTSTR(strLongCommandName)
			);
	} // for( nIongIndex = 0; nIongIndex < nLongCount; nIongIndex++ )
	if( ! pPopup->TrackPopupMenu( TPMX_OWNERDRAW_FIXED, point.x, point.y ) )
		delete pPopup;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

LRESULT CPagePopupMenus::OnDrawPopupLeftArea(WPARAM wParam, LPARAM lParam)
{
	wParam;
	CExtPopupMenuWnd::DRAWLEFTAREADATA * pDrawLeftAreaData =
		reinterpret_cast < CExtPopupMenuWnd::DRAWLEFTAREADATA * > ( lParam );
	ASSERT( pDrawLeftAreaData != NULL );
	ASSERT( pDrawLeftAreaData->m_pPopup != NULL );
	if( (pDrawLeftAreaData->m_pPopup->TrackFlagsGet()&TPMX_PALETTE) != 0 )
		return 0;
	
	// get draw DC
	CDC & dc = *( (CDC *) *pDrawLeftAreaData );
	
	CRect rcItem = LPCRECT(*pDrawLeftAreaData);
	CExtPaintManager::stat_PaintGradientRect(
		dc,
		&rcItem,
		dc.GetNearestColor( RGB( 0, 0, 0 ) ),
		dc.GetNearestColor( RGB( 100, 100, 255 ) ),
		true
		);
	
	LOGFONT lf;
	::memset(&lf,0,sizeof(LOGFONT));
	g_PaintManager->m_FontNormal.GetLogFont( &lf );
	lf.lfHeight = -18;
	lf.lfWidth = 0;
	lf.lfWeight = 900;
	lf.lfEscapement = 900;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("Times New Roman") );
	CFont font;
	VERIFY(
		font.CreateFontIndirect(&lf)
		);
	CFont * pOldFont = dc.SelectObject( &font );
	INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
	COLORREF clrOldText = dc.SetTextColor( RGB( 0, 0, 0 ) );
	
	static CString sTitle( _T("Prof-UIS") );
	CPoint ptText( rcItem.left+4, rcItem.bottom-5 );
	dc.DrawState(
		ptText, rcItem.Size(), (LPCTSTR)sTitle,
		DSS_NORMAL,  TRUE, 0, (CBrush*)NULL
		);
	dc.SetTextColor( RGB( 255, 255, 255 ) );
	ptText.Offset( -1, -2 );
	dc.DrawState(
		ptText, rcItem.Size(), (LPCTSTR)sTitle,
		DSS_NORMAL,  TRUE, 0, (CBrush*)NULL
		);
	
	const int nIconMetric = 24;
	HICON hIcon = (HICON)
		::LoadImage(
		::AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICON_FOR_POPUP_MENU),
		IMAGE_ICON,
		nIconMetric,
		nIconMetric,
		0
		);
	if( hIcon != NULL )
	{
		int nOffset = (rcItem.Width() - nIconMetric) / 2;
		VERIFY(
			::DrawIconEx(
			dc.GetSafeHdc(),
			rcItem.left + nOffset,
			rcItem.top + nOffset,
			hIcon,
			nIconMetric,
			nIconMetric,
			0,
			(HBRUSH)NULL,
			DI_NORMAL
			)
			);
		VERIFY( DestroyIcon( hIcon ) );
	}
	
	dc.SetBkMode( nOldBkMode );
	dc.SetTextColor( clrOldText );
	dc.SelectObject( pOldFont );
	
	return (!0);
}

//////////////////////////////////////////////////////////////////////////

LRESULT CPagePopupMenus::OnExtMenuPrepare(WPARAM wParam, LPARAM lParam)
{
	lParam;
CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
		reinterpret_cast
			< CExtPopupMenuWnd::MsgPrepareMenuData_t * >
			( wParam );
	ASSERT( pData != NULL );
CExtPopupMenuWnd * pPopup = pData->m_pPopup;
	ASSERT( pPopup != NULL );
	pData->m_bMenuChanged = true;

INT nReplacePos =
		pPopup->ItemFindPosForCmdID( ID_PALETTE_MENU );
	if( nReplacePos >= 0 )
	{
		pPopup->ItemRemove( nReplacePos );
		CExtPopupMenuWnd * pPalettePopup = new CExtPopupMenuWnd;
		pPopup->ItemInsertSpecPopup(
			pPalettePopup,
			nReplacePos,
			g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd( m_hWnd ),
				ID_PALETTE_MENU
				)->m_sMenuText
			);
		pPalettePopup->TrackFlagsSet(
			pPalettePopup->TrackFlagsGet()
			| TPMX_PALETTE
			);
		if( m_chkPaletteWithCustomDrawnBackground.GetCheck() != 0 )
			pPalettePopup->SetLeftAreaWidth( 10 );

		pPalettePopup->ItemInsertCommand(
			1,
			-1,
			_T("Palette 6 x 2:")
			);
		INT nLastPos = pPalettePopup->ItemGetCount() - 1;
		pPalettePopup->ItemEnabledSet( nLastPos, false );
		pPalettePopup->ItemEnabledAppearanceSet( nLastPos );
		pPalettePopup->ItemBoldSet( nLastPos );
		pPalettePopup->ItemPaletteRowWrapSet( nLastPos );

		CExtBitmap _bmp;
		VERIFY( _bmp.LoadBMP_Resource( MAKEINTRESOURCE(IDB_PAINT_PALETTE) ) );
		INT nRow, nColumn;
		static const INT nRowCountPaint = 2, nColumnCountPaint = 6;
		static const COLORREF clrTransparentPaint = RGB(255,0,255);
		static const CSize _sizeBitmapPartPaint(16,16);
		for( nRow = 0; nRow < nRowCountPaint; nRow ++ )
		{
			for( nColumn = 0; nColumn < nColumnCountPaint; nColumn ++ )
			{
				CPoint ptBitmapOffset(
					nRow * nColumnCountPaint * _sizeBitmapPartPaint.cx
						+ nColumn * _sizeBitmapPartPaint.cx,
					0
					);
				CRect rcBitmapPart(
					ptBitmapOffset,
					_sizeBitmapPartPaint
					);
				CExtCmdIcon _icon;
				_icon.m_bmpNormal.FromBitmap(
					_bmp,
					&rcBitmapPart
					);
				ASSERT( ! _icon.IsEmpty() );
				_icon.m_bmpNormal.AlphaColor( clrTransparentPaint, RGB(0,0,0), 0 );
				pPalettePopup->ItemInsertCommand(
					3 + nRow*nColumnCountPaint + nColumn,
					-1,
					NULL,
					NULL,
					_icon
					);
				if( nColumn == (nColumnCountPaint-1) )
					pPalettePopup->ItemPaletteRowWrapSet(
						pPalettePopup->ItemGetCount() - 1
						);
			} // for( nColumn = 0; nColumn < nColumnCountPaint; nColumn ++ )
		} // for( nRow = 0; nRow < nRowCountPaint; nRow ++ )

		pPalettePopup->ItemInsert();
		pPalettePopup->ItemInsertCommand(
			2,
			-1,
			_T("Palette 6 x 7:")
			);
		nLastPos = pPalettePopup->ItemGetCount() - 1;
		pPalettePopup->ItemEnabledSet( nLastPos, false );
		pPalettePopup->ItemEnabledAppearanceSet( nLastPos );
		pPalettePopup->ItemBoldSet( nLastPos );
		pPalettePopup->ItemPaletteRowWrapSet( nLastPos );

		VERIFY( _bmp.LoadBMP_Resource( MAKEINTRESOURCE(IDB_EXT_CUSTOM_TB_IMAGES) ) );
		static const INT nRowCountCustom = 7, nColumnCountCustom = 6;
		static const COLORREF clrTransparentCustom = RGB(255,0,255);
		static const CSize _sizeBitmapPartCustom(16,16);
		for( nRow = 0; nRow < nRowCountCustom; nRow ++ )
		{
			for( nColumn = 0; nColumn < nColumnCountCustom; nColumn ++ )
			{
				CPoint ptBitmapOffset(
					nRow * nColumnCountCustom * _sizeBitmapPartCustom.cx
						+ nColumn * _sizeBitmapPartCustom.cx,
					0
					);
				CRect rcBitmapPart(
					ptBitmapOffset,
					_sizeBitmapPartCustom
					);
				CExtCmdIcon _icon;
				_icon.m_bmpNormal.FromBitmap(
					_bmp,
					&rcBitmapPart
					);
				ASSERT( ! _icon.IsEmpty() );
				_icon.m_bmpNormal.AlphaColor( clrTransparentCustom, RGB(0,0,0), 0 );
				pPalettePopup->ItemInsertCommand(
					3 + nRow*nColumnCountCustom + nColumn,
					-1,
					NULL,
					NULL,
					_icon
					);
				if( nColumn == (nColumnCountCustom-1) )
					pPalettePopup->ItemPaletteRowWrapSet(
						pPalettePopup->ItemGetCount() - 1
						);
			} // for( nColumn = 0; nColumn < nColumnCountCustom; nColumn ++ )
		} // for( nRow = 0; nRow < nRowCountCustom; nRow ++ )
	} // if( nReplacePos >= 0 )

	return TRUE;
}

LRESULT CPagePopupMenus::OnExtMenuDrawBackground(WPARAM wParam, LPARAM lParam)
{
	wParam;
CExtPopupMenuWnd::DRAWBACKGROUNDDATA * pData =
		reinterpret_cast
			< CExtPopupMenuWnd::DRAWBACKGROUNDDATA * >
			( lParam );
	ASSERT( pData != NULL );
CExtPopupMenuWnd * pPopup = pData->m_pPopup;
	ASSERT( pPopup != NULL );
	if( (pPopup->TrackFlagsGet()&TPMX_PALETTE) == 0 )
		return 0;
	if( m_chkPaletteWithCustomDrawnBackground.GetCheck() == 0 )
		return 0;

	// tell pop-up menu to not paint default menu background
	pData->m_bCustomBackgroundDrawn = true;

	// now we can render more funny background
CRect rcBackground( pData->m_rcBackground );
CRect rcMB = pData->m_pPopup->OnQueryMenuBorderMetrics();
	rcBackground.DeflateRect(
		rcMB.left,
		rcMB.top,
		rcMB.right,
		rcMB.bottom
		);
COLORREF clrLight =
		g_PaintManager->GetColor( COLOR_3DHILIGHT );
	pData->m_DC.FillSolidRect(
		&rcBackground,
		clrLight
		);
	if( ::GetDeviceCaps( pData->m_DC.m_hDC, BITSPIXEL ) <= 8 )
		return 0;
COLORREF clrDark =
		g_PaintManager->GetColor( COLOR_3DFACE );
COLORREF clrDarker =
		g_PaintManager->GetColor( COLOR_3DDKSHADOW );
int nItemIndex, nItemCount = pPopup->ItemGetCount();
bool bNewRow = true, bFirstRow = true, bCaptionRow = false;
CRect rcRow( rcBackground );
	for( nItemIndex = 0; nItemIndex < nItemCount; nItemIndex ++ )
	{
		CRect rcItem;
		pPopup->_GetItemRect( nItemIndex, rcItem, true );
		if( pPopup->ItemBoldGet( nItemIndex ) )
			bCaptionRow = true;
		if( bNewRow )
		{
			bNewRow	= false;
			rcRow.top = rcItem.top;
			rcRow.bottom = rcItem.bottom;
		}
		else
		{
			rcRow.top = min( rcRow.top, rcItem.top );
			rcRow.bottom = max( rcRow.bottom, rcItem.bottom );
		}
		if(		nItemIndex == (nItemCount - 1)
			||	pPopup->ItemPaletteRowWrapGet( nItemIndex )
			)
		{
			if( bFirstRow )
				rcRow.top = rcBackground.top;
			if( nItemIndex == (nItemCount - 1) )
				rcRow.bottom = rcBackground.bottom;
			if( ! bCaptionRow )
			{
				CExtPaintManager::stat_PaintGradientRect(
					pData->m_DC,
					&rcRow,
					clrLight,
					clrDark,
					true
					);
				CRect rcTopLine(
					rcRow.left,
					rcRow.top,
					rcRow.right,
					rcRow.top + 1
					);
				static const SIZE g_sizeCurl = { 9, 9 };
				CRect rcRowCurl( rcRow.TopLeft(), g_sizeCurl );
				POINT points[3];
				points[0].x = rcRowCurl.left;
				points[0].y = rcRowCurl.bottom;
				points[1].x = rcRowCurl.right;
				points[1].y = rcRowCurl.top;
				points[2].x = rcRowCurl.right;
				points[2].y = rcRowCurl.bottom;
				CRgn _rgn;
				if( _rgn.CreatePolygonRgn(
						points,
						3,
						ALTERNATE
						)
					)
				{
					pData->m_DC.FillSolidRect(
						&rcRowCurl,
						clrLight
						);
					pData->m_DC.SelectClipRgn( &_rgn );
					CExtPaintManager::stat_PaintGradientRect(
						pData->m_DC,
						&rcRowCurl,
						clrDarker,
						clrDark,
						true
						);
					pData->m_DC.SelectClipRgn( NULL );
					rcTopLine.left = rcRowCurl.right;
				}
				CExtPaintManager::stat_PaintGradientRect(
					pData->m_DC,
					&rcTopLine,
					clrDark,
					clrDarker,
					false
					);
			}
			bNewRow	= true;
			bFirstRow = false;
			bCaptionRow = false;
		}
	}
	return 0;
}

LRESULT CPagePopupMenus::OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam)
{
	wParam;
CExtPopupMenuWnd::DRAWITEMDATA * pDrawItemData = reinterpret_cast < CExtPopupMenuWnd::DRAWITEMDATA * > ( lParam );
	ASSERT( pDrawItemData != NULL );
UINT nCmdID = pDrawItemData->GetCmdID();
	if( nCmdID != ID_OWNERDRAW_ITEM )
		return 0; // default painting
	
	// paint default menu item background
	pDrawItemData->PaintDefaultBk();
	
	// paint some staff in a little bit rect
	CRect rcItem = LPCRECT(*pDrawItemData);
	rcItem.DeflateRect( 10, 1, 10, 4 );
	
LPCTSTR sItemText = __EXT_MFC_SAFE_LPCTSTR( *pDrawItemData );
INT nItemTextLen = INT( _tcslen( sItemText ) );
	
	// get draw DC
	CDC & dc = *( (CDC *) *pDrawItemData );
	
	CExtWndShadow _shadow;
	_shadow.Paint(
		NULL,
		dc, rcItem, CRect(0,0,0,0), CRect(0,0,0,0),
		3,
		CExtWndShadow::DEF_BRIGHTNESS_MIN,
		CExtWndShadow::DEF_BRIGHTNESS_MAX,
		false
		);
	CExtPaintManager::stat_PaintGradientRect(
		dc,
		&rcItem,
		dc.GetNearestColor( RGB( 255, 0, 0 ) ),
		dc.GetNearestColor( RGB( 0, 0, 255 ) ),
		false
		);
	
	INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
	COLORREF clrOldText = dc.SetTextColor( RGB( 0, 0, 0 ) );
	CFont * pOldFont = dc.SelectObject( &g_PaintManager->m_FontBold );
	
	rcItem.OffsetRect( 2, 1 );
	CExtRichContentLayout::stat_DrawText(
		dc.m_hDC,
		sItemText, nItemTextLen, &rcItem, 
		DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE | DT_CENTER, 0
		);
	clrOldText = dc.SetTextColor( RGB( 255, 255, 255 ) );
	rcItem.OffsetRect( -2, -1 );
	CExtRichContentLayout::stat_DrawText(
		dc.m_hDC,
		sItemText, nItemTextLen, &rcItem, 
		DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS, 0
		);
	
	dc.SelectObject( pOldFont );
	dc.SetBkMode( nOldBkMode );
	dc.SetTextColor( clrOldText );

	return !0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CPagePopupMenus::OnMsgSelectionNotification( WPARAM wParam, LPARAM lParam )
{
	wParam;
	lParam;
#if (!defined __EXT_MFC_NO_DATE_PICKER)
	const CExtDatePickerWnd::SELECTION_NOTIFICATION * pSN =
		CExtDatePickerWnd::SELECTION_NOTIFICATION::FromWPARAM( wParam );

	if( !pSN->m_bFinalSelectionChanging )
		return 0L;
	
	CString	strMsg, strMsgAll,
		strDtBegin( pSN->m_dtBegin.Format() ),
		strDtEnd( pSN->m_dtEnd.Format() );
	strMsg.Format(
		_T("The date selected: %s"),
		strDtBegin
		);
	if( pSN->m_dtBegin != pSN->m_dtEnd ){
		strMsg += _T(" - ");
		strMsg += strDtEnd;
	}

	strMsg += _T("\r\n");
	TRACE( strMsg );
#endif // #if (!defined __EXT_MFC_NO_DATE_PICKER)	
	
	return 0L;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CPagePopupMenus::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
	switch( lParam )
	{
	case ID_COLOR_CTRL_8X5:
	case ID_COLOR_CTRL_8X2:
	case ID_COLOR_CTRL_GRAYSCALE:
	case ID_COLOR_CTRL_HLS:
		{
			COLORREF clrColorCtrl = COLORREF(wParam);
			if( clrColorCtrl == COLORREF(-1) )
				AfxMessageBox(_T("Default Color"));
			else
			{
				CString sColor;
				sColor.Format(
					_T( "RGB color is 0x%02X%02X%02X" ),
					INT( GetRValue(clrColorCtrl) ),
					INT( GetGValue(clrColorCtrl) ),
					INT( GetBValue(clrColorCtrl) )
					);
				AfxMessageBox( sColor );
			}
		}
		break;
	default:
		break;
	} // switch( lParam )
	return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CPagePopupMenus::OnColorSelectCustom(WPARAM wParam, LPARAM lParam)
{
	wParam;
	if(		lParam == ID_COLOR_CTRL_8X5
		||	lParam == ID_COLOR_CTRL_8X2
		||	lParam == ID_COLOR_CTRL_GRAYSCALE
		||	lParam == ID_COLOR_CTRL_HLS
		)
	{
		COLORREF clrNew;
		CExtColorDlg dlgColor;
		dlgColor.m_strSection = _T("ColorDialog");
		dlgColor.m_strEntryDialogPosition = _T("Position");
		dlgColor.m_strEntryColorModelSelection = _T("ColorModel");
		if( dlgColor.DoModal() != IDOK )
			return 0;
		clrNew = dlgColor.m_clrNew;
		OnColorChangedFinally( (WPARAM)clrNew, lParam );
	}
	return 0;
}


void CPagePopupMenus::OnCheckUseAcceleratedMenuScrolling() 
{
	CExtPopupMenuWnd::g_bUseAcceleratedMenuScrolling =
		m_checkUseAcceleratedMenuScrolling.GetCheck()
			? true : false;
BOOL bEnable =
		CExtPopupMenuWnd::g_bUseAcceleratedMenuScrolling
			? TRUE : FALSE;
static const UINT arrIDs[] =
{
	IDC_STATIC_INCREASE_PERCENT,
	IDC_EDIT_INCREASE_PERCENT,
	IDC_SPIN_INCREASE_PERCENT,
	IDC_STATIC_INCREASE_STEP,
	IDC_EDIT_INCREASE_STEP,
	IDC_SPIN_INCREASE_STEP,
	IDC_STATIC_MAX_PIXELS_PER_STEP,
	IDC_EDIT_MAX_PIXELS_PER_STEP,
	IDC_SPIN_MAX_PIXELS_PER_STEP,
};
INT nIndex, nCount = sizeof(arrIDs)/sizeof(arrIDs[0]);
	for( nIndex = 0; nIndex < nCount; nIndex++ )
	{
		UINT nID = arrIDs[nIndex];
		CWnd * pWnd = GetDlgItem( nID );
		ASSERT_VALID( pWnd );
		pWnd->EnableWindow( bEnable );
	} // for( nIndex = 0; nIndex < nCount; nIndex++ )
}

void CPagePopupMenus::OnChangeEditIncreasePercent() 
{
	if( m_editIncreasePercent.GetSafeHwnd() == NULL )
		return;
CString strTmp;
	m_editIncreasePercent.GetWindowText( strTmp );
}

void CPagePopupMenus::OnChangeEditIncreaseStep() 
{
	if( m_editIncreaseStep.GetSafeHwnd() == NULL )
		return;
CString strTmp;
	m_editIncreaseStep.GetWindowText( strTmp );
}

void CPagePopupMenus::OnChangeEditMaxPixelsPerStep() 
{
	if( m_editMaxPixelsPerStep.GetSafeHwnd() == NULL )
		return;
CString strTmp;
	m_editMaxPixelsPerStep.GetWindowText( strTmp );
}

void CPagePopupMenus::OnMenuRarelyItem1()
{
	::AfxMessageBox( _T("CPagePopupMenus::OnMenuRarelyItem1()") );
}

void CPagePopupMenus::OnUpdateMenuRarelyItem1( CCmdUI * pCmdUI )
{
	pCmdUI->Enable();
}

void CPagePopupMenus::OnMenuRarelyItem2()
{
	::AfxMessageBox( _T("CPagePopupMenus::OnMenuRarelyItem2()") );
}

void CPagePopupMenus::OnUpdateMenuRarelyItem2( CCmdUI * pCmdUI )
{
	pCmdUI->Enable();
}

void CPagePopupMenus::OnMenuRarelyItem3()
{
	::AfxMessageBox( _T("CPagePopupMenus::OnMenuRarelyItem3()") );
}

void CPagePopupMenus::OnUpdateMenuRarelyItem3( CCmdUI * pCmdUI )
{
	pCmdUI->Enable();
}

//			LRESULT CPagePopupMenus::OnMsgTranslateMouseClickEvent(WPARAM wParam, LPARAM lParam)
//			{
//				lParam;
//			CExtPopupBaseWnd::TranslateMouseClickEventData_t * pData =
//					reinterpret_cast< CExtPopupBaseWnd::TranslateMouseClickEventData_t  * >
//					( wParam );
//			CExtPopupMenuWnd * pPopupMenuWnd = (CExtPopupMenuWnd*)pData->m_pEventSourcePopup;
//				// the pData->m_point point depends on WM_*** or WM_NC*** message type
//				// we will get actual cursor position instead
//			CPoint ptHitTest;
//				if( ! ::GetCursorPos( &ptHitTest ) )
//					return 0L;
//				pPopupMenuWnd->ScreenToClient( &ptHitTest );
//			int nIndex = pPopupMenuWnd->_HitTest( ptHitTest );
//				if( nIndex >= 0 )
//				{
//					CExtPopupMenuWnd::MENUITEMDATA & mi = pPopupMenuWnd->ItemGetInfo( nIndex );
//					if( mi.IsPopup() )
//					{
//						// close displayed sub menu, un-focus menu item
//						pPopupMenuWnd->_ItemFocusCancel( FALSE, TRUE, FALSE );
//						// restore menu item focus, do not show next level submenu
//						pPopupMenuWnd->_ItemFocusSet( nIndex, FALSE, TRUE, FALSE, FALSE );
//						// move captured state into clicked popup menu
//						pPopupMenuWnd->_SetCapture();
//						pData->m_bMessageHandled = true; // we handled this message
//						pData->m_bNoEat = false; // this message should be filtered by thread hook of popup menu
//					}
//				}
//				return 0L;
//			}