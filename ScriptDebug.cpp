#include "stdafx.h"
#include "Compile\ScriptOMAll.h"

using namespace sci;
using namespace std;

//
// Contains implementations of SyntaxNode::DebugString for script objects.
//

//
// Convert some operators between SCIStudio and CPP syntax.
//
key_value_pair<PCSTR, PCSTR> c_rgSCIStudio2CPP[] = 
{
    { "bnot", "~" },
    { "not", "!" },
    { "<>", "!=" },
    { "neg", "-" },
};
PCSTR ToCPP(const string &name)
{
    return LookupStringValue(c_rgSCIStudio2CPP, ARRAYSIZE(c_rgSCIStudio2CPP), name, name.c_str());
}


template<typename _T>
void FwdDebugString(const _T &t, DebugStringOut &out, const char *pszSep = " ")
{
    _T::const_iterator it = t.begin();
    _T::const_iterator itEnd = t.end();
    bool fFirst = true;
    while (it != itEnd)
    {
        if (!fFirst && out.fInline)
        {
            out.out << pszSep;
        }
        (*it)->DebugString(out);
        fFirst = false;
        ++it;
    }
}

struct FwdDebugStringPtr : public std::binary_function<const SyntaxNode*, DebugStringOut, void>
{
    void operator()(const SyntaxNode* t, DebugStringOut out) const
    {
        if (t)
        {
            t->DebugString(out);
        }
        if (out.fInline)
        {
            // If these are all inline, add whitespace between them.
            out.out << " ";
        }
    }
};

void Indent(DebugStringOut &out)
{
    string spaces;
    spaces.append(out.iIndent, ' ');
    out.out << spaces;
}

void NewLine(DebugStringOut &out)
{
    if (!out.fInline)
    {
        out.out << out.NewLineString();
        Indent(out);
    }
}

void EndStatement(DebugStringOut &out)
{
    if (out.fInline)
    {
    }
    else
    {
        out.out << ";";
    }
}


template<typename _T>
void FwdDebugStatementList(const _T &t, DebugStringOut &out)
{
    ASSERT(out.lang != LangSyntaxSCIStudio);
    _T::const_iterator it = t.begin();
    _T::const_iterator itEnd = t.end();
    _T::const_iterator itLast = itEnd;
    --itLast;
    while (it != itEnd)
    {
        bool fLastOld = out.fLast;
        if (it == itLast)
        {
            out.fLast = true;
        }
        NewLine(out);
        (*it)->DebugString(out);
        ++it;
        out.fLast = fLastOld;
    }
}

void _OutputNumber(DebugStringOut &out, WORD wNum, bool fHex, bool fNegate)
{
    if (fNegate)
    {
        ASSERT(!fHex);
        wNum = (~wNum) + 1; // two's complement
        out.out << "-";
    }
    out.out << (fHex ? hex : dec);
    if (fHex)
    {
        out.out << "$" << setw(4) << setfill('0') << wNum;
    }
    else
    {
        out.out << wNum;
    }
}

void Define::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    bool fHex = !!(_flags & IFHex);
    bool fNegate = !!(_flags & IFNegative);
    if (out.lang == LangSyntaxSCIStudio)
    {
        out.out << "(define " << _label << " ";
        _OutputNumber(out, _wValue, fHex, fNegate);
        out.out <<  ")" << out.NewLineString();
    }
    else
    {
        out.out << "define " << _label << " ";
        _OutputNumber(out, _wValue, fHex, fNegate);
        out.out << ";" << out.NewLineString();
    }
}

