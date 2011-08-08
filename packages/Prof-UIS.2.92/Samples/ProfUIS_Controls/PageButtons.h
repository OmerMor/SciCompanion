#if !defined(AFX_PAGEBUTTONS_H__D5F47AD0_2BD7_4BFE_91DD_AD9337222252__INCLUDED_)
#define AFX_PAGEBUTTONS_H__D5F47AD0_2BD7_4BFE_91DD_AD9337222252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageButtons.h : header file
//
#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageButtons dialog

//////////////////////////////////////////////////////////////////////////
// class CPageButtons
//////////////////////////////////////////////////////////////////////////

class CPageButtons : public CPageBase
{
// Construction
public:
	CPageButtons(CWnd* pParent = NULL);   // standard constructor

	/////////////////////////////////////////////////////////////////////////////
	// CSkinCheckBox window
	/////////////////////////////////////////////////////////////////////////////
	
	class CSkinCheckBox : public CExtCheckBox
	{
	public:
		CSkinCheckBox()
			: CExtCheckBox()
			, m_nBitmapResource( 0 )
		{
		}
		int m_nBitmapResource;
		
	protected:
		virtual bool OnPaintBox(
			CDC & dc,
			CRect & rcBox,
			CExtPaintManager::eBoxState_t eState
			)
		{
			if( m_nBitmapResource == 0 )
				return CExtCheckBox::OnPaintBox(dc,rcBox,eState);

			COLORREF clrBackground = GetBkColor();

			CRect rcClient;
			GetClientRect( &rcClient );

			CRgn rgnBox;
			if( rgnBox.CreateRectRgnIndirect( &rcBox ) )
				dc.SelectClipRgn( &rgnBox );

			bool bTransparent = false;
			if(		(! bTransparent )
				&&	g_PaintManager->GetCb2DbTransparentMode(this)
				&&	( clrBackground == COLORREF(-1L) )
				)
			{
				if( g_PaintManager->PaintDockerBkgnd( true, dc, this ) )
					bTransparent = true;
			}
			if( ! bTransparent )
				dc.FillSolidRect( 
					&rcBox, 
					(clrBackground != COLORREF(-1L)) 
						? clrBackground 
						: g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_OUT, this ) 
				);

			INT nImageIndex = 0;
			switch( eState ) 
			{
			case CExtPaintManager::BOX_UNCHECKED:
				nImageIndex = 0;
				break;
			case CExtPaintManager::BOX_CHECKED:
				nImageIndex = 1;
				break;
			case CExtPaintManager::BOX_INDETERMINATE:
				nImageIndex = 2;
				break;
			
			case CExtPaintManager::BOX_LDOWN_UNCHECKED:
				nImageIndex = 3;
				break;
			case CExtPaintManager::BOX_LDOWN_CHECKED:
				nImageIndex = 4;
				break;
			case CExtPaintManager::BOX_LDOWN_INDETERMINATE:
				nImageIndex = 5;
				break;
			
			case CExtPaintManager::BOX_MOUSE_HOVER_UNCHECKED:
				nImageIndex = 6;
				break;
			case CExtPaintManager::BOX_MOUSE_HOVER_CHECKED:
				nImageIndex = 7;
				break;
			case CExtPaintManager::BOX_MOUSE_HOVER_INDETERMINATE:
				nImageIndex = 8;
				break;
			
			case CExtPaintManager::BOX_DISABLED_UNCHECKED:
				nImageIndex = 9;
				break;
			case CExtPaintManager::BOX_DISABLED_CHECKED:
				nImageIndex = 10;
				break;
			case CExtPaintManager::BOX_DISABLED_INDETERMINATE:
				nImageIndex = 11;
				break;
			}
			
			CExtImageList il(m_nBitmapResource, RGB(255,0,255), 13);
			bool bRet = il.Draw(
				&dc, 
				nImageIndex, 
				rcBox.TopLeft(), 
				ILD_NORMAL
				) == TRUE;

			dc.SelectClipRgn( NULL );
			
			return bRet;
		}
	};// class CSkinCheckBox
	
	/////////////////////////////////////////////////////////////////////////////
	// CSkinRadioButton window
	/////////////////////////////////////////////////////////////////////////////
	
	class CSkinRadioButton : public CExtRadioButton
	{
	public:
		CSkinRadioButton()
			: CExtRadioButton()
			, m_nBitmapResource( 0 )
		{
		}
		int m_nBitmapResource;
		
	protected:
		virtual bool OnPaintBox(
			CDC & dc,
			CRect & rcBox,
			CExtPaintManager::eBoxState_t eState
			)
		{
			if( m_nBitmapResource == 0 )
				return CExtRadioButton::OnPaintBox(dc,rcBox,eState);
			
			COLORREF clrBackground = GetBkColor();

			CRect rcClient;
			GetClientRect( &rcClient );

			CRgn rgnBox;
			if( rgnBox.CreateRectRgnIndirect( &rcBox ) )
				dc.SelectClipRgn( &rgnBox );

			bool bTransparent = false;
			if(		(! bTransparent )
				&&	g_PaintManager->GetCb2DbTransparentMode(this)
				&&	( clrBackground == COLORREF(-1L) )
				)
			{
				if( g_PaintManager->PaintDockerBkgnd( true, dc, this ) )
					bTransparent = true;
			}
			if( ! bTransparent )
				dc.FillSolidRect( 
					&rcBox, 
					(clrBackground != COLORREF(-1L)) 
						? clrBackground 
						: g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_OUT, this ) 
				);

			INT nImageIndex = 0;
			switch( eState ) 
			{
			case CExtPaintManager::BOX_UNCHECKED:
				nImageIndex = 0;
				break;
			case CExtPaintManager::BOX_CHECKED:
				nImageIndex = 1;
				break;
			
			case CExtPaintManager::BOX_LDOWN_UNCHECKED:
				nImageIndex = 2;
				break;
			case CExtPaintManager::BOX_LDOWN_CHECKED:
				nImageIndex = 3;
				break;
			
			case CExtPaintManager::BOX_MOUSE_HOVER_UNCHECKED:
				nImageIndex = 4;
				break;
			case CExtPaintManager::BOX_MOUSE_HOVER_CHECKED:
				nImageIndex = 5;
				break;
			
			case CExtPaintManager::BOX_DISABLED_UNCHECKED:
				nImageIndex = 6;
				break;
			case CExtPaintManager::BOX_DISABLED_CHECKED:
				nImageIndex = 7;
				break;
			}
			
			CExtImageList il(m_nBitmapResource, RGB(255,0,255), 13);
			bool bRet = il.Draw(
				&dc, 
				nImageIndex, 
				rcBox.TopLeft(), 
				ILD_NORMAL
				) == TRUE;

			dc.SelectClipRgn( NULL );

			return bRet;
		}
	};// class CSkinRadioButton
	
