// QuickScripts.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "QuickScripts.h"
#include "ScriptDocument.h"
#include "PicResource.h"
#include <boost/format.hpp>

using namespace sci;
using namespace std;

#define MAX_VIEWS (ID_GOTOVIEW10 - ID_GOTOVIEW1 + 1)

// CQuickScripts dialog
CQuickScripts::CQuickScripts(CWnd* pParent /*=NULL*/)
	: CExtResizableDialog(CQuickScripts::IDD, pParent)
{
    _bFirstTime = true;
    _pDoc = NULL;
    _wPic = InvalidResourceNumber;
    _wNorth = 0;
    _wEast = 0;
    _wSouth = 0;
    _wWest = 0;
    _viewNumbers.resize(MAX_VIEWS);
}

CQuickScripts::~CQuickScripts()
{
}

#define STR_NOPIC "No Pic."

// All this to handle the user pressing enter on a listview item.
BOOL CQuickScripts::PreTranslateMessage(MSG* pMsg)
{
    BOOL fRet = FALSE;
    if (pMsg->message == WM_KEYDOWN)
    {
        if ((GetFocus() == &m_wndList) && (pMsg->wParam == VK_RETURN))
        {
            // Get the selection
            int nItem = -1;
            POSITION pos = m_wndList.GetFirstSelectedItemPosition();
            if (pos != NULL)
            {
                _OpenItem(m_wndList.GetNextSelectedItem(pos));
            }
            fRet = TRUE; // Handled
        }
    }
    if (!fRet)
    {
        fRet = __super::PreTranslateMessage(pMsg);
    }
    return fRet;
}

void CQuickScripts::_PrepareViewMenu(int cItems)
{
    if (m_wndButtonViews.m_menu == NULL)
    {
        CMenu menu;
        if (menu.LoadMenu(IDR_MENUVIEWS))
        {
            // We only need to do this once...
            m_wndButtonViews.m_menu.Attach(menu.Detach());
        }
    }
    if (m_wndButtonViews.m_menu)
    {
        UINT nID;
        if (_GetMenuItem("placeholder", &m_wndButtonViews.m_menu, &nID))
        {
            CMenu *pMenuSub = m_wndButtonViews.m_menu.GetSubMenu(nID);

            // Clear the menu out first.
            UINT cItemsInMenu = pMenuSub->GetMenuItemCount();
            while (cItemsInMenu > 0)
            {
                pMenuSub->RemoveMenu(0, MF_BYPOSITION);
                cItemsInMenu--;
            }

            // And rebuild
            for (int iIndex = 0; iIndex < cItems; iIndex++)
            {
                MENUITEMINFO mii = { 0 };
                mii.cbSize = sizeof(mii);
                mii.fMask = MIIM_ID | MIIM_STRING;
                mii.wID = ID_GOTOVIEW1 + iIndex;
                mii.dwTypeData = "foo";
                pMenuSub->InsertMenuItem(ID_GOTOVIEW1 + iIndex, &mii, FALSE);
                //menu.InsertMenuItem(ID_GOTOVIEW1 + iIndex, &mii, FALSE);
            }
        }
    }
    m_wndButtonViews.EnableWindow(cItems > 0);
}

void CQuickScripts::_PrepareViewCommands(int iIndex, const ViewResource &view, int nLoop, int nCel)
{
    ASSERT(iIndex < 10);
    UINT nID = ID_GOTOVIEW1 + iIndex;
    _viewNumbers[iIndex] = view.GetResourceNumber();
    // Ensure we have a command entry for this.
    CExtCmdItem *pCmdItem;
	pCmdItem = g_CmdManager->CmdGetPtr(theApp._pszCommandProfile, nID);
    if (pCmdItem == NULL)
    {
        pCmdItem = g_CmdManager->CmdAllocPtr(theApp._pszCommandProfile, nID);
    }
    // Update the command entry with an icon and text
    if (pCmdItem)
    {
        CBitmap bitmap;
        // Note: if the index is out of bounds, it will return a NULL HBITMAP
        bitmap.Attach(view.GetBitmap(MAKE_INDEX(nLoop, nCel), 24, 24));
        if ((HBITMAP)bitmap == NULL)
        {
            // Load an all black bitmap, to indicate the loop/cel are invalid.
            bitmap.LoadBitmap(IDB_BITMAPNULL);
        }
        CExtBitmap extBitmap;
        extBitmap.FromBitmap((HBITMAP)bitmap);
        g_CmdManager->CmdSetIcon(theApp._pszCommandProfile, nID, extBitmap, RGB(255, 255, 255), CRect(0, 0, 24, 24));
        std::string name = theApp.GetResourceMap().FigureOutName(RS_VIEW, view.GetResourceNumber());
        pCmdItem->m_sMenuText = name.c_str();
        pCmdItem->m_sTipTool = pCmdItem->m_sMenuText;
    }
}

