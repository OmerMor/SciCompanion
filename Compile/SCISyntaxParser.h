
#pragma once

#include "ScriptOM.h"
#include "..\CrystalScriptStream.h"
#include "..\ParserCommon.h"

#include <stack>
#include <deque>

class MatchResult
{
public:
    MatchResult() { ASSERT(FALSE); }
    MatchResult(bool fResult) : _fResult(fResult) {}
    MatchResult(MatchResult &src)
    {
        _fResult = src._fResult;
    }
    MatchResult& operator=(MatchResult& src)
    {
        if (this != &src)
        {
            _fResult = src._fResult;
        }
        return *this;
    }
    ~MatchResult() { }
    bool Result() { return _fResult; }
    void SetResult(bool fResult) { _fResult = fResult; }
private:
    bool _fResult;
};

template<typename _TContext, typename _It>
class ParserBase
{
public:
#ifdef DEBUG
    void SetName(const std::string &name) { Name = name; }
    std::string Name;
#endif

    typedef bool(*MATCHINGFUNCTION)(const ParserBase *pParser, _TContext *pContext, _It &stream);
    typedef void(*DEBUGFUNCTION)(bool fEnter, bool fResult);
    typedef void(*ACTION)(MatchResult &match, const ParserBase *pParser, _TContext *pContext, const _It &stream);

    // Fwd decl (Used for circular references in grammer descriptions)
    // If an empty Parser is created (since you need to refer to it subsequently, but you can't
    // define it yet), it will be endowed with this matching function.
    template<typename _It>
    bool static ReferenceForwarderP(const ParserBase *pParser, _TContext *pContext, _It &stream)
    {
        ASSERT(FALSE);
        return false; // Just a dummy return value
    }

    template<typename _It>
    void static CopyParserVector(std::vector<ParserBase*> &out, const std::vector<ParserBase*> &in)
    {
        out.clear();
        std::vector<ParserBase*>::const_iterator start = in.begin();
        while (start != in.end())
        {
            out.push_back(new ParserBase(*(*start)));
            ++start;
        }
    }

    ParserBase(const ParserBase& src)
    {
        // Allocate new Parser objects
        if (src._fOnlyRef)
        {
            _pa = NULL;
            _psz = NULL;
            _pRef = &src;
            _pfn = ReferenceForwarderP<_It>;
            _pfnA = NULL;
            _pfnDebug = NULL;
            _fLiteral = false; // Doesn't matter
            _fOnlyRef = false; // We're a ref, so people can copy us.
            //ASSERT(src._pfn != ReferenceForwarderP<_It>); 
        }
        else
        {
            _pa = src._pa ? new ParserBase(*src._pa) : NULL;
            _psz = src._psz; // Ok, because this is always a static string
            _pfn = src._pfn;
            _pfnA = src._pfnA;
            _pfnDebug = src._pfnDebug;
            _pRef = src._pRef; // Don't make a new object.
            _fLiteral = src._fLiteral;
            _fOnlyRef = false; // Copied parsers are temporary objects, generally, and so we shouldn't take references to them.
            if (src._pfn == NULL)
            {
                // No matching function means this is an empty parser... pass a ref to the source
                _pRef = &src;
                _pfn = ReferenceForwarderP<_It>;
                ASSERT(src._pfn != ReferenceForwarderP<_It>); 
            }
            CopyParserVector<_It>(_parsers, src._parsers);
        }
#ifdef DEBUG
        Name = src.Name;
#endif
        //ASSERT(_pfn != ReferenceForwarderP<_It> || _pRef->_pfn != ReferenceForwarderP<_It>);
    }

    ParserBase& operator=(const ParserBase& src)
    {
        if (this != &src)
        {
            // Allocate new Parser objects
            _pa = src._pa ? new ParserBase(*src._pa) : NULL;
            _psz = src._psz; // Ok, because this is always a static string
            _pfn = src._pfn;
            _pfnA = src._pfnA;
            _pfnDebug = src._pfnDebug;
            _pRef = src._pRef; // Don't make a new object.
            _fLiteral = src._fLiteral;
            ASSERT(_fOnlyRef);
            if ((src._pfn == NULL) || src._fOnlyRef)
            {
                //ASSERT(FALSE); // We probably never hit this - but if we do, we should check if things are right.
                // No matching function means this is an empty parser... pass a ref to the source
                _pRef = &src;
                _pfn = ReferenceForwarderP<_It>;
            }
            else
            {
                CopyParserVector<_It>(_parsers, src._parsers);
            }
#ifdef DEBUG
            Name = src.Name;
#endif
        }
        return *this;
    }

