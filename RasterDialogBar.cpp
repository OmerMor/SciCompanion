// CRasterDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "RasterDialogBar.h"
#include "ViewResource.h"
#include "FontResource.h"
#include "ViewDoc.h"
#include "FontDoc.h"
#include "resource.h"
#include "ChoosePriConDialog.h"

// CRasterDialogBar

CRasterDialogBar::CRasterDialogBar(CWnd* pParent /*=NULL*/)
	: CExtDialogFwdCmd(0, pParent) // 0 for template, since we always use Create (since we use different templates)
{
    _pDoc = NULL;
    _cRows = 0;
    _cColumns = 0;
    _rgPalette = NULL;
    _fTransparency = false;
    _fSupportsPlacement = false;
    _fSupportsSize = false;
    _fSupportsFonts = false;
    _fSupportsLoops = false;
    _fSupportsApplyAll = false;
    _fRebuildingTree = false;

    // Load our accelerators
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_ACCELERATORRASTERBAR), RT_ACCELERATOR);
	_hAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATORRASTERBAR));
}

CRasterDialogBar::~CRasterDialogBar()
{
    m_font.DeleteObject();
}

BOOL CRasterDialogBar::PreTranslateMessage(MSG* pMsg)
{
    BOOL fRet = FALSE;
    if (_hAccel && (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST))
    {
        fRet = ::TranslateAccelerator(GetSafeHwnd(), _hAccel, pMsg);
    }
    if (!fRet)
    {
        fRet = __super::PreTranslateMessage(pMsg);
    }
    return fRet;
}

BEGIN_MESSAGE_MAP(CRasterDialogBar, CExtDialogFwdCmd)
    ON_CBN_SELCHANGE(IDC_COMBO_MIRROR, OnMirrorSelection)
    ON_CBN_SELCHANGE(IDC_COMBO_PENWIDTH, OnPenWidthSelection)
    ON_EN_KILLFOCUS(IDC_EDIT_CELWIDTH, OnEditCelWidth)
    ON_EN_KILLFOCUS(IDC_EDIT_CELHEIGHT, OnEditCelHeight)
    ON_EN_KILLFOCUS(IDC_EDIT_CELX, OnEditCelX)
    ON_EN_KILLFOCUS(IDC_EDIT_CELY, OnEditCelY)
    ON_EN_KILLFOCUS(IDC_EDIT_LINEHEIGHT, OnEditLineHeight)
    ON_EN_UPDATE(IDC_EDIT_SAMPLE, OnEditSampleText)
    ON_COMMAND(IDC_BUTTON_ADDCELBEFORE, OnAddCelBefore)
    ON_COMMAND(IDC_BUTTON_ADDCELAFTER, OnAddCelAfter)
    ON_COMMAND(IDC_BUTTON_DELETECEL, OnDeleteCel)
    ON_COMMAND(IDC_BUTTON_ADDLOOPBEFORE, OnAddLoopBefore)
    ON_COMMAND(IDC_BUTTON_ADDLOOPAFTER, OnAddLoopAfter)
    ON_COMMAND(IDC_BUTTON_DELETELOOP, OnDeleteLoop)
    ON_COMMAND(IDC_CHECKAPPLYTOALL, OnApplyToAll)
    ON_WM_DRAWITEM()
    ON_COMMAND(IDC_BUTTONUP, OnUp)
    ON_COMMAND(IDC_BUTTONDOWN, OnDown)
    ON_COMMAND(IDC_BUTTONRIGHT, OnRight)
    ON_COMMAND(IDC_BUTTONLEFT, OnLeft)
    ON_COMMAND(ID_MAKEFONT, OnMakeFont)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREELOOPS, OnTreeSelectionChanged)
END_MESSAGE_MAP()

void CRasterDialogBar::_OnUpdateCommandUIs()
{
    // Direction buttons:
    IViewResourceDoc *pvrDoc;
    const IViewResource *pvr;
    if (_pDoc && _pDoc->QueryResource(IID_PPV_ARGS_CONST(&pvr)) && _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)))
    {
        int nCel = pvrDoc->GetSelectedCel();
        int nLoop = pvrDoc->GetSelectedLoop();

        if (m_wndUp.GetSafeHwnd())
        {
            m_wndUp.EnableWindow(nLoop > 0);
        }
        if (m_wndDown.GetSafeHwnd())
        {
            m_wndDown.EnableWindow(nLoop < (pvr->GetLoopCount() - 1));
        }
        if (m_wndLeft.GetSafeHwnd())
        {
            m_wndLeft.EnableWindow(nCel > 0);
        }
        if (m_wndRight.GetSafeHwnd())
        {
            m_wndRight.EnableWindow(nCel < (pvr->GetCelCount(nLoop) - 1));
        }
    }

    // Apply to all
    if (m_wndApplyToAll.GetSafeHwnd())
    {
        IEditRasterResourceDoc *perrDoc;
        if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&perrDoc)))
        {
            m_wndApplyToAll.SetCheck(perrDoc->GetApplyToAllCels());
        }
    }

    if (_pDoc &&
        _pDoc->QueryResource(IID_PPV_ARGS_CONST(&pvr)))
    {
        if (m_wndDeleteLoop.GetSafeHwnd())
        {
            // We can only delete the loop if there is more than one.
            // REVIEW: this knowledge should belong in the doc (e.g. bool CanDeleteLoop())
            m_wndDeleteLoop.EnableWindow(pvr->GetLoopCount() > 1);
        }

        if (m_wndDeleteCel.GetSafeHwnd())
        {
            IViewResourceDoc *pvrDoc;
            if (_pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)))
            {
                int nLoop = pvrDoc->GetSelectedLoop();
                m_wndDeleteCel.EnableWindow((pvr->GetCelCount(nLoop) > 1) || (pvr->GetLoopCount() > 1));
            }
        }
    }



}

