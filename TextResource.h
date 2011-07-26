
#pragma once
#include "ResourceBase.h"
#include "interfaces.h"

// fwd decl
class ResourceBlob;

class TextResource : public IResourceBase, public ILookupNames
{
public:
    typedef std::vector<std::string>::iterator iterator;
    typedef std::vector<std::string> container_type;
    typedef std::vector<std::string>::const_iterator const_iterator;
    TextResource();

    // this for existing ones.
    HRESULT InitFromResource(const ResourceBlob *prd);
    HRESULT InitFromResource2(const ResourceBlob *prd);

    // Used by the compiler (resource tuples)
    int AddString(PCTSTR pszString);
    // Set string at (used by the editor).  Returns TRUE if it was modified.
    BOOL SetStringAt(int iIndex, PCTSTR pszString);
    BOOL MoveStringUp(int iIndex);
    BOOL MoveStringDown(int iIndex);
    BOOL IsEmpty() { return _texts.empty(); }
    BOOL Save();
    void DeleteString(int iIndex);

    // IResourceBase
    HRESULT InitNew() { return S_OK; }
    BOOL Serialize(sci::istream *pSerial) const;
    void SetResourceNumber(int i) { _iNumber = i; }
    int GetResourceNumber() const { return _iNumber; }
    void SetPackageNumber(int iNum) { _iPackageNumber = iNum; }
    int GetPackageNumber() const { return _iPackageNumber; }
    ResourceType GetType() const { return RS_TEXT; }
    void Serialize(std::vector<BYTE> &output) const;

    const container_type &GetTexts() const { return _texts; }
    container_type &GetTexts() { return _texts; }

    // ILookupNames
    std::string Lookup(WORD wName) const;

private:
    int _iNumber;
    int _iPackageNumber;
    container_type _texts;
};