    // The default constructor will create an object that can only be copied by reference (see the copy constructor
    // and == operator, and _pRef)
    ParserBase() : _pfn(NULL), _pfnA(NULL), _pfnDebug(NULL), _pa(NULL), _pRef(NULL), _fLiteral(false), _fOnlyRef(true), _psz(NULL) {}
    ParserBase(MATCHINGFUNCTION pfn) : _pfn(pfn), _pfnA(NULL), _pfnDebug(NULL), _pa(NULL), _pRef(NULL), _fLiteral(false), _fOnlyRef(false), _psz(NULL) {}
    ParserBase(MATCHINGFUNCTION pfn, const ParserBase &a) : _pfn(pfn), _pfnA(NULL), _pfnDebug(NULL), _pRef(NULL), _fLiteral(false), _fOnlyRef(false), _psz(NULL)
    {
        // Make a copy of the parsers passed in
        _pa = new ParserBase(a);
    }
    ParserBase(MATCHINGFUNCTION pfn, const char *psz) : _pfn(pfn) , _psz(psz), _pfnA(NULL), _pfnDebug(NULL), _pa(NULL), _pRef(NULL), _fLiteral(false), _fOnlyRef(false)
    {
    }
    ~ParserBase()
    {
        //ASSERT(!_fOnlyRef);
        delete _pa;
        std::for_each(_parsers.begin(), _parsers.end(), delete_object());
    }
    MatchResult Match(_TContext *pContext, _It &stream) const
    {
        ASSERT(_pfn);
        if (!_fLiteral)
        {
            _EatWhitespaceAndComments<_TContext, _It>(pContext, stream);
        }
        _It streamSave(stream);
#ifdef PARSE_DEBUG
        if (!Name.empty() || _psz)
        {
            string name = !Name.empty() ? Name : _psz;
            string text;
            _It streamTemp = stream;
            text += *streamTemp;
            if (text[0])
                text += *++streamTemp;
            if (text[1])
                text += *++streamTemp;
            std::stringstream ss;
            string spaces;
            spaces.append(g_ParseIndent, ' ');
            ss << spaces << "-->Matching " << name << ": " << text << "\n";
            OutputDebugString(ss.str().c_str());
        }
        g_ParseIndent++;
#endif
        MatchResult result(_pRef ? _pRef->Match(pContext, stream) : (*_pfn)(this, pContext, stream));
        
#ifdef PARSE_DEBUG
        g_ParseIndent--;
        if (!Name.empty() || _psz)
        {
            string name = !Name.empty() ? Name : _psz;
            string text;
            _It streamTemp = stream;
            text += *streamTemp;
            if (text[0])
                text += *++streamTemp;
            if (text[1])
                text += *++streamTemp;
            std::stringstream ss;
            string spaces;
            spaces.append(g_ParseIndent, ' ');
            ss << spaces << "<--Matching " << name << " " << (result.Result() ? "true: " : "false: ") << text << "\n";
            OutputDebugString(ss.str().c_str());
        }
#endif
        if (_pfnA)
        {
            (*_pfnA)(result, this, pContext, stream);
        }
        if (!result.Result())
        {
            // Revert the stream to what it was when we came in this function (after whitespace)
            stream = streamSave;
        }
        return result;
    }

    // This is for actions.
    ParserBase operator[](ACTION pfn)
    {
        ASSERT(_pfnA == NULL); // Ensure we're not overwriting any action.
        ParserBase newOne(*this);
        newOne._pfnA = pfn;
        return newOne;
    }

    // This is for wrapping a parser in another, such as when we want its
    // Match function to always match and act as a pre-action.
    // e.g. syntaxnode_d[value[FinishValueA]]
    ParserBase operator[](const ParserBase &parser)
    {
        // This should take a parser
        ASSERT(_pa == NULL);    // Ensure we're not overwriting a parser.
        ParserBase newOne(*this);
        // We'll contain the provided parser in ourselves.
        newOne._pa = new ParserBase(parser);
        return newOne;
    }

