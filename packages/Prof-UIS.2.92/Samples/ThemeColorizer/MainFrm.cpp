// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ThemeColorizer.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( CColorizedThemeOffice2003, CExtPaintManagerOffice2003, VERSIONABLE_SCHEMA|1 );

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP

	ON_COMMAND_EX( ID_VIEW_MENUBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_MENUBAR, OnUpdateControlBarMenu )

	ON_COMMAND_EX( ID_VIEW_TOOLBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI( ID_VIEW_TOOLBAR, OnUpdateControlBarMenu )

	ON_COMMAND_EX( IDR_TOOLBAR_SEEK, OnBarCheck )
	ON_UPDATE_COMMAND_UI( IDR_TOOLBAR_SEEK, OnUpdateControlBarMenu )
	
	ON_COMMAND_EX( IDR_TOOLBAR_ZOOM, OnBarCheck )
	ON_UPDATE_COMMAND_UI( IDR_TOOLBAR_ZOOM, OnUpdateControlBarMenu )

	ON_COMMAND_EX(ID_VIEW_IEDBAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_IEDBAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_0, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_0, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_1, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_1, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_RESIZABLE_BAR_2, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESIZABLE_BAR_2, OnUpdateControlBarMenu)

	ON_REGISTERED_MESSAGE(
		CExtControlBar::g_nMsgDrawTrackedArea,
		OnDrawControlBarTrackedArea
		)

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
	: m_pPaintManager(NULL)
	, m_rcTrackerEntireArea( 0, 0, 0, 0 )
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
	{
		m_pPaintManager = new CColorizedThemeOffice2003;
		g_PaintManager.InstallPaintManager( m_pPaintManager );
	}
	else
	{
		m_pPaintManager =
			STATIC_DOWNCAST(
				CColorizedThemeOffice2003,
				g_PaintManager.GetPM()
				);
	}

	// window placement persistence
	::memset( &m_dataFrameWP, 0, sizeof(WINDOWPLACEMENT) );
	m_dataFrameWP.length = sizeof(WINDOWPLACEMENT);
	m_dataFrameWP.showCmd = SW_HIDE;

	m_pWndView = NULL;

CBitmap _bmpPattern;
	if( _bmpPattern.LoadBitmap(IDB_BITMAP_TRACKER_PATTERN) )
	{
		VERIFY(
			m_brushTracker.CreatePatternBrush(
				&_bmpPattern
				)
			);
	} // if( _bmpPattern.LoadBitmap(IDB_BITMAP_TRACKER_PATTERN) )
#ifdef _DEBUG
	else
	{
		ASSERT( FALSE );
	} // else from if( _bmpPattern.LoadBitmap(IDB_BITMAP_TRACKER_PATTERN) )
#endif // _DEBUG
}

CMainFrame::~CMainFrame()
{
	if( m_bmpTrackerCache.GetSafeHandle() != NULL )
		m_bmpTrackerCache.DeleteObject();
}

void CMainFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
//	CFrameWnd::OnUpdateControlBarMenu( pCmdUI );
	CExtControlBar::DoFrameBarCheckUpdate(
		this,
		pCmdUI,
		false
		);
}

BOOL CMainFrame::OnBarCheck(UINT nID)
{
//	return CFrameWnd::OnBarCheck( nID );
	return
		CExtControlBar::DoFrameBarCheckCmd(
			this,
			nID,
			false
			);
}

