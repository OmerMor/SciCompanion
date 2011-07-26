
#include "stdafx.h"
#include "Vocab000.h"
#include "ResourceMap.h"
#include "SCIPicEditor.h"

using namespace std;
using namespace stdext;


bool IsValidVocabChar(TCHAR ch)
{
    return (((ch >= TEXT('a')) && (ch <= TEXT('z'))) || ((ch >= TEXT('A')) && (ch <= TEXT('Z'))) ||
            ((ch >= TEXT('0')) && (ch <= TEXT('9'))));
}

bool IsValidVocabString(PCTSTR pszWord, BOOL fShowUI)
{
    bool fRet = true;
    size_t cChars = lstrlen(pszWord);
    for (size_t i = 0; i < cChars; i++)
    {
        fRet = IsValidVocabChar(pszWord[i]);
        if (!fRet)
        {
            break;
        }
    }

    if (!fRet && fShowUI)
    {
        TCHAR szBuffer[MAX_PATH];
        StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), TEXT("Invalid word: %s.\nWords must contain only letters or numbers."), pszWord);
        AfxMessageBox(szBuffer, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
    }

    return fRet;
}

PCTSTR FindValidVocabStringFromRight(PCTSTR pszWord)
{
    PCTSTR pszValid = NULL;
    size_t cChars = lstrlen(pszWord);
    for (size_t i = (cChars - 1); i >= 0; i--)
    {
        if (IsValidVocabChar(pszWord[i]))
        {
            pszValid = &pszWord[i];
        }
        else
        {
            break;
        }
    }
    return pszValid;
}


Vocab000::WordClass GetWordClass(DWORD dwInfo) { return static_cast<Vocab000::WordClass>((dwInfo & 0x00FFF000) >> 12); }
Vocab000::WordGroup GetWordGroup(DWORD dwInfo) { return (dwInfo & 0x00000FFF); }
DWORD InfoFromClassAndGroup(Vocab000::WordClass dwClass, Vocab000::WordGroup dwGroup) { return dwGroup | (dwClass << 12); }

const TCHAR *g_rgszWordClasses[] =
{
    { TEXT("Number") },
    { TEXT("") },
    { TEXT("") },
    { TEXT("") },
    { TEXT("Preposition") },
    { TEXT("Article") },
    { TEXT("Qualifying Adjective") },
    { TEXT("Relative Pronoun") },
    { TEXT("Noun") },
    { TEXT("Indicative Verb") },
    { TEXT("Adverb") },
    { TEXT("Imperative Verb") },
};

void GetWordClassString(Vocab000::WordClass dwClass, std::string &str)
{
    for (int i = 0; i < ARRAYSIZE(g_rgszWordClasses); i++)
    {
        if (dwClass & (0x1 << i))
        {
            if (!str.empty())
            {
                str += TEXT(" | ");
            }
            str += g_rgszWordClasses[i];
        }
    }
}

Vocab000::Vocab000(bool fSimple)
{
    _fSimple = fSimple;
    _iNumber = -1;
    _iPackageNumber = -1;
    ZeroMemory(_rgbGroups, sizeof(_rgbGroups));
}

Vocab000::~Vocab000()
{
    int x = 0;
}

