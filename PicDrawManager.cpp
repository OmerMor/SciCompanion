#include "stdafx.h"
#include "SCIPicEditor.h"
#include "PicDrawManager.h"
#include "PicResource.h"

PicDrawManager::PicDrawManager(const PicResource *pPic) : _pPicWeak(pPic)
{
    _Reset();
}

void PicDrawManager::_Reset()
{
    _fValidVisual = false; 
    _fValidPriority = false;
    _fValidControl = false;
    _iDrawPos = -1;
    _fValidPalette = false;
    _fValidState = false;
    _bPaletteNumber = 0;
    _currentState.Reset(_bPaletteNumber);
    _iInsertPos = -1;
}

void PicDrawManager::SetPic(const PicResource *pPic)
{
    if (!pPic->IsSame(_pPicWeak))
    {
        _Reset();
    }
    _pPicWeak = pPic;
}

//
// Caller needs to free HBITMAP
//
HBITMAP PicDrawManager::CreateVisualBitmap(int cx, int cy)
{
    HBITMAP hbm = NULL;
    if (!_fValidVisual)
    {
        _RedrawBuffers(NULL, DRAW_ENABLE_VISUAL, _GetDrawPos(), false);
    }

    if (_fValidVisual)
    {
        hbm = _CreateBitmap(_dataVisual.get(), cx, cy);
    }
    return hbm;
}
HBITMAP PicDrawManager::CreatePriorityBitmap(int cx, int cy)
{
    HBITMAP hbm = NULL;
    if (!_fValidPriority)
    {
        _RedrawBuffers(NULL, DRAW_ENABLE_PRIORITY, _GetDrawPos(), false);
    }
    if (_fValidPriority)
    {
        hbm = _CreateBitmap(_dataPriority.get(), cx, cy);
    }
    return hbm;
}
HBITMAP PicDrawManager::CreateControlBitmap(int cx, int cy)
{
    HBITMAP hbm = NULL;
    if (!_fValidControl)
    {
        _RedrawBuffers(NULL, DRAW_ENABLE_CONTROL, _GetDrawPos(), false);
    }
    if (_fValidControl)
    {
        hbm = _CreateBitmap(_dataControl.get(), cx, cy);
    }
    return hbm;
}



const BYTE *PicDrawManager::GetPriorityBits()
{
    BYTE *pdata = NULL;
    if (!_fValidPriority)
    {
        _RedrawBuffers(NULL, DRAW_ENABLE_PRIORITY, _GetDrawPos(), false);
    }

    if (_fValidPriority)
    {
        pdata = _dataPriority.get();
    }
    return pdata;
}

const BYTE *PicDrawManager::GetControlBits()
{
    BYTE *pdata = NULL;
    if (!_fValidControl)
    {
        _RedrawBuffers(NULL, DRAW_ENABLE_CONTROL, _GetDrawPos(), false);
    }

    if (_fValidControl)
    {
        pdata = _dataControl.get();
    }
    return pdata;
}

const BYTE *PicDrawManager::GetVisualBits()
{
    BYTE *pdata = NULL;
    if (!_fValidVisual)
    {
        _RedrawBuffers(NULL, DRAW_ENABLE_VISUAL, _GetDrawPos(), false);
    }

    if (_fValidVisual)
    {
        pdata = _dataVisual.get();
    }
    return pdata;
}


