//
// General stuff 
//
//
#pragma once

//
// Resource types
// The numbers here are important, they map to the
// actual values that SCI uses.
//
enum ResourceType
{
    RS_NONE = -1,
    RS_VIEW = 0,
    RS_PIC = 1,
    RS_SCRIPT = 2,
    RS_TEXT = 3,
    RS_SOUND = 4,
    RS_MEMORY = 5,
    RS_VOCAB = 6,
    RS_FONT = 7,
    RS_CURSOR = 8,
    RS_PATCH = 9,
    // SCI1 and beyond....
    RS_BITMAP = 10,
    RS_PALETTE = 11,
};


enum MDITabType
{
    TAB_NONE           =  0x00000000,
    TAB_GAME           =  0x00000001,
    TAB_VIEW           =  0x00000002,
    TAB_PIC            =  0x00000004,
    TAB_SCRIPT         =  0x00000008,
    TAB_VOCAB          =  0x00000010,
    TAB_FONT           =  0x00000020,
    TAB_CURSOR         =  0x00000040,
    TAB_TEXT           =  0x00000080,
    TAB_SOUND          =  0x00000100,
    TAB_ROOMEXPLORER   =  0x00000200
};

static const int NumResourceTypesSCI0 = 10;
static const int NumResourceTypes = 12;

// Map ID_SHOW_VIEWS to RS_VIEW, for example.  -1 if no match.
ResourceType ResourceCommandToType(UINT nId);

BOOL IsValidResourceType(int iResourceType);
BOOL IsValidResourceNumber(int iResourceNum);

// Bitfield representations for resource types.
static const DWORD ResourceTypeFlagView =   (0x00000001 << RS_VIEW);
static const DWORD ResourceTypeFlagPic =    (0x00000001 << RS_PIC);
static const DWORD ResourceTypeFlagScript = (0x00000001 << RS_SCRIPT);
static const DWORD ResourceTypeFlagText =   (0x00000001 << RS_TEXT);
static const DWORD ResourceTypeFlagSound =  (0x00000001 << RS_SOUND);
static const DWORD ResourceTypeFlagMemory = (0x00000001 << RS_MEMORY);
static const DWORD ResourceTypeFlagVocab =  (0x00000001 << RS_VOCAB);
static const DWORD ResourceTypeFlagFont =   (0x00000001 << RS_FONT);
static const DWORD ResourceTypeFlagCursor = (0x00000001 << RS_CURSOR);
static const DWORD ResourceTypeFlagPatch =  (0x00000001 << RS_PATCH);

const TCHAR *g_rgszTypeToSectionName[];

PCTSTR IsResourceName(PCTSTR pszName, DWORD *pdwType);
DWORD ResourceTypeToFlag(DWORD dwType);
DWORD ResourceFlagToType(DWORD dwType);
PCTSTR ResourceTypeToText(DWORD dwType);
struct sPOINT
{
    __int16 x;
    __int16 y;
};

#define sPIC_WIDTH      320
#define sPIC_HEIGHT     190  

// 320 x 190 pixels, each a byte. (For our drawing buffers)
#define BMPSIZE (sPIC_WIDTH * sPIC_HEIGHT)

struct EGACOLOR
{
    // Color2 comes before color 1, as it occupies the low bits.
    BYTE color2:4;      // 0000 xxxx
    BYTE color1:4;      // xxxx 0000
};

#define EGACOLOR_TO_BYTE(x) ((x).color2 | ((x).color1 << 4))
#define EGACOLOR_EQUAL(a, b) (EGACOLOR_TO_BYTE(a) == EGACOLOR_TO_BYTE(b))

TCHAR g_szGdiplusFilter[];

RGBQUAD g_egaColors[];
COLORREF g_egaColorsCR[];
#define EGA_TO_RGBQUAD(x)  g_egaColors[(x)]
#define EGA_TO_COLORREF(x) g_egaColorsCR[(x)]
RGBQUAD _Combine(RGBQUAD color1, RGBQUAD color2);
RGBQUAD _Darker(RGBQUAD color);
RGBQUAD _Lighter(RGBQUAD color);


EGACOLOR g_egaColorChooserPalette[];

EGACOLOR EGAColorFromByte(BYTE b);

