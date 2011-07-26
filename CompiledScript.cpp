#include "stdafx.h"
#include "SCIPicEditor.h"
#include "CompiledScript.h"
#include "Decompiler.h"

#include "Compile\ScriptOM.h" // Just for DebugStringOut...

using namespace std;
using namespace sci;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Turn on/off decompiler, which is a work in progress.
#define DECOMPILE

#define STATE_CALCBRANCHES 0
#define STATE_OUTPUT 1

//#define TEST_OFFSET 0x000c // Pretend the script was loaded here.
#define TEST_OFFSET 0

//
// Returns the bounds of a function starting at codeStart, in terms of a CodeSection
// codeStart is an iterator into codePointersTo, which is a list of beginnings of functions, methods, etc...
//
bool _FindStartEndCode(set<WORD>::const_iterator codeStart, const set<WORD> &codePointersTO, const vector<CodeSection> &codeSections, CodeSection &sectionOut)
{
    sectionOut.begin = *codeStart;
    // That was easy.  Now find the CodeSection this is in.
    CodeSection containingSection;
    bool fFound = false;
    for (size_t i = 0; !fFound && i < codeSections.size(); i++)
    {
        if ((sectionOut.begin >= codeSections[i].begin) && (sectionOut.begin < codeSections[i].end))
        {
            fFound = true;
            containingSection = codeSections[i];
        }
    }
    if (fFound)
    {
        ++codeStart;
        if (codeStart != codePointersTO.end())
        {
            sectionOut.end = min(*codeStart, containingSection.end);
        }
        else
        {
            sectionOut.end = containingSection.end;
        }
    }
    return fFound;
}

void _GetVarType(ostream &out, BYTE bOpcode, WORD wIndex, IObjectFileScriptLookups *pOFLookups)
{
    // This is a 0-127 opcode.
    // Use the lowest two bits to determine the var type
    switch (bOpcode & 0x03)
    {
    case 0:
        // Get the global's name if possible
        if (pOFLookups)
        {
            out << pOFLookups->ReverseLookupGlobalVariableName(wIndex);
        }
        else
        {
            out << "global" << setw(4) << setfill('0') << wIndex;
        }
        break;
    case 1:
        out << "local" << setw(4) << setfill('0') << wIndex;
        break;
    case 2:
        out << "temp" << setw(4) << setfill('0') << wIndex;
        break;
    case 3:
        if (wIndex == 0)
        {
            // parameter 0 is the count of parameters.
            out << "paramTotal";
        }
        else
        {
            out << "param" << setw(4) << setfill('0') << wIndex;
        }
        break;
    }
}

std::string _GetProcNameFromScriptOffset(WORD wOffset)
{
    std::stringstream ss;
    ss << "localproc_" << wOffset;
    return ss.str();
}

sci::istream _GetScriptData(int iScript)
{
    sci::istream byteStreamRet;
    ResourceEnumerator *pEnum;
    if (SUCCEEDED(theApp.GetResourceMap().CreateEnumerator(NULL, ResourceTypeFlagScript, &pEnum)))
    {
        ResourceBlob *pData;
        bool fFound = false;
        while (!fFound && (S_OK == pEnum->Next(&pData)))
        {
            if (pData->GetNumber() == iScript)
            {
                byteStreamRet = pData->GetReadStream();
                fFound = true;

                // The following code was for modifying the script to fake loading at a different offset
                // (to simulate where it would be loaded during the game)
                /*
                fFound = true;
                ASSERT(pStream == NULL);
                sci::istream byteStream = pData->GetReadStream();
                WORD wType, wSectionSize;
                bool fFoundReloc = false;
                while (!fFoundReloc && byteStream.good())
                {
                    byteStream >> wType;
                    byteStream >> wSectionSize;
                    if (byteStream.good())
                    {
                        if (wType == 8)
                        {
                            fFoundReloc = true;
                            break;
                        }
                        else if (wType == 0)
                        {
                            break;
                        }
                        else
                        {
                            byteStream.skip(wSectionSize - 4); // Go to next section
                        }
                    }
                }

                if (fFoundReloc)
                {
                    if ((wSectionSize - 4) > 0)
                    {
                        byteStream >> wNumPointers;
                        for (WORD i = 0; byteStream.good() && (i < wNumPointers); i++)
                        {
                            WORD w;
                            byteStream >> w;
                            // Modify the address at that location:
                        }
                    }
                }*/
            }

            delete pData;
        }
        delete pEnum;
    }
    return byteStreamRet;
}


BOOL CompiledScript::Load(int iScriptNumber)
{
    _wScript = (WORD)iScriptNumber;
    BOOL fRet = FALSE;
    sci::istream byteStream(_GetScriptData(iScriptNumber));
    fRet = Load(byteStream);
    return fRet;
}

bool CompiledScript::_IsInCodeSection(WORD wOffset)
{
    for (size_t i = 0; i < _codeSections.size(); i++)
    {
        if ((wOffset >= _codeSections[i].begin) && (wOffset < _codeSections[i].end))
        {
            return true;
        }
    }
    return false;
}

