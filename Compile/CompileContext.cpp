
#include "stdafx.h"
#include <stack>
#include <numeric>
#include "SCO.h"
#include "ScriptOM.h"
#include "ScriptOMAll.h"
#include "CompileInterfaces.h"
#include "CompileContext.h"
#include "..\SCIPicEditor.h"
#include "SyntaxParser.h"
#include <boost/foreach.hpp>
#include "Kernels.h"

using namespace sci;
using namespace std;

static const WORD c_TempIndex = 0x1957; // A sentinel marker for an index we don't yet know.

typedef stdext::hash_map<std::string, DefinePtr> defines_map;
typedef multimap<string, code_pos> ref_multimap;
typedef map<string, const ISourceCodePosition*> stringcode_map; // Strings and associated syntax nodes (for line #)

// Ensure w is an even number.  Return true if it rounded up.
bool make_even(WORD &w)
{
    if (w % 2)
    {
        ++w;
        return true;
    }
    return false;
}
void zero_pad(vector<BYTE> &output, bool fMadeEven)
{
    if (fMadeEven)
    {
        output.push_back(0);
    }
}

void push_word(vector<BYTE> &output, WORD w)
{
    // big-endian
    output.push_back(w & 0xff);
    output.push_back(w >> 8);
}

//
// Push a temporary WORD value (0), and return the offset.
//
WORD push_temp(vector<BYTE> &output)
{
    WORD wOffset = (WORD)output.size();
    push_word(output, 0);
    return wOffset;
}

void write_word(vector<BYTE> &output, size_t index, WORD w)
{
    // big-endian
    output[index] = (w & 0xff);
    output[index + 1] = (w >> 8);
}

// Run length encoded string (no null terminator)
void push_string_rle(vector<BYTE> &output, const string &str)
{
    push_word(output, (WORD)str.length());
    copy(str.begin(), str.end(), back_inserter(output));
}

// WORD size, followed by null terminated string
void push_string(vector<BYTE> &output, const string &str)
{
    push_word(output, (WORD)(str.size() + 1)); // + 1 for terminating null
    copy(str.begin(), str.end(), back_inserter(output));
    output.push_back(0); // Terminating null.
}

// Plain old null terminated string...
void push_string_nt(vector<BYTE> &output, const string &str)
{
    copy(str.begin(), str.end(), back_inserter(output));
    output.push_back(0); // Terminating null.
}

WORD read_word(vector<BYTE> &output, vector<BYTE>::iterator index)
{
    WORD w = *index;
    index++;
    w += ((*index) << 8);
    return w;
}

WORD read_word(vector<BYTE> &output, WORD wOffset)
{
    ASSERT((WORD)output.size() > wOffset);
    return output[wOffset] + (((WORD)output[wOffset + 1]) << 8);
}

bool IsPublicProcedure(const ProcedurePtr proc)
{
    return proc->IsPublic();
}
bool IsPublicInstance(const ClassPtr theClass)
{
    return theClass->IsPublic() && theClass->IsInstance();
}

bool IsClass(const ClassPtr theClass)
{
    return !theClass->IsInstance();
}

//
// Compares two classes for the purposes of dependencies between them.
// Returns true if a is less than b (meaning b inherits from a)
//
bool CompareClassDefinition(const ClassPtr &a, const ClassPtr &b) 
{
    if (b->GetSuperClass() == a->GetName())
    {
        return true;
    }
    return false;
}


//
// Special properties
//
struct IndexAndType
{
    WORD index;
    SpeciesIndex type;
};
const key_value_pair<std::string, IndexAndType> c_specialSelectors[] =
{
    { "objectFunctionArea", { 0xFFFC, DataTypeVoid } },
    { "objectInfo", { 0x0004, DataTypeUInt } },
    { "objectLocal", { 0xFFFA, DataTypeVoid } },
    { "objectName", { 0x0006, DataTypeString } },
    { "objectSize", { 0xFFF6, DataTypeInt } },
    { "objectSpecies", { 0x0000, DataTypeInt} },
    { "objectSuperclass", { 0x0002, DataTypeInt } },
    { "objectTotalProperties", { 0xFFFE, DataTypeInt } },
    { "objectType", { 0xFFF4, DataTypeInt} },
};
bool IsSpecialSelector(const string &str, WORD &wOffset, SpeciesIndex &type)
{
    IndexAndType iat;
    bool fRet = FindKeyValue(c_specialSelectors, ARRAYSIZE(c_specialSelectors), str, iat);
    wOffset = iat.index;
    type = iat.type;
    return fRet;
}



typedef pair<code_pos, WORD> call_pair;

typedef struct
{
    vector<BYTE> *pOutput;
    ICompileContext *pContext;
} OUTPUTANDCONTEXT;

typedef std::vector<species_property> property_vector;

struct MatchSelector : public std::binary_function<species_property, WORD, bool>
{
    bool operator()(const species_property &prop, WORD wSelector) const
    {
        return (prop.wSelector == wSelector);
    }
};


string::size_type ComputeStringLength(string::size_type sumSoFar, const stringcode_map::value_type &thePair)
{
    // Return the length of the string, including a null terminator
    return sumSoFar + (thePair.first.size() + 1);
}

class WordToByte
{
public:
    WordToByte(vector<BYTE> &output) : _output(output) {}
    void operator()(WORD w) const
    {
        push_word(_output, w);
    }
private:
    vector<BYTE> &_output;
};

class OutputSimpleVarDeclValues
{
public:
    OutputSimpleVarDeclValues(vector<BYTE> &output) : _output(output) {}
    void operator()(const VariableDeclPtr variable) const
    {
        vector<WORD> values = variable->GetSimpleValues();
        for_each(values.begin(), values.end(), WordToByte(_output));
        // Fill any remaining spots with 0
        WORD wZeroFill = (variable->GetSize() - (WORD)values.size());
        _output.insert(_output.end(), wZeroFill * 2, 0); // WORD = BYTE * 2
    }
private:
    vector<BYTE> &_output;
};

struct InstanceOrClassOutput : public binary_function<ClassPtr, ICompileContext*, void>
{
    void operator()(const ClassPtr theClass, ICompileContext *pContext) const
    {
        const MethodVector &procs = theClass->GetMethods();
        for_each(procs.begin(), procs.end(), GenericOutputByteCode(*pContext));
    }
};

//
// Final fixups for saids, strings and instance references.
//
void _FixupReferencesHelper(vector<BYTE> &output, const ref_multimap &references, const string &name, WORD wPosInResource)
{
    // For all "name"s in references, ask the code_pos where its final position was, and get its post instruction
    // position.  Calc the diff of the wPosInResource with that post instruction position, and, then change the value in output.
    ref_multimap::const_iterator refIt = references.find(name);
    while (refIt != references.end())
    {
        // It's a multimap, so there could be multiple keys of "name"
        if (refIt->first == name)
        {
            const code_pos &instruction =  refIt->second;
            ASSERT((instruction->get_opcode() == acLOFSA) || (instruction->get_opcode() == acLOFSS));
            WORD wFinalPostOp = instruction->get_final_postop_offset();
            WORD wValue = wPosInResource - wFinalPostOp; // This is the number we write to the resource.
            // Where do we write it?  One byte after the start of the instruction (opcode is 1 byte)
            WORD wWhere = instruction->get_final_offset() + 1;
            write_word(output, wWhere, wValue);
        }
        ++refIt;
    }
}

bool CompileTables::Load()
{
    // REVIEW: this could be deleted while we're compiling.
    _pVocab = theApp.GetResourceMap().GetVocab000();
    return _kernels.Load() && _species.Load() && _selectors.Load();
}

void CompileTables::Save()
{
    _species.Save();
    _selectors.Save();
}

CompileResults::CompileResults(ICompileLog &log) : _log(log) {}

class CompileContext : public ICompileContext
{
private:
    SCIClassBrowser &_browser;
    CResourceMap &_resourceMap;
    Script &_script;       // Script being compiled
    Script *_pErrorScript;  // Current script used for error reporting (could be header file)

    typedef map<WORD, CSCOFile> WordSCOMap;
    WordSCOMap _scos;
    map<WORD, std::string> _numberToNameMap;
    vector<CSCOObjectClass> _instances;
    WORD _wScriptNumber;
    ICompileLog &_results;
    bool _fErrors;
    CompileTables &_tables;

    // The headers (and/or script) own the defines.
    PrecompiledHeaders &_headers;
    defines_map _localDefines; // defines in *this* script (not the headers)

    // e.g. Name is "Feature"
    void _LoadSCO(const std::string &name, LangSyntax langPref, bool fErrorIfNotFound = false)
    {
        ASSERT(!name.empty());
        string scoFileName = theApp.GetResourceMap().GetScriptObjectFileName(name, langPref);
        HANDLE hFile = CreateFile(scoFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            // Try to open the "other" kind of object file.
            if (langPref == LangSyntaxCpp)
            {
                langPref = LangSyntaxSCIStudio;
            }
            else
            {
                langPref = LangSyntaxCpp;
            }
            scoFileName = theApp.GetResourceMap().GetScriptObjectFileName(name, langPref);
            hFile = CreateFile(scoFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        }
        if (hFile != INVALID_HANDLE_VALUE)
        {
            sci::istream byteStream(hFile);
            CSCOFile scoFile;
            if (scoFile.Create(_IsSCOPFile(scoFileName), byteStream))
            {
                _scos[scoFile.GetScriptNumber()] = scoFile;
            }
            else
            {
                ReportError(_pErrorScript, "'%s' is corrupt.", scoFileName.c_str());
            }
            CloseHandle(hFile);
        }
        else if (fErrorIfNotFound)
        {
            char szError[200];
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, szError, ARRAYSIZE(szError), NULL);
            ReportError(_pErrorScript, "Unable to open '%s': %s", scoFileName.c_str(), szError);
        }
    }

    // Loads an SCOFile if we don't already have one for this script.
    // Doesn't produce an error if we can't get one.  (Maybe it should?)
    void _LoadSCOIfNone(WORD wScript)
    {
        WordSCOMap::iterator iter = _scos.find(wScript);
        if ((iter == _scos.end()) || (iter->second.IsEmpty()))
        {
            ASSERT((wScript != _wScriptNumber) && (_wScriptNumber != InvalidResourceNumber)); // The "this" script should always be found.
            std::string scriptName = _numberToNameMap[wScript];
            if (scriptName.empty())
            {
                std::stringstream ss;
                ss << "Unable to find name for script #" << wScript;
                ReportResult(CompileResult(ss.str()));
            }
            else
            {
                _LoadSCO(scriptName, LangSyntaxCpp);
            }
        }
    }

public:
    CompileContext(Script &script, PrecompiledHeaders &headers, CompileTables &tables, ICompileLog &results) :
            _browser(theApp.GetResourceMap().GetClassBrowser()),
            _resourceMap(theApp.GetResourceMap()),
            _results(results),
            _tables(tables),
            _headers(headers),
            _script(script)
    {
        _pErrorScript = &_script;
        _modifier = VM_None;
        _fErrors = false;
        _pClassProperties = NULL;
        _wScriptNumber = InvalidResourceNumber;

        //
        // Load all the sco files for the "use" statements.
        //
        const vector<string> &uses = _script.GetUses();
        BOOST_FOREACH(string use, uses)
        {
            LangSyntax lang = script.Language();
            _LoadSCO(use, lang, true);
        }

        // Get a map of script numbers to script names.  We use this when looking up a species index in the
        // global class table, and then looking in the script for its name.  This is for type checking, and
        // is only needed for the Cpp syntax.
        theApp.GetResourceMap().GetNumberToNameMap(_numberToNameMap);

        // We'll always have an accumulator stack context at the top, so just add it now
        PushOutputContext(OC_Accumulator);
    }
    ~CompileContext()
    {
    }

