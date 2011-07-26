
#include "stdafx.h"
#include "Codec.h"
#include "SCIPicEditor.h"
#include <errno.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


bool IsValidResourceName(PCTSTR pszName)
{
    size_t cChars = lstrlen(pszName);
    for (size_t i = 0; i < cChars; i++)
    {
        char ch = pszName[i];
        if (!isalpha(ch) && !isdigit(ch) && (ch != '_') && (ch != ' ') && (ch != '-'))
        {
            return false;
        }
    }
    return true;
}

void DisplayInvalidResourceNameMessage(PCTSTR pszName)
{
    TCHAR szBuffer[MAX_PATH];
    StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), TEXT("Invalid resource name: %s.\nNames must consist of letters, numbers, spaces, '_', or '-'."), pszName);
    AfxMessageBox(szBuffer, MB_ERRORFLAGS);
}

bool ValidateResourceSize(DWORD cb)
{
    bool fRet = true;
    if (cb > MaxResourceSize)
    {
        TCHAR szBuffer[MAX_PATH];
        StringCchPrintf(szBuffer, ARRAYSIZE(szBuffer), TEXT("Resources can't be bigger than %d bytes.  This resource is %d bytes."), MaxResourceSize, cb);
        AfxMessageBox(szBuffer, MB_ERRORFLAGS);
        fRet = false;
    }
    return fRet;
}


ResourceBlob::ResourceBlob()
{
    _id = (int)theApp.CreateUniqueRuntimeID();
    _iMethod = 0;
    _iNumber = -1;
    _bType = RS_NONE;
    _iPackageHint = 1;
    _fComputedChecksum = false;
}

ResourceBlob::ResourceBlob(const ResourceBlob &src)
{
    _bType = src._bType;
    _iNumber = src._iNumber;
    _iMethod = src._iMethod;
    _iPackageHint = src._iPackageHint;
    _cbDecompressed = src._cbDecompressed;
    _cbCompressed = src._cbCompressed;
    _strName = src._strName;
    _pData = src._pData;
    _pDataCompressed = src._pDataCompressed;
    _id = src._id; // Give it the same id.
    _iChecksum = src._iChecksum;
    _fComputedChecksum = src._fComputedChecksum;
}
ResourceBlob &ResourceBlob::operator=(const ResourceBlob &src)
{
    if (this != &src)
    {
        _bType = src._bType;
        _iNumber = src._iNumber;
        _iMethod = src._iMethod;
        _iPackageHint = src._iPackageHint;
        _cbDecompressed = src._cbDecompressed;
        _cbCompressed = src._cbCompressed;
        _strName = src._strName;
        _pData = src._pData;
        _pDataCompressed = src._pDataCompressed;
        _id = src._id; // Give it the same id.
        _iChecksum = src._iChecksum;
        _fComputedChecksum = src._fComputedChecksum;
    }
    return *this;
}

ResourceBlob::ResourceBlob(PCTSTR pszName, ResourceType iType, const std::vector<BYTE> &data, int iPackageHint, int iNumber)
{
    _pData = data;
    // REVIEW: do some validation
    _bType = iType;
    _iNumber = (WORD)iNumber;
    _iMethod = COMPRESSION_NONE;
    _cbDecompressed = (DWORD)data.size();
    _cbCompressed = _cbDecompressed + 4; // Compressed includes part of the resource header (4 extra bytes)
    _iChecksum = 0;
    _fComputedChecksum = false;

    _iPackageHint = iPackageHint;
    if (!pszName || (*pszName == 0))
    {
        _strName = theApp.GetResourceMap().FigureOutName(iType, iNumber);
    }
    else
    {
        _strName = pszName;
    }
}

