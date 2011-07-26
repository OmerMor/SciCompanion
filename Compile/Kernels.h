#pragma once

#include "ScriptOM.h"

static const WORD KernelScriptNumber = 0xfffe;

class ICompileContext;
class CSCOPublicExport;

sci::Script &GetKernelSignaturesScript(ICompileLog &log);

CSCOPublicExport GetKernelSCO(ICompileContext &context, const std::string &name);



