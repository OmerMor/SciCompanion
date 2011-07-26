#pragma once

// fwd declr
class CCursorResource;
class INonViewClient;
class IResourceBase;

#include "RasterResourceDocument.h"
#include "RasterResource.h"
#include "NonViewClient.h"
#include "CelResource.h"
#include "CursorResource.h"

EGACOLOR g_egaCursorPalette[];

// CCursorDoc document

class CCursorDoc : public CRasterResourceDocument, public ICelResourceDoc
{
	DECLARE_DYNCREATE(CCursorDoc)

public:
	CCursorDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    // CResourceDocument
    BOOL QueryCapability(const GUID guid, void **ppv);

    const CCursorResource *GetCursorResource() const { return static_cast<const CCursorResource*>(GetResource()); }
    void SetCursorResource(std::auto_ptr<CCursorResource> pCursor, int id = -1);

    // IEditRasterResource
    void SetTransparentColor(DWORD dwIndex, BYTE bTransparent) {} // Cursors don't support this.
    void MoveFromTo(DWORD dwIndexFrom, DWORD dwIndexTo) {}  // Cursors don't support this.
    Capabilities GetRasterCaps() { return Placement; }

    // IEditRasterResourceDoc
    DWORD GetSelectedIndex() { return 0; } // Always 0, we only have one cel
    void MoveSelectedCel(CPoint point) {}
    EGACOLOR GetViewColor() { return _color; }
    void SetViewColor(EGACOLOR color) { _color = color; UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_COLORCHANGED); }
    void RemoveCel(DWORD dwIndex) {}
    void GetPaletteAndColours(const EGACOLOR **prgPalette, int *pcRows, int *pcColumns)
    {
        *prgPalette = g_egaCursorPalette;
        *pcRows = 2;
        *pcColumns = 2;
    }
    int GetDefaultZoom() const { return theApp._iDefaultZoomCursor; }
    void SetDefaultZoom(int iZoom) const { theApp._iDefaultZoomCursor = iZoom; }
    BOOL CanDeleteCels() const { return FALSE; }
    OriginStyle GetOriginStyle() const { return OriginTopLeft; }

    // ICelResourceDoc
    int GetSelectedCel() const { return 0; } // Always 0
    void SetSelectedCel(int nChar);
    void InsertCel(int nLoop, int nCel, BOOL fBefore) { }; // Nope.
    void GetLabelString(PTSTR  pszLabel, size_t cch, int nCel) const;

protected:
    virtual void _OnSuccessfulSave(const IResourceBase *pDoc);
    virtual ResourceType _GetType() const { return RS_CURSOR; }

    void _ValidateCelIndex() { } // Nothing to do here, it's always index 0.

	DECLARE_MESSAGE_MAP()

    EGACOLOR _color;
};
