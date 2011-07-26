// SCIPicEditor.cpp : Defines the class behaviors for the application.
//

//
// SCICompanion:
// Major architectural issues:
//  - ScriptOM: assignment operators and copy constructors modify the source object
//  - ScriptOM: getting methods/properties/etc from these objects exposes too much of the internals.
//


#include "stdafx.h"

#include "SCIPicEditor.h"
#include "MainFrm.h"
#include "PicChildFrame.h"


#include "PicDoc.h"
#include "PicResource.h"
#include "PicView.h"

#include "VocabDoc.h"
#include "VocabChildFrame.h"
#include "VocabView.h"

#include "ResourceListDoc.h"
#include "ResourceManagerFrame.h"
#include "ResourceManagerView.h"

#include "ScriptFrame.h"
#include "ScriptDocument.h"
#include "ScriptView.h"

#include "ViewChildFrame.h"
#include "ViewDoc.h"
#include "RasterView.h"

#include "TextChildFrame.h"
#include "TextDoc.h"
#include "TextView.h"

#include "SoundChildFrame.h"
#include "SoundDoc.h"
#include "SoundView.h"

#include "CursorChildFrame.h"
#include "CursorDoc.h"

#include "FontChildFrame.h"
#include "FontDoc.h"
#include "FontResource.h"

#include "RoomExplorerFrame.h"
#include "RoomExplorerDoc.h"
#include "RoomExplorerView.h"

#include "ViewResource.h"

#include "GamePropertiesDialog.h"

#include "ColoredToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const char c_szExecutableString[] = "Executable";
static const char c_szExeParametersString[] = "ExeCmdLineParameters";
static const char c_szDefaultExe[] = "sciv.exe";

//
// This allows us to hook up the slider bar, celdialogbar, and other "non views"
//
class CMultiDocTemplateWithNonViews : public CMultiDocTemplate
{
public:
    CMultiDocTemplateWithNonViews(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) : CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
    {
        
    }

    virtual void InitialUpdateFrame(CFrameWnd *pFrame, CDocument *pDoc, BOOL bMakeVisible)
    {
        CMDITabChildWnd *pPicFrame = (CMDITabChildWnd*)pFrame;
        if (pPicFrame)
        {
            pPicFrame->HookUpNonViews(pDoc);
        }

        __super::InitialUpdateFrame(pFrame, pDoc, bMakeVisible);
    }

};

//
// Handle parsing /log log.txt
//
class CSCICommandLineInfo : public CCommandLineInfo
{
public:
    CSCICommandLineInfo()
    {
        m_bLogFlag = FALSE;
    }
    void ParseParam(const TCHAR *pszParam, BOOL bFlag, BOOL bLast)
    {
        if (m_bLogFlag && !bFlag)
        {
            m_strLogFile = pszParam;
        }
        m_bLogFlag = (0 == lstrcmpi(pszParam, TEXT("log")));
        __super::ParseParam(pszParam, bFlag, bLast);
    }

    CString m_strLogFile;

private:
    BOOL m_bLogFlag;
};

// CSCIPicEditorApp

