// ChildFormView.cpp : implementation file
//

#include "stdafx.h"
#include "ThemeColorizer.h"
#include "ChildFormView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFormView

IMPLEMENT_DYNCREATE(CChildFormView, CFormView)

CChildFormView::CChildFormView()
: CExtWS < CExtAFV < CFormView > > ( CChildFormView::IDD, ((CWnd *)NULL) )
{
	//{{AFX_DATA_INIT(CChildFormView)
	//}}AFX_DATA_INIT
	m_bInitComplete = false;
}

CChildFormView::~CChildFormView()
{
}

void CChildFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildFormView)
	DDX_Control(pDX, IDC_STATIC_COLOR_THEME, m_wndLabelColorTheme);
	DDX_Control(pDX, IDC_STATIC_SEL_TYPE, m_wndLabelSelType);
	DDX_Control(pDX, IDC_BUTTON_COLOR2, m_wndColor2);
	DDX_Control(pDX, IDC_BUTTON_COLOR1, m_wndColor1);
	DDX_Control(pDX, IDC_GROUP_BOX2, m_wndGroupBox2);
	DDX_Control(pDX, IDC_GROUP_BOX1, m_wndGroupBox1);
	DDX_Control(pDX, IDC_COMBO_SEL_TYPE, m_wndColorCtrlType);
	DDX_Control(pDX, IDC_COMBO_COLOR_THEME, m_wndColorTheme);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildFormView, CFormView)
	//{{AFX_MSG_MAP(CChildFormView)
	ON_CBN_SELENDOK(IDC_COMBO_SEL_TYPE, OnSelendokComboSelType)
	ON_CBN_SELENDOK(IDC_COMBO_COLOR_THEME, OnSelendokComboColorTheme)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(
		CExtColorCtrl::g_nMsgColorChanged,
		OnColorChanged
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyCustColor,
		OnColorSelectCustom
		)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFormView diagnostics

#ifdef _DEBUG
void CChildFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CChildFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFormView message handlers

BOOL CChildFormView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CFormView::PreCreateWindow(cs))
		return FALSE;
	
	cs.dwExStyle &= ~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE);
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);
	
	return TRUE;
}


