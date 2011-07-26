
#pragma once

#include "Vocab000.h"
#include "ClassBrowser.h"

// FWD declaration
class ResourceBlob;
class ResourceRecency;

HRESULT RebuildResources(BOOL fShowUI);

//
// Clients can implement this interface to be notified when
// resources have been added or removed.
//
__interface ISyncResourceMap
{
public:
    HRESULT OnResourceAdded(const ResourceBlob *pData);
    HRESULT OnResourceDeleted(const ResourceBlob *pData);
    HRESULT OnResourceMapReloaded();
    HRESULT OnResourceTypeReloaded(int iType);
};

//
// This object holds onto a file handle, and enumerates resource data.
//
class ResourceEnumerator
{
public:
    ResourceEnumerator();
    ~ResourceEnumerator();
    void Init(ResourceRecency *pResourceRecency, const std::string &folder, DWORD dwTypes);
    void Init(ResourceRecency *pResourceRecency, const std::string &folder, HANDLE hFile, DWORD dwTypes);

    // Override
    virtual HRESULT Next(ResourceBlob **ppData, WORD wNumber = InvalidResourceNumber) = 0; // Caller must free

protected:
    HRESULT _MoveToPackage(DWORD dwOffset, int iPackage);
    HRESULT _OpenResource();

    std::string _gameFolder;
    DWORD _dwTypes;

    HANDLE _hFileMap;
    BOOL _fDontCloseHandle;

    HANDLE _hFilePackage;
    HRESULT _hrPackageError;
    int _iCurrentPackage;

    // Weak ref
    ResourceRecency *_pResourceRecency;
};

class CSCI0ResourceEnumerator : public ResourceEnumerator
{
public:
    HRESULT Next(ResourceBlob **ppData, WORD wNumber = InvalidResourceNumber);

protected:
    BOOL _IsCorrectType(int iType);
    HRESULT _ReadNextMapEntry(RESOURCEMAPENTRY_SCI0 *pEntry);
};


// SCI1 Resource types
#define RS_SCI1_FIRST                 0x80
#define RS_SCI1_VIEW                  0x80
#define RS_SCI1_PIC                   0x81
#define RS_SCI1_SCRIPT                0x82
#define RS_SCI1_TEXT                  0x83
#define RS_SCI1_SOUND                 0x84
#define RS_SCI1_MEMORY                0x85
#define RS_SCI1_VOCAB                 0x86
#define RS_SCI1_FONT                  0x87
#define RS_SCI1_CURSOR                0x88
#define RS_SCI1_PATCH                 0x89
#define RS_SCI1_NUM_TYPES 0x12

// Need function to map SCI1 resource types to real resource types, and back
class CSCI1ResourceEnumerator : public ResourceEnumerator
{
public:
    CSCI1ResourceEnumerator() { _fMapHeaderValid = FALSE; _iMapHeaderIndex = 0; }
    HRESULT Next(ResourceBlob **ppData, WORD wNumber = InvalidResourceNumber);

protected:
    HRESULT _EnsureMapHeader();

    RESOURCEMAPHEADER_SCI1 _mapHeader[RS_SCI1_NUM_TYPES];
    BOOL _fMapHeaderValid;
    int _iMapHeaderIndex; // current map header
    DWORD _cbRemaining;
    int _iHeaderArrayIndex;
};


//
// REVIEW: CResourceMap needs to be protected with a critical section
//