    // ICompileContext
    virtual LangSyntax GetLanguage()
    {
        return _pErrorScript->Language();
    }
    virtual OutputContext GetOutputContext()
    {
        return _oc.top();
    }
    virtual WORD GetScriptNumber()
    {
        ASSERT(_wScriptNumber != 0xffff);
        return _wScriptNumber;
    }
    virtual WORD AddStringResourceTuple(const string &str)
    {
        _resourceStrings.push_back(str);
        return (WORD)(_resourceStrings.size() - 1);
    }
    const vector<string> &GetResourceStrings()
    {
        return _resourceStrings;
    }
    virtual WORD LookupSelectorAndAdd(const string &str)
    {
        WORD w;
        if (!_tables.Selectors().ReverseLookup(str, w))
        {
            // It doesn't exist ... add it.
            // We can just keep on adding to the selectors list (lots of room)
            w = _tables.Selectors().Add(str);
        }
        return w;
    }
    virtual bool LookupSelector(const string &str, WORD &wIndex)
    {
        return _tables.Selectors().ReverseLookup(str, wIndex);
    }
    virtual void DefineNewSelector(const std::string &str, WORD &wIndex)
    {
        wIndex = _tables.Selectors().Add(str);
    }
    virtual bool LookupDefine(const std::string &str, WORD &wValue)
    {
        bool fRet;
        // One special case
        if (str == "scriptNumber")
        {
            fRet = true;
            wValue = _wScriptNumber;
            ASSERT(_wScriptNumber != 0xffff);
        }
        else
        {
            stdext::hash_map<std::string, DefinePtr>::const_iterator nodeIt = _localDefines.find(str);
            fRet = (nodeIt != _localDefines.end());
            if (fRet)
            {
                wValue = nodeIt->second->GetValue();
            }
            if (!fRet)
            {
                // Try the headers.
                fRet = _headers.LookupDefine(str, wValue);
            }
        }
        return fRet;
    }
    virtual void AddDefine(Define *pDefine)
    {
        WORD wDummy;
        bool fDupe = false;
        const string &defineLabel = pDefine->GetLabel();
        if (_localDefines.lower_bound(defineLabel) != _localDefines.end())
        {
            fDupe = true;
        }
        else if (_headers.LookupDefine(defineLabel, wDummy))
        {
            fDupe = true;
        }
        if (fDupe)
        {
            ReportWarning(pDefine, "Duplicate defines: '%s'", defineLabel.c_str());
        }
        _localDefines[defineLabel] = pDefine;
    }
    //
    // wIndex - index of the item.  Valid for all.
    // pwScript - script of the item.  Only valid for TokenExportInstance (wIndex and wScript)
    //
    virtual TOKENTYPEFOO LookupToken(const SyntaxNode *pNode, const string &str, WORD &wIndex, SpeciesIndex &dataType, WORD *pwScript = NULL)
    {
        // Figure out what this thing is.
        // This is the order in which we should proceed:
        // 1) var/temp/local/global
        // 2) class name/instance name
        // 3) class properties
        // e.g. the earlier ones have precedence over the others.
        TOKENTYPEFOO tokenType = TokenUnknown;
        PropertyValue value;
        if (str == "self")
        {
            tokenType = TokenSelf; // No index needed
            if (_className.empty())
            {
                ReportError(pNode, "self can only be used within a class or instance method.");
            }
            dataType = LookupTypeSpeciesIndex(_fInstance ? _superClassName : _className, pNode);
        }
        else if (str == "super")
        {
            tokenType = TokenClass; // We'll return success, and generate any errors right here.
            // This is only valid from within a class.
            if (_className.empty())
            {
                ReportError(pNode, "self can only be used within a class or instance method.");
            }
            else if (_superClassName.empty())
            {
                // Possible to have no super.
                ReportError(pNode, "super can only be used within a class that has a super class.");
            }
            else
            {
                // Look up the class name
                // Don't check the result - if this fails, it means our class is invalid, and 
                // there will already be an error for that.
                LookupSpeciesIndex(_superClassName, dataType);
                wIndex = dataType.Type();
            }
        }
        else
        {
            // Go in reverse... more recent contexts are searched first.
            stack<const IVariableLookupContext *>::container_type::reverse_iterator index = _varContext._Get_container().rbegin();
            while ((tokenType == TokenUnknown) && (index != _varContext._Get_container().rend()))
            {
                tokenType = (*index)->LookupVariableName(*this, str, wIndex, dataType);
                index++;   
            }

            if (tokenType == TokenUnknown)
            {
                // TokenGlobalVariable
                // Keep going - check for global vars (script 0)
                WORD w;
                if (_scos[0].GetVariableIndex(str, wIndex, w)) // May not have a main - that's ok, this will create a dummy empty one.
                {
                    dataType = w;
                    tokenType = TokenGlobalVariable;
                }
            }

            if ((tokenType == TokenUnknown) && _pClassProperties)
            {
                tokenType = _pClassProperties->LookupVariableName(*this, str, wIndex, dataType);
            }

            if (tokenType == TokenUnknown)
            {
                // TokenClass
                if (LookupSpeciesIndex(str, dataType))
                {
                    // The data type of a class is the class itself.
                    wIndex = dataType.Type();
                    tokenType = TokenClass;
                }
            }

            // TokenInstance
            if (tokenType == TokenUnknown)
            {
                const ClassVector &classes = _script.GetClasses();
                BOOST_FOREACH(const ClassDefinition *pClass, classes)
                {
                    if (pClass->IsInstance() && (str == pClass->GetName()))
                    {
                        // Return a temporary index.
                        wIndex = c_TempIndex;
                        // Then, the caller should call TrackInstanceReference(name, code_pos), and we'll
                        // track his code_pos, and fix it up later when we write the code!
                        dataType = LookupTypeSpeciesIndex(pClass->GetSuperClass(), pNode); // Data type of an instance is its super class. REVIEW: we could change this to be the instance itself.
                        tokenType = TokenInstance;
                        break;
                    }
                }
            }

            if (tokenType == TokenUnknown)
            {
                // TokenClassProperty
                // Then, we need to find the "property index" (not selector index) of the current obect.
                // First, we need to look up the selector# of this property.
                // Then we need a class context.  This context should provide us with a way to lookup and see
                // if the class supports that property.  To do so, we need to get the species # of the class.
                // Then, we need to map that species# to a script/index pair.  From that, we can look up the
                // script in our _scos vector, and ask if class of index # supports that selector, and if so,
                // what is its index?  whew!!!
                // TODO: for now, just fake it if we can get a selector
                if (IsSpecialSelector(str, wIndex, dataType))
                {
                    tokenType = TokenClassProperty;
                }
                else
                {
                    // Other "normal" properties will be handled by the ClassDefinition's
                    // implementation of IVariableLookupContext.
                }
            }

            // TokenExportInstance in other scripts (e.g. dyingscript)
            if ((tokenType == TokenUnknown) && (GetLanguage() == LangSyntaxCpp) && pwScript)
            {
                // Then other sco files.
                BOOST_FOREACH(WordSCOMap::value_type &p, _scos)
                {
                    CSCOFile &scoFile = p.second;
                    if (scoFile.GetExportIndex(str, wIndex))
                    {
                        *pwScript = scoFile.GetScriptNumber();
                        // Found it.
                        const CSCOPublicExport &export = scoFile.GetExports()[wIndex];
                        if (export.GetInstanceSpecies(dataType))
                        {
                            tokenType = TokenExportInstance;
                        }
                        break;
                    }
                }
            }
        }
        return tokenType;
    }
    virtual bool LookupSpeciesIndex(const string &str, SpeciesIndex &wSpeciesIndex)
    {
        // First, we'll need to check the scoFiles for this class
        // Then, we'll need to use the global class table to find the "class #"
        BOOST_FOREACH(WordSCOMap::value_type &p, _scos)
        {
            CSCOFile &scoFile = p.second;
            WORD wClassIndexInScript;
            if (scoFile.GetClassIndex(str, wClassIndexInScript))
            {
                if (_tables.Species().GetSpeciesIndex(scoFile.GetScriptNumber(), wClassIndexInScript, wSpeciesIndex))
                {
                    return true;
                }
            }
        }
        return false;
    }
    virtual bool LookupTypeSpeciesIndex(const string &str, SpeciesIndex &wSpeciesIndex)
    {
        // This is just like LookupSpeciesIndex, except that we also map standard types.
        bool fRet = GetBuiltInDataTypeSpecies(str, wSpeciesIndex);
        if (!fRet)
        {
            fRet = LookupSpeciesIndex(str, wSpeciesIndex);
        }
        return fRet;
    }
    virtual SpeciesIndex LookupTypeSpeciesIndex(const string &str, const ISourceCodePosition *pPos)
    {
        // Call this one when it's an error if the type doesn't match something.
        SpeciesIndex wType = DataTypeAny;
        if (!LookupTypeSpeciesIndex(str, wType))
        {
            ReportError(pPos, "Unknown type '%s'", str.c_str());
        }
        return wType;
    }
    virtual std::string SpeciesIndexToDataTypeString(SpeciesIndex wSpeciesIndex)
    {
        std::string dataType = GetBuiltInDataTypeString(wSpeciesIndex);
        if (dataType.empty())
        {
            // Nope, not one of the built-in types.
            WORD wScript, wClassIndexInScript;
            if (_tables.Species().GetSpeciesLocation(wSpeciesIndex, wScript, wClassIndexInScript))
            {
                _LoadSCOIfNone(wScript);
                CSCOFile &scoFile = _scos[wScript];
                // Find the name.
                dataType = scoFile.GetClassName(wClassIndexInScript);
            }
        }
 
        if (dataType.empty())
        {
            dataType = "Unknown-type";
        }
        return dataType;
    }
    virtual SpeciesIndex GetSpeciesSuperClass(SpeciesIndex wSpeciesIndex)
    {
        SpeciesIndex ret = DataTypeNone;
        CSCOObjectClass object;
        if (_GetSCOObject(wSpeciesIndex, object))
        {
            ret = object.GetSuperClass();
        }
        return ret;
    }
    // The properties returned here, include the 4 default ones.  Even if we can't find the
    // species, we'll return the 4 default ones.
    virtual vector<species_property> GetSpeciesProperties(const string &speciesNames)
    {
        vector<species_property> propertiesRet;
        if (!speciesNames.empty())
        {
            // Find the scofile that contains this species.
            BOOST_FOREACH(WordSCOMap::value_type &p, _scos)
            {
                CSCOFile &scoFile = p.second;
                const CSCOObjectClass *pClass;
                if (scoFile.GetClass(speciesNames, &pClass))
                {
                    // We have the class.
                    const vector<CSCOObjectProperty> &properties = pClass->GetProperties();
                    BOOST_FOREACH(const CSCOObjectProperty &theProp, properties)
                    {
                        species_property specProp = { theProp.GetSelector(), theProp.GetValue(), theProp.GetType(), false };
                        propertiesRet.push_back(specProp);
                    }
                    break;
                }
            }
        }
        if (propertiesRet.empty())
        {
            // Either we have inherited from nothing (which is fine), or we couldn't find the species.
            // In any case, return the default four properties.
            const char* c_rgCommonProps[] = { "species", "superClass", "-info-", "name" };
            const WORD c_rgCommonPropsTypes[] = { DataTypeAny, DataTypeAny, DataTypeUInt, DataTypeString };
            // REVIEW: can we better describe species and superClass?  They are sort of Obj's
            for (int i = 0; i < ARRAYSIZE(c_rgCommonProps); i++)
            {
                WORD wSelector = 0;
                _tables.Selectors().ReverseLookup(c_rgCommonProps[i], wSelector);
                species_property commonProp = { wSelector, 0, c_rgCommonPropsTypes[i], false };
                propertiesRet.push_back(commonProp);
            }
        }
        return propertiesRet;
    }
    virtual void SetClassContext(const string &className, const string &superClass, bool fInstance)
    {
        _superClassName = superClass;
        _className = className;
        _fInstance = fInstance;
    }
    virtual string GetClassName()
    {
        return _className;
    }
    virtual bool IsInstance()
    {
        return _fInstance;
    }
    virtual string GetSuperClassName()
    {
        return _superClassName;
    }
    const ClassDefinition *LookupClassDefinition(const std::string &name)
    {
        const ClassVector &classes = _script.GetClasses();
        ClassVector::const_iterator classIt = match_name(classes.begin(), classes.end(), name);
        if (classIt != classes.end())
        {
            return *classIt;
        }
        else
        {
            return NULL;
        }
    }
    // Look up a string and map it to a procedure.  Return the script and index of the procedure, where appropraite
    // Script are looked up in this order:
    // ProcedureKernel:     wIndex
    // ProcedureLocal:      classOwner
    // ProcedureMain:       wIndex
    // ProcedureExternal:   wScript, wIndex
    //
    // pSignatures - optional: accepts the list of function signatures for this call.
    virtual ProcedureType LookupProc(const string &str, WORD &wScript, WORD &wIndex, string &classOwner, std::vector<CSCOFunctionSignature> *pSignatures)
    {
        std::vector<CSCOFunctionSignature> signatures;
        ProcedureType type = ProcedureUnknown;
        // First try kernel.
        if (_tables.Kernels().ReverseLookup(str, wIndex))
        {
            type = ProcedureKernel;
            signatures = GetKernelSCO(*this, str).GetSignatures();
        }
        else
        {
            // Then try local procedures first.
            if (_localProcs.find(str) != _localProcs.end())
            {
                type = ProcedureLocal;
                // Also return the class owner for this procedure.
                classOwner = _localProcClassOwner[str];
                wIndex = c_TempIndex; // A temporary index
                ASSERT(_localProcs2.find(str) != _localProcs2.end());
                signatures = _localProcs2[str].GetSignatures();
            }
            else
            {
                // Then main
                if (_scos[0].GetExportIndex(str, wIndex))
                {
                    // Found a proc in main.
                    type = ProcedureMain;
                    wScript = 0x3419; // Sentinel, to ensure no one uses the wScript value.
                    signatures = _scos[0].GetExportSignatures(wIndex);
                }
                else
                {
                    // Then other sco files.
                    BOOST_FOREACH(WordSCOMap::value_type &p, _scos)
                    {
                        CSCOFile &scoFile = p.second;
                        if (scoFile.GetExportIndex(str, wIndex))
                        {
                            // Found it.
                            wScript = scoFile.GetScriptNumber();
                            type = ProcedureExternal;
                            scoFile.GetExportSignatures(wIndex);
                        }
                    }
                }
            }
        }
        if (pSignatures)
        {
            *pSignatures = signatures;
        }
        return type;
    }
    virtual ProcedureType LookupProc(const std::string &str)
    {
        // Version where caller doesn't care about the information... just wants to know if its a proc
        WORD wScript, wIndex;
        string classOwner;
        return LookupProc(str, wScript, wIndex, classOwner, NULL);
    }
    bool _GetSCOObject(SpeciesIndex wSpecies, CSCOObjectClass &scoObject)
    {
        bool fRet = false;
        if (!IsPODType(wSpecies))
        {
            WORD wScript, wClassIndexInScript;
            if (_tables.Species().GetSpeciesLocation(wSpecies, wScript, wClassIndexInScript))
            {
                _LoadSCOIfNone(wScript);
                CSCOFile &scoFile = _scos[wScript];
                scoObject = scoFile.GetObject(wClassIndexInScript);
                fRet = true;
            }
        }
        return fRet;
    }
    virtual bool LookupSpeciesMethodOrProperty(SpeciesIndex wCallee, WORD wSelector, std::vector<CSCOFunctionSignature> &signatures, SpeciesIndex &propertyType, bool &fMethod)
    {
        bool fRet = false;
        fMethod = false;
        // Find the CSCOObject for wCallee.
        CSCOObjectClass object;
        while (!fRet && _GetSCOObject(wCallee, object))
        {
            BOOST_FOREACH(const CSCOMethod &method, object.GetMethods())
            {
                fRet = (method.GetSelector() == wSelector);
                if (fRet)
                {
                    // Match. Return the signatures.
                    signatures = method.GetSignatures();
                    fMethod = true;
                    break;
                }
            }
            if (!fRet)
            {
                BOOST_FOREACH(const CSCOObjectProperty &property, object.GetProperties())
                {
                    fRet = (property.GetSelector() == wSelector);
                    if (fRet)
                    {
                        propertyType = property.GetType();
                        break;
                    }
                }
            }
            wCallee = object.GetSuperClass(); // Try the super class
        }
        return fRet;
    }
    virtual void PushOutputContext(OutputContext outputContext)
    {
        _oc.push(outputContext);
    }
    virtual void PopOutputContext()
    {
        _oc.pop();
    }
    virtual bool HasMeaning() { return _meaning.top(); }
    virtual void PushMeaning(bool fMeaning) { _meaning.push(fMeaning); }
    virtual void PopMeaning() { _meaning.pop(); }
    virtual bool InConditional() { return _conditional.top(); }
    virtual void PushConditional(bool fConditional) { _conditional.push(fConditional); }
    virtual void PopConditional() { _conditional.pop(); }
    virtual bool SupportTypeChecking()
    {
        return GetLanguage() == LangSyntaxCpp;
    }
    virtual bool LookupWord(const string word, WORD &wWordGroup)
    {
        Vocab000::WordGroup group;
        bool fRet = _tables.Vocab()->LookupWord(word.c_str(), group);
        wWordGroup = (WORD)group;
        return fRet;
    }
    virtual sci::Script *SetErrorContext(sci::Script *pScript)
    {
        sci::Script *pOld = _pErrorScript;
        _pErrorScript = pScript;
        return pOld;
    }
    virtual void ReportResult(const CompileResult &result)
    {
        _results.ReportResult(result);
    }
    virtual void ReportWarning(const ISourceCodePosition *pPos, const char *pszFormat, ...)
    {
        va_list argList;
        va_start(argList, pszFormat);
        _ReportThing(false, pPos, pszFormat, argList);
        va_end(argList);
    }
    virtual void ReportError(const ISourceCodePosition *pPos, const char *pszFormat, ...)
    {
        va_list argList;
        va_start(argList, pszFormat);
        _ReportThing(true, pPos, pszFormat, argList);
        va_end(argList);
        _fErrors = true;
    }
    virtual void ReportTypeError(const ISourceCodePosition *pPos, SpeciesIndex w1, SpeciesIndex w2, const char *pszFormat)
    {
        std::string s1 = SpeciesIndexToDataTypeString(w1);
        std::string s2 = SpeciesIndexToDataTypeString(w2);
        ReportError(pPos, pszFormat, s1.c_str(), s2.c_str());
    }
    void _ReportThing(bool fError, const ISourceCodePosition *pPos, const char *pszFormat, va_list argList)
    {
        char szMessage[300];
        StringCchVPrintf(szMessage, ARRAYSIZE(szMessage), pszFormat, argList);
        ASSERT(pPos);
        char sz[300];
        // Add one to line number, since they are reported from a 0-base (parser), but displayed from a 1-base (script editor)
        int line = pPos->GetLineNumber() + 1;
        StringCchPrintf(sz, ARRAYSIZE(sz), "  [%s]: %s  Line: %d, col: %d", fError ? "Error" : "Warning", szMessage, line, pPos->GetColumnNumber());
        _results.ReportResult(CompileResult(sz, ScriptId(_pErrorScript->GetPath().c_str()), line, pPos->GetColumnNumber(), fError ? CompileResult::CRT_Error : CompileResult::CRT_Warning));
    }
    bool HasErrors() { return _fErrors; }