BOOL CChildFormView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	BOOL bRetVal = CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	if(		nCode == CN_UPDATE_COMMAND_UI
		&& (	nID == ID_FILE_NEW
			||	nID == ID_FILE_OPEN
			||	nID == ID_FILE_SAVE 
			||	nID == ID_EDIT_CUT
			||	nID == ID_EDIT_COPY
			||	nID == ID_EDIT_PASTE
			||	nID == ID_FILE_PRINT
			||	nID == ID_APP_ABOUT
			||	nID == ID_CONTEXT_HELP

			||	nID == ID_MYEXTBTN_SLIDER
			||	nID == ID_MYEXTBTN_SCROLLER
			||	nID == ID_IED_TOOL_RECT_SELECTION
			||	nID == ID_IED_TOOL_COLOR_SELECTION
			||	nID == ID_IED_TOOL_FILL
			||	nID == ID_IED_TOOL_MAGNIFICATION
			||	nID == ID_IED_TOOL_PENCIL
			||	nID == ID_IED_TOOL_BRUSH
			||	nID == ID_IED_TOOL_AIR_BRUSH
			||	nID == ID_IED_TOOL_LINE
			||	nID == ID_IED_TOOL_RECTANGLE
			||	nID == ID_IED_TOOL_OUTLINED_RECTANGLE
			||	nID == ID_IED_TOOL_FILLED_RECTANGLE
			||	nID == ID_IED_TOOL_ROUNDED_RECTANGLE
			||	nID == ID_IED_TOOL_ROUNDED_OUTLINED_RECTANGLE
			||	nID == ID_IED_TOOL_ROUNDED_FILLED_RECTANGLE
			||	nID == ID_IED_TOOL_ELLIPSE
			||	nID == ID_IED_TOOL_OUTLINED_ELLIPSE
			||	nID == ID_IED_TOOL_FILLED_ELLIPSE

			||	nID == ID_IED_TOOL_RECT_SELECTION
			||	nID == ID_IED_TOOL_COLOR_SELECTION
			||	nID == ID_IED_TOOL_FILL
			||	nID == ID_IED_TOOL_MAGNIFICATION
			||	nID == ID_IED_TOOL_PENCIL
			||	nID == ID_IED_TOOL_BRUSH
			||	nID == ID_IED_TOOL_AIR_BRUSH
			||	nID == ID_IED_TOOL_LINE
			||	nID == ID_IED_TOOL_RECTANGLE
			||	nID == ID_IED_TOOL_OUTLINED_RECTANGLE
			||	nID == ID_IED_TOOL_FILLED_RECTANGLE
			||	nID == ID_IED_TOOL_ROUNDED_RECTANGLE
			||	nID == ID_IED_TOOL_ROUNDED_OUTLINED_RECTANGLE
			||	nID == ID_IED_TOOL_ROUNDED_FILLED_RECTANGLE
			||	nID == ID_IED_TOOL_ELLIPSE           
			||	nID == ID_IED_TOOL_OUTLINED_ELLIPSE 
			||	nID == ID_IED_TOOL_FILLED_ELLIPSE     
			||	nID == ID_IED_TOOL_MAGNIFICATION_01X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_02X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_03X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_04X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_05X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_06X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_07X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_08X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_09X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_10X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_11X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_12X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_13X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_14X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_15X  
			||	nID == ID_IED_TOOL_MAGNIFICATION_16X  
			||	nID == ID_IED_TOOL_BRUSH_CIRCLE_SMALL 
			||	nID == ID_IED_TOOL_BRUSH_CIRCLE_MEDIUM
			||	nID == ID_IED_TOOL_BRUSH_CIRCLE_LARGE 
			||	nID == ID_IED_TOOL_BRUSH_SQUARE_SMALL 
			||	nID == ID_IED_TOOL_BRUSH_SQUARE_MEDIUM
			||	nID == ID_IED_TOOL_BRUSH_SQUARE_LARGE 
			||	nID == ID_IED_TOOL_BRUSH_LINE_LD_SMALL
			||	nID == ID_IED_TOOL_BRUSH_LINE_LD_MEDIUM
			||	nID == ID_IED_TOOL_BRUSH_LINE_LD_LARGE 
			||	nID == ID_IED_TOOL_BRUSH_LINE_RD_SMALL 
			||	nID == ID_IED_TOOL_BRUSH_LINE_RD_MEDIUM
			||	nID == ID_IED_TOOL_BRUSH_LINE_RD_LARGE 
			||	nID == ID_IED_TOOL_AIR_BRUSH_SMALL     
			||	nID == ID_IED_TOOL_AIR_BRUSH_MEDIUM    
			||	nID == ID_IED_TOOL_AIR_BRUSH_LARGE     
			||	nID == ID_IED_TOOL_LINE_CIRCLE_SMALL   
			||	nID == ID_IED_TOOL_LINE_CIRCLE_MEDIUM  
			||	nID == ID_IED_TOOL_LINE_CIRCLE_LARGE   
			||	nID == ID_IED_TOOL_LINE_SQUARE_SMALL   
			||	nID == ID_IED_TOOL_LINE_SQUARE_MEDIUM  
			||	nID == ID_IED_TOOL_LINE_SQUARE_LARGE   
			||	nID == ID_IED_TOOL_LINE_LINE_LD_SMALL  
			||	nID == ID_IED_TOOL_LINE_LINE_LD_MEDIUM 
			||	nID == ID_IED_TOOL_LINE_LINE_LD_LARGE  
			||	nID == ID_IED_TOOL_LINE_LINE_RD_SMALL  
			||	nID == ID_IED_TOOL_LINE_LINE_RD_MEDIUM 
			||	nID == ID_IED_TOOL_LINE_LINE_RD_LARGE  
			||	nID == ID_IED_TOOL_RECTANGLE_SMALLER   
			||	nID == ID_IED_TOOL_RECTANGLE_SMALL     
			||	nID == ID_IED_TOOL_RECTANGLE_MEDIUM    
			||	nID == ID_IED_TOOL_RECTANGLE_LARGE     
			||	nID == ID_IED_TOOL_RECTANGLE_LARGER    
			||	nID == ID_IED_TOOL_OUTLINED_RECTANGLE_SMALLER
			||	nID == ID_IED_TOOL_OUTLINED_RECTANGLE_SMALL
			||	nID == ID_IED_TOOL_OUTLINED_RECTANGLE_MEDIUM 
			||	nID == ID_IED_TOOL_OUTLINED_RECTANGLE_LARGE
			||	nID == ID_IED_TOOL_OUTLINED_RECTANGLE_LARGER 
			||	nID == ID_IED_TOOL_ROUNDED_RECTANGLE_SMALLER
			||	nID == ID_IED_TOOL_ROUNDED_RECTANGLE_SMALL
			||	nID == ID_IED_TOOL_ROUNDED_RECTANGLE_MEDIUM
			||	nID == ID_IED_TOOL_ROUNDED_RECTANGLE_LARGE
			||	nID == ID_IED_TOOL_ROUNDED_RECTANGLE_LARGER
			||	nID == ID_IED_TOOL_ELLIPSE_SMALLER
			||	nID == ID_IED_TOOL_ELLIPSE_SMALL
			||	nID == ID_IED_TOOL_ELLIPSE_MEDIUM 
			||	nID == ID_IED_TOOL_ELLIPSE_LARGE  
			||	nID == ID_IED_TOOL_ELLIPSE_LARGER 
			||	nID == ID_IED_TOOL_OUTLINED_ROUNDED_RECTANGLE_SMALLER
			||	nID == ID_IED_TOOL_OUTLINED_ROUNDED_RECTANGLE_SMALL
			||	nID == ID_IED_TOOL_OUTLINED_ROUNDED_RECTANGLE_MEDIUM 
			||	nID == ID_IED_TOOL_OUTLINED_ROUNDED_RECTANGLE_LARGE
			||	nID == ID_IED_TOOL_OUTLINED_ROUNDED_RECTANGLE_LARGER 
			||	nID == ID_IED_TOOL_OUTLINED_ELLIPSE_SMALLER
			||	nID == ID_IED_TOOL_OUTLINED_ELLIPSE_SMALL
			||	nID == ID_IED_TOOL_OUTLINED_ELLIPSE_MEDIUM
			||	nID == ID_IED_TOOL_OUTLINED_ELLIPSE_LARGE
			||	nID == ID_IED_TOOL_OUTLINED_ELLIPSE_LARGER
			)
		)
		{				
			CCmdUI * pCmdUI = (CCmdUI *)pExtra;
			ASSERT( pCmdUI != NULL );
			pCmdUI->Enable();
			return TRUE;
		}

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////////

void CChildFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ShowSizeGrip(FALSE);

	m_wndColorCtrlType.ResetContent();
	ASSERT( m_wndColorCtrlType.GetCount() == 0 );
	for(	int i = int(CExtColorCtrl::MODE_NAMED_MIN);
			i <= int(CExtColorCtrl::MODE_NAMED_MAX);
			i++
			)
			m_wndColorCtrlType.AddString(
				CExtColorCtrl::GetColorModeName(
					CExtColorCtrl::e_mode_t(i)
					)
				);
	m_wndColorCtrlType.SetCurSel( (int)(CExtColorCtrl::e_mode_t)m_wndColorCtrl1 );

	VERIFY(
		m_wndColorCtrl1.SubclassDlgItem(
			IDC_STATIC_HEX_FULL1,
			this
			)
		);

	VERIFY(
		m_wndColorCtrl2.SubclassDlgItem(
			IDC_STATIC_HEX_FULL2,
			this
			)
		);
	
	m_wndGroupBox1.SetStyle( CExtGroupBox::STYLE_ROUNDED );
	m_wndGroupBox2.SetStyle( CExtGroupBox::STYLE_ROUNDED );

	m_wndColor1.SetFlat( TRUE );
	m_wndColor1.SetDrawBorder( TRUE );
	m_wndColor1.SetDrawFocusRect( FALSE );
	m_wndColor1.m_lParamCookie = LPARAM(IDC_BUTTON_COLOR1);
	m_wndColor1.m_bEnableBtnColorDefault = false;

	m_wndColor2.SetFlat( TRUE );
	m_wndColor2.SetDrawBorder( TRUE );
	m_wndColor2.SetDrawFocusRect( FALSE );
	m_wndColor2.m_lParamCookie = LPARAM(IDC_BUTTON_COLOR2);
	m_wndColor2.m_bEnableBtnColorDefault = false;

	//////////////////////////////////////////////////////////////////////////
CMainFrame * pMainFrame = STATIC_DOWNCAST( CMainFrame, ::AfxGetMainWnd() );
	ASSERT_VALID( pMainFrame->m_pPaintManager );

