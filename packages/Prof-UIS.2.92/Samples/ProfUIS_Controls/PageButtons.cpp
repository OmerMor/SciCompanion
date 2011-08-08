// PageButtons.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageButtons.h"
#include "MainDlg.h"
#include <Resources/Resource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_HAND MAKEINTRESOURCE(32649)

/////////////////////////////////////////////////////////////////////////////
// CPageButtons dialog


CPageButtons::CPageButtons(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageButtons::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageButtons)
	//}}AFX_DATA_INIT
}


void CPageButtons::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageButtons)
	DDX_Control(pDX, IDC_TEXT_MARGINS_RESET, m_wndTextMarginsReset);
	DDX_Control(pDX, IDC_ICON_MARGINS_RESET, m_wndIconMarginsReset);
	DDX_Control(pDX, IDC_SPIN_ICON_MARGINS_TOP, m_wndSpinIconMarginsTop);
	DDX_Control(pDX, IDC_SPIN_ICON_MARGINS_RIGHT, m_wndSpinIconMarginsRight);
	DDX_Control(pDX, IDC_SPIN_ICON_MARGINS_LEFT, m_wndSpinIconMarginsLeft);
	DDX_Control(pDX, IDC_SPIN_ICON_MARGINS_BOTTOM, m_wndSpinIconMarginsBottom);
	DDX_Control(pDX, IDC_SPIN_TEXT_MARGINS_TOP, m_wndSpinTextMarginsTop);
	DDX_Control(pDX, IDC_SPIN_TEXT_MARGINS_RIGHT, m_wndSpinTextMarginsRight);
	DDX_Control(pDX, IDC_SPIN_TEXT_MARGINS_LEFT, m_wndSpinTextMarginsLeft);
	DDX_Control(pDX, IDC_SPIN_TEXT_MARGINS_BOTTOM, m_wndSpinTextMarginsBottom);
	DDX_Control(pDX, IDC_TEXT_MARGINS_TOP, m_wndTextMarginsTop);
	DDX_Control(pDX, IDC_TEXT_MARGINS_RIGHT, m_wndTextMarginsRight);
	DDX_Control(pDX, IDC_TEXT_MARGINS_LEFT, m_wndTextMarginsLeft);
	DDX_Control(pDX, IDC_TEXT_MARGINS_BOTTOM, m_wndTextMarginsBottom);
	DDX_Control(pDX, IDC_ICON_MARGINS_TOP, m_wndIconMarginsTop);
	DDX_Control(pDX, IDC_ICON_MARGINS_RIGHT, m_wndIconMarginsRight);
	DDX_Control(pDX, IDC_ICON_MARGINS_LEFT, m_wndIconMarginsLeft);
	DDX_Control(pDX, IDC_ICON_MARGINS_BOTTOM, m_wndIconMarginsBottom);
	DDX_Control(pDX, IDC_GROUP_BOX_TEXT_MARGINS, m_wndGroupBoxTextMargins);
	DDX_Control(pDX, IDC_GROUP_BOX_ICON_MARGINS, m_wndGroupBoxIconMargins);
	DDX_Control(pDX, IDC_STATIC_ICON_ALIGNMENT_VERT, m_wndlabelIconAlignmentVert);
	DDX_Control(pDX, IDC_STATIC_ICON_ALIGNMENT_HORZ, m_wndlabelIconAlignmentHorz);
	DDX_Control(pDX, IDC_STATIC_TEXT_ALIGNMENT_VERT, m_wndlabelTextAlignmentVert);
	DDX_Control(pDX, IDC_STATIC_TEXT_ALIGNMENT_HORZ, m_wndlabelTextAlignmentHorz);
	DDX_Control(pDX, IDC_TEXT_ALIGNMENT_VERT, m_wndTextAlignmentVert);
	DDX_Control(pDX, IDC_TEXT_ALIGNMENT_HORZ, m_wndTextAlignmentHorz);
	DDX_Control(pDX, IDC_ICON_ALIGNMENT_VERT, m_wndIconAlignmentVert);
	DDX_Control(pDX, IDC_ICON_ALIGNMENT_HORZ, m_wndIconAlignmentHorz);
	DDX_Control(pDX, IDC_GROUP_BOX_TEXT_ALIGNMENT, m_wndGroupBoxTextAlignment);
	DDX_Control(pDX, IDC_GROUP_BOX_ICON_ALIGNMENT, m_wndGroupBoxIconAlignment);
	DDX_Control(pDX, IDC_RADIO1, m_wndRadio1);
	DDX_Control(pDX, IDC_RADIO2, m_wndRadio2);
	DDX_Control(pDX, IDC_CHECK_BOX2, m_chkCheckBox2);
	DDX_Control(pDX, IDC_CHECK_BOX1, m_chkCheckBox1);
	DDX_Control(pDX, IDC_BUTTON_WITH_MENU, m_btnWithMenu);
	DDX_Control(pDX, IDC_BUTTON_COLOR2, m_btnColor2);
	DDX_Control(pDX, IDC_BUTTON_COLOR1, m_btnColor1);
	DDX_Control(pDX, IDC_CHECK_BOX_SHOW_FOCUS_RECT, m_chkCheckBoxShowFocusRect);
	DDX_Control(pDX, IDC_TOOLTIP, m_Tooltip);
	DDX_Control(pDX, IDC_STATIC_TOOLTIP, m_wndLabelTooltip);
	DDX_Control(pDX, IDC_STATIC_SKIN, m_wndLabelSkin);
	DDX_Control(pDX, IDC_STATIC_CHECK_BOX, m_wndGroupBoxCheck);
	DDX_Control(pDX, IDC_STATIC_CURSOR, m_wndLabelCursor);
	DDX_Control(pDX, IDC_SKIN, m_CheckBoxSkin);
	DDX_Control(pDX, IDC_PUSH_LIKE_BUTTON, m_btnPushLikeButton);
	DDX_Control(pDX, IDC_PUSH_BUTTON, m_btnPushButton);
	DDX_Control(pDX, IDC_CURSOR, m_Cursor);
	DDX_Control(pDX, IDC_CHECK_TRISTATE, m_chkTriState);
	DDX_Control(pDX, IDC_CHECK_BUTTON_SHOW_FOCUS_RECT, m_chkButtonShowFocusRect);
	DDX_Control(pDX, IDC_CHECK_BUTTON_ENABLE, m_chkButtonEnable);
	DDX_Control(pDX, IDC_CHECK_BOX_ENABLE, m_chkCheckBoxEnable);
	DDX_Control(pDX, IDC_CHECK_LEFT_TEXT, m_chkLeftText);
	DDX_Control(pDX, IDC_CHECK_SEPARATED_BUTTONS, m_chkSeparatedButtons);
	DDX_Control(pDX, IDC_CHECK_FLAT, m_chkFlat);
	DDX_Control(pDX, IDC_CHECK_BORDER, m_chkBorders);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageButtons, CPageBase)
	//{{AFX_MSG_MAP(CPageButtons)
	ON_CBN_SELENDOK(IDC_SKIN, OnSelendokSkin)
	ON_CBN_SELENDOK(IDC_CURSOR, OnSelendokCursor)
	ON_CBN_SELENDOK(IDC_ICON_ALIGNMENT_HORZ, OnSelendokIconAlignmentHorz)
	ON_CBN_SELENDOK(IDC_ICON_ALIGNMENT_VERT, OnSelendokIconAlignmentVert)
	ON_CBN_SELENDOK(IDC_TEXT_ALIGNMENT_HORZ, OnSelendokTextAlignmentHorz)
	ON_CBN_SELENDOK(IDC_TEXT_ALIGNMENT_VERT, OnSelendokTextAlignmentVert)
	ON_BN_CLICKED(IDC_TEXT_MARGINS_RESET, OnTextMarginsReset)
	ON_BN_CLICKED(IDC_ICON_MARGINS_RESET, OnIconMarginsReset)
	//}}AFX_MSG_MAP
	// color popup menu
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyCustColor,
		OnColorSelectCustom
		)
	ON_REGISTERED_MESSAGE(CExtPopupMenuWnd::g_nMsgPrepareMenu,OnExtMenuPrepare)
	ON_REGISTERED_MESSAGE(CExtPopupMenuWnd::g_nMsgPopupDrawItem,OnDrawPopupMenuItem)
	ON_REGISTERED_MESSAGE(CExtPopupMenuWnd::g_nMsgPopupDrawLeftArea,OnDrawPopupLeftArea)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageButtons message handlers