bool CompiledScript::Load(sci::istream &byteStream)
{
    bool fRet = true;
    // Make a copy of everything
    _scriptResource.resize(byteStream.GetDataSize());
    byteStream.read_data(&_scriptResource[0], byteStream.GetDataSize());
    // Then go back to the beginning
    byteStream.seekg(0);

    // Note: KQ4 has a different script resource format (it's interpreter is an older SCI0)
    // There is a WORD at the beginning, but there are more differences than that too.
    while (fRet)
    {
        DWORD dwSavePos = byteStream.tellg();
        // Read the type and size.
        WORD wType;
        WORD wSectionSize;
        byteStream >> wType;
        if (wType != 0)
        {
            byteStream >> wSectionSize;
            fRet = byteStream.good() && (wSectionSize >= 4);
            if (fRet)
            {
                switch (wType)
                {
                case 1:
                    {
                        // instance
                        CompiledObjectBase *pObject = new CompiledObjectBase;
                        WORD wInstanceOffsetTO;
                        fRet = pObject->Create(byteStream, FALSE, &wInstanceOffsetTO);
                        if (fRet)
                        {
                            _objectsOffsetTO.push_back(wInstanceOffsetTO);
                            _objects.push_back(pObject);
                        }
                        else
                        {
                            delete pObject;
                        }
                    }
                    break;

                case 2:
                    {
                        WORD wCodeLength = wSectionSize - 4;
                        WORD wCodePosTO = static_cast<WORD>(byteStream.tellg());
                        byteStream.seekg(wCodeLength); // Skip this section.
                        CodeSection section = { wCodePosTO, wCodePosTO + wCodeLength };
                        _codeSections.push_back(section);
                    }
                    break;

                case 3:
                    {
                        // Synonym word lists.
                        // WORD A - wordgroup,
                        // WORD B - wordgroup, replacement for a
                        WORD wNumPairs = ((wSectionSize - 4) / 4);
                        for (WORD i = 0; fRet && i < wNumPairs; i++)
                        {
                            WORD wA;
                            byteStream >> wA;
                            WORD wB;
                            byteStream >> wB;
                            _synonyms[wA] = wB;
                            fRet = byteStream.good();
                        }
                    }
                    break;

                case 4:
                    {
                        // Said specs
                        fRet = _ReadSaids(byteStream, wSectionSize - 4);
                    }
                    break;

                case 5:
                    {
                        // Strings
                        fRet = _ReadStrings(byteStream, wSectionSize - 4);
                    }
                    break;

                case 6:
                    {
                        // class
                        CompiledObjectBase *pObject = new CompiledObjectBase;
                        WORD wClassOffset;
                        fRet = pObject->Create(byteStream, TRUE, &wClassOffset);
                        if (fRet)
                        {
                            _objectsOffsetTO.push_back(wClassOffset);
                            _objects.push_back(pObject);
                        }
                        else
                        {
                            delete pObject;
                        }
                    }
                    break;
                case 7:
                    {
                        // Just what are the exports table?  Why are there entries in scripts that have no exports???
                        // Because they have a room, which is a public instance.  Unclear what this is supposed to point
                        // to though... it appears to point to the species selector.
                        // (also: magic entry point for script 0, entry 0, the play method.
                        fRet = _ReadExports(byteStream);
                    }
                    break;

                case 8:
                    {
                        // Relocation table.
                        // This is a list of script-relative pointers to pointers in the script.  These are how the
                        // interpreter tells what it needs to adjust when loading a script.
                        // This begins with a WORD that indicates how many pointers there are here.
                        // Brian's compiler seems to use these for the strings, but not for other things like Saids.
                        // Why would they be needed for strings?  It seems like they are only used for the name
                        // selectors?
                        // But the main script has 57 of these entries, but only 42 strings.  So clearly they are
                        // used for more than just strings.
                        // If I compile main, I get 34 offset strings (42 - 8 that aren't object names) and 23 offset properties,
                        // which were the said strings used in properties.
                        // So it seems like it's used for object names, and things that are property values (said, string and var)
                        // That means you can assign local variables to properties.  e.g.  "x @myLocalVar"
                        // (Doesn't work with globals though.)
                        // I guess everywhere else we can use relative offsets!
                    }
                    break;

                case 9:
                    {
                        _fPreloadText = TRUE;
                    }
                    break;

                case 10:
                    {
                        // Local variables.
                        // Just a series of WORDs.  The notion of arrays, etc... is not encoded in the resource.
                        // That's all just in source code.  Although arrays can kind of be infered from which ones
                        // are accessed in the script.
                        WORD wNumVars = ((wSectionSize - 4) / 2);
                        for (WORD i = 0; fRet && i < wNumVars; i++)
                        {
                            WORD w;
                            byteStream >> w;
                            fRet = byteStream.good();
                            _localVars.push_back(w);
                        }
                    }
                    break;
                default:
                    // Nada.
                    break;
                }
            }
        }
        if (wType == 0)
        {
            break; // Done
        }
        if (fRet)
        {
            ASSERT(wSectionSize > 0); // else we'll never get anywhere.
            if (wSectionSize > 0)
            {
                byteStream.seekg(dwSavePos + wSectionSize);
                fRet = byteStream.good();
            }
        }
    }
    return fRet;
}

// Pretend the code has been loaded at an address other than 0x0000
WORD _GetTestStreamPosition(sci::istream *pStream)
{
    return (WORD)(pStream->GetPosition()) + TEST_OFFSET;
}

bool CompiledObjectBase::Create(sci::istream &stream, BOOL fClass, WORD *pwOffset)
{
    *pwOffset = static_cast<WORD>(stream.tellg());
    _fInstance = !fClass;
    WORD wMagic;
    stream >> wMagic; //  ASSERT(wMagic == 0x1234);
    if (wMagic != 0x1234)
    {
        return false; // We'll hit this when loading KQ4 for example, which uses a different format
    }
    if (stream.good())
    {
        _wPosInResource = static_cast<WORD>(stream.tellg());
        WORD wLocalVarOffset;
        stream >> wLocalVarOffset;
        WORD wFunctionSelectorOffset;
        stream >> wFunctionSelectorOffset;
        WORD wNumVarSelectors;
        stream >> wNumVarSelectors;
        ASSERT(wNumVarSelectors >= 3); // Really 4, but iceman only has 3
        if (stream.good())
        {
            WORD wNumVarValuesLeft = wNumVarSelectors;
            while (stream.good() && wNumVarValuesLeft)
            {
                WORD wValue;
                stream >> wValue;
                if (stream.good())
                {
                    _propertyValues.push_back(wValue);
                }
                wNumVarValuesLeft--;
            }
        }
        WORD wName = 0;
        if (wNumVarSelectors >= 3)
        {
            _wSpecies = _propertyValues[0];
            _wSuperClass = _propertyValues[1];
            _wInfo = _propertyValues[2];
            if (wNumVarSelectors >= 4)
            {
                wName = _propertyValues[3];
            }
        }

        // Now read their selector IDs - but only if this is a class.
        // Instances don't need this, since all the selector IDs are defined by its class.
        if (stream.good() && fClass)
        {
            // If this is a class, it's now followed by selector IDs, matching the selectors.
            for (INT_PTR i = 0; stream.good() && i < (INT_PTR)wNumVarSelectors; i++)
            {
                WORD wSelectorID;
                stream >> wSelectorID;
                if (stream.good())
                {
                    _propertySelectors.push_back(wSelectorID);
                }
            }
        }

        // Now their function selectors, for both instances and classes.
        WORD wNumFunctionSelectors = 0;
        if (stream.good())
        {
            // Read function selectors.
            stream >> wNumFunctionSelectors;
            WORD wNumFunctionSelectorsLeft = wNumFunctionSelectors;

            //
            // The spec here is incorrect.
            // http://freesci.linuxgames.com/scihtml/c2890.html#AEN2894
            // For classes, it describes the function code pointers coming before the selectors for the functions.
            // But the selectors come before for both classes and instances.
            //
            while(stream.good() && wNumFunctionSelectorsLeft)
            {
                WORD wSelectorID;
                stream >> wSelectorID;
                if (stream.good())
                {
                    _functionSelectors.push_back(wSelectorID);
                }
                wNumFunctionSelectorsLeft--;
            }
        }

        // Now the method code pointers.
        WORD wZero;
        stream >> wZero;
        if (stream.good())
        {
            ASSERT(wZero == 0); // There is supposed to be a zero here.
            while (stream.good() && wNumFunctionSelectors)
            {
                WORD wPtr;
                stream >> wPtr;
                if (stream.good())
                {
                    // These are supposed to be offsets to within the script resource, so they
                    // had better be smaller!
                    ASSERT(stream.GetDataSize() > wPtr);
                    _functionOffsetsTO.push_back(wPtr + TEST_OFFSET);
                }
                wNumFunctionSelectors--;
            }
        }

        if (stream.good() && (wName != 0))
        {
            // Retrieve the name of the object.  wName is a pointer.
            DWORD dwSavePos = stream.tellg();
            stream.seekg(wName);
            stream >> _strName;
            // Restore
            stream.seekg(dwSavePos);
        }

        // The rest of the stuff we don't care about!
    }
    return stream.good();
}