BEGIN_MESSAGE_MAP(CSCIPicEditorApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    ON_COMMAND(ID_HELP_ONLINETUTORIALS, OnTutorials)
    ON_COMMAND(ID_HELP_FORUMS, OnForums)
    ON_COMMAND(ID_HELP_ONLINESCIDOCUMENTATION, OnDocumentation)
    ON_COMMAND(ID_HELP_GETLATESTUPDATES, OnUpdates)
    ON_COMMAND(ID_HELP_EMAIL, OnEmail)
    ON_COMMAND(ID_HELP_SCICOMPANION, OnSCICompHelp)
    // Standard file based document commands
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
    ON_COMMAND(ID_RUNGAME, OnRunGame)
    ON_COMMAND(ID_ROOM_EXPLORER, OnRoomExplorer)
    ON_COMMAND(ID_FILE_CLOSEGAME, OnCloseGame)
    ON_COMMAND(ID_GAME_PROPERTIES, OnGameProperties)
    ON_UPDATE_COMMAND_UI(ID_FILE_CLOSEGAME, OnUpdateGameLoaded)
    ON_UPDATE_COMMAND_UI(ID_GAME_PROPERTIES, OnUpdateGameLoaded)
END_MESSAGE_MAP()


// CSCIPicEditorApp construction

CSCIPicEditorApp::CSCIPicEditorApp()
{
    // Place all significant initialization in InitInstance
    _pPicTemplate = NULL;
    _fGridLines = FALSE;
    _fScaleTracingImages = TRUE;
    _fDontShowTraceScaleWarning = FALSE;
    _fUseAutoSuggest = FALSE;
    _fAllowBraceSyntax = FALSE;
    _fAutoLoadGame = TRUE;
    _fDupeNewCels = TRUE;
    _fUseBoxEgo = FALSE;
    _fSCI01 = FALSE;
    _fBrowseInfo = FALSE;
    _fParamInfo = TRUE;
    _fCodeCompletion = TRUE;
    _fHoverTips = TRUE;
    _fPlayCompileErrorSound = TRUE;

    _pVocabTemplate = NULL;
    _pPicTemplate = NULL;

    _cxFakeEgo = 30;
    _cyFakeEgo = 48;
    _ptFakeEgo = CPoint(160, 120);
    _iView = 0;
    _pViewResource = NULL;
    _fObserveControlLines = false;
    _iDefaultZoomView = 4;
    _iDefaultZoomFont = 6;
    _iDefaultZoomCursor = 6;
    _iPicZoom = 2;
    _fDontCheckPic = FALSE;
    _pidlFolder = NULL;
    _fNoGdiPlus = FALSE;
    m_eHelpType = afxHTMLHelp;

    _pResourceDoc = NULL;
    _shownType = RS_VIEW;

}

CSCIPicEditorApp::~CSCIPicEditorApp()
{
    CoTaskMemFree(_pidlFolder);
}

// Takes ownership of pidl
void CSCIPicEditorApp::SetExportFolder(LPITEMIDLIST pidl)
{
    CoTaskMemFree(_pidlFolder);
    _pidlFolder = pidl;
}



// The one and only CSCIPicEditorApp object

CSCIPicEditorApp theApp;

void CSCIPicEditorApp::_LoadSettings(BOOL fReset)
{
    PCTSTR pszRegName = fReset ? TEXT("SomethingThatIsntThere") : m_pszAppName;
    _cyFakeEgo = GetProfileInt(pszRegName, TEXT("FakeEgoY"), 48);
    _cxFakeEgo = GetProfileInt(pszRegName, TEXT("FakeEgoX"), 30);
    _fUseBoxEgo = GetProfileInt(pszRegName, TEXT("UseBoxEgo"), FALSE);
    _fGridLines = GetProfileInt(pszRegName, TEXT("GridLines"), FALSE);
    _fScaleTracingImages = GetProfileInt(pszRegName, TEXT("ScaleTracingImages"), TRUE);
    _fDontShowTraceScaleWarning = GetProfileInt(pszRegName, TEXT("DontShowTraceScaleWarning"), FALSE);
    _fUseAutoSuggest = GetProfileInt(pszRegName, TEXT("UseAutoSuggest"), FALSE);
    _fAllowBraceSyntax = GetProfileInt(pszRegName, TEXT("AllowBraceSyntax"), FALSE) != 0;
    _fAutoLoadGame = GetProfileInt(pszRegName, TEXT("AutoLoadGame"), FALSE);
    _fDupeNewCels = GetProfileInt(pszRegName, TEXT("DupeNewCels"), TRUE);
    _fSCI01 = GetProfileInt(pszRegName, TEXT("SCI01Compat"), FALSE);
    _fBrowseInfo = GetProfileInt(pszRegName, TEXT("BrowseInfo"), FALSE);
    _fParamInfo = GetProfileInt(pszRegName, TEXT("ParamInfo"), TRUE);
    _fCodeCompletion = GetProfileInt(pszRegName, TEXT("CodeCompletion"), TRUE);
    _fHoverTips = GetProfileInt(pszRegName, TEXT("HoverTips"), TRUE);
    _fPlayCompileErrorSound = GetProfileInt(pszRegName, TEXT("CompileErrorSound"), TRUE);
}

void CSCIPicEditorApp::_SaveSettings()
{
    WriteProfileInt(m_pszAppName, TEXT("FakeEgoY"), _cyFakeEgo);
    WriteProfileInt(m_pszAppName, TEXT("FakeEgoX"), _cxFakeEgo);
    WriteProfileInt(m_pszAppName, TEXT("UseBoxEgo"), _fUseBoxEgo);
    WriteProfileInt(m_pszAppName, TEXT("GridLines"), _fGridLines);
    WriteProfileInt(m_pszAppName, TEXT("ScaleTracingImages"), _fScaleTracingImages);
    WriteProfileInt(m_pszAppName, TEXT("DontShowTraceScaleWarning"), _fDontShowTraceScaleWarning);
    WriteProfileInt(m_pszAppName, TEXT("UseAutoSuggest"), _fUseAutoSuggest);
    WriteProfileInt(m_pszAppName, TEXT("AllowBraceSyntax"), _fAllowBraceSyntax);
    WriteProfileInt(m_pszAppName, TEXT("AutoLoadGame"), _fAutoLoadGame);
    WriteProfileInt(m_pszAppName, TEXT("DupeNewCels"), _fDupeNewCels);
    WriteProfileInt(m_pszAppName, TEXT("SCI01Compat"), _fSCI01);
    WriteProfileInt(m_pszAppName, TEXT("BrowseInfo"), _fBrowseInfo);
    WriteProfileInt(m_pszAppName, TEXT("ParamInfo"), _fParamInfo);
    WriteProfileInt(m_pszAppName, TEXT("CodeCompletion"), _fCodeCompletion);
    WriteProfileInt(m_pszAppName, TEXT("HoverTips"), _fHoverTips);
    WriteProfileInt(m_pszAppName, TEXT("CompileErrorSound"), _fPlayCompileErrorSound);
}


// CSCIPicEditorApp initialization
BOOL CSCIPicEditorApp::InitInstance()
{

    // InitCommonControls() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    InitCommonControls();

    InitDitherCritSec();

    HMODULE hinstGdiPlus = LoadLibrary("gdiplus.dll");
    if (hinstGdiPlus)
    {
        FreeLibrary(hinstGdiPlus);
    }
    else
    {
        _fNoGdiPlus = TRUE;
    }

    if (!_fNoGdiPlus)
    {
        if (Ok != GdiplusStartup(&_gdiplusToken, &_gdiplusStartupInput, NULL))
        {
            AfxMessageBox(TEXT("Unable to initialize gdi+."), MB_ERRORFLAGS);
            _fNoGdiPlus = TRUE;
        }
    }

    CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    SetRegistryKey(_T("mtnPhilms"));
    LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views

    BOOL fLoadRecentFile = FALSE;
    if (GetKeyState(VK_SHIFT) & 0x8000)
    {
        // Reset settings when the shift key is down
        _LoadSettings(TRUE);
        _SaveSettings();
        for (int i = 0; i < m_pRecentFileList->GetSize(); i++)
        {
            m_pRecentFileList->Remove(0);
        }
    }
    else
    {
        fLoadRecentFile = TRUE;
        _LoadSettings();
    }

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CResourceListDoc),
		RUNTIME_CLASS(CResourceManagerFrame), // custom MDI child frame
		RUNTIME_CLASS(CResourceManagerView));
    if (!pDocTemplate)
        return FALSE;
    AddDocTemplate(pDocTemplate);

    _pPicTemplate = new CMultiDocTemplateWithNonViews(IDR_TEXTFRAME,
        RUNTIME_CLASS(CPicDoc),
        RUNTIME_CLASS(CPicChildFrame),        // standard MDI child frame
        RUNTIME_CLASS(CPicView));
    if (!_pPicTemplate)
        return FALSE;
    AddDocTemplate(_pPicTemplate);

    _pVocabTemplate = new CMultiDocTemplate(IDR_VOCABFRAME,
        RUNTIME_CLASS(CVocabDoc),
        RUNTIME_CLASS(CVocabChildFrame),
        RUNTIME_CLASS(CVocabView));
    if (!_pVocabTemplate)
    {
        return FALSE;
    }
    AddDocTemplate(_pVocabTemplate);

    _pTextTemplate = new CMultiDocTemplate(IDR_TEXTFRAME,
        RUNTIME_CLASS(CTextDoc),
        RUNTIME_CLASS(CTextChildFrame),
        RUNTIME_CLASS(CTextView));
    if (!_pTextTemplate)
    {
        return FALSE;
    }
    AddDocTemplate(_pTextTemplate);

    _pSoundTemplate = new CMultiDocTemplate(IDR_TEXTFRAME,
        RUNTIME_CLASS(CSoundDoc),
        RUNTIME_CLASS(CSoundChildFrame),
        RUNTIME_CLASS(CSoundView));
    if (!_pSoundTemplate)
    {
        return FALSE;
    }
    AddDocTemplate(_pSoundTemplate);

    _pViewTemplate = new CMultiDocTemplateWithNonViews(IDR_TEXTFRAME,
        RUNTIME_CLASS(CViewDoc),
        RUNTIME_CLASS(CEditViewChildFrame),
        RUNTIME_CLASS(CRasterView));
    if (!_pViewTemplate)
    {
        return FALSE;
    }
    AddDocTemplate(_pViewTemplate);

    _pFontTemplate = new CMultiDocTemplateWithNonViews(IDR_TEXTFRAME,
        RUNTIME_CLASS(CFontDoc),
        RUNTIME_CLASS(CEditFontChildFrame),
        RUNTIME_CLASS(CRasterView));
    if (!_pFontTemplate)
    {
        return FALSE;
    }
    AddDocTemplate(_pFontTemplate);

    _pCursorTemplate = new CMultiDocTemplateWithNonViews(IDR_TEXTFRAME,
        RUNTIME_CLASS(CCursorDoc),
        RUNTIME_CLASS(CCursorChildFrame),
        RUNTIME_CLASS(CRasterView));
    if (!_pCursorTemplate)
    {
        return FALSE;
    }
    AddDocTemplate(_pCursorTemplate);

    _pScriptTemplate = new CMultiDocTemplate(IDR_SCRIPTFRAME,
        RUNTIME_CLASS(CScriptDocument),
        RUNTIME_CLASS(CScriptFrame),
        RUNTIME_CLASS(CScriptView));
    if (!_pScriptTemplate)
    {
        return FALSE;
    }
    AddDocTemplate(_pScriptTemplate);


    _pRoomExplorerTemplate = new CMultiDocTemplate(IDR_TEXTFRAME,
        RUNTIME_CLASS(CRoomExplorerDoc),
        RUNTIME_CLASS(CRoomExplorerFrame),
        RUNTIME_CLASS(CRoomExplorerView));
    if (!_pRoomExplorerTemplate)
    {
        return FALSE;
    }
    AddDocTemplate(_pRoomExplorerTemplate);

    // Prof-UIS stuff
    _pszCommandProfile = "SCIComp2";
    g_PaintManager.InstallPaintManager(new CExtPaintManagerOffice2007_R2_LunaBlue);
    //g_PaintManager.InstallPaintManager(new CExtPaintManagerOffice2007_R2_Silver);
    //g_PaintManager.InstallPaintManager(new CExtPaintManagerNativeXP);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

    // Register a clipboard format for pic commands
    _uClipboardFormat = RegisterClipboardFormat(TEXT("SCIPicCommands-V1"));

    CSCICommandLineInfo cmdLine;
    ParseCommandLine(cmdLine);
    if (!cmdLine.m_strLogFile.IsEmpty())
    {
        _logFile.Open(cmdLine.m_strLogFile, CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive);
    }

    if (!_RegisterWindowClasses())
    {
        LogInfo(TEXT("Couldn't register window classes."));
        return FALSE;
    }

    // The one and only window has been initialized, so show and update it
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

    if (_fAutoLoadGame && fLoadRecentFile && m_pRecentFileList && (m_pRecentFileList->GetSize() > 0) &&
        !m_pRecentFileList->m_arrNames[0].IsEmpty())
    {
        OnOpenRecentFile(ID_FILE_MRU_FILE1);
    }

    // Change extension for help file
    CString strHelpFile = m_pszHelpFilePath;
    strHelpFile.Replace(".HLP", ".chm");
    free((void*)m_pszHelpFilePath);
    m_pszHelpFilePath = _tcsdup(strHelpFile);

    // Create an intellisense window.
    // Note: even though it's a top-level window, we have to give it a parent, so it can ownerdraw.
    // Give the frame as the "owner"
    SystemParametersInfo(SPI_SETLISTBOXSMOOTHSCROLLING, FALSE, NULL, 0); // turn off smoothscrolling, it makes autocomplete suck
    m_wndIntel.CreateEx(0, _T("LISTBOX"), NULL, WS_THICKFRAME | WS_VSCROLL | WS_POPUP | LBS_SORT | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_WANTKEYBOARDINPUT | LBS_NOTIFY, 0, 0, 100, 100, pMainFrame->GetSafeHwnd(), NULL);
    // And the code tooltip
    m_wndToolTip.CreateEx(0, _T("STATIC"), NULL, SS_NOTIFY | SS_OWNERDRAW | WS_POPUP | WS_CLIPSIBLINGS | WS_BORDER, 0, 0, 100, 100, pMainFrame->GetSafeHwnd(), NULL);

    AfxInitRichEdit();

    return TRUE;
}