void Vocab000::_ReadWord(sci::istream &byteStream, char *pszBuffer, size_t cchBuffer)
{
    BYTE bCopyCount;
    byteStream >> bCopyCount;
    // Read the word first
    if (strlen(pszBuffer) >= bCopyCount)
    {
        // Move to the part after the letters that are the same between this and the last word.
        TCHAR *pszStartHere = pszBuffer + bCopyCount;
        cchBuffer -= bCopyCount;

        BYTE bChar;
        while ((cchBuffer > 0) && (byteStream >> bChar).good())
        {
            *pszStartHere = (0x7f & bChar);
            pszStartHere++;
            cchBuffer--;
            if (bChar & 0x80)
            {
                // This was the last char.
                if (cchBuffer > 0)
                {
                    *pszStartHere = 0; // Null terminate.
                }
                else
                {
                    // Ran over our buffer length.
                    // TODO: throw exception?
                }
                break;
            }
        }
    }
    else
    {
        return; // Bad data in the file.
    }

    if (byteStream.good())
    {
        _words.push_back(pszBuffer);

        // Ok, we have a word in our buffer.  Time to read the group and class
        DWORD dwInfo = 0;
        BYTE b;
        byteStream >> b;
        dwInfo |= (((DWORD)b) << 16);
        byteStream >> b;
        dwInfo |= (((DWORD)b) << 8);
        byteStream >> b;
        if (byteStream.good() && !_fSimple)
        {
            dwInfo |= b;

            // Keep track of which groups have been used.
            Vocab000::WordGroup dwGroup = GetWordGroup(dwInfo);
            ASSERT(dwGroup < ARRAYSIZE(_rgbGroups));
            _rgbGroups[dwGroup] = 0xff;

            // Now we have our class.
            _mapWordToGroup[pszBuffer] = dwGroup;
            _mapGroupToClass[dwGroup] = GetWordClass(dwInfo);

            group2words_map::iterator group2wordIt = _mapGroupToString.find(dwGroup);
            if (group2wordIt != _mapGroupToString.end())
            {
                // Already exists.. tack it on
                _mapGroupToString[dwGroup] = (group2wordIt->second + " | " + pszBuffer);
            }
            else
            {
                // New one...
                _mapGroupToString[dwGroup] = pszBuffer;
            }
        }
    }
}


HRESULT Vocab000::InitFromResource(const ResourceBlob *prd)
{
    sci::istream byteStream = prd->GetReadStream();
    // Avoid needless reallocations.
    _words.reserve(1000);

    // Skip the "alphabetical offset" table
    byteStream.skip(AlphaIndexLength);

    // Now we have started with the words.
    char sz[MAX_PATH];
    while (byteStream.good())
    {
        _ReadWord(byteStream, sz, ARRAYSIZE(sz));
    }

    _iPackageNumber = prd->GetPackageHint();
    _iNumber = prd->HasNumber() ? prd->GetNumber() : -1;
    return S_OK;
}

//
// Note: this code won't work under UNICODE.
//
BOOL Vocab000::Serialize(sci::istream *pSerial) const
{
    // _words should already be sorted.

    // Prepare the offset table
    WORD wOffsets[26];
    ZeroMemory(wOffsets, sizeof(wOffsets));
    pSerial->WriteBytes((BYTE*)wOffsets, sizeof(wOffsets));

    PCTSTR pszPreviousWord = TEXT("");
    size_t cWords = _words.size();
    for (size_t i = 0; i < cWords; i++)
    {
        const std::string &strWord = _words[i];
        PCTSTR pszWord = strWord.c_str();
        WordGroup dwGroup = _mapWordToGroup.find(pszWord)->second;   // Must exist, or we're corrupt
        WordClass dwClass = _mapGroupToClass.find(dwGroup)->second; // Must exist.
        DWORD dwInfo = InfoFromClassAndGroup(dwClass, dwGroup);

        if ((pszWord[0] >= TEXT('a')) && (pszWord[0] <= TEXT('z')))
        {
            if (pszWord[0] != pszPreviousWord[0])
            {
                // We're at the beginning of a new set of letters.  Take note of the offset.
                WORD *rgwOffsets = (WORD*)pSerial->GetInternalPointer();
                ASSERT(rgwOffsets[pszWord[0] - TEXT('a')] == 0);
                ASSERT(pSerial->GetPosition() < 0xffff);
                rgwOffsets[pszWord[0] - TEXT('a')] = (WORD)pSerial->GetPosition();
            }
        }

        // Write the word.
        // How many characters are in common with the last word?
        BYTE cSame = 0;
        PCTSTR pszPrevPtr = pszPreviousWord;
        PCTSTR pszCurrPtr = pszWord;
        while (pszPrevPtr[cSame] == pszCurrPtr[cSame])
        {
            cSame++;
        }

        pszPreviousWord = pszWord;

        // Byte indicating how many chars are in common with last word:
        pSerial->WriteByte(cSame);
        
        pszWord += cSame;
        int cRemaining = (int)lstrlen(pszWord);
        if (cRemaining > 1)
        {
            // Rest of the word, except for the last byte:
            pSerial->WriteBytes((BYTE*)pszWord, cRemaining - 1);
        }
        else if (cRemaining == 0)
        {
            // This means that, for example, "foo" came after "foobar", which should not be allowed.
            ASSERT(FALSE);
        }

        // Write the last character, and signify it as such:
        BYTE bLastCharacter = pszWord[cRemaining - 1];
        bLastCharacter |= 0x80;
        pSerial->WriteByte(bLastCharacter);

        // Now write the class and group information.
        pSerial->WriteByte((BYTE)(0xff & (dwInfo >> 16)));
        pSerial->WriteByte((BYTE)(0xff & (dwInfo >> 8)));
        pSerial->WriteByte((BYTE)(0xff & dwInfo));
    }

    return TRUE;
}


