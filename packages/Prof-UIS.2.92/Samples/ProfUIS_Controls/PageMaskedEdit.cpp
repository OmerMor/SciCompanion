// PageMaskedEdit.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageMaskedEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if (!defined __EXT_MFC_NO_GRIDWND )

/////////////////////////////////////////////////////////////////////////////
// CPageMaskedEdit dialog

CPageMaskedEdit::CPageMaskedEdit(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageMaskedEdit::IDD, pParent)
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageMaskedEdit)
	//}}AFX_DATA_INIT
	m_wndRuleGrid.m_pEditWithBehaviorWnd = &m_editTextMaskedRich;
	SetAutoSubclassChildControls();
}

void CPageMaskedEdit::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMaskedEdit)
	DDX_Control(pDX, IDC_CHECK_MODE_NULLABLE, m_checkModeNullable);
	DDX_Control(pDX, IDC_CHECK_MODE_INSERT, m_checkModeInsert);
	DDX_Control(pDX, IDC_CHECK_MODE_FIELD_SELECTION, m_checkModeFieldSelection);
	DDX_Control(pDX, IDC_MASKED_TEXT_EDIT_SIMPLE, m_editTextMaskedSimple);
	DDX_Control(pDX, IDC_MASKED_TEXT_EDIT_RICH, m_editTextMaskedRich);
	DDX_Control(pDX, IDC_CHECK_USE_RICH_EDIT, m_checkUseMaskedRichEdit);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RULE_GRID, m_wndRuleGrid);
}

BEGIN_MESSAGE_MAP(CPageMaskedEdit, CPageBase)
	//{{AFX_MSG_MAP(CPageMaskedEdit)
	ON_BN_CLICKED(IDC_CHECK_USE_RICH_EDIT, OnUseMaskedRichEdit)
	ON_BN_CLICKED(ID_CHAR_RULE_ADD, OnCharRuleAdd)
	ON_BN_CLICKED(ID_CHAR_RULE_REMOVE, OnCharRuleRemove)
	ON_EN_CHANGE(IDC_EDIT_MASK, OnChangeEditMask)
	ON_BN_CLICKED(ID_CHAR_RULE_RESET_ALL, OnCharRuleResetAll)
	ON_BN_CLICKED(IDC_CHECK_MODE_FIELD_SELECTION, OnOptionChanged)
	ON_BN_CLICKED(IDC_CHECK_MODE_INSERT, OnOptionChanged)
	ON_BN_CLICKED(IDC_CHECK_MODE_NULLABLE, OnOptionChanged)
	ON_EN_CHANGE(IDC_EDIT_NULL_CHAR, OnOptionChanged)
	ON_EN_SETFOCUS(IDC_EDIT_NULL_CHAR, OnSetfocusEditNullChar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMaskedEdit message handlers

BOOL CPageMaskedEdit::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	
static TCHAR g_strInitMask[] = _T("($)$$ z$$$$-$$$$ ($$y$$)");
	GetDlgItem(IDC_EDIT_MASK)->SetWindowText( g_strInitMask );
	m_editTextMaskedRich.MaskSet( LPCTSTR(g_strInitMask) );
	m_editTextMaskedSimple.MaskSet( LPCTSTR(g_strInitMask) );

	m_checkModeNullable.SetCheck( m_editTextMaskedRich.NullableModeGet() ? 1 : 0 );
	m_checkModeInsert.SetCheck( m_editTextMaskedRich.InsertModeGet() ? 1 : 0 );
	m_checkModeFieldSelection.SetCheck( m_editTextMaskedRich.FieldSelectionModeGet() ? 1 : 0 );
TCHAR strNullChar[2] = { m_editTextMaskedRich.NullCharGet(), _T('\0') };
	GetDlgItem( IDC_EDIT_NULL_CHAR )->SetWindowText( strNullChar );
	((CEdit*)GetDlgItem( IDC_EDIT_NULL_CHAR ))->SetSel( 0, -1 );

	AddAnchor( IDC_MASKED_TEXT_EDIT_SIMPLE, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_MASKED_TEXT_EDIT_RICH, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_EDIT_MASK, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_RULE_GRID, __RDA_LT, __RDA_RB );
	AddAnchor( ID_CHAR_RULE_ADD, __RDA_RT );
	AddAnchor( ID_CHAR_RULE_REMOVE, __RDA_RT );
	AddAnchor( ID_CHAR_RULE_RESET_ALL, __RDA_RT );
	AddAnchor( IDC_STATIC_MASKED_EDIT_OPTIONS, __RDA_LB );
	AddAnchor( IDC_STATIC_MASKED_EDIT_NULL_CHAR, __RDA_LB );
	AddAnchor( IDC_CHECK_MODE_INSERT, __RDA_LB );
	AddAnchor( IDC_CHECK_MODE_NULLABLE, __RDA_LB );
	AddAnchor( IDC_CHECK_MODE_FIELD_SELECTION, __RDA_LB );
	AddAnchor( IDC_EDIT_NULL_CHAR, __RDA_LB );
	m_wndRuleGrid.RuleResetAll();
	m_checkUseMaskedRichEdit.SetCheck( 1 );
	m_bInitComplete = true;
	OnUseMaskedRichEdit();
	return TRUE;
}

