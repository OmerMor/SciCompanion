// SDIDOCVIEWDoc.h : interface of the CSDIDOCVIEWDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIDOCVIEWDOC_H__36D65747_EC58_4FF1_B4CE_614A47B78C80__INCLUDED_)
#define AFX_SDIDOCVIEWDOC_H__36D65747_EC58_4FF1_B4CE_614A47B78C80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSDIDOCVIEWDoc : public CDocument
{
protected: // create from serialization only
	CSDIDOCVIEWDoc();
	DECLARE_DYNCREATE(CSDIDOCVIEWDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSDIDOCVIEWDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSDIDOCVIEWDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSDIDOCVIEWDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDIDOCVIEWDOC_H__36D65747_EC58_4FF1_B4CE_614A47B78C80__INCLUDED_)
