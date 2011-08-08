// ChildFormView.cpp : implementation file
//

#include "stdafx.h"
#include "StatusPanes.h"
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
	: CExtWA < CExtWS < CExtAFV < CFormView > > > ( CChildFormView::IDD, ((CWnd *)NULL) )
	, m_pWndEdit(NULL)
	, m_pWndSliderCtrl(NULL)
	, m_pWndButton(NULL)
	, m_pWndProgressCtrl(NULL)
	, m_pWndAnimateCtrl(NULL)
	, m_pWndLabel(NULL)
	, m_pWndToolBar(NULL)
	, m_pWndZoomScrollBar(NULL)
	, m_pWndScrollBar(NULL)
{
	//{{AFX_DATA_INIT(CChildFormView)
	m_nPosition = 1;
	//}}AFX_DATA_INIT
	m_bInitComplete = false;
	m_pMainFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
}

CChildFormView::~CChildFormView()
{
}

void CChildFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChildFormView)
	DDX_Control(pDX, IDC_RADIO_TYPE_TEXT, m_wndRadioText);
	DDX_Control(pDX, IDC_RADIO_TYPE_TOOLBAR, m_wndRadioToolbar);
	DDX_Control(pDX, IDC_RADIO_TYPE_STATIC, m_wndRadioStatic);
	DDX_Control(pDX, IDC_RADIO_TYPE_SLIDER, m_wndRadioSlider);
	DDX_Control(pDX, IDC_RADIO_TYPE_PROGRESS, m_wndRadioProgress);
	DDX_Control(pDX, IDC_RADIO_TYPE_EDIT, m_wndRadioEdit);
	DDX_Control(pDX, IDC_RADIO_TYPE_BUTTON, m_wndRadioButton);
	DDX_Control(pDX, IDC_RADIO_TYPE_ANIM_CTRL, m_wndRadioAnimCtrl);
	DDX_Control(pDX, IDC_RADIO_TYPE_ZOOM, m_wndRadioZoom);
	DDX_Control(pDX, IDC_RADIO_TYPE_SCROLLBAR, m_wndRadioScrollBar);
	DDX_Control(pDX, IDC_STATIC3, m_wndStatic3);
	DDX_Control(pDX, IDC_STATIC2, m_wndStatic2);
	DDX_Control(pDX, IDC_STATIC1, m_wndStatic1);
	DDX_Control(pDX, IDC_DISABLE_PANE, m_btnDisablePane);
	DDX_Control(pDX, IDC_SPIN_POSITION, m_SpinPosition);
	DDX_Control(pDX, IDC_REMOVE_PANE, m_btnRemovePane);
	DDX_Control(pDX, IDC_ADD_PANE, m_btnAddPane);
	DDX_Control(pDX, IDC_HIDE_TEXT_ON_DISABLED_PANES, m_chkHideTextOnDisabledPanes);
	DDX_Control(pDX, IDC_OUTER_RECT_IN_FIRST_BAND, m_chkOuterRectInFirstBand);
	DDX_Control(pDX, IDC_DRAW_PANE_SEPARATORS, m_chkDrawPaneSeparators);
	DDX_Text(pDX, IDC_POSITION, m_nPosition);
	DDV_MinMaxInt(pDX, m_nPosition, 0, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChildFormView, CFormView)
	//{{AFX_MSG_MAP(CChildFormView)
	ON_BN_CLICKED(IDC_DRAW_PANE_SEPARATORS, OnDrawPaneSeparators)
	ON_BN_CLICKED(IDC_OUTER_RECT_IN_FIRST_BAND, OnOuterRectInFirstBand)
	ON_BN_CLICKED(IDC_HIDE_TEXT_ON_DISABLED_PANES, OnHideTextOnDisabledPanes)
	ON_BN_CLICKED(IDC_ADD_PANE, OnAddPane)
	ON_BN_CLICKED(IDC_REMOVE_PANE, OnRemovePane)
	ON_BN_CLICKED(IDC_DISABLE_PANE, OnDisablePane)
	ON_BN_CLICKED(IDC_STATUS_BAR_BUTTON, OnStatusBarButton)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(IDC_STATUS_BAR_BUTTON, OnUpdateStatusBarButton)
	ON_UPDATE_COMMAND_UI(IDC_STATUS_BAR_PROGRESS, OnUpdateStatusBarProgress)
	ON_UPDATE_COMMAND_UI(IDC_STATUS_BAR_ANIMATE, OnUpdateStatusBarAnimate)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_POSITION, OnChangePosition)
	//}}AFX_MSG_MAP
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

