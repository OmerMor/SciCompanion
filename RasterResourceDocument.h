#pragma once

#include "ResourceDocument.h"
#include "UndoResource.h"
#include "RasterResource.h"

//
// Raster resource (view, font, cursor) view updates
//
// A new cel was selected
#define VIEWUPDATEHINT_CELSELECTIONCHANGED  0x00000001
// A new loop was selected
#define VIEWUPDATEHINT_LOOPSELECTIONCHANGED 0x00000002
// An aspect of the cel changed. The CObject is a CCelDescription.  This could apply to all loops (cel changed
//  for all loops, because of mirroring).
#define VIEWUPDATEHINT_CELCHANGED           0x00000004
// An aspect of the loop changed. The CObject is a CCelDescription.
#define VIEWUPDATEHINT_LOOPCHANGED          0x00000008
// A new view resource was set on the document (could be because of undo/redo)
#define VIEWUPDATEHINT_NEWVIEW              0x00000010
// The user chose a new colour.
#define VIEWUPDATEHINT_COLORCHANGED         0x00000020
// The user chose a new pen width.
#define VIEWUPDATEHINT_PENWIDTHCHANGED      0x00000040
// New sample text (font)
#define VIEWUPDATEHINT_SAMPLETEXTCHANGED    0x00000080
// The "apply to all" flag was changed
#define VIEWUPDATEHINT_APPLYTOALLCHANGED    0x00000100

// CRasterResourceDocument document

class CRasterResourceDocument : public CUndoResource<CResourceDocument, IResourceBase>, public IEditRasterResourceDoc, public IEditRasterResource
{
public:
    CRasterResourceDocument();
    virtual ~CRasterResourceDocument();
    void SetResource(IResourceBase *pResource, int id = -1);

    BOOL QueryCapability(const GUID guid, void **ppv)
    {
        BOOL fRet = FALSE;
        if (IsEqualGUID(guid, __uuidof(IEditRasterResourceDoc)))
        {
            fRet = TRUE;
            *ppv = static_cast<IEditRasterResourceDoc*>(this);
        }
        else if (IsEqualGUID(guid, __uuidof(IEditRasterResource)))
        {
            fRet = TRUE;
            *ppv = static_cast<IEditRasterResource*>(this);
        }
        else
        {
            fRet = __super::QueryCapability(guid, ppv);
        }
        return fRet;
    }

    // IEditRasterResourceDoc (partial implementation)
    void LockResource(bool fLock) { _fLocked = fLock; }
    int GetPenWidth() { return _nPenWidth; }
    void SetPenWidth(int nPenWidth);
    void SetApplyToAllCels(BOOL fApply) { _fApplyToAllCels = fApply; UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_APPLYTOALLCHANGED); }
    BOOL GetApplyToAllCels() const { return _fApplyToAllCels; }
    void SetGroupBitmapData(int cCels, DWORD *rgdwIndex, const BYTE* const* ppData);
    void SetGroupPlacement(int cCels, DWORD *rgdwIndex, CPoint pt);
    void SetGroupSize(int cItems, DWORD *rgdwIndex, const CSize *rgSizes, RasterBitmap::RasterResizeFlags flags);
    void SetGroupTransparentColor(int cItems, DWORD *rgdwIndex, BYTE bTransparent);

    // IEditRasterResource (partial implementation)
    BOOL SetSize(DWORD dwIndex, CSize size, RasterBitmap::RasterResizeFlags flags);
    void SetBitmapData(DWORD dwIndex, const BYTE *pBits);
    void SetPlacement(DWORD dwIndex, CPoint pt);
    void MoveFromTo(DWORD dwIndexFrom, DWORD dwIndexTo);

    bool v_PreventUndos() { return _fLocked; }
    void v_OnUndoRedo();

protected:
    // Ensure the currently selected cel index is correct.
    virtual void _ValidateCelIndex() = 0;

    void _TrimUndoStack();
    void _NewResource(IResourceBase *pResource, LPARAM lHint);
    std::list<IResourceBase*>::iterator _GetLastUndoFrame();
    
	DECLARE_MESSAGE_MAP()

    bool _fLocked;
    int _nPenWidth;
    BOOL _fApplyToAllCels;
};
