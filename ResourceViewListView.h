//
// ResourcePicListView.h
//

#pragma once

#include "PicResource.h"
#include "ResourceListView.h"

class CResourceViewListView : public CResourceListView
{
protected: // create from serialization only
    CResourceViewListView();
    DECLARE_DYNCREATE(CResourceViewListView)

// Attributes
public:
    virtual DWORD GetDefaultViewMode() { return LVS_ICON; }

// Operations
public:

// Implementation
public:
    virtual ~CResourceViewListView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
    void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);

protected:
    virtual void _PrepareLVITEM(LVITEM *pItem);
    virtual void _OnInitListView(int cItems);

// Generated message map functions
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()

private:
    HIMAGELIST _himlPics;
    int _iCorruptBitmapIndex;
};

