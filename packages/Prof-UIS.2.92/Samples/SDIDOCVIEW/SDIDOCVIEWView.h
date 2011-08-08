// SDIDOCVIEWView.h : interface of the CSDIDOCVIEWView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIDOCVIEWVIEW_H__C63AC053_87B3_44B1_8A6E_3E15CA9329DE__INCLUDED_)
#define AFX_SDIDOCVIEWVIEW_H__C63AC053_87B3_44B1_8A6E_3E15CA9329DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSDIDOCVIEWView : public CView
{
protected: // create from serialization only
	CSDIDOCVIEWView();
	DECLARE_DYNCREATE(CSDIDOCVIEWView)

// Attributes
public:
	CSDIDOCVIEWDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDIDOCVIEWView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSDIDOCVIEWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSDIDOCVIEWView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SDIDOCVIEWView.cpp
inline CSDIDOCVIEWDoc* CSDIDOCVIEWView::GetDocument()
   { return (CSDIDOCVIEWDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDIDOCVIEWVIEW_H__C63AC053_87B3_44B1_8A6E_3E15CA9329DE__INCLUDED_)
