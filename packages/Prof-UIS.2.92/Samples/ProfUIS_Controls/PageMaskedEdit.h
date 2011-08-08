#if !defined(__PAGE_MASKED_EDIT_H)
#define __PAGE_MASKED_EDIT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageMaskedEdit.h : header file
//

#include "PageBase.h"

#if (!defined __EXT_MFC_NO_GRIDWND )

/////////////////////////////////////////////////////////////////////////////
// CPageMaskedEdit dialog

class CPageMaskedEdit : public CPageBase 
{
public:
	CPageMaskedEdit(CWnd* pParent = NULL);   // standard constructor

	//{{AFX_DATA(CPageMaskedEdit)
	enum { IDD = IDD_PAGE_MASKED_EDIT };
	CExtCheckBox	m_checkModeNullable;
	CExtCheckBox	m_checkModeInsert;
	CExtCheckBox	m_checkModeFieldSelection;
	CExtEditMasked	m_editTextMaskedSimple;
	CExtEditMasked	m_editTextMaskedRich;
	CExtCheckBox	m_checkUseMaskedRichEdit;
	//}}AFX_DATA

	class CRuleGridWnd : public CExtNSB < CExtGridWnd >
	{
	public:
		CExtEditMasked * m_pEditWithBehaviorWnd;
		CRuleGridWnd()
			: m_pEditWithBehaviorWnd( NULL )
		{
		}
	protected:
		virtual void PreSubclassWindow()
		{
			CExtNSB < CExtGridWnd > :: PreSubclassWindow();
			SiwModifyStyle( __ESIS_STH_NONE|__ESIS_STV_PIXEL|__EGBS_SFB_CELLS|__EGBS_GRIDLINES|__EGBS_NO_HIDE_SELECTION
				|__EGBS_DYNAMIC_RESIZING_H|__EGBS_RESIZING_CELLS_INNER_H|__EGBS_RESIZING_CELLS_OUTER_H, 0, false );
			SiwModifyStyleEx( __EGWS_EX_PM_COLORS|__EGBS_EX_CELL_TOOLTIPS_OUTER|__EGBS_EX_CELL_EXPANDING_INNER, 0, false );
			BseModifyStyle( __EGWS_BSE_EDIT_CELLS_INNER, 0, false );
			BseModifyStyleEx( __EGBS_BSE_EX_PROPORTIONAL_COLUMN_WIDTHS|__EGBS_BSE_EX_EAL_BOTTOM_V, 0, false );
			OuterRowCountTopSet( 1L, false );
			ColumnAdd( 5L, false );
			CExtGridCell * pCell = NULL;
			// header cell 1
			pCell = GridCellGetOuterAtTop( 0L, 0L, RUNTIME_CLASS(CExtGridCellHeader) );
			pCell->TextSet( _T("Rule status") );
			pCell->ExtentPercentSet( 0.25 );
			// header cell 2
			pCell = GridCellGetOuterAtTop( 1L, 0L, RUNTIME_CLASS(CExtGridCellHeader) );
			pCell->TextSet( _T("One character") );
			pCell->ExtentPercentSet( 0.05 );
			// header cell 3
			pCell = GridCellGetOuterAtTop( 2L, 0L, RUNTIME_CLASS(CExtGridCellHeader) );
			pCell->TextSet( _T("Validation Function") );
			pCell->ExtentPercentSet( 0.35 );
			// header cell 4
			pCell = GridCellGetOuterAtTop( 3L, 0L, RUNTIME_CLASS(CExtGridCellHeader) );
			pCell->TextSet( _T("Conversion Function") );
			pCell->ExtentPercentSet( 0.35 );
			// header cell 5
			pCell = GridCellGetOuterAtTop( 4L, 0L, RUNTIME_CLASS(CExtGridCellHeader) );
			pCell->TextSet( _T("Color (rich edit mode only)") );
			pCell->ExtentPercentSet( 0.35 );
			// finish init
			OnSwUpdateScrollBars();
		}
	public:
		virtual void OnRuleSetChanged()
		{
			ASSERT_VALID( this );
			if( m_pEditWithBehaviorWnd->GetSafeHwnd() == NULL )
				return;
			ASSERT_VALID( m_pEditWithBehaviorWnd );
			m_pEditWithBehaviorWnd->m_arrRules.RemoveAll();
			LONG nRuleIndex, nRuleCount = RowCountGet();
			for( nRuleIndex = 0L; nRuleIndex < nRuleCount; nRuleIndex ++ )
			{
				CExtEditWithBehavior::CBehaviorMasked::CharRule _cr;
				if( ! RuleGetAt( nRuleIndex, _cr ) )
					continue;
				m_pEditWithBehaviorWnd->m_arrRules.Add( _cr );
			}
			
			CExtSafeString strTextValid = m_pEditWithBehaviorWnd->TextGetValid();

			LPCTSTR strTextToSet = (LPCTSTR(strTextValid) != NULL) ? LPCTSTR(strTextValid) : _T("");
			if( m_pEditWithBehaviorWnd->m_bRichMode )
			{
				CExtSafeString strSet( strTextToSet );
				m_pEditWithBehaviorWnd->Rich_PlainText_Set( strSet );
				m_pEditWithBehaviorWnd->_Rich_OnDraw();
			}
			else
				m_pEditWithBehaviorWnd->SetWindowText( strTextToSet );

			m_pEditWithBehaviorWnd->SetSel( -1, -1 );
		}
		virtual void OnGridCellInputComplete(
			CExtGridCell & _cell,
			LONG nColNo,
			LONG nRowNo,
			INT nColType,
			INT nRowType,
			HWND hWndInputControl = NULL
			)
		{
			CExtNSB < CExtGridWnd > :: OnGridCellInputComplete( _cell, nColNo, nRowNo, nColType, nRowType, hWndInputControl );
			OnRuleSetChanged();
		}
		void RuleAdd(
			bool bEnabled,
			CExtEditWithBehavior::CBehaviorMasked::CharRule & _cr,
			__EXT_MFC_SAFE_LPCTSTR strFunctionNameValidation,
			__EXT_MFC_SAFE_LPCTSTR strFunctionNameConversion
			)
		{
			ASSERT_VALID( this );
			LONG nRuleIndex = RowCountGet();
			RowAdd( 1L, false );

			CExtGridCellBool * pCellStatus =
				STATIC_DOWNCAST(
					CExtGridCellBool,
					GridCellGet( 0L, nRuleIndex, 0, 0, RUNTIME_CLASS(CExtGridCellBool) )
					);
			pCellStatus->LabelTextSet( true, _T("Enabled") );
			pCellStatus->LabelTextSet( false, _T("Disabled") );
			pCellStatus->DataSet( bEnabled );
			pCellStatus->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );

