// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F714E2DF_477E_45FB_99F4_0BF9B1494D80__INCLUDED_)
#define AFX_MAINFRM_H__F714E2DF_477E_45FB_99F4_0BF9B1494D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined(__EXT_TEMPL_H)
	#include <ExtTempl.h>
#endif

#include <Resources/Resource.h>

class CMyTaskAreaBar : public CExtControlBar
{
	class CTaskAreaWnd : public CExtWRB<CWnd>
	{
	public:
		INT m_nPageNo, m_nPageCount;
		CTaskAreaWnd()
			: m_nPageNo( 0 )
			, m_nPageCount( 5 )
		{
		}
		void SyncPage()
		{
			ASSERT( GetSafeHwnd() != NULL );
			CString sText;
			sText.Format(
				_T("TaskArea - Page %d"),
				m_nPageNo+1,
				m_nPageCount
				);
			CWnd * pBarWnd = GetParent();
			ASSERT( pBarWnd != NULL );
			ASSERT( pBarWnd->GetSafeHwnd() != NULL );
			pBarWnd->SetWindowText( sText );
			Invalidate();
		}
	private:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch( message )
			{
			case WM_CREATE:
				SyncPage();
			break;
			case WM_CONTEXTMENU:
				return 0;
			case WM_ERASEBKGND:
				return TRUE;
			case WM_PAINT:
				{
					BOOL bXpStyle = g_PaintManager->IsKindOf(RUNTIME_CLASS(CExtPaintManagerXP));
					CPaintDC dcPaint( this );
					CExtMemoryDC dc( &dcPaint );
					CRect rcPaint;
					GetClientRect( &rcPaint );
					dc.FillSolidRect(
						&rcPaint,
						g_PaintManager->GetColor(
							bXpStyle
								? CExtPaintManagerXP::XPCLR_3DFACE_NORMAL
								: COLOR_WINDOW
								,
							this
							)
						);
					rcPaint.DeflateRect( 4, 4 );
					if( rcPaint.right - rcPaint.left < 10
						|| rcPaint.bottom - rcPaint.top < 10
						)
						return 0;
					CFont * pOldFont = dc.SelectObject( &g_PaintManager->m_FontNormal );
					int nOldBkMode = dc.SetBkMode( TRANSPARENT );
					COLORREF clrOldText =
						dc.SetTextColor( g_PaintManager->GetColor( COLOR_BTNTEXT, this ) );
					CRgn rgnPaint;
					VERIFY( rgnPaint.CreateRectRgnIndirect(&rcPaint) );
					dc.SelectClipRgn( &rgnPaint );
					ASSERT( m_nPageNo >=0 && m_nPageNo < m_nPageCount );
					CString sText;
					sText.Format( _T("<< Page %d of %d >>"), m_nPageNo+1, m_nPageCount );
					CExtRichContentLayout::stat_DrawText( dc.m_hDC, LPCTSTR(sText), rcPaint, DT_SINGLELINE|DT_LEFT|DT_TOP|DT_END_ELLIPSIS, 0 );
					dc.SelectClipRgn( NULL );
					dc.SetTextColor(clrOldText);
					dc.SetBkMode( nOldBkMode );
					dc.SelectObject( pOldFont );
					return 0;
				}
			}
			return
				CExtWRB<CWnd>::WindowProc(message,wParam,lParam);
		}
	}; // class CTaskAreaWnd

	CTaskAreaWnd m_wndTaskArea;

	class CBarNcTaskAreaBtn : public CExtBarNcAreaButton
	{
	public:
		bool m_bSwitchToPrevPage:1;
		CTaskAreaWnd * m_pTaskAreaWnd;

		CBarNcTaskAreaBtn(
			CExtControlBar * pBar,
			CTaskAreaWnd * pTaskAreaWnd,
			bool bSwitchToPrevPage
			)
			: CExtBarNcAreaButton( pBar )
			, m_pTaskAreaWnd( pTaskAreaWnd )
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

			m_bDisabled = false;
			if( m_bSwitchToPrevPage )
			{
				if( m_pTaskAreaWnd->m_nPageNo == 0 )
					m_bDisabled = true;
			} // if( m_bSwitchToPrevPage )
			else
			{
				if( m_pTaskAreaWnd->m_nPageNo == m_pTaskAreaWnd->m_nPageCount-1 )
					m_bDisabled = true;
			} // else from if( m_bSwitchToPrevPage )

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

			ASSERT_VALID( m_pTaskAreaWnd );
			bool bRedrawTaskAreaWnd = false;
			if( m_bSwitchToPrevPage )
			{
				if( m_pTaskAreaWnd->m_nPageNo > 0 )
				{
					m_pTaskAreaWnd->m_nPageNo --;
					bRedrawTaskAreaWnd = true;
				}
			} // if( m_bSwitchToPrevPage )
			else
			{
				if( m_pTaskAreaWnd->m_nPageNo < m_pTaskAreaWnd->m_nPageCount-1 )
				{
					m_pTaskAreaWnd->m_nPageNo ++;
					bRedrawTaskAreaWnd = true;
				}
			} // else from if( m_bSwitchToPrevPage )
			if( bRedrawTaskAreaWnd )
				m_pTaskAreaWnd->SyncPage();
			
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
	}; // class CBarNcTaskAreaBtn

	virtual void OnNcAreaButtonsReinitialize()
	{
		INT nCountOfNcButtons = NcButtons_GetCount();
		if( nCountOfNcButtons > 0 )
			return;
		NcButtons_Add( new CExtBarNcAreaButtonClose(this) );
		NcButtons_Add( new CBarNcTaskAreaBtn(this,&m_wndTaskArea,false) );
		NcButtons_Add( new CBarNcTaskAreaBtn(this,&m_wndTaskArea,true) );
	}

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

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch( message )
		{
		case WM_CREATE:
			VERIFY(
				m_wndTaskArea.Create(
					::AfxRegisterWndClass(0),
					_T(""),
					WS_CHILD|WS_VISIBLE,
					CRect(0,0,0,0),
					this,
					(UINT)IDC_STATIC
					)
				);
		break;
		} // switch( message )
		return
			CExtControlBar::WindowProc(message,wParam,lParam);
	}

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

}; // class CMyTaskAreaBar

