#include "stdafx.h"
#include "App.h"
#include "Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CApp theApp;

BEGIN_MESSAGE_MAP(CApp, CWinApp)
	//{{AFX_MSG_MAP(CApp)
	//}}AFX_MSG
	ON_COMMAND( ID_HELP, CWinApp::OnHelp )
END_MESSAGE_MAP()

CApp::CApp()
{
}

CApp::~CApp()
{
}

BOOL CApp::InitInstance()
{
	VERIFY( ::AfxOleInit() );
	VERIFY( ::AfxInitRichEdit() );
	SetRegistryKey( _T("Foss") );
	CExtPopupMenuWnd::g_bMenuExpanding = CExtPopupMenuWnd::g_bMenuHighlightRarely = false;

#if (!defined __EXT_MFC_NO_TAB_CTRL)
	VERIFY( g_PaintManager.InstallPaintManager( RUNTIME_CLASS( CExtPaintManagerStudio2010 ) ) );
#else
	VERIFY( g_PaintManager.InstallPaintManager( RUNTIME_CLASS( CExtPaintManagerNativeXP ) ) );
#endif

CExtRichDocObjectBase & _GC = CExtRichContentLayout::stat_GetObjectCountainerGlobal();
CExtRichDocObjectImage * pObjPic = NULL;

	pObjPic = new CExtRichDocObjectImage( _T("smile.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(IDB_SMILE) ) );
	VERIFY( pObjPic->ImageGet().Make32() );
	VERIFY( _GC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("check.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(IDB_CHECK) ) );
	VERIFY( pObjPic->ImageGet().Make32() );
	VERIFY( _GC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("GreenSquare.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(IDB_GREEN_SQUARE) ) );
	VERIFY( pObjPic->ImageGet().Make32() );
	VERIFY( _GC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("gradient.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(IDB_GRADIENT) ) );
	VERIFY( pObjPic->ImageGet().Make32() );
	VERIFY( _GC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("SliceTest.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(IDB_SLICE_TEST) ) );
	VERIFY( pObjPic->ImageGet().Make32() );
	VERIFY( _GC.ObjectAdd( pObjPic ) );

	pObjPic = new CExtRichDocObjectImage( _T("GlowTest.bmp") );
	VERIFY( pObjPic->ImageGet().LoadBMP_Resource( MAKEINTRESOURCE(IDB_TEST_GLOW) ) );
	VERIFY( pObjPic->ImageGet().Make32() );
	VERIFY( _GC.ObjectAdd( pObjPic ) );

CMyResizablePropertySheet _PropSheet( _T("Rich Content Features") );
	_PropSheet.m_psh.dwFlags |= PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW | 0x02000000 /*PSH_NOCONTEXTHELP*/;
	_PropSheet.m_psh.dwFlags &= ~(PSH_HASHELP|PSH_WIZARDCONTEXTHELP);

CPageWELCOME _PageWELCOME;
	_PropSheet.AddPage( &_PageWELCOME );
CPageDT _PageDT;
	_PropSheet.AddPage( &_PageDT );
CPageHTML _PageHTML;
	_PropSheet.AddPage( &_PageHTML );
CPageGC _PageGC;
	_PropSheet.AddPage( &_PageGC );
CPageAC _PageAC;
	_PropSheet.AddPage( &_PageAC );

	_PropSheet.EnableSaveRestore( _T( "Resizable Dialogs" ), _T( "Main Property Sheet" ) );
	m_pMainWnd = &_PropSheet;
	_PropSheet.DoModal();
	m_pMainWnd = NULL;
	return FALSE;
}