void CPageMaskedEdit::OnCharRuleAdd() 
{
CExtEditWithBehavior::CBehaviorMasked::CharRule _cr( RGB(144,144,144), _T('?'), NULL, NULL );
	m_wndRuleGrid.RuleAdd( true, _cr, NULL, NULL );
}

void CPageMaskedEdit::OnCharRuleRemove() 
{
	m_wndRuleGrid.RuleRemove();
}

void CPageMaskedEdit::OnChangeEditMask() 
{
CString strMask;
	GetDlgItem(IDC_EDIT_MASK)->GetWindowText( strMask );
	m_editTextMaskedRich.MaskSet( LPCTSTR(strMask) );
	m_editTextMaskedSimple.MaskSet( LPCTSTR(strMask) );
}

void CPageMaskedEdit::OnCharRuleResetAll() 
{
	m_wndRuleGrid.RuleResetAll();
}

void CPageMaskedEdit::OnOptionChanged() 
{
	m_editTextMaskedRich  .NullableModeSet( m_checkModeNullable.GetCheck() ? true : false );
	m_editTextMaskedSimple.NullableModeSet( m_checkModeNullable.GetCheck() ? true : false );
	m_editTextMaskedRich  .InsertModeSet( m_checkModeInsert.GetCheck() ? true : false );
	m_editTextMaskedSimple.InsertModeSet( m_checkModeInsert.GetCheck() ? true : false );
	m_editTextMaskedRich  .FieldSelectionModeSet( m_checkModeFieldSelection.GetCheck() ? true : false );
	m_editTextMaskedSimple.FieldSelectionModeSet( m_checkModeFieldSelection.GetCheck() ? true : false );
CString strNullChar1, strNullChar2;
	GetDlgItem( IDC_EDIT_NULL_CHAR )->GetWindowText( strNullChar1 );
	strNullChar2 = strNullChar1;
	strNullChar2.TrimLeft( _T(" \r\n\t") );
	strNullChar2.TrimRight( _T(" \r\n\t") );
	if( strNullChar2.GetLength() != 1 )
		strNullChar2 = _T("_");
	if( strNullChar1 != strNullChar2 )
		GetDlgItem( IDC_EDIT_NULL_CHAR )->SetWindowText( strNullChar2 );
	m_editTextMaskedRich  .NullCharSet( strNullChar2.GetAt( 0 ) );
	m_editTextMaskedSimple.NullCharSet( strNullChar2.GetAt( 0 ) );
	m_editTextMaskedRich  .SetWindowText( _T("") );
	m_editTextMaskedSimple.SetWindowText( _T("") );
	m_editTextMaskedRich  .OnCheckText( true );
	m_editTextMaskedSimple.OnCheckText( true );
	((CEdit*)GetDlgItem( IDC_EDIT_NULL_CHAR ))->SetSel( 0, -1 );
}

void CPageMaskedEdit::OnSetfocusEditNullChar() 
{
	((CEdit*)GetDlgItem( IDC_EDIT_NULL_CHAR ))->SetSel( 0, -1 );
}

void CPageMaskedEdit::OnUseMaskedRichEdit()
{
int nStart, nEnd;
CString str;
	if( m_checkUseMaskedRichEdit.GetCheck() != 0 )
	{
		m_editTextMaskedRich.ShowWindow( SW_SHOW );
		m_editTextMaskedSimple.ShowWindow( SW_HIDE );
		m_wndRuleGrid.m_pEditWithBehaviorWnd = &m_editTextMaskedRich;
		m_wndRuleGrid.OnRuleSetChanged();
		m_editTextMaskedSimple.GetWindowText( str );
		m_editTextMaskedRich  .SetWindowText( LPCTSTR(str) );
		m_editTextMaskedSimple.GetSel( nStart, nEnd );
		m_editTextMaskedRich  .SetSel( nStart, nEnd );
	}
	else
	{
		m_editTextMaskedRich.ShowWindow( SW_HIDE );
		m_editTextMaskedSimple.ShowWindow( SW_SHOW );
		m_wndRuleGrid.m_pEditWithBehaviorWnd = &m_editTextMaskedSimple;
		m_wndRuleGrid.OnRuleSetChanged();
		m_editTextMaskedRich  .GetWindowText( str );
		m_editTextMaskedSimple.SetWindowText( LPCTSTR(str) );
		m_editTextMaskedRich  .GetSel( nStart, nEnd );
		m_editTextMaskedSimple.SetSel( nStart, nEnd );
	}
}

