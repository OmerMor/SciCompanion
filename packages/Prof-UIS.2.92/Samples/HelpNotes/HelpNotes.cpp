// HelpNotes.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HelpNotes.h"

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
	: m_hWndLastPopup( NULL )
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
	VERIFY( ::AfxOleInit() );
	::InitCommonControls();
	::AfxEnableControlContainer();
	VERIFY( ::AfxInitRichEdit() );
	SetRegistryKey( _T("Foss") );
CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	VERIFY(
		pFrame->LoadFrame(
			IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE,
			NULL,
			NULL
			)
		);
	pFrame->ActivateFrame( SW_SHOW );
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

BOOL CApp::PreTranslateMessage( MSG * pMsg ) 
{
	if(		pMsg->hwnd != NULL
		&&	pMsg->hwnd == m_hWndLastPopup
		)
		return FALSE;
	return CWinApp::PreTranslateMessage( pMsg );
}
