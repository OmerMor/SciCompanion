#pragma once

#include "CompileInterfaces.h"
#include "SCO.h"

class ICompileContext;

//
//
// Contains object model for script files.
//
// Script
//      ClassDefinition[]
//          ClassProperty
//              PropertyValue
//          MethodDefinition
//              VariableDecl[]
//              SingleStatement[]
//      VariableDecl[]
//          VariableDecl
//          PropertyValue[]
//      ProcedureDefinition[]
//          VariableDecl[]
//          SingleStatement[]
//      Synonym[]
//      Define[]
//
// SingleStatement
//      CodeBlock
//          SingleStatement[]
//      SendCall
//          LValue/SingleStatement
//          SendParam[]
//      ProcedureCall
//          SingleStatement[]
//      IfStatement
//          ConditionalExpression
//          SingleStatement[]
//      ElseClause
//          CodeBlock
//      SwitchStatement
//          SingleStatement[]
//          CaseStatement[]
//              SingleStatement
//              SingleStatement[]
//      CSCIOMAsm
//      ReturnStatement
//          SingleStatement
//      Assignment
//          LValue
//          SingleStatement
//      BinaryOp
//          SingleStatement
//          SingleStatement
//      UnaryOp
//          SingleStatement
//      ForLoop
//          SingleStatement
//          ConditionalExpression
//          SingleStatement
//          SingleStatement[]
//      WhileLoop
//          ConditionalExpression
//          SingleStatement
//      DoLoop
//          CodeBlock
//          SingleStatement - ???
//      BreakStatement
//      RestStatement
//          PropertyValue
//          
//      
// ConditionExpression
//      SingleStatement[]
//      
//      

struct CodeResult
{
public:
    CodeResult(WORD wBytes, SpeciesIndex wType) : _wBytes(wBytes), _type(wType) {}
    CodeResult(WORD wBytes) : _wBytes(wBytes) {}
    CodeResult() : _wBytes(0) {}
    WORD GetBytes() const { return _wBytes; }
    SpeciesIndex GetType() const { return _type; }
private:
    WORD _wBytes;           // Number of bytes pushed onto the stack
    SpeciesIndex _type;    // Data type
};

class IOutputByteCode
{
public:
    // Generate the byte code.
    virtual CodeResult OutputByteCode(ICompileContext &context) const { return 0; }
    // A prescan, used generally for "preprocessor" substitutions, and adding items to the symbol table
    virtual void PreScan(ICompileContext &context) {}
    virtual ~IOutputByteCode() {}
};



namespace sci
{
    // fwd decl
    class Script;

    class ScriptSite
    {
    public:
        const Script *GetOwnerScript() const { return _pScript; }
        Script *GetOwnerScript() { return _pScript; }
        void SetScript(Script *pScript) { _pScript = pScript; }
    protected:
        ScriptSite();
        ScriptSite(const ScriptSite &src);
        ScriptSite& operator=(const ScriptSite& src);
    private:
        Script *_pScript;
    };

    enum ValueType
    {
        ValueTypeString =           0x00000000,
        ValueTypeSaid =             0x00000001,
        ValueTypeNumber =           0x00000002,
        ValueTypeToken =            0x00000003,
        ValueTypeSelector =         0x00000004,
        ValueTypePointer =          0x00000005,
        ValueTypeNone =             0x00000006,
        ValueTypeResourceString =   0x00000007, // Not available in SCIStudio syntax
    };

    //
    // Various things that might be interesting to outsiders.
    //
    enum NodeType
    {
        NodeTypeUnknown,
        NodeTypeValue,
        NodeTypeComplexValue,
        NodeTypeAssignment,
        NodeTypeVariableDeclaration,
        NodeTypeDefine,
        NodeTypeScriptVariable,
        NodeTypeClassProperty,
        NodeTypeStatement,
        NodeTypeFunction,
        NodeTypeClassDefinition,
        NodeTypeSynonym,
        NodeTypeCodeBlock,
        NodeTypeSendParam,
        NodeTypeLValue,
        NodeTypeSendCall,
        NodeTypeProcedureCall,
        NodeTypeConditionalExpression,
        NodeTypeIf,
        NodeTypeElse,
        NodeTypeCase,
        NodeTypeSwitch,
        NodeTypeAsm,
        NodeTypeReturn,
        NodeTypeBinaryOperation,
        NodeTypeUnaryOperation,
        NodeTypeForLoop,
        NodeTypeWhileLoop,
        NodeTypeDoLoop,
        NodeTypeBreak,
        NodeTypeRest,
        NodeTypeScript,
        NodeTypeComment,
        NodeTypeFunctionParameter,
        NodeTypeFunctionSignature,
        NodeTypeCppIf,
        NodeTypeCast,
    };

