// ScriptDocument.cpp : implementation file
//

#include "stdafx.h"
#include "SCIPicEditor.h"
#include "ScriptDocument.h"
#include "FindAllDialog.h"
#include "TextResource.h"
#include "NewCompileDialog.h"
#include "Compile\SCO.h"
#include "CrystalScriptStream.h"
#include "ClassBrowserDialog.h"
#include "CompiledScript.h"
#include "Compile\SyntaxParser.h"
#include "Compile\CompileContext.h"
#include "Compile\ScriptOMAll.h"
#include "SCIDocs.h"
#include "Decompiler.h"
#include <ctime>

using namespace std;


void CompileLog::CalculateErrors()
{
    // Calculate errors;
    _cErrors += (int)count_if(_compileResults.begin(), _compileResults.end(), mem_fun_ref(&CompileResult::IsError));
    _cWarnings += (int)count_if(_compileResults.begin(), _compileResults.end(), mem_fun_ref(&CompileResult::IsWarning));
}


void CompileLog::SummarizeAndReportErrors()
{
    stringstream summaryMessage;
    summaryMessage << _cErrors << " errors, " << _cWarnings << " warnings.";
    ReportResult(CompileResult(summaryMessage.str()));

    if (_cErrors && theApp._fPlayCompileErrorSound)
    {
        // Play a sound.
        PlaySound((LPCSTR)SND_ALIAS_SYSTEMEXCLAMATION, NULL, SND_ALIAS_ID | SND_ASYNC);
    }
}

// CScriptDocument

IMPLEMENT_DYNCREATE(CScriptDocument, CDocument)

CScriptDocument::CScriptDocument() : _buffer(*this)
{
}

CScriptDocument::~CScriptDocument()
{
    _buffer.FreeAll();
}


BEGIN_MESSAGE_MAP(CScriptDocument, CDocument)
    ON_COMMAND(ID_FILE_SAVE, OnFileSave)
    ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
    ON_COMMAND(ID_COMPILE, OnCompile)
#ifdef DOCSUPPORT
    ON_COMMAND(ID_COMPILEDOCS, OnCompileDocs)
#endif
    ON_COMMAND(ID_SCRIPT_DISASSEMBLE, OnDisassemble)
    ON_COMMAND(ID_SCRIPT_DECOMPILE, OnDecompile)
    ON_COMMAND(ID_SCRIPT_VIEWOBJECTFILE, OnViewObjectFile)
    ON_COMMAND(ID_SCRIPT_VIEWSCRIPTRESOURCE, OnViewScriptResource)
    ON_COMMAND(ID_SCRIPT_VIEWSYNTAXTREE, OnViewSyntaxTree)
    ON_COMMAND(ID_CONVERTSCRIPT, OnConvertScript)
    ON_UPDATE_COMMAND_UI(ID_COMPILE, OnUpdateIsScript)
    ON_UPDATE_COMMAND_UI(ID_SCRIPT_DISASSEMBLE, OnUpdateIsScript)
    ON_UPDATE_COMMAND_UI(ID_SCRIPT_VIEWOBJECTFILE, OnUpdateIsScript)
    ON_UPDATE_COMMAND_UI(ID_SCRIPT_VIEWSCRIPTRESOURCE, OnUpdateIsScript)
    ON_UPDATE_COMMAND_UI(ID_CONVERTSCRIPT, OnUpdateConvertScript)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_RESSIZE, OnUpdateLineCount)
END_MESSAGE_MAP()



CCrystalTextBuffer *CScriptDocument::GetTextBuffer()
{
    return &_buffer;
}

void CScriptDocument::OnUpdateIsScript(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(!_scriptId.IsHeader());
}

#ifdef DOCSUPPORT
void CScriptDocument::OnCompileDocs()
{
    // Make a new buffer.
    CCrystalTextBuffer buffer;
    if (buffer.LoadFromFile(_scriptId.GetFullPath().c_str()))
    {
        CScriptStreamLimiter limiter(&buffer);
        CCrystalScriptStream stream(&limiter);
        scoped_ptr<sci::Script> pScript(new sci::Script(_scriptId));
        CompileLog log;
        if (g_Parser.Parse(*pScript, stream, &log, true))
        {
            CompileDocs(*pScript);
        }
        else
        {
            // An error while compiling.
            log.SummarizeAndReportErrors();
            theApp.OutputResults(log.Results());
        }
        buffer.FreeAll();
    }
}
#endif

