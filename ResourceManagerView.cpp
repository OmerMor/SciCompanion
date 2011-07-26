// CResourceManagerView.cpp : implementation of the CResourceManagerView class
//

#include "stdafx.h"
#include "ResourceManagerView.h"
#include "ResourceManagerFrame.h"
#include "ResourceListDoc.h"
#include "ResourceListView.h"
#include "ResourcePicListView.h"
#include "RasterResourceListView.h"
#include "resource.h"
#include "SCIPicEditor.h"
#include "ViewResource.h"
#include "Vocab000.h"
#include "VocabDoc.h"
#include "ViewDoc.h"
#include "FontResource.h"
#include "FontDoc.h"
#include "PicDoc.h"
#include "CursorDoc.h"
#include "PicResource.h"
#include "TextResource.h"
#include "CursorResource.h"
#include "TextDoc.h"
#include "SoundResource.h"
#include "SoundDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef void(* PFNRESOURCEOPEN )(const ResourceBlob *pData);
void g_OpenScript(const ResourceBlob *pData)
{
    theApp.OpenScript(pData->GetName(), pData);
}

void g_OpenView(const ResourceBlob *pData)
{
    // Create the vocab000
    CMultiDocTemplate *pTemplate = theApp.GetViewTemplate();
    if (pTemplate)
    {
        CViewDoc *pDocument = (CViewDoc*)pTemplate->OpenDocumentFile(NULL, TRUE);
        if (pDocument)
        {
            IResourceBase *pResource;
            if (SUCCEEDED(CreateResourceFromResourceData(pData, &pResource)))
            {
                pDocument->SetResource(pResource, pData->GetId());
            }
        }
    }
}

void g_OpenText(const ResourceBlob *pData)
{
    // Create the text
    CMultiDocTemplate *pTemplate = theApp.GetTextTemplate();
    if (pTemplate)
    {
        CTextDoc *pDocument = (CTextDoc*)pTemplate->OpenDocumentFile(NULL, TRUE);
        if (pDocument)
        {
            IResourceBase *pResource;
            if (SUCCEEDED(CreateResourceFromResourceData(pData, &pResource)))
            {
                std::auto_ptr<TextResource> pText(static_cast<TextResource*>(pResource));
                pDocument->SetTextResource(pText, pData->GetId());
            }
        }
    }
}

void g_OpenSound(const ResourceBlob *pData)
{
    // Create the text
    CMultiDocTemplate *pTemplate = theApp.GetSoundTemplate();
    if (pTemplate)
    {
        IResourceBase *pResource;
        if (SUCCEEDED(CreateResourceFromResourceData(pData, &pResource)))
        {
            std::auto_ptr<SoundResource> pSound(static_cast<SoundResource*>(pResource));
            CSoundDoc *pDocument = (CSoundDoc*)pTemplate->OpenDocumentFile(NULL, TRUE);
            if (pDocument)
            {
                pDocument->SetSoundResource(pSound, pData->GetId());
            }
        }
        else
        {
            AfxMessageBox("Unable to create resource.", MB_ERRORFLAGS);
        }
    }
}

void g_OpenVocab(const ResourceBlob *pData)
{
    if (pData->GetNumber() == 0)
    {
        // Create the vocab000
        CMultiDocTemplate *pTemplate = theApp.GetVocabTemplate();
        if (pTemplate)
        {
            CVocabDoc *pDocument = (CVocabDoc*)pTemplate->OpenDocumentFile(NULL, TRUE);
            if (pDocument)
            {
                IResourceBase *pResource;
                if (SUCCEEDED(CreateResourceFromResourceData(pData, &pResource)))
                {
                    pDocument->SetVocab(static_cast<Vocab000*>(pResource), pData->GetId());
                }
            }
        }
    }
    else
    {
        AfxMessageBox(TEXT("Only Vocab 000 is editable."), MB_OK | MB_APPLMODAL);
    }
}


