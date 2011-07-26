#pragma once

#include "RasterResourceDocument.h"
#include "RasterResource.h"
#include "NonViewClient.h"
#include "CelResource.h"
#include "ViewResource.h"

// CViewDoc document

class __declspec(uuid("fb91277e-9ecb-4ffc-9d77-b125bacf2978")) IViewResourceDoc : public ICelResourceDoc
{
public:
    virtual int GetSelectedLoop() const = 0;
    virtual void SetSelectedLoop(int nLoop) = 0;
    virtual void MakeMirrorOf(int nLoop, int nOriginal) = 0;
    virtual void InsertLoop(int nLoop, BOOL fBefore) = 0;
    virtual void RemoveLoop(int nLoop) = 0;
};

class CViewDoc : public CRasterResourceDocument, public IViewResourceDoc
{
	DECLARE_DYNCREATE(CViewDoc)

public:
	CViewDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    // CResourceDocument 
    BOOL QueryCapability(const GUID guid, void **ppv);

    const ViewResource *GetViewResource() const { return static_cast<const ViewResource*>(GetResource()); }
    void SetViewResource(std::auto_ptr<ViewResource> pView, int id = -1);

    // IViewResourceDoc
    int GetSelectedCel() const { return _nCel; }
    int GetSelectedLoop() const { return _nLoop; }
    void SetSelectedLoop(int nLoop);
    void SetSelectedCel(int nCel);
    void MakeMirrorOf(int nLoop, int nOriginal);
    void InsertCel(int nLoop, int nCel, BOOL fBefore);
    void GetLabelString(PTSTR  pszLabel, size_t cch, int nCel) const { StringCchPrintf(pszLabel, cch, TEXT("%d"), nCel); }
    void InsertLoop(int nLoop, BOOL fBefore);
    void RemoveLoop(int nLoop);

    // IEditRasterResource
    void SetTransparentColor(DWORD dwIndex, BYTE bTransparent);
    Capabilities GetRasterCaps()
    {
        return static_cast<Capabilities>(TransparentColor | Placement | Resize);
    }

    // IEditRasterResourceDoc
    DWORD GetSelectedIndex() { return MAKE_INDEX(_nLoop, _nCel); }
    int GetSelectedGroup(DWORD *rgGroups, size_t ceGroup);
    void MoveSelectedCel(CPoint point);
    EGACOLOR GetViewColor() { return _color; }
    void SetViewColor(EGACOLOR color) { _color = color; UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_COLORCHANGED); }
    void RemoveCel(DWORD dwIndex);
    void GetPaletteAndColours(const EGACOLOR **prgPalette, int *pcRows, int *pcColumns)
    {
        *prgPalette = g_egaColorChooserPalette;
        *pcRows = 4;
        *pcColumns = 4;
    }
    int GetDefaultZoom() const;
    void SetDefaultZoom(int iZoom) const;
    BOOL CanDeleteCels() const { return TRUE; }
    OriginStyle GetOriginStyle() const { return OriginBottomCenter; }

protected:
    virtual void _OnSuccessfulSave(const IResourceBase *pDoc);
    virtual ResourceType _GetType() const { return RS_VIEW; }
    void _InsertFiles(CStringList &files);

    void _ValidateCelIndex();

	DECLARE_MESSAGE_MAP()
    afx_msg void OnAnimate();
    afx_msg void OnImportImageSequence();
    afx_msg void OnIsGDIPAvailable(CCmdUI *pCmdUI);

    // Current cel and loop
    int _nCel;
    int _nLoop;

    // This is in the document, since multiple views need it.
    // Perhaps it should be in the frame.
    EGACOLOR _color;
};