void CQuickScripts::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_LISTSCRIPTS, m_wndList);

    DDX_Control(pDX, IDC_BUTTONVIEWS, m_wndButtonViews);

    DDX_Control(pDX, IDC_GOTOPIC, m_wndGotoPic);
    m_wndGotoPic.m_bUseStdCommandNotification = true;
    m_wndGotoPic.SetWindowText(STR_NOPIC);
    m_wndGotoPic.EnableWindow(FALSE);
    DDX_Control(pDX, IDC_STATICPIC, m_wndPic);

    DDX_Control(pDX, IDC_NORTH, m_wndNorth);
    m_wndNorth.m_bUseStdCommandNotification = true;
    m_wndNorth.EnableWindow(FALSE);
    DDX_Control(pDX, IDC_EAST, m_wndEast);
    m_wndEast.m_bUseStdCommandNotification = true;
    m_wndEast.EnableWindow(FALSE);
    DDX_Control(pDX, IDC_SOUTH, m_wndSouth);
    m_wndSouth.m_bUseStdCommandNotification = true;
    m_wndSouth.EnableWindow(FALSE);
    DDX_Control(pDX, IDC_WEST, m_wndWest);
    m_wndWest.m_bUseStdCommandNotification = true;
    m_wndWest.EnableWindow(FALSE);

    DDX_Control(pDX, IDC_STATICSCRIPTNUM, m_wndScriptNum);
}

BOOL CQuickScripts::OnInitDialog()
{
    BOOL fRet = __super::OnInitDialog();
    ShowSizeGrip(FALSE);
    AddAnchor(IDC_LISTSCRIPTS, CPoint(0, 0), CPoint(100, 100));
    AddAnchor(IDC_GOTOPIC, CPoint(0, 0), CPoint(100, 0));
    AddAnchor(IDC_BUTTONVIEWS, CPoint(100, 0), CPoint(100, 0));
    return fRet;
}


BEGIN_MESSAGE_MAP(CQuickScripts, CExtResizableDialog)
    ON_NOTIFY(NM_DBLCLK, IDC_LISTSCRIPTS, OnItemDoubleClick)
    ON_NOTIFY(LVN_KEYDOWN, IDC_LISTSCRIPTS, OnItemEnter)
    ON_COMMAND(IDC_GOTOPIC, OnGotoPic)
    ON_COMMAND_EX(IDC_NORTH, OnGotoRoom)
    ON_COMMAND_EX(IDC_EAST, OnGotoRoom)
    ON_COMMAND_EX(IDC_SOUTH, OnGotoRoom)
    ON_COMMAND_EX(IDC_WEST, OnGotoRoom)
    ON_UPDATE_COMMAND_UI(ID_VIEWMENU, OnUpdateAlwaysOn)
    ON_COMMAND_RANGE(ID_GOTOVIEW1, ID_GOTOVIEW10, OnGotoView)
END_MESSAGE_MAP()


// CQuickScripts message handlers


void CQuickScripts::_OpenItem(int iItem)
{
    if (iItem != -1) // Could be -1 if user clicked on the background.
    {
        TCHAR szName[MAX_PATH];
        LVITEM item = { 0 };
        item.iItem = iItem;
        item.pszText = szName;
        item.cchTextMax = ARRAYSIZE(szName);
        item.mask = LVIF_TEXT;
        if (m_wndList.GetItem(&item))
        {
            if (iItem < _nFirstScript)
            {
                theApp.OpenScriptHeader(szName);
            }
            else
            {
                theApp.OpenScript(szName);
            }
        }
    }
}

void CQuickScripts::OnItemDoubleClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW *pNMLV = (NMLISTVIEW*)pNMHDR;
    _OpenItem(pNMLV->iItem);
    *pResult = 0;
}

void CQuickScripts::OnItemEnter(NMHDR* pNMHDR, LRESULT* pResult)
{
    /*NMLVKEYDOWN *pNMKD = (NMLVKEYDOWN*)pNMHDR;
    if (pNMKD->wVKey == VK_RETURN)
    {
        int x = 0;
        //_OpenItem(m_wndList.GetSelectedItem());
    }*/
    *pResult = 0;
}

void CQuickScripts::_InitColumns()
{
    // Just one column
    LVCOLUMN col = { 0 };
    col.mask = LVCF_FMT | LVCF_ORDER | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 
    col.iOrder = 0;
    col.iSubItem = 0;
    col.pszText = TEXT("Quick links");
    col.cx = 100;
    col.fmt = LVCFMT_LEFT;
    m_wndList.InsertColumn(0, &col);
}


