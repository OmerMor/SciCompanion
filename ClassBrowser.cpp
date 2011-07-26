#include "stdafx.h"
#include "SCIPicEditor.h"
#include "Compile\ScriptOM.h"
#include "ClassBrowser.h"
#include "ClassBrowserInfo.h"
#include "Compile\SCO.h"
#include "Compile\SyntaxParser.h"
#include "CompiledScript.h"
#include "SCIDocs.h"
#include "Compile\CompileContext.h"

using namespace sci;
using namespace stdext;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
// helper that looks up a pointer in a map (e.g. like you use the operator[] for, but it doesn't create
// a new entry in the map)
//
template<typename T, typename T2>
T *lookup_ptr(const stdext::hash_map<T2, T*> &map, const T2 &str)
{
    T *pRet = NULL;
    stdext::hash_map<T2, T*>::const_iterator nodeIt = map.find(str);
    if (nodeIt != map.end())
    {
        pRet = nodeIt->second;
    }
    return pRet;
}

template<typename T, typename T2>
bool lookup_item(const stdext::hash_map<T2, T> &map, const T2 &str, T &result)
{
    bool fRet = false;
    stdext::hash_map<T2, T>::const_iterator nodeIt = map.find(str);
    if (nodeIt != map.end())
    {
        result = nodeIt->second;
        fRet = true;
    }
    return fRet;
}

bool IsBrowseInfoEnabled()
{
    return theApp.IsBrowseInfoEnabled();
}

class DummyCompileLog : public ICompileLog
{
public:
    virtual void ReportResult(const CompileResult &result) {};
};

// Parsing helper
Script *ParseScript(ScriptId script, CCrystalTextBuffer *pBuffer)
{
    Script *pRet = NULL;
    CScriptStreamLimiter limiter(pBuffer);
    CCrystalScriptStream stream(&limiter);
    std::auto_ptr<Script> pScript(new Script(script));
    DummyCompileLog log;
    if (g_Parser.Parse(*pScript, stream, &log))
    //if (SCISyntaxParser::Instance(pScript->Language(), theApp._fAllowBraceSyntax).Parse(*pScript, stream.begin(), NULL))
    {
        pRet = pScript.release();
    }
    return pRet;
}

SCIClassBrowser::SCIClassBrowser() : _kernelNames(_kernelNamesResource.GetNames())
{
    _hAborted = CreateEvent(NULL, FALSE, FALSE, NULL);
    _fPublicProceduresValid = false;
    _fPublicClassesValid = false;
    InitializeCriticalSection(&_csClassBrowser);
    InitializeCriticalSection(&_csErrorReport);
    _fCBLocked = 0;
    _pEvents = NULL;
}

SCIClassBrowser::~SCIClassBrowser()
{
    // ASSERT we've been reset.
    ASSERT(_scripts.empty());
    ASSERT(_pLKGScript == NULL);
    DeleteCriticalSection(&_csClassBrowser);
    DeleteCriticalSection(&_csErrorReport);
    if (_hAborted)
    {
        CloseHandle(_hAborted);
    }
}

void SCIClassBrowser::Lock() const
{
    EnterCriticalSection(&_csClassBrowser);
    ++_fCBLocked;
}

bool SCIClassBrowser::TryLock() const
{
    bool fRet = (TryEnterCriticalSection(&_csClassBrowser) != 0);
    if (fRet)
    {
        ++_fCBLocked;
    }
    return fRet;
}

bool SCIClassBrowser::HasLock() const
{
    return (_fCBLocked > 0);
}

void SCIClassBrowser::Unlock() const
{
    // ref count this (and TryLock/Lock) to be more robust
    // (since we might have code that locks it twice on the same thread, which is ok)
    --_fCBLocked;
    ASSERT(_fCBLocked >= 0);
    LeaveCriticalSection(&_csClassBrowser);
}

void SCIClassBrowser::SetClassBrowserEvents(IClassBrowserEvents *pEvents)
{
    CGuard guard(&_csClassBrowser);
    _pEvents = pEvents;
}

void SCIClassBrowser::Reset()
{
    SetEvent(_hAborted); // Abort before we go into the critical section, which can block...
    CGuard guard(&_csClassBrowser);
    ResetEvent(_hAborted); // Is this ok?
    _pLKGScript = NULL;
    _wLKG = 1000; // out of bounds

    // REVIEW: but not headers?
    for_each(_scripts.begin(), _scripts.end(), delete_object());
    _scripts.clear();

    // Delete all header scripts.
    for_each(_headerMap.begin(), _headerMap.end(), delete_map_value());
    _headerMap.clear();

    // Delete all our browser infos for the classes
    for_each(_classMap.begin(), _classMap.end(), delete_map_value());
    _classMap.clear();

    // And the objects
    for_each(_instances.begin(), _instances.end(), delete_object());
    _instances.clear();

    for_each(_instanceMap.begin(), _instanceMap.end(), delete_map_value());
    _instanceMap.clear();

    // These are the files we look at
    fFoundRoot = false;
    strRootNames.clear();

    _fPublicProceduresValid = false;
    _fPublicClassesValid = false;
}

void SCIClassBrowser::_AddInstanceToMap(Script& script, ClassDefinition *pClass)
{
    ASSERT(pClass->IsInstance());

    WORD wScript = GetScriptNumberHelper(&script);
    if (wScript == InvalidResourceNumber)
    {
        theApp.LogInfo(TEXT("Class browser: Invalid script number in %s."), script.GetPath());
    }
    ClassVector *pClassArray = lookup_ptr(_instanceMap, wScript);
    if (pClassArray == NULL)
    {
        // This is the first method for this script.
        pClassArray = new ClassVector();
        if (pClassArray)
        {
            _instanceMap[wScript] = pClassArray;
        }
    }
    if (pClassArray)
    {
        // As always, the script owns the class
        pClassArray->push_back(pClass);
    }
}