//
// Copies one of our bitmap (specified by dwDrawEnable) into pdataDisplay,
// and the aux bitmap into pdataAux.  So callers can take what we have,
// and add things.
// dwDrawEnable can specify only one of the flags.
// Caller must delete the optional parameter *ppbmi
//
void PicDrawManager::CopyBitmap(DWORD dwDrawEnable, BYTE *pdataDisplay, BYTE *pdataAux, BITMAPINFO **ppbmi)
{
    *ppbmi = NULL;
    switch (dwDrawEnable)
    {
    case DRAW_ENABLE_VISUAL:
        if (!_fValidVisual)
        {
            _RedrawBuffers(&_currentState, DRAW_ENABLE_VISUAL, _GetDrawPos(), false);
        }
        if (_fValidVisual)
        {
            CopyMemory(pdataDisplay, _dataVisual.get(), BMPSIZE);
            if (ppbmi)
            {
                *ppbmi = CreateSCIBMI();
            }
        }
        break;

    case DRAW_ENABLE_PRIORITY:
        if (!_fValidPriority)
        {
            _RedrawBuffers(NULL, DRAW_ENABLE_PRIORITY, _GetDrawPos(), false);
        }
        if (_fValidPriority)
        {
            CopyMemory(pdataDisplay, _dataPriority.get(), BMPSIZE);
            if (ppbmi)
            {
                *ppbmi = new SCIBitmapInfo(sPIC_WIDTH, sPIC_HEIGHT);
            }
        }
        break;

    case DRAW_ENABLE_CONTROL:
        if (!_fValidControl)
        {
            _RedrawBuffers(NULL, DRAW_ENABLE_CONTROL, _GetDrawPos(), false);
        }
        if (_fValidControl)
        {
            CopyMemory(pdataDisplay, _dataControl.get(), BMPSIZE);
            if (ppbmi)
            {
                *ppbmi = new SCIBitmapInfo(sPIC_WIDTH, sPIC_HEIGHT);
            }
        }
        break;

    default:
        // We can only be called with ONE of these things.
        ASSERT(false);
        break;
    }
    if (_dataAux.get()) // Could be some strange scenarios where _pPicWeak is NULL
    {
        CopyMemory(pdataAux, _dataAux.get(), BMPSIZE);
    }
}

void PicDrawManager::SetPalette(BYTE bPaletteNumber)
{
    if (bPaletteNumber != _bPaletteNumber)
    {
        _bPaletteNumber = bPaletteNumber;
        _fValidPalette = false; // Since the palette changed.
        _fValidVisual = false;  // And of course, the visual map is now invalid
        _fValidControl = false; // And since the palette could change to/from colors with white, the control
        _fValidPriority = false;// and priority screens could change too.

        _fValidState = false;
    }
}


void PicDrawManager::_PrepVisualBuffer()
{
    memset(_dataVisual.get(), 0x0f, BMPSIZE);
}

//
// Redraw our visual buffers, with
// - dwRequestedMaps (DRAW_ENABLE_VISUAL, DRAW_ENABLE_PRIORITY, DRAW_ENABLE_CONTROL)
// - iPos: all the way to iPos (-1 if entire thing)
// - fForce: override our logic that says "oh, this is already drawn", and draw anyway.
//
void PicDrawManager::_RedrawBuffers(SCIPicState *pState, DWORD dwRequestedMaps, INT_PTR iPos, BOOL fForce)
{
    if (_pPicWeak == NULL)
    {
        return;
    }
    CPrecisionTimer timer;
    timer.Start();

    _EnsureBitmaps(dwRequestedMaps);
    SCIPicState state(_bPaletteNumber);
    if (pState == NULL)
    {
        pState = &state;
    }
    else
    {
        pState->Reset(_bPaletteNumber);
    }

    // Clear out our cached bitmaps.
    BOOL fDoSomething = false;

    DWORD dwMapsToRedraw = 0;
    if ((dwRequestedMaps & DRAW_ENABLE_PRIORITY) && (!_fValidPriority || fForce))
    {
        memset(_dataPriority.get(), 0x00, BMPSIZE);
        fDoSomething = true;
        dwMapsToRedraw |= DRAW_ENABLE_PRIORITY;
        if (!_fValidPriority)
        {
            // We'll need to redraw the visual too, since it is needed for accurate
            // fills.
            _fValidVisual = false;
            dwMapsToRedraw |= DRAW_ENABLE_VISUAL;
        }
    }
    if ((dwRequestedMaps & DRAW_ENABLE_CONTROL) && (!_fValidControl || fForce))
    {
        memset(_dataControl.get(), 0x00, BMPSIZE);
        fDoSomething = true;
        dwMapsToRedraw |= DRAW_ENABLE_CONTROL;
        if (!_fValidControl)
        {
            // We'll need to redraw the visual too (even if it is "valid"), since it is needed for accurate
            // fills.
            _fValidVisual = false;
        }
    }
    if (!_fValidVisual || fForce) // Ignore dwRequestedMaps here - since we always need a valid visual map (for fill)
    {
        // We'll prep this one later on.
        fDoSomething = true;
        dwMapsToRedraw |= DRAW_ENABLE_VISUAL;
    }

    // Call this again, in case we added in some more maps.
    _EnsureBitmaps(dwMapsToRedraw);

    // Draw blank background on visual buffer
    if (dwMapsToRedraw & DRAW_ENABLE_VISUAL)
    {
        _PrepVisualBuffer();
    }
   
    if (fDoSomething)
    {
        // Prepare the auxiliary bitmap, if it hadn't been used before
        memset(_dataAux.get(), 0x00, BMPSIZE);

        PicData data =
        {
            dwMapsToRedraw,
            _dataVisual.get(), // Visual always needs to be provided (for fill)
            (dwMapsToRedraw & DRAW_ENABLE_PRIORITY) ? _dataPriority.get() : NULL,
            (dwMapsToRedraw & DRAW_ENABLE_CONTROL) ? _dataControl.get() : NULL,
            _dataAux.get(), // Aux always needs to be provided (for fill)
        };

        // Now draw!
        _pPicWeak->Draw(data, *pState, iPos);

        // Use dwMapsToRedraw here, instead of dwRequestedMaps.
        _fValidVisual = _fValidVisual || (dwMapsToRedraw & DRAW_ENABLE_VISUAL);
        _fValidPriority = _fValidPriority || (dwMapsToRedraw & DRAW_ENABLE_PRIORITY);
        _fValidControl = _fValidControl || (dwMapsToRedraw & DRAW_ENABLE_CONTROL);
        _fValidPalette = true;

        // SCIPicState is valid if all things have been redrawn.
        _fValidState = _fValidPriority && _fValidVisual && _fValidControl;
        _iDrawPos = iPos;
    }

#ifdef DEBUG
    // Print time it took to render the picture.
    TCHAR szBuf[MAX_PATH];
    StringCchPrintf(szBuf, ARRAYSIZE(szBuf), TEXT("Time: %03d %f\n"), _pPicWeak->GetResourceNumber(), timer.Stop());
    OutputDebugString(szBuf);
#endif
}