    // Returns a parser that is literal (doesn't skip whitespace prior to matching)
    ParserBase operator+()
    {
        ParserBase newOne(*this);
        newOne._fLiteral = true;
        return newOne;
    }

    void SetDebug(DEBUGFUNCTION pfnDebug)
    {
        _pfnDebug = pfnDebug;
    }

    void AddParser(const ParserBase &add)
    {
        // This is for matching functions that need an array
        // Be careful the matching function hasn't been replaced with a reference forwarder!
        ASSERT(_pfn != ReferenceForwarderP<_It>);
        _parsers.push_back(new ParserBase(add));
    }

    ParserBase *_pa;
    std::vector<ParserBase*> _parsers;
    const char *_psz;
    // PERF: perhaps we could optimize for some cases here, and not have a matching functino (e.g. char)
    MATCHINGFUNCTION _pfn;
    ACTION _pfnA;
    DEBUGFUNCTION _pfnDebug;
    const ParserBase *_pRef;
    bool _fLiteral; // Don't skip whitespace
    bool _fOnlyRef; // Only references to this parser... it's lifetime is guaranteed.
};



//
// Common parsing primitives
//
template<typename _It, typename _TContext>
bool AlphanumP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    bool fRet = false;
    std::string &str = pContext->ScratchString();
    str.clear();
    char ch = *stream;
    if (isalpha(ch) || (ch == '_'))     // First character must be a letter or _
    {
        fRet = true;
        str += ch;
        ch = *(++stream);
        while (isalnum(ch) || (ch == '_'))  // Then any alphanumeric character is fine.
        {
            fRet = true;
            str += ch;
            ch = *(++stream);
        }
    }
    return fRet;
}

template<typename _It, typename _TContext>
bool SequenceP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    std::vector<ParserBase<_TContext, _It>*>::const_iterator start = pParser->_parsers.begin();
    std::vector<ParserBase<_TContext, _It>*>::const_iterator end = pParser->_parsers.end();
    while (start != end)
    {
        if (!(*start)->Match(pContext, stream).Result())
        {
            return false;
        }
        ++start;
    }
    return true;
}

template<typename _It, typename _TContext>
bool AlternativeP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    std::vector<ParserBase<_TContext, _It> *>::const_iterator start = pParser->_parsers.begin();
    std::vector<ParserBase<_TContext, _It> *>::const_iterator end = pParser->_parsers.end();
    while (start != end)
    {
        if ((*start)->Match(pContext, stream).Result())
        {
            return true;
        }
        ++start;
    }
    return false;
}


template<typename _It, typename _TContext>
bool KleeneP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    while (pParser->_pa->Match(pContext, stream).Result())
    {
        while (isspace(*stream))
        {
            ++stream;
        }
    }
    return true; // Always matches
}



template<typename _It, typename _TContext>
bool ZeroOrOnceP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    pParser->_pa->Match(pContext, stream);
    return true; // Always matches
}


template<typename _It, typename _TContext>
bool QuotedStringP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    return _ReadString<_TContext, _It, '"', '"'>(stream, pContext->ScratchString());
}

template<typename _It, typename _TContext>
bool SQuotedStringP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    return _ReadString<_TContext, _It, '\'', '\''>(stream, pContext->ScratchString());
}

template<typename _It, typename _TContext>
bool BraceStringP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    return _ReadString<_TContext, _It, '{', '}'>(stream, pContext->ScratchString());
}


int charToI(char ch);
//
// Handles negation, hex, etc...
//
template<typename _It, typename _TContext>
bool IntegerP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    int i = 0;
    bool fNeg = false;
    bool fRet = false;
    bool fHex = false;
    if (*stream == '$')
    {
        fHex = true;
        ++stream;
        while (isxdigit(*stream))
        {
            i *= 16;
            i += charToI(*stream);
            fRet = true;
            ++stream;
        }
    }
    else
    {
        if (*stream == '-')
        {
            fNeg = true;
            ++stream;
        }
        while (isdigit(*stream))
        {
            i *= 10;
            i += charToI(*stream);
            fRet = true;
            ++stream;
        }
    }
    if (fRet)
    {
        // Make sure that the number isn't followed by an alphanumeric char
        fRet = !isalnum(*stream);
    }
    if (fNeg)
    {
        i = -i;
    }
    if (fRet)
    {
        // Let the context know so people can use it.
        pContext->SetInteger(i, fNeg, fHex, stream);
    }
    return fRet;
}


