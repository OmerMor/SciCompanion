// RasterResourceDocument.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "RasterResourceDocument.h"
#include "CelResource.h"
#include "RasterResource.h"

// CRasterResourceDocument

BEGIN_MESSAGE_MAP(CRasterResourceDocument, TCLASS_2(CUndoResource, CResourceDocument, IResourceBase))
    ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTASBITMAP, CResourceDocument::OnUpdateAlwaysOn) // Put this here instead of in CResourceDocument
END_MESSAGE_MAP()

CRasterResourceDocument::CRasterResourceDocument()
{
    _nPenWidth = 1;
    _fLocked = false;
    _fApplyToAllCels = FALSE;
}

CRasterResourceDocument::~CRasterResourceDocument()
{
}

void CRasterResourceDocument::v_OnUndoRedo()
{
    _ValidateCelIndex();
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_NEWVIEW);
}

void CRasterResourceDocument::SetResource(IResourceBase *pResource, int id)
{
    _id = id;
    AddFirstResource(pResource);
    _ValidateCelIndex();
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_NEWVIEW);
    _UpdateTitle();
}

void CRasterResourceDocument::SetPenWidth(int nPenWidth)
{
    int nNewPenWidth = max(1, min(8, nPenWidth));
    if (nNewPenWidth != _nPenWidth)
    {
        _nPenWidth = nNewPenWidth;
        UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_PENWIDTHCHANGED);
    }
}


//
// Adds a new resource to the undo stack.
//
void CRasterResourceDocument::_NewResource(IResourceBase *pResourceNew, LPARAM lHint)
{
    AddNewResourceToUndo(pResourceNew);
    _ValidateCelIndex();

    SetModifiedFlag(TRUE);

    IEditRasterResourceDoc *perrd;
    if (QueryCapability(IID_PPV_ARGS(&perrd)))
    {
        CCelDescription desc(perrd->GetSelectedIndex());
        UpdateAllViewsAndNonViews(NULL, lHint, &desc);
    }
    else
    {
        UpdateAllViewsAndNonViews(NULL, lHint, NULL);
    }
}

//
// IEditRasterResource
//
BOOL CRasterResourceDocument::SetSize(DWORD dwIndex, CSize size, RasterBitmap::RasterResizeFlags flags)
{
    BOOL fRet = FALSE;
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        // Take the current resource and clone it, then apply the changes.
        IResourceBase *pResourceClone = pResource->Clone();
        IEditRasterResource *perr;
        if (pResourceClone && pResourceClone->QueryCapability(IID_PPV_ARGS(&perr)))
        {
            fRet = perr->SetSize(dwIndex, size, flags);
            _NewResource(pResourceClone, VIEWUPDATEHINT_CELCHANGED);
        }
    }
    return fRet;
}

void CRasterResourceDocument::SetPlacement(DWORD dwIndex, CPoint pt)
{
    BOOL fRet = FALSE;
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        // Take the current resource and clone it, then apply the changes.
        IResourceBase *pClone = pResource->Clone();
        IEditRasterResource *perr;
        if (pClone && pClone->QueryCapability(IID_PPV_ARGS(&perr)))
        {
            perr->SetPlacement(dwIndex, pt);
            _NewResource(pClone, VIEWUPDATEHINT_CELCHANGED);
        }
    }
}


void CRasterResourceDocument::MoveFromTo(DWORD dwIndexFrom, DWORD dwIndexTo)
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        // Take the current resource and clone it, then apply the changes.
        IResourceBase *pClone = pResource->Clone();
        IEditRasterResource *perr;
        if (pClone && pClone->QueryCapability(IID_PPV_ARGS(&perr)))
        {
            perr->MoveFromTo(dwIndexFrom, dwIndexTo);
            _NewResource(pClone, VIEWUPDATEHINT_LOOPCHANGED);
        }
    }    
}

void CRasterResourceDocument::SetBitmapData(DWORD dwIndex, const BYTE *pBits)
{
    SetGroupBitmapData(1, &dwIndex, &pBits);
}

