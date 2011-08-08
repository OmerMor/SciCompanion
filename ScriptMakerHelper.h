#pragma once

template<typename _T>
void _AddStatement(_T &method, std::auto_ptr<sci::SyntaxNode> pNode)
{
    auto_ptr<sci::SingleStatement> pStatement(new sci::SingleStatement);
    pStatement->SetSyntaxNode(pNode);
    method.AddStatement(pStatement);
}

void _MakeTokenStatement(sci::SingleStatement &statement, const std::string &token);

void _MakeNumberStatement(sci::SingleStatement &statement, WORD w);

void _AddAssignment(sci::MethodDefinition &method, const std::string &lvalueName, const std::string &assigned);

template<typename _T>
void _AddComment(_T &method, const std::string &comment)
{
    std::auto_ptr<Comment> pComment(new sci::Comment);
    pComment->SetName(comment);
    _AddStatement(method, (auto_ptr<SyntaxNode>)pComment);
}

void _AddBasicSwitch(sci::MethodDefinition &method, const std::string &switchValue, const std::string &case0Comments);

void _AddSendCall(sci::MethodDefinition &method, const std::string &objectName, const std::string &methodName, const std::string &parameter);