void Vocab000::EnumGroups(groups_iterator &position, WordGroup &dwGroup, WordClass &dwClass, std::string &strWords) const
{
    dwGroup = position->first;
    strWords = position->second;
    dwClass = _mapGroupToClass.find(dwGroup)->second;
    ++position;
}

std::string Vocab000::GetGroupWordString(WordGroup dwGroup) const
{
    std::string strRet;
    group2words_map::const_iterator it = _mapGroupToString.find(dwGroup);
    if (it != _mapGroupToString.end())
    {
        strRet = _mapGroupToString.find(dwGroup)->second;
    }
    return strRet;
}


int CountMatchingCharacters(PCTSTR psz1, PCTSTR psz2)
{
    int cMatched = 0;
    while (*psz1 && (*psz1 == *psz2))
    {
        cMatched++;
        psz1++;
        psz2++;
    }
    return cMatched;
}

bool Vocab000::LookupWord(const std::string &word, WordGroup &dwGroup) const
{
    word2group_map::const_iterator it = _mapWordToGroup.lower_bound(word);
    if (it != _mapWordToGroup.end())
    {
        dwGroup = it->second;
        return true;
    }
    else
    {
        return false;
    }
}

Vocab000::WordGroup Vocab000::GroupFromString(PCTSTR pszString)
{
    std::string strLower = pszString;
    std::transform(strLower.begin(), strLower.end(), strLower.begin(), 
                   (int(*)(int)) tolower);

    PCTSTR pszLower = strLower.c_str();
    // Try for an exact match, before searching.
    WordGroup dwGroup = { 0 };
    if (!LookupWord(strLower, dwGroup))
    {
        // This matches an incomplete string to the most likely group.
        // First we'll find the best matching word by looking through the alphabetically
        // ordered _words array, using a binary search.
        size_t iLowerBound = 0;
        size_t cWords = _words.size();
        size_t iCurrent = 0;
        int cMaxLettersMatched = 0;
        size_t iBest = (size_t)-1;
        int cCurrentMatched = 0;
        while (iLowerBound <= cWords)
        {
            size_t iPrev = iCurrent;
            iCurrent = (cWords + iLowerBound) / 2;

            int iComp = lstrcmp(pszLower, _words[iCurrent].c_str());
            cCurrentMatched = CountMatchingCharacters(pszLower, _words[iCurrent].c_str());
            if (cCurrentMatched > cMaxLettersMatched)
            {
                iBest = iCurrent;
                cMaxLettersMatched = cCurrentMatched;
            }
            if (iComp > 0)
            {
                // Look higher.
                iLowerBound = iCurrent + 1;
            }
            else if (iComp < 0)
            {
                // Look lower
                cWords = iCurrent - 1;
            }
            else
            {
                // Found an exact match!
                break;
            }
        }

        // If we already passed a better word, then use it.
        if (cMaxLettersMatched > cCurrentMatched)
        {
            ASSERT(iBest != (size_t)-1);
            iCurrent = iBest;
        }

        // iCurrent is our best match.
        LookupWord(_words[iCurrent], dwGroup);

    }
    return dwGroup;
}

