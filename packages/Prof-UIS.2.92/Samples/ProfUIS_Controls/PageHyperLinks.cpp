// PageHyperLinks.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageHyperLinks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageHyperLinks dialog

#define IDC_HAND MAKEINTRESOURCE(32649)

#define __URL_WWW			_T("www.prof-uis.com")
#define __URL_E_MAIL		_T("info@prof-uis.com")

#define __TOOLTIP_WWW		_T("Learn more about Prof-UIS")
#define __TOOLTIP_E_MAIL	_T("Contact us if you have any questions")

CPageHyperLinks::CPageHyperLinks(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageHyperLinks::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageHyperLinks)
	//}}AFX_DATA_INIT
}


void CPageHyperLinks::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageHyperLinks)
	DDX_Control(pDX, IDC_LINK_SHOW_FOCUS, m_wndLinkShowFocus);
	DDX_Control(pDX, IDC_LINK_ENABLE, m_wndLinkEnable);
	DDX_Control(pDX, IDC_LINK_CURSOR, m_wndLinkCursor);
	DDX_Control(pDX, IDC_LINK_URL, m_wndLinkURL);
	DDX_Control(pDX, IDC_LINK_CAPTION, m_wndLinkCaption);
	DDX_Control(pDX, IDC_STATIC_URL, m_wndLabelURL);
	DDX_Control(pDX, IDC_STATIC_CAPTION, m_wndLabelCaption);
	DDX_Control(pDX, IDC_STATIC_TOOLTIP, m_wndLabelTooltip);
	DDX_Control(pDX, IDC_STATIC_CURSOR, m_wndLabelCursor);
	DDX_Control(pDX, IDC_LINK_UNDERLINE, m_wndLinkUnderline);
	DDX_Control(pDX, IDC_LINK_TOOLTIP, m_wndLinkTooltip);
	DDX_Control(pDX, IDC_LINK_COLOR_VISITED, m_wndLinkColorVisited);
	DDX_Control(pDX, IDC_LINK_COLOR_HOVER, m_wndLinkColorHover);
	DDX_Control(pDX, IDC_LINK_COLOR, m_wndLinkColor);
	DDX_Control(pDX, IDC_HYPER_LINK2, m_wndHyperLink2);
	DDX_Control(pDX, IDC_HYPER_LINK1, m_wndHyperLink1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageHyperLinks, CPageBase)
	//{{AFX_MSG_MAP(CPageHyperLinks)
	ON_CBN_SELENDOK(IDC_LINK_CURSOR, OnSelendokLinkCursor)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageHyperLinks message handlers

