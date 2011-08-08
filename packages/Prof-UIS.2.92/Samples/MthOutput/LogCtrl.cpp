// LogCtrl.cpp : implementation of the CLogCtrl class
//

#include "stdafx.h"
#include "MthOutput.h"
#include "LogCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogCtrl

CLogCtrl::CLogCtrl()
	: m_dwHelperThreadID(0)
	, m_nLineCountMax( 400 )
{
}

CLogCtrl::~CLogCtrl()
{
}


BEGIN_MESSAGE_MAP(CLogCtrl,CRichEditCtrl)
	//{{AFX_MSG_MAP(CLogCtrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USR_WRITE_LOG_TEXT, On_WM_USR_WRITE_LOG_TEXT )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLogCtrl message handlers

BOOL CLogCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	if( ! CRichEditCtrl :: PreCreateWindow(cs) )
		return FALSE;
	cs.style &= ~WS_BORDER;
	return TRUE;
}

int CLogCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if( CRichEditCtrl :: OnCreate(lpCreateStruct) == -1 )
		return -1;
	_InitRichEditProps();
	return 0;
}

void CLogCtrl::OnTimer(__EXT_MFC_UINT_PTR nIDEvent) 
{
	if( nIDEvent == ID_TIMER_DELAYED_UPDATE )
	{
		if( _CanUpdate() )
		{
			KillTimer( ID_TIMER_DELAYED_UPDATE );
			Invalidate();
			UpdateWindow();
		}
		return;
	} // if( nIDEvent == ID_TIMER_DELAYED_UPDATE )
	
	CRichEditCtrl::OnTimer(nIDEvent);
}

void CLogCtrl::OnEditCopy() 
{
CHARRANGE _cr;
	GetSel( _cr );
	if( _cr.cpMin != _cr.cpMax )
		CRichEditCtrl::Copy();
}

void CLogCtrl::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
CHARRANGE _cr;
	GetSel( _cr );
	pCmdUI->Enable(
		( _cr.cpMin != _cr.cpMax ) ? TRUE : FALSE
		);
}

LRESULT CLogCtrl::On_WM_USR_WRITE_LOG_TEXT( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
	wParam;
_MthMessageDataFor_WM_USR_WRITE_LOG_TEXT & _data =
	_MthMessageDataFor_WM_USR_WRITE_LOG_TEXT::FromLPARAM( lParam );
	WriteText(
		_data.m_bBold,
		_data.m_clrText,
		_data.m_sText
		);
	return 0L;
}

void CLogCtrl::_InitRichEditProps()
{
PARAFORMAT _pf;
	_pf.cbSize = sizeof(PARAFORMAT);
	_pf.dwMask = PFM_TABSTOPS ;
	_pf.cTabCount = MAX_TAB_STOPS;
	for( int nTabIdx = 0 ; nTabIdx < _pf.cTabCount; nTabIdx++ )
		_pf.rgxTabs[nTabIdx] = (nTabIdx + 1) * 1440/5;
	CRichEditCtrl::SetParaFormat( _pf );

CHARFORMAT _cf;
	memset( &_cf, 0, sizeof(CHARFORMAT) );
	_cf.cbSize = sizeof(_cf);
	_cf.dwEffects = 0; // CFE_PROTECTED; 
	_cf.dwMask =
		CFM_FACE|CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT
		|CFM_SIZE|CFM_CHARSET|CFM_PROTECTED|CFM_COLOR;
	_cf.yHeight = 160;
	_cf.bCharSet = DEFAULT_CHARSET;
#if _MFC_VER < 0x700
	strcpy( _cf.szFaceName, "Courier New" ); // <-- TO FIX <--
#else
	__EXT_MFC_STRCPY( _cf.szFaceName, LF_FACESIZE, _T("Courier New") );
#endif
	_cf.crTextColor = RGB(0,0,0);
	CRichEditCtrl::SetDefaultCharFormat( _cf );
	CRichEditCtrl::SetWordCharFormat( _cf );
	
	CRichEditCtrl::SetBackgroundColor( FALSE, RGB(255,255,255) );
	
	CRichEditCtrl::SetSel( -1, -1 );
}