std::string Vocab000::Lookup(WORD wIndex) const
{
    group2words_map::const_iterator it = _mapGroupToString.find((WordGroup)wIndex);
    if (it != _mapGroupToString.end())
    {
        return it->second;
    }
    else
    {
        return "--invalid--";
    }
}

Vocab000::WordGroup Vocab000::_FindLargestEmptyGroup()
{
    DWORD dw = 1;
    // Start at 1.  Group 000 is not used.
    for (; dw < ARRAYSIZE(_rgbGroups); dw++)
    {
        if (_rgbGroups[dw] == 0)
        {
            break;
        }
    }
    WordGroup dwGroup = { dw };
    return dwGroup;
}

HRESULT Vocab000::AddNewWord(PCTSTR pszWordIn, WordClass dwClass, BOOL fShowUI)
{
    std::string strLower = pszWordIn;
    transform(strLower.begin(), strLower.end(), strLower.begin(), tolower);
    HRESULT hr = E_INVALIDARG;
    TCHAR szBuffer[MAX_PATH];
    szBuffer[0] = 0;

    WordGroup dwGroupCurrent;
    if (!LookupWord(strLower, dwGroupCurrent))
    {
        hr = S_OK;
        Vocab000::WordGroup dwGroup = _FindLargestEmptyGroup();
        DWORD dwInfo = InfoFromClassAndGroup(dwClass, dwGroup);
        _rgbGroups[dwGroup] = 0xff;
        _InsertWord(strLower.c_str(), dwGroup);
        _mapGroupToClass[dwGroup] = dwClass;
        _FixupGroupToString(dwGroup);
    }
    else
    {
        StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), TEXT("Failed to add \"%s\": Word already exists!"), pszWordIn, hr);
    }

    if (FAILED(hr))
    {
        ASSERT(szBuffer[0]);
        AfxMessageBox(szBuffer, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
    }

    return hr;
}

void Vocab000::_InsertWord(PCTSTR pszWord, WordGroup dwGroup)
{
    _mapWordToGroup[pszWord] = dwGroup;
    
    // Insert it into the ordered _words array, at the correct spot.
    vector<string>::iterator wordIt = _words.begin();
    for (; wordIt != _words.end(); ++wordIt)
    {
        if (lstrcmp(pszWord, wordIt->c_str()) < 0)
        {
            // We're done.  Insert it here.
            break;
        }
    }
    _words.insert(wordIt, pszWord);
}

HRESULT Vocab000::AddSynonym(PCTSTR pszWordIn, PCTSTR pszOriginal)
{
    std::string strLower = pszWordIn;
    transform(strLower.begin(), strLower.end(), strLower.begin(), tolower);
    HRESULT hr = E_INVALIDARG;
    WordGroup dwGroup;
    if (LookupWord(pszOriginal, dwGroup))
    {
        hr = S_OK;
        _InsertWord(pszWordIn, dwGroup);
        _FixupGroupToString(dwGroup);
    }
    return hr;
}

//
// Similar to AddSynonym, but takes a group (and class) instead of a word.
// Fails if the word is already there.
//
HRESULT Vocab000::AddWord(PCTSTR pszWord, DWORD dwInfo, BOOL fShowUI)
{
    std::string strLower = pszWord;
    transform(strLower.begin(), strLower.end(), strLower.begin(), tolower);
    HRESULT hr = E_INVALIDARG;
    WordGroup dwGroupDummy;
    if (!LookupWord(strLower, dwGroupDummy))
    {
        hr = S_OK;
        _InsertWord(strLower.c_str(), GetWordGroup(dwInfo));
        _mapGroupToClass[GetWordGroup(dwInfo)] = GetWordClass(dwInfo);
        _FixupGroupToString(GetWordGroup(dwInfo));
    }
    else if (fShowUI)
    {
        TCHAR szBuffer[MAX_PATH];
        StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), TEXT("Failed to add \"%s\": Word already exists!"), pszWord);
        AfxMessageBox(szBuffer, MB_ERRORFLAGS);
    }
    return hr;
}



