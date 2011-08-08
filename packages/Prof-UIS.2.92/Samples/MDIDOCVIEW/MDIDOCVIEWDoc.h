// MDIDOCVIEWDoc.h : interface of the CMDIDOCVIEWDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDIDOCVIEWDOC_H__2369603C_6AE0_40A1_8F6B_35DE1B5D24A5__INCLUDED_)
#define AFX_MDIDOCVIEWDOC_H__2369603C_6AE0_40A1_8F6B_35DE1B5D24A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMDIDOCVIEWDoc : public CDocument
{
protected: // create from serialization only
	CMDIDOCVIEWDoc();
	DECLARE_DYNCREATE(CMDIDOCVIEWDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIDOCVIEWDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMDIDOCVIEWDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMDIDOCVIEWDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIDOCVIEWDOC_H__2369603C_6AE0_40A1_8F6B_35DE1B5D24A5__INCLUDED_)