#ifndef CS_DROPSHADOW
#define CS_DROPSHADOW 0x00020000
#endif

BOOL CSCIPicEditorApp::_RegisterWindowClasses()
{
    WNDCLASS wndcls;
    memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL
                                            // defaults
    // CS_DROPSHADOW fails on win2k... we need to put in selectively based on runtime os info (GetVersionEx). CS_DROPSHADOW is XP and above
    //wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;
    wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wndcls.lpfnWndProc = ::DefWindowProc; 
    wndcls.hInstance = AfxGetInstanceHandle();
    wndcls.hIcon = LoadIcon(IDI_APPICON); // or load a different icon
    wndcls.hCursor = LoadCursor(IDC_ARROW);
    wndcls.hbrBackground = NULL;
    wndcls.lpszMenuName = NULL;
    wndcls.lpszClassName = COLOREDTOOLTIP_CLASSNAME;

    // Register the new class and exit if it fails
    if(!AfxRegisterClass(&wndcls))
    {
       TRACE("Class Registration Failed\n");
       return FALSE;
    }
    return TRUE;
}

//
// strName is the header name, *including* the .sh
//
void CSCIPicEditorApp::OpenScriptHeader(std::string strName)
{
    if (_pScriptTemplate)
    {
        std::string fullPath = _resourceMap.GetIncludePath(strName);
        ScriptId scriptId(fullPath);
        if (!scriptId.IsNone())
        {
            // If it's already open, just activate it.
            CMainFrame *pMainWnd = static_cast<CMainFrame*>(m_pMainWnd);
            CScriptDocument *pDocAlready = pMainWnd->Tabs().ActivateScript(scriptId);
            if (pDocAlready == NULL)
            {
                CScriptDocument *pDocument =
                    static_cast<CScriptDocument*>(_pScriptTemplate->OpenDocumentFile(scriptId.GetFullPath().c_str(), TRUE));
                if (pDocument)
                {
                    // Initialize the document somehow.
                }
            }
        }
    }
}