const char c_szLine[] = "--------------------------------------------------------";
void CScriptDocument::OnCompile()
{
    if (_scriptId.IsHeader())
    {
        AfxMessageBox("Header files can not be compiled.", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL, 0);
    }
    else
    {
        // Check if the script is modified, and if so, save it.
        if (_buffer.IsModified())
        {
            OnFileSave();
        }

        DeferResourceAppend defer(theApp.GetResourceMap());
        CompileLog log;
        _ClearErrorCount();
        CompileTables tables;
        tables.Load();
        PrecompiledHeaders headers(theApp.GetResourceMap());
        bool fSuccess = NewCompileScript(log, tables, headers, _scriptId);
        if (fSuccess)
        {
            tables.Save();
        }

        // put a timestamp in.
        char sz[100];
        struct tm time;
        _getsystime(&time);
        strftime(sz, ARRAYSIZE(sz), "%I:%M:%S%p", &time);
        log.ReportResult(CompileResult(sz));

        stringstream str;
        str << "Compiling " << _scriptId.GetTitle();
        str << (fSuccess ? " succeeded." : " failed.");
        log.ReportResult(CompileResult(c_szLine));
        log.ReportResult(CompileResult(str.str()));

        HRESULT hr = defer.Commit();
        if (FAILED(hr))
        {
            char sz[200];
            StringCchPrintf(sz, ARRAYSIZE(sz), "There was a problem writing the compiled script: %x", hr);
            log.ReportResult(CompileResult(sz));
            char *dummyFN[1];
            dummyFN[0] = "";
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, HRESULT_CODE(hr), 0,  sz, ARRAYSIZE(sz), dummyFN);
            log.ReportResult(CompileResult(sz, CompileResult::CRT_Error));
            log.CalculateErrors();
        }
        _DoErrorSummary(log);

        theApp.OutputResults(log.Results());
    }
}

bool NewCompileScript(CompileLog &log, CompileTables &tables, PrecompiledHeaders &headers, ScriptId &script)
{
    bool fRet = false;
    ClassBrowserLock lock(theApp.GetResourceMap().GetClassBrowser());
    lock.Lock();

    // Make a new buffer.
    CCrystalTextBuffer buffer;
    if (buffer.LoadFromFile(script.GetFullPath().c_str()))
    {
        CScriptStreamLimiter limiter(&buffer);
        CCrystalScriptStream stream(&limiter);

        scoped_ptr<sci::Script> pScript(new sci::Script(script));

        if (g_Parser.Parse(*pScript, stream, &log))
        {
            // Compile and save script resource.
            // Compile our own script!
            CompileResults results(log);
            if (GenerateScriptResource(*pScript, headers, tables, results))
            {
                WORD wNum = results.GetScriptNumber();

                // Update any tables that need to be modified (global class table, selector table)
                // Save the text resource.
                if (!results.GetTextResource().IsEmpty())
                {
                    ASSERT(script.Language() == LangSyntaxCpp);
                    results.GetTextResource().SetResourceNumber(wNum);
                    results.GetTextResource().Save();
                }

                // Save the script resource
                std::vector<BYTE> &output = results.GetScriptResource();
                theApp.GetResourceMap().AppendResource(ResourceBlob(NULL, RS_SCRIPT, output, 1, wNum));

                // Save the corresponding sco file.
                CSCOFile &sco = results.GetSCO();

                {
                    vector<BYTE> scoOutput;
                    // First save the .sco file
                    sco.Save(false, scoOutput);
                    // Copy these bytes to a stream...
                    std::string scoFileName = theApp.GetResourceMap().GetScriptObjectFileName(script.GetTitle(), LangSyntaxSCIStudio);
                    ofstream scoFile(scoFileName.c_str(), ios::out | ios::binary);
                    // REVIEW: yucky
                    scoFile.write((const char *)&scoOutput[0], (std::streamsize)scoOutput.size());
                    scoFile.close();
                }

                {
                    // Then .scop
                    vector<BYTE> scopOutput;
                    // First save the .sco file
                    sco.Save(true, scopOutput);
                    // Copy these bytes to a stream...
                    std::string scoFileName = theApp.GetResourceMap().GetScriptObjectFileName(script.GetTitle(), LangSyntaxCpp);
                    ofstream scoFile(scoFileName.c_str(), ios::out | ios::binary);
                    // REVIEW: yucky
                    scoFile.write((const char *)&scopOutput[0], (std::streamsize)scopOutput.size());
                    scoFile.close();
                }

                fRet = true;
            }
        }
        log.CalculateErrors();
        buffer.FreeAll();
    }
    return fRet;
}