//////////////////////////////////////////////////////////////////////////

void CChildFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ShowSizeGrip(FALSE);
	m_SpinPosition.SetRange(0,20);
	m_chkHideTextOnDisabledPanes.SetCheck(0);
	OnHideTextOnDisabledPanes();
	m_bInitComplete = true;

	m_wndRadioText.SetCheck( 1 );

	CheckControls();
}

//////////////////////////////////////////////////////////////////////////

void CChildFormView::OnDrawPaneSeparators() 
{
	if( m_chkDrawPaneSeparators.GetCheck() == 1 ){
		m_pMainFrame->m_wndStatusBar.DrawPaneSeparatorsSet(true);
		m_chkOuterRectInFirstBand.EnableWindow(0);
	}
	else{
		m_pMainFrame->m_wndStatusBar.DrawPaneSeparatorsSet(false);
		m_chkOuterRectInFirstBand.EnableWindow(1);
	}

	if( m_pWndButton && ::IsWindow(m_pWndButton->m_hWnd)){
		if(m_chkDrawPaneSeparators.GetCheck() == 1){
			m_pWndButton->SetDrawBorder(1);
		}
		else{
			m_pWndButton->SetDrawBorder(0);
		}
	}
	
}

void CChildFormView::OnOuterRectInFirstBand() 
{
	m_pMainFrame->m_wndStatusBar.OuterRectInFirstBandSet( 
		m_chkOuterRectInFirstBand.GetCheck() == 1 
		);
}

void CChildFormView::OnHideTextOnDisabledPanes() 
{
	m_pMainFrame->m_wndStatusBar.HideTextOnDisabledPanesSet( 
		m_chkHideTextOnDisabledPanes.GetCheck() == 1 
		);
}

//////////////////////////////////////////////////////////////////////////

