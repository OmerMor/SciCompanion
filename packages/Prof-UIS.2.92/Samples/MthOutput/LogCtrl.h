// LogCtrl.h : interface of the CLogCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__7627BE0E_E398_44DE_B281_3696A9A0C8BA__INCLUDED_)
#define AFX_CHILDVIEW_H__7627BE0E_E398_44DE_B281_3696A9A0C8BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLogCtrl window

#define ID_TIMER_DELAYED_UPDATE 1
#define ELAPSE_TIMER_DELAYED_UPDATE 50

#define ID_TIMER_DELAYED_DESTROY 2

#define ID_TIMER_EVENT_SOURCE 3
#define ELAPSE_TIMER_EVENT_SOURCE 50

#define WM_USR_WRITE_LOG_TEXT (WM_USER + 0x1234)

class CLogCtrl : public CRichEditCtrl
{
	DWORD m_dwHelperThreadID;
	
	struct _MthMessageDataFor_WM_USR_WRITE_LOG_TEXT
	{
		bool m_bBold:1;
		COLORREF m_clrText;
		LPCTSTR m_sText;
		_MthMessageDataFor_WM_USR_WRITE_LOG_TEXT(
			bool bBold,
			COLORREF clrText,
			LPCTSTR sText
			)
			: m_bBold( bBold )
			, m_clrText( clrText )
			, m_sText( sText )
		{
		}
		operator LPARAM () const
		{
			return reinterpret_cast < LPARAM > ( this );
		}
		static _MthMessageDataFor_WM_USR_WRITE_LOG_TEXT &
			FromLPARAM(
				LPARAM lParam
				)
		{
			_MthMessageDataFor_WM_USR_WRITE_LOG_TEXT * pData =
				reinterpret_cast < _MthMessageDataFor_WM_USR_WRITE_LOG_TEXT * > ( lParam );
			ASSERT( pData != NULL );
			return * pData;
		}
	}; // _MthMessageDataFor_WM_USR_WRITE_LOG_TEXT

// Construction
public:
	CLogCtrl();

// Attributes
public:
	LONG m_nLineCountMax;

// Operations
public:
	BOOL Create(
		CWnd * pWndParent,
		UINT nID = UINT(IDC_STATIC),
		CRect rect = CRect(0,0,0,0),
		DWORD dwWindowStyles =
			WS_CHILD|WS_VISIBLE
				|WS_HSCROLL|WS_VSCROLL
				|ES_AUTOHSCROLL|ES_AUTOVSCROLL
				|ES_LEFT|ES_MULTILINE|ES_NOHIDESEL
				|ES_READONLY
		)
	{
		m_dwHelperThreadID = ::GetCurrentThreadId();
		return
			CRichEditCtrl::Create(
				dwWindowStyles,
				rect,
				pWndParent,
				nID
				);
	}

	void WriteText(
		bool bBold,
		COLORREF clrText,
		LPCTSTR sText
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogCtrl)
	public:
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogCtrl();

protected:
	void _InitRichEditProps();
	bool _CanUpdate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT On_WM_USR_WRITE_LOG_TEXT( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDemoEventSourceWnd window

class CDemoEventSourceWnd : public CWnd
{
// Construction
public:
	CLogCtrl * m_pWndLogCtrl;

	CDemoEventSourceWnd();

// Attributes
public:

private:
	int m_nDemoOutputLineIndex;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoEventSourceWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemoEventSourceWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDemoEventSourceWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDemoEventSourceThread

class CDemoEventSourceThread : public CWinThread
{
	CWnd * m_pWndParent;
public:
	CDemoEventSourceWnd m_wndDemoEventSource;
	CDemoEventSourceThread()
		: m_pWndParent( NULL )
	{
		m_bAutoDelete = FALSE;
	}
	
	CDemoEventSourceThread(
		CWnd * pWndParent
		)
		: m_pWndParent( pWndParent )
	{
		m_bAutoDelete = FALSE;
		ASSERT_VALID( m_pWndParent );
		ASSERT( m_pWndParent->GetSafeHwnd() != NULL );
		ASSERT( m_wndDemoEventSource.GetSafeHwnd() == NULL );
	}

	BOOL CreateThread(
		CWnd * pWndParent
		)
	{
		m_pWndParent = pWndParent;
		ASSERT_VALID( m_pWndParent );
		ASSERT( m_pWndParent->GetSafeHwnd() != NULL );
		ASSERT( m_wndDemoEventSource.GetSafeHwnd() == NULL );
		return CWinThread::CreateThread();
	}

	virtual BOOL InitInstance()
	{
		CRect rect;
		m_pWndParent->GetClientRect( &rect );
		if( !m_wndDemoEventSource.Create(
				::AfxRegisterWndClass(0),
				_T("Demo Event Source Windop"),
				WS_CHILD,
				CRect( 0, 0, 0, 0 ),
				m_pWndParent,
				UINT(IDC_STATIC)
				)
			)
		{
			ASSERT( FALSE );
			TRACE0("Failed to create m_wndDemoEventSource\n");
			return FALSE;
		}
		m_pMainWnd = &m_wndDemoEventSource;
		return TRUE;
	}

}; // class CDemoEventSourceThread

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__7627BE0E_E398_44DE_B281_3696A9A0C8BA__INCLUDED_)
