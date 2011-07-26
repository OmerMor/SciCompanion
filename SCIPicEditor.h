// SCIPicEditor.h : main header file for the SCIPicEditor application
//
#pragma once

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "ResourceMap.h"
#include "ResourceRecency.h"
#include "IntellisenseListBox.h"
#include "ColoredToolTip.h"


// CSCIPicEditorApp:
// See SCIPicEditor.cpp for the implementation of this class
//

// FWD decl
class CScriptDoc;
class ViewResource;
class CResourceListDoc;

class CSCIPicEditorApp : public CWinApp
{
public:
    CSCIPicEditorApp();
    ~CSCIPicEditorApp();

// Overrides
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    CMultiDocTemplate *GetPicTemplate() { return _pPicTemplate; }
    CMultiDocTemplate *GetVocabTemplate() { return _pVocabTemplate; }
    CMultiDocTemplate *GetViewTemplate() { return _pViewTemplate; }
    CMultiDocTemplate *GetTextTemplate() { return _pTextTemplate; }
    CMultiDocTemplate *GetSoundTemplate() { return _pSoundTemplate; }
    CMultiDocTemplate *GetFontTemplate() { return _pFontTemplate; }
    CMultiDocTemplate *GetCursorTemplate() { return _pCursorTemplate; }
    CMultiDocTemplate *GetScriptTemplate() { return _pScriptTemplate; }
    void OpenScript(std::string strName, const ResourceBlob *pData = NULL, WORD wScriptNum = InvalidResourceNumber);
    void OpenScript(WORD w);
    void OpenScriptHeader(std::string strName);
    void OpenScriptAtLine(ScriptId script, int iLine);
    void OpenMostRecentResource(ResourceType type, WORD wNum);
    void SetScriptFrame(CFrameWnd *pScriptFrame) { _pScriptFrame = pScriptFrame; }
    DWORD CreateUniqueRuntimeID();
    CResourceMap &GetResourceMap() { return _resourceMap; }
    UINT GetCommandClipboardFormat() { return _uClipboardFormat; }
    virtual void AddToRecentFileList(PCTSTR lpszPathName);
    virtual CDocument* OpenDocumentFile(PCTSTR lpszFileName);
    ViewResource *GetSelectedViewResource();
    void SetExportFolder(LPITEMIDLIST pidl); // Takes ownership
    const LPITEMIDLIST GetExportFolder() { return _pidlFolder; }
    CIntellisenseListBox &GetIntellisense() { return m_wndIntel; }
    CColoredToolTip &GetToolTipCtrl() { return m_wndToolTip; }
    bool IsBrowseInfoEnabled() { return _fBrowseInfo != 0; }
    BOOL IsCodeCompletionEnabled() { return _fBrowseInfo && _fCodeCompletion; }
    BOOL AreHoverTipsEnabled() { return _fBrowseInfo && _fHoverTips; }
    BOOL IsParamInfoEnabled() { return _fBrowseInfo && _fParamInfo; }
    void GenerateBrowseInfo();
    void ResetClassBrowser();
    void ClearResourceManagerDoc() {_pResourceDoc = NULL; } 

    // Output pane
    void OutputResults(std::vector<CompileResult> &compileResults);
    // For finer control
    void ShowOutputPane();
    void OutputClearResults();
    void OutputAddBatch(std::vector<CompileResult> &compileResults);
    void OutputFinishAdd();

    // Game explorer
    void ShowResourceType(ResourceType iType);
    ResourceType GetShownResourceType();
    void SetExplorerFrame(CFrameWnd *pFrame) { _pExplorerFrame = pFrame; }

    // Game properties
    std::string GetGameName();
    void SetGameName(PCTSTR pszName);
    std::string GetGameExecutable();
    void SetGameExecutable(PCTSTR pszName);
    std::string GetGameExecutableParameters();
    void SetGameExecutableParameters(PCTSTR pszName);

    void LogInfo(const TCHAR *pszFormat, ...);

    // Global settings:
    int _cxFakeEgo;
    int _cyFakeEgo;
    BOOL _fUseBoxEgo;
    int _fGridLines;
    int _fScaleTracingImages;
    BOOL _fDontShowTraceScaleWarning;
    BOOL _fUseAutoSuggest;
    bool _fAllowBraceSyntax;
    BOOL _fAutoLoadGame;
    BOOL _fDupeNewCels;
    BOOL _fSCI01;
    BOOL _fBrowseInfo;
    BOOL _fParamInfo;
    BOOL _fCodeCompletion;
    BOOL _fHoverTips;
    BOOL _fPlayCompileErrorSound;

    BOOL _fNoGdiPlus;   // GDI+ is not available

    // This setting is not persisted across instances of the app:
    BOOL _fDontCheckPic;

    // Last known position of a fake ego:
    CPoint _ptFakeEgo;
    // Last selected view
    int _iView;
    ViewResource *_pViewResource;
    bool _fObserveControlLines; // Does fake ego observe control lines?

    // Last view zoom size
    int _iDefaultZoomView;
    int _iDefaultZoomFont;
    int _iDefaultZoomCursor;
    // Last pic zoom size
    int _iPicZoom;

    // Prof-UIS command profile
    PCSTR _pszCommandProfile;

    ResourceRecency _resourceRecency;

// Implementation
    afx_msg void OnAppAbout();
    afx_msg void OnTutorials();
    afx_msg void OnForums();
    afx_msg void OnDocumentation();
    afx_msg void OnEmail();
    afx_msg void OnSCICompHelp();
    afx_msg void OnUpdates();
    afx_msg void OnRunGame();
    afx_msg void OnCloseGame();
    afx_msg void OnGameProperties();
    afx_msg void OnRoomExplorer();
    afx_msg void OnUpdateGameLoaded(CCmdUI *pCmdUI);
    DECLARE_MESSAGE_MAP()


private:
    void _LoadSettings(BOOL fReset = FALSE);
    void _SaveSettings();
    HRESULT _GetGameStringProperty(PCTSTR pszProp, PTSTR pszValue, size_t cchValue);
    HRESULT _SetGameStringProperty(PCTSTR pszProp, PCTSTR pszValue);
    HRESULT _GetGameIni(PTSTR pszValue, size_t cchValue);
    BOOL _RegisterWindowClasses();

    Gdiplus::GdiplusStartupInput _gdiplusStartupInput;
    ULONG_PTR _gdiplusToken;

    // Do not free these member.  It is just here for convenience.
    CMultiDocTemplate *_pPicTemplate;
    CMultiDocTemplate *_pVocabTemplate;
    CMultiDocTemplate *_pViewTemplate;
    CMultiDocTemplate *_pResourceTemplate;
    CMultiDocTemplate *_pScriptTemplate;
    CMultiDocTemplate *_pTextTemplate;
    CMultiDocTemplate *_pSoundTemplate;
    CMultiDocTemplate *_pFontTemplate;
    CMultiDocTemplate *_pCursorTemplate;
    CMultiDocTemplate *_pRoomExplorerTemplate;
    
    CFrameWnd *_pScriptFrame;

    // Game explorer
    CResourceListDoc *_pResourceDoc;
    ResourceType _shownType;
    CFrameWnd *_pExplorerFrame;

    CResourceMap _resourceMap;

    UINT _uClipboardFormat;

    CFile _logFile;

    // Last folder for exporting resources
    LPITEMIDLIST _pidlFolder;

    CIntellisenseListBox m_wndIntel;
    CColoredToolTip m_wndToolTip;
};

extern CSCIPicEditorApp theApp;

BOOL OpenResource(const ResourceBlob *pData);