void CChildFormView::OnAddPane() 
{
	CheckControls();
	
	if( m_nPosition < 0 || m_nPosition > 20 )
		return;
	
	if( m_wndRadioText.GetCheck() ) // text
	{
		bool bRet = m_pMainFrame->m_wndStatusBar.AddPane(IDS_PANE_TEXT,m_nPosition);
		if(!bRet){
			AfxMessageBox(_T("Pane index out of range\nor pane with same ID already exists in the status bar"), MB_ICONERROR);
			return;
		}
		//m_pMainFrame->m_wndStatusBar.SetTipText( m_nPosition, _T("Tooltip") );
	}
	
	if( m_wndRadioEdit.GetCheck() ) // edit
	{
		m_pWndEdit = new CExtStatusBarEdit;
		if (!m_pWndEdit->Create(
			WS_CHILD|WS_VISIBLE|WS_TABSTOP, 
			CRect(0,0,0,0), 
			&m_pMainFrame->m_wndStatusBar, 
			0))
		{
			TRACE(_T("Failed to create edit control.\n"));
			return;
		}
		
		m_pWndEdit->SetFont(
			CFont::FromHandle(
			(HFONT)::GetStockObject(DEFAULT_GUI_FONT)
			)
			);
		
		m_pWndEdit->SetWindowText(_T("Type text here..."));
		
		bool bRet = m_pMainFrame->m_wndStatusBar.AddPane(IDS_PANE_EDIT,m_nPosition);
		if(!bRet){
			AfxMessageBox(_T("Pane index out of range\nor pane with same ID already exists in the status bar"), MB_ICONERROR);
			return;
		}
		
		int nIndex = m_pMainFrame->m_wndStatusBar.CommandToIndex(IDS_PANE_EDIT);
		if (nIndex == -1)
			return;
		
		m_pMainFrame->m_wndStatusBar.SetPaneWidth(nIndex, 100);
		m_pMainFrame->m_wndStatusBar.SetPaneControl(m_pWndEdit, IDS_PANE_EDIT, true);
		return;
	}
	
	if( m_wndRadioProgress.GetCheck() ) // progress ctrl
	{
		m_pWndProgressCtrl = new CExtStatusBarProgressCtrl;
		if (!m_pWndProgressCtrl->Create(
			WS_CHILD|WS_VISIBLE|WS_TABSTOP, //|PBS_SMOOTH
			CRect(0,0,0,0), 
			&m_pMainFrame->m_wndStatusBar, 
			IDC_STATUS_BAR_PROGRESS))
		{
			TRACE(_T("Failed to create progress control.\n"));
			return;
		}
		
		m_pWndProgressCtrl->ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_FRAMECHANGED);
		
		m_pWndProgressCtrl->SetRange(0,300);
		m_pWndProgressCtrl->SetPos(0);
		m_pWndProgressCtrl->SetStep(1);
		
		bool bRet = m_pMainFrame->m_wndStatusBar.AddPane(IDS_PANE_PROGRESS,m_nPosition);
		if(!bRet){
			AfxMessageBox(_T("Pane index out of range\nor pane with same ID already exists in the status bar"), MB_ICONERROR);
			return;
		}
		
		int nIndex = m_pMainFrame->m_wndStatusBar.CommandToIndex(IDS_PANE_PROGRESS);
		if (nIndex == -1)
			return;
		
		m_pMainFrame->m_wndStatusBar.SetPaneWidth(nIndex, 100);
		m_pMainFrame->m_wndStatusBar.SetPaneControl(m_pWndProgressCtrl, IDS_PANE_PROGRESS, true);
		
		m_pWndProgressCtrl->SetTimer(0,1,NULL);
		return;
	}
	
	if( m_wndRadioSlider.GetCheck() ) // slider ctrl
	{
		m_pWndSliderCtrl = new CExtStatusBarSliderCtrl;
		if (!m_pWndSliderCtrl->Create(
			WS_CHILD|WS_VISIBLE|WS_TABSTOP|TBS_HORZ, 
			CRect(0,0,0,0), 
			&m_pMainFrame->m_wndStatusBar, 
			IDC_STATUS_BAR_SLIDER))
		{
			TRACE(_T("Failed to create slider control.\n"));
			return;
		}
		m_pWndSliderCtrl->PmBridge_Install();
		
		bool bRet = m_pMainFrame->m_wndStatusBar.AddPane(IDS_PANE_SLIDER_CTRL,m_nPosition);
		if(!bRet){
			AfxMessageBox(_T("Pane index out of range\nor pane with same ID already exists in the status bar"), MB_ICONERROR);
			return;
		}
		
		int nIndex = m_pMainFrame->m_wndStatusBar.CommandToIndex(IDS_PANE_SLIDER_CTRL);
		if (nIndex == -1)
			return;
		
		m_pMainFrame->m_wndStatusBar.SetPaneWidth(nIndex, 80);
		m_pMainFrame->m_wndStatusBar.SetPaneControl(m_pWndSliderCtrl, IDS_PANE_SLIDER_CTRL, true);
		return;
	}
	
	if( m_wndRadioButton.GetCheck() ) // button
	{
		m_pWndButton = new CExtStatusBarButton;
		if (!m_pWndButton->Create(
			_T("Button"),
			WS_CHILD|WS_VISIBLE|WS_TABSTOP,
			CRect(0,0,0,0), 
			&m_pMainFrame->m_wndStatusBar, 
			IDC_STATUS_BAR_BUTTON))
		{
			TRACE(_T("Failed to create button control.\n"));
			return;
		}
		m_pWndButton->SetFlat();
		m_pWndButton->SetDrawBorder( m_chkDrawPaneSeparators.GetCheck()==1 );
		
		bool bRet = m_pMainFrame->m_wndStatusBar.AddPane(IDS_PANE_BUTTON,m_nPosition);
		if(!bRet){
			AfxMessageBox(_T("Pane index out of range\nor pane with same ID already exists in the status bar"), MB_ICONERROR);
			return;
		}
		
		int nIndex = m_pMainFrame->m_wndStatusBar.CommandToIndex(IDS_PANE_BUTTON);
		if (nIndex == -1)
			return;
		
		m_pMainFrame->m_wndStatusBar.SetPaneWidth(nIndex, 50);
		m_pMainFrame->m_wndStatusBar.SetPaneControl(m_pWndButton, IDS_PANE_BUTTON, true);
		return;
	}
	
	if( m_wndRadioAnimCtrl.GetCheck() ) // animate ctrl
	{
		m_pWndAnimateCtrl = new CExtStatusBarAnimateCtrl;
		if (!m_pWndAnimateCtrl->Create(
			WS_CHILD|WS_VISIBLE|WS_TABSTOP|ACS_CENTER|ACS_AUTOPLAY|ACS_TRANSPARENT,
			CRect(0,0,0,0), 
			&m_pMainFrame->m_wndStatusBar, 
			IDC_STATUS_BAR_ANIMATE))
		{
			TRACE(_T("Failed to create animate control.\n"));
			return;
		}
		m_pWndAnimateCtrl->Open(IDR_DOWNLOAD);
		
		bool bRet = m_pMainFrame->m_wndStatusBar.AddPane(IDS_PANE_ANIMATE,m_nPosition);
		if(!bRet){
			AfxMessageBox(_T("Pane index out of range\nor pane with same ID already exists in the status bar"), MB_ICONERROR);
			return;
		}
		
		int nIndex = m_pMainFrame->m_wndStatusBar.CommandToIndex(IDS_PANE_ANIMATE);
		if (nIndex == -1)
			return;
		
		m_pMainFrame->m_wndStatusBar.SetPaneWidth(nIndex, 16);
		m_pMainFrame->m_wndStatusBar.SetPaneControl( m_pWndAnimateCtrl, IDS_PANE_ANIMATE, false );
		return;
	}
	
	if( m_wndRadioStatic.GetCheck() ) // label
	{
		m_pWndLabel = new CExtStatusBarLabel;
		if (!m_pWndLabel->Create(
			_T("Prof-UIS"),
			WS_CHILD|WS_VISIBLE|WS_TABSTOP|SS_CENTER,
			CRect(0,0,0,0), 
			&m_pMainFrame->m_wndStatusBar, 
			0))
		{
			TRACE(_T("Failed to create label control.\n"));
			return;
		}
		
		m_pWndLabel->SetFont(
			CFont::FromHandle(
			(HFONT)::GetStockObject(DEFAULT_GUI_FONT)
			)
			);
		
		bool bRet = m_pMainFrame->m_wndStatusBar.AddPane(IDS_PANE_LABEL,m_nPosition);
		if(!bRet){
			AfxMessageBox(_T("Pane index out of range\nor pane with same ID already exists in the status bar"), MB_ICONERROR);
			return;
		}
		
		int nIndex = m_pMainFrame->m_wndStatusBar.CommandToIndex(IDS_PANE_LABEL);
		if (nIndex == -1)
			return;
		
		m_pMainFrame->m_wndStatusBar.SetPaneWidth(nIndex, 70);
		m_pMainFrame->m_wndStatusBar.SetPaneControl(m_pWndLabel, IDS_PANE_LABEL, true);

		return;
	}

	if( m_wndRadioToolbar.GetCheck() ) // toolbar
	{
		m_pWndToolBar = new CExtStatusBarToolbar;
		if(		(! m_pWndToolBar->Create(
					NULL,
					&m_pMainFrame->m_wndStatusBar,
					UINT(IDC_STATIC),
					WS_CHILD|WS_VISIBLE
						|CBRS_ALIGN_BOTTOM|CBRS_TOOLTIPS
						|CBRS_FLYBY|CBRS_SIZE_DYNAMIC
					) )
			|| (! m_pWndToolBar->LoadToolBar(IDR_TOOLBAR_SMALL_ICONS) )
			)
		{
			ASSERT( FALSE );
			return;
		}
		
		m_pWndToolBar->SetOwner( this );

		bool bRet = m_pMainFrame->m_wndStatusBar.AddPane(IDS_PANE_TOOLBAR,m_nPosition);
		if( !bRet )
		{
			AfxMessageBox(_T("Pane index out of range\nor pane with same ID already exists in the status bar"), MB_ICONERROR);
			return;
		}
		
		int nIndex = m_pMainFrame->m_wndStatusBar.CommandToIndex(IDS_PANE_TOOLBAR);
		if( nIndex == -1 )
			return;
		
		CSize szTBSize = m_pWndToolBar->CalcDynamicLayout( -1, LM_HORZ|LM_COMMIT );
		m_pMainFrame->m_wndStatusBar.SetPaneWidth( nIndex, szTBSize.cx );
		m_pMainFrame->m_wndStatusBar.SetPaneControl( m_pWndToolBar, IDS_PANE_TOOLBAR, true );
	
		return;
	}

	if( m_wndRadioZoom.GetCheck() ) // zoom
	{
		m_pWndZoomScrollBar = new CExtStatusBarZoomScrollBar;
		if(	! m_pWndZoomScrollBar->Create(
				WS_CHILD|WS_VISIBLE|SBS_HORZ,
				CRect(0,0,0,0),
				&m_pMainFrame->m_wndStatusBar,
				UINT(IDC_STATIC)
				)
			)
		{
			ASSERT( FALSE );
			return;
		}
		m_pWndZoomScrollBar->SetScrollRange( 0, 100 );
		m_pWndZoomScrollBar->SetScrollPos( 50 );
		m_pWndZoomScrollBar->m_bEnabledToolTips         = true;
		m_pWndZoomScrollBar->m_strTipTextForThumbButton = _T("Zoom");
		m_pWndZoomScrollBar->m_strTipTextForUpButton    = _T("Zoom Out");
		m_pWndZoomScrollBar->m_strTipTextForDownButton  = _T("Zoom In");
		m_pWndZoomScrollBar->m_eSO = CExtScrollBar::__ESO_BOTTOM;
		
		m_pWndZoomScrollBar->SetOwner( this );

		bool bRet = m_pMainFrame->m_wndStatusBar.AddPane( IDS_PANE_ZOOM, m_nPosition );
		if( !bRet )
		{
			AfxMessageBox(_T("Pane index out of range\nor pane with same ID already exists in the status bar"), MB_ICONERROR);
			return;
		}
		
		int nIndex = m_pMainFrame->m_wndStatusBar.CommandToIndex(IDS_PANE_ZOOM);
		if( nIndex == -1 )
			return;
		
		m_pMainFrame->m_wndStatusBar.SetPaneWidth( nIndex, 120 );
		m_pMainFrame->m_wndStatusBar.SetPaneControl( m_pWndZoomScrollBar, IDS_PANE_ZOOM, true );
	
		return;
	}

	if( m_wndRadioScrollBar.GetCheck() ) // scrollbar
	{
		m_pWndScrollBar = new CExtStatusBarScrollBar;
		if(	! m_pWndScrollBar->Create(
				WS_CHILD|WS_VISIBLE|SBS_HORZ,
				CRect(0,0,0,0),
				&m_pMainFrame->m_wndStatusBar,
				UINT(IDC_STATIC)
				)
			)
		{
			ASSERT( FALSE );
			return;
		}
		m_pWndScrollBar->SetScrollRange( 0, 100 );
		m_pWndScrollBar->SetScrollPos( 50 );
		m_pWndScrollBar->m_bEnabledToolTips          = true;
		m_pWndScrollBar->m_strTipTextForThumbButton  = _T("Scroll");
		m_pWndScrollBar->m_strTipTextForUpButton     = _T("Scroll Left");
		m_pWndScrollBar->m_strTipTextForDownButton   = _T("Scroll Right");
		m_pWndScrollBar->m_strTipTextForPageUpArea   = _T("Page Up");
		m_pWndScrollBar->m_strTipTextForPageDownArea = _T("Page Down");
		m_pWndScrollBar->m_eSO = CExtScrollBar::__ESO_BOTTOM;
		
		m_pWndScrollBar->SetOwner( this );

		bool bRet = m_pMainFrame->m_wndStatusBar.AddPane( IDS_PANE_SCROLLBAR, m_nPosition );
		if( !bRet )
		{
			AfxMessageBox(_T("Pane index out of range\nor pane with same ID already exists in the status bar"), MB_ICONERROR);
			return;
		}
		
		int nIndex = m_pMainFrame->m_wndStatusBar.CommandToIndex(IDS_PANE_SCROLLBAR);
		if( nIndex == -1 )
			return;
		
		m_pMainFrame->m_wndStatusBar.SetPaneWidth( nIndex, 120 );
		m_pMainFrame->m_wndStatusBar.SetPaneControl( m_pWndScrollBar, IDS_PANE_SCROLLBAR, true );
	
		return;
	}

}

