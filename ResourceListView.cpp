// ResourceListView.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ResourceListView.h"
#include "ResourceListDoc.h"
#include "ResourceManagerFrame.h"

// CResourceListCtrl

static const int ColumnName = 0;
static const int ColumnNumber = 1;
static const int ColumnSize = 2;
static const int ColumnPackage = 3;
static const int ColumnEncoding = 4;
static const int ColumnStatus = 5;
const struct
{
    PTSTR pszName; // Should be const, but the LVCOLUMN struct accepts only non-const
    int cx;
}
c_ColumnInfo [] =
{
    { "Name", 100 },
    { "Number", 70 },
    { "Size", 70 },
    { "Package", 80 },
    { "Encoding", 80 },
    { "Status", 80 },
};

IMPLEMENT_DYNCREATE(CResourceListCtrl, CListCtrl)

CResourceListCtrl::CResourceListCtrl()
{
    _iView = -1; // Invalid viewmode
    _bFirstTime = TRUE;
    _bDidInitialUpdate = FALSE;
    _pfnOpen = NULL;
    _bInLabelEdit = FALSE;
    _bScrolling = FALSE;
    _iSortColumn = -1;
}

CResourceListCtrl::~CResourceListCtrl()
{
}

// Note: We can't put any ON_COMMAND or ON_UPDATE_COMMAND_UI maps here, because this is not a view.
// Put them in ResourceManagerView instead.
BEGIN_MESSAGE_MAP(CResourceListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemClick)
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnItemDoubleClick)
    ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
    ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClicked)
    ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginLabelEdit)
    ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnNotifyDelete)
    ON_NOTIFY_REFLECT(LVN_GETINFOTIP, OnGetInfoTip)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
    ON_WM_CONTEXTMENU()
    // XP only...
    //ON_NOTIFY_REFLECT(LVN_BEGINSCROLL, OnBeginScroll)
    //ON_NOTIFY_REFLECT(LVN_ENDSCROLL, OnEndScroll)
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

#define UNUSED_STATUS_TEXT TEXT("Unused")


void CResourceListCtrl::OnRename()
{
    // Enter label edit.
    int nItem = GetSelectedItem();
    if (nItem != -1)
    {
        EditLabel(nItem);
    }
}


void CResourceListCtrl::OnBeginScroll(NMHDR* pNMHDR, LRESULT* pResult)
{
    _bScrolling = TRUE;
}
void CResourceListCtrl::OnEndScroll(NMHDR* pNMHDR, LRESULT* pResult)
{
    _bScrolling = FALSE;
}


void CResourceListCtrl::OnItemClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    // When an item is selected, tell the frame so it can update its preview.
    NMLISTVIEW *plv = reinterpret_cast<NMLISTVIEW*>(pNMHDR);
    if ((plv->uChanged & LVIF_STATE) && (plv->uNewState & LVIS_SELECTED))
    {
        CResourceManagerFrame *pFrame = static_cast<CResourceManagerFrame*>(GetParentFrame());
        if (pFrame)
        {
            ResourceBlob *pData = _GetResourceForItem(plv->iItem);
            if (pData)
            {
                pFrame->OnResourceSelected(*pData);
            }
        }
    }
}

int CALLBACK ColumnSort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    int iColumn = static_cast<int>(lParamSort);
    ResourceBlob *p1 = reinterpret_cast<ResourceBlob*>(lParam1);
    ResourceBlob *p2 = reinterpret_cast<ResourceBlob*>(lParam2);
    int iRet = 0;
    switch (iColumn)
    {
    case ColumnName:
        iRet = (p1->GetName().compare(p2->GetName()));
        break;
    case ColumnNumber:
        iRet = p1->GetNumber() - p2->GetNumber();
        break;
    case ColumnSize:
        iRet = p1->GetLength() - p2->GetLength();
        break;
    case ColumnPackage:
        iRet = p1->GetPackageHint() - p2->GetPackageHint();
        break;
    case ColumnEncoding:
        iRet = p1->GetEncoding() - p2->GetEncoding();
        break;
    case ColumnStatus:
        iRet = 0;
        break;
    }
    return iRet;
}

void CResourceListCtrl::OnColumnClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLISTVIEW *plv = reinterpret_cast<NMLISTVIEW*>(pNMHDR);
    ASSERT(plv->iItem == -1);
    _iSortColumn = plv->iSubItem;
    SortItems(ColumnSort, _iSortColumn);
}

void CResourceListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = FALSE;
    NMLVDISPINFO *plvdi = (NMLVDISPINFO *)pNMHDR;
    if (plvdi->item.pszText)
    {
        // Validate the name.
        if (IsValidResourceName(plvdi->item.pszText))
        {
            // We have a new name.
            // Get the item.
            ResourceBlob *pData = _GetResourceForItem(plvdi->item.iItem);
            ASSERT(pData);
            // Put the name in game.ini
            pData->SetName(plvdi->item.pszText);
            theApp.GetResourceMap().AssignName(*pData);

            // Change the name in the view:
            *pResult = TRUE; // This doesn't seem to work.
            SetItemText(plvdi->item.iItem, 0, plvdi->item.pszText);
        }
        else
        {
            DisplayInvalidResourceNameMessage(plvdi->item.pszText);
        }
    }
    _bInLabelEdit = FALSE;
}

void CResourceListCtrl::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
    _bInLabelEdit = TRUE;
}

BOOL CResourceListCtrl::PreTranslateMessage(MSG *pMsg)
{
	if (_bInLabelEdit && (pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST))
	{
        // Fwd the delete key to the edit control
        if ((pMsg->message != WM_CHAR) && (pMsg->wParam == VK_DELETE))
        {
            HWND hwndEdit = reinterpret_cast<HWND>(SendMessage(LVM_GETEDITCONTROL, 0, 0));
            if (hwndEdit)
            {
                ::SendMessage(hwndEdit, pMsg->message, pMsg->wParam, pMsg->lParam);
                return 1; // Don't dispatch message, we handled it.
            }
        }
    }
    return __super::PreTranslateMessage(pMsg);
}

void CResourceListCtrl::OnNotifyDelete(NMHDR* pNMHDR, LRESULT* pResult)
{
    // Free the ResourceBlob that was attached to the item.
    NMLISTVIEW *pnmlv = (NMLISTVIEW*)pNMHDR;
    delete ((ResourceBlob*)pnmlv->lParam);
}

void CResourceListCtrl::OnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVGETINFOTIP *pGetInfoTip = reinterpret_cast<NMLVGETINFOTIP*>(pNMHDR);
    const ResourceBlob *pData = reinterpret_cast<const ResourceBlob*>(GetItemData(pGetInfoTip->iItem));
    if (pData)
    {
        StringCchPrintf(pGetInfoTip->pszText, pGetInfoTip->cchTextMax, "%03d\n%d bytes.", pData->GetNumber(), pData->GetLength());
    }
}

void CResourceListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_RETURN)
    {
        int nItem = GetSelectedItem();
        if (nItem != -1)
        {
            ResourceBlob *pData = _GetResourceForItem(nItem);
            ASSERT(pData);
            _OnItemDoubleClick(pData);
        }
    }
    else
    {
        __super::OnKeyDown(nChar, nRepCnt, nFlags);
    }
}

void CResourceListCtrl::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVCUSTOMDRAW *pnmlv = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
    NMCUSTOMDRAW *pNMCD = reinterpret_cast<NMCUSTOMDRAW*>(pNMHDR);
    *pResult = 0;
    switch (pNMCD->dwDrawStage)
    {
    case CDDS_PREPAINT:
        {
            *pResult = CDRF_NOTIFYITEMDRAW;
            break;
        }

    case CDDS_ITEMPREPAINT:
        {
            // If this is not the most recent resource of this type/number/package, then
            // color the item grey.
            const ResourceBlob *pData = (ResourceBlob*)pnmlv->nmcd.lItemlParam;
            if (!theApp._resourceRecency.IsResourceMostRecent(pData))
            {
                pnmlv->clrText = GetSysColor(COLOR_GRAYTEXT);
            }
            else
            {
                if (theApp._resourceRecency.WasResourceJustAdded(pData))
                {
                    pnmlv->clrText = RGB(255, 0, 0);
                }
                else
                {
                    pnmlv->clrText = GetSysColor(COLOR_HOTLIGHT);
                }
            }
            // FEATURE: highlight those most recently added item in red.
            break;
        }
    }
}