void DisassembleScript(WORD wScript)
{
    CompiledScript compiledScript(0);
    if (compiledScript.Load(wScript))
    {
        // Write some crap.
        GlobalCompiledScriptLookups scriptLookups;
        ObjectFileScriptLookups objectFileLookups;
        if (scriptLookups.Load())
        {
            std::stringstream out;
            compiledScript.Disassemble(out, &scriptLookups, &objectFileLookups, theApp.GetResourceMap().GetVocab000());
            ShowTextFile(out, "script.sca.txt");
        }
    }
}

void CScriptDocument::OnDisassemble()
{
    WORD wScript;
    // Make the compiled script...
    if (SUCCEEDED(theApp.GetResourceMap().GetScriptNumber(_scriptId, wScript)))
    {
        DisassembleScript(wScript);
    }
}

void DecompileScript(WORD wScript)
{
    CompiledScript compiledScript(0);
    if (compiledScript.Load(wScript))
    {
        GlobalCompiledScriptLookups scriptLookups;
        ObjectFileScriptLookups objectFileLookups;
        if (scriptLookups.Load())
        {
            // Ok if pText fails (and is NULL)
            auto_ptr<TextResource> pText(static_cast<TextResource*>(theApp.GetResourceMap().CreateResourceFromNumber(RS_TEXT, wScript)));

            IDecompileLookups decompileLookups(wScript, &scriptLookups, &objectFileLookups, &compiledScript, pText.get(), &compiledScript);
            auto_ptr<sci::Script> pScript(compiledScript.Decompile(decompileLookups, theApp.GetResourceMap().GetVocab000()));
            std::stringstream ss;
            sci::CommentTracker comments(*pScript);
            sci::DebugStringOut out(ss, LangSyntaxCpp, &comments);
            pScript->DebugString(out);
            ShowTextFile(ss, "script.scp.txt");
        }
    }
}

void CScriptDocument::OnDecompile()
{
    WORD wScript;
    // Make the compiled script...
    if (SUCCEEDED(theApp.GetResourceMap().GetScriptNumber(_scriptId, wScript)))
    {
        DecompileScript(wScript);
    }
}