void CRasterDialogBar::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    if (IDC_STATIC_CHOSENCOLORS == nIDCtl)
    {
        // Draw two rectangles.
        IEditRasterResourceDoc *perrd;
        if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&perrd)))
        {
            CRect rect(&lpDrawItemStruct->rcItem);

            EGACOLOR color = perrd->GetViewColor();
            EGACOLOR colorFore = { color.color1, color.color1 };
            EGACOLOR colorBack = { color.color2, color.color2 };

            HDC hdc = lpDrawItemStruct->hDC;

            // Erase the background
            CDC dc;
            dc.Attach(hdc);
            m_wndChosenColors.OnEraseBackground(dc, rect);
            dc.Detach();

            CBrush brushFore;
            brushFore.CreateSolidBrush(EGA_TO_COLORREF(colorFore.color1));
            CBrush brushBack;
            brushBack.CreateSolidBrush(EGA_TO_COLORREF(colorBack.color1));
            CBrush brushEmpty;
            brushEmpty.CreateSolidBrush(g_PaintManager->GetColor(COLOR_3DFACE));

            int cxThirds = rect.Width() / 3;
            int cyThirds = rect.Height() / 3;
            const CBrush* brushes[3][3] = { { &brushFore, &brushFore, NULL /*&brushEmpty*/ },
                                            { &brushFore, &brushFore, &brushBack },
                                            { NULL /*&brushEmpty*/, &brushBack, &brushBack } };

            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    if (brushes[x][y])
                    {
                        int xx = x * cxThirds;
                        int yy = y * cyThirds;
                        CRect rectThird(rect.left + xx, rect.top + yy, rect.left + xx + cxThirds, rect.top + yy + cyThirds);
                        FillRect(hdc, &rectThird, (HBRUSH)(*(brushes[x][y])));
                    }
                }
            }
            // Any missing edges?
            /*
            int dy = rect.Height() - cyThirds * 3;
            if (dy > 0)
            {
                FillRect(hdc, CRect(rect.left, rect.bottom - dy, rect.right, rect.bottom), brushEmpty);
            }
            int dx = rect.Width() - cxThirds * 3;
            if (dx > 0)
            {
                FillRect(hdc, CRect(rect.right - dx, rect.top, rect.right, rect.bottom), brushEmpty);
            }*/
        }
    }
    else
    {
        __super::OnDrawItem(nIDCtl, lpDrawItemStruct);
    }
}



void CRasterDialogBar::SetDocument(CDocument *pDoc)
{
    _pDoc = static_cast<CViewDoc*>(pDoc);

    if (_pDoc)
    {
        _pDoc->AddNonViewClient(this);
        IEditRasterResourceDoc *perrd;
        if (_pDoc->QueryCapability(IID_PPV_ARGS(&perrd)))
        {
            perrd->GetPaletteAndColours(&_rgPalette, &_cRows, &_cColumns);
            m_wndPalette.SetRowsAndColumns(_cRows, _cColumns);
            m_wndPalette.SetPalette(_rgPalette);
            m_wndPalette.OnPaletteUpdated();
        }
        m_wndChosenColors.Invalidate(FALSE);
        _SyncSampleText();
        _OnPenWidthChanged();
        _SyncCelPane();
        _SyncLoopPane();
        _RebuildLoopTree();
        _SyncLoopTree();
        _OnUpdateCommandUIs();
    }
}

void CRasterDialogBar::OnColorClick(BYTE bIndex, int nID, BOOL fLeft)
{
    IEditRasterResourceDoc *perrd;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&perrd)))
    {
        ASSERT((int)bIndex < (_cRows * _cColumns)); // Don't overflow our array.
        EGACOLOR color = perrd->GetViewColor();
        if (fLeft)
        {
            // Foreground colour.
            color.color1 = _rgPalette[bIndex].color1;
        }
        else
        {
            // Background colour.
            color.color2 = _rgPalette[bIndex].color1;
        }
        perrd->SetViewColor(color);
    }
}

