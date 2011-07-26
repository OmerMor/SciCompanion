// TextDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "TextDoc.h"
#include "TextResource.h"


// CTextDoc

IMPLEMENT_DYNCREATE(CTextDoc, CResourceDocument)

void CTextDoc::SetTextResource(std::auto_ptr<TextResource> pText, int id)
{
    _id = id;
    AddFirstResource(pText.get());
    pText.release();
    _UpdateTitle();
    UpdateAllViews(NULL, VIEWUPDATEHINT_TEXTCHANGED);
}

const TextResource *CTextDoc::GetTextResource() const
{
    return static_cast<const TextResource*>(GetResource());
}

BEGIN_MESSAGE_MAP(CTextDoc, TCLASS_2(CUndoResource, CResourceDocument, IResourceBase) )
END_MESSAGE_MAP()


void CTextDoc::_OnSuccessfulSave(const IResourceBase *pResource)
{
    SetLastSaved(pResource);
}

void CTextDoc::v_OnUndoRedo()
{
    UpdateAllViews(NULL, VIEWUPDATEHINT_TEXTCHANGED);
}

//
// Note, for all the following methods, we don't end up updating the view
// We assume that the view knows what it did and will adjust itself accordingly.
int CTextDoc::AddString(PCTSTR pszString)
{
    TextResource *pNew = new TextResource(*GetTextResource());
    int iRet = pNew->AddString(pszString);
    AddNewResourceToUndo(pNew);
    SetModifiedFlag(TRUE);
    return iRet;
}
BOOL CTextDoc::SetStringAt(int iIndex, PCTSTR pszString)
{
    TextResource *pNew = new TextResource(*GetTextResource());
    BOOL fRet = pNew->SetStringAt(iIndex, pszString);
    AddNewResourceToUndo(pNew);
    if (fRet)
    {
        SetModifiedFlag(TRUE);
    }
    return fRet;
}
BOOL CTextDoc::MoveStringUp(int iIndex)
{
    TextResource *pNew = new TextResource(*GetTextResource());
    BOOL fRet = pNew->MoveStringUp(iIndex);
    AddNewResourceToUndo(pNew);
    SetModifiedFlag(TRUE);
    return fRet;
}
BOOL CTextDoc::MoveStringDown(int iIndex)
{
    TextResource *pNew = new TextResource(*GetTextResource());
    BOOL fRet = pNew->MoveStringDown(iIndex);
    AddNewResourceToUndo(pNew);
    SetModifiedFlag(TRUE);
    return fRet;
}
void CTextDoc::DeleteString(int iIndex)
{
    TextResource *pNew = new TextResource(*GetTextResource());
    pNew->DeleteString(iIndex);
    AddNewResourceToUndo(pNew);
    SetModifiedFlag(TRUE);
}


// CTextDoc diagnostics

#ifdef _DEBUG
void CTextDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTextDoc serialization

void CTextDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CTextDoc commands