void g_OpenFont(const ResourceBlob *pData)
{
    CMultiDocTemplate *pTemplate = theApp.GetFontTemplate();
    if (pTemplate)
    {
        CFontDoc *pDocument = (CFontDoc*)pTemplate->OpenDocumentFile(NULL, TRUE);
        if (pDocument)
        {
            IResourceBase *pResource;
            if (SUCCEEDED(CreateResourceFromResourceData(pData, &pResource)))
            {
                pDocument->SetResource(pResource, pData->GetId());
            }
        }
    }
}

void g_OpenPic(const ResourceBlob *pData)
{
    // Get the document template, so we can create a new CPicDoc.
    CDocTemplate *pDocTemplate = theApp.GetPicTemplate();
    if (pDocTemplate)
    {
        // and create the PicResource for it.
        CPicDoc *pDocument = (CPicDoc*)pDocTemplate->OpenDocumentFile(NULL, TRUE);
        if (pDocument)
        {
            PicResource *pepic = new PicResource();
            if (pepic)
            {
                HRESULT hr = pepic->InitFromResource(pData);
                if (FAILED(hr))
                {
                    delete pepic;
                }
                else
                {
                    pDocument->SetEditPic(pepic, pData->GetId());
                }
            }
        }
    }
}


void g_OpenCursor(const ResourceBlob *pData)
{
    CMultiDocTemplate *pTemplate = theApp.GetCursorTemplate();
    if (pTemplate)
    {
        CCursorDoc *pDocument = (CCursorDoc*)pTemplate->OpenDocumentFile(NULL, TRUE);
        if (pDocument)
        {
            IResourceBase *pResource;
            if (SUCCEEDED(CreateResourceFromResourceData(pData, &pResource)))
            {
                pDocument->SetResource(pResource, pData->GetId());
            }
        }
    }
}

//
// These are used by the individual views, as well as by
// the OpenResource function.
// This list corresponds to ResourceType enumeration.
//
PFNRESOURCEOPEN g_openFunctions[] =
{
    g_OpenView,
    g_OpenPic,
    g_OpenScript,
    g_OpenText,
    g_OpenSound,
    NULL,
    g_OpenVocab,
    g_OpenFont,
    g_OpenCursor,
    NULL,
};

BOOL OpenResource(const ResourceBlob *pData)
{
    BOOL fRet = FALSE;
    int iType = pData->GetType();
    if (iType < ARRAYSIZE(g_openFunctions))
    {
        PFNRESOURCEOPEN pfnOpen = g_openFunctions[iType];
        if (pfnOpen)
        {
            (*pfnOpen)(pData);
            fRet = TRUE;
        }
        else
        {
            TCHAR szMsg[MAX_PATH];
            StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("%s resources are not supported for editing with this program."), ResourceTypeToText(iType));
            AfxMessageBox(szMsg, MB_ERRORFLAGS);
        }
    }
    else
    {
        AfxMessageBox(TEXT("Invalid resource."), MB_ERRORFLAGS);
    }
    return fRet;

}

CRuntimeClass *g_listClasses[] =
{
    RUNTIME_CLASS(CRasterResourceListCtrl),
    RUNTIME_CLASS(CResourcePicListCtrl),
    RUNTIME_CLASS(CResourceListCtrl),
    RUNTIME_CLASS(CResourceListCtrl),
    RUNTIME_CLASS(CResourceListCtrl),
    RUNTIME_CLASS(CResourceListCtrl),
    RUNTIME_CLASS(CResourceListCtrl),
    RUNTIME_CLASS(CRasterResourceListCtrl),
    RUNTIME_CLASS(CRasterResourceListCtrl),
    RUNTIME_CLASS(CResourceListCtrl),
};

// CResourceManagerView

IMPLEMENT_DYNCREATE(CResourceManagerView, CView)