HRESULT ResourceBlob::CreateFromBits(PCTSTR pszName, ResourceType iType, sci::istream *pStream, int iPackageHint, int iNumber)
{
    HRESULT hr = E_FAIL;
    if (pStream)
    {
        hr = S_OK;
        _pData.resize(pStream->GetDataSize());
        pStream->read_data(&_pData[0], pStream->GetDataSize());
        ASSERT(pStream->good()); // It told us the size, so it should always succeed.
        // REVIEW: do some validation
        _bType = iType;
        _iNumber = (WORD)iNumber;
        _iMethod = COMPRESSION_NONE;
        _cbDecompressed = pStream->GetDataSize();
        _cbCompressed = _cbDecompressed + 4; // Compressed includes part of the resource header (4 extra bytes)
        _iPackageHint = iPackageHint;

        if (!pszName || (*pszName == 0))
        {
            _strName = theApp.GetResourceMap().FigureOutName(iType, iNumber);
        }
        else
        {
            _strName = pszName;
        }
    }
    return hr; // TODO Do data validation
}

//
// Loads a resource that was saved into its own file by this application, or SCIStudio.
// These resources are not prepended with a header.  Instead, they have a WORD that is 0x80 | type,
// where type is the resource type.
//
HRESULT ResourceBlob::CreateFromFile(PCTSTR pszName, std::string strFileName, int iPackage, int iNumber)
{
    HRESULT hr;
    HANDLE hFile = CreateFile(strFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        hr = CreateFromHandle(pszName, hFile, TRUE, -1);
        if (SUCCEEDED(hr))
        {
            _Init(iPackage, iNumber);
        }
        CloseHandle(hFile);
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}

sci::istream ResourceBlob::GetReadStream() const
{
    return sci::istream(&_pData[0], _cbDecompressed);
}

std::string ResourceBlob::GetEncodingString() const
{
    int iVersion = theApp.GetResourceMap().GetSCI0Version();
    switch (_iMethod)
    {
    case 0:
        return "None";
        break;

    case COMPRESSION_LZW:
    //case COMPRESSION_SCI01_HUFFMAN:
        return (iVersion == 0) ? "LZW" : "Huffman";
        break;

    case COMPRESSION_HUFFMAN:
    //case COMPRESSION_SCI01_COMP3
        return (iVersion == 0) ? "Huffman" : "Comp3";
        break;

    default:
        return "Unknown";
        break;
    }
}

void ResourceBlob::_Init(RESOURCEHEADER_SCI0 *prh)
{
    _bType = (ResourceType)prh->iType;
    _iNumber = (WORD)prh->iNumber;
    _cbCompressed = (DWORD)prh->cbCompressed;
    _cbDecompressed = (DWORD)prh->cbDecompressed;
    _iMethod = (WORD)prh->iMethod;
}

//
// hFile is assumed to be offset to the beginning of a resource header.
//
HRESULT ResourceBlob::CreateFromHandle(PCTSTR pszName, HANDLE hFile, BOOL fNoHeader, int iPackageHint)
{
    _iPackageHint = iPackageHint;
    HRESULT hr = E_FAIL;
    DWORD cbRead;
    DWORD cbRequested;
    BOOL fRead;
    if (fNoHeader)
    {
        // This is the case of a resource existing in a solitary file.
        // Just a WORD consisting of 0x80 | type
        WORD w;
        cbRequested = sizeof(w);
        fRead = ReadFile(hFile, &w, cbRequested, &cbRead, NULL);
        fRead = (cbRead == cbRequested);
        if (fRead)
        {
            if ((w & 0x80) == 0)
            {
                // No good.
                fRead = FALSE;
                SetLastError(ERROR_INVALID_DATA);
            }
            else
            {
                // Set the type.
                _bType = (ResourceType)(w & ~0x0080);

                // Figure out the size from the filesize.
                DWORD dwSize = GetFileSize(hFile, NULL);
                if (dwSize != INVALID_FILE_SIZE)
                {
                    _cbDecompressed = (WORD)dwSize - sizeof(w);
                    // Fake this value up for consistency:
                    _cbCompressed = SCI0_ACTUAL_LENGTH_TO_CBCOMPRESSED(_cbDecompressed);
                    // No compression:
                    _iMethod = 0;
                }
                else
                {
                    fRead = FALSE;
                }
            }
        }
        if (pszName && *pszName)
        {
            _strName = pszName;
        }
    }
    else
    {
        // The normal case: just suck the resource header out of the current file position.
        RESOURCEHEADER_SCI0 rh;
        cbRequested = sizeof(rh);
        fRead = ReadFile(hFile, &rh, cbRequested, &cbRead, NULL);
        fRead = (cbRead == cbRequested);
        if (fRead)
        {
            _Init(&rh);
        }

        if (fRead)
        {
            // Figure out a name based on the number
            if (!pszName || (*pszName == 0))
            {
                _strName = theApp.GetResourceMap().FigureOutName(_bType, _iNumber);
            }
        }
    }

    if (fRead)
    {
        hr = S_OK;
    }
    else
    {
        hr = ResultFromLastError();
    }

    if (SUCCEEDED(hr))
    {
        // Now it is time to read in the bits.
        // We're already at the end of the header, so just start reading.
        hr = _ReadBits(hFile);
    }    
    return hr;
}


//
// Reads bits out of a resource file.
// Assumes that hFile is pointing just after the header
//
HRESULT ResourceBlob::_ReadBits(HANDLE hFile)
{
    HRESULT hr = E_FAIL;
    BOOL fCantDecompress = FALSE;
    DWORD cbCompressedRemaining = _cbCompressed - 4; // Because cbCompressed includes 4 bytes of the header.

    ASSERT(_pData.empty());
    // Allocate enough room for the uncompressed bits.
    _pData.resize(_cbDecompressed);

    switch(_iMethod)
    {
        case COMPRESSION_LZW:
        case COMPRESSION_HUFFMAN:
        //case COMPRESSION_SCI01_HUFFMAN:   // Same as one above
        //case COMPRESSION_SCI01_COMP3:     // Same as two above
        case COMPRESSION_SCI01_WHAT:
        case COMPRESSION_SCI01_WHO:
            {
                int iVersion = theApp.GetResourceMap().GetSCI0Version();
                ASSERT(_pDataCompressed.empty()); // Verify no leaks
                _pDataCompressed.resize(cbCompressedRemaining);
                DWORD cbRead;
                if (ReadFile(hFile, &_pDataCompressed[0], cbCompressedRemaining, &cbRead, NULL) && (cbRead == cbCompressedRemaining))
                {
                    int iResult = SCI_ERROR_UNKNOWN_COMPRESSION;
                    if (iVersion == 0)
                    {
                        // SCI0.
                        if (_iMethod == COMPRESSION_LZW)
                        {
                            iResult = decrypt1(&_pData[0], &_pDataCompressed[0], _cbDecompressed, cbCompressedRemaining);
                        }
                        else if (_iMethod == COMPRESSION_HUFFMAN)
                        {
                            iResult = decrypt2(&_pData[0], &_pDataCompressed[0], _cbDecompressed, cbCompressedRemaining);
                        }
                        else
                        {
                            // Unknown compression for SCI0
                            fCantDecompress = TRUE;
                        }
                    }
                    else
                    {
                        // SCI01
                        if (_iMethod == COMPRESSION_SCI01_HUFFMAN)
                        {
                            iResult = decrypt2(&_pData[0], &_pDataCompressed[0], _cbDecompressed, cbCompressedRemaining);
                        }
                        else if (_iMethod == COMPRESSION_SCI01_COMP3)
                        {
                            iResult = decrypt3(&_pData[0], &_pDataCompressed[0], _cbDecompressed, cbCompressedRemaining);
                        }
                        else if (_iMethod == COMPRESSION_SCI01_WHAT)
                        {
                            // REVIEW: The freesci specs say this is huffman, and method 1 is LZW.  But method 1 is huffman.
                            // Method 3 seems to be LZW.
                            // Is this right?
                            iResult = decrypt1(&_pData[0], &_pDataCompressed[0], _cbDecompressed, cbCompressedRemaining);
                        }
                        else if (_iMethod == COMPRESSION_SCI01_WHO)
                        {
                            // Is this right?
                            iResult = Decrypt_SCI1ENC4(&_pData[0], &_pDataCompressed[0], _cbDecompressed, cbCompressedRemaining);
                        }
                    }
                    hr = (iResult == 0) ? S_OK : E_FAIL;
                }
            }
            break;

        case COMPRESSION_NONE:
            {
                DWORD dwRead;
                if (ReadFile(hFile, &_pData[0], _cbDecompressed, &dwRead, NULL) && (_cbDecompressed == dwRead))
                {
                    // All was good
                    hr = S_OK;
                }
                else
                {
                    hr = ResultFromLastError();
                    if (SUCCEEDED(hr))
                    {
                        hr = HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
                    }
                }
            }
            break;

        default:
            // Some unknown compression.
            fCantDecompress = TRUE;
            break;
    }

    if (FAILED(hr))
    {
        _pData.clear();
    }

    if (fCantDecompress)
    {
        // We'll still make a ResourceBlob, but we just won't be able to get the data from it.
        hr = S_OK;
    }
    return hr;
}

//
// If pszName is null, the resource is called nxxx, where xxx is the resource number.
//
void ResourceBlob::_SetName(PCTSTR pszName)
{
    if (pszName)
    {
        _strName = pszName;
    }
    else
    {
        TCHAR szDefName[MAX_PATH];
        StringCchPrintf(szDefName, ARRAYSIZE(szDefName), TEXT("n%03d"), _iNumber);
        _strName = szDefName;
    }
}


//
// Export to a separate file
//
HRESULT ResourceBlob::SaveToFile(const std::string &strFileName) const
{
    // Open the file
    HRESULT hr;
    HANDLE hFile = CreateFile(strFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        // Save it
        hr = SaveToHandle(hFile, TRUE);
        CloseHandle(hFile);
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}

std::ostream &operator<< (std::ostream &os, WORD w) 
{
    return os;
}

bool ResourceBlob::SaveToStream(std::ostream &out, bool fNoHeader, DWORD *pcbWritten) const
{
    out.exceptions(std::ios_base::badbit);
    bool fWrote = false;
    bool fWriteCompressedIfPossible;
    if ((_cbCompressed > MaxResourceSize) || (_cbDecompressed > MaxResourceSize))
    {
        SetLastError(ERROR_OUTOFMEMORY);  // Not the best, but it will do.
    }
    else
    {
        if (fNoHeader)
        {
            // No resource header - this is in its own file.  It is just an indicator of what type it is.
            WORD w = 0x80 | _bType;
            out.write((char*)&w, sizeof(w));
            // When we're writing a resource to a separate file, we don't have a header, so we can't have any compression methods.
            fWriteCompressedIfPossible = false;
        }
        else
        {
            RESOURCEHEADER_SCI0 rh;
            rh.cbCompressed = (WORD)_cbCompressed;      // cast ok, we checked above
            rh.cbDecompressed = (WORD)_cbDecompressed;  // cast ok, we checked above
            rh.iMethod = _iMethod;
            rh.iNumber = _iNumber;
            rh.iType = _bType;
            out.write((char*)&rh, sizeof(rh));
            fWriteCompressedIfPossible = true;
        }

        if (fWriteCompressedIfPossible && !_pDataCompressed.empty())
        {
            // Write the compressed version (e.g. when rebuilding resources, etc...)
            DWORD cbActual = SCI0_ACTUAL_COMPRESSEDDATA_LENGTH(_cbCompressed);
            out.write((char*)&_pDataCompressed[0], cbActual);
        }
        else if (!_pData.empty())
        {
            // Write the uncompressed version.
            out.write((char*)&_pData[0], _cbDecompressed);
        }
    }
    if (!out)
    {
        int x = errno;
    }
    return out.good();
}

//
// Save into a resource file
//
HRESULT ResourceBlob::SaveToHandle(HANDLE hFile, BOOL fNoHeader, DWORD *pcbWritten) const
{
    HRESULT hr;
    DWORD cbWrittenHeader;
    BOOL fWrote = FALSE;
    bool fWriteCompressedIfPossible;

    if ((_cbCompressed > MaxResourceSize) || (_cbDecompressed > MaxResourceSize))
    {
        SetLastError(ERROR_OUTOFMEMORY);  // Not the best, but it will do.
    }
    else
    {
        if (fNoHeader)
        {
            // No resource header - this is in its own file.  It is just an indicator of what type it is.
            WORD w = 0x80 | _bType;
            ASSERT(sizeof(w) == 2);
            fWrote = WriteFile(hFile, &w, sizeof(w), &cbWrittenHeader, NULL) && (cbWrittenHeader == sizeof(w));
            // When we're writing a resource to a separate file, we don't have a header, so we can't have any compression methods.
            fWriteCompressedIfPossible = FALSE;
        }
        else
        {
            RESOURCEHEADER_SCI0 rh;
            rh.cbCompressed = (WORD)_cbCompressed;      // cast ok, we checked above
            rh.cbDecompressed = (WORD)_cbDecompressed;  // cast ok, we checked above
            rh.iMethod = _iMethod;
            rh.iNumber = _iNumber;
            rh.iType = _bType;
            fWrote = WriteFile(hFile, &rh, sizeof(rh), &cbWrittenHeader, NULL) && (cbWrittenHeader == sizeof(rh));
            fWriteCompressedIfPossible = TRUE;
        }
    }

    DWORD cbWrittenData;
    if (fWrote)
    {
        if (fWriteCompressedIfPossible && !_pDataCompressed.empty())
        {
            // Write the compressed version (e.g. when rebuilding resources, etc...)
            DWORD cbActual = SCI0_ACTUAL_COMPRESSEDDATA_LENGTH(_cbCompressed);
            fWrote = WriteFile(hFile, &_pDataCompressed[0], cbActual, &cbWrittenData, NULL) && (cbWrittenData == cbActual);
        }
        else if (!_pData.empty() || (_cbDecompressed == 0)) // Check for _cb == 0, since we might just have an empty resource, which is ok (not an error)
        {
            // Write the uncompressed version.
            fWrote = WriteFile(hFile, &_pData[0], _cbDecompressed, &cbWrittenData, NULL) && (cbWrittenData == _cbDecompressed);
        }
        else
        {
            fWrote = FALSE;
            SetLastError(ERROR_INVALID_PARAMETER);
        }
    }

    if (fWrote)
    {
        if (pcbWritten)
        {
            *pcbWritten = cbWrittenHeader + cbWrittenData;
        }
        hr = S_OK;
    }
    else
    {
        hr = ResultFromLastError();
    }
    return hr;
}

sci::istream *ResourceBlob::ConvertToByteStream() const
{
    // This is the one we want.
    BYTE *pData = new BYTE[_cbDecompressed];
    memcpy(pData, &_pData[0], _cbDecompressed);
    sci::istream *pStream = new sci::istream(pData, _cbDecompressed, TRUE);
    return pStream;
}

int ResourceBlob::GetId() const
{
    if (!_fComputedChecksum)
    {
        size_t size = _pData.size();
        _iChecksum = (int)size; // Include the size in the calculation.
        for (size_t i = 0; i < size; i++)
        {
            _iChecksum += (BYTE)_pData[i];
        }
        _fComputedChecksum = true;
    }
    return _iChecksum;
}


//
// What follows is code that allows us to stash a resource in a windows .bmp file.
//

//
// data: data to be encoded
// pBits: where it is put
// pBitsEnd: check to avoid overflow -> points to just past the end of the buffer at pBits
//
BYTE *_EncodeByteInHighNibble(BYTE *pBits, BYTE data, const BYTE *pBitsEnd)
{
    BYTE *pBitsReturn = NULL;
    if (pBits < pBitsEnd)
    {
        ASSERT((*pBits & 0xf0) == 0);
        *pBits |= (data & 0xf0);

        pBits++;
        if (pBits < pBitsEnd)
        {
            ASSERT((*pBits & 0xf0) == 0);
            *pBits |= ((data & 0x0f) << 4);
            pBitsReturn = pBits + 1;
        }
    }
    return pBitsReturn;
}


struct BMP_ENCODEDRESOURCE_HEADER
{
    WORD wSize; // Excluding header...
    WORD wType; // 0x80 | type
};

//
// Encodes a resource in bitmap by using the unused high nibble in each byte.
//
bool EncodeResourceInBitmap(const ResourceBlob &blob, const BITMAPINFO &info, BYTE *pBits)
{
    const BYTE *pResourceByte = blob.GetData();
    DWORD cb = blob.GetLength();
    BMP_ENCODEDRESOURCE_HEADER header = { (WORD)cb, 0x80 | blob.GetType() };
    cb += sizeof(header);

    DWORD dwSizeBmp = (info.bmiHeader.biBitCount / 8) * CX_ACTUAL(info.bmiHeader.biWidth) * info.bmiHeader.biHeight;
    if (dwSizeBmp / 2 >= cb)
    {
        BYTE *pBitmapByte = pBits;
        BYTE *pBitmapByteEnd = pBits + dwSizeBmp;

        // Yucky way to do header.
        BYTE *pHeader = (BYTE*)&header;
        pBitmapByte = _EncodeByteInHighNibble(pBitmapByte, (*pHeader), pBitmapByteEnd);
        pHeader++;
        pBitmapByte = _EncodeByteInHighNibble(pBitmapByte, (*pHeader), pBitmapByteEnd);
        pHeader++;
        pBitmapByte = _EncodeByteInHighNibble(pBitmapByte, (*pHeader), pBitmapByteEnd);
        pHeader++;
        pBitmapByte = _EncodeByteInHighNibble(pBitmapByte, (*pHeader), pBitmapByteEnd);

        cb = blob.GetLength();
        while (pBitmapByte && cb)
        {
            pBitmapByte = _EncodeByteInHighNibble(pBitmapByte, (*pResourceByte), pBitmapByteEnd);
            pResourceByte++;
            cb--;
        }
        return true;
    }
    else
    {
        // It won't fit!
        return false;
    }
}



const BYTE *_DecodeByteInHighNibble(const BYTE *pBits, BYTE *pByte, DWORD cbToRead, const BYTE *pBitsEnd)
{
    const BYTE *pBitsReturn = NULL;
    while (cbToRead)
    {
        if (pBits < pBitsEnd)
        {
            *pByte = *pBits & 0xf0;
            pBits++;
            if (pBits < pBitsEnd)
            {
                *pByte |= ((*pBits & 0xf0) >> 4);
                pBits++;
                pBitsReturn = pBits;
            }
        }
        pByte++;
        cbToRead--;
    }
    return pBitsReturn;
}

//
// Try to load a resource from a bitmap
// 
ResourceBlob *Load8BitBmp(const std::string &filename)
{
    ResourceBlob *pBlob = NULL;
    std::ifstream bmpFile(filename.c_str(), std::ios::in | std::ios::binary);
    if (bmpFile.is_open())
    {
        BITMAPFILEHEADER bmfh;
        bmpFile.read((char *)&bmfh, sizeof(bmfh));

        DWORD dwId = bmfh.bfReserved1;
        dwId |= (bmfh.bfReserved2 << 16);

        if (dwId == SCIResourceBitmapMarker)
        {
            // It's one of our secret resources!
            bmpFile.seekg(bmfh.bfOffBits, std::ios_base::beg);

            DWORD cbBitmap = (bmfh.bfSize - (bmfh.bfOffBits - sizeof(bmfh)));

            if (cbBitmap < 0xffffff) // Check to make sure we don't allocate anything crazy
            {
                scoped_array<BYTE> data(new BYTE[cbBitmap]);
                bmpFile.read((char*) data.get(), cbBitmap);

                BMP_ENCODEDRESOURCE_HEADER header;
                const BYTE *pData = data.get();
                const BYTE *pDataEnd = pData + cbBitmap;
                pData = _DecodeByteInHighNibble(pData, (BYTE*)&header, sizeof(header), pDataEnd);

                if (header.wSize <= (cbBitmap - sizeof(header))) // Sanity check
                {
                    ResourceType type = static_cast<ResourceType>(header.wType & ~0x0080);

                    // This is the buffer that contains our extracted data.
                    scoped_array<BYTE> extractedData(new BYTE[header.wSize]);
                    // Read it.
                    pData = _DecodeByteInHighNibble(pData, extractedData.get(), header.wSize, pDataEnd);
                    if (pData)
                    {
                        sci::istream stream(extractedData.get(), header.wSize);
                        std::auto_ptr<ResourceBlob> blob(new ResourceBlob());
                        if (SUCCEEDED(blob.get()->CreateFromBits(NULL, type, &stream, -1, -1)))
                        {
                            pBlob = blob.get();
                            blob.release();
                        }
                    }
                }
            }
        }
    }
    return pBlob;
}



