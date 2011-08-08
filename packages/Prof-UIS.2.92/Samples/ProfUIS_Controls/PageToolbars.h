#if !defined(AFX_PAGETOOLBARS_H__9A124E60_CFC1_4030_9A92_02516BB0C86B__INCLUDED_)
#define AFX_PAGETOOLBARS_H__9A124E60_CFC1_4030_9A92_02516BB0C86B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageToolbars.h : header file
//
#include "PageBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPageToolbars dialog

class CPageToolbars : public CPageBase
{
// Construction
public:
	CPageToolbars(CWnd* pParent = NULL);   // standard constructor
	void _Update();

// Dialog Data
	//{{AFX_DATA(CPageToolbars)
	enum { IDD = IDD_PAGE_TOOLBARS };
	CExtCheckBox	m_chkForceBalloonStyle;
	CExtCheckBox	m_chkShowGripper;
	//}}AFX_DATA

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageToolbars)
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInitComplete:1;
	COLORREF m_clrColorCtrl;
	
	class CThinColorButtonInToolBar : public CExtBarButton
	{
		COLORREF m_clr;
	public:
		CThinColorButtonInToolBar(
			COLORREF clr,
			CExtToolControlBar * pBar
			)
			: CExtBarButton(
				pBar,
				g_CmdManager->CmdAllocPtr(
					g_CmdManager->ProfileNameFromWnd(
						pBar->GetSafeHwnd()
						)
					)->m_nCmdID
				),
			m_clr( clr )
		{
			CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd(
						pBar->GetSafeHwnd()
						),
					GetCmdID()
					);
			ASSERT( pCmdItem != NULL );
			pCmdItem->StateSetBasic();

			pCmdItem->m_sMenuText.Format(
				_T( "RGB color is 0x%02X%02X%02X" ),
				INT( GetRValue(clr) ),
				INT( GetGValue(clr) ),
				INT( GetBValue(clr) )
				);
			pCmdItem->m_sTipStatus.Format(
				_T( "Color in the left toolbar\nRGB color is 0x%02X%02X%02X" ),
				INT( GetRValue(clr) ),
				INT( GetGValue(clr) ),
				INT( GetBValue(clr) )
				);
			//pCmdItem->m_sTipTool = pCmdItem->m_sTipStatus;

			HICON hIcon = CExtPaintManager::stat_GenerateColorIconBox( clr );
			ASSERT( hIcon != NULL );
			VERIFY(
				g_CmdManager->CmdSetIcon(
					g_CmdManager->ProfileNameFromWnd(
						pBar->GetSafeHwnd()
						),
					pCmdItem->m_nCmdID,
					hIcon,
					false
					)
				);

		};
	
		virtual CSize CalculateLayout(
			CDC & dc,
			CSize sizePreCalc,
			BOOL bHorz
			)
		{
			ASSERT_VALID( this );

			CSize _size =
				CExtBarButton::CalculateLayout(
					dc,
					sizePreCalc,
					bHorz
					);
			CExtToolControlBar * pBar = GetBar();
			ASSERT_VALID( pBar );
			CPageToolbars * pDlg = (CPageToolbars *)pBar->GetParent();
			ASSERT_VALID( pDlg );
			if( bHorz )
				_size.cx = pDlg->m_nThinColorBtnHeight + 1;
			else
				_size.cy = pDlg->m_nThinColorBtnHeight + 1;
			return _size;
		}

		virtual void PaintCompound(
			CDC & dc,
			bool bPaintParentChain,
			bool bPaintChildren,
			bool bPaintOneNearestChildrenLevelOnly
			)
		{
			ASSERT_VALID( this );

			if( ! IsPaintAble( dc ) )
				return;
			if( AnimationClient_StatePaint( dc ) )
				return;
			if( bPaintParentChain )
				PaintParentChain( dc );

			CRect rcArea( Rect() );
			rcArea.DeflateRect(
				__EXT_TB_BUTTON_INNER_MARGIN,
				__EXT_TB_BUTTON_INNER_MARGIN,
				__EXT_TB_BUTTON_INNER_MARGIN,
				__EXT_TB_BUTTON_INNER_MARGIN+1
				);
			dc.FillSolidRect(
				&rcArea,
				m_clr
				);
			bool bEnabled = IsDisabled() ? false : true;
			bool bPushed =
					IsPressed() ? true : false;
			bool bHover =
					(	bEnabled
						&& IsHover()
						&& !CExtToolControlBar::g_bMenuTracking
						&& !CExtPopupMenuWnd::IsMenuTracking()
					) ? true : false;
			if( bPushed || bHover )
			{
				COLORREF clrTL = g_PaintManager->GetColor( COLOR_3DHIGHLIGHT, this );
				COLORREF clrBR = g_PaintManager->GetColor( COLOR_3DDKSHADOW, this );
				if( bPushed )
				{
					dc.Draw3dRect( rcArea, clrBR, clrTL );
				}
				else
				{
					//dc.Draw3dRect( rcArea, clrTL, clrBR );
					dc.Draw3dRect( rcArea, clrBR, clrBR );
					rcArea.DeflateRect( 1, 1 );
					if( rcArea.Height() > 3 )
					{
						dc.Draw3dRect( rcArea, clrTL, clrTL );
						rcArea.DeflateRect( 1, 1 );
						if( rcArea.Height() > 3 )
							dc.Draw3dRect( rcArea, clrBR, clrBR );
					}
				}
			}

			if( bPaintChildren )
				PaintChildren( dc, bPaintOneNearestChildrenLevelOnly );
		}

		virtual void OnHover(
			CPoint point,
			bool bOn,
			bool & bSuspendTips
			)
		{
			ASSERT_VALID( this );

			CExtBarButton::OnHover(
				point,
				bOn,
				bSuspendTips
				);

			CExtToolControlBar * pBar = GetBar();
			ASSERT_VALID( pBar );
			CPageToolbars * pDlg = (CPageToolbars *)pBar->GetParent();
			ASSERT_VALID( pDlg );

			if( bOn )
			{
				if( CExtPopupMenuWnd::IsMenuTracking() )
					return;
				CExtCmdItem * pCmdItem =
				g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd(
						pBar->GetSafeHwnd()
						),
					GetCmdID()
					);
				ASSERT( pCmdItem != NULL );
				pDlg->m_wndLeftBarTip.SetText( pCmdItem->m_sTipStatus );
				CRect rcAreaScreen = Rect();
				pBar->ClientToScreen( &rcAreaScreen );
				pDlg->m_wndLeftBarTip.Show( pBar, rcAreaScreen );
				//pDlg->m_wndStatusBar.SetPaneText( 0, pCmdItem->m_sTipStatus );
			}
			else
			{
				pDlg->m_wndLeftBarTip.Hide();
			}
		}
		
		virtual void OnClick(
			CPoint point,
			bool bDown
			)
		{
			ASSERT_VALID( this );

			CExtToolControlBar * pBar = GetBar();
			ASSERT_VALID( pBar );
			CPageToolbars * pDlg = (CPageToolbars *)pBar->GetParent();
			ASSERT_VALID( pDlg );

			pDlg->m_wndLeftBarTip.Hide();

			CExtBarButton::OnClick(
				point,
				bDown
				);
		}

	}; // class CThinColorButtonInToolBar

	friend class CThinColorButtonInToolBar;

	class CZoomBarSliderButton: public CExtBarSliderButton
	{
	public:
		CZoomBarSliderButton(
			CExtToolControlBar * pBar = NULL,
			UINT nCmdID = ID_SEPARATOR,
			UINT nStyle = 0,
			ULONG nScrollTotalRange = 0L,
			ULONG nScrollPos = 0L,
			ULONG nScrollPageSize = 0L,
			INT nScrollButtonExtentH = 0,
			INT nScrollButtonExtentV = 0,
			INT nScrollControlExtentH = 50,
			INT nScrollControlExtentV = 50
			)
			: CExtBarSliderButton(
			pBar,
			nCmdID,
			nStyle,
			nScrollTotalRange,
			nScrollPos,
			nScrollPageSize,
			nScrollButtonExtentH,
			nScrollButtonExtentV,
			nScrollControlExtentH,
			nScrollControlExtentV
			)
		{
		}
		~CZoomBarSliderButton()
		{
		}
			
		ULONG ScrollPosSet( ULONG nScrollPos )
		{
			ULONG ulRet = CExtBarSliderButton::ScrollPosSet( nScrollPos );
			RedrawButton();
			return ulRet;
		}
	}; // class CZoomBarSliderButton
	
	
	class CSeekBarSliderButton: public CExtBarSliderButton
	{
	public:
		CSeekBarSliderButton(
			CExtToolControlBar * pBar = NULL,
			UINT nCmdID = ID_SEPARATOR,
			UINT nStyle = 0,
			ULONG nScrollTotalRange = 0L,
			ULONG nScrollPos = 0L,
			ULONG nScrollPageSize = 0L,
			INT nScrollButtonExtentH = 0,
			INT nScrollButtonExtentV = 0,
			INT nScrollControlExtentH = 50,
			INT nScrollControlExtentV = 50
			)
			: CExtBarSliderButton(
			pBar,
			nCmdID,
			nStyle,
			nScrollTotalRange,
			nScrollPos,
			nScrollPageSize,
			nScrollButtonExtentH,
			nScrollButtonExtentV,
			nScrollControlExtentH,
			nScrollControlExtentV
			)
		{
		}
		~CSeekBarSliderButton()
		{
		}
		
		ULONG ScrollPosSet( ULONG nScrollPos)
		{
			ULONG ulRet = CExtBarSliderButton::ScrollPosSet( nScrollPos );
			RedrawButton();
			return ulRet;
		}

		virtual void OnClick(
			CPoint point,
			bool bDown
			)
		{
			CExtBarSliderButton::OnClick(point,bDown);
		}
		
	};// class CSeekBarSliderButton
	
	CExtMenuControlBar	m_wndMenuBar;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;
	CExtToolControlBar	m_wndToolBarColor;
	CExtToolControlBar	m_wndToolBarControls1;
	CExtToolControlBar	m_wndToolBarControls2;
	CExtToolControlBar	m_wndToolBarPalette;
	CExtComboBox m_wndComboBoxInToolbar;
	CExtComboBox m_wndComboBoxInMenubar;
	CExtEdit m_wndEditInToolbar;
	CExtEdit m_wndEditInMenubar;
	CExtLabel m_wndLabelInMenubar;
	CExtLabel m_wndLabelInToolbar;
	
	INT m_nThinColorBtnHeight;
	CExtPopupMenuTipWnd m_wndLeftBarTip;
	
	// Generated message map functions
	//{{AFX_MSG(CPageToolbars)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGETOOLBARS_H__9A124E60_CFC1_4030_9A92_02516BB0C86B__INCLUDED_)
