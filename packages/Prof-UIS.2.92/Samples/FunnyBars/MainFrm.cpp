// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "FunnyBars.h"
#include "MainFrm.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFunnyToolButton

IMPLEMENT_DYNAMIC( CFunnyToolButton, CExtBarButton );

CFunnyToolButton::CFunnyToolButton(
	CExtToolControlBar * pBar,
	UINT nCmdID,
	LPCTSTR strFunnyText,
	double lfHotPercent
	) : CExtBarButton( pBar, nCmdID, 0 )
	, m_strFunnyText( (strFunnyText==NULL) ? _T("") : strFunnyText )
	, m_sizeFunnyTextCalc( 0, 0 )
	, m_sizeFunnyTextWell( 0, 0 )
	, m_lfHotPercent( lfHotPercent )
{
}

CSize CFunnyToolButton::CalculateLayout(
	CDC & dc,
	CSize sizePreCalc,
	BOOL bHorz
	)
{
	ASSERT_VALID( this );
CSize _size = CExtBarButton::CalculateLayout( dc, sizePreCalc, bHorz );
	ASSERT( !IsSeparator() );
	if( ! m_strFunnyText.IsEmpty() )	
	{
		CFont * pOldFont = dc.SelectObject( &g_PaintManager->m_FontNormal );
		CRect rcMeasured( 0, 0, 0, 0 );
		CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(m_strFunnyText), &rcMeasured, DT_LEFT|DT_CALCRECT, 0 );
		m_sizeFunnyTextWell = rcMeasured.Size();
		m_sizeFunnyTextWell.cx += __EXT_TB_BUTTON_TEXT_MARGIN*2;
		static CString strMeasureTemplate( _T("AapqWZz\nAapqWZz") );
		CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(strMeasureTemplate), &rcMeasured, DT_LEFT|DT_CALCRECT, 0 );
		dc.SelectObject( pOldFont );
		m_sizeFunnyTextCalc = rcMeasured.Size();
		m_sizeFunnyTextCalc.cx += __EXT_TB_BUTTON_TEXT_MARGIN*2;
		m_sizeFunnyTextCalc.cx = max( m_sizeFunnyTextWell.cx, m_sizeFunnyTextCalc.cy );
		m_sizeFunnyTextCalc.cy = max( m_sizeFunnyTextWell.cy, m_sizeFunnyTextCalc.cy );
		m_sizeFunnyTextCalc.cx += __EXT_TB_BUTTON_INNER_MARGIN*2;
		_size.cx = max( m_sizeFunnyTextCalc.cx, _size.cx );
		_size.cy += m_sizeFunnyTextCalc.cy + 4;
	} // if( ! m_strFunnyText.IsEmpty() )	
	m_ActiveSize = _size;
	return m_ActiveSize;
}

