// ResourceManagerFrame.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ResourceManagerFrame.h"
#include "ResourcePreviewer.h"

// CResourceListChildFrame

IMPLEMENT_DYNCREATE(CResourceManagerFrame, CMDITabChildWnd)

CResourceManagerFrame::CResourceManagerFrame()
{
    theApp.SetExplorerFrame(this);
}

CResourceManagerFrame::~CResourceManagerFrame()
{
    theApp.SetExplorerFrame(NULL);
    for_each(_previewers.begin(), _previewers.end(), delete_map_value());
}

//
// Get (or create and get) the previewer for a particular resource type.
//
ResourcePreviewer *CResourceManagerFrame::_GetPreviewer(ResourceType iType)
{
    if (!m_dlgEmpty.GetSafeHwnd())
    {
        return NULL;
    }
    else
    {
        previewer_map::iterator previewIt = _previewers.find(iType);
        if (previewIt != _previewers.end())
        {
            return previewIt->second;
        }
        else
        {
            // First time - create one.
            ResourcePreviewer *pPreviewer = NULL;
            switch (iType)
            {
            case RS_PIC:
                pPreviewer = new PicPreviewer;
                break;
            case RS_VIEW:
                pPreviewer = new ViewPreviewer;
                break;
            case RS_SCRIPT:
                pPreviewer = new ScriptPreviewer;
                break;
            case RS_TEXT:
                pPreviewer = new TextPreviewer;
                break;
            case RS_VOCAB:
                pPreviewer = new VocabPreviewer;
                break;
            case RS_FONT:
                pPreviewer = new FontPreviewer;
                break;
            case RS_SOUND:
                pPreviewer = new SoundPreviewer;
                break;
            case RS_PATCH:
            case RS_CURSOR:
                pPreviewer = new BlankPreviewer;
                break;
            }
            if (pPreviewer)
            {
                pPreviewer->Create(&m_dlgEmpty);
                _previewers[iType] = pPreviewer;
            }
            return pPreviewer;
        }
    }
}

void CResourceManagerFrame::OnResourceSelected(const ResourceBlob &blob)
{
    // Find the previewer for this item, and update it.
    ResourcePreviewer *pPreviewer = _GetPreviewer((ResourceType)blob.GetType());
    if (pPreviewer)
    {
        pPreviewer->SetResource(blob);
    }
}

void CResourceManagerFrame::OnResourceTypeShown(ResourceType type)
{
    ResourcePreviewer *pPreviewer = _GetPreviewer(type);
    if (pPreviewer)
    {
        m_dlgEmpty.SelectChild(pPreviewer, type);
    }
}

BEGIN_MESSAGE_MAP(CResourceManagerFrame, CMDITabChildWnd)
    ON_WM_CREATE()
    ON_COMMAND_EX(ID_SHOW_VIEWS, OnShowResource)
    ON_COMMAND_EX(ID_SHOW_PICS, OnShowResource)
    ON_COMMAND_EX(ID_SHOW_SCRIPTS, OnShowResource)
    ON_COMMAND_EX(ID_SHOW_TEXTS, OnShowResource)
    ON_COMMAND_EX(ID_SHOW_SOUNDS, OnShowResource)
    ON_COMMAND_EX(ID_SHOW_MEMORY, OnShowResource)
    ON_COMMAND_EX(ID_SHOW_VOCABS, OnShowResource)
    ON_COMMAND_EX(ID_SHOW_FONTS, OnShowResource)
    ON_COMMAND_EX(ID_SHOW_CURSORS, OnShowResource)
    ON_COMMAND_EX(ID_SHOW_PATCHES, OnShowResource)
    ON_UPDATE_COMMAND_UI(ID_SHOW_VIEWS, OnUpdateResourceTypeCommand)
    ON_UPDATE_COMMAND_UI(ID_SHOW_PICS, OnUpdateResourceTypeCommand)
    ON_UPDATE_COMMAND_UI(ID_SHOW_SCRIPTS, OnUpdateResourceTypeCommand)
    ON_UPDATE_COMMAND_UI(ID_SHOW_TEXTS, OnUpdateResourceTypeCommand)
    ON_UPDATE_COMMAND_UI(ID_SHOW_SOUNDS, OnUpdateResourceTypeCommand)
    ON_UPDATE_COMMAND_UI(ID_SHOW_VOCABS, OnUpdateResourceTypeCommand)
    ON_UPDATE_COMMAND_UI(ID_SHOW_FONTS, OnUpdateResourceTypeCommand)
    ON_UPDATE_COMMAND_UI(ID_SHOW_CURSORS, OnUpdateResourceTypeCommand)
    ON_UPDATE_COMMAND_UI(ID_SHOW_PATCHES, OnUpdateResourceTypeCommand)

    ON_COMMAND_EX(ID_BAR_PREVIEW, OnBarCheck)
    ON_UPDATE_COMMAND_UI(ID_BAR_PREVIEW, OnUpdateControlBarMenu)
END_MESSAGE_MAP()

BOOL CResourceManagerFrame::PreCreateWindow(CREATESTRUCT &cs)
{
    // This is the first window that opens - set the stage by making
    // it maximized.  (all future windows appear to inherit the
    // the current maximize/minimize state).
    cs.style |= WS_MAXIMIZE | WS_VISIBLE;
    return __super::PreCreateWindow(cs);
}

BOOL CResourceManagerFrame::OnShowResource(UINT nId)
{
    theApp.ShowResourceType(ResourceCommandToType(nId));
    return TRUE;
}
void CResourceManagerFrame::OnUpdateResourceTypeCommand(CCmdUI *pCmdUI)
{
    pCmdUI->Enable();
    pCmdUI->SetCheck(theApp.GetShownResourceType() == ResourceCommandToType(pCmdUI->m_nID));
}


// CResourceListChildFrame message handlers

int CResourceManagerFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Preview pane - 280px wide by default
    m_wndResizableBarPreview.SetInitDesiredSizeVertical(CSize(280, 100));
    if (!m_wndResizableBarPreview.Create("Preview", this, ID_BAR_PREVIEW))
    {
	    TRACE0("Failed to create m_wndResizableBarPreview\n");
	    return -1;		// fail to create
    }
    m_wndResizableBarPreview.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
    if (!m_dlgEmpty.Create(CBarContainerDialog::IDD, &m_wndResizableBarPreview))
    {
	    TRACE0("Failed to create empty dialog\n");
	    return -1;		// fail to create
    }
    // The first thing shown is views, so do that here.  It happens automatically in "OnCreate",
    // but we haven't created the m_dlgEmpty yet.
    OnResourceTypeShown(RS_VIEW);

    if (!CExtControlBar::FrameEnableDocking(this))
    {
        ASSERT( FALSE );
        return -1;
    }
    m_wndResizableBarPreview.DockControlBarInnerOuter(AFX_IDW_DOCKBAR_RIGHT, true, this, true);

    return 0;
}