// 	m_wndColorCtrl1.m_selClrs.m_clrCurrent = 
// 		m_wndColorCtrl1.m_selClrs.m_clrPrev = 
// 		m_wndColorCtrl1.m_selClrs.m_clrNew =
// 			pMainFrame->m_pPaintManager->m_clrFillHint;
// 
// 	m_wndColorCtrl2.m_selClrs.m_clrCurrent = 
// 		m_wndColorCtrl2.m_selClrs.m_clrPrev = 
// 		m_wndColorCtrl2.m_selClrs.m_clrNew =
// 			pMainFrame->m_pPaintManager->m_clrAccentHint;
// 
// 	OnColorChanged(
// 		(WPARAM)m_wndColorCtrl1.GetSafeHwnd(),
// 		(LPARAM)&m_wndColorCtrl1.m_selClrs
// 		);
// 	OnColorChanged(
// 		(WPARAM)m_wndColorCtrl2.GetSafeHwnd(),
// 		(LPARAM)&m_wndColorCtrl2.m_selClrs
// 		);
	m_wndColorCtrl1.SetupColor(
		pMainFrame->m_pPaintManager->m_clrFillHint,
		true
		);
	m_wndColorCtrl2.SetupColor(
		pMainFrame->m_pPaintManager->m_clrAccentHint,
		true
		);

	//////////////////////////////////////////////////////////////////////////
	
	int nItem = 0;
	nItem = m_wndColorTheme.AddString( _T("Luna Blue") );
	m_wndColorTheme.SetItemData( nItem, ThemeLunaBlue );
	nItem = m_wndColorTheme.AddString( _T("Luna Olive") );
	m_wndColorTheme.SetItemData( nItem, ThemeLunaOlive );
	nItem = m_wndColorTheme.AddString( _T("Luna Silver") );
	m_wndColorTheme.SetItemData( nItem, ThemeLunaSilver );
	nItem = m_wndColorTheme.AddString( _T("Visual Studio 2005") );
	m_wndColorTheme.SetItemData( nItem, ThemeStudio2005 );

	nItem = m_wndColorTheme.AddString( _T("Pumpkin") );
	m_wndColorTheme.SetItemData( nItem, ThemePumpkin );
	nItem = m_wndColorTheme.AddString( _T("Rose") );
	m_wndColorTheme.SetItemData( nItem, ThemeRose );
	nItem = m_wndColorTheme.AddString( _T("Eggplant") );
	m_wndColorTheme.SetItemData( nItem, ThemeEggplant );
	nItem = m_wndColorTheme.AddString( _T("Rainy Day") );
	m_wndColorTheme.SetItemData( nItem, ThemeRainyDay );
	nItem = m_wndColorTheme.AddString( _T("Spruce") );
	m_wndColorTheme.SetItemData( nItem, ThemeSpruce );
	nItem = m_wndColorTheme.AddString( _T("Lilac") );
	m_wndColorTheme.SetItemData( nItem, ThemeLilac );

	m_bInitComplete = true;
}

void CChildFormView::OnSelendokComboSelType() 
{
	if( !m_bInitComplete )
		return;
	CExtColorCtrl::e_mode_t eMode =
		(CExtColorCtrl::e_mode_t)m_wndColorCtrlType.GetCurSel();
	m_wndColorCtrl1 = eMode;
	m_wndColorCtrl2 = eMode;
}