BEGIN_MESSAGE_MAP(CResourceManagerView, CView)
    ON_COMMAND(ID_VIEWDETAILS, OnViewDetails)
    ON_COMMAND(ID_VIEWTHUMBNAILS, OnViewThumbnails)
    ON_UPDATE_COMMAND_UI(ID_VIEWDETAILS, OnUpdateViewCommands)
    ON_UPDATE_COMMAND_UI(ID_VIEWTHUMBNAILS, OnUpdateViewCommands)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_COMMAND(ID_EDIT_DELETE, OnDelete)
    ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateDelete)
    ON_UPDATE_COMMAND_UI(ID_EDIT_RENAME, OnUpdateDelete)
    ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTASRESOURCE, OnUpdateDelete)
    ON_COMMAND(ID_EDIT_RENAME, OnRename)
    ON_COMMAND(ID_FILE_EXPORTASRESOURCE, OnExtractResources)
    ON_COMMAND(ID_FILE_EXPORTASBITMAP, OnExtractAsBitmap)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_RESSIZE, OnUpdateItemCount)
END_MESSAGE_MAP()

// CResourceManagerView construction/destruction

CResourceManagerView::CResourceManagerView()
{
    _lists.resize(NumResourceTypes, NULL);
    _iSelectedTab = RS_NONE;
    _bFirstSizing = TRUE;
}

CResourceManagerView::~CResourceManagerView()
{
    for_each(_lists.begin(), _lists.end(), delete_object());
}

void CResourceManagerView::OnDraw(CDC *pDC)
{
    // We're a plain old CView, but we don't draw anything, since our children take
    // up all our space.
}

void CResourceManagerView::OnViewDetails()
{
    CResourceListCtrl *pView = _GetSelectedView();
    if (pView)
    {
        pView->ChangeViewMode(LVS_REPORT);
    }
}

void CResourceManagerView::OnViewThumbnails()
{
    CResourceListCtrl *pView = _GetSelectedView();
    if (pView)
    {
        pView->ChangeViewMode(LVS_ICON);
    }
}

void CResourceManagerView::OnUpdateViewCommands(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(TRUE);
    CResourceListCtrl *pView = _GetSelectedView();
    if (pView)
    {
        int iView = pView->GetViewMode();

        if (pCmdUI->m_nID == ID_VIEWTHUMBNAILS)
        {
            pCmdUI->SetCheck((iView == LVS_ICON) ? 1 : 0);
        }
        else if (pCmdUI->m_nID == ID_VIEWDETAILS)
        {
            pCmdUI->SetCheck((iView == LVS_REPORT) ? 1 : 0);
        }
    }
}


int CResourceManagerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;
    return 0;
}

void CResourceManagerView::OnSize(UINT nType, int cx, int cy)
{
    if (_bFirstSizing && (cx > 0) && (cy > 0))
    {
        // Select the first
        CResourceListDoc *pDoc = GetDocument();
        pDoc->ShowResourceType(RS_VIEW);
        _bFirstSizing = FALSE;
    }

    // Set the size of the currently selected tab:
    _SyncSize();
}

void CResourceManagerView::_SyncSize()
{
    CResourceListCtrl *pView = _GetSelectedView();
    if (pView && pView->GetSafeHwnd())
    {
        RECT rc;
        GetClientRect(&rc); // They occupy the full real-estate
        pView->MoveWindow(&rc);
    }
}

CResourceListCtrl *CResourceManagerView::_GetSelectedView()
{
    if ((_iSelectedTab >= 0) && (_iSelectedTab < (int)_lists.size()))
    {
        return _lists[_iSelectedTab];
    }
    return NULL;
}

