// ResizableChildSheet.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ResizableChildSheet.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoApp

BEGIN_MESSAGE_MAP(CDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CDemoApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoApp construction

CDemoApp::CDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDemoApp object

CDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// Prof-UIS advanced options

void CDemoApp::SetupUiAdvancedOptions()
{
	VERIFY(
		g_CmdManager->ProfileSetup(
			__PROF_UIS_PROJECT_CMD_PROFILE_NAME
			)
		);
	g_PaintManager.InstallPaintManager(
		new CExtPaintManager
		);
	CExtPopupMenuWnd::g_bMenuExpanding = false;
	CExtPopupMenuWnd::g_bMenuHighlightRarely = false;
}

/////////////////////////////////////////////////////////////////////////////
// CDemoApp initialization

BOOL CDemoApp::InitInstance()
{
	InitCommonControls();

	//
	// Prof-UIS advanced options
    //
	SetupUiAdvancedOptions();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey( _T("Prof-UIS Application Wizard Generated Applications") );
	ASSERT( m_pszRegistryKey != NULL );

    // Change the application profile name (usually product name).
	// NOTE: The CWinApp class destructor will free the memory automatically.
	if( m_pszProfileName != NULL )
		free( (void*)m_pszProfileName );
	m_pszProfileName =
		_tcsdup( _T("ResizableChildSheet") );
	ASSERT( m_pszProfileName != NULL );


	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMDIFrameWnd* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create main MDI frame window
	if( !pFrame->LoadFrame(IDR_MAINFRAME) )
		return FALSE;

	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need. 

	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_CHILDSTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_CHILDSTYPE));

	// The main window has been initialized, so show and update it.
	pFrame->ActivateFrame( m_nCmdShow );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDemoApp message handlers

int CDemoApp::ExitInstance() 
{
	//TODO: handle additional resources you may have added
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}

void CDemoApp::OnFileNew() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	// create a new MDI child window
	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_CHILDSTYPE, m_hMDIMenu, m_hMDIAccel);
}

// App command to run the dialog
void CDemoApp::OnAppAbout()
{
#ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
	VERIFY( ProfUISAbout() );
#endif // #ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
}

/////////////////////////////////////////////////////////////////////////////
// CDemoApp message handlers