//////////////////////////////////////////////////////////////////////////

void CChildFormView::OnRemovePane() 
{
	UpdateData();
	if( m_nPosition < 0 || m_nPosition > 20 )
		return;

	if(!m_pMainFrame->m_wndStatusBar.RemovePane(
			m_pMainFrame->m_wndStatusBar.GetItemID(m_nPosition))
		)
	{
		CString s;
		s.Format(_T("Pane %d was not found in the status bar \nor must be at least one of them."), m_nPosition);
		AfxMessageBox(s, MB_ICONERROR);
	}
}

//////////////////////////////////////////////////////////////////////////

void CChildFormView::OnStatusBarButton() 
{
	AfxMessageBox(_T("StatusBar Button Clicked."));
}

//////////////////////////////////////////////////////////////////////////

void CChildFormView::OnDisablePane() 
{
	UpdateData();
	if( m_nPosition < 0 || m_nPosition > 20 )
		return;
	
	UINT nID, nStyle;
	int cxWidth;
	
	m_pMainFrame->m_wndStatusBar.GetPaneInfo(m_nPosition, nID, nStyle, cxWidth);
	
	bool bDisabled = ((nStyle&SBPS_DISABLED) == 0);

	m_pMainFrame->m_wndStatusBar.SetPaneStyle(
		m_nPosition,
		//nID,
		bDisabled 
			? nStyle | SBPS_DISABLED 
			: nStyle & ~SBPS_DISABLED
		//`cxWidth
		);
}