void CQuickScripts::_AddFilesOfType(int &iItem, PCTSTR pszWildcard)
{
    // REVIEW: Could we just ask for the list of scripts instead?
    std::string srcFolder = theApp.GetResourceMap().GetSrcFolder();
    srcFolder += pszWildcard;
    WIN32_FIND_DATA findData = { 0 };
    HANDLE hFFF = FindFirstFile(srcFolder.c_str(), &findData);
    if (hFFF != INVALID_HANDLE_VALUE)
    {
        BOOL fOk = TRUE;
        while(fOk)
        {
            PTSTR pszFileName = PathFindFileName(findData.cFileName);
            *PathFindExtension(findData.cFileName) = 0; // Chop off file extension
            m_wndList.InsertItem(iItem++, pszFileName);
            fOk = FindNextFile(hFFF, &findData);
        }
        FindClose(hFFF);
    }
}

void CQuickScripts::Initialize()
{
    if (_bFirstTime)
    {
        _UpdateEntries();
    }
}

//
// Predicate for sorting syntax nodes by position.
//
bool SortScriptsIntelligently(ScriptId &s1, ScriptId &s2)
{
    std::string title1 = s1.GetTitle();
    std::string title2 = s2.GetTitle();
    bool isRoom1 = (0 == title1.compare(0, 2, "rm"));
    bool isRoom2 = (0 == title2.compare(0, 2, "rm"));
    if (isRoom1 && !isRoom2)
    {
        return true;
    }
    else if (isRoom2 && !isRoom1)
    {
        return false;
    }
    else
    {
        return title1 < title2;
    }
}

void CQuickScripts::_UpdateEntries()
{
    // Clear out old items.
    m_wndList.DeleteAllItems();
    if (_bFirstTime)
    {
        _bFirstTime = false;
        _InitColumns();
    }

    // Start with the headers.
    int iItem = 0;
    m_wndList.InsertItem(iItem++, TEXT("sci.sh"));
    m_wndList.InsertItem(iItem++, TEXT("keys.sh"));

    // Then the headers in the src directory.
    _AddFilesOfType(iItem, TEXT("\\*.sh"));
    _nFirstScript = iItem;

    std::vector<ScriptId> scripts;
    theApp.GetResourceMap().GetAllScripts(scripts);
    sort(scripts.begin(), scripts.end(), SortScriptsIntelligently);
    for (size_t i = 0; i < scripts.size(); ++i)
    {
        m_wndList.InsertItem(iItem++, scripts[i].GetTitle().c_str());
    }
}

void _EnableDirection(std::string direction, CWnd &wnd, WORD wNumber)
{
    if (wNumber)
    {
        direction += theApp.GetResourceMap().FigureOutName(RS_SCRIPT, wNumber);
        wnd.SetWindowText(direction.c_str());
        wnd.EnableWindow(TRUE);
    }
    else
    {
        wnd.SetWindowText(direction.c_str());
        wnd.EnableWindow(FALSE);
    }
}