void Script::DebugString(DebugStringOut &out) const
{
    if (out.lang == LangSyntaxSCIStudio)
    {
        vector<string>::const_iterator includesIt = _includes.begin();
        while (includesIt != _includes.end())
        {
            out.out << "(include \"" << *includesIt << "\")" << out.NewLineString();
            ++includesIt;
        }
        vector<string>::const_iterator usesIt = _uses.begin();
        while (usesIt != _uses.end())
        {
            out.out << "(use \"" << *usesIt << "\")" << out.NewLineString();
            ++usesIt;
        }
        if (_scriptId.GetResourceNumber() == InvalidResourceNumber)
        {
            out.out << "(script " << _scriptDefine << ")" << out.NewLineString();
        }
        else
        {
            out.out << "(script " << _scriptId.GetResourceNumber() << ")" << out.NewLineString();
        }
    }
    else
    {
        // C++
        vector<string>::const_iterator includesIt = _includes.begin();
        while (includesIt != _includes.end())
        {
            // If this is a conversion from SCI Syntax, don't include the standard headers.
            if ((*includesIt != "game.sh") && (*includesIt != "keys.sh") && (*includesIt != "sci.sh"))
            {
                out.SyncComments(this);
                out.out << "include \"" << *includesIt << "\";" << out.NewLineString();
            }
            ++includesIt;
        }
        NewLine(out);
        vector<string>::const_iterator usesIt = _uses.begin();
        while (usesIt != _uses.end())
        {
            out.SyncComments(this);
            out.out << "use \"" << *usesIt << "\";" << out.NewLineString();
            ++usesIt;
        }

        // c-style syntax has no script number.
        NewLine(out);
    }
    if (!_synonyms.empty())
    {
        for_each(_synonyms.begin(), _synonyms.end(), bind2nd(FwdDebugStringPtr(), out));   
        out.out << endl;
    }
    if (!_defines.empty())
    {
        for_each(_defines.begin(), _defines.end(), bind2nd(FwdDebugStringPtr(), out));
        out.out << endl;
    }
    if (!_scriptVariables.empty())
    {
        for_each(_scriptVariables.begin(), _scriptVariables.end(), bind2nd(FwdDebugStringPtr(), out));    
        out.out << endl;
    }
    if (out.lang == LangSyntaxSCIStudio)
    {
        for_each(_procedures.begin(), _procedures.end(), bind2nd(FwdDebugStringPtr(), out));
        out.out << endl;
        for_each(_classes.begin(), _classes.end(), bind2nd(FwdDebugStringPtr(), out));    
    }
    else
    {
        FwdDebugStatementList(_procedures, out);
        FwdDebugStatementList(_classes, out);
    }
}

//
// Adds DebugStringOut.iIndent spaces to the stream, if we're putting things on new lines.
//
class DebugLine
{
public:
    DebugLine(DebugStringOut &out) : _out(out)
    {
        if (!_out.fInline)
        {
            Indent(_out);
        }
    }
    ~DebugLine()
    {
        if (!_out.fInline)
        {
            _out.out << _out.NewLineString();
        }        
    }
private:
    DebugStringOut &_out;
};

//
// Handles a bracket scope (enforce order of operations)
//
class OutputBrackets
{
public:
    OutputBrackets(DebugStringOut &out) : _out(out)
    {
        if (_out.fUseBrackets)
        {
            _out.out << "(";
        }
    }
    ~OutputBrackets()
    {
        if (_out.fUseBrackets)
        {
            _out.out << ")";
        }
    }
private:
    DebugStringOut &_out;
};

//
// Handles an indentation scope
//
const cIndent = 4;
class DebugIndent
{
public:
    DebugIndent(DebugStringOut &out) : _out(out)
    {
        _out.iIndent += cIndent;
    }
    ~DebugIndent()
    {
        _out.iIndent -= cIndent;
    }
private:
    DebugStringOut &_out;
};

//
// Handles an "inline" scope (don't put statements on new lines)
//
class Inline
{
public:
    Inline(DebugStringOut &out, bool fInline) : _out(out)
    {
        _fOld = _out.fInline;
        _out.fInline = fInline;
    }
    ~Inline()
    {
        _out.fInline = _fOld;
    }
private:
    bool _fOld;
    DebugStringOut &_out;
};

//
// Indicates that order of operations must be made explicit (or not)
//
class ExplicitOrder
{
public:
    ExplicitOrder(DebugStringOut &out, bool fUseBrackets) : _out(out)
    {
        _fOld = _out.fUseBrackets;
        _out.fUseBrackets = fUseBrackets;
    }
    ~ExplicitOrder()
    {
        _out.fUseBrackets = _fOld;
    }
private:
    bool _fOld;
    DebugStringOut &_out;
};

