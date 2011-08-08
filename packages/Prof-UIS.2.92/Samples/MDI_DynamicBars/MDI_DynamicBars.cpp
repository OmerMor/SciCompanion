// MDI_DynamicBars.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MDI_DynamicBars.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MDI_DynamicBarsDoc.h"
#include "MDI_DynamicBarsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsApp

BEGIN_MESSAGE_MAP(CMDI_DynamicBarsApp, CWinApp)
	//{{AFX_MSG_MAP(CMDI_DynamicBarsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsApp construction

CMDI_DynamicBarsApp::CMDI_DynamicBarsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMDI_DynamicBarsApp object

CMDI_DynamicBarsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// Prof-UIS advanced options

void CMDI_DynamicBarsApp::SetupUiAdvancedOptions()
{
	ASSERT( m_pszRegistryKey != NULL );
	ASSERT( m_pszRegistryKey[0] != _T('\0') );
	ASSERT( m_pszProfileName != NULL );
	ASSERT( m_pszProfileName[0] != _T('\0') );

	//
	// Prof-UIS command manager profile
	//
	VERIFY(
		g_CmdManager->ProfileSetup( m_pszProfileName )
		);

	//
	// Popup menu option: Display menu shadows
    //
	CExtPopupMenuWnd::g_bMenuWithShadows = true;

	//
	// Popup menu option: Display menu cool tips
    //
	CExtPopupMenuWnd::g_bMenuShowCoolTips = true;

	//
	// Popup menu option: Initially hide rarely used items (RUI)
    //
	CExtPopupMenuWnd::g_bMenuExpanding = true;

	//
	// Popup menu option: Display RUI in different style
    //
	CExtPopupMenuWnd::g_bMenuHighlightRarely = true;

	//
	// Popup menu option: Animate when expanding RUI (like Office XP)
    //
	CExtPopupMenuWnd::g_bMenuExpandAnimation = true;

	//
	// Popup menu option: Align to desktop work area (false - to screen area)
    //
	CExtPopupMenuWnd::g_bUseDesktopWorkArea = true;

	//
	// Popup menu option: Popup menu animation effect (when displaying)
    //
	CExtPopupMenuWnd::g_DefAnimationType =
		CExtPopupMenuWnd::__AT_FADE;

}

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsApp initialization

BOOL CMDI_DynamicBarsApp::InitInstance()
{
	srand( (unsigned)time( NULL ) );
	InitCommonControls();
	AfxEnableControlContainer();

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey( _T("Foss") );
	ASSERT( m_pszRegistryKey != NULL );

    // Change the application profile name (usually product name).
	// NOTE: The CWinApp class destructor will free the memory automatically.
	if( m_pszProfileName != NULL )
		free( (void*)m_pszProfileName );
	m_pszProfileName =
		_tcsdup( _T("MDI_DynamicBars") );
	ASSERT( m_pszProfileName != NULL );

	//
	// Prof-UIS advanced options
    //
	SetupUiAdvancedOptions();

	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

#if (!defined __EXT_MFC_NO_SHELL_DIALOG_FILE)
	// make CDocManager using CExtShellDialogFile instead of CFileDialog
	ASSERT( m_pDocManager == NULL );
	m_pDocManager = new CExtDMFP < CDocManager >;
#endif // (!defined __EXT_MFC_NO_SHELL_DIALOG_FILE)

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_MDIDOCTYPE,
		RUNTIME_CLASS(CMDI_DynamicBarsDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMDI_DynamicBarsView)
		);
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ActivateFrame( SW_SHOWMAXIMIZED );

	return TRUE;
}

// App command to run the dialog
void CMDI_DynamicBarsApp::OnAppAbout()
{
#ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
	VERIFY( ProfUISAbout() );
#endif // #ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
}

/////////////////////////////////////////////////////////////////////////////
// CMDI_DynamicBarsApp message handlers