void CResourceListCtrl::OnContextMenu(CWnd *pWnd, CPoint point)
{
    int iItem = GetSelectedItem();
    CMenu contextMenu; 
    if (iItem != -1)
    {
        if ((point.x == -1) && (point.y == -1))
        {
            // Activated by keyboard.  Make the menu appear near the top left of the main selected item.
            CRect rc;
            if (GetItemRect(iItem, &rc, LVIR_BOUNDS))
            {
                point = rc.TopLeft();
                point.Offset(5, 5);
                ClientToScreen(&point);
            }
        }
        contextMenu.LoadMenu(IDR_MENUVIEWITEM);
    }
    else
    {
        if ((point.x == -1) && (point.y == -1))
        {
            // Activated by keyboard.  Make the menu appear at the top left of the view
            point.x = 0;
            point.y = 0;
            ClientToScreen(&point);
        }
        contextMenu.LoadMenu(IDR_MENUVIEWBG);
    }

    CMenu* pTracker; 
    pTracker = contextMenu.GetSubMenu(0); 
    if (pTracker)
    {
        //ClientToScreen(&point);
        pTracker->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x , point.y , AfxGetMainWnd()); 
    }
    //has always to be called (function of base class)
    __super::OnContextMenu(pWnd, point);
}

void CResourceListCtrl::_OnItemDoubleClick(const ResourceBlob *pData)
{
    if (_pfnOpen)
    {
        (*_pfnOpen)(pData);
    }
}

BOOL CResourceListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
    // We wish to enable label edits.
    cs.style |= LVS_EDITLABELS | LVS_SHOWSELALWAYS | LVS_AUTOARRANGE;
    cs.style |= WS_TABSTOP;
	return __super::PreCreateWindow(cs);
}


void CResourceListCtrl::OnItemDoubleClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW *pNMLV = (NMLISTVIEW*)pNMHDR;
    int iViewIndex = pNMLV->iItem;
    if (iViewIndex != -1) // Could be -1 if user clicked on the background.
    {
        ResourceBlob *pData = _GetResourceForItem(iViewIndex);
        ASSERT(pData);
        _OnItemDoubleClick(pData);
    }
    *pResult = 0;
}


//
// Returns -1 if there is no item selected.
//
int CResourceListCtrl::GetSelectedItem()
{
    int nItem = -1;
    POSITION pos = GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        nItem = GetNextSelectedItem(pos);
    }
    return nItem;
}

//
// BrowseForFolder callback to set default selection
// 
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if (uMsg == BFFM_INITIALIZED)
    {
        const LPITEMIDLIST pidl = theApp.GetExportFolder();
        if (pidl)
        {
            SendMessage(hwnd, BFFM_SETSELECTION, FALSE, (LPARAM)pidl);
        }
    }
    return 0;
}

void CResourceListCtrl::OnExtractAsBitmap()
{
    UINT cCount = GetSelectedCount();
    if (cCount == 1)
    {
        const ResourceBlob *pData = _GetResourceForItem(GetSelectedItem());
        if (pData)
        {
            IResourceBase *prb;
            if (SUCCEEDED(CreateResourceFromResourceData(pData, &prb)))
            {
                std::auto_ptr<IResourceBase> resource(prb);
                ExportResourceAsBitmap(prb);
            }
        }
    }
}

