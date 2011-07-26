// SCIPicEditorDoc.h : interface of the CResourceMapDoc class
//


#pragma once

#include "picresource.h"
#include "resourcedescription.h"
#include "EditPicDoc.h"
#include "pic.h"

// FWD declaration.
class CEditPic;


class CResourceMapDoc : public CDocument, public ISyncResourceMap
{
protected: // create from serialization only
    CResourceMapDoc();
    DECLARE_DYNCREATE(CResourceMapDoc)

// Attributes
public:

// Operations
public:
    // ISyncResourceMap
    HRESULT OnResourceAdded(const CResourceData *pData);
    HRESULT OnResourceDeleted(DWORD dwID);


// Overrides
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);

    // Create live resource docs
    CEditPic *CResourceMapDoc::CreateEditPicFromIndex(INT_PTR iIndex, CEditPicDoc *pDoc);

// Implementation
public:
    virtual ~CResourceMapDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
    CArray<CPic*, CPic*> &GetPics() { return _pics; }
    CArray<CResourceData*, CResourceData*> &GetPicResources() { return _resources; } 
    CPic *GetSelectedPic();

protected:

    void _DeleteAllPics();
    void _DeleteAllResourceData();
    HRESULT _SetEditPic(INT_PTR iIndex);

    CArray<CPic*, CPic*> _pics; 
    CArray<CResourceData*, CResourceData*> _resources; 
    
    INT_PTR _iSelectedPic;  // Index of selected pic in _pics;
    CEditPic *_pEditPic;     // Currently edited pic

// Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()

};