// IDialogFactory
void CRasterDialogBar::InvokeDialog(UINT nID, RECT *prcTarget)
{
    ASSERT(nID == IDC_BUTTON_TRANSPARENCY);
    const IRasterResource *prr;
    IEditRasterResource *perr;
    IEditRasterResourceDoc *perrDoc;
    if (_pDoc &&
        _pDoc->QueryResource(IID_PPV_ARGS_CONST(&prr)) &&
        _pDoc->QueryCapability(IID_PPV_ARGS(&perr)) &&
        _pDoc->QueryCapability(IID_PPV_ARGS(&perrDoc)))
    {
        DWORD dwIndex = perrDoc->GetSelectedIndex();
        BYTE bColor = prr->GetTransparentColor(dwIndex);
        CChoosePriConDialog dialog;
        dialog.SetCaptionID(IDS_CHOOSEATRANSPARENCY);
        dialog.SetColor(bColor);
        dialog.SetTrackRect(prcTarget);
        if (IDOK == dialog.DoModal())
        {
            bColor = dialog.GetColor();

            DWORD rgdwIndices[128];
            int cCels = perrDoc->GetSelectedGroup(rgdwIndices, ARRAYSIZE(rgdwIndices));
            if (cCels > 0)
            {
                perrDoc->SetGroupTransparentColor(cCels, rgdwIndices, bColor);
            }
        }
    }
}


void CRasterDialogBar::OnMirrorSelection()
{
    ASSERT(_fSupportsLoops);
    int iSel = m_wndMirrorOf.GetCurSel();
    IViewResourceDoc *pvrDoc;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)))
    {
        int iMirrorOf = (int)m_wndMirrorOf.GetItemData(iSel);
        int nLoop = pvrDoc->GetSelectedLoop();
        pvrDoc->MakeMirrorOf(nLoop, iMirrorOf);
    }
}


int _ValidateSize2(UINT nID, int nSize)
{
    if (nSize < 1)
    {
        return 1;
    }
    else if ((nID == IDC_EDIT_CELWIDTH) && (nSize > 320))
    {
        return 320;
    }
    else if ((nID == IDC_EDIT_CELHEIGHT) && (nSize > 190))
    {
        return 190;
    }
    else
    {
        return nSize;
    }

}
int _ValidatePlacement2(int nPlacement)
{
    if (nPlacement > 128)
    {
        return 128;
    }
    else if (nPlacement < -127)
    {
        return -127;
    }
    else
    {
        return nPlacement;
    }
}

void CRasterDialogBar::_OnEditSize(UINT nID, CWnd *pwnd)
{
    const IRasterResource *prr;
    IEditRasterResource *perr;
    IEditRasterResourceDoc *perrDoc;
    if (_pDoc &&
        _pDoc->QueryResource(IID_PPV_ARGS_CONST(&prr)) &&
        _pDoc->QueryCapability(IID_PPV_ARGS(&perr)) &&
        _pDoc->QueryCapability(IID_PPV_ARGS(&perrDoc)))
    {
        DWORD dwIndex = perrDoc->GetSelectedIndex();

        CSize size = prr->GetSize(dwIndex);
        CPoint point = prr->GetPlacement(dwIndex);

        CString strNumber;
        pwnd->GetWindowText(strNumber);
        int iNew = StrToInt(strNumber);
        int iNewValidated = iNew;
        if ((nID == IDC_EDIT_CELWIDTH) || (nID == IDC_EDIT_CELHEIGHT))
        {
            iNewValidated = _ValidateSize2(nID, iNew);
        }
        else
        {
            iNewValidated = _ValidatePlacement2(iNew);
        }

        if (iNew != iNewValidated)
        {
            iNew = iNewValidated;
            std::stringstream ss;
            ss << iNew;
            pwnd->SetWindowText(ss.str().c_str());
        }

        DWORD rgdwIndices[128];
        int cCels = perrDoc->GetSelectedGroup(rgdwIndices, ARRAYSIZE(rgdwIndices));
        if (cCels > 0)
        {
            BOOL fDoIt = FALSE;
            CSize sizes[128];
            switch(nID)
            {
            case IDC_EDIT_CELWIDTH:
                if (size.cx != iNew)
                {
                    fDoIt = TRUE;
                    sizes[0] = CSize(iNew, size.cy);
                }
                break;
            case IDC_EDIT_CELHEIGHT:
                if (size.cy != iNew)
                {
                    fDoIt = TRUE;
                    sizes[0] = CSize(size.cx, iNew);
                }
                break;
            case IDC_EDIT_CELX:
                if (point.x != iNew)
                {
                    perrDoc->SetGroupPlacement(cCels, rgdwIndices, CPoint(iNew, point.y));
                }
                break;
            case IDC_EDIT_CELY:
                if (point.y != iNew)
                {
                    perrDoc->SetGroupPlacement(cCels, rgdwIndices, CPoint(point.x, iNew));
                }
                break;
            }

            if (fDoIt)
            {
                for (int i = 1; i < cCels; i++)
                {
                    sizes[i] = sizes[0];
                }
                perrDoc->SetGroupSize(cCels, rgdwIndices, sizes, RasterBitmap::Normal);
            }
        }
    }
}