//
// An alternate way would be to allow batching of _NewResource things.  e.g.
// Turn off updates until the caller says.  However, we need to only have one cloned
// resource.  So that would mean keeping around a cloned resource across calls.  So maybe
// it is better this way.
//
void CRasterResourceDocument::SetGroupBitmapData(int cCels, DWORD *rgdwIndex, const BYTE* const* ppData)
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        // Take the current resource and clone it, then apply the changes.
        IResourceBase *pResourceClone = pResource->Clone();
        IEditRasterResource *perr;
        if (pResourceClone && pResourceClone->QueryCapability(IID_PPV_ARGS(&perr)))
        {
            int i = 0;
            for (; i < cCels; i++)
            {
                perr->SetBitmapData(rgdwIndex[i], ppData[i]);
            }
            LPARAM lHint = VIEWUPDATEHINT_LOOPCHANGED;
            if ((cCels == 1 && (rgdwIndex[i] == GetSelectedIndex())))
            {
                lHint = VIEWUPDATEHINT_CELCHANGED; // Turns out only the selected cel changed.
            }
            _NewResource(pResourceClone, lHint);
        }
    }
}

void CRasterResourceDocument::SetGroupPlacement(int cCels, DWORD *rgdwIndex, CPoint pt)
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        // Take the current resource and clone it, then apply the changes.
        IResourceBase *pResourceClone = pResource->Clone();
        IEditRasterResource *perr;
        if (pResourceClone && pResourceClone->QueryCapability(IID_PPV_ARGS(&perr)))
        {
            int i = 0;
            for (; i < cCels; i++)
            {
                perr->SetPlacement(rgdwIndex[i], pt);
            }
            LPARAM lHint = VIEWUPDATEHINT_LOOPCHANGED;
            if ((cCels == 1 && (rgdwIndex[i] == GetSelectedIndex())))
            {
                lHint = VIEWUPDATEHINT_CELCHANGED; // Turns out only the selected cel changed.
            }
            _NewResource(pResourceClone, lHint);
        }
    }
}


void CRasterResourceDocument::SetGroupSize(int cCels, DWORD *rgdwIndex, const CSize *rgSizes, RasterBitmap::RasterResizeFlags flags)
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        // Take the current resource and clone it, then apply the changes.
        IResourceBase *pResourceClone = pResource->Clone();
        IEditRasterResource *perr;
        if (pResourceClone && pResourceClone->QueryCapability(IID_PPV_ARGS(&perr)))
        {
            int i = 0;
            for (; i < cCels; i++)
            {
                perr->SetSize(rgdwIndex[i], rgSizes[i], flags);
            }
            LPARAM lHint = VIEWUPDATEHINT_LOOPCHANGED;
            if ((cCels == 1 && (rgdwIndex[i] == GetSelectedIndex())))
            {
                lHint = VIEWUPDATEHINT_CELCHANGED; // Turns out only the selected cel changed.
            }
            _NewResource(pResourceClone, lHint);
        }
    }
}


void CRasterResourceDocument::SetGroupTransparentColor(int cCels, DWORD *rgdwIndex, BYTE bTransparent)
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        // Take the current resource and clone it, then apply the changes.
        IResourceBase *pResourceClone = pResource->Clone();
        IEditRasterResource *perr;
        if (pResourceClone && pResourceClone->QueryCapability(IID_PPV_ARGS(&perr)))
        {
            int i = 0;
            for (; i < cCels; i++)
            {
                perr->SetTransparentColor(rgdwIndex[i], bTransparent);
            }
            LPARAM lHint = VIEWUPDATEHINT_LOOPCHANGED;
            if ((cCels == 1 && (rgdwIndex[i] == GetSelectedIndex())))
            {
                lHint = VIEWUPDATEHINT_CELCHANGED; // Turns out only the selected cel changed.
            }
            _NewResource(pResourceClone, lHint);
        }
    }
}




