// EditViewDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ViewDoc.h"
#include "ViewResource.h"
#include "NonViewClient.h"
#include "AnimateDialog.h"
#include "ResourceDocument.h"
#include "DrawHelper.h"
#include "dlgs.h"

volatile BOOL g_fDitherImages = FALSE;

// CViewDoc

IMPLEMENT_DYNCREATE(CViewDoc, CResourceDocument)

CViewDoc::CViewDoc()
{
    _nCel = 0;
    _nLoop = 0;
    _color.color1 = 0xf; // white
    _color.color2 = 0x0; // black
}


BOOL CViewDoc::QueryCapability(const GUID guid, void **ppv)
{
    BOOL fRet = FALSE;
    if (IsEqualGUID(guid, __uuidof(ICelResourceDoc)))
    {
        fRet = TRUE;
        *ppv = static_cast<ICelResourceDoc*>(this);
    }
    else if (IsEqualGUID(guid, __uuidof(IViewResourceDoc)))
    {
        fRet = TRUE;
        *ppv = static_cast<IViewResourceDoc*>(this);
    }
    else
    {
        fRet = __super::QueryCapability(guid, ppv);
    }
    return fRet;
}

//
// Ensures the select loop/cel exist in the current resource.
//
void CViewDoc::_ValidateCelIndex()
{
    const ViewResource *pResource = GetViewResource();
    if (pResource)
    {
        int cLoops = pResource->GetLoopCount();
        ASSERT(cLoops > 0);
        _nLoop = min(_nLoop, cLoops - 1);
        _nLoop = max(_nLoop, 0);

        int cCels = pResource->GetCelCount(_nLoop);
        ASSERT(cCels > 0);
        _nCel = min(_nCel, cCels - 1);
        _nCel = max(_nCel, 0);
    }
}

void CViewDoc::_OnSuccessfulSave(const IResourceBase *pDoc)
{
    SetLastSaved(pDoc);
}

// This transfers ownership of ViewResource to this class.
void CViewDoc::SetViewResource(std::auto_ptr<ViewResource> pView, int id)
{
    SetResource(pView.get(), id);
    pView.release();
}

void CViewDoc::SetSelectedLoop(int nLoop)
{
    if (nLoop != _nLoop)
    {
        _nLoop = nLoop;
        _ValidateCelIndex();
        UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_LOOPSELECTIONCHANGED);
    }
}

void CViewDoc::SetSelectedCel(int nCel)
{
    // It is essential to check this, otherwise we'll end up in an infinite loop!
    if (nCel != _nCel)
    {
        _nCel = nCel;
        _ValidateCelIndex();
        UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_CELSELECTIONCHANGED);
    }
}

//
// Allows the callers to move "selection"
//
void CViewDoc::MoveSelectedCel(CPoint point)
{
    // if the y changed, that indicates a loop change.
    if (point.y != 0)
    {
        int nLoop = _nLoop + point.y;
        SetSelectedLoop(nLoop);
    }
    if (point.x != 0)
    {
        int nCel = _nCel + point.x;
        SetSelectedCel(nCel);
    }
}

int CViewDoc::GetSelectedGroup(DWORD *rgGroups, size_t ceGroup)
{
    int iRet = 0;
    if (_fApplyToAllCels)
    {
        // Return a list of indexes for the current cel.
        const ViewResource *pResource = GetViewResource();
        if (pResource)
        {
            int iCelCount = pResource->GetCelCount(_nLoop);
            if ((size_t)iCelCount <= ceGroup)
            {
                iRet = iCelCount;
                for (int i = 0; i < iCelCount; i++)
                {
                    rgGroups[i] = MAKE_INDEX(_nLoop, i);
                }
            }
        }
    }
    else
    {
        iRet = __super::GetSelectedGroup(rgGroups, ceGroup);
    }
    return iRet;
}

