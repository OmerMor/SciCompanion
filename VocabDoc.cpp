// VocabDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "VocabDoc.h"
#include "Vocab000.h"

// CVocabDoc

IMPLEMENT_DYNCREATE(CVocabDoc, CResourceDocument)

CVocabDoc::CVocabDoc()
{
    _pVocab = NULL;
}

CVocabDoc::~CVocabDoc()
{
    if (_pVocab)
    {
        delete _pVocab;
        _pVocab = NULL;
    }
}

void CVocabDoc::SetVocab(Vocab000 *pVocab, int id)
{
    _id = id;
    _pVocab = pVocab;
    UpdateAllViews(NULL, VIEWUPDATEHINT_VOCABCHANGED);
    _UpdateTitle();
}

void CVocabDoc::_OnSuccessfulSave(const IResourceBase *pDoc)
{
    // If we saved a vocab resource, clear the vocab000 cache.
    theApp.GetResourceMap().ClearVocab000();
}

BEGIN_MESSAGE_MAP(CVocabDoc, CResourceDocument)
END_MESSAGE_MAP()

// CVocabDoc diagnostics

#ifdef _DEBUG
void CVocabDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVocabDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



// CVocabDoc commands
