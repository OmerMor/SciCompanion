// EditViewDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "FontDoc.h"
#include "FontResource.h"
#include "NonViewClient.h"
#include "ResourceDocument.h"

EGACOLOR g_egaFontPalette[] = 
{
    { 0, 0 },
    { 15, 15 },
};

// CFontDoc

IMPLEMENT_DYNCREATE(CFontDoc, CResourceDocument)

CFontDoc::CFontDoc()
{
    _nChar = 65; // Letter A
    _color.color1 = 0xf; // white
    _color.color2 = 0x0; // black
    // Default letters with which to preview:
    _strLetters = DEFAULT_FONTPREVIEWLETTERS;
}


BOOL CFontDoc::QueryCapability(const GUID guid, void **ppv)
{
    BOOL fRet = FALSE;
    if (IsEqualGUID(guid, __uuidof(ICelResourceDoc)))
    {
        fRet = TRUE;
        *ppv = static_cast<ICelResourceDoc*>(this);
    }
    else if (IsEqualGUID(guid, __uuidof(IFontResourceDoc)))
    {
        fRet = TRUE;
        *ppv = static_cast<IFontResourceDoc*>(this);
    }
    else
    {
        fRet = __super::QueryCapability(guid, ppv);
    }
    return fRet;
}

//
// Ensures the selected char exists in the current resource.
//
void CFontDoc::_ValidateCelIndex()
{
    const CFontResource *pEVR = GetFontResource();
    if (pEVR)
    {
        int cChars = pEVR->GetCelCount();
        ASSERT(cChars > 0);
        _nChar = min(_nChar, cChars - 1);
        _nChar = max(_nChar, 0);
    }
}


void CFontDoc::_OnSuccessfulSave(const IResourceBase *pDoc)
{
    SetLastSaved(pDoc);
}

void CFontDoc::SetFontResource(std::auto_ptr<CFontResource> pFont, int id)
{
    SetResource(pFont.get(), id);
    pFont.release();
}

void CFontDoc::SetSelectedCel(int nChar)
{
    // It is essential to check this, otherwise we'll end up in an infinite loop!
    if (_nChar != nChar)
    {
        _nChar = nChar;
        _ValidateCelIndex();
        UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_CELSELECTIONCHANGED);
    }
}

void CFontDoc::GetLabelString(PTSTR  pszLabel, size_t cch, int nCel) const
{
    if (nCel < 32)
    {
        StringCchPrintf(pszLabel, cch, TEXT("(%d)"), nCel);
    }
    else
    {
        StringCchPrintf(pszLabel, cch, TEXT("%c (%d)"), nCel, nCel);
    }
}

//
// Allows the callers to move "selection"
//
void CFontDoc::MoveSelectedCel(CPoint point)
{
    // if the y changed, that indicates a loop change.
    if (point.y != 0)
    {
        int nChar = _nChar + point.y;
        SetSelectedCel(nChar);
    }
    if (point.x != 0)
    {
        int nChar = _nChar + point.x;
        SetSelectedCel(nChar);
    }
}


void CFontDoc::SetLineHeight(WORD cyLine)
{
    const CFontResource *pEVR = GetFontResource();
    if (pEVR)
    {
        // Take the current resource and clone it, then apply the changes.
        CFontResource *pEVRClone = static_cast<CFontResource*>(pEVR->Clone());
        if (pEVRClone)
        {
            pEVRClone->SetLineHeight(cyLine);
            _NewResource(pEVRClone, VIEWUPDATEHINT_CELCHANGED);
        }
    }
}

void CFontDoc::SetPreviewLetters(PCTSTR pszLetters)
{
    _strLetters = pszLetters;
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SAMPLETEXTCHANGED);
}


void CFontDoc::RemoveCel(DWORD dwIndex)
{
    // Can't do this for fonts
}

int CFontDoc::GetDefaultZoom() const
{
    return theApp._iDefaultZoomFont;
}
void CFontDoc::SetDefaultZoom(int iZoom) const
{
    theApp._iDefaultZoomFont = iZoom;
}

int CFontDoc::GetSelectedGroup(DWORD *rgGroups, size_t ceGroup)
{
    int iRet = 0;
    if (_fApplyToAllCels)
    {
        // Just use indices 0 to 127
        ASSERT(ceGroup >= 128);
        for (DWORD i = 0; i < 128; i++)
        {
            rgGroups[i] = i;
        }
        iRet = 128;
    }
    else
    {
        iRet = __super::GetSelectedGroup(rgGroups, ceGroup);
    }
    return iRet;
}

void CFontDoc::MakeFont()
{
    CFontDialog fontDialog;
    if (IDOK == fontDialog.DoModal())
    {
        LOGFONT *pLogFont = fontDialog.m_cf.lpLogFont;
        if (pLogFont)
        {
            CFontResource *pEFR = new CFontResource();
            if (pEFR)
            {
                if (SUCCEEDED(pEFR->InitFromLOGFONT(pLogFont)))
                {
                    // Give it the same package and resource number as the current resource
                    const CFontResource *pEFRCurrent = GetFontResource();
                    if (pEFRCurrent)
                    {
                        pEFR->SetResourceNumber(pEFRCurrent->GetResourceNumber());
                        pEFR->SetPackageNumber(pEFRCurrent->GetPackageNumber());
                    }
                    _NewResource(pEFR, VIEWUPDATEHINT_CELCHANGED | VIEWUPDATEHINT_NEWVIEW);
                }
                else
                {
                    delete pEFR;
                }
            }
        }
    }
}

BEGIN_MESSAGE_MAP(CFontDoc, CRasterResourceDocument)
END_MESSAGE_MAP()



// CFontDoc diagnostics

#ifdef _DEBUG
void CFontDoc::AssertValid() const
{
	__super::AssertValid();
}

void CFontDoc::Dump(CDumpContext& dc) const
{
	__super::Dump(dc);
}
#endif //_DEBUG


// CFontDoc serialization

void CFontDoc::Serialize(CArchive& ar)
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


// CFontDoc commands