void CFunnyToolButton::PaintCompound(
	CDC & dc,
	bool bPaintParentChain,
	bool bPaintChildren,
	bool bPaintOneNearestChildrenLevelOnly
	)
{
	ASSERT_VALID( this );
	ASSERT( dc.GetSafeHdc() != NULL );
CExtToolControlBar * pBar = GetBar();
	ASSERT_VALID( pBar );
	if( (m_pCtrl != NULL) && (!m_bVertDocked || GetCtrlVisibleVertically()) )
		return;
	ASSERT( ! IsSeparator() );
	if( ! IsPaintAble( dc ) )
		return;
	if( AnimationClient_StatePaint( dc ) )
		return;
	if( bPaintParentChain )
		PaintParentChain( dc );

CRect rcArea( m_ActiveRect );
	rcArea.DeflateRect( __EXT_TB_BUTTON_INNER_MARGIN, __EXT_TB_BUTTON_INNER_MARGIN );
	if(		(!IsVisible())
		||	rcArea.right <= rcArea.left || rcArea.bottom <= rcArea.top
		||	(GetStyle() & TBBS_HIDDEN) != 0
		||	(!dc.RectVisible(&m_ActiveRect))
		)
		return;
bool bPushed = IsPressed() ? true : false;
bool bEnabled = IsDisabled() ? false : true;
bool bHover =
		(	IsHover()
			&& (! CExtToolControlBar::g_bMenuTracking )
			&& (! CExtPopupMenuWnd::IsMenuTracking() )
		) ? true : false;
bool bIndeterminate = IsIndeterminate() ? true : false;
CExtCmdIcon * pIcon = GetIconPtr();
	if( m_lfHotPercent >= -1.0 && pIcon != NULL && (! pIcon->IsEmpty() ) )
	{
		ASSERT( m_lfHotPercent <= 1.0 );
		if( m_iconCold.IsEmpty() )
		{
			m_iconCold.m_bmpNormal = pIcon->m_bmpNormal;
			COLORREF clrIconAlphaAccent =
				pBar->PmBridge_GetPM()->GetIconAlphaColor();
			if( clrIconAlphaAccent != COLORREF(-1L) )
			{
				double H = 0.0, S = 0.0, L = 0.0;
				CExtBitmap::stat_RGBtoHSL( clrIconAlphaAccent, &H, &S, &L );
				S = 0.3;
				clrIconAlphaAccent = CExtBitmap::stat_HLStoRGB( H, L, S );
			} // if( clrIconAlphaAccent != COLORREF(-1L) )
			m_iconCold.m_bmpNormal.MakeMono( clrIconAlphaAccent );
			m_iconCold.m_bmpNormal.AdjustHLS(
				COLORREF(-1L),
				COLORREF(-1L),
				0.0,
				m_lfHotPercent,
				0.0
				);
			m_iconCold.m_bmpNormal.AdjustAlpha( -0.5 );
		} // if( m_iconCold.IsEmpty() )
		if( (! bHover) && (! bPushed) && (! m_iconCold.IsEmpty() )  )
			pIcon = &m_iconCold;
	} // if( m_lfHotPercent >= -1.0 && pIcon != NULL && (! pIcon->IsEmpty() ) )

bool bHorz = true;
CExtPaintManager::PAINTPUSHBUTTONDATA _ppbd(
		this, 
		bHorz, 
		rcArea, 
		NULL, 
		pIcon,
		true, 
		bHover, 
		bPushed, 
		bIndeterminate, 
		bEnabled,
		true, 
		false, 
		false, 
		CExtPaintManager::__ALIGN_HORIZ_CENTER, 
		NULL,
		(	IsAbleToTrackMenu()
			&& (!m_pBar->IsKindOf(RUNTIME_CLASS(CExtMenuControlBar)))
			) ? true : false,
		0, ( bEnabled && (!bHover) && (!bPushed) )
		);
	
	_ppbd.m_rcBorderSizes = OnQueryBorderSizes( bHorz );
	_ppbd.m_nIconAlignment = CExtPaintManager::__ALIGN_HORIZ_CENTER | CExtPaintManager::__ALIGN_VERT_TOP;
	_ppbd.m_nTextAlignment = CExtPaintManager::__ALIGN_HORIZ_CENTER | CExtPaintManager::__ALIGN_VERT_BOTTOM;
	_ppbd.m_rcIconMargins = OnQueryMarginsIcon( bHorz );
	_ppbd.m_rcTextMargins = OnQueryMarginsText( bHorz );

	if( m_bSeparatedDropDown )
	{
		_ppbd.m_bSeparatedDropDown = true;
		if(		(m_bDropDownHT || CExtToolControlBar::g_bMenuTracking)
			&&	bPushed
			)
			_ppbd.m_bPushedDropDown = true;
	} // if( m_bSeparatedDropDown )
	g_PaintManager->PaintPushButton( dc, _ppbd );
CRect rcText( rcArea );
	rcText.DeflateRect( __EXT_TB_BUTTON_OUTER_MARGIN, __EXT_TB_BUTTON_OUTER_MARGIN+__EXT_TB_BUTTON_INNER_MARGIN  );
	rcText.top = rcText.bottom - m_sizeFunnyTextCalc.cy;
	rcText.OffsetRect(
		( m_sizeFunnyTextCalc.cx - m_sizeFunnyTextWell.cx ) / 2,
		( m_sizeFunnyTextCalc.cy - m_sizeFunnyTextWell.cy ) / 2
		);
	if( bPushed )
		rcText.OffsetRect( g_PaintManager->GetPushedOffset() );
CFont fontTmp;
CFont * pOldFont = NULL;
COLORREF clrText = 
		bEnabled
			? (	( m_lfHotPercent >= -1.0f )
				? g_PaintManager->GetColor(
					(bHover || bPushed) ? COLOR_HIGHLIGHT : COLOR_BTNTEXT,
					this
					)
				: dc.GetNearestColor(
					(bHover || bPushed) ? ( RGB(255,64,64) ) : ( RGB(64,64,255) )
					)
				)
			: g_PaintManager->GetColor( COLOR_3DSHADOW, this )
			;
	if( bEnabled && bHover || bPushed )
	{
		LOGFONT lf;
		VERIFY( g_PaintManager->m_FontNormal.GetLogFont(&lf) );
		lf.lfUnderline = TRUE;
		VERIFY( fontTmp.CreateFontIndirect(&lf) );
		dc.SelectObject( &fontTmp );
	}
	else
		dc.SelectObject( &g_PaintManager->m_FontNormal );
int nOldBkMode = dc.SetBkMode( TRANSPARENT );
COLORREF clrTextOld = dc.SetTextColor( clrText );
	CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(m_strFunnyText), &rcText, DT_CENTER|DT_VCENTER, 0 );
	dc.SetTextColor( clrTextOld );
	dc.SetBkMode( nOldBkMode );
	dc.SelectObject( pOldFont );

	if( bPaintChildren )
		PaintChildren( dc, bPaintOneNearestChildrenLevelOnly );
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC( CMainFrame, CFrameWnd )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWnd )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP

	ON_COMMAND( ID_256_FILE_NEW, OnAnyCommand )
	ON_COMMAND( ID_256_FILE_OPEN, OnAnyCommand )
	ON_COMMAND( ID_256_FILE_FAVORITES, OnAnyCommand )
	ON_COMMAND( ID_256_FILE_ADD, OnAnyCommand )
	ON_COMMAND( ID_256_FILE_EXTRACT, OnAnyCommand )
	ON_COMMAND( ID_256_FILE_VIEW, OnAnyCommand )
	ON_COMMAND( ID_256_FILE_CHECK_OUT, OnAnyCommand )
	ON_COMMAND( ID_256_FILE_WIZARD, OnAnyCommand )
	ON_COMMAND( ID_TRUE_FILE_NEW, OnAnyCommand )
	ON_COMMAND( ID_TRUE_FILE_OPEN, OnAnyCommand )
	ON_COMMAND( ID_TRUE_FILE_FAVORITES, OnAnyCommand )
	ON_COMMAND( ID_TRUE_FILE_ADD, OnAnyCommand )
	ON_COMMAND( ID_TRUE_FILE_EXTRACT, OnAnyCommand )
	ON_COMMAND( ID_TRUE_FILE_VIEW, OnAnyCommand )
	ON_COMMAND( ID_TRUE_FILE_CHECK_OUT, OnAnyCommand )
	ON_COMMAND( ID_TRUE_FILE_WIZARD, OnAnyCommand )
	ON_COMMAND( ID_VISTA_RECYCLE_BIN_EMPTY, OnAnyCommand )
	ON_COMMAND( ID_VISTA_RECYCLE_BIN_FULL, OnAnyCommand )
	ON_COMMAND( ID_VISTA_PICS, OnAnyCommand )
	ON_COMMAND( ID_VISTA_PARENTAL_CONTROL, OnAnyCommand )
	ON_COMMAND( ID_VISTA_SEARCH, OnAnyCommand )
	ON_COMMAND( ID_VISTA_MENU_RECYCLE_BIN_EMPTY, OnAnyCommand )
	ON_COMMAND( ID_VISTA_MENU_RECYCLE_BIN_FULL, OnAnyCommand )
	ON_COMMAND( ID_VISTA_MENU_PICS, OnAnyCommand )
	ON_COMMAND( ID_VISTA_MENU_PARENTAL_CONTROL, OnAnyCommand )
	ON_COMMAND( ID_VISTA_MENU_SEARCH, OnAnyCommand )

	ON_UPDATE_COMMAND_UI( ID_256_FILE_NEW, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_256_FILE_OPEN, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_256_FILE_FAVORITES, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_256_FILE_ADD, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_256_FILE_EXTRACT, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_256_FILE_VIEW, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_256_FILE_CHECK_OUT, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_256_FILE_WIZARD, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_TRUE_FILE_NEW, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_TRUE_FILE_OPEN, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_TRUE_FILE_FAVORITES, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_TRUE_FILE_ADD, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_TRUE_FILE_EXTRACT, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_TRUE_FILE_VIEW, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_TRUE_FILE_CHECK_OUT, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_TRUE_FILE_WIZARD, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_VISTA_RECYCLE_BIN_EMPTY, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_VISTA_RECYCLE_BIN_FULL, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_VISTA_PICS, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_VISTA_PARENTAL_CONTROL, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_VISTA_SEARCH, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_VISTA_MENU_RECYCLE_BIN_EMPTY, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_VISTA_MENU_RECYCLE_BIN_FULL, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_VISTA_MENU_PICS, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_VISTA_MENU_PARENTAL_CONTROL, OnUpdateAnyCommand )
	ON_UPDATE_COMMAND_UI( ID_VISTA_MENU_SEARCH, OnUpdateAnyCommand )

	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR_SMALL_256_COLOR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_SMALL_256_COLOR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR_BIG_TRUE_COLOR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_BIG_TRUE_COLOR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR_BIG_TRUE_COLOR2, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_BIG_TRUE_COLOR2, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR_BIG_TRUE_COLOR3, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_BIG_TRUE_COLOR3, OnUpdateControlBarMenu)

	ON_COMMAND_EX(IDR_TOOLBAR_VISTA_ICONS, OnBarCheck )
	ON_UPDATE_COMMAND_UI(IDR_TOOLBAR_VISTA_ICONS, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_UI_LOOK_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_UI_LOOK_BAR, OnUpdateControlBarMenu)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
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
			RUNTIME_CLASS(CExtPaintManagerOffice2007_R2_LunaBlue)
			);
	
	CExtPopupMenuWnd::g_bMenuExpanding = false;
	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;

	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( CExtNCW < CFrameWnd > :: OnCreate( lpCreateStruct ) == -1 )
		return -1;

