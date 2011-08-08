#if !defined(AFX_PAGEGRID_H__F54F4632_3768_42D0_9E0E_3F85326598E1__INCLUDED_)
#define AFX_PAGEGRID_H__F54F4632_3768_42D0_9E0E_3F85326598E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageGrid.h : header file
//

#if (!defined __EXT_MFC_NO_GRIDWND)

#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CExtGridCellCheckBoxEye

class CExtGridCellCheckBoxEye : public CExtGridCellCheckBox
{
public:
	DECLARE_SERIAL( CExtGridCellCheckBoxEye );
	IMPLEMENT_ExtGridCell_Clone( CExtGridCellCheckBoxEye, CExtGridCellCheckBox );
	CExtSafeString m_arrStrings[3];    // 0 - unchecked, 1 - checked, 2 - indeterminate
	mutable CExtCmdIcon m_arrIcons[3]; // 0 - unchecked, 1 - checked, 2 - indeterminate
	CExtGridCellCheckBoxEye(
		CExtGridDataProvider * pDataProvider = NULL
		)
		: CExtGridCellCheckBox ( pDataProvider )
	{
		m_arrStrings[ 0 ] = _T("Hidden");
		m_arrStrings[ 1 ] = _T("Visible");
		m_arrStrings[ 2 ] = _T("Indeterminate");
		static BYTE g_bitmapBytes[] =
		{
			0x42, 0x4D, 0x9E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xB6, 0xB6, 0xB6, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB3, 0xB5, 0xB5, 0xB5, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x8E, 0x8B, 0x90, 0x42, 0x42, 0x42, 0x3B, 0x3B, 0x3B, 0x3F, 0x3F, 0x3F, 0x43, 0x43, 0x43, 0x8A, 0x89, 0x8A, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00,
			0xFF, 0x47, 0x29, 0x46, 0x7B, 0x7A, 0x7A, 0xA3, 0xA3, 0xA3, 0xB4, 0xB4, 0xB4, 0xB6, 0xB6, 0xB6, 0xC7, 0xC7, 0xC7, 0x81, 0x81, 0x81, 0x29, 0x29, 0x29, 0x87, 0x90, 0x89, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xBB, 0xBB, 0xBB, 0xD2, 0xD2, 0xD2, 0x4C, 0x4C, 0x4C, 0x14, 0x14, 0x14, 0x02, 0x02, 0x02, 0x23, 0x23, 0x23, 0xB9, 0xB9, 0xB9, 0xBA, 0xBA, 0xBA, 0x2B, 0x2B, 0x2B, 0x85, 0x84, 0x85, 0xFF, 0x00, 0xFF, 0xA8, 0xA8, 0xA8, 0xAC, 0xAC, 0xAC, 0x37, 0x37, 0x37, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x34, 0x34, 0x34, 0xA0, 0xA0, 0xA0, 0x92, 0x92, 0x92, 0x30, 0x30, 0x30, 0xFF, 0x00, 0xFF, 0x5C, 0x5C, 0x5C, 0x91, 0x91, 0x91, 0x25, 0x25, 0x25, 0x4B, 0x4B, 0x4B, 0xD0, 0xD0, 0xD0, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x3F,
			0x3F, 0x3F, 0x89, 0x89, 0x89, 0x3A, 0x3A, 0x3A, 0x33, 0x33, 0x33, 0xFF, 0x00, 0xFF, 0x9E, 0x9E, 0x9E, 0x30, 0x30, 0x30, 0x25, 0x25, 0x25, 0x16, 0x16, 0x16, 0x34, 0x34, 0x34, 0x12, 0x12, 0x12, 0x07, 0x07, 0x07, 0x13, 0x13, 0x13, 0x3B, 0x3B, 0x3B, 0x2E, 0x2E, 0x2E, 0x87, 0x87, 0x87, 0x89, 0x91, 0x8B, 0x4A, 0x4A, 0x4A, 0x89, 0x89, 0x89, 0x78, 0x78, 0x78, 0x47, 0x47, 0x47, 0x20, 0x20, 0x20, 0x2F, 0x2F, 0x2F, 0x2E, 0x2E, 0x2E, 0x55, 0x55, 0x55, 0x63, 0x63, 0x63, 0x91, 0x91, 0x91, 0x60, 0x64, 0x61, 0x97, 0xA2, 0x99, 0x9C, 0x9C, 0x9C, 0x4E, 0x4E, 0x4E, 0x7A, 0x7A, 0x7A, 0x7C, 0x7C, 0x7C, 0x9D, 0x9D, 0x9D, 0x9C, 0x9C, 0x9C, 0x9C, 0x9C, 0x9C, 0x85, 0x85, 0x85, 0x70, 0x70, 0x70, 0x4F, 0x4F, 0x4F, 0xAB, 0xAB, 0xAB, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF,
			0x86, 0x8D, 0x87, 0x76, 0x76, 0x76, 0x4E, 0x4E, 0x4E, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x6D, 0x6D, 0x6D, 0x8A, 0x8A, 0x8A, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF,
		};
		VERIFY( m_arrIcons[0].m_bmpNormal.LoadBMP_Buffer( g_bitmapBytes, sizeof(g_bitmapBytes)/sizeof(g_bitmapBytes[0]), true ) );
		VERIFY( m_arrIcons[0].m_bmpNormal.Make32() );
		m_arrIcons[0].m_bmpNormal.AlphaColor( RGB(255,0,255), RGB(0,0,0), BYTE(0) );
		m_arrIcons[1].m_bmpNormal = m_arrIcons[2].m_bmpNormal = m_arrIcons[0].m_bmpNormal;
		m_arrIcons[0].m_bmpNormal.AdjustAlpha( -1.00 );
		m_arrIcons[2].m_bmpNormal.AdjustAlpha( -0.75 );
		SetCheck( 0 ); // initialize text
	}
	virtual __EXT_MFC_SAFE_LPCTSTR GetTextBuffer() const { ASSERT_VALID( this ); return NULL; }
protected:
	INT _GetTextAndCheckMarkIconIndex() const
	{
		ASSERT_VALID( this );
		INT nCheck = GetCheck(), nTextAndCheckMarkIconIndex = 2;
		switch( nCheck )
		{
		case 0:
		case 1:
			nTextAndCheckMarkIconIndex = nCheck;
		break;
		}
		return nTextAndCheckMarkIconIndex;
	}
public:
	virtual void TextGet( CExtSafeString & strCopy ) const
	{
		ASSERT_VALID( this );
		strCopy = m_arrStrings[ _GetTextAndCheckMarkIconIndex() ];
	}
	virtual void Assign( const CExtGridCell & other )
	{
		ASSERT_VALID( this );
		CExtGridCellCheckBox::Assign( other );
		CExtGridCellCheckBoxEye * pCell = DYNAMIC_DOWNCAST( CExtGridCellCheckBoxEye, ( const_cast < CExtGridCell * > ( &other ) ) );
		if( pCell != NULL )
		{
			INT nIndex, nCount = INT(sizeof(m_arrStrings)/sizeof(m_arrStrings[0]));
			for( nIndex = 0; nIndex < nCount; nIndex ++ )
				m_arrStrings[ nIndex ] = pCell -> m_arrStrings[ nIndex ];
		}
	}
	virtual void Serialize( CArchive & ar )
	{
		ASSERT_VALID( this );
		CExtGridCellCheckBox::Serialize( ar );
		INT nIndex, nCount = INT(sizeof(m_arrStrings)/sizeof(m_arrStrings[0]));
		if( ar.IsStoring() )
		{
			for( nIndex = 0; nIndex < nCount; nIndex ++ )
				ar << m_arrStrings[ nIndex ];
		} // if( ar.IsStoring() )
		else
		{
			for( nIndex = 0; nIndex < nCount; nIndex ++ )
				ar >> m_arrStrings[ nIndex ];
		} // else from if( ar.IsStoring() )
	}
	virtual CSize OnCalcCheckSize(
		bool bPressed,
		bool bHovered,
		bool bEnabled,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const
	{
		ASSERT_VALID( this );
		bPressed; bHovered; bEnabled; wndGrid; dc; nVisibleColNo; nVisibleRowNo; nColNo; nRowNo; nColType; nRowType; dwAreaFlags; dwHelperPaintFlags;
		CExtCmdIcon * pIcon = & ( m_arrIcons[ _GetTextAndCheckMarkIconIndex() ] );
		if( pIcon == NULL || pIcon->IsEmpty() )
			return CSize( 0, 0 );
		CSize _size = pIcon->GetSize();
		return _size;
	}
	virtual void OnPaintCheck(
		const RECT & rcCheck,
		bool bPressed,
		bool bHovered,
		bool bEnabled,
		const CExtGridWnd & wndGrid,
		CDC & dc,
		LONG nVisibleColNo,
		LONG nVisibleRowNo,
		LONG nColNo,
		LONG nRowNo,
		INT nColType,
		INT nRowType,
		const RECT & rcCellExtra,
		const RECT & rcCell,
		const RECT & rcVisibleRange,
		DWORD dwAreaFlags,
		DWORD dwHelperPaintFlags
		) const
	{
		ASSERT_VALID( this );
		ASSERT_VALID( (&wndGrid) );
		ASSERT( dc.GetSafeHdc() != NULL );
		if( ! dc.RectVisible(&rcCheck) )
			return;
		nVisibleColNo; nVisibleRowNo; nColNo; nRowNo; nColType; nRowType; rcCellExtra; rcCell; rcVisibleRange; dwAreaFlags; dwHelperPaintFlags;
		CExtCmdIcon * pIcon = & ( m_arrIcons[ _GetTextAndCheckMarkIconIndex() ] );
		if( pIcon == NULL || pIcon->IsEmpty() )
			return;
		CExtCmdIcon::e_paint_type_t ePT = CExtCmdIcon::__PAINT_NORMAL;
		if( ! bEnabled )
			ePT = CExtCmdIcon::__PAINT_DISABLED;
		else if( bPressed )
			ePT = CExtCmdIcon::__PAINT_PRESSED;
		else if( bHovered )
			ePT = CExtCmdIcon::__PAINT_HOVER;
		pIcon->Paint( wndGrid.PmBridge_GetPM(), dc.m_hDC, rcCheck, ePT );
	}
}; // class CExtGridCellCheckBoxEye

/////////////////////////////////////////////////////////////////////////////
// CPageGrid dialog

class CPageGrid : public CPageBase
{
// Construction
public:
	CPageGrid(CWnd* pParent = NULL);   // standard constructor

