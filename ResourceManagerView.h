// ResourceListView.h : interface of the ResourceManagerView class
//


#pragma once

#include "ResourceListView.h"

// FWD decl
class CResourceListDoc;

class CResourceManagerView : public CView
{
protected: // create from serialization only
    CResourceManagerView();
    DECLARE_DYNCREATE(CResourceManagerView)

// Attributes
public:
    CResourceListDoc* GetDocument() const;

// Operations
public:

// Overrides
    public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
    virtual ~CResourceManagerView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    virtual void OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint);
    virtual void OnInitialUpdate();

    DROPEFFECT OnDragEnter(COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
    DROPEFFECT OnDragOver(COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
    void OnDragLeave();
    BOOL OnDrop(COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint point);

protected:
// Generated message map functions
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSelChange(NMHDR *pnmhdr, LRESULT *plres);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    void OnViewDetails();
    void OnViewThumbnails();
    void OnUpdateViewCommands(CCmdUI *pCmdUI);
    afx_msg void OnDelete();
    afx_msg void OnRename();
    afx_msg void OnExtractResources();
    afx_msg void OnExtractAsBitmap();
    afx_msg void OnUpdateDelete(CCmdUI *pCmdUI);
    afx_msg void OnUpdateItemCount(CCmdUI *pCmdUI);
    void OnDraw(CDC *pDC);
    DECLARE_MESSAGE_MAP()

protected:
    void _SyncSize();
    void _OnSelChange();
    void _OnSelChange(ResourceType iSelectedTab);
    CResourceListCtrl *_GetSelectedView();

    // Array of list controls (not list views)
    std::vector<CResourceListCtrl*> _lists;
    BOOL _bFirstSizing;

    COleDropTarget _dropTarget;
    CArray<CString, CString&> _dropFiles;

    ResourceType _iSelectedTab;

    CFont m_font;
};

#ifndef _DEBUG  // debug version in ResourceManagerView.cpp
inline CResourceListDoc* CResourceManagerView::GetDocument() const
   { return reinterpret_cast<CResourceListDoc*>(m_pDocument); }
#endif