HBITMAP PicDrawManager::_GetBitmapGDIP(BYTE *pData, int cx, int cy) const
{
    HBITMAP hbm = NULL;
    std::auto_ptr<BITMAPINFO> pbmi(CreateSCIBMI());
    std::auto_ptr<Gdiplus::Bitmap> pimgVisual(Gdiplus::Bitmap::FromBITMAPINFO(pbmi.get(), pData));
    if (pimgVisual.get())
    {
        if ((cx == sPIC_WIDTH) && (cy == sPIC_HEIGHT))
        {
            // Exact size.
            pimgVisual->GetHBITMAP(Color::Black, &hbm);
        }
        else
        {
            // Make a different sized version
            // Note: this cast to (Bitmap*) is undocumented.
            std::auto_ptr<Bitmap> pimage((Bitmap*)pimgVisual->GetThumbnailImage(cx, cy, NULL, NULL));
            if (pimage.get())
            {
                pimage->GetHBITMAP(Color::Black, &hbm);
            }
        }
    }
    return hbm;
}

HBITMAP PicDrawManager::_CreateBitmap(BYTE *pData, int cx, int cy) const
{
    HBITMAP hbm = NULL;
    if (!theApp._fNoGdiPlus)
    {
        // Use GDI+ if we can - the images are much better.
        hbm = _GetBitmapGDIP(pData, cx, cy);
    }
    else
    {
        // Here we'll use some ugly GDI scaling.
        CDC dc;
        if (dc.CreateCompatibleDC(NULL))
        {
            int cxBmp = 320;
            int cyBmp = 190;
            SCIBitmapInfo bmi(cx, cy);
            BYTE *pBitsDest;
            CBitmap bitmap;
            if (bitmap.Attach(CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&pBitsDest, NULL, 0)))                
            {
                SCIBitmapInfo bmiSrc(cxBmp, cyBmp);
                HGDIOBJ hObjOld = dc.SelectObject((HBITMAP)bitmap);
                // NOTE: HALFTONE is not supported on Win 98/Me
                // As a result, the images will be resized poorly for dithered colours.
                SetStretchBltMode(dc, HALFTONE);
                SetBrushOrgEx(dc, 0, 0, NULL);
                StretchDIBits(dc, 0, 0, cx, cy,
                    0, 0, cxBmp, cyBmp, pData, &bmiSrc, DIB_RGB_COLORS, SRCCOPY);                    
                dc.SelectObject(hObjOld);
                hbm = (HBITMAP)bitmap.Detach();
            }
        }
    }
    return hbm;
}