void SCIClassBrowser::_AddToClassTree(Script& script)
{
    const ClassVector &classes = script.GetClasses();
    for (size_t i = 0; i < classes.size(); i++)
    {
        ClassDefinition *pTheClass = classes[i];
        SCIClassBrowserNode *pBrowserInfo;

        if (pTheClass->IsInstance())
        {
            // We don't create a pBrowserInfo for these.
            pBrowserInfo = NULL;
            // We only encounter instances once each... so make a new SCIClassBrowserNode for each.
            pBrowserInfo = new SCIClassBrowserNode(&script);
            if (pBrowserInfo)
            {
                pBrowserInfo->SetClassDefinition(pTheClass);
                pBrowserInfo->SetName(pTheClass->GetName());
                _instances.push_back(pBrowserInfo);
            }
            _AddInstanceToMap(script, pTheClass);
        }
        else
        {
            // Add an entry for ourselves, if there is not one already.
            //if (classMap.Lookup(pTheClass->GetName().c_str(), pBrowserInfo))
            class_map::iterator nodeIt = _classMap.find(pTheClass->GetName());
            if (nodeIt != _classMap.end())
            {
                pBrowserInfo = (*nodeIt).second;
                // Ensure that we don't add the same class twice (instances don't count)
                ASSERT(!pTheClass->IsInstance());
                if (pBrowserInfo->GetClassDefinition() == NULL)
                {
                    // We're already there.  That just means a subclass of ours was added
                    // first.  Just set our pClass.
                    pBrowserInfo->SetClassDefinition(pTheClass);

                    // And fill in the script.
                    ASSERT(pBrowserInfo->GetScript() == NULL);
                    pBrowserInfo->SetScript(&script);
                }
                else
                {
                    // SCI compiler seems to allow you to declare a class twice, even though they're automatically
                    // exported from the script.  The actual engine uses class selectors to identify superclasses, so
                    // it can handle it.  The name is just an artifact of the compiler. I'm not sure what would happen
                    // if you "included" two different scripts that had the same class, thus generating an ambiguity.
                    // I've not tried that.
                    theApp.LogInfo(TEXT("Encountered second class named %s.  Not adding to tree"), pTheClass->GetName().c_str());
                    // Set this to null so we bail out of the rest of the function.
                    pBrowserInfo = NULL;
                }
            }
            else
            {
                // This is the first time we've been encountered (didn't encounter any subclasses first)
                pBrowserInfo = new SCIClassBrowserNode(&script);
                if (pBrowserInfo)
                {
                    pBrowserInfo->SetClassDefinition(pTheClass);
                    pBrowserInfo->SetName(pTheClass->GetName());
                    //classMap.SetAt(pTheClass->GetName().c_str(), pBrowserInfo);
                    _classMap[pTheClass->GetName()] = pBrowserInfo;
                }
            }
        }

        if (pBrowserInfo)
        {
            // Now find our super class, and hook us up.
            std::string superClassName = pTheClass->GetSuperClass();
            if (superClassName.empty())
            {
                // This is the root.  Apparently we can have multiple roots though.
                fFoundRoot = true;
                strRootNames.push_back(pTheClass->GetName());
            }
            else
            {
                // Look up our superclass
                class_map::iterator nodeIt = _classMap.find(superClassName);
                if (nodeIt != _classMap.end())
                {
                    SCIClassBrowserNode *pBrowserInfoSuper = (*nodeIt).second;
                    // It's already there.  Just add ourselves to it.
                    pBrowserInfoSuper->AddSubClass(pBrowserInfo);
                    // And we get a reference on it.
                    pBrowserInfo->SetSuperClass(pBrowserInfoSuper);
                }
                else
                {
                    // There isn't any superclass for us yet.  Make a new one.
                    SCIClassBrowserNode *pBrowserInfoSuper = new SCIClassBrowserNode();
                    if (pBrowserInfoSuper)
                    {
                        pBrowserInfoSuper->AddSubClass(pBrowserInfo); // Add ourselves to its subclasses.
                        pBrowserInfoSuper->SetName(superClassName); // The name is all we have now.
                        _classMap[superClassName] = pBrowserInfoSuper; // Add our super to the classMap.
                        pBrowserInfo->SetSuperClass(pBrowserInfoSuper); // So we have a way to get to it.
                    }
                }
            }
        }
    }
}


bool SCIClassBrowser::ReLoadFromSources()
{
    CGuard guard(&_csClassBrowser);
    if (IsBrowseInfoEnabled())
    {
        Reset();
        // Load the kernel and selector names
        _kernelNamesResource.Load();
        _selectorNames.Load();

        // Add headers first, since they have defines that are needed by the other scripts.
        _AddHeaders();
        bool fRet = _CreateClassTree();
        return fRet;
    }
    else
    {
        return true; // No error
    }
}





void LoadClassFromCompiled(sci::ClassDefinition *pClass, CompiledObjectBase *pObject, SelectorTable *pNames, const std::vector<std::string> &classNames)
{
    pClass->SetInstance(pObject->IsInstance());
    pClass->SetName(pObject->GetName());

    // We don't look up super class names from the selector table.  In fact, we can't know what
    // the superclass really is, until we have read in all scripts.
    WORD wSuperClass = pObject->GetSuperClass();

    if (wSuperClass < classNames.size())
    {
        pClass->SetSuperClass(classNames[wSuperClass]);
    }
    // 0xffff means no superclass, we'll leave it empty.
    // Or maybe just > the number of class names means no class.  Seems so in SQ3.

    std::vector<WORD> &properties = pObject->GetProperties();
    std::vector<WORD> &propertyValues = pObject->GetPropertyValues();
    // Instances will have values, but no selector IDs - so that's ok that properties is smaller
    // than propertyValues.
    ASSERT(properties.size() <= propertyValues.size());
    if (pClass->IsInstance())
    {
        // Add the values anyway.... we'll resolve property selectors later.
        for (size_t i = 0; i < propertyValues.size(); i++)
        {
            pClass->AddProperty(new sci::ClassProperty(TEXT("Unknown"), propertyValues[i]));
        }
    }
    else
    {
        for (size_t i = 0; i < properties.size(); i++)
        {
            pClass->AddProperty(new sci::ClassProperty(pNames->Lookup(properties[i]), propertyValues[i]));
        }
    }

    std::vector<WORD> &methods = pObject->GetMethods();
    for (size_t i = 0; i < methods.size(); i++)
    {
        std::auto_ptr<sci::MethodDefinition> pMethod(new sci::MethodDefinition);
        pMethod->SetName(pNames->Lookup(methods[i]));
        pClass->AddMethod(pMethod.get());
        pMethod.release();
    }
}