void _OutputVariableAndSize(DebugStringOut &out, const std::string &type, const std::string &name, WORD wSize)
{
    out.out << (type.empty() ? "var" : type) << " " << name;
    if (wSize > 1)
    {
        out.out << "[" << wSize << "]"; // array
    }
}

void VariableDecl::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    // TODO: in caller, put a local block around things if these are script variables.
    _OutputVariableAndSize(out, _type, _name, _wSize);
    if (!out.fInline)
    {
        out.out << "\n";
    }
}

string UnescapeString(string src)
{
    size_t thePos;
    while ((thePos = src.find('\n')) != string::npos)
    {
        src.replace(thePos, 1, "\\n");
    }
    while ((thePos = src.find('\t')) != string::npos)
    {
        src.replace(thePos, 1, "\\t");
    }

    // Escape double-quote characters.
    size_t i = 0;
    while ((thePos = src.find('"', i)) != string::npos)
    {

        src.replace(thePos, 1, "\\\"");
        i = thePos + 2;
    }
    return src;
}

void PropertyValueBase::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    switch(_type)
    {
    case ValueTypeResourceString:
        if (out.lang == LangSyntaxCpp)
        {
            // This syntax is only available in LangSyntaxCpp
            out.out << "res ";
            break;
        }
        // fall through to ValueTypeString
    case ValueTypeString:
        out.out << "\"" << UnescapeString(_stringValue) << "\"";
        break;
    case ValueTypeSaid:
        out.out << "'" << _stringValue << "'";
        break;
    case ValueTypeNumber:
        _OutputNumber(out, _numberValue, _fHex, _fNegate);
        break;
    case ValueTypeToken:
        // Surround in braces if there are spaces in the string.
        if (_stringValue.find(' ') != std::string::npos)
        {
            out.out << '{' << _stringValue << '}';
        }
        // REVIEW: When this is C++ syntax, we should strip them out... and if it's -info-, use the replacement!
        // (objectSpecies)
        out.out << _stringValue;
        break;
    case ValueTypeSelector:
        out.out << "#" << _stringValue;
        break;
    case ValueTypePointer:
        out.out << "@" << _stringValue;
        break;
    }
}

void ComplexPropertyValue::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    __super::DebugString(out);
    if (_pArrayInternal)
    {
        Inline inln(out, true);
        out.out << "[";
        _pArrayInternal->DebugString(out);
        out.out << "]";
    }
}

void ClassProperty::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine line(out);
        out.out << _name << " ";
        _value.DebugString(out);
    }
    else
    {
        // Don't output the property type, as its only used in some cases
        // (in an object that newly defines a property)
        out.out  << _name << " = ";
        _value.DebugString(out);
        out.out << ";";
    }
}
void ClassDefinition::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        if (_fInstance)
        {
            out.out << "(instance ";
            if (_fPublic)
            {
                out.out << "public ";
            }
        }
        else
        {
            out.out << "(class ";
        }
        out.out << _name;
        if (!_superClass.empty())
        {
            out.out << " of " << _superClass;
        }
        out.out << out.NewLineString();

        {
            DebugIndent indent(out);
            {
                DebugLine line(out);
                if (_properties.empty())
                {
                    out.out << "(properties)";
                }
                else
                {
                    out.out << "(properties" << out.NewLineString();
                    {
                        DebugIndent indent2(out);
                        for_each(_properties.begin(), _properties.end(), bind2nd(FwdDebugStringPtr(), out));
                    }
                    Indent(out);
                    out.out << ")";
                }
            }

            // Body (methods)
            for_each(_methods.begin(), _methods.end(), bind2nd(FwdDebugStringPtr(), out));
        }
        out.out << ")" << out.NewLineString();
    }
    else
    {
        if (_fPublic)
        {
            out.out << "public ";
        }
        if (_fInstance)
        {
            out.out << "instance ";
        }
        else
        {
            out.out << "class ";
        }
        out.out << _name;
        if (!_superClass.empty())
        {
            out.out << " : " << _superClass;
        }
        out.out << out.NewLineString() << "{";

        {
            DebugIndent indent(out);
            {
                FwdDebugStatementList(_properties, out);
                //for_each(_properties.begin(), _properties.end(), bind2nd(FwdDebugStringPtr(), out));
            }
            if (!_properties.empty() && !_methods.empty())
            {
                NewLine(out); // Separator between props and methods.
            }
            // Body (methods)
            for_each(_methods.begin(), _methods.end(), bind2nd(FwdDebugStringPtr(), out));
        }
        NewLine(out);
        out.out << "}" << out.NewLineString();
    }
}