void CSCIPicEditorApp::OpenScript(WORD w)
{
    TCHAR szGameIni[MAX_PATH];
    HRESULT hr = _GetGameIni(szGameIni, ARRAYSIZE(szGameIni));
    if (SUCCEEDED(hr))
    {
        TCHAR szKeyName[MAX_PATH];
        StringCchPrintf(szKeyName, ARRAYSIZE(szKeyName), TEXT("n%03d"), w);
        TCHAR szScriptName[100];
        if (GetPrivateProfileString(g_resourceInfo[RS_SCRIPT].pszTitleDefault, szKeyName, szKeyName, szScriptName, ARRAYSIZE(szScriptName), szGameIni))
        {
            OpenScript(szScriptName, NULL, w);
        }
    }
}

//
// strName is the script name, with out the .sc.
// e.g. "rm050"
//
void CSCIPicEditorApp::OpenScript(std::string strName, const ResourceBlob *pData, WORD wScriptNum)
{
    if (_pScriptTemplate)
    {
        ScriptId scriptId = _resourceMap.GetScriptId(strName);
        if (!scriptId.IsNone())
        {
            if (wScriptNum == InvalidResourceNumber)
            {
                if (pData)
                {
                    wScriptNum = pData->GetNumber();
                }
                else
                {
                    if (FAILED(GetResourceMap().GetScriptNumber(scriptId, wScriptNum)))
                    {
                        LogInfo("Couldn't get script number for %s", scriptId.GetFullPath());
                    }
                }
            }
            scriptId.SetResourceNumber(wScriptNum);

            // If it's already open, just activate it.
            CMainFrame *pMainWnd = static_cast<CMainFrame*>(m_pMainWnd);
            CScriptDocument *pDocAlready = pMainWnd->Tabs().ActivateScript(scriptId);
            if (pDocAlready == NULL)
            {
                std::string fullPath = scriptId.GetFullPath();
                bool fOpened = false;
                // Do an extra check first here - we don't want MFC to put up error UI if the path
                // can not be found, since we're going to do that.
                if (PathFileExists(fullPath.c_str()))
                {
                    CScriptDocument *pDocument =
                        static_cast<CScriptDocument*>(_pScriptTemplate->OpenDocumentFile(fullPath.c_str(), TRUE));
                    fOpened = (pDocument != NULL);
                    if (pDocument)
                    {
                        // We lost context...
                        pDocument->SetScriptNumber(scriptId.GetResourceNumber());
                    }
                }
                if (!fOpened)
                {
                    std::string message = scriptId.GetFullPath();
                    message += " could not be opened.";
                    if (pData)
                    {
                        message += "\nWould you like to see the disassembly instead?";
                        if (IDYES == AfxMessageBox(message.c_str(), MB_YESNO | MB_APPLMODAL))
                        {
                            // Show the disassembly.
                            if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
                            {
                                DisassembleScript((WORD)pData->GetNumber());
                            }
                            else
                            {
                                DecompileScript((WORD)pData->GetNumber()); // phil fix
                            }
                        }
                    }
                    else
                    {
                        AfxMessageBox(message.c_str(), MB_OK | MB_ICONEXCLAMATION);
                    }
                }
            }
        }
    }
}

