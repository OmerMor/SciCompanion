// ResourceListDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ResourceListDoc.h"

// ResourceListDoc

IMPLEMENT_DYNCREATE(CResourceListDoc, CDocument)

CResourceListDoc::CResourceListDoc()
{
    // Add ourselves as a sync
    CResourceMap &map = theApp.GetResourceMap();
    map.AddSync(this);
    _shownResourceType = RS_NONE;
}

BOOL CResourceListDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    BOOL fRet =  __super::OnOpenDocument(lpszPathName);
    theApp.GenerateBrowseInfo();
    return fRet;
}

BOOL CResourceListDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    theApp.GenerateBrowseInfo();

	return TRUE;
}

void CResourceListDoc::OnCloseDocument()
{
    std::string strEmpty;
    theApp.GetResourceMap().SetGameFolder(strEmpty);

    // Remove ourselves as a sync
    CResourceMap &map = theApp.GetResourceMap();
    map.RemoveSync((ISyncResourceMap*)this);

    theApp.ResetClassBrowser();

    theApp.ClearResourceManagerDoc();

    __super::OnCloseDocument();
}

CResourceListDoc::~CResourceListDoc()
{
}


BEGIN_MESSAGE_MAP(CResourceListDoc, CDocument)
    ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnDisableCommand)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnDisableCommand)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnDisableCommand)
END_MESSAGE_MAP()


// Always disable certain commands when this doc has focus.
// You can't save/saveas or close a resource doc
void CResourceListDoc::OnDisableCommand(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(FALSE);
}

HRESULT CResourceListDoc::OnResourceAdded(const ResourceBlob *pData)
{
    // Add this as the most recent resource of this type/number/package
    theApp._resourceRecency.AddResourceToRecency(pData);

    // Heinous re-interpret cast.
    UpdateAllViews(NULL, VIEWUPDATEHINT_RESOURCEADDED, reinterpret_cast<CObject*>(const_cast<ResourceBlob*>(pData)));
    return S_OK;
}

HRESULT CResourceListDoc::OnResourceDeleted(const ResourceBlob *pDataDeleted)
{
    // Delete this from the recency
    // It is crucial that this come before the update below.  We will check the recency list
    // for the ResourceBlob passed to UpdateAllViews, and at this point, we want to not find
    // it in the list.
    theApp._resourceRecency.DeleteResourceFromRecency(pDataDeleted);

    // Cast away constness...
    UpdateAllViews(NULL, VIEWUPDATEHINT_RESOURCEDELETED, (CObject*)(LPARAM)pDataDeleted);
    return S_OK;
}

HRESULT CResourceListDoc::OnResourceMapReloaded()
{
    UpdateAllViews(NULL, VIEWUPDATEHINT_RESOURCEMAPCHANGED);
    return S_OK;
}

HRESULT CResourceListDoc::OnResourceTypeReloaded(int iType)
{
    CResourceTypeWrap resourceType(iType);
    UpdateAllViews(NULL, VIEWUPDATEHINT_RESOURCETYPECHANGED, &resourceType);
    return S_OK;
}

void CResourceListDoc::ShowResourceType(ResourceType iType)
{
    if (iType != _shownResourceType)
    {
char sz[200];
StringCchPrintf(sz, ARRAYSIZE(sz), "Setting to type %x\n", iType);
OutputDebugString(sz);
        _shownResourceType = iType;
        CResourceTypeWrap resourceType(iType);
        UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SHOWRESOURCETYPE, &resourceType);
    }
}

// ResourceListDoc diagnostics

#ifdef _DEBUG
void CResourceListDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CResourceListDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// ResourceListDoc serialization

void CResourceListDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // There is nothing to save.
        int x = 0;
    }
    else
    {
        CFile *pFile = ar.GetFile();

        // Set the current directory, so we know where to look for the resource files.
        // If not, clicking on an item in the recent documents list won't work
        CString path = pFile->GetFilePath();
        path.MakeLower();
        int iFileOffset = path.Find(TEXT("\\resource.map"));
        if (iFileOffset > 0)
        {
            path.SetAt(iFileOffset, 0); // Null terminate it

            // Set this folder as our new game folder
            CResourceMap &map = theApp.GetResourceMap();
            map.SetGameFolder((PCSTR)path);

            theApp.LogInfo(TEXT("Open game: %s"), (PCTSTR)path);

            UpdateAllViews(NULL, VIEWUPDATEHINT_RESOURCEMAPCHANGED, pFile);
        }
        else
        {
            AfxMessageBox(TEXT("SCI game resources must be called resource.map"), MB_OK | MB_ICONEXCLAMATION);
        }
    }
}


// ResourceListDoc commands