#endif // (!defined __EXT_MFC_NO_GRIDWND )

/////////////////////////////////////////////////////////////////////////////
// CPageSystemNumberCurrencyEditors dialog

CPageSystemNumberCurrencyEditors::CPageSystemNumberCurrencyEditors(CWnd* pParent /*=NULL*/)
	: CPageBase( CPageSystemNumberCurrencyEditors::IDD, pParent )
	, m_bInitComplete( false )
{
	//{{AFX_DATA_INIT(CPageSystemNumberCurrencyEditors)
	//}}AFX_DATA_INIT
	SetAutoSubclassChildControls();
	m_listColorButtonIDs.AddTail( IDC_BUTTON_COLOR_NUMBER );
	m_listColorButtonIDs.AddTail( IDC_BUTTON_COLOR_FRACTION );
	m_listColorButtonIDs.AddTail( IDC_BUTTON_COLOR_SIGN );
	m_listColorButtonIDs.AddTail( IDC_BUTTON_COLOR_BRACKETS );
	m_listColorButtonIDs.AddTail( IDC_BUTTON_COLOR_SEPARATOR_GROUP );
	m_listColorButtonIDs.AddTail( IDC_BUTTON_COLOR_SEPARATOR_DECIMAL );
	m_listColorButtonIDs.AddTail( IDC_BUTTON_COLOR_DEFAULT_TEXT );
	m_listColorButtonIDs.AddTail( IDC_BUTTON_COLOR_DEFAULT_BACK );
	m_listColorButtonIDs.AddTail( IDC_BUTTON_COLOR_POWER );
	m_listColorButtonIDs.AddTail( IDC_BUTTON_COLOR_SCIENTIFIC_SIGN );
}

void CPageSystemNumberCurrencyEditors::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSystemNumberCurrencyEditors)
	DDX_Control(pDX, IDC_BUTTON_COLOR_POWER, m_buttonColorPower);
	DDX_Control(pDX, IDC_BUTTON_COLOR_SCIENTIFIC_SIGN, m_buttonColorScientificSign);
	DDX_Control(pDX, IDC_EDIT_SCIENTIFIC_NUMBER_SIMPLE, m_editScientificNumberSimple);
	DDX_Control(pDX, IDC_EDIT_SCIENTIFIC_NUMBER_RICH, m_editScientificNumberRich );
	DDX_Control(pDX, IDC_CHECK_USE_RICH_EDIT, m_checkUseRichEditors);
	DDX_Control(pDX, IDC_EDIT_SYSTEM_NUMBER_SIMPLE, m_editSystemNumberSimple);
	DDX_Control(pDX, IDC_EDIT_SYSTEM_NUMBER_RICH, m_editSystemNumberRich);
	DDX_Control(pDX, IDC_STATIC_VALUE_OF_SYSTEM_NUMBER, m_labelValueOfSystemNumberEditor);
	DDX_Control(pDX, IDC_EDIT_SYSTEM_CURRENCY_SIMPLE, m_editSystemCurrencySimple);
	DDX_Control(pDX, IDC_EDIT_SYSTEM_CURRENCY_RICH, m_editSystemCurrencyRich);
	DDX_Control(pDX, IDC_STATIC_VALUE_OF_SYSTEM_CURRENCY, m_labelValueOfSystemCurrencyEditor);
	DDX_Control(pDX, IDC_EDIT_MAX_WHOLE_DIGITS, m_editMaxWholeDigits);
	DDX_Control(pDX, IDC_SPIN_MAX_WHOLE_DIGITS, m_spinMaxWholeDigits);
	DDX_Control(pDX, IDC_EDIT_MAX_FRACTION_DIGITS, m_editMaxFractionDigits);
	DDX_Control(pDX, IDC_SPIN_MAX_FRACTION_DIGITS, m_spinMaxFractionDigits);
	DDX_Control(pDX, IDC_CHECK_CAN_BE_NEGATIVE, m_checkCanBeNegative);
	DDX_Control(pDX, IDC_BUTTON_COLOR_NUMBER, m_buttonColorNumber);
	DDX_Control(pDX, IDC_BUTTON_COLOR_FRACTION, m_buttonColorFraction);
	DDX_Control(pDX, IDC_BUTTON_COLOR_SIGN, m_buttonColorSign);
	DDX_Control(pDX, IDC_BUTTON_COLOR_BRACKETS, m_buttonColorBrackets);
	DDX_Control(pDX, IDC_BUTTON_COLOR_SEPARATOR_GROUP, m_buttonColorSeparatorGroup);
	DDX_Control(pDX, IDC_BUTTON_COLOR_SEPARATOR_DECIMAL, m_buttonColorSeparatorDecimal);
	DDX_Control(pDX, IDC_BUTTON_COLOR_DEFAULT_TEXT, m_buttonDefaultText);
	DDX_Control(pDX, IDC_BUTTON_COLOR_DEFAULT_BACK, m_buttonDefaultBack);
	DDX_Control(pDX, IDC_CHECK_NO_GROUP_SEPARATORS, m_checkNoGroupSeparators);
	//}}AFX_DATA_MAP
