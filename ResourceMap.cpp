
#include "stdafx.h"
#include "ResourceMap.h"
#include "SCIPicEditor.h"
#include "ResourceRecency.h"
#include "ViewResource.h"
#include "FontResource.h"
#include "CursorResource.h"
#include "TextResource.h"
#include "SoundResource.h"
#include "PicResource.h"
#include "SaveResourceDialog.h"
#include "RemoveScriptDialog.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Returns "n004" for input of 4
std::string default_reskey(int iNumber)
{
    stringstream ss;
    ss << "n" << setfill('0') << setw(3) << iNumber;
    return ss.str();
}

HRESULT copyfile(const string &destination, const string &source)
{
    return CopyFile(destination.c_str(), source.c_str(), FALSE) ? S_OK : ResultFromLastError();
}

bool deletefile(const string &filename)
{
    return !!DeleteFile(filename.c_str());
}

HRESULT SetFilePositionHelper(HANDLE hFile, DWORD dwPos)
{
    HRESULT hr;
    if (INVALID_SET_FILE_POINTER != SetFilePointer(hFile, dwPos, NULL, FILE_BEGIN))
    {
        hr = S_OK;
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}



HRESULT WriteResourceMapTerminatingBits(HANDLE hFileMap)
{
    HRESULT hr;
    // Write the terminating bits.
    DWORD cbWritten;
    RESOURCEMAPENTRY_SCI0 entryTerm;
    memset(&entryTerm, 0xff, sizeof(entryTerm));
    if (WriteFile(hFileMap, &entryTerm, sizeof(entryTerm), &cbWritten, NULL) && (cbWritten == sizeof(entryTerm)))
    {
        hr = S_OK;
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}

HRESULT ReadNextResourceMapEntry(HANDLE hFile, RESOURCEMAPENTRY_SCI0 *pEntry)
{
    HRESULT hr = E_FAIL;

    DWORD cb;
    if (ReadFile(hFile, pEntry, sizeof(*pEntry), &cb, NULL))
    {
        if (cb == sizeof(*pEntry))
        {
            if ((pEntry->iType == 0x1f) && (pEntry->iNumber == 0x7ff) && (pEntry->iPackageNumber == 0x3f) && (pEntry->iOffset == 0x3ffffff))
            {
                hr = S_FALSE; // We're done.
            }
            else
            {
                hr = S_OK;
            }
        }
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}

// c:\foo\resource.map.bak
string resmap_filename_bak(const string &folder)
{
    stringstream ss;
    ss << folder << "\\" << "resource.map.bak";
    return ss.str();
}

// c:\foo\resource.map
string resmap_filename(const string &folder)
{
    stringstream ss;
    ss << folder << "\\" << "resource.map";
    return ss.str();
}

// c:\foo\resource.001.bak
string package_filename_bak(const string &folder, int iPackage)
{
    stringstream ss;
    ss << folder << "\\" << "resource." << setw(3) << setfill('0') << iPackage << ".bak";
    return ss.str();
}

// c:\foo\resource.001
string package_filename(const string &folder, int iPackage)
{
    stringstream ss;
    ss << folder << "\\" << "resource." << setw(3) << setfill('0') << iPackage;
    return ss.str();
}

HRESULT TestForReadOnly(const string &filename)
{
    HRESULT hr = S_OK;
    DWORD dwAttribs = GetFileAttributes(filename.c_str());
    if (INVALID_FILE_ATTRIBUTES == dwAttribs)
    {
        hr = ResultFromLastError();
    }
    else if (dwAttribs & FILE_ATTRIBUTE_READONLY)
    {
        hr = HRESULT_FROM_WIN32(ERROR_FILE_READ_ONLY);
    }
    return hr;
}

HRESULT TestDelete(const string &filename)
{
    HRESULT hr = S_OK;
    HANDLE hFile = CreateFile(filename.c_str(), DELETE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}

//
// Detect any errors before we try to write to the resource.map and resource.xxx.
// There is still a race condition here, but that shouldn't be too common.
//
HRESULT TestMapAndPackageWrite(const string &gameFolder, int iPackage)
{
    // Test the package.
    std::string respackage = package_filename(gameFolder, iPackage);
    HRESULT hr = TestDelete(respackage);
    if (SUCCEEDED(hr))
    {
        // Now the map
        std::string resmap = resmap_filename(gameFolder);
        hr = TestDelete(resmap);
    }
    return hr;
}


//
// Replaces resource.nnn with resource.nnn.bak
//
HRESULT ReplaceResourcePackage(const string &gameFolder, int iPackage)
{
    HRESULT hr = E_FAIL;
    string package_name = package_filename(gameFolder, iPackage);
    if (deletefile(package_name))
    {
        hr = MoveFile(package_filename_bak(gameFolder, iPackage).c_str(), package_name.c_str()) ? S_OK : E_FAIL;
    }

    if (FAILED(hr))
    {
        hr = ResultFromLastError();
    }
    return hr;
}

//
// Replaces resource.map with resource.map.bak
//
HRESULT ReplaceResourceMap(const string &gameFolder)
{
    HRESULT hr = E_FAIL;
    string resmap_name = resmap_filename(gameFolder);
    if (deletefile(resmap_name))
    {
        hr = MoveFile(resmap_filename_bak(gameFolder).c_str(), resmap_name.c_str()) ? S_OK : E_FAIL;
    }

    if (FAILED(hr))
    {
        hr = ResultFromLastError();
    }
    return hr;
}


//
// Open a backup resource.nnn.bak package for writing.
//
HRESULT OpenResourcePackageBak(const string &gameFolder, int iPackage, HANDLE *pHandle)
{
    HRESULT hr;
    // Open the main map file.
    *pHandle = CreateFile(package_filename_bak(gameFolder, iPackage).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
                                    FILE_ATTRIBUTE_NORMAL, NULL);
    if (*pHandle != INVALID_HANDLE_VALUE)
    {
        hr = S_OK;
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}


//
// Open a backup resource.map for writing.
//
HRESULT OpenResourceMapBak(const string &gameFolder, HANDLE *pHandle)
{
    HRESULT hr;
    // Open the main map file.
    *pHandle = CreateFile(resmap_filename_bak(gameFolder).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
                                    FILE_ATTRIBUTE_NORMAL, NULL);

    if (*pHandle != INVALID_HANDLE_VALUE)
    {
        hr = S_OK;
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}

//
// Opens the resource map in pszGameFolder, for reading;  or if fWrite is specified,
// then for both reading and writing.
//
HRESULT OpenResourceMap(const string &gameFolder, BOOL fWrite, HANDLE *pHandle)
{
    HRESULT hr;
    // Open the main map file.
    *pHandle = CreateFile(resmap_filename(gameFolder).c_str(), GENERIC_READ | (fWrite ? GENERIC_WRITE : 0), FILE_SHARE_READ, NULL, OPEN_EXISTING, 
                                    FILE_ATTRIBUTE_NORMAL, NULL);

    if (*pHandle != INVALID_HANDLE_VALUE)
    {
        hr = S_OK;
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}

//
// Opens a particular resource package, and moves the file pointer to dwOffset
//
HRESULT MoveToPackage(DWORD dwOffset, int iPackage, const string &gameFolder, HANDLE *pHandle)
{
    HRESULT hr;
    *pHandle = CreateFile(package_filename(gameFolder, iPackage).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (*pHandle != INVALID_HANDLE_VALUE)
    {
        // Move it to the right spot.
        if (INVALID_SET_FILE_POINTER != SetFilePointer(*pHandle, dwOffset, NULL, FILE_BEGIN))
        {
            hr = S_OK;
        }
        else
        {
            hr = ResultFromLastError();
        }
    }
    else
    {
        hr = ResultFromLastError();
    }
    if (FAILED(hr))
    {
        theApp.LogInfo(TEXT("Failed to move to a point %x in a package file %x: %x"), dwOffset, iPackage, hr);
    }
    return hr;
}

bool IsValidPackageNumber(int iPackageNumber)
{
    return (iPackageNumber >= 0) && (iPackageNumber < 63);
}

ResourceEnumerator::ResourceEnumerator()
{
    _dwTypes = 0;
    _hFileMap = INVALID_HANDLE_VALUE;
    _fDontCloseHandle = FALSE;
    _hFilePackage = INVALID_HANDLE_VALUE;
    _iCurrentPackage = NULL;
    _pResourceRecency = NULL;
}

void ResourceEnumerator::Init(ResourceRecency *pResourceRecency, const std::string &folder, HANDLE hFile, DWORD dwTypes)
{
    _pResourceRecency = pResourceRecency; // Weak ref
    _gameFolder = folder;
    ASSERT(hFile != NULL); // Should be a handle, or INVALID_HANDLE_VALUE
    // Can't use CString + here, because strFolder is actually the full path to the 
    // resource.map, with a null inserted before "resource.map".
    _dwTypes = dwTypes;
    _hFileMap = hFile;
    // If we we're given a handle to use, don't close it on delete.
    _fDontCloseHandle = (hFile != INVALID_HANDLE_VALUE);
    // If we're given a file handle, make sure it is set to the beginning:
    if (hFile != INVALID_HANDLE_VALUE)
    {
        SetFilePointer(hFile, 0, 0, FILE_BEGIN);
    }
    _hFilePackage = INVALID_HANDLE_VALUE;
    _iCurrentPackage = -1;

    // Someone is about to enumerate the resources.  Clear the global recency table for these.
    if (_pResourceRecency)
    {
        for (int i = 0; i < NumResourceTypes; i++)
        {
            if (dwTypes & (1 << i))
            {
                // This resource type is being re-enumerated.
                _pResourceRecency->ClearResourceType(i);
            }
        }
    }

    // Start a background thread to load the class browser.

}

void ResourceEnumerator::Init(ResourceRecency *pResourceRecency, const std::string &folder, DWORD dwTypes)
{
    Init(pResourceRecency, folder, INVALID_HANDLE_VALUE, dwTypes);
}

ResourceEnumerator::~ResourceEnumerator()
{
    if (!_fDontCloseHandle && (_hFileMap != INVALID_HANDLE_VALUE))
    {
        CloseHandle(_hFileMap);
        _hFileMap = INVALID_HANDLE_VALUE;
    }

    if (_hFilePackage != INVALID_HANDLE_VALUE)
    {
        CloseHandle(_hFilePackage);
        _hFilePackage = INVALID_HANDLE_VALUE;
    }
}


//
// Determines if pszName is possibly a resource filename, and if so, returns a pointer
// to the character past the resource type name, and the resource type.
//
PCTSTR IsResourceName(PCTSTR pszName, DWORD *pdwType)
{
    PCTSTR psz = NULL;
    for (DWORD i = 0; !psz && (i < ARRAYSIZE(g_resourceInfo)); i++)
    {
        int cch = lstrlen(g_resourceInfo[i].pszFileNameDefault);
        if (cch <= lstrlen(pszName))
        {
            if (CSTR_EQUAL == CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, g_resourceInfo[i].pszFileNameDefault, cch, pszName, cch))
            {
                psz = pszName + cch;
                *pdwType = i;
            }
        }
    }
    return psz;
}

//
// RS_*** to RTF_***
//
DWORD ResourceTypeToFlag(DWORD dwType)
{
    return 1 << dwType;
}

//
// RTF_*** to RS_***
//
DWORD ResourceFlagToType(DWORD dwFlags)
{
    DWORD dwType = dwFlags;
    int iShifts = 0;
    while (dwType > 1)
    {
        dwType = dwType >> 1;
        iShifts++;
    }
    return iShifts;
}

PCTSTR ResourceTypeToText(DWORD dwType)
{
    return (g_resourceInfo[dwType].pszTitleDefault);
}



//
// Opens the resource map for reading.
//
HRESULT ResourceEnumerator::_OpenResource()
{
    ASSERT(_hFileMap == INVALID_HANDLE_VALUE);
    HRESULT hr = OpenResourceMap(_gameFolder, FALSE, &_hFileMap);

    if (FAILED(hr))
    {
        theApp.LogInfo(TEXT("Failed to open resource map for folder %s: %x"), _gameFolder.c_str(), hr);
    }
    return hr;
}

BOOL CSCI0ResourceEnumerator::_IsCorrectType(int iType)
{
    return (_dwTypes & ResourceTypeToFlag(iType));
}

HRESULT CSCI0ResourceEnumerator::_ReadNextMapEntry(RESOURCEMAPENTRY_SCI0 *pEntry)
{
    return ReadNextResourceMapEntry(_hFileMap, pEntry);
}

HRESULT ResourceEnumerator::_MoveToPackage(DWORD dwOffset, int iPackage)
{
    HRESULT hr;
    if (iPackage != _iCurrentPackage)
    {
        // Close the current one and open a new one.
        if (_hFilePackage != INVALID_HANDLE_VALUE)
        {
            CloseHandle(_hFilePackage);
            _hFilePackage = INVALID_HANDLE_VALUE;
        }

        _hFilePackage = CreateFile(package_filename(_gameFolder, iPackage).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        _hrPackageError = ResultFromLastError();
        if ((_hFilePackage == INVALID_HANDLE_VALUE) && FAILED(_hrPackageError))
        {
            theApp.LogInfo("Couldn't open %s: %x", package_filename(_gameFolder, iPackage).c_str(), _hrPackageError);
        }
        _iCurrentPackage = (_hFilePackage == INVALID_HANDLE_VALUE) ? -1 : iPackage;
    }

    if (_hFilePackage != INVALID_HANDLE_VALUE)
    {
        // Move it to the right spot.
        if (INVALID_SET_FILE_POINTER != SetFilePointer(_hFilePackage, dwOffset, NULL, FILE_BEGIN))
        {
            hr = S_OK;
        }
        else
        {
            hr = ResultFromLastError();
        }
    }
    else
    {
        hr = _hrPackageError;
    }
    return hr;
}

//
// Resources we shouldn't load (use some other kind of compression we don't
// understand).
//
BOOL _ResourceException(RESOURCEMAPENTRY_SCI0 *prme)
{
    // Allow all for now.
    /*if ((prme->iType == RS_VOCAB) && (prme->iNumber != 000))
    {
        // Vocab.900 is bad
        return TRUE;
    }*/
    return FALSE;
}

//
// Returns the next resource of the type requested.
// Keep calling it until it returns failure or S_FALSE.
//
HRESULT CSCI0ResourceEnumerator::Next(ResourceBlob **ppData, WORD wNumber)
{
    HRESULT hr = S_OK;
    if (_hFileMap == INVALID_HANDLE_VALUE)
    {
        hr = _OpenResource();
    }

    while (SUCCEEDED(hr))
    {
        RESOURCEMAPENTRY_SCI0 rme;
        // Read an entry from the map
        hr = _ReadNextMapEntry(&rme);
        if (hr == S_OK)
        {
            // If it's of a type we're looking for
            if (_IsCorrectType(rme.iType) &&
                ((wNumber == InvalidResourceNumber) || (rme.iNumber == (int)wNumber)))
            {
                if (!_ResourceException(&rme))
                {
                    // Then open the package file to the right spot
                    hr = _MoveToPackage(rme.iOffset, rme.iPackageNumber);
                    if (SUCCEEDED(hr))
                    {
                        // And create a ResourceBlob from that spot.
                        std::auto_ptr<ResourceBlob> pData(new ResourceBlob());
                        hr = pData->CreateFromHandle(NULL, _hFilePackage, FALSE, rme.iPackageNumber);
                        if (SUCCEEDED(hr))
                        {
                            *ppData = pData.release();

                            // Found one!
                            // Add it to the END of the recency table.  The first ones we read (of a given number/package)
                            // are the most recent ones.  Later ones are less recent.
                            if (_pResourceRecency)
                            {
                                _pResourceRecency->AddResourceToRecency(*ppData, TRUE);
                            }
                            break;
                        }
                        else
                        {
                            theApp.LogInfo(TEXT("Failed to create resource data: %x"), hr);
                        }
                    }
                    else
                    {
                        theApp.LogInfo(TEXT("Failed to open resource package %d: %x"), rme.iPackageNumber, hr);
                    }
                }
            } // If not the right type, keep going.
        }
    }
    if (FAILED(hr))
    {
        theApp.LogInfo(TEXT("Failed to read resource entry: %x"), hr);
    }
    return hr;
}

HRESULT CSCI1ResourceEnumerator::_EnsureMapHeader()
{
    ASSERT(_hFileMap != INVALID_HANDLE_VALUE);
    HRESULT hr = S_OK;
    if (!_fMapHeaderValid)
    {
        ZeroMemory(_mapHeader, sizeof(_mapHeader));

        BOOL fDone = FALSE;
        int i = 0;
        while (!fDone && SUCCEEDED(hr) && (i < ARRAYSIZE(_mapHeader)))
        {
            DWORD dwRead;
            RESOURCEMAPHEADER_SCI1 headerEntry;
            if (ReadFile(_hFileMap, &headerEntry, sizeof(headerEntry), &dwRead, NULL) && (sizeof(headerEntry) == dwRead))
            {
                // Read an entry.
                CopyMemory(&_mapHeader[i], &headerEntry, sizeof(_mapHeader[i]));
                i++;

                if (headerEntry.bType == 0xff)
                {
                    // We're done, but we still read the entry into memory, since the offset is necessary.
                    fDone = TRUE;
                }
            }
            else
            {
                hr = ResultFromLastError();
            }
        }

        _fMapHeaderValid = SUCCEEDED(hr);
        _iMapHeaderIndex = -1;
    }
    return hr;
}

//
// Resource enumerator for SCI1 resources.
// Not yet implemented.
//
HRESULT CSCI1ResourceEnumerator::Next(ResourceBlob **ppData, WORD wNumber)
{
    ASSERT(FALSE);

    HRESULT hr = S_OK;
    *ppData = NULL;
    if (_hFileMap == INVALID_HANDLE_VALUE)
    {
        hr = _OpenResource();
    }

    if (SUCCEEDED(hr))
    {
        hr = _EnsureMapHeader();
    }

    if (SUCCEEDED(hr))
    {
        BOOL fGoToNextMapHeader = (_iMapHeaderIndex == -1);

        if (fGoToNextMapHeader)
        {
            // Find a map header.
            for (DWORD i = (_iMapHeaderIndex + 1); i < ARRAYSIZE(_mapHeader); i++)
            {
                hr = S_FALSE;
                if (_mapHeader[i].bType == 0xff)
                {
                    hr = S_FALSE;
                    break;
                }
                else
                {
                    // By the last entry, we must have encountered a 0xff
                    ASSERT(i < ARRAYSIZE(_mapHeader) - 1);
                }
                DWORD dwType = _mapHeader[i].bType - RS_SCI1_FIRST;
                ASSERT(dwType > 0);
                ASSERT(dwType < NumResourceTypes);
                if (ResourceTypeToFlag(dwType) & _dwTypes)
                {
                    _iMapHeaderIndex = i;
                    _cbRemaining = (DWORD)(_mapHeader[_iMapHeaderIndex + 1].wOffset - _mapHeader[_iMapHeaderIndex].wOffset);
                    // They asked for this type of resource.  Find the resource header arrays.
                    hr = SetFilePositionHelper(_hFileMap, (DWORD)_mapHeader[_iMapHeaderIndex].wOffset);
                    break;
                }
            }
        }

        if (hr == S_OK)
        {
            // Then the _hFileMap should be positioned at a header.
            if (_cbRemaining > 0)
            {
                DWORD dwRead;
                RESOURCEMAPENTRY_SCI1 mapEntry;
                if (ReadFile(_hFileMap,  &mapEntry, sizeof(mapEntry), &dwRead, NULL) && (dwRead == sizeof(mapEntry)))
                {
                    _cbRemaining -= sizeof(mapEntry);

                    // Now we have the map entry.
                    hr = _MoveToPackage(mapEntry.iOffset, mapEntry.iPackageNumber);

                    if (SUCCEEDED(hr))
                    {
                        // And create a ResourceBlob from that spot.
                        //hr = CSCI1ResourceData::CreateFromHandle(NULL, _hFilePackage, FALSE, rme.iPackageNumber, ppData);
                        if (SUCCEEDED(hr))
                        {
                            // Found one!
                            // Add it to the END of the recency table.  The first ones we read (of a given number/package)
                            // are the most recent ones.  Later ones are less recent.
                            if (_pResourceRecency)
                            {
                                _pResourceRecency->AddResourceToRecency(*ppData, TRUE);
                            }
                        }
                    }
                }
                else
                {
                    hr = ResultFromLastError();
                }
            }
            else
            {
                // TODO: we need to go to the next header.... up above
            }

        }

    }
    return hr;
}


HRESULT RebuildResources(BOOL fShowUI)
{
    BYTE rgDone[NumResourceTypes][1000];
    int x = sizeof(rgDone);
    ZeroMemory(rgDone, sizeof(rgDone));

    std::string gameFolder = theApp.GetResourceMap().GetGameFolder();

    HANDLE hFileMapBak;
    HRESULT hr = OpenResourceMapBak(gameFolder, &hFileMapBak);
    if (SUCCEEDED(hr))
    {
        HANDLE hFilePackageBak;
        // Open the resource package 1
        hr = OpenResourcePackageBak(gameFolder, 1, &hFilePackageBak);
        if (SUCCEEDED(hr))
        {
            // Now open the main map.
            HANDLE hFileMap;
            hr = OpenResourceMap(gameFolder, TRUE, &hFileMap);
            if (SUCCEEDED(hr))
            {
                HANDLE hFilePackage = INVALID_HANDLE_VALUE;
                int iCurrentPackage = -1;
                DWORD dwOffset = 0;

                RESOURCEMAPENTRY_SCI0 entry;
                while (SUCCEEDED(hr) && (S_OK == ReadNextResourceMapEntry(hFileMap, &entry)))
                {
                    if (IsValidResourceType(entry.iType) && IsValidResourceNumber(entry.iNumber))
                    {
                        // If we haven't already done this resource, do it now.
                        if (!rgDone[entry.iType][entry.iNumber])
                        {
                            // Mark it as done.
                            rgDone[entry.iType][entry.iNumber] = 0xff;

                            // Ensure we have the right source package file open
                            if (iCurrentPackage != entry.iPackageNumber)
                            {
                                if (hFilePackage != INVALID_HANDLE_VALUE)
                                {
                                    CloseHandle(hFilePackage);
                                }
                                hr = MoveToPackage(entry.iOffset, entry.iPackageNumber, gameFolder, &hFilePackage);
                                if (SUCCEEDED(hr))
                                {
                                    iCurrentPackage = entry.iPackageNumber;
                                }
                            }
                            else
                            {
                                ASSERT(hFilePackage != INVALID_HANDLE_VALUE);
                                // Move it to the right spot.
                                if (INVALID_SET_FILE_POINTER != SetFilePointer(hFilePackage, entry.iOffset, NULL, FILE_BEGIN))
                                {
                                    hr = S_OK;
                                }
                                else
                                {
                                    hr = ResultFromLastError();
                                }
                            }

                            if (SUCCEEDED(hr))
                            {
                                // Adjust the resource map entry.
                                entry.iPackageNumber = 1; // Everything goes into package 1
                                entry.iOffset = dwOffset;

                                // Transfer the resource data.
                                ResourceBlob data;
                                hr = data.CreateFromHandle("", hFilePackage, FALSE, 1);
                                if (SUCCEEDED(hr))
                                {
                                    DWORD cbWrittenByResource;
                                    hr = data.SaveToHandle(hFilePackageBak, FALSE, &cbWrittenByResource);
                                    if (SUCCEEDED(hr))
                                    {
                                        ASSERT(cbWrittenByResource == data.GetLengthOnDisk()); // Otherwise, we're corrupt.
                                        // Advance our offset.
                                        dwOffset += data.GetLengthOnDisk();

                                        DWORD cbWritten;
                                        // Write into our new resource map.
                                        if (WriteFile(hFileMapBak, &entry, sizeof(entry), &cbWritten, NULL) && (cbWritten == sizeof(entry)))
                                        {
                                            hr = S_OK;
                                        }
                                        else
                                        {
                                            hr = ResultFromLastError();
                                        }
                                    }
                                }
                                else
                                {
                                    theApp.LogInfo(TEXT("Failed to create resource while rebuilding resources. %x"), hr);
                                }
                            }
                        }
                    }
                }

                if (SUCCEEDED(hr))
                {
                    hr = WriteResourceMapTerminatingBits(hFileMapBak);
                }

                if (hFilePackage != INVALID_HANDLE_VALUE)
                {
                    CloseHandle(hFilePackage);
                }

                CloseHandle(hFileMap);
            }

            CloseHandle(hFilePackageBak);
        }
            
        CloseHandle(hFileMapBak);
    }

    BOOL fReplacing = FALSE;
    if (SUCCEEDED(hr))
    {
        hr = ReplaceResourcePackage(gameFolder, 1);
        if (SUCCEEDED(hr))
        {
            // This had better work from now on...
            fReplacing = TRUE;
            hr = ReplaceResourceMap(gameFolder);
        }
    }

    if (FAILED(hr) && fShowUI)
    {
        TCHAR szMsg[MAX_PATH];
        StringCchPrintf(szMsg, ARRAYSIZE(szMsg), TEXT("An error occured while rebuilding resources: %x%s"), hr, fReplacing ? TEXT(". Resources may be corrupt.") : TEXT(""));
        AfxMessageBox(szMsg, MB_ERRORFLAGS);
    }

    return hr;
}





//
// CResourceMap
// Helper class for managing resources.
//
CResourceMap::CResourceMap()
{
    _pVocab000 = NULL;
    _cDeferAppend = 0;
    _language = LangSyntaxUnknown;
}

CResourceMap::~CResourceMap()
{
    ASSERT(_syncs.empty()); // They should remove themselves.
    _classBrowser.Reset();
    if (_pVocab000)
    {
        delete _pVocab000;
    }
    ASSERT(_cDeferAppend == 0);
}

HRESULT CResourceMap::_UpdateMap(HANDLE hFileMap, DWORD dwOffset, const ResourceBlob &data)
{
    RESOURCEMAPENTRY_SCI0 rme;
    rme.iPackageNumber = data.GetPackageHint();
    rme.iNumber = data.GetNumber();
    rme.iOffset = dwOffset;
    rme.iType = data.GetType();
    ASSERT(IsValidPackageNumber(rme.iPackageNumber));

    HRESULT hr = E_INVALIDARG;
    // Move everything to the end of the file, then write ourselves in the beginning
    DWORD dwSize = GetFileSize(hFileMap, NULL);
    if (INVALID_FILE_SIZE != dwSize)
    {
        scoped_array<BYTE> buf(new BYTE[dwSize]);
        DWORD cbRead;
        // Read all the old data.
        if (ReadFile(hFileMap, buf.get(), dwSize, &cbRead, NULL) && (cbRead == dwSize))
        {
            // Go back to the beginning.
            if (INVALID_SET_FILE_POINTER != SetFilePointer(hFileMap, 0, 0, FILE_BEGIN))
            {
                DWORD cbWritten;
                // Stick our entry there.
                if (WriteFile(hFileMap, &rme, sizeof(rme), &cbWritten, NULL) && (cbWritten == sizeof(rme)))
                {
                    // Then write the rest.
                    if (WriteFile(hFileMap, buf.get(), dwSize, &cbWritten, NULL) && (cbWritten == dwSize))
                    {
                        hr = S_OK;
                    }
                    else
                    {
                        hr = ResultFromLastError();
                    }
                }
                else
                {
                    hr = ResultFromLastError();
                }
            }
            else
            {
                hr= ResultFromLastError();
            }
        }
        else
        {
            hr = ResultFromLastError();
        }
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}

HRESULT CResourceMap::_WriteResourceI(HANDLE hFileMap, HANDLE hFilePackage, const ResourceBlob &data)
{
    HRESULT hr = S_OK;
    // First write to the package.  Move to the end of the file.
    // And get the file size too.
    DWORD dwSize;
    DWORD dwSizeHigh;
    dwSize = GetFileSize(hFilePackage, &dwSizeHigh);
    if (dwSize != INVALID_FILE_SIZE)
    {
        if ((dwSizeHigh == 0) && (dwSize < 67108864))  // 2 ^ 26
        {
            if (INVALID_SET_FILE_POINTER != SetFilePointer(hFilePackage, 0, NULL, FILE_END))
            {
                // Start writing!
                // First write the resource to the package.
                hr = data.SaveToHandle(hFilePackage, FALSE);
                if (SUCCEEDED(hr))
                {
                    // Then update the map
                    hr = _UpdateMap(hFileMap, dwSize, data);
                }
            }
            else     
            {
                hr = ResultFromLastError();
            }
        }
        else
        {
            // This file is already too large to handle the offset.
            hr = E_FAIL;
        }
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}

//
// Adds the name of the resource to the game.ini file.
//
void CResourceMap::AssignName(const ResourceBlob &resource)
{
    // Assign the name of the item.
    std::string keyName = default_reskey(resource.GetNumber());
    std::string name = resource.GetName();
    if (!name.empty() && (0 != lstrcmpi(keyName.c_str(), name.c_str())))
    {
        SetIniString(g_resourceInfo[resource.GetType()].pszTitleDefault, keyName, name);
    }
}

void CResourceMap::AssignName(int iType, int iResourceNumber, PCTSTR pszName)
{
    // Assign the name of the item.
    std::string keyName = default_reskey(iResourceNumber);
    if (pszName && *pszName && (0 != lstrcmpi(keyName.c_str(), pszName)))
    {
        SetIniString(g_resourceInfo[iType].pszTitleDefault, keyName, pszName);
    }
}

HRESULT CResourceMap::_GetMapAndPackage(const ResourceBlob &resource, HANDLE *phFileMap, HANDLE *phFilePackage)
{
    return _GetMapAndPackageI(resource, FALSE, phFileMap, phFilePackage);
}

HRESULT CResourceMap::_GetMapAndPackageI(const ResourceBlob &resource, BOOL fBak, HANDLE *phFileMap, HANDLE *phFilePackage)
{
    bool fSuccess = false;
    std::string resmapName = fBak ? resmap_filename_bak() : resmap_filename();
    // Open the main map file.
    *phFileMap = CreateFile(resmapName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 
                                    FILE_ATTRIBUTE_NORMAL, NULL);
    if (*phFileMap != INVALID_HANDLE_VALUE)
    {
        std::string packageMap = fBak ? package_filename_bak(resource.GetPackageHint()) : package_filename(resource.GetPackageHint());
        *phFilePackage = CreateFile(packageMap.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL, NULL);
        if (*phFilePackage != INVALID_HANDLE_VALUE)
        {
            fSuccess = true;
        }
        else
        {
            CloseHandle(*phFileMap);
        }
    }
    return fSuccess ? S_OK : ResultFromLastError();
}

//
// Makes .bak versions of resource.map and resource.xxx, and opens them.
//
HRESULT CResourceMap::_CopyAndGetBakMapAndPackage(const ResourceBlob &resource, HANDLE *phFileMapBak, HANDLE *phFilePackageBak)
{
    // First the resource map - test for readonly first, because otherwise we'll create readonly
    // .bak files, which we'll never be able to overwrite.  Future CopyFiles will fail with "access denied"
    HRESULT hr = TestForReadOnly(resmap_filename());
    if (SUCCEEDED(hr))
    {
        hr = copyfile(resmap_filename(), resmap_filename_bak());
        if (SUCCEEDED(hr))
        {
            // Then the resource package
            hr = TestForReadOnly(package_filename(resource.GetPackageHint()));
            if (SUCCEEDED(hr))
            {
                hr = copyfile(package_filename(resource.GetPackageHint()),
                              package_filename_bak(resource.GetPackageHint()));
                if (SUCCEEDED(hr))
                {
                    // Ok, we copied both files - now let's open them, passing TRUE for fBak
                    hr = _GetMapAndPackageI(resource, TRUE, phFileMapBak, phFilePackageBak);
                }
            }
        }
    }
    return hr;
}

void CResourceMap::BeginDeferAppend()
{
    ASSERT((_cDeferAppend > 0) || _deferredResources.empty());
    ++_cDeferAppend;
}
void CResourceMap::AbandonAppend()
{
    // Abandon all the resources we were piling up.
    if (_cDeferAppend)
    {
        --_cDeferAppend;
        if (_cDeferAppend == 0)
        {
            _deferredResources.clear();
        }
    }
}
HRESULT CResourceMap::EndDeferAppend()
{
    HRESULT hr = S_OK;
    if (_cDeferAppend)
    {
        --_cDeferAppend;
        if (_cDeferAppend == 0)
        {
            if (!_deferredResources.empty())
            {
                hr = E_FAIL; // failure from here on out...
                // Commit the resources we were piling up.
                HANDLE hFileMapBak, hFilePackageBak;
                // Always do the loop anyway, because we have to delete our ResourceBlob's.
                bool reload[NumResourceTypes];
                ZeroMemory(reload, sizeof(reload));
                for (size_t i = 0; i < _deferredResources.size(); i++)
                {
                    ResourceBlob &resource = _deferredResources[i];
                    // Reopen the map and package with each resource... we could optimize this, but
                    // we'd need to retool the way _WriteResourceI works.
                    hr = _CopyAndGetBakMapAndPackage(resource, &hFileMapBak, &hFilePackageBak);
                    if (SUCCEEDED(hr))
                    {
                        bool fSuccess = false;
                        hr = _WriteResourceI(hFileMapBak, hFilePackageBak, resource);
                        if (SUCCEEDED(hr))
                        {
                            AssignName(resource);
                            reload[resource.GetType()] = true;
                            fSuccess = true;
                        }
                        CloseHandle(hFileMapBak);
                        CloseHandle(hFilePackageBak);

                        // Now replace
                        if (fSuccess)
                        {
                            hr = TestMapAndPackageWrite(_gameFolder, resource.GetPackageHint());
                            if (SUCCEEDED(hr))
                            {
                                // Replace the resource package first.  It's ok if that succeeds, but the resource map fails.
                                // (since we append to the end of resource package).
                                hr = ReplaceResourcePackage(_gameFolder, resource.GetPackageHint());
                                if (SUCCEEDED(hr))
                                {
                                    hr = ReplaceResourceMap(_gameFolder);
                                }
                            }
                        }
                    }
                }
                _deferredResources.clear();

                // Tell the views that might have changed, to reload:
                for (int iType = 0; iType < ARRAYSIZE(reload); iType++)
                {
                    if (reload[iType])
                    {
                        ReloadResourceType(iType);
                    }
                }
            }
        }
    }
    return hr;
}

//
// Suggest an unused resource number for this type.
//
int CResourceMap::SuggestResourceNumber(ResourceType type)
{
    int iNumber = 0;
    // Figure out a number to suggest...
    bool rgNumbers[1000];
    ZeroMemory(rgNumbers, sizeof(rgNumbers));
    auto_ptr<ResourceEnumerator> resEnum(CreateEnumerator(NULL, ResourceTypeToFlag(type))); // Guard against exceptions
    if (resEnum.get())
    {
        ResourceBlob *pBlob;
        while (S_OK == resEnum.get()->Next(&pBlob))
        {
            auto_ptr<ResourceBlob> blob(pBlob); // Guard against exceptions
            int iThisNumber = pBlob->GetNumber();
            if (iThisNumber >= 0 && iThisNumber < ARRAYSIZE(rgNumbers))
            {
                rgNumbers[iThisNumber] = true;
            }
        }

        // Find the first one that is still false
        // (iterators on c style arrays are pointers to those arrays)
        bool *pNumber = find(rgNumbers, &rgNumbers[ARRAYSIZE(rgNumbers)], false);
        iNumber = (int)(pNumber - rgNumbers);
    }
    return iNumber;
}


//
// Ask the user where to save the resource... and then save it.
//
HRESULT CResourceMap::AppendResourceAskForNumber(ResourceBlob &resource)
{
    // Invoke dialog to suggest/ask for a resource number
    SaveResourceDialog srd;
    srd.Init(-1, SuggestResourceNumber(resource.GetType()));
    if (IDOK == srd.DoModal())
    {
        // Assign it.
        resource.SetNumber(srd.GetResourceNumber());
        resource.SetPackage(srd.GetPackageNumber());

        // Save it.
        return AppendResource(resource);
    }
    else
    {
        return E_FAIL; // User cancelled.
    }
}

HRESULT CResourceMap::AppendResource(const ResourceBlob &resource)
{
    HRESULT hr;
    if (_cDeferAppend)
    {
        // If resource appends are deferred, then just add it to a list.
        _deferredResources.push_back(resource);
        hr = S_OK;
    }
    else
    {
        HANDLE hFileMapBak, hFilePackageBak;
        hr = _CopyAndGetBakMapAndPackage(resource, &hFileMapBak, &hFilePackageBak);
        if (SUCCEEDED(hr))
        {
            hr = _WriteResourceI(hFileMapBak, hFilePackageBak, resource);
            CloseHandle(hFilePackageBak);
            CloseHandle(hFileMapBak);

            if (SUCCEEDED(hr))
            {
                hr = TestMapAndPackageWrite(_gameFolder, resource.GetPackageHint());
                if (SUCCEEDED(hr))
                {
                    // Now replace.  Replace package first, since it's ok for the map to fail if 
                    // the package succeeds (since we appended to the end of the resource package).
                    hr = ReplaceResourcePackage(_gameFolder, resource.GetPackageHint());
                    if (SUCCEEDED(hr))
                    {
                        hr = ReplaceResourceMap(_gameFolder);
                    }
                }
            }
        }

        AssignName(resource);

        if (SUCCEEDED(hr))
        {
            // pResource is only valid for the length of this call.  Nonetheless, call our syncs
            for_each(_syncs.begin(), _syncs.end(), bind2nd(mem_fun(&ISyncResourceMap::OnResourceAdded), &resource));
        }

        if (FAILED(hr))
        {
            // Prepare error.
            TCHAR szError[MAX_PATH];
            szError[0] = 0;
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, HRESULT_CODE(hr), 0, szError, ARRAYSIZE(szError), NULL);
            if (hr == E_ACCESSDENIED)
            {
                StringCchCat(szError, ARRAYSIZE(szError), TEXT("\nThe file may be in use. Maybe the game is running?"));
            }

            TCHAR szMessage[MAX_PATH];
            StringCchPrintf(szMessage, ARRAYSIZE(szMessage), TEXT("There was an error writing the resource: 0x%x\n%s"), hr, szError);
            AfxMessageBox(szMessage, MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL, 0);
        }
    }
    return hr;
}

HRESULT CResourceMap::CreateEnumerator(ResourceRecency *_pResourceRecency, DWORD dwTypes, ResourceEnumerator **ppEnum)
{
    return CreateEnumerator(_pResourceRecency, dwTypes, INVALID_HANDLE_VALUE, ppEnum);
}

ResourceEnumerator *CResourceMap::CreateEnumerator(ResourceRecency *pResourceRecency, DWORD dwTypes)
{
    ResourceEnumerator *pEnum = NULL;
    HRESULT hr = CreateEnumerator(pResourceRecency, dwTypes, INVALID_HANDLE_VALUE, &pEnum);
    ASSERT(FAILED(hr) || pEnum);
    return pEnum;
}

HRESULT CResourceMap::CreateEnumerator(ResourceRecency *_pResourceRecency, DWORD dwTypes, HANDLE hFile, ResourceEnumerator **ppEnum)
{
    theApp.LogInfo(TEXT("Enumerating resources of type: %x"), dwTypes);
    DWORD dwType;
    HRESULT hr = GetResourceMapType(hFile, &dwType);
    if (SUCCEEDED(hr))
    {
        switch (dwType)
        {
        case RESOURCEMAPTYPE_SCI0:
            theApp.LogInfo(TEXT("Detected SCI0 resource.map"));
            *ppEnum = new CSCI0ResourceEnumerator();
            break;
        case RESOURCEMAPTYPE_SCI1:
            theApp.LogInfo(TEXT("Detected SCI1 resource.map"));
            AfxMessageBox("SCICompanion can not open SCI1 games.", MB_ICONEXCLAMATION | MB_APPLMODAL | MB_OK);
            *ppEnum = NULL;
            //*ppEnum = new CSCI1ResourceEnumerator();
            break;
        default:
            *ppEnum = NULL;
            break;
        }

        hr = (*ppEnum) ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            (*ppEnum)->Init(_pResourceRecency, _gameFolder, hFile, dwTypes);
        }
    }
    return hr;
}

void CResourceMap::PurgeUnnecessaryResources()
{
    // This is our array of bitfields, to keep track of resources we've found:
    if (SUCCEEDED(RebuildResources(TRUE)))
    {
        // Refresh everything.
        for_each(_syncs.begin(), _syncs.end(), mem_fun(&ISyncResourceMap::OnResourceMapReloaded));
    }
}

void CResourceMap::ReloadResourceType(int iType)
{
    for_each(_syncs.begin(), _syncs.end(), bind2nd(mem_fun(&ISyncResourceMap::OnResourceTypeReloaded), iType));
}

//
// Add a listener for events.
//
void CResourceMap::AddSync(ISyncResourceMap *pSync)
{
    _syncs.push_back(pSync);
}

//
// Before your object gets destroyed, it MUST remove itself
//
void CResourceMap::RemoveSync(ISyncResourceMap *pSync)
{
    // We wouldn't expect to remove a sync that isn't there, so we can just erase
    // whatever we find (which should be a valid iterator)
    _syncs.erase(find(_syncs.begin(), _syncs.end(), pSync));
}

//
//
//
HRESULT CResourceMap::_DeleteResourceI(HANDLE hFileMap, HANDLE hFilePackage, int iPackage, DWORD dwPackageOffset, int iResourceLength)
{
    // Redo the package file.
    HANDLE hFilePackageBak;
    HRESULT hr = OpenResourcePackageBak(_gameFolder, iPackage, &hFilePackageBak);
    if (SUCCEEDED(hr))
    {
        BYTE *pbPackageFirst = NULL;
        BYTE *pbPackageLast = NULL;
        DWORD dwSize = GetFileSize(hFilePackage, NULL);
        DWORD dwSizeLast = dwSize - dwPackageOffset - iResourceLength;
        if (dwSize != INVALID_FILE_SIZE)
        {
            ASSERT(dwSize >= (dwPackageOffset + iResourceLength));
            if (dwSize < (dwPackageOffset + iResourceLength))
            {
                // This means that a resource's offset with a resource package, plus the resource's length,
                // is longer than the resource package itself.  Something is corrupt.
                hr = E_FAIL;
                TCHAR szMessage[1024];
                StringCchPrintf(szMessage, ARRAYSIZE(szMessage),
                    TEXT("Deleting resource\nThe resource map or resource package is corrupt!\nSize of resource.%03d: $%x bytes.  Package offset: $%x   Resource length: $%x bytes"),
                    iPackage, dwSize, dwPackageOffset, iResourceLength);
                AfxMessageBox(szMessage, MB_APPLMODAL | MB_OK | MB_ICONEXCLAMATION);
            }

            if (SUCCEEDED(hr))
            {
                pbPackageFirst = new BYTE[dwPackageOffset];
                if (pbPackageFirst)
                {
                    pbPackageLast = new BYTE[dwSizeLast];
                }
                if ((pbPackageFirst == NULL) || (pbPackageLast == NULL))
                {
                    hr = E_OUTOFMEMORY;
                }
            }
        }
        else
        {
            hr = ResultFromLastError();
        }

        if (SUCCEEDED(hr))
        {
            // Grab the first dwPackageOffset bytes from the file, and write it to the new one.
            DWORD dwBytesRead;
            if (ReadFile(hFilePackage, pbPackageFirst, dwPackageOffset, &dwBytesRead, NULL) && (dwBytesRead == dwPackageOffset))
            {
                DWORD dwBytesWritten;
                if (WriteFile(hFilePackageBak, pbPackageFirst, dwPackageOffset, &dwBytesWritten, NULL) && (dwBytesWritten == dwPackageOffset))
                {
                    // Now skip over the resource we're removing.
                    if (INVALID_SET_FILE_POINTER != SetFilePointer(hFilePackage, iResourceLength, 0, FILE_CURRENT))
                    {
                        // Now write the rest of the file.
                        if (ReadFile(hFilePackage, pbPackageLast, dwSizeLast, &dwBytesRead, NULL) && (dwBytesRead == dwSizeLast))
                        {
                            if (WriteFile(hFilePackageBak, pbPackageLast, dwSizeLast, &dwBytesWritten, NULL) && (dwBytesWritten == dwSizeLast))
                            {
                                hr = S_OK;
                            }
                            else
                            {
                                hr = ResultFromLastError();
                            }
                        }
                        else
                        {
                            hr = ResultFromLastError();
                        }
                    }
                    else
                    {
                        hr = ResultFromLastError();
                    }
                }
                else
                {
                    hr = ResultFromLastError();
                }
            }
            else
            {
                hr = ResultFromLastError();
            }
        }

        if (pbPackageFirst)
        {
            delete pbPackageFirst;
        }

        if (pbPackageLast)
        {
            delete pbPackageLast;
        }

        CloseHandle(hFilePackageBak);
    }

    if (SUCCEEDED(hr))
    {
        // We wrote a new package file.  Now update the resource.map.
        HANDLE hFileMapBak;
        hr = OpenResourceMapBak(_gameFolder, &hFileMapBak);
        if (SUCCEEDED(hr))
        {
            RESOURCEMAPENTRY_SCI0 entry;
            while (S_OK == ReadNextResourceMapEntry(hFileMap, &entry))
            {
                BOOL fDeletedOne = FALSE;
                if (entry.iPackageNumber == iPackage)
                {
                    // We might need to adjust the entry.
                    if (entry.iOffset > dwPackageOffset)
                    {
                        // Yes, it's after the resource we're deleting, so it is now at a new spot.
                        entry.iOffset -= iResourceLength;
                    }
                    else if (entry.iOffset == dwPackageOffset)
                    {
                        // This was the one that was deleted. Skip it.
                        fDeletedOne = TRUE;
                    }
                }

                if (!fDeletedOne)
                {
                    DWORD cbWritten;
                    if (WriteFile(hFileMapBak, &entry, sizeof(entry), &cbWritten, NULL) && (cbWritten == sizeof(entry)))
                    {
                        // We're good.
                    }
                    else
                    {
                        hr = ResultFromLastError();
                        break;
                    }
                }
            }

            // Finish off the file (important!)
            if (SUCCEEDED(hr))
            {
                hr = WriteResourceMapTerminatingBits(hFileMapBak);
            }

            CloseHandle(hFileMapBak);
        }
    }

    return hr;
}

//
// Deletes the resource specified by pData, using the resource number, type and bits to compare.
//
void CResourceMap::DeleteResource(const ResourceBlob *pData)
{
    // TODO: Delete it from game.ini too.
    HANDLE hFileMap = INVALID_HANDLE_VALUE;
    HANDLE hFilePackage = INVALID_HANDLE_VALUE;
    int iPackage = -1;
    DWORD dwOffset = 0;
    int iLengthOfResource = 0;
    int iNumber = pData->GetNumber();
    HRESULT hr = OpenResourceMap(_gameFolder, TRUE, &hFileMap);
    if (SUCCEEDED(hr))
    {
        bool fCheckIfLast = (pData->GetType() == RS_SCRIPT);
        bool fFoundAnother = false;
        RESOURCEMAPENTRY_SCI0 entry;
        BOOL fFound = FALSE;
        while ((!fFound || (fCheckIfLast && !fFoundAnother)) && (S_OK == ReadNextResourceMapEntry(hFileMap, &entry)))
        {
            if ((entry.iNumber == pData->GetNumber()) && (entry.iPackageNumber == pData->GetPackageHint()) &&
                (entry.iType == pData->GetType()))
            {
                if (!fFound)
                {
                    // This is a close match.  Compare bits to see if it is an exact match.
                    ASSERT(hFilePackage == INVALID_HANDLE_VALUE);
                    hr = MoveToPackage(entry.iOffset, entry.iPackageNumber, _gameFolder, &hFilePackage);
                    if (SUCCEEDED(hr))
                    {
                        ResourceBlob dataInFile;
                        hr = dataInFile.CreateFromHandle(TEXT(""), hFilePackage, FALSE, entry.iPackageNumber);
                        if (SUCCEEDED(hr))
                        {
                            const BYTE *pBits1 = pData->GetData();
                            const BYTE *pBits2 = dataInFile.GetData();
                            if (pData->GetLength() == dataInFile.GetLength())
                            {
                                if ((!pBits1 && !pBits2) || (0 == memcmp(pBits1, pBits2, pData->GetLength())))
                                {
                                    // This is it.
                                    iPackage = entry.iPackageNumber;
                                    dwOffset = entry.iOffset;
                                    iLengthOfResource = pData->GetLengthOnDisk();
                                    fFound = TRUE;
                                }
                            }
                        }
                        if (!fFound)
                        {
                            // If we found it, leave the package file open.
                            CloseHandle(hFilePackage);
                            hFilePackage = INVALID_HANDLE_VALUE;
                        }
                    }
                }
                else if (fCheckIfLast)
                {
                    fFoundAnother = true; // Therefore, it is not the last.
                }
            }
        }

        if (SUCCEEDED(hr) && (iPackage != -1))
        {
            ASSERT(hFilePackage != INVALID_HANDLE_VALUE);

            // Put our file ptrs back to the beginning.
            if (INVALID_SET_FILE_POINTER != SetFilePointer(hFileMap, 0, 0, FILE_BEGIN))
            {
                if (INVALID_SET_FILE_POINTER != SetFilePointer(hFilePackage, 0, 0, FILE_BEGIN))
                {
                    hr = _DeleteResourceI(hFileMap, hFilePackage, iPackage, dwOffset, iLengthOfResource);
                }
                else
                {
                    hr = ResultFromLastError();
                }
            }
            else
            {
                hr = ResultFromLastError();
            }
        }
        else
        {
            if (SUCCEEDED(hr))
            {
                AfxMessageBox(TEXT("Resource not found."), MB_ERRORFLAGS);
                hr = E_INVALIDARG;
            }
        }

        if (hFilePackage != INVALID_HANDLE_VALUE)
        {
            CloseHandle(hFilePackage);
        }

        CloseHandle(hFileMap);

        // If all was good, then do the replace.
        if (SUCCEEDED(hr))
        {
            hr = TestMapAndPackageWrite(_gameFolder, iPackage);
            if (SUCCEEDED(hr))
            {
                hr = ReplaceResourceMap(_gameFolder);
                if (SUCCEEDED(hr))
                {
                    hr = ReplaceResourcePackage(_gameFolder, iPackage);
                }
            }
        }

        if (SUCCEEDED(hr))
        {
            // Call our syncs, so they update.
            for_each(_syncs.begin(), _syncs.end(), bind2nd(mem_fun(&ISyncResourceMap::OnResourceDeleted), pData));
        }

        if (fCheckIfLast && !fFoundAnother)
        {
            CRemoveScriptDialog dialog(static_cast<WORD>(iNumber));
            if (IDOK == dialog.DoModal())
            {
                // Remove it from the ini
                std::string iniKey = default_reskey(iNumber);
                std::string scriptTitle = GetIniString("Script", iniKey);
                ScriptId scriptId = GetScriptId(scriptTitle);
                // First, remove from the [Script] section
                WritePrivateProfileString("Script", iniKey.c_str(), NULL, GetGameIniFileName().c_str());
                // Second, remove from the [Language] section
                WritePrivateProfileString("Language", scriptTitle.c_str(), NULL, GetGameIniFileName().c_str());

                if (dialog.AlsoDelete())
                {
                    if (!DeleteFile(scriptId.GetFullPath().c_str()))
                    {
                        char szMessage[MAX_PATH * 2];
                        char szReason[200];
                        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, (DWORD)GetLastError(), 0,  szReason, ARRAYSIZE(szReason), NULL);
                        StringCchPrintf(szMessage, ARRAYSIZE(szMessage), "Couldn't delete script file.\n%s", szReason);
                        AfxMessageBox(szMessage, MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
                    }
                }
            }
        }
    }
}

//
// Returns an empty string (or pszDefault) if there is no key
//
std::string CResourceMap::GetIniString(const std::string &sectionName, const std::string &keyName, PCSTR pszDefault)
{
    std::string strRet;
    char sz[200];
    if (GetPrivateProfileString(sectionName.c_str(), keyName.c_str(), NULL, sz, (DWORD)ARRAYSIZE(sz), GetGameIniFileName().c_str()))
    {
        strRet = sz;
    }
    else
    {
        strRet = pszDefault;
    }
    return strRet;
}

void CResourceMap::SetIniString(const std::string &sectionName, const std::string &keyName, const std::string &value)
{
    WritePrivateProfileString(sectionName.c_str(), keyName.c_str(), value.c_str(), GetGameIniFileName().c_str());
}

//
// Given something like "main", returns "c:\foobar\mygame\src\main.sc"
//
std::string CResourceMap::GetScriptFileName(const std::string &name, LangSyntax lang)
{
    string filename = _gameFolder;
    filename += "\\src\\";
    filename += name;
    if (lang == LangSyntaxUnknown)
    {
        lang = (GetIniString("Language", name, "sc") == "scp") ? LangSyntaxCpp : LangSyntaxSCIStudio;
    }
    // Figure out what language the script is in (default is .sc), and append the default extension
    if (lang == LangSyntaxCpp)
    {
        filename += ".scp";
    }
    else
    {
        filename += ".sc";
    }
    return filename;
}

std::string CResourceMap::GetScriptObjectFileName(const std::string &title, LangSyntax lang)
{
    std::string filename = _gameFolder;
    if (!filename.empty())
    {
        filename += "\\src\\";
        filename += title;
        if (lang == LangSyntaxCpp)
        {
            filename += ".scop";
        }
        else
        {
            filename += ".sco";
        }
    }
    return filename;
}


std::string CResourceMap::GetGameFolder() const
{
    return _gameFolder;
}

//
// Gets the src folder
//
std::string CResourceMap::GetSrcFolder()
{
    std::string srcFolder = _gameFolder;
    if (!srcFolder.empty())
    {
        srcFolder += "\\src";
    }
    return srcFolder;
}

//
// Gets the include folder that has read-only headers
//
std::string CResourceMap::GetIncludeFolder()
{
    std::string includeFolder;
    TCHAR szPath[MAX_PATH];
    if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
    {
        PSTR pszFileName = PathFindFileName(szPath);
        *pszFileName = 0; // null it
        includeFolder = szPath;
        includeFolder += "include";
    }
    return includeFolder;
}

//
// Gets the include folder that has read-only headers
//
std::string CResourceMap::GetTemplateFolder()
{
    std::string templateFolder;
    HRESULT hr = E_FAIL;
    TCHAR szPath[MAX_PATH];
    if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
    {
        PSTR pszFileName = PathFindFileName(szPath);
        *pszFileName = 0; // null it
        hr = StringCchCat(szPath, ARRAYSIZE(szPath), "Template");
        if (SUCCEEDED(hr))
        {
            templateFolder = szPath;
        }
    }
    return templateFolder;
}

//
// Gets the samples folder 
//
std::string CResourceMap::GetSamplesFolder()
{
    std::string templateFolder;
    HRESULT hr = E_FAIL;
    TCHAR szPath[MAX_PATH];
    if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
    {
        PSTR pszFileName = PathFindFileName(szPath);
        *pszFileName = 0; // null it
        hr = StringCchCat(szPath, ARRAYSIZE(szPath), "Samples");
        if (SUCCEEDED(hr))
        {
            templateFolder = szPath;
        }
    }
    return templateFolder;
}

std::string CResourceMap::GetIncludePath(const std::string &includeFileName)
{
    std::string includeFolder = GetIncludeFolder();
    if (!includeFolder.empty())
    {
        includeFolder += "\\";
        includeFolder += includeFileName;
        if (PathFileExists(includeFolder.c_str()))
        {
            return includeFolder;
        }
    }
    includeFolder = GetSrcFolder();
    includeFolder += "\\";
    includeFolder += includeFileName;
    if (PathFileExists(includeFolder.c_str()))
    {
        return includeFolder;
    }
    return "";
}

#ifdef DOCSUPPORT
std::string CResourceMap::GetDocPath(const std::string &fileName)
{
    std::string docPath;
    TCHAR szPath[MAX_PATH];
    if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
    {
        PSTR pszFileName = PathFindFileName(szPath);
        *pszFileName = 0; // null it
        docPath = szPath;
        docPath += "docs\\";
        docPath += fileName;
    }
    return docPath;
}
#endif

std::string CResourceMap::GetGameIniFileName()
{
    string filename = _gameFolder;
    filename += "\\game.ini";
    return filename;
}

HRESULT CResourceMap::GetGameIni(PTSTR pszBuf, size_t cchBuf)
{
    HRESULT hr = E_FAIL;
    if (!_gameFolder.empty())
    {
        hr = StringCchPrintf(pszBuf, cchBuf, TEXT("%s\\%s"), _gameFolder.c_str(), TEXT("game.ini"));
    }
    return hr;
}

int CResourceMap::GetSCI0Version()
{
    return theApp._fSCI01 ? 1: 0;
}

//
// Perf: we're opening and closing the file each time.  We could do this once.
//
std::string CResourceMap::FigureOutName(int iType, int iNumber)
{
    std::string name;
    if (iType < ARRAYSIZE(g_resourceInfo))
    {
        std::string keyName = default_reskey(iNumber);
        name = GetIniString(g_resourceInfo[iType].pszTitleDefault, keyName, keyName.c_str());
    }
    return name;
}

HRESULT CResourceMap::GetScriptNumber(ScriptId script, WORD &wScript)
{
    wScript = 0xffff;
    TCHAR szGameIni[MAX_PATH];
    HRESULT hr = GetGameIni(szGameIni, ARRAYSIZE(szGameIni));
    if (SUCCEEDED(hr))
    {
        char szNameValues[3000];
        hr = E_INVALIDARG;
        DWORD cchNameValues = ARRAYSIZE(szNameValues);
        DWORD nLength =  GetPrivateProfileSection(TEXT("Script"), szNameValues, cchNameValues, szGameIni);
        if (nLength > 0 && ((cchNameValues - 2) != nLength)) // returns (cchNameValues - 2) in case of insufficient buffer 
        {
            TCHAR *psz = szNameValues;
            while(*psz && (FAILED(hr)))
            {
                size_t cch = strlen(psz);
                char *pszEq = StrChr(psz, TEXT('='));
                CString strTitle = script.GetTitle().c_str();
                if (pszEq && (0 == strTitle.CompareNoCase(pszEq + 1)))
                {
                    // We have a match in script name... find the number
                    TCHAR *pszNumber = StrChr(psz, TEXT('n'));
                    if (pszNumber)
                    {
                        wScript = (WORD)StrToInt(pszNumber + 1);
                        ASSERT(script.GetResourceNumber() == InvalidResourceNumber ||
                               script.GetResourceNumber() == wScript);
                        hr = S_OK;
                    }
                }
                // Advance to next string.
                psz += (cch + 1);
            }
        }
    }
    return hr;
}

//
// Return success if there is a vocab000 resource.
// ppData is optional, and will contain the ResourceBlob on success.
// It also caches the pVocab000
// hFile is an optional file handle.
//
HRESULT CResourceMap::_GetVocab000ResourceData(HANDLE hFile, ResourceBlob **ppData)
{
    HRESULT hr = E_FAIL;
    if (ppData)
    {
        *ppData = NULL;
    }
    ResourceEnumerator *pEnum;
    // Get all the vocab resources.
    if (SUCCEEDED(CreateEnumerator(NULL, ResourceTypeFlagVocab, hFile, &pEnum)))
    {
        ResourceBlob *pData;
        BOOL fDone = FALSE;
        while(!fDone && (S_OK == pEnum->Next(&pData)))
        {
            // And look for # 0.
            if (0 == pData->GetNumber())
            {
                if (ppData)
                {
                    *ppData = pData;
                    pData = NULL; // Transfer ownership
                }
                hr = S_OK;
                fDone = TRUE;
            }
            if (pData)
            {
                delete pData;
            }
        }
        delete pEnum;
    }
    return hr;    
}

Vocab000 *CResourceMap::GetVocab000()
{
    if (!_pVocab000)
    {
        ResourceBlob *pData;
        // No vocab000... try to get one.
        if (SUCCEEDED(_GetVocab000ResourceData(INVALID_HANDLE_VALUE, &pData)))
        {
            _pVocab000 = new Vocab000();
            if (_pVocab000)
            {
                if (FAILED(_pVocab000->InitFromResource(pData)))
                {
                    delete _pVocab000;
                    _pVocab000 = NULL;
                }
            }
            delete pData;
        }
    }
    return _pVocab000;
}

HRESULT GetFilePositionHelper(HANDLE hFile, DWORD *pdwPos)
{
    HRESULT hr = S_OK;
    *pdwPos = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
    if (*pdwPos == INVALID_SET_FILE_POINTER)
    {
        // Might have failed...
        hr = ResultFromLastError();
    }
    return hr;
}

HRESULT GetResourceMapType(HANDLE hFile, DWORD *pdwType)
{
    HRESULT hr = S_OK;
    *pdwType = RESOURCEMAPTYPE_SCI0;
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwOldPos;
        hr = GetFilePositionHelper(hFile, &dwOldPos);
        if (SUCCEEDED(hr))
        {
            // Set to beginning
            hr = SetFilePositionHelper(hFile, 0);
        }

        if (SUCCEEDED(hr))
        {
            BYTE byte;
            DWORD dwRead;
            if (ReadFile(hFile, &byte, 1, &dwRead, NULL) && dwRead == sizeof(byte))
            {
                if (byte == 0x80)
                {
                    // SCI1 resource.
                    *pdwType = RESOURCEMAPTYPE_SCI1;
                }
                // Restore file position
                hr = SetFilePositionHelper(hFile, dwOldPos);
            }
            else
            {
                hr = ResultFromLastError();
            }
        }
    }
    return hr;
}

std::string CResourceMap::GetScriptObjectFileName(WORD wScript, LangSyntax lang)
{
    std::string filename;
    std::string scriptTitle = GetIniString("Script", default_reskey(wScript));
    if (!scriptTitle.empty())
    {
        filename = GetScriptObjectFileName(scriptTitle, lang);
    }
    return filename;
}

//
// Returns the script identifier for something "main", or "rm001".
//
ScriptId CResourceMap::GetScriptId(const std::string &name)
{
    return ScriptId(GetScriptFileName(name).c_str());
}

void CResourceMap::SetScriptLanguage(ScriptId script, LangSyntax language)
{
    SetIniString("Language", script.GetTitle(), (language == LangSyntaxCpp) ? "scp" : "sc");
}

void CResourceMap::GetAllScripts(std::vector<ScriptId> &scripts)
{
    TCHAR szIniFile[MAX_PATH];
    if (SUCCEEDED(GetGameIni(szIniFile, ARRAYSIZE(szIniFile))))
    {
        DWORD cchBuf = 5000;
        scoped_array<char> szNameValues(new char[cchBuf]);
        if (szNameValues.get())
        {
            DWORD nLength =  GetPrivateProfileSection(TEXT("Script"), szNameValues.get(), cchBuf, szIniFile);
            if (nLength > 0 && ((cchBuf - 2) != nLength)) // returns (cchBuf - 2) in case of insufficient buffer 
            {
                TCHAR *psz = szNameValues.get();
                while(*psz)
                {
                    // The format is
                    // n000=ScriptName
                    size_t cch = strlen(psz);
                    char *pszNumber = StrChr(psz, TEXT('n'));
                    if (pszNumber)
                    {
                        char *pszEq = StrChr(pszNumber, TEXT('='));
                        if (pszEq)
                        {
                            // Add this script...
                            ScriptId scriptId(theApp.GetResourceMap().GetScriptFileName(pszEq + 1));
                            // Isolate the number.
                            *pszEq = 0;     // n123
                            pszNumber++;    // 123
                            scriptId.SetResourceNumber(StrToInt(pszNumber));
                            scripts.push_back(scriptId);
                        }
                    }
                    // Advance to next string.
                    psz += (cch + 1);
                }
            }
        }
    }
}

void CResourceMap::GetNumberToNameMap(std::map<WORD, std::string> &scos)
{
    TCHAR szIniFile[MAX_PATH];
    if (SUCCEEDED(GetGameIni(szIniFile, ARRAYSIZE(szIniFile))))
    {
        DWORD cchBuf = 5000;
        scoped_array<char> szNameValues(new char[cchBuf]);
        if (szNameValues.get())
        {
            DWORD nLength =  GetPrivateProfileSection(TEXT("Script"), szNameValues.get(), cchBuf, szIniFile);
            if (nLength > 0 && ((cchBuf - 2) != nLength)) // returns (cchBuf - 2) in case of insufficient buffer 
            {
                TCHAR *psz = szNameValues.get();
                while(*psz)
                {
                    // The format is
                    // n000=ScriptName
                    size_t cch = strlen(psz);
                    char *pszNumber = StrChr(psz, TEXT('n'));
                    if (pszNumber)
                    {
                        ++pszNumber; // Advance past the 'n'
                        char *pszEq = StrChr(pszNumber, TEXT('='));
                        if (pszEq)
                        {
                            scos[(WORD)StrToInt(pszNumber)] = pszEq + 1;
                        }
                    }
                    // Advance to next string.
                    psz += (cch + 1);
                }
            }
        }
    }
}

//
// Called when we open a new game.
//
void CResourceMap::SetGameFolder(const string &gameFolder)
{
    _gameFolder = gameFolder;
    ClearVocab000();
    _language = LangSyntaxUnknown;
}

IResourceBase *CResourceMap::CreateResourceFromNumber(DWORD dwType, WORD wNumber)
{
    IResourceBase *pRes = NULL;
    ResourceEnumerator *pEnum;
    if (SUCCEEDED(CreateEnumerator(NULL, ResourceTypeToFlag(dwType), &pEnum)))
    {
        ResourceBlob *pData;
        if (S_OK == pEnum->Next(&pData, wNumber))
        {
            auto_ptr<ResourceBlob> data(pData);
            CreateResourceFromResourceData(pData, &pRes);
        }
        delete pEnum;
    }
    return pRes;
}



#define DECLARE_CREATE_RESOURCE(type, object) \
    case type: \
    { \
        object *pResource = new object(); \
        if (pResource) \
        { \
            if (SUCCEEDED(pResource->InitFromResource(pData))) \
            { \
                *ppResource = static_cast<IResourceBase*>(pResource); \
                hr = S_OK; \
            } \
            else \
            { \
                 delete pResource; \
            } \
        } \
    } \
    break;



//
// Given a ResourceBlob, this creates the SCI resource represented by the data, and hands back
// an IResourceBase pointer.
//
HRESULT CreateResourceFromResourceData(const ResourceBlob *pData, IResourceBase **ppResource)
{
    *ppResource = NULL;
    HRESULT hr = E_FAIL;
    switch (pData->GetType())
    {
    DECLARE_CREATE_RESOURCE(RS_VIEW, ViewResource)
    DECLARE_CREATE_RESOURCE(RS_FONT, CFontResource)
    DECLARE_CREATE_RESOURCE(RS_CURSOR, CCursorResource)
    DECLARE_CREATE_RESOURCE(RS_TEXT, TextResource)
    DECLARE_CREATE_RESOURCE(RS_SOUND, SoundResource)
    DECLARE_CREATE_RESOURCE(RS_VOCAB, Vocab000)
    DECLARE_CREATE_RESOURCE(RS_PIC, PicResource)
    default:
        ASSERT(FALSE);
        break;
    }
    return hr;
}

string CResourceMap::resmap_filename_bak()
{
    return ::resmap_filename_bak(_gameFolder);
}
string CResourceMap::resmap_filename()
{
    return ::resmap_filename(_gameFolder);
}
string CResourceMap::package_filename_bak(int iPackage)
{
    return ::package_filename_bak(_gameFolder, iPackage);
}
string CResourceMap::package_filename(int iPackage)
{
    return ::package_filename(_gameFolder, iPackage);
}
LangSyntax CResourceMap::GetGameLanguage()
{
    if (_language == LangSyntaxUnknown)
    {
        if (GetIniString("Game", "Language", "sc") == "scp")
        {
            _language = LangSyntaxCpp;
        }
        else
        {
            _language = LangSyntaxSCIStudio;
        }
    }
    return _language;
}
void CResourceMap::SetGameLanguage(LangSyntax lang)
{
    SetIniString("Game", "Language", (lang == LangSyntaxCpp) ? "scp" : "sc");
    _language = GetGameLanguage();
}