void CQuickScripts::_ResetUI()
{
    bool fFound = false;
    _wPic = InvalidResourceNumber;
    _wNorth = _wEast = _wSouth = _wWest = 0;
    int iViewIndex = 0;

    // Display the script number.
    std::stringstream ss;
    WORD wNum = _pDoc->GetScriptId().GetResourceNumber();
    if (wNum == InvalidResourceNumber)
    {
        ss << "Unknown script number.";
    }
    else
    {
        ss << "Script " << boost::format("%03d") % wNum;
    }
    m_wndScriptNum.SetWindowText(ss.str().c_str());

    // Update the bitmap.
    // Get the script object for this script.
    SCIClassBrowser &browser = theApp.GetResourceMap().GetClassBrowser();
    ClassBrowserLock lock(browser);
    if (lock.TryLock()) // Don't hang UI... if we can't get the lock, just skip it.
    {
        bool fFoundRoom = false;
        const Script *pScript = browser.GetLKGScript(_pDoc->GetScriptId().GetFullPath().c_str());
        if (pScript)
        {
            // Find the Rm instance
            const ClassVector &classes = pScript->GetClasses();
            for (size_t i = 0; i < classes.size(); i++)
            {
                ClassDefinition *pClass = classes[i];
                if (pClass->IsInstance())
                {
                    if (!fFoundRoom && browser.IsSubClassOf(pClass->GetSuperClass().c_str(), "Rm"))
                    {
                        // Figure out the pic number of the Rm
                        if (browser.GetPropertyValue("picture", pClass, &_wPic))
                        {
                            // Create the pic resource
                            auto_ptr<PicResource> pic(static_cast<PicResource*>(theApp.GetResourceMap().CreateResourceFromNumber(RS_PIC, _wPic)));
                            if (pic.get())
                            {
                                CRect rc;
                                m_wndPic.GetClientRect(&rc);
                                CBitmap bitmap;
                                bitmap.Attach(pic.get()->GetVisualBitmap(rc.Width(), rc.Height()));
                                m_wndPic.FromBitmap((HBITMAP)bitmap, rc.Width(), rc.Height());
                                if (!m_wndPic.IsWindowVisible())
                                {
                                    m_wndPic.ShowWindow(SW_SHOW);
                                }
                                stringstream ss;
                                ss << "pic " << setfill('0') << setw(3) << (int)_wPic;
                                m_wndGotoPic.SetWindowText(ss.str().c_str());
                                m_wndGotoPic.EnableWindow(TRUE);
                                fFound = true;
                            }
                        }
                        browser.GetPropertyValue("north", pClass, &_wNorth);
                        browser.GetPropertyValue("east", pClass, &_wEast);
                        browser.GetPropertyValue("south", pClass, &_wSouth);
                        browser.GetPropertyValue("west", pClass, &_wWest);
                        fFoundRoom = true;
                    }

                    // Also do views, if we have room left
                    if (iViewIndex < MAX_VIEWS)
                    {
                        if (browser.IsSubClassOf(pClass->GetSuperClass().c_str(), "View"))
                        {
                            // We got a live one.
                            WORD wView;
                            browser.GetPropertyValue("view", pClass, &wView);
                            WORD wLoop;
                            browser.GetPropertyValue("loop", pClass, &wLoop);
                            WORD wCel;
                            browser.GetPropertyValue("cel", pClass, &wCel);

                            // Create the pic resource
                            auto_ptr<ViewResource> view(static_cast<ViewResource*>(theApp.GetResourceMap().CreateResourceFromNumber(RS_VIEW, wView)));
                            if (view.get())
                            {
                                _PrepareViewCommands(iViewIndex, *view, wLoop, wCel);
                                iViewIndex++;
                            }
                        }
                    }
                }
            }
        }
    }
    if (!fFound)
    {
        m_wndGotoPic.SetWindowText(STR_NOPIC);
        m_wndGotoPic.EnableWindow(FALSE);
        m_wndPic.ShowWindow(SW_HIDE);
    }
    _EnableDirection("North: ", m_wndNorth, _wNorth);
    _EnableDirection("East: ", m_wndEast, _wEast);
    _EnableDirection("South: ", m_wndSouth, _wSouth);
    _EnableDirection("West: ", m_wndWest, _wWest);
    _PrepareViewMenu(iViewIndex);
}

void CQuickScripts::OnGotoPic()
{
    if (_wPic != InvalidResourceNumber)
    {
        theApp.OpenMostRecentResource(RS_PIC, _wPic);
    }
}

void CQuickScripts::OnGotoView(UINT nID)
{
    theApp.OpenMostRecentResource(RS_VIEW, _viewNumbers[nID - ID_GOTOVIEW1]);
}

BOOL CQuickScripts::OnGotoRoom(UINT nID)
{
    WORD wScript = 0;
    switch (nID)
    {
    case IDC_NORTH:
        wScript = _wNorth;
        break;
    case IDC_EAST:
        wScript = _wEast;
        break;
    case IDC_SOUTH:
        wScript = _wSouth;
        break;
    case IDC_WEST:
        wScript = _wWest;
        break;
    default:
        ASSERT(FALSE);
        break;
    }
    if (wScript != 0)
    {
        theApp.OpenScript(wScript);
    }
    return TRUE;
}

void CQuickScripts::SetDocument(CDocument *pDoc)
{
    ASSERT(pDoc == NULL || (pDoc->IsKindOf(RUNTIME_CLASS(CScriptDocument))));
    _pDoc = static_cast<CScriptDocument*>(pDoc);
    if (_pDoc)
    {
        _pDoc->AddNonViewClient(this);
        _ResetUI();

        // See if we should update the script list.
        std::string newSrcFolder = theApp.GetResourceMap().GetSrcFolder();
        if (0 != _srcFolder.compare(newSrcFolder))
        {
            // The game has been reloaded.
            _srcFolder = newSrcFolder;
            _UpdateEntries();
        }
    }
}

// INonViewClient
void CQuickScripts::UpdateNonView(LPARAM lHint)
{
    if (lHint & (VIEWUPDATEHINT_SCRIPTSAVED | VIEWUPDATEHINT_SCRIPTCONVERTED))
    {
        // Go update ourselves again (might not be our script, but oh well)
        _ResetUI();
    }
}