int GetColorDistance(COLORREF color1, COLORREF color2);
EGACOLOR GetClosestEGAColor(int iAlgorithm, int iPalette, COLORREF color);
EGACOLOR GetClosestEGAColorFromSet(int iAlgorithm, COLORREF color, EGACOLOR *rgColors, int cColors);

#define PALETTE_SIZE 40

//#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))

// TODO: use for logging errors
#define REPORTERROR(x) 

// These values are important.  Pattern codes look like this:
// 87654321
// bits 6 an 5 are the flags below. Bits 3, 2 and 1 are the pattern size.
#define PATTERN_FLAG_RECTANGLE 0x10
#define PATTERN_FLAG_USE_PATTERN 0x20

#define COMPRESSION_NONE 0
#define COMPRESSION_LZW 1
#define COMPRESSION_HUFFMAN 2
#define COMPRESSION_SCI01_HUFFMAN 1
#define COMPRESSION_SCI01_COMP3 2
#define COMPRESSION_SCI01_WHAT 3
#define COMPRESSION_SCI01_WHO 4

// For view resources
#define DWORD_TO_CEL(dw) (LOWORD(dw))
#define DWORD_TO_LOOP(dw) (HIWORD(dw))
#define MAKE_INDEX(nLoop, nCel) (DWORD)(MAKELONG((nCel), (nLoop)))

#define RECTWIDTH(rc) ((rc).right - (rc).left)
#define RECTHEIGHT(rc) ((rc).bottom - (rc).top)

//
// BITMAPINFO for standard SCI0 raster resources.  We use 8bit
// even though we may only use 16 (or less) of the 256 colours.
//
struct SCIBitmapInfo : public BITMAPINFO
{
    SCIBitmapInfo(int cx, int cy);
    RGBQUAD _colors[16];
};

// Has an extended palette with duplicate colours, so we can hide information
// within.
struct SCIBitmapInfoEx : public BITMAPINFO
{
    SCIBitmapInfoEx(int cx, int cy);
    RGBQUAD _colors[256];
};

WORD g_iPriorityBands[];
BYTE PriorityFromY(WORD y);

// The actual width of a cel, used in the bitmaps (multiple of 32 bits)
#define CX_ACTUAL(cx) (((cx) + 3) / 4 * 4)

BOOL CreateDCCompatiblePattern(RGBQUAD color1, RGBQUAD color2, CDC *pDC, CBitmap *pbm);
void *_GetBitsPtrFromBITMAPINFO(BITMAPINFO *pbmi);
inline HRESULT ResultFromLastError() { return HRESULT_FROM_WIN32(GetLastError()); }

void DisplayFileError(HRESULT hr, BOOL fOpen, LPCTSTR pszFileName);

//
// Represents a position in a script.
//
class LineCol
{
public:
    LineCol() : _dwPos(0) {}
    LineCol(int line, int column) : _dwPos(((((DWORD)(line)) << 16)+ (DWORD)(column))) {}
    int Line() const { return static_cast<int>((((_dwPos) >> 16) & 0xffff)); }
    int Column() const { return static_cast<int>(((_dwPos) & 0x0000ffff)); }
    bool operator<(const LineCol& _Right) const
    {
        return _dwPos < _Right._dwPos;
    }
    bool operator<=(const LineCol& _Right) const
    {
        return _dwPos <= _Right._dwPos;
    }
private:
    DWORD _dwPos;
};

