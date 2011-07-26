#pragma once

// fwd decl
class PicResource;

class PicDrawManager
{
public:
    PicDrawManager(const PicResource *pPic = NULL);
    void SetPic(const PicResource *pPic);
    const PicResource *GetPic() const { return _pPicWeak; }

    HBITMAP CreateVisualBitmap(int cx, int cy);
    HBITMAP CreatePriorityBitmap(int cx, int cy);
    HBITMAP CreateControlBitmap(int cx, int cy);
    const BYTE *GetPriorityBits();
    const BYTE *GetControlBits();
    const BYTE *GetVisualBits();
    void CopyBitmap(DWORD dwDrawEnable, BYTE *pdataDisplay, BYTE *pdataAux, BITMAPINFO **ppbmi);
    void SetPalette(BYTE bPaletteNumber);
    BYTE GetPalette() { return _bPaletteNumber; }
    const SCIPicState *GetPicState();
    SCIPicState *ModifyPicState();
    bool SeekToPos(INT_PTR iPos); // true if changed
    INT_PTR PosFromPoint(int x, int y, INT_PTR iStart);
    INT_PTR GetPos() const { return _iInsertPos; }
    void SetPreview(bool fPreview);
    void Invalidate();

    // Call this if you know you're going to obtain multiple screens right away
    void RefreshAllScreens();

private:
    void _Reset();
    void _Init();
    void _PrepVisualBuffer();
    INT_PTR _GetDrawPos() const { return _iDrawPos; }
    void _EnsureBitmaps(DWORD dwEnable);
    void _RedrawBuffers(SCIPicState *pState, DWORD dwRequestedMaps, INT_PTR iPos, BOOL fForce);
    HBITMAP _CreateBitmap(BYTE *pData, int cx, int cy) const;
    HBITMAP _GetBitmapGDIP(BYTE *pData, int cx, int cy) const;
    void _EnsureCorrectState();
    void _OnPosChanged(bool fNotify = true);
    BITMAPINFO *CreateSCIBMI() const;

    const PicResource *_pPicWeak;

    scoped_array<BYTE> _dataVisual;
    scoped_array<BYTE> _dataPriority;
    scoped_array<BYTE> _dataControl;
    scoped_array<BYTE> _dataAux;

    // Are the bitmaps valid? (note, if any of these are valid, then the aux is valid too)
    bool _fValidVisual; 
    bool _fValidPriority;
    bool _fValidControl;
    bool _fValidPalette;
    BYTE _bPaletteNumber; // 0-3: the palette number in which to draw.
    bool _fValidState; // Is picstate valid?

    // command to which the buffers have been drawn. (-1 for all the way)
    // It does NOT include _pCommand(_iDrawPos).  So if _iValidPos is 0, then
    // nothing is drawn.
    INT_PTR _iDrawPos; 

    // Expose this, and adjust them whenever a position
    // is seeked.
    SCIPicState _currentState;

    INT_PTR _iInsertPos;
};