HICON hIcon = (::AfxGetApp())->LoadIcon(IDR_MAINFRAME);
	ASSERT( hIcon != NULL );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

CCreateContext _cc;
	_cc.m_pCurrentDoc = NULL;
	_cc.m_pCurrentFrame = this;
	_cc.m_pLastView = NULL;
	_cc.m_pNewDocTemplate = NULL;
	_cc.m_pNewViewClass = RUNTIME_CLASS( CChildView );
CWnd * pWndView = CreateView( &_cc );
	if( pWndView == NULL )
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	ASSERT_KINDOF( CChildView, pWndView );
CChildView * pChildView =
		STATIC_DOWNCAST( CChildView, pWndView );
	SetActiveView( pChildView );
	pWndView->ModifyStyleEx( WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME, 0, SWP_FRAMECHANGED );

CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	ASSERT( pApp->m_pszProfileName != NULL );
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
	VERIFY(
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_MAINFRAME
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_TOOLBAR_SMALL_256_COLOR,
			NULL,
			NULL,
			false,
			true,
			RGB(188,190,188)
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_TOOLBAR_BIG_TRUE_COLOR
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_TOOLBAR_VISTA_ICONS_SMALL,
			NULL,
			NULL,
			false,
			true,
			COLORREF( -1L )
			)
		);
