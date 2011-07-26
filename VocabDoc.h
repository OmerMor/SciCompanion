#pragma once

#include "ResourceDocument.h"
// CVocabDoc document

// FWD decl
class Vocab000;

//
// Vocab view updates
//
// Vocab000 is ready.
#define VIEWUPDATEHINT_VOCABCHANGED         0x00000100

class CVocabDoc : public CResourceDocument
{
	DECLARE_DYNCREATE(CVocabDoc)

public:
	CVocabDoc();
	virtual ~CVocabDoc();

    // Takes ownership:
    void SetVocab(Vocab000 *pVocab, int id = -1);
    Vocab000 *GetVocab() const { return _pVocab; }

    // CResourceDocument
    virtual IResourceBase *GetResource() const { return static_cast<IResourceBase*>(GetVocab()); }
    virtual ResourceType _GetType() const { return RS_VOCAB; }

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	DECLARE_MESSAGE_MAP()

    virtual void _OnSuccessfulSave(const IResourceBase *pDoc);

private:
    Vocab000 *_pVocab;
};
