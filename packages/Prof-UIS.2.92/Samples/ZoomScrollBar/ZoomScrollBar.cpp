// ZoomScrollBar.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ZoomScrollBar.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApp

BEGIN_MESSAGE_MAP(CApp, CWinApp)
	//{{AFX_MSG_MAP(CApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApp construction

CApp::CApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CApp object

CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CApp initialization

BOOL CApp::InitInstance()
{
	srand( (unsigned)time( NULL ) );
	InitCommonControls();
	VERIFY( ::AfxOleInit() );
	AfxEnableControlContainer();

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey( _T("Foss") );

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	if( ! pFrame->LoadFrame(
			IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
			NULL,
			NULL
			)
		)
	{
		m_pMainWnd = NULL;
		ASSERT( FALSE );
		return FALSE;
	}

	// window placement persistence
	pFrame->ActivateFrame( SW_SHOWMAXIMIZED );

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CApp message handlers

// App command to run the dialog
void CApp::OnAppAbout()
{
#ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
	VERIFY( ProfUISAbout() );
#endif // #ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
}

/////////////////////////////////////////////////////////////////////////////
// CApp message handlers