void CResourceListCtrl::OnExtractResources()
{
    UINT cCount = GetSelectedCount();
    if (cCount > 1)
    {
        if (SUCCEEDED(CoInitialize(NULL)))
        {
            BROWSEINFO browseInfo = { 0 };
            browseInfo.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
            browseInfo.lpszTitle = TEXT("Please choose a folder where you would like to export the resources.");
            browseInfo.ulFlags = BIF_RETURNONLYFSDIRS;
            browseInfo.lpfn = BrowseCallbackProc;
            LPITEMIDLIST pidl = SHBrowseForFolder(&browseInfo);
            if (pidl)
            {
                TCHAR szFolder[MAX_PATH];
                if (SHGetPathFromIDList(pidl, szFolder))
                {
                    DWORD dwCount = 0;
                    HRESULT hr = S_OK;
                    POSITION pos = GetFirstSelectedItemPosition();
                    const ResourceBlob *pData = NULL; // keep this in scope for below
                    while (SUCCEEDED(hr) && (pos != NULL))
                    {
                        int nItem = GetNextSelectedItem(pos);
                        pData = _GetResourceForItem(nItem);
                        if (pData)
                        {
                            TCHAR szPath[MAX_PATH];
                            hr = StringCchPrintf(szPath, ARRAYSIZE(szPath),
                                                TEXT("%s\\%s.%03d"),
                                                szFolder,
                                                g_resourceInfo[pData->GetType()].pszFileNameDefault,
                                                pData->GetNumber());
                            if (SUCCEEDED(hr))
                            {
                                hr = pData->SaveToFile(szPath);
                                if (SUCCEEDED(hr))
                                {
                                    dwCount++;
                                }
                            }
                        }
                    }

                    TCHAR szMsg[MAX_PATH * 2];
                    if (SUCCEEDED(hr))
                    {
                        StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("Success!  Wrote %d resources to %s."), dwCount, szFolder);
                        AfxMessageBox(szMsg, MB_OK | MB_APPLMODAL);
                    }
                    else
                    {
                        // Prepare error.
                        TCHAR szError[MAX_PATH];
                        szError[0] = 0;
                        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, HRESULT_CODE(hr), 0, szError, ARRAYSIZE(szError), NULL);

                        StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("There was an error writing the resource %03d to %s\n%s"), pData->GetNumber(), szFolder, szError);
                        AfxMessageBox(szMsg, MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL, 0);
                    }
                }

                theApp.SetExportFolder(pidl); // Takes ownership of pidl.
                //CoTaskMemFree(pidl);
            }
            CoUninitialize();
        }
    }
    else if (cCount == 1)
    {
        const ResourceBlob *pData = _GetResourceForItem(GetSelectedItem());
        if (pData)
        {
            TCHAR szFileName[MAX_PATH];
            StringCchPrintf(szFileName, ARRAYSIZE(szFileName), TEXT("%s.%03d"), g_resourceInfo[GetType()].pszFileNameDefault, pData->GetNumber());
            CFileDialog fileDialog(FALSE, NULL, szFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, g_resourceInfo[GetType()].pszFileDialogFilter);
            if (IDOK == fileDialog.DoModal())
            {
                CString strFileName = fileDialog.GetFileName();
                HRESULT hr = pData->SaveToFile((PCSTR)strFileName);
                if (FAILED(hr))
                {
                    DisplayFileError(hr, FALSE, strFileName);
                }
            }
        }
    }
}

void CResourceListCtrl::OnDelete()
{
    UINT cCount = GetSelectedCount();
    if (cCount > 1)
    {
        // Multiple items
        TCHAR szBuffer[MAX_PATH];
        StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), TEXT("Delete these %d items?"), cCount);
        if (IDYES == AfxMessageBox(szBuffer, MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION))
        {
            // Grab all the selected ResourceBlob's, before we start deleting any items.
            CArray<ResourceBlob*, ResourceBlob*> resourcesTempCopy;
            POSITION pos = GetFirstSelectedItemPosition();
            while (pos != NULL)
            {
                int nItem = GetNextSelectedItem(pos);
                resourcesTempCopy.Add(_GetResourceForItem(nItem));
            }

            for (int i = 0; i <= resourcesTempCopy.GetUpperBound(); i++)
            {
                ResourceBlob *pData = resourcesTempCopy.GetAt(i);
                ASSERT(pData);
                // We'll get updated a lot here.
                theApp.GetResourceMap().DeleteResource(pData);
                // We don't need to free the pData right here.  It will happen when
                // we tell the resource map to delete the resource - upon which we'll get notified that
                // a resource has been deleted.
            }
        }
    }
    else
    {
        int nItem = GetSelectedItem();
        if (nItem != -1)
        {
            ResourceBlob *pData = _GetResourceForItem(nItem);
            ASSERT(pData);
            TCHAR szBuffer[MAX_PATH];
            StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), TEXT("Delete %s?"), pData->GetName().c_str());
            if (IDYES == AfxMessageBox(szBuffer, MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION))
            {
                theApp.GetResourceMap().DeleteResource(pData);
            }
        }
    }
}

void CResourceListCtrl::ChangeViewMode(int iView)
{
    if (iView != _iView)
    {
        _iView = iView;
        LONG_PTR dwStyle = GetWindowLongPtr(m_hWnd, GWL_STYLE); 
        dwStyle &= ~LVS_TYPEMASK;
        dwStyle |= _iView;
        SetWindowLongPtr(m_hWnd, GWL_STYLE, (LONG)dwStyle);
    }
}

void CResourceListCtrl::_InitColumns()
{
    for (int i = 0; i < ARRAYSIZE(c_ColumnInfo); i++)
    {
        // Name
        LVCOLUMN col = { 0 };
        col.mask = LVCF_FMT | LVCF_ORDER | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 
        col.iOrder = i;
        col.iSubItem = i;
        col.pszText = c_ColumnInfo[i].pszName;
        col.cx = c_ColumnInfo[i].cx;
        col.fmt = LVCFMT_RIGHT;
        InsertColumn(i, &col);
    }
}