void CResourceManagerView::_OnSelChange(ResourceType iSelectedTab)
{
    if (_iSelectedTab != iSelectedTab)
    {
        _iSelectedTab = iSelectedTab;

        // Tell our frame
        CResourceManagerFrame *pFrame = static_cast<CResourceManagerFrame*>(GetParentFrame());
        if (pFrame)
        {
            pFrame->OnResourceTypeShown(_iSelectedTab);
        }
        CResourceListDoc *pDoc = GetDocument();
        if (pDoc)
        {
            pDoc->ShowResourceType(_iSelectedTab);
        }
        
        ASSERT(iSelectedTab < (int)_lists.size());
        if (_lists[iSelectedTab] == NULL)
        {
            _lists[iSelectedTab] = (CResourceListCtrl*)g_listClasses[iSelectedTab]->CreateObject();
            // First time this tab has been shown. Initialize and create the control.

            _lists[iSelectedTab]->SetDocument(GetDocument());
            _lists[iSelectedTab]->SetType(iSelectedTab);
            _lists[iSelectedTab]->SetOpenFunction(g_openFunctions[iSelectedTab]);
            
            RECT rc;
            GetClientRect(&rc);
            _lists[iSelectedTab]->Create(WS_CHILD | WS_VISIBLE, rc, this, 0);
            _lists[iSelectedTab]->UpdateView();
        }

        if (_lists[iSelectedTab])
        {
            // Show this one:
            _SyncSize();  ASSERT(_iSelectedTab == iSelectedTab);
            _lists[iSelectedTab]->ShowWindow(SW_SHOW);

            // And hide the others:
            for (int i = 0; i < (int)_lists.size(); i++)
            {
                if ((i != iSelectedTab) && (_lists[i]->GetSafeHwnd()))
                {
                    _lists[i]->ShowWindow(SW_HIDE);
                }
            }
        }
    }
}

void CResourceManagerView::_OnSelChange()
{
    // Activate this tab.
    _OnSelChange(_iSelectedTab);
}


void CResourceManagerView::OnDelete()
{
    CResourceListCtrl *pView = _GetSelectedView();
    if (pView && !pView->InLabelEdit())
    {
        pView->OnDelete();
    }
}

void CResourceManagerView::OnExtractResources()
{
    CResourceListCtrl *pView = _GetSelectedView();
    if (pView && !pView->InLabelEdit())
    {
        pView->OnExtractResources();
    }
}

void CResourceManagerView::OnExtractAsBitmap()
{
    CResourceListCtrl *pView = _GetSelectedView();
    if (pView && !pView->InLabelEdit())
    {
        pView->OnExtractAsBitmap();
    }
}

void CResourceManagerView::OnRename()
{
    CResourceListCtrl *pView = _GetSelectedView();
    if (pView && !pView->InLabelEdit())
    {
        pView->OnRename();
    }
}


void CResourceManagerView::OnUpdateDelete(CCmdUI *pCmdUI)
{
    BOOL bEnable = FALSE;
    CResourceListCtrl *pView = _GetSelectedView();
    if (pView && !pView->InLabelEdit())
    {
        bEnable = (pView->GetSelectedItem() != -1);
    }
    pCmdUI->Enable(bEnable);
}

void CResourceManagerView::OnUpdateItemCount(CCmdUI *pCmdUI)
{
    // We need to show something useful, so how about how many items are in the view.
    CResourceListCtrl *pView = _GetSelectedView();
    if (pView)
    {
        TCHAR szBuf[MAX_PATH];
        int iSelectedTab = _iSelectedTab;
        ASSERT(iSelectedTab < ARRAYSIZE(g_resourceInfo));
        StringCchPrintf(szBuf, ARRAYSIZE(szBuf), TEXT("%d %s resources"), pView->GetItemCount(), g_resourceInfo[iSelectedTab].pszTitleDefault);
        pCmdUI->SetText(szBuf);
    }
}

BOOL CResourceManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
    return __super::PreCreateWindow(cs);
}


void CResourceManagerView::OnInitialUpdate()
{
    theApp.ShowResourceType(RS_VIEW);
    _dropTarget.Register(this);
    __super::OnInitialUpdate();
}