    class Comment;
    class SyntaxNode;
    class Script;
    struct DebugStringOut;
    class CommentTracker
    {
    public:
        CommentTracker(Script &script);
        void Sync(const sci::SyntaxNode *pNode, DebugStringOut &out);
    private:
        size_t _commentIndex;
        const std::vector<sci::Comment*> &_comments;
    };

    //
    // Used for generating a text representation of a script from its SyntaxNodes
    //
    struct DebugStringOut
    {
        DebugStringOut(std::stringstream &ss, LangSyntax syntax, CommentTracker *pCommentsOut = NULL) : pComments(pCommentsOut), out(ss), lang(syntax),
            iIndent(0), fInline(false), fLast(false), fUseBrackets(false), pszNewLine("\n")
        { }
        void SyncComments(const sci::SyntaxNode *pNode)
        {
            if (pComments)
            {
                pComments->Sync(pNode, *this);
            }
        }
        const char *NewLineString() const
        {
            return pszNewLine;
        }
        LangSyntax lang;
        std::stringstream &out;
        int iIndent;
        bool fInline;
        bool fLast;
        bool fUseBrackets;
        CommentTracker *pComments;
        const char *pszNewLine;
    };

    class SyntaxNode;

    class IExploreNodeContext
    {
    };

    class IExploreNode
    {
    public:
        virtual void ExploreNode(IExploreNodeContext *pContext, SyntaxNode &node) = 0;
    };

    //
    // Base class for all script objects
    //
    class SyntaxNode : public ISourceCodePosition, public IOutputByteCode
    {
    public:
        SyntaxNode() {}
        SyntaxNode(const SyntaxNode &src) : ISourceCodePosition(src) {}
        virtual ~SyntaxNode() {}
        virtual NodeType GetNodeType() const = 0;

        // Outputs the syntax tree as code
        virtual void DebugString(DebugStringOut &out) const {  }

        // A simple string describing the node, for error reporting.
        virtual std::string ToString() const { return ""; }

        virtual void Traverse(IExploreNodeContext *pContext, IExploreNode &en) {}

        // Determines whether or not one syntax node is equal to another.
        // Used for comparing two generated trees to make sure they are the same.
        virtual bool Equal(const SyntaxNode &right) const { return false; }
    };

    //
    // This allows syntax nodes to support both the virtual GetNodeType method, and have a
    // static MyNodeType member.  Thus, give a class name e.g. SendParam, we can verify that
    // a particular SyntaxNode is of that side
    //
#define DECLARE_NODE_TYPE(type)\
    public: \
    const static NodeType MyNodeType = type; \
    virtual NodeType GetNodeType() const { return type; }

    template <typename _T>
    _T SafeSyntaxNode(const SyntaxNode *pNode)
    {
        if (pNode->GetNodeType() == _T->MyNodeType)
        {
            return static_cast<_T>(pNode);
        }
        else
        {
            return NULL;
        }
    }

    //
    // Composition class used for script objects that have names.
    //
    class NamedNode
    {
    public:
        void SetName(const std::string &name) { _innerName = name; }
        std::string GetName() const { return _innerName; }
    protected:
        NamedNode(std::string &name) : _innerName(name) {}
    private:
        std::string &_innerName;
    };

    //
    // Composition class used for script objects that have an explicit data type.
    //
    class TypedNode
    {
    public:
        void SetDataType(const std::string &type) { ASSERT(_innerType.empty()); _innerType = type; }
        std::string GetDataType() const { return _innerType; }
    protected:
        TypedNode(std::string &type) : _innerType(type) {}
    private:
        std::string &_innerType;
    };

    class SingleStatement; // fwd decl

    class PropertyValueBase : public SyntaxNode
    {
    public:
        PropertyValueBase(const PropertyValueBase& src);
        PropertyValueBase& operator=(const PropertyValueBase& src);
        bool operator==(const PropertyValueBase& value2) const;
        bool operator!=(const PropertyValueBase& value);
        void ToString(CString &str) const;
        std::string ToString() const;
        void SetValue(WORD wValue, bool fHexIn = false) { _numberValue = wValue; _type = sci::ValueTypeNumber; _fHex = fHexIn; _fNegate = false; _stringValue.clear(); } // Used for compiled scripts
        void SetValue(int iValue, IntegerFlags flags);
        void SetStringValue(const std::string &value) { _stringValue = value; _type = sci::ValueTypeString; }
        void SetValue(const std::string &value, ValueType type) { _stringValue = value; _type = type; }
        WORD GetNumberValue() const { return _numberValue; }
        std::string GetStringValue() const { return _stringValue; }
        bool IsEmpty() { return (_type == sci::ValueTypeNone); }
        void Negate() { _fNegate = true; }
        ValueType GetType() const { return _type; }
        void SetType(ValueType type) { ASSERT(_type == sci::ValueTypeNone); _type = type; }
        void Zero() { _type = sci::ValueTypeNumber; _numberValue = 0; _fHex = false; _fNegate = false; _stringValue.clear(); }
        bool IsZero() const { return (_type == sci::ValueTypeNumber) && (_numberValue == 0); }

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);

