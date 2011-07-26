// ResourceDocument.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ResourceDocument.h"
#include "ResourceMap.h"
#include "ResourceBase.h"
#include "SaveResourceDialog.h"
#include <errno.h>

// CResourceDocument

IMPLEMENT_DYNAMIC(CResourceDocument, CDocument)

BEGIN_MESSAGE_MAP(CResourceDocument, CDocument)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
    ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
    ON_COMMAND(ID_FILE_EXPORTASRESOURCE, OnExportAsResource)
    ON_COMMAND(ID_FILE_EXPORTASBITMAP, OnExportAsBitmap) // The UPDATE_COMMAND_UI is in the sub classes.
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateAlwaysOn)
    ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTASRESOURCE, OnUpdateAlwaysOn)
    ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTASBITMAP, OnUpdateAlwaysOn) // The UPDATE_COMMAND_UI is in the sub classes.
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_RESSIZE, OnUpdateResSize)
END_MESSAGE_MAP()

BOOL CResourceDocument::CanCloseFrame(CFrameWnd* pFrameArg)
{
	// Override this, since we may have more than one view in our childframes,
    // and if we do, the default implementation thinks there is still always
    // still one view around on the document (so it won't call SaveModified).
	return SaveModified();
}

void CResourceDocument::OnFileSave()
{
    if (IsModified())
    {
        DoPreResourceSave(FALSE);
    }
}

void CResourceDocument::OnFileSaveAs()
{
    DoPreResourceSave(TRUE);
}


// return TRUE if ok to continue
BOOL CResourceDocument::SaveModified()
{
	if (!IsModified())
		return TRUE;        // ok to continue

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, this->GetTitle());
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		if (!DoPreResourceSave(FALSE))
			return FALSE;       // don't continue
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
}

void CResourceDocument::_UpdateTitle()
{
    TCHAR sz[20];
    const IResourceBase *pResource = GetResource();
    if (pResource && (pResource->GetResourceNumber() != -1))
    {
        StringCchPrintf(sz, ARRAYSIZE(sz), TEXT("%s.%03d"), _GetTitleDefault(), pResource->GetResourceNumber());
        // TODO: Use game.ini name for the title.
    }
    else
    {
        StringCchPrintf(sz, ARRAYSIZE(sz), TEXT("%s - new"), _GetTitleDefault());
    }

    // Set the title:
    SetTitle(sz);
    // Also update recency
    _fMostRecent = (theApp._resourceRecency.IsResourceMostRecent(this) != FALSE);
}

//
// Actually go ahead and serialize the resource, just to give an indication of its size!
//
void CResourceDocument::OnUpdateResSize(CCmdUI *pCmdUI)
{
    TCHAR szBuf[MAX_PATH];
    StringCchCopy(szBuf, ARRAYSIZE(szBuf), TEXT("Unknown size"));
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        sci::istream serial;
        if (pResource->Serialize(&serial))
        {
            StringCchPrintf(szBuf, ARRAYSIZE(szBuf), TEXT("%s: %d bytes"), _GetTitleDefault(), serial.GetDataSize());
        }
    }
    pCmdUI->SetText(szBuf);
}


BOOL CResourceDocument::QueryResource(const GUID guid, const void **ppv) const
{
    BOOL fRet = FALSE;
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        fRet = pResource->QueryCapability(guid, ppv);
    }
    return fRet;
}


BOOL CResourceDocument::DoPreResourceSave(BOOL fSaveAs)
{
    BOOL fRet = FALSE;
    if (theApp.GetResourceMap().IsGameLoaded())
    {
        BOOL fCancelled = FALSE;
        // const_cast: special case, because we're saving... we're modifying the resource only
        // by giving it a resource package/number
        IResourceBase *pResource = const_cast<IResourceBase*>(GetResource());
        if (pResource)
        {
            // Make sure we have a package and resource number.
            int iResourceNumber = pResource->GetResourceNumber();
            int iPackageNumber = pResource->GetPackageNumber();
            if (fSaveAs || (iResourceNumber == -1) || (iPackageNumber == -1))
            {
                if (iResourceNumber == -1)
                {
                    iResourceNumber = theApp.GetResourceMap().SuggestResourceNumber(pResource->GetType());
                }
                // Invoke dialog.
                SaveResourceDialog srd;
                srd.Init(iPackageNumber, iResourceNumber);
                if (IDOK == srd.DoModal())
                {
                    iResourceNumber = srd.GetResourceNumber();
                    iPackageNumber = srd.GetPackageNumber();
                }
                else
                {
                    fCancelled = TRUE;
                }
            }

            if (!fCancelled && (iResourceNumber != -1) && (iPackageNumber != -1))
            {
                // We're good to go.
                fRet = DoResourceSave(iPackageNumber, iResourceNumber);
                if (fRet)
                {
                    // If we successfully saved, make sure our resource has these
                    // possibly new package/resource numbers.
                    pResource->SetPackageNumber(iPackageNumber);
                    pResource->SetResourceNumber(iResourceNumber);

                    // We might have a new resource number, so update our title.
                    _UpdateTitle();
                }
            }
        }
    }
    else
    {
        // No game is loaded!  Assume the user wants to export.
        OnExportAsResource();
    }
    return fRet;
}

