
#pragma once

class __declspec(uuid("c1439d6b-42e8-4602-a347-b1f8e4299d43")) IRasterResource
{
public:
    virtual HBITMAP GetBitmap(DWORD dwIndex, int cx, int cy) const = 0;
    // "fast bitmap" implies as quick as possible, padding/cropping instead of zooming to account for cx, cy
    virtual HBITMAP GetFastBitmap(DWORD dwIndex, int cx, int cy) const = 0;
    virtual void CopyBitmapData(DWORD dwIndex, BYTE *pData, CSize sizeDebug) const = 0;
    virtual CSize GetSize(DWORD dwIndex) const = 0;
    virtual BYTE GetTransparentColor(DWORD dwIndex) const = 0;
    virtual CPoint GetPlacement(DWORD dwIndex) const = 0;
    virtual DWORD GetPreviewIndex() const { return 0; }
};

class RasterBitmap
{
public:
    enum RasterResizeFlags
    {
        Normal,
        AnchorBottomRight,
        Stretch,
    };

    RasterBitmap() { _cx = 0; _cy = 0; }
    RasterBitmap(const RasterBitmap &src)
    {
        _cx = src._cx;
        _cy = src._cy;
        _bits = src._bits;
    }
    RasterBitmap& operator=(const RasterBitmap& src)
    {
        if (&src != this)
        {
            _cx = src._cx;
            _cy = src._cy;
            _bits = src._bits;
        }
        return *this;
    }
    const WORD &width() const { return _cx; }
    const WORD &height() const { return _cy; }
    WORD &width() { return _cx; }
    WORD &height() { return _cy; }
    void width(WORD w) { _cx = w; }
    void height(WORD w) { _cy = w; }
    const BYTE *get_bits() const { return &_bits[0]; }
    BYTE *get_bits() { return &_bits[0]; }
    void set_size(WORD cx, WORD cy)
    {
        _cx = cx;
        _cy = cy;
        _bits.resize(CX_ACTUAL(_cx) * _cy);
    }

    size_t datasize() const { return CX_ACTUAL(_cx) * _cy; }
    void SetData(const BYTE *pData)
    {
        _bits.assign(pData, pData + datasize());
    }
    void create_degenerate(BYTE bColor)
    {
        // Dummy 1x1 cel 
        _cx = 1;
        _cy = 1;
        _bits.assign(datasize(), bColor);
    }
    void ReallocBits(WORD cx, WORD cy, bool fCopy, bool fForce, bool fFill, BYTE bColor, RasterResizeFlags dwResizeFlags);

protected:
    void _blank_fill()
    {
        _bits.assign(datasize(), 0);
    }
    void _fill(size_t pos, BYTE bValue, int cAmount);
    virtual void _ClampSize(WORD &cx, WORD &cy) = 0;
private:
    WORD _cx;
    WORD _cy;
    std::vector<BYTE> _bits;
};

class __declspec(uuid("2fc92e46-d1aa-4101-8fd2-e372784bbaa5")) IEditRasterResource
{
public:
    enum Capabilities
    {
        TransparentColor    = 0x00000001,
        Placement           = 0x00000002,
        Resize              = 0x00000004,
        Reorder             = 0x00000008,
    };
    virtual void SetBitmapData(DWORD dwIndex, const BYTE *pData) = 0;
    // This needs to be robust to invalid sizes (the client doesn't know what valid sizes are).
    virtual BOOL SetSize(DWORD dwIndex, CSize size, RasterBitmap::RasterResizeFlags flags) = 0;
    virtual void SetTransparentColor(DWORD dwIndex, BYTE bTransparent) = 0;
    virtual void SetPlacement(DWORD dwIndex, CPoint pt) = 0;
    virtual Capabilities GetRasterCaps() = 0;
    virtual void MoveFromTo(DWORD dwIndexFrom, DWORD dwIndexTo) = 0;
};

class __declspec(uuid("8dae00a8-9710-46ef-b872-e4f0444a453d")) IEditRasterResourceDoc
{
public:
    enum OriginStyle
    {
        OriginBottomCenter,
        OriginTopLeft,
        OriginNA,
    };
    virtual void LockResource(bool fLock) = 0;
    virtual DWORD GetSelectedIndex() = 0;
    virtual void SetApplyToAllCels(BOOL fApply) = 0;
    virtual BOOL GetApplyToAllCels() const = 0;
    virtual void SetGroupBitmapData(int cItems, DWORD *rgdwIndex, const BYTE* const* ppData) = 0;
    virtual void SetGroupPlacement(int cItems, DWORD *rgdwIndex, CPoint pt) = 0;
    virtual void SetGroupSize(int cItems, DWORD *rgdwIndex, const CSize *rgSizes, RasterBitmap::RasterResizeFlags flags) = 0;
    virtual void SetGroupTransparentColor(int cItems, DWORD *rgdwIndex, BYTE bTransparent) = 0;
    virtual int GetSelectedGroup(DWORD *rgGroups, size_t ceGroup)
    {
        // Gets the  group indices of the selected cels in this resource. (e.g. all the cels of one view loop, if
        // drawing is applied to all cels).  By default, this is just the single selected cel.
        ASSERT(ceGroup > 0);
        if (ceGroup > 0)
        {
            *rgGroups = GetSelectedIndex();
            return 1;
        }
        else
        {
            return 0;
        }
    }
    virtual void MoveSelectedCel(CPoint point) = 0;
    virtual EGACOLOR GetViewColor() = 0;
    virtual void SetViewColor(EGACOLOR color) = 0;
    virtual int GetPenWidth() = 0;
    virtual void SetPenWidth(int nPenWidth) = 0;
    virtual void RemoveCel(DWORD dwIndex) = 0;
    // Only color1 of each colour is used:
    virtual void GetPaletteAndColours(const EGACOLOR **prgPalette, int *pcRows, int *pcColumns) = 0;
    virtual int GetDefaultZoom() const = 0;
    virtual void SetDefaultZoom(int iZoom) const = 0;
    virtual BOOL CanDeleteCels() const = 0;
    virtual OriginStyle GetOriginStyle() const = 0;
};
