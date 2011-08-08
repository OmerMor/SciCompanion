// AviFrames.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AviFrames.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAviFramesApp

BEGIN_MESSAGE_MAP(CAviFramesApp, CWinApp)
	//{{AFX_MSG_MAP(CAviFramesApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAviFramesApp construction

CAviFramesApp::CAviFramesApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAviFramesApp object

CAviFramesApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAviFramesApp initialization

BOOL CAviFramesApp::InitInstance()
{
	InitCommonControls();

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	if( ! ::AfxOleInit() )
	{
		ASSERT( FALSE );
		return FALSE;
	}
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
	AfxEnableControlContainer();

	SetRegistryKey( _T("Foss") );
CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
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
	pFrame->ActivateFrame( SW_SHOW );
	pFrame->SendMessage(WM_COMMAND, ID_FILE_OPEN);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAviFramesApp message handlers

void CAviFramesApp::OnAppAbout()
{
#ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
	VERIFY( ProfUISAbout() );
#endif // #ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
}