			CExtGridCellString * pCellOneCharacter =
				STATIC_DOWNCAST(
					CExtGridCellString,
					GridCellGet( 1L, nRuleIndex, 0, 0, RUNTIME_CLASS(CExtGridCellString) )
					);
			pCellOneCharacter->ModifyStyle( __EGCS_TA_HORZ_CENTER );
			TCHAR strTmp[ 2 ] = { _cr.Get(), _T('\0') };
			pCellOneCharacter->TextSet( strTmp );

			CExtGridCellComboBox * pCellValidationFunction =
				STATIC_DOWNCAST(
					CExtGridCellComboBox,
					GridCellGet( 2L, nRuleIndex, 0, 0, RUNTIME_CLASS(CExtGridCellComboBox) )
					);
			pCellValidationFunction->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
			pCellValidationFunction->AddString( _T("(null)") );
			pCellValidationFunction->AddString( _T("IsNumeric") );
			pCellValidationFunction->AddString( _T("IsAlpha") );
			pCellValidationFunction->AddString( _T("IsAlphaOrNumeric") );
			if( strFunctionNameValidation != NULL )
				pCellValidationFunction->SelectString( strFunctionNameValidation );
			else
				pCellValidationFunction->SetCurSel( 0L );

			CExtGridCellComboBox * pCellConversionFunction =
				STATIC_DOWNCAST(
					CExtGridCellComboBox,
					GridCellGet( 3L, nRuleIndex, 0, 0, RUNTIME_CLASS(CExtGridCellComboBox) )
					);
			pCellConversionFunction->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
			pCellConversionFunction->AddString( _T("(null)") );
			pCellConversionFunction->AddString( _T("MakeUpper") );
			pCellConversionFunction->AddString( _T("MakeLower") );
			if( strFunctionNameConversion != NULL )
				pCellConversionFunction->SelectString( strFunctionNameConversion );
			else
				pCellConversionFunction->SetCurSel( 0L );

			CExtGridCellColor * pCellColor =
				STATIC_DOWNCAST(
					CExtGridCellColor,
					GridCellGet( 4L, nRuleIndex, 0, 0, RUNTIME_CLASS(CExtGridCellColor) )
					);
			pCellColor->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
			pCellColor->SetColor( _cr.m_clrSymbol, false );

