// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
#define AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"

#if (!defined __EXT_TEMPL_H)
	#include <ExtTempl.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimpleSplashWnd window

class CSimpleSplashWnd : public CWnd
{
protected:
	CWnd m_wndInvisibleParent;
	CBitmap m_bitmap;
	CSize m_sizeBitmap;
	CString m_sStatusText;

// Construction
public:
	CSimpleSplashWnd();
	CSimpleSplashWnd(
		CWnd * pWndParent,
		UINT nBitmapID
		);

// Attributes
public:

// Operations
public:
	bool Create(
		CWnd * pWndParent,
		UINT nBitmapID
		);

	static bool RegisterSplashWndClass();

	void SetStatusText(
		LPCTSTR sStatusText
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleSplashWnd)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSimpleSplashWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSimpleSplashWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CSimpleSplashWnd

#define GL_VIEWS_SPLASHWND_WNDCLASS _T("GLViewsSplashWnd")

/////////////////////////////////////////////////////////////////////////////
// CSimpleThinFrame window

class CSimpleThinFrame : public CWnd
{
	bool m_bAutoDestroyOnPostNcDestroy;
public:
	CSimpleThinFrame()
		: m_bAutoDestroyOnPostNcDestroy( false )
	{
	}

	BOOL Create(
		CWnd * pParentWnd,
		UINT nID
		)
	{
		return
			CWnd::Create(
				AfxRegisterWndClass(0), NULL,
				WS_CHILD|WS_VISIBLE,
				CRect( 0,0,0,0 ),
				pParentWnd,
				nID,
				NULL
				);
	}

	BOOL CreateDynamicThinFrame(
		CWnd * pChildWnd
		)
	{
		ASSERT( !m_bAutoDestroyOnPostNcDestroy );
		ASSERT_VALID( pChildWnd );
		CWnd * pParentWnd = pChildWnd->GetParent();
		ASSERT_VALID( pParentWnd );
		UINT nID = pChildWnd->GetDlgCtrlID();
		if( ! Create( pParentWnd, nID ) )
		{
			ASSERT( FALSE );
			return FALSE;
		}
		pChildWnd->SetParent( this );
		m_bAutoDestroyOnPostNcDestroy = true;
		return TRUE;
	}
	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch( message )
		{
		case WM_SETFOCUS:
			{
				HWND hWndChild = ::GetWindow( GetSafeHwnd(), GW_CHILD );
				if( hWndChild != NULL )
				{
					::SetFocus( hWndChild );
					return 0;
				}
			}
		break;
		case WM_SIZE:
			if( wParam != SIZE_MINIMIZED )
			{
				HWND hWndChild = ::GetWindow( m_hWnd, GW_CHILD );
				if( hWndChild != NULL )
					::MoveWindow(
						hWndChild,
						0,
						0,
						LOWORD(lParam),
						HIWORD(lParam),
						TRUE
						);
			}
			return 0;
		case WM_NCCALCSIZE:
		{
			NCCALCSIZE_PARAMS * pNCCSP =
				reinterpret_cast < NCCALCSIZE_PARAMS * > ( lParam );
			ASSERT( pNCCSP != NULL );
			CRect rcWnd( pNCCSP->rgrc[0] );
			rcWnd.DeflateRect( 2, 2 );
			::CopyRect( &(pNCCSP->rgrc[0]), rcWnd );
			return 0;
		}
		case WM_NCPAINT:
		{
			CRect rcChildWnd, rcClient;
			GetWindowRect( &rcChildWnd );
			GetClientRect( &rcClient );
			ClientToScreen( &rcClient );
			if( rcChildWnd == rcClient )
				return 0;
			CPoint ptDevOffset = -rcChildWnd.TopLeft();
			rcChildWnd.OffsetRect( ptDevOffset );
			rcClient.OffsetRect( ptDevOffset );

			CWindowDC dc( this );
			ASSERT( dc.GetSafeHdc() != NULL );
			dc.ExcludeClipRect( &rcClient );

			dc.FillSolidRect(
				&rcChildWnd,
				g_PaintManager->GetColor( COLOR_3DFACE, this )
				);

			COLORREF clrThinFrame = g_PaintManager->GetColor( COLOR_3DSHADOW, this );
			dc.Draw3dRect(
				&rcChildWnd,
				clrThinFrame,
				clrThinFrame
				);

			return 0;
		}
		} // switch( message )

		return CWnd::WindowProc(message,wParam,lParam);
	}

	virtual void PostNcDestroy()
	{
		CWnd::PostNcDestroy();
		if( m_bAutoDestroyOnPostNcDestroy )
			delete this;
	}

}; // class CSimpleThinFrame

/////////////////////////////////////////////////////////////////////////////
// CSimpleHtmlCtrl window

class CSimpleHtmlCtrl : public CWnd
{
// Construction
public:
	CSimpleHtmlCtrl();

// Attributes
public:

protected:
	IWebBrowser2 * m_pBrowser;

// Operations
public:
	bool Create( CWnd * pWndParent );
	void NavigateURL( LPCTSTR lpszURL );
	void NavigateResourceID(
		UINT nResourceID
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleHtmlCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSimpleHtmlCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSimpleHtmlCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame window

class CMainFrame : public CExtNCW < CFrameWnd >
{
	void _ResetCameras( bool bLockViews );
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
	void SerializeCameraState(
		LPCTSTR sProfileName,
		LPCTSTR sSectionNameCompany,
		LPCTSTR sSectionNameProduct,
		bool bSave
		);
	void SerializeCameraState(
		CArchive & ar
		);

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

	void SyncCameraFovValue( int nCamIdx, int nFovIdx );
	void SyncCameraFovValue();

protected:  // control bar embedded members
	CExtStatusControlBar  m_wndStatusBar;
	CExtMenuControlBar    m_wndMenuBar;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;
	CExtControlBar    m_wndResizableBar0;
	CExtControlBar    m_wndResizableBar1;
	CExtControlBar    m_wndResizableBar2;
	CExtControlBar    m_wndResizableBar3;
	CChildView    m_wndViewCam0, m_wndViewCam1, m_wndViewCamMain;
	CObjectHierarchyTreeCtrl m_wndObjectHierarchyTree;
	CImageList m_ilObjectHierarchyTree;
	CSimpleHtmlCtrl m_wndHelpHtmlCtrl;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnAnimate();
	afx_msg void OnUpdateAnimate(CCmdUI* pCmdUI);
	afx_msg void OnShowCameras();
	afx_msg void OnUpdateShowCameras(CCmdUI* pCmdUI);
	afx_msg void OnShowMirror();
	afx_msg void OnUpdateShowMirror(CCmdUI* pCmdUI);
	afx_msg void OnShowAviPlayer();
	afx_msg void OnUpdateShowAviPlayer(CCmdUI* pCmdUI);
	afx_msg void OnResetCameras();
	afx_msg void OnShowOuterObjects();
	afx_msg void OnUpdateShowOuterObjects(CCmdUI* pCmdUI);
	afx_msg void OnShowTextObjects();
	afx_msg void OnUpdateShowTextObjects(CCmdUI* pCmdUI);
	afx_msg void OnShowCubeObjects();
	afx_msg void OnUpdateShowCubeObjects(CCmdUI* pCmdUI);
	afx_msg void OnShowPlanetObjects();
	afx_msg void OnUpdateShowPlanetObjects(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
