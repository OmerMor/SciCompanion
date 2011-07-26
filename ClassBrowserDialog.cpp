// ClassBrowserDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ClassBrowserDialog.h"
#include "Compile\ScriptOM.h"
#include "ResourceMap.h"
#include "ClassBrowser.h"
#include "ClassBrowserInfo.h"
#include ".\classbrowserdialog.h"

using namespace sci;

// CClassBrowserDialog dialog

IMPLEMENT_DYNAMIC(CClassBrowserDialog, CDialog)
CClassBrowserDialog::CClassBrowserDialog(const SCIClassBrowser *pBrowser, CWnd* pParent /*=NULL*/)
	: CDialog(CClassBrowserDialog::IDD, pParent)
{
    _iIndexPropSC = 0;
    _iIndexMethodSC = 0;
    _pBrowser = pBrowser;
    _fInitialized = false;
    // We need to lock the browser for the duration of the dialog, since we store some of its
    // data in the treeview.
    _pBrowser->Lock();
}

CClassBrowserDialog::~CClassBrowserDialog()
{
    _pBrowser->Unlock();
    m_font.DeleteObject();
}


void InitPropLogFont(LOGFONT *plogFont)
{
    ZeroMemory(plogFont, sizeof(*plogFont));
    StringCchCopy(plogFont->lfFaceName, ARRAYSIZE(plogFont->lfFaceName), TEXT("Arial"));
    plogFont->lfHeight = -12;
    plogFont->lfWeight = FW_BOLD;
    plogFont->lfCharSet = DEFAULT_CHARSET;
    plogFont->lfPitchAndFamily = VARIABLE_PITCH | FF_DONTCARE;
}

void CClassBrowserDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    if (!_fInitialized)
    {
        _fInitialized = true;
        DDX_Control(pDX, IDC_PROPERTIES, m_wndProperties);
        DDX_Control(pDX, IDC_METHODS, m_wndMethods);
        DDX_Control(pDX, IDC_TREE_CLASS, m_wndTree);
        DDX_Control(pDX, IDC_STATIC_CLASSNAME, m_wndClassName);

        LOGFONT logFont;
        InitPropLogFont(&logFont);
        m_font.CreateFontIndirect(&logFont);
        SetFont(&m_font);
        m_wndProperties.SetFont(&m_font);
        m_wndMethods.SetFont(&m_font);
        m_wndClassName.SetFont(&m_font);

        if (_imagelist.Create(IDB_CLASSBROWSER, 16, 0, RGB(255, 0, 255)))
        {
            m_wndTree.SetImageList(&_imagelist, TVSIL_NORMAL);
        }

        _PopulateTree();
        DDX_Control(pDX, IDC_INSTANCES, m_wndInstances);

        CRect rect;
        GetDlgItem(IDC_BROWSER)->GetWindowRect(&rect);
        ScreenToClient(&rect);
        m_wndBrowser.CreateControl(CLSID_WebBrowser, "Documentation", WS_VISIBLE | WS_CHILD, rect, this, AFX_IDW_PANE_FIRST); 
    }
}

//
// Takes a stream of html and shows it in the browser control.
//
void CClassBrowserDialog::_ShowHTML(const std::stringstream &html)
{
    const std::string tempFile = MakeFile(html, "foo.html");
    IUnknown *pUnk = m_wndBrowser.GetControlUnknown(); 
    IWebBrowser *pBrowser; 
    HRESULT hr=pUnk->QueryInterface(IID_PPV_ARGS(&pBrowser));
    if (SUCCEEDED(hr))
    {
        CString url(tempFile.c_str()); 
        BSTR bUrl = url.AllocSysString(); 
        COleVariant vNull((LPCTSTR)NULL, VT_BSTR); 
        hr= pBrowser->Navigate(bUrl, &vNull, &vNull, &vNull, &vNull);
        pBrowser->Release(); 
    }
}