	class CCellTypeHyperLinkButton : public CExtHyperLinkButton 
	{
	public:
		CCellTypeHyperLinkButton()
		{
			m_bUseStdCommandNotification = true;
			m_bDrawFocusRect = false;
			m_bTextUnderline = true;
		}
	};

// Dialog Data
	//{{AFX_DATA(CPageGrid)
	enum { IDD = IDD_PAGE_GRID };
	CCellTypeHyperLinkButton m_wndCellBoolean;
	CCellTypeHyperLinkButton m_wndCellCheckBox;
	CCellTypeHyperLinkButton m_wndCellRadioButton;
	CCellTypeHyperLinkButton m_wndCellColor;
	CCellTypeHyperLinkButton m_wndCellComboBox;
	CCellTypeHyperLinkButton m_wndCellCheckComboBox;
	CCellTypeHyperLinkButton m_wndCellDateTime;
	CCellTypeHyperLinkButton m_wndCellDuration;
	CCellTypeHyperLinkButton m_wndCellProgress;
	CCellTypeHyperLinkButton m_wndCellHyperlink;
	CCellTypeHyperLinkButton m_wndCellIPAddress;
	CCellTypeHyperLinkButton m_wndCellNumber;
	CCellTypeHyperLinkButton m_wndCellCurrency;
	CCellTypeHyperLinkButton m_wndCellScientific;
	CCellTypeHyperLinkButton m_wndCellFraction;
	CCellTypeHyperLinkButton m_wndCellPercentage;
	CCellTypeHyperLinkButton m_wndCellVariant;
	CCellTypeHyperLinkButton m_wndCellUpDown;
	CCellTypeHyperLinkButton m_wndCellPicture;
	CCellTypeHyperLinkButton m_wndCellSlider;
	CCellTypeHyperLinkButton m_wndCellText;
	CCellTypeHyperLinkButton m_wndCellPassword;
	CCellTypeHyperLinkButton m_wndCellFile;
	CCellTypeHyperLinkButton m_wndCellFileImage;
	CCellTypeHyperLinkButton m_wndCellFolder;
	CCellTypeHyperLinkButton m_wndCellFont;
	CCellTypeHyperLinkButton m_wndCellFontFaceName;
	CCellTypeHyperLinkButton m_wndCellFontWeight;
	CCellTypeHyperLinkButton m_wndCellColorPart;
	CCellTypeHyperLinkButton m_wndCellRectPart;
	CCellTypeHyperLinkButton m_wndCellInplaceSlider;
	CCellTypeHyperLinkButton m_wndCellInplaceScrollBar;
	CCellTypeHyperLinkButton m_wndCellHotKey;
	CCellTypeHyperLinkButton m_wndCellButton;
	CCellTypeHyperLinkButton m_wndCellObjectSize;
	CCellTypeHyperLinkButton m_wndCellGeographic;
	CCellTypeHyperLinkButton m_wndCellMaskedString;
	CCellTypeHyperLinkButton m_wndCellSystemNumber;
	CCellTypeHyperLinkButton m_wndCellSystemCurrency;
	CExtIconButton m_btnPPV;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageGrid)
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CExtNSB < CExtPPVW < CExtGridWnd > > m_wndGrid;