//////////////////////////////////////////////////////////////////////////

void CChildFormView::CheckControls()
{
	if( ! m_bInitComplete )
		return;
	UpdateData();
	if( m_nPosition < 0 )
	{
		m_nPosition = 0;
		UpdateData(0);
		return;
	}
//	if( m_nPosition > 20 )
//	{
//		m_nPosition = 20;
//		UpdateData(0);
//		return;
//	}
int nCount = m_pMainFrame->m_wndStatusBar.GetPaneCount();
	if( m_nPosition >= nCount )
	{
		m_btnDisablePane.EnableWindow(FALSE);
		m_btnRemovePane.EnableWindow(FALSE);
		if( m_nPosition != nCount ){
			m_btnAddPane.EnableWindow(FALSE);
			return;
		}
	}
	else
	{
		m_btnDisablePane.EnableWindow(TRUE);
		m_btnRemovePane.EnableWindow(TRUE);
	}

	bool bFinded = false;
	int i=0;

	if( m_wndRadioText.GetCheck() ){
		for(i=0; i<m_pMainFrame->m_wndStatusBar.GetPaneCount(); i++)
			if( IDS_PANE_TEXT == m_pMainFrame->m_wndStatusBar.GetItemID(i) ){
				bFinded = true;
				break;
			}
	}
	if( m_wndRadioEdit.GetCheck() ){
		for(i=0; i<m_pMainFrame->m_wndStatusBar.GetPaneCount(); i++)
			if( IDS_PANE_EDIT == m_pMainFrame->m_wndStatusBar.GetItemID(i) ){
				bFinded = true;
				break;
			}
	}
	if( m_wndRadioProgress.GetCheck() ){
		for(i=0; i<m_pMainFrame->m_wndStatusBar.GetPaneCount(); i++)
			if( IDS_PANE_PROGRESS == m_pMainFrame->m_wndStatusBar.GetItemID(i) ){
				bFinded = true;
				break;
			}
	}
	if( m_wndRadioSlider.GetCheck() ){
		for(i=0; i<m_pMainFrame->m_wndStatusBar.GetPaneCount(); i++)
			if( IDS_PANE_SLIDER_CTRL == m_pMainFrame->m_wndStatusBar.GetItemID(i) ){
				bFinded = true;
				break;
			}
	}
	if( m_wndRadioButton.GetCheck() ){
		for(i=0; i<m_pMainFrame->m_wndStatusBar.GetPaneCount(); i++)
			if( IDS_PANE_BUTTON == m_pMainFrame->m_wndStatusBar.GetItemID(i) ){
				bFinded = true;
				break;
			}
	}
	if( m_wndRadioAnimCtrl.GetCheck() ){
		for(i=0; i<m_pMainFrame->m_wndStatusBar.GetPaneCount(); i++)
			if( IDS_PANE_ANIMATE == m_pMainFrame->m_wndStatusBar.GetItemID(i) ){
				bFinded = true;
				break;
			}
	}
	if( m_wndRadioStatic.GetCheck() ){
		for(i=0; i<m_pMainFrame->m_wndStatusBar.GetPaneCount(); i++)
			if( IDS_PANE_LABEL == m_pMainFrame->m_wndStatusBar.GetItemID(i) ){
				bFinded = true;
				break;
			}
	}
	if( m_wndRadioToolbar.GetCheck() ){
		for(i=0; i<m_pMainFrame->m_wndStatusBar.GetPaneCount(); i++)
			if( IDS_PANE_TOOLBAR == m_pMainFrame->m_wndStatusBar.GetItemID(i) ){
				bFinded = true;
				break;
			}
	}
	if( m_wndRadioZoom.GetCheck() ){
		for(i=0; i<m_pMainFrame->m_wndStatusBar.GetPaneCount(); i++)
			if( IDS_PANE_ZOOM == m_pMainFrame->m_wndStatusBar.GetItemID(i) ){
				bFinded = true;
				break;
			}
	}
	if( m_wndRadioScrollBar.GetCheck() ){
		for(i=0; i<m_pMainFrame->m_wndStatusBar.GetPaneCount(); i++)
			if( IDS_PANE_SCROLLBAR == m_pMainFrame->m_wndStatusBar.GetItemID(i) ){
				bFinded = true;
				break;
			}
	}

	m_btnAddPane.EnableWindow( !bFinded );
}