class CheckForReturnValue : public IExploreNode
{
public:
    std::string GetSugggestReturnValue() const { return _retValue.empty() ? "void" : _retValue; }

private:
    void ExploreNode(IExploreNodeContext *pContext, SyntaxNode &node)
    {
        if (_retValue.empty() && (node.GetNodeType() == NodeTypeReturn))
        {
            sci::ReturnStatement &returnStatement = static_cast<sci::ReturnStatement&>(node);
            const SingleStatement &retValue = returnStatement.GetValue();
            if (retValue.GetSyntaxNode() != NULL)
            {
                // TODO: we could do a little more deduction and determine the actual type.
                _retValue = "var";
            }
        }
    }

    std::string _retValue;
};

std::string _DeduceReturnType(FunctionBase &function)
{
    CheckForReturnValue en;
    function.Traverse(NULL, en);
    return en.GetSugggestReturnValue();
}

void FunctionBase::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        out.out << "(" << _name;
        const FunctionParameterVector &params = _signatures[0]->GetParams();
        FunctionParameterVector::const_iterator stringIt = params.begin();
        bool fFirst = false;
        while (stringIt != params.end())
        {
            if (!fFirst)
            {
                out.out << " ";
            }
            out.out << (*stringIt)->GetName();
            ++stringIt;
        }
        out.out << ")" << out.NewLineString();

        // Body of function
        {
            DebugIndent indent(out);
            {
                Inline inln(out, true);
                if (!_tempVars.empty())
                {
                    Indent(out);
                    out.out << "(";
                    FwdDebugString(_tempVars, out, ", ");
                    out.out << ")" << out.NewLineString();
                }
                // TODO: spit out the values here too.  _tempVarValues
                // And somehow coalesce variables types? Or Maybe not...
            }
            for_each(_code.begin(), _code.end(), bind2nd(FwdDebugStringPtr(), out));
        }

        DebugLine line(out);
        out.out << ")" << out.NewLineString();
    }
    else
    {
        std::string returnType = _signatures[0]->GetDataType();
        if (returnType.empty())
        {
            returnType = _DeduceReturnType(const_cast<sci::FunctionBase&>(*this));
        }
        out.out  << returnType << " " <<  _name << "(";
        const FunctionParameterVector &params = _signatures[0]->GetParams();
        FunctionParameterVector::const_iterator stringIt = params.begin();
        bool fFirst = true;
        while (stringIt != params.end())
        {
            if (!fFirst)
            {
                out.out << ", ";
            }
            std::string type = (*stringIt)->GetDataType();
            if (type.empty())
            {
                // If none specified (e.g. conversion from SCI syntax), use "var"
                type = "var";
            }
            out.out << type << " " << (*stringIt)->GetName();
            ++stringIt;
            fFirst = false;
        }
        out.out << ")";

        // A little hack.
        _DebugStringExtras(out);
        
        // Body of function
        NewLine(out);
        {
            DebugIndent indent(out);
            out.out << "{";
            {
                // Do something special with the variables.  Lay them out like:
                // var foo, bar, thing;
                // int x, yt;
                Inline inln(out, true);
                if (!_tempVars.empty())
                {
                    out.out << out.NewLineString();
                    Indent(out);
                    std::string currentDataType;
                    bool fFirstInLine = true;
                    for (size_t i = 0; i < _tempVars.size(); ++i)
                    {
                        std::string dataType = _tempVars[i]->GetDataType();
                        if (dataType.empty())
                        {
                            dataType = "var";
                        }
                        if (dataType != currentDataType)
                        {
                            if (i > 0)
                            {
                                out.out << ";" << out.NewLineString(); // Next line...
                                Indent(out);
                            }
                            out.out << dataType << " "; //... since we're starting a new type
                            currentDataType = dataType;
                            fFirstInLine = true;
                        }

                        if (!fFirstInLine)
                        {
                            out.out << ", "; // Separate subsequent variables with commas
                        }
                        out.out << _tempVars[i]->GetName();
                        if (_tempVars[i]->GetSize() > 1)
                        {
                            out.out << "[" << _tempVars[i]->GetSize() << "]"; // array
                        }
                        fFirstInLine = false;
                    }
                    out.out << ";" << out.NewLineString();
                }
            }
            FwdDebugStatementList(_code, out);
        }
        NewLine(out);
        out.out << "}";
    }
}

