// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "FunnyBars.h"
#include "ChildView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int CALLBACK stat_FontEnumProc(
	ENUMLOGFONTEX * lpelfe,
	NEWTEXTMETRICEX * lpntme, 
	int FontType,
	CComboBox * pCombo
	)
{
	lpntme;
	FontType;
    if(	pCombo->FindStringExact(
			0,
			lpelfe->elfLogFont.lfFaceName
			) != CB_ERR
		)
	    return 1;
	pCombo->AddString( lpelfe->elfLogFont.lfFaceName );
    return 1;
}

/////////////////////////////////////////////////////////////////////////////
// CChildView

IMPLEMENT_DYNCREATE( CChildView, CFormView );

CChildView::CChildView()
	: CExtNCSB < CExtWS < CExtAFV < CFormView > > > ( CChildView::IDD, ((CWnd *)NULL), true )
	, m_bInitializingControls( false )
{
	
	::memset( &m_lf, 0, sizeof(LOGFONT) );
	ASSERT( g_PaintManager->m_FontNormal.GetSafeHandle() != NULL );
	g_PaintManager->m_FontNormal.GetLogFont( &m_lf );
}

CChildView::~CChildView()
{
}

void CChildView::DoDataExchange(CDataExchange* pDX)
{
	CExtNCSB < CExtWS < CExtAFV < CFormView > > > :: DoDataExchange(pDX);
	//{{AFX_DATA_MAP(	CChildView)
	DDX_Control(pDX, IDC_STATIC_FONT_WIDTH, m_wndLabelFontWidth);
	DDX_Control(pDX, IDC_STATIC_FONT_HEIGHT, m_wndLabelFontHeight);
	DDX_Control(pDX, IDC_STATIC_FONT_FACE_NAME, m_wndLabelFontFaceName);
	DDX_Control(pDX, IDC_GROUP_BOX_FONT, m_wndBoxFont);
	DDX_Control(pDX, IDC_GROUP_BOX_BRIGHTNESS, m_wndBoxBrightness);
	DDX_Control(pDX, IDC_SLIDER_HTBB, m_wndSliderHTBB);
	DDX_Control(pDX, IDC_EDIT_FONT_WIDTH, m_EditFontWidth);
	DDX_Control(pDX, IDC_EDIT_FONT_HEIGHT, m_EditFontHeight);
	DDX_Control(pDX, IDC_COMBO_FONT_FACE_NAME, m_ComboFontFaceName);
	DDX_Control(pDX, IDC_CHECK_FONT_UNDERLINE, m_BtnFontUnderline);
	DDX_Control(pDX, IDC_CHECK_FONT_STRIKEOUT, m_BtnFontStrikeout);
	DDX_Control(pDX, IDC_CHECK_FONT_ITALIC, m_BtnFontItalic);
	DDX_Control(pDX, IDC_CHECK_FONT_BOLD, m_BtnFontBold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP( CChildView, CFormView )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SETTINGCHANGE()
	ON_WM_SYSCOLORCHANGE()
	ON_BN_CLICKED(IDC_CHECK_FONT_BOLD, OnCheckFontBold)
	ON_BN_CLICKED(IDC_CHECK_FONT_ITALIC, OnCheckFontItalic)
	ON_BN_CLICKED(IDC_CHECK_FONT_UNDERLINE, OnCheckFontUnderline)
	ON_BN_CLICKED(IDC_CHECK_FONT_STRIKEOUT, OnCheckFontStrikeout)
	ON_EN_CHANGE(IDC_EDIT_FONT_WIDTH, OnChangeEditFontWidth)
	ON_EN_CHANGE(IDC_EDIT_FONT_HEIGHT, OnChangeEditFontHeight)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_FACE_NAME, OnSelchangeComboFontFaceName)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

void CChildView::_UpdateHotBar( bool bForceEmptyIconCache )
{
int nPos = m_wndSliderHTBB.GetPos();
	ASSERT( -100 <= nPos && nPos <= 100 );
double lfHotPercent = double(nPos) / 100.0;
CMainFrame * pMainFrame =
		STATIC_DOWNCAST( CMainFrame, GetParentFrame() );
int nCount = pMainFrame->m_wndToolBarTrueHot.GetButtonsCount();
	for( int i = 0; i < nCount; i++ )
	{
		CExtBarButton * pTBBxx =
			pMainFrame->m_wndToolBarTrueHot.GetButton( i );
		ASSERT_VALID( pTBBxx );
		CFunnyToolButton * pTBB =
			DYNAMIC_DOWNCAST( CFunnyToolButton, pTBBxx );
		if( pTBB == NULL )
			continue;
		ASSERT( !pTBB->IsSeparator() );
		pTBB->UpdateHotPercent( lfHotPercent, bForceEmptyIconCache );
	} // for( int i = 0; i < nCount; i++ )
	if( pMainFrame->m_wndToolBarTrueHot.IsVisible() )
	{
		pMainFrame->m_wndToolBarTrueHot.Invalidate();
		pMainFrame->m_wndToolBarTrueHot.UpdateWindow();
	}

//CString str;
//	srand((unsigned int)time(NULL) ^ clock());
//	str.Format("This is a random number: %d",rand());
//	this->m_wndBoxBrightness.SetWindowText(str);
//	str.Format("Another random number: %d",rand());
//	this->m_wndBoxFont.SetWindowText(str);
}

void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(		pScrollBar->GetSafeHwnd() != NULL
		&&	pScrollBar->GetDlgCtrlID() == IDC_SLIDER_HTBB
		)
	{
		if( ! m_bInitializingControls )
			_UpdateHotBar( false );
		return;
	}
	CExtNCSB < CExtWS < CExtAFV < CFormView > > > :: OnHScroll( nSBCode, nPos, pScrollBar );
	RedrawWindow( NULL, NULL, RDW_INVALIDATE|RDW_ALLCHILDREN );
	if( m_wndSliderHTBB.GetSafeHwnd() != NULL )
		m_wndSliderHTBB.UpdateSliderWnd();
}