        virtual void DebugString(DebugStringOut &out) const;
        virtual bool Equal(const SyntaxNode &right) const;
    protected:
        virtual SingleStatement *GetIndexer() const { return NULL; }
        // No one should construct this directly.
        PropertyValueBase()
        {
            _type = ValueTypeNone; _numberValue = 0; _fHex = false; _fNegate = false; 
        }

    private:
        ValueType _type;      // Can be of 3 types
        WORD _numberValue;
        std::string _stringValue;
        bool _fHex;     // Indicates that _numberValue was expressed in hex in the script.
        bool _fNegate;  // Indicates that _numberValue was the result of a negation.  _numberValue doesn't change.
    };

    class PropertyValue : public PropertyValueBase
    {
        DECLARE_NODE_TYPE(NodeTypeValue)
    };

    // PropertyValues are always passed by value
    typedef std::vector<PropertyValue> PropertyValueVector;

    //
    // Different from a regular property value, because it can have an array indexer.
    //
    class ComplexPropertyValue : public PropertyValueBase
    {
        DECLARE_NODE_TYPE(NodeTypeComplexValue)
    public:
        ComplexPropertyValue() : PropertyValueBase() { _pArrayInternal = NULL; }
        ComplexPropertyValue(ComplexPropertyValue& src);
        ComplexPropertyValue& operator=(ComplexPropertyValue& src);
        ~ComplexPropertyValue();
        void SetIndexer(SingleStatement *pIndexer) { ASSERT(_pArrayInternal == NULL); _pArrayInternal = pIndexer; }
        void SetIndexer(std::auto_ptr<SingleStatement> pIndexer) { ASSERT(_pArrayInternal == NULL); _pArrayInternal = pIndexer.get(); pIndexer.release();  }
        virtual SingleStatement *GetIndexer() const { return _pArrayInternal; }
        virtual void DebugString(DebugStringOut &out) const;
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
    private:
        SingleStatement *_pArrayInternal;
    };

    class Define : public SyntaxNode, public ScriptSite
    {
        DECLARE_NODE_TYPE(NodeTypeDefine)
    public:
        const std::string &GetLabel() const { return _label; }
        const std::string &GetName() const { return _label; }
        WORD GetValue() const { ASSERT(_strValue.empty()); return _wValue; }
        bool Match(const std::string &label) { return label == _label; }

        void SetLabel(const std::string &label) { _label = label; }
        void SetValue(WORD w) { _wValue = w; }
        void SetValue(int iNumber, IntegerFlags flags) { ASSERT(iNumber <= 0xFFFF); _wValue = static_cast<WORD>(iNumber); _flags = flags; }
        void SetValue(const std::string &value) { _strValue = value; };
        virtual void DebugString(DebugStringOut &out) const;
        virtual bool Equal(const SyntaxNode &right) const;

        // IOutputByteCode
        void PreScan(ICompileContext &context);

    private:
        std::string _label;

        // Either _strValue is non-empty, or _wValue is valid.
        WORD _wValue;
        IntegerFlags _flags;
        std::string _strValue;
    };

    typedef Define* DefinePtr;
    typedef std::vector<DefinePtr> DefineVector;

    class ClassProperty : public SyntaxNode, public NamedNode, public TypedNode
    {
        DECLARE_NODE_TYPE(NodeTypeClassProperty)
    public:
        ClassProperty() : NamedNode(_name), TypedNode(_type) {};
        ClassProperty(const std::string &str, WORD wValue);
        ClassProperty(const std::string &str, const std::string &value);
        const PropertyValue &GetValue() const { return _value; }
        void SetValue(PropertyValue value) { _value = value; }

        // IOutputByteCode
        void PreScan(ICompileContext &context);
        virtual void DebugString(DebugStringOut &out) const;
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

    private:
        PropertyValue _value;
        std::string _name;
        std::string _type;
    };

    typedef ClassProperty* ClassPropertyPtr;
    typedef std::vector<ClassPropertyPtr> ClassPropertyVector;

} // namespace sci


