#pragma once

#include "RasterResourceDocument.h"
#include "RasterResource.h"
#include "FontResource.h"
#include "CursorResource.h"

#define DEFAULT_FONTPREVIEWLETTERS "!@#$%^&*()_+0123456789-=~`ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}|[]\\:\";'<>?,./";

EGACOLOR g_egaFontPalette[];

class __declspec(uuid("1c640d04-9ce6-44f6-803e-d086d4bc160c")) IFontResourceDoc
{
public:
    virtual void SetLineHeight(WORD cy) = 0;
    virtual void SetPreviewLetters(PCTSTR pszLetters) = 0;
    virtual void GetPreviewLetters(CString &strLetters) = 0;
    virtual void MakeFont() = 0;
};

// CFontDoc document

class CFontDoc : public CRasterResourceDocument, public ICelResourceDoc, public IFontResourceDoc
{
	DECLARE_DYNCREATE(CFontDoc)

public:
	CFontDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    // CResourceDocument
    BOOL QueryCapability(const GUID guid, void **ppv);

    const CFontResource *GetFontResource() const { return static_cast<const CFontResource*>(GetResource()); }
    void SetFontResource(std::auto_ptr<CFontResource> pFont, int id = -1);

    // IEditRasterResource
    void SetTransparentColor(DWORD dwIndex, BYTE bTransparent) {} // Fonts don't support this.
    void SetPlacement(DWORD dwIndex, CPoint pt) {} // Fonts don't support this.
    void MoveFromTo(DWORD dwIndexFrom, DWORD dwIndexTo) {}  // Fonts don't support this.
    Capabilities GetRasterCaps() { return Resize; } // No placement or transparent colours

    // IEditRasterResourceDoc
    DWORD GetSelectedIndex() { return (DWORD)_nChar; }
    int GetSelectedGroup(DWORD *rgGroups, size_t ceGroup);
    void MoveSelectedCel(CPoint point);
    EGACOLOR GetViewColor() { return _color; }
    void SetViewColor(EGACOLOR color) { _color = color; UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_COLORCHANGED); }
    void RemoveCel(DWORD dwIndex);
    void GetPaletteAndColours(const EGACOLOR **prgPalette, int *pcRows, int *pcColumns)
    {
        *prgPalette = g_egaFontPalette;
        *pcRows = 1;
        *pcColumns = 2;
    }
    int GetDefaultZoom() const;
    void SetDefaultZoom(int iZoom) const;
    BOOL CanDeleteCels() const { return FALSE; }
    OriginStyle GetOriginStyle() const { return OriginNA; }

    // ICelResourceDoc
    int GetSelectedCel() const { return _nChar; }
    void SetSelectedCel(int nChar);
    void InsertCel(int nLoop, int nCel, BOOL fBefore) { }; // Nope.
    void GetLabelString(PTSTR  pszLabel, size_t cch, int nCel) const;

    // IFontResourceDoc
    void SetLineHeight(WORD cyLine);
    virtual void SetPreviewLetters(PCTSTR pszLetters); 
    virtual void GetPreviewLetters(CString &strLetters) { strLetters = _strLetters; }
    void MakeFont();

protected:
    virtual void _OnSuccessfulSave(const IResourceBase *pDoc);
    virtual ResourceType _GetType() const { return RS_FONT; }

    void _ValidateCelIndex();

	DECLARE_MESSAGE_MAP()
    afx_msg void OnAnimate();

    int _nChar;

    // This is in the document, since multiple views need it.
    // Perhaps it should be in the frame.
    EGACOLOR _color;

    // The letters with which to preview.
    CString _strLetters;
};