void CChildView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CExtNCSB < CExtWS < CExtAFV < CFormView > > > :: OnVScroll( nSBCode, nPos, pScrollBar );
	RedrawWindow( NULL, NULL, RDW_INVALIDATE|RDW_ALLCHILDREN );
	if( m_wndSliderHTBB.GetSafeHwnd() != NULL )
		m_wndSliderHTBB.UpdateSliderWnd();
}

void CChildView::OnInitialUpdate() 
{
	CExtNCSB < CExtWS < CExtAFV < CFormView > > > :: OnInitialUpdate();

	ASSERT( !m_bInitializingControls );
	m_bInitializingControls = true;

	m_wndSliderHTBB.SetRange( -100, 100 );
	m_wndSliderHTBB.SetPageSize( 10 );
	m_wndSliderHTBB.SetPos( 25 );

CWindowDC dc( this );
LOGFONT logfont;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfFaceName[0] = '\0';
	logfont.lfPitchAndFamily = 0;
	::EnumFontFamiliesEx(
		dc.m_hDC,
		&logfont,
		(FONTENUMPROC)stat_FontEnumProc,
		(LPARAM)&m_ComboFontFaceName,
		0
		);
int i = m_ComboFontFaceName.FindStringExact(
		0,
		m_lf.lfFaceName
		);
	if( i >= 0 )
		m_ComboFontFaceName.SetCurSel( i );

CString s;
	s.Format( _T("%ld"), m_lf.lfWidth );
	m_EditFontWidth.SetWindowText( s );
	s.Format( _T("%ld"), m_lf.lfHeight );
	m_EditFontHeight.SetWindowText( s );

	m_BtnFontBold.SetCheck(
		( m_lf.lfWeight > 400 ) ? TRUE : FALSE
		);
	m_BtnFontItalic.SetCheck(
		m_lf.lfItalic ? TRUE : FALSE
		);
	m_BtnFontUnderline.SetCheck(
		m_lf.lfUnderline ? TRUE : FALSE
		);
	m_BtnFontStrikeout.SetCheck(
		m_lf.lfStrikeOut ? TRUE : FALSE
		);

	m_wndSliderHTBB.SetStyle( CExtSliderWnd::ES_PROFUIS );

	m_bInitializingControls = false;

	ShowSizeGrip( FALSE );
}