BOOL CPageHyperLinks::OnInitDialog() 
{
	CPageBase::OnInitDialog();

// 	CMenu _menu;
// 	VERIFY(
// 		_menu.LoadMenu(IDR_POPUP_MENU)
// 		);
// 	m_wndHyperLink1.m_menu.Attach(
// 		_menu.Detach()
// 		);
// 	m_wndHyperLink1.m_bUseStdCommandNotification = true;

	m_wndLinkColor.m_lParamCookie = LPARAM(IDC_LINK_COLOR);
	m_wndLinkColorHover.m_lParamCookie = LPARAM(IDC_LINK_COLOR_HOVER);
	m_wndLinkColorVisited.m_lParamCookie = LPARAM(IDC_LINK_COLOR_VISITED);

	m_wndLinkColorVisited.m_clrDefault = 
		m_wndLinkColorVisited.m_clrSelected = m_wndHyperLink1.GetTextColorVisited();
	m_wndLinkColorHover.m_clrDefault = 
		m_wndLinkColorHover.m_clrSelected = m_wndHyperLink1.GetTextColorHover();
	m_wndLinkColor.m_clrDefault = 
		m_wndLinkColor.m_clrSelected = m_wndHyperLink1.GetTextColorNormal();

	m_wndLinkColorVisited.SetDrawFocusRect( TRUE );
	m_wndLinkColorHover.SetDrawFocusRect( TRUE );
	m_wndLinkColor.SetDrawFocusRect( TRUE );

	m_wndLinkColorVisited.SetFlat( TRUE );
	m_wndLinkColorHover.SetFlat( TRUE );
	m_wndLinkColor.SetFlat( TRUE );

	m_wndLinkUnderline.SetCheck( m_wndHyperLink1.GetTextUnderline() != false );

	int nIndex = 0;
	int nInitSel = 0;
	nIndex = m_wndLinkCursor.AddString( _T("Default") );
	m_wndLinkCursor.SetItemData( nIndex, 0 );
	nInitSel = nIndex = m_wndLinkCursor.AddString( _T("Hand") );
	m_wndLinkCursor.SetItemData( nIndex, (DWORD)IDC_HAND);
	nIndex = m_wndLinkCursor.AddString( _T("Cross") );
	m_wndLinkCursor.SetItemData( nIndex, (DWORD)IDC_CROSS);
	nIndex = m_wndLinkCursor.AddString( _T("UpArrow") );
	m_wndLinkCursor.SetItemData( nIndex, (DWORD)IDC_UPARROW);
	nIndex = m_wndLinkCursor.AddString( _T("Wait") );
	m_wndLinkCursor.SetItemData( nIndex, (DWORD)IDC_WAIT);
	nIndex = m_wndLinkCursor.AddString( _T("SizeAll") );
	m_wndLinkCursor.SetItemData( nIndex, (DWORD)IDC_SIZEALL);
	m_wndLinkCursor.SetCurSel( nInitSel );
	m_wndLinkCursor.SetTooltipText( _T("Hyperlink cursor") );

	CheckDlgButton( IDC_LINK_ENABLE, BST_CHECKED );

	m_wndHyperLink1.m_strURL = _T("http://");
	m_wndHyperLink1.m_strURL += __URL_WWW;
	m_wndHyperLink1.SetTooltipText( m_wndHyperLink1.m_strURL );
	m_wndLinkCaption.SetWindowText( __TOOLTIP_WWW );
	m_wndLinkURL.SetWindowText( m_wndHyperLink1.m_strURL );
	m_wndLinkTooltip.SetWindowText( __TOOLTIP_WWW );

	m_wndHyperLink2.m_strURL = _T("mailto:");
	m_wndHyperLink2.m_strURL += __URL_E_MAIL;
	m_wndHyperLink2.SetTooltipText( __TOOLTIP_E_MAIL );
	
	m_bInitComplete = true;

	OnCmdMsg( IDC_LINK_ENABLE, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_LINK_SHOW_FOCUS, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_LINK_UNDERLINE, CN_COMMAND, NULL, NULL );
	OnCmdMsg( IDC_LINK_TOOLTIP, EN_CHANGE, NULL, NULL );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CPageHyperLinks::OnCmdMsg(UINT nID, INT nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( !m_bInitComplete )
		return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	if( nCode == EN_CHANGE )
	{
		CString sText;
		switch( nID )
		{
		case IDC_LINK_TOOLTIP:
			m_wndLinkTooltip.GetWindowText( sText );
			m_wndHyperLink1.SetTooltipText( LPCTSTR(sText) );
			break;
		case IDC_LINK_CAPTION:
			m_wndLinkCaption.GetWindowText( sText );
			m_wndHyperLink1.SetWindowText( LPCTSTR(sText) );
			break;
		case IDC_LINK_URL:
			m_wndLinkURL.GetWindowText( sText );
			m_wndHyperLink1.m_strURL = LPCTSTR(sText);
			break;
		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		return TRUE;
	}

	if( nCode == CN_COMMAND )
	{
		BOOL bChecked = FALSE;
		
		switch( nID )
		{
		case IDC_LINK_ENABLE:
			bChecked = IsDlgButtonChecked( nID );
			m_wndHyperLink1.EnableWindow( bChecked );
			m_wndHyperLink2.EnableWindow( bChecked );
			break;
		
		case IDC_LINK_SHOW_FOCUS:
			bChecked = IsDlgButtonChecked( nID );
			m_wndHyperLink1.SetDrawFocusRect( bChecked );
			m_wndHyperLink2.SetDrawFocusRect( bChecked );
			break;

		case IDC_LINK_UNDERLINE:
			bChecked = IsDlgButtonChecked( nID );
			m_wndHyperLink1.SetTextUnderline( bChecked != FALSE );
			m_wndHyperLink2.SetTextUnderline( bChecked != FALSE );
			break;

		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		return TRUE;
	}
	return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//////////////////////////////////////////////////////////////////////////

LRESULT CPageHyperLinks::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
	COLORREF clrColor = COLORREF(wParam);

	switch( lParam )
	{
	case IDC_LINK_COLOR:
		m_wndHyperLink1.SetTextColorNormal( clrColor );
		m_wndHyperLink2.SetTextColorNormal( clrColor );
		break;
	case IDC_LINK_COLOR_HOVER:
		m_wndHyperLink1.SetTextColorHover( clrColor );
		m_wndHyperLink2.SetTextColorHover( clrColor );
		break;
	case IDC_LINK_COLOR_VISITED:
		m_wndHyperLink1.SetTextColorVisited( clrColor );
		m_wndHyperLink2.SetTextColorVisited( clrColor );
		break;
	default:
		break;
	} // switch( lParam )
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CPageHyperLinks::OnSelendokLinkCursor() 
{
int nCurSel = m_wndLinkCursor.GetCurSel();
int nCursorType = int( m_wndLinkCursor.GetItemData( nCurSel ) );
	if( nCursorType == 0 )
		nCursorType = -1;
	m_wndHyperLink1.SetBtnCursor( nCursorType );
	m_wndHyperLink2.SetBtnCursor( nCursorType );
}

/////////////////////////////////////////////////////////////////////////////
// CPageAviLabel dialog

CPageAviLabel::CPageAviLabel(CWnd* pParent /*=NULL*/)
	: CPageBase( CPageAviLabel::IDD, pParent )
	, m_nFrom( 0 )
	, m_nTo( -1 )
	, m_nRep( -1 )
{
	//{{AFX_DATA_INIT(CPageAviLabel)
	//}}AFX_DATA_INIT
}

void CPageAviLabel::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageAviLabel)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SPIN_FRAME_TO, m_spinTo);
	DDX_Control(pDX, IDC_SPIN_FRAME_REPEAT, m_spinRepeat);
	DDX_Control(pDX, IDC_SPIN_FRAME_FROM, m_spinFrom);
	DDX_Control(pDX, IDC_STATIC_AVI_FRAME_COUNT, m_staticAviFrameCount);
	DDX_Control(pDX, IDC_ZSB_SPEED_RATE, m_sbSpeedRate);
	DDX_Control(pDX, IDC_EDIT_FRAME_TO, m_editFrameTo);
	DDX_Control(pDX, IDC_EDIT_FRAME_REPEAT, m_editFrameRepeat);
	DDX_Control(pDX, IDC_EDIT_FRAME_FROM, m_editFrameFrom);
	DDX_Control(pDX, IDC_COMBO_IMAGE_MODE, m_comboImageMode);
	DDX_Control(pDX, IDC_CHECK_USE_RANGE, m_checkUseRange);
	DDX_Control(pDX, IDC_CHECK_TRANSPARENT_VIDEO, m_checkTransparentVideo);
	DDX_Control(pDX, IDC_CHECK_PLAY_AUTOMATICALLY, m_checkPlayAutomatically);
	DDX_Control(pDX, IDC_CHECK_EMPTY_BACKGROUND_ON_STOP, m_checkEmptyBackgroundOnStop);
	DDX_Control(pDX, IDC_CHECK_CENTER_VERTICALLY, m_checkCenterVertically);
	DDX_Control(pDX, IDC_BUTTON_TRANSPARENT_COLOR, m_buttonTransparentColor);
	DDX_Control(pDX, IDC_BUTTON_BACKGROUND_COLOR, m_buttonBackgroundColor);
	DDX_Control(pDX, IDC_EDIT_TT_TEXT, m_editToolTipText);
	DDX_Control(pDX, IDC_COMBO_ALIGN_H, m_comboAlignH);
	DDX_Control(pDX, IDC_COMBO_OPEN, m_comboOpen);
	DDX_Control(pDX, IDC_CHECK_CONTROL_BORDER, m_checkControlBorder);
	DDX_Control(pDX, IDC_BUTTON_SPEED_RATE_RESET, m_buttonResetSpeedRate);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_buttonPlay);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_buttonPause);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_buttonStop);
	DDX_Control(pDX, IDC_BUTTON_EMPTY, m_buttonEmpty);
	DDX_Control(pDX, IDC_STATIC_SPEED_RATE, m_staticSpeedRate);
	DDX_Control(pDX, IDC_EDIT_SELECT_FILE, m_editSelectFile);
	DDX_Control(pDX, IDC_BUTTON_SIZE_FROM_CONTENT, m_buttonSizeFromContent);
}

