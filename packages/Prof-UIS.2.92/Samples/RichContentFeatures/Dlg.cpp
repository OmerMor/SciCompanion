#include "stdafx.h"
#include "App.h"
#include "Dlg.h"
#include <../Src/ExtMemoryDC.h>

#if (! defined __VSSYM32_H__)
	#include <vssym32/vssym32.h>
#endif // (! defined __VSSYM32_H__)


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ( ! defined DT_HIDEPREFIX )
	#define DT_HIDEPREFIX               0x00100000
#endif

#if ( ! defined DT_PREFIXONLY )
	#define DT_PREFIXONLY               0x00200000
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageWELCOME window

CPageWELCOME::CPageWELCOME()
	: CExtResizablePropertyPage( CPageWELCOME::IDD )
{
	//{{AFX_DATA_INIT(CPageWELCOME)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

CPageWELCOME::~CPageWELCOME()
{
}

void CPageWELCOME::DoDataExchange( CDataExchange * pDX )
{
	CExtResizablePropertyPage::DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CPageWELCOME)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_CUSTOM1, m_wndRCS );
}

BEGIN_MESSAGE_MAP( CPageWELCOME, CExtResizablePropertyPage )
	//{{AFX_MSG_MAP(CPageWELCOME)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPageWELCOME::OnInitDialog()
{
	CExtResizablePropertyPage::OnInitDialog();
	m_wndRCS.SetFont( CFont::FromHandle( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) ) );

USES_CONVERSION;
CExtResourceManager::CExtResourceMemoryBuffer buffer;
	VERIFY( g_ResourceManager->LoadResourceBuffer( buffer, MAKEINTRESOURCE(RT_HTML), ID_HTML_WELCOME ) );
DWORD dwBufferSize = buffer.GetSize();
	ASSERT( dwBufferSize > 0 );
	ASSERT( buffer.GetBuffer() != NULL );
CString str;
LPTSTR pBuffer = str.GetBuffer( dwBufferSize + 2 );
	ASSERT( pBuffer != NULL );
	::memset( pBuffer, 0, ( dwBufferSize + 2 ) * sizeof(TCHAR) );
	__EXT_MFC_STRNCPY( pBuffer, dwBufferSize + 1, A2CT(LPSTR(buffer.GetBuffer())), dwBufferSize );
	str.ReleaseBuffer();

	m_wndRCS.RcsTextSet( str );
	AddAnchor( IDC_CUSTOM1, __RDA_LT, __RDA_RB );

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CTextEditor window

CTextEditor::CTextEditor()
	: CExtNCSB < CExtEdit > ( true )
{
	m_bRichMode = true;
	m_bHandleCtxMenus = true;
}

CTextEditor::~CTextEditor()
{
}

BEGIN_MESSAGE_MAP( CTextEditor, CExtEdit )
	//{{AFX_MSG_MAP(CTextEditor)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_PASTE, OnPaste )
END_MESSAGE_MAP()

LRESULT CTextEditor::WindowProc( UINT message, WPARAM wParam, LPARAM lParam ) 
{
	switch( message )
	{
	case WM_RBUTTONDOWN:
		{
			POINT ptScreen;
			if( ::GetCursorPos( &ptScreen) )
				SendMessage( WM_CONTEXTMENU, WPARAM(m_hWnd), MAKELPARAM(ptScreen.x,ptScreen.y) );
		}
		return 0L;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		{ // block - clipboard commands
			bool bAlt = ( (::GetAsyncKeyState(VK_MENU)&0x8000) != 0 ) ? true : false;
			bool bCtrl = ( (::GetAsyncKeyState(VK_CONTROL)&0x8000) != 0 ) ? true : false;
			bool bShift = ( (::GetAsyncKeyState(VK_SHIFT)&0x8000) != 0 ) ? true : false;
			if( ! bAlt )
			{
				if(		bCtrl && ! bShift 
					&&	(	INT(wParam) == VK_INSERT 
					||	INT(wParam) == INT( _T('C') )
					)
					)
				{
					if( message == WM_KEYDOWN )
						SendMessage( WM_COPY, 0, 0 );
					return 0L;
				}
				
				if(		( bCtrl && (!bShift) && int(wParam) == int( _T('V') ) )
					||	( (!bCtrl) && bShift && int(wParam) == VK_INSERT )
					)
				{
					if( message == WM_KEYDOWN && ( GetStyle() & ES_READONLY ) == 0 )
					{
						SendMessage( WM_PASTE, 0, 0 );
						//Default();
					}
					return 0L;
				}
				if(		( bCtrl && (!bShift) && int(wParam) == int( _T('X') ) )
					||	( (!bCtrl) && bShift && int(wParam) == VK_DELETE )
					)
				{
					if( message == WM_KEYDOWN )
						SendMessage( ( ( GetStyle() & ES_READONLY ) == 0 ) ? WM_CUT : WM_COPY, 0, 0 );
					return 0L;
				}
			}
		} // block - clipboard commands
	break;
	} // switch( message )
LRESULT lResult = CExtNCSB < CExtEdit > :: WindowProc( message, wParam, lParam );
	return lResult;
}

void CTextEditor::PreSubclassWindow()
{
	ASSERT_VALID( this );
	CExtNCSB < CExtEdit > :: PreSubclassWindow();
	::SendMessage( m_hWnd, EM_SETEVENTMASK, 0, ENM_CHANGE|ENM_SELCHANGE|ENM_PROTECTED );
	::SendMessage( m_hWnd, EM_SETOPTIONS, WPARAM(ECOOP_AND), LPARAM(ECO_AUTOWORDSELECTION|ECO_AUTOVSCROLL /*|ECO_NOHIDESEL*/ ) );
static CExtNotImplRichEditOleCallBack g_stat_cb_not_impl;
	::SendMessage( m_hWnd, EM_SETOLECALLBACK, 0, (LPARAM)(&g_stat_cb_not_impl) );
	ModifyStyle( 0, ES_WANTRETURN );
CHARFORMAT cfDefault;
	::memset( &cfDefault, 0, sizeof(CHARFORMAT) );
	cfDefault.cbSize = sizeof(CHARFORMAT);
	cfDefault.dwMask = CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT|CFM_FACE|CFM_SIZE|CFM_CHARSET|CFM_PROTECTED|CFM_COLOR;
	cfDefault.bCharSet = DEFAULT_CHARSET;
	cfDefault.dwEffects = CFE_PROTECTED; 
	//cfDefault.yHeight = 180;
	//cfDefault.crTextColor = ::GetSysColor( COLOR_WINDOWTEXT );
CFont * pFont = GetFont();
	if( pFont->GetSafeHandle() == NULL )
	{
		CWnd * pWndParent = GetParent();
		if( pWndParent->GetSafeHwnd() != NULL )
			pFont = pWndParent->GetFont();
		if( pFont->GetSafeHandle() == NULL )
			pFont = &(PmBridge_GetPM()->m_FontNormal);
	}
LOGFONT _lf; ::memset( &_lf, 0, sizeof(LOGFONT) );
	pFont->GetLogFont( &_lf );
#if (! defined _RICHEDIT_VER )
	USES_CONVERSION;
	strcpy( cfDefault.szFaceName, T2CA(_lf.lfFaceName) );
#else
#if ( _RICHEDIT_VER < 0x0200 )
USES_CONVERSION;
	strcpy( cfDefault.szFaceName, T2CA(_lf.lfFaceName) );
#else
	__EXT_MFC_STRCPY( cfDefault.szFaceName, sizeof(cfDefault.szFaceName)/sizeof(cfDefault.szFaceName[0]), _lf.lfFaceName );
#endif
#endif
CWindowDC dcDesktop( NULL );
	cfDefault.yHeight = ::MulDiv( _lf.lfHeight, 72 * 20, dcDesktop.GetDeviceCaps( LOGPIXELSY ) );
	cfDefault.yHeight = abs( cfDefault.yHeight );
	cfDefault.bCharSet = DEFAULT_CHARSET;
bool bEnabled = IsWindowEnabled() ? true : false, bReadOnly = ( ( GetStyle() & ES_READONLY ) != 0 ) ? true : false;
	cfDefault.crTextColor = OnQueryTextColor();
	if( cfDefault.crTextColor == COLORREF(-1L) )
		cfDefault.crTextColor = ::GetSysColor( bEnabled ? ( bReadOnly ? COLOR_3DSHADOW : COLOR_WINDOWTEXT ) : COLOR_GRAYTEXT );
COLORREF clrBack = OnQueryBackColor();
	if( clrBack == COLORREF(-1L) )
		clrBack = ::GetSysColor( bEnabled ? ( bReadOnly ? COLOR_3DFACE : COLOR_WINDOW ) : COLOR_3DFACE );
	Rich_SetBackgroundColor( FALSE, clrBack );

	Rich_HideSelection( true, false );
CHARRANGE crRestore;
	Rich_GetSel( crRestore );
	Rich_SetSel( 0, -1 ); 
	SendMessage( EM_SETCHARFORMAT, SCF_DEFAULT, LPARAM(&cfDefault) );
	SendMessage( EM_SETCHARFORMAT, SCF_SELECTION, LPARAM(&cfDefault) );
	Rich_SetSelectionColor( cfDefault.crTextColor );
	Rich_SetSel( crRestore );
	Rich_HideSelection( false, false );
}

void CTextEditor::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	ASSERT_VALID( this );
bool bAlt = ( (::GetAsyncKeyState(VK_MENU)&0x8000) != 0 ) ? true : false;
bool bCtrl = ( (::GetAsyncKeyState(VK_CONTROL)&0x8000) != 0 ) ? true : false;
bool bShift = ( (::GetAsyncKeyState(VK_SHIFT)&0x8000) != 0 ) ? true : false;
	if( ! bAlt )
	{
		if( (!bCtrl) && (!bShift) && int(nChar) == VK_RETURN )
		{
			ReplaceSel( _T("\r\n"), TRUE );
			return;
		}
		if( bCtrl && (!bShift) && (	int(nChar) == VK_INSERT || int(nChar) == int( _T('C') ) ) )
		{
			SendMessage( WM_COPY );
			return;
		}
		if( ( bCtrl && (!bShift) && int(nChar) == int( _T('V') ) ) || ( (!bCtrl) && bShift && int(nChar) == VK_INSERT ) )
		{
			SendMessage( WM_PASTE );
			return;
		}
		if( ( bCtrl && (!bShift) && int(nChar) == int( _T('X') ) ) || ( (!bCtrl) && bShift && int(nChar) == VK_DELETE ) )
		{
			SendMessage( WM_CUT );
			return;
		}
		if( bCtrl && (!bShift) && int(nChar) == int( _T('A') ) )
		{
			SendMessage( WM_COMMAND, ID_EDIT_SELECT_ALL );
			return;
		}
		if( (!bCtrl) && (!bShift) && ( ( int(nChar) == VK_DELETE || int(nChar) == VK_BACK ) && (!Rich_GetSelText().IsEmpty()) ) )
		{
			SendMessage( WM_CLEAR );
			return;
		}
		if( ( (!bAlt) && bCtrl && (!bShift) && int(nChar) == int( _T('Z') ) ) || ( bAlt && (!bCtrl) && (!bShift) && int(nChar) == VK_BACK ) )
		{
			SendMessage( EM_UNDO );
			return;
		}
		if( (!bAlt) && bCtrl && (!bShift) && int(nChar) == int( _T('Y') ) )
		{
			SendMessage( EM_REDO );
			return;
		}
	} // if( ! bAlt )
	CExtNCSB < CExtEdit > :: OnKeyDown( nChar, nRepCnt, nFlags );
}