void CViewDoc::SetTransparentColor(DWORD dwIndex, BYTE bTransparent)
{
    const ViewResource *pResource = GetViewResource();
    if (pResource)
    {
        // Take the current resource and clone it, then apply the changes.
        IResourceBase *pClone = pResource->Clone();
        IEditRasterResource *perr;
        if (pClone && pClone->QueryCapability(IID_PPV_ARGS(&perr)))
        {
            perr->SetTransparentColor(dwIndex, bTransparent);
            _NewResource(pClone, VIEWUPDATEHINT_CELCHANGED);
        }
    }
}

void CViewDoc::MakeMirrorOf(int nLoop, int nOriginal)
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        // Folks should only be making the "selected" loop a mirror of something else.
        ASSERT(nLoop == _nLoop);

        // Take the current resource and clone it, then apply the changes (for undo)
        IResourceBase *pClone = pResource->Clone();
        IViewResource *pvr;
        if (pClone && pClone->QueryCapability(IID_PPV_ARGS(&pvr)))
        {
            pvr->MakeMirrorOf(nLoop, nOriginal);
            //_NewResource(pClone, VIEWUPDATEHINT_LOOPCHANGED);
            _NewResource(pClone, VIEWUPDATEHINT_NEWVIEW);
        }
    }
}

void CViewDoc::InsertLoop(int nLoop, BOOL fBefore)
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        // Take the current resource and clone it, then apply the changes (for undo)
        IResourceBase *pClone = pResource->Clone();
        IViewResource *pvr;
        if (pClone && pClone->QueryCapability(IID_PPV_ARGS(&pvr)))
        {
            if (SUCCEEDED(pvr->InsertLoop(nLoop, fBefore)))
            {
                // Reset the selection.
                _nCel = 0;
                _nLoop = fBefore ? nLoop : nLoop + 1;
                _NewResource(pClone, VIEWUPDATEHINT_NEWVIEW);
            }
            else
            {
                delete pClone;
            }
        }
    }
}



void CViewDoc::InsertCel(int nLoop, int nCel, BOOL fBefore)
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        IResourceBase *pClone = pResource->Clone();
        IViewResource *pvr;
        if (pClone && pClone->QueryCapability(IID_PPV_ARGS(&pvr)))
        {
            if (SUCCEEDED(pvr->InsertCel(nLoop, nCel, fBefore)))
            {
                // Send a 'NEWVIEW', since several loops could have changed because of this (mirroring)
                _NewResource(pClone, VIEWUPDATEHINT_NEWVIEW);

                // Reset the selection
                SetSelectedCel(fBefore ? nCel : nCel + 1);
            }
            else
            {
                delete pClone;
            }
        }
    }
}


void CViewDoc::RemoveLoop(int nLoop)
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        std::auto_ptr<IResourceBase> pClone(pResource->Clone());
        IViewResource *pvr;
        if (pClone.get() && pClone->QueryCapability(IID_PPV_ARGS(&pvr)))
        {
            if (SUCCEEDED(pvr->RemoveLoop(nLoop)))
            {
                _NewResource(pClone.get(), VIEWUPDATEHINT_NEWVIEW);
                pClone.release();
            }
        }
    }
}

void CViewDoc::RemoveCel(DWORD dwIndex)
{
    const IResourceBase *pResource = GetResource();
    if (pResource)
    {
        std::auto_ptr<IResourceBase> pClone(pResource->Clone());
        IViewResource *pvr;
        if (pClone.get() && pClone->QueryCapability(IID_PPV_ARGS(&pvr)))
        {
           if (SUCCEEDED(pvr->RemoveCel(dwIndex)))
           {
                // Send a 'NEWVIEW', since several loops could have changed because of this (mirroring)
                _NewResource(pClone.get(), VIEWUPDATEHINT_NEWVIEW);
                pClone.release();
           }
        }
    }
}

int CViewDoc::GetDefaultZoom() const
{
    return theApp._iDefaultZoomView;
}
void CViewDoc::SetDefaultZoom(int iZoom) const
{
    theApp._iDefaultZoomView = iZoom;
}

