// ProfUIS_Controls.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ProfUIS_Controls.h"
#include "MainDlg.h"

#include <ExtCmdManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainApp

BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
	//{{AFX_MSG_MAP(CMainApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainApp construction

CMainApp::CMainApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMainApp object

CMainApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainApp initialization

BOOL CMainApp::InitInstance()
{
	AfxEnableControlContainer();
	VERIFY( AfxOleInit() );
	VERIFY( AfxInitRichEdit() );
	
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

	SetRegistryKey( _T("Foss") );
	ASSERT( m_pszRegistryKey != NULL );
	
    // Change the application profile name (usually product name).
	// NOTE: The CWinApp class destructor will free the memory automatically.
	if( m_pszProfileName != NULL )
		free( (void*)m_pszProfileName );
	m_pszProfileName =
		_tcsdup( _T("ProfUIS_Controls") );
	ASSERT( m_pszProfileName != NULL );
	VERIFY(
		g_CmdManager->ProfileSetup(
			m_pszProfileName
			)
		);
	
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	if( ! g_PaintManager.PaintManagerStateLoad(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		)
		g_PaintManager.InstallPaintManager(
			RUNTIME_CLASS(CExtPaintManagerOffice2010_R1)
			);

// 					CExtPaintManagerSkin * pPM = new CExtPaintManagerSkin;
// 					bool bLoaded = true;
// 					if( ! pPM->m_Skin.Load( _T("..\\..\\Skins\\XML\\Aqua.XML") ) )
// 	//				if( ! pPM->m_Skin.Load( _T("..\\..\\Skins\\XML\\BlackDiamond.xml") ) )
// 					{
// 						bLoaded = false;
// 						::AfxMessageBox( _T("Failed to load initial skin.") );
// 						delete pPM;
// 					}
// 					if( bLoaded )
// 						g_PaintManager.InstallPaintManager( pPM );

CMainDlg dlgMain;
	m_pMainWnd = &dlgMain;
__EXT_MFC_INT_PTR nResponse = dlgMain.DoModal();
	if( nResponse == IDOK )
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	VERIFY(
		g_PaintManager.PaintManagerStateSave(
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			pApp->m_pszProfileName
			)
		);
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
