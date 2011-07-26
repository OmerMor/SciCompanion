// ScriptOM.cpp : implementation file
//
#include "stdafx.h"
#include "ScriptOMAll.h"
#include "..\SCIProps.h"
#include "..\CompiledScript.h"
#include "CompileContext.h"

using namespace sci;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TOKENTYPEFOO Script::LookupVariableName(ICompileContext &context, const std::string &str, WORD &wIndex, SpeciesIndex &dataType) const
{
    TOKENTYPEFOO tokenType = TokenUnknown;
    // Find the index of a script variable
    wIndex = 0;
    for (VariableDeclVector::const_iterator index = _scriptVariables.begin(); index != _scriptVariables.end(); index++)
    {
        if ((*index)->GetName() == str)
        {
            tokenType = TokenScriptVariable;
            dataType = context.LookupTypeSpeciesIndex((*index)->GetDataType(), *index);
            break;
        }
        wIndex += (*index)->GetSize();
    }
    return tokenType;
}





//
// Used to create a new class programmatically.
//
void ClassDefinition::CreateNew(const Script *pScript, PCTSTR pszName, PCTSTR pszSpecies)
{
    _name = pszName;
    _superClass = pszSpecies;
    _fInstance = true;
    _fPublic = false;
}

bool ClassDefinition::SetProperty(PCTSTR pszName, PropertyValue value)
{
    bool fFound = false;
    for (size_t i = 0; !fFound && i < _properties.size(); i++)
    {
        fFound = (_properties[i]->GetName() == pszName);
        if (fFound)
        {
            _properties[i]->SetValue(value);
        }
    }
    if (!fFound)
    {
        ClassProperty *pProp = new ClassProperty();
        if (pProp)
        {
            pProp->SetName(pszName);
            pProp->SetValue(value);
            _properties.push_back(pProp);
            fFound = true;
        }
    }
    return fFound;
}

bool ClassDefinition::GetPropertyConst(PCTSTR pszName, PropertyValue &value) const
{
    bool fFound = false;
    // STLCLEANUP
    for (size_t i = 0; !fFound && i < _properties.size(); i++)
    {
        fFound = (_properties[i]->GetName() == pszName);
        if (fFound)
        {
            value = _properties[i]->GetValue();
        }
    }
    if (!fFound)
    {
        // Is it a pseudo prop?
        PFNCLASSTOPSEUDOPROP pfnClassToProp;
        if (IsPseudoProp(pszName, &pfnClassToProp))
        {
            fFound = (*pfnClassToProp)(this, pszName, value);
        }
    }
    return fFound;
}

bool ClassDefinition::GetProperty(PCTSTR pszName, PropertyValue &value)
{
    return GetPropertyConst(pszName, value);
}

TOKENTYPEFOO FunctionSignature::LookupVariableName(ICompileContext &context, const std::string &str, WORD &wIndex, SpeciesIndex &dataType) const
{
    // Is it a parameter?
    wIndex = 0;
    for (FunctionParameterVector::const_iterator position = _params.begin(); position != _params.end(); position++)
    {
        if ((*position)->GetName() == str)
        {
            wIndex++; // since position 0 was taken up by "paramTotal"
            dataType = context.LookupTypeSpeciesIndex((*position)->GetDataType(), this);
            return TokenParameter;
        }
        wIndex++;
    }
    return TokenUnknown;
}

//
// Looks up a temp var or a parameter name, and provides the index
//
TOKENTYPEFOO FunctionBase::LookupVariableName(ICompileContext &context, const std::string &str, WORD &wIndex, SpeciesIndex &dataType) const
{
    // Is it a temp variable?
    wIndex = 0;
    if (str == "paramTotal")
    {
        dataType = DataTypeInt; // paramTotal is a numerical value
        return TokenParameter;
    }

    for (VariableDeclVector::const_iterator position = _tempVars.begin(); position != _tempVars.end(); position++)
    {
        if ((*position)->GetName() == str)
        {
            // Found it.  Index should already be correct.
            dataType = context.LookupTypeSpeciesIndex((*position)->GetDataType(), *position);
            return TokenTemplVariable;
        }
        // Keep track of the "index".  Increase it by the size of the temp var (e.g. arrays have size > 1)
        wIndex += (*position)->GetSize();
    }

    wIndex = 0;
    TOKENTYPEFOO token = TokenUnknown;
    for (FunctionSignatureVector::const_iterator position = _signatures.begin(); (TokenUnknown == token) && (position != _signatures.end()); ++position)
    {
        token = (*position)->LookupVariableName(context, str, wIndex, dataType);
    }
    return token;
}