//
// This creates it from a representation of a compiled script
//
void LoadScriptFromCompiled(sci::Script *pScript, CompiledScript *pCompiledScript, SelectorTable *pNames, const std::vector<std::string> &classNames)
{
    BOOL fRet = TRUE;
    pScript->SetScriptNumber(pCompiledScript->GetScriptNumber());
    std::vector<CompiledObjectBase*> &objects = pCompiledScript->GetObjects();
    for (size_t i = 0; i < objects.size(); i++)
    {
        std::auto_ptr<sci::ClassDefinition> pClass(new sci::ClassDefinition);
        pClass->SetScript(pScript);
        LoadClassFromCompiled(pClass.get(), objects[i], pNames, classNames);
        pScript->AddClass(pClass.get());
        pClass.release();
    }
}

#define REENABLE_COMPILEDSCRIPTS

//
// Generates a class browser from compiled scripts
// 
void SCIClassBrowser::ReLoadFromCompiled()
{
    CGuard guard(&_csClassBrowser); 
    Reset();

#ifdef REENABLE_COMPILEDSCRIPTS

    // Load the kernel and selector names
    _kernelNamesResource.Load();
    _selectorNames.Load();

    GlobalClassTable classTable;
    if (!classTable.Load())
    {
        return;
    }

    fFoundRoot = FALSE;
    int iWorked = 0;
    int iFailed = 0;
    ResourceEnumerator *pEnum;
    if (SUCCEEDED(theApp.GetResourceMap().CreateEnumerator(NULL, ResourceTypeFlagScript, &pEnum)))
    {
        // We need to read in all the compiled scripts before we go making a CSCIOMScript for each.
        // This is because CSCIOMScript needs any and all superclass names, and we don't have those until all
        // classes are loaded.
        CompiledScript *rgCompiledScripts[1000];
        ZeroMemory(rgCompiledScripts, sizeof(rgCompiledScripts));
        ResourceBlob *pData;
        while (S_OK == pEnum->Next(&pData))
        {
            // Ensure we don't have the same script twice!
            if (rgCompiledScripts[pData->GetNumber()] == NULL)
            {
                scoped_ptr<sci::istream> pStream(new sci::istream(pData->GetData(), pData->GetLength()));
                std::auto_ptr<CompiledScript> pCompiledScript(new CompiledScript(pData->GetNumber()));
                if (pCompiledScript->Load(*pStream))
                {
                    rgCompiledScripts[pData->GetNumber()] = pCompiledScript.release();
                }
            }
            delete pData;
        }

        // Now we need to prepare the array of superclass names, so we can give it to the CSCIOMScript's.
        std::vector<std::string> classNumberToName;
        const std::vector<WORD> &scriptNums = classTable.GetScriptNums();
        classNumberToName.resize(scriptNums.size());
        for (size_t i = 0; i < scriptNums.size(); i++)
        {
            WORD wScriptNum = scriptNums[i];
            if (rgCompiledScripts[wScriptNum])
            {
                std::vector<CompiledObjectBase*> &objects = rgCompiledScripts[wScriptNum]->GetObjects();
                BOOL fFoundIt = FALSE;
                for (size_t j = 0; j < objects.size(); j++)
                {
                    CompiledObjectBase *pObject = objects[j];
                    if (pObject->IsInstance())
                    {
                        continue; // Not interested in these.
                    }
                    else
                    {
                        // This is a class.  Ask for its species.
                        if (pObject->GetSpecies() == i)
                        {
                            std::string strName = pObject->GetName();
                            // Yay!  Store it's name!
                            int iHowManyOfThisNameAlready = 0;
                            for (size_t iNameIndex = 0; iNameIndex < classNumberToName.size(); iNameIndex++)
                            {
                                if (classNumberToName[iNameIndex] == strName)
                                {
                                    iHowManyOfThisNameAlready++;
                                }
                            }
                            if (iHowManyOfThisNameAlready > 0)
                            {
                                // Uh oh - we've encountered this one before!  That means there
                                // are two classes of the same name, but different class ids.  Give it
                                // a unique name.
                                TCHAR szQualifier[10];
                                StringCchPrintf(szQualifier, ARRAYSIZE(szQualifier), TEXT(" - %d"), iHowManyOfThisNameAlready + 1);
                                strName += szQualifier;

                                // We need to tell the class itself about its new name, since we'll insert
                                // it in the master table based on this.
                                pObject->AdjustName(strName.c_str());

                            }
                            classNumberToName[i] = strName;
                            break;
                        }
                    }
                }
            }
            else
            {
                // This means we didn't find a script of this number
                // Oh well, they could just be leftover turds that were left in the class table!
                TCHAR szMsg[100];
                StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("class %d: couldn't find script %d!\n"), i, wScriptNum);
                OutputDebugString(szMsg);
            }
        }
        // Now we have our map of class # to class names.    
    
        for (int i = 0; i < ARRAYSIZE(rgCompiledScripts); i++)
        {
            if (rgCompiledScripts[i])
            {
                // There was a script of this number...
                TCHAR szScriptNum[20];
                StringCchPrintf(szScriptNum, ARRAYSIZE(szScriptNum), TEXT("script %03d"), i);
                std::auto_ptr<sci::Script> pScript(new sci::Script(szScriptNum));
                LoadScriptFromCompiled(pScript.get(), rgCompiledScripts[i], &_selectorNames, classNumberToName);
                _scripts.push_back(pScript.get()); // Takes ownership
                _AddToClassTree(*pScript.release());
                delete rgCompiledScripts[i];
            }
        }

        // Now we have one missing piece when loading compiled things.  We couldn't get property names
        // resolved for instances, since we didn't necessarily already encounter the super class which
        // mapped property index to property selector.  So do that now.
        for (size_t i = 0; i < _instances.size(); i++)
        {
            // SCIClassBrowserNode has the class ptr being const for reasons of public
            // consumption.  It's ok for us to modify it though.
            sci::ClassDefinition *pClass = const_cast<sci::ClassDefinition*>(_instances[i]->GetClassDefinition());
            std::string superClass = pClass->GetSuperClass();
            // Find the super class...
            class_map::const_iterator nodeIt = _classMap.find(superClass);
            if (nodeIt != _classMap.end())
            {
                SCIClassBrowserNode *pNodeSuper = nodeIt->second;
                sci::ClassPropertyVector &classProps = const_cast<sci::ClassPropertyVector&>(pClass->GetProperties());
                const sci::ClassPropertyVector &superClassProps = pNodeSuper->GetClassDefinition()->GetProperties();

                for (size_t i = 0; i < superClassProps.size(); i++)
                {
                    if (i < classProps.size())
                    {
                        classProps[i]->SetName(superClassProps[i]->GetName());
                    }
                }
            }
        }

        delete pEnum;
    }