BOOL CResourceDocument::DoResourceSave(int iPackageNumber, int iResourceNumber)
{
    BOOL fSaved = FALSE;
    // Ignore path name.
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        fSaved = pResource->PerformChecks();
        if (fSaved)
        {
            sci::istream serial;
            fSaved = pResource->Serialize(&serial);
            if (fSaved)
            {
                fSaved = ValidateResourceSize(serial.GetDataSize());
                if (fSaved)
                {
                    fSaved = FALSE;
                    CResourceMap &map = theApp.GetResourceMap();
                    ResourceBlob data;
                    serial.Seek(0); // Reset to beginning
                    if (SUCCEEDED(data.CreateFromBits(NULL, _GetType(), &serial, iPackageNumber, iResourceNumber)))
                    {
                        if (SUCCEEDED(map.AppendResource(data)))
                        {
                            fSaved = TRUE;
                            _OnSuccessfulSaveInternal(pResource, data.GetId());
                        }
                    }
                }
            }

            if (fSaved)
            {
                _UpdateTitle();
            }

            SetModifiedFlag(!fSaved);
        }
    }
    return fSaved;
}

void CResourceDocument::_OnSuccessfulSaveInternal(const IResourceBase *pRes, int id)
{
    _id = id;
    _OnSuccessfulSave(pRes);
}

void CResourceDocument::OnExportAsBitmap()
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        ExportResourceAsBitmap(pResource);
    }
}

void ExportResourceAsBitmap(const IResourceBase *pResource)
{
    sci::istream serial;
    if (pResource->Serialize(&serial))
    {
        ResourceBlob data;
        serial.Seek(0);
        // Bring up the file dialog
        int iNumber = pResource->GetResourceNumber();
        if (iNumber == -1)
        {
            iNumber = 0;
        }
        if (SUCCEEDED(data.CreateFromBits(NULL, pResource->GetType(), &serial, pResource->GetPackageNumber(), iNumber)))
        {
            CBitmap bitmap;
            SCIBitmapInfoEx bmi(0, 0);
            BYTE *pBitsDest;
            bitmap.Attach(CreateBitmapFromResource(pResource, &bmi, &pBitsDest));
            if ((HBITMAP)bitmap)
            {
                // Now let's encode it...
                if (EncodeResourceInBitmap(data, bmi, pBitsDest))
                {
                    // Get a file name
                    std::string name = data.GetName();
                    name += ".bmp";
                    CFileDialog fileDialog(FALSE, NULL, name.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmaps (*.bitmap)|*.bitmap|All files (*.*)|*.*|");
                    if (IDOK == fileDialog.DoModal())
                    {
                        std::string fileName = (PCSTR)fileDialog.GetFileName();
                        if (!Save8BitBmp(fileName, bmi, pBitsDest, SCIResourceBitmapMarker))
                        {
                            AfxMessageBox("Unable to save bitmap.", MB_ERRORFLAGS);
                        }
                    }
                }
                else
                {
                    AfxMessageBox("Unable to put resource in bitmap.", MB_ERRORFLAGS);
                }
            }
        }
    }
}

void CResourceDocument::OnExportAsResource()
{
    // Ignore path name.
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        sci::istream serial;
        BOOL fSaved = FALSE;
        fSaved = pResource->Serialize(&serial);
        if (fSaved)
        {
            fSaved = FALSE;
            // Bring up the file dialog
            int iNumber = pResource->GetResourceNumber();
            if (iNumber == -1)
            {
                iNumber = 0;
            }
            TCHAR szFileName[MAX_PATH];
            StringCchPrintf(szFileName, ARRAYSIZE(szFileName), TEXT("%s.%03d"), _GetFileNameDefault(), iNumber);

            CFileDialog fileDialog(FALSE, NULL, szFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _GetFileDialogFilter());
            if (IDOK == fileDialog.DoModal())
            {
                CString strFileName = fileDialog.GetFileName();
                ResourceBlob data;
                serial.Seek(0);
                if (SUCCEEDED(data.CreateFromBits(NULL, _GetType(), &serial, pResource->GetPackageNumber(), iNumber)))
                {
                    HRESULT hr = data.SaveToFile((PCSTR)strFileName);
                    if (FAILED(hr))
                    {
                        DisplayFileError(hr, FALSE, strFileName);
                    }
                    fSaved = SUCCEEDED(hr);
                }
            }
            if (fSaved)
            {
                SetModifiedFlag(FALSE);
            }
            // else if we didn't save, we don't want to touch the modified flag.
            // The document may not have been dirty at all.
        }
    }
}

bool CResourceDocument::IsMostRecent() const
{
    return _fMostRecent;
}

int CResourceDocument::GetPackageHint() const
{
    const IResourceBase *pRes = GetResource();
    if (pRes)
    {
        return pRes->GetPackageNumber();
    }
    else
    {
        return -1;
    }
}
int CResourceDocument::GetNumber() const
{
    const IResourceBase *pRes = GetResource();
    if (pRes)
    {
        return pRes->GetResourceNumber();
    }
    else
    {
        return -1;
    }
}
ResourceType CResourceDocument::GetType() const
{
    return _GetType();
}


// CResourceDocument commands