namespace sci
{
    //
    // Class used to serialize data back into a blob to put in a resource.
    //
    class istream
    {
    public:
        istream(const sci::istream &original)
        {
            _iIndex = original._iIndex;
            _cbSize = original._cbSize;
            _cbSizeValid = original._cbSizeValid;
            _pDataReadOnly = new BYTE[_cbSizeValid];
            memcpy(const_cast<BYTE *>(_pDataReadOnly), original._pDataReadOnly, _cbSizeValid);
            _fDeleteOnDestroy = true;
            _pData = NULL; // Or else we'd have to copy the data...
            _state = original._state;
        }
        istream &operator=(const sci::istream &original)
        {
            _iIndex = original._iIndex;
            _cbSize = original._cbSize;
            _cbSizeValid = original._cbSizeValid;
            if (_fDeleteOnDestroy)
            {
                delete _pDataReadOnly;
            }
            _pDataReadOnly = new BYTE[_cbSizeValid];
            memcpy(const_cast<BYTE *>(_pDataReadOnly), original._pDataReadOnly, _cbSizeValid);
            _fDeleteOnDestroy = true;
            delete _pData;
            _pData = NULL; // Or else we'd have to copy the data...
            _state = original._state;
            return *this;
        }
        istream()
        {
            _pData = new BYTE[500];
            _cbSize = 500;
            _iIndex = 0;
            _cbSizeValid = 0;
            _pDataReadOnly = NULL;
            _fDeleteOnDestroy = FALSE;
            _state = std::ios_base::goodbit;
        }
        ~istream()
        {
            delete [] _pData;
            // Don't delete _pDataReadOnly unless we were told to
            if (_fDeleteOnDestroy)
            {
                delete [] _pDataReadOnly;
            }
        }
        istream(const BYTE *pData, DWORD cbSize, bool fDelete = FALSE)
        {
            _pData = NULL;
            _cbSize = 0;
            _iIndex = 0;
            _cbSizeValid = cbSize;
            _pDataReadOnly = pData;
            _fDeleteOnDestroy = fDelete;
            _state = std::ios_base::goodbit;
        }
        istream(HANDLE hFile);

        bool WriteWord(WORD w);
        bool WriteByte(BYTE b);
        bool WriteBytes(const BYTE *pData, int cCount);

        bool Seek(DWORD iIndex);

        DWORD GetPosition() const { return _iIndex; }
        DWORD GetDataSize() const { return _cbSizeValid; }

        // Use with caution!
        BYTE *GetInternalPointer() { return _pData; }

        // New api
        bool good() { return (_state & (std::ios_base::failbit | std::ios_base::eofbit)) == 0; }
        istream &operator>> (WORD &w);
        istream &operator>> (BYTE &b);
        istream &operator>> (std::string &str);
        void getRLE(std::string &str);
        void read_data(BYTE *pBuffer, DWORD cbBytes)
        {
            _Read(pBuffer, cbBytes);
        }
        template<class _T>
        istream &operator>> (typename _T &t)
        {
            DWORD dwSave = tellg();
            if (!_Read(reinterpret_cast<BYTE*>(&t), sizeof(t)))
            {
                seekg(dwSave);
                memset(&t, 0, sizeof(t));
                _state = std::ios_base::eofbit | std::ios_base::failbit;
            }
            return *this;
        }
        void seekg(DWORD dwIndex) { _iIndex = dwIndex; }
        DWORD tellg() { return _iIndex; }
        void skip(DWORD cBytes);
        bool peek(BYTE &b);

    private:
        bool _ReadWord(WORD *pw) { return _Read((BYTE*)pw, 2); }
        bool _Read(BYTE *pData, DWORD cCount);
        bool _ReadByte(BYTE *pb) { return _Read(pb, 1); }
        bool _Grow(DWORD cbGrowMin);

        BYTE *_pData;    // Our data
        DWORD _iIndex;   // Current index into it.
        DWORD _cbSize;   // The size in bytes of pData
        DWORD _cbSizeValid; // Size that contains valid data.
        const BYTE *_pDataReadOnly;
        bool _fDeleteOnDestroy;
        std::ios_base::iostate _state;
    };
}

//
// Since exceptions may be thrown, use a C++ class to
// handle leaving the critical section.
//
class CGuard
{
public:
    CGuard(CRITICAL_SECTION *pcs);
    ~CGuard();

private:
    CRITICAL_SECTION *_pcs;
};

// Common combination of flags.
#define MB_ERRORFLAGS (MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL)

#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), (static_cast<void*>(*(ppType)),reinterpret_cast<void**>(ppType))
#define IID_PPV_ARGS_CONST(ppType) __uuidof(**(ppType)), (static_cast<const void*>(*(ppType)),reinterpret_cast<const void**>(ppType))

WORD _HexToWord(PCTSTR psz);

enum LangSyntax
{
    LangSyntaxUnknown,
    LangSyntaxSCIStudio,
    LangSyntaxCpp,
};

bool IsSCIKeyword(LangSyntax lang, const std::string &word);
bool IsTopLevelKeyword(LangSyntax lang, const std::string &word);
bool IsCodeLevelKeyword(LangSyntax lang, const std::string &word);
bool IsValueKeyword(const std::string &word);

static const WORD InvalidResourceNumber = 0xffff;