void CRasterDialogBar::OnEditCelWidth() { _OnEditSize(IDC_EDIT_CELWIDTH, &m_wndCelWidth); }
void CRasterDialogBar::OnEditCelHeight() { _OnEditSize(IDC_EDIT_CELHEIGHT, &m_wndCelHeight); }
void CRasterDialogBar::OnEditCelX() { _OnEditSize(IDC_EDIT_CELX, &m_wndCelX); }
void CRasterDialogBar::OnEditCelY() { _OnEditSize(IDC_EDIT_CELY, &m_wndCelY); }

void CRasterDialogBar::OnEditLineHeight()
{
    IFontResourceDoc *pfrd;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&pfrd)))
    {
        ASSERT(_fSupportsFonts);
        CString strNumber;
        m_wndLineHeight.GetWindowText(strNumber);
        WORD wNew = (WORD)StrToInt(strNumber);
        // Clip to between 1 and 127 (don't know the real limits, but those seem more than reasonable)
        wNew = max(wNew, 1);
        wNew = min(wNew, 127);
        pfrd->SetLineHeight(wNew);
    }
}

void CRasterDialogBar::OnEditSampleText()
{
    IFontResourceDoc *pfrd;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&pfrd)))
    {
        ASSERT(_fSupportsFonts);
        CString strSample;
        m_wndSampleText.GetWindowText(strSample);
        pfrd->SetPreviewLetters(strSample);
    }
}

void CRasterDialogBar::UpdateNonView(LPARAM lHint)
{
    // Take care of color first, since that is a lightweight operation.
    if (lHint & VIEWUPDATEHINT_COLORCHANGED)
    {
        m_wndChosenColors.Invalidate(FALSE);
        lHint &= ~VIEWUPDATEHINT_COLORCHANGED;
    }

    if (lHint & VIEWUPDATEHINT_PENWIDTHCHANGED)
    {
        _OnPenWidthChanged();
        lHint &= ~VIEWUPDATEHINT_PENWIDTHCHANGED;
    }

    if (lHint & VIEWUPDATEHINT_SAMPLETEXTCHANGED)
    {
        _SyncSampleText();
        lHint &= ~VIEWUPDATEHINT_SAMPLETEXTCHANGED;
    }

    if (lHint & VIEWUPDATEHINT_NEWVIEW)
    {
        _RebuildLoopTree();
        // Don't remove VIEWUPDATEHINT_NEWVIEW, because we still want
        // to do other things if its present, below.
    }

    if (lHint)
    {
        _OnPenWidthChanged();
        _SyncCelPane();
        _SyncLoopPane();
        _SyncLoopTree();
        _OnUpdateCommandUIs(); // TODO: could be more efficient..
    }
}


void CRasterDialogBar::_SyncLoopPane()
{
    _SyncMirror();

    IViewResourceDoc *pvrDoc;
    const IViewResource *pvr;
    if (_pDoc &&
        _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)) &&
        _pDoc->QueryResource(IID_PPV_ARGS_CONST(&pvr)))
    {
        int nLoop = pvrDoc->GetSelectedLoop();
        TCHAR szMsg[MAX_PATH];
        StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("Loop %d/%d    (%d cels)"), nLoop, pvr->GetLoopCount(), pvr->GetCelCount(nLoop));
        SetDlgItemText(IDC_STATIC_LOOPGROUP, szMsg);
    }
}

void CRasterDialogBar::_SyncMirror()
{
    if (_fSupportsLoops)
    {
        const IViewResource *pvr;
        IViewResourceDoc *pvrDoc;
        if (_pDoc &&
            _pDoc->QueryResource(IID_PPV_ARGS_CONST(&pvr)) &&
            _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)))
        {
            m_wndMirrorOf.SetRedraw(FALSE);
            int nLoop = pvrDoc->GetSelectedLoop();
            // Redo the combobox.
            m_wndMirrorOf.ResetContent();
            int iNoneIndex = m_wndMirrorOf.AddString(TEXT("None"));
            m_wndMirrorOf.SetItemData(iNoneIndex, -1);
            int iCurrentMirrorOf = pvr->GetMirrorOf(nLoop);
            int cLoops = pvr->GetLoopCount();
            BOOL fEnableCombo = TRUE;
            for (int i = 0; i < cLoops; i++)
            {
                int iMirrorOf = pvr->GetMirrorOf(i);
                int iIndex = 0;
                if ((iMirrorOf == -1) && (i != nLoop))
                {
                    // This one is not a mirror, and it's not ourselves.  Thus, it is a candidate for being listed in the combobox.
                    std::stringstream ss;
                    ss << "Loop " << i;
                    iIndex = m_wndMirrorOf.AddString(ss.str().c_str());
                    m_wndMirrorOf.SetItemData(iIndex, i);
                }

                if (iMirrorOf == nLoop)
                {
                    // Someone is a mirror of us.  So we should not offer a choice to make us a mirror of anyone.
                    fEnableCombo = FALSE;
                }

                if (iCurrentMirrorOf == i)
                {
                    // Select this one.
                    m_wndMirrorOf.SetCurSel(iIndex);
                }
            }
            if (iCurrentMirrorOf == -1)
            {
                m_wndMirrorOf.SetCurSel(iNoneIndex);
            }
            m_wndMirrorOf.EnableWindow(fEnableCombo);
            m_wndStaticMirrorOf.EnableWindow(fEnableCombo);
            m_wndMirrorOf.SetRedraw(TRUE);
            m_wndMirrorOf.Invalidate();
        }
    }
}

