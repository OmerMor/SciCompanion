// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
#define AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"

//class CMyControlBar : public CExtControlBar
//{
//protected:
//	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam ) 
//	{
//		switch( message )
//		{
//		case WM_MOUSEMOVE:
//		case WM_NCMOUSEMOVE:
//			if( IsBarWithGripper() )
//			{
//				if( ! OnQueryBarHoverProcessingEnabled() )
//					return 0;
//				CExtPopupMenuTipWnd * pATTW = OnAdvancedPopupMenuTipWndGet();
//				if( pATTW == NULL )
//					return 0;
//				CPoint ptScreen;
//				if( ! ::GetCursorPos(&ptScreen) )
//					return 0;
//				CExtBarNcAreaButton * pBtn = NULL;
//				NcButtons_HitTest( ptScreen, &pBtn );
//				if( pBtn != NULL )
//					break;
//				CRect rcArea = _RectGripGet();
//				rcArea.DeflateRect( 2, 0 );
//				NcButtons_CalcGripperTextLocation( rcArea );
//				if( rcArea.left >= rcArea.right )
//					return 0;
//				CWindowDC dcMeasure( this );
//				CExtSafeString sCaption;
//				OnGetBarCaptionText( __EBCT_SINGLE_CAPTION_DOCKED, sCaption );
//				INT nCaptionTextWidthPX = CExtPaintManager::stat_CalcTextDimension( dcMeasure, PmBridge_GetPM()->m_FontCaption, sCaption ).Width();
//				INT nAreaWidthPX = rcArea.Width();
//				if( nAreaWidthPX >= nCaptionTextWidthPX )
//					return 0;
//				CRect rcDefOffsetWnd;
//				GetWindowRect( &rcDefOffsetWnd );
//				rcArea.OffsetRect( rcDefOffsetWnd.TopLeft() );
//				OnAdvancedPopupMenuTipWndDisplay( *pATTW, rcArea, LPCTSTR( sCaption ) );
//				return 0;
//			}
//			break;
//		}
//		LRESULT lResult = CExtControlBar::WindowProc( message, wParam, lParam );
//		return lResult;
//	}
//};

class CMainFrame : public CExtNCW < CFrameWnd >
{
public:
	CMainFrame();
protected: 
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
	virtual void RecalcLayout(BOOL bNotify = TRUE);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
	CExtMenuControlBar    m_wndMenuBar;
	CExtToolControlBar    m_wndToolBar;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;
	CExtControlBar        m_wndResizableBar0;
	CExtControlBar        m_wndResizableBar1;
	CExtControlBar        m_wndResizableBar2;
	CExtControlBar        m_wndResizableBar3;
	CExtControlBar        m_wndResizableBar4;
	CChildView            m_wndView;

	CExtNCSB < CExtEdit >				m_wndInBarEdit;
	CExtNCSB < CExtWFF < CListBox > >	m_wndInBarListBox;
	CExtWRB < CExtColorCtrl >			m_wndInBarColorPicker;
	CExtWRB < CExtResizableDialog >		m_wndInBarDlg;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	//}}AFX_MSG
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