std::string PropertyValueBase::ToString() const
{
    TCHAR szDesc[MAX_PATH];
    szDesc[0] = 0;
    switch (_type)
    {
    case ValueTypeSelector:
    case ValueTypePointer:
        StringCchCopy(szDesc, ARRAYSIZE(szDesc), (_type == ValueTypeSelector) ? "#" : "@");
        // fall through...
    case ValueTypeString:
    case ValueTypeResourceString:
    case ValueTypeSaid:
    case ValueTypeToken:
        StringCchCat(szDesc, ARRAYSIZE(szDesc), _stringValue.c_str());
        break;
    case ValueTypeNumber:
        StringCchPrintf(szDesc, ARRAYSIZE(szDesc), _fHex ? TEXT("$%04x") : TEXT("%d"), _numberValue);
        break;
    case ValueTypeNone:
        StringCchCopy(szDesc, ARRAYSIZE(szDesc), TEXT("Undefined"));
        break;
    default:
        ASSERT(false);
        break;
    }
    return szDesc;
}

void PropertyValueBase::ToString(CString &str) const
{
    str = ToString().c_str();
}

std::string FunctionBase::ToString() const
{
    TCHAR szDesc[MAX_PATH * 2];
    StringCchPrintf(szDesc, ARRAYSIZE(szDesc), TEXT("%s("), _name.c_str());
    // TODO
    /*for (size_t i = 0; i < _params.size(); i++)
    {
        StringCchCat(szDesc, ARRAYSIZE(szDesc), (PCTSTR)_params[i]->GetName().c_str());
    }*/
    StringCchCat(szDesc, ARRAYSIZE(szDesc), TEXT(")"));
    return szDesc;
}


//
// Returns true if there is a statement of type type within this statement.
//
bool SingleStatement::_ContainsStatement(NodeType type) const
{
    if (_type == NodeTypeCodeBlock)
    {
        CodeBlock *pBlock = static_cast<CodeBlock*>(_pThing.get());
        const SingleStatementVector pStatements = pBlock->GetList();
        if (!pStatements.empty())
        {
            return (pStatements[0]->GetType() == type);
        }
    }
    return false;
}


//
// Equal implementations for script objects.
//
// For debugging purposes.
//
#define ASSERT_EQUAL(x)  ASSERT(x)

template<typename T>
bool equal_ptrs(const T &container1, const T &container2)
{
    if (container1.size() != container2.size())
    {
        return false;
    }
    T::const_iterator it1begin = container1.begin();
    T::const_iterator it2begin = container2.begin();
    T::const_iterator it1end = container1.end();
    T::const_iterator it2end = container2.end();
    bool fRet = true;
    while (fRet && (it1begin != it1end))
    {
        fRet = (*it1begin)->Equal(**it2begin);
        it1begin++;
        it2begin++;
    }
    return fRet;
}
Script::Script(PCTSTR pszFilePath, PCTSTR pszFileName)
{
    _scriptId = ScriptId(pszFileName, pszFilePath);
}
Script::Script(ScriptId script) : _scriptId(script)
{
}
Script::Script()
{
}

void Script::SetScriptNumber(WORD wNumber)
{
    _scriptId.SetResourceNumber(wNumber);
}

bool Script::Equal(const SyntaxNode &right) const
{
    const Script &Right = static_cast<const Script&>(right);
    bool fRet =
        _scriptDefine == Right._scriptDefine &&
        _scriptId == Right._scriptId &&
        _uses == Right._uses &&
        _includes == Right._includes &&
        equal_ptrs(_scriptVariables, Right._scriptVariables) &&
        equal_ptrs(_classes, Right._classes) &&
        equal_ptrs(_procedures, Right._procedures) &&
        equal_ptrs(_synonyms, Right._synonyms) &&
        equal_ptrs(_defines, Right._defines);
    ASSERT_EQUAL(fRet);
    return fRet;
}


