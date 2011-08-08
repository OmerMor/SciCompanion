#if !defined(AFX_PAGESTATUSBAR_H__9E21316E_7C7F_4E8E_B28F_7DC29EB448A3__INCLUDED_)
#define AFX_PAGESTATUSBAR_H__9E21316E_7C7F_4E8E_B28F_7DC29EB448A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageStatusBar.h : header file
//
#include "PageBase.h"

class CExtStatusBarSliderCtrl : public CExtSliderWnd
{
public:
	virtual void PostNcDestroy()
	{
		delete this;
	}
	//{{AFX_MSG(CExtStatusBarSliderCtrl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CExtStatusBarSliderCtrl

class CExtStatusBarAnimateCtrl : public CAnimateCtrl
{
public:
	DECLARE_DYNCREATE( CExtStatusBarAnimateCtrl );		

	CExtStatusBarAnimateCtrl();
	virtual ~CExtStatusBarAnimateCtrl();

	//{{AFX_MSG(CExtStatusBarAnimateCtrl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
protected:
	COLORREF m_clrBackgroundPrev;
	CBrush m_brBack;

	virtual void PostNcDestroy()
	{
		delete this;
	}
}; // class CExtStatusBarAnimateCtrl

/////////////////////////////////////////////////////////////////////////////
// CPageStatusBar dialog

class CPageStatusBar : public CPageBase
{
// Construction
public:
	CPageStatusBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageStatusBar)
	enum { IDD = IDD_PAGE_STATUS_BAR };
	CExtEdit		m_wndPosition;
	CExtRadioButton m_wndRadioText;
	CExtRadioButton	m_wndRadioStatic;
	CExtRadioButton	m_wndRadioSlider;
	CExtRadioButton	m_wndRadioProgress;
	CExtRadioButton	m_wndRadioEdit;
	CExtRadioButton	m_wndRadioButton;
	CExtRadioButton	m_wndRadioAnimCtrl;
	CExtRadioButton m_wndRadioToolbar;
	CExtRadioButton m_wndRadioZoom;
	CExtRadioButton m_wndRadioScrollBar;
	CExtLabel		m_StaticPosition;
	CExtButton		m_btnDisablePane;
	CExtSpinWnd		m_SpinPosition;
	CExtButton		m_btnRemovePane;
	CExtButton		m_btnAddPane;
	CExtCheckBox	m_chkHideTextOnDisabledPanes;
	CExtCheckBox	m_chkOuterRectInFirstBand;
	CExtCheckBox	m_chkDrawPaneSeparators;
	int				m_nPosition;
	//}}AFX_DATA

	class CExtStatusBarEdit : public CExtEdit{
		virtual void PostNcDestroy(){
			delete this;
		}
	}; // class CExtStatusBarEdit

	class CExtStatusBarButton : public CExtButton
	{
	protected:
		virtual LRESULT WindowProc(    
			UINT uMsg,
			WPARAM wParam,
			LPARAM lParam
			)
		{
			if(  uMsg == BM_SETSTYLE 
				&& wParam == BS_DEFPUSHBUTTON
				)
				return 0L;
			return CExtButton::WindowProc(uMsg, wParam, lParam);
		}
		virtual void PostNcDestroy()
		{
			delete this;
		}
	}; // class CExtStatusBarButton

