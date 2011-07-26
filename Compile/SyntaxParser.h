#pragma once

#include "SCISyntaxParser.h"
#include "CPPSyntaxParser.h"

class SyntaxParser
{
public:
    bool Parse(sci::Script &script, CCrystalScriptStream &stream, ICompileLog *pLog = NULL, bool fParseComments = false);

private:
    SCISyntaxParser _sci;
    CPPSyntaxParser _cpp;
};

extern SyntaxParser g_Parser;