#endif
}


//
// Reloads a single script into the class browser.
//
void SCIClassBrowser::ReLoadScript(PCTSTR pszFullPath)
{
    ClearErrors();
    CGuard guard(&_csClassBrowser); 
    if (!IsBrowseInfoEnabled())
    {
        return;
    }
    if (StrRStrI(PathFindFileName(pszFullPath), NULL, TEXT(".sh")))
    {
        // It's a header file
        _AddHeader(pszFullPath);
        // Regenerate the defines cache
        _CacheHeaderDefines();
    }
    else
    {
        // It's a regular one.
        _AddFileName(pszFullPath, PathFindFileName(pszFullPath), TRUE);

        if (_pEvents)
        {
            _pEvents->NotifyClassBrowserStatus(HasErrors() ? IClassBrowserEvents::Errors : IClassBrowserEvents::Ok, 0);
        }
    }
}

//
// Removes any data we have cached, that lives in pScript 
//
void SCIClassBrowser::_RemoveAllRelatedData(Script *pScript)
{
    // Remove stuff from this script's key in the instance classMap.
    instance_map::iterator instanceIt = _instanceMap.find(GetScriptNumberHelper(pScript));
    if (instanceIt != _instanceMap.end())
    {
        delete instanceIt->second;
        _instanceMap.erase(instanceIt);
    }

    // Go through our instancemap.  Go backwards, since we're deleting items as we go.
    // REVIEW PERF: could probably use a better data structure here.  All we are doing is keeping
    // track of SCIClassBrowserNode's.  We remove some, remove all, add some.  We never lookup.
    if (!_instances.empty())
    {
        size_t i = _instances.size();
        do
        {
            i--;
            SCIClassBrowserNode *pNode = _instances[i];
            if (pNode->GetScript() == pScript)
            {
                // Remove this node.
                _instances.erase(&_instances[i]);
                ASSERT(pNode->GetSubClasses().size() == 0); // since it's an instance
                if (pNode->GetSuperClass())
                {
                    // Remove ourself from our super's list of subclasses
                    pNode->GetSuperClass()->RemoveSubClass(pNode);
                }
                delete pNode;
            }
        } while (i > 0);
    }

    // Now remove all classes defined herein.  This is a little more tricky.
    class_map::iterator nodeIt = _classMap.begin();
    while (nodeIt != _classMap.end())
    {
        SCIClassBrowserNode *pNode = (*nodeIt).second;
        if (pNode->GetScript() == pScript)
        {
            // This is from the script we're removing.  We need to delete this node.
            // But first, remove ourselves from our superclass.
            if (pNode->GetSuperClass())
            {
                pNode->GetSuperClass()->RemoveSubClass(pNode);
            }
            // And remove ourselves from our subclasses
            std::vector<SCIClassBrowserNode*> &subClasses = pNode->GetSubClasses();
            for (size_t i = 0; i < subClasses.size(); i++)
            {
                SCIClassBrowserNode *pNodeSubClass = subClasses[i];
                if (pNodeSubClass)
                {
                    ASSERT(pNodeSubClass->GetSuperClass() == pNode); // Make sure its us.
                    pNodeSubClass->SetSuperClass(NULL); // Remove it
                }
            }
            // Now we can delete ourselves.
            nodeIt = _classMap.erase(nodeIt);
            // And delete ourselves
            delete pNode;
        }
        else
        {
            nodeIt++;
        }
    }

    // Finally, mark our caches as being invalid.  We'll recalculate them next time someone
    // asks for them.
    _fPublicProceduresValid = false;
    _fPublicClassesValid = false;
}



