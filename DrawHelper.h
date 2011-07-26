
#pragma once

//
// Provides an easy way to use GDI drawing commands directly on
// SCI bitmaps 
//
// Usage:
// {
//      CSCIDrawHelper helper(NULL, _pData, size);
//      SomeGdiFunction(helper.dc, param, param, param);
// }
// pData must be at least (CX_ACTUAL(size.cx) * size.cy) bytes
//
class CSCIDrawHelper
{
public:
    CSCIDrawHelper(CDC *pdc, BYTE *pData, const CSize &size);
    ~CSCIDrawHelper();

    // Public member for folks to use to draw into.
    CDC dc;

private:
    HBITMAP _hDib;
    void *_pBits;       // Our buffer.  This belongs to the _hDib, which we delete.
    BYTE *_pData;       // The buffer we affect in the end.  We don't delete this.
    HGDIOBJ _hgdiObjOld;
    CSize _size;
};
