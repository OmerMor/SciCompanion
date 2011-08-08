#if !defined(AFX_AVIPLAYERWND_H__8F4BEFD7_66AD_45A5_A392_408FB18F7A05__INCLUDED_)
#define AFX_AVIPLAYERWND_H__8F4BEFD7_66AD_45A5_A392_408FB18F7A05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AviPlayerWnd.h : header file
//

class CAviPlayer;

/////////////////////////////////////////////////////////////////////////////
// CAviPlayerWnd window

class CAviPlayerWnd : public CWnd
{
// Construction
public:
	CAviPlayerWnd( CAviPlayer *pAviPlayer = NULL);
	DECLARE_DYNAMIC( CAviPlayerWnd );
	
// Attributes
public:

// Operations
public:
	bool Create(
		CWnd * pWndParent,
		UINT nDlgCtrlID = 0,
		CRect rc = CRect( 0,0,0,0 ),
		DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		DWORD dwStyleEx = 0L
		);

	DWORD m_nTimerStart;
	
	UINT SetTimer (UINT nIDEvent, UINT nElapse)
	{
		m_nTimerStart = GetTickCount();
		return UINT( CWnd::SetTimer( nIDEvent, nElapse, NULL ) );
	}
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAviPlayerWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAviPlayerWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAviPlayerWnd)
	afx_msg void OnTimer(__EXT_MFC_UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CAviPlayer* m_pAviPlayer;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVIPLAYERWND_H__8F4BEFD7_66AD_45A5_A392_408FB18F7A05__INCLUDED_)
