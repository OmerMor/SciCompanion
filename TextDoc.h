#pragma once

#include "ResourceDocument.h"
#include "UndoResource.h"

// FWD decl
class TextResource;

// CTextDoc document

//
// Text view updates
//
// Text is ready.
#define VIEWUPDATEHINT_TEXTCHANGED         0x00000100

class CTextDoc : public CUndoResource<CResourceDocument, IResourceBase>
{
	DECLARE_DYNCREATE(CTextDoc)

public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o

    // Takes ownership:
    void SetTextResource(std::auto_ptr<TextResource> pText, int id = -1);
    const TextResource *GetTextResource() const;

    // This reflects the operations on TextResource
    int AddString(PCTSTR pszString);
    BOOL SetStringAt(int iIndex, PCTSTR pszString);
    BOOL MoveStringUp(int iIndex);
    BOOL MoveStringDown(int iIndex);
    void DeleteString(int iIndex);

    // CUndoResource
    void v_OnUndoRedo();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    virtual void _OnSuccessfulSave(const IResourceBase *pDoc);
    virtual ResourceType _GetType() const { return RS_TEXT; }

	DECLARE_MESSAGE_MAP()
};
