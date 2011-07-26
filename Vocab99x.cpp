#include "stdafx.h"
#include "SCIPicEditor.h"
#include "Vocab99x.h"
#include "ResourceMap.h"
#include "CompiledScript.h"

const static int VocabClassTable = 996;
const static int VocabSelectorNames = 997;
const static int VocabKernelNames = 999;

using namespace std;

ResourceBlob *_GetVocabData(int iVocab)
{
    ResourceBlob *pDataRet = NULL;
    ResourceEnumerator *pEnum;
    if (SUCCEEDED(theApp.GetResourceMap().CreateEnumerator(NULL, ResourceTypeFlagVocab, &pEnum)))
    {
        ResourceBlob *pData;
        while (S_OK == pEnum->Next(&pData))
        {
            if (pData->GetNumber() == iVocab)
            {
                pDataRet = pData;
                pData = NULL;
                break;
            }
            delete pData;
        }
        delete pEnum;
    }
    return pDataRet;
}


bool CVocabWithNames::_Create(sci::istream &byteStream, bool fTruncationOk)
{
    _names.clear();
    WORD wMaxIndex;
    byteStream >> wMaxIndex;
    // if the WORD read 400, it means there are 401 entries.
    // REVIEW - really? Someone is asking for kernel 113 (Joystick), but they get a garbage character because of this
    // I'm changing this back to <
    // REVIEW: Yes, really - if we leave this as <, then we'll miss the last entry in the selector names, for example.
    // Hmm - only true for some vocabs it seems.  For non essential cases, the caller can pass true
    // in, and we won't try for the extra item.
    if (!fTruncationOk)
    {
        wMaxIndex++;
    }
    for (WORD i = 0; byteStream.good() && i < wMaxIndex; i++)
    {
        WORD wOffset;
        byteStream >> wOffset;
        DWORD dwSavePos = byteStream.tellg();
        byteStream.seekg(wOffset);
        if (byteStream.good())
        {
            std::string str;
            // Vocab files strings are run-length encoded.
            byteStream.getRLE(str);
            if (byteStream.good())
            {
                _names.push_back(str);
            }
        }
        byteStream.seekg(dwSavePos); // Go back
    }
    return byteStream.good();
}

WORD CVocabWithNames::Add(const string &str)
{
    // Assert that this isn't already in here.
    ASSERT(find(_names.begin(), _names.end(), str) == _names.end());
    _names.push_back(str);
    _fDirty = true;
    return static_cast<WORD>(_names.size() - 1);
}


bool SelectorTable::Load()
{
    bool fRet = false;
    auto_ptr<ResourceBlob> blob(_GetVocabData(VocabSelectorNames));
    if (blob.get())
    {
        fRet = _Create(blob->GetReadStream());
    }
    if (!fRet)
    {
        theApp.LogInfo("Failed to load selector names from vocab resource");
    }
    return fRet;
}
void SelectorTable::Save()
{
    if (_IsDirty())
    {
        // Save ourselves
        const vector<string> &names = GetNames();
        WORD cNames = (WORD)names.size();
        if (cNames > 0)
        {
            vector<BYTE> output;
            // The max index: (size - 1)
            push_word(output, cNames - 1);
            // Then come the offsets - we can run through the strings to calculate these.
            WORD wOffset = (WORD)output.size() + cNames * 2; // the strings will start after the offsets.
            vector<string>::const_iterator nameIt = names.begin();
            while (nameIt != names.end())
            {
                push_word(output, wOffset);
                wOffset += (WORD)(nameIt->length() + 2); // Increase by size of rle string.
                ++nameIt;
            }
            // Now write the strings
            nameIt = names.begin();
            while (nameIt != names.end())
            {
                push_string_rle(output, *nameIt);
                ++nameIt;
            }
            // Now create a resource data for it and save it.
            theApp.GetResourceMap().AppendResource(ResourceBlob(NULL, RS_VOCAB, output, 1, VocabSelectorNames));
        }
    }
}

bool KernelTable::Load()
{
    bool fRet = false;
    auto_ptr<ResourceBlob> blob(_GetVocabData(VocabKernelNames));
    if (blob.get())
    {
        fRet = _Create(blob->GetReadStream());
        // KQ1 is stored differently... null terminated strings.
        // if the normal kind of create fails, we will try that instead...
        if (!fRet)
        {
            theApp.LogInfo("Failed to load kernel names from vocab resource - trying alternate KQ1-style method");
            sci::istream byteStream = blob->GetReadStream();
            while (byteStream.good())
            {
                std::string kernelName;
                byteStream >> kernelName;
                if (byteStream.good())
                {
                    Add(kernelName);
                }
            }
            fRet = true;
        }
    }
    return fRet;
}