void ProcedureDefinition::_DebugStringExtras(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (!_class.empty())
    {
        out.out << " : " << _class;
    }
}

void ProcedureDefinition::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    DebugLine line(out);
    if (out.lang == LangSyntaxSCIStudio)
    {
        out.out << "(procedure ";
    }
    else
    {
        NewLine(out);
    }
    if (_public)
    {
        out.out << "public ";
    }
    __super::DebugString(out);
}
void MethodDefinition::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    DebugLine line(out);
    if (out.lang == LangSyntaxSCIStudio)
    {
        out.out << "(method ";
    }
    else
    {
        NewLine(out);
    }
    __super::DebugString(out);
}
void Assignment::DebugString(DebugStringOut &out) const
{
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine line(out);
        Inline inln(out, true);
        out.out << _assignment << " ";
        _variable.DebugString(out);
        out.out << " ";
        _value.DebugString(out);
    }
    else
    {
        {
            Inline inln(out, true);
            OutputBrackets brackets(out);
            _variable.DebugString(out);
            out.out << " " << _assignment << " ";
            _value.DebugString(out);
        }
        EndStatement(out);
    }
}
void BinaryOp::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine line(out);
        Inline inln(out, true);
        out.out << _operator << " ";
        _leftOperand.DebugString(out);
        out.out << " ";
        _rightOperand.DebugString(out);
    }
    else
    {
        {
            Inline inln(out, true);
            OutputBrackets brackets(out);
            {
                ExplicitOrder order(out, true);
                _leftOperand.DebugString(out);
                out.out << " " << ToCPP(_operator) << " ";
                _rightOperand.DebugString(out);
            }
        }
        EndStatement(out);
    }
}
std::string OperatorToString(const std::string &op)
{
    std::stringstream output;
    output << "operator '" << op << "'";
    return output.str();
}
std::string BinaryOp::ToString() const
{
    return OperatorToString(_operator);
}

bool IsNonAlphaOperator(const string &op)
{
    return (op == "--") || (op == "++") || (op == "!");
}
void UnaryOp::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine line(out);
        Inline inln(out, true);
        out.out << _operator;
        if (!IsNonAlphaOperator(_operator))
        {
            out.out << " ";
        }
        _operand.DebugString(out);
    }
    else
    {
        {
            Inline inln(out, true);
            OutputBrackets brackets(out);
            std::string newOp = ToCPP(_operator);
            out.out << newOp;
            if (!IsNonAlphaOperator(newOp))
            {
                out.out << " ";
            }
            // Wrap the target in (), or else we could end up with something like
            // !x - 5
            // when we want
            // !(x - 5)
            out.out << "(";
            _operand.DebugString(out);
            out.out << ")";
        }
        EndStatement(out);
    }
}
std::string UnaryOp::ToString() const
{
    return OperatorToString(_operator);
}
void LValue::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    out.out << _name;
    if (_fHasIndexer)
    {
        out.out << "[";
        Inline inln(out, true);
        _array.DebugString(out);
        out.out << "]";
    }
}