	void _InitColumnBoolean( LONG nColNo, LONG nRowCount );
	void _InitColumnCheckBox( LONG nColNo, LONG nRowCount );
	void _InitColumnRadioButton( LONG nColNo, LONG nRowCount );
	void _InitColumnColor( LONG nColNo, LONG nRowCount );
	void _InitColumnComboBox( LONG nColNo, LONG nRowCount );
	void _InitColumnCheckComboBox( LONG nColNo, LONG nRowCount );
	void _InitColumnDateTime( LONG nColNo, LONG nRowCount );
	void _InitColumnDuration( LONG nColNo, LONG nRowCount );
	void _InitColumnHyperLink( LONG nColNo, LONG nRowCount );
	void _InitColumnIPAddress( LONG nColNo, LONG nRowCount );
	void _InitColumnNumber( LONG nColNo, LONG nRowCount );
	void _InitColumnCurrency( LONG nColNo, LONG nRowCount );
	void _InitColumnScientific( LONG nColNo, LONG nRowCount );
	void _InitColumnFraction( LONG nColNo, LONG nRowCount );
	void _InitColumnPercentage( LONG nColNo, LONG nRowCount );
	void _InitColumnVariant( LONG nColNo, LONG nRowCount );
	void _InitColumnUpDown( LONG nColNo, LONG nRowCount );
	void _InitColumnPicture( LONG nColNo, LONG nRowCount );
	void _InitColumnProgress( LONG nColNo, LONG nRowCount );
	void _InitColumnSlider( LONG nColNo, LONG nRowCount );
	void _InitColumnText( LONG nColNo, LONG nRowCount );
	void _InitColumnPassword( LONG nColNo, LONG nRowCount );
	void _InitColumnFile( LONG nColNo, LONG nRowCount );
	void _InitColumnFileImage( LONG nColNo, LONG nRowCount );
	void _InitColumnFolder( LONG nColNo, LONG nRowCount );
	void _InitColumnFont( LONG nColNo, LONG nRowCount );
	void _InitColumnFontFaceName( LONG nColNo, LONG nRowCount );
	void _InitColumnFontWeight( LONG nColNo, LONG nRowCount );
	void _InitColumnColorPart( LONG nColNo, LONG nRowCount );
	void _InitColumnRectPart( LONG nColNo, LONG nRowCount );
	void _InitColumnInplaceSlider( LONG nColNo, LONG nRowCount, bool bSliderMode = true );
	void _InitColumnInplaceScrollBar( LONG nColNo, LONG nRowCount );
	void _InitColumnHotKey( LONG nColNo, LONG nRowCount );
	void _InitColumnButton( LONG nColNo, LONG nRowCount );
	void _InitColumnObjectSize( LONG nColNo, LONG nRowCount );
	void _InitColumnGeographic( LONG nColNo, LONG nRowCount );
	void _InitColumnStringMasked( LONG nColNo, LONG nRowCount );
	void _InitColumnSystemNumber( LONG nColNo, LONG nRowCount );
	void _InitColumnSystemCurrency( LONG nColNo, LONG nRowCount );

	// Generated message map functions
	//{{AFX_MSG(CPageGrid)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnTimer( __EXT_MFC_UINT_PTR nIDEvent );
	DECLARE_MESSAGE_MAP()
};

#endif // #if (!defined __EXT_MFC_NO_GRIDWND)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEGRID_H__F54F4632_3768_42D0_9E0E_3F85326598E1__INCLUDED_)