void CResourceListCtrl::_ReevaluateRecency(const ResourceBlob *pData)
{
    int cItems = GetItemCount();
    for (int i = 0; i < cItems; i++)
    {
        ResourceBlob *pDataCur = _GetResourceForItem(i);
        if ((pDataCur->GetPackageHint() == pData->GetPackageHint()) &&
            (pDataCur->GetNumber() == pData->GetNumber()))
        {
            // This is just like the item that was added or removed.  So let's re-evaluate its recency.
            BOOL fMostRecent = theApp._resourceRecency.IsResourceMostRecent(pDataCur);

            // ...by specifying an overlay image (if appropriate)
            LVITEM item = { 0 };
            item.iItem = i;
            item.mask = LVIF_STATE;
            item.state = INDEXTOOVERLAYMASK(fMostRecent ? 0 : OVERLAYINDEX_UNUSED);
            item.stateMask = LVIS_OVERLAYMASK;
            SetItem(&item);

            // ...and by adjusting its status.
            TCHAR szBuf[100];
            szBuf[0] = 0;
            LVITEM itemSub = { 0 };
            itemSub.iItem = i;
            itemSub.iSubItem = ColumnStatus;
            itemSub.mask = LVIF_TEXT;
            itemSub.pszText = szBuf;
            if (!fMostRecent)
            {
                StringCchCopy(szBuf, ARRAYSIZE(szBuf), UNUSED_STATUS_TEXT);
            }
            SetItem(&itemSub);
        }
    }
}


void CResourceListCtrl::_PrepareLVITEM(LVITEM *pItem)
{
    // Subclasses override to provide more LVITEM properties.
}

void CResourceListCtrl::_OnInitListView(int cItems)
{
    // subclasses can override.
}

void CResourceListCtrl::_DeleteItem(const ResourceBlob *pData)
{
    int iIndex = _GetItemForResource(pData);
    DeleteItem(iIndex);
}

void CResourceListCtrl::_InsertItem(ResourceBlob *pData)
{
    TCHAR szName[100];
    StringCchCopy(szName, ARRAYSIZE(szName), pData->GetName().c_str()); // Copy, since pszText takes a non-const
    LVITEM item = { 0 };
    item.mask = LVIF_TEXT | LVIF_PARAM;
    item.iItem = 0;
    item.iSubItem = 0;
    item.pszText = szName;
    item.lParam = (LPARAM)pData;
    _PrepareLVITEM(&item);
    int iIndex = InsertItem(&item);

    if (iIndex == -1)
    {
        theApp.LogInfo(TEXT("Failed to insert item in listview: %s"), szName);    
    }

    // Now the columns
    for (int iSub = 1; iSub < ARRAYSIZE(c_ColumnInfo); iSub++)
    {
        TCHAR szBuf[100];
        LVITEM item = { 0 };
        item.mask = LVIF_TEXT;
        item.iItem = iIndex;
        item.iSubItem = iSub;
        item.pszText = szBuf;
        switch (iSub)
        {
        case ColumnNumber:
            StringCchPrintf(szBuf, ARRAYSIZE(szBuf), TEXT("%03d"), pData->GetNumber());
            break;
        case ColumnPackage:
            StringCchPrintf(szBuf, ARRAYSIZE(szBuf), TEXT("%03d"), pData->GetPackageHint());
            break;
        case ColumnSize:
            {
                int iLength = pData->GetLength();
                int iThousandths = (iLength % 1024) * 1000 / 1024;
                int kiloBytes = iLength / 1024;
                int iRoundedHundredths = (iLength % 1024) * 100 / 1024;
                if (iRoundedHundredths * 10 + 5 < iThousandths)
                {
                    iRoundedHundredths++;
                    if (iRoundedHundredths == 100)
                    {
                        kiloBytes++;
                        iRoundedHundredths = 0;
                    }
                }
                StringCchPrintf(szBuf, ARRAYSIZE(szBuf), TEXT("%d.%02dKB"), kiloBytes, iRoundedHundredths);
            }
            break;

        case ColumnEncoding:
            {
                StringCchCopy(szBuf, ARRAYSIZE(szBuf), pData->GetEncodingString().c_str());
            }
            break;
        case ColumnStatus:
            szBuf[0] = 0;
            if (!theApp._resourceRecency.IsResourceMostRecent(pData))
            {
                StringCchCopy(szBuf, ARRAYSIZE(szBuf), UNUSED_STATUS_TEXT);
            }
            break;

        default:
            StringCchCopy(szBuf, ARRAYSIZE(szBuf), "Test");
            break;
        }
        SetItem(&item);
    }
}