void CScriptDocument::OnViewObjectFile()
{
    if (!_scriptId.IsHeader())
    {
        // Need the script name.
        string objectFileName = theApp.GetResourceMap().GetScriptObjectFileName(_scriptId.GetTitle());
        if (!objectFileName.empty())
        {
            HANDLE hFile = CreateFile(objectFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
            if (hFile)
            {
                sci::istream byteStream(hFile);
                CSCOFile scoFile;
                if (scoFile.Create(false, byteStream))
                {
                    stringstream out;
                    scoFile.DebugOut(out);
                    string strFileName = _scriptId.GetTitle();
                    strFileName += ".sco.txt";
                    ShowTextFile(out, strFileName);
                }
                CloseHandle(hFile);
            }
        }
    }
}
void CScriptDocument::OnViewScriptResource()
{
    if (!_scriptId.IsHeader())
    {
        WORD wScript;
        // Make the compiled script...
        if (SUCCEEDED(theApp.GetResourceMap().GetScriptNumber(_scriptId, wScript)))
        {
            CompiledScript compiledScript(0);
            if (compiledScript.Load(wScript))
            {
                // Write some crap.
                std::stringstream out;
                compiledScript.DebugOut(out);
                ShowTextFile(out, "scriptresource.txt");
            }
        }
    }
}
void CScriptDocument::OnViewSyntaxTree()
{
    CScriptStreamLimiter limiter(&_buffer);
    CCrystalScriptStream stream(&limiter);
    SCIClassBrowser &browser = theApp.GetResourceMap().GetClassBrowser(); 
    browser.Lock();

    sci::Script script(_scriptId);
    if (g_Parser.Parse(script, stream, NULL))
    {
        std::stringstream out;
        sci::DebugStringOut debugOut(out, theApp.GetResourceMap().GetGameLanguage());
        script.DebugString(debugOut);
        ShowTextFile(out, "syntaxtree.txt");
    }
}

void CScriptDocument::OnConvertScript()
{
    if (theApp.GetResourceMap().GetGameLanguage() != _scriptId.Language())
    {
        if (LangSyntaxCpp == theApp.GetResourceMap().GetGameLanguage())
        {
            // What we do
            // 1) Parse this script and generate a syntax tree.
            // 2) Create a new file of the same name, but with a different file extension.
            // 3) Officially mark the script as being in a new language.
            // 4) Reload
            CScriptStreamLimiter limiter(&_buffer);
            CCrystalScriptStream stream(&limiter);
            SCIClassBrowser &browser = theApp.GetResourceMap().GetClassBrowser(); 
            browser.Lock();
            // 1)
            sci::Script script(_scriptId);
            CompileLog log;
            bool fCompile = g_Parser.Parse(script, stream, &log);;
            if (fCompile)
            {
                // 1.5 Make some substitutions for properties that have minus signs in them.
                // e.g. convert b-moveCnt to b_moveCnt, since c-style syntax can't handle - in a variable name.
                class PropertyAliasConvert : public sci::IExploreNode
                {
                public:
                    void ExploreNode(sci::IExploreNodeContext *pContext, sci::SyntaxNode &node)
                    {
                        switch (node.GetNodeType())
                        {
                        case sci::NodeTypeValue:
                        case sci::NodeTypeComplexValue:
                            {
                                sci::PropertyValueBase &base = static_cast<sci::PropertyValueBase&>(node);
                                if ((base.GetType() == sci::ValueTypeToken) ||
                                    (base.GetType() == sci::ValueTypeSelector) ||
                                    (base.GetType() == sci::ValueTypePointer))
                                {
                                    base.SetValue(GetTokenAlias(base.GetStringValue()), base.GetType());
                                }
                            }
                            break;
                        case sci::NodeTypeClassProperty:
                            {
                                sci::ClassProperty &classProp = static_cast<sci::ClassProperty&>(node);
                                classProp.SetName(GetTokenAlias(classProp.GetName()));
                            }
                            break;

                        case sci::NodeTypeLValue:
                            {
                                sci::LValue &lvalue = static_cast<sci::LValue&>(node);
                                lvalue.SetName(GetTokenAlias(lvalue.GetName()));
                            }
                            break;
                        case sci::NodeTypeSendParam:
                            {
                                sci::SendParam &sendParam = static_cast<sci::SendParam&>(node);
                                sendParam.SetName(GetTokenAlias(sendParam.GetName()));
                            }
                            break;
                        }
                    }
                };
                PropertyAliasConvert propertyAliasConvert;
                script.Traverse(NULL, propertyAliasConvert);

                std::stringstream out;
                sci::CommentTracker commentTracker(script); // Output comments too.
                sci::DebugStringOut debugOut(out, LangSyntaxCpp, &commentTracker);
                script.DebugString(debugOut);

                // 2)
                // Can't use CResourceMap::GetScriptFileName just yet, since we haven't offically converted the script
                std::string fullPath = _scriptId.GetFullPath();
                if (_scriptId.IsHeader())
                {
                    fullPath.replace(fullPath.find(".sh"), strlen(".sh"), ".shp");
                }
                else
                {
                    fullPath.replace(fullPath.find(".sc"), strlen(".sc"), ".scp");
                }
                ScriptId newScriptId(fullPath);
                newScriptId.SetResourceNumber(_scriptId.GetResourceNumber()); // Transfer resource num
                // Headers don't have resource numbers:
                ASSERT(!newScriptId.IsHeader() || (newScriptId.GetResourceNumber() == InvalidResourceNumber));
                ofstream newFile(fullPath.c_str());
                newFile << out.str();
                if (newFile.good())
                {
                    // All went well
                    newFile.close();

                    // 3)
                    if (!newScriptId.IsHeader())
                    {
                        theApp.GetResourceMap().SetScriptLanguage(newScriptId, LangSyntaxCpp);
                    }

                    std::stringstream ss;
                    ss << _scriptId.GetFileName() << " was successfully converted to " << newScriptId.GetFileName();
                    log.ReportResult(CompileResult(ss.str()));
                    theApp.OutputResults(log.Results());

                    // 4) Switch to the new script
                    _scriptId = newScriptId;
                    _buffer.FreeAll();
                    _buffer.LoadFromFile(_scriptId.GetFullPath().c_str());

                    // Play a sound and rejoice!
                    PlaySound((LPCSTR)SND_ALIAS_SYSTEMHAND, NULL, SND_ALIAS_ID | SND_ASYNC);
                    // Tell the view.
                    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SCRIPTCONVERTED | VIEWUPDATEHINT_SCRIPTPOSCHANGED, this);

                    // Update our title
                    _OnUpdateTitle();
                }
            }
            else
            {
                log.SummarizeAndReportErrors();
                theApp.OutputResults(log.Results());
                AfxMessageBox("The original script has compile errors. They must be fixed before conversion can take place.", MB_ERRORFLAGS);
            }
        }
    }
}