    // Try to figure out which script, if any, this identifier is exported from.
    // This is just used for error reporting.
    virtual string ScanForIdentifiersScriptName(const std::string &identifier)
    {
        string strRet;
        const VariableDeclVector *globals = _browser.GetMainGlobals();
        if (globals)
        {
            if (matches_name(globals->begin(), globals->end(), identifier))
            {
                strRet = "main";
            }
        }
        if (strRet.empty())
        {
            const Script *pContainerScript = NULL;
            // Try exported procedures.
            const ProcedureVector &procs = _browser.GetPublicProcedures();
            ProcedureVector::const_iterator procIt = match_name(procs.begin(), procs.end(), identifier);
            if (procIt != procs.end())
            {
                pContainerScript = (*procIt)->GetOwnerScript();
            }
            if (pContainerScript == NULL)
            {
                // Try classes.
                const ClassVector &classes = _browser.GetAllClasses();
                ClassVector::const_iterator classIt = match_name(classes.begin(), classes.end(), identifier);
                if (classIt != classes.end())
                {
                    pContainerScript = (*classIt)->GetOwnerScript();
                }
            }
            if (pContainerScript)
            {
                strRet = pContainerScript->GetName();
                // Trim the ".sc" off.
                strRet.erase(strRet.find('.'));
            }
        }
        return strRet;
    }