HRESULT Vocab000::SetGroupClass(WordGroup dwGroup, WordClass dwClass)
{
    HRESULT hr = E_INVALIDARG;
    if (_mapGroupToClass.find(dwGroup) != _mapGroupToClass.end())
    {
        // Make sure it exists before setting it.
        hr = S_OK;
        _mapGroupToClass[dwGroup] = dwClass;
    }
    return hr;
}

HRESULT Vocab000::GetGroupClass(WordGroup dwGroup, WordClass *pdwClass)
{
    HRESULT hr = E_INVALIDARG;
    group2class_map::iterator groupClassIt = _mapGroupToClass.find(dwGroup);
    if (groupClassIt != _mapGroupToClass.end())
    {
        hr = S_OK;
        *pdwClass = groupClassIt->second;
    }
    return hr;
}


//
// Ensures there is an entry for the group dwGroup
// This should be called after adding/removing items.
//
void Vocab000::_FixupGroupToString(WordGroup dwGroup)
{
    ASSERT(dwGroup < ARRAYSIZE(_rgbGroups));
    std::string strGroup;
    size_t cMax = _words.size();
    for (size_t i = 0; i < cMax; i++)
    {
        WordGroup dwGroupWord;
        std::string strWord = _words[i];
        BOOL fTemp = LookupWord(strWord, dwGroupWord);
        ASSERT(fTemp); // Else we're corrupt.
        if (dwGroup == dwGroupWord)
        {
            // This is a word from our group, add it to our string.
            if (strGroup.empty())
            {
                strGroup += strWord;
            }
            else
            {
                strGroup += TEXT(" | ");
                strGroup += strWord;
            }
        }
    }

    if (strGroup.empty())
    {
        // There are no more words from this group.  Remove the group.
        _rgbGroups[dwGroup] = 0;
        _mapGroupToString.erase(_mapGroupToString.find(dwGroup));
    }
    else
    {
        _mapGroupToString[dwGroup] = strGroup;
    }
}

HRESULT Vocab000::RemoveWord(PCTSTR pszWord)
{
    std::string strLower = pszWord;
    transform(strLower.begin(), strLower.end(), strLower.begin(), tolower);
    HRESULT hr = E_INVALIDARG;
    WordGroup dwGroup;
    if (LookupWord(strLower, dwGroup))
    {
        hr = S_OK;
        // Found it.  Remove from word array.
        BOOL fFound = FALSE;
        vector<string>::iterator wordIt = _words.begin();
        for (; wordIt != _words.end(); ++wordIt)
        {
            if (strLower == *wordIt)
            {
                // We're done.  Insert it here.
                fFound = TRUE;
                break;
            }
        }
        ASSERT(fFound); // Otherwise it means our _words and our _mapWordToGroup are out of sync!
        _words.erase(wordIt);

        // Remove it from the lookup table.
        _mapWordToGroup.erase(_mapWordToGroup.lower_bound(strLower));

        _FixupGroupToString(dwGroup);
    }
    return hr;
}

//
// Saving functionality that circumvents having to have a CDocument
//
HRESULT Vocab000::QuickSave()
{
    HRESULT hr = E_FAIL;
    sci::istream serial;
    if (Serialize(&serial))
    {
        CResourceMap &map = theApp.GetResourceMap();
        ResourceBlob resource;
        serial.Seek(0);
        if (SUCCEEDED(resource.CreateFromBits(NULL, RS_VOCAB, &serial, _iPackageNumber, _iNumber)))
        {
            hr = map.AppendResource(resource);
        }
    }
    return hr;
}