void CChildView::_ConrolsData_2_UiSettings()
{
	ASSERT( !m_bInitializingControls );
	if( g_PaintManager->m_FontNormal.GetSafeHandle() != NULL )
		g_PaintManager->m_FontNormal.DeleteObject();

CString s;

	m_ComboFontFaceName.GetWindowText( s );
	if( !s.IsEmpty() )
		__EXT_MFC_STRCPY( m_lf.lfFaceName, LF_FACESIZE, s );

	m_EditFontWidth.GetWindowText( s );
	if( !s.IsEmpty() )
		m_lf.lfWidth = _ttol( s );
	m_EditFontHeight.GetWindowText( s );
	if( !s.IsEmpty() )
		m_lf.lfHeight = _ttol( s );

	m_lf.lfWeight = m_BtnFontBold.GetCheck() ? 900 : 400;
	m_lf.lfItalic = (BYTE)(m_BtnFontItalic.GetCheck() ? TRUE : FALSE);
	m_lf.lfUnderline = (BYTE)(m_BtnFontUnderline.GetCheck() ? TRUE : FALSE);
	m_lf.lfStrikeOut = (BYTE)(m_BtnFontStrikeout.GetCheck() ? TRUE : FALSE);

	g_PaintManager->m_FontNormal.CreateFontIndirect( &m_lf );
	ASSERT( g_PaintManager->m_FontNormal.GetSafeHandle() != NULL );

CMainFrame * pMainFrame =
		STATIC_DOWNCAST( CMainFrame, GetParentFrame() );
	pMainFrame->RecalcLayout();
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN
		);
	CExtControlBar::stat_RedrawFloatingFrames( pMainFrame );
	CExtControlBar::stat_RecalcBarMetrics( pMainFrame );
	
	m_wndSliderHTBB.UpdateSliderWnd();
}

void CChildView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CExtNCSB < CExtWS < CExtAFV < CFormView > > > :: OnSettingChange(uFlags, lpszSection);
	_ConrolsData_2_UiSettings();
}

LRESULT CChildView::OnDisplayChange( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CExtNCSB < CExtWS < CExtAFV < CFormView > > > :: OnDisplayChange( wParam, lParam );
	_ConrolsData_2_UiSettings();
	return lResult;
}

void CChildView::OnSysColorChange() 
{
	CExtNCSB < CExtWS < CExtAFV < CFormView > > > :: OnSysColorChange();
	_ConrolsData_2_UiSettings();
}

void CChildView::OnCheckFontBold() 
{
	if( !m_bInitializingControls )
		_ConrolsData_2_UiSettings();
}
void CChildView::OnCheckFontItalic() 
{
	if( !m_bInitializingControls )
		_ConrolsData_2_UiSettings();
}
void CChildView::OnCheckFontUnderline() 
{
	if( !m_bInitializingControls )
		_ConrolsData_2_UiSettings();
}
void CChildView::OnCheckFontStrikeout() 
{
	if( !m_bInitializingControls )
		_ConrolsData_2_UiSettings();
}
void CChildView::OnChangeEditFontWidth() 
{
	if( !m_bInitializingControls )
		_ConrolsData_2_UiSettings();
}
void CChildView::OnChangeEditFontHeight() 
{
	if( !m_bInitializingControls )
		_ConrolsData_2_UiSettings();
}
void CChildView::OnSelchangeComboFontFaceName() 
{
	if( !m_bInitializingControls )
		_ConrolsData_2_UiSettings();
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CExtNCSB < CExtWS < CExtAFV < CFormView > > > :: OnSize(nType, cx, cy);
	if( m_wndSliderHTBB.GetSafeHwnd() != NULL )
		m_wndSliderHTBB.UpdateSliderWnd();
}