void CRasterDialogBar::_SyncSampleText()
{
    // Don't set the text if its the same
    IFontResourceDoc *pfrd;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&pfrd)))
    {
        ASSERT(_fSupportsFonts);
        CString strSampleCurrent;
        m_wndSampleText.GetWindowText(strSampleCurrent);
        CString strSampleNew;
        pfrd->GetPreviewLetters(strSampleNew);
        if (0 != strSampleNew.Compare(strSampleCurrent))
        {
            m_wndSampleText.SetWindowText(strSampleNew);
        }
    }
}

void CRasterDialogBar::_SyncCelPane()
{
    const IRasterResource *prr;
    ICelResourceDoc *pvrDoc;
    IEditRasterResourceDoc *perrDoc;
    if (_pDoc &&
        _pDoc->QueryResource(IID_PPV_ARGS_CONST(&prr)) &&
        _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)) &&
        _pDoc->QueryCapability(IID_PPV_ARGS(&perrDoc)))
    {
        if (GetDlgItem(IDC_STATIC_CELGROUP))
        {
            int nCel = pvrDoc->GetSelectedCel();
            std::stringstream ss;
            ss << "Cel: " << nCel;
            SetDlgItemText(IDC_STATIC_CELGROUP, ss.str().c_str());
        }

        DWORD dwIndex = perrDoc->GetSelectedIndex();

        if (_fSupportsSize)
        {
            CSize size = prr->GetSize(dwIndex);
            std::stringstream ss;
            ss << size.cx;
            m_wndCelWidth.SetWindowText(ss.str().c_str());
            std::stringstream ss2;
            ss2 << size.cy;
            m_wndCelHeight.SetWindowText(ss2.str().c_str());
        }

        if (_fSupportsPlacement)
        {
            CPoint pt = prr->GetPlacement(dwIndex);
            std::stringstream ss;
            ss << pt.x;
            m_wndCelX.SetWindowText(ss.str().c_str());
            std::stringstream ss2;
            ss2 << pt.y;
            m_wndCelY.SetWindowText(ss2.str().c_str());
        }

        if (_fTransparency)
        {
            BYTE bTransparent = prr->GetTransparentColor(dwIndex);
            EGACOLOR color = { bTransparent, bTransparent };
            m_wndTransparent.SetColorAndState(color, TRUE);
        }

        // Font line height
        const IFontResource *pfr;
        if (_pDoc->QueryResource(IID_PPV_ARGS_CONST(&pfr)))
        {
            ASSERT(_fSupportsFonts);
            std::stringstream ss;
            ss << (int)pfr->GetLineHeight();
            m_wndLineHeight.SetWindowText(ss.str().c_str());
        }
    }
}

void CRasterDialogBar::_PreparePenWidth()
{
    // Add the numbers 1-8 to the combobox.
    for (int i = 0; i < 8; i++)
    {
        std::stringstream ss;
        ss << (i + 1);
        m_wndPenWidth.AddString(ss.str().c_str());
    }
}

void CRasterDialogBar::OnApplyToAll()
{
    IEditRasterResourceDoc *perrDoc;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&perrDoc)))
    {
        perrDoc->SetApplyToAllCels(m_wndApplyToAll.GetCheck());
    }
}




void CRasterDialogBar::OnTreeSelectionChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
    if (!_fRebuildingTree)
    {
        IViewResourceDoc *pvrDoc;
        if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)))
        {
            LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
            LPARAM lParam = pNMTreeView->itemNew.lParam;
            if (TVI_ROOT == m_wndTree.GetParentItem(pNMTreeView->itemNew.hItem))
            {
                // This was a loop.  Select that loop.
                pvrDoc->SetSelectedLoop(DWORD_TO_LOOP(lParam));
            }
            else
            {
                // Must be a cel.
                pvrDoc->SetSelectedLoop(DWORD_TO_LOOP(lParam));
                pvrDoc->SetSelectedCel(DWORD_TO_CEL(lParam));
            }
        }   
    }
}

void CRasterDialogBar::OnPenWidthSelection()
{
    IEditRasterResourceDoc *perrDoc;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&perrDoc)))
    {
        perrDoc->SetPenWidth(m_wndPenWidth.GetCurSel() + 1);
    }
}

void CRasterDialogBar::_OnPenWidthChanged()
{
    IEditRasterResourceDoc *perrDoc;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&perrDoc)))
    {
        // Indicies 0-7 correspond to pen widths 1-8
        m_wndPenWidth.SetCurSel(perrDoc->GetPenWidth() - 1);
    }
}

