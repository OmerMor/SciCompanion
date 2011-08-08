// MDI_DynamicBarsView.h : interface of the CMDI_DynamicBarsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDI_DynamicBarsVIEW_H__624EA67D_29E4_4F42_8F28_E649785CDFDC__INCLUDED_)
#define AFX_MDI_DynamicBarsVIEW_H__624EA67D_29E4_4F42_8F28_E649785CDFDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMDI_DynamicBarsView : public CView
{
protected: // create from serialization only
	CMDI_DynamicBarsView();
	DECLARE_DYNCREATE(CMDI_DynamicBarsView)

// Attributes
public:
	CMDI_DynamicBarsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDI_DynamicBarsView)
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
	virtual ~CMDI_DynamicBarsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMDI_DynamicBarsView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MDI_DynamicBarsView.cpp
inline CMDI_DynamicBarsDoc* CMDI_DynamicBarsView::GetDocument()
   { return (CMDI_DynamicBarsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDI_DynamicBarsVIEW_H__624EA67D_29E4_4F42_8F28_E649785CDFDC__INCLUDED_)
