// PageGrid.cpp : implementation file
//

#include "stdafx.h"
#include "profuis_controls.h"
#include "PageGrid.h"

#if (!defined __EXT_MFC_NO_GRIDWND)
	IMPLEMENT_SERIAL( CExtGridCellCheckBoxEye, CExtGridCellCheckBox, VERSIONABLE_SCHEMA|1 );
#endif // (!defined __EXT_MFC_NO_GRIDWND)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if (!defined __EXT_MFC_NO_GRIDWND)

#define __COLUMN_NUM_BOOLEAN              0L
#define __COLUMN_NUM_CHECKBOX             1L
#define __COLUMN_NUM_RADIOBUTTON          2L
#define __COLUMN_NUM_COLOR                3L
#define __COLUMN_NUM_COMBOBOX             4L
#define __COLUMN_NUM_CHECKCOMBOBOX        5L
#define __COLUMN_NUM_DATETIME             6L
#define __COLUMN_NUM_DURATION             7L
#define __COLUMN_NUM_PROGRESS             8L
#define __COLUMN_NUM_HYPERLINK            9L
#define __COLUMN_NUM_IPADDRESS           10L
#define __COLUMN_NUM_NUMBER              11L
#define __COLUMN_NUM_CURRENCY            12L
#define __COLUMN_NUM_SCIENTIFIC          13L
#define __COLUMN_NUM_FRACTION            14L
#define __COLUMN_NUM_PERCENTAGE          15L
#define __COLUMN_NUM_VARIANT             16L
#define __COLUMN_NUM_UPDOWN              17L
#define __COLUMN_NUM_PICTURE             18L
#define __COLUMN_NUM_SLIDER              19L
#define __COLUMN_NUM_TEXT                20L
#define __COLUMN_NUM_PASSWORD            21L
#define __COLUMN_NUM_FILE                22L
#define __COLUMN_NUM_FILEIMAGE           23L
#define __COLUMN_NUM_FOLDER              24L
#define __COLUMN_NUM_FONT                25L
#define __COLUMN_NUM_FONTFACENAME        26L
#define __COLUMN_NUM_FONTWEIGHT          27L
#define __COLUMN_NUM_COLORPART           28L
#define __COLUMN_NUM_RECTPART            29L
#define __COLUMN_NUM_INPLACE_SLIDER      30L
#define __COLUMN_NUM_INPLACE_SCROLLBAR   31L
#define __COLUMN_NUM_HOTKEY				 32L
#define __COLUMN_NUM_BUTTON				 33L
#define __COLUMN_NUM_OBJECT_SIZE		 34L
#define __COLUMN_NUM_GEOGRAPHIC			 35L
#define __COLUMN_NUM_MASKED_STRING		 36L
#define __COLUMN_NUM_SYSTEM_NUMBER		 37L
#define __COLUMN_NUM_SYSTEM_CURRENCY	 38L

/////////////////////////////////////////////////////////////////////////////
// CPageGrid dialog

CPageGrid::CPageGrid(CWnd* pParent /*=NULL*/)
	: CPageBase(CPageGrid::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageGrid)
	//}}AFX_DATA_INIT
}

void CPageGrid::DoDataExchange(CDataExchange* pDX)
{
	CPageBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageGrid)
	DDX_Control(pDX, IDC_CELL_BOOLEAN, m_wndCellBoolean);
	DDX_Control(pDX, IDC_CELL_CHECKBOX, m_wndCellCheckBox);
	DDX_Control(pDX, IDC_CELL_RADIOBUTTON, m_wndCellRadioButton);
	DDX_Control(pDX, IDC_CELL_COLOR, m_wndCellColor);
	DDX_Control(pDX, IDC_CELL_COMBOBOX, m_wndCellComboBox);
	DDX_Control(pDX, IDC_CELL_CHECKCOMBOBOX, m_wndCellCheckComboBox);
	DDX_Control(pDX, IDC_CELL_DATETIME, m_wndCellDateTime);
	DDX_Control(pDX, IDC_CELL_DURATION, m_wndCellDuration);
	DDX_Control(pDX, IDC_CELL_PROGRESS, m_wndCellProgress);
	DDX_Control(pDX, IDC_CELL_HYPERLINK, m_wndCellHyperlink);
	DDX_Control(pDX, IDC_CELL_IPADDRESS, m_wndCellIPAddress);
	DDX_Control(pDX, IDC_CELL_NUMBER, m_wndCellNumber);
	DDX_Control(pDX, IDC_CELL_CURRENCY, m_wndCellCurrency);
	DDX_Control(pDX, IDC_CELL_SCIENTIFIC, m_wndCellScientific);
	DDX_Control(pDX, IDC_CELL_FRACTION, m_wndCellFraction);
	DDX_Control(pDX, IDC_CELL_PERCENTAGE, m_wndCellPercentage);
	DDX_Control(pDX, IDC_CELL_VARIANT, m_wndCellVariant);
	DDX_Control(pDX, IDC_CELL_UPDOWN, m_wndCellUpDown);
	DDX_Control(pDX, IDC_CELL_PICTURE, m_wndCellPicture);
	DDX_Control(pDX, IDC_CELL_SLIDER, m_wndCellSlider);
	DDX_Control(pDX, IDC_CELL_TEXT, m_wndCellText);
	DDX_Control(pDX, IDC_CELL_PASSWORD, m_wndCellPassword);
	DDX_Control(pDX, IDC_CELL_FILE, m_wndCellFile);
	DDX_Control(pDX, IDC_CELL_FILEIMAGE, m_wndCellFileImage);
	DDX_Control(pDX, IDC_CELL_FOLDER, m_wndCellFolder);
	DDX_Control(pDX, IDC_CELL_FONT, m_wndCellFont);
	DDX_Control(pDX, IDC_CELL_FONTFACENAME, m_wndCellFontFaceName);
	DDX_Control(pDX, IDC_CELL_FONTWEIGHT, m_wndCellFontWeight);
	DDX_Control(pDX, IDC_CELL_COLORPART, m_wndCellColorPart);
	DDX_Control(pDX, IDC_CELL_RECTPART, m_wndCellRectPart);
	DDX_Control(pDX, IDC_CELL_INPLACE_SLIDER, m_wndCellInplaceSlider);
	DDX_Control(pDX, IDC_CELL_INPLACE_SCROLLBAR, m_wndCellInplaceScrollBar);
	DDX_Control(pDX, IDC_CELL_HOTKEY, m_wndCellHotKey);
	DDX_Control(pDX, IDC_CELL_BUTTON, m_wndCellButton);
	DDX_Control(pDX, IDC_CELL_OBJECT_SIZE, m_wndCellObjectSize);
	DDX_Control(pDX, IDC_CELL_GEOGRAPHIC, m_wndCellGeographic);
	DDX_Control(pDX, IDC_CELL_MASKED_STRING, m_wndCellMaskedString);
	DDX_Control(pDX, IDC_CELL_SYSTEM_NUMBER, m_wndCellSystemNumber);
	DDX_Control(pDX, IDC_CELL_SYSTEM_CURRENCY, m_wndCellSystemCurrency);
	DDX_Control(pDX, ID_FILE_PRINT_PREVIEW, m_btnPPV );
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_wndGrid);
}

BEGIN_MESSAGE_MAP(CPageGrid, CPageBase)
	//{{AFX_MSG_MAP(CPageGrid)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageGrid message handlers

