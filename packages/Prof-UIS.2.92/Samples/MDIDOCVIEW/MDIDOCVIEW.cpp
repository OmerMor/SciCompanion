// MDIDOCVIEW.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MDIDOCVIEW.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MDIDOCVIEWDoc.h"
#include "MDIDOCVIEWView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWApp

BEGIN_MESSAGE_MAP(CMDIDOCVIEWApp, CWinApp)
	//{{AFX_MSG_MAP(CMDIDOCVIEWApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWApp construction

CMDIDOCVIEWApp::CMDIDOCVIEWApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMDIDOCVIEWApp object

CMDIDOCVIEWApp theApp;

/////////////////////////////////////////////////////////////////////////////
// Prof-UIS advanced options

void CMDIDOCVIEWApp::SetupUiAdvancedOptions()
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
// CMDIDOCVIEWApp initialization

BOOL CMDIDOCVIEWApp::InitInstance()
{
	InitCommonControls();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MFC_VER < 0x700
	#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
	#else
		Enable3dControlsStatic();	// Call this when linking to MFC statically
	#endif
#endif

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
		_tcsdup( _T("MDIDOCVIEW") );
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
		RUNTIME_CLASS(CMDIDOCVIEWDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMDIDOCVIEWView));
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
	pMainFrame->ActivateFrame( m_nCmdShow );
// 	::SetActiveWindow( NULL );
// 	pMainFrame->SetActiveWindow();

	return TRUE;
}

// App command to run the dialog
void CMDIDOCVIEWApp::OnAppAbout()
{
#ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
	VERIFY( ProfUISAbout() );
#endif // #ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
}

/////////////////////////////////////////////////////////////////////////////
// CMDIDOCVIEWApp message handlers