	class CExtStatusBarLabel : public CExtLabel
	{
		virtual void OnDrawLabelText(
			CDC & dc,
			const RECT & rcText,
			__EXT_MFC_SAFE_LPCTSTR strText,
			DWORD dwDrawTextFlags,
			bool bEnabled
			)
		{
			bEnabled;
			INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
			LOGFONT lf;
			memset( &lf, 0, sizeof(LOGFONT) );
			lf.lfQuality = PROOF_QUALITY;
			lf.lfHeight = 16;
			lf.lfWeight = 600;
			lstrcpy( lf.lfFaceName, _T("Arial") );
			CFont f;
			f.CreateFontIndirect( &lf );
			CFont * pOldFont = dc.SelectObject( &f );

			CRect rc( rcText );
			COLORREF clrOldText =
				dc.SetTextColor( g_PaintManager->GetColor( COLOR_3DHILIGHT, this ) );
			rc.top -= 3;
			CExtRichContentLayout::stat_DrawText(
				dc.m_hDC,
				LPCTSTR(strText), int( _tcslen( LPCTSTR( strText ) ) ),
				rc,
				dwDrawTextFlags, 0
				);
			
			rc = rcText;
			rc.top -= 1;
			rc.left += 2;
			dc.SetTextColor( GetSysColor( COLOR_3DSHADOW ) );
			CExtRichContentLayout::stat_DrawText(
				dc.m_hDC,
				LPCTSTR(strText), int( _tcslen( LPCTSTR( strText ) ) ),
				rc,
				dwDrawTextFlags, 0
				);
			
			rc = rcText;
			rc.left += 1;
			rc.top -= 2;
			dc.SetTextColor( RGB(0,0,255) );
			CExtRichContentLayout::stat_DrawText(
				dc.m_hDC,
				LPCTSTR(strText), int( _tcslen( LPCTSTR( strText ) ) ),
				rc,
				dwDrawTextFlags, 0
				);
			
			dc.SelectObject( pOldFont );
			f.DeleteObject();
			dc.SetBkMode( nOldBkMode );
			dc.SetTextColor( clrOldText );
		}
		void OnEraseBackground(
			CDC & dc,
			const CRect & rcClient
			)
		{
			ASSERT_VALID( this );
			COLORREF clrBackground = GetBkColor();
			bool bTransparent = false;
			if( clrBackground == COLORREF(-1L) )
			{
				CExtStatusControlBar * pWndParent = 
					DYNAMIC_DOWNCAST( CExtStatusControlBar, GetParent() );
				ASSERT( pWndParent != NULL );

				CRect rcPaint;
				pWndParent->GetWindowRect( &rcPaint );
				ScreenToClient( &rcPaint );

				CRect _rcClient( rcClient );
				ClientToScreen( &_rcClient );
				pWndParent->ScreenToClient( &_rcClient );
				
				if( pWndParent->m_bCompleteRepaint )
				{
					dc.OffsetViewportOrg( 
						_rcClient.left, 
						0 
						);

					bool bRet = 
						PmBridge_GetPM()->StatusBar_EraseBackground(
							dc,
							rcPaint,
							pWndParent
							);

					dc.OffsetViewportOrg( 
						-_rcClient.left, 
						0 
						);

					if( !bRet )
					{
						bool bTransparent = false;
						if( PmBridge_GetPM()->GetCb2DbTransparentMode(pWndParent) )
						{
							if( PmBridge_GetPM()->PaintDockerBkgnd( true, dc, this ) )
								bTransparent = true;
						}
						if( ! bTransparent )
							dc.FillSolidRect( 
								&rcPaint, 
								PmBridge_GetPM()->GetColor( CExtPaintManager::CLR_3DFACE_OUT, pWndParent ) 
								);
					}

					INT nIndex = pWndParent->CommandToIndex( IDS_PANE_LABEL );
					if( nIndex >= 0 )
					{
						dc.OffsetViewportOrg( 
							_rcClient.left, 
							0 
							);
						PmBridge_GetPM()->StatusBar_ErasePaneBackground(
							dc,
							nIndex,
							rcPaint,
							pWndParent
							);
						dc.OffsetViewportOrg( 
							-_rcClient.left, 
							0 
							);
						bTransparent = true;
					}
				} // if( pWndParent->m_bCompleteRepaint )

			}
			if( ! bTransparent )
				dc.FillSolidRect(
					&rcClient,
					(clrBackground != COLORREF(-1L)) 
						? clrBackground 
						: PmBridge_GetPM()->GetColor( CExtPaintManager::CLR_3DFACE_OUT, this ) 
					);	
		}
		virtual void PostNcDestroy()
		{
			delete this;
		}
	}; // class CExtStatusBarLabel