void CSCIPicEditorApp::OpenScriptAtLine(ScriptId script, int iLine)
{
    CMainFrame *pMainWnd = static_cast<CMainFrame*>(m_pMainWnd);
    if (script.GetResourceNumber() == InvalidResourceNumber)
    {
        WORD wScriptNumber;
        GetResourceMap().GetScriptNumber(script, wScriptNumber);
        script.SetResourceNumber(wScriptNumber);
    }
    CScriptDocument *pDoc = pMainWnd->Tabs().ActivateScript(script);
    if (pDoc == NULL)
    {
        // Make an new one.
        pDoc = static_cast<CScriptDocument*>(_pScriptTemplate->OpenDocumentFile(script.GetFullPath().c_str(), TRUE));
    }
    if (pDoc)
    {
        // We lost context...
        pDoc->SetScriptNumber(script.GetResourceNumber());

        CFrameWnd *pFrame = pMainWnd->GetActiveFrame();
        if (pFrame)
        {
            CView *pView = pFrame->GetActiveView();
            if (pView->IsKindOf(RUNTIME_CLASS(CScriptView)))
            {
                CScriptView *pSV = static_cast<CScriptView*>(pView);
                int y = iLine - 1; // Off by 1
                // Ensure within bounds
                CCrystalTextBuffer *pBuffer = pSV->LocateTextBuffer();
                if (pBuffer)
                {
                    y = min(y, pBuffer->GetLineCount() - 1);
                    CPoint pt(0, y);
                    pSV->HighlightLine(pt);
                    pSV->EnsureVisible(pt);
                    pSV->SetCursorPos(pt);
                }
            }
        }
    }
}

//
// Checks to see if the resource is already open - if so, it activates it.
// Otherwise, it opens a new copy.
//
void CSCIPicEditorApp::OpenMostRecentResource(ResourceType type, WORD wNum)
{
    CMainFrame *pMainWnd = static_cast<CMainFrame*>(m_pMainWnd);
    CResourceDocument *pDocAlready = pMainWnd->Tabs().ActivateResourceDocument(type, wNum);
    if (pDocAlready == NULL)
    {
        // Make new one.
        std::auto_ptr<ResourceEnumerator> resEnum(_resourceMap.CreateEnumerator(NULL, ResourceTypeToFlag(type)));
        ResourceBlob *pData;
        if (S_OK == resEnum->Next(&pData, wNum))
        {
            std::auto_ptr<ResourceBlob> data(pData);
	        OpenResource(pData);
            // It's the most recent one, so put that into effect.
            _resourceRecency.AddResourceToRecency(pData);
        }
    }
}

// Output pane stuff
void CSCIPicEditorApp::OutputResults(std::vector<CompileResult> &compileResults)
{
    OutputClearResults();
    ShowOutputPane();
    OutputAddBatch(compileResults);
    OutputFinishAdd();
}
void CSCIPicEditorApp::ShowOutputPane()
{
    static_cast<CMainFrame*>(m_pMainWnd)->ShowOutputPane();
}
void CSCIPicEditorApp::OutputClearResults()
{
    static_cast<CMainFrame*>(m_pMainWnd)->GetOutputPane().ClearResults();
}
void CSCIPicEditorApp::OutputAddBatch(std::vector<CompileResult> &compileResults)
{
    static_cast<CMainFrame*>(m_pMainWnd)->GetOutputPane().AddBatch(compileResults);
}
void CSCIPicEditorApp::OutputFinishAdd()
{
    static_cast<CMainFrame*>(m_pMainWnd)->GetOutputPane().FinishAdd();
}