bool PropertyValueBase::Equal(const SyntaxNode &right) const
{
    const PropertyValueBase &value2 = static_cast<const PropertyValueBase&>(right);
    bool fRet = (this == &value2);
    if (!fRet)
    {
        bool fIndexerEqual = false;
        if (GetIndexer() == NULL)
        {
            fIndexerEqual = (value2.GetIndexer() == NULL);
        }
        else if (value2.GetIndexer())
        {
            fIndexerEqual = (GetIndexer()->Equal(*value2.GetIndexer()));
        }
        fRet = fIndexerEqual &&
                (_type == value2._type) &&
                (_stringValue == value2._stringValue) &&
                (_numberValue == value2._numberValue) &&
                (_fNegate == value2._fNegate);
    }
    ASSERT_EQUAL(fRet);
    return fRet;
}

bool VariableDecl::Equal(const SyntaxNode &right) const
{
    const VariableDecl &Right = static_cast<const VariableDecl&>(right);
    bool fRet =
        _name == Right.GetName() &&
        _wSize == Right.GetSize() &&
        _type == Right.GetDataType();
    ASSERT_EQUAL(fRet);
    return fRet;
}


bool Define::Equal(const SyntaxNode &right) const
{
    const Define &Right = static_cast<const Define&>(right);
    bool fRet =
        _label == Right._label &&
        _wValue == Right._wValue;
    ASSERT_EQUAL(fRet);
    return fRet;
}

bool SingleStatement::Equal(const SyntaxNode &right) const
{
    const SingleStatement &Right = static_cast<const SingleStatement&>(right);
    bool fRet =
        _type == Right._type/* &&
        _pThing == Right._pThing*/;
    ASSERT_EQUAL(fRet);
    return fRet;
}

bool Synonym::Equal(const SyntaxNode &right) const
{
    const Synonym &Right = static_cast<const Synonym&>(right);
    bool fRet = 
        MainWord == Right.MainWord &&
        Replacement == Right.Replacement;
    ASSERT_EQUAL(fRet);
    return fRet;
}


bool FunctionBase::Equal(const SyntaxNode &right) const
{
    const FunctionBase &Right = static_cast<const FunctionBase&>(right);
    bool fRet = 
        _name == Right._name &&
        _signatures == Right._signatures &&
        equal_ptrs(_tempVars, Right._tempVars) &&
        equal_ptrs(_code, Right._code);
    ASSERT_EQUAL(fRet);
    return fRet;
}

bool ProcedureDefinition::Equal(const SyntaxNode &right) const
{
    const ProcedureDefinition &Right = static_cast<const ProcedureDefinition&>(right);
    bool fRet =
        __super::Equal(right) &&
        _public == Right._public;
    ASSERT_EQUAL(fRet);
    return fRet;
}

//
// Destructors for script objects.
//
WhileLoop::~WhileLoop()
{
    for_each(_code.begin(), _code.end(), delete_object());
}
CodeBlock::~CodeBlock ()
{
    for_each(_code.begin(), _code.end(), delete_object());
}
IfStatement::~IfStatement()
{
    for_each(_code.begin(), _code.end(), delete_object());
}
ElseClause::~ElseClause()
{
    for_each(_code.begin(), _code.end(), delete_object());
}
CaseStatement::~CaseStatement()
{
    for_each(_code.begin(), _code.end(), delete_object());
}
ForLoop::~ForLoop()
{
    for_each(_code.begin(), _code.end(), delete_object());
}
DoLoop::~DoLoop()
{
    for_each(_doList.begin(), _doList.end(), delete_object());
}
SendParam::~SendParam()
{
    for_each(_params.begin(), _params.end(), delete_object());
}
ProcedureCall::~ProcedureCall()
{
    for_each(_params.begin(), _params.end(), delete_object());
}
ConditionalExpression::~ConditionalExpression()
{
    for_each(_terms.begin(), _terms.end(), delete_object());
}
SingleStatement::~SingleStatement()
{
}
SendCall::~SendCall()
{
    for_each(_params.begin(), _params.end(), delete_object());
}
SwitchStatement::~SwitchStatement()
{
    for_each(_cases.begin(), _cases.end(), delete_object());
}
ComplexPropertyValue::~ComplexPropertyValue()
{
    delete _pArrayInternal;
}

