#pragma once


// ResourceListDoc document

// FWD decl
class ResourceBlob;

#include "ResourceMap.h"
#include "NonViewHostDocument.h"

// A resource was added or deleted from the resource map.  The Object parameter is a ResourceBlob
#define VIEWUPDATEHINT_RESOURCEMAPCHANGED   0x00000040
// A particular resource was added.  The Object parameter is a CResourceDataAndIndex
#define VIEWUPDATEHINT_RESOURCEADDED        0x00000100
// A particular resource was deleted.  The Object parameter is a CResourceDataAndIndex
#define VIEWUPDATEHINT_RESOURCEDELETED      0x00000200
// A particular resource type was changed (outside our control), tell that type to reload. The Object is a CResourceTypeWrap
#define VIEWUPDATEHINT_RESOURCETYPECHANGED  0x00000400
// Show a resource type. The Object is a CResourceTypeWrap
#define VIEWUPDATEHINT_SHOWRESOURCETYPE     0x00001000

//
// Manages a bunch of ResourceBlob's.
//
class CResourceListDoc : public CNonViewHostDocument, public ISyncResourceMap
{
	DECLARE_DYNCREATE(CResourceListDoc)

public:
	CResourceListDoc();
	virtual ~CResourceListDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o

    // ISyncResourceMap
    HRESULT OnResourceAdded(const ResourceBlob *pData);
    HRESULT OnResourceDeleted(const ResourceBlob *pData);
    HRESULT OnResourceMapReloaded();
    HRESULT OnResourceTypeReloaded(int iType);

    void ShowResourceType(ResourceType iType);    
    ResourceType GetShownResourceType() { return _shownResourceType; }

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    void OnDisableCommand(CCmdUI *pCmdUI);

	virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void OnCloseDocument();

	DECLARE_MESSAGE_MAP()

private:
    ResourceType _shownResourceType;
};
