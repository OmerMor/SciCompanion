#pragma once

#include "ScriptOM.h"

//
// This file contains less commonly used script objects that aren't
// directly referenced by any other common script objects
// (speeds up compile time for those that include "ScriptOM.h")
//

namespace sci
{

    //
    // A single send param.
    // 
    class SendParam : public SyntaxNode, public StatementsNode, public NamedNode//, public CodeBlockNode
    {
        DECLARE_NODE_TYPE(NodeTypeSendParam)
    public:
        SendParam() : StatementsNode(_params), NamedNode(_selector)/*, CodeBlockNode(_selectorBlock)*/ { _fIsMethodCall = false; }
        SendParam(SendParam &src) : StatementsNode(_params), NamedNode(_selector)/*, CodeBlockNode(_selectorBlock)*/, SyntaxNode(src)
        {
            _params.swap(src._params);
            _fIsMethodCall = src._fIsMethodCall;
        }
        SendParam& operator=(SendParam& src)
        {
            if (this != &src)
            {
                _params.swap(src._params);
                _fIsMethodCall = src._fIsMethodCall;
                _selector = src._selector;
                //_selectorBlock = src._selectorBlock;
            }
            return *this;
        }
        ~SendParam();
        bool HasValue() const { return !_params.empty(); }
        std::string GetSelectorName() const { return _selector; }
        const SingleStatementVector &GetSelectorParams() const { return _params; }
        virtual void DebugString(DebugStringOut &out) const;
        void SetIsMethod(bool fMethod) { _fIsMethodCall = fMethod; }
        bool IsMethod() const { return _fIsMethodCall; }

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

    private:
        std::string _selector; // Alias for _name
        SingleStatementVector _params;
        bool _fIsMethodCall;
    };
    typedef std::vector<SendParam*> SendParamVector;

    //
    // A variable that appears in code (name and possibly array indexer)
    //
    class LValue : public SyntaxNode, public NamedNode
    {
        DECLARE_NODE_TYPE(NodeTypeLValue)
    public:
        LValue() : NamedNode(_name) { _fHasIndexer = false; }
        LValue(LValue &src) : NamedNode(_name)
        {
            _fHasIndexer = src._fHasIndexer;
            _array = src._array;
            _name = src._name;
        }
        LValue& operator=(LValue& src)
        {
            if (this != &src)
            {
                _fHasIndexer = src._fHasIndexer;
                _array = src._array;
                _name = src._name;
            }
            return *this;
        }
        const SingleStatement &GetIndexer() const { return _array; }
        bool HasIndexer() const { return _fHasIndexer; }

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

        void SetIndexer(SingleStatement &indexer) { _array = indexer; _fHasIndexer = true; }
        virtual void DebugString(DebugStringOut &out) const;
    private:
        bool _fHasIndexer;
        SingleStatement _array; // Alias for statement1
        std::string _name;
    };

    //
    // The rest keyword, which takes as a parameter a method parameter
    //
    class RestStatement : public SyntaxNode, public NamedNode
    {
        DECLARE_NODE_TYPE(NodeTypeRest)
    public:
        RestStatement() : NamedNode(_param) {}
        RestStatement(RestStatement &src);
        RestStatement& operator=(RestStatement& src);

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        virtual void DebugString(DebugStringOut &out) const;
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

    private:
        std::string _param; // Alias for _name
    };

    //
    // Represents a casting operation
    // 
    class Cast : public SyntaxNode, public OneStatementNode, public TypedNode
    {
        DECLARE_NODE_TYPE(NodeTypeCast)
    public:
        Cast() : OneStatementNode(_object), TypedNode(_type) {};
        Cast(Cast &src);
        Cast& operator=(Cast &src);

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context) { _object.PreScan(context); }
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

