// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "MDI_InnerOuterBars.h"
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
	ON_COMMAND(ID_CHILD_BILD, OnChildBuild)
	ON_COMMAND(ID_CHILD_COMPILE, OnChildCompile)
	ON_COMMAND(ID_CHILD_DEBUG, OnChildDebug)
	ON_COMMAND(ID_CHILD_REBUILD, OnChildRebuild)
	ON_COMMAND(ID_CHILD_STOP_BUILD, OnChildStopBuild)
	ON_COMMAND(ID_CHILD_STOP_DBG, OnChildStopDbg)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CExtColorCtrl::g_nMsgColorChanged,OnColorChanged)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ! CWnd::PreCreateWindow( cs ) )
		return FALSE;

	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_DLGMODALFRAME);
	cs.style &= ~WS_BORDER;
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	cs.lpszClass =
		AfxRegisterWndClass(
			CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
			::LoadCursor(NULL, IDC_ARROW),
			HBRUSH(NULL),
			NULL
			);

	return TRUE;
}

void CChildView::OnPaint() 
{
CRect rcClient;
	GetClientRect( &rcClient );
CPaintDC dcPaint( this );
	CExtPaintManager::stat_ExcludeChildAreas(
		dcPaint.GetSafeHdc(),
		GetSafeHwnd()
		);
CExtMemoryDC dc( &dcPaint, &rcClient );
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
		dc, rcPattern0, CRect(0,0,0,0), CRect(0,0,0,0),
		CExtWndShadow::DEF_SHADOW_SIZE,
		CExtWndShadow::DEF_BRIGHTNESS_MIN, CExtWndShadow::DEF_BRIGHTNESS_MAX,
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
		dc, rcPattern1, CRect(0,0,0,0), CRect(0,0,0,0),
		CExtWndShadow::DEF_SHADOW_SIZE,
		CExtWndShadow::DEF_BRIGHTNESS_MIN, CExtWndShadow::DEF_BRIGHTNESS_MAX,
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
		dc, rc2, CRect(0,0,0,0), CRect(0,0,0,0),
		CExtWndShadow::DEF_SHADOW_SIZE,
		CExtWndShadow::DEF_BRIGHTNESS_MIN, CExtWndShadow::DEF_BRIGHTNESS_MAX,
		false
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
		dc, rc5, CRect(0,0,0,0), CRect(0,0,0,0),
		CExtWndShadow::DEF_SHADOW_SIZE,
		CExtWndShadow::DEF_BRIGHTNESS_MIN, CExtWndShadow::DEF_BRIGHTNESS_MAX,
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
	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	VERIFY(
		m_wndColorCtrlCubeRGB_R.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD,
			CRect(10,210,230,410),
			this,
			0x200
			)
		);
	m_wndColorCtrlCubeRGB_R = CExtColorCtrl::MODE_XYZ_R;

	VERIFY(
		m_wndColorCtrlCubeRGB_G.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD,
			CRect(240,210,460,410),
			this,
			0x201
			)
		);
	m_wndColorCtrlCubeRGB_G = CExtColorCtrl::MODE_XYZ_G;
	
	VERIFY(
		m_wndColorCtrlCubeRGB_B.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD,
			CRect(470,210,690,410),
			this,
			0x202
			)
		);
	m_wndColorCtrlCubeRGB_B = CExtColorCtrl::MODE_XYZ_B;

	VERIFY(
		m_wndColorCtrlHSL_BH.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD,
			CRect(470,10,690,200),
			this,
			0x203
			)
		);
	m_wndColorCtrlHSL_BH = CExtColorCtrl::MODE_HSL_BASIC_HUE;
	
	VERIFY(
		m_wndColorCtrlHSL_Panel.Create(
			_T("BUTTON"),
			_T(""),
			WS_CHILD,
			CRect(240,10,460,200),
			this,
			0x204
			)
		);
	m_wndColorCtrlHSL_Panel = CExtColorCtrl::MODE_HSL_PANEL;

COLORREF clrInit = RGB(196,128,64);
	m_wndColorCtrlCubeRGB_R.SetupColor(clrInit,false);
	m_wndColorCtrlCubeRGB_G.SetupColor(clrInit,false);
	m_wndColorCtrlHSL_BH.SetupColor(clrInit,false);
	m_wndColorCtrlHSL_Panel.SetupColor(clrInit,false);
	m_wndColorCtrlCubeRGB_B.SetupColor(clrInit,false);
	
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
	if( hWndColorCtrlFrom != m_wndColorCtrlCubeRGB_R.GetSafeHwnd() )
		m_wndColorCtrlCubeRGB_R.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlCubeRGB_G.GetSafeHwnd() )
		m_wndColorCtrlCubeRGB_G.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlHSL_BH.GetSafeHwnd() )
		m_wndColorCtrlHSL_BH.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlHSL_Panel.GetSafeHwnd() )
		m_wndColorCtrlHSL_Panel.SetupColor(clrNew,false);
	if( hWndColorCtrlFrom != m_wndColorCtrlCubeRGB_B.GetSafeHwnd() )
		m_wndColorCtrlCubeRGB_B.SetupColor(clrNew,false);
	m_bSyncingValues = false;
	return 0;
}

void CChildView::OnViewColorControls() 
{
	m_bColorControlsVisible = !m_bColorControlsVisible;
	m_wndColorCtrlCubeRGB_R.ShowWindow( m_bColorControlsVisible ? SW_SHOW : SW_HIDE );
	m_wndColorCtrlCubeRGB_G.ShowWindow( m_bColorControlsVisible ? SW_SHOW : SW_HIDE );
	m_wndColorCtrlHSL_BH.ShowWindow( m_bColorControlsVisible ? SW_SHOW : SW_HIDE );
	m_wndColorCtrlHSL_Panel.ShowWindow( m_bColorControlsVisible ? SW_SHOW : SW_HIDE );
	m_wndColorCtrlCubeRGB_B.ShowWindow( m_bColorControlsVisible ? SW_SHOW : SW_HIDE );
}

void CChildView::OnUpdateViewColorControls(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(
		m_bColorControlsVisible ? 1 : 0
		);
}

void CChildView::OnChildBuild() 
{
	AfxMessageBox(
		_T("CChildView::OnChildBuild()")
		);
}

void CChildView::OnChildCompile() 
{
	AfxMessageBox(
		_T("CChildView::OnChildCompile()")
		);
}

void CChildView::OnChildDebug() 
{
	AfxMessageBox(
		_T("CChildView::OnChildDebug()")
		);
}

void CChildView::OnChildRebuild() 
{
	AfxMessageBox(
		_T("CChildView::OnChildRebuild()")
		);
}

void CChildView::OnChildStopBuild() 
{
	AfxMessageBox(
		_T("CChildView::OnChildStopBuild()")
		);
}

void CChildView::OnChildStopDbg() 
{
	AfxMessageBox(
		_T("CChildView::OnChildStopDbg()")
		);
}
