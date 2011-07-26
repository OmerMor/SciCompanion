
#include "stdafx.h"
#include "TextResource.h"
#include "ResourceMap.h"
#include "SCIPicEditor.h"

#include <istream>
#include <ostream>

using namespace std;

TextResource::TextResource()
{
    _iNumber = -1;
    _iPackageNumber = -1;
}

class scibuf : public std::streambuf
{
public:
    scibuf(const char *pCurrent, std::streamsize cBytes)
    {
        sputn(pCurrent, cBytes);
    }
};

HRESULT TextResource::InitFromResource(const ResourceBlob *prd)
{
    sci::istream byteStream = prd->GetReadStream();
    ASSERT(_texts.empty());
    while (byteStream.good())
    {
        std::string str;
        byteStream >> str;
        _texts.push_back(str);
    }
    _iPackageNumber = prd->GetPackageHint();
    _iNumber = prd->HasNumber() ? prd->GetNumber() : -1;
    return S_OK;
}


HRESULT TextResource::InitFromResource2(const ResourceBlob *prd)
{
    // Note: this function is not unicode aware
    const BYTE *pCurrent = prd->GetData();
    size_t cBytesRemaining = (size_t)prd->GetLength();  
    if (pCurrent)
    {
        ASSERT(_texts.empty());

        // Start reading in the strings.
        // Use StringCchLength 
        size_t length;
        HRESULT hr = StringCchLength((PCTSTR)pCurrent, cBytesRemaining, &length);
        while (SUCCEEDED(hr))
        {
            size_t bufLength = length + 1;
            // Allocate something big enough.
            TCHAR *pszTemp = new TCHAR[bufLength];
            if (pszTemp)
            {
                // This must succeed, since we know we allocated enough.
                hr = StringCchCopy(pszTemp, cBytesRemaining, (PCTSTR)pCurrent);
                _texts.push_back(pszTemp);
                delete pszTemp;
            }
            else
            {
                hr = E_OUTOFMEMORY;
            }
            if (SUCCEEDED(hr))
            {
                // assuming sizeof(TCHAR) = byte:
                cBytesRemaining -= bufLength;
                pCurrent += bufLength;

                hr = StringCchLength((PCTSTR)pCurrent, cBytesRemaining, &length);
            }
        }

        _iPackageNumber = prd->GetPackageHint();
        _iNumber = prd->HasNumber() ? prd->GetNumber() : -1;
    }
    return S_OK;
}

int TextResource::AddString(PCTSTR pszString)
{
    for (size_t i = 0; i < _texts.size(); i++)
    {
        if (_texts[i] == pszString)
        {
            // This string already exists.  Just re-use it.
            return (int)i;
        }
    }
    _texts.push_back(pszString);
    return (int)(_texts.size() - 1); // Index of added string
}

BOOL TextResource::SetStringAt(int iIndex, PCTSTR pszString)
{
    BOOL fRet = lstrcmp(pszString, _texts[iIndex].c_str());
    _texts[iIndex] = pszString;
    return fRet;
}

BOOL TextResource::MoveStringUp(int iIndex)
{
    BOOL fRet = FALSE;
    if ((iIndex > 0) && (iIndex < (int)_texts.size()))
    {
        fRet = TRUE;
        _texts[iIndex - 1].swap(_texts[iIndex]);
    }
    return fRet;
}

BOOL TextResource::MoveStringDown(int iIndex)
{
    BOOL fRet = FALSE;
    if (_texts.size() > 1)
    {
        if (iIndex < (int)(_texts.size() - 1))
        {
            fRet = TRUE;
            _texts[iIndex + 1].swap(_texts[iIndex]);
        }
    }
    return fRet;
}


void TextResource::DeleteString(int iIndex)
{
    _texts.erase(&_texts[iIndex]);
}

BOOL TextResource::Serialize(sci::istream *pSerial) const
{
    // Note: this function is not unicode aware
    BOOL fRet = TRUE;
    for (size_t i = 0; fRet && (i < _texts.size()); i++)
    {
        string str = _texts[i];
        fRet = pSerial->WriteBytes((BYTE*)str.c_str(), (int)str.length() + 1);
    }
    return fRet;
}

void TextResource::Serialize(vector<BYTE> &output) const
{
    push_strings_nt(output, _texts.begin(), _texts.end());
}

// ILookupNames
std::string TextResource::Lookup(WORD wName) const
{
    std::string ret;
    if (wName < _texts.size())
    {
        ret = _texts[wName];
    }
    return ret;
}

BOOL TextResource::Save()
{
    // By default, things go in 1
    if (_iPackageNumber == -1)
    {
        _iPackageNumber = 1;
    }
    ASSERT(_iNumber != -1);
    vector<BYTE> output;
    Serialize(output);
    return SUCCEEDED(theApp.GetResourceMap().AppendResource(ResourceBlob(NULL, RS_TEXT, output, _iPackageNumber, _iNumber)));
}