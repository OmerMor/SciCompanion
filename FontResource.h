//
// FontResource implements an SCI font resource.
//

#pragma once

#include "ResourceBase.h"
#include "RasterResource.h"
#include "CelResource.h"

// FWD declaration
class ResourceBlob;

class __declspec(uuid("789a38e5-6c16-4f27-91c8-61b435a3f22b")) IFontResource
{
public:
    virtual WORD GetLineHeight() const = 0;
    virtual void SetLineHeight(WORD cy) = 0;
};

class CFontResource : public IResourceBase, public IRasterResource, public ICelResource, public IEditRasterResource, public IFontResource
{
public:
    CFontResource();

    // This is for existing ones
    HRESULT InitFromResource(const ResourceBlob *prd);
    // For in-memory bit streams
    HRESULT InitFromData(const BYTE *pData, INT_PTR cbSize);
    // This for ones created from a gdi font
    HRESULT InitFromLOGFONT(const LOGFONT *pLogFont);

    // IResourceBase    
    HRESULT InitNew();
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
        else if (IsEqualGUID(guid, __uuidof(IFontResource)))
        {
            fRet = TRUE;
            *ppv = static_cast<IFontResource*>(this);
        }
        else
        {
            fRet = __super::QueryCapability(guid, ppv);
        }
        return fRet;
    }
    ResourceType GetType() const { return RS_FONT; }

    // IRasterResource
    HBITMAP GetBitmap(DWORD dwIndex, int cx, int cy) const;
    HBITMAP GetFastBitmap(DWORD dwIndex, int cx, int cy) const;
    void CopyBitmapData(DWORD dwIndex, BYTE *pData, CSize sizeDebug) const;
    CSize GetSize(DWORD dwIndex) const;
    BYTE GetTransparentColor(DWORD dwIndex) const { return 0x0f; } // White
    CPoint GetPlacement(DWORD dwIndex) const { return CPoint(0, 0); }
    virtual DWORD GetPreviewIndex() const { return 65 + 32; } // Small letter 'a'

    // IEditRasterResource
    virtual void SetBitmapData(DWORD dwIndex, const BYTE *pData);
    virtual BOOL SetSize(DWORD dwIndex, CSize size, RasterBitmap::RasterResizeFlags flags);
    virtual void SetTransparentColor(DWORD dwIndex, BYTE bTransparent) {}
    virtual void SetPlacement(DWORD dwIndex, CPoint pt) {}
    virtual void MoveFromTo(DWORD dwIndexFrom, DWORD dwIndexTo) {}  // Fonts don't support this.
    virtual Capabilities GetRasterCaps() { return Resize; } // No placement or transparent colours

    // Not can you remove letters
    virtual HRESULT RemoveCel(DWORD dwIndex) { return E_NOTIMPL; }

    // ICelResource
    virtual int GetCelCount(int nLoop = 0) const { return (int)_cChars; } // Ignore nLoop

    // IFontResource
    WORD GetLineHeight() const { return _cyLine; }
    void SetLineHeight(WORD cyLine) { _cyLine = cyLine; }

private:
    class LetterInfo : public RasterBitmap
    {
    public:
        BYTE bwidth() const { return static_cast<BYTE>(width()); }
        BYTE bheight() const { return static_cast<BYTE>(height()); }
    protected:
        virtual void _ClampSize(WORD &cx, WORD &cy)
        {
            cx = min(127, cx);
            cx = max(1, cx);
            cy = min(127, cy);
            cy = max(1, cy);
        }
    };

    BOOL _WriteLetter(sci::istream *pSerial, WORD iChar) const;
    void _RenderLetter(CDC *pDC, LetterInfo *pli, TCHAR ch, BOOL fCalc, WORD *pwMaxCharHeight, int *piAbove, int *piBelow) const;
    void _ReadLetter(sci::istream &byteStream, LetterInfo &letter);
    void _ValidateSize(CSize *pSize);

    // Information about the file itself
    int _iResourceNumber;
    int _iPackageNumber;

    std::vector<LetterInfo> _rgLetters;
    WORD _cChars;
    WORD _cyLine;
};