			OnSwUpdateScrollBars();
			FocusSet( CPoint( 0L, nRuleIndex ) );
			OnRuleSetChanged();
		}
		void RuleRemove(
			LONG nRuleIndex = -1 // -1 - focused
			)
		{
			ASSERT_VALID( this );
			LONG nRowCount = RowCountGet();
			if( ! ( 0L <= nRuleIndex && nRuleIndex < nRowCount ) )
			{
				nRuleIndex = FocusGet().y;
				if( nRuleIndex < 0 )
					return;
				ASSERT( 0L <= nRuleIndex && nRuleIndex < nRowCount );
			}
			RowRemove( nRuleIndex );
			OnRuleSetChanged();
		}
		bool RuleGetAt(
			LONG nRuleIndex,
			CExtEditWithBehavior::CBehaviorMasked::CharRule & _cr
			)
		{
			LONG nRowCount = RowCountGet();
			if( ! ( 0L <= nRuleIndex && nRuleIndex < nRowCount ) )
				return false;

			CExtGridCellBool * pCellStatus =
				STATIC_DOWNCAST(
					CExtGridCellBool,
					GridCellGet( 0L, nRuleIndex )
					);
			if( ! pCellStatus->DataGet() )
				return false;

			CExtGridCellString * pCellOneCharacter =
				STATIC_DOWNCAST(
					CExtGridCellString,
					GridCellGet( 1L, nRuleIndex )
					);
			CExtSafeString strChr;
			pCellOneCharacter->TextGet( strChr );
			static __EXT_MFC_SAFE_LPCTSTR g_strTrimChars = _T(" \r\n\t");
			strChr.TrimLeft( g_strTrimChars );
			strChr.TrimRight( g_strTrimChars );
			if( strChr.GetLength() != 1 )
				return false;
			_cr.Set( strChr.GetAt( 0 ) );

			CExtGridCellComboBox * pCellValidationFunction =
				STATIC_DOWNCAST(
					CExtGridCellComboBox,
					GridCellGet( 2L, nRuleIndex )
					);
			switch( pCellValidationFunction->GetCurSel() )
			{
			case 0: /* valid case, null function */ break;
			case 1: _cr.m_fnValidation = (CExtEditWithBehavior::CBehaviorMasked::CharRule::ValidationFunction)CExtEditMasked::stat_IsCharDigit; break;
			case 2: _cr.m_fnValidation = (CExtEditWithBehavior::CBehaviorMasked::CharRule::ValidationFunction)CExtEditMasked::stat_IsCharAlpha; break;
			case 3: _cr.m_fnValidation = (CExtEditWithBehavior::CBehaviorMasked::CharRule::ValidationFunction)CExtEditMasked::stat_IsCharAlphaOrDigit; break;
#ifdef _DEBUG
			default:
				ASSERT( FALSE );
			break;
#endif // _DEBUG
			}

			CExtGridCellComboBox * pCellConversionFunction =
				STATIC_DOWNCAST(
					CExtGridCellComboBox,
					GridCellGet( 3L, nRuleIndex )
					);
			switch( pCellConversionFunction->GetCurSel() )
			{
			case 0: /* valid case, null function */ break;
			case 1: _cr.m_fnConversion = (CExtEditWithBehavior::CBehaviorMasked::CharRule::ConversionFunction)_totupper; break;
			case 2: _cr.m_fnConversion = (CExtEditWithBehavior::CBehaviorMasked::CharRule::ConversionFunction)_totlower; break;
#ifdef _DEBUG
			default:
				ASSERT( FALSE );
			break;
#endif // _DEBUG
			}

			CExtGridCellColor * pCellColor =
				STATIC_DOWNCAST(
					CExtGridCellColor,
					GridCellGet( 4L, nRuleIndex )
					);
			_cr.m_clrSymbol = pCellColor->GetColor();

			return true;
		}
		void RuleResetAll()
		{
			RowRemoveAll( false );
			// default mask symbol number
			CExtEditWithBehavior::CBehaviorMasked::CharRule _rule1( RGB(164,0,0), _T('#'), (CExtEditWithBehavior::CBehaviorMasked::CharRule::ValidationFunction)CExtEditMasked::stat_IsCharDigit );
			RuleAdd( true, _rule1, _T("IsNumeric"), NULL );
			// default mask symbol letter
			CExtEditWithBehavior::CBehaviorMasked::CharRule _rule2( RGB(0,164,0), _T('@'), (CExtEditWithBehavior::CBehaviorMasked::CharRule::ValidationFunction)CExtEditMasked::stat_IsCharAlpha );
			RuleAdd( true, _rule2, _T("IsAlpha"), NULL );
			// default mask symbol number and letter
			CExtEditWithBehavior::CBehaviorMasked::CharRule _rule3( RGB(0,0,164), _T('$'), (CExtEditWithBehavior::CBehaviorMasked::CharRule::ValidationFunction)CExtEditMasked::stat_IsCharAlphaOrDigit );
			RuleAdd( true, _rule3, _T("IsAlphaOrNumeric"), NULL );
		}
	};

	CRuleGridWnd m_wndRuleGrid;

	//{{AFX_VIRTUAL(CPageMaskedEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	bool m_bInitComplete:1;
	
	//{{AFX_MSG(CPageMaskedEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnUseMaskedRichEdit();
	afx_msg void OnCharRuleAdd();
	afx_msg void OnCharRuleRemove();
	afx_msg void OnChangeEditMask();
	afx_msg void OnCharRuleResetAll();
	afx_msg void OnOptionChanged();
	afx_msg void OnSetfocusEditNullChar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // (!defined __EXT_MFC_NO_GRIDWND )