//	DDX_Text( pDX, IDC_EDIT_MAX_WHOLE_DIGITS, m_nTestNumberWholeDigits );
//	DDX_Text( pDX, IDC_EDIT_MAX_FRACTION_DIGITS, m_nTestNumberFractionDigits );
}

BEGIN_MESSAGE_MAP(CPageSystemNumberCurrencyEditors, CPageBase)
	//{{AFX_MSG_MAP(CPageSystemNumberCurrencyEditors)
	ON_BN_CLICKED(IDC_CHECK_USE_RICH_EDIT, OnCheckUseRichEditors)
	ON_EN_CHANGE(IDC_EDIT_SYSTEM_NUMBER_SIMPLE, OnChangeEditSystemNumberSimple)
	ON_EN_CHANGE(IDC_EDIT_SYSTEM_NUMBER_RICH, OnChangeEditSystemNumberRich)
	ON_EN_CHANGE(IDC_EDIT_SYSTEM_CURRENCY_SIMPLE, OnChangeEditSystemCurrencySimple)
	ON_EN_CHANGE(IDC_EDIT_SYSTEM_CURRENCY_RICH, OnChangeEditSystemCurrencyRich)
	ON_EN_CHANGE(IDC_EDIT_MAX_WHOLE_DIGITS, OnChangeMaxWholeDigits)
	ON_EN_CHANGE(IDC_EDIT_MAX_FRACTION_DIGITS, OnChangeMaxFractionDigits)
	ON_BN_CLICKED(IDC_CHECK_CAN_BE_NEGATIVE, OnCheckCanBeNegative)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_WINDOWS_LOCALE_SETTINGS, OnChangeWindowsLocaleSettings)
	ON_BN_CLICKED(IDC_CHECK_NO_GROUP_SEPARATORS, OnCheckNoGroupSeparators)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyCustColor,
		OnColorSelectCustom
		)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSystemNumberCurrencyEditors message handlers

BOOL CPageSystemNumberCurrencyEditors::OnInitDialog() 
{
	CPageBase::OnInitDialog();
POSITION pos;
	for( pos = m_listColorButtonIDs.GetHeadPosition(); pos != NULL; )
	{
		UINT nID = m_listColorButtonIDs.GetNext( pos );
		CExtColorButton * pColorButton = DYNAMIC_DOWNCAST( CExtColorButton, GetDlgItem( nID ) );
		ASSERT_VALID( pColorButton );
		pColorButton->m_clrDefault = COLORREF(-1L);
		pColorButton->m_lParamCookie = LPARAM(nID);
	}
	m_buttonColorNumber.m_clrSelected = RGB(0,0,164);
	m_buttonColorFraction.m_clrSelected = RGB(0,164,0);
	m_buttonColorSign.m_clrSelected = RGB(164,0,0);
	m_buttonColorBrackets.m_clrSelected = RGB(128,128,128);
	m_buttonColorSeparatorGroup.m_clrSelected = RGB(164,164,164);
	m_buttonColorSeparatorDecimal.m_clrSelected = RGB(164,0,0);
	m_buttonColorPower.m_clrSelected = RGB(255,0,255);
	m_buttonColorScientificSign.m_clrSelected = RGB(0,255,255);
	m_buttonDefaultText.m_clrSelected = RGB(0,0,0);
	m_buttonDefaultBack.m_clrSelected = RGB(255,255,255);
	m_checkUseRichEditors.SetCheck( 1 );
	m_checkCanBeNegative.SetCheck( 1 );
	m_spinMaxWholeDigits.SetRange( 1, 15 );
	m_spinMaxFractionDigits.SetRange( 0, 20 );
CString str;
	str.Format( _T("%d"), m_editSystemNumberSimple.MaxWholeDigitsGet() );
	m_editMaxWholeDigits.SetWindowText( LPCTSTR(str) );
	str.Format( _T("%d"), m_editSystemNumberSimple.MaxFractionDigitsGet() );
	m_editMaxFractionDigits.SetWindowText( LPCTSTR(str) );
	m_editScientificNumberSimple.TextSet( _T("123.5e678901") );
	AddAnchor( IDC_EDIT_SYSTEM_NUMBER_SIMPLE, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_EDIT_SYSTEM_NUMBER_RICH, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_EDIT_SYSTEM_CURRENCY_SIMPLE, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_EDIT_SYSTEM_CURRENCY_RICH, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_EDIT_SCIENTIFIC_NUMBER_SIMPLE, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_EDIT_SCIENTIFIC_NUMBER_RICH, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_STATIC_VALUE_OF_SYSTEM_NUMBER, __RDA_RT );
	AddAnchor( IDC_STATIC_VALUE_OF_SYSTEM_CURRENCY, __RDA_RT );
	AddAnchor( IDC_STATIC_ETCHED_LINE_00, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_STATIC_ETCHED_LINE_01, __RDA_LT, __RDA_RT );
	AddAnchor( IDC_STATIC_ETCHED_LINE_02, __RDA_LT, __RDA_RT );
	m_bInitComplete = true;
	OnCheckUseRichEditors();
	OnCheckCanBeNegative();
	OnChangeMaxWholeDigits();
	OnChangeMaxFractionDigits();
	for( pos = m_listColorButtonIDs.GetHeadPosition(); pos != NULL; )
	{
		UINT nID = m_listColorButtonIDs.GetNext( pos );
		CExtColorButton * pColorButton = DYNAMIC_DOWNCAST( CExtColorButton, GetDlgItem( nID ) );
		ASSERT_VALID( pColorButton );
		OnColorChangedFinally( (WPARAM)pColorButton->m_clrSelected, (LPARAM)pColorButton->m_lParamCookie );
	}
	_SyncValueOfNumber();
	_SyncValueOfCurrency();
	return TRUE;
}