//
// A script description consists of:
// 1) full folder name
// 2) script title
// 2) script file name
//
class ScriptId
{
public:
    ScriptId() { _wScriptNum = InvalidResourceNumber; };

    ScriptId(const std::string &fullPath)
    {
        _Init(fullPath.c_str());
    }
    ScriptId(PCTSTR pszFullFileName)
    {
        _Init(pszFullFileName);
    }
    ScriptId(PCTSTR pszFileName, PCTSTR pszFolder)
    {
        ASSERT(StrChr(pszFileName, '\\') == NULL); // Ensure file and path are not mixed up.
        _strFileName = pszFileName;
        _strFolder = pszFolder;
        _MakeLower();
        _wScriptNum = InvalidResourceNumber;
    }

    ScriptId& operator=( const ScriptId& src )
	{
        _strFolder = src.GetFolder();
        _strFileName = src.GetFileName();
        _MakeLower();
        _wScriptNum = src.GetResourceNumber();
		return( *this );
	}

    BOOL IsNone() const { return _strFileName.empty(); }
    const std::string &GetFileName() const { return _strFileName; }
    const std::string &GetFolder() const { return _strFolder; } 

    // e.g. for Main.sc, it returns Main.  For keys.sh, it returns keys
    std::string GetTitle() const;
    
    // Returns the complete path, for loading/saving, etc...
    std::string GetFullPath() const;

    // Set the path w/o changing the resource number.
    void SetFullPath(const std::string &fullPath)
    {
        _Init(fullPath.c_str(), GetResourceNumber());
    }

    // Script resource number
    WORD GetResourceNumber() const { return _wScriptNum; }
    void SetResourceNumber(WORD wScriptNum);

    // Is this a header, or a script file?
    bool IsHeader() const;
    LangSyntax Language() const;

    friend bool operator<(const ScriptId& script1, const ScriptId& script2);

private:
    void _MakeLower();

    void _Init(PCTSTR pszFullFileName, WORD wScriptNum = InvalidResourceNumber)
    {
        _wScriptNum = wScriptNum;
        if (pszFullFileName)
        {
            CString str = pszFullFileName;
            int iIndexBS = str.ReverseFind('\\');
            _strFolder = str.Left(iIndexBS);
            _strFileName = str.Right(str.GetLength() - iIndexBS - 1);
            _MakeLower();
        }
    }
    std::string _strFolder;
    std::string _strFileName;
    WORD _wScriptNum;
};

//
// equality operator for ScriptId
//
bool operator==(const ScriptId& script1, const ScriptId& script2);

bool operator<(const ScriptId& script1, const ScriptId& script2);


//
// Hash function for ScriptId
//
template<> UINT AFXAPI HashKey<ScriptId&>( ScriptId& key );

#define PACKVERSION(major,minor) MAKELONG(minor,major)
DWORD GetDllVersion(LPCTSTR lpszDllName);

class CPrecisionTimer
{
  LARGE_INTEGER lFreq, lStart;

public:
  CPrecisionTimer()
  {
    QueryPerformanceFrequency(&lFreq);
  }

  inline void Start()
  {
    //SetThreadAffinityMask( GetCurrentThread(), 1 );
    QueryPerformanceCounter(&lStart);
  }
  
  inline double Stop()
  {
    // Return duration in seconds...
    LARGE_INTEGER lEnd;
    QueryPerformanceCounter(&lEnd);
    return (static_cast<double>(lEnd.QuadPart - lStart.QuadPart) / static_cast<double>(lFreq.QuadPart));
  }
};

const std::string MakeFile(const std::stringstream &out, const std::string &filename);
void ShowTextFile(const std::stringstream &out, const std::string &filename);
std::string MakeTextFile(const std::stringstream &out, const std::string &filename);
bool Save8BitBmp(const std::string &filename, const BITMAPINFO &info, BYTE *pBits, DWORD id = 0);

#define RGB_TO_COLORREF(x) RGB(x.rgbRed, x.rgbGreen, x.rgbBlue)

BOOL _GetMenuItem(PCTSTR pszText, CMenu *pMenu, UINT *pnID);

class IClassBrowserEvents
{
public:
    enum BrowseInfoStatus
    {
        Ok,
        Errors,
        InProgress
    };
    virtual void NotifyClassBrowserStatus(BrowseInfoStatus status, int iPercent) = 0;
};