BYTE *GetSCIBitsFromFileName(PCTSTR pszFileName, BOOL bTransparent, CSize &sizeOut)
{
    BYTE *pBits = NULL;
#ifdef UNICODE
    Bitmap *pgdiplusBitmap = Bitmap::FromFile(pszFileName);
#else
    // GDI+ only deals with unicode.
    int a = lstrlenA(pszFileName);
    BSTR unicodestr = SysAllocStringLen(NULL, a);
    MultiByteToWideChar(CP_ACP, 0, pszFileName, a, unicodestr, a);
    Bitmap *pgdiplusBitmap = Bitmap::FromFile(unicodestr);
    //... when done, free the BSTR
    SysFreeString(unicodestr);
#endif    
    if (pgdiplusBitmap)
    {
        sizeOut.cx = pgdiplusBitmap->GetWidth();
        sizeOut.cy = pgdiplusBitmap->GetHeight();

        // Clamp size to allowed values.
        sizeOut.cx = min(320, sizeOut.cx);
        sizeOut.cx = max(1, sizeOut.cx);
        sizeOut.cy = min(190, sizeOut.cy);
        sizeOut.cy = max(1, sizeOut.cy);

        // Allocate our SCI bitmap data.
        pBits = new BYTE[CX_ACTUAL(sizeOut.cx)* sizeOut.cy];
        if (pBits)
        {
            for (int y = 0; y < sizeOut.cy; y++)
            {
                BYTE *pBitsRow = pBits + ((sizeOut.cy - y - 1) * CX_ACTUAL(sizeOut.cx));
                for (int x = 0; x < sizeOut.cx; x++)
                {
                    Color color;
                    if (Ok == pgdiplusBitmap->GetPixel(x, y, &color))
                    {
                        // find closest match.
                        EGACOLOR curColor = GetClosestEGAColor(1, g_fDitherImages ? 3 : 2, color.ToCOLORREF());
                        *(pBitsRow + x) = ((x^y) & 1) ? curColor.color1 : curColor.color2;
                    }
                }
            }
        }
        delete pgdiplusBitmap;
    }
    return pBits;
}

void CViewDoc::_InsertFiles(CStringList &files)
{
    // Take the current resource and clone it, then apply the changes (for undo)
    IResourceBase *pClone = GetResource()->Clone();
    IViewResource *pvr;
    IEditRasterResource *perr;
    IRasterResource *prr;
    if (pClone &&
        pClone->QueryCapability(IID_PPV_ARGS(&pvr)) &&
        pClone->QueryCapability(IID_PPV_ARGS(&perr)) &&
        pClone->QueryCapability(IID_PPV_ARGS(&prr)))
    {
        // Use the current loop.
        int nLoop = GetSelectedLoop();

        // Delete all but cel 0.
        int cCels = pvr->GetCelCount(nLoop);
        for (int i = cCels -1; i > 0; i--)
        {
            pvr->RemoveCel(MAKE_INDEX(nLoop, i));
        }

        int nCel = 0;
        POSITION pos = files.GetHeadPosition();
        while (pos != NULL)
        {
            DWORD dwIndex = MAKE_INDEX(nLoop, nCel);
            CString strFile = files.GetNext(pos);
            CSize size;
            scoped_array<BYTE> pBits(GetSCIBitsFromFileName(strFile, prr->GetTransparentColor(dwIndex), size));
            if (pBits.get()) // Could fail for other reasons
            {
                BOOL fOk = TRUE;
                if (nCel > 0)
                {
                    // Loop already has one cel.  Use it - but for subsequent cels,
                    // we need to add them.
                    fOk = SUCCEEDED(pvr->InsertCel(nLoop, nCel - 1, FALSE));
                }
                if (fOk)
                {
                    nCel++;
                }
                if (perr->SetSize(dwIndex, size, RasterBitmap::Normal))
                {
                    perr->SetBitmapData(dwIndex, pBits.get());
                }
            }
        }
        // Reset the selection.
        _nCel = 0;
        _NewResource(pClone, VIEWUPDATEHINT_NEWVIEW);
    }
}