void CPageSystemNumberCurrencyEditors::OnCheckUseRichEditors()
{
	if( ! m_bInitComplete )
		return;
int nStart, nEnd;
CString str;
bool bUseRichEditors = ( m_checkUseRichEditors.GetCheck() != 0 ) ? true : false;
	if( bUseRichEditors )
	{
		m_editSystemNumberRich.ShowWindow( SW_SHOW );
		m_editSystemNumberSimple.ShowWindow( SW_HIDE );
		m_editSystemNumberSimple.GetWindowText( str );
		m_editSystemNumberRich  .SetWindowText( LPCTSTR(str) );
		m_editSystemNumberSimple.GetSel( nStart, nEnd );
		m_editSystemNumberRich  .SetSel( nStart, nEnd );

		m_editSystemCurrencyRich.ShowWindow( SW_SHOW );
		m_editSystemCurrencySimple.ShowWindow( SW_HIDE );
		m_editSystemCurrencySimple.GetWindowText( str );
		m_editSystemCurrencyRich  .SetWindowText( LPCTSTR(str) );
		m_editSystemCurrencySimple.GetSel( nStart, nEnd );
		m_editSystemCurrencyRich  .SetSel( nStart, nEnd );
		
		m_editScientificNumberRich.ShowWindow( SW_SHOW );
		m_editScientificNumberSimple.ShowWindow( SW_HIDE );
		m_editScientificNumberSimple.GetWindowText( str );
		m_editScientificNumberRich  .SetWindowText( LPCTSTR(str) );
		m_editScientificNumberSimple.GetSel( nStart, nEnd );
		m_editScientificNumberRich  .SetSel( nStart, nEnd );
	}
	else
	{
		m_editSystemNumberRich.ShowWindow( SW_HIDE );
		m_editSystemNumberSimple.ShowWindow( SW_SHOW );
		m_editSystemNumberRich  .GetWindowText( str );
		m_editSystemNumberSimple.SetWindowText( LPCTSTR(str) );
		m_editSystemNumberRich  .GetSel( nStart, nEnd );
		m_editSystemNumberSimple.SetSel( nStart, nEnd );

		m_editSystemCurrencyRich.ShowWindow( SW_HIDE );
		m_editSystemCurrencySimple.ShowWindow( SW_SHOW );
		m_editSystemCurrencyRich  .GetWindowText( str );
		m_editSystemCurrencySimple.SetWindowText( LPCTSTR(str) );
		m_editSystemCurrencyRich  .GetSel( nStart, nEnd );
		m_editSystemCurrencySimple.SetSel( nStart, nEnd );

		m_editScientificNumberRich.ShowWindow( SW_HIDE );
		m_editScientificNumberSimple.ShowWindow( SW_SHOW );
		m_editScientificNumberRich  .GetWindowText( str );
		m_editScientificNumberSimple.SetWindowText( LPCTSTR(str) );
		m_editScientificNumberRich  .GetSel( nStart, nEnd );
		m_editScientificNumberSimple.SetSel( nStart, nEnd );
	}
POSITION pos;
	for( pos = m_listColorButtonIDs.GetHeadPosition(); pos != NULL; )
	{
		UINT nID = m_listColorButtonIDs.GetNext( pos );
		CExtColorButton * pColorButton = DYNAMIC_DOWNCAST( CExtColorButton, GetDlgItem( nID ) );
		ASSERT_VALID( pColorButton );
		pColorButton->EnableWindow( bUseRichEditors ? TRUE : FALSE );
	}
}

