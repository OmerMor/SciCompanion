#pragma once
#include "interfaces.h"
#include "Vocab99x.h"
#include "Compile\PMachine.h"
#include "Compile\SCO.h"

class sci::Script;
class sci::FunctionBase;
class IDecompileLookups;

//
// Information gleaned from the actual script resources.
//
class ICompiledScriptLookups
{
public:
    virtual std::string LookupSelectorName(WORD wIndex) = 0;
    virtual std::string LookupKernelName(WORD wIndex) = 0;
    virtual std::string LookupClassName(WORD wIndex) = 0;
    virtual bool LookupSpeciesPropertyList(WORD wIndex, std::vector<WORD> &props) = 0;
    virtual bool LookupSpeciesPropertyListAndValues(WORD wIndex, std::vector<WORD> &props, std::vector<WORD> &values) = 0;
};

class ICompiledScriptSpecificLookups
{
public:
    enum ObjectType
    {
        ObjectTypeSaid,
        ObjectTypeString,
        ObjectTypeClass,
    };
    virtual std::string LookupObjectName(WORD wOffset, ObjectType &type) const = 0;
};

class IPrivateSpeciesLookups
{
public:
    virtual std::string LookupClassName(WORD wIndex) = 0;
    virtual bool LookupSpeciesPropertyList(WORD wIndex, std::vector<WORD> &props) = 0;
    virtual bool LookupSpeciesPropertyListAndValues(WORD wIndex, std::vector<WORD> &props, std::vector<WORD> &values) = 0;
};


//
// Information gleaned from the .sco object files that the compiler generates for linking.
//
class IObjectFileScriptLookups
{
public:
    virtual std::string ReverseLookupGlobalVariableName(WORD wIndex) = 0;
    virtual std::string ReverseLookupPublicExportName(WORD wScript, WORD wIndex) = 0;
};



class GlobalCompiledScriptLookups : public ICompiledScriptLookups
{
public:
    bool Load();
    std::string LookupSelectorName(WORD wIndex);
    std::string LookupKernelName(WORD wIndex);
    std::string LookupClassName(WORD wIndex);
    bool LookupSpeciesPropertyList(WORD wIndex, std::vector<WORD> &props);
    bool LookupSpeciesPropertyListAndValues(WORD wIndex, std::vector<WORD> &props, std::vector<WORD> &values);

private:
    SelectorTable       _selectors;
    KernelTable         _kernels;
    GlobalClassTable    _classes;
};

class ObjectFileScriptLookups : public IObjectFileScriptLookups
{
public:
    std::string ReverseLookupGlobalVariableName(WORD wIndex);
    std::string ReverseLookupPublicExportName(WORD wScript, WORD wIndex);

private:
    bool _GetSCOFile(WORD wScript, CSCOFile &scoFile);
    bool _LoadSCOFile(WORD wScript);
    std::map<WORD, CSCOFile> _mapScriptToObject;
};

class ILookupPropertyName
{
public:
    virtual std::string LookupPropertyName(ICompiledScriptLookups *pLookup, WORD wPropertyIndex) = 0;
};

struct CodeSection
{
    WORD begin;
    WORD end;

};

class CompiledObjectBase : public ILookupPropertyName
{
public:
    CompiledObjectBase() { _fInstance = FALSE; }
    bool IsInstance() { return _fInstance; }
    bool Create(sci::istream &stream, BOOL fClass, WORD *pwOffset);
    std::string &GetName() { return _strName; }
    void SetName(PCTSTR pszName) { _strName = pszName; }
    WORD GetSuperClass() { return _wSuperClass; }
    WORD GetSpecies() { return _wSpecies; }
    void AdjustName(PCTSTR pszNewName) { ASSERT(!_fInstance); _strName = pszNewName; }
    std::vector<WORD> &GetProperties() { return _propertySelectors; }
    std::vector<WORD> &GetMethods() { return _functionSelectors; }
    std::vector<WORD> &GetPropertyValues() { return _propertyValues; }
    std::vector<WORD> &GetMethodCodePointersTO() { return _functionOffsetsTO; }
    void DebugOut(std::ostream &out, bool fRN = false) const;
    void Disassemble(std::ostream &out,
        ICompiledScriptLookups *pLookups,
        IObjectFileScriptLookups *pOFLookups,
        ICompiledScriptSpecificLookups *pScriptThings,
        const std::vector<BYTE> &scriptResource,
        const std::vector<CodeSection> &codeSections,
        std::set<WORD> &codePointers);

