#pragma once


// CResourceListChildFrame frame

#include "MDITabChildWnd.h"
#include "BarContainerDialog.h"

class ResourcePreviewer;

class CResourceManagerFrame : public CMDITabChildWnd
{
	DECLARE_DYNCREATE(CResourceManagerFrame)
protected:
	CResourceManagerFrame();           // protected constructor used by dynamic creation
	virtual ~CResourceManagerFrame();
    virtual MDITabType GetTabType() { return TAB_GAME; }

public:
    virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
    void OnResourceSelected(const ResourceBlob &blob);
    void OnResourceTypeShown(ResourceType type);

protected:
	DECLARE_MESSAGE_MAP()
    afx_msg BOOL OnShowResource(UINT nId);
    afx_msg void OnUpdateResourceTypeCommand(CCmdUI *pCmdUI);

private:
    ResourcePreviewer *_GetPreviewer(ResourceType iType);

    CExtControlBar m_wndResizableBarPreview;
    CBarContainerDialog m_dlgEmpty;

    typedef std::map<ResourceType, ResourcePreviewer*> previewer_map;
    previewer_map _previewers;
};


