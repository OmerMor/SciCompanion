// RibbonBarMDI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RibbonBarMDI.h"

#include "MainFrm.h"

#if (!defined __RibbonIcons_H)
	#include "res/RibbonIcons.h"
#endif // (!defined __RibbonIcons_H)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRibbonBarMDIApp

BEGIN_MESSAGE_MAP(CRibbonBarMDIApp, CWinApp)
	//{{AFX_MSG_MAP(CRibbonBarMDIApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FB_NEW, OnFileNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRibbonBarMDIApp construction

CRibbonBarMDIApp::CRibbonBarMDIApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRibbonBarMDIApp object

CRibbonBarMDIApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRibbonBarMDIApp initialization

BOOL CRibbonBarMDIApp::InitInstance()
{
	srand( (unsigned)time( NULL ) );
	InitCommonControls();
	AfxEnableControlContainer();
	SetRegistryKey( _T("Foss") );
	ASSERT( m_pszRegistryKey != NULL );
	if( m_pszProfileName != NULL )
		free( (void*)m_pszProfileName );
	m_pszProfileName =
		_tcsdup( _T("RibbonBarMDI") );
	ASSERT( m_pszProfileName != NULL );
	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)

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

CMainFrame * pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	if( ! pFrame->LoadFrame(
			IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE,
			NULL,
			NULL
			)
		)
	{
		m_pMainWnd = NULL;
		return FALSE;
	}

	m_pMainWnd->DragAcceptFiles();
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	
	pFrame->ActivateFrame( SW_SHOW );
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRibbonBarMDIApp message handlers

// App command to run the dialog
void CRibbonBarMDIApp::OnAppAbout()
{
#ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
	VERIFY( ProfUISAbout() );
#endif // #ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
}

void CRibbonBarMDIApp::OnFileNew() 
{
CMainFrame * pFrame =
		STATIC_DOWNCAST(
			CMainFrame,
			m_pMainWnd
			);
	// create a new MDI child window
//CMDIChildWnd * pChild =
		pFrame->CreateNewChild(
			RUNTIME_CLASS(CChildFrame),
			IDR_RIBBON_BAR_MDI_TYPE,
			NULL, // m_hMDIMenu,
			NULL  // m_hMDIAccel
			);
//	pChild->ActivateFrame( SW_SHOWMAXIMIZED );
}