void CPageSystemNumberCurrencyEditors::_SyncFrom( CExtEditSystemNumberCurrencyBase * pWndEditSyncFrom )
{
	if( m_editSystemNumberSimple.m_bUseScientificNumberMode )
		return;
	ASSERT_VALID( pWndEditSyncFrom );
	ASSERT( pWndEditSyncFrom->GetSafeHwnd() != NULL );
	if( ! m_bInitComplete )
		return;
	m_bInitComplete = false;
CExtEditSystemNumberCurrencyBase * arrEditors[ 4 ] =
		{
			&m_editSystemNumberSimple,
			&m_editSystemNumberRich,
			&m_editSystemCurrencySimple,
			&m_editSystemCurrencyRich,
		};
long double lfVal = pWndEditSyncFrom->LongDoubleGet();
INT nIndex, nCount = INT( sizeof(arrEditors) / sizeof(arrEditors[0]) );
	for( nIndex = 0; nIndex < nCount; nIndex ++ )
	{
		CExtEditSystemNumberCurrencyBase * pWndEdit = arrEditors[ nIndex ];
		ASSERT_VALID( pWndEdit );
		ASSERT( pWndEdit->GetSafeHwnd() != NULL );
		if( LPVOID(pWndEdit) == LPVOID(pWndEditSyncFrom) )
			continue;
		pWndEdit->LongDoubleSet( lfVal );
	}
	m_bInitComplete = true;
	_SyncValueOfNumber();
	_SyncValueOfCurrency();
}

void CPageSystemNumberCurrencyEditors::_SyncValueOfNumber()
{
	if( m_editSystemNumberSimple.m_bUseScientificNumberMode )
		return;
	ASSERT( m_editSystemNumberSimple.GetSafeHwnd() != NULL );
	ASSERT( m_editSystemNumberRich.GetSafeHwnd() != NULL );
	ASSERT( m_labelValueOfSystemNumberEditor.GetSafeHwnd() != NULL );
CExtEditSystemNumber & wndEdit =
		( ( m_editSystemNumberSimple.GetStyle() & WS_VISIBLE ) != 0 )
			? m_editSystemNumberSimple : m_editSystemNumberRich;
long double lfVal = wndEdit.LongDoubleGet();
CString strText;
	strText.Format( _T("%le"), lfVal );
	m_labelValueOfSystemNumberEditor.SetWindowText( LPCTSTR(strText) );
}

void CPageSystemNumberCurrencyEditors::_SyncValueOfCurrency()
{
	if( m_editSystemNumberSimple.m_bUseScientificNumberMode )
		return;
	ASSERT( m_editSystemCurrencySimple.GetSafeHwnd() != NULL );
	ASSERT( m_editSystemCurrencyRich.GetSafeHwnd() != NULL );
	ASSERT( m_labelValueOfSystemCurrencyEditor.GetSafeHwnd() != NULL );
CExtEditSystemCurrency & wndEdit =
		( ( m_editSystemCurrencySimple.GetStyle() & WS_VISIBLE ) != 0 )
			? m_editSystemCurrencySimple : m_editSystemCurrencyRich;
long double lfVal = wndEdit.LongDoubleGet();
CString strText;
	strText.Format( _T("%e"), lfVal );
	m_labelValueOfSystemCurrencyEditor.SetWindowText( LPCTSTR(strText) );
}

void CPageSystemNumberCurrencyEditors::OnChangeEditSystemNumberSimple()
{
	_SyncFrom( &m_editSystemNumberSimple );
}

void CPageSystemNumberCurrencyEditors::OnChangeEditSystemNumberRich()
{
	_SyncFrom( &m_editSystemNumberRich );
}

void CPageSystemNumberCurrencyEditors::OnChangeEditSystemCurrencySimple()
{
	_SyncFrom( &m_editSystemCurrencySimple );
}

void CPageSystemNumberCurrencyEditors::OnChangeEditSystemCurrencyRich()
{
	_SyncFrom( &m_editSystemCurrencyRich );
}

