#include "stdafx.h"
#include "Kernels.h"
#include "ScriptOM.h"
#include "ScriptOMAll.h"
#include "SyntaxParser.h"
#include "..\CrystalScriptStream.h"
#include "..\SCIPicEditor.h"
#include "CompileContext.h"
#include "SCO.h"
#include <boost/foreach.hpp>

sci::Script g_kernelScript;
bool g_fKernelLoaded = false;
stdext::hash_map<std::string, CSCOPublicExport> _scoKernels;
bool g_fSCOKernelsLoaded = false;

sci::Script &GetKernelSignaturesScript(ICompileLog &log)
{
    if (!g_fKernelLoaded)
    {
        g_fKernelLoaded = true;
        ScriptId scriptId(theApp.GetResourceMap().GetIncludePath("kernels.scp"));
        g_kernelScript.SetScriptId(scriptId);
        ASSERT(scriptId.Language() == LangSyntaxCpp);

        CCrystalTextBuffer buffer;
        if (buffer.LoadFromFile(scriptId.GetFullPath().c_str()))
        {
            CScriptStreamLimiter limiter(&buffer);
            CCrystalScriptStream stream(&limiter);
            if (g_Parser.Parse(g_kernelScript, stream, &log))
            {
                g_kernelScript.SetScriptNumber(KernelScriptNumber);
            }
            else
            {
                ASSERT(FALSE);
            }
            buffer.FreeAll();
        }
    }
    return g_kernelScript;
}

// ASSERTs and makes code prettier.
SpeciesIndex _GetSpeciesIndex(ICompileContext &context, const std::string &typeName)
{
    SpeciesIndex si = DataTypeAny;
    bool fResult = context.LookupTypeSpeciesIndex(typeName, si);
    ASSERT(fResult); // Everything in the kernel file should be known
    return si;
}

CSCOPublicExport GetKernelSCO(ICompileContext &context, const std::string &name)
{
    sci::Script &script = GetKernelSignaturesScript(context);
    CSCOPublicExport scoSigs;
    if (g_fKernelLoaded)
    {
        if (!g_fSCOKernelsLoaded)
        {
            CompileTables tables;
            tables.Load();
            CompileLog log;
            CompileResults results(log);
            PrecompiledHeaders headers(theApp.GetResourceMap());
            if (GenerateScriptResource(script, headers, tables, results))
            {
                BOOST_FOREACH(const CSCOPublicExport &export, results.GetSCO().GetExports())
                {
                    _scoKernels[export.GetName()] = export;
                }
                g_fSCOKernelsLoaded = true;
            }
            else
            {
                context.ReportResult(CompileResult("Couldn't load kernels", CompileResult::CRT_Error));
                BOOST_FOREACH(CompileResult &result, log.Results())
                {
                    context.ReportResult(result);    
                }
            }
        }
        if (g_fSCOKernelsLoaded)
        {
            ASSERT(_scoKernels.find(name) != _scoKernels.end());
            scoSigs = _scoKernels[name];
        }
    }
    return scoSigs;
}