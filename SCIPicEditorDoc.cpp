// SCIPicEditorDoc.cpp : implementation of the CResourceMapDoc class
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "SCIPicEditorDoc.h"
#include "pic.h"
#include "EditPic.h"
#include "ResourceMap.h"
#include "ResourceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CResourceMapDoc

IMPLEMENT_DYNCREATE(CResourceMapDoc, CDocument)

BEGIN_MESSAGE_MAP(CResourceMapDoc, CDocument)
END_MESSAGE_MAP()


// CResourceMapDoc construction/destruction

CResourceMapDoc::CResourceMapDoc()
{
    _iSelectedPic = -1;
    _pEditPic = NULL;

    // Add ourselves as a sync
    CResourceMap &map = theApp.GetResourceMap();
    map.AddSync((ISyncResourceMap*)this);
}

CResourceMapDoc::~CResourceMapDoc()
{
    // Remove ourselves as a sync
    CResourceMap &map = theApp.GetResourceMap();
    map.RemoveSync((ISyncResourceMap*)this);

    // Delete all the pics
    _DeleteAllPics();

    _DeleteAllResourceData();

    if (_pEditPic)
    {
        delete _pEditPic;
    }

}

HRESULT CResourceMapDoc::OnResourceAdded(const CResourceData *pData)
{
    // Add these resource data.
    // Clone it first, since we just got a const copy.
    CResourceData *pDataClone = pData->Clone();

    _resources.Add(pDataClone);

    // And create a pic from them.
    CPic *ppic = new CPic();
    if (ppic)
    {
        if (SUCCEEDED(ppic->InitFromResource(pData)))
        {
            _pics.Add(ppic);
        }
        else
        {
            delete ppic;
        }
    }

    UpdateAllViews(NULL, VIEWUPDATEHINT_RESOURCEMAPCHANGED);

    return S_OK;
}

HRESULT CResourceMapDoc::OnResourceDeleted(DWORD dwID)
{
    // phil TODO
    UpdateAllViews(NULL, VIEWUPDATEHINT_RESOURCEMAPCHANGED);
    return S_OK;
}




void CResourceMapDoc::_DeleteAllPics()
{
    for (INT_PTR i = 0; i <= _pics.GetUpperBound(); i++)
    {
        CPic *pic = _pics.GetAt(i);
        delete pic;
        _pics.SetAt(i, NULL);
    }
    _pics.RemoveAll();
}

void CResourceMapDoc::_DeleteAllResourceData()
{
    for (INT_PTR i = 0; i <= _resources.GetUpperBound(); i++)
    {
        CResourceData *pData = _resources.GetAt(i);
        delete pData;
        _resources.SetAt(i, NULL);
    }
    _resources.RemoveAll();
}


BOOL CResourceMapDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    return TRUE;
}

CPic *CResourceMapDoc::GetSelectedPic()
{
    CPic *ppicRet = NULL;
    if ((-1 != _iSelectedPic) && (_iSelectedPic <= _pics.GetUpperBound()))
    {
        ppicRet = _pics.GetAt(_iSelectedPic);
    }
    return ppicRet;
}



CEditPic *CResourceMapDoc::CreateEditPicFromIndex(INT_PTR iIndex, CEditPicDoc *pDoc)
{
    CEditPic *pepic = NULL;

    if ((-1 != iIndex) && (iIndex <= _resources.GetUpperBound()))
    {
        CResourceData *prd = _resources.GetAt(iIndex);
        if (prd)
        {
            // REVIEW: Need to bail if out of memory, or else indicies will be off.
            pepic = new CEditPic();
            if (pepic)
            {
                pepic->Init(pDoc);
                HRESULT hr = pepic->InitFromResource(prd);
                if (FAILED(hr))
                {
                    delete pepic;
                }
            }
        }
    }
    return pepic;
}



// CResourceMapDoc serialization

void CResourceMapDoc::Serialize(CArchive& ar)
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
            map.SetGameFolder(path);

            // Close it.  We only wanted the path.
            pFile->Close();

            // Clear the current view.
            _DeleteAllResourceData();
            _DeleteAllPics();

            CResourceEnumerator *pEnum;
            if (SUCCEEDED(map.CreateEnumerator(RTF_PIC, &pEnum)))
            {
                CResourceData *pData;
                while (S_OK == pEnum->Next(&pData))
                {
                    // TODO: try catch, and free pData?

                    // Add these resource datas.
                    _resources.Add(pData);

                    // And create a pic from them.
                    CPic *ppic = new CPic();
                    if (ppic)
                    {
                        if (SUCCEEDED(ppic->InitFromResource(pData)))
                        {
                            _pics.Add(ppic);
                        }
                        else
                        {
                            delete ppic;
                        }
                    }
                }

#ifdef DEBUG
                INT_PTR iSizeMemory = 0;
                for (INT_PTR i = 0; i < _pics.GetSize(); i++)
                {
                    CPic *ppic = _pics.GetAt(i);
                    iSizeMemory += ppic->GetMemorySize();
                }
                TCHAR sz[MAX_PATH];
                StringCchPrintf(sz, ARRAYSIZE(sz), TEXT("Memory size of pics: %d"), iSizeMemory);
                OutputDebugString(sz);
#endif 

                SetModifiedFlag(TRUE);
                UpdateAllViews(NULL, VIEWUPDATEHINT_RESOURCEMAPCHANGED);

                delete pEnum;
            }

        }
        else
        {
            AfxMessageBox(TEXT("SCI game resources must be called resource.map"), MB_OK | MB_ICONEXCLAMATION);
        }
    }
}



// CResourceMapDoc diagnostics

#ifdef _DEBUG
void CResourceMapDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CResourceMapDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG


// CResourceMapDoc commands
