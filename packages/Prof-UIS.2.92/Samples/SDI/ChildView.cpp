// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "SDI.h"
#include "ChildView.h"

#if !defined(__EXT_MEMORY_DC_H)
	#include <../Src/ExtMemoryDC.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
	: m_bSyncingValues( false )
	, m_bColorControlsVisible( false )
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_COLOR_CONTROLS, OnViewColorControls)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_CONTROLS, OnUpdateViewColorControls)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CExtColorCtrl::g_nMsgColorChanged,OnColorChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME);
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), NULL, NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
CPaintDC dcPaint( this );
	CExtPaintManager::stat_ExcludeChildAreas(
		dcPaint.GetSafeHdc(),
		GetSafeHwnd()
		);
CExtMemoryDC dc( &dcPaint );
	g_PaintManager->PaintDocumentClientAreaBkgnd( dc, this );

CGdiObject * pOld0, * pOld1;
CExtWndShadow _shadow1, _shadow2, _shadow3, _shadow4;

WORD _PatternBits0[8],_PatternBits1[8];
	for( INT iPatternLine=0; iPatternLine < 8; iPatternLine++ )
	{
		_PatternBits0[iPatternLine] =
			( iPatternLine & 1 )
				? WORD(0xFFFF)
				: WORD( ~ ( 5<<iPatternLine ) )
				;
		_PatternBits1[iPatternLine] =
			( iPatternLine & 1 )
				? WORD(0xFFFF)
				: WORD( ~ ( 1<<(8-iPatternLine) ) )
				;
	}
CBitmap bmpPattern0,bmpPattern1;
CBrush brPattern0,brPattern1;
	VERIFY( bmpPattern0.CreateBitmap(8,8,1,1,_PatternBits0) );
	VERIFY( bmpPattern1.CreateBitmap(8,8,1,1,_PatternBits1) );
	VERIFY( brPattern0.CreatePatternBrush(&bmpPattern0) );
	VERIFY( brPattern1.CreatePatternBrush(&bmpPattern1) );

CRect rcPattern0(10,10,180,150), rcPattern1(40,30,80,200);

	dc.FillRect(rcPattern0,&brPattern0);
	_shadow1.Paint(
		NULL,
		dc,
		rcPattern0,
		CRect(0,0,0,0),
		CRect(0,0,0,0),
		CExtWndShadow::DEF_SHADOW_SIZE,
		CExtWndShadow::DEF_BRIGHTNESS_MIN,
		CExtWndShadow::DEF_BRIGHTNESS_MAX,
		false
		);
	pOld0 = dc.SelectStockObject( NULL_BRUSH );
	pOld1 = dc.SelectStockObject( BLACK_PEN );
	dc.Rectangle( rcPattern0 );
	if( pOld0 != NULL )
		dc.SelectObject( pOld0 );
	if( pOld1 != NULL )
		dc.SelectObject( pOld1 );
	
	dc.FillRect(rcPattern1,&brPattern1);
	_shadow2.Paint(
		NULL,
		dc,
		rcPattern1,
		CRect(0,0,0,0),
		CRect(0,0,0,0),
		CExtWndShadow::DEF_SHADOW_SIZE,
		CExtWndShadow::DEF_BRIGHTNESS_MIN,
		CExtWndShadow::DEF_BRIGHTNESS_MAX,
		false
		);
	pOld0 = dc.SelectStockObject( NULL_BRUSH );
	pOld1 = dc.SelectStockObject( BLACK_PEN );
	dc.Rectangle( rcPattern1 );
	if( pOld0 != NULL )
		dc.SelectObject( pOld0 );
	if( pOld1 != NULL )
		dc.SelectObject( pOld1 );

CRect rc0(20,50,160,80);
CRect rc1(160,50,210,80);
CRect rc2(rc0.left,rc0.top,rc1.right,rc1.bottom);
COLORREF clrLeft = RGB(0,255,0);
COLORREF clrMiddle = RGB(255,255,0);
COLORREF clrRight = RGB(255,0,0);
	CExtPaintManager::stat_PaintGradientRect(
		dc, rc0, clrLeft, clrMiddle );
	CExtPaintManager::stat_PaintGradientRect(
		dc, rc1, clrMiddle,clrRight );
	_shadow3.Paint(
		NULL,
		dc,
		rc2,
		CRect(0,0,0,0),
		CRect(0,0,0,0),
		CExtWndShadow::DEF_SHADOW_SIZE,
		CExtWndShadow::DEF_BRIGHTNESS_MIN,
		CExtWndShadow::DEF_BRIGHTNESS_MAX,
		false,
		true
		);
	pOld0 = dc.SelectStockObject( NULL_BRUSH );
	pOld1 = dc.SelectStockObject( BLACK_PEN );
	dc.Rectangle(rc2);
	if( pOld0 != NULL )
		dc.SelectObject( pOld0 );
	if( pOld1 != NULL )
		dc.SelectObject( pOld1 );