//
// Delayed allocation of bitmaps
//
void PicDrawManager::_EnsureBitmaps(DWORD dwEnable)
{
    HRESULT hr = S_OK;
    bool fVisualCreate = (dwEnable & DRAW_ENABLE_VISUAL) && !_dataVisual;
    bool fPriorityCreate = (dwEnable & DRAW_ENABLE_PRIORITY) && !_dataPriority;
    bool fControlCreate = (dwEnable & DRAW_ENABLE_CONTROL) && !_dataControl;
    bool fAuxCreate = ((fVisualCreate || fVisualCreate || fVisualCreate) && !_dataAux);

    if (fVisualCreate || fPriorityCreate || fControlCreate || fAuxCreate)
    {
        if (fVisualCreate)
        {
            ASSERT(!_dataVisual);
            _dataVisual.reset(new BYTE[BMPSIZE]);
            // REVIEW: bug for SCI1 pics here, since we are generating the bitmaps with the wrong (blank) palettes.
        }
        if (fPriorityCreate)
        {
            ASSERT(!_dataPriority);
            _dataPriority.reset(new BYTE[BMPSIZE]);
        }
        if (fControlCreate)
        {
            ASSERT(!_dataControl);
            _dataControl.reset(new BYTE[BMPSIZE]);
        }
        if (fAuxCreate)
        {
            ASSERT(!_dataAux);
            _dataAux.reset(new BYTE[BMPSIZE]);
        }
    }
}

BITMAPINFO *PicDrawManager::CreateSCIBMI() const
{
    //if (_f256Color)
    //{
    //    return new SCI1BitmapInfo(sPIC_WIDTH, sPIC_HEIGHT, _currentState.rgPaletteSCI1);
    //}
    //else
    {
        return new SCIBitmapInfo(sPIC_WIDTH, sPIC_HEIGHT);
    }
}

void PicDrawManager::RefreshAllScreens()
{
    _RedrawBuffers(NULL, DRAW_ENABLE_ALL, _GetDrawPos(), false);
}


const SCIPicState *PicDrawManager::GetPicState()
{
    return ModifyPicState();
}

SCIPicState *PicDrawManager::ModifyPicState()
{
    if (!_fValidState)
    {
        _EnsureCorrectState();
    }
    return &_currentState;
}


//
// This ensures that the current GetPicState is up-to-date
//
void PicDrawManager::_EnsureCorrectState()
{
    _pPicWeak->UpdatePicState(_currentState, _GetDrawPos());
    _fValidState = true;
}


//
// Given a point, determines the position in the pic, before iState (-1 = end)
// where that position changed.
// Returns -1 if that point was never changed.
// Currently only works for visual screen.
//
INT_PTR PicDrawManager::PosFromPoint(int x, int y, INT_PTR iStart)
{
    SCIPicState state(0);

    // Clear out our cached bitmaps.
    DWORD dwMapsToRedraw = 0;
    std::vector<BYTE> pdataVisual(BMPSIZE, 0x0f);
    std::vector<BYTE> pdataAux(BMPSIZE, 0x00);
    dwMapsToRedraw |= DRAW_ENABLE_VISUAL;
    PicData data =
    {
        dwMapsToRedraw,
        &pdataVisual[0], // Visual always needs to be provided (for fill)
        NULL,
        NULL,
        &pdataAux[0], // Aux always needs to be provided (for fill)
    };

    return _pPicWeak->GetLastChangedSpot(data, state, x, y);
}

//
// Seek to a particular position.
// The picture is drawn up to and including that position.
// 
//
bool PicDrawManager::SeekToPos(INT_PTR iPos)
{
    bool fChanged = ((iPos != _iInsertPos) || (iPos != _iDrawPos));
    if (fChanged)
    {
        _iInsertPos = iPos;
        _iDrawPos = iPos;
        _OnPosChanged();
    }
    return fChanged;
}

void PicDrawManager::Invalidate()
{
    _fValidVisual = false;
    _fValidControl = false;
    _fValidPriority = false;
    _fValidState = false;
}

void PicDrawManager::_OnPosChanged(bool fNotify)
{
    // FEATURE: we could be smarter about redrawing stuff.  If the new pos is after
    // the current one, we only need to redraw part of the pic.  Store a variable
    // "oldpos", which would be set to 0 if everything was invalidated (ie palette changed)
    // Otherwise, set the invalids as below, and when we _Redraw, if only
    Invalidate();
}