CExtCmdItem * pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName, ID_APP_ABOUT );
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = pCmdItem->m_sMenuText;
	pCmdItem->m_sToolbarText.Replace( __EXT_MFC_SAFE_LPCTSTR( _T("&") ), __EXT_MFC_SAFE_LPCTSTR( _T("") ) );

	pCmdItem =
		g_CmdManager->CmdGetPtr(
			pApp->m_pszProfileName, ID_APP_EXIT );
	ASSERT( pCmdItem != NULL );
	pCmdItem->m_sToolbarText = pCmdItem->m_sMenuText;
	pCmdItem->m_sToolbarText.Replace( __EXT_MFC_SAFE_LPCTSTR( _T("&") ), __EXT_MFC_SAFE_LPCTSTR( _T("") ) );

	if( !m_wndMenuBar.Create(
			NULL, // _T("Menu Bar"),
			this,
			ID_VIEW_MENUBAR
			)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }

	if(		(! m_wndToolBarStd.Create(
				_T("Standard ToolBar"), this, AFX_IDW_TOOLBAR
				) )
		||	(! m_wndToolBarStd.LoadToolBar(IDR_MAINFRAME) )
		)
	{
		TRACE0("Failed to create standard toolbar\n");
		return -1;      // fail to create
	}

	if(		(! m_wndToolBar256.Create(
				_T("256 Color (16x16 buttons)"), this, IDR_TOOLBAR_SMALL_256_COLOR
				) )
			||	(! m_wndToolBar256.LoadToolBar(IDR_TOOLBAR_SMALL_256_COLOR,RGB(188,190,188)) )
		)
	{
		TRACE0("Failed to create 256-color toolbar\n");
		return -1;      // fail to create
	}

	if(		(! m_wndToolBarTrue.Create(
				_T("16777216 Color (44x40 buttons)"), this, IDR_TOOLBAR_BIG_TRUE_COLOR
				) )
		||	(! m_wndToolBarTrue.LoadToolBar(IDR_TOOLBAR_BIG_TRUE_COLOR) )
		)
	{
		TRACE0("Failed to create true-color toolbar\n");
		return -1;      // fail to create
	}
	
	if(		(! m_wndToolBarTruePlusText.Create(
				_T("Text + 16777216 Color (44x40 buttons)"), this, IDR_TOOLBAR_BIG_TRUE_COLOR2
				) )
		)
	{
		TRACE0("Failed to create true-color toolbar with text\n");
		return -1;      // fail to create
	}
	if(		(! m_wndToolBarTrueHot.Create(
				_T("Hot + 16777216 Color (44x40 buttons)"), this, IDR_TOOLBAR_BIG_TRUE_COLOR3
				) )
		)
	{
		TRACE0("Failed to create hot-true-color toolbar\n");
		return -1;      // fail to create
	}
	if(		(! m_wndToolBarVista.Create(
				_T("Vista Icons (RGB/A 32 bits with Alpha Channel)"), this, IDR_TOOLBAR_VISTA_ICONS
				) )
		||	(! m_wndToolBarVista.LoadToolBar(IDR_TOOLBAR_VISTA_ICONS, COLORREF( -1L ) ) )
		)
	{
		TRACE0("Failed to create Vista Icons toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBarVista.SetInitDesiredSizeFloating( CSize( 500, 100 ) );

	if(		(! m_wndToolBarUiLook.Create( NULL, this, ID_VIEW_UI_LOOK_BAR ) )
		||	(! m_wndToolBarUiLook.ThemeSwitcherInit() )
		)
	{
		TRACE0("Failed to create Vista Icons toolbar\n");
		return -1;      // fail to create
	}
	
int nCount = m_wndToolBarTrue.GetButtonsCount();
	for( int i = 0; i < nCount; i++ )
	{
		CExtBarButton * pTBB =
			m_wndToolBarTrue.GetButton( i );
		ASSERT_VALID( pTBB );
		if( pTBB->IsKindOf( RUNTIME_CLASS(CExtBarContentExpandButton) ) )
			continue;
		if( pTBB->IsSeparator() )
		{
			m_wndToolBarTruePlusText.InsertButton(
				-1,
				ID_SEPARATOR,
				FALSE
				);
			m_wndToolBarTrueHot.InsertButton(
				-1,
				ID_SEPARATOR,
				FALSE
				);
			continue;
		} // if( pTBB->IsSeparator() )
		CString strFunnyText( _T("") );
		switch( pTBB->GetCmdID() )
		{
		case ID_TRUE_FILE_NEW:
			strFunnyText = _T("New file\nentry ...");
			break;
		case ID_TRUE_FILE_OPEN:
			strFunnyText = _T("Open file\nentry ...");
			break;
		case ID_TRUE_FILE_FAVORITES:
			strFunnyText = _T("Organize\nfavorites");
			break;
		case ID_TRUE_FILE_ADD:
			strFunnyText = _T("Add\nitem");
			break;
		case ID_TRUE_FILE_EXTRACT:
			strFunnyText = _T("Extact\nfile");
			break;
		case ID_TRUE_FILE_VIEW:
			strFunnyText = _T("View\nselected");
			break;
		case ID_TRUE_FILE_CHECK_OUT:
			strFunnyText = _T("Check Out");
			break;
		case ID_TRUE_FILE_WIZARD:
			strFunnyText = _T("Wizard");
			break;
		} // switch( pTBB->GetCmdID() )
		CFunnyToolButton * pSpecTBB =
			new CFunnyToolButton(
				&m_wndToolBarTruePlusText,
				pTBB->GetCmdID(),
				(LPCTSTR)strFunnyText,
				-2.0f
				);
		m_wndToolBarTruePlusText.InsertSpecButton(
			-1,
			pSpecTBB,
			FALSE
			);
		pSpecTBB =
			new CFunnyToolButton(
				&m_wndToolBarTrueHot,
				pTBB->GetCmdID(),
				(LPCTSTR)strFunnyText,
				0.25
				);
		m_wndToolBarTrueHot.InsertSpecButton(
			-1,
			pSpecTBB,
			FALSE
			);
	} // for( int i = 0; i < nCount; i++ )
	m_wndToolBarTruePlusText.InitContentExpandButton();
	m_wndToolBarTrueHot.InitContentExpandButton();
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarStd.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar256.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarTrue.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarTruePlusText.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarTrueHot.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarVista.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarUiLook.EnableDocking(CBRS_ALIGN_ANY);
	
	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}

	if(	!CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{
		DockControlBar( &m_wndMenuBar, AFX_IDW_DOCKBAR_TOP );
		DockControlBar( &m_wndToolBarStd, AFX_IDW_DOCKBAR_TOP );
		DockControlBar( &m_wndToolBar256, AFX_IDW_DOCKBAR_TOP );
		DockControlBar( &m_wndToolBarTrue, AFX_IDW_DOCKBAR_LEFT );
		DockControlBar( &m_wndToolBarTruePlusText, AFX_IDW_DOCKBAR_BOTTOM );
		DockControlBar( &m_wndToolBarTrueHot, AFX_IDW_DOCKBAR_TOP );
		FloatControlBar( &m_wndToolBarVista, CPoint(300,300) );
		DockControlBar( &m_wndToolBarUiLook, AFX_IDW_DOCKBAR_RIGHT );
	}
	RecalcLayout();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CExtNCW < CFrameWnd > :: PreCreateWindow( cs ) )
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CExtNCW < CFrameWnd > :: AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CExtNCW < CFrameWnd > :: Dump( dc );
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	pOldWnd;
	// forward focus to the view window
CView * pView = GetActiveView();
	if( pView != NULL )
		pView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
CView * pView = GetActiveView();
	if( pView != NULL )
	{
		if( pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) )
			return TRUE;
	}
	// otherwise, do default handling
	return CExtNCW < CFrameWnd > :: OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}


void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CExtNCW < CFrameWnd > :: ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	CExtNCW < CFrameWnd > :: ActivateFrame( nCmdShow );
}

BOOL CMainFrame::DestroyWindow() 
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

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
	
	return CExtNCW < CFrameWnd > :: DestroyWindow();
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndToolBarUiLook.PreTranslateMessage( pMsg ) )
		return TRUE;
	if( m_wndMenuBar.TranslateMainFrameMessage( pMsg ) )
		return TRUE;
	return CExtNCW < CFrameWnd > :: PreTranslateMessage(pMsg);
}

void CMainFrame::OnAnyCommand()
{
}
void CMainFrame::OnUpdateAnyCommand( CCmdUI* pCmdUI )
{
	pCmdUI->Enable( TRUE );
}