void CScriptDocument::OnUpdateConvertScript(CCmdUI *pCmdUI)
{
    // Enable conversion if this script's language is different than the game's language.
    pCmdUI->Enable(theApp.GetResourceMap().GetGameLanguage() != _scriptId.Language());
}

void CScriptDocument::OnUpdateLineCount(CCmdUI *pCmdUI)
{
    std::stringstream ss;
    ss << _buffer.GetLineCount();
    ss << " lines.";
    pCmdUI->SetText(ss.str().c_str());
}


void CScriptDocument::_DoErrorSummary(ICompileLog &log)
{
    log.SummarizeAndReportErrors();
}

void CScriptDocument::_ClearErrorCount()
{
}

// CScriptDocument diagnostics

#ifdef _DEBUG
void CScriptDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CScriptDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CScriptDocument serialization

void CScriptDocument::Serialize(CArchive& ar)
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

BOOL CScriptDocument::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!__super::OnOpenDocument(lpszPathName))
		return FALSE;
	_scriptId = ScriptId(lpszPathName);
	return _buffer.LoadFromFile(lpszPathName);
}
BOOL CScriptDocument::OnSaveDocument(LPCTSTR lpszPathName) 
{
	_buffer.SaveToFile(lpszPathName);
    _scriptId = ScriptId(lpszPathName);
	return TRUE;	//	Note - we didn't call inherited member!
}
BOOL CScriptDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
    return _buffer.InitNew();
}
void CScriptDocument::SetNameAndContent(ScriptId scriptId, int iResourceNumber, std::string &text)
{
    _scriptId = scriptId;
    int nEndLine = 0;
    int nEndChar = 0;
    _buffer.InsertText(NULL, 0, 0, text.c_str(), nEndLine, nEndChar);
    _buffer.SetModified();
    theApp.GetResourceMap().AssignName(RS_SCRIPT, iResourceNumber, scriptId.GetTitle().c_str());
    // Since we're assigning a name, we'd better save it too.
    OnFileSave();
}

void CScriptDocument::OnFileSave()
{
    std::string path = _scriptId.GetFullPath();
    // Get the current active script, and save it.
    _buffer.SaveToFile(path.c_str());
    // Update the classbrowser...
    SCIClassBrowser &browser = theApp.GetResourceMap().GetClassBrowser();
    {
        ClassBrowserLock lock(browser);
        lock.Lock();
        browser.ReLoadScript(path.c_str());
    }
    // Notify clients *after* we have updated the class browser.
    UpdateAllViewsAndNonViews(NULL, VIEWUPDATEHINT_SCRIPTSAVED, this);
}

const TCHAR g_rgszScriptFilter[] = TEXT("Script (*.sc)|*.sc;|Header (*.sh)|*.sh|All Files (*.*)|*.*");
const TCHAR g_rgszHeaderFilter[] = TEXT("Header (*.sh)|*.sh|Script (*.sc)|*.sc;|All Files (*.*)|*.*");


void CScriptDocument::OnFileSaveAs()
{
    // Set up the save dialog.
    CString strTitle = _scriptId.GetTitle().c_str();
    CFileDialog fileDialog(FALSE,
                           _scriptId.IsHeader() ? ".sh" : ".sc",
                           strTitle,
                           OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                           _scriptId.IsHeader() ? g_rgszHeaderFilter : g_rgszScriptFilter);
    CString strFolder = _scriptId.GetFolder().c_str();
    fileDialog.m_pOFN->lpstrInitialDir = strFolder;
    if (IDOK == fileDialog.DoModal())
    {
        // We have a new filename.
        CString strFileName = fileDialog.GetPathName();
        ScriptId scriptNew(strFileName);
        // But save it under the new name.
        if (_buffer.SaveToFile(strFileName))
        {
            // We have a new identity
            _scriptId = scriptNew;
        }
    }
}

void CScriptDocument::UpdateModified()
{
    SetModifiedFlag(_buffer.IsModified());
    _OnUpdateTitle();
}

void CScriptDocument::_OnUpdateTitle()
{
    SetTitle(_scriptId.GetFileName().c_str());
}