//
// Generic way for clients to interact with something that has a name and
// some properties.
//
class ISCIPropertyBag
{
public:
    // Implement these
    virtual bool SetProperty(PCTSTR pszName, sci::PropertyValue value) = 0;
    virtual bool GetProperty(PCTSTR pszName, sci::PropertyValue &value) = 0;
    virtual void SetBagName(PCTSTR pszName) = 0;
    virtual const std::string GetBagName() = 0;
    virtual void SetSpecies(PCTSTR pszName) = 0;
    virtual const std::string GetSpecies() const = 0;
    virtual const sci::ClassPropertyVector &GetProperties() = 0;
};

class ISCIPropertyChange
{
public:
    virtual void OnPropertyChange(ISCIPropertyBag *pSource, PCTSTR pszName, sci::PropertyValue value) = 0;
    virtual void OnNameChange(ISCIPropertyBag *pSource, PCTSTR pszName) = 0;
    virtual void OnSpeciesChange(ISCIPropertyBag *pSource, PCTSTR pszSpecies) = 0;
    virtual void OnAddProperty(ISCIPropertyBag *pSource, PCTSTR pszName) = 0;
    virtual void OnRemoveProperty(ISCIPropertyBag *pSource, PCTSTR pszName) = 0;
};


namespace sci
{

    //
    // Code segments
    //
    class SingleStatement : public SyntaxNode
    {
        DECLARE_NODE_TYPE(NodeTypeStatement)
    public:
        SingleStatement() { _type = NodeTypeUnknown; }
        SingleStatement(SingleStatement &src);
        SingleStatement& operator=(SingleStatement& src);
        ~SingleStatement();

        const SyntaxNode *GetSegment() const { return _pThing.get(); }
        SyntaxNode *GetSegment() { return _pThing.get(); }
        NodeType GetType() const { return _type; }
        bool IsIf() const { return _ContainsStatement(NodeTypeIf); }
        bool IsElse() const { return _ContainsStatement(NodeTypeElse); }

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

        template<typename _statementT>
        const typename _statementT *CastSyntaxNode() const
        {
            if (_statementT::MyNodeType == _type)
            {
                return static_cast<const _statementT*>(_pThing.get());
            }
            else
            {
                return NULL;
            }
        }
        template<typename _statementT>
        typename _statementT *CastSyntaxNode()
        {
            if (_statementT::MyNodeType == _type)
            {
                return static_cast<_statementT*>(_pThing.get());
            }
            else
            {
                return NULL;
            }
        }

        void SetSyntaxNode(SyntaxNode *pThing) { _pThing.reset(pThing); _type = pThing->GetNodeType(); }
        void SetSyntaxNode(std::auto_ptr<SyntaxNode> pThing) { _pThing = pThing; _type = _pThing->GetNodeType(); }
        const SyntaxNode *GetSyntaxNode() const { return _pThing.get(); }
        SyntaxNode *GetSyntaxNode() { return _pThing.get(); }
        SyntaxNode *ReleaseSyntaxNode() { return _pThing.release(); }
        void DebugString(DebugStringOut &out) const;
        bool Equal(const SyntaxNode &right) const;
    private:
        bool _ContainsStatement(NodeType) const;
        std::auto_ptr<SyntaxNode> _pThing;
        NodeType _type;
    };

    typedef SingleStatement* SingleStatementPtr;
    typedef std::vector<SingleStatementPtr> SingleStatementVector;

    //
    // Composition class for nodes that have a series of statements
    // These help objects adhere to a generic "API" so that template functions that
    //  construct these objects can be used in parsing.
    //
    class StatementsNode
    {
    public:
        void AddStatement(SingleStatementPtr pStatement) { _segments.push_back(pStatement); }
        void AddStatement(std::auto_ptr<SingleStatement> pStatement) { _segments.push_back(pStatement.get()); pStatement.release(); }
    protected:
        StatementsNode(SingleStatementVector &segments) : _segments(segments) {}
    private:
        SingleStatementVector &_segments;
    };

    //
    // Composition class for nodes that have an statement.
    //
    class OneStatementNode
    {
    public:
        void SetStatement1(SingleStatement &statement) { _statement1 = statement; }
    protected:
        OneStatementNode(SingleStatement &statement) : _statement1(statement) {}
    private:
        SingleStatement &_statement1;
    };

    //
    // Composition class for nodes that have a second statement.
    //
    class TwoStatementNode
    {
    public:
        void SetStatement2(SingleStatement &statement) { _statement2 = statement; }
    protected:
        TwoStatementNode(SingleStatement &statement) : _statement2(statement) {}
    private:
        SingleStatement &_statement2;
    };

