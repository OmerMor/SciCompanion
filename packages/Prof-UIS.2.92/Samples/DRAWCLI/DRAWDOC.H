// drawdoc.h : interface of the CDrawDoc class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "drawobj.h"
#include "summinfo.h"

class CDrawDoc;
class CDrawView;
class CDrawCntrItem;
class CDrawSrvrItem;

class CDrawDoc : public COleServerDoc
{
protected: // create from serialization only
	CDrawDoc();
	DECLARE_DYNCREATE(CDrawDoc)

// Attributes
public:
	CDrawObjList* GetObjects() { return &m_objects; }
	const CSize& GetSize() const { return m_size; }
	void ComputePageSize();
	int GetMapMode() const { return m_nMapMode; }
	COLORREF GetPaperColor() const { return m_paperColor; }
	CSummInfo *m_pSummInfo;

	CDrawSrvrItem* GetEmbeddedItem()
		{ return (CDrawSrvrItem*)COleServerDoc::GetEmbeddedItem(); }
protected:
	virtual COleServerItem* OnGetEmbeddedItem();

// Operations
public:
	CDrawObj* ObjectAt(const CPoint& point);
	void Draw(CDC* pDC, CDrawView* pView);
	void Add(CDrawObj* pObj);
	void Remove(CDrawObj* pObj);

// Implementation
public:
	virtual ~CDrawDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	CDrawObjList m_objects;
	CSize m_size;
	int m_nMapMode;
	COLORREF m_paperColor;

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawDoc)
	afx_msg void OnViewPaperColor();
	afx_msg void OnFileSummaryInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CDrawCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CDrawCntrItem)

// Constructors
public:
	CDrawCntrItem(CDrawDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CDrawDoc* GetDocument()
		{ return (CDrawDoc*)COleClientItem::GetDocument(); }
	CDrawView* GetActiveView()
		{ return (CDrawView*)COleClientItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawCntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual BOOL CanActivate();
	//}}AFX_VIRTUAL

// Implementation
public:
	~CDrawCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

class CDrawSrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(CDrawSrvrItem)

// Constructors
public:
	CDrawSrvrItem(CDrawDoc* pContainerDoc);

// Attributes
	CDrawDoc* GetDocument() const
		{ return (CDrawDoc*)COleServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CDrawSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};



/////////////////////////////////////////////////////////////////////////////
