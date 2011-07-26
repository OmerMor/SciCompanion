#pragma once

#include "ResourceBase.h"

bool IsValidResourceName(PCTSTR pszName);
void DisplayInvalidResourceNameMessage(PCTSTR pszName);
bool ValidateResourceSize(DWORD cb);
bool IsValidPackageNumber(int iPackageNumber);

static const DWORD MaxResourceSize = 0xffef; // (0xfff0 - 1)
static const DWORD SCIResourceBitmapMarker = (('S' << 24) + ('C' << 16) + ('I' << 8) + 'R');

// The following two structure need to have 1-byte packing, since
// they are used to read from disk.
#include <pshpack1.h>

//
// SCI0
//

// resource.map consists of an array of these structures, terminated by a sequence where all bits are set to on.
struct RESOURCEMAPENTRY_SCI0
{
	WORD iNumber:11;        // (0 to 999)
	WORD iType:5;           // (0 to 9) e.g. PIC == 1
	DWORD iOffset:26;       // Offset within the file resource.iPackageNumber
	DWORD iPackageNumber:6; // Specifies which resource.xxx to use.
};

// header for each entry in resource.xxx
struct RESOURCEHEADER_SCI0
{
	WORD iNumber:11;        // The number of the resource (0 to 999)
	WORD iType:5;           // PIC == 1
	WORD cbCompressed;      // Compressed byte count (includes cbDecompressed and iMethod)
	WORD cbDecompressed;    // Uncompressed byte count (doesn't include cbDecompressed or iMethod)
	WORD iMethod;           // Compression method.
};

// If cbCompressed is 45 bytes, then size on disk (including the header itself) is 45+4 bytes
#define SCI0_COMPRESSED_TO_SIZEONDISK(x) ((x) + (sizeof(WORD) + sizeof(WORD)))

// cbCompressed includes cbDecompressed and iMethod, so subtract two WORDs to figure out the actual data length.
#define SCI0_ACTUAL_COMPRESSEDDATA_LENGTH(x) ((x) - (sizeof(WORD) + sizeof(WORD)))

// x is the length of the actual compressed data, and this returns the value for cbCompressed
#define SCI0_ACTUAL_LENGTH_TO_CBCOMPRESSED(x)  ((x) + (sizeof(WORD) + sizeof(WORD)))


//
// SCI1
//

// resource.map starts with an array of these structures, terminated when bType = 0xff
// There should be one entry for each resource type, and resource types are grouped together.
struct RESOURCEMAPHEADER_SCI1
{
    BYTE bType;             // (0x80 ... 0x91) 0xff = terminator
    WORD wOffset;           // absolute offset in resource.map, to resource header (for terminator, wOffset points to after last header)
};

// resource.map contains sorted arrays of these structures, grouped by resource type.  The length of the arrays
// is determined by the RESOURCEMAPHEADER_SCI1's.
struct RESOURCEMAPENTRY_SCI1
{
    WORD wNumber;           // (0 to 999)
    DWORD iOffset:28;       // Offset within the file resource.iPackageNumber
    DWORD iPackageNumber:4; // Specifies with resource.xxx to use.
};

// header for each entry in resource.xxx
struct RESOURCEHEADER_SCI1
{
    BYTE bType;             // type (0x80 ... 0x91)
    WORD iNumber;           // The number of the resource (0 to 999)
	WORD cbCompressed;      // Compressed byte count (includes cbDecompressed and iMethod)
	WORD cbDecompressed;    // Uncompressed byte count (doesn't include cbDecompressed or iMethod)
	WORD iMethod;           // Compression method. (0 - 3?)
};

#include <poppack.h>


//
// This represents the generic encoded resource.  From this object, we create the
// type-specific resources.
//
class ResourceBlob : public IResourceIdentifier
{
    // REVIEW: workitem.  Make ResourceBloc() private, so no one can accidentally inherit

public:
    ResourceBlob();
    ResourceBlob(const ResourceBlob &src);
    ResourceBlob &operator=(const ResourceBlob &src);
    ResourceBlob(PCTSTR pszName, ResourceType iType, const std::vector<BYTE> &data, int iPackageHint, int iNumberHint);

    HRESULT CreateFromBits(PCTSTR pszName, ResourceType iType, sci::istream *pStream, int iPackageHint, int iNumberHint);
    HRESULT CreateFromHandle(PCTSTR pszName, HANDLE hFile, BOOL hNoHeader, int iPackageHint);
    HRESULT CreateFromFile(PCTSTR pszName, std::string strFileName, int iPackage = 1, int iNumber = -1);

    sci::istream GetReadStream() const;

    bool SaveToStream(std::ostream &out, bool fNoHeader, DWORD *pcbWritten = NULL) const;
    HRESULT SaveToHandle(HANDLE hFile, BOOL fNoHeader, DWORD *pcbWritten = NULL) const;
    int GetLengthOnDisk() const { return SCI0_COMPRESSED_TO_SIZEONDISK(_cbCompressed); }

    // IResourceIdentifier
    int GetPackageHint() const { return _iPackageHint; }
    int GetNumber() const { return _iNumber; }
    ResourceType GetType() const { return _bType; }
    int GetId() const;

    HRESULT SaveToFile(const std::string &strFileName) const;
    int GetEncoding() const { return _iMethod; }
    std::string GetEncodingString() const;
    const BYTE *GetData() const { return &_pData[0]; } // WARNING: this can be NULL!
    int GetLength() const { return _cbDecompressed; }
    std::string GetName() const { return _strName; }
    void SetName(PCTSTR pszName) { _SetName(pszName); }
    void SetNumber(int iNumber) { _iNumber = iNumber; }
    void SetPackage(int iPackage) { _iPackageHint = iPackage; }
    BOOL HasNumber() const { return (_iNumber != (WORD)-1); }  // Does this resource have a number (if you open it from a file, it won't)
    sci::istream *ConvertToByteStream() const;

private:
    void _Init(RESOURCEHEADER_SCI0 *prh);
    void _Init(int iPackageNumber, int iResourceNumber) { _iPackageHint = iPackageNumber; _iNumber = iResourceNumber; }
    HRESULT _ReadBits(HANDLE hFile);
    void _SetName(PCTSTR pszName);

    // Resource header information
    ResourceType _bType;
    WORD _iNumber;           // The number of the resource (0 to 999)
	WORD _iMethod;           // Compression method.

    // These two values are actually persisted as WORDs.  However, we need to allow for creation of these
    // objects while editing, and we allow the user to go over the limit at that time.  We validate at
    // persistence time that they are ok.
	DWORD _cbCompressed;     // Compressed byte count (includes cbDecompressed and iMethod)
	DWORD _cbDecompressed;   // Uncompressed byte count (doesn't include cbDecompressed or iMethod)

    // Uncompressed data.
    std::vector<BYTE> _pData;
    // Compressed data (optional, can be NULL)
    std::vector<BYTE> _pDataCompressed;

    // Unique identifier for this resource.
    int _id;
    // Only the resource recency object is allowed to touch the id.
    friend class ResourceRecency;

    // Hint about what package this should be in.
    int _iPackageHint;

    // The name of the resource (that which is stored in game.ini)
    std::string _strName;

    mutable bool _fComputedChecksum;
    mutable int _iChecksum;
};

//
// Methods for encoding an SCI resource inside a windows .bmp file.
//
bool EncodeResourceInBitmap(const ResourceBlob &blob, const BITMAPINFO &info, BYTE *pBits);
ResourceBlob *Load8BitBmp(const std::string &filename);