BOOL CPageButtons::OnInitDialog() 
{
	CPageBase::OnInitDialog();

	m_wndTextMarginsReset.SetFlat( TRUE );
	m_wndIconMarginsReset.SetFlat( TRUE );
	m_wndTextMarginsReset.SetDrawBorder( TRUE );
	m_wndIconMarginsReset.SetDrawBorder( TRUE );

	m_wndSpinIconMarginsTop.SetRange(0,20);
	m_wndSpinIconMarginsRight.SetRange(0,20);
	m_wndSpinIconMarginsLeft.SetRange(0,20);
	m_wndSpinIconMarginsBottom.SetRange(0,20);
	m_wndSpinTextMarginsTop.SetRange(0,20);
	m_wndSpinTextMarginsRight.SetRange(0,20);
	m_wndSpinTextMarginsLeft.SetRange(0,20);
	m_wndSpinTextMarginsBottom.SetRange(0,20);

	SetDlgItemInt( IDC_TEXT_MARGINS_TOP, 3 );
	SetDlgItemInt( IDC_TEXT_MARGINS_RIGHT, 3 );
	SetDlgItemInt( IDC_TEXT_MARGINS_LEFT, 3 );
	SetDlgItemInt( IDC_TEXT_MARGINS_BOTTOM, 3 );
	SetDlgItemInt( IDC_ICON_MARGINS_TOP, 3 );
	SetDlgItemInt( IDC_ICON_MARGINS_RIGHT, 3 );
	SetDlgItemInt( IDC_ICON_MARGINS_LEFT, 3 );
	SetDlgItemInt( IDC_ICON_MARGINS_BOTTOM, 3 );

	CString sTooltip( _T("Tooltip") );
	m_Tooltip.SetWindowText( sTooltip );
	m_btnPushButton.SetTooltipText( LPCTSTR(sTooltip) );
	m_Tooltip.SetTooltipText( LPCTSTR(sTooltip) );

	m_btnPushButton.m_icon.AssignFromHICON(
		(HICON) ::LoadImage(
			::AfxGetInstanceHandle(),
			MAKEINTRESOURCE(IDI_NOTEPAD),
			IMAGE_ICON,
			48,
			48,
			0
			),
		false
		);

HICON hIconIn = (HICON)
		::LoadImage(
			::AfxGetInstanceHandle(),
			MAKEINTRESOURCE(IDI_ICON_IN),
			IMAGE_ICON,
			32,
			32,
			0
			);
	m_btnWithMenu.m_icon.m_bmpHover.AssignFromHICON( hIconIn );
	::DeleteObject( hIconIn );
HICON hIconOut = (HICON)
		::LoadImage(
			::AfxGetInstanceHandle(),
			MAKEINTRESOURCE(IDI_ICON_OUT),
			IMAGE_ICON,
			32,
			32,
			0
			);
	m_btnWithMenu.m_icon.m_bmpNormal.AssignFromHICON( hIconOut );
	::DeleteObject( hIconOut );
	m_btnWithMenu.m_icon.m_dwFlags |= __EXT_ICON_PERSISTENT_BITMAP_HOVER;

m_btnPushLikeButton.m_icon.AssignFromHICON(
		(HICON) ::LoadImage(
			::AfxGetInstanceHandle(),
				MAKEINTRESOURCE(IDI_LED_OFF),
				IMAGE_ICON,
				16,
				16,
				0
				),
			false
			);
	
	CheckDlgButton( IDC_CHECK_BUTTON_ENABLE, BST_CHECKED );
	CheckDlgButton( IDC_CHECK_BOX_ENABLE, BST_CHECKED );
	
	m_btnColor1.m_lParamCookie = LPARAM(IDC_BUTTON_COLOR1);
	m_btnColor1.m_nMenuLeftAreaWidth = 30;
	m_btnColor2.m_lParamCookie = LPARAM(IDC_BUTTON_COLOR2);
	
	m_btnWithMenu.m_dwMenuOpt |= TPMX_OWNERDRAW_FIXED;
	CMenu _menu;
	VERIFY(
		_menu.LoadMenu(IDR_POPUP_MENU)
		);
	m_btnWithMenu.m_menu.Attach(
		_menu.Detach()
		);
	
	m_bInitComplete = true;

	int nIndex = -1, nInitSel = -1;

	//////////////////////////////////////////////////////////////////////////

	nInitSel = nIndex = m_wndTextAlignmentHorz.AddString( _T("Default") );
	m_wndTextAlignmentHorz.SetItemData( nIndex, CExtPaintManager::__ALIGN_HORIZ_CENTER );
	nIndex = m_wndTextAlignmentHorz.AddString( _T("Left") );
	m_wndTextAlignmentHorz.SetItemData( nIndex, CExtPaintManager::__ALIGN_HORIZ_LEFT );
	nIndex = m_wndTextAlignmentHorz.AddString( _T("Right") );
	m_wndTextAlignmentHorz.SetItemData( nIndex, CExtPaintManager::__ALIGN_HORIZ_RIGHT );
	nIndex = m_wndTextAlignmentHorz.AddString( _T("Center") );
	m_wndTextAlignmentHorz.SetItemData( nIndex, CExtPaintManager::__ALIGN_HORIZ_CENTER );
	m_wndTextAlignmentHorz.SetCurSel( nInitSel );

	nInitSel = nIndex = m_wndIconAlignmentHorz.AddString( _T("Default") );
	m_wndIconAlignmentHorz.SetItemData( nIndex, CExtPaintManager::__ALIGN_HORIZ_LEFT );
	nIndex = m_wndIconAlignmentHorz.AddString( _T("Left") );
	m_wndIconAlignmentHorz.SetItemData( nIndex, CExtPaintManager::__ALIGN_HORIZ_LEFT );
	nIndex = m_wndIconAlignmentHorz.AddString( _T("Right") );
	m_wndIconAlignmentHorz.SetItemData( nIndex, CExtPaintManager::__ALIGN_HORIZ_RIGHT );
	nIndex = m_wndIconAlignmentHorz.AddString( _T("Center") );
	m_wndIconAlignmentHorz.SetItemData( nIndex, CExtPaintManager::__ALIGN_HORIZ_CENTER );
	m_wndIconAlignmentHorz.SetCurSel( nInitSel );

	nInitSel = nIndex = m_wndTextAlignmentVert.AddString( _T("Default") );
	m_wndTextAlignmentVert.SetItemData( nIndex, CExtPaintManager::__ALIGN_VERT_CENTER );
	nIndex = m_wndTextAlignmentVert.AddString( _T("Top") );
	m_wndTextAlignmentVert.SetItemData( nIndex, CExtPaintManager::__ALIGN_VERT_TOP );
	nIndex = m_wndTextAlignmentVert.AddString( _T("Bottom") );
	m_wndTextAlignmentVert.SetItemData( nIndex, CExtPaintManager::__ALIGN_VERT_BOTTOM );
	nIndex = m_wndTextAlignmentVert.AddString( _T("Center") );
	m_wndTextAlignmentVert.SetItemData( nIndex, CExtPaintManager::__ALIGN_VERT_CENTER );
	m_wndTextAlignmentVert.SetCurSel( nInitSel );

	nInitSel = nIndex = m_wndIconAlignmentVert.AddString( _T("Default") );
	m_wndIconAlignmentVert.SetItemData( nIndex, CExtPaintManager::__ALIGN_VERT_CENTER );
	nIndex = m_wndIconAlignmentVert.AddString( _T("Top") );
	m_wndIconAlignmentVert.SetItemData( nIndex, CExtPaintManager::__ALIGN_VERT_TOP );
	nIndex = m_wndIconAlignmentVert.AddString( _T("Bottom") );
	m_wndIconAlignmentVert.SetItemData( nIndex, CExtPaintManager::__ALIGN_VERT_BOTTOM );
	nIndex = m_wndIconAlignmentVert.AddString( _T("Center") );
	m_wndIconAlignmentVert.SetItemData( nIndex, CExtPaintManager::__ALIGN_VERT_CENTER );
	m_wndIconAlignmentVert.SetCurSel( nInitSel );

	//////////////////////////////////////////////////////////////////////////

	nInitSel = nIndex = m_CheckBoxSkin.AddString( _T("Standard") );
	m_CheckBoxSkin.SetItemData( nIndex, 0 );
	nIndex = m_CheckBoxSkin.AddString( _T("XP") );
	m_CheckBoxSkin.SetItemData( nIndex, IDB_BOX_IMAGES_XP );
	nIndex = m_CheckBoxSkin.AddString( _T("XP Blue") );
	m_CheckBoxSkin.SetItemData( nIndex, IDB_BOX_IMAGES_XP_BLUE );
	nIndex = m_CheckBoxSkin.AddString( _T("XP Cross") );
	m_CheckBoxSkin.SetItemData( nIndex, IDB_BOX_IMAGES_XP_CROSS );
	m_CheckBoxSkin.SetCurSel( nInitSel );
	
	nInitSel = nIndex = m_Cursor.AddString( _T("Default") );
	m_Cursor.SetItemData( nIndex, 0 );
	nIndex = m_Cursor.AddString( _T("Hand") );
	m_Cursor.SetItemData( nIndex, (DWORD)IDC_HAND);
	nIndex = m_Cursor.AddString( _T("Cross") );
	m_Cursor.SetItemData( nIndex, (DWORD)IDC_CROSS);
	nIndex = m_Cursor.AddString( _T("UpArrow") );
	m_Cursor.SetItemData( nIndex, (DWORD)IDC_UPARROW);
	nIndex = m_Cursor.AddString( _T("Wait") );
	m_Cursor.SetItemData( nIndex, (DWORD)IDC_WAIT);
	nIndex = m_Cursor.AddString( _T("SizeAll") );
	m_Cursor.SetItemData( nIndex, (DWORD)IDC_SIZEALL);
	m_Cursor.SetCurSel( nInitSel );
	m_Cursor.SetTooltipText( _T("Button cursor") );

	OnCmdMsg( IDC_CHECK_BUTTON_ENABLE, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_CHECK_FLAT, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_CHECK_BORDER, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_CHECK_BUTTON_SHOW_FOCUS_RECT, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_CHECK_SEPARATED_BUTTONS, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_CHECK_BOX_ENABLE, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_CHECK_TRISTATE, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_CHECK_LEFT_TEXT, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_CHECK_BOX_SHOW_FOCUS_RECT, CN_COMMAND, NULL, NULL );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

BOOL CPageButtons::OnCmdMsg(UINT nID, INT nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( !m_bInitComplete ){
		return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}

	if( nCode == EN_CHANGE )
	{
		switch( nID )
		{
		case IDC_TOOLTIP:
			{
				CString sTooltip;
				m_Tooltip.GetWindowText( sTooltip );
				m_btnPushButton.SetTooltipText( LPCTSTR(sTooltip) );
				m_Tooltip.SetTooltipText( LPCTSTR(sTooltip) );
			}
			break;

		case IDC_TEXT_MARGINS_TOP:
		case IDC_TEXT_MARGINS_RIGHT:
		case IDC_TEXT_MARGINS_LEFT:
		case IDC_TEXT_MARGINS_BOTTOM:
			{
				UINT nValTop = GetDlgItemInt( IDC_TEXT_MARGINS_TOP );
				UINT nValRight = GetDlgItemInt( IDC_TEXT_MARGINS_RIGHT );
				UINT nValLeft = GetDlgItemInt( IDC_TEXT_MARGINS_LEFT );
				UINT nValBottom = GetDlgItemInt( IDC_TEXT_MARGINS_BOTTOM );
				m_btnPushButton.SetTextMargins(
					nValLeft,
					nValTop,
					nValRight,
					nValBottom
					);
			}
			break;

		case IDC_ICON_MARGINS_TOP:
		case IDC_ICON_MARGINS_RIGHT:
		case IDC_ICON_MARGINS_LEFT:
		case IDC_ICON_MARGINS_BOTTOM:
			{
				UINT nValTop = GetDlgItemInt( IDC_ICON_MARGINS_TOP );
				UINT nValRight = GetDlgItemInt( IDC_ICON_MARGINS_RIGHT );
				UINT nValLeft = GetDlgItemInt( IDC_ICON_MARGINS_LEFT );
				UINT nValBottom = GetDlgItemInt( IDC_ICON_MARGINS_BOTTOM );
				m_btnPushButton.SetIconMargins(
					nValLeft,
					nValTop,
					nValRight,
					nValBottom
					);
			}
			break;

		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}

		return TRUE;
	}

	if(		nCode == CN_UPDATE_COMMAND_UI
		&& (	nID == IDC_MENU_ITEM_RARELY1
			||	nID == IDC_MENU_ITEM_RARELY2
			||	nID == IDC_MENU_ITEM_RARELY3 )
		)
		{				
			CCmdUI * pCmdUI = (CCmdUI *)pExtra;
			ASSERT( pCmdUI != NULL );
			pCmdUI->Enable();
			return TRUE;
		}

	if( nCode == CN_COMMAND )
	{
		BOOL bChecked = FALSE;
		
		switch( nID )
		{
		case IDC_CHECK_BUTTON_ENABLE:
			bChecked = IsDlgButtonChecked( nID );
			m_btnPushButton.EnableWindow( bChecked );
			m_btnPushLikeButton.EnableWindow( bChecked );
			m_btnWithMenu.EnableWindow( bChecked );
			m_btnColor1.EnableWindow( bChecked );
			m_btnColor2.EnableWindow( bChecked );
			break;

		case IDC_CHECK_FLAT:
			bChecked = IsDlgButtonChecked( nID );
			m_btnPushButton.SetFlat( bChecked );
			m_btnPushLikeButton.SetFlat( bChecked );
			m_btnWithMenu.SetFlat( bChecked );
			m_btnColor1.SetFlat( bChecked );
			m_btnColor2.SetFlat( bChecked );
			break;
			
		case IDC_CHECK_BORDER:
			bChecked = IsDlgButtonChecked( nID );
			m_btnPushButton.SetDrawBorder( bChecked );
			m_btnPushLikeButton.SetDrawBorder( bChecked );
			m_btnWithMenu.SetDrawBorder( bChecked );
			m_btnColor1.SetDrawBorder( bChecked );
			m_btnColor2.SetDrawBorder( bChecked );
			break;
			
		case IDC_PUSH_LIKE_BUTTON:
			m_btnPushLikeButton.m_icon.AssignFromHICON(
				(HICON) ::LoadImage(
					::AfxGetInstanceHandle(),
					IsDlgButtonChecked( nID )
						? MAKEINTRESOURCE(IDI_LED_ON)
						: MAKEINTRESOURCE(IDI_LED_OFF),
					IMAGE_ICON,
					16,
					16,
					0
					),
				false
				);
			break;
			
		case IDC_CHECK_BUTTON_SHOW_FOCUS_RECT:
			bChecked = IsDlgButtonChecked( nID );
			m_btnPushButton.SetDrawFocusRect( bChecked );
			m_btnPushLikeButton.SetDrawFocusRect( bChecked );
			m_btnWithMenu.SetDrawFocusRect( bChecked );
			m_btnColor1.SetDrawFocusRect( bChecked );
			m_btnColor2.SetDrawFocusRect( bChecked );
			break;
			
		case IDC_CHECK_SEPARATED_BUTTONS:
			bChecked = IsDlgButtonChecked( nID );
			m_btnWithMenu.SetSeparatedDropDown( bChecked );
			m_btnColor1.SetSeparatedDropDown( bChecked );
			m_btnColor2.SetSeparatedDropDown( bChecked );
			break;

		case IDC_CHECK_BOX_ENABLE:
			m_chkCheckBox1.EnableWindow( IsDlgButtonChecked( nID ) );
			m_chkCheckBox2.EnableWindow( IsDlgButtonChecked( nID ) );
			m_wndRadio1.EnableWindow( IsDlgButtonChecked( nID ) );
			m_wndRadio2.EnableWindow( IsDlgButtonChecked( nID ) );
			break;
			
		case IDC_CHECK_TRISTATE:
			if( IsDlgButtonChecked( nID ) ){
				m_chkCheckBox1.SetButtonStyle( BS_AUTO3STATE );
				m_chkCheckBox2.SetButtonStyle( BS_AUTO3STATE );
			}else{
				m_chkCheckBox1.SetButtonStyle( BS_AUTOCHECKBOX );
				m_chkCheckBox2.SetButtonStyle( BS_AUTOCHECKBOX );
			}
			m_chkCheckBox1.Invalidate();
			m_chkCheckBox2.Invalidate();
			break;
			
		case IDC_CHECK_LEFT_TEXT:
			if( IsDlgButtonChecked( nID ) ){
				m_chkCheckBox1.ModifyStyle( 0, BS_LEFTTEXT );
				m_chkCheckBox2.ModifyStyle( 0, BS_LEFTTEXT );
				m_wndRadio1.ModifyStyle( 0, BS_LEFTTEXT );
				m_wndRadio2.ModifyStyle( 0, BS_LEFTTEXT );
			}else{
				m_chkCheckBox1.ModifyStyle( BS_LEFTTEXT, 0 );
				m_chkCheckBox2.ModifyStyle( BS_LEFTTEXT, 0 );
				m_wndRadio1.ModifyStyle( BS_LEFTTEXT, 0 );
				m_wndRadio2.ModifyStyle( BS_LEFTTEXT, 0 );
			}
			m_chkCheckBox1.Invalidate();
			m_chkCheckBox2.Invalidate();
			m_wndRadio1.Invalidate();
			m_wndRadio2.Invalidate();
			break;
			
		case IDC_CHECK_BOX_SHOW_FOCUS_RECT:
			bChecked = IsDlgButtonChecked( nID );
			m_chkCheckBox1.SetDrawFocusRect( bChecked );
			m_chkCheckBox2.SetDrawFocusRect( bChecked );
			m_wndRadio1.SetDrawFocusRect( bChecked );
			m_wndRadio2.SetDrawFocusRect( bChecked );
			break;

		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		return TRUE;
	}
	return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//////////////////////////////////////////////////////////////////////////

LRESULT CPageButtons::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
	switch( lParam )
	{
	case IDC_BUTTON_COLOR1:
	case IDC_BUTTON_COLOR2:
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

LRESULT CPageButtons::OnColorSelectCustom(WPARAM wParam, LPARAM lParam)
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

//////////////////////////////////////////////////////////////////////////

LRESULT CPageButtons::OnDrawPopupLeftArea(WPARAM wParam, LPARAM lParam)
{
	wParam;
	CExtPopupMenuWnd::DRAWLEFTAREADATA * pDrawLeftAreaData =
		reinterpret_cast < CExtPopupMenuWnd::DRAWLEFTAREADATA * > ( lParam );
	ASSERT( pDrawLeftAreaData != NULL );
	
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
	
	return !0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CPageButtons::OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam)
{
	wParam;
	CExtPopupMenuWnd::DRAWITEMDATA * pDrawItemData =
		reinterpret_cast < CExtPopupMenuWnd::DRAWITEMDATA * > ( lParam );
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
		DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS, 0
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

LRESULT CPageButtons::OnExtMenuPrepare(WPARAM wParam, LPARAM lParam)
{
	lParam;
CExtPopupMenuWnd::MsgPrepareMenuData_t * pData =
		reinterpret_cast
		< CExtPopupMenuWnd::MsgPrepareMenuData_t * >
		( wParam );
	ASSERT( pData != NULL );
CExtPopupMenuWnd * pPopup = pData->m_pPopup;
	ASSERT( pPopup != NULL );
	
INT nReplacePos =
		pPopup->ItemFindPosForCmdID( ID_COLOR_CTRL_8X5 );
	if( nReplacePos >= 0 )
	{
		CExtPopupColorMenuWnd * pColorPopup = new CExtPopupColorMenuWnd;
		pColorPopup->m_lParamCookie = LPARAM( ID_COLOR_CTRL_8X5 );
		pColorPopup->m_hWndNotifyColorChanged = GetSafeHwnd();
		pColorPopup->m_clrDefault = COLORREF(-1);
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
		CExtPopupDatePickerMenuWnd * pDatePickerPopup = new CExtPopupDatePickerMenuWnd;
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
	
	nReplacePos = pPopup->ItemFindPosForCmdID( ID_PALETTE_MENU );
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

//////////////////////////////////////////////////////////////////////////

void CPageButtons::OnSelendokSkin() 
{
int nCurSel = m_CheckBoxSkin.GetCurSel();
int nBitmapResource = int( m_CheckBoxSkin.GetItemData( nCurSel ) );

	m_chkCheckBox2.m_nBitmapResource 
		= m_chkCheckBox1.m_nBitmapResource 
		= nBitmapResource;

	if( nBitmapResource == IDB_BOX_IMAGES_XP )
	{
		m_wndRadio1.m_nBitmapResource 
			= m_wndRadio2.m_nBitmapResource 
			= IDB_RADIO_BOX_IMAGES_XP;
	}
	else if( nBitmapResource == IDB_BOX_IMAGES_XP_CROSS )
	{
		m_wndRadio1.m_nBitmapResource 
			= m_wndRadio2.m_nBitmapResource 
			= IDB_RADIO_BOX_IMAGES_XP_CROSS;
	}
	else if( nBitmapResource == IDB_BOX_IMAGES_XP_BLUE )
	{
		m_wndRadio1.m_nBitmapResource 
			= m_wndRadio2.m_nBitmapResource 
			= IDB_RADIO_BOX_IMAGES_XP_BLUE;
	}
	else
	{
		m_wndRadio1.m_nBitmapResource 
			= m_wndRadio2.m_nBitmapResource 
			= m_chkCheckBox1.m_nBitmapResource
			= m_chkCheckBox2.m_nBitmapResource
			= 0;
	}
		
	m_chkCheckBox1.Invalidate();
	m_chkCheckBox2.Invalidate();
	m_wndRadio1.Invalidate();
	m_wndRadio2.Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void CPageButtons::OnSelendokCursor() 
{
int nCurSel = m_Cursor.GetCurSel();
int nCursorType = int( m_Cursor.GetItemData( nCurSel ) );
	if( nCursorType == 0 )
		nCursorType = -1;
	m_btnPushButton.SetBtnCursor( nCursorType );
}

void CPageButtons::OnSelendokIconAlignmentHorz() 
{
INT nCurSel = m_wndIconAlignmentHorz.GetCurSel();
DWORD dwStyle = DWORD( m_wndIconAlignmentHorz.GetItemData( nCurSel ) );
INT nIconAlignment = m_btnPushButton.GetIconAlignment();
	ASSERT( nIconAlignment >= 0 );
	nIconAlignment &= ~CExtPaintManager::__ALIGN_HORIZ_MASK;
	nIconAlignment |= dwStyle;

	m_btnPushButton.SetIconAlignment( nIconAlignment );
}

void CPageButtons::OnSelendokIconAlignmentVert() 
{
INT nCurSel = m_wndIconAlignmentVert.GetCurSel();
DWORD dwStyle = DWORD( m_wndIconAlignmentVert.GetItemData( nCurSel ) );
INT nIconAlignment = m_btnPushButton.GetIconAlignment();
	ASSERT( nIconAlignment >= 0 );
	nIconAlignment &= ~CExtPaintManager::__ALIGN_VERT_MASK;
	nIconAlignment |= dwStyle;

	m_btnPushButton.SetIconAlignment( nIconAlignment );
}

void CPageButtons::OnSelendokTextAlignmentHorz() 
{
INT nCurSel = m_wndTextAlignmentHorz.GetCurSel();
DWORD dwStyle = (DWORD)m_wndTextAlignmentHorz.GetItemData( nCurSel );
INT nTextAlignment = m_btnPushButton.GetTextAlignment();
	ASSERT( nTextAlignment >= 0 );
	nTextAlignment &= ~CExtPaintManager::__ALIGN_HORIZ_MASK;
	nTextAlignment |= dwStyle;

	m_btnPushButton.SetTextAlignment( nTextAlignment );
}

void CPageButtons::OnSelendokTextAlignmentVert() 
{
INT nCurSel = m_wndTextAlignmentVert.GetCurSel();
DWORD dwStyle = DWORD( m_wndTextAlignmentVert.GetItemData( nCurSel ) );
INT nTextAlignment = m_btnPushButton.GetTextAlignment();
	ASSERT( nTextAlignment >= 0 );
	nTextAlignment &= ~CExtPaintManager::__ALIGN_VERT_MASK;
	nTextAlignment |= dwStyle;

	m_btnPushButton.SetTextAlignment( nTextAlignment );
}

void CPageButtons::OnTextMarginsReset() 
{
	SetDlgItemInt( IDC_TEXT_MARGINS_TOP, 3 );
	SetDlgItemInt( IDC_TEXT_MARGINS_RIGHT, 3 );
	SetDlgItemInt( IDC_TEXT_MARGINS_LEFT, 3 );
	SetDlgItemInt( IDC_TEXT_MARGINS_BOTTOM, 3 );
}

void CPageButtons::OnIconMarginsReset() 
{
	SetDlgItemInt( IDC_ICON_MARGINS_TOP, 3 );
	SetDlgItemInt( IDC_ICON_MARGINS_RIGHT, 3 );
	SetDlgItemInt( IDC_ICON_MARGINS_LEFT, 3 );
	SetDlgItemInt( IDC_ICON_MARGINS_BOTTOM, 3 );
}