    private:
        SingleStatement _object;    // alias for OneStatementNode
        std::string _type;          // alias for TypedNode
    };

    //
    // A send call e.g. (send gEgo:x)
    //
    class SendCall : public SyntaxNode, public OneStatementNode, public NamedNode
    {
        DECLARE_NODE_TYPE(NodeTypeSendCall)
    public:
        SendCall() : OneStatementNode(_object2), NamedNode(_object), _fRest(false) {};
        SendCall(SendCall &src);
        SendCall& operator=(SendCall& src);
        ~SendCall();
        //const LValue &GetObject() const { return _object; }
        std::string GetObject() const { return _object.empty() ? _object3.GetName() : _object; }
        const SendParamVector &GetParams() const { return _params; }
        void TransferParamsFrom(SendCall *pSend);
        void SimplifySendObject();

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

        void SetLValue(LValue &var) { _object3 = var; }
        void AddSendParam(SendParam* pParam) { _params.push_back(pParam); }
        void AddSendParam(std::auto_ptr<SendParam> pParam) { _params.push_back(pParam.get()); pParam.release(); }
        void SetSendRest(RestStatement &sendRest) { _rest = sendRest; _fRest = true; }
        virtual void DebugString(DebugStringOut &out) const;

    private:
        LValue _object3;            // (send theFlakes[i]:x)
        std::string _object;        // (theMan:x)               (alias for name)
        SingleStatement _object2;   // (send (= client foo):x)  (alias for statement1)
        SendParamVector _params;    // The actual send params (selectors)
        // Hack for SCIStudio template game:
        bool _fRest;
        RestStatement _rest;      
    };

    //
    // A call to a procedure
    //
    class ProcedureCall : public SyntaxNode, public NamedNode, public StatementsNode
    {
        DECLARE_NODE_TYPE(NodeTypeProcedureCall)
    public:
        ProcedureCall() : NamedNode(_name), StatementsNode(_params) {}
        ProcedureCall(ProcedureCall &src);
        ProcedureCall& operator=(ProcedureCall& src);
        ~ProcedureCall();

        // Needed for syntax tree adjustment.
        void StealParams(SingleStatementVector &stolen) { stolen.swap(_params); }

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;

        SingleStatementPtr GetParameter(size_t i) { return (i < _params.size()) ? _params[i] : NULL; }

    private:
        SingleStatementVector _params;
        std::string _name;
    };

    class ReturnStatement : public SyntaxNode, public OneStatementNode
    {
        DECLARE_NODE_TYPE(NodeTypeReturn)
    public:
        ReturnStatement() : OneStatementNode(_retValue) {}

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context) { _retValue.PreScan(context); }
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;

        const SingleStatement &GetValue() const { return _retValue; }

    private:
        SingleStatement _retValue; // statement1
    };

    class ForLoop : public SyntaxNode, public StatementsNode, public ConditionNode, public CodeBlockNode
    {
        DECLARE_NODE_TYPE(NodeTypeForLoop)
    public:
        ForLoop() : StatementsNode(_code), ConditionNode(_condition), CodeBlockNode(_initializer) {};
        ForLoop(ForLoop &src);
        ForLoop& operator=(ForLoop& src);
        ~ForLoop();

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context); 
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;
        void SetLooper(CodeBlock &block) { _looper = block; }

    private:
        CodeBlock _initializer; // alias for _list from CodeBlockNode
        CodeBlock _looper;
        ConditionalExpression _condition;
        SingleStatementVector _code;
    };

    class WhileLoop : public SyntaxNode, public StatementsNode, public ConditionNode
    {
        DECLARE_NODE_TYPE(NodeTypeWhileLoop)
    public:
        WhileLoop() : StatementsNode(_code), ConditionNode(_condition) {}
        WhileLoop(WhileLoop &src);
        WhileLoop& operator=(WhileLoop& src);
        ~WhileLoop();

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;

    private:
        ConditionalExpression _condition;
        SingleStatementVector _code;
    };

    class DoLoop : public SyntaxNode, public ConditionNode, public StatementsNode
    {
        DECLARE_NODE_TYPE(NodeTypeDoLoop)
    public:
        DoLoop() : ConditionNode(_condition), StatementsNode(_doList) {}
        DoLoop(DoLoop &src);
        DoLoop& operator=(DoLoop& src);
        ~DoLoop();

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;

    private:
        SingleStatementVector _doList;
        ConditionalExpression _condition;
    };

    class BreakStatement : public SyntaxNode
    {
        DECLARE_NODE_TYPE(NodeTypeBreak)
    public:
        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void DebugString(DebugStringOut &out) const;
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
    };

    class CaseStatement : public SyntaxNode, public StatementsNode, public OneStatementNode
    {
        DECLARE_NODE_TYPE(NodeTypeCase)
    public:
        CaseStatement() : StatementsNode(_code), OneStatementNode(_case) { _fDefault = false; }
        CaseStatement(CaseStatement &src);
        CaseStatement& operator=(CaseStatement& src);
        ~CaseStatement();

        const SingleStatement &GetCaseValue() const { return _case; }
        SingleStatement &GetCaseValue() { return _case; }
        const SingleStatementVector &GetCodeSegments() const { return _code; }
        SingleStatementVector &GetCodeSegments() { return _code; }
        bool IsDefault() const { return _fDefault; }

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;
        void SetDefault(bool fDefault) { _fDefault = fDefault; }

    private:
        bool _fDefault;
        SingleStatement _case;
        SingleStatementVector _code;
    };

    class SwitchStatement : public SyntaxNode, public OneStatementNode
    {
        DECLARE_NODE_TYPE(NodeTypeSwitch)
    public:
        SwitchStatement() : OneStatementNode(_switch) {}
        SwitchStatement(SwitchStatement &src);
        SwitchStatement& operator=(SwitchStatement& src);
        ~SwitchStatement();

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

        virtual void DebugString(DebugStringOut &out) const;
        void AddCase(CaseStatement *pCase) { _cases.push_back(pCase); }
        void AddCase(std::auto_ptr<CaseStatement> pCase) { AddCase(pCase.get()); pCase.release(); }

    private:
        SingleStatement _switch;
        std::vector<CaseStatement*> _cases;
    };

    class CSCIOMAsm : public SyntaxNode
    {
        DECLARE_NODE_TYPE(NodeTypeAsm)
    public:
        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
    };

    //
    // Assignment statement (e.g. += foo 1)
    //
    class Assignment : public SyntaxNode, public NamedNode, public OneStatementNode
    {
        DECLARE_NODE_TYPE(NodeTypeAssignment)
    public:
        Assignment() : NamedNode(_assignment), OneStatementNode(_value) {}

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);

        void SetVariable(LValue &var) { _variable = var; }
        virtual void DebugString(DebugStringOut &out) const;
    private:
        LValue _variable;
        SingleStatement _value; // Statement1
        std::string _assignment;
    };

    //
    // A binary operation (e.g. + x 6)
    //
    class BinaryOp : public SyntaxNode, public NamedNode, public OneStatementNode, public TwoStatementNode
    {
        DECLARE_NODE_TYPE(NodeTypeBinaryOperation)
    public:
        BinaryOp() : NamedNode(_operator), OneStatementNode(_leftOperand), TwoStatementNode(_rightOperand) {}

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context) { _leftOperand.PreScan(context); _rightOperand.PreScan(context); }
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;
        virtual std::string ToString() const;
    private:
        CodeResult _OutputByteCodeAnd(ICompileContext &context) const;
        CodeResult _OutputByteCodeOr(ICompileContext &context) const;
        SingleStatement _leftOperand;  // statement1
        SingleStatement _rightOperand; // statement2
        std::string _operator;
    };

    //
    // A unary operation (e.g. ++x)
    //
    class UnaryOp : public SyntaxNode, public NamedNode, public OneStatementNode
    {
        DECLARE_NODE_TYPE(NodeTypeUnaryOperation)
    public:
        UnaryOp() : NamedNode(_operator), OneStatementNode(_operand) {}

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context) { _operand.PreScan(context); }
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;
        virtual std::string ToString() const;
    private:
        SingleStatement _operand; // statement1
        std::string _operator;
    };

    //
    // If statement (SCI only)
    //
    class IfStatement : public SyntaxNode, public StatementsNode, public ConditionNode
    {
        DECLARE_NODE_TYPE(NodeTypeIf)
    public:
        IfStatement() : StatementsNode(_code), ConditionNode(_condition) {}
        IfStatement(IfStatement &src);
        IfStatement& operator=(IfStatement& src);
        ~IfStatement();

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context) ;
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;

    private:
        ConditionalExpression _condition;
        SingleStatementVector _code;
    };

    class ElseClause : public SyntaxNode, public StatementsNode
    {
        DECLARE_NODE_TYPE(NodeTypeElse)
    public:
        ElseClause() : StatementsNode(_code) {}
        ~ElseClause();

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context);
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;

    private:
        SingleStatementVector _code;
    };


    //
    // If statement (SCI only)
    //
    class CppIfStatement : public SyntaxNode, public ConditionNode, public OneStatementNode, public TwoStatementNode
    {
        DECLARE_NODE_TYPE(NodeTypeCppIf)
    public:
        CppIfStatement() : OneStatementNode(_if), TwoStatementNode(_else), ConditionNode(_condition), _fTernary(false) {}
        CppIfStatement(CppIfStatement &src);
        CppIfStatement& operator=(CppIfStatement& src);

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const;
        void PreScan(ICompileContext &context) ;
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en);
        virtual void DebugString(DebugStringOut &out) const;

        // Causes output to be of the form _condition ? _if : _else
        void MakeTernary() { _fTernary = true; }

    private:
        ConditionalExpression _condition;
        SingleStatement _if;
        SingleStatement _else;      // CPP only.
        bool _fTernary;             // Currently for decompilation only - never used for compilation.
    };

    // Not supported yet
    class Asm : public SyntaxNode, public NamedNode
    {
        DECLARE_NODE_TYPE(NodeTypeAsm)
    public:
        Asm() : NamedNode(_instruction) {}
        Asm(const Asm &src);
        Asm& operator=(const Asm &src);

        // IOutputByteCode
        CodeResult OutputByteCode(ICompileContext &context) const { return 0; }
        void PreScan(ICompileContext &context) {}
        void Traverse(IExploreNodeContext *pContext, IExploreNode &en) {}
        virtual void DebugString(DebugStringOut &out) const;

        void SetArguments(const std::string &arguments) { _arguments = arguments; }

    private:
        std::string _instruction;
        std::string _arguments;
    };

}; //namespace sci

