#pragma once


// CChooseColorStatic

#define INVALID_COLORINDEX 0xff

#include "ColorDialogCallback.h"

class CChooseColorStatic : public CExtLabel
{
	DECLARE_DYNAMIC(CChooseColorStatic)

public:
    CChooseColorStatic();
	virtual ~CChooseColorStatic();
    void SetCallback(IColorDialogCallback *pCallback) { _pCallback = pCallback; }
    void SetRowsAndColumns(int cRows, int cColumns) { _cRows = cRows; _cColumns = cColumns; }
    void SetPalette(const EGACOLOR *pPalette) { _pPalette = pPalette; }
    void SetPrintIndex(BOOL fPrintIndex) { _fPrintIndex = fPrintIndex; }
    void ShowSelection(BOOL fShowSelection) { _fShowSelection = fShowSelection; }
    void ShowSelectionNumbers(BOOL fShow) { _fSelectionNumbers = fShow; }
    void SetSelection(BYTE bIndex);
    void SetAuxSelection(BYTE bIndex);
    void ShowIndices(BOOL fShowIndices) { _fShowIndices = fShowIndices; }
    void OnPaletteUpdated();

protected:
	DECLARE_MESSAGE_MAP()

    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnRButtonDown(UINT nFlags, CPoint point);
    void _OnButtonDown(UINT nFlags, CPoint point, BOOL fLeft);
    void OnMouseMove(UINT nFlags, CPoint point);

    BYTE _MapPointToIndex(CPoint point);
    void _MapRowColToRect(int iRow, int iColumn, RECT *prcOut);
    void _MapIndexToRect(BYTE bIndex, RECT *prc);
    virtual void _DrawItem(CDC *pDC, int cx, int cy); // for double-buffering
    virtual void _DrawIndex(CDC *pDC, BYTE bIndex);
    virtual void _DrawTextAtIndex(CDC *pDC, BYTE bIndex, PCTSTR psz);
    virtual void _DrawHover(CDC *pDC);
    virtual void _DrawSelection(CDC *pDC);

protected:
    BYTE _bHoverIndex;
    int _cRows;
    int _cColumns;
    BOOL _fPrintIndex;
    BOOL _fShowSelection;
    BOOL _fShowIndices;
    BYTE _bSelectedColorIndex;
    BYTE _bAuxSelectedColorIndex;
    BOOL _fSelectionNumbers;
    BOOL _fShowAuxSel;
    const EGACOLOR *_pPalette;
    IColorDialogCallback *_pCallback;
};

//
// Overrides drawing functionality, to draw brushes instead.
// Also ignores the SetPalette method.
// This should really be re-factored more.
//
class CChooseBrushStatic : public CChooseColorStatic
{
	DECLARE_DYNAMIC(CChooseBrushStatic)

public:
    CChooseBrushStatic();
	virtual ~CChooseBrushStatic();

protected:
	DECLARE_MESSAGE_MAP()

    virtual void _DrawItem(CDC *pDC, int cx, int cy);

private:
};