void CRasterDialogBar::_SyncLoopTree()
{
    // Update selection
    IViewResourceDoc *pvrDoc;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)))
    {
        int nLoop = pvrDoc->GetSelectedLoop();
        int nCel = pvrDoc->GetSelectedCel();
        ASSERT(nLoop >= 0);
        ASSERT(nCel >= 0);
        DWORD dwIndex = MAKE_INDEX(nLoop, nCel);
        HTREEITEM hLoop = m_wndTree.GetChildItem(TVI_ROOT);
        while(hLoop && nLoop > 0)
        {
            hLoop = m_wndTree.GetNextSiblingItem(hLoop);
            nLoop--;
        }
        HTREEITEM hCel = m_wndTree.GetChildItem(hLoop);
        while(hCel && nCel > 0)
        {
            hCel = m_wndTree.GetNextSiblingItem(hCel);
            nCel--;
        }
        m_wndTree.SelectItem(hCel);
        m_wndTree.EnsureVisible(hCel);
    }
}

void CRasterDialogBar::_RebuildLoopTree()
{
    const IViewResource *pvr;
    if (_pDoc &&
        _pDoc->QueryResource(IID_PPV_ARGS_CONST(&pvr)))
    {
        _fRebuildingTree = true;
        m_wndTree.SetRedraw(FALSE);
        m_wndTree.DeleteAllItems();
        int cLoops = pvr->GetLoopCount();
        for (int loop = 0; loop < cLoops; loop++)
        {
            std::stringstream ssLoop;
            ssLoop << "loop " << loop;
            HTREEITEM hParent = m_wndTree.InsertItem(ssLoop.str().c_str());
            m_wndTree.SetItemData(hParent, MAKE_INDEX(loop, -1)); // -1 for cel - we should never use this info anyway.
            int cLoops = pvr->GetCelCount(loop);
            for (int cel = 0; cel < cLoops; cel++)
            {
                std::stringstream ssCel;
                ssCel << "cel " << cel;
                HTREEITEM hItem = m_wndTree.InsertItem(ssCel.str().c_str(), hParent);
                m_wndTree.SetItemData(hItem, MAKE_INDEX(loop, cel));
            }
            m_wndTree.Expand(hParent, TVE_EXPAND); 
        }
        m_wndTree.SetRedraw(TRUE);
        _fRebuildingTree = false;
    }
}

// CRasterDialogBar message handlers