#include "crysedit/CCrystalTextBuffer.h"
#include "CrystalScriptStream.h"
bool SCIClassBrowser::_AddFileName(PCTSTR pszFullPath, PCTSTR pszFileName, BOOL fReplace)
{
    _pLKGScript = NULL; // Clear cache.  Possible optimization: check LKG number, and if this is the same, then set _pLKGScript to this one.

    bool fRet = false;
    CCrystalTextBuffer buffer;
    if (buffer.LoadFromFile(pszFullPath))
    {
        CScriptStreamLimiter limiter(&buffer);
        CCrystalScriptStream stream(&limiter);
        std::auto_ptr<Script> pScript(new Script(pszFullPath));
        if (g_Parser.Parse(*pScript, stream, this))
        {
#ifdef DOCSUPPORT
            // CompileDocs(*pScript);
#endif

            bool fAdded = false;
            if (fReplace)
            {
                WORD wScriptNumber = GetScriptNumberHelper(pScript.get());
                _filenameToScriptNumber[pszFullPath] = wScriptNumber;

                ASSERT(wScriptNumber != InvalidResourceNumber); // Do something about this.
                // Find matching script number and replace
                for (size_t i = 0; i < _scripts.size(); i++)
                {
                    if (GetScriptNumberHelper(_scripts[i]) == wScriptNumber)
                    {
                        _RemoveAllRelatedData(_scripts[i]);
                        fAdded = true;
                        // Replace
                        delete _scripts[i];
                        _scripts[i] = pScript.get(); // Takes ownership
                    }
                }
            }
            else
            {
                WORD wScriptNumber = GetScriptNumberHelper(pScript.get());
                _filenameToScriptNumber[pszFullPath] = wScriptNumber;
            }

            if (!fAdded)
            {
                _scripts.push_back(pScript.get()); // Takes ownership
            }
            _AddToClassTree(*pScript);
            // Relinquish ownership:
            pScript.release();
            fRet = true;
        }
        buffer.FreeAll();
    }
    return fRet;
}

bool SCIClassBrowser::_CreateClassTree()
{
    ClearErrors();

    CPrecisionTimer timer;
    timer.Start();

    if (_pEvents)
    {
        _pEvents->NotifyClassBrowserStatus(IClassBrowserEvents::InProgress, 0);
    }
    std::vector<ScriptId> scripts;
    theApp.GetResourceMap().GetAllScripts(scripts);
    bool fRet = false;
    std::vector<ScriptId>::iterator scriptIt = scripts.begin();
    int cItems = (int)scripts.size();
    int iItem = 0;
    bool fAborted = false;
    while ((scriptIt != scripts.end()) && !fAborted)
    {
        if (_pEvents)
        {
            _pEvents->NotifyClassBrowserStatus(IClassBrowserEvents::InProgress, 100 * iItem / cItems);
        }
        if (_AddFileName(scriptIt->GetFullPath().c_str(), scriptIt->GetFileName().c_str()))
        {
            fRet = true;
        }
        ++scriptIt;
        ++iItem;
        fAborted = (WAIT_OBJECT_0 == WaitForSingleObject(_hAborted, 0));
    }

    if (_pEvents)
    {
        _pEvents->NotifyClassBrowserStatus(HasErrors() ? IClassBrowserEvents::Errors : IClassBrowserEvents::Ok, 0);
    }

TCHAR szMsg[100];
StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("milliseconds: %d\n"), (int)(timer.Stop() * 1000));
OutputDebugString(szMsg);

    return fRet;
}
void SCIClassBrowser::_CacheHeaderDefines()
{
    _headerDefines.clear();
    script_map::iterator headerIt = _headerMap.begin();
    while (headerIt != _headerMap.end())
    {
        // Suck out the defines and make them convenienty accessible in a classMap
        const DefineVector &defines = headerIt->second->GetDefines();
        for (DefineVector::const_iterator defineIt = defines.begin(); defineIt != defines.end(); defineIt++)
        {
            const Define *pDefine = *defineIt;
            _headerDefines[pDefine->GetLabel()] = pDefine->GetValue();
        }
        headerIt++;
    }
}

sci::Script *SCIClassBrowser::_LoadScript(PCTSTR pszPath)
{
    Script *pScript = NULL;
    CCrystalTextBuffer buffer;
    if (buffer.LoadFromFile(pszPath))
    {
        CScriptStreamLimiter limiter(&buffer);
        CCrystalScriptStream stream(&limiter);
        std::auto_ptr<Script> pScriptT(new Script(pszPath));
        if (g_Parser.Parse(*pScriptT, stream, this))
        {
            pScript = pScriptT.get();
            pScriptT.release();
        }
        buffer.FreeAll(); // REVIEW: not exception safe.
    }
    return pScript;
}

void SCIClassBrowser::_AddHeader(PCTSTR pszHeaderPath)
{
    Script *pScript = _LoadScript(pszHeaderPath); 
    if (pScript)
    {
        // Delete old header for this path
        delete lookup_ptr<Script, std::string>(_headerMap, pszHeaderPath);
        // Takes ownership
        _headerMap[pszHeaderPath] = pScript;
        // No reason to add headers to _AddToClassTree
    }
}


void SCIClassBrowser::_AddHeaders()
{
    // REVIEW: ideally we'll want to include any new headers the user has made, by analyzing the
    // include statements in the scripts.  For now, we'll just hard-code 3 scripts:

    // game.sh
    TCHAR szHeaderPath[MAX_PATH];
    if (SUCCEEDED(StringCchPrintf(szHeaderPath, ARRAYSIZE(szHeaderPath), TEXT("%s\\game.sh"), theApp.GetResourceMap().GetSrcFolder().c_str())))
    {
        _AddHeader(szHeaderPath);
    }

    // sci.sh
    std::string includeFolder = theApp.GetResourceMap().GetIncludeFolder();
    if (!includeFolder.empty())
    {
        TCHAR szHeaderPath[MAX_PATH];
        if (SUCCEEDED(StringCchPrintf(szHeaderPath, ARRAYSIZE(szHeaderPath), TEXT("%s\\sci.sh"), includeFolder.c_str())))
        {
            _AddHeader(szHeaderPath);
        }
        if (SUCCEEDED(StringCchPrintf(szHeaderPath, ARRAYSIZE(szHeaderPath), TEXT("%s\\keys.sh"), includeFolder.c_str())))
        {
            _AddHeader(szHeaderPath);
        }
    }

    // Make all the header defines accessible in a classMap.
    _CacheHeaderDefines();
}