LRESULT CTextEditor::OnPaste( WPARAM wParam, LPARAM lParam )
{
	ASSERT_VALID( this );
	wParam; lParam;
bool bAvailableTextA = false, bAvailableTextW = false;
	if( g_PaintManager.m_bIsWinNT4orLater && IsClipboardFormatAvailable( CF_UNICODETEXT ) )
		bAvailableTextW = true;
	else if( IsClipboardFormatAvailable( CF_TEXT ) )
		bAvailableTextA = true;
	if( ! ( bAvailableTextW || bAvailableTextA ) )
		return 0L;
	if( ! ::OpenClipboard( GetSafeHwnd() ) )
		return 0L;
CExtSafeString strClipboardText;
bool bHaveClipboardText = false;
	if( bAvailableTextW )
	{
		HGLOBAL h = ::GetClipboardData( CF_UNICODETEXT );
		if( h != NULL )
		{
			LPWSTR strUnicodeBuffer = (LPWSTR) ::GlobalLock( h );
			if( strUnicodeBuffer != NULL )
			{
				bHaveClipboardText = true;
				USES_CONVERSION;
				LPCTSTR strBuffer = W2CT(strUnicodeBuffer);
				strClipboardText = strBuffer;
				::GlobalUnlock( h );
			}
		}
	} // if( bAvailableTextW )
	if( ( ! bHaveClipboardText ) && bAvailableTextA )
	{
		HGLOBAL h = ::GetClipboardData( CF_TEXT );
		if( h != NULL )
		{
			LPSTR strBuffer = (LPSTR) ::GlobalLock( h );
			if( strBuffer != NULL )
			{
				bHaveClipboardText = true;
				strClipboardText = LPCTSTR(strBuffer);
				::GlobalUnlock( h );
			} // if( strBuffer != NULL )
		}
	} // if( ( ! bHaveClipboardText ) && bAvailableTextA )
	::CloseClipboard();
	if( (! bHaveClipboardText) || strClipboardText.IsEmpty() )
		return 0L;
	//Rich_PlainText_Set( strClipboardText );
	ReplaceSel( strClipboardText, TRUE );
	//::SendMessage( ::GetParent( m_hWnd ), WM_COMMAND, MAKEWPARAM( GetDlgCtrlID(), EN_CHANGE ), LPARAM(m_hWnd) );
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// CPageHTML window

CPageHTML::CPageHTML()
	: CExtResizablePropertyPage( CPageHTML::IDD )
{
	//{{AFX_DATA_INIT(CPageHTML)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();

USES_CONVERSION;
CExtResourceManager::CExtResourceMemoryBuffer buffer;
	VERIFY( g_ResourceManager->LoadResourceBuffer( buffer, _T("CSS"), ID_SAMPLE123_CSS ) );
DWORD dwBufferSize = buffer.GetSize();
	ASSERT( dwBufferSize > 0 );
	ASSERT( buffer.GetBuffer() != NULL );
CString str;
LPTSTR pBuffer = str.GetBuffer( dwBufferSize + 2 );
	ASSERT( pBuffer != NULL );
	::memset( pBuffer, 0, ( dwBufferSize + 2 ) * sizeof(TCHAR) );
	__EXT_MFC_STRNCPY( pBuffer, dwBufferSize + 1, A2CT(LPSTR(buffer.GetBuffer())), dwBufferSize );
	str.ReleaseBuffer();
LPCTSTR strHREF = _T("sample123.css");
CExtRichDocObjectBase & _GC = CExtRichContentLayout::stat_GetObjectCountainerGlobal();
CExtRichDocObjectLayoutKeeper * pObjCSS = new CExtRichDocObjectLayoutKeeper( strHREF );
	VERIFY( _GC.ObjectAdd( pObjCSS, false ) );
	pObjCSS->SourceTextSet( LPCTSTR(str), CExtRichContentLayout::__ELFMT_CSS );

}

CPageHTML::~CPageHTML()
{
}

void CPageHTML::DoDataExchange( CDataExchange * pDX )
{
	CExtResizablePropertyPage::DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CPageHTML)
	DDX_Control( pDX, IDC_COMBO_TYPE_SCROLL_BARS, m_comboScollBarsType );
	DDX_Control( pDX, IDC_COMBO_TYPE_TEXT_ORIENTATION, m_comboTextOrientation );
	DDX_Control( pDX, IDC_BUTTON_RELAYOUT, m_hyperlinkReLayout );
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_CUSTOM1, m_wndRCS );
	DDX_Control( pDX, IDC_EDIT_DRAW_TEXT, m_wndEditHTML );
}

BEGIN_MESSAGE_MAP( CPageHTML, CExtResizablePropertyPage )
	//{{AFX_MSG_MAP(CPageHTML)
	ON_COMMAND( IDC_BUTTON_RELAYOUT, OnReLayout )
	ON_CBN_SELENDOK( IDC_COMBO_TYPE_SCROLL_BARS, OnSelEndOkScrollBarsType )
	ON_CBN_SELENDOK( IDC_COMBO_TYPE_TEXT_ORIENTATION, OnReLayout )
	//}}AFX_MSG_MAP
	ON_EN_CHANGE( IDC_EDIT_DRAW_TEXT, OnChangeEditHTML )
END_MESSAGE_MAP()

BOOL CPageHTML::OnInitDialog()
{
	CExtResizablePropertyPage::OnInitDialog();
	m_wndRCS.SetFont( CFont::FromHandle( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) ) );

	m_hyperlinkReLayout.m_bUseStdCommandNotification = true;

	m_comboScollBarsType.SetItemData( m_comboScollBarsType.AddString( _T("<html>classic</html>") ), INT(CExtRichStyleDescription::e_ui_sbt_classic) );
	m_comboScollBarsType.SetItemData( m_comboScollBarsType.AddString( _T("<html>pop-over <i>(light)</i></html>") ), INT(CExtRichStyleDescription::e_ui_sbt_pop_over_light) );
	m_comboScollBarsType.SetItemData( m_comboScollBarsType.AddString( _T("<html>pop-over <i>(full)</i></html>") ), INT(CExtRichStyleDescription::e_ui_sbt_pop_over_full) );
	m_comboScollBarsType.SetItemData( m_comboScollBarsType.AddString( _T("<html>hidden</html>") ), INT(CExtRichStyleDescription::e_ui_sbt_none) );
	m_comboScollBarsType.SetCurSel( 0 );

	m_comboTextOrientation.AddString( _T("<html>Normal/default (0&deg;)</html>") );
	m_comboTextOrientation.AddString( _T("<html>90&deg; clockwise</html>") );
	m_comboTextOrientation.AddString( _T("<html>180&deg; clockwise</html>") );
	m_comboTextOrientation.AddString( _T("<html>270&deg; clockwise</html>") );
	m_comboTextOrientation.SetCurSel( 0 );

USES_CONVERSION;
CExtResourceManager::CExtResourceMemoryBuffer buffer;
	VERIFY( g_ResourceManager->LoadResourceBuffer( buffer, MAKEINTRESOURCE(RT_HTML), ID_HTML_DEMO_PAGE ) );
DWORD dwBufferSize = buffer.GetSize();
	ASSERT( dwBufferSize > 0 );
	ASSERT( buffer.GetBuffer() != NULL );
CString str;
LPTSTR pBuffer = str.GetBuffer( dwBufferSize + 2 );
	ASSERT( pBuffer != NULL );
	::memset( pBuffer, 0, ( dwBufferSize + 2 ) * sizeof(TCHAR) );
	__EXT_MFC_STRNCPY( pBuffer, dwBufferSize + 1, A2CT(LPSTR(buffer.GetBuffer())), dwBufferSize );
	str.ReleaseBuffer();

	m_wndEditHTML.SetWindowText( LPCTSTR(str) );

	AddAnchor( IDC_CUSTOM1, __RDA_LT, CSize( 100, 50 ) );
	AddAnchor( IDC_EDIT_DRAW_TEXT, CSize( 0, 50 ), __RDA_RB );
	AddAnchor( IDC_STATIC_TYPE_TEXT_HERE, CSize( 0, 50 ) );
	//IDC_STATIC_SEE_TEXT_HERE
	AddAnchor( IDC_STATIC_TYPE_SCROLL_BARS, CSize( 100, 50 ) );
	AddAnchor( IDC_COMBO_TYPE_SCROLL_BARS, CSize( 100, 50 ) );
	AddAnchor( IDC_STATIC_TYPE_TEXT_ORIENTATION, CSize( 100, 50 ) );
	AddAnchor( IDC_COMBO_TYPE_TEXT_ORIENTATION, CSize( 100, 50 ) );
	AddAnchor( IDC_BUTTON_RELAYOUT, CSize( 100, 50 ) );

	OnChangeEditHTML();

int nIndex, nCount;
static struct
{
	UINT m_nID;
	LPCTSTR m_strToolTip;
} g_arrToolTipData[] =
{
	{ IDC_EDIT_DRAW_TEXT,							_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Type HTML here <img src=\"smile.bmp\"> ...</p></html>") },
	{ IDC_COMBO_TYPE_TEXT_ORIENTATION,				_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Specifies HTML orientation.</p></html>") },
	{ IDC_COMBO_TYPE_SCROLL_BARS,					_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Specifies type of scroll bars.</p></html>") },
	{ IDC_BUTTON_RELAYOUT,							_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Performs the following actions:<ol><li>Reload styles of all the elements in the <b>D</b>ocument <b>O</b>bject <b>M</b>odel.</li><li>Recompute layout of all the elements.</li><li>Repaint the HTML output window.</li></ol><hr>To reparse HTML, just enter some text in the editor window below.</p></html>") },
};
	nCount = int( sizeof(g_arrToolTipData) / sizeof(g_arrToolTipData[0]) ) ;
	for( nIndex = 0; nIndex < nCount; nIndex ++ )
	{
		CWnd * pDlgItem = GetDlgItem( g_arrToolTipData[nIndex].m_nID );
		ASSERT( pDlgItem != NULL );
		CExtButton * pBtn = DYNAMIC_DOWNCAST( CExtButton, pDlgItem );
		if( pBtn != NULL )
		{
			pBtn->SetTooltipText( g_arrToolTipData[nIndex].m_strToolTip );
			continue;
		}
		CExtComboBox * pComboBox = DYNAMIC_DOWNCAST( CExtComboBox, pDlgItem );
		if( pComboBox != NULL )
		{
			pComboBox->SetTooltipText( g_arrToolTipData[nIndex].m_strToolTip );
			continue;
		}
		CExtEdit * pEdit = DYNAMIC_DOWNCAST( CExtEdit, pDlgItem );
		if( pEdit != NULL )
		{
			pEdit->SetTooltipText( g_arrToolTipData[nIndex].m_strToolTip );
			continue;
		}
	}


	return FALSE;
}

void CPageHTML::OnSelEndOkScrollBarsType()
{
int nCurSel = int( m_comboScollBarsType.GetCurSel() );
	if( nCurSel < 0 )
		return;
CExtRichStyleDescription::e_ui_sb_type_t _eUiSBT = (CExtRichStyleDescription::e_ui_sb_type_t)m_comboScollBarsType.GetItemData( nCurSel );
	if( _eUiSBT == CExtRichStyleDescription::e_ui_sbt_pop_over_light )
	{
		m_wndRCS.m_bHideNormalScrollBars = true;
		m_wndRCS.m_bDisplayPopOverScrollBars = true;
		m_wndRCS.m_bLightPopOverScrollBars = true;
	}
	else if( _eUiSBT == CExtRichStyleDescription::e_ui_sbt_pop_over_full )
	{
		m_wndRCS.m_bHideNormalScrollBars = true;
		m_wndRCS.m_bDisplayPopOverScrollBars = true;
		m_wndRCS.m_bLightPopOverScrollBars = false;
	}
	else if( _eUiSBT == CExtRichStyleDescription::e_ui_sbt_none )
	{
		m_wndRCS.m_bHideNormalScrollBars = true;
		m_wndRCS.m_bDisplayPopOverScrollBars = false;
	}
	else
	{	// else switch to classic scroll bars
		m_wndRCS.m_bHideNormalScrollBars = false;
		m_wndRCS.m_bDisplayPopOverScrollBars = false;
	}
	OnReLayout();
}

void CPageHTML::OnChangeEditHTML() 
{
CString strHTML = m_wndEditHTML.Rich_PlainText_Get();
	m_wndRCS.RcsTextSet( strHTML );
}

void CPageHTML::OnReLayout()
{
CExtRichContentLayout::e_layout_orientation_t eLO = (CExtRichContentLayout::e_layout_orientation_t)m_comboTextOrientation.GetCurSel();
	if( eLO == 0 )
		eLO = CExtRichContentLayout::__ELOT_NORMAL;
	else if( eLO == 1 )
		eLO = CExtRichContentLayout::__ELOT_90_CW;
	else if( eLO == 2 )
		eLO = CExtRichContentLayout::__ELOT_180_CW;
	else if( eLO == 3 )
		eLO = CExtRichContentLayout::__ELOT_270_CW;
CExtRichContentLayout * pRCL = m_wndRCS.RcsLayoutGet();
	pRCL->m_eLO = eLO;
//	pRCL->_DoLayout_ListOfCSS_Rebuild();
CExtRichContentItem * pRootRCI = ( pRCL->m_listHeap.GetCount() > 0 ) ? pRCL->m_listHeap.GetHead() : NULL;
	if( pRootRCI != NULL )
		pRootRCI->ResetStyleInfo();
	m_wndRCS.RcsDelayRecalcLayout();
	m_wndRCS.OnSwRecalcLayout( true );
	m_wndRCS.OnSwUpdateScrollBars();
	m_wndRCS.OnSwDoRedraw();
}

/////////////////////////////////////////////////////////////////////////////
// CDialogCssEditor window

CDialogCssEditor::CDialogCssEditor(
	bool bEditingNewCSS,
	LPCTSTR strHREF, // = NULL
	LPCTSTR strCSS // = NULL
	)
	: CExtNCW < CExtResizableDialog > ( CDialogCssEditor::IDD )
	, m_bEditingNewCSS( bEditingNewCSS )
	, m_strInitialHREF( ( strHREF != NULL ) ? strHREF : _T("") )
	, m_strInitialCSS( ( strCSS != NULL ) ? strCSS : _T("") )
	, m_editCSS( true )
{
	//{{AFX_DATA_INIT(CDialogCssEditor)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
	m_strHREF = m_strInitialHREF;
	m_strInitialCSS.Replace( _T("\r"), _T("") );
	m_strInitialCSS.Replace( _T("\n"), _T("\r\n") );
	m_strCSS = m_strInitialCSS;
}

CDialogCssEditor::~CDialogCssEditor()
{
}

void CDialogCssEditor::DoDataExchange( CDataExchange * pDX )
{
	CExtNCW < CExtResizableDialog > :: DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CDialogCssEditor)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_EDIT_HREF, m_editHREF );
	DDX_Control( pDX, IDC_EDIT_CSS_CODE, m_editCSS );
}