bool CompiledScript::_ReadExports(sci::istream &stream)
{
    // These aren't really interesting actually.
    WORD wNumExports;
    stream >> wNumExports;
    if (stream.good())
    {
        for (WORD i = 0; stream.good() && i < wNumExports; i++)
        {
            WORD wOffset;

            stream >> wOffset;
            if (stream.good())
            {
                _exportsTO.push_back(wOffset + TEST_OFFSET);
            }
        }
    }
    return stream.good();
}

bool CompiledScript::_ReadSaids(sci::istream &stream, WORD wDataSize)
{
    // A single Said spec consists of wordgroups (0x0000-0x0fff) and BYTE tokens (0xf0-0xf9),
    // terminated by 0xff.  I don't think there is any count of them.  But there is a count of
    // the total length of all of them of course.
    // We pass the offset of this thing to callk Said.  This would be a program-counter-relative
    // offset.
    DWORD dwMaxPos = stream.tellg() + wDataSize;
    while (stream.good() && (stream.tellg() < (dwMaxPos - 1))) // -1 since we need at least a little more data to read...
    {
        // Store the actual position in the stream (this is how other parts of the script refer to it).
        WORD wBeginingOfSaid = static_cast<WORD>(stream.tellg());
        vector<WORD> saidSequence;
        bool fDone = false;
        do
        {
            BYTE b;
            stream >> b;
            //fRet = pStream->ReadByte(&b);
            if (stream.good())
            {
                fDone = (b == 0xff);
                if (!fDone)
                {
                    if (b >= 0xf0)
                    {
                        // It's a operator.
                        saidSequence.push_back((WORD)b);
                    }
                    else
                    {
                        // It's a word group.
                        BYTE b2;
                        stream >> b2;
                        //fRet = pStream->ReadByte(&b2);
                        if (stream.good())
                        {
                            saidSequence.push_back((((WORD)b) << 8) | b2);
                        }
                    }
                }
            }
        } while (stream.good() && !fDone);
        if (stream.good())
        {
            _saidsOffset.push_back(wBeginingOfSaid);
            _saids.push_back(saidSequence);
        }

    } 

    // Just return TRUE.  Some of the code generated by brian's compiler seems to have an addition byte
    // in the overall length, so 0xff doesn't correspond to end the of the data stream (presumably so it ends up on a WORD boundary)
    return true;
}

bool CompiledScript::_ReadStrings(sci::istream &stream, WORD wDataSize)
{
    // I have a feeling these may map to objects, which would be great for us.
    // Look up a string, and then its index corresponds to an object?
    DWORD dwMaxPos = stream.tellg() + wDataSize;
    while (stream.good() && (stream.tellg() < dwMaxPos))
    {
        string str;
        DWORD dwOffset = stream.tellg();
        stream >> str;
        if (stream.good())
        {
            ASSERT(dwOffset <= 0xffff);
            _stringsOffset.push_back(static_cast<WORD>(dwOffset));
            _strings.push_back(str);
        }
    }
    return stream.good();
}


//
// Calculate the offset wRelOffset from wHere, where wHere is the current location
// of the last operand in an instruction.  1 or 2 will be added to it prior to
// calculating the offset.
//
WORD CalcOffset(WORD wOperandStart, WORD wRelOffset, bool bByte, BYTE bRawOpcode)
{
    // Move to the pc post instruction.
    WORD wResult = wOperandStart + OpArgs[bRawOpcode];
    if (bByte)
    {
        if (wRelOffset >= 0x0080)
        {
            // It was a negative number, expressed in a byte.
            // We need to "sign extend" this thing.
            wRelOffset |= 0xff00;
        }
        wResult += wRelOffset;
    }
    else
    {
        wResult += wRelOffset; // Should just work
    }
    return wResult;
}

int GetOperandSize(BYTE bOpcode, OperandType operandType)
{
    int cIncr = 0;
    switch (operandType)
    {
    case otEMPTY:
        cIncr = 0;
        break;
    case otVAR:
    case otPVAR:
    case otCLASS:
    case otPROP:
    case otSTRING:
    case otSAID:
    case otKERNEL:
    case otLABEL:
    case otPUBPROC:
    case otINT:
    case otOFFS:
        cIncr = (bOpcode & 1) ? 1 : 2;
        break;
    case otINT16:
        cIncr = 2;
        break;
    case otINT8:
        cIncr = 1;
        break;
    }
    return cIncr;
}

