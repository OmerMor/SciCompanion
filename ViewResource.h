//
// ViewResource implements a runtime version of a view resource.
//

#pragma once

#include "ResourceBase.h"
#include "RasterResource.h"
#include "CelResource.h"

// FWD declaration
class ResourceBlob;

class __declspec(uuid("f70416b8-f3ec-48aa-be3b-9ca6e6ceb97b")) IViewResource : public ICelResource
{
public:
    virtual int GetLoopCount() const = 0;
    virtual int GetMirrorOf(int nLoop) const = 0;
    virtual HRESULT InsertCel(int nLoop, int nCel, BOOL fBefore) = 0;
    virtual HRESULT InsertLoop(int nLoop, BOOL fBefore) = 0;
    virtual HRESULT RemoveLoop(int nLoop) = 0;
    virtual HRESULT MakeMirrorOf(int nLoop, int nOriginal) = 0;
    virtual HRESULT RemoveCel(DWORD dwIndex) = 0;
};

class ViewResource : public IResourceBase, public IRasterResource, public IViewResource, public IEditRasterResource
{
private:
    class CelInfo : public RasterBitmap
    {
    public:
        CelInfo();
        CelInfo(const CelInfo &src);
        CelInfo& operator=(const CelInfo& src);
        bool read(sci::istream &byteStream);
        void copy_mirrored(const CelInfo &celOrig);
        BYTE transparent() const { return _bTransparent; }
        void transparent(BYTE b) { _bTransparent = b; }
        char x_placement() const { return _xPlacement; }
        char y_placement() const { return _yPlacement; }
        void x_placement(char b) { _xPlacement = b; }
        void y_placement(char b) { _yPlacement = b; }
        void write_cel(sci::istream *pSerial) const;
        HBITMAP get_bitmap(int cx, int cy, bool fAllowZoom) const;
        void copy_bitmap_data(BYTE *pData, CSize sideDebug) const;
        CSize get_size() const { return CSize(width(), height()); }
        CPoint get_placement() const { return CPoint(_xPlacement, _yPlacement); }
        static CelInfo InheritFromTemplate(const CelInfo &celTemplate);

    private:
        void _ClampSize(WORD &cx, WORD &cy)
        {
            cx = min(320, cx);
            cx = max(1, cx);
            cy = min(190, cy);
            cy = max(1, cy);
        };
        bool _read_imagedata(sci::istream &byteStream);
        BYTE _bTransparent;
        char _xPlacement;
        char _yPlacement;
    };

    class LoopInfo
    {
    public:
        typedef std::vector<CelInfo> cel_container;

        LoopInfo();
        LoopInfo(const CelInfo &src);
        LoopInfo(const LoopInfo &src);
        LoopInfo& operator=(const LoopInfo& src);
        bool read(sci::istream &byteStream);
        int size() const;
        CelInfo &operator[](int iIndex);
        const CelInfo &operator[](int iIndex) const;
        const CelInfo &at(int iIndex) const;
        bool mirror() const { return _fMirror; }
        void mirror(bool fMirror) { _fMirror = fMirror; }
        BYTE mirror_of() const { return _bMirrorOf; }
        void mirror_of(BYTE b) { _bMirrorOf = b; }
        void move_from_to(int celFrom, int celTo);
        void insert_cel(int nCel, const CelInfo &cel);
        void remove_cel(int nCel);
        void mirror_from(BYTE nOriginal, const LoopInfo &loopOrig);
        void write_loop(sci::istream *pSerial) const;

    private:
        cel_container _cels;
        WORD _wDummy;
        bool _fMirror;
        BYTE _bMirrorOf;
    };

public:
    ViewResource();

    // this for existing ones.
    HRESULT InitFromResource(const ResourceBlob *prd);
    // This for new ones
    HRESULT InitNew();

    // IRasterResource
    HBITMAP GetBitmap(DWORD dwIndex, int cx, int cy) const;
    HBITMAP GetFastBitmap(DWORD dwIndex, int cx, int cy) const;
    void CopyBitmapData(DWORD dwIndex, BYTE *pData, CSize sizeDebug) const;
    CSize GetSize(DWORD dwIndex) const;
    BYTE GetTransparentColor(DWORD dwIndex) const;
    CPoint GetPlacement(DWORD dwIndex) const;

    // IResourceBase
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
        else if (IsEqualGUID(guid, __uuidof(IViewResource)))
        {
            fRet = TRUE;
            *ppv = static_cast<IViewResource*>(this);
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
    ResourceType GetType() const { return RS_VIEW; }

    BOOL Serialize(sci::istream *pSerial) const;

    // IViewResource
    int GetLoopCount() const { return (int)_loops.size(); }
    int GetCelCount(int nLoop = 0) const { return _loops[nLoop].size(); }
    int GetMirrorOf(int nLoop) const;
    HRESULT InsertCel(int nLoop, int nCel, BOOL fBefore);
    HRESULT InsertLoop(int nLoop, BOOL fBefore);
    HRESULT RemoveLoop(int nLoop);
    HRESULT MakeMirrorOf(int nLoop, int nOriginal);
    HRESULT RemoveCel(DWORD dwIndex);

    // IEditRasterResource
    void SetBitmapData(DWORD dwIndex, const BYTE *pData);
    BOOL SetSize(DWORD dwIndex, CSize size, RasterBitmap::RasterResizeFlags flags);
    void SetTransparentColor(DWORD dwIndex, BYTE bTransparent);
    void SetPlacement(DWORD dwIndex, CPoint pt);
    void MoveFromTo(DWORD dwIndexFrom, DWORD dwIndexTo);
    Capabilities GetRasterCaps()
    {
        return static_cast<Capabilities>(TransparentColor | Placement | Resize | Reorder);
    }

private:
    CelInfo &_GetCel(DWORD dwIndex);
    const CelInfo &_GetCel(DWORD dwIndex) const;
    bool _ValidIndex(DWORD dwIndex) const;
    bool _ValidIndex(int nLoop, int nCel) const;
    CelInfo &_GetCel(int nLoop, int nCel);
    void _UpdateMirrors(DWORD dwIndex);
    void _UpdateMirrors(int nLoop, int nCel);
    void _UpdateMirrorsWholeLoop(int nLoop);
    HBITMAP _GetBitmapI(DWORD dwIndex, int cx, int cy, BOOL fAllowZoom) const;
    bool _IsLastLoop(int nLoop);
    static void _SyncCelMirrorState(const CelInfo &celOrig, CelInfo &celMirror);

    // Information about the file itself
    int _iResourceNumber;
    int _iPackageNumber;

    // Information about the view
    std::vector<LoopInfo> _loops;
    DWORD _dwDummy;
};

