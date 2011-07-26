#pragma once

#include "DialogFactory.h"
#include "PicCommandsCommon.h"

// CColorPickerButton

class CColorPickerButton : public CButton
{
	DECLARE_DYNAMIC(CColorPickerButton)

public:
	CColorPickerButton();
	virtual ~CColorPickerButton();
    void SetColorAndState(EGACOLOR color, BOOL bOn);
    void SetDialogFactory(IDialogFactory *pFactory) { _pFactory = pFactory; }

protected:
    virtual void _OnDraw(HDC hdc, RECT *prc, EGACOLOR color);
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void DrawItem(LPDRAWITEMSTRUCT pdis);

	DECLARE_MESSAGE_MAP()

    IDialogFactory *_pFactory;
    EGACOLOR _color;
    BOOL _bOn;
};


//
// CBrushButton
// (subclassed from CColorPickerButton due to laziness)
//
class CBrushButton : public CColorPickerButton
{
    DECLARE_DYNAMIC(CBrushButton)

public:
	CBrushButton();
	virtual ~CBrushButton();
    void SetPenStyle(const PenStyle *pPenStyle);

protected:
    virtual void _OnDraw(HDC hdc, RECT *prc, EGACOLOR color);
    
private:
    PenStyle _penStyle;
};