BEGIN_MESSAGE_MAP( CDialogCssEditor, CExtResizableDialog )
	//{{AFX_MSG_MAP(CDialogCssEditor)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE( IDC_EDIT_HREF, OnChangeEditors )
	ON_EN_CHANGE( IDC_EDIT_CSS_CODE, OnChangeEditors )
END_MESSAGE_MAP()

BOOL CDialogCssEditor::OnInitDialog()
{
	CExtNCW < CExtResizableDialog > :: OnInitDialog();
	if( ! m_bEditingNewCSS )
	{
		m_editHREF.SetWindowText( LPCTSTR(m_strInitialHREF) );
		m_editHREF.ModifyStyle( 0, ES_READONLY );
		m_editCSS.SetWindowText( LPCTSTR(m_strInitialCSS) );
	}
	SetWindowText( m_bEditingNewCSS ? _T("CSS Editor (adding new CSS document)") : _T("CSS Editor (editing CSS document)") );
	AddAnchor( IDC_EDIT_HREF, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_EDIT_CSS_CODE, __RDA_LT, __RDA_RB );
	AddAnchor( IDOK, __RDA_RB );
	AddAnchor( IDCANCEL, __RDA_RB );
	EnableSaveRestore( _T( "Resizable Dialogs" ), _T( "CSS Editor Dialog" ) );
	_Validate( false );
	if( m_bEditingNewCSS )
		m_editHREF.SetFocus();
	else
		m_editCSS.SetSel( 0, 0 );
	return TRUE;
}

void CDialogCssEditor::OnChangeEditors()
{
	_Validate( false );
}

bool CDialogCssEditor::_Validate( bool bSave )
{
	if( m_bEditingNewCSS )
	{
		m_editHREF.GetWindowText( m_strHREF );
		m_strHREF.TrimLeft( __EXT_RICH_CONTENT_TRIM_CHARS_AND_QUOTES );
		m_strHREF.TrimRight( __EXT_RICH_CONTENT_TRIM_CHARS_AND_QUOTES );
		if( m_strHREF.IsEmpty() )
		{
			if( bSave )
				m_editHREF.SetFocus();
			GetDlgItem( IDOK )->EnableWindow( FALSE );
			return false;
		}
	}
//	if( bSave )
	{
		m_editCSS.GetWindowText( m_strCSS );
		if( ( ! m_bEditingNewCSS ) && m_strInitialCSS == m_strCSS )
		{
			GetDlgItem( IDOK )->EnableWindow( FALSE );
			return false;
		}
	}
	GetDlgItem( IDOK )->EnableWindow( TRUE );
	return true;
}

void CDialogCssEditor::OnOK()
{
	if( _Validate( true ) )
		CExtNCW < CExtResizableDialog > :: OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CPageGC window

CPageGC::CPageGC()
	: CExtResizablePropertyPage( CPageGC::IDD )
{
	//{{AFX_DATA_INIT(CPageGC)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

CPageGC::~CPageGC()
{
}

void CPageGC::DoDataExchange( CDataExchange * pDX )
{
	CExtResizablePropertyPage::DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CPageGC)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_LIST1, m_wndLV );
	DDX_Control( pDX, IDC_BUTTON_ADD_IMAGE, m_wndHyperLinkAddImage );
	DDX_Control( pDX, IDC_BUTTON_ADD_CSS, m_wndHyperLinkAddCSS );
	DDX_Control( pDX, IDC_BUTTON_EDIT_SELECTED_CSS, m_wndHyperLinkEditSelectedCSS );
	DDX_Control( pDX, IDC_BUTTON_DELETE_SELECTED, m_wndHyperLinkDeleteSelected );
}

BEGIN_MESSAGE_MAP( CPageGC, CExtResizablePropertyPage )
	//{{AFX_MSG_MAP(CPageGC)
	ON_WM_DESTROY()
	ON_NOTIFY( LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1 )
	ON_NOTIFY( NM_DBLCLK, IDC_LIST1, OnDblclkList1 )
	//}}AFX_MSG_MAP
	ON_COMMAND( IDC_BUTTON_ADD_IMAGE, OnAddImage )
	ON_COMMAND( IDC_BUTTON_ADD_CSS, OnAddCSS )
	ON_COMMAND( IDC_BUTTON_EDIT_SELECTED_CSS, OnEditSelectedCSS )
	ON_COMMAND( IDC_BUTTON_DELETE_SELECTED, OnDeleteSelected )
END_MESSAGE_MAP()

BOOL CPageGC::OnInitDialog()
{
	CExtResizablePropertyPage::OnInitDialog();

	m_wndHyperLinkAddImage.m_bUseStdCommandNotification = true;
	m_wndHyperLinkAddCSS.m_bUseStdCommandNotification = true;
	m_wndHyperLinkEditSelectedCSS.m_bUseStdCommandNotification = true;
	m_wndHyperLinkDeleteSelected.m_bUseStdCommandNotification = true;
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	m_wndLV.SetExtendedStyle( m_wndLV.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP );
	m_wndLV.InsertColumn( 0, _T("Name"), LVCFMT_LEFT, 250 );
	m_wndLV.InsertColumn( 1, _T("Type"), LVCFMT_CENTER, 80 );
	_ReInsertObjectsIntoListView();
	m_wndLV.StateLoad( pApp->m_pszRegistryKey, pApp->m_pszProfileName, _T("ExtListCtrl_GC") );

	AddAnchor( IDC_LIST1, __RDA_LT, __RDA_LB );

	_CheckListViewSelection();

	return FALSE;
}

void CPageGC::OnDestroy() 
{
	ASSERT_VALID( this );
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );
	ASSERT( pApp->m_pszProfileName != NULL );
	ASSERT( pApp->m_pszProfileName[0] != _T('\0') );

	m_wndLV.StateSave( pApp->m_pszRegistryKey, pApp->m_pszProfileName, _T("ExtListCtrl_GC") );
	CExtResizablePropertyPage::OnDestroy();
}

void CPageGC::_ReInsertObjectsIntoListView()
{
	ASSERT_VALID( this );
	m_wndLV.DeleteAllItems();
CExtRichDocObjectBase & _GC = CExtRichContentLayout::stat_GetObjectCountainerGlobal();
POSITION pos = _GC.ObjectGetStartPosition();
	for( ; pos != NULL; )
	{
		CExtRichDocObjectBase * pRCO = _GC.ObjectGetNext( pos );
		ASSERT_VALID( pRCO );
		INT nItemIndex = m_wndLV.InsertItem( -1, pRCO->GetName() );
		CString strSubItemText;
		if( pRCO->IsObjectContainer() )
			strSubItemText = _T("Container");
		else if( pRCO->IsImage() )
			strSubItemText = _T("Image");
		else if( pRCO->IsLayoutKeeper() )
		{
			CExtRichContentLayout & _layout = ((CExtRichDocObjectLayoutKeeper*)pRCO)->LayoutGet();
			if( _layout.m_eLFMT == CExtRichContentLayout::__ELFMT_HTML )
				strSubItemText = _T("HTML");
			else if( _layout.m_eLFMT == CExtRichContentLayout::__ELFMT_CSS )
				strSubItemText = _T("CSS");
			else if( _layout.m_eLFMT == CExtRichContentLayout::__ELFMT_PLAIN_TEXT )
				strSubItemText = _T("Text");
			else
				strSubItemText.Format( _T("Unknown document type (%d)"), int(_layout.m_eLFMT) );
		}
		else
			strSubItemText = pRCO->GetRuntimeClass()->m_lpszClassName;
		VERIFY( m_wndLV.SetItem( nItemIndex, 1, LVIF_TEXT, strSubItemText, -1, 0L, 0L, 0L ) );
	}
}

void CPageGC::_CheckListViewSelection()
{
	ASSERT_VALID( this );
CList < INT, INT > _listSelectedItems;
	m_wndLV.GetSelectedItemsList( _listSelectedItems );

BOOL bHaveSelection = ( _listSelectedItems.GetCount() == 1 ) ? TRUE : FALSE;
	m_wndHyperLinkDeleteSelected.EnableWindow( bHaveSelection );

BOOL bEnableEditCSS = bHaveSelection;
	if( bEnableEditCSS )
	{
		bEnableEditCSS = FALSE;
		INT nCurSel = _listSelectedItems.GetHead();
		CString strName = m_wndLV.GetItemText( nCurSel, 0 );
		CExtRichDocObjectBase & _GC = CExtRichContentLayout::stat_GetObjectCountainerGlobal();
		CExtRichDocObjectBase * pDO = _GC.ObjectByName( LPCTSTR(strName) );
		if( pDO != NULL )
		{
			CExtRichDocObjectLayoutKeeper * pObjCSS = DYNAMIC_DOWNCAST( CExtRichDocObjectLayoutKeeper, pDO );
			if( pObjCSS != NULL && pObjCSS->LayoutGet().m_eLFMT == CExtRichContentLayout::__ELFMT_CSS )
				bEnableEditCSS = TRUE;
		}
	}
	m_wndHyperLinkEditSelectedCSS.EnableWindow( bEnableEditCSS );
}

void CPageGC::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ASSERT_VALID( this );
NM_LISTVIEW * pNMListView = (NM_LISTVIEW *)pNMHDR;
	pNMListView;
	*pResult = 0;
	_CheckListViewSelection();
}

void CPageGC::OnAddImage()
{
	ASSERT_VALID( this );

#if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtShellDialogFile dlgShellFile( this, CExtShellDialogFile::__EFDT_OPEN_SINGLE );
	dlgShellFile.m_comboFileTypes.SetFilter(
		_T("Bitmap files (*.bmp)|*.bmp|")
		_T("All Files (*.*)|*.*|")
		_T("|")
		);
INT nRetVal = (INT)dlgShellFile.DoModal();
	if( nRetVal != IDOK )
		return;
CExtSafeString strFilePath = dlgShellFile.m_arrRetValNames.ElementAt( 0 );
	ASSERT( ! strFilePath.IsEmpty() );

#else // ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CFileDialog dlgFile(
		TRUE,
		_T("bmp"),
		_T(""),
		OFN_ENABLESIZING|OFN_EXPLORER
			|OFN_LONGNAMES|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST
			|OFN_NOCHANGEDIR|OFN_NODEREFERENCELINKS|OFN_HIDEREADONLY
			,
		_T("Bitmap files (*.bmp)|*.bmp|")
		_T("All Files (*.*)|*.*|")
		_T("|"),
		this
		);
	dlgFile.m_ofn.lpstrTitle = _T("Open image file");
	if( dlgFile.DoModal() != IDOK )
		return;
CExtSafeString strFilePath = dlgFile.m_ofn.lpstrFile;
	ASSERT( ! strFilePath.IsEmpty() );

#endif // else from if ( ! defined __EXT_MFC_NO_SHELL_DIALOG_FILE )

CExtBitmap _bmp;
	if( ! _bmp.LoadBMP_File( LPCTSTR(strFilePath), true ) )
		return;
TCHAR	strDrive[    _MAX_PATH + 1 ],
		strDir[      _MAX_PATH + 1 ],
		strFileName[ _MAX_PATH + 1 ],
		strFileExt[  _MAX_PATH + 1 ];
	::memset( strDrive,    0, _MAX_PATH + 1 );
	::memset( strDir,      0, _MAX_PATH + 1 );
	::memset( strFileName, 0, _MAX_PATH + 1 );
	::memset( strFileExt,  0, _MAX_PATH + 1 );
	__EXT_MFC_SPLITPATH(
		LPCTSTR(strFilePath),
		strDrive, _MAX_PATH,
		strDir, _MAX_PATH,
		strFileName, _MAX_PATH,
		strFileExt, _MAX_PATH
		);
	strFilePath.Format( _T("%s%s"), strFileName, strFileExt );
CExtRichDocObjectBase & _GC = CExtRichContentLayout::stat_GetObjectCountainerGlobal();
	if(		_GC.ObjectByName( LPCTSTR(strFilePath) ) != NULL
		&&	::AfxMessageBox( _T("Image with the same name is already in the global container. Replace it with new image?"), MB_YESNO|MB_ICONQUESTION ) != IDYES
		)
		return;
CExtRichDocObjectImage * pObjPic = new CExtRichDocObjectImage( LPCTSTR(strFilePath) );
	pObjPic->ImageGet() = _bmp;
	VERIFY( _GC.ObjectAdd( pObjPic ) );
	_ReInsertObjectsIntoListView();
}

void CPageGC::OnAddCSS()
{
	ASSERT_VALID( this );
CDialogCssEditor dlg( true );
	if( dlg.DoModal() != IDOK )
		return;
CExtRichDocObjectBase & _GC = CExtRichContentLayout::stat_GetObjectCountainerGlobal();
CExtRichDocObjectLayoutKeeper * pObjCSS = new CExtRichDocObjectLayoutKeeper( LPCTSTR(dlg.m_strHREF) );
	VERIFY( _GC.ObjectAdd( pObjCSS, false ) );
	pObjCSS->SourceTextSet( LPCTSTR(dlg.m_strCSS), CExtRichContentLayout::__ELFMT_CSS );
	_ReInsertObjectsIntoListView();
	_CheckListViewSelection();
}

