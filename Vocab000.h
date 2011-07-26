#pragma once

#include "ResourceBase.h"
#include "interfaces.h"

// FWD declarations
class ResourceBlob;

class Vocab000 : public IResourceBase, public ILookupNames
{
public:
    enum WordClass
    {
        UnknownWordClass    = 0x000,
        Number              = 0x001,
        Special1            = 0x002,
        Special2            = 0x004,
        Special3            = 0x008,
        Proposition         = 0x010,
        Article             = 0x020,
        QualifyingAdjective = 0x040,
        RelativePronoun     = 0x080,
        Noun                = 0x100,
        IndicativeVerb      = 0x200,
        Adverb              = 0x400,
        ImperativeVerb      = 0x800
    };

    typedef DWORD WordGroup;

    typedef stdext::hash_map<std::string, WordGroup> word2group_map;
    typedef stdext::hash_map<WordGroup, std::string> group2words_map;
    typedef stdext::hash_map<WordGroup, WordClass> group2class_map;

    Vocab000(bool fSimple = false);
    ~Vocab000();

    // this for existing ones.
    HRESULT InitFromResource(const ResourceBlob *prd);
    // This for new ones
    HRESULT InitNew() { return E_NOTIMPL; } // Can't create new vocabs

    HRESULT AddNewWord(PCTSTR pszWord, WordClass dwClass, BOOL fShowUI);
    HRESULT AddSynonym(PCTSTR pszWord, PCTSTR pszOriginal);
    HRESULT AddWord(PCTSTR pszWord, DWORD dwInfo, BOOL fShowUI);
    HRESULT SetGroupClass(WordGroup dwGroup, WordClass dwClass);
    HRESULT GetGroupClass(WordGroup dwGroup, WordClass *pdwClass);
    HRESULT RemoveWord(PCTSTR pszWord);

    void SetResourceNumber(int iNum) { _iNumber = iNum; }
    void SetPackageNumber(int iNum) { _iPackageNumber = iNum; }
    int GetResourceNumber() const { return _iNumber; }
    int GetPackageNumber() const { return _iPackageNumber; }
    BOOL QueryCapability(const GUID guid, void **ppv)
    {
        BOOL fRet = FALSE;
        if (IsEqualGUID(guid, __uuidof(IResourceBase)))
        {
            fRet = TRUE;
            *ppv = static_cast<IResourceBase*>(this);
        }
        else
        {
            fRet = __super::QueryCapability(guid, ppv);
        }
        return fRet;
    }
    BOOL Serialize(sci::istream *pSerial) const;
    ResourceType GetType() const { return RS_VOCAB; }

    HRESULT QuickSave();

    typedef group2words_map::const_iterator groups_iterator;
    void EnumGroups(groups_iterator &position, WordGroup &dwGroup, WordClass &dwClass, std::string &strWords) const;
    std::string GetGroupWordString(WordGroup dwGroup) const;
    groups_iterator GroupsBegin() const { return _mapGroupToString.begin(); }
    groups_iterator GroupsEnd() const { return _mapGroupToString.end(); }
    std::vector<std::string> &GetWords() { return _words; }
    bool LookupWord(const std::string &word, WordGroup &dwGroup) const;
    WordGroup GroupFromString(PCTSTR pszString);

    // ILookupNames
    std::string Lookup(WORD wIndex) const;

private:
    static const WORD AlphaIndexLength = (26 * 2); // 26 letters, 2 bytes for each offset.

    void _InsertWord(PCTSTR pszWord, WordGroup dwGroup);
    WordGroup _FindLargestEmptyGroup();
    void _FixupGroupToString(WordGroup dwGroup);
    void _ReadWord(sci::istream &byteStream, char *pszBuffer, size_t cchBuffer);

    // Map from word to class
    word2group_map _mapWordToGroup;
    group2words_map _mapGroupToString;
    group2class_map _mapGroupToClass;
    std::vector<std::string> _words;
    BYTE _rgbGroups[0x1000]; // 4096 entries, one for each group.

    int _iNumber;
    int _iPackageNumber;

    // In "simple" mode, for perf, we don't bother with calculating word groups.
    bool _fSimple;
};

bool IsValidVocabString(PCTSTR pszWord, BOOL fShowUI);
bool IsValidVocabChar(TCHAR ch);
PCTSTR FindValidVocabStringFromRight(PCTSTR pszWord);
void GetWordClassString(Vocab000::WordClass dwClass, std::string &str);
Vocab000::WordClass GetWordClass(DWORD dwInfo);
Vocab000::WordGroup GetWordGroup(DWORD dwInfo);
DWORD InfoFromClassAndGroup(Vocab000::WordClass dwClass, Vocab000::WordGroup dwGroup);