bool GlobalClassTable::Load()
{
    bool fRet = FALSE;
    auto_ptr<ResourceBlob> blob(_GetVocabData(VocabClassTable));
    if (blob.get())
    {
        fRet = _Create(blob->GetReadStream());
    }
    if (!fRet)
    {
        theApp.LogInfo("Failed to load class table from vocab resource");
    }
    return fRet;
}

bool GlobalClassTable::_Create(sci::istream &byteStream)
{
    _scriptNums.clear();
    while (byteStream.good())
    {
        byteStream.skip(2);; // Don't know what this is.
        WORD wScriptNum;
        byteStream >> wScriptNum;
        if (byteStream.good())
        {
            _scriptNums.push_back(wScriptNum);
        }
    }
    // Now figure out the class name for each entry.
    WORD wCurrentScript = 0xffff;
    WORD wCurrentIndexInScript = 0xffff;
    CompiledScript *pCompiledScript = NULL;
    for (size_t i = 0; i < _scriptNums.size(); i++)
    {
        if (_scriptNums[i] != wCurrentScript)
        {
            wCurrentScript = _scriptNums[i];
            // Load the script.
            pCompiledScript = new CompiledScript(wCurrentScript);
            if (pCompiledScript)
            {
                if (pCompiledScript->Load((int)wCurrentScript))
                {
                    _scripts.push_back(pCompiledScript);
                }
                else
                {
                    delete pCompiledScript;
                    pCompiledScript = NULL;
                }
            }
            wCurrentIndexInScript = 0;
        }
        else
        {
            wCurrentIndexInScript++;
        }
        vector<WORD> emptyList;
        if (pCompiledScript)
        {
            // Add the class name.
            bool fSuccess = false;
            vector<CompiledObjectBase*> &objects = pCompiledScript->GetObjects();
            while (wCurrentIndexInScript < objects.size())
            {
                CompiledObjectBase *pObject = objects[wCurrentIndexInScript];
                if (pObject->IsInstance())
                {
                    wCurrentIndexInScript++; // Skip instances
                }
                else
                {
                    _classNames.push_back(pObject->GetName());
                    _speciesSelectors.push_back(pObject->GetProperties());
                    _speciesSelectorPropertyValues.push_back(pObject->GetPropertyValues());
                    fSuccess = true;
                    break;
                }
            }
            if (!fSuccess)
            {
                _classNames.push_back("N/A - invalid class index");
                _speciesSelectors.push_back(emptyList);
                _speciesSelectorPropertyValues.push_back(emptyList);
            }
        }
        else
        {
            // No compiled script for some reason... something value.
            _classNames.push_back("N/A - script won't load");
            _speciesSelectors.push_back(emptyList);
            _speciesSelectorPropertyValues.push_back(emptyList);
        }
    }
    ASSERT(_scriptNums.size() == _speciesSelectors.size());
    ASSERT(_scriptNums.size() == _speciesSelectorPropertyValues.size());
    return true; // We're done when we run out of stuff to read... it's not failure.
}

std::string GlobalClassTable::Lookup(WORD wIndex) const
{
    if (wIndex < _classNames.size())
    {
        return _classNames[wIndex];
    }
    else
    {
        return "";
    }
}

GlobalClassTable::~GlobalClassTable()
{
    for (size_t i = 0; i < _scripts.size(); i++)
    {
        delete _scripts[i];
    }
}

bool SpeciesTable::Load()
{
    bool fRet = FALSE;
    auto_ptr<ResourceBlob> blob(_GetVocabData(VocabClassTable));
    if (blob.get())
    {
        fRet = _Create(blob->GetReadStream());
    }
    if (!fRet)
    {
        theApp.LogInfo("Failed to load class table from vocab resource");
    }
    return fRet;
}

void SpeciesTable::Save()
{
    if (_fDirty)
    {
        // Save ourselves
        vector<BYTE> output;
        output.reserve(4 * _direct.size());
        for (vector<WORD>::const_iterator speciesIt = _direct.begin(); speciesIt != _direct.end(); ++speciesIt)
        {
            push_word(output, 0);
            push_word(output, *speciesIt);
        }
        // Now create a resource data for it
        theApp.GetResourceMap().AppendResource(ResourceBlob(NULL, RS_VOCAB, output, 1, VocabClassTable));
    }
}

bool SpeciesTable::_Create(sci::istream &byteStream)
{
    _wNewSpeciesIndex = 0;
    while (byteStream.good())
    {
        byteStream.skip(2); // Don't know what this is.
        WORD wScriptNum;
        byteStream >> wScriptNum;
        if (byteStream.good())
        {
            _direct.push_back(wScriptNum);
            _map[wScriptNum].push_back(_wNewSpeciesIndex++);
        }
    }
    return true;
}