void CPageGC::OnEditSelectedCSS()
{
	ASSERT_VALID( this );
CList < INT, INT > _listSelectedItems;
	m_wndLV.GetSelectedItemsList( _listSelectedItems );
	ASSERT( _listSelectedItems.GetCount() == 1 );
INT nCurSel = _listSelectedItems.GetHead();
CString strName = m_wndLV.GetItemText( nCurSel, 0 );
CExtRichDocObjectBase & _GC = CExtRichContentLayout::stat_GetObjectCountainerGlobal();
CExtRichDocObjectBase * pDO = _GC.ObjectByName( LPCTSTR(strName) );
	ASSERT( pDO != NULL );
CExtRichDocObjectLayoutKeeper * pObjCSS = DYNAMIC_DOWNCAST( CExtRichDocObjectLayoutKeeper, pDO );
	ASSERT( pObjCSS != NULL && pObjCSS->LayoutGet().m_eLFMT == CExtRichContentLayout::__ELFMT_CSS );
CDialogCssEditor dlg( false, pObjCSS->GetName(), pObjCSS->SourceTextGet() );
	if( dlg.DoModal() != IDOK )
		return;
	pObjCSS->SourceTextSet( LPCTSTR(dlg.m_strCSS), CExtRichContentLayout::__ELFMT_CSS );
	_CheckListViewSelection();
}

void CPageGC::OnDeleteSelected()
{
	ASSERT_VALID( this );
	if( ::AfxMessageBox( _T("Delete selected?"), MB_YESNO|MB_ICONQUESTION ) == IDYES )
	{
		CList < INT, INT > _listSelectedItems;
		m_wndLV.GetSelectedItemsList( _listSelectedItems );
		if( _listSelectedItems.GetCount() > 0 )
		{
			CExtRichDocObjectBase & _GC = CExtRichContentLayout::stat_GetObjectCountainerGlobal();
			POSITION pos = _listSelectedItems.GetTailPosition();
			for( ; pos != NULL; )
			{
				INT nIndex = _listSelectedItems.GetPrev( pos );
				CString strName = m_wndLV.GetItemText( nIndex, 0 );
				VERIFY( _GC.ObjectRemove( LPCTSTR(strName) ) );
				VERIFY( m_wndLV.DeleteItem( nIndex ) );
			}
			//_ReInsertObjectsIntoListView();
		}
	}
	_CheckListViewSelection();
}

void CPageGC::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	pNMHDR;
	(*pResult) = 0;
CPoint ptCursor;
	if( ! ::GetCursorPos( &ptCursor ) )
		return;
	m_wndLV.ScreenToClient( &ptCursor );
INT nHT = m_wndLV.HitTest( ptCursor, NULL );
	if( nHT < 0 )
		return;
CList < INT, INT > _listSelectedItems;
	_listSelectedItems.AddTail( nHT );
	m_wndLV.SelectedItemsListSet( _listSelectedItems );
	_CheckListViewSelection();
	if( m_wndHyperLinkEditSelectedCSS.IsWindowEnabled() )
		OnEditSelectedCSS();
}

/////////////////////////////////////////////////////////////////////////////
// CDrawTextSurfaceWnd window

CDrawTextSurfaceWnd::CDrawTextSurfaceWnd()
	: m_nDTF( 0 )
	, m_nExtDTF( 0 )
	, m_rcCustomRect( 0, 0, 0, 0 )
{
}

CDrawTextSurfaceWnd::~CDrawTextSurfaceWnd()
{
}

const CExtAnimationParameters *
	CDrawTextSurfaceWnd::AnimationClient_OnQueryAnimationParameters(
		INT eAPT // __EAPT_*** animation type
		) const
{
	ASSERT_VALID( this );
	eAPT;
	return (&CExtPaintManager::g_DefAnimationParametersEmpty);
}

CRect CDrawTextSurfaceWnd::_GetLayoutRect()
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
CRect rcLayout( 0, 0, 0, 0 );
CPageDT * pDlg = (CPageDT*)GetParent();
	if( pDlg->m_checkUseCustomRect.GetCheck() != 0 )
		rcLayout = m_rcCustomRect;
	else
		GetClientRect( rcLayout );
	return rcLayout;
}

void CDrawTextSurfaceWnd::_RenderImpl( // non-buffered paint
	 CDC & dc,
	 bool bTransparent, // = false,
	 bool bCombinedContent // = false
	)
{
	ASSERT_VALID( this );
	ASSERT( GetSafeHwnd() != NULL );
	bTransparent;
	bCombinedContent;

COLORREF clrText = ::GetSysColor( COLOR_WINDOWTEXT );
COLORREF clrBack = ::GetSysColor( COLOR_WINDOW );
COLORREF clrOldText = dc.SetTextColor( clrText );
COLORREF clrOldBack = dc.SetBkColor( clrBack );
INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
CFont * pOldFont = dc.SelectObject( GetFont() );

CPageDT * pDlg = (CPageDT*)GetParent();
CExtRichContentLayout::e_layout_format_t eLFMT = CExtRichContentLayout::__ELFMT_AUTO_DETECT;
int nComboTextFormatCurSel = pDlg->m_comboTextFormat.GetCurSel();
	switch( nComboTextFormatCurSel )
	{
	case 1: eLFMT = CExtRichContentLayout::__ELFMT_PLAIN_TEXT; break;
	case 2: eLFMT = CExtRichContentLayout::__ELFMT_HTML; break;
	} // switch( nComboTextFormatCurSel )

CFont * pFont = CFont::FromHandle( (HFONT) ::GetStockObject( DEFAULT_GUI_FONT ) );
CRect rcClient;
	GetClientRect( &rcClient );
INT nBackgroundType = INT( pDlg->m_comboBackgroundType.GetCurSel() );
bool bDrawDefaultWindowBackground = true;
	switch( nBackgroundType )
	{
	case 1:
		if( PmBridge_GetPM()->PaintDockerBkgnd( true, dc, this ) )
			bDrawDefaultWindowBackground = false;
	break;
	case 2:
		dc.FillSolidRect( &rcClient, ::GetSysColor( COLOR_INFOBK ) );
		bDrawDefaultWindowBackground = false;
	break;
	case 3:
		dc.FillSolidRect( &rcClient, RGB(255,255,255) );
		bDrawDefaultWindowBackground = false;
	break;
	case 4:
		dc.FillSolidRect( &rcClient, RGB(192,192,192) );
		bDrawDefaultWindowBackground = false;
	break;
	case 5:
		dc.FillSolidRect( &rcClient, RGB(128,128,128) );
		bDrawDefaultWindowBackground = false;
	break;
	case 6:
		dc.FillSolidRect( &rcClient, RGB(0,0,0) );
		bDrawDefaultWindowBackground = false;
	break;
	}
	if( bDrawDefaultWindowBackground )
		dc.FillSolidRect( &rcClient, ::GetSysColor( COLOR_WINDOW ) );
	dc.SetBkMode( TRANSPARENT );
bool bShowProfUisText = ( pDlg->m_checkProfUisText.GetCheck() != 0 ) ? true : false;
bool bShowWin32Text   = ( pDlg->m_checkWin32TextCompare.GetCheck() != 0 ) ? true : false;
bool bUseVistaTextAPIs = ( pDlg->m_checkBoxUseVistaTextAPIs.GetCheck() != 0 ) ? true : false;
CRect rcLayout = _GetLayoutRect();
CRect rcLayout_W32 = rcLayout;
CRect rcLayout_Our = rcLayout;

CString strPlainText = pDlg->m_wndDrawTextEdit.Rich_PlainText_Get();

CExtRichContentLayout::e_layout_orientation_t eLO = pDlg->m_eUserLO;

HWND hWndOpenThemeData = NULL;
LPCVOID wstrOpenThemeDataClassList = NULL;
DWORD dwOpenThemeDataFlags = 0;
int nDtThemePartID = 0, nDtThemeStateID = 0;
CExtUxTheme::__EXT_UX_DTTOPTS * pDTO = NULL;
	if( bUseVistaTextAPIs )
	{
		hWndOpenThemeData = m_hWnd;
		wstrOpenThemeDataClassList = LPVOID( VSCLASS_WINDOW );
		pDTO = &(pDlg->m_DTO);
	}

INT nRetW32 = 0, nRetOur = 0;
CString strTextForPainting_W32 = strPlainText;
CString strTextForPainting_Our = strPlainText;
UINT nDtfForPainting = m_nDTF & ( ~ ( DT_CALCRECT | DT_MODIFYSTRING ) );
	if( ( m_nDTF & ( DT_CALCRECT | DT_MODIFYSTRING ) ) != 0 )
	{
		if( bShowWin32Text )
		{
			CFont * pOldFont = dc.SelectObject( pFont );
			nRetW32 = dc.DrawText( strTextForPainting_W32, int( _tcslen( strTextForPainting_W32 ) ), &rcLayout_W32, m_nDTF );
			dc.SelectObject( pOldFont );
		}
		nRetOur =
			CExtRichContentLayout::stat_DrawText(
				eLFMT, eLO, dc, LPCTSTR(strTextForPainting_Our), int( _tcslen( strTextForPainting_Our ) ), &rcLayout_Our, m_nDTF, m_nExtDTF,
				hWndOpenThemeData, LPVOID(wstrOpenThemeDataClassList), dwOpenThemeDataFlags, nDtThemePartID, nDtThemeStateID, pDTO
				);
	}


	if( ( m_nDTF & DT_CALCRECT ) != 0 && ( bShowProfUisText || bShowWin32Text ) )
	{
		CRect rcCalcResult = rcClient;
		rcCalcResult.top = rcCalcResult.bottom - g_PaintManager->UiScalingDo( 30, CExtPaintManager::__EUIST_Y );
		dc.FillSolidRect( &rcCalcResult, RGB(220,220,220) );

		CString strCalcResultProfUIS, strCalcResultWin32;
		strCalcResultProfUIS.Format(
			_T("Prof-UIS: ret=%4d, left=%4d, top=%4d, right=%4d, bottom=%4d, width=%4d, height=%4d"),
			nRetOur,rcLayout_Our.left,rcLayout_Our.top,rcLayout_Our.right,rcLayout_Our.bottom,rcLayout_Our.Width(),rcLayout_Our.Height()
			);
		strCalcResultWin32.Format(
			_T("Win32:    ret=%4d, left=%4d, top=%4d, right=%4d, bottom=%4d, width=%4d, height=%4d"),
			nRetW32,rcLayout_W32.left,rcLayout_W32.top,rcLayout_W32.right,rcLayout_W32.bottom,rcLayout_W32.Width(),rcLayout_W32.Height()
			);
		CString strCalcResult;
		if( bShowProfUisText )
			strCalcResult += strCalcResultProfUIS;
		strCalcResult += _T("\r\n");
		if( bShowWin32Text )
			strCalcResult += strCalcResultWin32;

		LOGFONT lf;
		::memset( &lf, 0, sizeof(LOGFONT) );
		pFont->GetLogFont(&lf);
		lf.lfWidth  = 6;
		lf.lfHeight = 14;
		__EXT_MFC_STRCPY( lf.lfFaceName, sizeof(lf.lfFaceName)/sizeof(TCHAR), _T("Courier New") );
		CFont _aFont;
		_aFont.CreateFontIndirect( &lf );

		COLORREF clrTextOld = dc.SetTextColor( RGB(64,64,64) );
		int nOldBkMode = dc.SetBkMode( TRANSPARENT );
		CFont * pOldFont = dc.SelectObject( &_aFont );
		dc.DrawText( strCalcResult, &rcCalcResult, DT_NOCLIP ); 
		dc.SelectObject( pOldFont );
		dc.SetBkMode( nOldBkMode );
		dc.SetTextColor( clrTextOld );
	}

	if( bShowWin32Text )
	{
		bool bOffsetRedText = ( pDlg->m_checkWin32TextOffset.GetCheck() != 0 ) ? true : false;
		COLORREF clrTextOld = dc.SetTextColor( RGB(255,0,0) );
		CFont * pOldFont = dc.SelectObject( pFont );
		if( bOffsetRedText )
			dc.OffsetViewportOrg( 1, 1 );

		dc.DrawText( strTextForPainting_W32, int( _tcslen( strTextForPainting_W32 ) ), &rcLayout, nDtfForPainting );

		if( bOffsetRedText )
			dc.OffsetViewportOrg( -1, -1 );
		dc.SelectObject( pOldFont );
		dc.SetTextColor( clrTextOld );
	} // if( bShowWin32Text )

	if( bShowProfUisText )
		CExtRichContentLayout::stat_DrawText(
			eLFMT, eLO, dc, LPCTSTR(strTextForPainting_Our), int( _tcslen( strTextForPainting_Our ) ), &rcLayout, nDtfForPainting, m_nExtDTF,
			hWndOpenThemeData, LPVOID(wstrOpenThemeDataClassList), dwOpenThemeDataFlags, nDtThemePartID, nDtThemeStateID, pDTO
			);

	dc.SelectObject( pOldFont );
	dc.SetBkMode( nOldBkMode );
	dc.SetBkColor( clrOldBack );
	dc.SetTextColor( clrOldText );
}

/////////////////////////////////////////////////////////////////////////////
// CPageDT window