void _Disassemble(ostream &out, ICompiledScriptLookups *pLookups, IObjectFileScriptLookups *pOFLookups, ICompiledScriptSpecificLookups *pScriptThings, ILookupPropertyName *pPropertyNames, const BYTE *pBegin, const BYTE *pEnd, WORD wBaseOffset)
{
    try
    {
        vector<WORD> codeLabelOffsets; // Keep track of places that are branched to.
        for (int state = 0; state < 2; state++)
        {
            const BYTE *pCur = pBegin;
            WORD wOffset = wBaseOffset;
            int iCurLabelOffset = 0; // for STATE_CALCBRANCHES
            while (pCur < pEnd) // Possibility of read AVs here, but we catch exceptions.
            {
                BYTE bRawOpcode = *pCur;
                BYTE bOpcode = bRawOpcode >> 1;
                
                ASSERT(bOpcode < 128);
                const char *pszOpcode = OpcodeNames[bOpcode];
                bool bByte = (*pCur) & 1; // Is this a "byte" opcode or a "word" opcode.
                bool fDone = false;

                char szBuf[50];
                size_t cchBufLimit = 30;
                ASSERT(cchBufLimit < ARRAYSIZE(szBuf));
                if (state == STATE_OUTPUT)
                {
                    if (codeLabelOffsets.size() > 0 && (codeLabelOffsets[iCurLabelOffset] == wOffset))
                    {
                        // We're at label.
                        out << endl << "        code_" << setw(4) << setfill('0') << wOffset << endl;
                        iCurLabelOffset++;
                    }
                    out << "  " << setw(4) << setfill('0') << wOffset << ":";
                    int indent = 22;
                    const BYTE *pCurTemp = pCur; // skip past opcode
                    for (int i = -1; i < 3; i++)
                    {
                        int cIncr = (i == -1) ? 1 : GetOperandSize(bRawOpcode, OpArgTypes[bOpcode][i]);
                        if (cIncr == 0)
                        {
                            break;
                        }                    
                        else
                        {
                            WORD wOperandTemp = (cIncr == 2) ? *((WORD*)pCurTemp) : *pCurTemp;
                            out << setw((cIncr == 1) ? 2 : 4);
                            out << setfill('0') << wOperandTemp << " ";
                            pCurTemp += cIncr;
                            indent  -= (cIncr == 1) ? 3 : 5; // How many chars were written...
                        }
                    }
                    ASSERT(indent > 0);
                    out << setw(indent) << setfill(' ') << pszOpcode << " "; // Indent x chars, and print opcode
                }


                pCur++; // skip past opcode
                wOffset++;
                WORD wOperandStart = wOffset;

                if (state == STATE_CALCBRANCHES)
                {
                    // Keep track of the targets of branch instructions
                    if ((bOpcode == acBNT) || (bOpcode == acBT) || (bOpcode == acJMP))
                    {
                        // This is a branch instruction.  Figure out the offset.
                        // The relative offset is either a byte or word, and is calculated post instruction
                        // (hence we add 1 or 2 to our calculation)
                        codeLabelOffsets.push_back(CalcOffset(wOperandStart, (bByte ? ((WORD)*pCur) : (*((WORD*)pCur))), bByte, bRawOpcode));
                    }
                }

                WORD wOperands[3];
                for (int i = 0; !fDone && i < 3; i++)
                {
                    szBuf[0] = 0;
                    int cIncr = GetOperandSize(bRawOpcode, OpArgTypes[bOpcode][i]);
                    if (cIncr == 0)
                    {
                        break;
                    }
                    if (state == STATE_OUTPUT)
                    {
                        wOperands[i] = (cIncr == 2) ? *((WORD*)pCur) : *pCur;
                        switch (OpArgTypes[bOpcode][i])
                        {
                        case otINT:
                        case otINT16:
                        case otINT8:
                            out << wOperands[i];
                            break;
                        case otKERNEL:
                            out << pLookups->LookupKernelName(wOperands[i]);
                            break;
                        case otPUBPROC:
                            out << "procedure_" << setw(4) << setfill('0') << wOperands[i];
                            break;
                        case otSAID:
                            out << "said_" << setw(4) << setfill('0') << wOperands[i];
                            break;
                        case otOFFS:
                            // This is a bit of a hack here.  We're making the assumption that a otOFFS parameter
                            // is the one and only parameter for this opcode.  CalcOffset makes this assumption
                            // in order to calculate the offset.
                            ASSERT(OpArgTypes[bOpcode][i+1] == otEMPTY);
                            // Kind of hokey, we reassign the operand here.
                            wOperands[i] = CalcOffset(wOperandStart, wOperands[i], bByte, bRawOpcode);
                            out << "$" << setw(4) << setfill('0') << wOperands[i];
                            break;
                        case otEMPTY:
                            break;

                        case otPROP:
                            // This value is an offset from the beginning of this object's species
                            // So, get the species, and then divide this value by 2, and use it as an index into its
                            // selector thang.
                            //
                            if (pPropertyNames)
                            {
                                out << pPropertyNames->LookupPropertyName(pLookups, wOperands[i]);
                            }
                            else
                            {
                                out << "**ENCOUNTERED PROPERTY OPCODE IN NON-OBJECT CODE**";
                            }
                            break;

                        case otCLASS:
                            out << pLookups->LookupClassName(wOperands[i]);
                            break;

                        case otVAR:
                            _GetVarType(out, bOpcode, wOperands[i], pOFLookups);
                            break;

                        case otLABEL:
                            // This is a relative position from the post pc
                            out << ((bOpcode == acCALL) ? "proc" : "code") << "_" << setw(4) << setfill('0') << CalcOffset(wOperandStart, wOperands[i], bByte, bRawOpcode);
                            break;

                        default:
                            out << "$" << setw(4) << setfill('0') << wOperands[i];
                            break;
                        }
                        out << " ";
                    }
                    pCur += cIncr;
                    wOffset += cIncr;
                }
                if (state == STATE_OUTPUT)
                {
                    // Time for comments (for some instructions)
                    szBuf[0] = 0;
                    switch (bOpcode)
                    {
                    case acLINK:
                        out << "// (var $" << wOperands[0] << ")";
                        break;

                    case acLOFSS:
                    case acLOFSA:
                        // This is an offset... it could be an interesting one, like a string or said.
                        ICompiledScriptSpecificLookups::ObjectType type;
                        out << "// " << pScriptThings->LookupObjectName(wOperands[0], type);
                        break;

                    case acPUSHI:
                        out << "// $" << wOperands[0] << " " << pLookups->LookupSelectorName(wOperands[0]);
                        break;
                        // could do it for push0, push1, etc..., but it's just clutter, and rarely the intention.

                    case acCALLE:
                    case acCALLB:
                        // Try to get the public export name.
                        if (pOFLookups)
                        {
                            WORD wScript;
                            WORD wIndex;
                            if (acCALLB == bOpcode)
                            {
                                wScript = 0;
                                wIndex = wOperands[0];
                            }
                            else
                            {
                                wScript = wOperands[0];
                                wIndex = wOperands[1];
                            }
                            out << "// " << pOFLookups->ReverseLookupPublicExportName(wScript, wIndex) << " ";
                        }
                        break;

                    }

                    out << endl;
                    switch (bOpcode)
                    {
                    case acSEND:
                    case acCALL:
                    case acCALLB:
                    case acCALLE:
                    case acCALLK:
                    case acSELF:
                    case acSUPER:
                        // Add another carriage return after these instructions
                        out << endl;
                        break;
                    }
                }
            }

            if (state == STATE_CALCBRANCHES)
            {
                sort(codeLabelOffsets.begin(), codeLabelOffsets.end());
            }
        }
    }
    catch (...)
    {
        // In case we read more than there was.
        theApp.LogInfo("Error while disassembling script.");
    }
}


//
// Scan for direct access to local variables, to find out if they are arrays or not.
//
/*
void _ScanForLocalVarUsage(BYTE *rgRawCode, WORD wCodeBase, WORD wCodeLength)
{
    // REVIEW: coalesce this code with _FindInternalCalls
    WORD wCurrentOffset = wCodeBase;
    BYTE *pCur = rgRawCode;
    BYTE *pEnd = pCur + wCodeLength;
    while (pCur < pEnd)
    {
        WORD wRelOffset;
        BYTE bRawOpcode = *pCur;
        BYTE bOpcode = bRawOpcode >> 1;
        bool bByte = (bRawOpcode & 1);
        pCur++;
        wCurrentOffset++;
        if (OpArgTypes[bOpcode][0] == otVAR)
        {
            if ((bOpcode & 0x03) == 1)
            {
                // This was a local var instruction
                WORD wIndex = (bByte ? (WORD)*pCur : *((WORD*)pCur));

            }
        }
        // Skip past to the next instruction
        pCur += OpArgs[bRawOpcode];
        wCurrentOffset += OpArgs[bRawOpcode];
    }
}*/