//
// This lets you operate on anything involving the resource.map, and is basically
// where we go to learn information about the currently loaded game.
//
class CResourceMap
{
public:
    CResourceMap();
    ~CResourceMap();
    HRESULT AppendResource(const ResourceBlob &resource);
    HRESULT AppendResourceAskForNumber(ResourceBlob &resource);
    int SuggestResourceNumber(ResourceType type);
    void AssignName(const ResourceBlob &resource);
    void AssignName(int iType, int iResourceNumber, PCTSTR pszName);
    HRESULT CreateEnumerator(ResourceRecency *_pResourceRecency, DWORD dwTypes, ResourceEnumerator **pEnum); // Caller must free
    ResourceEnumerator *CreateEnumerator(ResourceRecency *_pResourceRecency, DWORD dwTypes); // Caller must free
    HRESULT CreateEnumerator(ResourceRecency *_pResourceRecency, DWORD dwTypes, HANDLE hFile, ResourceEnumerator **pEnum); // Caller must free
    void PurgeUnnecessaryResources();
    void ReloadResourceType(int iType);
    void DeleteResource(const ResourceBlob *pResource);
    void AddSync(ISyncResourceMap *pSync);
    void RemoveSync(ISyncResourceMap *pSync);
    void SetGameFolder(const std::string &gameFolder);
    std::string GetGameFolder() const;
    std::string GetSrcFolder();
    std::string GetIncludeFolder();
    std::string GetIncludePath(const std::string &includeFileName);
#ifdef DOCSUPPORT
    std::string GetDocPath(const std::string &fileName);
#endif
    std::string GetTemplateFolder();
    std::string GetSamplesFolder();
    bool IsGameLoaded() { return !_gameFolder.empty(); }
    std::string GetScriptFileName(const std::string &name, LangSyntax lang = LangSyntaxUnknown);
    std::string GetScriptObjectFileName(const std::string &title, LangSyntax lang = LangSyntaxUnknown);
    std::string GetScriptObjectFileName(WORD wScript, LangSyntax lang = LangSyntaxUnknown);
    HRESULT GetGameIni(PTSTR pszBuf, size_t cchBuf);
    std::string GetGameIniFileName();
    std::string GetIniString(const std::string &sectionName, const std::string &keyName, PCSTR pszDefault = "");
    void SetIniString(const std::string &sectionName, const std::string &keyName, const std::string &value);
    std::string FigureOutName(int iType, int iResourceNum);
    HRESULT GetScriptNumber(ScriptId script, WORD &wScript);
    int GetSCI0Version();
    Vocab000 *GetVocab000();
    void ClearVocab000() { delete _pVocab000; _pVocab000 = NULL; }
    SCIClassBrowser &GetClassBrowser() { return _classBrowser; }
    IResourceBase *CreateResourceFromNumber(DWORD dwType, WORD wNumber);
    void GetAllScripts(std::vector<ScriptId> &scripts);
    void GetNumberToNameMap(std::map<WORD, std::string> &scos);
    ScriptId GetScriptId(const std::string &name);
    void SetScriptLanguage(ScriptId script, LangSyntax language);
    LangSyntax GetGameLanguage();
    void SetGameLanguage(LangSyntax language);
    void RemoveScriptFromGame(WORD wScript);

private:
    HRESULT _UpdateMap(HANDLE hFileMap, DWORD dwOffset, const ResourceBlob &data);
    HRESULT _WriteResourceI(HANDLE hFileMap, HANDLE hFilePackage, const ResourceBlob &data);
    HRESULT _DeleteResourceI(HANDLE hFileMap, HANDLE hFilePackage, int iPackage, DWORD dwPackageOffset, int iResourceLength);
    HRESULT _GetMapAndPackage(const ResourceBlob &resource, HANDLE *phFileMap, HANDLE *phFilePackage);
    HRESULT _CopyAndGetBakMapAndPackage(const ResourceBlob &resource, HANDLE *phFileMap, HANDLE *phFilePackage);
    HRESULT _GetMapAndPackageI(const ResourceBlob &resource, BOOL fBak, HANDLE *phFileMap, HANDLE *phFilePackage);
    HRESULT _GetVocab000ResourceData(HANDLE hFile, ResourceBlob **pData);
    std::string resmap_filename_bak();
    std::string resmap_filename();
    std::string package_filename_bak(int iPackage);
    std::string package_filename(int iPackage);

    void BeginDeferAppend();
    HRESULT EndDeferAppend();
    void AbandonAppend();
    friend class DeferResourceAppend;

    std::string _gameFolder;
    std::vector<ISyncResourceMap*> _syncs;
    Vocab000 *_pVocab000;
    BOOL _cDeferAppend;
    std::vector<ResourceBlob> _deferredResources;
    SCIClassBrowser _classBrowser;

    LangSyntax _language;
};

//
// This class is just a CObject class that wraps a resource type number
//
class CResourceTypeWrap : public CObject
{
public:
    CResourceTypeWrap(int iType) { _iType = iType; }
    int GetType() { return _iType; }

private:
    int _iType;
};

#define RESOURCEMAPTYPE_SCI0 0
#define RESOURCEMAPTYPE_SCI1 1
HRESULT GetResourceMapType(HANDLE hFile, DWORD *pdwType);

//
// Defer the actual writing of resources so it happens in one big batch at the end.
//
class DeferResourceAppend
{
public:
    DeferResourceAppend(CResourceMap &map, bool fDoIt = true) : _map(map)
    {
        if (fDoIt)
        {
            _map.BeginDeferAppend();
        }
        _fDoIt = fDoIt;
    }
    HRESULT Commit()
    {
        if (_fDoIt)
        {
            return _map.EndDeferAppend();
        }
        else
        {
            return S_OK;
        }
    }
    ~DeferResourceAppend()
    {
        if (_fDoIt)
        {
            _map.AbandonAppend();
        }
    }
private:
    CResourceMap &_map;
    bool _fDoIt;
};

// Returns "n004" for 4.
std::string default_reskey(int iNumber);