    virtual scicode &code() { return _code; }

    // Tell someone about where we have 1) exports, 2) saids, and 3) internal strings
    virtual void TrackExport(code_pos where)
    {
        _exports.push_back(where);
    }
    virtual void TrackPublicInstance(WORD wOffset)
    {
        _publicInstances.push_back(wOffset);
    }
    virtual bool PreScanLocalProc(const string &name, const std::string &ownerClass)
    {
        // Inform the context about a local procedure name.
        if (_localProcs.find(name) == _localProcs.end())
        {
            _localProcs[name] = code().get_undetermined();
            _localProcClassOwner[name] = ownerClass;
            return true;
        }
        else
        {
            return false; // Already exists.
        }
    }
    virtual void TrackLocalProc(const string &name, code_pos where)
    {
        // Tell the context the code position of the implemented procedure.
        ASSERT(_localProcs.find(name) != _localProcs.end());
        _localProcs[name] = where;
    }
    virtual void TrackInstanceReference(const string &name)
    {
        _instanceReferences.insert(ref_multimap::value_type(name, code().get_cur_pos()));
    }
    virtual void TrackStringReference(const string &name)
    {
        _stringReferences.insert(ref_multimap::value_type(name, code().get_cur_pos()));
    }
    virtual void TrackSaidReference(const string &name)
    {
        _saidReferences.insert(ref_multimap::value_type(name, code().get_cur_pos()));
    }
    const ref_multimap &GetInstanceReferences() { return _instanceReferences; }
    const ref_multimap &GetStringReferences() { return _stringReferences; }
    const ref_multimap &GetSaidReferences() { return _saidReferences; }
    virtual bool TrackMethod(const string &name, code_pos where)
    {
        if (_localProcs.find(name) == _localProcs.end())
        {
            _localProcs[name] = where;
            return true;
        }
        else
        {
            return false; // Already exists.
        }
    }
    virtual void TrackLocalProcCall(const string &name)
    {
        ASSERT(_localProcs.find(name) != _localProcs.end());
        _localProcCalls.insert(ref_multimap::value_type(name, code().get_cur_pos()));
    }
    virtual code_pos GetLocalProcPos(const string &name)
    {
        ASSERT(_localProcs.find(name) != _localProcs.end());
        return _localProcs[name];
    }
    virtual void TrackRelocation(WORD wOffset)
    {
        // This is used for instance names and saids/strings that are property values.
        // Their addresses are put into the relocation table for the script.
        _relocations.push_back(wOffset);
    }
    void FixupLocalCalls()
    {
        // Tell all the calls where they're calling to.
        multimap<string, code_pos>::const_iterator theCall = _localProcCalls.begin();
        while (theCall != _localProcCalls.end())
        {
            ASSERT(_localProcs.find((*theCall).first) != _localProcs.end());
            code_pos callTarget = _localProcs[(*theCall).first];
            _code.set_call_target((*theCall).second, callTarget);
            theCall++;
        }
    }
    virtual void TrackCallOffsetInstruction(WORD wProcIndex)
    {
        _calls.push_back(call_pair(_code.get_cur_pos(), wProcIndex));
    }
    virtual void PreScanSaid(const string &theSaid, const ISourceCodePosition *pPos)
    {
        _allSaids[theSaid] = pPos;
    }
    virtual WORD GetSaidTempOffset(const string &theSaid)
    {
        stringcode_map::iterator saidIt = _allSaids.find(theSaid);
        ASSERT(saidIt != _allSaids.end());
        size_t index = distance(_allSaids.begin(), saidIt);
        return (WORD)index;
    }
    void SpecifyFinalSaidOffset(const string &theString, WORD wFinalOffset)
    {
        WORD wTempOffset = GetSaidTempOffset(theString);
        _finalSaidOffsets[wTempOffset] = wFinalOffset;
    }
    virtual void PreScanStringAndUnescape(string &theString, const ISourceCodePosition *pPos)
    {
        // Strings are unescaped during parsing.
        _allStrings[theString] = pPos;
    }
    virtual WORD GetStringTempOffset(const string &theString)
    {
        stringcode_map::iterator stringIt = _allStrings.find(theString);
        ASSERT(stringIt != _allStrings.end());
        size_t index = distance(_allStrings.begin(), stringIt);
        return ((WORD)index) + 0x8000; // Strings start at 0x8000 imaginary offset.
    }
    void SpecifyFinalStringOffset(const string &theString, WORD wFinalOffset)
    {
        WORD wTempOffset = GetStringTempOffset(theString);
        _finalStringOffsets[wTempOffset] = wFinalOffset;
    }
    WORD LookupFinalStringOrSaidOffset(WORD wTempOffset)
    {
        if (wTempOffset >= 0x8000)
        {
            ASSERT(!_finalStringOffsets.empty());
            return _finalStringOffsets[wTempOffset];
        }
        else
        {
            ASSERT(!_finalSaidOffsets.empty());
            return _finalSaidOffsets[wTempOffset];
        }
    }
    virtual void SpecifyOffsetIndexResolvedValue(WORD wOffsetIndex, WORD wOffset)
    {
        _offsetIndexToOffset[wOffsetIndex] = wOffset;    
    }
    virtual void PushVariableLookupContext(const IVariableLookupContext *pVarContext)
    {
        _varContext.push(pVarContext);
    }
    virtual void PopVariableLookupContext()
    {
        _varContext.pop();
    }
    virtual void SetClassPropertyLookupContext(const IVariableLookupContext *pVarContext)
    {
        _pClassProperties = pVarContext;
    }
    virtual void SetReturnValueTypes(const std::vector<SpeciesIndex> &types)
    {
        _allowedReturnValues = types;
    }
    virtual std::vector<SpeciesIndex> GetReturnValueTypes()
    {
        return _allowedReturnValues;
    }
    virtual void SetVariableModifier(VariableModifier modifier)
    {
        _modifier = modifier;
    }
    virtual VariableModifier GetVariableModifier()
    {
        return _modifier;
    }
    virtual void PushSendCallType(SpeciesIndex wType)
    {
        _sendCallType.push(wType);
    }
    virtual void PopSendCallType()
    {
        _sendCallType.pop();
    }
    virtual SpeciesIndex GetSendCalleeType()
    {
        return _sendCallType.top();
    }

    // Other public functions
    vector<call_pair> &GetCalls() { return _calls; }
    vector<code_pos> &GetExports() { return _exports; }
    vector<WORD> &GetPublicInstanceOffsets() { return _publicInstances; }
    stringcode_map &GetSaids() { return _allSaids; }
    stringcode_map &GetStrings() { return _allStrings; }
    const map<WORD, WORD> &GetOffsetFixups()
    {
        return _offsetIndexToOffset;
    }
    virtual void SetScriptNumber()
    {
        _wScriptNumber = _script.GetScriptNumber();
        if (KernelScriptNumber != _wScriptNumber) // Special case for the "kernel" script
        {
            if (_wScriptNumber == InvalidResourceNumber)
            {
                _wScriptNumber = 0; // Safe, because we'll report an error if we leave it at zero.
                if (!LookupDefine(_script.GetScriptNumberDefine(), _wScriptNumber))
                {
                    ReportError(&_script, "Couldn't determine script number: '%s' is undefined.", _script.GetScriptNumberDefine().c_str());
                }
            }
            if (_wScriptNumber >= 1000)
            {
                ReportError(&_script, "Script number must be less than 1000: %d", _wScriptNumber);
            }
        }
        _scos[_wScriptNumber].SetScriptNumber(_wScriptNumber);
    }
    WORD EnsureSpeciesTableEntry(WORD wIndexInScript)
    {
        // This won't work unless we have a valid script number
        ASSERT(_wScriptNumber != InvalidResourceNumber);
        return _tables.Species().MaybeAddSpeciesIndex(_wScriptNumber, wIndexInScript);
    }
    void LoadIncludes()
    {
        _headers.Update(*this, _script);
    }
    void AddSCOClass(CSCOObjectClass scoClass, bool fInstance)
    {
        // TODO - if it's a class, add it to some list that gets used when looking classnames up, etc...
        // If it's an instance, add it to another list.
        if (fInstance)
        {
            _instances.push_back(scoClass);
        }
        else
        {
            ASSERT(_wScriptNumber != InvalidResourceNumber);
            ASSERT(_scos[_wScriptNumber].GetScriptNumber() != 0xffff); // Script number not supplied yet.
            _scos[_wScriptNumber].AddObject(scoClass);
        }
    }
    void ReplaceSCOClass(CSCOObjectClass scoClass)
    {
        // This is a bit of a hack.
        ASSERT(_wScriptNumber != InvalidResourceNumber);
        _scos[_wScriptNumber].ReplaceObject(scoClass);
    }
    void AddSCOVariable(CSCOLocalVariable scoVar)
    {
        ASSERT(_wScriptNumber != InvalidResourceNumber);
        _scos[_wScriptNumber].AddVariable(scoVar);
    }
    void AddSCOPublics(CSCOPublicExport scoPublic)
    {
        ASSERT(_wScriptNumber != InvalidResourceNumber);
        _scos[_wScriptNumber].AddPublic(scoPublic);
    }
    void AddFakeSCOPublic(CSCOPublicExport scoNotActuallyPublic)
    {
        _localProcs2[scoNotActuallyPublic.GetName()] = scoNotActuallyPublic;
    }
    std::vector<CSCOObjectClass> &GetInstanceSCOs()
    {
        return _instances;
    }
    CSCOFile &GetScriptSCO()
    {
        ASSERT(_wScriptNumber != InvalidResourceNumber);
        return _scos[_wScriptNumber];
    }
    std::string LookupSelectorName(WORD wIndex) const
    {
        return _tables.Selectors().Lookup(wIndex);
    }
    const vector<WORD> &GetRelocations()
    {
        // This is used for instance names and saids/strings that are property values.
        // Their addresses are put into the relocation table for the script.
        return _relocations;
    }

private:

    // Our code
    scicode _code;

    // The current output context
    stack<OutputContext> _oc;
    stack<bool> _meaning;
    stack<bool> _conditional;

    // Local, script vars
    stack<const IVariableLookupContext *> _varContext;
    // class properties
    const IVariableLookupContext *_pClassProperties;
    // Type of the target object of the current send call
    stack<SpeciesIndex> _sendCallType;

    vector<string> _errors;

    // List of code positions of public procedure exports - order is determined implicitly
    vector<code_pos> _exports;
    // List of offsets of public instance exports.
    vector<WORD> _publicInstances;
    // List of local procedures
    map<string, code_pos> _localProcs;
    typedef map<string, CSCOPublicExport> LocalProcsMap;
    LocalProcsMap _localProcs2;
    map<string, string> _localProcClassOwner;
    ref_multimap _localProcCalls;
    // MultiMap of instance/string/said references in code (string -> code_pos)
    ref_multimap _instanceReferences;
    ref_multimap _stringReferences;
    ref_multimap _saidReferences;