    //
    // Represents a variable decl that could be an array (includes name and size)
    //
    class VariableDecl : public SyntaxNode, public StatementsNode, public TypedNode, public ScriptSite
    {
        DECLARE_NODE_TYPE(NodeTypeVariableDeclaration)
    public:
        VariableDecl();
        VariableDecl(VariableDecl &src);
        VariableDecl& operator=(VariableDecl& src);
        ~VariableDecl();

        const std::string &GetName() const { return _name; }
        WORD GetSize() const { return _wSize; }
        const SingleStatementVector &GetInitializers() const { return _initializers; }

        void AddSimpleInitializer(const PropertyValue &value);
        std::vector<WORD> GetSimpleValues() const;

        void SetName(const std::string &name) { _name = name; }
        void SetSize(WORD wSize) { _wSize = wSize; }

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);

        virtual void DebugString(DebugStringOut &out) const;
        virtual bool Equal(const SyntaxNode &right) const;

    private:
        std::string _name;
        WORD _wSize;
        std::string _type;
        SingleStatementVector _initializers;
    };

    typedef VariableDecl* VariableDeclPtr;
    typedef std::vector<VariableDecl*> VariableDeclVector;

    class ClassDefinition; // fwd decl

    class FunctionParameter : public SyntaxNode, public NamedNode, public TypedNode
    {
        DECLARE_NODE_TYPE(NodeTypeFunctionParameter)
    public:
        FunctionParameter();
        FunctionParameter(const std::string &str);
        FunctionParameter(FunctionParameter &src);
        FunctionParameter& operator=(FunctionParameter& src);

    private:
        std::string _name;
        std::string _type;
    };
    typedef FunctionParameter* FunctionParameterPtr;
    typedef std::vector<FunctionParameterPtr> FunctionParameterVector;

    class FunctionSignature : public SyntaxNode, public ScriptSite, public IVariableLookupContext, public TypedNode
    {
        DECLARE_NODE_TYPE(NodeTypeFunctionSignature)
    public:
        FunctionSignature();
        FunctionSignature(FunctionSignature &src);
        FunctionSignature& operator=(FunctionSignature& src);
        ~FunctionSignature();

        const FunctionParameterVector &GetParams() const { return _params; }
        FunctionParameterVector &GetParams() { return _params; }
        void AddParam(const std::string &name);
        void AddParam(FunctionParameter *pParam, bool fOptional);
        void AddParam(std::auto_ptr<FunctionParameter> pParam, bool fOptional);
        void SetMoreParametersAllowed(bool fOpt) { _fMoreParameters = fOpt; }
        bool GetMoreParametersAllowed() const { return _fMoreParameters; }
        size_t GetRequiredParameterCount() const { return (_iOptional == NoOptional) ? _params.size() : _iOptional; }

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const { ASSERT(FALSE); return CodeResult(); }
        void PreScan(ICompileContext &context);

        // IVariableLookupContext
        TOKENTYPEFOO LookupVariableName(ICompileContext &context, const std::string &str, WORD &wIndex, SpeciesIndex &dataType) const;

    private:
        static const size_t NoOptional = ((size_t)-1);
        FunctionParameterVector _params;
        bool _fMoreParameters;              // Does this have more parameters? (relevant to CPP only)
        size_t _iOptional;                  // Index at which the parameters become optional
        std::string _type;                      // Data type
    };
    typedef FunctionSignature* FunctionSignaturePtr;
    typedef std::vector<FunctionSignaturePtr> FunctionSignatureVector;

    class FunctionBase : public SyntaxNode, public ScriptSite, public IVariableLookupContext, public NamedNode, public StatementsNode
    {
        DECLARE_NODE_TYPE(NodeTypeFunction)
    public:
        FunctionBase();
        FunctionBase(const std::string &str);
        FunctionBase(FunctionBase &src);
        FunctionBase& operator=(FunctionBase& src);
        ~FunctionBase();
        const FunctionSignatureVector &GetSignatures() const { return _signatures; }
        FunctionSignatureVector &GetSignaturesNC() { return _signatures; }
        void AddSignature(FunctionSignaturePtr pSig) { _signatures.push_back(pSig); }
        void AddSignature(std::auto_ptr<FunctionSignature> pSig) { AddSignature(pSig.get()); pSig.release(); }
        void AddVariable(VariableDeclPtr pVar, PropertyValue value);
        void AddVariable(std::auto_ptr<VariableDecl> pVar) { _tempVars.push_back(pVar.get()); pVar.release(); }
        std::string ToString() const;
        const VariableDeclVector &GetVariables() const { return _tempVars; }
        const ClassDefinition *GetOwnerClass() const { return _pOwnerClass; }
        void SetOwnerClass(const ClassDefinition *pOwnerClass) { _pOwnerClass = pOwnerClass; }
        const SingleStatementVector &GetCodeSegments() const { return _code; }

        void AddParam(const std::string &param);

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

        // IVariableLookupContext
        TOKENTYPEFOO LookupVariableName(ICompileContext &context, const std::string &str, WORD &wIndex, SpeciesIndex &dataType) const;
        virtual void DebugString(DebugStringOut &out) const;
        virtual bool Equal(const SyntaxNode &right) const;

    protected:
        virtual void _DebugStringExtras(DebugStringOut &out) const {}; // A little hack.

    private:
        FunctionSignatureVector _signatures;
        VariableDeclVector _tempVars;           // Temp variables
        const ClassDefinition *_pOwnerClass;    // Our owner class (weak ref)
        std::string _name;                      // Function name
        SingleStatementVector _code;            // The code in our function

        // Documentation
        std::string _docDescription;            // Description of the function
    };

    typedef FunctionBase* FunctionPtr;
    typedef std::vector<FunctionPtr> FunctionVector;

    class MethodDefinition : public FunctionBase
    {
    public:
        MethodDefinition();
        MethodDefinition(MethodDefinition &src);
        MethodDefinition& operator=(MethodDefinition& src);

        void SetPrivate(bool f) { _fPrivate = f; }
        bool SetPrivate() const { return _fPrivate; }
        virtual void DebugString(DebugStringOut &out) const;

    private:
        bool _fPrivate;
    };

    typedef MethodDefinition* MethodPtr;
    typedef std::vector<MethodPtr> MethodVector;

    class ClassDefinition; // fwd decl
    class ProcedureDefinition : public FunctionBase
    {
    public:
        ProcedureDefinition();
        ProcedureDefinition(ProcedureDefinition &src);
        ProcedureDefinition& operator=(ProcedureDefinition& src);
        bool IsPublic() const { return _public; }
        void SetPublic(bool f) { _public = f; }
        const std::string &GetClass() { return _class; }
        void SetClass(const std::string className) { _class = className; }

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        virtual void DebugString(DebugStringOut &out) const;
        virtual bool Equal(const SyntaxNode &right) const;
    private:
        void _DebugStringExtras(DebugStringOut &out) const;

        bool _public;
        std::string _class;                 // for class procedures.
    };

    typedef ProcedureDefinition* ProcedurePtr;
    typedef std::vector<ProcedurePtr> ProcedureVector;

    struct SelectorValuePair
    {
        WORD wSelector;
        WORD wValue;
    };

    class ClassDefinition : public SyntaxNode, public ScriptSite, public ISCIPropertyBag, public IVariableLookupContext, public NamedNode
    {
        DECLARE_NODE_TYPE(NodeTypeClassDefinition)
    public:
        ClassDefinition();
        ~ClassDefinition();
        void CreateNew(const Script *pScript, PCTSTR pszName, PCTSTR pszSpecies);

        // Methods out of which to extract data.
        std::string GetSuperClass() const { return _superClass; }
        bool IsInstance() const { return _fInstance; }
        bool IsPublic() const { return _fPublic; }
        const ClassPropertyVector &GetProperties() const { return _properties; }
        ClassPropertyVector &GetPropertiesNC() { return _properties; }
        const MethodVector &GetMethods() const { return _methods; }
        MethodVector &GetMethodsNC() { return _methods; }

        bool GetPropertyConst(PCTSTR pszName, PropertyValue &value) const;

        // ISCIPropertyBag
        bool SetProperty(PCTSTR pszName, PropertyValue value);
        bool GetProperty(PCTSTR pszName, PropertyValue &value);
        void SetBagName(PCTSTR pszName) { _name = pszName; }
        const std::string GetBagName() { return _name; }
        void SetSpecies(PCTSTR pszName) { _superClass = pszName; }
        const std::string GetSpecies() const { return _superClass; }
        const ClassPropertyVector &GetProperties() { return _properties; }

        void SetSuperClass(const std::string &superClass) { _superClass = superClass; }
        void SetPublic(bool fPublic) { _fPublic = fPublic; }
        void SetInstance(bool fInstance) { _fInstance = fInstance; }
        void AddProperty(ClassPropertyPtr classProp) { _properties.push_back(classProp); }
        void AddProperty(std::auto_ptr<ClassProperty> pClassProp) { AddProperty(pClassProp.get()); pClassProp.release(); }
        void AddMethod(MethodPtr method) { _methods.push_back(method); }
        void AddMethod(std::auto_ptr<MethodDefinition> pMethod) { AddMethod(pMethod.get()); pMethod.release(); }

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

        // IVariableLookupContext
        virtual TOKENTYPEFOO LookupVariableName(ICompileContext &context, const std::string &str, WORD &wIndex, SpeciesIndex &dataType) const;

        virtual void DebugString(DebugStringOut &out) const;

    private:
        // Disable copy/assignment, since other objects hold weak references to us.
        ClassDefinition(ClassDefinition &src);
        ClassDefinition& operator=(ClassDefinition& src);

        std::string _superClass;
        bool _fPublic;
        bool _fInstance; // Instance or class.
        ClassPropertyVector _properties;
        MethodVector _methods;
        std::string _name;
    };

    typedef ClassDefinition* ClassPtr;
    typedef std::vector<ClassPtr> ClassVector;

    class Synonym : public SyntaxNode
    {
        DECLARE_NODE_TYPE(NodeTypeSynonym)
    public:
        virtual bool Equal(const SyntaxNode &right) const;
        std::string MainWord;
        std::string Replacement;
    };

    typedef Synonym* SynonymPtr;
    typedef std::vector<SynonymPtr> SynonymVector;



    //
    // Series of code segments, surrounded by ()
    //
    class CodeBlock : public SyntaxNode, public StatementsNode
    {
        DECLARE_NODE_TYPE(NodeTypeCodeBlock)
    public:
        CodeBlock();
        CodeBlock(CodeBlock &src);
        CodeBlock& operator=(CodeBlock& src);
        ~CodeBlock();
        const SingleStatementVector &GetList() const { return _code; }
        virtual void DebugString(DebugStringOut &out) const;

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

        template<typename _T>
        _T *ReduceBlock()
        {
            _T *pReduced = NULL;
            if ((_code.size() == 1) && (_code[0]->CastSyntaxNode<_T>()))
            {
                pReduced = static_cast<_T*>(_code[0]->ReleaseSyntaxNode());
            }
            return pReduced;
        }

    private:
        SingleStatementVector _code;
    };

    //
    // Helper for nodes with code blocks
    //
    class CodeBlockNode
    {
    public:
        void SetCodeBlock(CodeBlock &list) { _list = list; }
    protected:
        CodeBlockNode(CodeBlock &list) : _list(list) {}
    private:
        CodeBlock &_list;
    };

    //
    // Expression with and/or that can appear in while/if/for/do statements
    //
    class ConditionalExpression : public SyntaxNode, public StatementsNode
    {
        DECLARE_NODE_TYPE(NodeTypeConditionalExpression)
    public:
        ConditionalExpression() : StatementsNode(_terms) {};
        ConditionalExpression(ConditionalExpression &src);
        ConditionalExpression& operator=(ConditionalExpression& src);
        ~ConditionalExpression();

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;
        void AddAndOr(bool fAnd) { _andOrs.push_back(fAnd); }
    private:
        std::vector<bool> _andOrs;   // true for and, false for or
        SingleStatementVector _terms;
    };

    //
    // Helper class for things with conditional expressions
    //
    class ConditionNode
    {
    public:
        void SetCondition(ConditionalExpression &cond) { _innerCondition = cond; }
    protected:
        ConditionNode(ConditionalExpression &condition) : _innerCondition(condition) { }
    private:
        ConditionalExpression &_innerCondition;
    };

    //
    // This represents a comment.
    // Currently, it can be used just as any other syntax node, OR
    // it can be added specially to the comment list for the script.
    //
    class Comment : public SyntaxNode, public NamedNode
    {
        DECLARE_NODE_TYPE(NodeTypeComment)
    public:
        Comment() : NamedNode(_comment) {}

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const { return CodeResult(); }
        void PreScan(ICompileContext &context) {}
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en) {}
        virtual void DebugString(DebugStringOut &out) const;
        std::string GetSanitizedText() const;

        bool IsMultiline() const;
    private:
        std::string _comment;
    };
    typedef Comment* CommentPtr;
    typedef std::vector<CommentPtr> CommentVector;

    //
    // This represents an entire script
    //
    class Script : public SyntaxNode, public IVariableLookupContext
    {
        DECLARE_NODE_TYPE(NodeTypeScript)
    public:
        Script(PCTSTR pszFilePath, PCTSTR pszFileName);
        Script(ScriptId script);
        Script();
        ~Script();

        // Methods to retrieve information from a Loaded script:
        const ClassVector &GetClasses() const { return _classes; }
        const ProcedureVector &GetProcedures() const { return _procedures; }
        ClassVector &GetClassesNC() { return _classes; }
        ProcedureVector &GetProceduresNC() { return _procedures; }
        WORD GetScriptNumber() const { return _scriptId.GetResourceNumber(); }
        void SetScriptNumber(WORD wNumber);
        void SetScriptNumberDefine(const std::string &define) { _scriptDefine = define; }
        const std::string &GetScriptNumberDefine() const { return _scriptDefine; }
        std::string GetTitle() const { return _scriptId.GetTitle(); }
        std::string GetName() const { return _scriptId.GetFileName(); }
        std::string GetPath() const { return _scriptId.GetFullPath(); }
        const VariableDeclVector &GetScriptVariables() const { return _scriptVariables; }
        const DefineVector &GetDefines() const { return _defines; }
        const SynonymVector &GetSynonyms() const { return _synonyms; }
        const CommentVector &GetComments() const { return _comments; }
        const std::vector<std::string> &GetUses() const { return _uses; }
        const std::vector<std::string> &GetIncludes() const { return _includes; }

        // IVariableLookupContext
        TOKENTYPEFOO LookupVariableName(ICompileContext &context, const std::string &str, WORD &wIndex, SpeciesIndex &dataType) const;

        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

        void AddUse(const std::string &use) { _uses.push_back(use); }
        void AddInclude(const std::string &include) { _includes.push_back(include); }
        void AddSynonym(SynonymPtr synonym) { _synonyms.push_back(synonym); }
        void AddSynonym(std::auto_ptr<Synonym> pSynonym) { AddSynonym(pSynonym.get()); pSynonym.release(); }
        void AddDefine(DefinePtr define) { _defines.push_back(define); }
        void AddDefine(std::auto_ptr<Define> pDefine) { AddDefine(pDefine.get()); pDefine.release(); }
        void AddClass(ClassPtr theClass) { _classes.push_back(theClass); }
        void AddClass(std::auto_ptr<ClassDefinition> pClass) { AddClass(pClass.get()); pClass.release(); }
        void AddVariable(VariableDeclPtr var) { _scriptVariables.push_back(var); }
        void AddVariable(std::auto_ptr<VariableDecl> pVar) { AddVariable(pVar.get()); pVar.release(); }
        void AddProcedure(ProcedurePtr proc) { _procedures.push_back(proc); }
        void AddProcedure(std::auto_ptr<ProcedureDefinition> pProc) { AddProcedure(pProc.get()); pProc.release(); }
        void AddComment(CommentPtr comment) { _comments.push_back(comment); }
        void AddComment(std::auto_ptr<Comment> pComment) { AddComment(pComment.get()); pComment.release(); }

        void SetScriptId(ScriptId scriptId) { _scriptId = scriptId; }
        LangSyntax Language() const { return _scriptId.Language(); }
        bool IsHeader() const { return _scriptId.IsHeader(); }

        virtual void DebugString(DebugStringOut &out) const;
        virtual bool Equal(const SyntaxNode &right) const;

        ScriptId GetScriptId() const { return _scriptId; }

    private:
        // Disable copy/assignment, since other classes hold weak references to us.
        Script(Script &src) {}
        Script& operator=(Script& src) { return *this; }

        // Script number
        std::string _scriptDefine; // This is used if _scriptId.GetResourceNumber() is InvalidResourceNumber, but it needs to use the classbrowser to resolve.

        std::vector<std::string> _uses;
        std::vector<std::string> _includes;
        VariableDeclVector _scriptVariables;
        ClassVector _classes;
        ProcedureVector _procedures;
        SynonymVector _synonyms;
        DefineVector _defines;

        // Since comments can be anywhere in the script, including the middle of statements,
        // we don't generally store comments as distinct nodes (though it is supported).
        // Instead, comments are added at the script level.  The position property of the comments
        // can be used to place it in the right spot.
        CommentVector _comments;

        // These are not serialized:
        ScriptId _scriptId;
        LangSyntax _language;
    };

}; // namespace sci	

//
// Generate a unique name for a method on an object
//
std::string GetMethodTrackingName(const sci::ClassDefinition *pClass, const sci::FunctionBase *pMethod);

SpeciesIndex MatchParameters(const std::string &name,
                             ICompileContext &context,
                             const ISourceCodePosition *pPos,
                             const std::vector<CSCOFunctionSignature> &signatures, 
                             const std::vector<SpeciesIndex> &parameterTypes, 
                             const sci::SingleStatementVector &parameterStatements);