void CClassBrowserDialog::_ShowClass(const ClassDefinition *pClass)
{
    std::stringstream build;
    build << "<body style=\"background-color:#FF9933\">";
    build << "<h2>" << pClass->GetName() << "</h2>";
    build << "</body>";
    _ShowHTML(build);
}

class ShowParam : public std::unary_function<const FunctionParameterPtr, void>
{
public:
    ShowParam(std::stringstream &build) : _build(build) {}
    void operator()(const FunctionParameterPtr pParam) const
    {
        _build << "<li><b>" << pParam->GetName() << "</b> " /*<< pParam->GetDocDescription()*/;
    }
private:
    std::stringstream &_build;
};

void CClassBrowserDialog::_ShowFunction(const FunctionBase *pFunc)
{
    std::stringstream build;
    build << "<body style=\"background-color:#FF9933\">";
    build << "<h2>" << pFunc->GetName() << "</h2>";
    const FunctionSignatureVector &sigs = pFunc->GetSignatures();
    if (!sigs.empty())
    {
        const FunctionParameterVector &params = sigs[0]->GetParams();
        for_each(params.begin(), params.end(), ShowParam(build));
        build << "</ul>";
    }
    build << "</body>";
    _ShowHTML(build);
}


void CClassBrowserDialog::_PopulateTree(HTREEITEM hItemParent, const SCIClassBrowserNode *pBrowserInfoParent)
{
    // Add a node for each subclass.
    HTREEITEM hPrevious = TVI_FIRST;
    const std::vector<SCIClassBrowserNode*> &subClasses = pBrowserInfoParent->GetSubClasses();
    for (size_t i = 0; i < subClasses.size(); i++)
    {
        const SCIClassBrowserNode *pBrowserInfo = subClasses[i];
        if (pBrowserInfo && !pBrowserInfo->GetClassDefinition()->IsInstance()) // Don't add instances to tree.
        {
            TVINSERTSTRUCT insert = { 0 };
            insert.hParent = hItemParent;
            insert.hInsertAfter = hPrevious;
            insert.item.pszText = const_cast<LPSTR>((LPCSTR)pBrowserInfo->GetName().c_str());
            insert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE;
            insert.item.lParam = (LPARAM)pBrowserInfo;
            insert.item.iImage = 0;
            hPrevious = m_wndTree.InsertItem(&insert);
            if (hPrevious)
            {
                _PopulateTree(hPrevious, pBrowserInfo);
            }
            // Expand after we have children.
            m_wndTree.Expand(hPrevious, TVE_EXPAND);
        }
    }
}

void CClassBrowserDialog::_PopulateTree()
{
    HTREEITEM hPrevious = TVI_FIRST;
    for (size_t i = 0; i < _pBrowser->GetNumRoots(); i++)
    {
        const SCIClassBrowserNode *pBrowserRoot = _pBrowser->GetRoot(i);
        if (pBrowserRoot)
        {
            TVINSERTSTRUCT insert = { 0 };
            insert.hParent = TVI_ROOT;
            insert.hInsertAfter = hPrevious;
            insert.item.pszText = const_cast<LPSTR>((LPCSTR)pBrowserRoot->GetName().c_str());
            insert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE;
            insert.item.lParam = (LPARAM)pBrowserRoot;
            insert.item.iImage = 0;
            hPrevious = m_wndTree.InsertItem(&insert);
            if (hPrevious)
            {
                _PopulateTree(hPrevious, pBrowserRoot);
            }
            // Expand after we have children.
            m_wndTree.Expand(hPrevious, TVE_EXPAND);
        }
    }
}


//
// Assembles all the scripts in the src directory, and provides them to the class browser.
//


BEGIN_MESSAGE_MAP(CClassBrowserDialog, CDialog)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CLASS, OnTvnSelchangedTreeClass)
    ON_LBN_SELCHANGE(IDC_METHODS, OnSelChange)
    ON_LBN_SELCHANGE(IDC_INSTANCES, OnInstanceSelChange)
    ON_WM_DRAWITEM()
    ON_BN_CLICKED(IDC_BUTTONPRINT, OnBnClickedButtonprint)