BEGIN_MESSAGE_MAP( CPageAviLabel, CExtResizableDialog )
	//{{AFX_MSG_MAP(CPageAviLabel)
	ON_CBN_SELENDOK( IDC_COMBO_OPEN, OnComboOpen )
	ON_CBN_SELENDOK( IDC_COMBO_ALIGN_H, OnChangeState )
	ON_BN_CLICKED( IDC_BUTTON_PLAY, OnButtonPlay )
	ON_BN_CLICKED( IDC_BUTTON_PAUSE, OnButtonPause )
	ON_BN_CLICKED( IDC_BUTTON_STOP, OnButtonStop )
	ON_BN_CLICKED( IDC_BUTTON_EMPTY, OnButtonEmpty )
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SPEED_RATE_RESET, OnButtonSpeedRateReset)
	ON_BN_CLICKED(IDC_CHECK_CONTROL_BORDER, OnCheckControlBorder)
	ON_WM_SIZE()
	ON_CBN_SELENDOK( IDC_COMBO_IMAGE_MODE, OnChangeState )
	ON_BN_CLICKED( IDC_CHECK_USE_RANGE, OnChangeState )
	ON_BN_CLICKED( IDC_CHECK_CENTER_VERTICALLY, OnChangeState )
	ON_BN_CLICKED( IDC_CHECK_PLAY_AUTOMATICALLY, OnChangeState )
	ON_BN_CLICKED( IDC_CHECK_EMPTY_BACKGROUND_ON_STOP, OnChangeState )
	ON_BN_CLICKED( IDC_CHECK_TRANSPARENT_VIDEO, OnChangeState )
	ON_EN_CHANGE( IDC_EDIT_TT_TEXT, OnChangeState )
	ON_EN_CHANGE( IDC_EDIT_FRAME_FROM, OnChangeState )
	ON_EN_CHANGE( IDC_EDIT_FRAME_TO, OnChangeState )
	ON_EN_CHANGE( IDC_EDIT_FRAME_REPEAT, OnChangeState )
	ON_BN_CLICKED( IDC_SPIN_FRAME_FROM, OnChangeState )
	ON_BN_CLICKED( IDC_SPIN_FRAME_TO, OnChangeState )
	ON_BN_CLICKED( IDC_SPIN_FRAME_REPEAT, OnChangeState )
	ON_BN_CLICKED(IDC_BUTTON_SIZE_FROM_CONTENT, OnButtonSizeFromContent)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally, OnNotifyColorChangedFinally )
