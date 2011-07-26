#pragma once

#include "ResourceBase.h"
#include "NonViewHostDocument.h"

//
// CResourceDocument document
// 
// Documents that represent a particular SCI resource should inherit from this.
// It provides saving/export functionality that is common across resource types.
//
class CResourceDocument : public CNonViewHostDocument, public IResourceIdentifier
{
	DECLARE_DYNAMIC(CResourceDocument)

public:
    CResourceDocument() { _id = -1; _fMostRecent = true; }
    BOOL CanCloseFrame(CFrameWnd* pFrameArg);
	virtual BOOL SaveModified(); // return TRUE if ok to continue
    void OnFileSave();
    void OnFileSaveAs();
    virtual BOOL QueryCapability(const GUID guid, void **ppv) { return FALSE; }
    // Use this to get an interface on the resource that this document represents.
    virtual BOOL QueryResource(const GUID guid, const void **ppv) const;
    // This returns a const pointer - we should never manipulate the resource via this.
    virtual const IResourceBase *GetResource() const = 0;

    // IResourceIdentifier
    int GetPackageHint() const;
    int GetNumber() const;
    ResourceType GetType() const;
    int GetId() const { return _id; }

    bool IsMostRecent() const;

private:
    afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI) { pCmdUI->Enable(IsModified()) ; }
    afx_msg void OnUpdateResSize(CCmdUI *pCmdUI);
    
    BOOL DoPreResourceSave(BOOL fSaveAs);
    BOOL DoResourceSave(int iPackageNumber, int iResourceNumber);
    void OnExportAsBitmap();
    void OnExportAsResource();
    void _OnSuccessfulSaveInternal(const IResourceBase *pRes, int id);

protected:
    void _UpdateTitle();
    afx_msg void OnUpdateAlwaysOn(CCmdUI *pCmdUI) { pCmdUI->Enable() ; }

    virtual void _OnSuccessfulSave(const IResourceBase *pDoc) {};
    virtual ResourceType _GetType() const = 0;
    virtual PCTSTR _GetFileNameDefault() { return g_resourceInfo[_GetType()].pszFileNameDefault; }
    virtual PCTSTR _GetTitleDefault() { return g_resourceInfo[_GetType()].pszTitleDefault; }
    virtual PCTSTR _GetFileDialogFilter() { return g_resourceInfo[_GetType()].pszFileDialogFilter; }

    DECLARE_MESSAGE_MAP()

    int _id;
    bool _fMostRecent;
};

#define ResourceDocQI(obj, Expr) \
     (static_cast<CResourceDocument*>(obj))->QueryCapability(IID_PPV_ARGS(Expr))