//
// Allocates a method array, containing all the methods implemented by the
// object strObject.
//
MethodVector *SCIClassBrowser::CreateMethodArray(const std::string &strObject, Script *pScript) const
{
    CGuard guard(&_csClassBrowser); 
    MethodVector *pMethods = new MethodVector();
    if (IsBrowseInfoEnabled())
    {
        if (pMethods)
        {
            bool fFound = false;
            std::string strClass;
            // If a script was provided, try first looking up the object in the instance array for that script.
            if (pScript)
            {
                ClassVector *pInstanceArray = lookup_ptr(_instanceMap, GetScriptNumberHelper(pScript));
                if (pInstanceArray)
                {
                    // Find a match
                    for (size_t i = 0; !fFound && i < pInstanceArray->size(); i++)
                    {
                        ClassDefinition *pClass = (*pInstanceArray)[i];
                        fFound = (pClass->GetName() == strObject);
                        if (fFound)
                        {
                            // Add this instance's methods to the method array
                            pMethods->insert(pMethods->end(), pClass->GetMethods().begin(), pClass->GetMethods().end());
                            // And use instance's super class as the class to lookup
                            strClass = pClass->GetSuperClass();
                        }
                    }
                }
            }

            if (!fFound)
            {
                strClass = strObject; // Must be a class
            }

            class_map::const_iterator nodeIt = _classMap.find(strClass);
            if (nodeIt != _classMap.end())
            {
                int iNeedThisEventually = (*nodeIt).second->ComputeAllMethods(*pMethods);
            }
        }
    }
    return pMethods;
}

//
// Allocates a method array, containing all the methods implemented by the
// object strObject.
//
ClassPropertyVector *SCIClassBrowser::CreatePropertyArray(const std::string &strObject, Script *pScript, PCTSTR pszSuper) const
{
    CGuard guard(&_csClassBrowser); 
    ClassPropertyVector *pProperties = new ClassPropertyVector();
    if (IsBrowseInfoEnabled())
    {
        // Assume first that this is a class name (since this should be quick lookup)
        class_map::const_iterator nodeIt = _classMap.find(strObject);
        if (nodeIt != _classMap.end())
        {
            int iNeedThisEventually = nodeIt->second->ComputeAllProperties(*pProperties);
        }
        else if (pScript)
        {
            // If looking up strObject didn't work, maybe it's an instance (which isn't in the class classMap)
            ClassVector *pInstanceArray = lookup_ptr(_instanceMap, GetScriptNumberHelper(pScript));
            if (pInstanceArray)
            {
                bool fFound = false;
                for (size_t i = 0; !fFound && i < pInstanceArray->size(); i++)
                {
                    ClassDefinition *pClass = (*pInstanceArray)[i];
                    fFound = (pClass->GetName() == strObject);
                    if (fFound)
                    {
                        // Found it.  Now look up its superclass (species)
                        nodeIt = _classMap.find(pClass->GetSuperClass());
                        if (nodeIt != _classMap.end())
                        {
                            int iNeedThisEventually = nodeIt->second->ComputeAllProperties(*pProperties);
                        }
                    }
                }
            }
        }
        else if (pszSuper)
        {
            ASSERT(_classMap.find(strObject) == _classMap.end());
            //nodeIt = classMap && classMap.Lookup(strObject.c_str(), pBrowserInfo))
            // Or maybe the caller already new the super 
            //int iNeedThisEventually = pBrowserInfo->ComputeAllProperties(*pProperties);
        }
    }
    return pProperties;
}

//
// Given a species, returns an array of all subclasses of that species (not instances, just classes),
// including the original.
// The caller must delete the std::vector<std::string>
//
std::vector<std::string> *SCIClassBrowser::CreateSubSpeciesArray(PCTSTR pszSpecies)
{
    // REVIEW: it might be faster to go through all classes, and ask if they are a subclass of pszSpecies.
    CGuard guard(&_csClassBrowser); 
    std::vector<std::string> *pArray = new std::vector<std::string>;
    if (pArray)
    {
        class_map::iterator nodeIt = _classMap.find(pszSpecies);
        if (nodeIt != _classMap.end())
        {
            _AddSubclassesToArray(pArray, nodeIt->second);
        }
    }
    return pArray;
}

//
// Recursively adds subclasses to a string array
//
void SCIClassBrowser::_AddSubclassesToArray(std::vector<std::string> *pArray, SCIClassBrowserNode *pBrowserInfo)
{
    CGuard guard(&_csClassBrowser);
    // Add the name of this class.
    pArray->push_back(pBrowserInfo->GetName());
    std::vector<SCIClassBrowserNode*> &subClasses = pBrowserInfo->GetSubClasses();
    for (size_t i = 0; i < subClasses.size(); i++)
    {
        SCIClassBrowserNode *pNode = subClasses[i];
        if (!pNode->GetClassDefinition()->IsInstance())
        {
            // It's a class, not an instance, add it.
            _AddSubclassesToArray(pArray, pNode);
        }
    }
}





//
// Returns the root node in the class tree, so you can enumerate everything from there.
//
const SCIClassBrowserNode *SCIClassBrowser::GetRoot(size_t i) const
{
    CGuard guard(&_csClassBrowser); 
    ASSERT(_fCBLocked);
    SCIClassBrowserNode *pRootBrowserInfo = NULL;
    if (IsBrowseInfoEnabled())
    {
        if (i < strRootNames.size())
        {
            pRootBrowserInfo = lookup_ptr(_classMap, strRootNames[i]);
        }
    }
    return pRootBrowserInfo;
}

size_t SCIClassBrowser::GetNumRoots() const
{
    CGuard guard(&_csClassBrowser);
    ASSERT(_fCBLocked);
    return IsBrowseInfoEnabled() ? strRootNames.size() : 0;
}

const Script *SCIClassBrowser::GetLKGScript(PCTSTR pszScriptFullPath)
{
    CGuard guard(&_csClassBrowser); 
    ASSERT(_fCBLocked);
    const Script *pScript = NULL;
    if (IsBrowseInfoEnabled())
    {
        word_map::iterator filenameIt = _filenameToScriptNumber.find(pszScriptFullPath);
        if (filenameIt != _filenameToScriptNumber.end())
        {
            pScript = GetLKGScript(filenameIt->second);
        }
    }
    return pScript;
}