bool CLogCtrl::_CanUpdate()
{
	if( CExtControlBar::FindHwndNotSafeForMutualPaint(this) != NULL )
		return false;
	if( CExtPopupMenuWnd::g_bMenuWithShadows )
	{
		if( CExtPopupMenuWnd::IsMenuTracking() )
		{
			CExtPopupMenuWnd * pPopup = CExtPopupMenuSite::g_DefPopupMenuSite.GetInstance();
			ASSERT_VALID( pPopup );
			if( pPopup->_IsPopupWithShadows() )
				return false;
		}
		else if( CExtToolControlBar::g_bMenuTracking )
			return false;
	} // if( CExtPopupMenuWnd::g_bMenuWithShadows )
CExtPopupMenuTipWnd & _ATTW = CExtPopupMenuSite::g_DefPopupMenuSite.GetTip();
	if(		_ATTW.GetSafeHwnd() != NULL
		&&	(_ATTW.GetStyle()&WS_VISIBLE) != 0
		&&	_ATTW.m_ctrlShadow.GetSafeHwnd() == NULL
		&&	_ATTW.GetShadowSize() > 0
		)
		return false;
	return true;
}

void CLogCtrl::WriteText(
	bool bBold,
	COLORREF clrText,
	LPCTSTR sText
	)
{
	ASSERT( this != NULL );
	if( m_dwHelperThreadID != ::GetCurrentThreadId() )
	{
		_MthMessageDataFor_WM_USR_WRITE_LOG_TEXT _data(
			bBold,
			clrText,
			sText
			);
		SendMessage(
			WM_USR_WRITE_LOG_TEXT,
			WPARAM(0),
			_data
			);
		return;
	}
	
	ASSERT_VALID( this );
	if(		sText == NULL
		||	_tcslen( sText ) == 0
		)
		return;

bool bCanUpdate = _CanUpdate();
	SetRedraw( FALSE );

int nFirstLine1 = CRichEditCtrl::GetFirstVisibleLine();
CHARRANGE crOld1, crOld2;
	CRichEditCtrl::GetSel( crOld1 );
	CRichEditCtrl::SetSel( -1, -1 );
	CRichEditCtrl::GetSel( crOld2 );
bool bResetSel = true;
	if(		crOld1.cpMin == crOld2.cpMin
		&&	crOld1.cpMax == crOld2.cpMax
		)
		bResetSel = false;
	CRichEditCtrl::ReplaceSel( sText );

CHARFORMAT _cf;
	_cf.cbSize = sizeof(CHARFORMAT);
	_cf.dwMask = CFM_COLOR|CFM_BOLD;
	_cf.dwEffects = bBold ? CFE_BOLD : 0;
	_cf.crTextColor = clrText;
	CRichEditCtrl::SetSel( crOld2.cpMax, -1 );
	CRichEditCtrl::SetSelectionCharFormat( _cf );

	if( bResetSel )
	{
		CRichEditCtrl::SetSel( crOld1 );
		int nFirstLine2 = CRichEditCtrl::GetFirstVisibleLine();
		if( nFirstLine1 != nFirstLine2 )
			LineScroll( nFirstLine1-nFirstLine2, 0) ;
	} // if( bResetSel )

	if( m_nLineCountMax > 0 )
	{
		int nLineCount = CRichEditCtrl::GetLineCount();
		if( nLineCount > m_nLineCountMax )
		{
			int nEndRemovePos =
				CRichEditCtrl::LineIndex(
					nLineCount > m_nLineCountMax 
					);
			CHARRANGE crRestore;
			GetSel( crRestore );
			crRestore.cpMin -= nEndRemovePos;
			crRestore.cpMax -= nEndRemovePos;
			SetSel( 0, nEndRemovePos );
			CRichEditCtrl::ReplaceSel( _T("") );
			SetSel( crRestore );
		}
	} // if( m_nLineCountMax > 0 )

	if( !bResetSel )
		CRichEditCtrl::SetSel( -1, -1 );

	SetRedraw( TRUE );
	if( bCanUpdate )
		Invalidate();
	else
		SetTimer(
			ID_TIMER_DELAYED_UPDATE,
			ELAPSE_TIMER_DELAYED_UPDATE,
			NULL
			);
}

