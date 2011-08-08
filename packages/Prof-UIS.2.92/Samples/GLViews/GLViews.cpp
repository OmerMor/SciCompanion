// GLViews.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GLViews.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLViewsApp

BEGIN_MESSAGE_MAP(CGLViewsApp, CWinApp)
	//{{AFX_MSG_MAP(CGLViewsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLViewsApp construction

CGLViewsApp::CGLViewsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGLViewsApp object

CGLViewsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGLViewsApp initialization

BOOL CGLViewsApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();
	SetRegistryKey( _T("Foss") );
CMainFrame * pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	if( ! pFrame->LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL ) )
	{
		m_pMainWnd = NULL;
		ASSERT( FALSE );
		return FALSE;
	}
	// window placement persistence
	pFrame->ActivateFrame( SW_SHOW );
// 	::SetActiveWindow( NULL );
// 	pFrame->SetActiveWindow();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGLViewsApp message handlers

// App command to run the dialog
void CGLViewsApp::OnAppAbout()
{
#ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
	VERIFY( ProfUISAbout() );
#endif // #ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
}

/////////////////////////////////////////////////////////////////////////////
// CGLViewsApp message handlers