class CMainFrame : public CExtNCW < CMDIFrameWnd >
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

#ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
	
	class CMyMdiWindowsListDlg : public CExtMdiWindowsListDlg
	{
	public:
		CMyMdiWindowsListDlg(
				CMDIFrameWnd * pMDIFrameWnd,
				CWnd * pParent = NULL
				)
			: CExtMdiWindowsListDlg(
				pMDIFrameWnd,
				pParent
				)
		{
		}
		CMyMdiWindowsListDlg(
				HWND hMDIFrameWnd,
				CWnd * pParent = NULL
				)
			: CExtMdiWindowsListDlg(
				hMDIFrameWnd,
				pParent
				)
		{
		}
	protected:
		virtual void OnUpdateButtons()
		{
			CExtMdiWindowsListDlg::OnUpdateButtons();
			m_btnSave.ShowWindow(SW_HIDE);
		}
	}; // class CMyMdiWindowsListDlg : public CExtMdiWindowsListDlg
	
#endif // #ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	
	
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
	virtual void RecalcLayout(BOOL bNotify = TRUE);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
public: // for child frame
	CExtMenuControlBar    m_wndMenuBar;
protected:
	CExtToolControlBar    m_wndToolBar;
	CExtThemeSwitcherToolControlBar  m_wndToolBarUiLook;
	CExtControlBar    m_wndResizableBar0;
	CExtControlBar    m_wndResizableBar1;
	CExtControlBar    m_wndResizableBar2;
	CExtControlBar    m_wndResizableBar3;
	CExtControlBar    m_wndResizableBar4;
	CMyTaskAreaBar    m_wndResizableBarTA;

	CExtNCSB < CExtEditBase > m_wndInBarEdit;
	CExtNCSB < CListBox > m_wndInBarListBox;
	CExtWRB< CExtColorCtrl > m_wndInBarColorPicker;
	CExtWRB< CExtResizableDialog > m_wndInBarDlg;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F714E2DF_477E_45FB_99F4_0BF9B1494D80__INCLUDED_)