    // These are a set of all the saids/strings in the resource.  We use the index of a particular
    // said/string as a temporary value to write in code (not needed)
    stringcode_map _allSaids;      // Indicies start from 0
    stringcode_map _allStrings;    // Indicies start from 0x8000
    // Mapping of our imaginary indices to actually offsets within the generated resource
    map<WORD, WORD> _finalSaidOffsets;
    map<WORD, WORD> _finalStringOffsets;

    map<WORD, WORD> _offsetIndexToOffset;

    vector<call_pair> _calls;

    // List of resource strings we'll need to write to a text resource
    vector<string> _resourceStrings;

    vector<WORD> _relocations;

    string _className;
    string _superClassName;
    bool _fInstance;

    VariableModifier _modifier; // Increment or decrement modifier

    vector<SpeciesIndex> _allowedReturnValues; // Current allowed return values
};


PrecompiledHeaders::PrecompiledHeaders(CResourceMap &resourceMap) : _resourceMap(resourceMap), _fValid(false) {}

PrecompiledHeaders::~PrecompiledHeaders()
{
    for_each(_allHeaders.begin(), _allHeaders.end(), delete_map_value());
}

std::vector<std::string> g_defaultCPPHeaders;
std::vector<std::string> g_defaultSCIStudioHeaders;
std::vector<std::string> &GetDefaultHeaders(Script &script)
{
    if (script.Language() == LangSyntaxCpp)
    {
        if (g_defaultCPPHeaders.empty())
        {
            g_defaultCPPHeaders.push_back("sci.shp");
            g_defaultCPPHeaders.push_back("keys.shp");
            g_defaultCPPHeaders.push_back("game.shp");
        }
        return g_defaultCPPHeaders;
    }
    else
    {
        // No default headers for SCI Studio syntax, sorry!
        return g_defaultSCIStudioHeaders; // empty
    }
}


void PrecompiledHeaders::Update(ICompileContext &context, Script &script)
{
    // Here, we look at the headers included by this script, and add them to a scanlist
    // We look to see if they've already been parsed.
    //          If so, look in the parsed version, and add all its includes to the scanlist
    //          If not, parse it, and add all its includes to the scanlist
    // Now go through the scanlist again, and repeat, until nothing gets added to the scanlist.
    set<string> headerScanList;
    headerScanList.insert(GetDefaultHeaders(script).begin(), GetDefaultHeaders(script).end());
    headerScanList.insert(script.GetIncludes().begin(), script.GetIncludes().end());
    // Now also include any headers that *those* headers include.  To do so, we'll need to parse
    // the header - ideally we can use the pre-parsed versions.
    bool fDone = false;
    while (!fDone)
    {
        set<string> newHeaders; // The additional ones we'll pick up
        set<string>::iterator curHeaderIt = headerScanList.begin();
        for (; curHeaderIt != headerScanList.end(); ++curHeaderIt)
        {
            // Get the script for this header.
            header_map::iterator oldHeader = _allHeaders.find(*curHeaderIt);
            if (oldHeader == _allHeaders.end())
            {
                // It's a header we have not yet encountered. Parse it.
                ScriptId scriptId(_resourceMap.GetIncludePath(*curHeaderIt));
                CCrystalTextBuffer buffer;
                if (buffer.LoadFromFile(scriptId.GetFullPath().c_str()))
                {
                    CScriptStreamLimiter limiter(&buffer);
                    CCrystalScriptStream stream(&limiter);

                    auto_ptr<Script> pNewHeader(new Script(scriptId));
                    if (g_Parser.Parse(*pNewHeader, stream, &context))
                    {
                        // Look for any includes in here, and add them to our set.
                        newHeaders.insert(pNewHeader->GetIncludes().begin(), pNewHeader->GetIncludes().end());
                        // And now that we've parsed something, add it to the master list
                        _allHeaders[*curHeaderIt] = pNewHeader.get();
                        pNewHeader.release(); // Transfer ownership to _allHeaders
                    }
                    else
                    {
                        std::stringstream ss;
                        ss << "Parsing errors while loading " << scriptId.GetFullPath() << ".";
                        context.ReportResult(CompileResult(ss.str(), CompileResult::CRT_Error));
                    }
                    buffer.FreeAll();
                }
                else
                {
                    std::stringstream ss;
                    ss << "Unable to load " << scriptId.GetFullPath() << ".";
                    context.ReportResult(CompileResult(ss.str(), CompileResult::CRT_Error));
                }
            }
            else
            {
                Script *pOldHeader = oldHeader->second;
                newHeaders.insert(pOldHeader->GetIncludes().begin(), pOldHeader->GetIncludes().end());
            }
        }
        // Add them to our master list for this particular script - we're done when we didn't add any new ones.
        size_t oldSize = headerScanList.size();
        headerScanList.insert(newHeaders.begin(), newHeaders.end());
        fDone = (oldSize == headerScanList.size());
    }

    // Once we're here, we should have:
    // 1) A complete map of all Script objects we need (and possibly others), in _allHeaders
    // 2) A complete set of the headers used in *this* script in headerScanList
    // If headerScanList is the same as the last set of headers (_currentHeaders), then we're done.
    // Otherwise, we have to re-evaluate all our defines.
    // We can use equality to compare the header lists, since they are in a std::set, which contains
    // ordered and unique values.
    if (headerScanList != _curHeaderList)
    {
        _curHeaderList = headerScanList;
        // Clear out our defines
        _defines.clear();
        // And generate them anew
        set<string>::iterator nameIt = _curHeaderList.begin();
        while (nameIt != _curHeaderList.end())
        {
            header_map::iterator headerIt = _allHeaders.find(*nameIt);
            if (headerIt != _allHeaders.end())
            {
                Script *pHeaderScript = headerIt->second;

                // Get the script for this.  We know it exists, so no need to check for failure
                Script *pOldError = context.SetErrorContext(pHeaderScript);    // All errors henceforth are in this header.

                const DefineVector &defines = pHeaderScript->GetDefines();
                DefineVector::const_iterator defineIt = defines.begin();
                for (; defineIt != defines.end(); ++defineIt)
                {
                    const string &defineLabel = (*defineIt)->GetLabel();
                    if (_defines.lower_bound(defineLabel) != _defines.end())
                    {
                        context.ReportWarning((*defineIt), "Duplicate defines: '%s'", defineLabel.c_str());
                    }
                    _defines[defineLabel] = (*defineIt);
                }
                context.SetErrorContext(pOldError);   // Now they're in the main script again.
            }
            ++nameIt;
        }
    }
    _fValid = true;
}

bool PrecompiledHeaders::LookupDefine(const std::string &str, WORD &wValue)
{
    ASSERT(_fValid);
    bool fRet = false;
    defines_map::const_iterator nodeIt = _defines.find(str);
    fRet = (nodeIt != _defines.end());
    if (fRet)
    {
        wValue = nodeIt->second->GetValue();
    }
    return fRet;
}


//
// Streams a said word to the output.
//
void ParseSaidWord(ICompileContext *pContext, string &word, const stringcode_map::value_type &stringCode, vector<BYTE> &output)
{
    size_t nLen = word.length();
    if (nLen > 0)
    {
        // Trim whitespace from the ends.
        string::size_type notwhite = word.find_first_not_of(" \t\n");
        word.erase(0, notwhite);
        notwhite = word.find_last_not_of(" \t\n"); 
        word.erase(notwhite + 1); 
    }
    if (!word.empty())
    {
        WORD wWordGroup = 0;
        if (pContext->LookupWord(word, wWordGroup))
        {
            ASSERT((wWordGroup >> 8) < 0xf0); // Otherwise it's treated as a said token
            // Note, we can't use push_word, because this must be in big-endian notation.
            output.push_back(wWordGroup >> 8);
            output.push_back(wWordGroup & 0xff);
        }
        else
        {
            pContext->ReportError(stringCode.second, "'%s' is not in the vocabulary.", word.c_str());
        }
    }
    word.clear();
}

//
// Streams said tokens described by stringCode, to the output.
//
void ParseSaidString(ICompileContext *pContext, const stringcode_map::value_type &stringCode, vector<BYTE> &output)
{
    static string c_saidTokens = ",&/()[]#<>"; // 0xf0 to 0xf9
    string::const_iterator saidIt = stringCode.first.begin();
    string word; // We build up this word as we scan the string passed in.
    while (saidIt != stringCode.first.end())
    {
        size_t x = c_saidTokens.find(*saidIt);
        if (x < c_saidTokens.size())
        {
            // It was a token - if we've got a word ready, look it up now, and add its vocab index
            ParseSaidWord(pContext, word, stringCode, output);
            // Then add the token
            output.push_back(0xf0 + (BYTE)x);
        }
        else
        {
            // Just a regular letter (or non token)... add it to our word.
            word.push_back(*saidIt);
        }
        saidIt++;
    }
    // In case we had something in "word", parse it now...
    ParseSaidWord(pContext, word, stringCode, output);
    output.push_back(0xff); // terminator
}


//
// Comes up with a resolved list of properties that were specified in the class or instance definition.
// (does not include implicit properties: species, superclass, info, name (except when overridden)
// Does not check the super class - this is just the (properties) declaration made in script.
//
std::vector<species_property> GetOverriddenProperties(ICompileContext &context, const ClassPtr pClass)
{
    std::vector<species_property> propRet;
    const ClassPropertyVector &properties = pClass->GetProperties();
    ClassPropertyVector::const_iterator propIt = properties.begin();
    for (; propIt != properties.end(); propIt++)
    {
        WORD wSelectorIndex = 0;
        WORD wValue = 0;
        SpeciesIndex wType = DataTypeNone;
        // First the selector
        string selectorName = (*propIt)->GetName();
        if (!context.LookupSelector(selectorName, wSelectorIndex))
        {
            if (pClass->IsInstance())
            {
                // This error is not comprehensive - the user might have added a new property
                // on an instance, and there is already a selector for this property.  Hence, the caller
                // of this function needs to compare against the species props too.
                context.ReportError(*propIt, "Unknown property '%s'.  Instances can not define new properties.", selectorName.c_str());
            }
            else
            {
                // We'll add a new selector for this.
                context.DefineNewSelector(selectorName, wSelectorIndex);
            }
        }
        // Then the value - defines should already be resolved.
        bool fTrackRelocation = false;
        const PropertyValue &value = (*propIt)->GetValue();
        switch (value.GetType())
        {
        case ValueTypeNumber:
            wValue = value.GetNumberValue();
            break;
        case ValueTypeString: // For now, strings are ok in property lists
            wValue = context.GetStringTempOffset(value.GetStringValue());
            fTrackRelocation = true;
            break;
        case ValueTypeSaid:
            wValue = context.GetSaidTempOffset(value.GetStringValue());
            fTrackRelocation = true;
            break;
        case ValueTypeSelector:
            if (!context.LookupSelector(value.GetStringValue(), wValue))
            {
                context.ReportError(&value, "Unknown selector '%s'.", value.GetStringValue().c_str());
            }
            break;
        // FEATURE: possibly support pointers here.
        default:
            context.ReportError(&value, "%s is not allowed as a property value.", value.ToString().c_str());
            break;
        }

        // For classes, see if the property has a type, and make sure it makes sense.
        if (!pClass->IsInstance())
        {
            std::string type = (*propIt)->GetDataType();
            if (!type.empty())
            {
                if (!context.LookupTypeSpeciesIndex(type, wType))
                {
                    context.ReportError(*propIt, "Unknown type '%s' for property '%s'.", type.c_str(), selectorName.c_str());
                }
            }
        }

        species_property newProp = { wSelectorIndex, wValue, wType, fTrackRelocation };
        propRet.push_back(newProp);
    }
    return propRet;
}