void CRasterDialogBar::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);

	__super::DoDataExchange(pDX);
    ShowSizeGrip(FALSE);

    if (GetDlgItem(IDC_EDIT_CELWIDTH))
    {
	    DDX_Control(pDX, IDC_EDIT_CELWIDTH, m_wndCelWidth);
        DDX_Control(pDX, IDC_EDIT_CELHEIGHT, m_wndCelHeight);
        AddAnchor(IDC_EDIT_CELWIDTH, CPoint(100, 0), CPoint(100, 0));
        AddAnchor(IDC_EDIT_CELHEIGHT, CPoint(100, 0), CPoint(100, 0));
        _fSupportsSize = true;
    }
    if (GetDlgItem(IDC_EDIT_CELX))
    {
        DDX_Control(pDX, IDC_EDIT_CELX, m_wndCelX);
	    DDX_Control(pDX, IDC_EDIT_CELY, m_wndCelY);
        AddAnchor(IDC_EDIT_CELX, CPoint(100, 0), CPoint(100, 0));
        AddAnchor(IDC_EDIT_CELY, CPoint(100, 0), CPoint(100, 0));
        _fSupportsPlacement = true;
    }
    if (GetDlgItem(IDC_BUTTON_TRANSPARENCY))
    {
        DDX_Control(pDX, IDC_BUTTON_TRANSPARENCY, m_wndTransparent);
        AddAnchor(IDC_BUTTON_TRANSPARENCY, CPoint(100, 0), CPoint(100, 0));
        m_wndTransparent.SetDialogFactory(this);
        _fTransparency = true;
    }
    if (GetDlgItem(IDC_EDIT_LINEHEIGHT))
    {
        DDX_Control(pDX, IDC_EDIT_LINEHEIGHT, m_wndLineHeight);
        DDX_Control(pDX, IDC_EDIT_SAMPLE, m_wndSampleText);
        AddAnchor(IDC_EDIT_LINEHEIGHT, CPoint(100, 0), CPoint(100, 0));
        AddAnchor(IDC_EDIT_SAMPLE, CPoint(0, 0), CPoint(100, 0));
        _fSupportsFonts = true;
    }
    if (GetDlgItem(IDC_COMBO_MIRROR))
    {
        DDX_Control(pDX, IDC_COMBO_MIRROR, m_wndMirrorOf);
        DDX_Control(pDX, IDC_STATIC_MIRROROF, m_wndStaticMirrorOf);
        AddAnchor(IDC_STATIC_MIRROROF, CPoint(0, 0), CPoint(0, 0));
        AddAnchor(IDC_COMBO_MIRROR, CPoint(0, 0), CPoint(100, 0));
        _fSupportsLoops = true;
    }
    if (GetDlgItem(IDC_CHECKAPPLYTOALL))
    {
        DDX_Control(pDX, IDC_CHECKAPPLYTOALL, m_wndApplyToAll);
        AddAnchor(IDC_CHECKAPPLYTOALL, CPoint(0, 0), CPoint(100, 0));
        _fSupportsApplyAll = true;
    }

    // Prepare the palette - a 4x4 grid of colours - we'll set this up later
    DDX_Control(pDX, IDC_STATIC_PALETTE, m_wndPalette);
    AddAnchor(IDC_STATIC_PALETTE, CPoint(100, 0), CPoint(100, 0));
    m_wndPalette.SetRowsAndColumns(0, 0);
    m_wndPalette.SetPalette(NULL);
    m_wndPalette.SetCallback(this);
    DDX_Control(pDX, IDC_STATIC_CHOSENCOLORS, m_wndChosenColors);
    AddAnchor(IDC_STATIC_CHOSENCOLORS, CPoint(100, 0), CPoint(100, 0));
    DDX_Control(pDX, IDC_COMBO_PENWIDTH, m_wndPenWidth);
    AddAnchor(IDC_COMBO_PENWIDTH, CPoint(0, 0), CPoint(100, 0));
    _PreparePenWidth();

    // Prepare the left, right, up, down buttons, by applying a wingdings font.
    CFont *pFont = GetFont();
    LOGFONT logFont = { 0 };
    logFont.lfHeight = -18;
    logFont.lfCharSet = SYMBOL_CHARSET;
    logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_DONTCARE;
    logFont.lfWeight = FW_NORMAL;
    m_font;
    StringCchCopy(logFont.lfFaceName, ARRAYSIZE(logFont.lfFaceName), TEXT("WingDings"));
    m_font.CreateFontIndirect(&logFont);
    if (GetDlgItem(IDC_BUTTONUP))
    {
        DDX_Control(pDX, IDC_BUTTONUP, m_wndUp);
        DDX_Control(pDX, IDC_BUTTONDOWN, m_wndDown);
        AddAnchor(IDC_BUTTONUP, CPoint(100, 0), CPoint(100, 0));
        AddAnchor(IDC_BUTTONDOWN, CPoint(100, 0), CPoint(100, 0));
        m_wndUp.SetFont(&m_font);
        m_wndDown.SetFont(&m_font);
        m_wndUp.SetDrawFocusRect(FALSE);
        m_wndDown.SetDrawFocusRect(FALSE);
    }
    if (GetDlgItem(IDC_BUTTONLEFT))
    {
        DDX_Control(pDX, IDC_BUTTONLEFT, m_wndLeft);
        DDX_Control(pDX, IDC_BUTTONRIGHT, m_wndRight);
        AddAnchor(IDC_BUTTONLEFT, CPoint(100, 0), CPoint(100, 0));
        AddAnchor(IDC_BUTTONRIGHT, CPoint(100, 0), CPoint(100, 0));
        m_wndLeft.SetFont(&m_font);
        m_wndRight.SetFont(&m_font);
        m_wndLeft.SetDrawFocusRect(FALSE);
        m_wndRight.SetDrawFocusRect(FALSE);
    }

    if (GetDlgItem(IDC_TREELOOPS))
    {
        DDX_Control(pDX, IDC_TREELOOPS, m_wndTree);
        AddAnchor(m_wndTree.GetSafeHwnd(), CPoint(0, 0), CPoint(100, 100));
    }

    // Other visuals...
    if (GetDlgItem(IDC_BUTTON_ADDCELBEFORE))
    {
        DDX_Control(pDX, IDC_BUTTON_ADDCELBEFORE, m_wndCelBefore);
        DDX_Control(pDX, IDC_BUTTON_ADDCELAFTER, m_wndCelAfter);
        DDX_Control(pDX, IDC_BUTTON_DELETECEL, m_wndDeleteCel);
        AddAnchor(IDC_BUTTON_ADDCELBEFORE, CPoint(0, 0), CPoint(50, 0));
        AddAnchor(IDC_BUTTON_ADDCELAFTER, CPoint(0, 0), CPoint(50, 0));
        AddAnchor(IDC_BUTTON_DELETECEL, CPoint(50, 0), CPoint(100, 0));
    }
    if (GetDlgItem(IDC_BUTTON_ADDLOOPBEFORE))
    {
        DDX_Control(pDX, IDC_BUTTON_ADDLOOPBEFORE, m_wndLoopBefore);
        DDX_Control(pDX, IDC_BUTTON_ADDLOOPAFTER, m_wndLoopAfter);
        DDX_Control(pDX, IDC_BUTTON_DELETELOOP, m_wndDeleteLoop);
        AddAnchor(IDC_BUTTON_ADDLOOPBEFORE, CPoint(0, 0), CPoint(50, 0));
        AddAnchor(IDC_BUTTON_ADDLOOPAFTER, CPoint(0, 0), CPoint(50, 0));
        AddAnchor(IDC_BUTTON_DELETELOOP, CPoint(50, 0), CPoint(100, 0));
    }
    if (GetDlgItem(IDC_STATIC1))
    {
        DDX_Control(pDX, IDC_STATIC1, m_wndStatic1);
    }
    if (GetDlgItem(IDC_STATIC2))
    {
        DDX_Control(pDX, IDC_STATIC2, m_wndStatic2);
        AddAnchor(IDC_STATIC2, CPoint(0, 0), CPoint(100, 0));
    }
    if (GetDlgItem(IDC_STATIC3))
    {
        DDX_Control(pDX, IDC_STATIC3, m_wndStatic3);
        AddAnchor(IDC_STATIC3, CPoint(0, 0), CPoint(100, 0));
    }
    if (GetDlgItem(IDC_STATIC4))
    {
        DDX_Control(pDX, IDC_STATIC4, m_wndStatic4);
        AddAnchor(IDC_STATIC4, CPoint(0, 0), CPoint(100, 0));
    }
    if (GetDlgItem(IDC_STATIC5))
    {
        DDX_Control(pDX, IDC_STATIC5, m_wndStatic5);
        AddAnchor(IDC_STATIC5, CPoint(100, 0), CPoint(100, 0));
    }
    if (GetDlgItem(IDC_STATIC6))
    {
        DDX_Control(pDX, IDC_STATIC6, m_wndStatic6);
        AddAnchor(IDC_STATIC6, CPoint(100, 0), CPoint(100, 0));
    }
    if (GetDlgItem(IDC_STATIC_CELGROUP))
    {
        DDX_Control(pDX, IDC_STATIC_CELGROUP, m_wndGroup1);
        AddAnchor(IDC_STATIC_CELGROUP, CPoint(0, 0), CPoint(100, 0));
    }
    if (GetDlgItem(IDC_STATIC_LOOPGROUP))
    {
        DDX_Control(pDX, IDC_STATIC_LOOPGROUP, m_wndGroup2);
        AddAnchor(IDC_STATIC_LOOPGROUP, CPoint(0, 0), CPoint(100, 0));
    }
    if (GetDlgItem(ID_MAKEFONT))
    {
        DDX_Control(pDX, ID_MAKEFONT, m_wndMakeFont);
        AddAnchor(ID_MAKEFONT, CPoint(100, 0), CPoint(100, 0));
    }
}