void SingleStatement::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (_pThing.get() == NULL)
    {
        out.out << "ERROR - EMPTY STATEMENT";
    }
    else
    {
        _pThing->DebugString(out);
    }
}
void CodeBlock::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine debugLine(out);
        out.out << "(";
        Inline inln(out, true);
        FwdDebugString(_code, out);
        out.out << ")";
    }
    else
    {
        /*
        if (_code.size() == 1)
        {
            // Don't bother with {}
            _code[0]->DebugString(out);
        }
        else
        {
            out.out << "{";
            {
                Inline inln(out, false);
                DebugIndent indent(out);
                NewLine(out);
                FwdDebugString(_code, out);
            }
            out.out << "}";
        }*/

        // phil -> I changed this for decompilation ... it may mess up script conversions.
        NewLine(out);
        out.out << "{";
        // Now the code, indented
        {
            DebugIndent indent(out);
            FwdDebugStatementList(_code, out);
        }
        NewLine(out);
        out.out << "}";
    }
}
void SendParam::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine debugLine(out);
        out.out << _selector;
        if (!_params.empty() || _fIsMethodCall)
        {
            out.out << "(";
            Inline inln(out, true);
            FwdDebugString(_params, out);
            out.out << ")";
        }
    }
    else
    {
        out.out << _selector;
        if (!_params.empty() || _fIsMethodCall)
        {
            out.out << "(";
            Inline inln(out, true);
            FwdDebugString(_params, out, ", ");
            out.out << ")";
        }
    }
}
void SendCall::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine debugLine(out);
        if (!_object.empty())
        {
            out.out << _object;
        }
        else if (_object2.GetType() != NodeTypeUnknown)
        {
            Inline inln(out, true);
            out.out << "send (";
            _object2.DebugString(out);
            out.out << ")";
        }
        else
        {
            out.out << "send ";
            Inline inln(out, true);
            _object3.DebugString(out);
        }
        out.out << ":";

        // Now the params.
        DebugIndent indent(out);
        FwdDebugString(_params, out);
    }
    else
    {
        SendParamVector::const_iterator it = _params.begin();
        bool fFirst = true;
        while (it != _params.end())
        {
            if (!fFirst)
            {
                NewLine(out);
            }
            fFirst = false;
            if (!_object.empty())
            {
                out.out << _object;
            }
            else if (_object2.GetType() != NodeTypeUnknown)
            {
                Inline inln(out, true);
                out.out << "(";
                _object2.DebugString(out);
                out.out << ")";
            }
            else
            {
                Inline inln(out, true);
                _object3.DebugString(out);
            }
            out.out << ".";
            {
                Inline inln(out, true);
                (*it)->DebugString(out);
            }
            ++it;
            EndStatement(out);
        }
    }
}