bool SpeciesTable::GetSpeciesIndex(WORD wScript, WORD wClassIndexInScript, SpeciesIndex &wSpeciesIndex) const
{
    bool fRet = false;
    WORD wClassIndex = 0;
    species_map::const_iterator script = _map.find(wScript);
    if (script != _map.end())
    {
        const vector<WORD> &classesInScript = (*script).second;
        if (wClassIndexInScript < (WORD)classesInScript.size())
        {
            fRet = true;
            wSpeciesIndex = classesInScript[wClassIndexInScript];
        }
    }
    return fRet;
}

bool SpeciesTable::GetSpeciesLocation(SpeciesIndex wSpeciesIndex, WORD &wScript, WORD &wClassIndexInScript) const
{
    WORD wType = wSpeciesIndex.Type();
    bool fRet = (wType < static_cast<WORD>(_direct.size()));
    if (fRet)
    {
        wScript = _direct[wType];
        wClassIndexInScript = 0;
        // Go backwards while this is still the same script:
        while (wType && (_direct[wType - 1] == wScript))
        {
            --wType;
            ++wClassIndexInScript;
        }
    }
    return fRet;
}

SpeciesIndex SpeciesTable::MaybeAddSpeciesIndex(WORD wScript, WORD wClassIndexInScript)
{
    SpeciesIndex wSpeciesIndex;
    if (GetSpeciesIndex(wScript, wClassIndexInScript, wSpeciesIndex))
    {
        return wSpeciesIndex;
    }
    else
    {
        // REVIEW: this code is not exception safe.
        // For adding classes in the middle of other classes.
        // phil -> what we're doing here is totally busted.  script numbers do not need to be
        // sequential.  Look at what SCIStudio does - it adds it at the end.  The only thing is,
        // the class #'s change for classes within the current file only.  That could still
        // be catastrophic for some files for the type system.
        // If this script didn't exist in the map before, it should now.
        WORD wNewSpeciesIndex = _wNewSpeciesIndex;
        _map[wScript].push_back(wNewSpeciesIndex);

        // We need to search for this script number in _direct.
        std::vector<WORD>::iterator it = find(_direct.begin(), _direct.end(), wScript);
        if (it == _direct.end())
        {
            // This is the first time this script appears in the species table.  Add it.
            _direct.push_back(wScript);
        }
        else
        {
            // Find the next one that isn't this script.
            std::vector<WORD>::iterator itEnd = find_if(it, _direct.end(), bind2nd(not_equal_to<WORD>(), wScript));
            // Now insert this script here.
            _direct.insert(itEnd, wScript);
        }

        _wNewSpeciesIndex++;
        ASSERT(_wNewSpeciesIndex == (WORD)_direct.size());
        _fDirty = true;
        return wNewSpeciesIndex;
    }
}

// Textual form of all the script names.
std::vector<std::string> SpeciesTable::GetNames() const
{
    std::vector<std::string> names;
    for (size_t i = 0; i < _direct.size(); ++i)
    {
        std::stringstream stream;
        stream << "species " << static_cast<DWORD>(i) << ": script " << _direct[i];
        names.push_back(stream.str());
    }
    return names;
}

bool CVocabWithNames::ReverseLookup(std::string name, WORD &wIndex) const
{
    // PERF: use a more efficient container for looking up words
    std::vector<std::string>::const_iterator position = find(_names.begin(), _names.end(), name);
    bool fRet = (position != _names.end());
    if (fRet)
    {
        wIndex = (WORD)distance(_names.begin(), position);
    }
    return fRet;
}

std::string CVocabWithNames::Lookup(WORD wName) const
{
    std::string strRet;
    if ((size_t)wName < _names.size())
    {
        strRet = _names[wName];
    }
    else
    {
        strRet = _GetMissingName(wName);
    }
    return strRet;
}

std::string KernelTable::_GetMissingName(WORD wName) const
{
    char sz[100];
    StringCchPrintf(sz, ARRAYSIZE(sz), "kernel[%d]", wName);
    return sz;
}

//
// For some reason, SCI0 games are missing the "Joystick" kernel in the kernel
// vocab resource (or rather, the name is garbage characters). Fix that here.
//
const char szMissingKernel[] = "Joystick";
WORD wMissingKernel = 113;
std::string KernelTable::Lookup(WORD wName) const
{
    if (wName == wMissingKernel)
    {
        return szMissingKernel;
    }
    else
    {
        return __super::Lookup(wName);
    }
}
bool KernelTable::ReverseLookup(std::string name, WORD &wIndex) const
{
    if (name == szMissingKernel)
    {
        wIndex = wMissingKernel;
        return true;
    }
    else
    {
        return __super::ReverseLookup(name, wIndex);
    }
}