END_MESSAGE_MAP()

BOOL CPageAviLabel::OnInitDialog()
{
	CPageBase:: OnInitDialog();
	VERIFY( m_wndAviLabel.SubclassDlgItem( IDC_ST_VIDEO, this ) );
//	m_wndAviLabel.SetTooltipText( _T("m_wndAviLabel") );
	m_editToolTipText.SetCueBanner( _T("Type tooltip text here.") );

	m_comboOpen.SetCurSel( 0 );
	m_comboImageMode.SetCurSel( 0 );
	m_comboAlignH.SetCurSel( 0 );

	m_editFrameFrom.SetWindowText( _T("0") );
	m_editFrameTo.SetWindowText( _T("-1") );
	m_editFrameRepeat.SetWindowText( _T("-1") );

	m_spinFrom.SetRange( 100, 0 );
	m_spinTo.SetRange( 100, -1 );
	m_spinRepeat.SetRange( 100, -1 );

SCROLLINFO _si;
	::memset( &_si, 0, sizeof(SCROLLINFO) );
	_si.fMask = SIF_RANGE|SIF_PAGE|SIF_POS;
	_si.nMin = 0;
	_si.nMax = 2000;
	_si.nPage = 1;
	_si.nPos = 1000;
	m_sbSpeedRate.SetScrollInfo( &_si );

	m_checkPlayAutomatically.SetCheck( 1 );
	m_checkEmptyBackgroundOnStop.SetCheck( 1 );
	m_checkTransparentVideo.SetCheck( 1 );

	m_buttonTransparentColor.m_lParamCookie = IDC_BUTTON_TRANSPARENT_COLOR;
	m_buttonTransparentColor.m_clrDefault = m_buttonTransparentColor.m_clrSelected = COLORREF(-1L);
	m_buttonTransparentColor.m_sBtnTextColorDefault = _T("Auto-detect");

	m_buttonBackgroundColor.m_lParamCookie = IDC_BUTTON_BACKGROUND_COLOR;
	m_buttonBackgroundColor.m_clrDefault = m_buttonBackgroundColor.m_clrSelected = COLORREF(-1L);
	m_buttonBackgroundColor.m_sBtnTextColorDefault = _T("Themed background");

	m_editSelectFile.SetWindowText( _T("") );

	m_buttonResetSpeedRate.m_bUseStdCommandNotification = true;
	m_buttonPlay.m_bUseStdCommandNotification = true;
	m_buttonPause.m_bUseStdCommandNotification = true;
	m_buttonStop.m_bUseStdCommandNotification = true;
	m_buttonEmpty.m_bUseStdCommandNotification = true;
	m_buttonSizeFromContent.m_bUseStdCommandNotification = true;

	SubclassChildControls();
	OnChangeState();

	AddAnchor( IDC_ST_VIDEO, __RDA_LT, __RDA_RB );
	AddAnchor( IDC_STATIC_OPEN, __RDA_RT );
	AddAnchor( IDC_COMBO_OPEN, __RDA_RT );
	AddAnchor( IDC_STATIC_AVI_FRAME_COUNT, __RDA_RT );
	AddAnchor( IDC_BUTTON_PLAY, __RDA_RT );
	AddAnchor( IDC_BUTTON_PAUSE, __RDA_RT );
	AddAnchor( IDC_BUTTON_STOP, __RDA_RT );
	AddAnchor( IDC_BUTTON_EMPTY, __RDA_RT );
	AddAnchor( IDC_CHECK_USE_RANGE, __RDA_RT );
	AddAnchor( IDC_STATIC_FRAME_FROM, __RDA_RT );
	AddAnchor( IDC_EDIT_FRAME_FROM, __RDA_RT );
	AddAnchor( IDC_SPIN_FRAME_FROM, __RDA_RT );
	AddAnchor( IDC_STATIC_FRAME_TO, __RDA_RT );
	AddAnchor( IDC_EDIT_FRAME_TO, __RDA_RT );
	AddAnchor( IDC_SPIN_FRAME_TO, __RDA_RT );
	AddAnchor( IDC_STATIC_FRAME_REPEAT, __RDA_RT );
	AddAnchor( IDC_EDIT_FRAME_REPEAT, __RDA_RT );
	AddAnchor( IDC_SPIN_FRAME_REPEAT, __RDA_RT );
	AddAnchor( IDC_STATIC_IMAGE_MODE, __RDA_RT );
	AddAnchor( IDC_COMBO_IMAGE_MODE, __RDA_RT );
	AddAnchor( IDC_STATIC_ALIGN_H, __RDA_RT );
	AddAnchor( IDC_COMBO_ALIGN_H, __RDA_RT );
	AddAnchor( IDC_CHECK_CENTER_VERTICALLY, __RDA_RT );
	AddAnchor( IDC_BUTTON_SPEED_RATE_RESET, __RDA_RT );
	AddAnchor( IDC_STATIC_SPEED_RATE, __RDA_RT );
	AddAnchor( IDC_ZSB_SPEED_RATE, __RDA_RT );
	AddAnchor( IDC_STATIC_TT_TEXT, __RDA_RT );
	AddAnchor( IDC_EDIT_TT_TEXT, __RDA_RT );
	AddAnchor( IDC_CHECK_PLAY_AUTOMATICALLY, __RDA_RT );
	AddAnchor( IDC_CHECK_EMPTY_BACKGROUND_ON_STOP, __RDA_RT );
	AddAnchor( IDC_CHECK_TRANSPARENT_VIDEO, __RDA_RT );
	AddAnchor( IDC_BUTTON_TRANSPARENT_COLOR, __RDA_RT );
	AddAnchor( IDC_STATIC_BACKGROUND_COLOR, __RDA_RT );
	AddAnchor( IDC_BUTTON_BACKGROUND_COLOR, __RDA_RT );
	AddAnchor( IDC_CHECK_CONTROL_BORDER, __RDA_RT );
	AddAnchor( IDC_EDIT_SELECT_FILE, __RDA_RT );
	AddAnchor( IDC_BUTTON_SIZE_FROM_CONTENT, __RDA_RT );

	return TRUE;
}