void CResourceManagerView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
    if (lHint & (VIEWUPDATEHINT_RESOURCEADDED | VIEWUPDATEHINT_RESOURCEDELETED))
    {
        // Go tell each of our listviews that a resource was added (or deleted)
        ResourceBlob *pData = (ResourceBlob*)pHint;
        if (pData)
        {
            // If we got a resource data, be selective, and only tell the view that represents that resource type
            int iType = pData->GetType();
            ASSERT(iType < (int)_lists.size());
            if (_lists[iType])
            {
                _lists[iType]->OnUpdate(lHint, pHint);
            }
        }
    }
    else if (lHint & VIEWUPDATEHINT_RESOURCEMAPCHANGED)
    {
        // Tell everyone.
        for (int i = 0; i < (int)_lists.size(); i++)
        {
            if (_lists[i])
            {
                _lists[i]->OnUpdate(lHint, pHint);
            }
        }
    }
    else if (lHint & VIEWUPDATEHINT_RESOURCETYPECHANGED)
    {
        int iType = ((CResourceTypeWrap*)pHint)->GetType();
        if ((iType >= 0) && (iType < (int)_lists.size()) && _lists[iType])
        {
            // Tell the appropriate list (assuming it exists) to update
            _lists[iType]->OnUpdate(lHint, pHint);
        }
    }
    else if(lHint & VIEWUPDATEHINT_SHOWRESOURCETYPE)
    {
        ResourceType iType = (ResourceType)((CResourceTypeWrap*)pHint)->GetType();
        if ((iType >= 0) && (iType < (int)_lists.size()))
        {
            _OnSelChange(iType);
        }
    }
}

BOOL IsResourceFileName(PCTSTR pszFileName, int *piNumber)
{
    BOOL fRet = FALSE;
    DWORD dwType;
    PCTSTR psz = IsResourceName(pszFileName, &dwType);
    if (psz)
    {
        if (*psz == TEXT('.'))
        {
            psz++;
            // Now come three numbers.
            if (3 == lstrlen(psz))
            {
                *piNumber = StrToInt(psz);
                fRet = TRUE;
            }
        }
    }
    return fRet;
}

bool _IsBitmapFile(PCSTR pszFileName)
{
    return (0 == strcmpi(PathFindExtension(pszFileName), ".bmp"));
}

//
// Drops the files in pDropFiles into the game.
//
void DropResourceFiles(CArray<CString, CString&> *pDropFiles)
{
    // Be sparing in our DeferAppends, since if they are just dropping one file,
    // we'll be nice and highlight it in red.  If we Begin/End DeferAppend, then we'll lose
    // this information (since we completely reload the resource map for that type)
    DeferResourceAppend defer(theApp.GetResourceMap(), pDropFiles->GetCount() > 1);

    for (int i = 0; i < pDropFiles->GetCount(); i++)
    {
        int iNumber;
        if (IsResourceFileName(PathFindFileName(pDropFiles->GetAt(i)), &iNumber))
        {
            ResourceBlob data;
            if (SUCCEEDED(data.CreateFromFile(NULL, (PCSTR)pDropFiles->GetAt(i), 1, iNumber)))
            {
                theApp.GetResourceMap().AppendResource(data);
            }
        }
        else if (_IsBitmapFile(pDropFiles->GetAt(i)))
        {
            // It's a bmp file... it might have a resource encoded in it.
            std::auto_ptr<ResourceBlob> data(Load8BitBmp((PCSTR)pDropFiles->GetAt(i)));
            if (data.get())
            {
                char szName[MAX_PATH];
                StringCchCopy(szName, ARRAYSIZE(szName), PathFindFileName(pDropFiles->GetAt(i)));
                *PathFindExtension(szName) = 0; // cut off extension.
                data->SetName(szName);
                theApp.GetResourceMap().AppendResourceAskForNumber(*data);
            }
            else
            {
                AfxMessageBox("There doesn't appear to be an SCI resource encoded in this .bmp file.", MB_ERRORFLAGS);
            }
        }
    }
    defer.Commit();
}