//
// Ensure that the folder name appears in the recent docs list, instead of the file name (resource.map)
//
void CSCIPicEditorApp::AddToRecentFileList(PCTSTR lpszPathName)
{
    TCHAR szBuffer[MAX_PATH];
    if (SUCCEEDED(StringCchCopy(szBuffer, ARRAYSIZE(szBuffer), lpszPathName)))
    {
        PTSTR pszFileName = StrStrI(szBuffer, TEXT("resource.map"));
        if (pszFileName)
        {
            *pszFileName = 0;
            __super::AddToRecentFileList(szBuffer);
        }
    }
}

//
// Background thread to load the class browser.
//
UINT LoadClassBrowserThreadWorker(void *pParam)
{
    SCIClassBrowser *pBrowser = (SCIClassBrowser *)pParam;
    pBrowser->Lock();
    if (!pBrowser->ReLoadFromSources())
    {
        // Might not be a fan-made game... try loading from the resources themselves so
        // that we are able to provide a class hierarchy at least.
        pBrowser->ReLoadFromCompiled();
    }
    pBrowser->Unlock();
    return 1;
}

//
// LoadGame - this is where we load a new game.
//
// If we are opening a folder, append resource.map
//
CDocument* CSCIPicEditorApp::OpenDocumentFile(PCTSTR lpszFileName)
{
    TCHAR szBuffer[MAX_PATH];
    if (SUCCEEDED(StringCchCopy(szBuffer, ARRAYSIZE(szBuffer), lpszFileName)))
    {
        PTSTR pszFileName = StrStrI(szBuffer, TEXT("resource.map"));
        if (pszFileName == NULL)
        {
            PathCombine(szBuffer, lpszFileName, TEXT("resource.map"));
        }

        // FEATURE - before closing everything, ask the user if they want to just close the resource
        // map.

        // Save any modified documents:
        if (this->SaveAllModified())
        {
            // Then close them all.
            this->CloseAllDocuments(FALSE);

            _pResourceDoc = static_cast<CResourceListDoc*>(__super::OpenDocumentFile(szBuffer));
            if (_pResourceDoc)
            {
                // Override the title - get the game name
                PCTSTR pszName = NULL;
                std::string strGameName = GetGameName();
                std::string gameFolder = GetResourceMap().GetGameFolder(); // Keep gameFolder in scope...
                if (!strGameName.empty())
                {
                    pszName = strGameName.c_str();
                }
                else
                {
                    pszName = gameFolder.c_str(); // Keep gameFolder in scope.
                    pszName = PathFindFileName(pszName);
                }
                if (pszName)
                {
                    TCHAR szTitle[MAX_PATH];
                    StringCchPrintf(szTitle, ARRAYSIZE(szTitle), TEXT("Explorer (%s)"), pszName);
                    _pResourceDoc->SetTitle(szTitle);
                }
            }
            return _pResourceDoc;
        }
    }
    return NULL;
}

void CSCIPicEditorApp::ShowResourceType(ResourceType iType)
{
    if (_pExplorerFrame)
    {
        _shownType = iType;
        if (_pResourceDoc) // Could be initial update, when we don't have it yet.
        {
            _pResourceDoc->ShowResourceType(iType);
            ((CMDIFrameWnd*)m_pMainWnd)->MDIActivate(_pExplorerFrame);
        }
    }
}
ResourceType CSCIPicEditorApp::GetShownResourceType()
{
    return _shownType;
}

void CSCIPicEditorApp::GenerateBrowseInfo()
{
    CWinThread *pThread = AfxBeginThread(LoadClassBrowserThreadWorker, &GetResourceMap().GetClassBrowser(), THREAD_PRIORITY_BELOW_NORMAL, 0, 0, NULL);
    if (pThread == NULL)
    {
        LogInfo(TEXT("Failed to create background class browser thread."));
    }
}

void CSCIPicEditorApp::ResetClassBrowser()
{
    GetResourceMap().GetClassBrowser().Reset();
}

int CSCIPicEditorApp::ExitInstance()
{
    _SaveSettings();

    if (_pViewResource)
    {
        delete _pViewResource;
    }

    _pPicTemplate = NULL; // Just in case someone asks us (note: don't need to free)

    DeleteDitherCritSec();

    int iRet = __super::ExitInstance();
    if (!_fNoGdiPlus)
    {
        GdiplusShutdown(_gdiplusToken);
    }
    return iRet;
}

