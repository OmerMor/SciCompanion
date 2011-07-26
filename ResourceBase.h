
#pragma once

class IResourceIdentifier
{
public:
    virtual int GetPackageHint() const = 0;
    virtual int GetNumber() const = 0;
    virtual ResourceType GetType() const = 0;
    virtual int GetId() const = 0;
};

class __declspec(uuid("2629254f-62bf-41fc-ab0f-de7a8bb3067c")) IResourceBase
{
public:
    // Implement these
    virtual BOOL Serialize(sci::istream *pSerial) const = 0;
    virtual void SetResourceNumber(int iNum) = 0;
    virtual void SetPackageNumber(int iNum) = 0;
    virtual int GetResourceNumber() const = 0;
    virtual int GetPackageNumber() const = 0;
    virtual ResourceType GetType() const = 0;
    virtual HRESULT InitNew() = 0;

    // Some methods already implemented
    virtual BOOL QueryCapability(const GUID guid, void **ppv) { return FALSE; }
    virtual BOOL QueryCapability(const GUID guid, const void **ppv) const
    {
        // This makes it so callers with const references to the IResourceBase can
        // obtain const references to different capabilities.
        void *ppvNC;
        BOOL fRet = const_cast<IResourceBase*>(this)->QueryCapability(guid, &ppvNC);
        *ppv = ppvNC;
        return fRet;
    }
    virtual IResourceBase *Clone() const;
    virtual ~IResourceBase() { }
    virtual BOOL PerformChecks() const { return TRUE; }   // Perform some validation before allowing the user to save.
};

#define ResourceQI(obj, Expr) \
     (static_cast<IResourceBase*>(obj))->QueryCapability(IID_PPV_ARGS(Expr))

// fwd decl
class ResourceBlob;
HRESULT CreateResourceFromResourceData(const ResourceBlob *pData, IResourceBase **ppResource);

HBITMAP CreateBitmapFromResource(const IResourceBase *prb, SCIBitmapInfoEx *pbmi, BYTE **ppBitsDest);
void ExportResourceAsBitmap(const IResourceBase *pResource);

struct SCI_RESOURCE_INFO
{
    PCTSTR pszFileNameDefault;      // What we use for filenames (for generating, and detecting)
    PCTSTR pszTitleDefault;         // Name of the resource in the editor, and the header in game.ini
    PCTSTR pszFileDialogFilter;     // File dialog filter used for this resource.
};

extern SCI_RESOURCE_INFO g_resourceInfo[10];
extern DWORD g_celResourceInfo;