LRESULT CChildFormView::OnColorChanged(WPARAM wParam,LPARAM lParam)
{
	CExtColorCtrl::selection_colors_t * pClrs =
		(CExtColorCtrl::selection_colors_t *)lParam;
	ASSERT( pClrs != NULL );

CMainFrame * pMainFrame = STATIC_DOWNCAST( CMainFrame, ::AfxGetMainWnd() );
	ASSERT_VALID( pMainFrame->m_pPaintManager );
	
	if( (HWND)wParam == m_wndColorCtrl1.GetSafeHwnd() )
	{
		m_wndColor1.m_clrSelected = pClrs->m_clrNew;
		m_wndColor1.Invalidate();

		pMainFrame->m_pPaintManager->SetThemeColors(
			pClrs->m_clrNew,
			pMainFrame->m_pPaintManager->m_clrAccentHint
		);
	}
	else if( (HWND)wParam == m_wndColorCtrl2.GetSafeHwnd() )
	{
		m_wndColor2.m_clrSelected = pClrs->m_clrNew;
		m_wndColor2.Invalidate();

		pMainFrame->m_pPaintManager->SetThemeColors(
			pMainFrame->m_pPaintManager->m_clrFillHint,
			pClrs->m_clrNew
		);
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CChildFormView::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
	COLORREF clrColorCtrl = COLORREF(wParam);
	switch( lParam )
	{
	case IDC_BUTTON_COLOR1:
		if( clrColorCtrl == COLORREF(-1) )
		{
		}
		else
			m_wndColorCtrl1.SetupColor( clrColorCtrl, true );
		break;
	case IDC_BUTTON_COLOR2:
		if( clrColorCtrl == COLORREF(-1) )
		{
		}
		else
			m_wndColorCtrl2.SetupColor( clrColorCtrl, true );
		break;
	} // switch( lParam )
	return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CChildFormView::OnColorSelectCustom(WPARAM wParam, LPARAM lParam)
{
	wParam;
	if(		lParam == IDC_BUTTON_COLOR1 
		||	lParam == IDC_BUTTON_COLOR2
		)
	{
		COLORREF clrNew;
		CExtColorDlg dlgColor;
		if( dlgColor.DoModal() != IDOK )
			return 0;
		clrNew = dlgColor.m_clrNew;
		OnColorChangedFinally((WPARAM)clrNew, lParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CChildFormView::OnSelendokComboColorTheme() 
{
	int nIndex = m_wndColorTheme.GetCurSel();
	if(LB_ERR != nIndex)
	{
		COLORREF clrFillHint = NULL;
		COLORREF clrAccentHint = NULL;

		e_ColorThemes_t eColorTheme = (e_ColorThemes_t)m_wndColorTheme.GetItemData(nIndex);
		switch(eColorTheme) 
		{
		case ThemeLunaBlue:
			clrFillHint = 
				CExtBitmap::stat_HLS_Adjust(
					RGB(151, 190, 244),
					0.00, -0.07, -0.10
					);
			clrAccentHint = RGB(255, 200, 91);
			break;

		case ThemeLunaOlive:
			clrFillHint = 
				CExtBitmap::stat_HLS_Adjust(
					RGB(207, 215, 180),
					0.00, -0.07, -0.10
					);
			clrAccentHint = RGB(242, 195, 115);;
			break;

		case ThemeLunaSilver:
			clrFillHint = 
				CExtBitmap::stat_HLS_Adjust(
					RGB(188, 189, 207),
					0.00, -0.07, -0.10
					);
			clrAccentHint = RGB(255, 200, 91);
			break;

		case ThemeStudio2005:
			clrFillHint = 
				CExtBitmap::stat_HLS_Adjust(
					RGB(225, 225, 210),
					0.00, -0.05, -0.05
					);
			clrAccentHint = RGB(49, 106, 197);
			break;

		case ThemePumpkin:
			clrFillHint		= RGB(248,219,148);
			clrAccentHint	= RGB(147,32,205);
			break;

		case ThemeRose:
			clrFillHint		= RGB(202,166,179);
			clrAccentHint	= RGB(164,118,143);
			break;

		case ThemeEggplant:
			clrFillHint		= RGB(128,175,164);
			clrAccentHint	= RGB(114,144,150);
			break;

		case ThemeRainyDay:
			clrFillHint		= RGB(131,163,191);
			clrAccentHint	= RGB(101,135,174);
			break;

		case ThemeSpruce:
			clrFillHint		= RGB(147,192,149);
			clrAccentHint	= RGB(89,165,102);
			break;

		case ThemeLilac:
			clrFillHint		= RGB(146,139,197);
			clrAccentHint	= RGB(112,109,177);
			break;
		
		default:
			return;
		}

		m_wndColorCtrl1.m_selClrs.m_clrCurrent = 
			m_wndColorCtrl1.m_selClrs.m_clrPrev = 
			m_wndColorCtrl1.m_selClrs.m_clrNew = clrFillHint;
		m_wndColorCtrl1.SetupColor( clrFillHint, false );

		m_wndColorCtrl2.m_selClrs.m_clrCurrent = 
			m_wndColorCtrl2.m_selClrs.m_clrPrev = 
			m_wndColorCtrl2.m_selClrs.m_clrNew = clrAccentHint;
		m_wndColorCtrl2.SetupColor( clrAccentHint, false );

		OnColorChanged(
			(WPARAM)m_wndColorCtrl1.GetSafeHwnd(),
			(LPARAM)&m_wndColorCtrl1.m_selClrs
			);

		OnColorChanged(
			(WPARAM)m_wndColorCtrl2.GetSafeHwnd(),
			(LPARAM)&m_wndColorCtrl2.m_selClrs
			);
	}
}