void CPageAviLabel::OnComboOpen()
{
	if( m_comboOpen.GetSafeHwnd() == NULL )
		return;

int nAviIndex = m_comboOpen.GetCurSel();
	m_editSelectFile.SetWindowText( _T("") );
CString strSelFile;

	switch( nAviIndex )
	{
	case 0:
	{
		CString strPathAvi = _DoFileOpenDialog();
		if( ! strPathAvi.IsEmpty() )
		{
			m_wndAviLabel.AviOpen( LPCTSTR(strPathAvi) );
			strSelFile = _T("Selected file is: ") + strPathAvi;
		}
	}
	break;
	case 1:  m_wndAviLabel.AviOpen( IDR_AVI_DOWNLOAD ); break;
	case 2:  m_wndAviLabel.AviOpen( IDR_AVI_FILECOPY ); break;
	case 3:  m_wndAviLabel.AviOpen( IDR_AVI_FILEDEL  ); break;
	case 4:  m_wndAviLabel.AviOpen( IDR_AVI_FILEDELR ); break;
	case 5:  m_wndAviLabel.AviOpen( IDR_AVI_FILEMOVE ); break;
	case 6:  m_wndAviLabel.AviOpen( IDR_AVI_FILENUKE ); break;
	case 7:  m_wndAviLabel.AviOpen( IDR_AVI_FINDCOMP ); break;
	case 8:  m_wndAviLabel.AviOpen( IDR_AVI_FINDFILE ); break;
	case 9:  m_wndAviLabel.AviOpen( IDR_AVI_SEARCH   ); break;
	default: m_wndAviLabel.AviOpen( IDR_AVI_DOWNLOAD ); break;
	} // switch( nAviIndex )
	if( ! m_wndAviLabel.AviIsEmpty() )
	{
		UINT nFrameCount = m_wndAviLabel.AviFrameGetCount();
		CString strFrameCount;
		strFrameCount.Format( _T("Frame count: %d"), nFrameCount );
		m_staticAviFrameCount.SetWindowText( LPCTSTR(strFrameCount) );
		if( ! strSelFile.IsEmpty() )
			m_editSelectFile.SetWindowText( strSelFile );
		else
			m_editSelectFile.SetWindowText( m_wndAviLabel.AviTempFileNameGet() );
	}
	OnChangeState();
}

