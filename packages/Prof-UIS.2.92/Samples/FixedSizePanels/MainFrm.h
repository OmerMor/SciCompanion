// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
#define AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"

#if !defined(__EXT_TEMPL_H)
	#include <ExtTempl.h>
#endif

#include <Resources/Resource.h>

/////////////////////////////////////////////////////////////////////////////
// CDemoHtmlCtrl window

class CDemoHtmlCtrl : public CWnd
{
// Construction
public:
	CDemoHtmlCtrl();

	DECLARE_DYNAMIC( CDemoHtmlCtrl );

// Attributes
public:
	bool m_bReflectParentSizing:1;

protected:
	IWebBrowser2 * m_pBrowser;

// Operations
public:
	bool Create(
		CWnd * pWndParent,
		CRect rectInit = CRect(0,0,0,0)
		);
	void GoBack();
	void GoForward();
	void Refresh();
	void NavigateURL( LPCTSTR lpszURL );
	void NavigateResourceID(
		UINT nResourceID
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoHtmlCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemoHtmlCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDemoHtmlCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMyResizableInFixedModeBar

class CMyResizableInFixedModeBar : public CExtControlBar
{
public:

	virtual bool IsFixedMode() const
	{
		return false;
	}
	virtual bool IsFixedDockStyle() const
	{
		return true;
	}

protected:
	virtual bool _IsShowContentWhenDragging() const
	{
		return true;
	}
	virtual bool _IsShowContentWhenRowResizing() const
	{
		return true;
	}
	virtual bool _IsShowContentWhenRowRecalcing() const
	{
		return true;
	}
	virtual bool _CanDockToInnerCircles() const
	{
		return false;
	}
	virtual bool _CanDockToTabbedContainers(
		CExtControlBar * pDestBar
		) const
	{
		pDestBar;
		return false;
	}
	virtual bool _GetFullRowMode() const
	{
		return true;
	}

	class CNcBackForwardNavigationBtn : public CExtBarNcAreaButton
	{
	public:
		bool m_bSwitchToPrevPage:1;

		CNcBackForwardNavigationBtn(
			CExtControlBar * pBar,
			bool bSwitchToPrevPage
			)
			: CExtBarNcAreaButton( pBar )
			, m_bSwitchToPrevPage( bSwitchToPrevPage )
		{
		}

		virtual bool OnQueryVisibility() const
		{
			ASSERT_VALID( this );
			if( !CExtBarNcAreaButton::OnQueryVisibility() )
				return false;
			return true;
		}
		
		virtual void OnNcAreaDraw( CDC & dc )
		{
			ASSERT_VALID( this );
			ASSERT( dc.GetSafeHdc() != NULL );
			ASSERT( OnQueryVisibility() );
			if( m_rc.IsRectEmpty() )
				return;

			m_bDisabled = true;
			CWnd * pWnd = GetBar()->GetWindow(GW_CHILD);
			if( pWnd != NULL )
			{
				ASSERT_KINDOF( CDemoHtmlCtrl, pWnd );
				m_bDisabled = false;
			}

			NcDrawDefault(
				dc,
				m_bSwitchToPrevPage
					? CExtPaintManager::__DCBT_ARROW_LEFT_L
					: CExtPaintManager::__DCBT_ARROW_RIGHT_L
				);
		}
		
		virtual UINT OnNcAreaHitTest( CPoint point )
		{
			ASSERT_VALID( this );
			UINT nHT = CExtBarNcAreaButton::OnNcAreaHitTest( point );
			if( nHT == HTCLIENT )
				return HTCLIENT; // not inside nc-button
			return HTMAXBUTTON; // non HTCLIENT
		}
		
		virtual bool OnNcAreaClicked( CPoint point )
		{
			ASSERT_VALID( this );
			if( !m_rc.PtInRect(point) )
				return false; // continue asking nc-buttins

			CWnd * pWnd = GetBar()->GetWindow(GW_CHILD);
			if( pWnd != NULL )
			{
				ASSERT_VALID( pWnd );
				CDemoHtmlCtrl * pDemoHtmlCtrl =
					STATIC_DOWNCAST( CDemoHtmlCtrl, pWnd );
				if( m_bSwitchToPrevPage )
					pDemoHtmlCtrl->GoBack();
				else
					pDemoHtmlCtrl->GoForward();
			} // if( pWnd != NULL )

			
			return true;
		}
		
		__EXT_MFC_INT_PTR OnToolHitTest(
			CPoint point,
			TOOLINFO * pTI
			)
		{
			ASSERT_VALID( this );
			
			point; // shuld be in button area
			return
				DoDefaultReflectForToolHitTest(
					pTI,
					m_bSwitchToPrevPage ? IDS_CBNCAREA_PREV : IDS_CBNCAREA_NEXT,
					m_bSwitchToPrevPage ? _T("Previous page") : _T("Next page")
					);
		}

		void OnNcAreaReposition(
			CExtBarNcAreaButton * pPrevBtn
			)
		{
			pPrevBtn;
			ASSERT_VALID( this );
			bool bGripperAtTop = false;
			if(	!GetBarNcAreaInfo( &m_rc, NULL, &bGripperAtTop ) )
				return;
			const int _nGapToBordersH = 2;
			const int _nGapToBordersV = 3;
			const int _nGapBetweenButtons = 3;
			ASSERT( bGripperAtTop );
			m_rc.DeflateRect(
				_nGapToBordersH,
				_nGapToBordersV
				);
			m_rc.right = m_rc.left + m_rc.Height();
			m_rc.OffsetRect( 1, 0 );
			if( !m_bSwitchToPrevPage )
				m_rc.OffsetRect(
					m_rc.Width() + _nGapBetweenButtons,
					0
					);
		}
	}; // class CNcBackForwardNavigationBtn

	virtual void OnNcAreaButtonsReinitialize()
	{
		INT nCountOfNcButtons = NcButtons_GetCount();
		if( nCountOfNcButtons > 0 )
			return;
		NcButtons_Add( new CExtBarNcAreaButtonClose(this) );
		NcButtons_Add( new CNcBackForwardNavigationBtn(this,false) );
		NcButtons_Add( new CNcBackForwardNavigationBtn(this,true) );
	};

	virtual CRect & NcButtons_CalcGripperTextLocation(
		CRect & rcPreCalcText
		)
	{
		INT nCountOfNcButtons = NcButtons_GetCount();
		if( nCountOfNcButtons == 0 )
			return CExtControlBar::NcButtons_CalcGripperTextLocation(rcPreCalcText);
		ASSERT( nCountOfNcButtons == 3 );
		if( (g_ResourceManager->OnQueryLangLayout()&LAYOUT_RTL) != 0 )
		{
			CExtBarNcAreaButton * pBtnExamine = NcButtons_GetAt( 0 );
			CRect rc = *pBtnExamine;
			if( rcPreCalcText.left < rc.right )
				rcPreCalcText.left = rc.right;
			pBtnExamine = NcButtons_GetAt( 1 );
			rc = *pBtnExamine;
			if( rcPreCalcText.right > rc.left )
				rcPreCalcText.right = rc.left;
		}
		else
		{
			CExtBarNcAreaButton * pBtnExamine = NcButtons_GetAt( 0 );
			CRect rc = *pBtnExamine;
			if( rcPreCalcText.right > rc.left )
				rcPreCalcText.right = rc.left;
			pBtnExamine = NcButtons_GetAt( 1 );
			rc = *pBtnExamine;
			if( rcPreCalcText.left < rc.right )
				rcPreCalcText.left = rc.right;
		}
		return rcPreCalcText;
	}
}; // class CMyResizableInFixedModeBar

/////////////////////////////////////////////////////////////////////////////
// CPanelEmpty

class CPanelEmpty : public CExtPanelControlBar
{
public:
	CPanelEmpty();

	DECLARE_DYNAMIC(CPanelEmpty)

// Attributes
public:


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelEmpty)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPanelEmpty();

protected:
	virtual CSize OnQueryFixedBarNewSize(
		CExtPanelControlBar::e_fixed_bar_size_query_t eWhichSizeNeedToKnow
		) const;

// Generated message map functions
protected:
	//{{AFX_MSG(CPanelEmpty)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CPanelEmpty

/////////////////////////////////////////////////////////////////////////////
// CPanelCustomDrawn

class CPanelCustomDrawn : public CPanelEmpty
{
public:
	CPanelCustomDrawn();

	DECLARE_DYNAMIC(CPanelCustomDrawn)

// Attributes
public:


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelCustomDrawn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPanelCustomDrawn();

// Generated message map functions
protected:
	//{{AFX_MSG(CPanelCustomDrawn)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CPanelCustomDrawn


// 						class CExtFullRowPanelControlBar : public CExtPanelControlBar
// 						{
// 						public:
// 							CSize OnQueryFixedBarNewSize( CExtPanelControlBar::e_fixed_bar_size_query_t eWhichSizeNeedToKnow ) const
// 							{
// 								ASSERT_VALID( this );
// 								CSize sizeResult = CExtPanelControlBar::OnQueryFixedBarNewSize( eWhichSizeNeedToKnow );
// 								if( eWhichSizeNeedToKnow == __FSQ_DOCKED_H && m_pDockSite->GetSafeHwnd() != NULL )
// 								{
// 									CRect rcDockSiteClient, rcBarWindow, rcBarClient;
// 									m_pDockSite->GetClientRect( &rcDockSiteClient );
// 									GetWindowRect( &rcBarWindow );
// 									GetClientRect( &rcBarClient );
// 									sizeResult.cx = rcDockSiteClient.Width() - ( rcBarWindow.Width() - rcBarClient.Width() );
// 								}
// 								return sizeResult;
// 							}
// 							virtual void _RepositionChild( bool bRedraw = true )
// 							{
// 								ASSERT_VALID( this );
// 								CWnd * pWndParent = GetParent(), * pWndChild = GetWindow( GW_CHILD );
// 								if( pWndChild->GetSafeHwnd() != NULL && pWndParent->GetSafeHwnd() != NULL && pWndParent->IsKindOf( RUNTIME_CLASS(CControlBar) ) )
// 								{
// 									UINT nParentDlgCtrlID = UINT( pWndParent->GetDlgCtrlID() );
// 									if( nParentDlgCtrlID == AFX_IDW_DOCKBAR_TOP || nParentDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM )
// 									{
// 										CRect rcChildWindow, rcBarClient;
// 										pWndChild->GetWindowRect( &rcChildWindow );
// 										ScreenToClient( &rcChildWindow );
// 										GetClientRect( &rcBarClient );
// 										rcBarClient.DeflateRect( 6, 6 );
// 										if( rcBarClient != rcChildWindow )
// 											pWndChild->MoveWindow( &rcBarClient, bRedraw ? TRUE : FALSE );
// 										return;
// 									}
// 								}
// 							}
// 						}; /// class CExtFullRowPanelControlBar



/////////////////////////////////////////////////////////////////////////////
// CMainFrame

class CMainFrame : public CExtNCW < CFrameWnd >
{
public:

	class CColorStatic : public CStatic
	{
	public:
		COLORREF m_clrIn, m_clrOut;
		CColorStatic();
		LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	}; // class CColorStatic
	
	HWND m_hWndSelectedToolButton;

	class CDynamicButton : public CExtButton
	{
	public:
		virtual void PostNcDestroy()
		{
			delete this;
		}
		LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			if( message == WM_SETFOCUS )
			{
				CFrameWnd * pFrame = GetParentFrame();
				ASSERT_VALID( pFrame );
				if( pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) )
				{
					pFrame = pFrame->GetParentFrame();
					ASSERT_VALID( pFrame );
				}
				pFrame->SetFocus();
				return 0L;
			}
			return CExtButton::WindowProc(message,wParam,lParam);
		}
		virtual void _OnClick(
			bool bSelectAny,
			bool bSeparatedDropDownClicked
			);
	}; // class CDynamicButton

	class CDynamicPaletteButton : public CDynamicButton
	{
	public:
		COLORREF m_clr;
		CDynamicPaletteButton( COLORREF clr );
		virtual void _OnClick(
			bool bSelectAny,
			bool bSeparatedDropDownClicked
			);
		virtual void _RenderImpl( // non-buffered paint
			CDC & dc,
			bool bTransparent = false,
			bool bCombinedContent = false
			);
		LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	}; // class CDynamicPaletteButton
	
public:
	CMainFrame();
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

private:
	// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
	CExtMenuControlBar    m_wndMenuBar;
	CExtToolControlBar    m_wndToolBar;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;
	CPanelEmpty m_wndPanelEmpty;
	CPanelCustomDrawn m_wndPanelCustomDrawn;
	CExtPanelControlBar
		m_wndPanelEdit, m_wndPanelDialog,
		m_wndPanelTools, m_wndPanelPalette;
	CEdit m_wndEdit;
	CExtResizableDialog
		m_dlgForPanelDialog, m_dlgForPanelTools,
		m_dlgForPanelPalette;
	CExtLabel m_wndToolsPanelLabel, m_wndDialogPanelLabel;
	CExtCheckBox m_wndDialogPanelCheck1, m_wndDialogPanelCheck2;
	CChildView    m_wndView;
	CColorStatic m_wndColorStatic;
	CMyResizableInFixedModeBar m_wndResizableBar1,m_wndResizableBar2;
	CDemoHtmlCtrl m_wndHtmlCtrl1, m_wndHtmlCtrl2;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	//}}AFX_MSG
	afx_msg void OnClickedToolButton( UINT nID );
	afx_msg void OnClickedPaletteButtonL( UINT nID );
	afx_msg void OnClickedPaletteButtonR( UINT nID );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