const Script *SCIClassBrowser::GetLKGScript(WORD wScriptNumber)
{
    CGuard guard(&_csClassBrowser); 
    ASSERT(_fCBLocked);
    const Script *pScript = NULL;
    if (IsBrowseInfoEnabled())
    {
        if (wScriptNumber != InvalidResourceNumber)
        {
            if (_pLKGScript && (wScriptNumber == _wLKG))
            {
                // We cached this...
                pScript = _pLKGScript;
            }
            else
            {
                for (size_t i = 0; i < _scripts.size(); i++)
                {
                    if (GetScriptNumberHelper(_scripts[i]) == wScriptNumber)
                    {
                        pScript = _scripts[i];
                        _pLKGScript = pScript;
                        _wLKG = wScriptNumber;
                        break;
                    }
                }
            }
        }
    }
    return pScript;
}



const VariableDeclVector *SCIClassBrowser::GetMainGlobals() const
{
    CGuard guard(&_csClassBrowser); 
    ASSERT(_fCBLocked);
    const VariableDeclVector *pArray = NULL;
    if (IsBrowseInfoEnabled())
    {
        if (!_scripts.empty() && _scripts[0]) // main is 0
        {
            pArray = &_scripts[0]->GetScriptVariables();
        }
    }
    return pArray;
}

const std::vector<std::string> &SCIClassBrowser::GetKernelNames() const
{
    CGuard guard(&_csClassBrowser);
    ASSERT(_fCBLocked);
    return _kernelNames;
}


const ProcedureVector &SCIClassBrowser::GetPublicProcedures()
{
    CGuard guard(&_csClassBrowser); 
    ASSERT(_fCBLocked);
    if (!_fPublicProceduresValid)
    {
        _publicProcedures.clear();
        if (IsBrowseInfoEnabled())
        {
            for (size_t i = 0; i < _scripts.size(); i++)
            {
                const ProcedureVector &procedures = _scripts[i]->GetProcedures();
                for (size_t iProcs = 0; iProcs < procedures.size(); iProcs++)
                {
                    // STLCLEANUP
                    ProcedurePtr pProc = procedures[iProcs];
                    if (pProc->IsPublic())
                    {
                        _publicProcedures.push_back(pProc);
                    }
                }
            }
            _fPublicProceduresValid = true;
        }
    }
    return _publicProcedures;
}


const ClassVector &SCIClassBrowser::GetAllClasses()
{
    CGuard guard(&_csClassBrowser); 
    ASSERT(_fCBLocked);
    if (!_fPublicClassesValid)
    {
        _allClasses.clear();
        if (IsBrowseInfoEnabled())
        {
            for (size_t i = 0; i < _scripts.size(); i++)
            {
                // STLCLEANUP
                const ClassVector &classes = _scripts[i]->GetClasses();
                for (size_t iClasses = 0; iClasses < classes.size(); iClasses++)
                {
                    ClassDefinition *pClass = classes[iClasses];
                    if (!pClass->IsInstance())
                    {
                        _allClasses.push_back(pClass);
                    }
                }
            }
            _fPublicClassesValid = true;
        }
    }
    return _allClasses;
}

const std::vector<sci::Script*> &SCIClassBrowser::GetHeaders()
{
    CGuard guard(&_csClassBrowser);
    ASSERT(_fCBLocked);
    // REVIEW: why are we doing this every time?
    // Regenerate the _headers array.
    _headers.clear();
    script_map::const_iterator headerIt = _headerMap.begin();
    while (headerIt != _headerMap.end())
    {
        _headers.push_back(headerIt->second);
        headerIt++;
    }
    return _headers;
}

const SelectorTable &SCIClassBrowser::GetSelectorNames()
{
    CGuard guard(&_csClassBrowser); 
    ASSERT(_fCBLocked);
    return _selectorNames;
}

//
// Returns true if pszClass is pszSuper, or a subclass of pszSuper
//
bool SCIClassBrowser::IsSubClassOf(PCTSTR pszClass, PCTSTR pszSuper)
{
    CGuard guard(&_csClassBrowser); 
    ASSERT(_fCBLocked);
    bool fRet = false;
    SCIClassBrowserNode *pNode;
    int iOverflow = 100;
    while (!fRet && (pNode = lookup_ptr<SCIClassBrowserNode, std::string>(_classMap, pszClass)) && (iOverflow--)) // in case of infinite loop?
    {
        if (pNode->GetName() == pszSuper)
        {
            fRet = true;
        }
        else
        {
            pszClass = pNode->GetSuperClass() ? pNode->GetSuperClass()->GetName().c_str() : NULL;
            if (pszClass == NULL)
            {
                break;
            }
        }
    }
    return fRet;
}


//
// Helper that attempts to get a number value for the script number.
// A script may define the scriptnumber in terms of a define.
// e.g.
// (scriptnumber MAIN_SCRIPT)
// This will attempt to resolve MAIN_SCRIPT to a number.
//
WORD SCIClassBrowser::GetScriptNumberHelper(Script *pScript) const
{
    WORD w = InvalidResourceNumber;
    if (pScript->Language() == LangSyntaxCpp)
    {
        theApp.GetResourceMap().GetScriptNumber(pScript->GetScriptId(), w);
    }
    else
    {
        w = GetScriptNumberHelperConst(pScript);
        if (w != InvalidResourceNumber)
        {
            // Cache it in the script for future use.
            pScript->SetScriptNumber(w);
        }
        ASSERT(w != InvalidResourceNumber);
    }
    return w;
}

WORD SCIClassBrowser::GetScriptNumberHelperConst(const Script *pScript) const
{
    WORD w = pScript->GetScriptNumber();
    if (w == InvalidResourceNumber)
    {
        const std::string &strDefine = pScript->GetScriptNumberDefine();
        if (!strDefine.empty())
        {
            // Look it up.
            PropertyValue Value;
            if (ResolveValue(pScript, strDefine, Value))
            {
                w = Value.GetNumberValue();
            }
        }
    }
    return w;
}