void CPageAviLabel::OnChangeState()
{
	if( m_comboOpen.GetSafeHwnd() == NULL )
		return;

bool bPlaying = m_wndAviLabel.AviIsPlaying();

	int nImageModeIndex = m_comboImageMode.GetCurSel();
	switch( nImageModeIndex )
	{
		case 0:
			m_wndAviLabel.SetImageMode( CExtLabel::eAlign );
		break;
		case 1:
			m_wndAviLabel.SetImageMode( CExtLabel::eTile );
		break;
		case 2:
			m_wndAviLabel.SetImageMode( CExtLabel::eStretch );
		break;
		case 3:
			m_wndAviLabel.SetImageMode( CExtLabel::eTouchInside );
		break;
		case 4:
			m_wndAviLabel.SetImageMode( CExtLabel::eTouchOutside );
		break;
		default:
			m_wndAviLabel.SetImageMode( CExtLabel::eAlign );
		break;
	}

	int nAlignIndex = m_comboAlignH.GetCurSel();
	switch( nAlignIndex )
	{
		case 0:
			m_wndAviLabel.ModifyStyle( (SS_CENTER|SS_RIGHT), SS_LEFT );
		break;
		case 1:
			m_wndAviLabel.ModifyStyle( (SS_LEFT|SS_RIGHT), SS_CENTER );
		break;
		case 2:
			m_wndAviLabel.ModifyStyle( (SS_LEFT|SS_CENTER), SS_RIGHT );
		break;
		default:
			m_wndAviLabel.ModifyStyle( (SS_CENTER|SS_RIGHT), SS_LEFT );
		break;
	}

	if( m_checkCenterVertically.GetCheck() != 0 )
		m_wndAviLabel.ModifyStyle( 0, SS_CENTERIMAGE );
	else
		m_wndAviLabel.ModifyStyle( SS_CENTERIMAGE, 0 );

CString strToolTip;
	m_editToolTipText.GetWindowText( strToolTip );
	m_wndAviLabel.SetTooltipText( LPCTSTR(strToolTip) );

	if( m_checkUseRange.GetCheck() != 0 )
	{
		m_editFrameTo.EnableWindow( TRUE );
		m_editFrameRepeat.EnableWindow( TRUE );
		m_editFrameFrom.EnableWindow( TRUE );
		GetDlgItem( IDC_SPIN_FRAME_FROM )->EnableWindow( TRUE );
		GetDlgItem( IDC_SPIN_FRAME_TO )->EnableWindow( TRUE );
		GetDlgItem( IDC_SPIN_FRAME_REPEAT )->EnableWindow( TRUE );
		GetDlgItem( IDC_STATIC_FRAME_FROM )->EnableWindow( TRUE );
		GetDlgItem( IDC_STATIC_FRAME_TO )->EnableWindow( TRUE );
		GetDlgItem( IDC_STATIC_FRAME_REPEAT )->EnableWindow( TRUE );
		CString strFrom, strTo, strRepeat;
		m_editFrameFrom.GetWindowText( strFrom );
		m_editFrameTo.GetWindowText( strTo );
		m_editFrameRepeat.GetWindowText( strRepeat );
		m_nFrom = _ttoi( strFrom );
		m_nTo = _ttoi( strTo );
		m_nRep = _ttoi( strRepeat );
	}
	else
	{
		m_editFrameTo.EnableWindow( FALSE );
		m_editFrameRepeat.EnableWindow( FALSE );
		m_editFrameFrom.EnableWindow( FALSE );
		GetDlgItem( IDC_SPIN_FRAME_FROM )->EnableWindow( FALSE );
		GetDlgItem( IDC_SPIN_FRAME_TO )->EnableWindow( FALSE );
		GetDlgItem( IDC_SPIN_FRAME_REPEAT )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_FRAME_FROM )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_FRAME_TO )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_FRAME_REPEAT )->EnableWindow( FALSE );
		m_nFrom = 0;
		m_nTo = -1;
		m_nRep = -1;
	}