void ConditionalExpression::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    SingleStatementVector::const_iterator stateIt = _terms.begin();
    std::vector<bool>::const_iterator andOrIt = _andOrs.begin();
    bool fFirst = true;
    bool fLastOld = out.fLast;
    out.fLast = true; // Treat all items like "last items" - we don't want spaces around them.
    ExplicitOrder order(out, _terms.size() > 1);// Enclose terms in () if there are more than one.
    while (stateIt != _terms.end())
    {
        if (!fFirst)
        {
            if (out.lang == LangSyntaxSCIStudio)
            {
                out.out << ((*andOrIt) ? "and" : "or");
            }
            else
            {
                out.out << ((*andOrIt) ? " && " : " || ");
            }
            ++andOrIt;
        }
        (*stateIt)->DebugString(out);
        fFirst = false;
        ++stateIt;
    }
    out.fLast = fLastOld;
}
void CppIfStatement::DebugString(DebugStringOut &out) const
{
    if (_fTernary)
    {
        _condition.DebugString(out);
        out.out << " ? ";
        _if.DebugString(out);
        out.out << " : ";
        _else.DebugString(out);
    }
    else
    {
        out.out << "if (";
        {
            Inline inln(out, true);
            _condition.DebugString(out);
        }
        out.out << ")";
        //out.out << "{";
        // Now the code, indented
        {
            //DebugIndent indent(out);
            _if.DebugString(out);
        }
        NewLine(out);
        //out.out << "}";

        if (_else.GetSyntaxNode())
        {
            NewLine(out);
            out.out << "else";
            //out.out << "{";
            {
                //DebugIndent indent(out);
                _else.DebugString(out);
            }
            NewLine(out);
            //out.out << "}";
        }
    }
}
void IfStatement::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        {
            DebugLine line(out);
            out.out << "(if (";
            {
                Inline inln(out, true);
                _condition.DebugString(out);
            }
            out.out << ")";
        }
        // Now the code, indented
        {
            DebugIndent indent(out);
            for_each(_code.begin(), _code.end(), bind2nd(FwdDebugStringPtr(), out));
        }
        {
            DebugLine line(out);
            out.out << ")";
        }
    }
    else
    {
        out.out << "if (";
        {
            Inline inln(out, true);
            _condition.DebugString(out);
        }
        out.out << ")";
        NewLine(out);
        out.out << "{";
        // Now the code, indented
        {
            DebugIndent indent(out);
            FwdDebugStatementList(_code, out);
        }
        NewLine(out);
        out.out << "}";
    }
}
void ElseClause::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        {
            DebugLine line(out);
            out.out << "(else";
        }
        // Now the code, indented
        {
            DebugIndent indent(out);
            for_each(_code.begin(), _code.end(), bind2nd(FwdDebugStringPtr(), out));
        }
        {
            DebugLine line(out);
            out.out << ")";
        }
    }
    else
    {
        {
            out.out << "else";
        }
        {
            NewLine(out);
            out.out << "{";
        }
        // Now the code, indented
        {
            DebugIndent indent(out);
            FwdDebugStatementList(_code, out);
        }
        {
            NewLine(out);
            out.out << "}";
        }
    }
}
void CaseStatement::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        {
            DebugLine line(out);
            if (_fDefault)
            {
                out.out << "(default";
            }
            else
            {
                out.out << "(case";
                Inline inln(out, true);
                _case.DebugString(out);
            }
        }
        // Now the code, indented.
        {
            DebugIndent indent(out);
            for_each(_code.begin(), _code.end(), bind2nd(FwdDebugStringPtr(), out));
        }
        {
            DebugLine line(out);
            out.out << ")";
        }
    }
    else
    {
        {
            NewLine(out);
            if (_fDefault)
            {
                out.out << "default:";
            }
            else
            {
                out.out << "case ";
                Inline inln(out, true);
                _case.DebugString(out);
                out.out << ":";
            }
        }
        // Now the code, indented.
        {
            DebugIndent indent(out);
            FwdDebugStatementList(_code, out);
        }
    }
}
void SwitchStatement::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        {
            DebugLine line(out);
            out.out << "(switch (";
            Inline inln(out, true);
            _switch.DebugString(out);
            out.out << ")";
        }
        // Now the cases, indented.
        {
            DebugIndent indent(out);
            for_each(_cases.begin(), _cases.end(), bind2nd(FwdDebugStringPtr(), out));
        }
        {
            DebugLine line(out);
            out.out << ")";
        }
    }
    else
    {
        {
            out.out << "switch (";
            Inline inln(out, true);
            _switch.DebugString(out);
            out.out << ")";
        }
        {
            NewLine(out);
            out.out << "{";
        }
        // Now the cases.
        {
            for_each(_cases.begin(), _cases.end(), bind2nd(FwdDebugStringPtr(), out));
        }
        {
            NewLine(out);
            out.out << "}";
        }
    }
}
void ReturnStatement::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine line(out);
        out.out << "return ";
        Inline inln(out, true);
        if (_retValue.GetType() != NodeTypeUnknown)
        {
            _retValue.DebugString(out);
        }
    }
    else
    {
        out.out << "return";
        {
            Inline inln(out, true);
            if (_retValue.GetType() != NodeTypeUnknown)
            {
                out.out << " ";
                _retValue.DebugString(out);
            }
        }
        EndStatement(out);
    }
}
void ForLoop::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        {
            DebugLine line(out);
            out.out << "(for ";
            Inline inln(out, true);
            _initializer.DebugString(out);
            out.out << " (";
            _condition.DebugString(out);
            out.out << ") ";
            _looper.DebugString(out);
        }
        // Now the code
        {
            DebugIndent indent(out);
            for_each(_code.begin(), _code.end(), bind2nd(FwdDebugStringPtr(), out));
        }
        {
            DebugLine line(out);
            out.out << ")";
        }
    }
    else
    {
        {
            out.out << "for (";
            Inline inln(out, true);
            _initializer.DebugString(out);
            out.out << "; ";
            _condition.DebugString(out);
            out.out << "; ";
            _looper.DebugString(out);
            out.out << ") ";
        }
        NewLine(out);
        out.out << "{";
        // Now the code
        {
            DebugIndent indent(out);
            FwdDebugStatementList(_code, out);
        }
        NewLine(out);
        out.out << "}";
    }
}
void WhileLoop::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        {
            DebugLine line(out);
            out.out << "(while (";
            Inline inln(out, true);
            _condition.DebugString(out);
            out.out << ")";
        }
        // Now the code, indented.
        {
            DebugIndent indent(out);
            for_each(_code.begin(), _code.end(), bind2nd(FwdDebugStringPtr(), out));
        }
        {
            DebugLine line(out);
            out.out << ")";
        }
    }
    else
    {
        out.out << "while (";
        {
            Inline inln(out, true);
            _condition.DebugString(out);
        }
        out.out << ")";
        NewLine(out);
        out.out << "{";
        {
            DebugIndent indent(out);
            FwdDebugStatementList(_code, out);
        }
        NewLine(out);
        out.out << "}"; 
        NewLine(out);
    }
}
void DoLoop::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        {
            DebugLine line(out);
            out.out << "(do ";
        }
        // Now the code, indented.
        {
            DebugIndent indent(out);
            for_each(_doList.begin(), _doList.end(), bind2nd(FwdDebugStringPtr(), out));
        }
        {
            DebugLine line(out);
            {
                Inline inln(out, true);
                out.out << ") while (";
                _condition.DebugString(out);
                out.out << ")";
            }
        }
    }
    else
    {
        out.out << "do";
        NewLine(out);
        out.out << "{";
        {
            DebugIndent indent(out);
            FwdDebugStatementList(_doList, out);
        }
        NewLine(out);
        out.out << "} while (";
        {
            Inline inln(out, true);
            _condition.DebugString(out);
        }
        out.out << ")";
        EndStatement(out);
    }
}
void RestStatement::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine line(out);
    }
    out.out << "rest " << _param;
}
void ProcedureCall::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine line(out);
        out.out << _name << "(";
        Inline inln(out, true);
        FwdDebugString(_params, out);
        out.out << ")";
    }
    else
    {
        out.out << _name << "(";
        {
            Inline inln(out, true);
            FwdDebugString(_params, out, ", ");
            out.out << ")";
        }
        EndStatement(out);
    }
}
void BreakStatement::DebugString(DebugStringOut &out) const
{
    out.SyncComments(this);
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine line(out);
        out.out << "break";
    }
    else
    {
        out.out << "break";
        EndStatement(out);
    }
}