void CPageSystemNumberCurrencyEditors::OnChangeMaxWholeDigits()
{
	if( ! m_bInitComplete )
		return;
CString str;
	m_editMaxWholeDigits.GetWindowText( str );
INT nVal = (INT)_ttol( LPCTSTR(str) );
INT nValSrc = nVal;
	nVal = max( nVal, 1 );
	nVal = min( nVal, 15 );
	if( nValSrc != nVal )
	{
		str.Format( _T("%d"), nVal );
		m_editMaxWholeDigits.SetWindowText( LPCTSTR(str) );
		m_editMaxWholeDigits.SetSel( 0, -1 );
	}
	m_editSystemNumberSimple	.MaxWholeDigitsSet( nVal );
	m_editSystemNumberRich		.MaxWholeDigitsSet( nVal );
	m_editSystemCurrencySimple	.MaxWholeDigitsSet( nVal );
	m_editSystemCurrencyRich	.MaxWholeDigitsSet( nVal );
	m_editScientificNumberSimple.MaxWholeDigitsSet( nVal );
	m_editScientificNumberRich  .MaxWholeDigitsSet( nVal );
}

void CPageSystemNumberCurrencyEditors::OnChangeMaxFractionDigits()
{
	if( ! m_bInitComplete )
		return;
CString str;
	m_editMaxFractionDigits.GetWindowText( str );
INT nVal = (INT)_ttol( LPCTSTR(str) );
INT nValSrc = nVal;
	nVal = max( nVal, 0 );
	nVal = min( nVal, 20 );
	if( nValSrc != nVal )
	{
		str.Format( _T("%d"), nVal );
		m_editMaxFractionDigits.SetWindowText( LPCTSTR(str) );
		m_editMaxFractionDigits.SetSel( 0, -1 );
	}
	m_editSystemNumberSimple	.MaxFractionDigitsSet( nVal );
	m_editSystemNumberRich		.MaxFractionDigitsSet( nVal );
	m_editSystemCurrencySimple	.MaxFractionDigitsSet( nVal );
	m_editSystemCurrencyRich	.MaxFractionDigitsSet( nVal );
	m_editScientificNumberSimple.MaxFractionDigitsSet( nVal );
	m_editScientificNumberRich  .MaxFractionDigitsSet( nVal );
}

void CPageSystemNumberCurrencyEditors::OnCheckCanBeNegative()
{
	if( ! m_bInitComplete )
		return;
bool bAllowNegative = ( m_checkCanBeNegative.GetCheck() != 0 ) ? true : false;
	m_editSystemNumberSimple	.AllowNegativeSet( bAllowNegative );
	m_editSystemNumberRich		.AllowNegativeSet( bAllowNegative );
	m_editSystemCurrencySimple	.AllowNegativeSet( bAllowNegative );
	m_editSystemCurrencyRich	.AllowNegativeSet( bAllowNegative );
	m_editScientificNumberSimple.AllowNegativeSet( bAllowNegative );
	m_editScientificNumberRich  .AllowNegativeSet( bAllowNegative );
}

LRESULT CPageSystemNumberCurrencyEditors::OnColorChangedFinally( WPARAM wParam, LPARAM lParam )
{
UINT nID = UINT(lParam);
	if( m_listColorButtonIDs.Find( nID ) == NULL )
		return 0L;
COLORREF clr = COLORREF(wParam);
	switch( nID )
	{
	case IDC_BUTTON_COLOR_NUMBER:
		m_editSystemNumberRich		.NumberColorSet( clr );
		m_editSystemCurrencyRich	.NumberColorSet( clr );
		m_editScientificNumberRich	.NumberColorSet( clr );
	break;
	case IDC_BUTTON_COLOR_FRACTION:
		m_editSystemNumberRich		.FractionColorSet( clr );
		m_editSystemCurrencyRich	.FractionColorSet( clr );
		m_editScientificNumberRich	.FractionColorSet( clr );
	break;
	case IDC_BUTTON_COLOR_SIGN:
		m_editSystemNumberRich		.SignColorSet( clr );
		m_editSystemCurrencyRich	.SignColorSet( clr );
		m_editScientificNumberRich	.SignColorSet( clr );
	break;
	case IDC_BUTTON_COLOR_BRACKETS:
		m_editSystemNumberRich		.BracketsColorSet( clr );
		m_editSystemCurrencyRich	.BracketsColorSet( clr );
		m_editScientificNumberRich	.BracketsColorSet( clr );
	break;
	case IDC_BUTTON_COLOR_SEPARATOR_GROUP:
		m_editSystemNumberRich		.GroupSeparatorColorSet( clr );
		m_editSystemCurrencyRich	.GroupSeparatorColorSet( clr );
		m_editScientificNumberRich	.GroupSeparatorColorSet( clr );
	break;
	case IDC_BUTTON_COLOR_SEPARATOR_DECIMAL:
		m_editSystemNumberRich		.DecimalSeparatorColorSet( clr );
		m_editSystemCurrencyRich	.DecimalSeparatorColorSet( clr );
		m_editScientificNumberRich	.DecimalSeparatorColorSet( clr );
	break;
	case IDC_BUTTON_COLOR_DEFAULT_TEXT:
		m_editSystemNumberRich		.m_clrRichTextNormal = clr;
		m_editSystemCurrencyRich	.m_clrRichTextNormal = clr;
		m_editScientificNumberRich	.m_clrRichTextNormal = clr;
		m_editSystemNumberRich		._Rich_OnDraw();
		m_editSystemCurrencyRich	._Rich_OnDraw();
		m_editScientificNumberRich	._Rich_OnDraw();
	break;
	case IDC_BUTTON_COLOR_DEFAULT_BACK:
		m_editSystemNumberRich		.m_clrRichBackNormal = clr;
		m_editSystemCurrencyRich	.m_clrRichBackNormal = clr;
		m_editScientificNumberRich	.m_clrRichBackNormal = clr;
		m_editSystemNumberRich		._Rich_OnDraw();
		m_editSystemCurrencyRich	._Rich_OnDraw();
		m_editScientificNumberRich	._Rich_OnDraw();
	break;
	case IDC_BUTTON_COLOR_POWER:
		m_editScientificNumberRich.PowerColorSet( clr );
	break;
	case IDC_BUTTON_COLOR_SCIENTIFIC_SIGN:
		m_editScientificNumberRich.ScientificSignColorSet( clr );
	break;
	} // switch( nID )
	return 0L;
}