BOOL CChildFormView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	BOOL bRetVal = CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	if(		nCode == CN_COMMAND && pExtra == NULL
		&&	(	nID == IDC_POSITION
			||	nID == IDC_SPIN_POSITION
			||	nID == IDC_ADD_PANE
			||	nID == IDC_REMOVE_PANE
			||	nID == IDC_DISABLE_PANE
			||	nID == IDC_RADIO_TYPE_TEXT
			||	nID == IDC_RADIO_TYPE_EDIT
			||	nID == IDC_RADIO_TYPE_PROGRESS
			||	nID == IDC_RADIO_TYPE_SLIDER
			||	nID == IDC_RADIO_TYPE_BUTTON
			||	nID == IDC_RADIO_TYPE_ANIM_CTRL
			||	nID == IDC_RADIO_TYPE_STATIC
			||	nID == IDC_RADIO_TYPE_TOOLBAR
			||	nID == IDC_RADIO_TYPE_ZOOM
			||	nID == IDC_RADIO_TYPE_SCROLLBAR
			)
		)
		CheckControls();

	if(		nCode == CN_UPDATE_COMMAND_UI
		&& (	nID == ID_CMD_NAVIGATE_TOP
			||	nID == ID_CMD_NAVIGATE_BOTTOM
			||	nID == ID_CMD_NAVIGATE_FOCUS 
			||	nID == ID_CMD_NAVIGATE_PAGE_UP
			||	nID == ID_CMD_NAVIGATE_PAGE_DOWN
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

void CChildFormView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
}

void CChildFormView::OnUpdateStatusBarButton(CCmdUI* pCmdUI) 
{
	if( m_pWndButton && ::IsWindow(m_pWndButton->m_hWnd))
	{
		pCmdUI->Enable( m_pWndButton->IsWindowEnabled() );
	}
}

void CChildFormView::OnUpdateStatusBarProgress(CCmdUI* pCmdUI) 
{
	if( m_pWndProgressCtrl && ::IsWindow(m_pWndProgressCtrl->m_hWnd))
	{
		pCmdUI->Enable( m_pWndProgressCtrl->IsWindowEnabled() );
	}
}

void CChildFormView::OnUpdateStatusBarAnimate(CCmdUI* pCmdUI) 
{
	if( m_pWndAnimateCtrl && ::IsWindow(m_pWndAnimateCtrl->m_hWnd))
	{
		static BOOL bEnabledPrev = true;
		BOOL bEnabled = m_pWndAnimateCtrl->IsWindowEnabled();
		if( bEnabled != bEnabledPrev )
		{
			bEnabled 
				? m_pWndAnimateCtrl->Open(IDR_DOWNLOAD)
				: m_pWndAnimateCtrl->Stop();
			pCmdUI->Enable( bEnabled );		
			bEnabledPrev = bEnabled;
		}
	}
}

void CChildFormView::OnChangePosition() 
{
	ASSERT_VALID( this );
	CheckControls();
}

/////////////////////////////////////////////////////////////////////////////
// CExtStatusBarSliderCtrl

BEGIN_MESSAGE_MAP(CExtStatusBarSliderCtrl, CExtSliderWnd)
	//{{AFX_MSG_MAP(CExtStatusBarSliderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HBRUSH CExtStatusBarSliderCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	ASSERT_VALID( this );
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		COLORREF clrBk = GetBkColor();
	
		CRect rcClient;
		GetClientRect( &rcClient );
		
		CBitmap bmpGradient;
		if( bmpGradient.CreateCompatibleBitmap( 
				pDC, 
				rcClient.Width(), 
				rcClient.Height() 
				) 
			)
		{
			CBitmap * pBmpOld = pDC->SelectObject( &bmpGradient );
			bool bTransparent = false;
			if( clrBk == COLORREF(-1L) )
			{
				CExtStatusControlBar * pWndParent = 
					DYNAMIC_DOWNCAST( CExtStatusControlBar, GetParent() );
				ASSERT( pWndParent != NULL );

				CRect rcPaint;
				pWndParent->GetWindowRect( &rcPaint );
				ScreenToClient( &rcPaint );

				CRect _rcClient( rcClient );
				ClientToScreen( &_rcClient );
				pWndParent->ScreenToClient( &_rcClient );
				
				if( pWndParent->m_bCompleteRepaint )
				{
					pDC->OffsetViewportOrg( 
						_rcClient.left, 
						0 
						);

					bool bRet = 
						PmBridge_GetPM()->StatusBar_EraseBackground(
							*pDC,
							rcPaint,
							pWndParent
							);

					pDC->OffsetViewportOrg( 
						-_rcClient.left, 
						0 
						);

					if( !bRet )
					{
						bool bTransparent = false;
						if( PmBridge_GetPM()->GetCb2DbTransparentMode(pWndParent) )
						{
							if( PmBridge_GetPM()->PaintDockerBkgnd( true, *pDC, this ) )
								bTransparent = true;
						}
						if( ! bTransparent )
							pDC->FillSolidRect( 
								&rcPaint, 
								PmBridge_GetPM()->GetColor( CExtPaintManager::CLR_3DFACE_OUT, pWndParent ) 
								);
					}

					INT nIndex = pWndParent->CommandToIndex( IDS_PANE_LABEL );
					if( nIndex >= 0 )
					{
						pDC->OffsetViewportOrg( 
							_rcClient.left, 
							0 
							);
						PmBridge_GetPM()->StatusBar_ErasePaneBackground(
							*pDC,
							nIndex,
							rcPaint,
							pWndParent
							);
						pDC->OffsetViewportOrg( 
							-_rcClient.left, 
							0 
							);
						bTransparent = true;
					}
				} // if( pWndParent->m_bCompleteRepaint )
			}
			pDC->SelectObject( pBmpOld );
			if( bTransparent )
			{
				if( m_brBack.GetSafeHandle() != NULL )
					m_brBack.DeleteObject();
				if( m_brBack.CreatePatternBrush( &bmpGradient ) )
				{
					m_clrBackgroundPrev = NULL;
					return m_brBack;
				}
			}
		}

		if( clrBk == COLORREF(-1L) )
			clrBk = PmBridge_GetPM()->GetColor( CExtPaintManager::CLR_3DFACE_OUT, this );

		if( m_clrBackgroundPrev != clrBk )
		{
			if( m_brBack.GetSafeHandle() != NULL )
				m_brBack.DeleteObject();
			m_brBack.CreateSolidBrush( clrBk );
			m_clrBackgroundPrev = clrBk;
		}
		return m_brBack;
	}
	else
		return (HBRUSH)Default();
}