DWORD g_dwID = 0;
DWORD CSCIPicEditorApp::CreateUniqueRuntimeID()
{
    return g_dwID++;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CExtResizableDialog
{
public:
    CAboutDlg();

// Dialog Data
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()

    // Visuals
    CExtButton m_wndOk;
    CExtLabel m_wndLabel1;
    CExtLabel m_wndLabel2;
    CExtLabel m_wndLabel3;
    CExtLabel m_wndLabel4;
};

CAboutDlg::CAboutDlg() : CExtResizableDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    ShowSizeGrip(FALSE);
    DDX_Control(pDX, IDOK, m_wndOk);
    DDX_Control(pDX, IDC_STATIC1, m_wndLabel1);
    DDX_Control(pDX, IDC_STATIC2, m_wndLabel2);
    DDX_Control(pDX, IDC_STATIC3, m_wndLabel3);
    DDX_Control(pDX, IDC_STATIC4, m_wndLabel4);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CSCIPicEditorApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

void CSCIPicEditorApp::OnForums()
{
    ShellExecute(NULL, "open", "http://scicommunity.com", "", "", SW_SHOWNORMAL);
}

void CSCIPicEditorApp::OnTutorials()
{
    ShellExecute(NULL, "open", "http://scicommunity.com/tutorials.php", "", "", SW_SHOWNORMAL);
}

void CSCIPicEditorApp::OnDocumentation()
{
    ShellExecute(NULL, "open", "http://www.bripro.com/scistudio/help/", "", "", SW_SHOWNORMAL);
}

void CSCIPicEditorApp::OnUpdates()
{
    ShellExecute(NULL, "open", "http://www.mtnphil.com/Games/SCIPicEditor.html", "", "", SW_SHOWNORMAL);
}

void CSCIPicEditorApp::OnEmail()
{
    ShellExecute(NULL, "open", "mailto:SCIComp@mtnphil.com", "", "", SW_SHOWNORMAL);
}

void CSCIPicEditorApp::OnSCICompHelp()
{
    HtmlHelp(HH_DISPLAY_TOPIC, HH_HELP_CONTEXT);
}


//
// Game properties
//
std::string CSCIPicEditorApp::GetGameName()
{
    TCHAR szGameName[MAX_PATH];
    _GetGameStringProperty(TEXT("Name"), szGameName, ARRAYSIZE(szGameName));
    return szGameName;
}

void CSCIPicEditorApp::SetGameName(PCTSTR pszName)
{
    _SetGameStringProperty(TEXT("Name"), pszName);
}

std::string CSCIPicEditorApp::GetGameExecutable()
{
    TCHAR szGameExe[MAX_PATH];
    _GetGameStringProperty(c_szExecutableString, szGameExe, ARRAYSIZE(szGameExe));
    return szGameExe;

}

void CSCIPicEditorApp::SetGameExecutable(PCTSTR pszExe)
{
    _SetGameStringProperty(c_szExecutableString, pszExe);
}

std::string CSCIPicEditorApp::GetGameExecutableParameters()
{
    TCHAR szGameExe[MAX_PATH];
    _GetGameStringProperty(c_szExeParametersString, szGameExe, ARRAYSIZE(szGameExe));
    return szGameExe;

}

void CSCIPicEditorApp::SetGameExecutableParameters(PCTSTR pszExe)
{
    _SetGameStringProperty(c_szExeParametersString, pszExe);
}

HRESULT CSCIPicEditorApp::_GetGameStringProperty(PCTSTR pszProp, PTSTR pszValue, size_t cchValue)
{
    TCHAR szGameIni[MAX_PATH];
    HRESULT hr = _GetGameIni(szGameIni, ARRAYSIZE(szGameIni));
    if (SUCCEEDED(hr))
    {
        hr = GetPrivateProfileString(TEXT("Game"), pszProp, TEXT(""), pszValue, (DWORD)cchValue, szGameIni) ? S_OK : ResultFromLastError();
    }
    return hr;
}

HRESULT CSCIPicEditorApp::_SetGameStringProperty(PCTSTR pszProp, PCTSTR pszValue)
{
    TCHAR szGameIni[MAX_PATH];
    HRESULT hr = _GetGameIni(szGameIni, ARRAYSIZE(szGameIni));
    if (SUCCEEDED(hr))
    {
        hr = WritePrivateProfileString(TEXT("Game"), pszProp, pszValue, szGameIni) ? S_OK : ResultFromLastError();
    }
    return hr;
}





HRESULT CSCIPicEditorApp::_GetGameIni(PTSTR pszValue, size_t cchValue)
{
    HRESULT hr = E_FAIL;
    if (_resourceMap.IsGameLoaded())
    {
        hr = StringCchPrintf(pszValue, cchValue, TEXT("%s\\game.ini"), _resourceMap.GetGameFolder().c_str());
    }
    return hr;
}


void CSCIPicEditorApp::OnRunGame()
{
    if (_resourceMap.IsGameLoaded())
    {
        BOOL fShellEx = FALSE;
        std::string gameFolder = _resourceMap.GetGameFolder();
        TCHAR szGameIni[MAX_PATH];
        if (SUCCEEDED(StringCchPrintf(szGameIni, ARRAYSIZE(szGameIni), TEXT("%s\\game.ini"), gameFolder.c_str())))
        {
            // Warning if any script patches are applied.
            // TODO: scan game folder for script.000, pic.000, etc...

            char szGameName[MAX_PATH];
            if (GetPrivateProfileString("Game", c_szExecutableString, c_szDefaultExe, szGameName, ARRAYSIZE(szGameName), szGameIni))
            {
                char szParameters[MAX_PATH];
                *szParameters = 0;
                GetPrivateProfileString("Game", c_szExeParametersString, "", szParameters, ARRAYSIZE(szParameters), szGameIni);
                if (SUCCEEDED(StringCchPrintf(szGameIni, ARRAYSIZE(szGameIni), "%s\\%s", gameFolder.c_str(), szGameName)))
                {
                    fShellEx = TRUE;
                    INT_PTR iResult = (INT_PTR)ShellExecute(AfxGetMainWnd()->GetSafeHwnd(), 0, szGameIni, szParameters, gameFolder.c_str(), SW_SHOWNORMAL);
                    if (iResult <= 32)
                    {
                        // Prepare error.
                        TCHAR szReason[MAX_PATH];
                        szReason[0] = 0;
                        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, (DWORD)iResult, 0,  szReason, ARRAYSIZE(szReason), NULL);

                        TCHAR szError[MAX_PATH];
                        StringCchPrintf(szError, ARRAYSIZE(szError), TEXT("Failed to start %s: %s"), szGameIni, szReason);
                        AfxMessageBox(szError, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
                    }
                }
            }
        }
        if (!fShellEx)
        {
            AfxMessageBox(TEXT("Failed to locate game executable."), MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
        }
    }
    else
    {
        AfxMessageBox(TEXT("Please load a game first."), MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
    }
}

void CSCIPicEditorApp::OnRoomExplorer()
{
    if (_pRoomExplorerTemplate)
    {
        // Create the one and only script doc.
        CRoomExplorerDoc *pDoc = (CRoomExplorerDoc*)_pRoomExplorerTemplate->OpenDocumentFile(NULL, TRUE);
    }
}

ViewResource *CSCIPicEditorApp::GetSelectedViewResource()
{
    // TODO: If the user modifies a view, he'll have to select a different one in order
    // for this to reload.  We should fix that.
    if (!_pViewResource || (_pViewResource->GetResourceNumber() != _iView))
    {
        if (_pViewResource)
        {
            delete _pViewResource;
            _pViewResource = NULL;
        }

        // Enumerate all the views until we find one that matches.
        ResourceEnumerator *pEnum;
        if (SUCCEEDED(GetResourceMap().CreateEnumerator(NULL, ResourceTypeFlagView, &pEnum)))
        {
            ResourceBlob *prd;
            BOOL fGotOne = FALSE;
            while (!fGotOne && (S_OK == pEnum->Next(&prd)))
            {
                if (prd->GetNumber() == _iView)
                {
                    // A match!  Make a view resource.
                    ViewResource *pViewResource = new ViewResource();
                    if (pViewResource)
                    {
                        if (SUCCEEDED(pViewResource->InitFromResource(prd)))
                        {
                            _pViewResource = pViewResource;
                            fGotOne = TRUE;
                        }
                        else
                        {
                            delete pViewResource;
                        }
                    }
                }
                delete prd;
            }
            delete pEnum;
        }
    }
    return _pViewResource;
}

void CSCIPicEditorApp::LogInfo(const TCHAR *pszFormat, ...)
{
    if (_logFile.m_hFile != INVALID_HANDLE_VALUE)
    {
        TCHAR szMessage[MAX_PATH];
        va_list argList;
        va_start(argList, pszFormat);
        StringCchVPrintf(szMessage, ARRAYSIZE(szMessage), pszFormat, argList);
        StringCchCat(szMessage, ARRAYSIZE(szMessage), TEXT("\n"));
        _logFile.Write(szMessage, lstrlen(szMessage) * sizeof(TCHAR));
        va_end(argList);
    }
}

void CSCIPicEditorApp::OnCloseGame()
{
    if (SaveAllModified())
    {
        // Then close them all.
        CloseAllDocuments(FALSE);

        _resourceRecency.ClearAllResourceTypes();
    }
}

void CSCIPicEditorApp::OnGameProperties()
{
    CGamePropertiesDialog dialog;
    dialog._strGameName = GetGameName().c_str();
    dialog._strGameExecutable = GetGameExecutable().c_str();
    dialog._strGameExecutableParameters = GetGameExecutableParameters().c_str();
    LangSyntax lang = _resourceMap.GetGameLanguage();
    dialog._fCPPSyntax = (lang == LangSyntaxCpp);
    dialog._fSCIStudioSyntax = (lang == LangSyntaxSCIStudio);
    if (IDOK == dialog.DoModal())
    {
        SetGameExecutable(dialog._strGameExecutable);
        SetGameExecutableParameters(dialog._strGameExecutableParameters);
        SetGameName(dialog._strGameName);
        _resourceMap.SetGameLanguage(dialog._fCPPSyntax ? LangSyntaxCpp : LangSyntaxSCIStudio);
    }
}

void CSCIPicEditorApp::OnUpdateGameLoaded(CCmdUI *pCmdUI)
{
    // If we have a resource map loaded, then we can close game.
    pCmdUI->Enable(GetResourceMap().IsGameLoaded());
}



// CSCIPicEditorApp message handlers


// TODO: better error messages.
void DisplayFileError(HRESULT hr, BOOL fOpen, LPCTSTR pszFileName)
{
    TCHAR szMessage[MAX_PATH * 2];
    StringCchPrintf(szMessage, ARRAYSIZE(szMessage), TEXT("%s: There was an error %s this file: %x"),
        pszFileName ? pszFileName : TEXT(""),
        fOpen ? TEXT("opening") : TEXT("saving"),
        hr);
    AfxMessageBox(szMessage, MB_OK | MB_ICONSTOP);
}