//
// Scan all the code in the script, looking for call instructions
//
set<WORD> CompiledScript::_FindInternalCallsTO()
{
    set<WORD> wOffsets;
    for (size_t i = 0; i < _codeSections.size(); i++)
    {
        WORD wCurrentOffsetTO = _codeSections[i].begin;
        BYTE *pCur = &_scriptResource[wCurrentOffsetTO];
        if (pCur)
        {
            BYTE *pEnd = &_scriptResource[_codeSections[i].end];
            while (pCur < pEnd)
            {
                WORD wRelOffset;
                BYTE bRawOpcode = *pCur;
                bool bByte = (bRawOpcode & 1);
                pCur++;
                wCurrentOffsetTO++;
                if ((bRawOpcode >> 1) == acCALL)
                {
                    // This is one. The first operand is a word or byte
                    wRelOffset = (bByte ? ((WORD)*pCur) : (WORD)*pCur + (((WORD)*(pCur + 1)) << 8));
                    //wOffsets.push_back(CalcOffset(wCurrentOffset, wRelOffset, bByte, bRawOpcode));
                    wOffsets.insert(wOffsets.end(), CalcOffset(wCurrentOffsetTO, wRelOffset, bByte, bRawOpcode));
                }
                // Skip past to the next instruction
                pCur += OpArgs[bRawOpcode];
                wCurrentOffsetTO += OpArgs[bRawOpcode];
            }
        }
    }
    return wOffsets;
}

Script *CompiledScript::Decompile(IDecompileLookups &lookups, ILookupNames *pWords)
{
    auto_ptr<Script> pScript(new Script);

    // Get readable strings for them
    for (size_t i = 0; i < _saids.size(); i++)
    {
        _saidStrings.push_back(_SaidSequenceToString(_saids[i], pWords));
    }
    ASSERT(_saidStrings.size() == _saidsOffset.size());

    // Synonyms
    if (_synonyms.size() > 0)
    {
        map<WORD, WORD>::iterator it = _synonyms.begin();
        while (it != _synonyms.end())
        {
            auto_ptr<Synonym> pSynonym(new Synonym);
            ICompiledScriptSpecificLookups::ObjectType type;
            pSynonym->MainWord = lookups.LookupScriptThing(it->first, type);
            pSynonym->Replacement = lookups.LookupScriptThing(it->second, type);
            pScript->AddSynonym(pSynonym);
            ++it;
        }
    }

    // Local variables
    for (size_t i = 0; i < _localVars.size(); i++)
    {
        auto_ptr<VariableDecl> pVar(new VariableDecl);
        pVar->SetDataType("var"); // For now...
        std::stringstream ss;
        ss << "local_" << setw(4) << setfill('0') << (int)i;
        pVar->SetName(ss.str());
        pVar->SetSize(1);
        PropertyValue value;
        value.SetValue(_localVars[i]);
        pVar->AddSimpleInitializer(value);
        pScript->AddVariable(pVar);
    }
    
    // Now its time for code.
    // Make an index of code pointers by looking at the object methods
    set<WORD> codePointersTO;
    for (size_t i = 0; i < _objects.size(); i++)
    {
        vector<WORD> &methodPointersTO = _objects[i]->GetMethodCodePointersTO();
        codePointersTO.insert(methodPointersTO.begin(), methodPointersTO.end());
    }

    // and the exports
    for (size_t i = 0; i < _exportsTO.size(); i++)
    {
        WORD wCodeOffset = _exportsTO[i];
        // Export offsets could point to objects too - we're only interested in code pointers, so
        // check that it's within bounds.
        if (_IsInCodeSection(wCodeOffset))
        {
            codePointersTO.insert(wCodeOffset);
        }
    }

    // and finally, the most difficult of all, we'll need to scan though for any call calls...
    // those would be our internal procs
    set<WORD> internalProcOffsetsTO = _FindInternalCallsTO();//_rgRawCode, _wCodePosTO, _wCodeLength);
    // Before adding these though, remove any exports from the internalProcOffsets.
    vector<WORD>::iterator exportsIndex = _exportsTO.begin();
    while (exportsIndex != _exportsTO.end())
    {
        set<WORD>::iterator internalsIndex = find(internalProcOffsetsTO.begin(), internalProcOffsetsTO.end(), *exportsIndex);
        if (internalsIndex != internalProcOffsetsTO.end())
        {
            // Remove this guy.
            internalProcOffsetsTO.erase(internalsIndex);
        }
        exportsIndex++;
    }
    // Now add the internal guys to the full list
    codePointersTO.insert(internalProcOffsetsTO.begin(), internalProcOffsetsTO.end());
    // Now we know the length of each code segment (assuming none overlap)

    // Spit out code segments:
    // First, the objects (instances, classes)
    for (size_t i = 0; i < _objects.size(); i++)
    {
        _objects[i]->DecompileObject(*pScript, lookups, _scriptResource, _codeSections, codePointersTO);
    }

    // Now the exported procedures.
    for (size_t i = 0; i < _exportsTO.size(); i++)
    {
        // _exportsTO, in addition to containing code pointers for public procedures, also
        // contain the Rm class.  Filter these out by ignoring code pointers which point outside
        // the codesegment.
        if (_IsInCodeSection(_exportsTO[i]))
        {
            std::auto_ptr<ProcedureDefinition> pProc(new ProcedureDefinition);
            pProc->SetScript(pScript.get());
            pProc->SetName(lookups.ReverseLookupPublicExportName(_wScript, (WORD)i));
            pProc->SetPublic(true);
            _DecompileFunction(*pProc, lookups, _exportsTO[i], codePointersTO);
            pScript->AddProcedure(pProc);
        }
    }

    // Now the internal procedures (REVIEW - possibly overlap with exported ones)
    set<WORD>::iterator it = internalProcOffsetsTO.begin();
    while (it != internalProcOffsetsTO.end())
    {
        std::auto_ptr<ProcedureDefinition> pProc(new ProcedureDefinition);
        pProc->SetScript(pScript.get());
        pProc->SetName(_GetProcNameFromScriptOffset(*it));
        pProc->SetPublic(true);
        _DecompileFunction(*pProc, lookups, *it, codePointersTO);
        pScript->AddProcedure(pProc);
        ++it;
    }

    return pScript.release();
}