    void DecompileObject(sci::Script &script,
        IDecompileLookups &lookups,
        const std::vector<BYTE> &scriptResource,
        const std::vector<CodeSection> &codeSections,
        std::set<WORD> &codePointers);

    // ILookupNames
    std::string LookupPropertyName(ICompiledScriptLookups *pLookup, WORD wPropertyIndex);

protected:
    WORD _wSpecies;
    WORD _wSuperClass;
    std::string _strName;
    WORD _wInfo;
    // These start from the 4th position (e.g. leave out species, superclass, --info-- and name)
    std::vector<WORD> _propertySelectors;
    std::vector<WORD> _propertyValues;
    std::vector<WORD> _functionSelectors;      // selectors for the methods
    std::vector<WORD> _functionOffsetsTO;
    bool _fInstance;
    WORD _wPosInResource;
    // If anything else is added, make sure to add it to the = operator.
};

//
// This represents all the information in a compiled script resources
//
class CompiledScript : /*public ILookupNames, */ public IPrivateSpeciesLookups, public ICompiledScriptSpecificLookups
{
public:
    ~CompiledScript();
    CompiledScript(WORD wScript) { _wScript = wScript; }
    BOOL Load(int iScriptNumber);
    bool Load(sci::istream &byteStream);
    std::vector<CompiledObjectBase*> &GetObjects() { return _objects; }
    WORD GetScriptNumber() { return _wScript; }
    void DebugOut(std::ostream &out, bool fRN = false) const;

    void Disassemble(std::ostream &out, ICompiledScriptLookups *pLookups, IObjectFileScriptLookups *pOFLookups, ILookupNames *pWords, const std::string *pMessage = NULL);
    sci::Script *Decompile(IDecompileLookups &lookups, ILookupNames *pWords);

    // ICompiledScriptSpecificLookups
    std::string LookupObjectName(WORD wOffset, ObjectType &type) const;

    // IPrivateSpeciesLookups
    std::string LookupClassName(WORD wIndex);
    bool LookupSpeciesPropertyList(WORD wIndex, std::vector<WORD> &props);
    bool LookupSpeciesPropertyListAndValues(WORD wIndex, std::vector<WORD> &props, std::vector<WORD> &values);

private:
    bool _ReadExports(sci::istream &stream);
    bool _ReadStrings(sci::istream &stream, WORD wDataSize);
    bool _ReadSaids(sci::istream &stream, WORD wDataSize);
    std::string _SaidSequenceToString(std::vector<WORD> saidSequence, ILookupNames *pWords);
    void _DisassembleFunction(std::ostream &out, ICompiledScriptLookups *pLookups, IObjectFileScriptLookups *pOFLookups, WORD wCodeOffset, std::set<WORD> &sortedCodePointers);
    void _DecompileFunction(sci::FunctionBase &func, IDecompileLookups &lookups, WORD wCodeOffsetTO, std::set<WORD> &sortedCodePointersTO);
    std::set<WORD> _FindInternalCallsTO();
    bool _IsInCodeSection(WORD wOffset);
    CompiledObjectBase *_FindObjectWithSpecies(WORD wIndex);

    std::vector<CompiledObjectBase*> _objects;
    std::vector<WORD> _objectsOffsetTO;

    std::vector<WORD> _exportsTO;

    std::vector<std::string> _strings;
    std::vector<WORD> _stringsOffset;

    std::vector<std::string> _saidStrings;
    std::vector<std::vector<WORD> > _saids;
    std::vector<WORD> _saidsOffset;

    std::vector<WORD> _localVars;

    std::map<WORD, WORD> _synonyms;

    WORD _wScript;
    BOOL _fPreloadText;

    std::vector<BYTE> _scriptResource;
    std::vector<CodeSection> _codeSections;
};

int GetOperandSize(BYTE bOpcode, OperandType operandType);
WORD CalcOffset(WORD wOperandStart, WORD wRelOffset, bool bByte, BYTE bRawOpcode);