/////////////////////////////////////////////////////////////////////////////
// CExtStatusBarAnimateCtrl

IMPLEMENT_DYNCREATE( CExtStatusBarAnimateCtrl, CAnimateCtrl );		

CExtStatusBarAnimateCtrl::CExtStatusBarAnimateCtrl()
	: m_clrBackgroundPrev( COLORREF(-1L) )
{
}

CExtStatusBarAnimateCtrl::~CExtStatusBarAnimateCtrl()
{
	if( m_brBack.GetSafeHandle() != NULL )
		m_brBack.DeleteObject();
}

BEGIN_MESSAGE_MAP(CExtStatusBarAnimateCtrl, CAnimateCtrl)
	//{{AFX_MSG_MAP(CExtStatusBarAnimateCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HBRUSH CExtStatusBarAnimateCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	ASSERT_VALID( this );
	pDC;
	if( nCtlColor == CTLCOLOR_STATIC )
	{
		COLORREF clrBk = g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_OUT, this );
		if( m_clrBackgroundPrev != clrBk )
		{
			if( m_brBack.GetSafeHandle() != NULL )
				m_brBack.DeleteObject();
			m_brBack.CreateSolidBrush( clrBk );
			m_clrBackgroundPrev = clrBk;
		}
		return m_brBack;
	}
	else
		return (HBRUSH)Default();
}