//
// Given an array of FunctionSignatures, put them in the SCO function signature object.
//
void _CreateSCOFunctionSignature(CompileContext &context, std::vector<CSCOFunctionSignature> &scoSigs, const FunctionSignatureVector &signatures)
{
    BOOST_FOREACH(FunctionSignature *pSig, signatures)
    {
        // Return type
        CSCOFunctionSignature scoSig;
        SpeciesIndex wReturnType = 0;
        if (!context.LookupTypeSpeciesIndex(pSig->GetDataType(), wReturnType))
        {
            context.ReportError(pSig, "Unknown return type: %s.", pSig->GetDataType().c_str());
        }
        scoSig.SetReturnType(wReturnType.Type());
        // Parameters
        BOOST_FOREACH(FunctionParameter *pParam, pSig->GetParams())
        {
            SpeciesIndex wParamType;
            if (!context.LookupTypeSpeciesIndex(pParam->GetDataType(), wParamType))
            {
                context.ReportError(pSig, "Unknown type for parameter %s: %s.", pParam->GetName().c_str(), pParam->GetDataType().c_str());
            }
            scoSig.AddParameterType(wParamType.Type());
        }
        scoSig.SetAdditionalParametersOk(pSig->GetMoreParametersAllowed()); // Additional params
        scoSig.SetRequiredParameterCount(static_cast<WORD>(pSig->GetRequiredParameterCount()));
        scoSigs.push_back(scoSig);
    }
}



//
// Adds the script's public instances and procedures to the sco file being compiled by the context
//
void GenerateSCOPublics(CompileContext &context, const Script &script)
{
    WORD wIndex = 0;
    const ClassVector &classes = script.GetClasses();
    BOOST_FOREACH(const ClassDefinition *pClass, classes)
    {
        if (pClass->IsPublic() && pClass->IsInstance())
        {
            CSCOPublicExport pe(pClass->GetName(), wIndex);
            SpeciesIndex si = DataTypeAny;;
            if (!context.LookupTypeSpeciesIndex(pClass->GetSuperClass(), si))
            {
                context.ReportError(pClass, "Unknown superclass: %s.", pClass->GetSuperClass().c_str());
            }
            pe.SetInstanceSpecies(si);
            context.AddSCOPublics(pe);
            ++wIndex;
        }
    }
    const ProcedureVector &procs = script.GetProcedures();
    BOOST_FOREACH(const ProcedureDefinition *pProc, procs)
    {
        CSCOPublicExport procExport(pProc->GetName(), wIndex);
        // Store the function signature(s).
        std::vector<CSCOFunctionSignature> signatures;
        _CreateSCOFunctionSignature(context, signatures, pProc->GetSignatures());
        procExport.SetSignatures(signatures);
        if (pProc->IsPublic())
        {
            context.AddSCOPublics(procExport);
            ++wIndex;
        }
        // Also add a fake one, even if it's not public - we'll use this for function signature matching.
        context.AddFakeSCOPublic(procExport);
    }
}

//
// Adds the script's procedures to the sco file being compiled by the context
//
void GenerateSCOVariables(CompileContext &context, const Script &script)
{
    const VariableDeclVector &vars = script.GetScriptVariables();
    WORD wIndex = 0;
    BOOST_FOREACH(const VariableDecl *pVar, vars)
    {
        WORD wSize = pVar->GetSize();
        ASSERT(wSize != 0);

        // Determine the species number of the type.
        SpeciesIndex wTypeSpecies = 0;
        if (!context.LookupTypeSpeciesIndex(pVar->GetDataType(), wTypeSpecies))
        {
            context.ReportError(pVar, "Unknown type for %s: %s.", pVar->GetName().c_str(), pVar->GetDataType().c_str());
        }

        context.AddSCOVariable(CSCOLocalVariable(pVar->GetName(), wTypeSpecies.Type()));
        --wSize;
        // Fill up arrays with empty var names... this is important so the index remains correct
        for (WORD w = 0; w < wSize; w++)
        {
            context.AddSCOVariable(CSCOLocalVariable());
        }
        ++wIndex;
    }
}

//
// Generates SCO objects for a Script and adds them to the context.
// These are used so that other parts of the script can reference
// them, just like there were in another script that we included that was already compiled.
//
// This is also where we potentially generate new species indexes
//
void GenerateSCOObjects(CompileContext &context, const Script &script)
{
    ClassVector classes = script.GetClasses(); // Note: this is a copy of the ClassVector

    // First, ensure that we have an entry in the species table for each class that is defined.
    // We do it here (instead of sorted, below), because they should appear in the order that
    // they were defined in the script.
    // scriptClassIndexToSpeciesIndex will make the index of each in class in this script,
    // to a global species index.
    vector<WORD> scriptClassIndexToSpeciesIndex;
    BOOST_FOREACH(const ClassPtr pClass, classes)
    {
        if (!pClass->IsInstance())
        {
            // This is a class.  Ensure it has a spot in the species table.
            WORD wSpeciesIndex = context.EnsureSpeciesTableEntry((WORD)scriptClassIndexToSpeciesIndex.size());
            scriptClassIndexToSpeciesIndex.push_back(wSpeciesIndex);

            // This is sort of a hack.  We need to have the CSCOObjectClass for all the classes in this file
            // around, prior to executing the rest of this function.  For example, if the Script class has
            // a property of type Script (which it does), we need this data type to resolve propertly.
            // That won't happen unless we have added the CSCOObjectClass to the "this" CSCOFile in the
            // CompileContext.  So add dummy ones now.  We'll replace with the real ones later.
            CSCOObjectClass scoDummy;
            scoDummy.SetName(pClass->GetName());
            context.AddSCOClass(scoDummy, false);
        }
    }

    // NOTE: if the user moves classes around in a file, things will get corrupt.  The species indicies will
    // be wrong.  We need to figure this out now (look in the SCO file?) - and tell the species table that
    // it needs to dirty itself, even though the number of classes didn't change... hmm. Actually - that won't
    // change the species table at all.  But it does mean that any file that references one of these classes
    // will need to recompile.

    // Now that we're sure we have an entry in the species table for each class, we
    // can construct the SCO object for each.
    WORD wClassIndexInScript = 0;
    BOOST_FOREACH(const ClassPtr pClass, classes)
    {
        CSCOObjectClass sco;
        sco.SetPublic(pClass->IsPublic());
        // Make SCO objects for instances too, but only as a convenience for writing in the script resource.
        WORD wSuperClass; // This must already exist elsewhere, since we just scanned through the classes.
        if (pClass->GetSuperClass().empty())
        {
            // This is the game where there is no super class.  0xffff is a sentinel value that the
            // interpreter uses for this.  We want to put this logic here, instead of in LookupSpeciesIndex,
            // since we only want to allow empty superclasses right here (and not other places where we
            // call LookupSpeciesIndex).
            wSuperClass = DataTypeNone; // 0xffff
        }
        else
        {
            SpeciesIndex sindex;
            if (context.LookupSpeciesIndex(pClass->GetSuperClass(), sindex))
            {
                wSuperClass = sindex.Type();
            }
            else
            {
                wSuperClass = 0x50FE; // sentinel value for debugging
                ErrorHelper(context, pClass, "Unknown class", pClass->GetSuperClass());
            }
        }
        // Set superclass
        sco.SetSuperClass(wSuperClass);
        // set name
        sco.SetName(pClass->GetName()); // TODO: check conflicts?
        // set species
        if (pClass->IsInstance())
        {
            sco.SetSpecies(wSuperClass);
        }
        else
        {
            sco.SetSpecies(scriptClassIndexToSpeciesIndex[wClassIndexInScript]);
            wClassIndexInScript++;
        }
        // The --info-- is implied... although we are making temporary scos for instances.

        vector<CSCOObjectProperty> scoProperties;
        // Now get the properties for our super class
        property_vector speciesProps = context.GetSpeciesProperties(pClass->GetSuperClass());
        // And get the properties that we declared
        property_vector newProps = GetOverriddenProperties(context, pClass);

        // But first, provide some of our own overrides.
        ASSERT(speciesProps.size() >= 4);
        speciesProps[0].wValue = sco.GetSpecies();
        speciesProps[1].wValue = wSuperClass; // superclass index
        speciesProps[2].wValue = pClass->IsInstance() ? 0x0000 : 0x8000; // --info--
        speciesProps[3].wValue = context.GetStringTempOffset(pClass->GetName()); // name (can be overridden explicitly too)

        // Replace the species default values, with any that the user specified.
        int iIndex = 0;
        BOOST_FOREACH(species_property &speciesProp, speciesProps)
        {
            WORD wValue = speciesProp.wValue;
            // name is tracked by default (since we populate with a string by default)
            // Other species props are not tracked by default.
            bool fTrackRelocation = (iIndex == 3);
            property_vector::const_iterator overriddenIt = find_if(newProps.begin(), newProps.end(), bind2nd(MatchSelector(), speciesProp.wSelector));
            if (overriddenIt != newProps.end())
            {
                wValue = overriddenIt->wValue;
                fTrackRelocation = overriddenIt->fTrackRelocation;
                // We overrode it... class property syntax allows specifying a type.
                // Make sure the user didn't do this, since the base class already specified the type.
                if (overriddenIt->wType != DataTypeNone) // (Assumed that it's always DataTypeNone for instances.)
                {
                    ASSERT(!pClass->IsInstance()); // Had better be a class if the user specified a data type.
                    string propName = context.LookupSelectorName(overriddenIt->wSelector);
                    context.ReportError(pClass, "The base class '%s' already has a type for '%s'.", pClass->GetSuperClass().c_str(), propName.c_str());
                }
            }

            if (pClass->IsInstance())
            {
                // Don't care about type for instances
                scoProperties.push_back(CSCOObjectProperty(speciesProp.wSelector, wValue, fTrackRelocation));
            }
            else
            {
                // For classes, make sure no type was specified.  BUT!!!! Re-use the type we know from
                // the super class (just for convenience).
                scoProperties.push_back(CSCOObjectProperty(speciesProp.wSelector, wValue, speciesProp.wType, fTrackRelocation));
            }
            ++iIndex;
        }
        // Now scan the new props for any new properties!
        BOOST_FOREACH(species_property &newProp, newProps)
        {
            // Is this a new property, not defined by the superclass?
            property_vector::const_iterator speciesIt = find_if(speciesProps.begin(), speciesProps.end(), bind2nd(MatchSelector(), newProp.wSelector));
            if (speciesIt == speciesProps.end())
            {
                // Must be - we didn't find it in the species props.
                // Make sure it's not a default property either
                if (newProp.wSelector < 3)
                {
                    // Someone tried to override a default property
                    context.ReportError(pClass, "species, superclass, and -info- may not be overridden.");
                }
                else
                {
                    if (pClass->IsInstance())
                    {
                        // User tried to declare a new property on an instance.
                        string propName = context.LookupSelectorName(newProp.wSelector);
                        context.ReportError(pClass, "Instances can not declare new properties: '%s'.", propName.c_str());
                    }
                    else
                    {
                        // This is a new property for this class.  The selector should have already been defined, if its new.
                        // Also, this had better have a type.
                        if (newProp.wType == DataTypeNone)
                        {
                            // Get some info for the error.
                            string propName = context.LookupSelectorName(newProp.wSelector);
                            ClassPropertyVector::const_iterator propIt = match_name(pClass->GetProperties().begin(), pClass->GetProperties().end(), propName);
                            // propIt should always be valid.
                            context.ReportError(*propIt, "Property '%s' is not defined on the base class.  A data type must be supplied.", propName.c_str());
                        }
                        scoProperties.push_back(CSCOObjectProperty(newProp.wSelector, newProp.wValue, newProp.wType, newProp.fTrackRelocation));
                    }
                }
            }
        }
        sco.SetProperties(scoProperties);

        // Now methods
        vector<CSCOMethod> methodsOut;
        BOOST_FOREACH(const MethodDefinition *pMethod, pClass->GetMethods())
        {
            const string &methodName = pMethod->GetName();
            CSCOMethod theMethod;
            theMethod.SetSelector(context.LookupSelectorAndAdd(methodName)); // Add it if it doesn't exist.

            // Add the signatures:
            std::vector<CSCOFunctionSignature> signatures;
            _CreateSCOFunctionSignature(context, signatures, pMethod->GetSignatures());
            theMethod.SetSignatures(signatures);

            methodsOut.push_back(theMethod);
        }
        sco.SetMethods(methodsOut);
        if (pClass->IsInstance())
        {
            context.AddSCOClass(sco, pClass->IsInstance());
        }
        else
        {
            context.ReplaceSCOClass(sco); // Since we had to already add it.
        }
    }
}

