// MDIDOCVIEWView.h : interface of the CMDIDOCVIEWView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDIDOCVIEWVIEW_H__624EA67D_29E4_4F42_8F28_E649785CDFDC__INCLUDED_)
#define AFX_MDIDOCVIEWVIEW_H__624EA67D_29E4_4F42_8F28_E649785CDFDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMDIDOCVIEWView : public CView
{
protected: // create from serialization only
	CMDIDOCVIEWView();
	DECLARE_DYNCREATE(CMDIDOCVIEWView)

// Attributes
public:
	CMDIDOCVIEWDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIDOCVIEWView)
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
	virtual ~CMDIDOCVIEWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMDIDOCVIEWView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MDIDOCVIEWView.cpp
inline CMDIDOCVIEWDoc* CMDIDOCVIEWView::GetDocument()
   { return (CMDIDOCVIEWDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIDOCVIEWVIEW_H__624EA67D_29E4_4F42_8F28_E649785CDFDC__INCLUDED_)