//
// Similar to GetPropertyValue, but doesn't resolve the value.
//
bool SCIClassBrowser::GetProperty(PCTSTR pszName, const ClassDefinition *pClass, PropertyValue &value)
{
    bool fRet = false;
    fRet = pClass->GetPropertyConst(pszName, value);
    if (!fRet)
    {
        // Ask the super
        SCIClassBrowserNode *pNode = lookup_ptr(_classMap, pClass->GetSuperClass());
        if (pNode && pNode->GetClassDefinition())
        {
            fRet = GetProperty(pszName, pNode->GetClassDefinition(), value);
        }
    }
    return fRet;    
}

//
// Gets a numeric property from a class.  Returns true if found.
// Check the superclass via GetProperty
//
bool  SCIClassBrowser::GetPropertyValue(PCTSTR pszName, const ClassDefinition *pClass, WORD *pw)
{
    *pw = 0;
    bool fRet = false;
    const Script *pScript = pClass->GetOwnerScript();
    PropertyValue value;
    fRet = pClass->GetPropertyConst(pszName, value);
    if (fRet)
    {
        // Found!
        if (value.GetStringValue().empty())
        {
            fRet = (value.GetType() == ValueTypeNumber);
            *pw = value.GetNumberValue();
        }
        else
        {
            PropertyValue valueResolved;
            fRet = ResolveValue(pScript, value.GetStringValue().c_str(), valueResolved);
            if (fRet)
            {
                *pw = valueResolved.GetNumberValue();
            }
        }
    }
    else
    {
        // Ask the super
        SCIClassBrowserNode *pNode = lookup_ptr(_classMap, pClass->GetSuperClass());
        if (pNode && pNode->GetClassDefinition())
        {
            fRet = GetPropertyValue(pszName, pNode->GetClassDefinition(), pw);
        }
    }
    return fRet;
}


//
// Gets a numeric property from a class.  Returns true if found.
// It does not check the superclass.
//
bool SCIClassBrowser::GetPropertyValue(PCTSTR pszName, ISCIPropertyBag *pBag, const ClassDefinition *pClass, WORD *pw)
{
    *pw = 0;
    PropertyValue value;
    bool fRet = pBag->GetProperty(pszName, value);
    if (fRet)
    {
        if (value.GetStringValue().empty())
        {
            *pw = value.GetNumberValue();
        }
        else
        {
            const Script *pScript = pClass->GetOwnerScript();
            PropertyValue valueResolved;
            fRet = ResolveValue(pScript, value.GetStringValue().c_str(), valueResolved);
            if (fRet)
            {
                *pw = valueResolved.GetNumberValue();
            }
        }        
    }
    return fRet;
}

void SCIClassBrowser::ReportResult(const CompileResult &result)
{
    CGuard guard(&_csErrorReport); 
    _errors.push_back(result);
}
void SCIClassBrowser::ClearErrors()
{
    CGuard guard(&_csErrorReport); 
    _errors.clear();
}

// Passes back a copy...
std::vector<CompileResult> SCIClassBrowser::GetErrors()
{
    CGuard guard(&_csErrorReport); 
    return _errors;
}

int SCIClassBrowser::HasErrors()
{
    int iHasErrors = -1; // Unknown
    // Don't want to block if we're just asking if errors are around
    if (TryEnterCriticalSection(&_csErrorReport))
    {
        iHasErrors = _errors.empty() ? 0 : 1;
        LeaveCriticalSection(&_csErrorReport);
    }
    return iHasErrors;
}


bool SCIClassBrowser::ResolveValue(const Script *pScript, const std::string &strValue, PropertyValue &Out) const
{
    CGuard guard(&_csClassBrowser); 
    ASSERT(_fCBLocked);
    bool fFound = false;

    if (!strValue.empty())
    {
        // One special case
        if (strValue == "scriptNumber")
        {
            Out.SetValue(GetScriptNumberHelperConst(pScript));
            fFound = true;
        }
        else
        {
            // STLCLEANUP
            const DefineVector &defines = pScript->GetDefines();
            for (DefineVector::const_iterator defineIt = defines.begin(); !fFound && (defineIt != defines.end()); defineIt++)
            {
                if (strValue == (*defineIt)->GetLabel())
                {
                    // found it
                    Out.SetValue((*defineIt)->GetValue());
                    fFound = true;
                }
            }
            if (!fFound)
            {
                WORD wValue;
                if (lookup_item(_headerDefines, strValue, wValue))
                {
                    fFound = true;
                    Out.SetValue(wValue);
                }
            }
        }
    }
    return fFound;
}

//
// Given a value:
//      if it's a string, it will look in the current script to see if it matches and defines,
//      and then in the headers appropriate for that script. TODO: uses all headers now.
//
void SCIClassBrowser::ResolveValue(WORD wScript, const PropertyValue &In, PropertyValue &Out)
{
    CGuard guard(&_csClassBrowser); 
    ASSERT(_fCBLocked);
    const Script *pScript = GetLKGScript(wScript);
    
    // Look through this script's defines first.
    std::string strValue = In.GetStringValue();
    bool fFound = ResolveValue(pScript, strValue.c_str(), Out);
    if (!fFound)
    {
        Out = In;
    }
}


//
// Some public helpers
//

WORD GetPicResourceFromScript(SCIClassBrowser *pBrowser, const Script *pScript)
{
    WORD wPic = InvalidResourceNumber;
    const ClassVector &classes = pScript->GetClasses();
    for (size_t i = 0; i < classes.size(); i++)
    {
        const ClassDefinition *pClass = classes[i];
        if (pClass->IsInstance())
        {
            if (pBrowser->IsSubClassOf(pClass->GetSuperClass().c_str(), "Rm"))
            {
                // Figure out the pic.
                pBrowser->GetPropertyValue("picture", pClass, &wPic);
                break;
            }
        }
    }
    return wPic;
}
