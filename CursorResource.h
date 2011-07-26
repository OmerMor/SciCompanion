//
// CView implements a runtime version of a view resource.
//

#pragma once

#include "ResourceBase.h"
#include "RasterResource.h"
#include "CelResource.h"

// FWD declaration
class ResourceBlob;

class CCursorResource : public IResourceBase, public IRasterResource, public ICelResource, public IEditRasterResource
{
public:
    CCursorResource();

    // This is for existing ones
    HRESULT InitFromResource(const ResourceBlob *prd);

    // IResourceBase    
    HRESULT InitNew() { return S_OK; }
    BOOL Serialize(sci::istream *pSerial) const;
    void SetResourceNumber(int iNum) { _iResourceNumber = iNum; }
    void SetPackageNumber(int iNum) { _iPackageNumber = iNum; }
    int GetResourceNumber() const { return _iResourceNumber; }
    int GetPackageNumber() const { return _iPackageNumber; }
    BOOL QueryCapability(const GUID guid, void **ppv)
    {
        BOOL fRet = FALSE;
        if (IsEqualGUID(guid, __uuidof(IResourceBase)))
        {
            fRet = TRUE;
            *ppv = static_cast<IResourceBase*>(this);
        }
        else if (IsEqualGUID(guid, __uuidof(IRasterResource)))
        {
            fRet = TRUE;
            *ppv = static_cast<IRasterResource*>(this);
        }
        else if (IsEqualGUID(guid, __uuidof(ICelResource)))
        {
            fRet = TRUE;
            *ppv = static_cast<ICelResource*>(this);
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
    ResourceType GetType() const { return RS_CURSOR; }

    // IRasterResource
    HBITMAP GetBitmap(DWORD dwIndex, int cx, int cy) const;
    HBITMAP GetFastBitmap(DWORD dwIndex, int cx, int cy) const;
    void CopyBitmapData(DWORD dwIndex, BYTE *pData, CSize sizeDebug) const;
    CSize GetSize(DWORD dwIndex) const;
    BYTE GetTransparentColor(DWORD dwIndex) const { return CursorTransparent; }
    CPoint GetPlacement(DWORD dwIndex) const { return CPoint(_xPlacement, _yPlacement); }

    // IEditRasterResource
    void SetBitmapData(DWORD dwIndex, const BYTE *pData);
    BOOL SetSize(DWORD dwIndex, CSize size, RasterBitmap::RasterResizeFlags flags);
    void SetTransparentColor(DWORD dwIndex, BYTE bTransparent) {}
    void SetPlacement(DWORD dwIndex, CPoint pt);
    void MoveFromTo(DWORD dwIndexFrom, DWORD dwIndexTo) {}  // Cursors don't support this.
    Capabilities GetRasterCaps() { return Placement; } // No resize or transparent colours

    // ICelResource
    int GetCelCount(int nLoop = 0) const { return 1; } // Only 1 cel

private:
    // The EGA color we use for transparency (aqua)
    static const BYTE CursorTransparent = 0x03;

    // Information about the file itself
    int _iResourceNumber;
    int _iPackageNumber;

    WORD _xPlacement;
    WORD _yPlacement;

    // 16 x 16 bitmap, in visual colours.
    // 0x0f: white
    // 0x00: black
    // 0x03: transparent
    // other values are illegal
    BYTE _rgData[256]; 
};