// Dialog Data
	//{{AFX_DATA(CPageButtons)
	enum { IDD = IDD_PAGE_BUTTONS };
	CExtButton	m_wndTextMarginsReset;
	CExtButton	m_wndIconMarginsReset;
	CExtSpinWnd m_wndSpinIconMarginsTop;
	CExtSpinWnd	m_wndSpinIconMarginsRight;
	CExtSpinWnd	m_wndSpinIconMarginsLeft;
	CExtSpinWnd	m_wndSpinIconMarginsBottom;
	CExtSpinWnd	m_wndSpinTextMarginsTop;
	CExtSpinWnd	m_wndSpinTextMarginsRight;
	CExtSpinWnd	m_wndSpinTextMarginsLeft;
	CExtSpinWnd	m_wndSpinTextMarginsBottom;
	CExtEdit	m_wndTextMarginsTop;
	CExtEdit	m_wndTextMarginsRight;
	CExtEdit	m_wndTextMarginsLeft;
	CExtEdit	m_wndTextMarginsBottom;
	CExtEdit	m_wndIconMarginsTop;
	CExtEdit	m_wndIconMarginsRight;
	CExtEdit	m_wndIconMarginsLeft;
	CExtEdit	m_wndIconMarginsBottom;
	CExtGroupBox	m_wndGroupBoxTextMargins;
	CExtGroupBox	m_wndGroupBoxIconMargins;
	CExtLabel	m_wndlabelIconAlignmentVert;
	CExtLabel	m_wndlabelIconAlignmentHorz;
	CExtLabel	m_wndlabelTextAlignmentVert;
	CExtLabel	m_wndlabelTextAlignmentHorz;
	CExtComboBox	m_wndTextAlignmentVert;
	CExtComboBox	m_wndTextAlignmentHorz;
	CExtComboBox	m_wndIconAlignmentVert;
	CExtComboBox	m_wndIconAlignmentHorz;
	CExtGroupBox	m_wndGroupBoxTextAlignment;
	CExtGroupBox	m_wndGroupBoxIconAlignment;
	CSkinRadioButton	m_wndRadio1;
	CSkinRadioButton m_wndRadio2;
	CSkinCheckBox	m_chkCheckBox2;
	CSkinCheckBox	m_chkCheckBox1;
	CExtButton		m_btnWithMenu;
	CExtColorButton	m_btnColor2;
	CExtColorButton	m_btnColor1;
	CExtCheckBox	m_chkCheckBoxShowFocusRect;
	CExtEdit		m_Tooltip;
	CExtLabel		m_wndLabelTooltip;
	CExtLabel		m_wndLabelSkin;
	CExtGroupBox	m_wndGroupBoxCheck; 
	CExtLabel		m_wndLabelCursor;
	CExtComboBox	m_CheckBoxSkin;
	CExtButton		m_btnPushLikeButton;
	CExtButton		m_btnPushButton;
	CExtComboBox	m_Cursor;
	CExtCheckBox	m_chkTriState;
	CExtCheckBox	m_chkButtonShowFocusRect;
	CExtCheckBox	m_chkButtonEnable;
	CExtCheckBox	m_chkCheckBoxEnable;
	CExtCheckBox	m_chkLeftText;
	CExtCheckBox	m_chkSeparatedButtons;
	CExtCheckBox	m_chkFlat;
	CExtCheckBox	m_chkBorders;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageButtons)
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;
	
	// Generated message map functions
	//{{AFX_MSG(CPageButtons)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokSkin();
	afx_msg void OnSelendokCursor();
	afx_msg void OnSelendokIconAlignmentHorz();
	afx_msg void OnSelendokIconAlignmentVert();
	afx_msg void OnSelendokTextAlignmentHorz();
	afx_msg void OnSelendokTextAlignmentVert();
	afx_msg void OnTextMarginsReset();
	afx_msg void OnIconMarginsReset();
	//}}AFX_MSG
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawPopupLeftArea(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEBUTTONS_H__D5F47AD0_2BD7_4BFE_91DD_AD9337222252__INCLUDED_)