BOOL CPageGrid::OnInitDialog() 
{
	CPageBase::OnInitDialog();
	
	AddAnchor( IDC_GRID, __RDA_LT, __RDA_RB );
	AddAnchor( ID_FILE_PRINT_PREVIEW, __RDA_RB );

	AddAnchor( IDC_CELL_BOOLEAN, __RDA_LB );
	AddAnchor( IDC_CELL_CHECKBOX, __RDA_LB );
	AddAnchor( IDC_CELL_CHECKCOMBOBOX, __RDA_LB );
	AddAnchor( IDC_CELL_COLOR, __RDA_LB );
	AddAnchor( IDC_CELL_COLORPART, __RDA_LB );
	AddAnchor( IDC_CELL_COMBOBOX, __RDA_LB );
	AddAnchor( IDC_CELL_CURRENCY, __RDA_LB );
	AddAnchor( IDC_CELL_DATETIME, __RDA_LB );
	AddAnchor( IDC_CELL_DURATION, __RDA_LB );
	AddAnchor( IDC_CELL_FILE, __RDA_LB );
	AddAnchor( IDC_CELL_FILEIMAGE, __RDA_LB );
	AddAnchor( IDC_CELL_FOLDER, __RDA_LB );
	AddAnchor( IDC_CELL_FONT, __RDA_LB );
	AddAnchor( IDC_CELL_FONTFACENAME, __RDA_LB );
	AddAnchor( IDC_CELL_FONTWEIGHT, __RDA_LB );
	AddAnchor( IDC_CELL_FRACTION, __RDA_LB );
	AddAnchor( IDC_CELL_HYPERLINK, __RDA_LB );
	AddAnchor( IDC_CELL_IPADDRESS, __RDA_LB );
	AddAnchor( IDC_CELL_NUMBER, __RDA_LB );
	AddAnchor( IDC_CELL_PASSWORD, __RDA_LB );
	AddAnchor( IDC_CELL_PERCENTAGE, __RDA_LB );
	AddAnchor( IDC_CELL_PICTURE, __RDA_LB );
	AddAnchor( IDC_CELL_PROGRESS, __RDA_LB );
	AddAnchor( IDC_CELL_RADIOBUTTON, __RDA_LB );
	AddAnchor( IDC_CELL_RECTPART, __RDA_LB );
	AddAnchor( IDC_CELL_INPLACE_SLIDER, __RDA_LB );
	AddAnchor( IDC_CELL_INPLACE_SCROLLBAR, __RDA_LB );
	AddAnchor( IDC_CELL_SCIENTIFIC, __RDA_LB );
	AddAnchor( IDC_CELL_SLIDER, __RDA_LB );
	AddAnchor( IDC_CELL_TEXT, __RDA_LB );
	AddAnchor( IDC_CELL_UPDOWN, __RDA_LB );
	AddAnchor( IDC_CELL_VARIANT, __RDA_LB );
	AddAnchor( IDC_CELL_HOTKEY, __RDA_LB );
	AddAnchor( IDC_CELL_BUTTON, __RDA_LB );
	AddAnchor( IDC_CELL_OBJECT_SIZE, __RDA_LB );
	AddAnchor( IDC_CELL_GEOGRAPHIC, __RDA_LB );
	AddAnchor( IDC_CELL_MASKED_STRING, __RDA_LB );
	AddAnchor( IDC_CELL_SYSTEM_NUMBER, __RDA_LB );
	AddAnchor( IDC_CELL_SYSTEM_CURRENCY, __RDA_LB );

	m_btnPPV.SetTooltipText( _T("Print and Print Preview") );
	m_btnPPV.m_icon.m_bmpNormal.LoadBMP_Resource( MAKEINTRESOURCE( IDB_PRINT_PREVIEW ) );
	m_btnPPV.m_rcFocusRectPadding.SetRect( 0, 0, 0, 0 );

	m_wndGrid.OuterRowCountTopSet( 1L, false );
	m_wndGrid.OuterRowCountBottomSet( 1L, false );
	m_wndGrid.OuterColumnCountLeftSet( 1L, false );
	m_wndGrid.OuterColumnCountRightSet( 1L, false );
	m_wndGrid.OuterColumnWidthSet( true, 0L, 14 );
	m_wndGrid.OuterColumnWidthSet( false, 0L, 14 );

	m_wndGrid.NoHideSelectionSet( true, false );
	m_wndGrid.SubtractSelectionAreasSet( true, false );
	m_wndGrid.LbExtSelectionSet( true );
	m_wndGrid.GridLinesHorzSet( true, false );
	m_wndGrid.GridLinesVertSet( true, false );
	m_wndGrid.HoverEventsSet( true, true );
	m_wndGrid.MultiAreaSelectionSet( true );
	m_wndGrid.HoverHighlightSet( true, false, true, true, true, true );

	m_wndGrid.SiwModifyStyle(
			  (__ESIS_STH_PIXEL|__ESIS_STV_ITEM)	// item scroll window styles
			| __EGBS_SFB_CELLS						// selection/focus type
			| __EGBS_RESIZING_CELLS_OUTER			// enable row/column resizing
			| __EGBS_RESIZING_CELLS_INNER			
			| __EGBS_DYNAMIC_RESIZING				// resize rows/columns on-the-fly
			//|__ESIS_DISABLE_AUTOHIDE_SB_H|__ESIS_DISABLE_AUTOHIDE_SB_V
			,
		0,
		false
		);
	m_wndGrid.BseModifyStyleEx(
			  __EGBS_BSE_EX_DBLCLK_BEST_FIT_ROW		// do best fit sizing on double clicks over column divider
			| __EGBS_BSE_EX_DBLCLK_BEST_FIT_COLUMN	// do best fit sizing on double clicks over row divider
		,
		__EGWS_BSE_EX_DEFAULT,
		false
		);
	m_wndGrid.BseModifyStyle( 
		__EGWS_BSE_SORT_COLUMNS, 
		0, 
		false 
		);
	m_wndGrid.SiwModifyStyleEx(
			  __EGBS_EX_CELL_TOOLTIPS_INNER
			| __EGBS_EX_CELL_EXPANDING_INNER
			| __EGWS_EX_PM_COLORS
			| __EGBS_EX_CORNER_AREAS_3D			
			,
		0,
		false
		);
	//m_wndGrid.BseModifyStyleEx( __EGBS_BSE_EX_AUTO_BEST_FIT_ROWS, 0, false ); // auto-best-fit
static struct
{
	LPCTSTR m_strText;
	INT m_nWidth;
	DWORD m_dwStyle;
}
g_arrHeaderDefinitions[] =
{
	{ _T("Boolean"),           85, __EGCS_TA_HORZ_LEFT },
	{ _T("CheckBox"),          85, __EGCS_TA_HORZ_LEFT },
	{ _T("RadioButton"),       85, __EGCS_TA_HORZ_LEFT },
	{ _T("Color"),            110, __EGCS_TA_HORZ_LEFT },
	{ _T("ComboBox"),          95, __EGCS_TA_HORZ_LEFT },
	{ _T("CheckComboBox"),    160, __EGCS_TA_HORZ_LEFT },
	{ _T("DateTime"),         140, __EGCS_TA_HORZ_LEFT },
	{ _T("Duration"),         110, __EGCS_TA_HORZ_LEFT },
	{ _T("Progress"),         160, __EGCS_TA_HORZ_LEFT },
	{ _T("HyperLink"),        130, __EGCS_TA_HORZ_LEFT },
	{ _T("IPAddress"),        120, __EGCS_TA_HORZ_LEFT },
	{ _T("Number"),           100, __EGCS_TA_HORZ_LEFT },
	{ _T("Currency"),         120, __EGCS_TA_HORZ_LEFT },
	{ _T("Scientific"),        85, __EGCS_TA_HORZ_LEFT },
	{ _T("Fraction"),          75, __EGCS_TA_HORZ_LEFT },
	{ _T("Percentage"),        75, __EGCS_TA_HORZ_LEFT },
	{ _T("Variant"),          140, __EGCS_TA_HORZ_LEFT },
	{ _T("UpDown"),            95, __EGCS_TA_HORZ_LEFT },
	{ _T("Picture"),           50, __EGCS_TA_HORZ_LEFT },
	{ _T("Slider"),            85, __EGCS_TA_HORZ_LEFT },
	{ _T("Text"),              85, __EGCS_TA_HORZ_LEFT },
	{ _T("Password"),          85, __EGCS_TA_HORZ_LEFT },
	{ _T("File"),             160, __EGCS_TA_HORZ_LEFT },
	{ _T("FileImage"),        160, __EGCS_TA_HORZ_LEFT },
	{ _T("Folder"),           160, __EGCS_TA_HORZ_LEFT },
	{ _T("Font"),             130, __EGCS_TA_HORZ_LEFT },
	{ _T("FontFaceName"),     140, __EGCS_TA_HORZ_LEFT },
	{ _T("FontWeight"),        90, __EGCS_TA_HORZ_LEFT },
	{ _T("ColorPart"),         90, __EGCS_TA_HORZ_LEFT },
	{ _T("RectPart"),          90, __EGCS_TA_HORZ_LEFT },
	{ _T("InplaceSlider"),    120, __EGCS_TA_HORZ_LEFT },
	{ _T("InplaceScrollBar"), 200, __EGCS_TA_HORZ_LEFT },
	{ _T("HotKey"),			   90, __EGCS_TA_HORZ_LEFT },
	{ _T("Button"),			   90, __EGCS_TA_HORZ_LEFT },
	{ _T("ObjectSize"),		   80, __EGCS_TA_HORZ_LEFT },
	{ _T("Geographic"),		  200, __EGCS_TA_HORZ_LEFT },
	{ _T("StringMasked"),	  250, __EGCS_TA_HORZ_LEFT },
	{ _T("SystemNumber"),	  100, __EGCS_TA_HORZ_LEFT },
	{ _T("SystemCurrency"),	  100, __EGCS_TA_HORZ_LEFT },
};

LONG nColNo, nColCount = sizeof(g_arrHeaderDefinitions) / sizeof(g_arrHeaderDefinitions[0]);
	m_wndGrid.ColumnAdd( nColCount, false );
	for( nColNo = 0L; nColNo < nColCount; nColNo++ )
	{
		CExtGridCellHeader * pCellHeaderAtTop =
			STATIC_DOWNCAST(
				CExtGridCellHeader,
				m_wndGrid.GridCellGetOuterAtTop(
					nColNo,
					0L,
					RUNTIME_CLASS(CExtGridCellHeader)
					)
				);
		ASSERT_VALID( pCellHeaderAtTop );
		CExtGridCellHeader * pCellHeaderAtBottom =
			STATIC_DOWNCAST(
				CExtGridCellHeader,
				m_wndGrid.GridCellGetOuterAtBottom(
					nColNo,
					0L,
					RUNTIME_CLASS(CExtGridCellHeader)
					)
				);
		ASSERT_VALID( pCellHeaderAtBottom );
		pCellHeaderAtTop	-> TextSet( g_arrHeaderDefinitions[nColNo].m_strText );
		pCellHeaderAtBottom	-> TextSet( g_arrHeaderDefinitions[nColNo].m_strText );
		pCellHeaderAtTop	-> ExtentSet( g_arrHeaderDefinitions[nColNo].m_nWidth );
		pCellHeaderAtBottom	-> ExtentSet( g_arrHeaderDefinitions[nColNo].m_nWidth );
		pCellHeaderAtTop	-> ModifyStyle( g_arrHeaderDefinitions[nColNo].m_dwStyle|__EGCS_HDR_FOCUS_ARROW_DISPLAY );
		pCellHeaderAtBottom	-> ModifyStyle( g_arrHeaderDefinitions[nColNo].m_dwStyle|__EGCS_HDR_FOCUS_ARROW_DISPLAY );
		pCellHeaderAtTop	-> FontWeightSet( FW_BOLD );
		pCellHeaderAtBottom	-> FontWeightSet( FW_BOLD );
		//pCellHeaderAtTop	-> ModifyStyleEx( __EGCS_EX_AUTO_BEST_FIT ); // auto-best-fit
	} // for( nColNo = 0L; nColNo < nColCount; nColNo++ )

LONG nRowNo, nRowCount = 10;
	m_wndGrid.RowAdd( 10, false );
	for( nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellHeader * pCellLeft =
			STATIC_DOWNCAST(
				CExtGridCellHeader,
				m_wndGrid.GridCellGetOuterAtLeft(
					0L,
					nRowNo,
					RUNTIME_CLASS(CExtGridCellHeader)
					)
				);
		pCellLeft->ExtentSet( 20 );
		pCellLeft->ModifyStyle(
			__EGCS_HDR_FOCUS_ARROW_RESERVE_SPACE
				|__EGCS_HDR_FOCUS_ARROW_DISPLAY
				|__EGCS_TA_HORZ_RIGHT
			);
		CExtGridCellHeader * pCellRight =
			STATIC_DOWNCAST(
				CExtGridCellHeader,
				m_wndGrid.GridCellGetOuterAtRight(
					0L,
					nRowNo,
					RUNTIME_CLASS(CExtGridCellHeader)
					)
				);
		pCellRight->ModifyStyle(
			__EGCS_HDR_FOCUS_ARROW_RESERVE_SPACE
				|__EGCS_HDR_FOCUS_ARROW_DISPLAY
				|__EGCS_TA_HORZ_RIGHT
			);
	}

	m_wndGrid.GridIconRemove();
CBitmap bmpIcons;
	VERIFY( bmpIcons.LoadBitmap( IDB_BITMAP_GRID_ICONS ) );
BITMAP bmpInfo;
	::memset( &bmpInfo, 0, sizeof(BITMAP) );
	bmpIcons.GetBitmap( &bmpInfo );
INT nIconWidth = 16;
INT nIconCount = bmpInfo.bmWidth / nIconWidth;
CRect rcIcon( 0, 0, nIconWidth, bmpInfo.bmHeight );
	for( INT nIconIndex = 0; nIconIndex < nIconCount; nIconIndex++, rcIcon.OffsetRect(nIconWidth,0) )
	{
		CExtCmdIcon _icon;
		_icon.AssignFromHBITMAP(
			(HBITMAP)bmpIcons.GetSafeHandle(),
			RGB(255,0,255),
			&rcIcon
			);
		ASSERT( !_icon.IsEmpty() );
		m_wndGrid.GridIconInsert( &_icon );
	}

nColNo = 0L;
	_InitColumnBoolean( nColNo++, nRowCount );
	_InitColumnCheckBox( nColNo++, nRowCount );
	_InitColumnRadioButton( nColNo++, nRowCount );
	_InitColumnColor( nColNo++, nRowCount );
	_InitColumnComboBox( nColNo++, nRowCount );
	_InitColumnCheckComboBox( nColNo++, nRowCount );
	_InitColumnDateTime( nColNo++, nRowCount );
	_InitColumnDuration( nColNo++, nRowCount );
	_InitColumnProgress( nColNo++, nRowCount );
	_InitColumnHyperLink( nColNo++, nRowCount );
	_InitColumnIPAddress( nColNo++, nRowCount );
	_InitColumnNumber( nColNo++, nRowCount );
	_InitColumnCurrency( nColNo++, nRowCount );
	_InitColumnScientific( nColNo++, nRowCount );
	_InitColumnFraction( nColNo++, nRowCount );
	_InitColumnPercentage( nColNo++, nRowCount );
	_InitColumnVariant( nColNo++, nRowCount );
	_InitColumnUpDown( nColNo++, nRowCount );
	_InitColumnPicture( nColNo++, nRowCount );
	_InitColumnSlider( nColNo++, nRowCount );
	_InitColumnText( nColNo++, nRowCount );
	_InitColumnPassword( nColNo++, nRowCount );
	_InitColumnFile( nColNo++, nRowCount );
	_InitColumnFileImage( nColNo++, nRowCount );
	_InitColumnFolder( nColNo++, nRowCount );
	_InitColumnFont( nColNo++, nRowCount );
	_InitColumnFontFaceName( nColNo++, nRowCount );
	_InitColumnFontWeight( nColNo++, nRowCount );
	_InitColumnColorPart( nColNo++, nRowCount );
	_InitColumnRectPart( nColNo++, nRowCount );
	_InitColumnInplaceSlider( nColNo++, nRowCount );
	_InitColumnInplaceScrollBar( nColNo++, nRowCount );
	_InitColumnHotKey( nColNo++, nRowCount );
	_InitColumnButton( nColNo++, nRowCount );
	_InitColumnObjectSize( nColNo++, nRowCount );
	_InitColumnGeographic( nColNo++, nRowCount );
	_InitColumnStringMasked( nColNo++, nRowCount );
	_InitColumnSystemNumber( nColNo++, nRowCount );
	_InitColumnSystemCurrency( nColNo++, nRowCount );

	m_wndGrid.OnSwUpdateScrollBars();
	m_wndGrid.OnSwInvalidate( false );

	m_wndCellBoolean.SetTooltipText( 
		_T("The CExtGridCellBool class implements a boolean cell.") 
		);
 	m_wndCellCheckBox.SetTooltipText( 
		_T("The CExtGridCellCheckBox class implements a three-state check box button cell.") 
		);
	m_wndCellRadioButton.SetTooltipText( 
		_T("The CExtGridCellRadioButton class implements a radio button cell.") 
		);
 	m_wndCellColor.SetTooltipText( 
		_T("The CExtGridCellColor class implements a simple color cell that is able to track the \n")
		_T("CExtPopupColorMenuWnd color picker menu when the built-in button is clicked.\n") 
		_T("It also allows you to display the CExtColorDlg color picker dialog.") 
		);
	m_wndCellComboBox.SetTooltipText( 
		_T("The CExtGridCellComboBox class implements a drop-down combo box cell.\n")
		_T("The class implements the most useful methods of the CComboBox class.") 
		);
 	m_wndCellCheckComboBox.SetTooltipText( 
		_T("The CExtGridCellCheckListComboBox class implements \n")
		_T("a combo box cell with a pop-up list box that contains items \n")
		_T("with a check box next to each item in the list.") 
		);
 	m_wndCellDateTime.SetTooltipText( 
		_T("The CExtGridCellDateTime class implements the date and time cell.\n") 
		_T("This cell displays hours, minutes, seconds and a time designator in its time part and day, \n")
		_T("month and year in its date part. \n")
		_T("You can use different modes of the cell (e.g., by displaying only date, only time or date \n")
		_T("and time together). \n")
		_T("With the Drop Down button you can invoke the embedded pop-up date picker \n")
		_T("for selecting dates in a handy way.") 
		);
	m_wndCellDuration.SetTooltipText( 
		_T("The CExtGridCellDuration class implements a duration cell.") 
		);
	m_wndCellProgress.SetTooltipText( 
		_T("The CExtGridCellProgress class implements a progress bar inside a cell. \n")
		_T("In addition to all the features of the standard progress bar control, \n")
		_T("the progress cell supports the following text formats:\n")
		_T("  -  No text \n")
		_T("  -  Custom text \n")
		_T("  -  Current position in percents \n")
		_T("  -  Current position \n")
		_T("  -  Custom text and current position in percents \n")
		_T("  -  Custom text and current position \n")
		_T("You can also make the progress bar grow in a reverse direction \n")
		_T("(from right to left) and vertically, make the text tied, and specify any \n")
		_T("color both for the text and for the progress bar.") 
		);
	m_wndCellHyperlink.SetTooltipText( 
		_T("The CExtGridCellHyperLink class implements a hyperlink cell.") 
		);
	m_wndCellIPAddress.SetTooltipText( 
		_T("The CExtGridCellIPAddress class implements an IP address cell.") 
		);
	m_wndCellNumber.SetTooltipText( 
		_T("The CExtGridCellNumber class implements a numeric cell. \n")
		_T("By default, the numeric value is formatted according to the locale's settings \n")
		_T("but you can specify a custom format by modifying the following properties:\n")
		_T("  -  Number of fractional digits \n")
		_T("  -  Number of leading zeroes \n")
		_T("  -  Format for negative values \n")
		_T("  -  Size of each group of digits to the left of the decimal point \n")
		_T("  -  Decimal separator \n")
		_T("  -  Thousand separator ") 
		);
	m_wndCellCurrency.SetTooltipText( 
		_T("The CExtGridCellCurrency class implements a currency cell.\n")
		_T("By default, the currency value is formatted according to the locale's settings \n")
		_T("but you can specify a custom format by modifying the following properties:\n")
		_T("  -  Monetary symbol and its position \n")
		_T("  -  Number of fractional digits \n")
		_T("  -  Number of leading zeroes \n")
		_T("  -  Format for negative values \n")
		_T("  -  Format for positive values \n")
		_T("  -  Size of each group of digits to the left of the decimal point \n")
		_T("  -  Decimal separator \n")
		_T("  -  Thousand separator \n")
		);
 	m_wndCellScientific.SetTooltipText( 
		_T("The CExtGridCellScientific class implements an exponential cell, \n")
		_T("which displays a floating-point number in the exponential form.") 
		);
	m_wndCellFraction.SetTooltipText( 
		_T("The CExtGridCellFraction class implements a fraction cell, \n")
		_T("which displays the cell floating-point value as a fraction. \n")
		_T("You can customize fraction representation using several predefined constants.") 
		);
	m_wndCellPercentage.SetTooltipText( 
		_T("The CExtGridCellPercentage class implements a percentage cell: \n")
		_T("the cell value is multiplied by 100 and the result is displayed with a percent symbol. \n")
		_T("You can additionally specify a position for the percent sign.") 
		);
	m_wndCellVariant.SetTooltipText( 
		_T("The CExtGridCellVariant class implements \n")
		_T("the cell which stores a value of the VARIANT type.") 
		);
	m_wndCellUpDown.SetTooltipText( 
		_T("The CExtGridCellUpDown class implements a spin edit cell.") 
		);
	m_wndCellPicture.SetTooltipText( 
		_T("The CExtGridCellPicture class is designed for displaying a bitmap in a cell. \n")
		_T("The bitmap can be aligned, tiled, or stretched \n")
		_T("The class is based on CExtBitmap, which allows you to use 32-bit bitmaps \n")
		_T("with alpha channel.") 
		);
	m_wndCellSlider.SetTooltipText( 
		_T("The CExtGridCellSlider class implements a slider cell, \n")
		_T("which is useful when you want the user to select a discrete \n")
		_T("value or a set of consecutive values in a range.") 
		);
	m_wndCellText.SetTooltipText( 
		_T("The CExtGridCellString class implements a text cell \n")
		_T("which keeps text data in the CString property.\n")
		_T("The text can be single line or multi-line\n")
		);
	m_wndCellPassword.SetTooltipText( 
		_T("The CExtGridCellPassword class implements a password cell.") 
		);
 	m_wndCellFile.SetTooltipText( 
		_T("The CExtGridCellFile class implements a cell which stores \n")
		_T("the path to a particular file. \n")
		_T("You can set the path by typing it or with the Open dialog \n")
		_T("that pops up when you click the cell ellipsis button.") 
		);
 	m_wndCellFileImage.SetTooltipText( 
		_T("The CExtGridCellFileImage class implements a cell which stores \n")
		_T("the path to a particular icon file and displays its thumbnail image. \n")
		_T("You can set the path by typing it or with the Open dialog \n")
		_T("that pops up when you click the cell ellipsis button.") 
		);
	m_wndCellFolder.SetTooltipText( 
		_T("The CExtGridCellFolder class implements a cell which \n")
		_T("stores the path to a particular folder. \n")
		_T("With the Browse button you can display a folder-selection dialog box \n")
		_T("which allows you to select a particular folder.") 
		);
	m_wndCellFont.SetTooltipText( 
		_T("The CExtGridCellFont class implements a font cell which stores \n")
		_T("the LOGFONT structure that contains information about the current font. \n")
		_T("You can select a particular font and set its attributes with the Font dialog \n")
		_T("that pops up when you click the cell ellipsis button.") 
		);
	m_wndCellFontFaceName.SetTooltipText( 
		_T("The CExtGridCellFontFaceName class implements \n")
		_T("an extended version of the combo box grid cell for displaying \n")
		_T("the list of the fonts installed in the system. \n")
		);
	m_wndCellFontWeight.SetTooltipText( 
		_T("The CExtGridCellUpDownFontWeight class implements an up-down \n")
		_T("grid cell for selecting the font weight. \n")
		_T("This cell displays a drop-down list box with a list of \n")
		_T("commonly used font weights including: \n")
		_T("  -  100 (Thin), \n")
		_T("  -  200 (Ultra Light), \n")
		_T("  -  300 (Light), \n")
		_T("  -  400 (Normal), \n")
		_T("  -  500 (Medium), \n")
		_T("  -  600 (Semi Bold), \n")
		_T("  -  700 (Bold), \n")
		_T("  -  800 (Ultra Bold), \n")
		_T("  -  900 (Heavy)")
		);
	m_wndCellColorPart.SetTooltipText( 
		_T("The CExtGridCellUpDownColorPart class implements an extended version \n")
		_T("of the up-down grid cell for displaying the red, greed and blue part of a color value. \n")
		_T("This cell class features a small color icon and a drop-down menu. \n")
		_T("The latter is handy editing color values.") 
		);
	m_wndCellRectPart.SetTooltipText( 
		_T("The CExtGridCellUpDownRectPart class implements an extended version \n")
		_T("of an up-down grid cell for displaying X, Y, Width and Height parts \n")
		_T("of the rectangle data type.") 
		);
	m_wndCellInplaceSlider.SetTooltipText( 
		_T("The CExtGridCellInplaceSlider class implements \n")
		_T("an always visible slider/scroll bar in a grid cell.") 
		);
 	m_wndCellInplaceScrollBar.SetTooltipText( 
		_T("The CExtGridCellInplaceSlider class implements \n")
		_T("an always visible slider/scroll bar in a grid cell.") 
		);
 	m_wndCellHotKey.SetTooltipText( 
		_T("The CExtGridCellHotKey class implements a hot key in a grid cell. \n")
		_T("It displays a text representation of the key combination the user types into it.") 
		);
 	m_wndCellButton.SetTooltipText( 
		_T("The CExtGridCellButton class implements a button in a grid cell. \n")
		);
 	m_wndCellObjectSize.SetTooltipText( 
		_T("The CExtGridCellObjectSize class implements an object size grid cell. \n")
		);
 	m_wndCellGeographic.SetTooltipText( 
		_T("The CExtGridCellLL class implements an longitude/latitude grid cell. \n")
		);
 	m_wndCellMaskedString.SetTooltipText( 
		_T("The CExtGridCellStringMasked class implements a text grid cell\nwith masked input validation. \n")
		);
 	m_wndCellSystemNumber.SetTooltipText( 
		_T("The CExtGridCellSystemNumber class implements a numeric value input grid cell\ndisplaying number formatted according Windows locale settings. \n")
		);
 	m_wndCellSystemCurrency.SetTooltipText( 
		_T("The CExtGridCellSystemCurrency class implements a currency value input grid cell\ndisplaying currency formatted according Windows locale settings. \n")
		);

	SetTimer( 987, 200, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageGrid::_InitColumnBoolean( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellBool * pCellBool =
			STATIC_DOWNCAST(
				CExtGridCellBool,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellBool)
					)
				);
		pCellBool->DataSet( false );
	}

	CExtGridCellBool * pCellBool0 =
		STATIC_DOWNCAST(
			CExtGridCellBool,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellBool0->DataSet( true );

	CExtGridCellBool * pCellBool1 =
		STATIC_DOWNCAST(
			CExtGridCellBool,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellBool1->DataSet( false );
	pCellBool1->ModifyStyle( __EGCS_BUTTON_UPDOWN, __EGCS_BUTTON_DROPDOWN );

	CExtGridCellBool * pCellBool2 =
		STATIC_DOWNCAST(
			CExtGridCellBool,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellBool2->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
	pCellBool2->DataSet( true );
	pCellBool2->LabelTextSet( true, _T("Yes") );
	pCellBool2->LabelTextSet( false, _T("No") );
	pCellBool2->ModifyStyle(
		__EGCS_CHK_CHECK
			|__EGCS_ICA_HORZ_CENTER
			|__EGCS_ICA_VERT_MIDDLE
			|__EGCS_NO_INPLACE_CONTROL
			|__EGCS_BUTTON_UPDOWN
		);
	pCellBool2->TextColorSet( CExtGridCell::__ECS_ALL, RGB(255,0,0) );

	CExtGridCellBool * pCellBool3 =
		STATIC_DOWNCAST(
			CExtGridCellBool,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellBool3->TextColorSet( CExtGridCell::__ECS_NORMAL, RGB(0,255,0) );

	CExtGridCellBool * pCellBool4 =
		STATIC_DOWNCAST(
			CExtGridCellBool,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellBool4->DataSet( true );
	pCellBool4->LabelTextSet( true, _T("Available") );
	pCellBool4->LabelTextSet( false, _T("Unavailable") );
	pCellBool4->ModifyStyle( __EGCS_BUTTON_UPDOWN, __EGCS_BUTTON_DROPDOWN );
	pCellBool4->TextColorSet( CExtGridCell::__ECS_NORMAL, RGB(0,0,255) );
	pCellBool4->TextColorSet( CExtGridCell::__ECS_HOVERED, RGB(255,0,255) );

	CExtGridCellBool * pCellBool5 =
		STATIC_DOWNCAST(
			CExtGridCellBool,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellBool5->ModifyStyle( __EGCS_TA_HORZ_CENTER );

	CExtGridCellBool * pCellBool6 =
		STATIC_DOWNCAST(
			CExtGridCellBool,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellBool6->ModifyStyle( __EGCS_TA_HORZ_RIGHT );

	CExtGridCellBool * pCellBool7 =
		STATIC_DOWNCAST(
			CExtGridCellBool,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellBool7->DataSet( true );
	pCellBool7->LabelTextSet( true, _T("") );
	pCellBool7->LabelTextSet( false, _T("") );
	pCellBool7->ModifyStyle(
		__EGCS_CHK_CHECK
			|__EGCS_ICA_HORZ_CENTER
			|__EGCS_ICA_VERT_MIDDLE
			|__EGCS_NO_INPLACE_CONTROL
			,
		__EGCS_BUTTON_DROPDOWN
		);

	CExtGridCellBool * pCellBool8 =
		STATIC_DOWNCAST(
			CExtGridCellBool,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellBool8->ModifyStyle(
		__EGCS_CHK_CHECK
			|__EGCS_ICA_HORZ_CENTER
			|__EGCS_ICA_VERT_MIDDLE
			|__EGCS_NO_INPLACE_CONTROL
			|__EGCS_BUTTON_UPDOWN
			,
		__EGCS_BUTTON_DROPDOWN
		);
	pCellBool8->LabelTextSet( true, _T("") );
	pCellBool8->LabelTextSet( false, _T("") );

	CExtGridCellBool * pCellBool9 =
		STATIC_DOWNCAST(
			CExtGridCellBool,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellBool9->DataSet( true );
}

void CPageGrid::_InitColumnCheckBox( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		if( nRowNo == 1 || nRowNo == 3 )
		{
			CExtGridCellCheckBox * pCellCheckBox =
				STATIC_DOWNCAST( CExtGridCellCheckBox, m_wndGrid.GridCellGet( nColNo, nRowNo, 0, 0, RUNTIME_CLASS(CExtGridCellCheckBoxEye) ) );
			ASSERT_VALID( pCellCheckBox );
			pCellCheckBox;
			continue;
		}
		CExtGridCellCheckBox * pCellCheckBox =
			STATIC_DOWNCAST( CExtGridCellCheckBox, m_wndGrid.GridCellGet( nColNo, nRowNo, 0, 0, RUNTIME_CLASS(CExtGridCellCheckBox) ) );
		ASSERT_VALID( pCellCheckBox );
		CString sText;
		sText.Format( _T("Check%d"), nRowNo );
		pCellCheckBox->TextSet( __EXT_MFC_SAFE_LPCTSTR( sText ) );
	}

	CExtGridCellCheckBox * pCellCheckBox0 =
		STATIC_DOWNCAST(
			CExtGridCellCheckBox,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellCheckBox0->SetCheck( 1 );

	CExtGridCellCheckBox * pCellCheckBox1 =
		STATIC_DOWNCAST(
			CExtGridCellCheckBox,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellCheckBox1->Set3StateMode( true );
	pCellCheckBox1->SetCheck( 2 );

	CExtGridCellCheckBox * pCellCheckBox2 =
		STATIC_DOWNCAST(
			CExtGridCellCheckBox,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellCheckBox2->SetAutoTextMode( true );
	pCellCheckBox2->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
	pCellCheckBox2->TextColorSet( CExtGridCell::__ECS_ALL, RGB(255,0,0) );
	pCellCheckBox2->LabelTextSet( 0, _T("Available") );
	pCellCheckBox2->LabelTextSet( 1, _T("Unavailable") );

	CExtGridCellCheckBox * pCellCheckBox3 =
		STATIC_DOWNCAST(
			CExtGridCellCheckBox,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellCheckBox3->TextColorSet( CExtGridCell::__ECS_ALL, RGB(0,255,0) );
	pCellCheckBox3->SetCheck( 1 );

	CExtGridCellCheckBox * pCellCheckBox4 =
		STATIC_DOWNCAST(
			CExtGridCellCheckBox,
			m_wndGrid.GridCellGet( nColNo, 4L )
			); 
	pCellCheckBox4->TextColorSet( CExtGridCell::__ECS_NORMAL, RGB(0,0,255) );
	pCellCheckBox4->TextColorSet( CExtGridCell::__ECS_HOVERED, RGB(255,0,255) );
	pCellCheckBox4->SetAutoTextMode( true );
	pCellCheckBox4->Set3StateMode( true );
	pCellCheckBox4->LabelTextSet( 0, _T("Yes") );
	pCellCheckBox4->LabelTextSet( 1, _T("No") );
	pCellCheckBox4->LabelTextSet( 2, _T("N/A") );
	pCellCheckBox4->SetCheck( 2 );

	CExtGridCellCheckBox * pCellCheckBox5 =
		STATIC_DOWNCAST(
			CExtGridCellCheckBox,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellCheckBox5->ModifyStyle( __EGCS_TA_HORZ_CENTER );

	CExtGridCellCheckBox * pCellCheckBox6 =
		STATIC_DOWNCAST(
			CExtGridCellCheckBox,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellCheckBox6->SetCheck( 1 );
	pCellCheckBox6->ModifyStyle( __EGCS_TA_HORZ_RIGHT );

	CExtGridCellCheckBox * pCellCheckBox7 =
		STATIC_DOWNCAST(
			CExtGridCellCheckBox,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellCheckBox7->Set3StateMode( true );
	pCellCheckBox7->SetCheck( 2 );

	CExtGridCellCheckBox * pCellCheckBox8 =
		STATIC_DOWNCAST(
			CExtGridCellCheckBox,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellCheckBox8->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
	pCellCheckBox8->SetAutoTextMode( true );
	pCellCheckBox8->SetCheck( 1 );

	CExtGridCellCheckBox * pCellCheckBox9 =
		STATIC_DOWNCAST(
			CExtGridCellCheckBox,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellCheckBox9->SetAutoTextMode();
	pCellCheckBox9->Set3StateMode();
	pCellCheckBox9->SetCheck( 0 );
	pCellCheckBox9->LabelTextSet( 0, _T("Limited") );
	pCellCheckBox9->LabelTextSet( 1, _T("Unlimited") );
	pCellCheckBox9->LabelTextSet( 2, _T("Unknown") );
}

void CPageGrid::_InitColumnRadioButton( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellRadioButton * pCellRadioButton =
			STATIC_DOWNCAST(
				CExtGridCellRadioButton,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellRadioButton)
					)
				);
		CString sText;
		sText.Format( _T("Radio%d"), nRowNo );
		pCellRadioButton->TextSet( __EXT_MFC_SAFE_LPCTSTR( sText ) );
	}

	CExtGridCellRadioButton * pCellRadioButton0 =
		STATIC_DOWNCAST(
			CExtGridCellRadioButton,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellRadioButton0->SetCheck( 1 );

	CExtGridCellRadioButton * pCellRadioButton1 =
		STATIC_DOWNCAST(
			CExtGridCellRadioButton,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellRadioButton1->SetCheck( 0 );

	CExtGridCellRadioButton * pCellRadioButton2 =
		STATIC_DOWNCAST(
			CExtGridCellRadioButton,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellRadioButton2->SetCheck( 1 );
	pCellRadioButton2->TextColorSet( CExtGridCell::__ECS_ALL, RGB(255,0,0) );

	CExtGridCellRadioButton * pCellRadioButton3 =
		STATIC_DOWNCAST(
			CExtGridCellRadioButton,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellRadioButton3->TextColorSet( CExtGridCell::__ECS_NORMAL, RGB(0,255,0) );

	CExtGridCellRadioButton * pCellRadioButton4 =
		STATIC_DOWNCAST(
			CExtGridCellRadioButton,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellRadioButton4->TextColorSet( CExtGridCell::__ECS_NORMAL, RGB(0,0,255) );
	pCellRadioButton4->TextColorSet( CExtGridCell::__ECS_HOVERED, RGB(255,0,255) );

	CExtGridCellRadioButton * pCellRadioButton5 =
		STATIC_DOWNCAST(
			CExtGridCellRadioButton,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellRadioButton5->ModifyStyle( __EGCS_TA_HORZ_CENTER );

	CExtGridCellRadioButton * pCellRadioButton6 =
		STATIC_DOWNCAST(
			CExtGridCellRadioButton,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellRadioButton6->ModifyStyle( __EGCS_TA_HORZ_RIGHT );

	CExtGridCellRadioButton * pCellRadioButton7 =
		STATIC_DOWNCAST(
			CExtGridCellRadioButton,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellRadioButton7->SetCheck( 1 );

	CExtGridCellRadioButton * pCellRadioButton8 =
		STATIC_DOWNCAST(
			CExtGridCellRadioButton,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellRadioButton8->SetCheck( 0 );

	CExtGridCellRadioButton * pCellRadioButton9 =
		STATIC_DOWNCAST(
			CExtGridCellRadioButton,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellRadioButton9->SetCheck( 1 );
}

void CPageGrid::_InitColumnColor( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellColor * pCellColor =
			STATIC_DOWNCAST(
				CExtGridCellColor,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellColor)
					)
				);
		pCellColor;
	}

COLORREF clrLimeGreen = RGB(50,205,50);
COLORREF clrYellow = RGB(255,255,0);
COLORREF clrRed = RGB(255,0,0);
COLORREF clrBlue = RGB(0,0,255);

	CExtGridCellColor * pCellColor0 =
		STATIC_DOWNCAST(
			CExtGridCellColor,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellColor0->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
	pCellColor0->SetMode( CExtGridCellColor::eNone );
	pCellColor0->SetColor( clrBlue );
	pCellColor0->m_bEnableButtonDefaultColor = true;
	pCellColor0->m_clrDefault = clrBlue;

	CExtGridCellColor * pCellColor1 =
		STATIC_DOWNCAST(
			CExtGridCellColor,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellColor1->SetMode( CExtGridCellColor::eRGB );
	pCellColor1->SetColor( clrLimeGreen );
	pCellColor1->m_bEnableButtonDefaultColor = true;
	pCellColor1->m_clrDefault = clrLimeGreen;

	CExtGridCellColor * pCellColor2 =
		STATIC_DOWNCAST(
			CExtGridCellColor,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellColor2->SetMode( CExtGridCellColor::eRGBName );
	pCellColor2->SetColor( clrLimeGreen );
	pCellColor2->m_bEnableButtonDefaultColor = true;
	pCellColor2->m_clrDefault = clrLimeGreen;

	CExtGridCellColor * pCellColor3 =
		STATIC_DOWNCAST(
			CExtGridCellColor,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellColor3->SetMode( CExtGridCellColor::eRGBHex );
	pCellColor3->SetColor( clrLimeGreen );
	pCellColor3->m_bEnableButtonRandomColor = true;

	CExtGridCellColor * pCellColor4 =
		STATIC_DOWNCAST(
			CExtGridCellColor,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellColor4->SetMode( CExtGridCellColor::eRGB );
	pCellColor4->SetColor( clrYellow );
	pCellColor4->m_bEnableButtonDefaultColor = true;
	pCellColor4->m_clrDefault = clrYellow;

	CExtGridCellColor * pCellColor5 =
		STATIC_DOWNCAST(
			CExtGridCellColor,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellColor5->SetMode( CExtGridCellColor::eRGBName );
	pCellColor5->SetColor( clrYellow );
	pCellColor5->m_bEnableButtonDefaultColor = true;
	pCellColor5->m_clrDefault = clrYellow;

	CExtGridCellColor * pCellColor6 =
		STATIC_DOWNCAST(
			CExtGridCellColor,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellColor6->SetMode( CExtGridCellColor::eRGBAName );
	pCellColor6->SetColor( clrYellow );
	pCellColor6->m_bEnableButtonRandomColor = true;

	CExtGridCellColor * pCellColor7 =
		STATIC_DOWNCAST(
			CExtGridCellColor,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellColor7->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
	pCellColor7->SetMode( CExtGridCellColor::eNone );
	pCellColor7->m_bEnableButtonDefaultColor = true;
	pCellColor7->m_clrDefault = clrRed;
	pCellColor7->SetColor( clrRed );

	CExtGridCellColor * pCellColor8 =
		STATIC_DOWNCAST(
			CExtGridCellColor,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellColor8->SetMode( CExtGridCellColor::eRGBHex );
	pCellColor8->SetColor( clrRed );
	pCellColor8->m_bEnableButtonDefaultColor = true;
	pCellColor8->m_clrDefault = clrRed;

	CExtGridCellColor * pCellColor9 =
		STATIC_DOWNCAST(
			CExtGridCellColor,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellColor9->SetMode( CExtGridCellColor::eCMY );
	pCellColor9->SetColor( clrRed );
	pCellColor9->m_bEnableButtonRandomColor = true;
}

void CPageGrid::_InitColumnComboBox( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellComboBox * pCellComboBox =
			STATIC_DOWNCAST(
				CExtGridCellComboBox,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellComboBox)
					)
				);
		pCellComboBox;
	}

	CExtImageList ilComboCell;
	ilComboCell.Load( IDB_COMBO_CELL_IMAGES, RGB(255,0,255), 16 );
	
	LONG nItem;
	CExtGridCellComboBox::ITEM_INFO itemInfo;

	CExtGridCellComboBox * pCellComboBox0 =
		STATIC_DOWNCAST(
			CExtGridCellComboBox,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellComboBox0->SetEnumMode( true );
	pCellComboBox0->AddString( _T("Item1") );
	pCellComboBox0->AddString( _T("Item2") );
	pCellComboBox0->AddString( _T("Item3") );
	pCellComboBox0->AddString( _T("Item4") );
	pCellComboBox0->AddString( _T("Item5") );
	pCellComboBox0->AddString( _T("Item6") );
	pCellComboBox0->AddString( _T("Item7") );
	pCellComboBox0->SetCurSel( 2 );
	pCellComboBox0->TextColorSet( CExtGridCell::__ECS_ALL, RGB(255,0,0) );
	//pCellComboBox0->SortItems( false );

	CExtGridCellComboBox * pCellComboBox1 =
		STATIC_DOWNCAST(
			CExtGridCellComboBox,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	nItem = pCellComboBox1->AddString( _T("Item1") );
	pCellComboBox1->SetItemIndent( nItem, 0 );

	nItem = pCellComboBox1->AddString( _T("Item2") );
	pCellComboBox1->SetItemIndent( nItem, 1 );

	nItem = pCellComboBox1->AddString( _T("Item3") );
	pCellComboBox1->SetItemIndent( nItem, 2 );
	
	nItem = pCellComboBox1->AddString( _T("Item4") );
	pCellComboBox1->SetItemIndent( nItem, 0 );
	
	nItem = pCellComboBox1->AddString( _T("Item5") );
	pCellComboBox1->SetItemIndent( nItem, 1 );
	
	nItem = pCellComboBox1->AddString( _T("Item6") );
	pCellComboBox1->SetItemIndent( nItem, 2 );
	
	nItem = pCellComboBox1->AddString( _T("Item7") );
	pCellComboBox1->SetItemIndent( nItem, 3 );
	
	pCellComboBox1->SetCurSel( 4 );

	pCellComboBox1->TextColorSet( CExtGridCell::__ECS_ALL, RGB(0,255,0) );

	CExtGridCellComboBox * pCellComboBox2 =
		STATIC_DOWNCAST(
			CExtGridCellComboBox,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellComboBox2->SetEnumMode( true );
	pCellComboBox2->SetImageList( &ilComboCell );
	
	nItem = pCellComboBox2->AddString( _T("Item1") );
	pCellComboBox2->SetItemImages( nItem, 1, 1, 1 );
	
	nItem = pCellComboBox2->AddString( _T("Item2") );
	pCellComboBox2->SetItemImages( nItem, 2, 2, 2 );
	
	nItem = pCellComboBox2->AddString( _T("Item3") );
	pCellComboBox2->SetItemImages( nItem, 3, 3, 3 );
	
	nItem = pCellComboBox2->AddString( _T("Item4") );
	pCellComboBox2->SetItemImages( nItem, 4, 4, 4 );
	
	nItem = pCellComboBox2->AddString( _T("Item5") );
	pCellComboBox2->SetItemImages( nItem, 5, 5, 5 );
	
	nItem = pCellComboBox2->AddString( _T("Item6") );
	pCellComboBox2->SetItemImages( nItem, 6, 6, 6 );
	
	nItem = pCellComboBox2->AddString( _T("Item7") );
	pCellComboBox2->SetItemImages( nItem, 7, 7, 7 );
	
	pCellComboBox2->SetCurSel( 1 );

	CExtGridCellComboBox * pCellComboBox3 =
		STATIC_DOWNCAST(
			CExtGridCellComboBox,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellComboBox3->SetImageList( &ilComboCell );
	
	nItem = pCellComboBox3->AddString( _T("Item1") );
	pCellComboBox3->SetItemIndent( nItem, 0 );
	pCellComboBox3->SetItemImages( nItem, 1, 1, 1 );
	
	nItem = pCellComboBox3->AddString( _T("Item2") );
	pCellComboBox3->SetItemIndent( nItem, 1 );
	pCellComboBox3->SetItemImages( nItem, 2, 2, 2 );
	
	nItem = pCellComboBox3->AddString( _T("Item3") );
	pCellComboBox3->SetItemIndent( nItem, 2 );
	pCellComboBox3->SetItemImages( nItem, 3, 3, 3 );
	
	nItem = pCellComboBox3->AddString( _T("Item4") );
	pCellComboBox3->SetItemIndent( nItem, 0 );
	pCellComboBox3->SetItemImages( nItem, 4, 4, 4 );
	
	nItem = pCellComboBox3->AddString( _T("Item5") );
	pCellComboBox3->SetItemIndent( nItem, 1 );
	pCellComboBox3->SetItemImages( nItem, 5, 5, 5 );
	
	nItem = pCellComboBox3->AddString( _T("Item6") );
	pCellComboBox3->SetItemIndent( nItem, 2 );
	pCellComboBox3->SetItemImages( nItem, 6, 6, 6 );
	
	nItem = pCellComboBox3->AddString( _T("Item7") );
	pCellComboBox3->SetItemIndent( nItem, 3 );
	pCellComboBox3->SetItemImages( nItem, 7, 7, 7 );
	
	pCellComboBox3->SetCurSel( 4 );

	CExtGridCellComboBox * pCellComboBox4 =
		STATIC_DOWNCAST(
			CExtGridCellComboBox,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellComboBox4->SetEnumMode( true );
	pCellComboBox4->SetImageList( &ilComboCell );
	pCellComboBox4->ModifyStyle( __EGCS_BUTTON_UPDOWN );
	pCellComboBox4->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );

	itemInfo.m_sString = _T("Item1");
	itemInfo.m_nImage = 0;
	itemInfo.m_nSelectedImage = 1;
	itemInfo.m_nOverlayImage = 2;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item2");
	itemInfo.m_nImage = 1;
	itemInfo.m_nSelectedImage = 2;
	itemInfo.m_nOverlayImage = 3;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item3");
	itemInfo.m_nImage = 2;
	itemInfo.m_nSelectedImage = 3;
	itemInfo.m_nOverlayImage = 4;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item4");
	itemInfo.m_nImage = 3;
	itemInfo.m_nSelectedImage = 4;
	itemInfo.m_nOverlayImage = 5;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item5");
	itemInfo.m_nImage = 4;
	itemInfo.m_nSelectedImage = 5;
	itemInfo.m_nOverlayImage = 6;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item6");
	itemInfo.m_nImage = 5;
	itemInfo.m_nSelectedImage = 6;
	itemInfo.m_nOverlayImage = 7;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item7");
	itemInfo.m_nImage = 6;
	itemInfo.m_nSelectedImage = 7;
	itemInfo.m_nOverlayImage = 8;
	pCellComboBox4->AddItem( &itemInfo );
	
	pCellComboBox4->SetCurSel( 6 );

	CExtGridCellComboBox * pCellComboBox5 =
		STATIC_DOWNCAST(
			CExtGridCellComboBox,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellComboBox5->SetImageList( &ilComboCell );
	
	nItem = pCellComboBox5->AddString( _T("Item1") );
	pCellComboBox5->SetItemIndent( nItem, 0 );
	pCellComboBox5->SetItemImages( nItem, 3, 7, 9 );
	
	nItem = pCellComboBox5->AddString( _T("Item2") );
	pCellComboBox5->SetItemIndent( nItem, 1 );
	pCellComboBox5->SetItemImages( nItem, 4, 2, 0 );
	
	nItem = pCellComboBox5->AddString( _T("Item3") );
	pCellComboBox5->SetItemIndent( nItem, 2 );
	pCellComboBox5->SetItemImages( nItem, 5, 4, 11 );
	
	nItem = pCellComboBox5->AddString( _T("Item4") );
	pCellComboBox5->SetItemIndent( nItem, 3 );
	pCellComboBox5->SetItemImages( nItem, 6, 1, 3 );
	
	nItem = pCellComboBox5->AddString( _T("Item5") );
	pCellComboBox5->SetItemIndent( nItem, 4 );
	pCellComboBox5->SetItemImages( nItem, 7, 9, 2 );
	
	nItem = pCellComboBox5->AddString( _T("Item6") );
	pCellComboBox5->SetItemIndent( nItem, 5 );
	pCellComboBox5->SetItemImages( nItem, 8, 10, 5 );
	
	nItem = pCellComboBox5->AddString( _T("Item7") );
	pCellComboBox5->SetItemIndent( nItem, 6 );
	pCellComboBox5->SetItemImages( nItem, 9, 11, 6 );
	
	pCellComboBox5->SetCurSel( 3 );

	CExtGridCellComboBox * pCellComboBox6 =
		STATIC_DOWNCAST(
			CExtGridCellComboBox,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellComboBox6->SetEnumMode( true );
	pCellComboBox6->AddString( _T("Item1") );
	pCellComboBox6->AddString( _T("Item2") );
	pCellComboBox6->AddString( _T("Item3") );
	pCellComboBox6->AddString( _T("Item4") );
	pCellComboBox6->AddString( _T("Item5") );
	pCellComboBox6->AddString( _T("Item6") );
	pCellComboBox6->AddString( _T("Item7") );
	pCellComboBox6->SetCurSel( 6 );
	pCellComboBox6->ModifyStyle( __EGCS_BUTTON_UPDOWN );

	CExtGridCellComboBox * pCellComboBox7 =
		STATIC_DOWNCAST(
			CExtGridCellComboBox,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellComboBox7->SetEnumMode( true );
	pCellComboBox7->AddString( _T("Item1") );
	pCellComboBox7->AddString( _T("Item2") );
	pCellComboBox7->AddString( _T("Item3") );
	pCellComboBox7->AddString( _T("Item4") );
	pCellComboBox7->AddString( _T("Item5") );
	pCellComboBox7->AddString( _T("Item6") );
	pCellComboBox7->AddString( _T("Item7") );
	pCellComboBox7->SetCurSel( 4 );
	pCellComboBox7->ModifyStyle( __EGCS_BUTTON_UPDOWN, __EGCS_BUTTON_DROPDOWN );

	CExtGridCellComboBox * pCellComboBox8 =
		STATIC_DOWNCAST(
			CExtGridCellComboBox,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellComboBox8->TextSet( _T("Custom Text") );
	pCellComboBox8->AddString( _T("Item1") );
	pCellComboBox8->AddString( _T("Item2") );
	pCellComboBox8->AddString( _T("Item3") );
	pCellComboBox8->AddString( _T("Item4") );
	pCellComboBox8->AddString( _T("Item5") );
	pCellComboBox8->AddString( _T("Item6") );
	pCellComboBox8->AddString( _T("Item7") );

	CExtGridCellComboBox * pCellComboBox9 =
		STATIC_DOWNCAST(
			CExtGridCellComboBox,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellComboBox9->TextSet( _T("Empty") );
}

void CPageGrid::_InitColumnCheckComboBox( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellCheckListComboBox * pCellComboBox =
			STATIC_DOWNCAST(
				CExtGridCellCheckListComboBox,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellCheckListComboBox)
					)
				);
		pCellComboBox;
	}

	CExtImageList ilComboCell;
	ilComboCell.Load( IDB_COMBO_CELL_IMAGES, RGB(255,0,255), 16 );
	
	LONG nItem = 0L;
	CExtGridCellComboBox::ITEM_INFO itemInfo;

	CExtGridCellCheckListComboBox * pCellComboBox0 =
		STATIC_DOWNCAST(
			CExtGridCellCheckListComboBox,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellComboBox0->AddString( _T("Windows 98") );
	pCellComboBox0->AddString( _T("Windows NT") );
	pCellComboBox0->AddString( _T("Windows 2000") );
	pCellComboBox0->AddString( _T("Windows XP") );
	pCellComboBox0->AddString( _T("Windows Vista") );
	pCellComboBox0->ItemCheckSet( 0, 1 );
	pCellComboBox0->ItemCheckSet( 3, 1 );

	CExtGridCellCheckListComboBox * pCellComboBox1 =
		STATIC_DOWNCAST(
			CExtGridCellCheckListComboBox,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	nItem = pCellComboBox1->AddString( _T("Item1") );
	pCellComboBox1->SetItemIndent( nItem, 0 );

	nItem = pCellComboBox1->AddString( _T("Item2") );
	pCellComboBox1->SetItemIndent( nItem, 1 );

	nItem = pCellComboBox1->AddString( _T("Item3") );
	pCellComboBox1->SetItemIndent( nItem, 2 );
	
	nItem = pCellComboBox1->AddString( _T("Item4") );
	pCellComboBox1->SetItemIndent( nItem, 0 );
	
	nItem = pCellComboBox1->AddString( _T("Item5") );
	pCellComboBox1->SetItemIndent( nItem, 1 );
	
	nItem = pCellComboBox1->AddString( _T("Item6") );
	pCellComboBox1->SetItemIndent( nItem, 2 );
	
	nItem = pCellComboBox1->AddString( _T("Item7") );
	pCellComboBox1->SetItemIndent( nItem, 3 );
	
	pCellComboBox1->ItemCheckSet( 0, 1 );
	pCellComboBox1->ItemCheckSet( 4, 1 );

	CExtGridCellCheckListComboBox * pCellComboBox2 =
		STATIC_DOWNCAST(
			CExtGridCellCheckListComboBox,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellComboBox2->SetImageList( &ilComboCell );
	
	nItem = pCellComboBox2->AddString( _T("Item1") );
	pCellComboBox2->SetItemImages( nItem, 1, 1, 1 );
	
	nItem = pCellComboBox2->AddString( _T("Item2") );
	pCellComboBox2->SetItemImages( nItem, 2, 2, 2 );
	
	nItem = pCellComboBox2->AddString( _T("Item3") );
	pCellComboBox2->SetItemImages( nItem, 3, 3, 3 );
	
	nItem = pCellComboBox2->AddString( _T("Item4") );
	pCellComboBox2->SetItemImages( nItem, 4, 4, 4 );
	
	nItem = pCellComboBox2->AddString( _T("Item5") );
	pCellComboBox2->SetItemImages( nItem, 5, 5, 5 );
	
	nItem = pCellComboBox2->AddString( _T("Item6") );
	pCellComboBox2->SetItemImages( nItem, 6, 6, 6 );
	
	nItem = pCellComboBox2->AddString( _T("Item7") );
	pCellComboBox2->SetItemImages( nItem, 7, 7, 7 );
	
	pCellComboBox2->ItemCheckSet( 1, 1 );

	CExtGridCellCheckListComboBox * pCellComboBox3 =
		STATIC_DOWNCAST(
			CExtGridCellCheckListComboBox,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellComboBox3->SetImageList( &ilComboCell );
	
	nItem = pCellComboBox3->AddString( _T("Item1") );
	pCellComboBox3->SetItemIndent( nItem, 0 );
	pCellComboBox3->SetItemImages( nItem, 1, 1, 1 );
	
	nItem = pCellComboBox3->AddString( _T("Item2") );
	pCellComboBox3->SetItemIndent( nItem, 1 );
	pCellComboBox3->SetItemImages( nItem, 2, 2, 2 );
	
	nItem = pCellComboBox3->AddString( _T("Item3") );
	pCellComboBox3->SetItemIndent( nItem, 2 );
	pCellComboBox3->SetItemImages( nItem, 3, 3, 3 );
	
	nItem = pCellComboBox3->AddString( _T("Item4") );
	pCellComboBox3->SetItemIndent( nItem, 0 );
	pCellComboBox3->SetItemImages( nItem, 4, 4, 4 );
	
	nItem = pCellComboBox3->AddString( _T("Item5") );
	pCellComboBox3->SetItemIndent( nItem, 1 );
	pCellComboBox3->SetItemImages( nItem, 5, 5, 5 );
	
	nItem = pCellComboBox3->AddString( _T("Item6") );
	pCellComboBox3->SetItemIndent( nItem, 2 );
	pCellComboBox3->SetItemImages( nItem, 6, 6, 6 );
	
	nItem = pCellComboBox3->AddString( _T("Item7") );
	pCellComboBox3->SetItemIndent( nItem, 3 );
	pCellComboBox3->SetItemImages( nItem, 7, 7, 7 );
	
	pCellComboBox3->ItemCheckSet( 1, 1 );
	pCellComboBox3->ItemCheckSet( 3, 1 );
	pCellComboBox3->ItemCheckSet( 5, 1 );

	CExtGridCellCheckListComboBox * pCellComboBox4 =
		STATIC_DOWNCAST(
			CExtGridCellCheckListComboBox,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellComboBox4->SetImageList( &ilComboCell );
	pCellComboBox4->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );

	itemInfo.m_sString = _T("Item1");
	itemInfo.m_nImage = 0;
	itemInfo.m_nSelectedImage = 1;
	itemInfo.m_nOverlayImage = 2;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item2");
	itemInfo.m_nImage = 1;
	itemInfo.m_nSelectedImage = 2;
	itemInfo.m_nOverlayImage = 3;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item3");
	itemInfo.m_nImage = 2;
	itemInfo.m_nSelectedImage = 3;
	itemInfo.m_nOverlayImage = 4;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item4");
	itemInfo.m_nImage = 3;
	itemInfo.m_nSelectedImage = 4;
	itemInfo.m_nOverlayImage = 5;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item5");
	itemInfo.m_nImage = 4;
	itemInfo.m_nSelectedImage = 5;
	itemInfo.m_nOverlayImage = 6;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item6");
	itemInfo.m_nImage = 5;
	itemInfo.m_nSelectedImage = 6;
	itemInfo.m_nOverlayImage = 7;
	pCellComboBox4->AddItem( &itemInfo );

	itemInfo.m_sString = _T("Item7");
	itemInfo.m_nImage = 6;
	itemInfo.m_nSelectedImage = 7;
	itemInfo.m_nOverlayImage = 8;
	pCellComboBox4->AddItem( &itemInfo );
	
	pCellComboBox4->ItemCheckSet( 5, 1 );
	pCellComboBox4->ItemCheckSet( 6, 1 );

	CExtGridCellCheckListComboBox * pCellComboBox5 =
		STATIC_DOWNCAST(
			CExtGridCellCheckListComboBox,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellComboBox5->SetImageList( &ilComboCell );
	
	nItem = pCellComboBox5->AddString( _T("Item1") );
	pCellComboBox5->SetItemIndent( nItem, 0 );
	pCellComboBox5->SetItemImages( nItem, 3, 7, 9 );
	
	nItem = pCellComboBox5->AddString( _T("Item2") );
	pCellComboBox5->SetItemIndent( nItem, 1 );
	pCellComboBox5->SetItemImages( nItem, 4, 2, 0 );
	
	nItem = pCellComboBox5->AddString( _T("Item3") );
	pCellComboBox5->SetItemIndent( nItem, 2 );
	pCellComboBox5->SetItemImages( nItem, 5, 4, 11 );
	
	nItem = pCellComboBox5->AddString( _T("Item4") );
	pCellComboBox5->SetItemIndent( nItem, 3 );
	pCellComboBox5->SetItemImages( nItem, 6, 1, 3 );
	
	nItem = pCellComboBox5->AddString( _T("Item5") );
	pCellComboBox5->SetItemIndent( nItem, 4 );
	pCellComboBox5->SetItemImages( nItem, 7, 9, 2 );
	
	nItem = pCellComboBox5->AddString( _T("Item6") );
	pCellComboBox5->SetItemIndent( nItem, 5 );
	pCellComboBox5->SetItemImages( nItem, 8, 10, 5 );
	
	nItem = pCellComboBox5->AddString( _T("Item7") );
	pCellComboBox5->SetItemIndent( nItem, 6 );
	pCellComboBox5->SetItemImages( nItem, 9, 11, 6 );
	
	pCellComboBox5->ItemCheckSet( 3, 1 );
	pCellComboBox5->ItemCheckSet( 5, 1 );

	CExtGridCellCheckListComboBox * pCellComboBox6 =
		STATIC_DOWNCAST(
			CExtGridCellCheckListComboBox,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellComboBox6->AddString( _T("Item1") );
	pCellComboBox6->AddString( _T("Item2") );
	pCellComboBox6->AddString( _T("Item3") );
	pCellComboBox6->AddString( _T("Item4") );
	pCellComboBox6->AddString( _T("Item5") );
	pCellComboBox6->AddString( _T("Item6") );
	pCellComboBox6->AddString( _T("Item7") );
	pCellComboBox6->ItemCheckSet( 6, 1 );

	CExtGridCellCheckListComboBox * pCellComboBox7 =
		STATIC_DOWNCAST(
			CExtGridCellCheckListComboBox,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellComboBox7->AddString( _T("Item1") );
	pCellComboBox7->AddString( _T("Item2") );
	pCellComboBox7->AddString( _T("Item3") );
	pCellComboBox7->AddString( _T("Item4") );
	pCellComboBox7->AddString( _T("Item5") );
	pCellComboBox7->AddString( _T("Item6") );
	pCellComboBox7->AddString( _T("Item7") );
	pCellComboBox7->ItemCheckSet( 0, 1 );
	pCellComboBox7->ItemCheckSet( 2, 1 );
	pCellComboBox7->ItemCheckSet( 4, 1 );
	pCellComboBox7->ItemCheckSet( 6, 1 );

	CExtGridCellCheckListComboBox * pCellComboBox8 =
		STATIC_DOWNCAST(
			CExtGridCellCheckListComboBox,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellComboBox8->AddString( _T("Item1") );
	pCellComboBox8->AddString( _T("Item2") );
	pCellComboBox8->AddString( _T("Item3") );
	pCellComboBox8->AddString( _T("Item4") );
	pCellComboBox8->AddString( _T("Item5") );
	pCellComboBox8->AddString( _T("Item6") );
	pCellComboBox8->AddString( _T("Item7") );

	CExtGridCellCheckListComboBox * pCellComboBox9 =
		STATIC_DOWNCAST(
			CExtGridCellCheckListComboBox,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellComboBox9->TextSet( _T("") );
}

void CPageGrid::_InitColumnDateTime( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellDateTime * pCellDateTime =
			STATIC_DOWNCAST(
				CExtGridCellDateTime,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellDateTime)
					)
				);
		pCellDateTime->SetDateTime( COleDateTime::GetCurrentTime() );
	}

	CExtGridCellDateTime * pCellDateTime0 =
		STATIC_DOWNCAST(
			CExtGridCellDateTime,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellDateTime0->SetMode( CExtGridCellDateTime::all );
	pCellDateTime0->SetTimeFormat( CExtGridCellDateTime::automatic );
	pCellDateTime0->ModifyStyle( __EGCS_BUTTON_UPDOWN, __EGCS_BUTTON_ELLIPSIS | __EGCS_BUTTON_DROPDOWN );

	CExtGridCellDateTime * pCellDateTime1 =
		STATIC_DOWNCAST(
			CExtGridCellDateTime,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellDateTime1->SetMode( CExtGridCellDateTime::date );
	pCellDateTime1->SetTimeFormat( CExtGridCellDateTime::automatic );
	pCellDateTime1->ModifyStyle( __EGCS_BUTTON_UPDOWN, __EGCS_BUTTON_ELLIPSIS | __EGCS_BUTTON_DROPDOWN );
	pCellDateTime1->ModifyStyle( __EGCS_READ_ONLY, 0 );
	pCellDateTime1->IconIndexSet( 7 );

	CExtGridCellDateTime * pCellDateTime2 =
		STATIC_DOWNCAST(
			CExtGridCellDateTime,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellDateTime2->SetMode( CExtGridCellDateTime::time );
	pCellDateTime2->SetTimeFormat( CExtGridCellDateTime::automatic );
	pCellDateTime2->ModifyStyle( 0, __EGCS_BUTTON_DROPDOWN );

	CExtGridCellDateTime * pCellDateTime3 =
		STATIC_DOWNCAST(
			CExtGridCellDateTime,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellDateTime3->SetMode( CExtGridCellDateTime::all );
	pCellDateTime3->SetTimeFormat( CExtGridCellDateTime::h24 );

	CExtGridCellDateTime * pCellDateTime4 =
		STATIC_DOWNCAST(
			CExtGridCellDateTime,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellDateTime4->SetMode( CExtGridCellDateTime::all );
	pCellDateTime4->SetTimeFormat( CExtGridCellDateTime::h12 );

	CExtGridCellDateTime * pCellDateTime5 =
		STATIC_DOWNCAST(
			CExtGridCellDateTime,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellDateTime5->SetMode( CExtGridCellDateTime::time );
	pCellDateTime5->SetTimeFormat( CExtGridCellDateTime::h24 );
	pCellDateTime5->ModifyStyle( 0, __EGCS_BUTTON_DROPDOWN );

	CExtGridCellDateTime * pCellDateTime6 =
		STATIC_DOWNCAST(
			CExtGridCellDateTime,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellDateTime6->SetMode( CExtGridCellDateTime::time );
	pCellDateTime6->SetTimeFormat( CExtGridCellDateTime::h12 );
	pCellDateTime6->ModifyStyle( 0, __EGCS_BUTTON_DROPDOWN );

	CExtGridCellDateTime * pCellDateTime7 =
		STATIC_DOWNCAST(
			CExtGridCellDateTime,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellDateTime7->SetMode( CExtGridCellDateTime::date );
	pCellDateTime7->SetTimeFormat( CExtGridCellDateTime::automatic );

	CExtGridCellDateTime * pCellDateTime8 =
		STATIC_DOWNCAST(
			CExtGridCellDateTime,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellDateTime8->SetMode( CExtGridCellDateTime::time );
	pCellDateTime8->SetTimeFormat( CExtGridCellDateTime::automatic );
	pCellDateTime8->ModifyStyle( 0, __EGCS_BUTTON_DROPDOWN );

	CExtGridCellDateTime * pCellDateTime9 =
		STATIC_DOWNCAST(
			CExtGridCellDateTime,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellDateTime9->SetMode( CExtGridCellDateTime::all );
	pCellDateTime9->SetTimeFormat( CExtGridCellDateTime::automatic );
}

void CPageGrid::_InitColumnDuration( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellDuration * pCellDuration =
			STATIC_DOWNCAST(
				CExtGridCellDuration,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellDuration)
					)
				);
		pCellDuration->ModifyStyle( 0, __EGCS_BUTTON_DROPDOWN );
	}

	CExtGridCellDuration * pCellDuration0 =
		STATIC_DOWNCAST(
			CExtGridCellDuration,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellDuration0->DurationSet( 1, 2, 3, 4 );

	CExtGridCellDuration * pCellDuration1 =
		STATIC_DOWNCAST(
			CExtGridCellDuration,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellDuration1->DurationSet( 10, 5, 25, 50 );
	pCellDuration1->ContinuousScrollingSet( true );
	pCellDuration1->ModifyStyle( __EGCS_BUTTON_UPDOWN, __EGCS_BUTTON_ELLIPSIS | __EGCS_BUTTON_DROPDOWN );

	CExtGridCellDuration * pCellDuration2 =
		STATIC_DOWNCAST(
			CExtGridCellDuration,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellDuration2->DurationSet( 2, 10, 0, 0 );

	CExtGridCellDuration * pCellDuration3 =
		STATIC_DOWNCAST(
			CExtGridCellDuration,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellDuration3->DurationSet( 10000L );
	pCellDuration3->ContinuousScrollingSet( true );
	pCellDuration3->ModifyStyle( __EGCS_BUTTON_UPDOWN, __EGCS_BUTTON_ELLIPSIS | __EGCS_BUTTON_DROPDOWN );

	CExtGridCellDuration * pCellDuration4 =
		STATIC_DOWNCAST(
			CExtGridCellDuration,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellDuration4->DurationSet( 10, 1, 0, 0 );

	CExtGridCellDuration * pCellDuration5 =
		STATIC_DOWNCAST(
			CExtGridCellDuration,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellDuration5->DurationSet( COleDateTimeSpan( 5, 0, 0, 0 ) );
	pCellDuration5->ContinuousScrollingSet( true );
	pCellDuration5->ModifyStyle( __EGCS_BUTTON_UPDOWN, __EGCS_BUTTON_ELLIPSIS | __EGCS_BUTTON_DROPDOWN );

	CExtGridCellDuration * pCellDuration6 =
		STATIC_DOWNCAST(
			CExtGridCellDuration,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellDuration6->DurationSet( 365, 12, 0, 0 );

	CExtGridCellDuration * pCellDuration7 =
		STATIC_DOWNCAST(
			CExtGridCellDuration,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellDuration7->DurationSet( 100.9573 );
	pCellDuration7->ContinuousScrollingSet( true );
	pCellDuration7->ModifyStyle( __EGCS_BUTTON_UPDOWN, __EGCS_BUTTON_ELLIPSIS | __EGCS_BUTTON_DROPDOWN );

	CExtGridCellDuration * pCellDuration8 =
		STATIC_DOWNCAST(
			CExtGridCellDuration,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellDuration8->DurationSet( 12345L );

	CExtGridCellDuration * pCellDuration9 =
		STATIC_DOWNCAST(
			CExtGridCellDuration,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellDuration9->DurationSet( COleDateTimeSpan( 5.05 ) );
	pCellDuration9->ContinuousScrollingSet( true );
}

void CPageGrid::_InitColumnHyperLink( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellHyperLink * pCellHyperLink =
			STATIC_DOWNCAST(
				CExtGridCellHyperLink,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellHyperLink)
					)
				);
		UNREFERENCED_PARAMETER(pCellHyperLink);
	}

	CExtGridCellHyperLink * pCellHyperLink0 =
		STATIC_DOWNCAST(
			CExtGridCellHyperLink,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellHyperLink0->TextSet( _T("www.prof-uis.com") );

	CExtGridCellHyperLink * pCellHyperLink1 =
		STATIC_DOWNCAST(
			CExtGridCellHyperLink,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellHyperLink1->SetURL( _T("mailto:info@prof-uis.com") );
	pCellHyperLink1->TextSet( _T("Mail to Prof-UIS Team") );
	pCellHyperLink1->SetAllowVisitedState( FALSE );

	CExtGridCellHyperLink * pCellHyperLink2 =
		STATIC_DOWNCAST(
			CExtGridCellHyperLink,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellHyperLink2->SetURL( _T("www.prof-uis.com") );
	pCellHyperLink2->TextSet( _T("Prof-UIS Home") );
	pCellHyperLink2->SetTextHoverUnderline( FALSE );
	pCellHyperLink2->SetTextColorVisited( RGB(255,0,0) );

	CExtGridCellHyperLink * pCellHyperLink3 =
		STATIC_DOWNCAST(
			CExtGridCellHyperLink,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellHyperLink3->SetURL( _T("http://www.prof-uis.com/elegantgridnet.aspx") );
	pCellHyperLink3->TextSet( _T("Prof-UIS Elegant Grid") );

	CExtGridCellHyperLink * pCellHyperLink4 =
		STATIC_DOWNCAST(
			CExtGridCellHyperLink,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellHyperLink4->SetURL( _T("http://www.prof-uis.com/Overview.aspx?view=FrameFeatures") );
	pCellHyperLink4->TextSet( _T("Prof-UIS Frame Features") );
	pCellHyperLink4->SetTextHoverUnderline( FALSE );

	CExtGridCellHyperLink * pCellHyperLink5 =
		STATIC_DOWNCAST(
			CExtGridCellHyperLink,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellHyperLink5->SetURL( _T("http://www.prof-uis.com/Forum.aspx") );
	pCellHyperLink5->TextSet( _T("Prof-UIS Forum") );
	pCellHyperLink5->SetAllowVisitedState( FALSE );

	CExtGridCellHyperLink * pCellHyperLink6 =
		STATIC_DOWNCAST(
			CExtGridCellHyperLink,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellHyperLink6->SetURL( _T("http://www.prof-uis.com/VersionHistory.aspx") );
	pCellHyperLink6->TextSet( _T("Prof-UIS Version History") );
	pCellHyperLink6->SetTextHoverUnderline( FALSE );

	CExtGridCellHyperLink * pCellHyperLink7 =
		STATIC_DOWNCAST(
			CExtGridCellHyperLink,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellHyperLink7->SetURL( _T("http://www.prof-uis.com/download.aspx") );
	pCellHyperLink7->TextSet( _T("Download") );
	pCellHyperLink7->SetTextHoverUnderline( FALSE );

	CExtGridCellHyperLink * pCellHyperLink8 =
		STATIC_DOWNCAST(
			CExtGridCellHyperLink,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellHyperLink8->SetURL( _T("http://www.prof-uis.com/purchase.aspx") );
	pCellHyperLink8->TextSet( _T("Purchase") );
	pCellHyperLink8->SetTextColorVisited( RGB(0,255,0) );

	CExtGridCellHyperLink * pCellHyperLink9 =
		STATIC_DOWNCAST(
			CExtGridCellHyperLink,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellHyperLink9->SetURL( _T("http://www.prof-uis.com/WhatsNew.aspx") );
	pCellHyperLink9->TextSet( _T("What's New") );
	pCellHyperLink9->SetTextColorVisited( RGB(0,255,255) );
}

void CPageGrid::_InitColumnIPAddress( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellIPAddress * pCellIPAddress =
			STATIC_DOWNCAST(
				CExtGridCellIPAddress,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellIPAddress)
					)
				);
		UNREFERENCED_PARAMETER(pCellIPAddress);
	}

	CExtGridCellIPAddress * pCellIPAddress0 =
		STATIC_DOWNCAST(
			CExtGridCellIPAddress,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellIPAddress0->TextSet( _T("202.212.5.30") );

	CExtGridCellIPAddress * pCellIPAddress1 =
		STATIC_DOWNCAST(
			CExtGridCellIPAddress,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellIPAddress1->SetAddress( 207, 126, 239, 224 );

	CExtGridCellIPAddress * pCellIPAddress2 =
		STATIC_DOWNCAST(
			CExtGridCellIPAddress,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellIPAddress2->TextSet( _T("209.67.206.126") );

	CExtGridCellIPAddress * pCellIPAddress3 =
		STATIC_DOWNCAST(
			CExtGridCellIPAddress,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellIPAddress3->SetAddress( 213, 216, 143, 37 );

	CExtGridCellIPAddress * pCellIPAddress4 =
		STATIC_DOWNCAST(
			CExtGridCellIPAddress,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellIPAddress4->TextSet( _T("62.172.199.20") );

	CExtGridCellIPAddress * pCellIPAddress5 =
		STATIC_DOWNCAST(
			CExtGridCellIPAddress,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellIPAddress5->SetAddress( MAKEIPADDRESS( 62, 27, 59, 245 ) );

	CExtGridCellIPAddress * pCellIPAddress6 =
		STATIC_DOWNCAST(
			CExtGridCellIPAddress,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellIPAddress6->SetAddress( MAKEIPADDRESS( 64, 75, 36, 80 ) );

	CExtGridCellIPAddress * pCellIPAddress7 =
		STATIC_DOWNCAST(
			CExtGridCellIPAddress,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellIPAddress7->TextSet( _T("68.142.195.80") );

	CExtGridCellIPAddress * pCellIPAddress8 =
		STATIC_DOWNCAST(
			CExtGridCellIPAddress,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellIPAddress8->SetAddress( 207, 46, 130, 108 );

	CExtGridCellIPAddress * pCellIPAddress9 =
		STATIC_DOWNCAST(
			CExtGridCellIPAddress,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellIPAddress9->TextSet( _T("195.128.127.200") );
}

void CPageGrid::_InitColumnNumber( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellNumber * pCellNumber =
			STATIC_DOWNCAST(
				CExtGridCellNumber,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellNumber)
					)
				);
		UNREFERENCED_PARAMETER(pCellNumber);
	}

	CExtGridCellNumber * pCellNumber0 =
		STATIC_DOWNCAST(
			CExtGridCellNumber,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellNumber0->_VariantAssign( 0.005 );
	pCellNumber0->SetAutoNumDigits( true );
	pCellNumber0->SetLeadingZero( 0 );

	CExtGridCellNumber * pCellNumber1 =
		STATIC_DOWNCAST(
			CExtGridCellNumber,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellNumber1->_VariantAssign( 123456 );

	CExtGridCellNumber * pCellNumber2 =
		STATIC_DOWNCAST(
			CExtGridCellNumber,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellNumber2->_VariantAssign( 123456.123456789 );
	pCellNumber2->SetAutoNumDigits( true );

	CExtGridCellNumber * pCellNumber3 =
		STATIC_DOWNCAST(
			CExtGridCellNumber,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellNumber3->_VariantAssign( -12345667890L );

	CExtGridCellNumber * pCellNumber4 =
		STATIC_DOWNCAST(
			CExtGridCellNumber,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellNumber4->_VariantAssign( 123456678901L );

	CExtGridCellNumber * pCellNumber5 =
		STATIC_DOWNCAST(
			CExtGridCellNumber,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellNumber5->_VariantAssign( -458.46 );
	pCellNumber5->SetNegativeOrder( 0 ); // Example: (1.1)

	CExtGridCellNumber * pCellNumber6 =
		STATIC_DOWNCAST(
			CExtGridCellNumber,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellNumber6->_VariantAssign( -56789217L );
	pCellNumber6->SetNegativeOrder( 3 ); // Example: 1.1-

	CExtGridCellNumber * pCellNumber7 =
		STATIC_DOWNCAST(
			CExtGridCellNumber,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellNumber7->_VariantAssign( 54761.458 );

	CExtGridCellNumber * pCellNumber8 =
		STATIC_DOWNCAST(
			CExtGridCellNumber,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellNumber8->_VariantAssign( 547614584167L );
	pCellNumber8->SetGrouping( _T("3;2;0") );
	pCellNumber8->SetThousandSeparator( _T("'") );

	CExtGridCellNumber * pCellNumber9 =
		STATIC_DOWNCAST(
			CExtGridCellNumber,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellNumber9->_VariantAssign( 18.0005 );
	pCellNumber9->SetAutoNumDigits( true );
	pCellNumber9->SetDecimalSeparator( _T("^") );
}

void CPageGrid::_InitColumnCurrency( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellCurrency * pCellCurrency =
			STATIC_DOWNCAST(
				CExtGridCellCurrency,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellCurrency)
					)
				);
		UNREFERENCED_PARAMETER(pCellCurrency);
	}

	TCHAR szEuro[2] = { TCHAR(-120), 0 };

	CExtGridCellCurrency * pCellCurrency0 =
		STATIC_DOWNCAST(
			CExtGridCellCurrency,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellCurrency0->_VariantAssign( 0.005 );
	pCellCurrency0->SetLeadingZero( 0 );
	pCellCurrency0->SetNumDigits( 4 );

	CExtGridCellCurrency * pCellCurrency1 =
		STATIC_DOWNCAST(
			CExtGridCellCurrency,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellCurrency1->_VariantAssign( 123456 );
	pCellCurrency1->SetPositiveOrder( 1 ); // Example: 1.1$
	pCellCurrency1->SetCurrencySymbol( _T("$") );
	
	pCellCurrency1->SetAutoNumDigits( false );
	pCellCurrency1->SetAllowTrailingZeroes( true );
	pCellCurrency1->SetNumDigits( 9 );

	CExtGridCellCurrency * pCellCurrency2 =
		STATIC_DOWNCAST(
			CExtGridCellCurrency,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellCurrency2->_VariantAssign( 123456.123456789 );
	pCellCurrency2->SetPositiveOrder( 3 ); // Example: 1.1 $
	pCellCurrency2->SetCurrencySymbol( szEuro );

	CExtGridCellCurrency * pCellCurrency3 =
		STATIC_DOWNCAST(
			CExtGridCellCurrency,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellCurrency3->_VariantAssign( -12345667890L );

	CExtGridCellCurrency * pCellCurrency4 =
		STATIC_DOWNCAST(
			CExtGridCellCurrency,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellCurrency4->_VariantAssign( 123456678901L );

	CExtGridCellCurrency * pCellCurrency5 =
		STATIC_DOWNCAST(
			CExtGridCellCurrency,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellCurrency5->_VariantAssign( -458.46 );
	pCellCurrency5->SetNegativeOrder( 0 ); // Example: ($1.1)
	pCellCurrency5->SetCurrencySymbol( szEuro );

	CExtGridCellCurrency * pCellCurrency6 =
		STATIC_DOWNCAST(
			CExtGridCellCurrency,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellCurrency6->_VariantAssign( -56789217L );
	pCellCurrency6->SetNegativeOrder( 3 ); // Example: $1.1-

	CExtGridCellCurrency * pCellCurrency7 =
		STATIC_DOWNCAST(
			CExtGridCellCurrency,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellCurrency7->_VariantAssign( 54761.458 );
	pCellCurrency7->SetPositiveOrder( 2 ); // Example: $ 1.1
	pCellCurrency7->SetCurrencySymbol( _T("$") );

	CExtGridCellCurrency * pCellCurrency8 =
		STATIC_DOWNCAST(
			CExtGridCellCurrency,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellCurrency8->_VariantAssign( 547614584167L );
	pCellCurrency8->SetGrouping( _T("3;2;0") );
	pCellCurrency8->SetThousandSeparator( _T("'") );

	CExtGridCellCurrency * pCellCurrency9 =
		STATIC_DOWNCAST(
			CExtGridCellCurrency,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellCurrency9->_VariantAssign( 18.0005 );
	pCellCurrency9->SetDecimalSeparator( _T("^") );
	pCellCurrency9->SetCurrencySymbol( szEuro );
}

void CPageGrid::_InitColumnScientific( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellScientific * pCellScientific =
			STATIC_DOWNCAST(
				CExtGridCellScientific,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellScientific)
					)
				);
		UNREFERENCED_PARAMETER(pCellScientific);
	}

	CExtGridCellScientific * pCellScientific0 =
		STATIC_DOWNCAST(
			CExtGridCellScientific,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellScientific0->_VariantAssign( 0.005 );
	pCellScientific0->SetDecimalPlaces( 0 );

	CExtGridCellScientific * pCellScientific1 =
		STATIC_DOWNCAST(
			CExtGridCellScientific,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellScientific1->_VariantAssign( 123456 );
	pCellScientific1->SetDecimalPlaces( 3 );

	CExtGridCellScientific * pCellScientific2 =
		STATIC_DOWNCAST(
			CExtGridCellScientific,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellScientific2->_VariantAssign( 123456.123456789 );
	pCellScientific2->SetDecimalPlaces( 4 );

	CExtGridCellScientific * pCellScientific3 =
		STATIC_DOWNCAST(
			CExtGridCellScientific,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellScientific3->_VariantAssign( -12345667890L );

	CExtGridCellScientific * pCellScientific4 =
		STATIC_DOWNCAST(
			CExtGridCellScientific,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellScientific4->_VariantAssign( 123456678901L );

	CExtGridCellScientific * pCellScientific5 =
		STATIC_DOWNCAST(
			CExtGridCellScientific,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellScientific5->_VariantAssign( -458.46 );

	CExtGridCellScientific * pCellScientific6 =
		STATIC_DOWNCAST(
			CExtGridCellScientific,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellScientific6->_VariantAssign( -56789217L );

	CExtGridCellScientific * pCellScientific7 =
		STATIC_DOWNCAST(
			CExtGridCellScientific,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellScientific7->_VariantAssign( 54761.458 );
	pCellScientific7->SetDecimalPlaces( 4 );

	CExtGridCellScientific * pCellScientific8 =
		STATIC_DOWNCAST(
			CExtGridCellScientific,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellScientific8->_VariantAssign( 547614584167L );

	CExtGridCellScientific * pCellScientific9 =
		STATIC_DOWNCAST(
			CExtGridCellScientific,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellScientific9->_VariantAssign( 18.0005 );
	pCellScientific9->SetDecimalPlaces( 5 );
}

void CPageGrid::_InitColumnFraction( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellFraction * pCellFraction =
			STATIC_DOWNCAST(
				CExtGridCellFraction,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellFraction)
					)
				);
		UNREFERENCED_PARAMETER(pCellFraction);
	}

	CExtGridCellFraction * pCellFraction0 =
		STATIC_DOWNCAST(
			CExtGridCellFraction,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellFraction0->_VariantAssign( 1.5 );

	CExtGridCellFraction * pCellFraction1 =
		STATIC_DOWNCAST(
			CExtGridCellFraction,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellFraction1->_VariantAssign( -102.25 );

	CExtGridCellFraction * pCellFraction2 =
		STATIC_DOWNCAST(
			CExtGridCellFraction,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellFraction2->_VariantAssign( 125.125 );

	CExtGridCellFraction * pCellFraction3 =
		STATIC_DOWNCAST(
			CExtGridCellFraction,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellFraction3->_VariantAssign( -100.005 );

	CExtGridCellFraction * pCellFraction4 =
		STATIC_DOWNCAST(
			CExtGridCellFraction,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellFraction4->_VariantAssign( 0.0025 );

	CExtGridCellFraction * pCellFraction5 =
		STATIC_DOWNCAST(
			CExtGridCellFraction,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellFraction5->_VariantAssign( -2.99 );

	CExtGridCellFraction * pCellFraction6 =
		STATIC_DOWNCAST(
			CExtGridCellFraction,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellFraction6->_VariantAssign( 50.3 );

	CExtGridCellFraction * pCellFraction7 =
		STATIC_DOWNCAST(
			CExtGridCellFraction,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellFraction7->_VariantAssign( 85.655 );

	CExtGridCellFraction * pCellFraction8 =
		STATIC_DOWNCAST(
			CExtGridCellFraction,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellFraction8->_VariantAssign( -0.8 );

	CExtGridCellFraction * pCellFraction9 =
		STATIC_DOWNCAST(
			CExtGridCellFraction,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellFraction9->_VariantAssign( 1.33 );
}

void CPageGrid::_InitColumnPercentage( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellPercentage * pCellPercentage =
			STATIC_DOWNCAST(
				CExtGridCellPercentage,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellPercentage)
					)
				);
		UNREFERENCED_PARAMETER(pCellPercentage);
	}

	CExtGridCellPercentage * pCellPercentage0 =
		STATIC_DOWNCAST(
			CExtGridCellPercentage,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellPercentage0->_VariantAssign( 0.6 );
	pCellPercentage0->SetPercentSignPos( 0 );

	CExtGridCellPercentage * pCellPercentage1 =
		STATIC_DOWNCAST(
			CExtGridCellPercentage,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellPercentage1->_VariantAssign( 0.7 );
	pCellPercentage1->SetPercentSignPos( 1 );

	CExtGridCellPercentage * pCellPercentage2 =
		STATIC_DOWNCAST(
			CExtGridCellPercentage,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellPercentage2->_VariantAssign( 1.9 );
	pCellPercentage2->SetPercentSignPos( 2 );

	CExtGridCellPercentage * pCellPercentage3 =
		STATIC_DOWNCAST(
			CExtGridCellPercentage,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellPercentage3->_VariantAssign( 0.04 );
	pCellPercentage3->SetPercentSignPos( 3 );

	CExtGridCellPercentage * pCellPercentage4 =
		STATIC_DOWNCAST(
			CExtGridCellPercentage,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellPercentage4->_VariantAssign( -0.05 );

	CExtGridCellPercentage * pCellPercentage5 =
		STATIC_DOWNCAST(
			CExtGridCellPercentage,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellPercentage5->_VariantAssign( 5.2 );
	pCellPercentage5->SetPercentSignPos( 1 );

	CExtGridCellPercentage * pCellPercentage6 =
		STATIC_DOWNCAST(
			CExtGridCellPercentage,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellPercentage6->_VariantAssign( -1.25 );
	pCellPercentage6->SetPercentSignPos( 2 );

	CExtGridCellPercentage * pCellPercentage7 =
		STATIC_DOWNCAST(
			CExtGridCellPercentage,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellPercentage7->_VariantAssign( 0.01 );

	CExtGridCellPercentage * pCellPercentage8 =
		STATIC_DOWNCAST(
			CExtGridCellPercentage,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellPercentage8->_VariantAssign( 0.005 );
	pCellPercentage2->SetPercentSignPos( 0 );

	CExtGridCellPercentage * pCellPercentage9 =
		STATIC_DOWNCAST(
			CExtGridCellPercentage,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellPercentage9->_VariantAssign( -0.002 );
	pCellPercentage9->SetPercentSignPos( 1 );
}

void CPageGrid::_InitColumnVariant( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellVariant * pCellVariant =
			STATIC_DOWNCAST(
				CExtGridCellVariant,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellVariant)
					)
				);
		pCellVariant->ModifyStyle( __EGCS_BUTTON_UPDOWN );
	}

	CExtGridCellVariant * pCellVariant0 =
		STATIC_DOWNCAST(
			CExtGridCellVariant,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellVariant0->_VariantAssign( 12345, VT_I4 );

	CExtGridCellVariant * pCellVariant1 =
		STATIC_DOWNCAST(
			CExtGridCellVariant,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellVariant1->_VariantAssign( 12345.12345, VT_R8 );

	CExtGridCellVariant * pCellVariant2 =
		STATIC_DOWNCAST(
			CExtGridCellVariant,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	COleCurrency curA( 4, 500 ); // value: 4.0500
	pCellVariant2->_VariantAssign( curA );

	CExtGridCellVariant * pCellVariant3 =
		STATIC_DOWNCAST(
			CExtGridCellVariant,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellVariant3->_VariantAssign( DATE( COleDateTime::GetCurrentTime() ), VT_DATE );

	CExtGridCellVariant * pCellVariant4 =
		STATIC_DOWNCAST(
			CExtGridCellVariant,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	USES_CONVERSION;
//BSTR bstrCopy = ::SysAllocString( T2COLE(_T("BSTR String")) );
//	pCellVariant4->_VariantAssign( bstrCopy, VT_BSTR );
	pCellVariant4->_VariantClear(false);
	pCellVariant4->vt = VT_BSTR;
	pCellVariant4->bstrVal = ::SysAllocString( T2COLE(_T("BSTR String")) );

	CExtGridCellVariant * pCellVariant5 =
		STATIC_DOWNCAST(
			CExtGridCellVariant,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellVariant5->_VariantAssign( true );

	CExtGridCellVariant * pCellVariant6 =
		STATIC_DOWNCAST(
			CExtGridCellVariant,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellVariant6->_VariantAssign( USHORT(65535) );

	CExtGridCellVariant * pCellVariant7 =
		STATIC_DOWNCAST(
			CExtGridCellVariant,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellVariant7->_VariantAssign( FLOAT(-345.85) );

	CExtGridCellVariant * pCellVariant8 =
		STATIC_DOWNCAST(
			CExtGridCellVariant,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellVariant8->_VariantAssign( signed char(-128) );

	CExtGridCellVariant * pCellVariant9 =
		STATIC_DOWNCAST(
			CExtGridCellVariant,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellVariant9->_VariantAssign( unsigned char(255) );
}

void CPageGrid::_InitColumnUpDown( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellUpDown * pCellUpDown =
			STATIC_DOWNCAST(
				CExtGridCellUpDown,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellUpDown)
					)
				);
		UNREFERENCED_PARAMETER(pCellUpDown);
	}

	CExtGridCellUpDown * pCellUpDown0 =
		STATIC_DOWNCAST(
			CExtGridCellUpDown,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellUpDown0->_VariantAssign( 12345, VT_I4 );

	CExtGridCellUpDown * pCellUpDown1 =
		STATIC_DOWNCAST(
			CExtGridCellUpDown,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellUpDown1->_VariantAssign( 12345.12345, VT_R8 );

	CExtGridCellUpDown * pCellUpDown2 =
		STATIC_DOWNCAST(
			CExtGridCellUpDown,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	COleCurrency curA( 4, 500 ); // value: 4.0500
	pCellUpDown2->_VariantAssign( curA );

	CExtGridCellUpDown * pCellUpDown3 =
		STATIC_DOWNCAST(
			CExtGridCellUpDown,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellUpDown3->_VariantAssign( 555 );

	CExtGridCellUpDown * pCellUpDown4 =
		STATIC_DOWNCAST(
			CExtGridCellUpDown,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellUpDown4->_VariantAssign( -555 );

	CExtGridCellUpDown * pCellUpDown5 =
		STATIC_DOWNCAST(
			CExtGridCellUpDown,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellUpDown5->_VariantAssign( true );

	CExtGridCellUpDown * pCellUpDown6 =
		STATIC_DOWNCAST(
			CExtGridCellUpDown,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellUpDown6->_VariantAssign( USHORT(65535) );

	CExtGridCellUpDown * pCellUpDown7 =
		STATIC_DOWNCAST(
			CExtGridCellUpDown,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellUpDown7->_VariantAssign( FLOAT(-345.85) );

	CExtGridCellUpDown * pCellUpDown8 =
		STATIC_DOWNCAST(
			CExtGridCellUpDown,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellUpDown8->_VariantAssign( signed char(-128) );

	CExtGridCellUpDown * pCellUpDown9 =
		STATIC_DOWNCAST(
			CExtGridCellUpDown,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellUpDown9->_VariantAssign( unsigned char(255) );
}

void CPageGrid::_InitColumnPicture( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellPicture * pCellPicture =
			STATIC_DOWNCAST(
				CExtGridCellPicture,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellPicture)
					)
				);
		UNREFERENCED_PARAMETER(pCellPicture);
	}

CExtBitmap * pBmp = NULL;

	CExtGridCellPicture * pCellPicture0 =
		STATIC_DOWNCAST(
			CExtGridCellPicture,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pBmp = pCellPicture0->BitmapGetBuffer();
	VERIFY(
		pBmp->LoadBMP_Resource( 
			MAKEINTRESOURCE( IDB_BITMAP_VISTA1 ) 
			)
		);
	pCellPicture0->ImageModeSet( CExtGridCellPicture::eTouchOutside );

	CExtGridCellPicture * pCellPicture1 =
		STATIC_DOWNCAST(
			CExtGridCellPicture,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pBmp = pCellPicture1->BitmapGetBuffer();
	VERIFY(
		pBmp->LoadBMP_Resource( 
			MAKEINTRESOURCE( IDB_BITMAP_VISTA2 ) 
			)
		);

	CExtGridCellPicture * pCellPicture2 =
		STATIC_DOWNCAST(
			CExtGridCellPicture,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pBmp = pCellPicture2->BitmapGetBuffer();
	VERIFY(
		pBmp->LoadBMP_Resource( 
			MAKEINTRESOURCE( IDB_BITMAP_VISTA3 ) 
			)
		);
	pCellPicture2->ImageModeSet( CExtGridCellPicture::eStretch );

	CExtGridCellPicture * pCellPicture3 =
		STATIC_DOWNCAST(
			CExtGridCellPicture,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pBmp = pCellPicture3->BitmapGetBuffer();
	VERIFY(
		pBmp->LoadBMP_Resource( 
			MAKEINTRESOURCE( IDB_BITMAP_VISTA3 ) 
			)
		);
	pCellPicture3->ImageModeSet( CExtGridCellPicture::eTile );

	CExtGridCellPicture * pCellPicture4 =
		STATIC_DOWNCAST(
			CExtGridCellPicture,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pBmp = pCellPicture4->BitmapGetBuffer();
	VERIFY(
		pBmp->LoadBMP_Resource( 
			MAKEINTRESOURCE( IDB_BITMAP_VISTA4 ) 
			)
		);
	pCellPicture4->ImageModeSet( CExtGridCellPicture::eStretch );

	CExtGridCellPicture * pCellPicture5 =
		STATIC_DOWNCAST(
			CExtGridCellPicture,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pBmp = pCellPicture5->BitmapGetBuffer();
	VERIFY(
		pBmp->LoadBMP_Resource( 
			MAKEINTRESOURCE( IDB_BITMAP_VISTA5 ) 
			)
		);

	CExtGridCellPicture * pCellPicture6 =
		STATIC_DOWNCAST(
			CExtGridCellPicture,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pBmp = pCellPicture6->BitmapGetBuffer();
	VERIFY(
		pBmp->LoadBMP_Resource( 
			MAKEINTRESOURCE( IDB_BITMAP_VISTA6 ) 
			)
		);

	CExtGridCellPicture * pCellPicture7 =
		STATIC_DOWNCAST(
			CExtGridCellPicture,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pBmp = pCellPicture7->BitmapGetBuffer();
	VERIFY(
		pBmp->LoadBMP_Resource( 
			MAKEINTRESOURCE( IDB_BITMAP_VISTA6 ) 
			)
		);
	pCellPicture7->ImageModeSet( CExtGridCellPicture::eTile );

	CExtGridCellPicture * pCellPicture8 =
		STATIC_DOWNCAST(
			CExtGridCellPicture,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pBmp = pCellPicture8->BitmapGetBuffer();
	VERIFY(
		pBmp->LoadBMP_Resource( 
			MAKEINTRESOURCE( IDB_BITMAP_VISTA7 ) 
			)
		);

	CExtGridCellPicture * pCellPicture9 =
		STATIC_DOWNCAST(
			CExtGridCellPicture,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pBmp = pCellPicture9->BitmapGetBuffer();
	VERIFY(
		pBmp->LoadBMP_Resource( 
			MAKEINTRESOURCE( IDB_BITMAP_VISTA8 ) 
			)
		);
	pCellPicture9->ImageModeSet( CExtGridCellPicture::eTile );
}

void CPageGrid::_InitColumnProgress( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellProgress * pCellProgress =
			STATIC_DOWNCAST(
				CExtGridCellProgress,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellProgress)
					)
				);
		UNREFERENCED_PARAMETER(pCellProgress);
	}

	CExtGridCellProgress * pCellProgress0 =
		STATIC_DOWNCAST(
			CExtGridCellProgress,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellProgress0->SetRange( 0, 100 );
	pCellProgress0->SetStep( 2 );

	CExtGridCellProgress * pCellProgress1 =
		STATIC_DOWNCAST(
			CExtGridCellProgress,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellProgress1->ModifyStyle( __EGCS_TA_HORZ_CENTER );
	pCellProgress1->SetRange( 0, 100 );
	pCellProgress1->SetStep( 4 );
	pCellProgress1->TextSet( _T("Done:") );
	pCellProgress1->TextModeSet( CExtGridCellProgress::eTextAndPercent );
	pCellProgress1->BarColorsSet(
		::GetSysColor( COLOR_HIGHLIGHT ),
		::GetSysColor( COLOR_HIGHLIGHT )
		);
	pCellProgress1->TextColorsSet(
		COLORREF( -1L ),
		::GetSysColor( COLOR_HIGHLIGHTTEXT )
		);

	CExtGridCellProgress * pCellProgress2 =
		STATIC_DOWNCAST(
			CExtGridCellProgress,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellProgress2->ModifyStyle( __EGCS_TA_HORZ_LEFT );
	pCellProgress2->SetRange( 0, 100 );
	pCellProgress2->SetStep( 3 );
	pCellProgress2->TextModeSet( CExtGridCellProgress::ePercent );
	pCellProgress2->BarColorsSet(
		RGB(233,233,233),
		RGB(40,40,40)
		);
	pCellProgress2->TextColorsSet(
		COLORREF( -1L ),
		RGB(255,255,0)
		);
	pCellProgress2->InvertBarGradientSet( true );

	CExtGridCellProgress * pCellProgress3 =
		STATIC_DOWNCAST(
			CExtGridCellProgress,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellProgress3->ModifyStyle( __EGCS_TA_HORZ_CENTER );
	pCellProgress3->SetRange( 0, 150 );
	pCellProgress3->SetStep( 2 );
	pCellProgress3->TextModeSet( CExtGridCellProgress::ePercent );
	pCellProgress3->TiedTextSet( true );
	pCellProgress3->BarColorsSet(
		RGB(80,167,255),
		RGB(0,0,150)
		);
	pCellProgress3->TextColorsSet(
		COLORREF( -1L ),
		RGB(255,255,255)
		);

	CExtGridCellProgress * pCellProgress4 =
		STATIC_DOWNCAST(
			CExtGridCellProgress,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellProgress4->ModifyStyle( __EGCS_TA_HORZ_RIGHT );
	pCellProgress4->SetRange( -100, 400 );
	pCellProgress4->SetStep( 10 );
	pCellProgress4->TextModeSet( CExtGridCellProgress::ePosition );
	pCellProgress4->TiedTextSet( true );
	pCellProgress4->BarColorsSet(
		RGB(255,255,0),
		RGB(255,0,0)
		);
	pCellProgress4->TextColorsSet(
		COLORREF( -1L ),
		RGB(255,255,0)
		);

	CExtGridCellProgress * pCellProgress5 =
		STATIC_DOWNCAST(
			CExtGridCellProgress,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellProgress5->ModifyStyle( __EGCS_TA_HORZ_LEFT );
	pCellProgress5->SetRange( 100, 250 );
	pCellProgress5->SetStep( 5 );
	pCellProgress5->TextSet( _T("Some text...") );
	pCellProgress5->TextModeSet( CExtGridCellProgress::eText );
	pCellProgress5->BarColorsSet(
		RGB(80,167,255),
		RGB(0,0,150)
		);
	pCellProgress5->TextColorsSet(
		COLORREF( -1L ),
		RGB(255,255,0)
		);
	pCellProgress5->InvertBarGradientSet( true );

	CExtGridCellProgress * pCellProgress6 =
		STATIC_DOWNCAST(
			CExtGridCellProgress,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellProgress6->ModifyStyle( __EGCS_TA_HORZ_LEFT );
	pCellProgress6->SetRange( -100, 400 );
	pCellProgress6->SetStep( 10 );
	pCellProgress6->TextModeSet( CExtGridCellProgress::ePercent );
	pCellProgress6->TiedTextSet( true );
	pCellProgress6->BarColorsSet(
		RGB(255,255,0),
		RGB(255,0,0)
		);
	pCellProgress6->TextColorsSet(
		COLORREF( -1L ),
		RGB(255,255,255)
		);
	pCellProgress6->InvertBarGradientSet( true );
	pCellProgress6->ReversedSet( true );

	CExtGridCellProgress * pCellProgress7 =
		STATIC_DOWNCAST(
			CExtGridCellProgress,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellProgress7->ModifyStyle( __EGCS_TA_HORZ_CENTER );
	pCellProgress7->SetRange( -200, 200 );
	pCellProgress7->SetStep( 6 );
	pCellProgress7->TextSet( _T("Pos:") );
	pCellProgress7->TextModeSet( CExtGridCellProgress::eTextAndPosition );
	pCellProgress7->TiedTextSet( true );
	pCellProgress7->BarColorsSet(
		RGB(255,255,255),
		RGB(128,20,250)
		);
	pCellProgress7->TextColorsSet(
		COLORREF( -1L ),
		RGB(0,255,0)
		);
	pCellProgress7->InvertBarGradientSet( true );
	pCellProgress7->ReversedSet( true );

	CExtGridCellProgress * pCellProgress8 =
		STATIC_DOWNCAST(
			CExtGridCellProgress,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellProgress8->SetRange( 0, 100 );
	pCellProgress8->SetStep( 4 );
	pCellProgress8->TextModeSet( CExtGridCellProgress::eNone );
	pCellProgress8->BarColorsSet(
		RGB(255,255,255),
		RGB(0,0,0)
		);

	CExtGridCellProgress * pCellProgress9 =
		STATIC_DOWNCAST(
			CExtGridCellProgress,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellProgress9->ModifyStyle( __EGCS_TA_HORZ_RIGHT );
	pCellProgress9->SetRange( 0, 100 );
	pCellProgress9->SetStep( 2 );
	pCellProgress9->TextModeSet( CExtGridCellProgress::ePercent );
}

void CPageGrid::_InitColumnSlider( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellSlider * pCellSlider =
			STATIC_DOWNCAST(
				CExtGridCellSlider,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellSlider)
					)
				);
		UNREFERENCED_PARAMETER(pCellSlider);
	}

	CExtGridCellSlider * pCellSlider0 =
		STATIC_DOWNCAST(
			CExtGridCellSlider,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellSlider0->SetLineSize( 1 );
	pCellSlider0->SetPageSize( 20 );
	pCellSlider0->SetRange( 0, 100 );
	pCellSlider0->SetPos( 0 );

	CExtGridCellSlider * pCellSlider1 =
		STATIC_DOWNCAST(
			CExtGridCellSlider,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellSlider1->ModifyStyleEx( __EGCS_EX_NO_INPLACE_CONTROL_BORDER );
	pCellSlider1->SetLineSize( 5 );
	pCellSlider1->SetPageSize( 50 );
	pCellSlider1->SetRange( -100, 100 );
	pCellSlider1->SetPos( -50 );

	CExtGridCellSlider * pCellSlider2 =
		STATIC_DOWNCAST(
			CExtGridCellSlider,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellSlider2->ModifyStyle( __EGCS_TA_HORZ_CENTER );
	pCellSlider2->SetLineSize( 2 );
	pCellSlider2->SetPageSize( 40 );
	pCellSlider2->SetRange( 0, 200 );
	pCellSlider2->SetPos( 150 );

	CExtGridCellSlider * pCellSlider3 =
		STATIC_DOWNCAST(
			CExtGridCellSlider,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellSlider3->ModifyStyleEx( __EGCS_EX_NO_INPLACE_CONTROL_BORDER );
	pCellSlider3->ModifyStyle( __EGCS_TA_HORZ_LEFT );
	pCellSlider3->SetLineSize( 10 );
	pCellSlider3->SetPageSize( 100 );
	pCellSlider3->SetRange( 0, 1000 );
	pCellSlider3->SetPos( 555 );

	CExtGridCellSlider * pCellSlider4 =
		STATIC_DOWNCAST(
			CExtGridCellSlider,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellSlider4->SetLineSize( 1 );
	pCellSlider4->SetPageSize( 20 );
	pCellSlider4->SetRange( -200, -100 );
	pCellSlider4->SetPos( -150 );

	CExtGridCellSlider * pCellSlider5 =
		STATIC_DOWNCAST(
			CExtGridCellSlider,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellSlider5->ModifyStyleEx( __EGCS_EX_NO_INPLACE_CONTROL_BORDER );
	pCellSlider5->SetLineSize( 1 );
	pCellSlider5->SetPageSize( 2 );
	pCellSlider5->SetRange( 0, 10 );
	pCellSlider5->SetPos( 2 );

	CExtGridCellSlider * pCellSlider6 =
		STATIC_DOWNCAST(
			CExtGridCellSlider,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellSlider6->ModifyStyle( __EGCS_TA_HORZ_CENTER );
	pCellSlider6->SetLineSize( 1 );
	pCellSlider6->SetPageSize( 2 );
	pCellSlider6->SetRange( 0, 1 );
	pCellSlider6->SetPos( 1 );

	CExtGridCellSlider * pCellSlider7 =
		STATIC_DOWNCAST(
			CExtGridCellSlider,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellSlider7->ModifyStyle( __EGCS_TA_HORZ_CENTER );
	pCellSlider7->SetLineSize( 1 );
	pCellSlider7->SetPageSize( 2 );
	pCellSlider7->SetRange( -1, 1 );
	pCellSlider7->SetPos( 0 );

	CExtGridCellSlider * pCellSlider8 =
		STATIC_DOWNCAST(
			CExtGridCellSlider,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellSlider8->SetLineSize( 1 );
	pCellSlider8->SetPageSize( 2 );
	pCellSlider8->SetRange( -10, 10 );
	pCellSlider8->SetPos( -9 );
//	pCellSlider8->BackColorSet( CExtGridCell::__ECS_ALL, RGB(255,0,0) );
	pCellSlider8->BackColorSet( CExtGridCell::__ECS_NORMAL, RGB(255,0,0) );
	pCellSlider8->BackColorSet( CExtGridCell::__ECS_SELECTED, RGB(128,0,0) );
	pCellSlider8->BackColorSet( CExtGridCell::__ECS_HOVERED, RGB(255,128,128) );
	pCellSlider8->BackColorSet( CExtGridCell::__ECS_HIGHLIGHT_BY_HOVER, RGB(255,192,192) );

	CExtGridCellSlider * pCellSlider9 =
		STATIC_DOWNCAST(
			CExtGridCellSlider,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellSlider9->ModifyStyleEx( __EGCS_EX_NO_INPLACE_CONTROL_BORDER );
	pCellSlider9->SetLineSize( 1 );
	pCellSlider9->SetPageSize( 20 );
	pCellSlider9->SetRange( -50, 50 );
	pCellSlider9->SetPos( 25 );
//	pCellSlider9->BackColorSet( CExtGridCell::__ECS_ALL, RGB(0,255,0) );
	pCellSlider9->BackColorSet( CExtGridCell::__ECS_NORMAL, RGB(0,255,0) );
	pCellSlider9->BackColorSet( CExtGridCell::__ECS_SELECTED, RGB(0,128,0) );
	pCellSlider9->BackColorSet( CExtGridCell::__ECS_HOVERED, RGB(128,255,128) );
	pCellSlider9->BackColorSet( CExtGridCell::__ECS_HIGHLIGHT_BY_HOVER, RGB(192,255,192) );
}

void CPageGrid::_InitColumnText( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellString * pCellString =
			STATIC_DOWNCAST(
				CExtGridCellString,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellString)
					)
				);
		UNREFERENCED_PARAMETER(pCellString);
	}

	CExtGridCellString * pCellString0 =
		STATIC_DOWNCAST(
			CExtGridCellString,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellString0->TextSet( _T("Cell1") );

	CExtGridCellString * pCellString1 =
		STATIC_DOWNCAST(
			CExtGridCellString,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellString1->TextSet( _T("Cell2 Line1\r\nCell2 Line2\r\nCell2 Line3") );
	pCellString1->ModifyStyleEx( __EGCS_EX_WRAP_TEXT );
	pCellString1->ModifyStyle( __EGCS_TA_HORZ_CENTER );

	CExtGridCellString * pCellString2 =
		STATIC_DOWNCAST(
			CExtGridCellString,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellString2->TextSet( _T("Cell3") );

	CExtGridCellString * pCellString3 =
		STATIC_DOWNCAST(
			CExtGridCellString,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellString3->TextSet( _T("Cell4") );

	CExtGridCellString * pCellString4 =
		STATIC_DOWNCAST(
			CExtGridCellString,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellString4->TextSet( _T("Cell5 Line1\r\nCell5 Line2\r\nCell5 Line3") );
	pCellString4->ModifyStyleEx( __EGCS_EX_WRAP_TEXT );
	pCellString4->ModifyStyle( __EGCS_TA_HORZ_RIGHT );

	CExtGridCellString * pCellString5 =
		STATIC_DOWNCAST(
			CExtGridCellString,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellString5->TextSet( _T("Cell6") );

	CExtGridCellString * pCellString6 =
		STATIC_DOWNCAST(
			CExtGridCellString,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellString6->TextSet( _T("Cell7") );
	pCellString6->ModifyStyle( __EGCS_TA_HORZ_CENTER );

	CExtGridCellString * pCellString7 =
		STATIC_DOWNCAST(
			CExtGridCellString,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellString7->TextSet( _T("Cell8 Line1\r\nCell8 Line2\r\nCell8 Line3") );
	pCellString7->ModifyStyleEx( __EGCS_EX_WRAP_TEXT );

	CExtGridCellString * pCellString8 =
		STATIC_DOWNCAST(
			CExtGridCellString,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellString8->TextSet( _T("Cell9") );

	CExtGridCellString * pCellString9 =
		STATIC_DOWNCAST(
			CExtGridCellString,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellString9->TextSet( _T("Cell10") );
}

void CPageGrid::_InitColumnPassword( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellPassword * pCellPassword =
			STATIC_DOWNCAST(
				CExtGridCellPassword,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellPassword)
					)
				);
		UNREFERENCED_PARAMETER(pCellPassword);
	}

	CExtGridCellPassword * pCellPassword0 =
		STATIC_DOWNCAST(
			CExtGridCellPassword,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellPassword0->TextSet( _T("Password1") );

	CExtGridCellPassword * pCellPassword1 =
		STATIC_DOWNCAST(
			CExtGridCellPassword,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellPassword1->TextSet( _T("Password2") );
	pCellPassword1->ModifyStyle( __EGCS_TA_HORZ_CENTER );

	CExtGridCellPassword * pCellPassword2 =
		STATIC_DOWNCAST(
			CExtGridCellPassword,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellPassword2->TextSet( _T("Password3") );

	CExtGridCellPassword * pCellPassword3 =
		STATIC_DOWNCAST(
			CExtGridCellPassword,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellPassword3->TextSet( _T("Password4") );

	CExtGridCellPassword * pCellPassword4 =
		STATIC_DOWNCAST(
			CExtGridCellPassword,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellPassword4->TextSet( _T("Password5") );
	pCellPassword4->ModifyStyle( __EGCS_TA_HORZ_RIGHT );

	CExtGridCellPassword * pCellPassword5 =
		STATIC_DOWNCAST(
			CExtGridCellPassword,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellPassword5->TextSet( _T("Password6") );

	CExtGridCellPassword * pCellPassword6 =
		STATIC_DOWNCAST(
			CExtGridCellPassword,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellPassword6->TextSet( _T("Password7") );

	CExtGridCellPassword * pCellPassword7 =
		STATIC_DOWNCAST(
			CExtGridCellPassword,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellPassword7->TextSet( _T("Password8") );
	pCellPassword7->ModifyStyle( __EGCS_TA_HORZ_CENTER );

	CExtGridCellPassword * pCellPassword8 =
		STATIC_DOWNCAST(
			CExtGridCellPassword,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellPassword8->TextSet( _T("Password9") );
	pCellPassword8->TextColorSet( CExtGridCell::__ECS_ALL, RGB(255,0,0) );

	CExtGridCellPassword * pCellPassword9 =
		STATIC_DOWNCAST(
			CExtGridCellPassword,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellPassword9->TextSet( _T("Password0") );
	pCellPassword9->TextColorSet( CExtGridCell::__ECS_ALL, RGB(0,255,0) );
}

void CPageGrid::_InitColumnFile( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellFile * pCellFile =
			STATIC_DOWNCAST(
				CExtGridCellFile,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellFile)
					)
				);
		UNREFERENCED_PARAMETER(pCellFile);
	}

	CExtGridCellFile * pCellFile0 =
		STATIC_DOWNCAST(
			CExtGridCellFile,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellFile0->TextSet( _T("C:\\autoexec.bat") );
	pCellFile0->m_bOpenFileDialog = TRUE;

	CExtGridCellFile * pCellFile1 =
		STATIC_DOWNCAST(
			CExtGridCellFile,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellFile1->TextSet( _T("C:\\Program Files\\Microsoft Visual Studio\\install.htm") );
	pCellFile1->m_bOpenFileDialog = TRUE;

	CExtGridCellFile * pCellFile2 =
		STATIC_DOWNCAST(
			CExtGridCellFile,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellFile2->TextSet( _T("C:\\Windows\\win.ini") );
	pCellFile2->m_bOpenFileDialog = FALSE;

	CExtGridCellFile * pCellFile3 =
		STATIC_DOWNCAST(
			CExtGridCellFile,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellFile3->TextSet( _T("C:\\Windows\\System32\\win.com") );
	pCellFile3->m_bOpenFileDialog = FALSE;

	CExtGridCellFile * pCellFile4 =
		STATIC_DOWNCAST(
			CExtGridCellFile,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	UNREFERENCED_PARAMETER(pCellFile4);

	CExtGridCellFile * pCellFile5 =
		STATIC_DOWNCAST(
			CExtGridCellFile,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	UNREFERENCED_PARAMETER(pCellFile5);

	CExtGridCellFile * pCellFile6 =
		STATIC_DOWNCAST(
			CExtGridCellFile,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	UNREFERENCED_PARAMETER(pCellFile6);

	CExtGridCellFile * pCellFile7 =
		STATIC_DOWNCAST(
			CExtGridCellFile,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	UNREFERENCED_PARAMETER(pCellFile7);

	CExtGridCellFile * pCellFile8 =
		STATIC_DOWNCAST(
			CExtGridCellFile,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	UNREFERENCED_PARAMETER(pCellFile8);

	CExtGridCellFile * pCellFile9 =
		STATIC_DOWNCAST(
			CExtGridCellFile,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	UNREFERENCED_PARAMETER(pCellFile9);
	pCellFile9->ModifyStyle( __EGCS_READ_ONLY );
}

void CPageGrid::_InitColumnFileImage( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellFileImage * pCellFileImage =
			STATIC_DOWNCAST(
				CExtGridCellFileImage,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellFileImage)
					)
				);
		UNREFERENCED_PARAMETER(pCellFileImage);
	}

	CExtGridCellFileImage * pCellFileImage0 =
		STATIC_DOWNCAST(
			CExtGridCellFileImage,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	UNREFERENCED_PARAMETER(pCellFileImage0);

	CExtGridCellFileImage * pCellFileImage1 =
		STATIC_DOWNCAST(
			CExtGridCellFileImage,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	UNREFERENCED_PARAMETER(pCellFileImage1);

	CExtGridCellFileImage * pCellFileImage2 =
		STATIC_DOWNCAST(
			CExtGridCellFileImage,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	UNREFERENCED_PARAMETER(pCellFileImage2);

	CExtGridCellFileImage * pCellFileImage3 =
		STATIC_DOWNCAST(
			CExtGridCellFileImage,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	UNREFERENCED_PARAMETER(pCellFileImage3);

	CExtGridCellFileImage * pCellFileImage4 =
		STATIC_DOWNCAST(
			CExtGridCellFileImage,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	UNREFERENCED_PARAMETER(pCellFileImage4);

	CExtGridCellFileImage * pCellFileImage5 =
		STATIC_DOWNCAST(
			CExtGridCellFileImage,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	UNREFERENCED_PARAMETER(pCellFileImage5);

	CExtGridCellFileImage * pCellFileImage6 =
		STATIC_DOWNCAST(
			CExtGridCellFileImage,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	UNREFERENCED_PARAMETER(pCellFileImage6);

	CExtGridCellFileImage * pCellFileImage7 =
		STATIC_DOWNCAST(
			CExtGridCellFileImage,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	UNREFERENCED_PARAMETER(pCellFileImage7);

	CExtGridCellFileImage * pCellFileImage8 =
		STATIC_DOWNCAST(
			CExtGridCellFileImage,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	UNREFERENCED_PARAMETER(pCellFileImage8);

	CExtGridCellFileImage * pCellFileImage9 =
		STATIC_DOWNCAST(
			CExtGridCellFileImage,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	UNREFERENCED_PARAMETER(pCellFileImage9);
	pCellFileImage9->ModifyStyle( __EGCS_READ_ONLY );
}

void CPageGrid::_InitColumnFolder( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellFolder * pCellFolder =
			STATIC_DOWNCAST(
				CExtGridCellFolder,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellFolder)
					)
				);
		UNREFERENCED_PARAMETER(pCellFolder);
	}

	CExtGridCellFolder * pCellFolder0 =
		STATIC_DOWNCAST(
			CExtGridCellFolder,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellFolder0->TextSet( _T("C:\\") );

	CExtGridCellFolder * pCellFolder1 =
		STATIC_DOWNCAST(
			CExtGridCellFolder,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellFolder1->TextSet( _T("C:\\Program Files\\") );

	CExtGridCellFolder * pCellFolder2 =
		STATIC_DOWNCAST(
			CExtGridCellFolder,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellFolder2->TextSet( _T("C:\\Windows") );

	CExtGridCellFolder * pCellFolder3 =
		STATIC_DOWNCAST(
			CExtGridCellFolder,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellFolder3->TextSet( _T("C:\\Windows\\System32") );

	CExtGridCellFolder * pCellFolder4 =
		STATIC_DOWNCAST(
			CExtGridCellFolder,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	UNREFERENCED_PARAMETER(pCellFolder4);

	CExtGridCellFolder * pCellFolder5 =
		STATIC_DOWNCAST(
			CExtGridCellFolder,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	UNREFERENCED_PARAMETER(pCellFolder5);

	CExtGridCellFolder * pCellFolder6 =
		STATIC_DOWNCAST(
			CExtGridCellFolder,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	UNREFERENCED_PARAMETER(pCellFolder6);

	CExtGridCellFolder * pCellFolder7 =
		STATIC_DOWNCAST(
			CExtGridCellFolder,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	UNREFERENCED_PARAMETER(pCellFolder7);

	CExtGridCellFolder * pCellFolder8 =
		STATIC_DOWNCAST(
			CExtGridCellFolder,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	UNREFERENCED_PARAMETER(pCellFolder8);

	CExtGridCellFolder * pCellFolder9 =
		STATIC_DOWNCAST(
			CExtGridCellFolder,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	UNREFERENCED_PARAMETER(pCellFolder9);
	pCellFolder9->ModifyStyle( __EGCS_READ_ONLY );
}

void CPageGrid::_InitColumnFont( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellFont * pCellFont =
			STATIC_DOWNCAST(
				CExtGridCellFont,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellFont)
					)
				);
		UNREFERENCED_PARAMETER(pCellFont);
	}

LOGFONT lf;
::memset( &lf, 0, sizeof(LOGFONT) );

	CExtGridCellFont * pCellFont0 =
		STATIC_DOWNCAST(
			CExtGridCellFont,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	::memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight = -18;
	lf.lfWeight = FW_BOLD;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("Arial") );
	pCellFont0->DataSet( lf );

	CExtGridCellFont * pCellFont1 =
		STATIC_DOWNCAST(
			CExtGridCellFont,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	::memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight = -12;
	lf.lfItalic = 1;
	lf.lfWeight = FW_BOLD;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("MS Sans Serif") );
	pCellFont1->DataSet( lf );

	CExtGridCellFont * pCellFont2 =
		STATIC_DOWNCAST(
			CExtGridCellFont,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	::memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight = -14;
	lf.lfItalic = 1;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("Tahoma") );
	pCellFont2->DataSet( lf );

	CExtGridCellFont * pCellFont3 =
		STATIC_DOWNCAST(
			CExtGridCellFont,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	::memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight = -10;
	lf.lfStrikeOut = 1;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("Courier") );
	pCellFont3->DataSet( lf );

	CExtGridCellFont * pCellFont4 =
		STATIC_DOWNCAST(
			CExtGridCellFont,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	::memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight = -8;
	lf.lfStrikeOut = 1;
	lf.lfWeight = FW_BOLD;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("Courier New") );
	pCellFont4->DataSet( lf );

	CExtGridCellFont * pCellFont5 =
		STATIC_DOWNCAST(
			CExtGridCellFont,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	::memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight = -20;
	lf.lfStrikeOut = 1;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("Times New Roman") );
	pCellFont5->DataSet( lf );

	CExtGridCellFont * pCellFont6 =
		STATIC_DOWNCAST(
			CExtGridCellFont,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	::memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight = -16;
	lf.lfUnderline = 1;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("System") );
	pCellFont6->DataSet( lf );

	CExtGridCellFont * pCellFont7 =
		STATIC_DOWNCAST(
			CExtGridCellFont,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	::memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight = -14;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("Marlett") );
	pCellFont7->DataSet( lf );

	CExtGridCellFont * pCellFont8 =
		STATIC_DOWNCAST(
			CExtGridCellFont,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	::memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight = -12;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("Symbol") );
	pCellFont8->DataSet( lf );

	CExtGridCellFont * pCellFont9 =
		STATIC_DOWNCAST(
			CExtGridCellFont,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	::memset( &lf, 0, sizeof(LOGFONT) );
	lf.lfHeight = -18;
	__EXT_MFC_STRCPY( lf.lfFaceName, LF_FACESIZE, _T("Verdana") );
	pCellFont9->DataSet( lf );
	pCellFont9->ModifyStyle( __EGCS_READ_ONLY );
}

void CPageGrid::_InitColumnFontFaceName( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellFontFaceName * pCellFontFaceName =
			STATIC_DOWNCAST(
				CExtGridCellFontFaceName,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellFontFaceName)
					)
				);
		UNREFERENCED_PARAMETER(pCellFontFaceName);
	}

	CExtGridCellFontFaceName * pCellFontFaceName0 =
		STATIC_DOWNCAST(
			CExtGridCellFontFaceName,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellFontFaceName0->SelectString( _T("Arial") );

	CExtGridCellFontFaceName * pCellFontFaceName1 =
		STATIC_DOWNCAST(
			CExtGridCellFontFaceName,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellFontFaceName1->SelectString( _T("MS Sans Serif") );

	CExtGridCellFontFaceName * pCellFontFaceName2 =
		STATIC_DOWNCAST(
			CExtGridCellFontFaceName,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellFontFaceName2->SelectString( _T("Tahoma") );

	CExtGridCellFontFaceName * pCellFontFaceName3 =
		STATIC_DOWNCAST(
			CExtGridCellFontFaceName,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellFontFaceName3->SelectString( _T("Courier") );

	CExtGridCellFontFaceName * pCellFontFaceName4 =
		STATIC_DOWNCAST(
			CExtGridCellFontFaceName,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellFontFaceName4->SelectString( _T("Courier New") );

	CExtGridCellFontFaceName * pCellFontFaceName5 =
		STATIC_DOWNCAST(
			CExtGridCellFontFaceName,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellFontFaceName5->SelectString( _T("Times New Roman") );

	CExtGridCellFontFaceName * pCellFontFaceName6 =
		STATIC_DOWNCAST(
			CExtGridCellFontFaceName,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellFontFaceName6->SelectString( _T("System") );

	CExtGridCellFontFaceName * pCellFontFaceName7 =
		STATIC_DOWNCAST(
			CExtGridCellFontFaceName,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellFontFaceName7->SelectString( _T("Marlett") );

	CExtGridCellFontFaceName * pCellFontFaceName8 =
		STATIC_DOWNCAST(
			CExtGridCellFontFaceName,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellFontFaceName8->SelectString( _T("Symbol") );

	CExtGridCellFontFaceName * pCellFontFaceName9 =
		STATIC_DOWNCAST(
			CExtGridCellFontFaceName,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellFontFaceName9->SelectString( _T("Verdana") );
	pCellFontFaceName9->ModifyStyle( __EGCS_READ_ONLY );
}

void CPageGrid::_InitColumnFontWeight( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellUpDownFontWeight * pCellFontWeight =
			STATIC_DOWNCAST(
				CExtGridCellUpDownFontWeight,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellUpDownFontWeight)
					)
				);
		UNREFERENCED_PARAMETER(pCellFontWeight);
	}

	CExtGridCellUpDownFontWeight * pCellFontWeight0 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownFontWeight,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellFontWeight0->_VariantAssign( FW_DONTCARE );

	CExtGridCellUpDownFontWeight * pCellFontWeight1 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownFontWeight,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellFontWeight1->_VariantAssign( FW_THIN );

	CExtGridCellUpDownFontWeight * pCellFontWeight2 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownFontWeight,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellFontWeight2->_VariantAssign( FW_EXTRALIGHT );

	CExtGridCellUpDownFontWeight * pCellFontWeight3 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownFontWeight,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellFontWeight3->_VariantAssign( FW_LIGHT );

	CExtGridCellUpDownFontWeight * pCellFontWeight4 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownFontWeight,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellFontWeight4->_VariantAssign( FW_NORMAL );

	CExtGridCellUpDownFontWeight * pCellFontWeight5 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownFontWeight,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellFontWeight5->_VariantAssign( FW_MEDIUM );

	CExtGridCellUpDownFontWeight * pCellFontWeight6 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownFontWeight,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellFontWeight6->_VariantAssign( FW_SEMIBOLD );

	CExtGridCellUpDownFontWeight * pCellFontWeight7 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownFontWeight,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellFontWeight7->_VariantAssign( FW_BOLD );

	CExtGridCellUpDownFontWeight * pCellFontWeight8 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownFontWeight,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellFontWeight8->_VariantAssign( FW_EXTRABOLD );

	CExtGridCellUpDownFontWeight * pCellFontWeight9 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownFontWeight,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellFontWeight9->_VariantAssign( FW_HEAVY );
	pCellFontWeight9->ModifyStyle( __EGCS_READ_ONLY );
}

void CPageGrid::_InitColumnColorPart( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellUpDownColorPart * pCellColorPart =
			STATIC_DOWNCAST(
				CExtGridCellUpDownColorPart,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellUpDownColorPart)
					)
				);
		UNREFERENCED_PARAMETER(pCellColorPart);
	}

	CExtGridCellUpDownColorPart * pCellColorPart0 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownColorPart,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellColorPart0->_VariantAssign( 0 );
	pCellColorPart0->m_eCPT = CExtGridCellUpDownColorPart::__ECPT_UNKNOWN;

	CExtGridCellUpDownColorPart * pCellColorPart1 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownColorPart,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellColorPart1->_VariantAssign( 32 );
	pCellColorPart1->m_eCPT = CExtGridCellUpDownColorPart::__ECPT_RED;

	CExtGridCellUpDownColorPart * pCellColorPart2 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownColorPart,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellColorPart2->_VariantAssign( 64 );
	pCellColorPart2->m_eCPT = CExtGridCellUpDownColorPart::__ECPT_GREEN;

	CExtGridCellUpDownColorPart * pCellColorPart3 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownColorPart,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellColorPart3->_VariantAssign( 96 );
	pCellColorPart3->m_eCPT = CExtGridCellUpDownColorPart::__ECPT_BLUE;

	CExtGridCellUpDownColorPart * pCellColorPart4 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownColorPart,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellColorPart4->_VariantAssign( 128 );
	pCellColorPart4->m_eCPT = CExtGridCellUpDownColorPart::__ECPT_UNKNOWN;

	CExtGridCellUpDownColorPart * pCellColorPart5 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownColorPart,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellColorPart5->_VariantAssign( 160 );
	pCellColorPart5->m_eCPT = CExtGridCellUpDownColorPart::__ECPT_RED;

	CExtGridCellUpDownColorPart * pCellColorPart6 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownColorPart,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellColorPart6->_VariantAssign( 192 );
	pCellColorPart6->m_eCPT = CExtGridCellUpDownColorPart::__ECPT_GREEN;

	CExtGridCellUpDownColorPart * pCellColorPart7 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownColorPart,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellColorPart7->_VariantAssign( 224 );
	pCellColorPart7->m_eCPT = CExtGridCellUpDownColorPart::__ECPT_BLUE;

	CExtGridCellUpDownColorPart * pCellColorPart8 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownColorPart,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellColorPart8->_VariantAssign( 240 );
	pCellColorPart8->m_eCPT = CExtGridCellUpDownColorPart::__ECPT_RED;

	CExtGridCellUpDownColorPart * pCellColorPart9 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownColorPart,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellColorPart9->_VariantAssign( 255 );
	pCellColorPart9->m_eCPT = CExtGridCellUpDownColorPart::__ECPT_GREEN;
}

void CPageGrid::_InitColumnRectPart( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellUpDownRectPart * pCellRectPart =
			STATIC_DOWNCAST(
				CExtGridCellUpDownRectPart,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellUpDownRectPart)
					)
				);
		UNREFERENCED_PARAMETER(pCellRectPart);
	}

	CExtGridCellUpDownRectPart * pCellRectPart0 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownRectPart,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellRectPart0->_VariantAssign( 0 );
	pCellRectPart0->m_eCPT = CExtGridCellUpDownRectPart::__ECPT_UNKNOWN;

	CExtGridCellUpDownRectPart * pCellRectPart1 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownRectPart,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellRectPart1->_VariantAssign( 32 );
	pCellRectPart1->m_eCPT = CExtGridCellUpDownRectPart::__ECPT_X;

	CExtGridCellUpDownRectPart * pCellRectPart2 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownRectPart,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellRectPart2->_VariantAssign( 64 );
	pCellRectPart2->m_eCPT = CExtGridCellUpDownRectPart::__ECPT_Y;

	CExtGridCellUpDownRectPart * pCellRectPart3 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownRectPart,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellRectPart3->_VariantAssign( 96 );
	pCellRectPart3->m_eCPT = CExtGridCellUpDownRectPart::__ECPT_WIDTH;

	CExtGridCellUpDownRectPart * pCellRectPart4 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownRectPart,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellRectPart4->_VariantAssign( 128 );
	pCellRectPart4->m_eCPT = CExtGridCellUpDownRectPart::__ECPT_HEIGHT;

	CExtGridCellUpDownRectPart * pCellRectPart5 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownRectPart,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellRectPart5->_VariantAssign( 160 );
	pCellRectPart5->m_eCPT = CExtGridCellUpDownRectPart::__ECPT_UNKNOWN;

	CExtGridCellUpDownRectPart * pCellRectPart6 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownRectPart,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellRectPart6->_VariantAssign( 192 );
	pCellRectPart6->m_eCPT = CExtGridCellUpDownRectPart::__ECPT_X;

	CExtGridCellUpDownRectPart * pCellRectPart7 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownRectPart,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellRectPart7->_VariantAssign( 224 );
	pCellRectPart7->m_eCPT = CExtGridCellUpDownRectPart::__ECPT_Y;

	CExtGridCellUpDownRectPart * pCellRectPart8 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownRectPart,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellRectPart8->_VariantAssign( 240 );
	pCellRectPart8->m_eCPT = CExtGridCellUpDownRectPart::__ECPT_WIDTH;

	CExtGridCellUpDownRectPart * pCellRectPart9 =
		STATIC_DOWNCAST(
			CExtGridCellUpDownRectPart,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellRectPart9->_VariantAssign( 255 );
	pCellRectPart9->m_eCPT = CExtGridCellUpDownRectPart::__ECPT_HEIGHT;
}

void CPageGrid::_InitColumnInplaceSlider( LONG nColNo, LONG nRowCount, bool bSliderMode /*= true*/ )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellInplaceSlider * pCell =
			STATIC_DOWNCAST(
				CExtGridCellInplaceSlider,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellInplaceSlider)
					)
				);
		//pCell->ModifyStyleEx( 0, __EGCS_EX_EMPTY );
		pCell->ScrollTotalRangeSet( 100 );
		if( bSliderMode )
		{
			if( nRowNo > 5 )
			{
				pCell->ScrollPageSizeSet( ::rand() % 40 );
				pCell->ScrollPosSet( ::rand() % 60 );
			}
			else
			{
				pCell->ScrollPageSizeSet( 0 );
				pCell->ScrollPosSet( ::rand() % 100 );
			}
		}
		else
		{
			if( nRowNo <= 2 )
			{
				pCell->ScrollPageSizeSet( 0 );
				pCell->ScrollPosSet( ::rand() % 100 );
			}
			else
			{
				ULONG nScrollPageSize = ::rand() % 50;
				pCell->ScrollPageSizeSet( nScrollPageSize );
				pCell->ScrollPosSet( ::rand() % ( 100 - nScrollPageSize ) );
			}
			pCell->ScrollButtonExtentHorzSet( ( ::rand() % 20 ) + 10 );
		}
		if( nRowNo == 0 || nRowNo == 6 )
		{
			pCell->ModifyStyle( __EGCS_READ_ONLY );
			if( nRowNo == 6 )
				pCell->TextSet( _T("Read-only: ") );
		}
		if( nRowNo == 0 )
		{
			pCell->TextSet( _T("First: ") );
			//pCell->ModifyStyle( __EGCS_BUTTON_DROPDOWN|__EGCS_BUTTON_ELLIPSIS );
		}
		else if( nRowNo == 1 )
		{
			pCell->TextSet( _T("Second: ") );
		}
		else if( nRowNo == 2 )
		{
			pCell->TextSet( _T("Third: ") );
			//pCell->ModifyStyle( __EGCS_BUTTON_DROPDOWN );
		}
// 		else if( nRowNo == 3 )
// 			pCell->ModifyStyle( __EGCS_BUTTON_ELLIPSIS );
// 		else if( nRowNo == 4 )
// 			pCell->ModifyStyle( __EGCS_BUTTON_DROPDOWN );
// 		else if( nRowNo == 7 )
// 			pCell->ModifyStyle( __EGCS_BUTTON_DROPDOWN|__EGCS_BUTTON_ELLIPSIS );
	}
}

void CPageGrid::_InitColumnInplaceScrollBar( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );
	_InitColumnInplaceSlider( nColNo, nRowCount, false );
}

void CPageGrid::_InitColumnHotKey( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellHotKey * pCellHotKey =
			STATIC_DOWNCAST(
				CExtGridCellHotKey,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellHotKey)
					)
				);
		UNREFERENCED_PARAMETER(pCellHotKey);
	}

	CExtGridCellHotKey * pCellHotKey0 =
		STATIC_DOWNCAST(
			CExtGridCellHotKey,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellHotKey0->SetHotKey(
		MAKELONG( 'P', FCONTROL|FVIRTKEY )
		);

	CExtGridCellHotKey * pCellHotKey1 =
		STATIC_DOWNCAST(
			CExtGridCellHotKey,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellHotKey1->SetHotKey(
		MAKELONG( 'S', FCONTROL|FSHIFT|FVIRTKEY )
		);

	CExtGridCellHotKey * pCellHotKey2 =
		STATIC_DOWNCAST(
			CExtGridCellHotKey,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellHotKey2->SetHotKey(
		MAKELONG( 'X', FALT|FVIRTKEY )
		);

	CExtGridCellHotKey * pCellHotKey3 =
		STATIC_DOWNCAST(
			CExtGridCellHotKey,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellHotKey3->SetHotKey(
		MAKELONG( VK_F1, FSHIFT|FVIRTKEY)
		);

	CExtGridCellHotKey * pCellHotKey4 =
		STATIC_DOWNCAST(
			CExtGridCellHotKey,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellHotKey4->SetHotKey(
		VK_F12, 
		FCONTROL|FVIRTKEY
		);

	CExtGridCellHotKey * pCellHotKey5 =
		STATIC_DOWNCAST(
			CExtGridCellHotKey,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellHotKey5->SetHotKey(
		'S', 
		FALT|FSHIFT|FCONTROL|FVIRTKEY
		);

	CExtGridCellHotKey * pCellHotKey6 =
		STATIC_DOWNCAST(
			CExtGridCellHotKey,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellHotKey6->SetHotKey(
		VK_ADD, 
		FVIRTKEY
		);

	CExtGridCellHotKey * pCellHotKey7 =
		STATIC_DOWNCAST(
			CExtGridCellHotKey,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellHotKey7->SetHotKey(
		'1', 
		FALT|FVIRTKEY
		);
	
	CExtGridCellHotKey * pCellHotKey8 =
		STATIC_DOWNCAST(
			CExtGridCellHotKey,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellHotKey8->SetHotKey(
		'2', 
		FSHIFT|FVIRTKEY
		);

	CExtGridCellHotKey * pCellHotKey9 =
		STATIC_DOWNCAST(
			CExtGridCellHotKey,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellHotKey9->SetHotKey(
		VK_F10, 
		FVIRTKEY
		);
}

void CPageGrid::_InitColumnButton( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellButton * pCellButton =
			STATIC_DOWNCAST(
				CExtGridCellButton,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellButton)
					)
				);
		UNREFERENCED_PARAMETER(pCellButton);
	}

	CExtGridCellButton * pCellButton0 =
		STATIC_DOWNCAST(
			CExtGridCellButton,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellButton0->TextSet( _T("Cell1") );
	pCellButton0->IconIndexSet( 0 );

	CExtGridCellButton * pCellButton1 =
		STATIC_DOWNCAST(
			CExtGridCellButton,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellButton1->TextSet( _T("Cell2 Line1\r\nCell2 Line2\r\nCell2 Line3") );
	pCellButton1->ModifyStyleEx( __EGCS_EX_WRAP_TEXT );
	pCellButton1->ModifyStyle( __EGCS_TA_HORZ_CENTER );
	pCellButton1->IconIndexSet( 1 );

	CExtGridCellButton * pCellButton2 =
		STATIC_DOWNCAST(
			CExtGridCellButton,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellButton2->TextSet( _T("Cell3") );

	CExtGridCellButton * pCellButton3 =
		STATIC_DOWNCAST(
			CExtGridCellButton,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellButton3->TextSet( _T("Cell4") );
	pCellButton3->ModifyStyle( __EGCS_CHK_CHECK );
	pCellButton3->IconIndexSet( 2 );

	CExtGridCellButton * pCellButton4 =
		STATIC_DOWNCAST(
			CExtGridCellButton,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellButton4->TextSet( _T("Cell5 Line1\r\nCell5 Line2\r\nCell5 Line3") );
	pCellButton4->ModifyStyleEx( __EGCS_EX_WRAP_TEXT );
	pCellButton4->ModifyStyle( __EGCS_TA_HORZ_RIGHT );
	pCellButton4->IconIndexSet( 3 );

	CExtGridCellButton * pCellButton5 =
		STATIC_DOWNCAST(
			CExtGridCellButton,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellButton5->TextSet( _T("Cell6") );
	pCellButton5->IconIndexSet( 4 );

	CExtGridCellButton * pCellButton6 =
		STATIC_DOWNCAST(
			CExtGridCellButton,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellButton6->TextSet( _T("Cell7") );
	pCellButton6->ModifyStyle( __EGCS_TA_HORZ_CENTER );
	pCellButton6->IconIndexSet( 5 );

	CExtGridCellButton * pCellButton7 =
		STATIC_DOWNCAST(
			CExtGridCellButton,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellButton7->TextSet( _T("Cell8 Line1\r\nCell8 Line2\r\nCell8 Line3") );
	pCellButton7->ModifyStyleEx( __EGCS_EX_WRAP_TEXT );
	pCellButton7->IconIndexSet( 6 );

	CExtGridCellButton * pCellButton8 =
		STATIC_DOWNCAST(
			CExtGridCellButton,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellButton8->TextSet( _T("Cell9") );
	pCellButton8->IconIndexSet( 7 );
	pCellButton8->ModifyStyle( __EGCS_CHK_CHECK );

	CExtGridCellButton * pCellButton9 =
		STATIC_DOWNCAST(
			CExtGridCellButton,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellButton9->TextSet( _T("Cell10") );
	pCellButton9->ModifyStyle( __EGCS_CHK_CHECK );
}

void CPageGrid::_InitColumnObjectSize( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellObjectSize * pCellObjectSize =
			STATIC_DOWNCAST(
				CExtGridCellObjectSize,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellObjectSize)
					)
				);
		UNREFERENCED_PARAMETER(pCellObjectSize);
	}

	CExtGridCellObjectSize * pCellObjectSize0 =
		STATIC_DOWNCAST(
			CExtGridCellObjectSize,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellObjectSize0->SetMode( CExtGridCellObjectSize::eAuto );
	pCellObjectSize0->_VariantAssign( 7486347 );

	CExtGridCellObjectSize * pCellObjectSize1 =
		STATIC_DOWNCAST(
			CExtGridCellObjectSize,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellObjectSize1->SetMode( CExtGridCellObjectSize::eBytes );
	pCellObjectSize1->_VariantAssign( 1579 );

	CExtGridCellObjectSize * pCellObjectSize2 =
		STATIC_DOWNCAST(
			CExtGridCellObjectSize,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellObjectSize2->SetMode( CExtGridCellObjectSize::eKilobytes );
	pCellObjectSize2->_VariantAssign( 36864 );

	CExtGridCellObjectSize * pCellObjectSize3 =
		STATIC_DOWNCAST(
			CExtGridCellObjectSize,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellObjectSize3->SetMode( CExtGridCellObjectSize::eMegabytes );
	pCellObjectSize3->_VariantAssign( 1425917184 );

	CExtGridCellObjectSize * pCellObjectSize4 =
		STATIC_DOWNCAST(
			CExtGridCellObjectSize,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellObjectSize4->SetMode( CExtGridCellObjectSize::eGigabytes );
	pCellObjectSize4->_VariantAssign( 370348935 );

	CExtGridCellObjectSize * pCellObjectSize5 =
		STATIC_DOWNCAST(
			CExtGridCellObjectSize,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellObjectSize5->SetMode( CExtGridCellObjectSize::eTerabytes );
	pCellObjectSize5->_VariantAssign( 24234539196416 );

	CExtGridCellObjectSize * pCellObjectSize6 =
		STATIC_DOWNCAST(
			CExtGridCellObjectSize,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellObjectSize6->SetMode( CExtGridCellObjectSize::ePetabytes );
	pCellObjectSize6->_VariantAssign( DOUBLE( DOUBLE(6.5) * DOUBLE( ::pow( DOUBLE(2), DOUBLE(50) ) ) ), VT_R8 );

	CExtGridCellObjectSize * pCellObjectSize7 =
		STATIC_DOWNCAST(
			CExtGridCellObjectSize,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellObjectSize7->SetMode( CExtGridCellObjectSize::eExabytes );
	pCellObjectSize7->_VariantAssign( DOUBLE( DOUBLE(2.45) * DOUBLE( ::pow( DOUBLE(2), DOUBLE(60) ) ) ), VT_R8 );

	CExtGridCellObjectSize * pCellObjectSize8 =
		STATIC_DOWNCAST(
			CExtGridCellObjectSize,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellObjectSize8->SetMode( CExtGridCellObjectSize::eAuto );
	pCellObjectSize8->_VariantAssign( 10490037248 );

	CExtGridCellObjectSize * pCellObjectSize9 =
		STATIC_DOWNCAST(
			CExtGridCellObjectSize,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellObjectSize9->SetMode( CExtGridCellObjectSize::eAuto );
	pCellObjectSize9->_VariantAssign( 814804992 );
}

void CPageGrid::_InitColumnGeographic( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellLL * pCellLL =
			STATIC_DOWNCAST(
				CExtGridCellLL,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellLL)
					)
				);
		UNREFERENCED_PARAMETER(pCellLL);
	}

	CExtGridCellLL * pCellLL0 =
		STATIC_DOWNCAST(
			CExtGridCellLL,
			m_wndGrid.GridCellGet( nColNo, 0L )
			);
	pCellLL0->SetMode( CExtLLBaseControlProperties::longitude_latitude );
	pCellLL0->ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	CExtLongitude _Longitude0( CExtLongitude::hemisphere_east, 20, 30, 40 );
	pCellLL0->LongitudeSet( _Longitude0, true );
	CExtLatitude _Latitude0( CExtLatitude::hemisphere_north, 50, 40, 30 );
	pCellLL0->LatitudeSet( _Latitude0, true );

	CExtGridCellLL * pCellLL1 =
		STATIC_DOWNCAST(
			CExtGridCellLL,
			m_wndGrid.GridCellGet( nColNo, 1L )
			);
	pCellLL1->SetMode( CExtLLBaseControlProperties::latitude_longitude );
	pCellLL1->ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	CExtLongitude _Longitude1( CExtLongitude::hemisphere_west, 44, 33, 22 );
	pCellLL1->LongitudeSet( _Longitude0, true );
	CExtLatitude _Latitude1( CExtLatitude::hemisphere_south, 55, 44, 11 );
	pCellLL1->LatitudeSet( _Latitude0, true );

	CExtGridCellLL * pCellLL2 =
		STATIC_DOWNCAST(
			CExtGridCellLL,
			m_wndGrid.GridCellGet( nColNo, 2L )
			);
	pCellLL2->SetMode( CExtLLBaseControlProperties::longitude );
	pCellLL2->ModifyStyleEx( 0, __EGCS_EX_EMPTY );

	CExtGridCellLL * pCellLL3 =
		STATIC_DOWNCAST(
			CExtGridCellLL,
			m_wndGrid.GridCellGet( nColNo, 3L )
			);
	pCellLL3->SetMode( CExtLLBaseControlProperties::latitude );
	pCellLL3->ModifyStyleEx( 0, __EGCS_EX_EMPTY );

	CExtGridCellLL * pCellLL4 =
		STATIC_DOWNCAST(
			CExtGridCellLL,
			m_wndGrid.GridCellGet( nColNo, 4L )
			);
	pCellLL4->SetMode( CExtLLBaseControlProperties::longitude_latitude );
	pCellLL4->ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	pCellLL4->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
	pCellLL4->m_bMapDisplayMarkers = pCellLL4->m_bMapDisplayMarkerTips = pCellLL4->m_bMapDisplayCurrentLocationTip = true;
	pCellLL4->m_bMapHR = true;

	CExtGridCellLL * pCellLL5 =
		STATIC_DOWNCAST(
			CExtGridCellLL,
			m_wndGrid.GridCellGet( nColNo, 5L )
			);
	pCellLL5->SetMode( CExtLLBaseControlProperties::latitude_longitude );
	pCellLL5->ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	pCellLL5->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
	pCellLL5->m_bMapDisplayMarkers = pCellLL5->m_bMapDisplayMarkerTips = pCellLL5->m_bMapDisplayCurrentLocationTip = true;
	pCellLL5->m_bMapHR = true;

	CExtGridCellLL * pCellLL6 =
		STATIC_DOWNCAST(
			CExtGridCellLL,
			m_wndGrid.GridCellGet( nColNo, 6L )
			);
	pCellLL6->SetMode( CExtLLBaseControlProperties::longitude );
	pCellLL6->ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	pCellLL6->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
	pCellLL6->m_bMapHighlightTimeZones = pCellLL6->m_bMapDisplayTimeZoneTips = pCellLL6->m_bMapDisplayLocalDate = pCellLL6->m_bMapDisplayLocalTime = true;
	pCellLL6->m_bMapHR = true;

	CExtGridCellLL * pCellLL7 =
		STATIC_DOWNCAST(
			CExtGridCellLL,
			m_wndGrid.GridCellGet( nColNo, 7L )
			);
	pCellLL7->SetMode( CExtLLBaseControlProperties::latitude );
	pCellLL7->ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	pCellLL7->ModifyStyle( __EGCS_NO_INPLACE_CONTROL );
	//pCellLL7->m_bMapHR = true;

	CExtGridCellLL * pCellLL8 =
		STATIC_DOWNCAST(
			CExtGridCellLL,
			m_wndGrid.GridCellGet( nColNo, 8L )
			);
	pCellLL8->SetMode( CExtLLBaseControlProperties::longitude_latitude );
	pCellLL8->ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	//pCellLL8->ModifyStyle( __EGCS_READ_ONLY );
	pCellLL8->m_bMapDisplayMarkers = pCellLL8->m_bMapDisplayMarkerTips = pCellLL8->m_bMapDisplayCurrentLocationTip = true;
	pCellLL8->m_bMapHighlightTimeZones = pCellLL8->m_bMapDisplayTimeZoneTips = pCellLL8->m_bMapDisplayLocalDate = pCellLL8->m_bMapDisplayLocalTime = true;
	pCellLL8->m_bMapHR = true;

	CExtGridCellLL * pCellLL9 =
		STATIC_DOWNCAST(
			CExtGridCellLL,
			m_wndGrid.GridCellGet( nColNo, 9L )
			);
	pCellLL9->SetMode( CExtLLBaseControlProperties::latitude_longitude );
	pCellLL9->ModifyStyleEx( 0, __EGCS_EX_EMPTY );
	pCellLL9->ModifyStyle( __EGCS_READ_ONLY );
}

void CPageGrid::_InitColumnStringMasked( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );

// default mask symbol number
CExtEditWithBehavior::CBehaviorMasked::CharRule _rule1( RGB(164,0,0), _T('#'), (CExtEditWithBehavior::CBehaviorMasked::CharRule::ValidationFunction)_istdigit );
// default mask symbol letter
CExtEditWithBehavior::CBehaviorMasked::CharRule _rule2( RGB(0,164,0), _T('@'), (CExtEditWithBehavior::CBehaviorMasked::CharRule::ValidationFunction)_istalpha );
// default mask symbol number and letter
CExtEditWithBehavior::CBehaviorMasked::CharRule _rule3( RGB(0,0,164), _T('$'), (CExtEditWithBehavior::CBehaviorMasked::CharRule::ValidationFunction)_istalnum );
	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellStringMasked * pCellStringMasked =
			STATIC_DOWNCAST(
				CExtGridCellStringMasked,
				m_wndGrid.GridCellGet(
					nColNo,
					nRowNo,
					0,
					0,
					RUNTIME_CLASS(CExtGridCellStringMasked)
					)
				);
		CExtEditWithBehavior::CBehaviorMasked::CharRuleArray & _arrRules = pCellStringMasked->GetSymbolArray();
		_arrRules.Add( _rule1 );
		_arrRules.Add( _rule2 );
		_arrRules.Add( _rule3 );
	}

CExtGridCellStringMasked * pCellStringMasked0 = STATIC_DOWNCAST( CExtGridCellStringMasked, m_wndGrid.GridCellGet( nColNo, 0L ) );
	pCellStringMasked0->MaskSet( _T("($)$$ ####-@@@@ ($$y$$)") );
	pCellStringMasked0->TextSet( _T("(_)__ ____-____ (__y__)") );
	pCellStringMasked0->NullableModeSet( true );

CExtGridCellStringMasked * pCellStringMasked1 = STATIC_DOWNCAST( CExtGridCellStringMasked, m_wndGrid.GridCellGet( nColNo, 1L ) );
	pCellStringMasked1->MaskSet( _T("(####)-##-####-###") );
	pCellStringMasked1->TextSet( _T("(____)-__-____-___") );
	pCellStringMasked1->NullableModeSet( false );

CExtGridCellStringMasked * pCellStringMasked2 = STATIC_DOWNCAST( CExtGridCellStringMasked, m_wndGrid.GridCellGet( nColNo, 2L ) );
	pCellStringMasked2->MaskSet( _T("AnyChars-$$$$-ThenNumber-####") );
	pCellStringMasked2->TextSet( _T("AnyChars-____-ThenNumber-____") );
	pCellStringMasked2->InsertModeSet( false );

CExtGridCellStringMasked * pCellStringMasked3 = STATIC_DOWNCAST( CExtGridCellStringMasked, m_wndGrid.GridCellGet( nColNo, 3L ) );
	pCellStringMasked3->MaskSet( _T("Number-###-ThenLetters-@@@@@") );
	pCellStringMasked3->TextSet( _T("Number-___-ThenLetters-_____") );

CExtGridCellStringMasked * pCellStringMasked4 = STATIC_DOWNCAST( CExtGridCellStringMasked, m_wndGrid.GridCellGet( nColNo, 4L ) );
	pCellStringMasked4->MaskSet( _T("Tel. (###) ### ## ##") );
	pCellStringMasked4->TextSet( _T("Tel. (^^^) ^^^ ^^ ^^") );
	pCellStringMasked4->NullCharSet( _T('^') );
	pCellStringMasked4->InsertModeSet( false );

CExtGridCellStringMasked * pCellStringMasked5 = STATIC_DOWNCAST( CExtGridCellStringMasked, m_wndGrid.GridCellGet( nColNo, 5L ) );
	pCellStringMasked5->MaskSet( _T("Postal index: ## ###") );
	pCellStringMasked5->TextSet( _T("Postal index: xx xxx") );
	pCellStringMasked5->NullCharSet( _T('x') );

CExtGridCellStringMasked * pCellStringMasked6 = STATIC_DOWNCAST( CExtGridCellStringMasked, m_wndGrid.GridCellGet( nColNo, 6L ) );
	pCellStringMasked6->MaskSet( _T("Date: ## / ## / ####") );
	pCellStringMasked6->TextSet( _T("Date: -- / -- / ----") );
	pCellStringMasked6->NullCharSet( _T('-') );

CExtGridCellStringMasked * pCellStringMasked7 = STATIC_DOWNCAST( CExtGridCellStringMasked, m_wndGrid.GridCellGet( nColNo, 7L ) );
	pCellStringMasked7->MaskSet( _T("Five letter code: @@@@@") );
	pCellStringMasked7->TextSet( _T("Five letter code: *****") );
	pCellStringMasked7->NullCharSet( _T('*') );

CExtGridCellStringMasked * pCellStringMasked8 = STATIC_DOWNCAST( CExtGridCellStringMasked, m_wndGrid.GridCellGet( nColNo, 8L ) );
	pCellStringMasked8->MaskSet( _T("Number is ##") );
	pCellStringMasked8->TextSet( _T("Number is ~~") );
	pCellStringMasked8->ModifyStyle( __EGCS_TA_HORZ_RIGHT );
	pCellStringMasked8->NullCharSet( _T('~') );

CExtGridCellStringMasked * pCellStringMasked9 = STATIC_DOWNCAST( CExtGridCellStringMasked, m_wndGrid.GridCellGet( nColNo, 9L ) );
	pCellStringMasked9->MaskSet( _T("C++ ? # : #") );
	pCellStringMasked9->TextSet( _T("C++ ? 1 : 0") );
	pCellStringMasked9->ModifyStyle( __EGCS_TA_HORZ_CENTER );
}

void CPageGrid::_InitColumnSystemNumber( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );
	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellSystemNumber * pCellSystemNumber =
			STATIC_DOWNCAST(
				CExtGridCellSystemNumber,
				m_wndGrid.GridCellGet( nColNo, nRowNo, 0, 0, RUNTIME_CLASS(CExtGridCellSystemNumber) )
				);
		pCellSystemNumber->ModifyStyleEx( __EGCS_EX_INPLACE_RICH_EDIT );
		pCellSystemNumber->LongDoubleSet( ((long double)::rand()) + ((long double)::rand())/1000 );
		pCellSystemNumber->NumberColorSet( RGB(80,80,164) );
		pCellSystemNumber->FractionColorSet( RGB(80,164,80) );
		pCellSystemNumber->GroupSeparatorColorSet( RGB(128,128,128) );
		pCellSystemNumber->DecimalSeparatorColorSet( RGB(128,128,128) );
		pCellSystemNumber->SignColorSet( RGB(192,0,0) );
		pCellSystemNumber->BracketsColorSet( RGB(164,164,164) );
	}
}

void CPageGrid::_InitColumnSystemCurrency( LONG nColNo, LONG nRowCount )
{
	ASSERT_VALID( this );
	ASSERT_VALID( &m_wndGrid );
	for( LONG nRowNo = 0L; nRowNo < nRowCount; nRowNo++ )
	{
		CExtGridCellSystemCurrency * pCellSystemCurrency =
			STATIC_DOWNCAST(
				CExtGridCellSystemCurrency,
				m_wndGrid.GridCellGet( nColNo, nRowNo, 0, 0, RUNTIME_CLASS(CExtGridCellSystemCurrency) )
				);
		pCellSystemCurrency->ModifyStyleEx( __EGCS_EX_INPLACE_RICH_EDIT );
		pCellSystemCurrency->LongDoubleSet( ((long double)::rand()) + ((long double)::rand())/1000 );
		pCellSystemCurrency->NumberColorSet( RGB(80,80,164) );
		pCellSystemCurrency->FractionColorSet( RGB(80,164,80) );
		pCellSystemCurrency->GroupSeparatorColorSet( RGB(128,128,128) );
		pCellSystemCurrency->DecimalSeparatorColorSet( RGB(128,128,128) );
		pCellSystemCurrency->SignColorSet( RGB(192,0,0) );
		pCellSystemCurrency->BracketsColorSet( RGB(164,164,164) );
	}
}

void CPageGrid::OnTimer( __EXT_MFC_UINT_PTR nIDEvent )
{
	if( nIDEvent == 987 )
	{
		if( IsWindowVisible() )
		{
			if( m_wndGrid.IsPrintPreviewMode() )
				m_wndGrid.SendMessage( WM_IDLEUPDATECMDUI, 1 );
			else
				for( LONG nRowNo = 0L; nRowNo < 10L; nRowNo++ ) 
				{
					CExtGridCellProgress * pCellProgress =
						STATIC_DOWNCAST(
							CExtGridCellProgress ,
							m_wndGrid.GridCellGet(
								__COLUMN_NUM_PROGRESS,
								nRowNo
								)
							);
					if( pCellProgress != NULL )
					{
						pCellProgress->StepIt();
						CRect rcCellExtra;    
						if( m_wndGrid.GridCellRectsGet( __COLUMN_NUM_PROGRESS, nRowNo, 0, 0, NULL, &rcCellExtra ) )
							m_wndGrid.InvalidateRect( &rcCellExtra );
					}
				}
		}
		return;
	}
	CPageBase::OnTimer(nIDEvent);
}

BOOL CPageGrid::OnCmdMsg(UINT nID, INT nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( m_wndGrid.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo) )
		return TRUE;
	if( nCode == CN_COMMAND )
	{
		LONG nColNo = -1L;
		switch( nID )
		{
		case IDC_CELL_BOOLEAN:
			nColNo = __COLUMN_NUM_BOOLEAN;
		break;
		case IDC_CELL_CHECKBOX:
			nColNo = __COLUMN_NUM_CHECKBOX;
		break;
		case IDC_CELL_CHECKCOMBOBOX:
			nColNo = __COLUMN_NUM_CHECKCOMBOBOX;
		break;
		case IDC_CELL_COLOR:
			nColNo = __COLUMN_NUM_COLOR;
		break;
		case IDC_CELL_COLORPART:
			nColNo = __COLUMN_NUM_COLORPART;
		break;
		case IDC_CELL_COMBOBOX:
			nColNo = __COLUMN_NUM_COMBOBOX;
		break;
		case IDC_CELL_CURRENCY:
			nColNo = __COLUMN_NUM_CURRENCY;
		break;
		case IDC_CELL_DATETIME:
			nColNo = __COLUMN_NUM_DATETIME;
		break;
		case IDC_CELL_DURATION:
			nColNo = __COLUMN_NUM_DURATION;
		break;
		case IDC_CELL_FILE:
			nColNo = __COLUMN_NUM_FILE;
		break;
		case IDC_CELL_FILEIMAGE:
			nColNo = __COLUMN_NUM_FILEIMAGE;
		break;
		case IDC_CELL_FOLDER:
			nColNo = __COLUMN_NUM_FOLDER;
		break;
		case IDC_CELL_FONT:
			nColNo = __COLUMN_NUM_FONT;
		break;
		case IDC_CELL_FONTFACENAME:
			nColNo = __COLUMN_NUM_FONTFACENAME;
		break;
		case IDC_CELL_FONTWEIGHT:
			nColNo = __COLUMN_NUM_FONTWEIGHT;
		break;
		case IDC_CELL_FRACTION:
			nColNo = __COLUMN_NUM_FRACTION;
		break;
		case IDC_CELL_HYPERLINK:
			nColNo = __COLUMN_NUM_HYPERLINK;
		break;
		case IDC_CELL_IPADDRESS:
			nColNo = __COLUMN_NUM_IPADDRESS;
		break;
		case IDC_CELL_NUMBER:
			nColNo = __COLUMN_NUM_NUMBER;
		break;
		case IDC_CELL_PASSWORD:
			nColNo = __COLUMN_NUM_PASSWORD;
		break;
		case IDC_CELL_PERCENTAGE:
			nColNo = __COLUMN_NUM_PERCENTAGE;
		break;
		case IDC_CELL_PICTURE:
			nColNo = __COLUMN_NUM_PICTURE;
		break;
		case IDC_CELL_PROGRESS:
			nColNo = __COLUMN_NUM_PROGRESS;
		break;
		case IDC_CELL_RADIOBUTTON:
			nColNo = __COLUMN_NUM_RADIOBUTTON;
		break;
		case IDC_CELL_RECTPART:
			nColNo = __COLUMN_NUM_RECTPART;
		break;
		case IDC_CELL_INPLACE_SLIDER:
			nColNo = __COLUMN_NUM_INPLACE_SLIDER;
		break;
		case IDC_CELL_INPLACE_SCROLLBAR:
			nColNo = __COLUMN_NUM_INPLACE_SCROLLBAR;
		break;
		case IDC_CELL_SCIENTIFIC:
			nColNo = __COLUMN_NUM_SCIENTIFIC;
		break;
		case IDC_CELL_SLIDER:
			nColNo = __COLUMN_NUM_SLIDER;
		break;
		case IDC_CELL_TEXT:
			nColNo = __COLUMN_NUM_TEXT;
		break;
		case IDC_CELL_UPDOWN:
			nColNo = __COLUMN_NUM_UPDOWN;
		break;
		case IDC_CELL_VARIANT:
			nColNo = __COLUMN_NUM_VARIANT;
		break;
		case IDC_CELL_HOTKEY:
			nColNo = __COLUMN_NUM_HOTKEY;
		break;
		case IDC_CELL_BUTTON:
			nColNo = __COLUMN_NUM_BUTTON;
		break;
		case IDC_CELL_OBJECT_SIZE:
			nColNo = __COLUMN_NUM_OBJECT_SIZE;
		break;
		case IDC_CELL_GEOGRAPHIC:
			nColNo = __COLUMN_NUM_GEOGRAPHIC;
		break;
		case IDC_CELL_MASKED_STRING:
			nColNo = __COLUMN_NUM_MASKED_STRING;
		break;
		case IDC_CELL_SYSTEM_NUMBER:
			nColNo = __COLUMN_NUM_SYSTEM_NUMBER;
		break;
		case IDC_CELL_SYSTEM_CURRENCY:
			nColNo = __COLUMN_NUM_SYSTEM_CURRENCY;
		break;
		default:
			return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		}
		
		if(		nColNo >= 0L 
			&&	m_wndGrid.m_hWnd != NULL
			&&	::IsWindow( m_wndGrid.m_hWnd )
			)
		{
			m_wndGrid.FocusSet( CPoint( nColNo, 0 ), true, true, true, false );
			m_wndGrid.SelectionSet( CRect( nColNo, 0, nColNo, m_wndGrid.RowCountGet() - 1 ), true, true, true );
			m_wndGrid.SetFocus();
		}

		return TRUE;
	}
	return CPageBase::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

#endif // #if (!defined __EXT_MFC_NO_GRIDWND)