void CRasterDialogBar::_OnAddCel(BOOL fBefore)
{
    IViewResourceDoc *pvrDoc;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)))
    {
        int nLoop = pvrDoc->GetSelectedLoop();
        int nCel = pvrDoc->GetSelectedCel();
        pvrDoc->InsertCel(nLoop, nCel, fBefore);
    }
}


void CRasterDialogBar::OnAddCelBefore()
{
    _OnAddCel(TRUE);
}

void CRasterDialogBar::OnAddCelAfter()
{
    _OnAddCel(FALSE);
}

void CRasterDialogBar::OnDeleteCel()
{
    IViewResourceDoc *pvrDoc;
    IEditRasterResourceDoc *perrDoc;
    if (_pDoc &&
        _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)) &&
        _pDoc->QueryCapability(IID_PPV_ARGS(&perrDoc)))
    {
        int nLoop = pvrDoc->GetSelectedLoop();
        int nCel = pvrDoc->GetSelectedCel();
        perrDoc->RemoveCel(MAKE_INDEX(nLoop, nCel));
    }
}

void CRasterDialogBar::_OnAddLoop(BOOL fBefore)
{
    IViewResourceDoc *pvrDoc;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)))
    {
        int nLoop = pvrDoc->GetSelectedLoop();
        pvrDoc->InsertLoop(nLoop, fBefore);
    }
}

void CRasterDialogBar::OnAddLoopBefore()
{
    _OnAddLoop(TRUE);
}

void CRasterDialogBar::OnAddLoopAfter()
{
    _OnAddLoop(FALSE);
}

void CRasterDialogBar::OnDeleteLoop()
{
    IViewResourceDoc *pvrDoc;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&pvrDoc)))
    {
        int nLoop = pvrDoc->GetSelectedLoop();
        pvrDoc->RemoveLoop(nLoop);
    }
}

void CRasterDialogBar::_OnDirectionButton(CPoint point)
{
    IEditRasterResourceDoc *perrDoc;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&perrDoc)))
    {
        if ((point.x != 0) || (point.y != 0))
        {
            perrDoc->MoveSelectedCel(point);
        }
    }
}

void CRasterDialogBar::OnUp()
{
    _OnDirectionButton(CPoint(0, -1));
}
void CRasterDialogBar::OnDown()
{
    _OnDirectionButton(CPoint(0, 1));
}
void CRasterDialogBar::OnRight()
{
    _OnDirectionButton(CPoint(1, 0));
}
void CRasterDialogBar::OnLeft()
{
    _OnDirectionButton(CPoint(-1, 0));
}
void CRasterDialogBar::OnMakeFont()
{
    IFontResourceDoc *pfrd;
    if (_pDoc && _pDoc->QueryCapability(IID_PPV_ARGS(&pfrd)))
    {
        pfrd->MakeFont();
    }
}