//
// Writes the class or instance described by object, into the script resource output stream.
//
void _WriteClassOrInstance(CSCOObjectClass &object, bool fInstance, vector<BYTE> &output, CompileContext *pContext)
{
    size_t sizeBeforeOutput = output.size();
    // Section header and magic number
    push_word(output, fInstance ? 0x0001 : 0x0006); // 1 = instance, 6 = class
    size_t iPlaceToWriteSize = output.size();
    push_word(output, 0);   // A temporary value where we'll write the size
    push_word(output, 0x1234);  // Magic value

    // WORD: local variable offset (filled in at runtime)
    push_word(output, 0x0000);

    vector<CSCOObjectProperty> &properties = object.GetPropertiesNonConst();
    WORD wNumProps = (WORD)properties.size();
    // WORD: offset to function selectors
    push_word(output, 2 + (fInstance ? 2 : 4) * wNumProps); // classes have both selectors and values, instances just have values
    // WORD: number of property values
    push_word(output, wNumProps);

    // This is where code that has offsets to this instance should point to (for some reason)
    WORD wObjectPointer = (WORD)output.size();
    if (fInstance)
    {
        // So tell that code to point here.
        _FixupReferencesHelper(output, pContext->GetInstanceReferences(), object.GetName(), wObjectPointer);
        if (object.IsPublic())
        {
            // If it's public, we need to put this in the export table.
            pContext->TrackPublicInstance(wObjectPointer);
        }
    }

    // Property selectors and values
    vector<CSCOObjectProperty>::iterator propertyIt = properties.begin();
    while(propertyIt != properties.end())
    {
        WORD wValue = propertyIt->GetValue();
        if (propertyIt->NeedsReloc())
        {
            pContext->TrackRelocation((WORD)output.size());
            // If it needs a relocation, it's an offset, for which we current only have a temp
            // value.  Get the real value.
            wValue = pContext->LookupFinalStringOrSaidOffset(wValue);
        }
        push_word(output, wValue);
        propertyIt++;
    }
    if (!fInstance)
    {
        // Only classes specify their property selectors (defined implicitly for instances by their species)
        propertyIt = properties.begin();
        while(propertyIt != properties.end())
        {
            // (Don't mess with for_each in here.)
            push_word(output, propertyIt->GetSelector());
            propertyIt++;
        }
    }

    // Method pointers
    const vector<CSCOMethod> &methods = object.GetMethods();
    // WORD: the number of methods
    push_word(output, (WORD)methods.size());
    // n*WORD: the method selectors
    BOOST_FOREACH(const CSCOMethod &scoMethod, methods)
    {
        push_word(output, scoMethod.GetSelector());
    }

    // WORD: zero
    push_word(output, 0x0000);
    // n*WORD: the method function pointers
    string className = object.GetName();
    BOOST_FOREACH(const CSCOMethod &scoMethod, methods)
    {
        string methodName = pContext->LookupSelectorName(scoMethod.GetSelector());
        ASSERT(!methodName.empty()); // Means we have a bug.
        code_pos methodPos = pContext->GetLocalProcPos(className + "::" + methodName);
        // Then from the code_pos, we get the offset at which it was written.
        push_word(output, methodPos->get_final_offset());
    }

    // Finally write the size of this section
    write_word(output, iPlaceToWriteSize, (WORD)(output.size() - sizeBeforeOutput));
}

//
// Write all class and instance sections to the script resource output stream
//
void _Section1And6_ClassesAndInstances(vector<BYTE> &output, CompileContext *pContext)
{
    
    CSCOFile &sco = pContext->GetScriptSCO();
    BOOST_FOREACH(CSCOObjectClass &oClass, sco.GetObjects())
    {
        _WriteClassOrInstance(oClass, false, output, pContext);
    }

    // Instances
    BOOST_FOREACH(CSCOObjectClass &iClass, pContext->GetInstanceSCOs())
    {
        _WriteClassOrInstance(iClass, true, output, pContext);
    }
}

void _Section10_LocalVariables(Script &script, vector<BYTE> &output)
{
    const VariableDeclVector &scriptVars = script.GetScriptVariables();
    if (!scriptVars.empty())
    {
        push_word(output, 10);
        size_t localVarSizeIndex = output.size();
        push_word(output, 0); // Temporary value.
        for_each(scriptVars.begin(), scriptVars.end(), OutputSimpleVarDeclValues(output));
        write_word(output, localVarSizeIndex, (WORD)(output.size() - localVarSizeIndex + 2));
    }
}

void _Section2_Code(Script &script, CompileContext &context, vector<BYTE> &output, WORD &wStartOfCode)
{
    context.PushVariableLookupContext(&script); // Don't really need to pop this, ever.
    const ProcedureVector &procs = script.GetProcedures();
    for_each(procs.begin(), procs.end(), GenericOutputByteCode(context));
    // b) classes, methods
    const ClassVector &classes = script.GetClasses();
    for_each(classes.begin(), classes.end(), GenericOutputByteCode(context));
    // Now some dirty work...
    context.code().fixup_offsets(context.GetOffsetFixups());
    context.FixupLocalCalls();
    push_word(output, 2);
    WORD wCodeSize = context.code().calc_size() + 4;
    bool fRoundUp = make_even(wCodeSize);
    push_word(output, wCodeSize);
    wStartOfCode = (WORD)output.size(); // Store where the code begins
    context.code().write_code(output);
    zero_pad(output, fRoundUp);
}

void _Section3_Synonyms(Script &script, CompileContext &context, vector<BYTE> &output)
{
    const SynonymVector &synonyms = script.GetSynonyms();
    if (!synonyms.empty())
    {
        push_word(output, 3); // 3 = synonyms
        push_word(output, (WORD)(synonyms.size() * 4) + 4); // 4 bytes per synonym entry
        SynonymVector::const_iterator synIndex = synonyms.begin();
        while (synIndex != synonyms.end())
        {
            WORD wGroup = 0;
            if (!context.LookupWord((*synIndex)->MainWord, wGroup))
            {
                context.ReportError(*synIndex, "'%s' is not in the vocabulary.", (*synIndex)->MainWord.c_str());
            }
            push_word(output, wGroup); // Write anyway, so we can continue...
            if (!context.LookupWord((*synIndex)->Replacement, wGroup))
            {
                context.ReportError(*synIndex, "'%s' is not in the vocabulary.", (*synIndex)->Replacement.c_str());
            }
            push_word(output, wGroup);
            ++synIndex;
        }
    }
}

void _Section4_Saids(CompileContext &context, vector<BYTE> &output)
{
    stringcode_map &saids = context.GetSaids();
    if (!saids.empty())
    {
        push_word(output, 4);
        size_t saidSizeIndex = output.size();
        push_word(output, 0); // Temporary size...
        // Now actually write out the saids into the stream.
        stringcode_map::iterator saidIt = saids.begin();
        while (saidIt != saids.end())
        {
            // a) Get the index to which we'll write the said stream.
            WORD wAbsolute = (WORD)output.size();
            context.SpecifyFinalSaidOffset(saidIt->first, wAbsolute);
            _FixupReferencesHelper(output, context.GetSaidReferences(), saidIt->first, wAbsolute);
            // b) Parse the said stream
            ParseSaidString(&context, *saidIt, output);
            // c) update the offset...
            ++saidIt;
        }
        // How much room did that take?
        WORD wSaidSectionSize = (WORD)(output.size() - saidSizeIndex) + 2;
        // Round it up to a WORD boundary and pad if necessary:
        zero_pad(output, make_even(wSaidSectionSize));
        // Go back and write the size.
        write_word(output, saidSizeIndex, wSaidSectionSize);
    }
}

