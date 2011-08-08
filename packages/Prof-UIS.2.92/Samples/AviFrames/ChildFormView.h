#if !defined(AFX_CHILDFORMVIEW_H__5A1BD4EF_2576_4480_BC1A_54B9357C8DC8__INCLUDED_)
#define AFX_CHILDFORMVIEW_H__5A1BD4EF_2576_4480_BC1A_54B9357C8DC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildFormView.h : header file
//
#include "AviPlayer.h"

/////////////////////////////////////////////////////////////////////////////
// CAviView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CAviView : public CExtNSB < CExtScrollWnd >
{
public:
	CAviView();
	DECLARE_DYNAMIC( CAviView );

// Attributes
public:

// Operations
public:
	bool Create(
		CWnd * pWndParent,
		UINT nDlgCtrlID = AFX_IDW_PANE_FIRST,
		CRect rc = CRect( 0,0,0,0 ),
		DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
		DWORD dwStyleEx = 0L
		);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAviView)
	public:
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAviView();

	CExtLabel m_wndLabelCorner;
	void _AdjustCornerBetweenScrollBars();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual CSize OnSwGetTotalSize() const;
	virtual CSize OnSwGetPageSize( int nDirection ) const
	{
		nDirection;
		return CSize( 50, 50 );
	}
	virtual CSize OnSwGetLineSize( int nDirection ) const
	{
		nDirection;
		return CSize( 10, 10 );
	}
	
	virtual bool OnSwDoScrollBy(
		CSize sizeScroll,
		bool bDoScroll = true
	);

	virtual CRect OnSwRecalcLayout(
		bool bDoLayout,
		LPCRECT pRectClientSrc  = NULL
		);
	
		
	// Generated message map functions
	//{{AFX_MSG(CAviView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool m_bInitComplete;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFORMVIEW_H__5A1BD4EF_2576_4480_BC1A_54B9357C8DC8__INCLUDED_)