//
// Fills pFileList with filenames from the dataobject that meet "being a resource" requirements.
// Returns TRUE if there was at least one that met this requirement.
//
BOOL GetDropFiles(COleDataObject *pDataObject, CArray<CString, CString&> *pFileList)
{
    BOOL fRet = FALSE;
    // Get the HDROP data from the data object.
    HGLOBAL hg = pDataObject->GetGlobalData(CF_HDROP);
    if (hg)
    {
        DROPFILES *pDropFiles = (DROPFILES*)(HDROP)GlobalLock(hg);
        if (pDropFiles )
        {
            // We have to do different things depending on whether it's unicode or ansi.
            if (pDropFiles->fWide)
            {
                PWSTR pwsz = (PWSTR)((BYTE*)pDropFiles + pDropFiles->pFiles);
                while (*pwsz)
                {
                    CString strFile;
                    PSTR psz = strFile.GetBuffer(lstrlenW(pwsz) + 1);
                    WideCharToMultiByte(CP_ACP, 0, pwsz, -1, psz, lstrlenW(pwsz) + 1, NULL, NULL);
                    PTSTR pszFileName = PathFindFileName(strFile);
                    int iNumber;
                    if (IsResourceFileName(pszFileName, &iNumber) || _IsBitmapFile(pszFileName))
                    {
                        pFileList->Add(strFile);
                        fRet = TRUE;
                    }
                    strFile.ReleaseBuffer();
                    pwsz += lstrlenW(pwsz);
                }
            }
            else
            {
                PSTR psz = (PSTR)((BYTE*)pDropFiles + pDropFiles->pFiles);
                while (*psz)
                {
                    PTSTR pszFileName = PathFindFileName(psz);
                    int iNumber;
                    if (IsResourceFileName(pszFileName, &iNumber) || _IsBitmapFile(pszFileName))
                    {
                        CString strFile = psz;
                        pFileList->Add(strFile);
                        fRet = TRUE;
                    }
                    psz += lstrlen(psz);
                }
            }
        }
        GlobalUnlock(hg);
    }    
    return fRet;
}

DROPEFFECT CResourceManagerView::OnDragEnter(COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
    DROPEFFECT effect = DROPEFFECT_NONE;
    _dropFiles.RemoveAll();
    // Get the HDROP data from the data object.
    HGLOBAL hg = pDataObject->GetGlobalData(CF_HDROP);
    if (hg)
    {
        DROPFILES *pDropFiles = (DROPFILES*)(HDROP)GlobalLock(hg);
        if (pDropFiles )
        {
            if (GetDropFiles(pDataObject, &_dropFiles))
            {
                effect = DROPEFFECT_COPY;
            }
            GlobalUnlock(hg);
        }
    }
    return effect;
}
DROPEFFECT CResourceManagerView::OnDragOver(COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
    DROPEFFECT effect = DROPEFFECT_NONE;
    if (!_dropFiles.IsEmpty())
    {
        effect = DROPEFFECT_COPY;
    }
    return effect;

}
void CResourceManagerView::OnDragLeave()
{
    _dropFiles.RemoveAll();
}

BOOL CResourceManagerView::OnDrop(COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint point)
{
    BOOL fRet = FALSE;
    if (dropEffect == DROPEFFECT_COPY)
    {
        _dropFiles.RemoveAll();
        // Get the HDROP data from the data object.
        HGLOBAL hg = pDataObject->GetGlobalData(CF_HDROP);
        if (hg)
        {
            DROPFILES *pDropFiles = (DROPFILES*)(HDROP)GlobalLock(hg);
            if (pDropFiles )
            {
                GetDropFiles(pDataObject, &_dropFiles);
                if (!_dropFiles.IsEmpty())
                {
                    // Do the drop.
                    DropResourceFiles(&_dropFiles);
                }
                GlobalUnlock(hg);
            }
        }
    }
    return fRet;
}



// CResourceManagerView diagnostics

#ifdef _DEBUG
void CResourceManagerView::AssertValid() const
{
    __super::AssertValid();
}

void CResourceManagerView::Dump(CDumpContext& dc) const
{
    __super::Dump(dc);
}

CResourceListDoc* CResourceManagerView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResourceListDoc)));
    return (CResourceListDoc*)m_pDocument;
}
#endif //_DEBUG


// CResourceManagerView message handlers