CPageDT::CPageDT()
	: CExtResizablePropertyPage( CPageDT::IDD )
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageDT)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
	m_eUserLO = CExtRichContentLayout::__ELOT_NORMAL;
	::memset( &m_DTO, 0, sizeof(CExtUxTheme::__EXT_UX_DTTOPTS) );
	m_DTO.dwSize = sizeof(CExtUxTheme::__EXT_UX_DTTOPTS);
}

CPageDT::~CPageDT()
{
}

void CPageDT::DoDataExchange( CDataExchange * pDX )
{
	CExtResizablePropertyPage::DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CPageDT)
	DDX_Control( pDX, IDC_SPIN_RECT_BOTTOM, m_spinCustomRectBottom );
	DDX_Control( pDX, IDC_SPIN_RECT_TOP, m_spinCustomRectTop );
	DDX_Control( pDX, IDC_SPIN_RECT_RIGHT, m_spinCustomRectRight );
	DDX_Control( pDX, IDC_SPIN_RECT_LEFT, m_spinCustomRectLeft );
	DDX_Control( pDX, IDC_USE_CUSTOM_RECT, m_checkUseCustomRect );
	DDX_Control( pDX, IDC_BUTTON_GET_CURRENT, m_wndHyperLinkUseCurrentRect );
	DDX_Control( pDX, IDC_CHECK_WIN32_DRAW_TEXT_OFFSET, m_checkWin32TextOffset );
	DDX_Control( pDX, IDC_CHECK_EXT_DT_MULTI_LINE_VERT_ALIGNMENT, m_checkExtMultilineVerticalAlignment );
	DDX_Control( pDX, IDC_CHECK_EXT_DT_CALC_EFFECTIVE_RECT, m_checkExtCalcEffectiveRect );
	DDX_Control( pDX, IDC_CHECK_PROF_UIS_DRAW_TEXT, m_checkProfUisText );
	DDX_Control( pDX, IDC_CHECK_WIN32_DRAW_TEXT_COMPARE, m_checkWin32TextCompare );
	DDX_Control( pDX, IDC_COMBO_VERTICAL_ALIGNMENT, m_comboVerticalAlignment );
	DDX_Control( pDX, IDC_COMBO_HORIZONTAL_ALIGNMENT, m_comboHorizontalAlignment );
	DDX_Control( pDX, IDC_COMBO_TYPE_TEXT_FORMAT, m_comboTextFormat );
	DDX_Control( pDX, IDC_COMBO_TYPE_TEXT_ORIENTATION, m_comboTextOrientation );
	DDX_Control( pDX, IDC_STATIC_VISTA_TEXT_RENDERING_OPTIONS, m_groupBoxVistarTextRenderingOptions );
	DDX_Control( pDX, IDC_USE_VISTA_TEXT_APIS, m_checkBoxUseVistaTextAPIs );
	DDX_Control( pDX, IDC_EDIT_VISTA_TEXT_BORDER_SIZE, m_editVistaTextBorderSize );
	DDX_Control( pDX, IDC_SPIN_VISTA_TEXT_BORDER_SIZE, m_spinVistaTextBorderSize );
	DDX_Control( pDX, IDC_BUTTON_VISTA_TEXT_BORDER_COLOR, m_buttonVistaTextBorderColor );
	DDX_Control( pDX, IDC_EDIT_VISTA_TEXT_GLOW_SIZE, m_editVistaTextGlowSize );
	DDX_Control( pDX, IDC_SPIN_VISTA_TEXT_GLOW_SIZE, m_spinVistaTextGlowSize );
	DDX_Control( pDX, IDC_COMBO_VISTA_TEXT_SHADOW_TYPE, m_comboVistaShadowType );
	DDX_Control( pDX, IDC_BUTTON_VISTA_TEXT_SHADOW_COLOR, m_buttonVistaTextShadowColor );
	DDX_Control( pDX, IDC_EDIT_VISTA_SHADOW_OFFSET_X, m_editVistaTextShadowOffsetX );
	DDX_Control( pDX, IDC_SPIN_VISTA_SHADOW_OFFSET_X, m_spinVistaTextShadowOffsetX );
	DDX_Control( pDX, IDC_EDIT_VISTA_SHADOW_OFFSET_Y, m_editVistaTextShadowOffsetY );
	DDX_Control( pDX, IDC_SPIN_VISTA_SHADOW_OFFSET_Y, m_spinVistaTextShadowOffsetY );
	DDX_Control( pDX, IDC_COMBO_BACKGROUND_TYPE, m_comboBackgroundType );
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_TEXT_OUTPUT_BUTTON, m_wndDrawTextSurface );
	DDX_Control( pDX, IDC_EDIT_DRAW_TEXT, m_wndDrawTextEdit );
}

BEGIN_MESSAGE_MAP( CPageDT, CExtResizablePropertyPage )
	//{{AFX_MSG_MAP(CPageDT)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE( IDC_EDIT_DRAW_TEXT, OnChangeEditDrawText )

	ON_CBN_SELENDOK( IDC_COMBO_HORIZONTAL_ALIGNMENT, OnRescanDtFlags )
	ON_CBN_SELENDOK( IDC_COMBO_VERTICAL_ALIGNMENT, OnRescanDtFlags )

	ON_BN_CLICKED( IDC_CHECK_WIN32_DRAW_TEXT_OFFSET, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_EXT_DT_MULTI_LINE_VERT_ALIGNMENT, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_EXT_DT_CALC_EFFECTIVE_RECT, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_PROF_UIS_DRAW_TEXT, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_WIN32_DRAW_TEXT_COMPARE, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_HIDEPREFIX, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_NOPREFIX, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_PREFIXONLY, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_WORDBREAK, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_WORD_ELLIPSIS, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_SINGLE_LINE, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_RTLREADING, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_PATH_ELLIPSIS, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_NOCLIP, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_MODIFYSTRING, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_ENDELLIPSIS, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_CHECK_CALCRECT, OnRescanDtFlags )
	ON_BN_CLICKED( IDC_USE_CUSTOM_RECT, OnRescanDtFlags )
	ON_EN_CHANGE( IDC_EDIT_RECT_LEFT, OnRescanCustomRect )
	ON_EN_CHANGE( IDC_EDIT_RECT_RIGHT, OnRescanCustomRect )
	ON_EN_CHANGE( IDC_EDIT_RECT_TOP, OnRescanCustomRect )
	ON_EN_CHANGE( IDC_EDIT_RECT_BOTTOM, OnRescanCustomRect )
	ON_BN_CLICKED( IDC_BUTTON_GET_CURRENT, OnButtonGetCurrentRect )

	ON_CBN_SELENDOK( IDC_COMBO_TYPE_TEXT_FORMAT, OnRescanDtFlags )
	ON_CBN_SELENDOK( IDC_COMBO_TYPE_TEXT_ORIENTATION, OnRescanDtFlags )

	ON_BN_CLICKED( IDC_USE_VISTA_TEXT_APIS, OnRescanDtFlags )
	ON_EN_CHANGE( IDC_EDIT_VISTA_TEXT_BORDER_SIZE, OnRescanDtFlags )
//	IDC_BUTTON_VISTA_TEXT_BORDER_COLOR
	ON_EN_CHANGE( IDC_EDIT_VISTA_TEXT_GLOW_SIZE, OnRescanDtFlags )
	ON_CBN_SELENDOK( IDC_COMBO_VISTA_TEXT_SHADOW_TYPE, OnRescanDtFlags )
//	IDC_BUTTON_VISTA_TEXT_SHADOW_COLOR
	ON_EN_CHANGE( IDC_EDIT_VISTA_SHADOW_OFFSET_X, OnRescanDtFlags )
	ON_EN_CHANGE( IDC_EDIT_VISTA_SHADOW_OFFSET_Y, OnRescanDtFlags )

	ON_CBN_SELENDOK( IDC_COMBO_BACKGROUND_TYPE, OnRescanDtFlags )

	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
END_MESSAGE_MAP()