	class CExtStatusBarProgressCtrl : public CExtProgressWnd
	{
	public:
		CExtStatusBarProgressCtrl()
		{
		}
	protected:
		virtual LRESULT WindowProc(    
			UINT uMsg,
			WPARAM wParam,
			LPARAM lParam
			)
		{
//			if( uMsg == WM_ERASEBKGND )
//				return (!0);
//			if( uMsg == WM_PAINT )
//			{
//				CRect rcClient;
//				GetClientRect( &rcClient );
//				CPaintDC dcPaint( this );
//				CExtMemoryDC dc( &dcPaint, &rcClient );
//				if( g_PaintManager->GetCb2DbTransparentMode(this) )
//				{
//					CExtPaintManager::stat_ExcludeChildAreas(
//						dc,
//						GetSafeHwnd(),
//						CExtPaintManager::stat_DefExcludeChildAreaCallback
//						);
//					g_PaintManager->PaintDockerBkgnd( true, dc, this );
//				} // if( g_PaintManager->GetCb2DbTransparentMode(this) )
//				else
//					dc.FillSolidRect( &rcClient, g_PaintManager->GetColor( CExtPaintManager::CLR_3DFACE_OUT, this ) );
//				DefWindowProc( WM_PAINT, WPARAM(dc.GetSafeHdc()), 0L );
//				g_PaintManager->OnPaintSessionComplete( this );
//				return 0;
//			}
			if( uMsg == WM_TIMER )
			{
				StepIt();
			}
			else if( uMsg == WM_DESTROY )
			{
				KillTimer(0);
			}
			LRESULT lResult = CExtProgressWnd::WindowProc( uMsg, wParam, lParam );
			return lResult;
		}
		
		virtual void PostNcDestroy(){
			delete this;
		}
	}; // class CExtStatusBarProgressCtrl

	class CExtStatusBarToolbar : public CExtToolControlBar
	{
	public:
		CExtStatusBarToolbar()
		{
			m_cxLeftBorder = m_cxRightBorder = 
				m_cyTopBorder = m_cyBottomBorder = 0;
		}

		class TinyButton : public CExtBarButton
		{
		public:
			TinyButton(
				CExtToolControlBar * pBar = NULL,
				UINT nCmdID = ID_SEPARATOR,
				UINT nStyle = 0
				)
				: CExtBarButton(
					pBar,
					nCmdID,
					nStyle
					)
			{
			}
			virtual CSize CalculateLayout(
				CDC & dc,
				CSize sizePreCalc,
				BOOL bHorz
				)
			{
				ASSERT_VALID( this );
				dc;
				sizePreCalc;
				bHorz;
				return CSize( 15, 14 );
			}
		}; // class TinyButton
	protected:
		virtual LRESULT WindowProc(
			UINT message,
			WPARAM wParam,
			LPARAM lParam
			)
		{
			ASSERT_VALID( this );
			if( message == WM_NCCALCSIZE )
				return 0L;
			return CExtToolControlBar::WindowProc( message, wParam, lParam );
		}
		virtual CExtBarContentExpandButton * OnCreateBarRightBtn()
		{
			ASSERT_VALID( this );
			return NULL;
		}
		virtual CExtBarButton * OnCreateBarCommandBtn(
			UINT nCmdID,
			UINT nStyle = 0
			)
		{
			ASSERT_VALID( this );
			CExtBarButton * pTBB = new TinyButton( this, nCmdID, nStyle );
			ASSERT_VALID( pTBB );
			return pTBB;
		}

		virtual void _RecalcPositionsImpl()
		{
			if(		m_pDockSite->GetSafeHwnd() != NULL
				&&	IsFloating()
				)
			{
				CExtToolControlBar::_RecalcPositionsImpl();
				return;
			}
			INT nCountOfButtons = GetButtonsCount();
			if( nCountOfButtons == 0 )
				return;
			CExtBarButton * pTBB = GetButton( nCountOfButtons - 1 );
			ASSERT_VALID( pTBB );
			CExtToolControlBar::_RecalcPositionsImpl();
			CRect rcClient;
			GetClientRect( &rcClient );
			INT nButtonIndex, nSummaryButtonsExtent = 0;
			for( nButtonIndex = 0; nButtonIndex < nCountOfButtons; nButtonIndex ++ )
			{
				pTBB = GetButton( nButtonIndex );
				ASSERT_VALID( pTBB );
				if( ! pTBB->IsVisible() )
					continue;
				if( (pTBB->GetStyle()&TBBS_HIDDEN) != 0 )
					continue;
				CRect rcButton = *pTBB;
				INT nButtonExtent = rcButton.Width();
				nSummaryButtonsExtent += nButtonExtent;
			}

			INT nClientExtent = rcClient.Width();
			int nShift = 0;

			nShift = (nClientExtent - nSummaryButtonsExtent) / 2;

			if( nShift == 0 )
				return;
			for( nButtonIndex = 0; nButtonIndex < nCountOfButtons; nButtonIndex ++ )
			{
				pTBB = GetButton( nButtonIndex );
				ASSERT_VALID( pTBB );
				if( ! pTBB->IsVisible() )
					continue;
				if( (pTBB->GetStyle()&TBBS_HIDDEN) != 0 )
					continue;
				CRect rcButton = *pTBB;
				rcButton.OffsetRect( nShift, 1 );
				pTBB->SetRect( rcButton );
			}
		}
		virtual CSize _CalcLayout(
			DWORD dwMode,
			int nLength = -1
			)
		{
			ASSERT_VALID( this );
			CSize szSize = 
				CExtToolControlBar::_CalcLayout(
					dwMode,
					nLength
					);
			szSize.cx -= 5;
			return szSize;
		}

