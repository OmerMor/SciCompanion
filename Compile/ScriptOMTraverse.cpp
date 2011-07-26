#include "stdafx.h"
#include "ScriptOMAll.h"

using namespace sci;
using namespace std;

class PerformTraverse : public std::binary_function<SyntaxNode*, IExploreNodeContext*, void>
{
public:
    PerformTraverse(IExploreNodeContext *pContext, IExploreNode &en) : _pContext(pContext), _en(en) {}
    void operator()(SyntaxNode* pNode) const
    {
        pNode->Traverse(_pContext, _en);
    }
private:
    IExploreNodeContext *_pContext;
    IExploreNode &_en;
};

template<typename T>
void ForwardTraverse(const T &container, IExploreNodeContext *pContext, IExploreNode &en)
{
    for_each(container.begin(), container.end(), PerformTraverse(pContext, en));
}


void FunctionBase::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    ForwardTraverse(_tempVars, pContext, en);
    ForwardTraverse(_code, pContext, en);
}


void CodeBlock::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    ForwardTraverse(_code, pContext, en);
}
void Cast::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _object.Traverse(pContext, en);
}
void SendCall::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    // One of three forms of send params.
    ForwardTraverse(_params, pContext, en);
    if (_object.empty())
    {
        if (_object3.GetName().empty())
        {
            _object2.Traverse(pContext, en);
        }
        else
        {
            _object3.Traverse(pContext, en);
        }
    }
}
void ProcedureCall::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    ForwardTraverse(_params, pContext, en);
}
void ConditionalExpression::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    ForwardTraverse(_terms, pContext, en);
}
void ElseClause::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    ForwardTraverse(_code, pContext, en);
}
void SwitchStatement::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _switch.Traverse(pContext, en);
    ForwardTraverse(_cases, pContext, en);
}
void ForLoop::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _initializer.Traverse(pContext, en);
    _condition.Traverse(pContext, en);
    _looper.Traverse(pContext, en);
    ForwardTraverse(_code, pContext, en);
}
void WhileLoop::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _condition.Traverse(pContext, en);
    ForwardTraverse(_code, pContext, en);
}
void Script::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    ForwardTraverse(_procedures, pContext, en);
    ForwardTraverse(_classes, pContext, en);
    ForwardTraverse(_scriptVariables, pContext, en);
}


void DoLoop::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _condition.Traverse(pContext, en);
    ForwardTraverse(_doList, pContext, en);
}
void Assignment::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _variable.Traverse(pContext, en);
    _value.Traverse(pContext, en);
}

void SingleStatement::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    if (_pThing.get() && (GetType() != NodeTypeUnknown))
    {
        _pThing->Traverse(pContext, en);
    }
}
void ClassProperty::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
}
void ClassDefinition::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    ForwardTraverse(_methods, pContext, en);
    ForwardTraverse(_properties, pContext, en);
}
void SendParam::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    ForwardTraverse(_params, pContext, en);
}
void LValue::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    if (_fHasIndexer)
    {
        _array.Traverse(pContext, en);
    }
}
void ReturnStatement::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _retValue.Traverse(pContext, en);
}
void CaseStatement::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _case.Traverse(pContext, en);
    ForwardTraverse(_code, pContext, en);
}
void UnaryOp::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _operand.Traverse(pContext, en);
}
void BinaryOp::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _leftOperand.Traverse(pContext, en);
    _rightOperand.Traverse(pContext, en);
}
void IfStatement::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _condition.Traverse(pContext, en);
    ForwardTraverse(_code, pContext, en);
}
void CppIfStatement::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    _condition.Traverse(pContext, en);
    _if.Traverse(pContext, en);
    _else.Traverse(pContext, en);
}
void ComplexPropertyValue::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
    if (_pArrayInternal)
    {
        _pArrayInternal->Traverse(pContext, en);
    }
}
void BreakStatement::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
}
void RestStatement::Traverse(IExploreNodeContext *pContext, IExploreNode &en)
{
    en.ExploreNode(pContext, *this);
}
