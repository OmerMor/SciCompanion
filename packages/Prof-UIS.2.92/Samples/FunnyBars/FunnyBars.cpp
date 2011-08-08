// FunnyBars.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FunnyBars.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFunnyBarsApp

BEGIN_MESSAGE_MAP(CFunnyBarsApp, CWinApp)
	//{{AFX_MSG_MAP(CFunnyBarsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFunnyBarsApp construction

CFunnyBarsApp::CFunnyBarsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFunnyBarsApp object

CFunnyBarsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFunnyBarsApp initialization

BOOL CFunnyBarsApp::InitInstance()
{
	InitCommonControls();

	AfxEnableControlContainer();
#if _MFC_VER < 0x700
	#ifdef _AFXDLL
		Enable3dControls();			// Call this when using MFC in a shared DLL
	#else
		Enable3dControlsStatic();	// Call this when linking to MFC statically
	#endif
#endif
	SetRegistryKey( _T("Foss") );
CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	pFrame->ActivateFrame( SW_SHOW );
// 	::SetActiveWindow( NULL );
// 	pFrame->SetActiveWindow();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFunnyBarsApp message handlers

// App command to run the dialog
void CFunnyBarsApp::OnAppAbout()
{
#ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
	VERIFY( ProfUISAbout() );
#endif // #ifndef __EXT_MFC_NO_PROF_UIS_ABOUT_DIALOG	
}

/////////////////////////////////////////////////////////////////////////////
// CFunnyBarsApp message handlers