#define MULTISELECT_BUFFERLENGTH 4096

class CDitherFileDialog : public CFileDialog
{
public:
    CDitherFileDialog(int fDither) : CFileDialog(TRUE, NULL, NULL,
        fDither ? OFN_ENABLESIZING | OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_READONLY : OFN_ENABLESIZING | OFN_EXPLORER | OFN_ALLOWMULTISELECT,
        g_szGdiplusFilter)
    {
        _pszOldBuffer = GetOFN().lpstrFile;
        GetOFN().nMaxFile = MULTISELECT_BUFFERLENGTH;
        GetOFN().lpstrFile = new TCHAR[MULTISELECT_BUFFERLENGTH];
        *GetOFN().lpstrFile = 0;
        _fDither = fDither;
    }

    ~CDitherFileDialog()
    {
        delete [] GetOFN().lpstrFile;
        GetOFN().lpstrFile = _pszOldBuffer;
    }

    BOOL GetDither()
    {
        return _fDither;
    }

protected:
    void OnInitDone()
    {
        __super::OnInitDone();
        this->GetParent()->SetDlgItemText(chx1, TEXT("Dither images"));
    }

    BOOL OnFileNameOK()
    {
        // Get the state of the checkbox (OFN_READONLY doesn't work when multi-select)
        HWND hwndCheck = GetParent()->GetDlgItem(chx1)->GetSafeHwnd();
        _fDither = (::SendMessage(hwndCheck, BM_GETCHECK, 0, 0) == BST_CHECKED);
        return __super::OnFileNameOK();
    }

private:
    TCHAR *_pszOldBuffer;
    BOOL _fDither;
};

void CViewDoc::OnImportImageSequence()
{
    // Create a file dialog.
    CDitherFileDialog fileDialog(g_fDitherImages);
    fileDialog.m_ofn.lpstrTitle = TEXT("Import image sequence");
    if (IDOK == fileDialog.DoModal())
    {
        g_fDitherImages = fileDialog.GetDither();
        // Make an ordered CStringList.
        CStringList _fileList;
        POSITION pos = fileDialog.GetStartPosition();
        while (pos != NULL)
        {
            CString strPath = fileDialog.GetNextPathName(pos);
            POSITION posList = _fileList.GetHeadPosition();
            BOOL fInserted = FALSE;
            while (posList != NULL)
            {
                POSITION posOld = posList;
                if (strPath < _fileList.GetNext(posList))
                {
                    fInserted = TRUE;
                    _fileList.InsertBefore(posOld, strPath);
                    break;
                }
            }
            if (!fInserted)
            {
                _fileList.AddTail(strPath);
            }
        }
        _InsertFiles(_fileList);
    }
}

void CViewDoc::OnIsGDIPAvailable(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(!theApp._fNoGdiPlus);
}

void CViewDoc::OnAnimate()
{
    if (HasUndos() && !_fLocked)
    {
        const ViewResource *pResource = GetViewResource();
        if (pResource)
        {
            CAnimateDialog dialog;
            dialog.SetTitle(GetTitle());
            dialog.SetView(pResource);
            dialog.SetLoop(_nLoop);
            dialog.DoModal();
        }
    }
}


BEGIN_MESSAGE_MAP(CViewDoc, CRasterResourceDocument)
    ON_COMMAND(ID_ANIMATE, OnAnimate)
    ON_COMMAND(ID_IMPORT_IMAGESEQUENCE, OnImportImageSequence)
    ON_UPDATE_COMMAND_UI(ID_IMPORT_IMAGESEQUENCE, OnIsGDIPAvailable)
END_MESSAGE_MAP()



// CViewDoc diagnostics

#ifdef _DEBUG
void CViewDoc::AssertValid() const
{
	__super::AssertValid();
}

void CViewDoc::Dump(CDumpContext& dc) const
{
	__super::Dump(dc);
}
#endif //_DEBUG


// CViewDoc serialization

void CViewDoc::Serialize(CArchive& ar)
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


// CViewDoc commands