END_MESSAGE_MAP()



// CClassBrowserDialog message handlers

void CClassBrowserDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT pDrawItemStruct)
{
    CDC dc;
    dc.Attach(pDrawItemStruct->hDC);

    // Save these values to restore them when done drawing.
    COLORREF crOldTextColor = dc.GetTextColor();
    COLORREF crOldBkColor = dc.GetBkColor();
    int iMode = dc.GetBkMode();

    if (pDrawItemStruct->itemState & ODS_SELECTED)
    {
        CBrush brush;
        brush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        dc.FillRect(&pDrawItemStruct->rcItem, &brush);
        dc.SetBkMode(TRANSPARENT);
    }

    if (nIDCtl == IDC_PROPERTIES)
    {

        ClassProperty *pProperty = (ClassProperty *)pDrawItemStruct->itemData;
        if (pProperty)
        {
            CRect rcName;
            dc.DrawText(pProperty->GetName().c_str(), &rcName, DT_SINGLELINE | DT_CALCRECT);

            COLORREF crName = ((int)pDrawItemStruct->itemID >= _iIndexPropSC) ? RGB(128, 128, 128) : RGB(0, 0, 0);
            dc.SetTextColor(crName);
            CRect rcProp = pDrawItemStruct->rcItem;
            rcProp.left += 3; // Indent a bit
            dc.DrawText(pProperty->GetName().c_str(), &rcProp, DT_SINGLELINE);
            
            // Put the prop value a little after.
            dc.SetTextColor(RGB(0, 0, 255));
            rcProp.left += rcName.Width() + 6;
            CString strProp;
            pProperty->GetValue().ToString(strProp);
            dc.DrawText(strProp, &rcProp, DT_SINGLELINE);
        }
    }
    else if (nIDCtl == IDC_METHODS)
    {
        MethodDefinition *pMethod = (MethodDefinition *)pDrawItemStruct->itemData;
        if (pMethod)
        {
            CRect rcBlob;
            dc.DrawText(pMethod->GetName().c_str(), &rcBlob, DT_SINGLELINE | DT_CALCRECT);

            // Draw in grey or black
            COLORREF crName = ((int)pDrawItemStruct->itemID >= _iIndexMethodSC) ? RGB(128, 128, 128) : RGB(0, 0, 0);
            dc.SetTextColor(crName);
            CRect rcProp = pDrawItemStruct->rcItem;
            rcProp.left += 3; // Indent a bit
            dc.DrawText((pMethod->GetName() + TEXT("(")).c_str(), &rcProp, DT_SINGLELINE);

            // Now the parameters
            // Put the prop value a little after.
            const FunctionSignatureVector &signatures = pMethod->GetSignatures();
            if (!signatures.empty())
            {
                COLORREF crParam = ((int)pDrawItemStruct->itemID >= _iIndexMethodSC) ? RGB(128, 128, 255) : RGB(0, 0, 255);
                dc.SetTextColor(crParam);
                rcProp.left += rcBlob.Width() + 5;
                const FunctionParameterVector &params = signatures[0]->GetParams();
                for (size_t i = 0; i < params.size(); i++)
                {
                    dc.DrawText(params[i]->GetName().c_str(), &rcBlob, DT_SINGLELINE | DT_CALCRECT);
                    dc.DrawText(params[i]->GetName().c_str(), &rcProp, DT_SINGLELINE);
                    rcProp.left += rcBlob.Width() + 3;
                }
            }
            // Closing paren
            dc.SetTextColor(crName);
            dc.DrawText(TEXT(")"), &rcProp, DT_SINGLELINE);
        }
    }
    dc.SetTextColor(crOldTextColor);
    dc.SetBkColor(crOldBkColor);
    dc.SetBkMode(iMode);
    dc.Detach();
}