template<typename _It, typename _TContext>
bool AlphanumOpenP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    bool fRet = false;
    std::string &str = pContext->ScratchString();
    str.clear();
    char ch = *stream;
    if (isalpha(ch) || (ch == '_'))
    {
        fRet = true;
        str += ch;
        ch = *(++stream);
        while (isalnum(ch) || (ch == '_'))
        {
            fRet = true;
            str += ch;
            ch = *(++stream);
        }
    }
    if (fRet && (*stream == '('))
    {
        ++stream;
        return true;
    }
    else
    {
        return false;
    }
}

template<typename _It, typename _TContext>
bool AlwaysMatchP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    return true; // Always matches
}


//
// Matches a single character
//
template<typename _It, typename _TContext>
bool CharP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    bool fRet = false;
    const char *psz = pParser->_psz;
    while (*psz && (*stream == *psz))
    {
        ++stream;
        ++psz;
    }
    return (*psz == 0);
}

//
// Matches a keyword (e.g. a piece of text NOT followed by an alphanumeric character).
//
template<typename _It, typename _TContext>
bool KeywordP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    bool fRet = false;
    const char *psz = pParser->_psz;
    while (*psz && (*stream == *psz))
    {
        ++stream;
        ++psz;
    }
    if (*psz == 0)
    {
        // We used up the whole keyword.  Make sure it isn't followed by another alpha numeric char.
        return !isalnum(*stream);
    }
    return false;
}