LRESULT CPageSystemNumberCurrencyEditors::OnColorSelectCustom( WPARAM wParam, LPARAM lParam )
{
	wParam;
UINT nID = UINT(lParam);
	if( m_listColorButtonIDs.Find( nID ) == NULL )
		return 0L;
CExtColorDlg dlgColor;
	dlgColor.m_strSection = _T("ColorDialog");
	dlgColor.m_strEntryDialogPosition = _T("Position");
	dlgColor.m_strEntryColorModelSelection = _T("ColorModel");
	switch( nID )
	{
	case IDC_BUTTON_COLOR_NUMBER:
		dlgColor.m_strCaption = _T("Number Color");
	break;
	case IDC_BUTTON_COLOR_FRACTION:
		dlgColor.m_strCaption = _T("Fraction Color");
	break;
	case IDC_BUTTON_COLOR_SIGN:
		dlgColor.m_strCaption = _T("Sign Color");
	break;
	case IDC_BUTTON_COLOR_BRACKETS:
		dlgColor.m_strCaption = _T("Brackets Color");
	break;
	case IDC_BUTTON_COLOR_SEPARATOR_GROUP:
		dlgColor.m_strCaption = _T("Group Separator Color");
	break;
	case IDC_BUTTON_COLOR_SEPARATOR_DECIMAL:
		dlgColor.m_strCaption = _T("Decimal Separator Color");
	break;
	case IDC_BUTTON_COLOR_DEFAULT_TEXT:
		dlgColor.m_strCaption = _T("Default Text Color");
	break;
	case IDC_BUTTON_COLOR_DEFAULT_BACK:
		dlgColor.m_strCaption = _T("Default Background Color");
	break;
	case IDC_BUTTON_COLOR_POWER:
		dlgColor.m_strCaption = _T("Power Text Color");
	break;
	case IDC_BUTTON_COLOR_SCIENTIFIC_SIGN:
		dlgColor.m_strCaption = _T("Scientific Sign Color");
	break;
	} // switch( nID )
	if( dlgColor.DoModal() != IDOK )
		return 0;
	OnColorChangedFinally( (WPARAM)dlgColor.m_clrNew, lParam );
	return 0L;
}

void CPageSystemNumberCurrencyEditors::OnChangeWindowsLocaleSettings()
{
	::ShellExecute(
		::GetDesktopWindow(), // GetParent()->GetSafeHwnd(),
		g_PaintManager.m_bIsWin2000orLater ? LPCTSTR(NULL) : _T("run"),
		_T("intl.cpl"),
		NULL,
		NULL,
		SW_SHOW
		);
}

void CPageSystemNumberCurrencyEditors::OnCheckNoGroupSeparators()
{
	m_editSystemNumberSimple.m_bEmptyGroupSeparatorMode
		= m_editSystemNumberRich.m_bEmptyGroupSeparatorMode
		= m_editSystemCurrencySimple.m_bEmptyGroupSeparatorMode
		= m_editSystemCurrencyRich.m_bEmptyGroupSeparatorMode
		= m_editScientificNumberSimple.m_bEmptyGroupSeparatorMode
		= m_editScientificNumberRich.m_bEmptyGroupSeparatorMode
		= ( m_checkNoGroupSeparators.GetCheck() != 0 ) ? true : false;
}