Script::~Script()
{
    for_each(_scriptVariables.begin(), _scriptVariables.end(), delete_object());
    for_each(_classes.begin(), _classes.end(), delete_object());
    for_each(_procedures.begin(), _procedures.end(), delete_object());
    for_each(_synonyms.begin(), _synonyms.end(), delete_object());
    for_each(_defines.begin(), _defines.end(), delete_object());
    for_each(_comments.begin(), _comments.end(), delete_object());
}
ClassDefinition::~ClassDefinition()
{
    for_each(_properties.begin(), _properties.end(), delete_object());
    for_each(_methods.begin(), _methods.end(), delete_object());
}
FunctionSignature::~FunctionSignature()
{
    for_each(_params.begin(), _params.end(), delete_object());
}
FunctionBase::~FunctionBase()
{
    for_each(_tempVars.begin(), _tempVars.end(), delete_object());
    for_each(_code.begin(), _code.end(), delete_object());
    for_each(_signatures.begin(), _signatures.end(), delete_object());
}

//
// Copy constructors and assignment operators
// 
Cast::Cast(Cast &src) : OneStatementNode(_object), TypedNode(_type)
{
    _object = src._object;
    _type = src._type;
}
Cast& Cast::operator=(Cast &src)
{
    if (this != &src)
    {
        _object = src._object;
        _type = src._type;
    }
    return *this;
}

SendCall::SendCall(SendCall &src) : OneStatementNode(_object2), NamedNode(_object), SyntaxNode(src) 
{
    _object = src._object;
    _object2 = src._object2;
    _object3 = src._object3;
    _params.swap(src._params);
    _rest = src._rest;
    _fRest = src._fRest;
}
SendCall& SendCall::operator=(SendCall& src)
{
    if (this != &src)
    {
        _object = src._object;
        _object2 = src._object2;
        _object3 = src._object3;
        _params.swap(src._params);
        _rest = src._rest;
        _fRest = src._fRest;
    }
    return *this;
}
void SendCall::TransferParamsFrom(SendCall *pSend)
{
    ASSERT(pSend->_params.size() == 1); // Used for coalescing of "unit" send calls... so should only be one param.
    AddSendParam(pSend->_params[0]);
    pSend->_params.clear(); // Since we transfered....
}
void SendCall::SimplifySendObject()
{
    // Attempt to turn a complex object into a simple name.
    const PropertyValue *pValue = _object2.CastSyntaxNode<PropertyValue>();
    if (pValue && (pValue->GetType() == ValueTypeToken))
    {
        _object = pValue->GetStringValue();
    }
}
RestStatement::RestStatement(RestStatement &src) : NamedNode(_param), SyntaxNode(src) 
{
    _param = src._param;
}
RestStatement& RestStatement::operator=(RestStatement& src)
{
    if (this != &src)
    {
        _param = src._param;
    }
    return *this;
}
ProcedureCall::ProcedureCall(ProcedureCall &src) : NamedNode(_name), StatementsNode(_params), SyntaxNode(src) 
{
    _name = src._name;
    _params.swap(src._params);
}
ProcedureCall& ProcedureCall::operator=(ProcedureCall& src)
{
    if (this != &src)
    {
        _name = src._name;
        _params.swap(src._params);
    }
    return *this;
}
ConditionalExpression::ConditionalExpression(ConditionalExpression &src) : StatementsNode(_terms), SyntaxNode(src) 
{
    _terms.swap(src._terms);
    _andOrs.swap(src._andOrs);
}
ConditionalExpression& ConditionalExpression::operator=(ConditionalExpression& src)
{
    if (this != &src)
    {
        _terms.swap(src._terms);
        _andOrs.swap(src._andOrs);
    }
    return *this;
}
IfStatement::IfStatement(IfStatement &src) : StatementsNode(_code), ConditionNode(_condition), SyntaxNode(src) 
{
    _condition = src._condition;
    _code.swap(src._code);
}
IfStatement& IfStatement::operator=(IfStatement& src)
{
    if (this != &src)
    {
        _condition = src._condition;
        _code.swap(src._code);
    }
    return *this;
}

CppIfStatement::CppIfStatement(CppIfStatement &src) : OneStatementNode(_if), TwoStatementNode(_else), ConditionNode(_condition)
{
    _if = src._if;
    _else = src._else;
    _condition = src._condition;
    _fTernary = src._fTernary;
}
CppIfStatement& CppIfStatement::operator=(CppIfStatement& src)
{
    if (this != &src)
    {
        _if = src._if;
        _else = src._else;
        _condition = src._condition;
        _fTernary = src._fTernary;
    }
    return *this;
}