BOOL CPageDT::OnInitDialog()
{
	CExtResizablePropertyPage::OnInitDialog();

	m_wndHyperLinkUseCurrentRect.m_bUseStdCommandNotification = true;
	m_spinCustomRectLeft.SetRange( -32000, +32000 );
	m_spinCustomRectTop.SetRange( -32000, +32000 );
	m_spinCustomRectRight.SetRange( -32000, +32000 );
	m_spinCustomRectBottom.SetRange( -32000, +32000 );

	m_comboTextFormat.AddString( _T("Auto-detect") );
	m_comboTextFormat.AddString( _T("Plain text") );
	m_comboTextFormat.AddString( _T("HTML / CSS") );
	m_comboTextFormat.SetCurSel( 1 );

	m_comboTextOrientation.AddString( _T("<html>Normal/default (0&deg;)</html>") );
	m_comboTextOrientation.AddString( _T("<html>90&deg; clockwise</html>") );
	m_comboTextOrientation.AddString( _T("<html>180&deg; clockwise</html>") );
	m_comboTextOrientation.AddString( _T("<html>270&deg; clockwise</html>") );
	m_comboTextOrientation.SetCurSel( 0 );

	m_comboBackgroundType.AddString( _T("<html>Window <i>(Win32)</i></html>") );
	m_comboBackgroundType.AddString( _T("<html>Dialog <i>(Prof-UIS)</i></html>") );
	m_comboBackgroundType.AddString( _T("<html>Tooltip <i>(Win32)</i></html>") );
	m_comboBackgroundType.AddString( _T("<html><div width=2em height=0.7em style=\"background-color:rgb(255,255,255); display:inline-block; vertical-align:middle; border: 1px solid black;\"/> White</html>") );
	m_comboBackgroundType.AddString( _T("<html><div width=2em height=0.7em style=\"background-color:rgb(192,192,192); display:inline-block; vertical-align:middle; border: 1px solid black;\"/> Light grey</html>") );
	m_comboBackgroundType.AddString( _T("<html><div width=2em height=0.7em style=\"background-color:rgb(128,128,128); display:inline-block; vertical-align:middle; border: 1px solid black;\"/> Dark grey</html>") );
	m_comboBackgroundType.AddString( _T("<html><div width=2em height=0.7em style=\"background-color:rgb(0,0,0); display:inline-block; vertical-align:middle; border: 1px solid black;\"/> Black</html>") );
	m_comboBackgroundType.SetCurSel( 0 );

	m_comboHorizontalAlignment.AddString( _T("DT_LEFT") );
	m_comboHorizontalAlignment.AddString( _T("DT_CENTER") );
	m_comboHorizontalAlignment.AddString( _T("DT_RIGHT") );
	m_comboHorizontalAlignment.SetCurSel( 0 );

	m_comboVerticalAlignment.AddString( _T("DT_TOP") );
	m_comboVerticalAlignment.AddString( _T("DT_VCENTER") );
	m_comboVerticalAlignment.AddString( _T("DT_BOTTOM") );
	m_comboVerticalAlignment.SetCurSel( 0 );

	m_groupBoxVistarTextRenderingOptions.SetStyle( CExtGroupBox::STYLE_ROUNDED );

	m_buttonVistaTextBorderColor.m_bEnableBtnColorDefault = false;
	m_buttonVistaTextBorderColor.m_lParamCookie = LPARAM( m_buttonVistaTextBorderColor.GetDlgCtrlID() );
	m_buttonVistaTextBorderColor.m_clrDefault = m_buttonVistaTextBorderColor.m_clrSelected = RGB(128,128,128);
	m_buttonVistaTextShadowColor.m_bEnableBtnColorDefault = false;
	m_buttonVistaTextShadowColor.m_lParamCookie = LPARAM( m_buttonVistaTextShadowColor.GetDlgCtrlID() );
	m_buttonVistaTextShadowColor.m_clrDefault = m_buttonVistaTextShadowColor.m_clrSelected = RGB(128,128,128);

	m_comboVistaShadowType.SetCurSel( 0 );

	m_spinVistaTextBorderSize.SetRange( 0, +32000 );
	m_spinVistaTextGlowSize.SetRange( 0, +32000 );
	m_spinVistaTextShadowOffsetX.SetRange( 0, +32000 );
	m_spinVistaTextShadowOffsetY.SetRange( 0, +32000 );

	AddAnchor( IDC_TEXT_OUTPUT_BUTTON, __RDA_LT, CSize( 100, 50 ) );
	AddAnchor( IDC_EDIT_DRAW_TEXT, CSize( 0, 50 ), __RDA_RB );
	AddAnchor( IDC_STATIC_TYPE_TEXT_HERE, CSize( 0, 50 ) );
	//IDC_STATIC_SEE_TEXT_HERE
	AddAnchor( IDC_STATIC_TYPE_TEXT_FORMAT, CSize( 100, 50 ) );
	AddAnchor( IDC_COMBO_TYPE_TEXT_FORMAT, CSize( 100, 50 ) );
	AddAnchor( IDC_STATIC_TYPE_TEXT_ORIENTATION, CSize( 100, 50 ) );
	AddAnchor( IDC_COMBO_TYPE_TEXT_ORIENTATION, CSize( 100, 50 ) );

	AddAnchor( IDC_STATIC_BACKGROUND_TYPE, __RDA_RT );
	AddAnchor( IDC_COMBO_BACKGROUND_TYPE, __RDA_RT );

	AddAnchor( IDC_CHECK_EXT_DT_MULTI_LINE_VERT_ALIGNMENT, __RDA_RT );
	AddAnchor( IDC_CHECK_EXT_DT_CALC_EFFECTIVE_RECT, __RDA_RT );

	AddAnchor( IDC_CHECK_PROF_UIS_DRAW_TEXT, __RDA_RT );
	AddAnchor( IDC_CHECK_WIN32_DRAW_TEXT_COMPARE, __RDA_RT );
	AddAnchor( IDC_CHECK_WIN32_DRAW_TEXT_OFFSET, __RDA_RT );

	AddAnchor( IDC_COMBO_VERTICAL_ALIGNMENT, __RDA_RT );
	AddAnchor( IDC_COMBO_HORIZONTAL_ALIGNMENT, __RDA_RT );
	AddAnchor( IDC_CHECK_HIDEPREFIX, __RDA_RT );
	AddAnchor( IDC_CHECK_NOPREFIX, __RDA_RT );
	AddAnchor( IDC_CHECK_PREFIXONLY, __RDA_RT );
	AddAnchor( IDC_CHECK_WORDBREAK, __RDA_RT );
	AddAnchor( IDC_CHECK_WORD_ELLIPSIS, __RDA_RT );
	AddAnchor( IDC_CHECK_SINGLE_LINE, __RDA_RT );
	AddAnchor( IDC_CHECK_RTLREADING, __RDA_RT );
	AddAnchor( IDC_CHECK_PATH_ELLIPSIS, __RDA_RT );
	AddAnchor( IDC_CHECK_NOCLIP, __RDA_RT );
	AddAnchor( IDC_CHECK_MODIFYSTRING, __RDA_RT );
	AddAnchor( IDC_CHECK_ENDELLIPSIS, __RDA_RT );
	AddAnchor( IDC_CHECK_CALCRECT, __RDA_RT );
	AddAnchor( IDC_USE_CUSTOM_RECT, __RDA_RT );
	AddAnchor( IDC_BUTTON_GET_CURRENT, __RDA_RT );
	AddAnchor( IDC_STATIC_RECT_L, __RDA_RT );
	AddAnchor( IDC_STATIC_RECT_T, __RDA_RT );
	AddAnchor( IDC_STATIC_RECT_R, __RDA_RT );
	AddAnchor( IDC_STATIC_RECT_B, __RDA_RT );
	AddAnchor( IDC_EDIT_RECT_LEFT, __RDA_RT );
	AddAnchor( IDC_EDIT_RECT_RIGHT, __RDA_RT );
	AddAnchor( IDC_EDIT_RECT_TOP, __RDA_RT );
	AddAnchor( IDC_EDIT_RECT_BOTTOM, __RDA_RT );
	AddAnchor( IDC_SPIN_RECT_LEFT, __RDA_RT );
	AddAnchor( IDC_SPIN_RECT_RIGHT, __RDA_RT );
	AddAnchor( IDC_SPIN_RECT_TOP, __RDA_RT );
	AddAnchor( IDC_SPIN_RECT_BOTTOM, __RDA_RT );

	AddAnchor( IDC_STATIC_VISTA_TEXT_RENDERING_OPTIONS, __RDA_RT );
	AddAnchor( IDC_USE_VISTA_TEXT_APIS, __RDA_RT );
	AddAnchor( IDC_STATIC_VISTA_TEXT_BORDER_SIZE, __RDA_RT );
	AddAnchor( IDC_EDIT_VISTA_TEXT_BORDER_SIZE, __RDA_RT );
	AddAnchor( IDC_SPIN_VISTA_TEXT_BORDER_SIZE, __RDA_RT );
	AddAnchor( IDC_BUTTON_VISTA_TEXT_BORDER_COLOR, __RDA_RT );
	AddAnchor( IDC_STATIC_VISTA_TEXT_GLOW_SIZE, __RDA_RT );
	AddAnchor( IDC_EDIT_VISTA_TEXT_GLOW_SIZE, __RDA_RT );
	AddAnchor( IDC_SPIN_VISTA_TEXT_GLOW_SIZE, __RDA_RT );
	AddAnchor( IDC_STATIC_VISTA_TEXT_SHADOW_TYPE, __RDA_RT );
	AddAnchor( IDC_COMBO_VISTA_TEXT_SHADOW_TYPE, __RDA_RT );
	AddAnchor( IDC_BUTTON_VISTA_TEXT_SHADOW_COLOR, __RDA_RT );
	AddAnchor( IDC_STATIC_VISTA_SHADOW_OFFSET_X, __RDA_RT );
	AddAnchor( IDC_EDIT_VISTA_SHADOW_OFFSET_X, __RDA_RT );
	AddAnchor( IDC_SPIN_VISTA_SHADOW_OFFSET_X, __RDA_RT );
	AddAnchor( IDC_STATIC_VISTA_SHADOW_OFFSET_Y, __RDA_RT );
	AddAnchor( IDC_EDIT_VISTA_SHADOW_OFFSET_Y, __RDA_RT );
	AddAnchor( IDC_SPIN_VISTA_SHADOW_OFFSET_Y, __RDA_RT );

	m_checkProfUisText.SetCheck( 1 );
	m_checkWin32TextCompare.SetCheck( 0 );
	m_checkWin32TextOffset.SetCheck( 1 );
	m_checkWin32TextOffset.EnableWindow( ( m_checkWin32TextCompare.GetCheck() != 0 ) ? TRUE : FALSE );
	
	m_comboVerticalAlignment.SetCurSel(0);
	m_comboHorizontalAlignment.SetCurSel(0);

//	ModifyStyleEx( 0, WS_EX_LAYOUT_RTL, SWP_FRAMECHANGED );

	SubclassChildControls();
	m_bInitComplete = true;
	OnRescanDtFlags();

	m_wndDrawTextEdit.SetWindowText( _T("here is the &sample text...") );
	OnChangeEditDrawText();

int nIndex, nCount;
static struct
{
	UINT m_nID;
	LPCTSTR m_strToolTip;
} g_arrToolTipData[] =
{
	{ IDC_TEXT_OUTPUT_BUTTON,						_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Text output window. Renders text using the <code>CExtRichContentLayout::stat_DrawText()</code> method (Prof-UIS) and/or the <code>DrawText()</code> Win32 API.</p></html>") },
	{ IDC_EDIT_DRAW_TEXT,							_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Type text here <img src=\"smile.bmp\"> ...</p></html>") },
	{ IDC_CHECK_PROF_UIS_DRAW_TEXT,					_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Display output of the <code>CExtRichContentLayout::stat_DrawText()</code> method using black color.</p></html>") },
	{ IDC_CHECK_WIN32_DRAW_TEXT_COMPARE,			_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Display output of the <code>DrawText()</code> Win32 API using red color.</p></html>") },
	{ IDC_CHECK_WIN32_DRAW_TEXT_OFFSET,				_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Offset Win32 output by 1 pixel horizontally and vertically.</p></html>") },
	{ IDC_CHECK_EXT_DT_MULTI_LINE_VERT_ALIGNMENT,	_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Enables vertical alignment without <code>DT_SINGLELINE</code> flag.</p></html>") },
	{ IDC_CHECK_EXT_DT_CALC_EFFECTIVE_RECT,			_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Used with <code>DT_CALCRECT</code> flag. Computes effective text rectangle where the text should be painted.</p></html>") },
	{ IDC_CHECK_SINGLE_LINE,						_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Displays text on a single line only. Carriage returns and linefeeds do not break the line.</p></html>") },
	{ IDC_CHECK_WORDBREAK,							_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Breaks words. <code>DrawText</code> automatically breaks lines between words if a word would extend past the edge of the rectangle specified by the lpRect parameter. A carriage return-linefeed sequence also breaks the line.</p></html>") },
	{ IDC_CHECK_NOPREFIX,							_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Turns off processing of prefix characters. Normally, <code>DrawText</code> interprets the mnemonic-prefix character &amp; as a directive to underscore the character that follows, and the mnemonic-prefix characters &amp;&amp; as a directive to print a single &amp;. By specifying <code>DT_NOPREFIX</code>, this processing is turned off.</p></html>") },
	{ IDC_CHECK_HIDEPREFIX,							_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Ignores the ampersand (&amp;) prefix character in the text. The letter that follows will not be underlined, but other mnemonic-prefix characters are still processed.</p></html>") },
	{ IDC_CHECK_PREFIXONLY,							_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Draws only an underline at the position of the character following the ampersand (&amp;) prefix character. Does not draw any character in the string.</p></html>") },
	{ IDC_CHECK_RTLREADING,							_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Layout in right-to-left reading order for bi-directional text when the font selected into the hdc is a Hebrew or Arabic font. The default reading order for all text is left-to-right.</p></html>") },
	{ IDC_CHECK_CALCRECT,							_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Determines the width and height of the rectangle. <code>DrawText</code> returns the height of the formatted text, but does not draw the text.</p></html>") },
	{ IDC_CHECK_MODIFYSTRING,						_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Modifies the specified string to match the displayed text. This value has no effect unless <code>DT_END_ELLIPSIS</code> or <code>DT_PATH_ELLIPSIS</code> is specified.</p></html>") },
	{ IDC_CHECK_NOCLIP,								_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Draws without clipping.</p></html>") },
	{ IDC_CHECK_ENDELLIPSIS,						_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Truncates a text string that is wider than the display rectangle and adds an ellipsis to indicate the truncation.</p></html>") },
	{ IDC_CHECK_PATH_ELLIPSIS,						_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>For displayed text, replaces characters in the middle of the string with ellipses so that the result fits in the specified rectangle. If the string contains backslash (\\) characters, <code>DT_PATH_ELLIPSIS</code> preserves as much as possible of the text after the last backslash.</p></html>") },
	{ IDC_CHECK_WORD_ELLIPSIS,						_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Truncates any word that does not fit in the display rectangle and adds an ellipsis.</p></html>") },
	{ IDC_USE_CUSTOM_RECT,							_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Allows to specify the rectangle in which you want to format the text.</p></html>") },
	{ IDC_BUTTON_GET_CURRENT,						_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Gets current rectangle of the text output window.</p></html>") },
	{ IDC_EDIT_RECT_LEFT,							_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip><code>RECT::left</code> field.</p></html>") },
	{ IDC_EDIT_RECT_RIGHT,							_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip><code>RECT::right</code> field.</p></html>") },
	{ IDC_EDIT_RECT_TOP,							_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip><code>RECT::top</code> field.</p></html>") },
	{ IDC_EDIT_RECT_BOTTOM,							_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip><code>RECT::bottom</code> field.</p></html>") },
	{ IDC_COMBO_HORIZONTAL_ALIGNMENT,				_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Horizontal text alignment.</p></html>") },
	{ IDC_COMBO_VERTICAL_ALIGNMENT,					_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Vertical text alignment.</p></html>") },
	{ IDC_COMBO_TYPE_TEXT_FORMAT,					_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Specifies text format. If you choose <i>Auto-detect</i> here, then the HTML format is auto detected successfully by the following first 5 characters in the entered text:</p><p><big><big><big><code>&lt;HTML</code></big></big></big></p></html>") },
	{ IDC_COMBO_TYPE_TEXT_ORIENTATION,				_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Specifies text orientation.</p></html>") },
	{ IDC_USE_VISTA_TEXT_APIS,						_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Use text rendering APIs from <code>UxTheme.DLL</code>. This feature is available on Windows Vista or later. It's supported only for 0&deg; rotated text because of <code>UxTheme.DLL</code> implementation limitations.</p></html>") },
	{ IDC_EDIT_VISTA_TEXT_BORDER_SIZE,				_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Specifies size of text boder in pixels.</p></html>") },
	{ IDC_BUTTON_VISTA_TEXT_BORDER_COLOR,			_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Specifies text boder color.</p></html>") },
	{ IDC_EDIT_VISTA_TEXT_GLOW_SIZE,				_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Specifies text glow size. The glow color is white and cannot be customized. Please use non-white background to test it.</p></html>") },
	{ IDC_COMBO_VISTA_TEXT_SHADOW_TYPE,				_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Specifies type of text shadow.</p></html>") },
	{ IDC_BUTTON_VISTA_TEXT_SHADOW_COLOR,			_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Specifies text shadow color.</p></html>") },
	{ IDC_EDIT_VISTA_SHADOW_OFFSET_X,				_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Horizontal shadow offset in pixels.</p></html>") },
	{ IDC_EDIT_VISTA_SHADOW_OFFSET_Y,				_T("<html><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Vertical shadow offset in pixels.</p></html>") },
	{ IDC_COMBO_BACKGROUND_TYPE,					_T("<html width=300><link rel=\"stylesheet\" type=\"text/css\" href=\"sample123.css\"><p class=tooltip>Specifies type of background used by the text output window.</p></html>") },
};
	nCount = int( sizeof(g_arrToolTipData) / sizeof(g_arrToolTipData[0]) ) ;
	for( nIndex = 0; nIndex < nCount; nIndex ++ )
	{
		CWnd * pDlgItem = GetDlgItem( g_arrToolTipData[nIndex].m_nID );
		ASSERT( pDlgItem != NULL );
		CExtButton * pBtn = DYNAMIC_DOWNCAST( CExtButton, pDlgItem );
		if( pBtn != NULL )
		{
			pBtn->SetTooltipText( g_arrToolTipData[nIndex].m_strToolTip );
			continue;
		}
		CExtComboBox * pComboBox = DYNAMIC_DOWNCAST( CExtComboBox, pDlgItem );
		if( pComboBox != NULL )
		{
			pComboBox->SetTooltipText( g_arrToolTipData[nIndex].m_strToolTip );
			continue;
		}
		CExtEdit * pEdit = DYNAMIC_DOWNCAST( CExtEdit, pDlgItem );
		if( pEdit != NULL )
		{
			pEdit->SetTooltipText( g_arrToolTipData[nIndex].m_strToolTip );
			continue;
		}
	}

	return FALSE;
}


void CPageDT::OnButtonGetCurrentRect()
{
	if( ! m_bInitComplete )
		return;

bool bUseCustomRect = ( m_checkUseCustomRect.GetCheck() != 0 ) ? true : false;
	if( bUseCustomRect )
	{
		m_bInitComplete = false;
		m_wndDrawTextSurface.GetClientRect( &m_wndDrawTextSurface.m_rcCustomRect );
		CString s;
		s.Format( _T("%d"), m_wndDrawTextSurface.m_rcCustomRect.left );
		SetDlgItemText( IDC_EDIT_RECT_LEFT, LPCTSTR(s) );
		s.Format( _T("%d"), m_wndDrawTextSurface.m_rcCustomRect.top );
		SetDlgItemText( IDC_EDIT_RECT_TOP, LPCTSTR(s) );
		s.Format( _T("%d"), m_wndDrawTextSurface.m_rcCustomRect.right );
		SetDlgItemText( IDC_EDIT_RECT_RIGHT, LPCTSTR(s) );
		s.Format( _T("%d"), m_wndDrawTextSurface.m_rcCustomRect.bottom );
		SetDlgItemText( IDC_EDIT_RECT_BOTTOM, LPCTSTR(s) );
		m_bInitComplete = true;
	}
	OnRescanDtFlags();
}

void CPageDT::OnRescanCustomRect()
{
	if( ! m_bInitComplete )
		return;

bool bUseCustomRect = ( m_checkUseCustomRect.GetCheck() != 0 ) ? true : false;
	if( bUseCustomRect )
	{
		m_bInitComplete = false;
		CString s;
		GetDlgItemText( IDC_EDIT_RECT_LEFT, s );
		m_wndDrawTextSurface.m_rcCustomRect.left = ::_ttoi( LPCTSTR(s) );
		GetDlgItemText( IDC_EDIT_RECT_TOP, s );
		m_wndDrawTextSurface.m_rcCustomRect.top = ::_ttoi( LPCTSTR(s) );
		GetDlgItemText( IDC_EDIT_RECT_RIGHT, s );
		m_wndDrawTextSurface.m_rcCustomRect.right = ::_ttoi( LPCTSTR(s) );
		GetDlgItemText( IDC_EDIT_RECT_BOTTOM, s );
		m_wndDrawTextSurface.m_rcCustomRect.bottom = ::_ttoi( LPCTSTR(s) );
		m_bInitComplete = true;
	}
	OnRescanDtFlags();
}

void CPageDT::OnRescanDtFlags()
{
	if( ! m_bInitComplete )
		return;

UINT arrCustomRectFeatureIDs[] =
{
IDC_BUTTON_GET_CURRENT,
IDC_STATIC_RECT_L,
IDC_STATIC_RECT_T,
IDC_STATIC_RECT_R,
IDC_STATIC_RECT_B,
IDC_EDIT_RECT_LEFT,
IDC_EDIT_RECT_RIGHT,
IDC_EDIT_RECT_TOP,
IDC_EDIT_RECT_BOTTOM,
IDC_SPIN_RECT_LEFT,
IDC_SPIN_RECT_RIGHT,
IDC_SPIN_RECT_TOP,
IDC_SPIN_RECT_BOTTOM,
};
bool bUseCustomRect = ( m_checkUseCustomRect.GetCheck() != 0 ) ? true : false;
int i;
	for( i = 0; i < ( sizeof(arrCustomRectFeatureIDs) / sizeof(arrCustomRectFeatureIDs[0]) ); i++ )
		GetDlgItem( arrCustomRectFeatureIDs[i] ) -> EnableWindow( bUseCustomRect ? TRUE : FALSE );

CExtComboBox * pCombo = NULL;
CExtCheckBox * pCheck = NULL;
INT nVal;

	m_wndDrawTextSurface.m_nDTF = 0;
	m_wndDrawTextSurface.m_nExtDTF = 0;

	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_EXT_DT_MULTI_LINE_VERT_ALIGNMENT ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nExtDTF |= __EXT_DT_MULTI_LINE_VERT_ALIGNMENT;

	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_EXT_DT_CALC_EFFECTIVE_RECT ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nExtDTF |= __EXT_DT_CALC_EFFECTIVE_RECT;

	pCombo = STATIC_DOWNCAST( CExtComboBox, GetDlgItem( IDC_COMBO_HORIZONTAL_ALIGNMENT ) );
	nVal = pCombo->GetCurSel();
	switch( nVal )
	{
	case 0: m_wndDrawTextSurface.m_nDTF |= DT_LEFT; break;
	case 1: m_wndDrawTextSurface.m_nDTF |= DT_CENTER; break;
	case 2: m_wndDrawTextSurface.m_nDTF |= DT_RIGHT; break;
	}

	pCombo = STATIC_DOWNCAST( CExtComboBox, GetDlgItem( IDC_COMBO_VERTICAL_ALIGNMENT ) );
	nVal = pCombo->GetCurSel();
	switch( nVal )
	{
	case 0: m_wndDrawTextSurface.m_nDTF |= DT_TOP; break;
	case 1: m_wndDrawTextSurface.m_nDTF |= DT_VCENTER; break;
	case 2: m_wndDrawTextSurface.m_nDTF |= DT_BOTTOM; break;
	}

	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_HIDEPREFIX ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_HIDEPREFIX;
	
	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_NOPREFIX ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_NOPREFIX;
	
	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_PREFIXONLY ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_PREFIXONLY;

	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_WORDBREAK ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_WORDBREAK;
	
	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_WORD_ELLIPSIS ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_WORD_ELLIPSIS;

// 	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_TABSTOP ) );
// 	if( pCheck->GetCheck() != 0 )
// 		m_wndDrawTextSurface.m_nDTF |= DT_TABSTOP;
	
	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_SINGLE_LINE ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_SINGLELINE;
	
	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_RTLREADING ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_RTLREADING;
	
	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_PATH_ELLIPSIS ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_PATH_ELLIPSIS;
	
	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_NOCLIP ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_NOCLIP;
	
	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_MODIFYSTRING ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_MODIFYSTRING;
	
// 	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_INTERNAL ) );
// 	if( pCheck->GetCheck() != 0 )
// 		m_wndDrawTextSurface.m_nDTF |= DT_INTERNAL;
	
// 	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_EXTERNALLEADING ) );
// 	if( pCheck->GetCheck() != 0 )
// 		m_wndDrawTextSurface.m_nDTF |= DT_EXTERNALLEADING;
	
// 	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_EXPANDTABS ) );
// 	if( pCheck->GetCheck() != 0 )
// 		m_wndDrawTextSurface.m_nDTF |= DT_EXPANDTABS;
	
	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_ENDELLIPSIS ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_END_ELLIPSIS;
	
// 	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_EDITCONTROL ) );
// 	if( pCheck->GetCheck() != 0 )
// 		m_wndDrawTextSurface.m_nDTF |= DT_EDITCONTROL;
	
	pCheck = STATIC_DOWNCAST( CExtCheckBox, GetDlgItem( IDC_CHECK_CALCRECT ) );
	if( pCheck->GetCheck() != 0 )
		m_wndDrawTextSurface.m_nDTF |= DT_CALCRECT;

	m_checkWin32TextOffset.EnableWindow( ( m_checkWin32TextCompare.GetCheck() != 0 ) ? TRUE : FALSE );

	m_eUserLO = (CExtRichContentLayout::e_layout_orientation_t)m_comboTextOrientation.GetCurSel();
	if( m_eUserLO == 0 )
		m_eUserLO = CExtRichContentLayout::__ELOT_NORMAL;
	else if( m_eUserLO == 1 )
		m_eUserLO = CExtRichContentLayout::__ELOT_90_CW;
	else if( m_eUserLO == 2 )
		m_eUserLO = CExtRichContentLayout::__ELOT_180_CW;
	else if( m_eUserLO == 3 )
		m_eUserLO = CExtRichContentLayout::__ELOT_270_CW;

UINT arrVistaTextAPIs[] =
{
IDC_USE_VISTA_TEXT_APIS,
IDC_STATIC_VISTA_TEXT_BORDER_SIZE,
IDC_EDIT_VISTA_TEXT_BORDER_SIZE,
IDC_SPIN_VISTA_TEXT_BORDER_SIZE,
IDC_BUTTON_VISTA_TEXT_BORDER_COLOR,
IDC_STATIC_VISTA_TEXT_GLOW_SIZE,
IDC_EDIT_VISTA_TEXT_GLOW_SIZE,
IDC_SPIN_VISTA_TEXT_GLOW_SIZE,
IDC_STATIC_VISTA_TEXT_SHADOW_TYPE,
IDC_COMBO_VISTA_TEXT_SHADOW_TYPE,
IDC_BUTTON_VISTA_TEXT_SHADOW_COLOR,
IDC_STATIC_VISTA_SHADOW_OFFSET_X,
IDC_EDIT_VISTA_SHADOW_OFFSET_X,
IDC_SPIN_VISTA_SHADOW_OFFSET_X,
IDC_STATIC_VISTA_SHADOW_OFFSET_Y,
IDC_EDIT_VISTA_SHADOW_OFFSET_Y,
IDC_SPIN_VISTA_SHADOW_OFFSET_Y,
};
	if( g_PaintManager.m_bIsWinVistaOrLater )
	{
		m_groupBoxVistarTextRenderingOptions.EnableWindow( TRUE );
		m_checkBoxUseVistaTextAPIs.EnableWindow( TRUE );
		bool bUseVistaTextAPIs = ( m_checkBoxUseVistaTextAPIs.GetCheck() != 0 ) ? true : false;
		for( i = 1; i < ( sizeof(arrVistaTextAPIs) / sizeof(arrVistaTextAPIs[0]) ); i++ ) // from 1, excluding m_checkBoxUseVistaTextAPIs
			GetDlgItem( arrVistaTextAPIs[i] ) -> EnableWindow( bUseVistaTextAPIs ? TRUE : FALSE );
		if( bUseVistaTextAPIs )
		{
			CString s;

			m_editVistaTextGlowSize.GetWindowText( s );
			m_DTO.iGlowSize = ::_ttoi( LPCTSTR(s) );
			if( m_DTO.iGlowSize > 0 )
				m_DTO.dwFlags |= DWORD(__EXT_UX_DTT_GLOWSIZE);
			else
				m_DTO.dwFlags &= ~(DWORD(__EXT_UX_DTT_GLOWSIZE));

			m_editVistaTextBorderSize.GetWindowText( s );
			m_DTO.iBorderSize = ::_ttoi( LPCTSTR(s) );
			if( m_DTO.iBorderSize > 0 )
				m_DTO.dwFlags |= DWORD(__EXT_UX_DTT_BORDERSIZE);
			else
				m_DTO.dwFlags &= ~(DWORD(__EXT_UX_DTT_BORDERSIZE));

			m_DTO.crBorder = m_buttonVistaTextBorderColor.m_clrSelected;
			if( m_DTO.crBorder != COLORREF(-1L) )
				m_DTO.dwFlags |= DWORD(__EXT_UX_DTT_BORDERCOLOR);
			else
				m_DTO.dwFlags &= ~(DWORD(__EXT_UX_DTT_BORDERCOLOR));

			m_editVistaTextShadowOffsetX.GetWindowText( s );
			m_DTO.ptShadowOffset.x = ::_ttoi( LPCTSTR(s) );
			m_editVistaTextShadowOffsetY.GetWindowText( s );
			m_DTO.ptShadowOffset.y = ::_ttoi( LPCTSTR(s) );
			if( m_DTO.ptShadowOffset.x > 0 || m_DTO.ptShadowOffset.y )
				m_DTO.dwFlags |= DWORD(__EXT_UX_DTT_SHADOWOFFSET);
			else
				m_DTO.dwFlags &= ~(DWORD(__EXT_UX_DTT_SHADOWOFFSET));

			m_DTO.crShadow = m_buttonVistaTextShadowColor.m_clrSelected;
			if( m_DTO.crShadow != COLORREF(-1L) )
				m_DTO.dwFlags |= DWORD(__EXT_UX_DTT_SHADOWCOLOR);
			else
				m_DTO.dwFlags &= ~(DWORD(__EXT_UX_DTT_SHADOWCOLOR));

			INT nShadowTypeCurSel = m_comboVistaShadowType.GetCurSel();
			m_DTO.iTextShadowType = TST_NONE;
			m_DTO.dwFlags &= ~(DWORD(__EXT_UX_DTT_SHADOWTYPE));
			if( nShadowTypeCurSel == 1 )
			{
				m_DTO.iTextShadowType = TST_SINGLE;
				m_DTO.dwFlags |= DWORD(__EXT_UX_DTT_SHADOWTYPE);
			}
			else if( nShadowTypeCurSel == 2 )
			{
				m_DTO.iTextShadowType = TST_CONTINUOUS;
				m_DTO.dwFlags |= DWORD(__EXT_UX_DTT_SHADOWTYPE);
			}
		} // if( bUseVistaTextAPIs )
	} // if( g_PaintManager.m_bIsWinVistaOrLater )
	else
	{
		m_groupBoxVistarTextRenderingOptions.EnableWindow( FALSE );
		for( i = 0; i < ( sizeof(arrVistaTextAPIs) / sizeof(arrVistaTextAPIs[0]) ); i++ )
			GetDlgItem( arrVistaTextAPIs[i] ) -> EnableWindow( FALSE );
	} // else from if( g_PaintManager.m_bIsWinVistaOrLater )

	m_wndDrawTextSurface.Invalidate();
	m_wndDrawTextSurface.UpdateWindow();
}


LRESULT CPageDT::OnColorChangedFinally( WPARAM wParam, LPARAM lParam )
{
	wParam;
	switch( lParam )
	{
	case IDC_BUTTON_VISTA_TEXT_BORDER_COLOR:
	case IDC_BUTTON_VISTA_TEXT_SHADOW_COLOR:
		OnRescanDtFlags();
	break;
	} // switch( lParam )
	return 0;
}

void CPageDT::OnChangeEditDrawText() 
{
	m_wndDrawTextSurface.Invalidate();
}


/////////////////////////////////////////////////////////////////////////////
// CPageAC window

CPageAC::CPageAC()
	: CExtResizablePropertyPage( CPageAC::IDD )
//	, m_shellTree( true )
{
	//{{AFX_DATA_INIT(CPageAC)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
}

CPageAC::~CPageAC()
{
}

void CPageAC::DoDataExchange( CDataExchange * pDX )
{
	CExtResizablePropertyPage::DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CPageAC)
	DDX_Control( pDX, IDC_COMBO_TYPE_SCROLL_BARS, m_comboScollBarsType );
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_CUSTOM1, m_wndRCS );
}

BEGIN_MESSAGE_MAP( CPageAC, CExtResizablePropertyPage )
	//{{AFX_MSG_MAP(CPageAC)
	ON_CBN_SELENDOK( IDC_COMBO_TYPE_SCROLL_BARS, OnSelEndOkScrollBarsType )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPageAC::OnInitDialog()
{
	CExtResizablePropertyPage::OnInitDialog();

	m_comboScollBarsType.SetItemData( m_comboScollBarsType.AddString( _T("<html>classic</html>") ), INT(CExtRichStyleDescription::e_ui_sbt_classic) );
	m_comboScollBarsType.SetItemData( m_comboScollBarsType.AddString( _T("<html>pop-over <i>(light)</i></html>") ), INT(CExtRichStyleDescription::e_ui_sbt_pop_over_light) );
	m_comboScollBarsType.SetItemData( m_comboScollBarsType.AddString( _T("<html>pop-over <i>(full)</i></html>") ), INT(CExtRichStyleDescription::e_ui_sbt_pop_over_full) );
	m_comboScollBarsType.SetItemData( m_comboScollBarsType.AddString( _T("<html>hidden</html>") ), INT(CExtRichStyleDescription::e_ui_sbt_none) );
	m_comboScollBarsType.SetCurSel( 0 );

HWND hWnd = NULL;
CFont * pDefGuiFont = CFont::FromHandle( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) );
	m_wndRCS.SetFont( pDefGuiFont );
	m_wndRCS.ModifyStyle( 0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN );
	m_wndRCS.ModifyStyleEx( 0, WS_EX_CONTROLPARENT );
CExtRichContentLayout * pRCL = m_wndRCS.RcsLayoutGet();
	ASSERT( pRCL != NULL );

	VERIFY( m_buttonPush.Create( _T("Push Button"), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP|BS_PUSHBUTTON, CFrameWnd::rectDefault, &m_wndRCS, 100 ) );
		m_buttonPush.SetFont( pDefGuiFont );
		pRCL->AttachHwndToElementID( _T("push_button"), m_buttonPush.m_hWnd );

	VERIFY( m_edit.Create( WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL, CFrameWnd::rectDefault, &m_wndRCS, 101 ) );
		m_edit.SetFont( pDefGuiFont );
		m_edit.SetCueBanner( _T("type text here") );
		pRCL->AttachHwndToElementID( _T("text_editor"), m_edit.m_hWnd );

	hWnd =
		::CreateWindowEx(
			0,
#if (defined _UNICODE)
				L"RichEdit20W",
#else
				"RichEdit20A",
#endif
			_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL, 0,0,0,0, m_wndRCS.m_hWnd, (HMENU)102, ::AfxGetInstanceHandle(), 0L
			);
		ASSERT( hWnd != NULL );
		m_editSystemNumber.m_bRichMode = true;
		VERIFY( m_editSystemNumber.SubclassWindow( hWnd ) );
		m_editSystemNumber.NumberColorSet( RGB(164,64,64) );
		m_editSystemNumber.FractionColorSet( RGB(96,96,192) );
		m_editSystemNumber.SignColorSet( RGB(255,64,64) );
		m_editSystemNumber.BracketsColorSet( RGB(64,255,64) );
		m_editSystemNumber.GroupSeparatorColorSet( RGB(164,164,164) );
		m_editSystemNumber.DecimalSeparatorColorSet( RGB(64,255,192) );
		m_editSystemNumber.SetFont( pDefGuiFont );
		pRCL->AttachHwndToElementID( _T("system_number_editor"), m_editSystemNumber.m_hWnd );

	hWnd =
		::CreateWindowEx(
			0,
#if (defined _UNICODE)
				L"RichEdit20W",
#else
				"RichEdit20A",
#endif
			_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP|WS_BORDER|ES_AUTOHSCROLL, 0,0,0,0, m_wndRCS.m_hWnd, (HMENU)103, ::AfxGetInstanceHandle(), 0L
			);
		ASSERT( hWnd != NULL );
		m_editSystemCurrency.m_bRichMode = true;
		VERIFY( m_editSystemCurrency.SubclassWindow( hWnd ) );
		m_editSystemCurrency.NumberColorSet( RGB(164,64,64) );
		m_editSystemCurrency.FractionColorSet( RGB(96,96,192) );
		m_editSystemCurrency.SignColorSet( RGB(255,64,64) );
		m_editSystemCurrency.BracketsColorSet( RGB(64,255,64) );
		m_editSystemCurrency.GroupSeparatorColorSet( RGB(164,164,164) );
		m_editSystemCurrency.DecimalSeparatorColorSet( RGB(64,255,192) );
		m_editSystemCurrency.SetFont( pDefGuiFont );
		pRCL->AttachHwndToElementID( _T("system_currency_editor"), m_editSystemCurrency.m_hWnd );

// 	VERIFY( m_editDateTime.Create( &m_wndRCS, CFrameWnd::rectDefault, 104 ) );
// 		m_editDateTime.SetFont( pDefGuiFont );
// 		pRCL->AttachHwndToElementID( _T("date_time_editor"), m_editDateTime.m_hWnd );

// 	VERIFY( m_editDuration.Create( &m_wndRCS, CFrameWnd::rectDefault, 105 ) );
// 		m_editDuration.SetFont( pDefGuiFont );
// 		pRCL->AttachHwndToElementID( _T("duration_editor"), m_editDuration.m_hWnd );

// 	VERIFY( m_editLL.Create( &m_wndRCS, CFrameWnd::rectDefault, 106 ) );
// 		m_editLL.SetFont( pDefGuiFont );
// 		pRCL->AttachHwndToElementID( _T("geo_location_editor"), m_editLL.m_hWnd );

	VERIFY( m_buttonColor.Create( _T(""), WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_TABSTOP|WS_CLIPCHILDREN|BS_PUSHBUTTON, CFrameWnd::rectDefault, &m_wndRCS, 107 ) );
		m_buttonColor.SetFont( pDefGuiFont );
		pRCL->AttachHwndToElementID( _T("color_button"), m_buttonColor.m_hWnd );

	VERIFY( m_comboBox.Create( WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP|CBS_DROPDOWNLIST, CRect(0,0,0,400), &m_wndRCS, 108 ) );
		m_comboBox.SetFont( pDefGuiFont );
		m_comboBox.AddString( _T("1") );
		m_comboBox.AddString( _T("2") );
		m_comboBox.AddString( _T("3") );
		m_comboBox.SetCurSel( 0 );
		pRCL->AttachHwndToElementID( _T("combo_box"), m_comboBox.m_hWnd );

// 	VERIFY( m_imageEditor.Create( &m_wndRCS, __EIEDWS_DEFAULT_STYLES, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_BORDER, 0, 109 ) );
// 		m_imageEditor.SetFont( pDefGuiFont );
// 		VERIFY( m_imageEditor.IedBitmapLoadResource( IDB_GRADIENT ) );
// 		pRCL->AttachHwndToElementID( _T("image_editor"), m_imageEditor.m_hWnd );

// 	VERIFY( m_shellTree.Create( WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP|WS_BORDER|TVS_HASBUTTONS|TVS_HASLINES|TVS_INFOTIP|TVS_NONEVENHEIGHT|TVS_SHOWSELALWAYS|TVS_TRACKSELECT, CFrameWnd::rectDefault, &m_wndRCS, 110  ) );
// 		m_shellTree.SetFont( pDefGuiFont );
// 		m_shellTree.RefreshShellRoot();
// 		pRCL->AttachHwndToElementID( _T("shell_tree"), m_shellTree.m_hWnd );

USES_CONVERSION;
CExtResourceManager::CExtResourceMemoryBuffer buffer;
	VERIFY( g_ResourceManager->LoadResourceBuffer( buffer, MAKEINTRESOURCE(RT_HTML), ID_HTML_AC ) );
DWORD dwBufferSize = buffer.GetSize();
	ASSERT( dwBufferSize > 0 );
	ASSERT( buffer.GetBuffer() != NULL );
CString str;
LPTSTR pBuffer = str.GetBuffer( dwBufferSize + 2 );
	ASSERT( pBuffer != NULL );
	::memset( pBuffer, 0, ( dwBufferSize + 2 ) * sizeof(TCHAR) );
	__EXT_MFC_STRNCPY( pBuffer, dwBufferSize + 1, A2CT(LPSTR(buffer.GetBuffer())), dwBufferSize );
	str.ReleaseBuffer();

	m_wndRCS.RcsTextSet( str );
	AddAnchor( IDC_CUSTOM1, __RDA_LT, __RDA_RB );
	AddAnchor( IDC_STATIC_TYPE_SCROLL_BARS, __RDA_LB );
	AddAnchor( IDC_COMBO_TYPE_SCROLL_BARS, __RDA_LB );

	return FALSE;
}

void CPageAC::OnSelEndOkScrollBarsType()
{
int nCurSel = int( m_comboScollBarsType.GetCurSel() );
	if( nCurSel < 0 )
		return;
CExtRichStyleDescription::e_ui_sb_type_t _eUiSBT = (CExtRichStyleDescription::e_ui_sb_type_t)m_comboScollBarsType.GetItemData( nCurSel );
	if( _eUiSBT == CExtRichStyleDescription::e_ui_sbt_pop_over_light )
	{
		m_wndRCS.m_bHideNormalScrollBars = true;
		m_wndRCS.m_bDisplayPopOverScrollBars = true;
		m_wndRCS.m_bLightPopOverScrollBars = true;
	}
	else if( _eUiSBT == CExtRichStyleDescription::e_ui_sbt_pop_over_full )
	{
		m_wndRCS.m_bHideNormalScrollBars = true;
		m_wndRCS.m_bDisplayPopOverScrollBars = true;
		m_wndRCS.m_bLightPopOverScrollBars = false;
	}
	else if( _eUiSBT == CExtRichStyleDescription::e_ui_sbt_none )
	{
		m_wndRCS.m_bHideNormalScrollBars = true;
		m_wndRCS.m_bDisplayPopOverScrollBars = false;
	}
	else
	{	// else switch to classic scroll bars
		m_wndRCS.m_bHideNormalScrollBars = false;
		m_wndRCS.m_bDisplayPopOverScrollBars = false;
	}

// 		CExtRichContentLayout * pRCL = m_wndRCS.RcsLayoutGet();
// 			ASSERT( pRCL != NULL );
// 		//	pRCL->_DoLayout_ListOfCSS_Rebuild();
// 		CExtRichContentItem * pRootRCI = ( pRCL->m_listHeap.GetCount() > 0 ) ? pRCL->m_listHeap.GetHead() : NULL;
// 			if( pRootRCI != NULL )
// 				pRootRCI->ResetStyleInfo();
	m_wndRCS.RcsDelayRecalcLayout();
	m_wndRCS.OnSwRecalcLayout( true );
	m_wndRCS.OnSwUpdateScrollBars();
	m_wndRCS.OnSwInvalidate( true );

// 	m_imageEditor.m_bHideNormalScrollBars      = m_wndRCS.m_bHideNormalScrollBars;
// 	m_imageEditor.m_bDisplayPopOverScrollBars = m_wndRCS.m_bDisplayPopOverScrollBars;
// 	m_imageEditor.m_bLightPopOverScrollBars   = m_wndRCS.m_bLightPopOverScrollBars;
// 	m_imageEditor.OnSwRecalcLayout( true );
// 	m_imageEditor.OnSwUpdateScrollBars();
// 	m_imageEditor.OnSwInvalidate( true );
}