DWORD dwStyle = __EXT_AVI_DEFAULT_STYLES;
	if( m_checkEmptyBackgroundOnStop.GetCheck() != 0 )
		dwStyle |= __EXT_AVI_EMPTY_BACKGROUND_ON_STOP;
	
	if( m_checkPlayAutomatically.GetCheck() != 0 )
		bPlaying = true;
	
	if( m_checkTransparentVideo.GetCheck() != 0 )
		dwStyle |= __EXT_AVI_TRANSPARENT_VIDEO;
	m_buttonTransparentColor.EnableWindow( ( ( dwStyle & __EXT_AVI_TRANSPARENT_VIDEO ) != 0 ) ? TRUE : FALSE );
	
	if( dwStyle != __EXT_AVI_DEFAULT_STYLES )
		m_wndAviLabel.AviStyleSet( dwStyle );

	m_buttonResetSpeedRate.EnableWindow( ( m_wndAviLabel.AviSpeedRateGet() == 1.0 ) ? false : true );

BOOL bEnableAlignmentControls = ( m_wndAviLabel.GetImageMode() == CExtLabel::eAlign ) ? TRUE : FALSE;
	m_comboAlignH.EnableWindow( bEnableAlignmentControls );
	GetDlgItem( IDC_STATIC_ALIGN_H )->EnableWindow( bEnableAlignmentControls );
	m_checkCenterVertically.EnableWindow( bEnableAlignmentControls );

	if( bPlaying )
		m_wndAviLabel.AviPlay( m_nFrom, m_nTo, m_nRep );

CString strSpeedRate;
	strSpeedRate.Format( _T("Speed rate: %.3lf"), m_wndAviLabel.AviSpeedRateGet() );
	m_staticSpeedRate.SetWindowText( LPCTSTR(strSpeedRate) );
}

void CPageAviLabel::OnButtonPlay() 
{
	m_wndAviLabel.AviPlay( m_nFrom, m_nTo, m_nRep );
}

void CPageAviLabel::OnButtonPause() 
{
	m_wndAviLabel.AviStop();
}

void CPageAviLabel::OnButtonStop() 
{
	m_wndAviLabel.AviStop( true );
}

void CPageAviLabel::OnButtonEmpty() 
{
	m_wndAviLabel.AviEmpty();
	m_comboOpen.SetCurSel( 0 );
	m_staticAviFrameCount.SetWindowText( _T("AVI frame count is: 0") );
	m_editSelectFile.SetWindowText( _T("") );
}