CaseStatement::CaseStatement(CaseStatement &src) : StatementsNode(_code), OneStatementNode(_case), SyntaxNode(src) 
{
    _fDefault = src._fDefault;
    _case = src._case;
    _code.swap(src._code);
}
CaseStatement& CaseStatement::operator=(CaseStatement& src)
{
    if (this != &src)
    {
        _fDefault = src._fDefault;
        _case = src._case;
        _code.swap(src._code);
    }
    return *this;
}
SwitchStatement::SwitchStatement(SwitchStatement &src) :OneStatementNode(_switch), SyntaxNode(src) 
{
    _switch = src._switch;
    _cases.swap(src._cases);
}
SwitchStatement& SwitchStatement::operator=(SwitchStatement& src)
{
    if (this != &src)
    {
        _switch = src._switch;
        _cases.swap(src._cases);
    }
    return *this;
}
ForLoop::ForLoop(ForLoop &src) : StatementsNode(_code), ConditionNode(_condition), CodeBlockNode(_initializer), SyntaxNode(src)
{
    _initializer = src._initializer;
    _condition = src._condition;
    _looper = src._looper;
    _code.swap(src._code);
}
ForLoop& ForLoop::operator=(ForLoop& src)
{
    if (this != &src)
    {
        _initializer = src._initializer;
        _condition = src._condition;
        _looper = src._looper;
        _code.swap(src._code);
    }
    return *this;
}
WhileLoop::WhileLoop(WhileLoop &src) : StatementsNode(_code), ConditionNode(_condition), SyntaxNode(src) 
{
    _condition = src._condition;
    _code.swap(src._code);
}
WhileLoop& WhileLoop::operator=(WhileLoop& src)
{
    if (this != &src)
    {
        _condition = src._condition;
        _code.swap(src._code);
    }
    return *this;
}
DoLoop::DoLoop(DoLoop &src) : ConditionNode(_condition), StatementsNode(_doList)
{
    _condition =src._condition;
    _doList.swap(src._doList);
}
DoLoop& DoLoop::operator=(DoLoop& src)
{
    if (this != &src)
    {
        _condition =src._condition;
        _doList.swap(src._doList);
    }
    return *this;
}
PropertyValueBase::PropertyValueBase(const PropertyValueBase& src) : SyntaxNode(src)
{
    _numberValue = src.GetNumberValue();
    _stringValue = src._stringValue;
    _type = src._type;
    _fHex = src._fHex;
    _fNegate = src._fNegate;
}
PropertyValueBase& PropertyValueBase::operator=(const PropertyValueBase& src)
{
    if (this != &src)
    {
        _type = src._type;
        _stringValue = src._stringValue;
        _numberValue = src._numberValue;
        _fHex = src._fHex;
        _fNegate = src._fNegate;
        ISourceCodePosition::operator=(src);
    }
    return (*this);
}
bool PropertyValueBase::operator==(const PropertyValueBase& value2) const
{
    bool fRet = (this == &value2);
    if (!fRet)
    {
        fRet = (this->_type == value2._type) &&
                (this->_stringValue == value2._stringValue) &&
                (this->_numberValue == value2._numberValue) &&
                (this->_fNegate == value2._fNegate);
    }
    return fRet;
}
bool PropertyValueBase::operator!=(const PropertyValueBase& value)
{
    return !(*this == value);
}

void PropertyValueBase::SetValue(int iValue, IntegerFlags flags)
{
    ASSERT(iValue <= 65536);
    _numberValue = static_cast<WORD>(iValue);
    _type = sci::ValueTypeNumber;
    _fHex = !!(flags & IFHex);
    _fNegate = !!(flags & IFNegative);
    _stringValue.clear();
}

ComplexPropertyValue::ComplexPropertyValue(ComplexPropertyValue& src) : PropertyValueBase(src)
{
    // Transfer arrayinternal.
    _pArrayInternal = src._pArrayInternal;
    src._pArrayInternal = NULL;
}

ComplexPropertyValue& ComplexPropertyValue::operator=(ComplexPropertyValue& src)
{
    if (this != &src)
    {
        PropertyValueBase::operator=(src);
        // Transfer arrayinternal.
        _pArrayInternal = src._pArrayInternal;
        src._pArrayInternal = NULL;
    }
    return (*this);
}