template<typename _It, typename _TContext>
bool OperatorP(const ParserBase<_TContext, _It> *pParser, _TContext *pContext, _It &stream)
{
    bool fRet = false;
    const char *psz = pParser->_psz;
    while (*psz && (*stream == *psz))
    {
        ++stream;
        ++psz;
    }
    if (*psz == 0)
    {
        pContext->ScratchString() = pParser->_psz;
        char chNext = *stream;
        // REVIEW: we're relying on careful ordering of rules to have this parser work.
        // The "operator" parser needs to make sure there isn't another "operator char" following
        // this:
        if ((chNext != '|') && (chNext != '&') && (chNext != '=') && (chNext != '<') && (chNext != '>'))
        {
            // Probably more...
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}



//
// Parser operators
//

//
// *a
// 
// Matches n instances of a  (where n can be 0)
//
template<typename _It, typename _TContext>
inline ParserBase<_TContext, _It> operator*(const ParserBase<_TContext, _It> &a)
{
    return ParserBase<_TContext, _It>(KleeneP<_It, _TContext>, a);
}

//
// a >> b
// 
// Matches a followed by b
//
template<typename _It, typename _TContext>
inline ParserBase<_TContext, _It> operator>>(const ParserBase<_TContext, _It> &a, const ParserBase<_TContext, _It> &b)
{
    // We must create a new "sequence" thing if a is not a sequence, or if a has an action.
    // If a has an action, then we'll mis-place it if we don't create a new sequence.
    // e.g.
    // c = a >> b;
    // e = c[act1] >> d[act2]
    // would result in
    // Seq[act1]
    //      a
    //      b
    //      d[act2]
    // The result would be act2 gets called prior to act1.
    // It looks like this optimization won't be useful, since most things have actions.
    if ((a._pfn != SequenceP<_It, _TContext>) || (a._pfnA))
    {
        ParserBase<_TContext, _It> alternative(SequenceP<_It, _TContext>);
        alternative.AddParser(a);
        alternative.AddParser(b);
        return alternative;
    }
    else
    {
        // A is already an alternative.  Add b to it.
        ParserBase<_TContext, _It> parserA(a);
        if (parserA._pfn == SequenceP<_It, _TContext>)
        {
            parserA.AddParser(b);
            return parserA;
        }
        else
        {
            // The matching function changed (reference forwarding...) we can't just add it...
            ParserBase<_TContext, _It> alternative(SequenceP<_It, _TContext>);
            alternative.AddParser(a);
            alternative.AddParser(b);
            return alternative;
        }
    }
}

//
// a | b
// 
// Matches a or b
//
template<typename _It, typename _TContext>
inline ParserBase<_TContext, _It> operator|(const ParserBase<_TContext, _It> &a, const ParserBase<_TContext, _It> &b)
{
    // See operator>>
    if ((a._pfn != AlternativeP<_It, _TContext>) || (a._pfnA))
    {
        ParserBase<_TContext, _It> alternative(AlternativeP<_It, _TContext>);
        alternative.AddParser(a);
        alternative.AddParser(b);
        return alternative;
    }
    else
    {
        // A is already an alternative.  Add b to it.
        ParserBase<_TContext, _It> parserA(a);
        if (parserA._pfn == AlternativeP<_It, _TContext>)
        {
            parserA.AddParser(b);
            return parserA;
        }
        else
        {
            // The matching function of a changed... we can no longer just add.
            ParserBase<_TContext, _It> alternative(AlternativeP<_It, _TContext>);
            alternative.AddParser(a);
            alternative.AddParser(b);
            return alternative;
        }
    }
}

//
// (a % b)     
//
// equivalent to    (a >> *(b >> a))
//
template<typename _It, typename _TContext>
inline ParserBase<_TContext, _It> operator%(const ParserBase<_TContext, _It> &a, const ParserBase<_TContext, _It> &b)
{
    return a >> *(b >> a);
}

//
// !a
// 
// 0 or 1 instances of a
//
template<typename _It, typename _TContext>
inline ParserBase<_TContext, _It> operator!(const ParserBase<_TContext, _It> &a)
{
    return ParserBase<_TContext, _It>(ZeroOrOnceP<_It, _TContext>, a);
}


//
// Common error primitives
//
template<typename _It, typename _TContext>
void GeneralE(MatchResult &match, const ParserBase<_TContext, _It> *pParser, _TContext *pContext, const _It &stream)
{
    ASSERT(pParser->_psz); // Not valid to call this if there isn't something we can say was expected.
    if (!match.Result())
    {
        std::string error = "Expected \"";
        error += pParser->_psz;
        error += "\" ";
        pContext->ReportError(error, stream);
    }
}









// The kind of iterator we use.
typedef CCrystalScriptStream::const_iterator streamIt;

class SyntaxContext
{
public:
    SyntaxContext(streamIt beginning, sci::Script &script) : _beginning(beginning), _script(script)
    {
        SynonymPtr = NULL;
        DefinePtr = NULL;
        VariableDeclPtr = NULL;
        FunctionPtr = NULL;
        ClassPtr = NULL;
        ClassPropertyPtr = NULL;
        StatementPtrReturn = NULL;
    }
    ~SyntaxContext()
    {
        delete SynonymPtr;
        delete DefinePtr;
        delete VariableDeclPtr;
        delete FunctionPtr;
        delete ClassPtr;
        delete ClassPropertyPtr;
        delete StatementPtrReturn;
        ASSERT(_statements.empty()); // Or else someone messed up, or there could have been an exception
        for_each(_statements.c.begin(), _statements.c.end(), delete_object());
    }
    void ReportError(std::string error, streamIt pos)
    {
        // Prefer previous errors at the same spot
        if ((_beginning < pos) || _error.empty())
        {
            _error = error;
            _beginning = pos;
        }
    }
    std::string GetErrorText()
    {
        return _error;
    }
    streamIt GetErrorPosition()
    {
        return _beginning;
    }

    // Accessors
    sci::Script &Script() { return _script; }
    std::string &ScratchString() { return _scratch; }
    std::string &ScratchString2() { return _scratch2; }
    void SetInteger(int i, bool fNeg, bool fHex, streamIt pos)
    {
        if ((i > 0xffff) || (i < -32768))
        {
            ReportError("Integer value is too large.", pos);
        }
        Integer = (WORD)i;
        NegInt = fNeg;
        HexInt = fHex;
    }
    WORD Integer;   // Last encountered number 
    bool NegInt;    // Was the number a result of negation
    bool HexInt;    // Was the number expressed in hex

    void CreateSynonym() { delete SynonymPtr; SynonymPtr = new sci::Synonym; }
    sci::SynonymPtr SynonymPtr;
    void CommitSynonym() { SynonymPtr = NULL; }

    void CreateDefine() { delete DefinePtr; DefinePtr = new sci::Define; }
    sci::DefinePtr DefinePtr;
    void CommitDefine() { DefinePtr = NULL; }

    void CreateProcedure() { delete FunctionPtr; FunctionPtr = new sci::ProcedureDefinition; FunctionPtr->AddSignature(new sci::FunctionSignature); }
    void CreateMethod() { delete FunctionPtr; FunctionPtr = new sci::MethodDefinition; FunctionPtr->AddSignature(new sci::FunctionSignature); }
    sci::FunctionPtr FunctionPtr;
    void CommitMethod() { FunctionPtr = NULL; }

    void CreateClass() { delete ClassPtr; ClassPtr = new sci::ClassDefinition; }
    sci::ClassPtr ClassPtr;
    void CommitClass() { ClassPtr = NULL; }

    void CreateClassProperty() { delete ClassPropertyPtr; ClassPropertyPtr = new sci::ClassProperty; }
    sci::ClassPropertyPtr ClassPropertyPtr;
    void CommitClassProperty() { ClassPropertyPtr = NULL; }

    void StartVariableDecl(const std::string &name)
    {
        VariableDecl.SetName(name);
        VariableDecl.SetSize(1); // Vars are size 1 by default.
    }

    // 1) Each statement must begin with a push onto the statement stack.
    // 2) The subrule will fill that in with the required node (forloop, assignment, etc...)
    // 3) Once the subrule is complete, we'll either
    //      a) transfer the node to a new statement object, and pop the node of the statement stack (if successful), or
    //      b) delete the node, and pop it off the statement stack.
    void PushSyntaxNode()
    {
        // Put a null on the statement stack to make room for a statement
        _statements.push(NULL);
    }
    // This is the "return value" from a generic statement
    sci::SingleStatementPtr StatementPtrReturn;
    void FinishStatement(bool fSuccess)
    {
        if (fSuccess)
        {
            // The statement is moved to the "return value"
            delete StatementPtrReturn;
            StatementPtrReturn = new sci::SingleStatement;
            StatementPtrReturn->SetSyntaxNode(_statements.top());
        }
        else
        {
            // We shall never hear of this statement again.
            delete _statements.top();
        }
        _statements.pop();
    }
    // If you transfer ownership of StatementPtrReturn to someone, call this
    void ClearStatementReturn() { StatementPtrReturn = NULL; }

    // Make a syntax node (forloop, assignment, etc...) at the top of the statement stack
    // (use PushSyntaxNode to push a new spot onto the stack)
    template<typename _T>
    void CreateSyntaxNode(const streamIt &stream)
    {
        ASSERT(!_statements.empty());       // That would mean there is no statement stack frame at all.
        if (_statements.top())
        {
            // Someone created a statement, then didn't clean up (e.g. property value was created, but then failed!)
            delete _statements.top();
            _statements.top() = NULL;
        }
        _T *p = new _T;
        // Assign a line/char #
        p->SetPosition(stream.GetPosition());
        _statements.top() = p;
    }

    // Returns a pointer to the first syntax node of this type
    const sci::SyntaxNode *GetSyntaxNode(sci::NodeType type) const;
    // Returns the type of the topmost non-null syntax node
    sci::NodeType GetTopKnownNode() const;

    // Get the syntax node at the top of the statement stack
    template<typename _statementT>
    _statementT GetSyntaxNode() const
    {
        SyntaxNode *pNode = _statements.top();
        ASSERT(_statements.top()->GetNodeType() == _statementT->MyNodeType);
        return static_cast<_statementT>(_statements.top());
    }
    template<typename _statementT>
    _statementT GetPrevSyntaxNode() const
    {
        // Get the item that isn't at the top of the stack... it's one up.
        std::deque<sci::SyntaxNode*>::const_reverse_iterator prevIt = _statements.c.rbegin();
        prevIt++;
        NodeType type1 = (*prevIt)->GetNodeType();
        NodeType type2 = _statementT->MyNodeType;
        ASSERT((*prevIt)->GetNodeType() == _statementT->MyNodeType);
        return static_cast<_statementT>((*prevIt));
    }
    // Like GetSyntaxNode, but sets the top of the stack to NULL (doesn't pop),
    // assuming that ownership has been transfered to the caller.
    template<typename _statementT>
    _statementT StealSyntaxNode()
    {
        SyntaxNode *pNode = _statements.top();
        ASSERT(_statements.top()->GetNodeType() == _statementT->MyNodeType);
        _statements.top() = NULL;
        return static_cast<_statementT>(pNode);
    }
    void DeleteAndPopSyntaxNode()
    {
        delete _statements.top();
        _statements.pop();
    }

    void CreateScriptVariable() { delete VariableDeclPtr; VariableDeclPtr = new sci::VariableDecl(); }
    sci::VariableDeclPtr VariableDeclPtr;
    void CommitScriptVariable() { VariableDeclPtr = NULL; }

    // A generic values for everyone to use (one at a time!)
    // These kinds of objects are "dead ends" on the parse tree, so it should be ok if
    // there is only instance of them.
    sci::PropertyValue PropertyValue;
    sci::VariableDecl VariableDecl;

private:
    std::string _error;
    streamIt _beginning;

    sci::Script &_script;
    std::string _scratch;
    std::string _scratch2;
    std::stack<sci::SyntaxNode*> _statements;
};

// Our parser...
typedef ParserBase<SyntaxContext, streamIt> Parser;

class ICompileLog; // fwd decl

class IReportError
{
public:
    virtual void ReportError(PCTSTR pszError) = 0;
};

class SCISyntaxParser
{
public:
    bool Parse(sci::Script &script, CCrystalScriptStream::const_iterator &stream, ICompileLog *pError);
    bool Parse(sci::Script &script, CCrystalScriptStream::const_iterator &stream, SyntaxContext &context);
    bool ParseHeader(sci::Script &script, CCrystalScriptStream::const_iterator &stream, ICompileLog *pError);
    void Load();

private:
    bool _fLoaded;

    // Grammar rules

    Parser statement;
    Parser immediateValue;
    Parser general_token;
    Parser pointer;
    Parser selector;
    Parser value;
    Parser simple_value;
    Parser property_value;
    Parser property_decl;
    Parser properties_decl;
    Parser array_init;

    // Operators
    Parser binary_operator;
    Parser unary_operator;
    Parser assignment_operator;
    // Variable declaration
    Parser var_decl;

    // General code pieces
    Parser variable;
    Parser base_conditional;
    Parser conditional;
    Parser do_loop;
    Parser while_loop;
    Parser if_statement;
    Parser else_clause;
    Parser for_loop;
    Parser case_statement;
    Parser default_statement;
    Parser switch_statement;
    Parser break_statement;
    Parser rest_statement;
    Parser return_statement;
    // Force the oppar to come right after the alphanum_token, to eliminate ambiguity
    // = gWnd clBlack (send gEgo:x)        = gWnd Print("foo")         (= button (+ 5 5))
    Parser procedure_call;
    Parser send_param_call;
    Parser send_call;
    Parser code_block;
    Parser assignment;
    Parser binary_operation;
    Parser unary_operation;

    // Functions
    Parser function_var_decl;
    Parser method_base;
    Parser method_decl;
    Parser procedure_base;

    // Classes
    Parser classbase_decl;
    Parser instancebase_decl;

    // Main script section.
    Parser include;
    Parser use;
    Parser define;
    Parser scriptNum;
    Parser instance_decl;
    Parser class_decl;
    Parser procedure_decl;
    Parser synonyms;
    Parser script_var;
    Parser script_string;
  
    Parser entire_script;
    Parser entire_header;


    // Additional ones required for c++ syntax
    Parser expression_statement;
    Parser argument_expression_list;
    Parser expression;
    Parser assignment_expression;
    Parser assignment_expression_core;
    Parser lvalue_expression;
    Parser rvalue_expression;
    Parser unary_expression;
    Parser unary_expression_core;
    Parser conditional_expression;
    Parser logical_or_expression;
    Parser logical_and_expression;
    Parser inclusive_or_expression;
    Parser primary_expression;
    Parser postfix_expression;
    Parser cast_expression;
    Parser multiplicative_expression;
    Parser additive_expression;
    Parser shift_expression;
    Parser relational_expression;
    Parser equality_expression;
    Parser and_expression;
    Parser exclusive_or_expression;
    Parser parameter;
    Parser labeled_statement;
    Parser selection_statement;
    Parser iteration_statement;
    Parser for_loop_initializer;
    Parser for_loop_condition;
    Parser for_loop_looper;
    Parser jump_statement;
    Parser array_index;
    Parser method_call;
    Parser function_call;
    Parser property_access;
    Parser postfix_expression_root;
    Parser statement_list;
};

