#include "stdafx.h"
#include "ClassBrowser.h"
#include "Compile\ScriptOMAll.h"
#include "Compile\SCISyntaxParser.h"
#include "CodeToolTips.h"
#include "SCIPicEditor.h"
#include <string>

//
// CodeToolTips.cpp
//
// Figures out the tooltip to be used when the user hovers over certain pieces of code.
//

using namespace sci;
using namespace std;

//
// Fills szBuf with a textual representation of the method.
//
void _GetMethodInfoHelper(PTSTR szBuf, size_t cchBuf, const FunctionBase *pMethod)
{
    // TODO: needs updating due to pMethod->GetParams()
    /*
    StringCchPrintf(szBuf, cchBuf, TEXT("%s("), pMethod->GetName().c_str());
    const std::vector<std::string> &params = pMethod->GetParams();
    for (size_t iParam = 0; iParam < params.size(); iParam++)
    {
        StringCchCat(szBuf, cchBuf, params[iParam].c_str());
        if (iParam < (params.size() - 1))
        {
            StringCchCat(szBuf, cchBuf, TEXT(" ")); // add a space
        }
    }
    StringCchCat(szBuf, cchBuf, TEXT(")"));*/
}

void _GetClassInfoHelper(PTSTR szBuf, size_t cchBuf, const ClassDefinition *pClass)
{
    TCHAR szTemp[100];
    szTemp[0] = 0;
    if (!pClass->GetSuperClass().empty())
    {
        StringCchPrintf(szTemp, ARRAYSIZE(szTemp), TEXT(" of %s"), pClass->GetSuperClass().c_str());
    }
    StringCchPrintf(szBuf, cchBuf, TEXT("(class %s%s\n    (properties\n"), pClass->GetName().c_str(), szTemp);
    const ClassPropertyVector &properties = pClass->GetProperties(); // STLCLEANUP
    for (size_t iProp = 0; iProp < properties.size(); iProp++)
    {
        TCHAR szProp[100];
        const ClassProperty *pProp = properties[iProp];
        StringCchPrintf(szProp, ARRAYSIZE(szProp), TEXT("        %s = %s\n"), pProp->GetName().c_str(), pProp->GetValue().ToString().c_str());
        StringCchCat(szBuf, cchBuf, szProp);
    }
    StringCchCat(szBuf, cchBuf, TEXT("    )\n"));

    // Prepare methods
    TCHAR szMethodPart[1000];
    StringCchPrintf(szMethodPart, ARRAYSIZE(szMethodPart), TEXT("Methods:\n"));
    const MethodVector &methods = pClass->GetMethods();
    for (size_t iMethod = 0; iMethod < methods.size(); iMethod++)
    {
        TCHAR szMethod[100];
        const MethodDefinition *pMethod = methods[iMethod];
        StringCchCat(szMethodPart, ARRAYSIZE(szMethodPart), TEXT("    "));
        _GetMethodInfoHelper(szMethod, ARRAYSIZE(szMethod), pMethod);
        StringCchCat(szMethodPart, ARRAYSIZE(szMethodPart), szMethod);
        StringCchCat(szMethodPart, ARRAYSIZE(szMethodPart), TEXT("\n"));
    }
    StringCchCat(szMethodPart, ARRAYSIZE(szMethodPart), TEXT(")"));
    StringCchCat(szBuf, cchBuf, szMethodPart);
}