void Asm::DebugString(DebugStringOut &out) const
{
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine line(out);
        out.out << _instruction << " " << _arguments;
    }
    else
    {
        out.out << _instruction << " " << _arguments;
        NewLine(out);
        EndStatement(out);
    }
}

void Comment::DebugString(DebugStringOut &out) const
{
    if (out.lang == LangSyntaxSCIStudio)
    {
        DebugLine line(out);
        out.out << _comment;
    }
    else
    {
        out.out << _comment;
        EndStatement(out);
    }
}


//
// Comment tracking
//
CommentTracker::CommentTracker(Script &script) : _comments(script.GetComments()) , _commentIndex(0)
{
}

void CommentTracker::Sync(const sci::SyntaxNode *pNode, DebugStringOut &out)
{
    if (_commentIndex < _comments.size())
    {
        LineCol commentPosition = _comments[_commentIndex]->GetEndPosition();
        LineCol nodePosition = pNode->GetPosition(); // Note: can't use end position, it's not consistently set.
        while ((_commentIndex < _comments.size()) && (commentPosition < nodePosition))
        {
            out.out << _comments[_commentIndex]->GetName();
            NewLine(out);
            ++_commentIndex;
            if (_commentIndex < _comments.size())
            {
                commentPosition = _comments[_commentIndex]->GetEndPosition();
                nodePosition = pNode->GetPosition();
            }
        }
    }
}