const char c_indent[] = "    ";
void CompiledScript::Disassemble(std::ostream &out, ICompiledScriptLookups *pLookups, IObjectFileScriptLookups *pOFLookups, ILookupNames *pWords, const std::string *pMessage)
{
    // (script x)
    if (pMessage)
    {
        out << *pMessage << endl;
    }
    out << "(script " << _wScript << ")" << endl << endl;

    // Internal strings
    out << "(string" << endl;
    out << hex;
    ASSERT(_strings.size() == _stringsOffset.size());
    for (size_t i = 0; i < _strings.size(); i++)
    {
        out << c_indent << "string_" << setw(4) << setfill('0') << _stringsOffset[i] << " \"" << _strings[i] << "\"" << endl;
    }
    out << ")" << endl << endl;


    // Prepare the saids.
    out << "(said" << endl;
    // Get readable strings for them
    for (size_t i = 0; i < _saids.size(); i++)
    {
        _saidStrings.push_back(_SaidSequenceToString(_saids[i], pWords));
    }
    ASSERT(_saidStrings.size() == _saidsOffset.size());
    // And then print them out
    for (size_t i = 0; i < _saidStrings.size(); i++)
    {
        //    said_0x03EC 'look/tree'
        out << c_indent << "said_" << setw(4) << setfill('0') << _saidsOffset[i] << " " << _saidStrings[i] << endl;
    }
    out << ")" << endl << endl;   


    // Synonyms
    if (_synonyms.size() > 0)
    {
        out << "(synonym" << endl;
        map<WORD, WORD>::iterator it = _synonyms.begin();
        while (it != _synonyms.end())
        {
            out << c_indent << pWords->Lookup(it->first) << " = " << pWords->Lookup(it->second).c_str() << endl;
            it++;
        }
        out << ")" << endl << endl;   
    }

    // Local variables
    out << "(local" << endl;
    for (size_t i = 0; i < _localVars.size(); i++)
    {
        out << c_indent << "local_"  << setw(4) << setfill('0') << (int)i << " = $"  << setw(4) << setfill('0') << _localVars[i] << endl;
    }
    out << ")" << endl << endl;   


    // Now its time for code.
    // Make an index of code pointers by looking at the object methods
    set<WORD> codePointersTO;
    for (size_t i = 0; i < _objects.size(); i++)
    {
        vector<WORD> &methodPointersTO = _objects[i]->GetMethodCodePointersTO();
        codePointersTO.insert(methodPointersTO.begin(), methodPointersTO.end());
    }

    // and the exports
    for (size_t i = 0; i < _exportsTO.size(); i++)
    {
        WORD wCodeOffset = _exportsTO[i];
        // Export offsets could point to objects too - we're only interested in code pointers, so
        // check that it's within bounds.
        if (_IsInCodeSection(wCodeOffset))
        {
            codePointersTO.insert(wCodeOffset);
        }
    }

    // and finally, the most difficult of all, we'll need to scan though for any call calls...
    // those would be our internal procs
    set<WORD> internalProcOffsetsTO = _FindInternalCallsTO();//_rgRawCode, _wCodePosTO, _wCodeLength);
    // Before adding these though, remove any exports from the internalProcOffsets.
    vector<WORD>::iterator exportsIndex = _exportsTO.begin();
    while (exportsIndex != _exportsTO.end())
    {
        set<WORD>::iterator internalsIndex = find(internalProcOffsetsTO.begin(), internalProcOffsetsTO.end(), *exportsIndex);
        if (internalsIndex != internalProcOffsetsTO.end())
        {
            // Remove this guy.
            internalProcOffsetsTO.erase(internalsIndex);
        }
        exportsIndex++;
    }
    // Now add the internal guys to the full list
    codePointersTO.insert(internalProcOffsetsTO.begin(), internalProcOffsetsTO.end());
    // Now we know the length of each code segment (assuming none overlap)

    // Spit out code segments - first, the objects (instances, classes)
    for (size_t i = 0; i < _objects.size(); i++)
    {
        _objects[i]->Disassemble(out, pLookups, pOFLookups, this, _scriptResource, _codeSections, codePointersTO);
        out << endl;
    }
    out << endl;

    // Now the exported procedures.
    for (size_t i = 0; i < _exportsTO.size(); i++)
    {
        // _exportsTO, in addition to containing code pointers for public procedures, also
        // contain the Rm class.  Filter these out by ignoring code pointers which point outside
        // the codesegment.
        if (_IsInCodeSection(_exportsTO[i]))
        {
            std::string strProcName = "";
            if (pOFLookups)
            {
                strProcName = pOFLookups->ReverseLookupPublicExportName(_wScript, (WORD)i);
            }
            out << dec;
            out << "// EXPORTED procedure #"  << (int)i << " (" << strProcName << ")" << endl;
            out << hex;
            _DisassembleFunction(out, pLookups, pOFLookups, _exportsTO[i], codePointersTO);
        }
    }
    out << endl;

    // Now the internal procedures (REVIEW - possibly overlap with exported ones)
    set<WORD>::iterator it = internalProcOffsetsTO.begin();
    while (it != internalProcOffsetsTO.end())
    {
        _DisassembleFunction(out, pLookups, pOFLookups, *it, codePointersTO);
        it++;
    }
}

void CompiledScript::_DecompileFunction(FunctionBase &func, IDecompileLookups &lookups, WORD wCodeOffsetTO, set<WORD> &sortedCodePointersTO)
{
    set<WORD>::const_iterator codeStartIt = sortedCodePointersTO.find(wCodeOffsetTO);
    ASSERT(codeStartIt != sortedCodePointersTO.end());
    CodeSection section;
    if (_FindStartEndCode(codeStartIt, sortedCodePointersTO, _codeSections, section))
    {
        BYTE *pBegin = &_scriptResource[section.begin];
        BYTE *pEnd = &_scriptResource[section.end];
        DecompileRaw(func, lookups, pBegin, pEnd, wCodeOffsetTO);
    }
    else
    {
        ASSERT(FALSE);
        // CORRUPTED SCRIPT comment
    }
}

void CompiledScript::_DisassembleFunction(ostream &out, ICompiledScriptLookups *pLookups, IObjectFileScriptLookups *pOFLookups, WORD wCodeOffsetTO, set<WORD> &sortedCodePointersTO)
{
    out << "(procedure proc_" << setw(4) << setfill('0') << wCodeOffsetTO << endl;

    set<WORD>::const_iterator codeStartIt = sortedCodePointersTO.find(wCodeOffsetTO);
    ASSERT(codeStartIt != sortedCodePointersTO.end());
    CodeSection section;
    if (_FindStartEndCode(codeStartIt, sortedCodePointersTO, _codeSections, section))
    {
        BYTE *pBegin = &_scriptResource[section.begin];
        BYTE *pEnd = &_scriptResource[section.end];
        _Disassemble(out, pLookups, pOFLookups, this, NULL, pBegin, pEnd, wCodeOffsetTO);
    }
    else
    {
        out << "CORRUPT SCRIPT" << endl;
    }
    out << ')' << endl << endl;
}

CompiledScript::~CompiledScript()
{
    for_each(_objects.begin(), _objects.end(), delete_object());
}



std::string CompiledObjectBase::LookupPropertyName(ICompiledScriptLookups *pLookup, WORD wPropertyIndex)
{
    // PERF: vector copy that is used frequently.
    std::vector<WORD> propertySelectorList;
    if (!pLookup->LookupSpeciesPropertyList(_wSpecies, propertySelectorList))
    {
        // We might be a "private" class.  So use our own list... (REVIEW: can't we always do this?)
        propertySelectorList = _propertySelectors;
    }
    ASSERT((wPropertyIndex %2) == 0);
    // REVIEW: Leisure Suit Larry 3, room 22, hits this ASSERT.
    wPropertyIndex /= 2;
    if (wPropertyIndex < propertySelectorList.size())
    {
        return pLookup->LookupSelectorName(propertySelectorList[wPropertyIndex]);
    }
    else
    {
        return "--UNKNOWN-PROP-NAME--";
    }
}