/////////////////////////////////////////////////////////////////////////////
// CPageSystemNumberCurrencyEditors dialog

class CPageSystemNumberCurrencyEditors : public CPageBase 
{
protected:
	CList < UINT, UINT > m_listColorButtonIDs;
public:
//	INT m_nTestNumberWholeDigits, m_nTestNumberFractionDigits;
	CPageSystemNumberCurrencyEditors(CWnd* pParent = NULL);   // standard constructor

	//{{AFX_DATA(CPageSystemNumberCurrencyEditors)
	enum { IDD = IDD_PAGE_SYSTEM_NUMBER_CURRENCY_EDITORS };
	CExtEditScientificNumber	m_editScientificNumberSimple;
	CExtEditScientificNumber	m_editScientificNumberRich;
	CExtCheckBox				m_checkUseRichEditors;
	CExtEditSystemNumber		m_editSystemNumberSimple;
	CExtEditSystemNumber		m_editSystemNumberRich;
	CExtLabel					m_labelValueOfSystemNumberEditor;
	CExtEditSystemCurrency		m_editSystemCurrencySimple;
	CExtEditSystemCurrency		m_editSystemCurrencyRich;
	CExtLabel					m_labelValueOfSystemCurrencyEditor;
	CExtEdit					m_editMaxWholeDigits;
	CExtSpinWnd					m_spinMaxWholeDigits;
	CExtEdit					m_editMaxFractionDigits;
	CExtSpinWnd					m_spinMaxFractionDigits;
	CExtCheckBox				m_checkCanBeNegative;
	CExtColorButton				m_buttonColorNumber;
	CExtColorButton				m_buttonColorFraction;
	CExtColorButton				m_buttonColorSign;
	CExtColorButton				m_buttonColorBrackets;
	CExtColorButton				m_buttonColorSeparatorGroup;
	CExtColorButton				m_buttonColorSeparatorDecimal;
	CExtColorButton				m_buttonDefaultText;
	CExtColorButton				m_buttonDefaultBack;
	CExtColorButton				m_buttonColorPower;
	CExtColorButton				m_buttonColorScientificSign;
	CExtCheckBox				m_checkNoGroupSeparators;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPageSystemNumberCurrencyEditors)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	bool m_bInitComplete:1;
	void _SyncFrom( CExtEditSystemNumberCurrencyBase * pWndEditSyncFrom );
	void _SyncValueOfNumber();
	void _SyncValueOfCurrency();
	
	//{{AFX_MSG(CPageSystemNumberCurrencyEditors)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckUseRichEditors();
	afx_msg void OnChangeEditSystemNumberSimple();
	afx_msg void OnChangeEditSystemNumberRich();
	afx_msg void OnChangeEditSystemCurrencySimple();
	afx_msg void OnChangeEditSystemCurrencyRich();
	afx_msg void OnChangeMaxWholeDigits();
	afx_msg void OnChangeMaxFractionDigits();
	afx_msg void OnCheckCanBeNegative();
	afx_msg void OnChangeWindowsLocaleSettings();
	afx_msg void OnCheckNoGroupSeparators();
	//}}AFX_MSG
	afx_msg LRESULT OnColorChangedFinally( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorSelectCustom( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

#endif // ( ! defined __PAGE_MASKED_EDIT_H )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

