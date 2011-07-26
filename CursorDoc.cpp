// EditViewDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "CursorDoc.h"
#include "CursorResource.h"
#include "NonViewClient.h"
#include "ResourceDocument.h"

EGACOLOR g_egaCursorPalette[] = 
{
    { 0, 0 },
    { 0xf, 0xf },
    { 0x3, 0x3 },
    { 0x3, 0x3 },
};


// CCursorDoc

IMPLEMENT_DYNCREATE(CCursorDoc, CResourceDocument)

CCursorDoc::CCursorDoc()
{
    _color.color1 = 0xf; // white
    _color.color2 = 0x0; // black
}


BOOL CCursorDoc::QueryCapability(const GUID guid, void **ppv)
{
    BOOL fRet = FALSE;
    if (IsEqualGUID(guid, __uuidof(ICelResourceDoc)))
    {
        fRet = TRUE;
        *ppv = static_cast<ICelResourceDoc*>(this);
    }
    else
    {
        fRet = __super::QueryCapability(guid, ppv);
    }
    return fRet;
}


void CCursorDoc::_OnSuccessfulSave(const IResourceBase *pDoc)
{
    SetLastSaved(pDoc);
}

void CCursorDoc::SetCursorResource(std::auto_ptr<CCursorResource> pCursor, int id)
{
    SetResource(pCursor.get(), id);
    pCursor.release();
}

void CCursorDoc::SetSelectedCel(int nChar)
{
    // Always zero
    ASSERT(nChar == 0);
}

void CCursorDoc::GetLabelString(PTSTR  pszLabel, size_t cch, int nCel) const
{
    // Unused.
}

BEGIN_MESSAGE_MAP(CCursorDoc, CRasterResourceDocument)
END_MESSAGE_MAP()


// CCursorDoc diagnostics

#ifdef _DEBUG
void CCursorDoc::AssertValid() const
{
	__super::AssertValid();
}

void CCursorDoc::Dump(CDumpContext& dc) const
{
	__super::Dump(dc);
}
#endif //_DEBUG


// CCursorDoc serialization

void CCursorDoc::Serialize(CArchive& ar)
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


// CCursorDoc commands