void CompiledObjectBase::DecompileObject(sci::Script &script,
        IDecompileLookups &lookups,
        const std::vector<BYTE> &scriptResource,
        const std::vector<CodeSection> &codeSections,
        std::set<WORD> &codePointersTO)
{
    lookups.EndowWithProperties(this);

    auto_ptr<ClassDefinition> pClass(new ClassDefinition);
    pClass->SetScript(&script);
    pClass->SetInstance(_fInstance);
    pClass->SetName(_strName);
    pClass->SetSuperClass(lookups.LookupClassName(_wSuperClass));
    vector<WORD> propertySelectorList;
    vector<WORD> speciesPropertyValueList;
    bool fSuccess = lookups.LookupSpeciesPropertyListAndValues(_wSpecies, propertySelectorList, speciesPropertyValueList);
    if (!fSuccess && !_fInstance)
    {
        // We're a class - our species is ourself
        propertySelectorList = _propertySelectors;
        speciesPropertyValueList = _propertyValues;
        fSuccess = true;
    }
    if (fSuccess)
    {
        ASSERT(propertySelectorList.size() == speciesPropertyValueList.size());
        size_t size1 = propertySelectorList.size();
        size_t size2 = _propertyValues.size();
        if (size1 == size2)
        {
            for (size_t i = 4; i < _propertyValues.size(); i++)
            {
                // If this is an instance, look up the species values, and only
                // include those that are different.
                if (!_fInstance || (_propertyValues[i] != speciesPropertyValueList[i]))
                {
                    std::auto_ptr<ClassProperty> pProperty(new ClassProperty);
                    pProperty->SetName(lookups.LookupSelectorName(propertySelectorList[i]));
                    PropertyValue value;
                    ICompiledScriptSpecificLookups::ObjectType type;
                    std::string saidOrString = lookups.LookupScriptThing(_propertyValues[i], type);
                    if (saidOrString.empty())
                    {
                        // Just give it a number
                        value.SetValue(_propertyValues[i]);
                    }
                    else
                    {
                        // REVIEW: we could provide a hit here when we shouldn't... oh well.
                        // Use ValueTypeToken, since the ' or " is already provided in the string.
                        value.SetValue(saidOrString, _ScriptObjectTypeToPropertyValueType(type));
                    }
                    pProperty->SetValue(value);
                    pClass->AddProperty(pProperty);
                }
            }
        }
        // else -> make ERROR by adding script comment.
    } // else make ERROR

    // Methods
    ASSERT(_functionSelectors.size() == _functionOffsetsTO.size());
    for (size_t i = 0; i < _functionSelectors.size(); i++)
    {
        // Now the code.
        set<WORD>::const_iterator functionIndex = find(codePointersTO.begin(), codePointersTO.end(), _functionOffsetsTO[i]);
        if (functionIndex != codePointersTO.end())
        {
            CodeSection section;
            if (_FindStartEndCode(functionIndex, codePointersTO, codeSections, section))
            {
                const BYTE *pStartCode = &scriptResource[section.begin];
                const BYTE *pEndCode = &scriptResource[section.end];
                std::auto_ptr<MethodDefinition> pMethod(new MethodDefinition);
                pMethod->SetScript(&script);
                pMethod->SetName(lookups.LookupSelectorName(_functionSelectors[i]));
                DecompileRaw(*pMethod, lookups, pStartCode, pEndCode, _functionOffsetsTO[i]);
                pClass->AddMethod(pMethod);
            }
        }
    }

    script.AddClass(pClass);
    lookups.EndowWithProperties(NULL);
}

void CompiledObjectBase::Disassemble(std::ostream &out,
    ICompiledScriptLookups *pLookups,
    IObjectFileScriptLookups *pOFLookups,
    ICompiledScriptSpecificLookups *pScriptThings,
    const std::vector<BYTE> &scriptResource,
    const std::vector<CodeSection> &codeSections,
    std::set<WORD> &codePointersTO)
{
    out << "// " << setw(4) << setfill('0') << _wPosInResource << endl;
    out << (_fInstance ? "(instance " : "(class ");

    // Header
    out << _strName << " of " << pLookups->LookupClassName(_wSuperClass) << endl;

    // Properties (skip the first 4)
    out << "    (properties" << endl;
    vector<WORD> propertySelectorList;
    if (pLookups->LookupSpeciesPropertyList(_wSpecies, propertySelectorList))
    {
        size_t size1 = propertySelectorList.size();
        size_t size2 = _propertyValues.size();
        if (size1 == size2)
        {
            for (size_t i = 4; i < _propertyValues.size(); i++)
            {
                out << "        " << pLookups->LookupSelectorName(propertySelectorList[i]) << "$" << _propertyValues[i] << endl;
            }
            out << "    )" << endl;
        }
        else
        {
            out << "// Problem with properties. Species has " << (int)size1 << " but class has " << (int)size2 << "." << endl;
        }
    }
    else
    {
        out << "// Can't get properties for class" << endl;
    }

    // Methods
    ASSERT(_functionSelectors.size() == _functionOffsetsTO.size());
    for (size_t i = 0; i < _functionSelectors.size(); i++)
    {
        out << "    (method (" << pLookups->LookupSelectorName(_functionSelectors[i]) << ") // method_" << setw(4) << setfill('0') << _functionOffsetsTO[i] << endl;

        // Now the code.
        set<WORD>::const_iterator functionIndex = find(codePointersTO.begin(), codePointersTO.end(), _functionOffsetsTO[i]);
        if (functionIndex != codePointersTO.end())
        {
            CodeSection section;
            if (_FindStartEndCode(functionIndex, codePointersTO, codeSections, section))
            {
                const BYTE *pStartCode = &scriptResource[section.begin];
                const BYTE *pEndCode = &scriptResource[section.end];
                _Disassemble(out, pLookups, pOFLookups, pScriptThings, this, pStartCode, pEndCode, _functionOffsetsTO[i]);
            }
            else
            {
                out << "CORRUPT SCRIPT" << endl;
            }

        }
        out << "    )" << endl << endl;
    }
    out << ")" << endl;
}


//
// Provide things like saids or internal strings.
//
std::string CompiledScript::LookupObjectName(WORD wOffset, ObjectType &type) const
{
    // See if it's an internal string:
    vector<WORD>::const_iterator offsetIndex = find(_stringsOffset.begin(), _stringsOffset.end(), wOffset);
    if (offsetIndex != _stringsOffset.end())
    {
        type = ObjectTypeString;
        return _strings[distance(_stringsOffset.begin(), offsetIndex)];
    }
    else
    {
        // See if it's a said:
        offsetIndex = find(_saidsOffset.begin(), _saidsOffset.end(), wOffset);
        if (offsetIndex != _saidsOffset.end())
        {
            type = ObjectTypeSaid;
            return _saidStrings[distance(_saidsOffset.begin(), offsetIndex)];
        }
        else
        {
            // See if it's an object
            // REVIEW: for some reason the offsets in the script point 8 bytes into the object.
            // Like, to the 4th selector... weird.  So add 8.
            offsetIndex = find(_objectsOffsetTO.begin(), _objectsOffsetTO.end(), wOffset - 8);
            if (offsetIndex != _objectsOffsetTO.end())
            {
                type = ObjectTypeClass;
                return _objects[distance(_objectsOffsetTO.begin(), offsetIndex)]->GetName();
            }
        }
    }

    return "";
}

