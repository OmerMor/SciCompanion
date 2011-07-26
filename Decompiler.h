#pragma once

#include "CompiledScript.h"
#include "Decompiler.h"
#include "Compile\scii.h"

// fwd decl
class sci::FunctionBase;
class sci::SyntaxNode;

std::string GetBinaryOperatorForInstruction(BYTE b);
std::string GetUnaryOperatorForInstruction(BYTE b);

class IDecompileLookups : public ICompiledScriptLookups
{
public:
    IDecompileLookups(WORD wScript, ICompiledScriptLookups *pLookups, IObjectFileScriptLookups *pOFLookups, ICompiledScriptSpecificLookups *pScriptThings, ILookupNames *pTextResource, IPrivateSpeciesLookups *pPrivateSpecies) :
      _wScript(wScript), _pLookups(pLookups), _pOFLookups(pOFLookups), _pScriptThings(pScriptThings), _pTextResource(pTextResource), _pPrivateSpecies(pPrivateSpecies)
    {
    }

    // ICompiledScriptLookups
    std::string LookupSelectorName(WORD wIndex);
    std::string LookupKernelName(WORD wIndex);
    std::string LookupClassName(WORD wIndex);
    bool LookupSpeciesPropertyList(WORD wIndex, std::vector<WORD> &props);
    bool LookupSpeciesPropertyListAndValues(WORD wIndex, std::vector<WORD> &props, std::vector<WORD> &values);

    // IObjectFileScriptLookups
    std::string ReverseLookupGlobalVariableName(WORD wIndex);
    std::string ReverseLookupPublicExportName(WORD wScript, WORD wIndex);

    // ILookupPropertyName
    std::string LookupPropertyName(WORD wPropertyIndex);

    std::string LookupScriptThing(WORD wName, ICompiledScriptSpecificLookups::ObjectType &type) const;

    std::string LookupParameterName(WORD wIndex); // 1-based

    WORD GetScriptNumber() const { return _wScript; }
    std::string LookupTextResource(WORD wIndex) const;

    void SetPosition(sci::SyntaxNode *pNode);

    void EndowWithProperties(ILookupPropertyName *pPropertyNames) { _pPropertyNames = pPropertyNames; }
    void EndowWithFunction(sci::FunctionBase *pFunc) { _pFunc = pFunc; }

private:
    WORD _wScript;
    ICompiledScriptLookups *_pLookups;
    IObjectFileScriptLookups *_pOFLookups;
    ICompiledScriptSpecificLookups *_pScriptThings;
    ILookupNames *_pTextResource;
    ILookupPropertyName *_pPropertyNames;
    IPrivateSpeciesLookups *_pPrivateSpecies;
    sci::FunctionBase *_pFunc;
    LineCol _fakePosition;
};

void DecompileRaw(sci::FunctionBase &func, IDecompileLookups &lookups, const BYTE *pBegin, const BYTE *pEnd, WORD wBaseOffset);
std::string _GetProcNameFromScriptOffset(WORD wOffset);
sci::ValueType _ScriptObjectTypeToPropertyValueType(ICompiledScriptSpecificLookups::ObjectType type);