void _Section5_Strings(CompileContext &context, vector<BYTE> &output)
{
    stringcode_map &strings = context.GetStrings();
    if (!strings.empty())
    {
        push_word(output, 5);
        // Compute the length of all the strings.
        WORD wStringSectionSize = accumulate(strings.begin(), strings.end(), 0, ComputeStringLength);
        // Round it up to a WORD boundary:
        bool fRoundUp = make_even(wStringSectionSize);
        // Indicate the section size:
        push_word(output, wStringSectionSize + 4);
        // Now actually write the strings, and update the parts of the code with the absolute positions
        // of each string (not yet relative pos's).
        // REVIEW: does this need to be a pointer?  References aren't allowed... does that mean it's already a ref?
        // Can't get an algorithm to work for this.
        stringcode_map::iterator stringIt = strings.begin();
        while (stringIt != strings.end())
        {
            // Get the index to which we'll write the string.
            WORD wAbsolute = (WORD)output.size();
            context.SpecifyFinalStringOffset(stringIt->first, wAbsolute);
            _FixupReferencesHelper(output, context.GetStringReferences(), stringIt->first, wAbsolute);
            output.insert(output.end(), stringIt->first.begin(), stringIt->first.end());
            output.push_back(0);
            stringIt++;
        }
        zero_pad(output, fRoundUp);
    }
}

void _Section7_Exports_Part1(Script &script, CompileContext &context, vector<BYTE> &output, size_t &numExports, size_t &indexOfExports)
{
    numExports = 0;
    const ClassVector &classes = script.GetClasses();
    numExports += count_if(classes.begin(), classes.end(), IsPublicInstance);   // public instances
    const ProcedureVector &procs = script.GetProcedures();
    numExports += count_if(procs.begin(), procs.end(), IsPublicProcedure);      // public procedures
    push_word(output, 7); // 7 = exports
    push_word(output, (WORD)(numExports * 2) + 4 + 2); // exports + header + exportCount.
    push_word(output, (WORD)numExports); // the number of exports
    indexOfExports = output.size(); // Store where the ptrs will go.
    // Fill with zeroes for now:
    output.insert(output.end(), numExports * 2, 0);
}

void _Section7_Exports_Part2(CompileContext &context, vector<BYTE> &output, WORD wStartOfCode, size_t numExports, size_t indexOfExports)
{
    // Now code  and classes have been written to the stream.  It's time to set up the exports table, which starts back at
    // indexOfExports.  We need to take one additional step for the main script.
    // There are two types of exports:
    // 1) public procedures (common)
    // 2) public instances (generally 1 per script).
    // public instances need to come first - as the interpreter looks for the first export, and may assume its
    // a room/locale/rgn, or in the case of script 0, the Game instance.
    // In here, we write the offset from the begining of the resource to the beginning of the code.
    vector<code_pos> &exports = context.GetExports();
    vector<WORD> &instanceOffsets = context.GetPublicInstanceOffsets();
    if (numExports == (exports.size() + instanceOffsets.size())) // Otherwise we generate a corrupt resource!
    {
        // Public instances first.
        for (vector<WORD>::iterator instIt = instanceOffsets.begin(); instIt != instanceOffsets.end(); ++instIt)
        {
            write_word(output, indexOfExports, *instIt);
            indexOfExports += 2;
        }
        // Then procedures.
        for (vector<code_pos>::iterator foo = exports.begin(); foo != exports.end(); foo++)
        {
            code_pos procEntry = (*foo);
            WORD wCodeOffset = context.code().offset_of(procEntry);
            write_word(output, indexOfExports, wStartOfCode + wCodeOffset);
            indexOfExports += 2;
        }
    }
    else
    {
        // We're not putting the exports in, so it's an incomplete scritp resource - we'd better have some errors.
        ASSERT(context.HasErrors());
    }
}

void _Section8_RelocationTable(CompileContext &context, vector<BYTE> &output)
{
    const vector<WORD> &relocations = context.GetRelocations();
    push_word(output, 8);
    push_word(output, 6 + (WORD)relocations.size() * 2);  // section size
    push_word(output, (WORD)relocations.size());    // # of relocation entries
    for_each(relocations.begin(), relocations.end(), WordToByte(output));
}

class FixCaseStatements : public IExploreNode
{
public:
    FixCaseStatements(ICompileContext &context) : _context(context) {}

private:
    //
    // Problems with parsing the SCIStudio syntax means we may need to make some adjustments in the
    // syntax tree.
    //
    void ExploreNode(IExploreNodeContext *pContext, SyntaxNode &node)
    {
        // 1)
        // Look for case statements of the form
        // (case foo(stuff))    // foo is a procedure call, and stuff is parameters
        // and turn them into
        // (case foo (stuff))   // foo is a value, and stuff is code
        if (node.GetNodeType() == NodeTypeCase)
        {
            CaseStatement &caseStatement = static_cast<CaseStatement&>(node);
            SingleStatement &caseValue = caseStatement.GetCaseValue();
            if (caseValue.GetType() == NodeTypeProcedureCall)
            {
                ProcedureCall *pProc = static_cast<ProcedureCall*>(caseValue.GetSegment());
                if (ProcedureUnknown == _context.LookupProc(pProc->GetName()))
                {
                    // This isn't a procedure call.  "Undo it"
                    // The name becomes a simple token value - let's construct that.
                    auto_ptr<ComplexPropertyValue> pTokenValue(new ComplexPropertyValue);
                    pTokenValue->SetValue(pProc->GetName(), ValueTypeToken);
                    // ...and the parameters become the code statements. So steal the procedures parameters.
                    SingleStatementVector formerParams;
                    pProc->StealParams(formerParams);
                    // Replace the casevalue's procedure, with our "simple token value"
                    // whose parameters we have now stolen.
                    caseValue.SetSyntaxNode(pTokenValue.get()); // This will delete pProc.
                    pTokenValue.release();
                    // Finally, add back the statements.  In case there is already some code in the
                    // case statement, we'll make sure to insert ours at the beginning.
                    SingleStatementVector &existingCode = caseStatement.GetCodeSegments();
                    existingCode.insert(existingCode.begin(), formerParams.begin(), formerParams.end());
                }
            }
        }
    }

    ICompileContext &_context;
};

bool GenerateScriptResource(Script &script, PrecompiledHeaders &headers, CompileTables &tables, CompileResults &results)
{
    vector<BYTE> &output = results.GetScriptResource();

    // Create our "CompileContext", which holds state during the compilation.
    CompileContext context(script, headers, tables, results.GetLog());

    _Section3_Synonyms(script, context, output);

    // Load the include files
    context.LoadIncludes();
    // Set the script number now (might have relied on defines)
    context.SetScriptNumber();
    results.SetScriptNumber(context.GetScriptNumber());
    // Do some prescans (script number must already be set!)
    script.PreScan(context);
    // Ok, now we should have been told about all the saids and strings.
    if (script.Language() == LangSyntaxSCIStudio)
    {
        // Fix up case statements we may have mis-interpreted.
        FixCaseStatements hack(context);
        script.Traverse(NULL, hack);
    }

    // To figure out how many exports we have, let's look at the public procedures and public instances
    size_t numExports = 0;
    size_t indexOfExports = 0;
    _Section7_Exports_Part1(script, context, output, numExports, indexOfExports);

    // Generate SCO objects for the classes and instances in the script.  We want to do this before generating any code,
    // since some code relies on it.
    GenerateSCOObjects(context, script);
    GenerateSCOPublics(context, script);
    GenerateSCOVariables(context, script);

    // It would be nice to put the code right after the saids and strings,
    // in the hope that we can use some 8bit opcodes instead of 16bit ones.  However, it
    // turns out it's easier to put them after, with the instances.  It makes it easier to fix up
    // the references to saids, strings and instances that are in the code.
    WORD wStartOfCode = 0;
    _Section2_Code(script, context, output, wStartOfCode);

    _Section5_Strings(context, output);

    _Section4_Saids(context, output);

    _Section1And6_ClassesAndInstances(output, &context);

    _Section7_Exports_Part2(context, output, wStartOfCode, numExports, indexOfExports);

    _Section10_LocalVariables(script, output);

    _Section8_RelocationTable(context, output);

    // Finish off by writing 0x0000
    push_word(output, 0);

#ifdef DEBUG
    char sz[100];
    int iLength = (int)output.size();
    int iThousandths = (iLength % 1024) * 1000 / 1024;
    int iRoundedHundredths = (iLength % 1024) * 100 / 1024;
    if (iRoundedHundredths * 10 + 5 < iThousandths)
    {
        iRoundedHundredths++;
    }

    StringCchPrintf(sz, ARRAYSIZE(sz), "Generated script resource: %d.%02dKB\n", iLength / 1024, iRoundedHundredths);
    OutputDebugString(sz);
#endif

    // Get the .sco file produced.
    results.GetSCO() = context.GetScriptSCO();
    
    // Fill the text resource.
    const vector<string> &resourceStrings = context.GetResourceStrings();
    copy(resourceStrings.begin(), resourceStrings.end(), back_inserter(results.GetTextResource().GetTexts()));

    return !context.HasErrors();
}


//
// Helper for ensuring that parameters to a function (or method) call match.
// Returns: the return type of the function that was called.
//
SpeciesIndex MatchParameters(const std::string &name, ICompileContext &context, const ISourceCodePosition *pPos, const std::vector<CSCOFunctionSignature> &signatures, const std::vector<SpeciesIndex> &parametersCaller, const SingleStatementVector &parameterStatements)
{
    // NOTE: there may be multiple matches.  In which case, we might have multiple return types.
    // The easiest thing to do is to return DataTypeAny in that case.  Otherwise we would have to payload
    // multiple return types into one.

    SpeciesIndex returnType = DataTypeAny;
    if (!signatures.empty())
    {
        bool fMatch = false;
        BOOST_FOREACH(const CSCOFunctionSignature &signature, signatures)
        {
            // Quick check:
            size_t cRequiredParams = static_cast<size_t>(signature.GetRequiredParameterCount());
            size_t cParams = static_cast<size_t>(signature.GetParameterCount());
            if (parametersCaller.size() >= cRequiredParams)
            {
                // They provided at least the required parameters.
                if ((parametersCaller.size() <= cParams) ||
                    ((parametersCaller.size() > cParams) && signature.GetAdditionalParametersOk()))
                {
                    // Now check each parameter.
                    size_t calleeIndex = 0;
                    size_t callerIndex = 0;
                    fMatch = true; // Optimistic
                    while (fMatch && (callerIndex < parametersCaller.size()))
                    {
                        if (callerIndex < cParams)
                        {
                            fMatch = DoesTypeMatch(context, signature.GetParameters()[callerIndex], parametersCaller[callerIndex], NULL, parameterStatements[callerIndex]);
                        }
                        else
                        {
                            fMatch = signature.GetAdditionalParametersOk();
                        }
                        ++callerIndex;
                    }
                    if (fMatch)
                    {
                        returnType = signature.GetReturnType();
                        break;
                    }
                }
            }
        }
        if (!fMatch)
        {
            std::stringstream ss;
            ss << "No overload matches '" << name << "(";
            bool fFirst = true;
            BOOST_FOREACH(SpeciesIndex si, parametersCaller)
            {
                if (!fFirst)
                {
                    ss << ", ";
                }
                ss << context.SpeciesIndexToDataTypeString(si);
                fFirst = false;
            }
            ss << ").";
            context.ReportError(pPos, ss.str().c_str());
        }
    }
    return returnType;
}