/////////////////////////////////////////////////////////////////////////////
// CDemoEventSourceWnd

static struct
{
	bool m_bBold;
	COLORREF m_clrText;
	LPCTSTR m_sLine;
} arrDemoOutputLines[] =
{
	{ false, RGB(128,128,128), _T("Deleting intermediate files and output files for project \'ProfUISDLL - Win32 MBCS Debug\'.\n") },
	{ false, RGB(128,128,128), _T("Deleting intermediate files and output files for project \'ProfUISDLL - Win32 MBCS Release\'.\n") },
	{ true,  RGB(0,0,0), _T("--------------------Configuration: ProfUISDLL - Win32 MBCS Debug--------------------\n") },
	{ false, RGB(128,0,0), _T("Compiling resources...\n") },
	{ false, RGB(128,0,0), _T("Compiling...\n") },
	{ false, RGB(0,0,0), _T("StdAfx.cpp\n") },
	{ false, RGB(0,0,128), _T("   Automatically linking with WinMM library\n") },
	{ false, RGB(0,0,128), _T("      (Windows Multimedia System)\n") },
	{ false, RGB(128,0,0), _T("Compiling...\n") },
	{ false, RGB(0,0,0), _T("exdlgbase.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtBtnOnFlat.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtButton.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtCmdManager.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtColorCtrl.cpp\n") },
	{ false, RGB(0,0,128), _T("   Automatically linking with version.lib\n") },
	{ false, RGB(0,0,128), _T("      (Version info support)\n") },
	{ false, RGB(0,0,0), _T("ExtColorDlg.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtComboBox.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtContentExpandWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("extcontrolbar.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtControlBarTabbedFeatures.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtDockBar.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtEdit.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtHook.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtMenuControlBar.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtMiniDockFrameWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtMouseCaptureSink.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtPageContainerWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtPaintManager.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtPopupMenuWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtResizableDialog.cpp\n") },
	{ false, RGB(128,0,0), _T("Generating Code...\n") },
	{ false, RGB(128,0,0), _T("Compiling...\n") },
	{ false, RGB(0,0,0), _T("ExtShortcutListWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtTabWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtToolBoxWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtToolControlBar.cpp\n") },
	{ false, RGB(0,0,0), _T("ProfUISDLL.cpp\n") },
	{ false, RGB(128,0,0), _T("Generating Code...\n") },
	{ false, RGB(128,0,0), _T("Compiling...\n") },
	{ false, RGB(0,0,0), _T("ExtStatusControlBar.cpp\n") },
	{ true,  RGB(128,128,0), _T("Linking...\n") },
	{ false, RGB(128,0,128), _T("   Creating library ../Bin_600/ProfUISmd.lib and object ../Bin_600/ProfUISmd.exp\n") },
	{ false, RGB(0,0,0), _T("\n") },
	{ true,  RGB(0,128,0), _T("ProfUIS.dll - 0 error(s), 0 warning(s)\n") },
	{ false, RGB(0,0,0), _T("\n") },
	{ true,  RGB(0,0,0), _T("--------------------Configuration: ProfUISDLL - Win32 MBCS Release--------------------\n") },
	{ false, RGB(128,0,0), _T("Compiling resources...\n") },
	{ false, RGB(128,0,0), _T("Compiling...\n") },
	{ false, RGB(0,0,0), _T("StdAfx.cpp\n") },
	{ false, RGB(0,0,128), _T("   Automatically linking with WinMM library\n") },
	{ false, RGB(0,0,128), _T("      (Windows Multimedia System)\n") },
	{ false, RGB(128,0,0), _T("Compiling...\n") },
	{ false, RGB(0,0,0), _T("exdlgbase.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtBtnOnFlat.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtButton.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtCmdManager.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtColorCtrl.cpp\n") },
	{ false, RGB(0,0,128), _T("   Automatically linking with version.lib\n") },
	{ false, RGB(0,0,128), _T("      (Version info support)\n") },
	{ false, RGB(0,0,0), _T("ExtColorDlg.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtComboBox.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtContentExpandWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("extcontrolbar.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtControlBarTabbedFeatures.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtDockBar.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtEdit.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtHook.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtMenuControlBar.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtMiniDockFrameWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtMouseCaptureSink.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtPageContainerWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtPaintManager.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtPopupMenuWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtResizableDialog.cpp\n") },
	{ false, RGB(128,0,0), _T("Generating Code...\n") },
	{ false, RGB(128,0,0), _T("Compiling...\n") },
	{ false, RGB(0,0,0), _T("ExtShortcutListWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtTabWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtToolBoxWnd.cpp\n") },
	{ false, RGB(0,0,0), _T("ExtToolControlBar.cpp\n") },
	{ false, RGB(0,0,0), _T("ProfUISDLL.cpp\n") },
	{ false, RGB(128,0,0), _T("Generating Code...\n") },
	{ false, RGB(128,0,0), _T("Compiling...\n") },
	{ false, RGB(0,0,0), _T("ExtStatusControlBar.cpp\n") },
	{ true,  RGB(128,128,0), _T("Linking...\n") },
	{ false, RGB(128,0,128), _T("   Creating library ../Bin_600/ProfUISm.lib and object ../Bin_600/ProfUISm.exp\n") },
	{ false, RGB(0,0,0), _T("\n") },
	{ true,  RGB(0,128,0), _T("ProfUISm.dll - 0 error(s), 0 warning(s)\n") },
	{ false, RGB(0,0,0), _T("\n") },
};

CDemoEventSourceWnd::CDemoEventSourceWnd()
	: m_nDemoOutputLineIndex( 0 )
	, m_pWndLogCtrl( NULL )
{
}

CDemoEventSourceWnd::~CDemoEventSourceWnd()
{
}


BEGIN_MESSAGE_MAP(CDemoEventSourceWnd,CWnd)
	//{{AFX_MSG_MAP(CDemoEventSourceWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDemoEventSourceWnd message handlers

int CDemoEventSourceWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetTimer(
		ID_TIMER_EVENT_SOURCE,
		ELAPSE_TIMER_EVENT_SOURCE,
		NULL
		);
	
	return 0;
}

void CDemoEventSourceWnd::OnTimer(__EXT_MFC_UINT_PTR nIDEvent) 
{
	if( nIDEvent == ID_TIMER_EVENT_SOURCE )
	{
		if( m_nDemoOutputLineIndex >= sizeof(arrDemoOutputLines)/sizeof(arrDemoOutputLines[0]) )
			m_nDemoOutputLineIndex = 0;
		ASSERT( m_pWndLogCtrl != NULL );
		if( m_pWndLogCtrl->GetSafeHwnd() == NULL )
		{
			KillTimer( ID_TIMER_EVENT_SOURCE );
			return;
		}
		m_pWndLogCtrl->WriteText(
			arrDemoOutputLines[m_nDemoOutputLineIndex].m_bBold,
			arrDemoOutputLines[m_nDemoOutputLineIndex].m_clrText,
			arrDemoOutputLines[m_nDemoOutputLineIndex].m_sLine
			);
		m_nDemoOutputLineIndex++;
		return;
	} // if( nIDEvent == ID_TIMER_EVENT_SOURCE )
	
	if( nIDEvent == ID_TIMER_DELAYED_DESTROY )
	{
		KillTimer( ID_TIMER_DELAYED_DESTROY );
		DestroyWindow();
		return;
	} // if( nIDEvent == ID_TIMER_DELAYED_DESTROY )

	CWnd::OnTimer(nIDEvent);
}