VariableDecl::VariableDecl() : StatementsNode(_initializers), TypedNode(_type), _wSize(1)
{
}
VariableDecl::VariableDecl(VariableDecl &src) : SyntaxNode(src), StatementsNode(_initializers), TypedNode(_type), ScriptSite(src)
{
    _name = src._name;
    _wSize = src._wSize;
    _type = src._type;
    _initializers.swap(_initializers);
}
VariableDecl& VariableDecl::operator=(VariableDecl& src)
{
    if (this != &src)
    {
        _name = src._name;
        _wSize = src._wSize;
        _type = src._type;
        _initializers.swap(_initializers);
        ScriptSite::operator =(src);
    }
    return *this;
}
VariableDecl::~VariableDecl()
{
    for_each(_initializers.begin(), _initializers.end(), delete_object());
}
void VariableDecl::AddSimpleInitializer(const PropertyValue &value)
{
    // Add a copy of the property value
    auto_ptr<SingleStatement> sValue(new SingleStatement);
    sValue->SetSyntaxNode(new PropertyValue(value));
    AddStatement(sValue);
}
ClassProperty::ClassProperty(const std::string &str, WORD wValue) : NamedNode(_name), TypedNode(_type)
{
    _name = str;
    _value.SetValue(wValue);
}

ClassProperty::ClassProperty(const std::string &str, const std::string &value) : NamedNode(_name), TypedNode(_type)
{
    _name = str;
    _value.SetValue(value, ValueTypeToken);
}

SingleStatement::SingleStatement(SingleStatement &src) : SyntaxNode(src)
{
    _type = src._type;
    _pThing = src._pThing;
}
SingleStatement& SingleStatement::operator=(SingleStatement& src)
{
    if (this != &src)
    {
        _type = src._type;
        _pThing = src._pThing;
    }
    return (*this);
}

FunctionParameter::FunctionParameter() : NamedNode(_name), TypedNode(_type) {}
FunctionParameter::FunctionParameter(const std::string &str) : NamedNode(_name), TypedNode(_type)
{
    _name = str;
}
FunctionParameter::FunctionParameter(FunctionParameter &src) : SyntaxNode(src), NamedNode(_name), TypedNode(_type)
{
    _name = src._name;
    _type = src._type;
}
FunctionParameter& FunctionParameter::operator=(FunctionParameter& src)
{
    if (this != &src)
    {
        _name = src._name;
        _type = src._type;
    }
    return (*this);
}


FunctionSignature::FunctionSignature() : TypedNode(_type), ScriptSite(), _fMoreParameters(false), _iOptional(NoOptional) {  }
FunctionSignature::FunctionSignature(FunctionSignature &src) : SyntaxNode(src), TypedNode(_type) 
{
    _type = src._type;
    _params.swap(src._params);
    _fMoreParameters = src._fMoreParameters;
    _iOptional = src._iOptional;
}
FunctionSignature& FunctionSignature::operator=(FunctionSignature& src)
{
    if (this != &src)
    {
        _type = src._type;
        _params.swap(src._params);
        _fMoreParameters = src._fMoreParameters;
        _iOptional = src._iOptional;
        ScriptSite::operator=(src);
    }
    return (*this);
}
void FunctionSignature::AddParam(const std::string &name)
{
    _params.push_back(new FunctionParameter(name));
}
void FunctionSignature::AddParam(FunctionParameter *pParam, bool fOptional)
{
    _params.push_back(pParam);
    if (fOptional && (_iOptional == NoOptional))
    {
        _iOptional = _params.size() - 1; // This is the beginning of optional parameters.
    }
    ASSERT(fOptional || (_iOptional == NoOptional));
}
void FunctionSignature::AddParam(std::auto_ptr<FunctionParameter> pParam, bool fOptional)
{
    AddParam(pParam.get(), fOptional);
    pParam.release();
}