void CClassBrowserDialog::_OnClassOrInstanceSelected(const SCIClassBrowserNode *pBrowserInfo)
{
    // Update properties
    m_wndProperties.ResetContent();
    ClassPropertyVector properties;
    _iIndexPropSC = pBrowserInfo->ComputeAllProperties(properties);
    for (size_t i = 0; i < properties.size(); i++)
    {
        ClassProperty *pProperty = properties[i];
        std::string strFullName = pProperty->GetName();
        strFullName += pProperty->GetValue().ToString();
        int iIndex = m_wndProperties.InsertString(-1, strFullName.c_str());
        m_wndProperties.SetItemDataPtr(iIndex, pProperty);
    }

    // TODO: restore the listbox position if possible. It gets reset to the top whenever you choose something.

    TCHAR szClass[MAX_PATH];
    StringCchPrintf(szClass, ARRAYSIZE(szClass), TEXT("%s  (implemented in %s)"), pBrowserInfo->GetName().c_str(), pBrowserInfo->GetClassDefinition()->GetOwnerScript()->GetName().c_str());
    m_wndClassName.SetWindowText(szClass);
}

void CClassBrowserDialog::OnTvnSelchangedTreeClass(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    SCIClassBrowserNode *pBrowserInfo = (SCIClassBrowserNode*)pNMTreeView->itemNew.lParam;
    if (pBrowserInfo)
    {
        _OnClassOrInstanceSelected(pBrowserInfo);

        // Update methods
        m_wndMethods.ResetContent();
        MethodVector methods;
        _iIndexMethodSC = pBrowserInfo->ComputeAllMethods(methods);
        for (size_t i = 0; i < methods.size(); i++)
        {
            MethodDefinition *pMethod = methods[i];
            int iIndex = m_wndMethods.InsertString(-1, pMethod->ToString().c_str());
            m_wndMethods.SetItemDataPtr(iIndex, pMethod);
        }

        // Update instances of.
        BOOL fNone = TRUE;
        m_wndInstances.ResetContent();
        const std::vector<SCIClassBrowserNode*> &subClasses = pBrowserInfo->GetSubClasses();
        for (size_t i = 0; i < subClasses.size(); i++)
        {
            const SCIClassBrowserNode *pSubClasses = subClasses[i];
            if (pSubClasses)
            {
                if (pSubClasses->GetClassDefinition()->IsInstance())
                {
                    fNone = FALSE;
                    // This is an instance.
                    TCHAR szInstance[MAX_PATH];
                    StringCchPrintf(szInstance,
                        ARRAYSIZE(szInstance),
                        "%s (%s)",
                        pSubClasses->GetName().c_str(),
                        pSubClasses->GetClassDefinition()->GetOwnerScript()->GetName().c_str());
                    int nIndex = m_wndInstances.InsertString(-1, szInstance);
                    m_wndInstances.SetItemDataPtr(nIndex, const_cast<SCIClassBrowserNode*>(pSubClasses));
                }
            }
        }
        if (fNone)
        {
            m_wndInstances.InsertString(-1, TEXT("(None...)"));
        }

        _ShowClass(pBrowserInfo->GetClassDefinition());
    }
    *pResult = 0;
}

void CClassBrowserDialog::OnSelChange()
{
    int iSel = (int)m_wndMethods.SendMessage(LB_GETCURSEL, 0, 0);
    if (iSel != LB_ERR)
    {
        MethodDefinition *pMethod = reinterpret_cast<MethodDefinition*>(m_wndMethods.GetItemDataPtr(iSel));
        _ShowFunction(pMethod);
    }
}

void CClassBrowserDialog::OnInstanceSelChange()
{
    int nSel = m_wndInstances.GetCurSel();
    if (nSel != LB_ERR)
    {
        const SCIClassBrowserNode *pNode = reinterpret_cast<const SCIClassBrowserNode*>(m_wndInstances.GetItemDataPtr(nSel));
        _OnClassOrInstanceSelected(pNode);
    }
}

void CClassBrowserDialog::OnBnClickedButtonprint()
{
}