void CResourceListCtrl::UpdateView()
{
    _UpdateEntries(NULL);
}

void CResourceListCtrl::OnUpdate(LPARAM lHint, CObject *pHint)
{
    if (lHint & VIEWUPDATEHINT_RESOURCEMAPCHANGED)
    {
        // Pass in optional CFile
        _UpdateEntries((CFile*)pHint);
    }
    else if (lHint & VIEWUPDATEHINT_RESOURCETYPECHANGED)
    {
        _UpdateEntries(NULL);
    }
    else if (lHint & VIEWUPDATEHINT_RESOURCEADDED)
    {
        ResourceBlob *pData = (ResourceBlob*)pHint;
        if (pData)
        {
            // Make our own copy, since we're storing this away.
            ResourceBlob *pDataClone = new ResourceBlob(*pData);
            _InsertItem(pDataClone);
            _ReevaluateRecency(pDataClone);
            // Sort
            SortItems(ColumnSort, _iSortColumn);
        }
    }
    else if (lHint & VIEWUPDATEHINT_RESOURCEDELETED)
    {
        const ResourceBlob *pData = (ResourceBlob*)pHint;
        if (pData)
        {
            // Re-evaluate recency while the pData is still valid.
            _ReevaluateRecency(pData);
            // This will free the memory for pData.
            _DeleteItem(pData);
        }
    }
}

HRESULT CResourceListCtrl::_UpdateEntries(CFile *pFile)
{
    HRESULT hr = E_FAIL;
    // Clear out old items.
    DeleteAllItems();

    // Temporary cache of ResourceBlob's from the resource map enumerator
    CArray<ResourceBlob *, ResourceBlob*> resources;

    // Enumerate the resources that are of our type:
    // (we might optionally have been given a file handle... use it, as it means the
    // resource map file is open).
    HANDLE hFileTemp = pFile ? pFile->m_hFile : INVALID_HANDLE_VALUE;
    ResourceEnumerator *pEnum;
    if (SUCCEEDED(theApp.GetResourceMap().CreateEnumerator(&theApp._resourceRecency, ResourceTypeToFlag(GetType()), hFileTemp, &pEnum)))
    {
        ResourceBlob *pData;
        while (S_OK == pEnum->Next(&pData))
        {
            // Ownership of pData is temporarily transfered to the array.
            resources.Add(pData);
        }
        delete pEnum;
    }

    if (resources.GetCount() > 0)
    {
        theApp.LogInfo("Found %d resources for type %x", resources.GetCount(), GetType());
        if (_bFirstTime)
        {
            _InitColumns();
            ChangeViewMode(GetDefaultViewMode());
            _bFirstTime = FALSE;
            _OnInitListView((int)resources.GetCount());
        }
        _bDidInitialUpdate = TRUE;

        SetRedraw(FALSE);

        for (INT_PTR i = 0; i <= resources.GetUpperBound(); i++)
        {
            // Ownership of prd is transfered to listview:
            _InsertItem(resources.GetAt(i));
        }

        // Sort
        _iSortColumn = ColumnNumber;
        SortItems(ColumnSort, _iSortColumn);
        SetRedraw(TRUE);
        hr = S_OK;
    }
    else
    {
        theApp.LogInfo("Found zero resources for type %x", GetType());
    }
    return hr;
}

int CResourceListCtrl::_GetItemForResource(const ResourceBlob *pData)
{
    int cItems = GetItemCount();
    BOOL fFound = FALSE;
    int i = 0;
    for (; !fFound && (i < cItems); i++)
    {
        fFound = (_GetResourceForItem(i) == pData);
    }
    ASSERT(fFound); // Otherwise someone called us with a bad resource data.
    return i - 1;
}


// CResourceListCtrl diagnostics

#ifdef _DEBUG
void CResourceListCtrl::AssertValid() const
{
	CListCtrl::AssertValid();
}

void CResourceListCtrl::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);
}


CResourceListDoc* CResourceListCtrl::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResourceListDoc)));
    return (CResourceListDoc*)m_pDocument;
}

#endif //_DEBUG


// CResourceListCtrl message handlers