		virtual void DoEraseBk( CDC * pDC )
		{
			ASSERT( pDC->GetSafeHdc() != NULL );

			CExtStatusControlBar * pWndParent = 
				DYNAMIC_DOWNCAST( CExtStatusControlBar, GetParent() );
			ASSERT( pWndParent != NULL );

			CRect rcPaint;
			pWndParent->GetWindowRect( &rcPaint );
			ScreenToClient( &rcPaint );

			CRect rcClient;
			GetClientRect( &rcClient );
			ClientToScreen( &rcClient );
			pWndParent->ScreenToClient( &rcClient );
			
			if( pWndParent->m_bCompleteRepaint )
			{
				pDC->OffsetViewportOrg( 
					rcClient.left, 
					0 
					);

				bool bRet = 
					PmBridge_GetPM()->StatusBar_EraseBackground(
						*pDC,
						rcPaint,
						pWndParent
						);

				pDC->OffsetViewportOrg( 
					-rcClient.left, 
					0 
					);

				if( !bRet )
				{
					bool bTransparent = false;
					if( PmBridge_GetPM()->GetCb2DbTransparentMode(pWndParent) )
					{
						if( PmBridge_GetPM()->PaintDockerBkgnd( true, *pDC, this ) )
							bTransparent = true;
					}
					if( ! bTransparent )
						pDC->FillSolidRect( 
							&rcPaint, 
							PmBridge_GetPM()->GetColor( CExtPaintManager::CLR_3DFACE_OUT, pWndParent ) 
							);
				}

				INT nIndex = pWndParent->CommandToIndex( IDS_PANE_TOOLBAR );
				if( nIndex >= 0 )
				{
					pDC->OffsetViewportOrg( 
						rcClient.left, 
						0 
						);
					PmBridge_GetPM()->StatusBar_ErasePaneBackground(
						*pDC,
						nIndex,
						rcPaint,
						pWndParent
						);
					pDC->OffsetViewportOrg( 
						-rcClient.left, 
						0 
						);
				}
			} // if( pWndParent->m_bCompleteRepaint )
		}		
		virtual void PostNcDestroy()
		{
			delete this;
		}
	}; // class CExtStatusBarToolbar

	class CExtStatusBarScrollBar : public CExtScrollBar
	{
	protected:
		virtual void PostNcDestroy()
		{
			delete this;
		}
	}; // class CExtStatusBarScrollBar	

	class CExtStatusBarZoomScrollBar : public CExtZoomScrollBar
	{
	protected:
		virtual void PostNcDestroy()
		{
			delete this;
		}
	}; // class CExtStatusBarZoomScrollBar	

	CExtStatusBarEdit			* m_pWndEdit;
	CExtStatusBarSliderCtrl		* m_pWndSliderCtrl;
	CExtStatusBarButton			* m_pWndButton;
	CExtStatusBarProgressCtrl	* m_pWndProgressCtrl;
	CExtStatusBarAnimateCtrl	* m_pWndAnimateCtrl;
	CExtStatusBarLabel			* m_pWndLabel;
	CExtStatusBarToolbar		* m_pWndToolBar;
	CExtStatusBarScrollBar		* m_pWndScrollBar;
	CExtStatusBarZoomScrollBar	* m_pWndZoomScrollBar;

	CExtStatusControlBar m_wndStatusBar;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageStatusBar)
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageStatusBar)
	afx_msg void OnDrawPaneSeparators();
	afx_msg void OnOuterRectInFirstBand();
	afx_msg void OnHideTextOnDisabledPanes();
	afx_msg void OnAddPane();
	afx_msg void OnRemovePane();
	afx_msg void OnDisablePane();
	afx_msg void OnStatusBarButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangePosition();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CheckControls();
	bool m_bInitComplete;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESTATUSBAR_H__9E21316E_7C7F_4E8E_B28F_7DC29EB448A3__INCLUDED_)