CRect rc3(120,40,150,80);
CRect rc4(120,80,150,160);
CRect rc5(rc3.left,rc3.top,rc4.right,rc4.bottom);
COLORREF clrUpper = RGB(255,0,255);
COLORREF clrMiddleV = RGB(255,255,255);
COLORREF clrLower = RGB(0,0,255);
	CExtPaintManager::stat_PaintGradientRect(
		dc, rc3, clrMiddleV, clrUpper, TRUE );
	CExtPaintManager::stat_PaintGradientRect(
		dc, rc4, clrLower, clrMiddleV, TRUE );
	_shadow4.Paint(
		NULL,
		dc,
		rc5,
		CRect(0,0,0,0),
		CRect(0,0,0,0),
		CExtWndShadow::DEF_SHADOW_SIZE,
		CExtWndShadow::DEF_BRIGHTNESS_MIN,
		CExtWndShadow::DEF_BRIGHTNESS_MAX,
		false
		);
	pOld0 = dc.SelectStockObject( NULL_BRUSH );
	pOld1 = dc.SelectStockObject( BLACK_PEN );
	dc.Rectangle(rc5);
	if( pOld0 != NULL )
		dc.SelectObject( pOld0 );
	if( pOld1 != NULL )
		dc.SelectObject( pOld1 );
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC) 
{
	pDC;

/*

COMMENTED BLOCK: dragging controlbar unwanted redrawing test

	class FrienlyExtControlBar : public CExtControlBar
	{
	public:
		friend class CChildView;
	};
CRect rcClient;
	GetClientRect( &rcClient );
	if( rcClient.IsRectEmpty() )
		return TRUE;
	if( !pDC->RectVisible(&rcClient) )
		return TRUE;
COLORREF clr = RGB( 255, 0, 0 );
	if( FrienlyExtControlBar::g_bUpdatingDragState )
		clr = RGB( 0, 255, 0 );
	else if( FrienlyExtControlBar::_DraggingGetBar() != NULL )
		clr = RGB( 0, 0, 255 );
	pDC->FillSolidRect( &rcClient, clr );
*/

	return TRUE;
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	VERIFY(
		m_wndColorCtrlCubeRGB.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD,
			CRect(10,210,230,410),
			this,
			0x200
			)
		);
	m_wndColorCtrlCubeRGB = CExtColorCtrl::MODE_RGB_CUBE;

	VERIFY(
		m_wndColorCtrlCubeCMY.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD,
			CRect(240,210,460,410),
			this,
			0x201
			)
		);
	m_wndColorCtrlCubeCMY = CExtColorCtrl::MODE_CMY_CUBE;
	
	VERIFY(
		m_wndColorCtrlHSL.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD,
			CRect(240,10,460,200),
			this,
			0x202
			)
		);
	m_wndColorCtrlHSL = CExtColorCtrl::MODE_HSL_PANEL;

	VERIFY(
		m_wndColorCtrlMixerRGB.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD,
			CRect(470,210,690,410),
			this,
			0x203
			)
		);
	m_wndColorCtrlMixerRGB = CExtColorCtrl::MODE_RGB_MIX;
	
	VERIFY(
		m_wndColorCtrlMixerCMY.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD,
			CRect(470,10,690,200),
			this,
			0x204
			)
		);
	m_wndColorCtrlMixerCMY = CExtColorCtrl::MODE_CMY_MIX;

COLORREF clrInit = RGB(196,128,64);
	m_wndColorCtrlCubeRGB.SetupColor(clrInit,false);
	m_wndColorCtrlCubeCMY.SetupColor(clrInit,false);
	m_wndColorCtrlMixerRGB.SetupColor(clrInit,false);
	m_wndColorCtrlMixerCMY.SetupColor(clrInit,false);
	m_wndColorCtrlHSL.SetupColor(clrInit,false);

	return 0;
}

LRESULT CChildView::OnColorChanged(WPARAM wParam,LPARAM lParam)
{
	if( m_bSyncingValues )
		return 0;
	m_bSyncingValues = true;
CExtColorCtrl::selection_colors_t * pClrs =
		(CExtColorCtrl::selection_colors_t *)lParam;
	ASSERT( pClrs != NULL );
COLORREF clrNew = pClrs->m_clrNew;
HWND hWndColorCtrlFrom = (HWND)wParam;
	ASSERT( ::IsWindow(hWndColorCtrlFrom) );
	if( hWndColorCtrlFrom != m_wndColorCtrlCubeRGB.GetSafeHwnd() )
		m_wndColorCtrlCubeRGB.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlCubeCMY.GetSafeHwnd() )
		m_wndColorCtrlCubeCMY.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlMixerRGB.GetSafeHwnd() )
		m_wndColorCtrlMixerRGB.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlMixerCMY.GetSafeHwnd() )
		m_wndColorCtrlMixerCMY.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlHSL.GetSafeHwnd() )
		m_wndColorCtrlHSL.SetupColor(clrNew,false);
	m_bSyncingValues = false;
	return 0;
}

void CChildView::OnViewColorControls() 
{
	m_bColorControlsVisible = ! m_bColorControlsVisible;
	m_wndColorCtrlCubeRGB.ShowWindow(  m_bColorControlsVisible ? SW_SHOW : SW_HIDE );
	m_wndColorCtrlCubeCMY.ShowWindow(  m_bColorControlsVisible ? SW_SHOW : SW_HIDE );
	m_wndColorCtrlMixerRGB.ShowWindow( m_bColorControlsVisible ? SW_SHOW : SW_HIDE );
	m_wndColorCtrlMixerCMY.ShowWindow( m_bColorControlsVisible ? SW_SHOW : SW_HIDE );
	m_wndColorCtrlHSL.ShowWindow(      m_bColorControlsVisible ? SW_SHOW : SW_HIDE );
}

void CChildView::OnUpdateViewColorControls(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(
		m_bColorControlsVisible ? 1 : 0
		);
}
