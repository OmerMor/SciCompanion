// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
#define AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
				{
					::MoveWindow(
						hWndChild,
						0,
						0,
						LOWORD(lParam),
						HIWORD(lParam),
						FALSE
						);
					RedrawWindow(
						NULL, NULL,
						RDW_INVALIDATE|RDW_UPDATENOW
						|RDW_ERASE|RDW_ERASENOW
						|RDW_ALLCHILDREN
						);
				} // if( hWndChild != NULL )
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
		case WM_ERASEBKGND:
			return 1;
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
		case WM_PAINT:
		{
			HWND hWndChild = ::GetWindow( m_hWnd, GW_CHILD );
			if( hWndChild == NULL )
				break;
			if( !CExtPaintManager::stat_DefIsHwndNeedsDirectRepaint(hWndChild) )
				break;
			CRect rcClient;
			GetClientRect( &rcClient );
			CPaintDC dc( this );
			bool bNoFill = false;
			if( g_PaintManager->GetCb2DbTransparentMode(this) )
				bNoFill =
					g_PaintManager->PaintDockerBkgnd(
						true,
						dc,
						this
						);
			if( ! bNoFill )
				dc.FillSolidRect(
					&rcClient,
					g_PaintManager->GetColor(
						CExtPaintManager::CLR_3DFACE_OUT, this
						)
					);
			return 1;
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

class CSimpleDynamicEdit : public CExtNCSB < CExtEditBase >
{
public:
	CBrush m_brush;
	CSimpleDynamicEdit()
		: CExtNCSB < CExtEditBase > ( true , true )
	{
	}
	bool Create(
		HWND hWndParent,
		LPCTSTR strEditorText
		)
	{
		ASSERT_VALID( this );
		ASSERT( m_hWnd == NULL );
		ASSERT( hWndParent != NULL && ::IsWindow(hWndParent) );
		if(	! CWnd::CreateEx(
				0,
				_T("EDIT"),
				strEditorText,
				WS_CHILD|WS_VISIBLE|WS_VSCROLL
					|ES_LEFT|ES_WANTRETURN
					|ES_DISABLENOSCROLL
					|ES_MULTILINE|ES_AUTOVSCROLL
					|ES_NOHIDESEL|ES_READONLY
					,
				0,
				0,
				0,
				0,
				hWndParent,
				(HMENU)NULL,
				0L
				)
			)
		{
			ASSERT( FALSE );
			return false;
		}
		SetFont(
			CFont::FromHandle(
				(HFONT)::GetStockObject(DEFAULT_GUI_FONT)
				)
			);
		CSimpleThinFrame * pWndThinFrame = new CSimpleThinFrame;
		VERIFY( pWndThinFrame->CreateDynamicThinFrame( this ) );
		return true;
	}
	virtual void PostNcDestroy()
	{
		delete this;
	}
protected:
	virtual BOOL OnChildNotify(
		UINT message,
		WPARAM wParam,
		LPARAM lParam,
		LRESULT * pResult
		)
	{
		if(		message == WM_CTLCOLORMSGBOX
			||	message == WM_CTLCOLOREDIT
			||	message == WM_CTLCOLORLISTBOX
			||	message == WM_CTLCOLORBTN
			||	message == WM_CTLCOLORDLG
			||	message == WM_CTLCOLORSCROLLBAR
			||	message == WM_CTLCOLORSTATIC
			)
		{
			if( m_brush.GetSafeHandle() != NULL )
			{
				CDC * pDC = CDC::FromHandle( (HDC)wParam );
				pDC->SetBkMode( TRANSPARENT );
				(*pResult) = (LRESULT)m_brush.GetSafeHandle();
				return TRUE;
			} // if( m_brush.GetSafeHandle() != NULL )
		} // if( message == WM_CTLCOLOREDIT )
		return
			CWnd::OnChildNotify(
				message,
				wParam,
				lParam,
				pResult
				);
	}
}; // class CSimpleDynamicEdit

class CSimpleControlBar : public CExtDynamicControlBar
{
public:
	DECLARE_SERIAL( CSimpleControlBar );
	COLORREF m_clrEditorBackground;
	CSimpleControlBar()
	{
		m_clrEditorBackground =
			CExtBitmap::stat_HLStoRGB(
				double( ::rand() % 1000 ) / 1000.0,
				0.8,
				0.7
				);
	}
	virtual void OnSerializeDynamicProps( CArchive & ar )
	{
		CExtDynamicControlBar::OnSerializeDynamicProps( ar );
		if( ar.IsStoring() )
			ar << DWORD(m_clrEditorBackground);
		else
		{
			DWORD dwTmp;
			ar >> dwTmp; m_clrEditorBackground = COLORREF(dwTmp);
		}
	}
}; // class CSimpleControlBar

/////////////////////////////////////////////////////////////////////////////
// CChildView form view

class CChildView : public CExtLabel
{
public:
	CChildView();
	DECLARE_DYNCREATE( CChildView );

// Attributes
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CChildView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

class CMainFrame
	: public CExtNCW < CFrameWnd >
	, public CExtDynamicBarSite
{
public:
	CMainFrame();
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:
	CExtMenuControlBar		   m_wndMenuBar;
	CExtThemeSwitcherToolControlBar m_wndToolBarUiLook;
	CExtStatusControlBar	   m_wndStatusBar;
	CChildView				   m_wndView;
protected:
	WINDOWPLACEMENT            m_dataFrameWP;
	CExtDynamicControlBar *    m_pBarPersistent;
	CTypedPtrArray < CPtrArray, CExtDynamicControlBar * > m_arrAllDynamicBars;
	CExtControlBar             m_wndSimpleBar;

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

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnPersistenceLoadFromFile_Resizable();
	afx_msg void OnPersistenceSaveToFile_Resizable();
	afx_msg void OnPersistenceLoadFromFile_Fixedsized();
	afx_msg void OnPersistenceSaveToFile_Fixedsized();
	//}}AFX_MSG
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8AD8EDA0_FE43_4657_B159_597A63DD8A98__INCLUDED_)