static UINT g_statBasicCommands[] =
{
	ID_APP_ABOUT,
	ID_APP_EXIT,
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
	ID_VIEW_MENUBAR,
	ID_VIEW_TOOLBAR,
	ID_VIEW_STATUS_BAR,
	IDR_TOOLBAR_SEEK,
	IDR_TOOLBAR_ZOOM,
	ID_VIEW_IEDBAR,

	ID_IED_TOOL_RECT_SELECTION,
	ID_IED_TOOL_COLOR_SELECTION,
	ID_IED_TOOL_FILL,
	ID_IED_TOOL_MAGNIFICATION,
	ID_IED_TOOL_PENCIL,
	ID_IED_TOOL_BRUSH,
	ID_IED_TOOL_AIR_BRUSH,
	ID_IED_TOOL_LINE,
	ID_IED_TOOL_RECTANGLE,
	ID_IED_TOOL_ROUNDED_RECTANGLE,
	ID_IED_TOOL_ELLIPSE,            
	ID_IED_TOOL_MAGNIFICATION_01X,  
	ID_IED_TOOL_MAGNIFICATION_02X,  
	ID_IED_TOOL_MAGNIFICATION_03X,  
	ID_IED_TOOL_MAGNIFICATION_04X,  
	ID_IED_TOOL_MAGNIFICATION_05X,  
	ID_IED_TOOL_MAGNIFICATION_06X,  
	ID_IED_TOOL_MAGNIFICATION_07X,  
	ID_IED_TOOL_MAGNIFICATION_08X,  
	ID_IED_TOOL_MAGNIFICATION_09X,  
	ID_IED_TOOL_MAGNIFICATION_10X,  
	ID_IED_TOOL_MAGNIFICATION_11X,  
	ID_IED_TOOL_MAGNIFICATION_12X,  
	ID_IED_TOOL_MAGNIFICATION_13X,  
	ID_IED_TOOL_MAGNIFICATION_14X,  
	ID_IED_TOOL_MAGNIFICATION_15X,  
	ID_IED_TOOL_MAGNIFICATION_16X,  
	ID_IED_TOOL_BRUSH_CIRCLE_SMALL, 
	ID_IED_TOOL_BRUSH_CIRCLE_MEDIUM,
	ID_IED_TOOL_BRUSH_SQUARE_SMALL, 
	ID_IED_TOOL_BRUSH_SQUARE_MEDIUM,
	ID_IED_TOOL_BRUSH_LINE_LD_SMALL,
	ID_IED_TOOL_BRUSH_LINE_LD_MEDIUM,
	ID_IED_TOOL_BRUSH_LINE_LD_LARGE, 
	ID_IED_TOOL_BRUSH_LINE_RD_SMALL, 
	ID_IED_TOOL_BRUSH_LINE_RD_MEDIUM,
	ID_IED_TOOL_BRUSH_LINE_RD_LARGE, 
	ID_IED_TOOL_AIR_BRUSH_SMALL,     
	ID_IED_TOOL_AIR_BRUSH_MEDIUM,    
	ID_IED_TOOL_AIR_BRUSH_LARGE,     
	ID_IED_TOOL_LINE_CIRCLE_SMALL,   
	ID_IED_TOOL_LINE_CIRCLE_MEDIUM,  
	ID_IED_TOOL_LINE_SQUARE_SMALL,   
	ID_IED_TOOL_LINE_SQUARE_MEDIUM,  
	ID_IED_TOOL_LINE_LINE_LD_SMALL,  
	ID_IED_TOOL_LINE_LINE_LD_MEDIUM, 
	ID_IED_TOOL_LINE_LINE_LD_LARGE,  
	ID_IED_TOOL_LINE_LINE_RD_SMALL,  
	ID_IED_TOOL_LINE_LINE_RD_MEDIUM, 
	ID_IED_TOOL_LINE_LINE_RD_LARGE,  
	ID_IED_TOOL_RECTANGLE_SMALLER,   
	ID_IED_TOOL_RECTANGLE_SMALL,     
	ID_IED_TOOL_RECTANGLE_LARGE,     
	ID_IED_TOOL_OUTLINED_RECTANGLE_SMALLER,
	ID_IED_TOOL_OUTLINED_RECTANGLE_SMALL,
	ID_IED_TOOL_OUTLINED_RECTANGLE_LARGE,
	ID_IED_TOOL_ROUNDED_RECTANGLE_SMALLER,
	ID_IED_TOOL_ROUNDED_RECTANGLE_SMALL,
	ID_IED_TOOL_ROUNDED_RECTANGLE_LARGE,
	ID_IED_TOOL_ELLIPSE_SMALLER,
	ID_IED_TOOL_ELLIPSE_SMALL,
	ID_IED_TOOL_ELLIPSE_LARGE,  
	ID_IED_TOOL_OUTLINED_ROUNDED_RECTANGLE_SMALLER,
	ID_IED_TOOL_OUTLINED_ROUNDED_RECTANGLE_SMALL,
	ID_IED_TOOL_OUTLINED_ROUNDED_RECTANGLE_LARGE,
	ID_IED_TOOL_OUTLINED_ELLIPSE_SMALLER,
	ID_IED_TOOL_OUTLINED_ELLIPSE_SMALL,
	ID_IED_TOOL_OUTLINED_ELLIPSE_LARGE,

	ID_VIEW_RESIZABLE_BAR_0,
	ID_VIEW_RESIZABLE_BAR_1,
	ID_VIEW_RESIZABLE_BAR_2,
		
	0, // end of list
};

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	m_pWndView = new CChildFormView;
	if (!m_pWndView->Create(
		NULL, 
		NULL, 
		AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), 
		this, 
		AFX_IDW_PANE_FIRST, 
		NULL)
		)
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
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
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_IED_MENU_MAGNIFICATION
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_IED_MENU_BRUSH
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_IED_MENU_AIR_BRUSH
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_IED_MENU_LINE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_IED_MENU_RECTANGLE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_IED_MENU_OUTLINED_RECTANGLE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_IED_MENU_ROUNDED_RECTANGLE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_IED_MENU_ROUNDED_OUTLINED_RECTANGLE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_IED_MENU_ELLIPSE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromMenu(
			pApp->m_pszProfileName,
			IDR_IED_MENU_OUTLINED_ELLIPSE
			)
		);
	VERIFY(
		g_CmdManager->UpdateFromToolBar(
			pApp->m_pszProfileName,
			IDR_TOOLBAR_HELPER_ICONS
			)
		);

	//////////////////////////////////////////////////////////////////////////
	
	if( !m_wndMenuBar.Create(
			NULL,
			this,
			ID_VIEW_MENUBAR
			)
		)
    {
        TRACE0("Failed to create menubar\n");
        return -1;      // failed to create
    }

	if( !m_wndToolBar.Create(
			_T("ToolBar"),
			this,
			ID_VIEW_TOOLBAR
			)
		||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	/////////////////////////////////////////////////////////////////

	if( !m_wndToolBarSeek.Create(
		_T("Seek Slider"),
		this,
		IDR_TOOLBAR_SEEK)
		)
	{
		TRACE0("Failed to create toolbar m_wndToolBarSeek \n");
		return -1;      // fail to create
	}
	m_wndToolBarSeek.InitContentExpandButton();

	if( !m_wndToolBarZoom.Create(
		_T("Zoom Slider"),
		this,
		IDR_TOOLBAR_ZOOM)
		)
	{
		TRACE0("Failed to create toolbar m_wndToolBarZoom \n");
		return -1;      // fail to create
	}
	m_wndToolBarZoom.InitContentExpandButton();
	
	CExtCmdItem * pCmdItemSlider =
		g_CmdManager->CmdAllocPtr(
		pApp->m_pszProfileName,
		ID_MYEXTBTN_SLIDER
		);
	ASSERT( pCmdItemSlider != NULL );
	pCmdItemSlider->m_sToolbarText = _T("Zoom:");
	pCmdItemSlider->m_sMenuText = _T("Zoom slider");
	pCmdItemSlider->m_sTipTool
		= pCmdItemSlider->m_sTipStatus
		= _T("Zoom slider");
	
	CExtCmdItem * pCmdItemScroller =
		g_CmdManager->CmdAllocPtr(
		pApp->m_pszProfileName,
		ID_MYEXTBTN_SCROLLER
		);
	ASSERT( pCmdItemScroller != NULL );
	pCmdItemScroller->m_sToolbarText = _T("Seek:");
	pCmdItemScroller->m_sMenuText = _T("Seek slider");
	pCmdItemScroller->m_sTipTool
		= pCmdItemScroller->m_sTipStatus
		= _T("Seek slider");
	
	CExtBarSliderButton *pZoomSliderTBB =
		new CExtBarSliderButton(
		/////////////////
		// standard CExtBarButton parms
		/////////////////
		&m_wndToolBarZoom,
		ID_MYEXTBTN_SLIDER,
		0,
		/////////////////
		// CExtBarSliderButton specific parms
		/////////////////
		//
		// scroll total/pos/page
		50, 20, 0,
		// button extent horz(left/right)/vert(up/down) in pixels
		// (if zeros - use slider-like layout instead of scrollbar-like)
		0, 0,
		// total slider control extent horz/vert in pixels
		80, 80
		);
	VERIFY(
		m_wndToolBarZoom.InsertSpecButton(
		-1,
		pZoomSliderTBB,
		FALSE
		)
		);

	CExtBarSliderButton *pSeekScrollerTBB = new CExtBarSliderButton(
		/////////////////
		// standard CExtBarButton parms
		/////////////////
		&m_wndToolBarSeek,
		ID_MYEXTBTN_SCROLLER,
		0,
		/////////////////
		// CExtBarScrollerButton specific parms
		/////////////////
		//
		// scroll total/pos/page
		50, 20, 0,
		// button extent horz(left/right)/vert(up/down) in pixels
		// (if zeros - use slider-like layout instead of scrollbar-like)
		10, 10,
		// total slider control extent horz/vert in pixels
		150, 150
		);
	VERIFY(
		m_wndToolBarSeek.InsertSpecButton(
		-1,
		pSeekScrollerTBB,
		FALSE
		)
		);
	
	//////////////////////////////////////////////////////////////////////////
	
	if(	!m_wndToolBarIed.Create(
			NULL,
			this,
			ID_VIEW_IEDBAR
			)
		||
		!m_wndToolBarIed.LoadToolBar( ID_VIEW_IEDBAR )
		)
	{
		TRACE0("Failed to create image editor toolbar\n");
		return -1;      // fail to create
	}

CMenu _menu;
CExtBarButton * pTBB;
int nBtnIdx;
	
	VERIFY( _menu.LoadMenu(IDR_IED_MENU_MAGNIFICATION) );
	nBtnIdx = m_wndToolBarIed.CommandToIndex(ID_IED_TOOL_MAGNIFICATION);
	m_wndToolBarIed.SetButtonMenu( nBtnIdx, _menu.Detach() );
	pTBB = m_wndToolBarIed.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();

	VERIFY( _menu.LoadMenu(IDR_IED_MENU_BRUSH) );
	nBtnIdx = m_wndToolBarIed.CommandToIndex(ID_IED_TOOL_BRUSH);
	m_wndToolBarIed.SetButtonMenu( nBtnIdx, _menu.Detach() );
	pTBB = m_wndToolBarIed.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();

	VERIFY( _menu.LoadMenu(IDR_IED_MENU_AIR_BRUSH) );
	nBtnIdx = m_wndToolBarIed.CommandToIndex(ID_IED_TOOL_AIR_BRUSH);
	m_wndToolBarIed.SetButtonMenu( nBtnIdx, _menu.Detach() );
	pTBB = m_wndToolBarIed.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();

	VERIFY( _menu.LoadMenu(IDR_IED_MENU_LINE) );
	nBtnIdx = m_wndToolBarIed.CommandToIndex(ID_IED_TOOL_LINE);
	m_wndToolBarIed.SetButtonMenu( nBtnIdx, _menu.Detach() );
	pTBB = m_wndToolBarIed.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();

	VERIFY( _menu.LoadMenu(IDR_IED_MENU_RECTANGLE) );
	nBtnIdx = m_wndToolBarIed.CommandToIndex(ID_IED_TOOL_RECTANGLE);
	m_wndToolBarIed.SetButtonMenu( nBtnIdx, _menu.Detach() );
	pTBB = m_wndToolBarIed.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();

	VERIFY( _menu.LoadMenu(IDR_IED_MENU_OUTLINED_RECTANGLE) );
	nBtnIdx = m_wndToolBarIed.CommandToIndex(ID_IED_TOOL_OUTLINED_RECTANGLE);
	m_wndToolBarIed.SetButtonMenu( nBtnIdx, _menu.Detach() );
	pTBB = m_wndToolBarIed.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();

	VERIFY( _menu.LoadMenu(IDR_IED_MENU_ROUNDED_RECTANGLE) );
	nBtnIdx = m_wndToolBarIed.CommandToIndex(ID_IED_TOOL_ROUNDED_RECTANGLE);
	m_wndToolBarIed.SetButtonMenu( nBtnIdx, _menu.Detach() );
	pTBB = m_wndToolBarIed.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();

	VERIFY( _menu.LoadMenu(IDR_IED_MENU_ROUNDED_OUTLINED_RECTANGLE) );
	nBtnIdx = m_wndToolBarIed.CommandToIndex(ID_IED_TOOL_ROUNDED_OUTLINED_RECTANGLE);
	m_wndToolBarIed.SetButtonMenu( nBtnIdx, _menu.Detach() );
	pTBB = m_wndToolBarIed.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();

	VERIFY( _menu.LoadMenu(IDR_IED_MENU_ELLIPSE) );
	nBtnIdx = m_wndToolBarIed.CommandToIndex(ID_IED_TOOL_ELLIPSE);
	m_wndToolBarIed.SetButtonMenu( nBtnIdx, _menu.Detach() );
	pTBB = m_wndToolBarIed.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();

	VERIFY( _menu.LoadMenu(IDR_IED_MENU_OUTLINED_ELLIPSE) );
	nBtnIdx = m_wndToolBarIed.CommandToIndex(ID_IED_TOOL_OUTLINED_ELLIPSE);
	m_wndToolBarIed.SetButtonMenu( nBtnIdx, _menu.Detach() );
	pTBB = m_wndToolBarIed.GetButton(nBtnIdx);
	ASSERT_VALID( pTBB );
	pTBB->SetNoRotateVerticalLayout();
	pTBB->SetSeparatedDropDown();
	
	/////////////////////////////////////////////////////////////////

	if(		!m_wndResizableBar0.Create(
				_T("Resizable Bar 0"),
				this,
				ID_VIEW_RESIZABLE_BAR_0
				)
		||	!m_wndResizableBar1.Create(
				_T("Resizable Bar 1"),
				this,
				ID_VIEW_RESIZABLE_BAR_1
				)
		||	!m_wndResizableBar2.Create(
				_T("Resizable Bar 2"),
				this,
				ID_VIEW_RESIZABLE_BAR_2
				)
		)
	{
		TRACE0("Failed to create \"m_wndResizableBar\"s\n");
		return -1;		// fail to create
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	m_wndStatusBar.m_bDrawPaneSeparatorsInsteadOfBorders = true;
	if(		!m_wndStatusBar.Create(this)
		||	!m_wndStatusBar.SetIndicators(
				indicators,
				sizeof(indicators) / sizeof(UINT)
			)
		)
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarZoom.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarSeek.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarIed.EnableDocking( CBRS_ALIGN_ANY );
	m_wndResizableBar0.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar2.EnableDocking(CBRS_ALIGN_ANY);

	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
	
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( !CExtControlBar::FrameInjectAutoHideAreas(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	
	VERIFY(
		g_CmdManager->SetBasicCommands(
			pApp->m_pszProfileName,
			g_statBasicCommands
			)
		);

	if( !CExtControlBar::ProfileBarStateLoad(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName,
			&m_dataFrameWP
			)
		)
	{
		DockControlBar( &m_wndMenuBar );
		DockControlBar( &m_wndToolBar );
		DockControlBar( &m_wndToolBarIed );
		DockControlBar( &m_wndToolBarZoom );
		
		RecalcLayout();
		
		CRect wrAlreadyDockedBar;
		m_wndToolBarZoom.GetWindowRect( &wrAlreadyDockedBar );
		wrAlreadyDockedBar.OffsetRect( 1, 0 );
		DockControlBar( &m_wndToolBarSeek, AFX_IDW_DOCKBAR_TOP, &wrAlreadyDockedBar );
		
		RecalcLayout();

		static CSize
			_sizeInitH( 150, 80  ),
			_sizeInitV( 80,  150 ),
			_sizeInitF( 150, 150 );
		m_wndResizableBar0.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar0.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar0.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar1.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar1.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar1.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar2.SetInitDesiredSizeHorizontal( _sizeInitH );
		m_wndResizableBar2.SetInitDesiredSizeVertical( _sizeInitV );
		m_wndResizableBar2.SetInitDesiredSizeFloating( _sizeInitF );
		m_wndResizableBar0.DockControlBar(
			AFX_IDW_DOCKBAR_LEFT, 
			1,
			this 
			);
		m_wndResizableBar1.DockControlBar( 
			AFX_IDW_DOCKBAR_LEFT, 
			1,
			this 
			);
		m_wndResizableBar2.DockControlBar(
			AFX_IDW_DOCKBAR_LEFT, 
			1,
			this 
			);

		RecalcLayout();
	}
	g_CmdManager->SerializeState(
		pApp->m_pszProfileName,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		false
		);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( ! CFrameWnd::PreCreateWindow(cs) )
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
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	pOldWnd;
	// forward focus to the view window
	m_pWndView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
//	// TEST FOR INDETERIMINATE BUTTON STATE IN RIBBONs
//	if(		nID == ID_EDIT_PASTE
//		||	nID == ID_FILE_OPEN
//		)
//	{
//		if( nCode == CN_UPDATE_COMMAND_UI )
//		{
//			((CCmdUI*)pExtra)->Enable();
//			((CCmdUI*)pExtra)->SetCheck( 2 );
//		}
//		return TRUE;
//	}

	// let the view have first crack at the command
	if (m_pWndView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	
	return CFrameWnd::PreTranslateMessage(pMsg);
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
		g_CmdManager->SerializeState(
			pApp->m_pszProfileName,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			true
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
	
	return CFrameWnd::DestroyWindow();
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// window placement persistence
	if( m_dataFrameWP.showCmd != SW_HIDE )
	{
		SetWindowPlacement( &m_dataFrameWP );
		CFrameWnd::ActivateFrame( m_dataFrameWP.showCmd );
		m_dataFrameWP.showCmd = SW_HIDE;
		return;
	}
	
	CFrameWnd::ActivateFrame(nCmdShow);
}


LRESULT CMainFrame::OnDrawControlBarTrackedArea(WPARAM wParam, LPARAM lParam)
{
	lParam;
CExtControlBar::MsgDrawTrackedAreaData_t * pMsgDrawTrackedAreaData =
		(CExtControlBar::MsgDrawTrackedAreaData_t *)wParam;
	ASSERT( pMsgDrawTrackedAreaData != NULL );
	ASSERT_VALID( (&(pMsgDrawTrackedAreaData->m_wndBar)) );
	ASSERT( pMsgDrawTrackedAreaData->m_wndBar.GetSafeHwnd() != NULL );
	ASSERT( pMsgDrawTrackedAreaData->m_dcDraw.GetSafeHdc() != NULL );
	pMsgDrawTrackedAreaData->m_bMessageHandled = true;
	if( pMsgDrawTrackedAreaData->m_bErasingPrevious )
	{
		if( m_bmpTrackerCache.GetSafeHandle() != NULL )
		{
			CDC dcSrc;
			if( dcSrc.CreateCompatibleDC( &(pMsgDrawTrackedAreaData->m_dcDraw) ) )
			{
				CBitmap * pOldBitmap = dcSrc.SelectObject( &m_bmpTrackerCache );
				pMsgDrawTrackedAreaData->m_dcDraw.BitBlt(
					m_rcTrackerEntireArea.left,
					m_rcTrackerEntireArea.top,
					m_rcTrackerEntireArea.Width(),
					m_rcTrackerEntireArea.Height(),
					&dcSrc,
					0,
					0,
					SRCCOPY
					);
				dcSrc.SelectObject( pOldBitmap );
				dcSrc.DeleteDC();
			} // if( dcSrc.CreateCompatibleDC( &(pMsgDrawTrackedAreaData->m_dcDraw) ) )
			m_bmpTrackerCache.DeleteObject(); // no longer needed
		} // if( m_bmpTrackerCache.GetSafeHandle() != NULL )
	} // if( pMsgDrawTrackedAreaData->m_bErasingPrevious )
	else
	{
		m_rcTrackerEntireArea = pMsgDrawTrackedAreaData->m_rcDrawArea;
		if( m_bmpTrackerCache.GetSafeHandle() != NULL )
			m_bmpTrackerCache.DeleteObject();
		CDC dcDst;
		if( dcDst.CreateCompatibleDC( &(pMsgDrawTrackedAreaData->m_dcDraw) ) )
		{
			if( m_bmpTrackerCache.CreateCompatibleBitmap(
					&(pMsgDrawTrackedAreaData->m_dcDraw),
					m_rcTrackerEntireArea.Width(),
					m_rcTrackerEntireArea.Height()
					)
				)
			{
				CBitmap * pOldBitmap = dcDst.SelectObject( &m_bmpTrackerCache );
				dcDst.BitBlt(
					0,
					0,
					m_rcTrackerEntireArea.Width(),
					m_rcTrackerEntireArea.Height(),
					&(pMsgDrawTrackedAreaData->m_dcDraw),
					m_rcTrackerEntireArea.left,
					m_rcTrackerEntireArea.top,
					SRCCOPY
					);
				dcDst.SelectObject( pOldBitmap );
			} // if( m_bmpTrackerCache.CreateCompatibleBitmap( ...
			dcDst.DeleteDC();
		} // if( dcDst.CreateCompatibleDC( &(pMsgDrawTrackedAreaData->m_dcDraw) ) )
		HBRUSH hBrush = (HBRUSH)
			m_brushTracker.GetSafeHandle();
		if( hBrush == NULL )
			hBrush = (HBRUSH)
			::GetStockObject( WHITE_BRUSH );
		pMsgDrawTrackedAreaData->Draw(
			hBrush,
			PATCOPY
			);
	} // else from if( pMsgDrawTrackedAreaData->m_bErasingPrevious )
	return 0L;
}