CompiledObjectBase *CompiledScript::_FindObjectWithSpecies(WORD wIndex)
{
    CompiledObjectBase *pObject = NULL;
    for (size_t i = 0; !pObject && (i < _objects.size()); i++)
    {
        if ((_objects[i]->GetSpecies() == wIndex) && (!_objects[i]->IsInstance()))
        {
            pObject = _objects[i];
        }
    }
    return pObject;
}

std::string CompiledScript::LookupClassName(WORD wIndex)
{
    std::string ret;
    // Look for a class with this species index.
    CompiledObjectBase *pObject = _FindObjectWithSpecies(wIndex);
    if (pObject)
    {
        ret = pObject->GetName();
    }
    return ret;
}

bool CompiledScript::LookupSpeciesPropertyList(WORD wIndex, std::vector<WORD> &props)
{
    bool fRet = false;
    CompiledObjectBase *pObject = _FindObjectWithSpecies(wIndex);
    if (pObject)
    {
        props = pObject->GetProperties();
        fRet = true;
    }
    return fRet;
}

bool CompiledScript::LookupSpeciesPropertyListAndValues(WORD wIndex, std::vector<WORD> &props, std::vector<WORD> &values)
{
    bool fRet = false;
    CompiledObjectBase *pObject = _FindObjectWithSpecies(wIndex);
    if (pObject)
    {
        props = pObject->GetProperties();
        values = pObject->GetPropertyValues();
        fRet = true;
    }
    return fRet;
}

bool _IsPreferredWord(const std::string &word)
{
    return ((word == "look") ||
            (word == "get") ||
            (word == "talk") ||
            (word == "climb") ||
            (word == "man")
        );
    
}

const char c_szSeparator[] = " | ";

//
// Returns the preferred word in a " | " separated string.
// e.g. returns "get" for "acquire | get | obtain"
//
std::string _FindPreferredWord(const std::string &words)
{
    std::string word;
    size_t iStart = 0;
    size_t iEndOfFirst = words.find(c_szSeparator, iStart);
    while (iEndOfFirst != string::npos)
    {
        // Guess it's a loner
        if (_IsPreferredWord(words.substr(iStart, iEndOfFirst - iStart)))
        {
            word = words.substr(iStart, iEndOfFirst - iStart);
            break;
        }
        else if (word.empty())
        {
            word = words.substr(iStart, iEndOfFirst - iStart); // Use the first one otherwise.
        }
        iStart = iEndOfFirst + strlen(c_szSeparator);
        iEndOfFirst = words.find(c_szSeparator, iStart);
    }
    if (word.empty())
    {
        word = words; // loner
    }
    return word;
}

string CompiledScript::_SaidSequenceToString(vector<WORD> saidSequence, ILookupNames *pWords)
{
    string strRet = "'";
    for (size_t i = 0; i < saidSequence.size(); i++)
    {
        WORD wOp = saidSequence[i];
        if ((wOp >= 0x00f0) && (wOp <= 0x00f9))
        {
            strRet += SaidArgs[wOp - 0xf0].Character;
        }
        else
        {
            // Must be a word.
            std::string strWords = pWords->Lookup(wOp);
            // This is a little hokey -> this is " | " separated.  Let's just take the first word.
            strRet += _FindPreferredWord(strWords);
        }
    }
    strRet += "'";
    return strRet;
}

PCSTR el(bool fRN) { return  fRN ? "\r\n" : "\n"; }

const char c_szDivider[] = "------------------------------";

void CompiledScript::DebugOut(std::ostream &out, bool fRN) const
{
    out << "Script: " << _wScript << el(fRN) << el(fRN);

    out << (WORD)_localVars.size() << " script variables." << el(fRN) << c_szDivider << el(fRN);

    out << (WORD)_exportsTO.size() << " exports." << el(fRN) << c_szDivider << el(fRN);
    vector<WORD>::const_iterator it = _exportsTO.begin();
    out << hex;
    for (; it != _exportsTO.end(); ++it)
    {
        out << "    $" << (*it) << el(fRN);
    }
    out << el(fRN);
    out << dec;
    out << (WORD)_objects.size() << " objects." << el(fRN) << c_szDivider << el(fRN);
    out << hex;
    vector<CompiledObjectBase*>::const_iterator objectIt = _objects.begin();
    for (; objectIt != _objects.end(); ++objectIt)
    {
        (*objectIt)->DebugOut(out, fRN);
    }

}

void CompiledObjectBase::DebugOut(std::ostream &out, bool fRN) const
{
    out << (_fInstance ? "instance" : "class") << "@$" << _wPosInResource << el(fRN);
    out << "properties:" << el(fRN);
    out << "    species: " << _wSpecies << el(fRN);
    out << "    super: " << _wSuperClass << el(fRN);
    out << "    name: " << _strName << el(fRN);
    out << "    -info-: " << _wInfo << el(fRN);
    vector<WORD>::const_iterator selIt = _propertySelectors.begin();
    vector<WORD>::const_iterator valueIt = _propertyValues.begin();
    for (; selIt != _propertySelectors.end() && valueIt != _propertyValues.end(); ++selIt, ++valueIt)
    {
        out << "    " << *selIt << " : " << *valueIt << el(fRN);
    }
    out << el(fRN) << "methods:" << el(fRN);
    vector<WORD>::const_iterator funcIt = _functionSelectors.begin();
    vector<WORD>::const_iterator offIt = _functionOffsetsTO.begin();
    for (; funcIt != _functionSelectors.end() && offIt != _functionOffsetsTO.end(); ++funcIt, ++offIt)
    {
        out << "    " << *funcIt << " @" << *offIt << el(fRN);
    }
    out << el(fRN);
}





//
// GlobalCompiledScriptLookups
//
bool GlobalCompiledScriptLookups::Load()
{
    return _selectors.Load() && _kernels.Load() && _classes.Load();
}
std::string GlobalCompiledScriptLookups::LookupSelectorName(WORD wIndex)
{
    std::string str = _selectors.Lookup(wIndex);
    if (str.empty())
    {
        // It is legit (e.g. script 99 in SQ3) for there to be selectors that don't have a name
        // in the official selector list. "private" selectors for "private" classes.
        std::stringstream ss;
        ss << "selector" << wIndex;
        str = ss.str();
    }
    return str;
}
std::string GlobalCompiledScriptLookups::LookupKernelName(WORD wIndex)
{
    return _kernels.Lookup(wIndex);
}
std::string GlobalCompiledScriptLookups::LookupClassName(WORD wIndex)
{
    return _classes.Lookup(wIndex);
}
bool GlobalCompiledScriptLookups::LookupSpeciesPropertyList(WORD wIndex, std::vector<WORD> &props)
{
    std::vector<WORD> values; // Throw away
    return _classes.GetSpeciesPropertySelector(wIndex, props, values);
}
bool GlobalCompiledScriptLookups::LookupSpeciesPropertyListAndValues(WORD wIndex, std::vector<WORD> &props, std::vector<WORD> &values)
{
    return _classes.GetSpeciesPropertySelector(wIndex, props, values);
}

