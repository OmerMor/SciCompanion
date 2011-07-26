#include "stdafx.h"
#include "SyntaxParser.h"

// Our parser global variable
SyntaxParser g_Parser;

bool SyntaxParser::Parse(sci::Script &script, CCrystalScriptStream &stream, ICompileLog *pLog, bool fParseComments)
{
    bool fRet = false;
    if (script.Language() == LangSyntaxSCIStudio)
    {
        _sci.Load();
        if (script.IsHeader())
        {
            fRet = _sci.ParseHeader(script, stream.begin(), pLog);
        }
        else
        {
            fRet = _sci.Parse(script, stream.begin(), pLog);
        }
    }
    else if (script.Language() == LangSyntaxCpp)
    {
        _cpp.Load();

        // Tokenize it first.
        CrystalScriptTokenStream tokenStream(stream);
        tokenStream.Tokenize(fParseComments ? &script : NULL);
        if (script.IsHeader())
        {
            fRet = _cpp.ParseHeader(script, tokenStream.begin(), pLog);
        }
        else
        {
            fRet = _cpp.Parse(script, tokenStream.begin(), pLog);
        }
    }
    else
    {
        ASSERT(FALSE);
    }
    return fRet;
}
