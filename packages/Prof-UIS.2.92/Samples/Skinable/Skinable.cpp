// Skinable.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Skinable.h"
#include "MainFrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinableApp

BEGIN_MESSAGE_MAP(CSkinableApp, CWinApp)
	//{{AFX_MSG_MAP(CSkinableApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinableApp construction

CSkinableApp::CSkinableApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSkinableApp object

CSkinableApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSkinableApp initialization

BOOL CSkinableApp::InitInstance()
{
	InitCommonControls();
	AfxEnableControlContainer();
	if( ! ::AfxOleInit() )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	SetRegistryKey( _T("Foss") );
	ASSERT( m_pszRegistryKey != NULL );
	if( m_pszProfileName != NULL )
		free( (void*)m_pszProfileName );
	m_pszProfileName =
		_tcsdup( _T("Skinable") );
	ASSERT( m_pszProfileName != NULL );
CMainFrame * pMainFrame = new CMainFrame;
	m_pMainWnd = pMainFrame;
	if( ! pMainFrame->LoadFrame( IDR_MAINFRAME ) )
	{
		m_pMainWnd = NULL;
		return FALSE;
	}
	pMainFrame->ActivateFrame( SW_SHOWNORMAL );
	return TRUE;
}

// App command to run the dialog
void CSkinableApp::OnAppAbout()
{
	VERIFY( ProfUISAbout() );
}

/////////////////////////////////////////////////////////////////////////////
// CSkinableApp message handlers