FunctionBase::FunctionBase() : NamedNode(_name), StatementsNode(_code), ScriptSite() { _pOwnerClass = NULL; }
FunctionBase::FunctionBase(const std::string &str) : NamedNode(_name), StatementsNode(_code), ScriptSite()
{
    _name = str;
}
FunctionBase::FunctionBase(FunctionBase &src) : SyntaxNode(src), ScriptSite(src), NamedNode(_name), StatementsNode(_code) 
{
    _name = src._name;
    _signatures.swap(src._signatures);
    _tempVars.swap(src._tempVars);
    _code.swap(src._code);
    _pOwnerClass = src._pOwnerClass;
    _docDescription = src._docDescription;
}
FunctionBase& FunctionBase::operator=(FunctionBase& src)
{
    if (this != &src)
    {
        _name = src._name;
        _signatures.swap(src._signatures);
        _tempVars.swap(src._tempVars);
        _code.swap(src._code);
        _pOwnerClass = src._pOwnerClass;
        _docDescription = src._docDescription;
        ScriptSite::operator=(src);
    }
    return (*this);
}
void FunctionBase::AddParam(const std::string &param)
{
    // This is for "backwards compatibility" with SCI syntax only...
    // Most uses should go directly through FunctionSignature.
    ASSERT(!_signatures.empty()); // Whomever calls AddParam had better add a signature.
    _signatures[0]->AddParam(param);
}
void FunctionBase::AddVariable(VariableDeclPtr pVar, PropertyValue value)
{
    // This is used by the SCIStudio compiler.
    std::auto_ptr<SingleStatement> pTemp(new SingleStatement);
    std::auto_ptr<PropertyValue> pValue(new PropertyValue(value));
    pTemp->SetSyntaxNode(pValue.get());
    pValue.release();
    pVar->AddStatement(pTemp);
    _tempVars.push_back(pVar);
}

MethodDefinition::MethodDefinition() { _fPrivate = false; }
MethodDefinition::MethodDefinition(MethodDefinition &src) : FunctionBase(src)
{
    _fPrivate = src._fPrivate;
}
MethodDefinition& MethodDefinition::operator=(MethodDefinition& src)
{
    if (this != &src)
    {
        _fPrivate = src._fPrivate;
        FunctionBase::operator=(src);
    }
    return (*this);
}

ProcedureDefinition::ProcedureDefinition() { _public = false; }
ProcedureDefinition::ProcedureDefinition(ProcedureDefinition &src) : FunctionBase(src)
{
    _public = src._public;
    _class = src._class;
}
ProcedureDefinition& ProcedureDefinition::operator=(ProcedureDefinition& src)
{
    if (this != &src)
    {
        _public = src._public;
        _class = src._class;
        FunctionBase::operator=(src);
    }
    return (*this);
}
ClassDefinition::ClassDefinition() : ScriptSite(), NamedNode(_name) { _fPublic = false; }

// Copy constructor and assignment for ClassDefinition are private.
ClassDefinition::ClassDefinition(ClassDefinition &src) : SyntaxNode(src), ScriptSite(src), NamedNode(_name) 
{
    ASSERT(FALSE);
}
ClassDefinition& ClassDefinition::operator=(ClassDefinition& src)
{
    if (this != &src)
    {
        ASSERT(FALSE);
    }
    return (*this);
}
CodeBlock::CodeBlock() : StatementsNode(_code) {}
CodeBlock::CodeBlock(CodeBlock &src) : SyntaxNode(src), StatementsNode(_code)
{
    _code.swap(src._code);
}
CodeBlock& CodeBlock::operator=(CodeBlock& src)
{
    if (this != &src)
    {
        _code.swap(src._code);
    }
    return *this;
}

ScriptSite::ScriptSite()
{
    _pScript = NULL;
}
ScriptSite::ScriptSite(const ScriptSite &src)
{
    _pScript = src._pScript;
}
ScriptSite& ScriptSite::operator=(const ScriptSite& src)
{
    if (this != &src)
    {
        _pScript = src._pScript;
    }
    return *this;
}

std::string Comment::GetSanitizedText() const
{
    if ((_comment[0] == '/') && (_comment[1] == '/'))
    {
        return _comment.substr(2);
    }
    else
    {
        ASSERT((_comment[0] == '/') && (_comment[1] == '*'));
        size_t len = _comment.length();
        ASSERT(len >= 4);
        return _comment.substr(2, len - 4);
    }
}

Asm::Asm(const Asm &src) : SyntaxNode(src), NamedNode(_instruction)
{
    _instruction = src._instruction;
    _arguments = src._arguments;
}

Asm& Asm::operator =(const Asm& src)
{
    if (this != &src)
    {
        _instruction = src._instruction;
        _arguments = src._arguments;
    }
    return *this;
}