void CPageAviLabel::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar * pScrollBar )
{
	if( LPVOID(pScrollBar) == LPVOID(&m_sbSpeedRate) )
	{
		SCROLLINFO _si;
		::memset( &_si, 0, sizeof(SCROLLINFO) );
		_si.fMask = SIF_POS;
		m_sbSpeedRate.GetScrollInfo( &_si );
		INT nSliderPos = _si.nPos;
		nSliderPos = max( 0, nSliderPos );
		nSliderPos = min( 2000, nSliderPos );
		DOUBLE lfSpeedRate = 1.0;
		if( nSliderPos < 1000 )
			lfSpeedRate =  double(nSliderPos) / 1000.0 * ( 1.0 - __EXT_MFC_AVI_SPEED_RATE_MIN ) + __EXT_MFC_AVI_SPEED_RATE_MIN;
		else if( nSliderPos > 1000 )
			lfSpeedRate =  double(nSliderPos-1000) * (__EXT_MFC_AVI_SPEED_RATE_MAX - 1.0) / 1000.0 + 1.0;
		m_wndAviLabel.AviSpeedRateSet( lfSpeedRate );
		m_buttonResetSpeedRate.EnableWindow( ( lfSpeedRate == 1.0 ) ? false : true );
		CString strSpeedRate;
		strSpeedRate.Format( _T("Speed rate: %.3lf"), lfSpeedRate );
		m_staticSpeedRate.SetWindowText( LPCTSTR(strSpeedRate) );
		return;
	}
	CPageBase::OnHScroll( nSBCode, nPos, pScrollBar );
}

void CPageAviLabel::OnButtonSpeedRateReset()
{
SCROLLINFO _si;
	::memset( &_si, 0, sizeof(SCROLLINFO) );
	_si.fMask = SIF_POS;
	_si.nPos = 1000;
	m_sbSpeedRate.SetScrollInfo( &_si );
	m_wndAviLabel.AviSpeedRateSet( 1.0 );
	OnChangeState();
}

LRESULT CPageAviLabel::OnNotifyColorChangedFinally( WPARAM wParam, LPARAM lParam )
{
	if( lParam == IDC_BUTTON_TRANSPARENT_COLOR )
	{
		COLORREF clr = COLORREF(wParam);
		m_wndAviLabel.AviTransparentColorSet( clr );
		m_wndAviLabel.Invalidate();
		return 0L;
	}
	if( lParam == IDC_BUTTON_BACKGROUND_COLOR )
	{
		COLORREF clr = COLORREF(wParam);
		m_wndAviLabel.SetBkColor( clr );
		m_wndAviLabel.Invalidate();
		return 0L;
	}
	return 0L;
}

void CPageAviLabel::OnCheckControlBorder() 
{
bool bBorder = ( m_checkControlBorder.GetCheck() != 0 ) ? true : false;
static const DWORD g_dwBorderExStyles = WS_EX_CLIENTEDGE|WS_EX_DLGMODALFRAME;
DWORD dwExStyleAdd = bBorder ? g_dwBorderExStyles : 0;
DWORD dwExStyleRemove = bBorder ? 0 : g_dwBorderExStyles;
	 m_wndAviLabel.ModifyStyleEx( dwExStyleRemove, dwExStyleAdd, SWP_FRAMECHANGED );
}

void CPageAviLabel::OnSize(UINT nType, int cx, int cy) 
{
	CPageBase::OnSize(nType, cx, cy);
/*
	if( m_sliderSpeedRate.GetSafeHwnd() != NULL )	
	{
		m_sliderSpeedRate.Invalidate( TRUE );
		m_sliderSpeedRate.UpdateWindow();
	}
*/
}

CString CPageAviLabel::_DoFileOpenDialog()
{

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile( this, CExtShellDialogFile::__EFDT_OPEN_SINGLE );
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("Avi (*.avi)|*.avi|")
		_T("All Files (*.*)|*.*|")
		_T("|")
		);
INT nRetVal = (INT)dlgShellFile.DoModal();
	if( nRetVal != IDOK )
		return _T("");
CExtSafeString strFilePath = dlgShellFile.m_arrRetValNames.ElementAt( 0 );
	ASSERT( ! strFilePath.IsEmpty() );

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog dlgFile(
		TRUE,
		_T("bmp"),
		_T(""),
		OFN_ENABLESIZING|OFN_EXPLORER
			|OFN_LONGNAMES|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST
			|OFN_NOCHANGEDIR|OFN_NODEREFERENCELINKS|OFN_HIDEREADONLY
			,
		_T("AVI files (*.avi)|*.avi|")
		_T("All Files (*.*)|*.*|")
		_T("|"),
		this
		);
	dlgFile.m_ofn.lpstrTitle = _T("Open AVI file");
	if( dlgFile.DoModal() != IDOK )
		return _T("");
CExtSafeString strFilePath = dlgFile.m_ofn.lpstrFile;
	ASSERT( ! strFilePath.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

	return LPCTSTR(strFilePath);
}

void CPageAviLabel::OnButtonSizeFromContent() 
{
	m_wndAviLabel.AviSizeFromContent();
}

