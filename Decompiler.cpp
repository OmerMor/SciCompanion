#include "stdafx.h"
#include "SCIPicEditor.h"
#include "Decompiler.h"
#include "Compile\PMachine.h"
#include "Compile\ScriptOMAll.h"
#include "Compile\scii.h"
#include <boost/foreach.hpp>

using namespace sci;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ValueType _ScriptObjectTypeToPropertyValueType(ICompiledScriptSpecificLookups::ObjectType type)
{
    switch (type)
    {
    case ICompiledScriptSpecificLookups::ObjectTypeSaid:
        return ValueTypeToken; // since it already includes the ' '
        break;
    case ICompiledScriptSpecificLookups::ObjectTypeString:
        return ValueTypeString;
        break;
    case ICompiledScriptSpecificLookups::ObjectTypeClass:
        return ValueTypeToken;
        break;
    }
    return ValueTypeToken;
}

//
// Represents whether an instruction consumes or generates stack or accumulator
// In general, values will be 0 or 1, except for cStackConsume which could be
// much larger (e.g. for send or call instruction)
//
struct Consumption
{
    Consumption() 
    {
        cAccConsume = 0;
        cStackConsume = 0;
        cAccGenerate = 0;
        cStackGenerate = 0;
    }

    int cAccConsume;
    int cStackConsume;
    int cAccGenerate;
    int cStackGenerate;
};

bool _IsVOIndexed(BYTE bOpcode)
{
    return !!(bOpcode & 0x08);
}
bool _IsVOPureStack(BYTE bOpcode)
{
    return !!(bOpcode & 0x04);
}
bool _IsVOStack(BYTE bOpcode)
{
    // It's a stack operation if it says it's a stack operation,
    // or if the accumulator is being used as an index.
    // WARNING PHIL: true for store, but maybe no load???
    return _IsVOPureStack(bOpcode) || _IsVOIndexed(bOpcode);
}
bool _IsVOStoreOperation(BYTE bOpcode)
{
    return ((bOpcode & 0x30) == 0x10);
}
bool _IsVOIncremented(BYTE bOpcode)
{
    return ((bOpcode & 0x30) == VO_INC_AND_LOAD);
}
bool _IsVODecremented(BYTE bOpcode)
{
    return ((bOpcode & 0x30) == VO_DEC_AND_LOAD);
}

//
// A currently abandoned attempt at writing a decompiler.  Not even close to being done.
// 

//
// De-compiler notes
//
// We'll to, in DisassembleFunction, arrange them into scii instructions.  Point the branches to each other.
// Each instruction needs to claim whether it puts something onto the stack or acc.
// Some things start new lines, line assignments, sends, calls.
// Could look for patterns. e.g.
// *IF-ELSE*
//      blah
//      bnt A
//      code
//      jmp B
//  A:  code
//  B:  code
//
//  *DO*
//  A:  code
//      code
//      bt A
//
//  *WHILE*
//      code
//  A:  bnt B
//      code
//      jmp A:
//
//
// We don't have goto statements, so jmps are a problem.
//  I think they only happen in if and switches and loops.
//
// We could have tables of common param names (init/client) and var names ?
//
//
//

std::string _GetPublicProcedureName(WORD wScript, WORD wIndex)
{
    std::stringstream ss;
    ss << "proc" << wScript << "_" << wIndex;
    return ss.str();
}


typedef std::list<scii>::reverse_iterator rcode_pos;

struct Fixup
{
    code_pos branchInstruction;
    WORD wTarget;
    bool fForward;
};

code_pos get_cur_pos(std::list<scii> &code)
{
    code_pos pos = code.end();
    --pos;
    return pos;
}

// 
// pBegin/pEnd - bounding pointers for the raw byte code
// wBaseOffset - byte offset in script file where pBegin is (used to calculate absolute code offsets)
// code        - (out) list of sci instructions.
//
void _ConvertToInstructions(std::list<scii> &code, const BYTE *pBegin, const BYTE *pEnd, WORD wBaseOffset)
{
    std::map<WORD, code_pos> referenceToCodePos;
    std::vector<Fixup> branchTargetsToFixup;

    code_pos undetermined = code.end();

    WORD wReferencePosition = 0;
    const BYTE *pCur = pBegin;
    while (pCur < pEnd)
    {
        const BYTE *pThisInstruction = pCur;
        BYTE bRawOpcode = *pCur;
        bool bByte = (*pCur) & 1;
        BYTE bOpcode = bRawOpcode >> 1;
        ASSERT(bOpcode < 128);
        ++pCur; // Advance past opcode.
        WORD wOperands[3];
        ZeroMemory(wOperands, sizeof(wOperands));
        int cIncr = 0;
        for (int i = 0; i < 3; i++)
        {
            cIncr = GetOperandSize(bRawOpcode, OpArgTypes[bOpcode][i]);
            switch (cIncr)
            {
            case 1:
                wOperands[i] = (WORD)*pCur;
                break;
            case 2:
                wOperands[i] = *((WORD*)pCur); // REVIEW
                break;
            default:
                break;
            }
            pCur += cIncr;
            if (cIncr == 0) // No more operands
            {
                break;
            }
        }
        // Add the instruction - use the constructor that takes all arguments, even if
        // not all are valid.
        WORD wReferenceNextInstruction = (wReferencePosition + (WORD)(pCur - pThisInstruction));
        if ((bOpcode == acBNT) || (bOpcode == acBT) || (bOpcode == acJMP))
        {
            // +1 because its the operand start pos.
            WORD wTarget = CalcOffset(wReferencePosition + 1, wOperands[0], bByte, bRawOpcode);
            code.push_back(scii(bOpcode, undetermined, true));
            bool fForward = (wTarget > wReferencePosition);
            Fixup fixup = { get_cur_pos(code), wTarget, fForward };
            branchTargetsToFixup.push_back(fixup);

            char sz[100];
            StringCchPrintf(sz, ARRAYSIZE(sz), "Ref pos: %d --> %d\n", wReferencePosition, wTarget);
            OutputDebugString(sz);
        }
        else
        {
            code.push_back(scii(bOpcode, wOperands[0], wOperands[1], wOperands[2]));

            char sz[100];
            StringCchPrintf(sz, ARRAYSIZE(sz), "Ref pos: %d\n", wReferencePosition);
            OutputDebugString(sz);
        }
        // Store the position of the instruction we just added:
        referenceToCodePos[wReferencePosition] = get_cur_pos(code);
        // Store the actual offset in the instruction itself:
        WORD wSize = (WORD)(pCur - pThisInstruction);
        get_cur_pos(code)->set_offset_and_size(wReferencePosition + wBaseOffset, wSize);

        wReferencePosition += wSize;
    }

    // Now fixup any branches.
    for (size_t i = 0; i < branchTargetsToFixup.size(); i++)
    {
        Fixup &fixup = branchTargetsToFixup[i];
        char sz[100];
        StringCchPrintf(sz, ARRAYSIZE(sz), "%s: %d", fixup.fForward ? "fwd" : "back", fixup.wTarget);
        OutputDebugString(sz);
        std::map<WORD, code_pos>::iterator it = referenceToCodePos.find(fixup.wTarget);
        if (it != referenceToCodePos.end())
        {
            OutputDebugString(" - success\n");
            fixup.branchInstruction->set_branch_target(it->second, fixup.fForward);
        }
        else
        {
            ASSERT(FALSE);
            OutputDebugString(" - Error in script\n");
        }
    }

    // Special hack... function code is placed at even intervals.  That means there might be an extra bogus
    // instruction at the end, just after a ret statement (often it is acBNOT, which is 0).  If the instruction
    // before the end is a ret instruction, remove the last instruction (unless it's also a ret - since sometimes
    // functions will end with two RETs, both of which are jump targets).
    if (code.size() > 1)
    {
        code_pos theEnd = code.end();
        --theEnd; // Last instruction
        code_pos maybeRET = theEnd;
        --maybeRET;
        if ((maybeRET->get_opcode() == acRET) && (theEnd->get_opcode() != acRET))
        {
            code.erase(theEnd);
        }
    }
}

struct BranchBlock
{
    code_pos begin;
    code_pos end;
};

bool IsDelineatingInstruction(code_pos pos)
{
    BYTE bOpcode = pos->get_opcode();
    return (bOpcode == acBNT) || (bOpcode == acBT) || (bOpcode == acJMP);
}
bool IsBranchInstruction(code_pos pos)
{
    return IsDelineatingInstruction(pos);
}

bool _IsVariableUse(code_pos pos, BYTE variableType, WORD &wIndex)
{
    bool fRet = false;
    BYTE bOpcode = pos->get_opcode();
    if (OpArgTypes[bOpcode][0] == otVAR)
    {
        // It's a variable opcode.
        wIndex = pos->get_first_operand();
        fRet = ((bOpcode & 0x03) == variableType);
    }
    else if (bOpcode == acLEA)
    {
        // The "load effective address" instruction
        WORD wType = pos->get_first_operand();
        fRet = ((bOpcode & 0x03) == variableType);
        if (fRet)
        {
            wIndex = pos->get_second_operand();
        }
    }
    return fRet;
}

std::string _GetSuitableParameterName(FunctionBase &function, WORD iIndex)
{
    if ((function.GetName() == "changeState") && (iIndex == 1))
    {
        return "newState";
    }
    if ((function.GetName() == "handleEvent") && (iIndex == 1))
    {
        return "pEvent";
    }
    std::stringstream ss;
    ss << "param" << iIndex;
    return ss.str();
}

void _FigureOutParameters(FunctionBase &function, FunctionSignature &signature, std::list<scii> &code)
{
    WORD wBiggest = 0;
    for (code_pos pos = code.begin(); pos != code.end(); ++pos)
    {
        WORD wIndex;
        if (_IsVariableUse(pos, 3, wIndex)) // 3 -> param
        {
            wBiggest = max(wIndex, wBiggest);
        }
    }
    if (wBiggest) // Parameters start at index 1, so 0 means no parameters
    {
        for (WORD i = 1; i <= wBiggest; i++)
        {
            std::auto_ptr<FunctionParameter> pParam(new FunctionParameter);
            pParam->SetName(_GetSuitableParameterName(function, i));
            pParam->SetDataType("var"); // Generic for now... we could try to detect things in the future.
            signature.AddParam(pParam, false);   // false -> hard to detect if optional or not... serious code analysis req'd
        }
    }
}

//
// Scans the code for local variable usage, and adds "local0, local1, etc..." variables to function.
//
template<typename _TVarHolder>
void _FigureOutTempVariables(_TVarHolder &function, BYTE variableType, std::list<scii> &code)
{
    // Look for the link instruction.
    WORD cTotalVariableRoom = 0;
    for (code_pos pos = code.begin(); pos != code.end(); ++pos)
    {
        if (pos->get_opcode() == acLINK)
        {
            cTotalVariableRoom = pos->get_first_operand();
            break;
        }
    }

    if (cTotalVariableRoom)
    {
        // Track all accesses of variables
        std::set<WORD> variableIndexAccess; // Index of accesses.
        variableIndexAccess.insert(0); // Always put 0 access at least
        for (code_pos pos = code.begin(); pos != code.end(); ++pos)
        {
            WORD wIndex;
            if (_IsVariableUse(pos, variableType, wIndex))
            {
                variableIndexAccess.insert(wIndex);
            }
        }

        // Now let's create a bunch of variables - but at least one.
        WORD cVariables = static_cast<WORD>(variableIndexAccess.size());
        std::set<WORD>::iterator accessIt = variableIndexAccess.begin();
        for (WORD i = 0; i < cVariables; i++)
        {
            WORD wBegin = *accessIt;
            ++accessIt;
            WORD wEnd;
            if (accessIt != variableIndexAccess.end())
            {
                wEnd = *accessIt;
            }
            else
            {
                wEnd = cTotalVariableRoom;
            }

            std::auto_ptr<VariableDecl> pVar(new VariableDecl());
            std::stringstream ss;
            ss << "temp" << i;
            pVar->SetName(ss.str());
            pVar->SetSize(wEnd - wBegin);
            function.AddVariable(pVar);
        }
    }
}

std::string _indent(int iIndent)
{
    std::string theFill;
    theFill.insert(theFill.begin(), iIndent, ' ');
    return theFill;
}


// Fwd decl
class CodeShape;

//
// Attempt at generating an instruction tree.  Used to turn a flat sequence of
// instructions into a tree so that, for example, a send call at the root, would
// have each of its parameters.
//
// e.g. if (5 == gEgo.x)
// would be something like:
//
//   bnt
//     eq?
//        ldi 5
//        push
//          send
//            push0
//            pushi
//
class CodeNode
{
public:

    CodeNode(code_pos pos) : _pos(pos), _targetLabel(0) {}
    ~CodeNode()
    {
        for_each(_children.begin(), _children.end(), delete_object());
    }
    void SetCode(code_pos pos) { _pos = pos; }
    code_pos GetCode() const { return _pos; }
    void AddChild(std::auto_ptr<CodeNode> pNode) { _children.insert(_children.begin(), pNode.get()); pNode.release(); }
    size_t GetChildCount() { return _children.size(); }
    CodeNode *Child(size_t i) { return _children[i]; }

    std::vector<CodeNode*>::iterator begin() { return _children.begin(); }
    std::vector<CodeNode*>::iterator end() { return _children.end(); }

    // For debugging
    void SetLabel(char c) { if (!_label.empty()) { _label += ","; }  _label += c; }
    void SetTargetLabel(char c) { _targetLabel = c; }

    void Print(std::ostream &os, int iIndent) const
    {
        os << _indent(iIndent);
        if (!_label.empty())
        {
            os << _label << ":";
        }
        os << OpcodeNames[_pos->get_opcode()];
        if (_targetLabel)
        {
           os << "--> " << _targetLabel; 
        }
        os << "\n";

        std::vector<CodeNode*>::const_iterator it;
        for (it = _children.begin(); it != _children.end(); ++it)
        {
            (*it)->Print(os, iIndent + 2);
        }
    }

private:
    code_pos _pos;                          // The instruction
    std::vector<CodeNode*> _children;       // The child code nodes
    std::string _label;                     // For debugging
    char _targetLabel;                      // For debugging
};

typedef std::vector<CodeNode*>::iterator codenode_it;


Consumption _GetInstructionConsumption(scii &inst)
{
    BYTE bOpcode = inst.get_opcode();
    ASSERT(bOpcode < 128);
    int cEatStack = 0;
    bool fChangesAcc = false;
    bool fEatsAcc = false;
    bool fPutsOnStack = false;
    switch (bOpcode)
    {
    case acSELF:
        cEatStack = inst.get_first_operand() / 2;
        fChangesAcc = true;
        break;

    case acSEND:
        cEatStack = inst.get_first_operand() / 2;
        fChangesAcc = true;
        fEatsAcc = true;
        break;

    case acSUPER:
        cEatStack = inst.get_second_operand() / 2;
        fChangesAcc = true;
        break;

    case acBNOT:
    case acNOT:
    case acNEG:
        fChangesAcc = true;
        fEatsAcc = true;
        break;

    case acSUB:
    case acMUL:
    case acDIV:
    case acMOD:
    case acSHR:
    case acSHL:
    case acXOR:
    case acAND:
    case acOR:
    case acADD:
    case acEQ:
    case acGT:
    case acLT:
    case acLE:
    case acNE:
    case acGE:
    case acUGT:
    case acUGE:
    case acULT:
    case acULE:
        fChangesAcc = true;
        fEatsAcc = true;
        cEatStack = 1;
        break;

    case acBT:
    case acBNT:
        fEatsAcc = true;
        break;

    case acRET:
        // Wreaks havoc
        //fEatsAcc = true; // But not always intentional...
        break;

    case acJMP:
    case acLINK:
        break;

    case acLDI:
        fChangesAcc = true;
        break;

    case acPUSH:
        fEatsAcc = true;
        fPutsOnStack = true;
        break;

    case acPUSHI:
    case acPUSH0:
    case acPUSH1:
    case acPUSH2:
        fPutsOnStack = true;
        break;

    case acPUSHSELF:
        fPutsOnStack = true;
        break;

    case acTOSS:
        //fChangesAcc = true; // doesn't touch the acc, by definition (toss)
        cEatStack = 1;
        break;

    case acDUP:
        fPutsOnStack = true;
        break;

    case acCALL:
    case acCALLK:
    case acCALLB:
        cEatStack = inst.get_second_operand() / 2;
        cEatStack++; // the number didn't include the # of instructions push.
        fChangesAcc = true;
        break;

    case acCALLE:
        //fEatsAcc = true;
        cEatStack = inst.get_third_operand() / 2;
        cEatStack++; // Also a parameter count.
        fChangesAcc = true;
        break;

    case acCLASS:
    case acSELFID:
        fChangesAcc = true;
        break;

    case acPPREV:
        fPutsOnStack = true;
        break;

    case acREST:
        // Doesn't really affect anything
        break;

    case acLEA:
        fEatsAcc = !!(bOpcode & LEA_ACC_AS_INDEX_MOD);
        fChangesAcc = true;
        break;

    case acPTOA:
    case acIPTOA:
    case acDPTOA:
        fChangesAcc = true;
        break;

    case acATOP:
        fEatsAcc = true;
        fChangesAcc = true; 
        // Not technically, but it leaves the value in the accumulator, so
        // it's what people should look at.
        break;

    case acPTOS:
    case acIPTOS:
    case acDPTOS:
        fPutsOnStack = true;
        break;

    case acSTOP:
        cEatStack = 1;
        break;

    case acLOFSA:
        fChangesAcc = true;
        break;

    case acLOFSS:
        fPutsOnStack = true;
        break;

    default:

//sali -> store acc in local, indexed by acc
//ac is local4
        ASSERT((bOpcode >= 64) && (bOpcode <= 127));
        // TODO: use our defines/consts
        if (_IsVOStoreOperation(bOpcode))
        {
            // Store operation
            if (_IsVOPureStack(bOpcode))
            {
                cEatStack = 1;
            }
            else
            {
                if (_IsVOStack(bOpcode))
                {
                    cEatStack = 1;
                }
                fEatsAcc = true;
                fChangesAcc = true; // Not really, but leaves a valid thing in the acc.
            }
        }
        else
        {
            // Load operation
            if (_IsVOPureStack(bOpcode))
            {
                fPutsOnStack = true;
            }
            else
            {
                fChangesAcc = true;
            }
        }
        if (_IsVOIndexed(bOpcode))
        {
            fEatsAcc = true; // index is in acc
        }
        break;

    }

    Consumption cons;
    if (fEatsAcc)
    {
        cons.cAccConsume++;
    }
    if (fChangesAcc)
    {
        cons.cAccGenerate++;
    }
    cons.cStackConsume = cEatStack;
    if (fPutsOnStack)
    {
        cons.cStackGenerate++;
    }

    return cons;
}

bool _IsJumpToPush(code_pos posJump, CodeNode *pNodePush)
{
    return (posJump->get_branch_target() == pNodePush->GetCode());
}

// fwd decl
void _InstructionTreeHelper(CodeNode &parent, code_pos begin, code_pos &end);

//
// end -> next instruction before push tree defined by pNodePush
//
code_pos _CheckForTernary(code_pos end, code_pos begin, CodeNode *pNodePush)
{
    code_pos endReturn = end;
    if ((end->get_opcode() == acJMP) && _IsJumpToPush(end, pNodePush))
    {
        // Good
        --end;
        if (end != begin)
        {
            // Now we're looking for some kind of accumulator thing.
            Consumption cons = _GetInstructionConsumption(*end);
            if (cons.cAccGenerate == 1)
            {
                ASSERT(cons.cStackGenerate == 0);
                std::auto_ptr<CodeNode> pFirstValue(new CodeNode(end));
                _InstructionTreeHelper(*pFirstValue, begin, end);
                
                // Now see if we have a bnt
                if (end->get_opcode() == acBNT)
                {
                    // This is it.
                    std::auto_ptr<CodeNode> pCondition(new CodeNode(end));
                    _InstructionTreeHelper(*pCondition, begin, end);

                    // Now we're going to do someting special.  Take the original push,
                    // and add some crap to it.
                    ASSERT(pNodePush->GetChildCount() == 1); // Should just be the "second" value.
                    // push
                    //      ldi (2)
                    //
                    // We want it to be:
                    // push
                    //      bnt
                    //          ???
                    //      ldi (1)
                    //      ldi (2)
                    pNodePush->AddChild(pFirstValue);
                    pNodePush->AddChild(pCondition);

                    // Update the end...
                    endReturn = end;
                }
            }
        }
    }
    return endReturn;
}

// end is where you start looking.
void _InstructionTreeHelper(CodeNode &parent, code_pos begin, code_pos &end)
{
    if (end != begin)
    {
        // 1) Ok, now check what this thing eats
        // 2) Work backwards and add children until that hunger is satisfied.
        Consumption consTemp = _GetInstructionConsumption(*end);
        int cStackConsume = consTemp.cStackConsume;
        int cAccConsume = consTemp.cAccConsume;

        --end;

        bool fHadAPush = false;
        CodeNode *pNodePush = NULL;
        bool fHadAJump = false;
        bool fHadAccumulatorThing = false;

        code_pos posAnomalyBegin = begin;;

        // Now we try to bring the stack and acc balance to zero.
        while((end != begin) && ((cStackConsume > 0) || (cAccConsume > 0)))
        {
            Consumption cons = _GetInstructionConsumption(*end);

            // phil -> consider only doing acc stuff for the first thing..?
            cStackConsume -= cons.cStackGenerate;
            cAccConsume -= cons.cAccGenerate;
            // Bound the acc consumption at 0... many things put stuff into the acc, but it doesn't mean it
            // should be used.
            cAccConsume = max(0, cAccConsume); 
            // On the other hand, the stack should never go below 0 if the code is good - we probably need to handle and bail
            // once we've got the analysis down pat - phil
            if (cStackConsume < 0)
            {
                // We have an issue here.  This could arise in a case like this:
                // ldi 5
                // pushi 6
                // push   // uses the value in the accumulator, which is 5
                // 
                // We need to ignore the pushi 6 then, but only for right here....
                if (posAnomalyBegin == begin)
                {
                    // This was our first anomoly.  When we exit the function end should be set to this.
                    posAnomalyBegin = end;
                }
                cStackConsume = 0;
                // ignore this instruction - we took note of it in posAnomaly;
                --end;

                // phil: We may need a better model... we're essentially skipping instructions.  They should be put
                // back into a queue to be used somewhere.  Think of the following:
                // class
                // <switch statement stuff where each case puts something in the acc>
                // toss
                // push  //-> we'll let the toss do its thing and become a switch statement, and accidentally
                //       //   eat the class before the switch statement.
                // Ok -> I guess this is unrelated.  Do we need a special hack for this???
            }
            else
            {
                // ASSERT for now, though we'll need to deal with this:
                ASSERT((cons.cStackConsume >= 0) && (cons.cAccConsume >= 0));

                std::auto_ptr<CodeNode> pChild(new CodeNode(end));

                // If it consumes anything, then call recursively
                if (cons.cStackConsume || cons.cAccConsume)
                {
                    _InstructionTreeHelper(*pChild, begin, end);

                    // phil HACK
                    if (pChild->GetCode()->get_opcode() == acPUSH)
                    {
                        end = _CheckForTernary(end, begin, pChild.get());
                    }
                }
                else
                {
                    --end;
                }
                parent.AddChild(pChild);
            }
        }

        if (posAnomalyBegin != begin)
        {
            // We had to skip some instructions - so reset end to point to where we
            // had to start skipping.
            end = posAnomalyBegin;
        }
    }
}

void _GenerateInstructionTree(CodeNode &root, code_pos begin, code_pos &end)
{
    if (end != begin) // At least on instruction
    {
        _InstructionTreeHelper(root, begin, end);
    }
}

// fwd decl
SyntaxNode *_CodeNodeToSyntaxNode(CodeNode &node, IDecompileLookups &lookups, CodeNode *pPreviousNode = NULL);
void _ApplySyntaxNodeToCodeNode(CodeNode &node, StatementsNode &statementsNode, IDecompileLookups &lookups, CodeNode *pPreviousNode = NULL);
void _ApplySyntaxNodeToCodeNode1(CodeNode &node, OneStatementNode &statementsNode, IDecompileLookups &lookups, CodeNode *pPreviousNode = NULL);
void _ApplySyntaxNodeToCodeNode2(CodeNode &node, TwoStatementNode &statementsNode, IDecompileLookups &lookups, CodeNode *pPreviousNode = NULL);



// Determines if a code_pos is within a CodeNode...
// TODO: really should just check if a CodeNode *starts* with a code_pos
bool _ScanCodeNodeForCodePos(CodeNode *pNode, code_pos pos)
{
    bool fRet = (pNode->GetCode() == pos);
    for (size_t i = 0; !fRet && (i < pNode->GetChildCount()); i++)
    {
        fRet = _ScanCodeNodeForCodePos(pNode->Child(i), pos);
    }
    return fRet;
}

//
// Scans a series of CodeNodes to find one that contains with code_pos.
// (note: should really be the one that starts with the code_pos, and by starts with
//  we don't mean it's at the root of the hierarchy, but simply the first instruction
//  that appears in code)
// If none are found, then it returns end.
// If one is found, it doesn't returns the codenode_it with the code_pos (because there
// may be no codenode_it), but the 'root' codenode_it.
//
codenode_it _FindCodeNodeWithCodePos(codenode_it begin, codenode_it end, code_pos pos)
{
    while ((begin != end) && !_ScanCodeNodeForCodePos(*begin, pos))
    {
        ++begin;
    }
    return begin;
}

//
// Defines the "shape" of code, such as if statements, switch statements, loops, etc....
//
class CodeShape
{
public:
    virtual SingleStatementPtr DoIt(IDecompileLookups &lookups) = 0;
    virtual void Print(std::ostream &os, int iIndent) const = 0;
};

// Fwd decl
void _PopulateCodeShapes(std::vector<CodeShape*> &shapes, codenode_it begin, codenode_it end, IDecompileLookups &lookups);

void _ProcessCodeNodesIntoStatements(StatementsNode &statements, codenode_it begin, codenode_it end, IDecompileLookups &lookups)
{
    std::vector<CodeShape*> shapes;
    _PopulateCodeShapes(shapes, begin, end, lookups);
    std::vector<CodeShape*>::iterator it = shapes.begin();
    while (it != shapes.end())
    {
        auto_ptr<SingleStatement> pStatement((*it)->DoIt(lookups));
        statements.AddStatement(pStatement);
        ++it;
    }
    for_each(shapes.begin(), shapes.end(), delete_object());
}

class CodeTypeNormal : public CodeShape
{
public:
    CodeTypeNormal(codenode_it code) : _code(code) {}
    virtual SingleStatementPtr DoIt(IDecompileLookups &lookups)
    {
        auto_ptr<SingleStatement> pStatement(new SingleStatement);
        pStatement->SetSyntaxNode(_CodeNodeToSyntaxNode(*(*_code), lookups));
        return pStatement.release();
    }
    virtual void Print(std::ostream &os, int iIndent) const
    {
        (*_code)->Print(os, iIndent);
    }

private:
    codenode_it _code;
};

class CodeTypeSwitch : public CodeShape
{
public:
    CodeTypeSwitch() : _fSwitch(false) { }
    void SetSwitch(codenode_it switchPivot) { _switch = switchPivot; _fSwitch = true; }
    void AddValue(codenode_it switchValue, codenode_it begin, codenode_it end)
    {
        switchvalue_t value = { switchValue, begin, end };
        _values.push_back(value);
    }

    virtual SingleStatementPtr DoIt(IDecompileLookups &lookups)
    {
        auto_ptr<SingleStatement> pStatement(new SingleStatement);
        if (_fSwitch)
        {
            auto_ptr<SwitchStatement> pSwitch(new SwitchStatement);

            // The switch pivot:
            _ApplySyntaxNodeToCodeNode1(*(*_switch), *pSwitch, lookups);

            // The cases:
            for (size_t i = 0; i < _values.size(); ++i)
            {
                auto_ptr<CaseStatement> pCase(new CaseStatement);

                // Case value:
                CodeNode *pNodeValue = *_values[i].value;
                CodeNode *pNodeSomething = _IsolateCaseValue(*pNodeValue);
                _ApplySyntaxNodeToCodeNode1(*pNodeSomething, *pCase, lookups);

                // The code in the case:
                _ProcessCodeNodesIntoStatements(*pCase, _values[i].begin, _values[i].end, lookups);

                pSwitch->AddCase(pCase);
            }

            pStatement->SetSyntaxNode(pSwitch);
        }
        else
        {
            auto_ptr<Comment> pSwitch(new Comment);
            pSwitch->SetName("ERROR");
            pStatement->SetSyntaxNode(pSwitch);
        }
        return pStatement.release();
    }
    virtual void Print(std::ostream &os, int iIndent) const
    {
        os << _indent(iIndent) << "[switch]\n";
        if (_fSwitch)
        {
            (*_switch)->Print(os, iIndent + 2);
        }
        else
        {
            os << "ERROR\n";
        }
        for (size_t i = 0; i < _values.size(); ++i)
        {
            os << _indent(iIndent + 2) << "[case]\n";
            (*_values[i].value)->Print(os, iIndent + 4);
            codenode_it it = _values[i].begin;
            while (it != _values[i].end)
            {
                (*it)->Print(os, iIndent + 4);
                ++it;
            }
        }
    }

private:
    CodeNode *_IsolateCaseValue(CodeNode &node)
    {
        // This should consist of:
        // bnt
        //   eq?
        //     dup
        //     *something*
        // We're really only interested in the *someting*
        CodeNode *pNodeRet = &node;
        if ((node.GetCode()->get_opcode() == acBNT) && (node.GetChildCount() >= 1))
        {
            CodeNode *pNodeEq = node.Child(0);
            if ((pNodeEq->GetCode()->get_opcode() == acEQ) && (pNodeEq->GetChildCount() >= 2))
            {
                if (pNodeEq->Child(0)->GetCode()->get_opcode() == acDUP)
                {
                    pNodeRet = pNodeEq->Child(1);
                }
            }
        }
        return pNodeRet;
    }

    bool _fSwitch;
    codenode_it _switch;
    struct switchvalue_t
    {
        codenode_it value;  // The pivot 
        codenode_it begin;  // Beginning of code for this value
        codenode_it end;    // End of code for this value
    };
    std::vector<switchvalue_t> _values;
};

//
// Handle if statements, as well as while loops
//
class CodeTypeIfElse : public CodeShape
{
public:
    CodeTypeIfElse() : _fElse(false), _fWhile(false) {}

    void AddCondition(codenode_it condition)
    {
        _conditions.push_back(condition);
    }

    void SetIf(codenode_it ifBegin, codenode_it ifEnd)
    {
        _ifBegin = ifBegin;
        _ifEnd = ifEnd;
    }

    void SetWhile(bool fWhile)
    {
        _fWhile = fWhile;
    }

    void SetElse(codenode_it elseBegin, codenode_it elseEnd)
    {
        _fElse = true;
        _elseBegin = elseBegin;
        _elseEnd = elseEnd;
    }

    virtual SingleStatementPtr DoIt(IDecompileLookups &lookups)
    {
        auto_ptr<SingleStatement> pStatement(new SingleStatement);
        if (_fWhile)
        {
            ASSERT(!_fElse);
            auto_ptr<WhileLoop> pWhile(new WhileLoop);
            _ApplyConditions(*pWhile, lookups);
            _ProcessCodeNodesIntoStatements(*pWhile, _ifBegin, _ifEnd, lookups);
            pStatement->SetSyntaxNode(pWhile);
        }
        else
        {
            auto_ptr<CppIfStatement> pIf(new CppIfStatement);

            // The conditions - they are all OR'd together
            _ApplyConditions(*pIf, lookups);

            _ApplyMainCodeBlock(*pIf, lookups);

            // The else
            if (_fElse)
            {
                auto_ptr<CodeBlock> pElseBlock(new CodeBlock);
                _ProcessCodeNodesIntoStatements(*pElseBlock, _elseBegin, _elseEnd, lookups);
                SingleStatement elseBlockStatement;
                elseBlockStatement.SetSyntaxNode(pElseBlock);
                pIf->SetStatement2(elseBlockStatement);
            }

            pStatement->SetSyntaxNode(pIf);
        }
        return pStatement.release();
    }

    virtual void Print(std::ostream &os, int iIndent) const
    {
        os << _indent(iIndent) << (_fWhile ? "[while]\n" : "[if]\n");
        for (size_t i = 0; i < _conditions.size(); ++i)
        {
            (*_conditions[i])->Print(os, iIndent + 2);
        }
        os << _indent(iIndent) << "[then]\n";
        codenode_it it = _ifBegin;
        while (it != _ifEnd)
        {
            (*it)->Print(os, iIndent + 2);
            ++it;
        }

        if (_fElse)
        {
            os << _indent(iIndent) << "[else]\n";
            it = _elseBegin;
            while (it != _elseEnd)
            {
                (*it)->Print(os, iIndent + 2);
                ++it;
            }
        }
    }

    // Used to identify a while loop - we try to get the codenode_it that
    // represents the first condition in a loop
    codenode_it GetBeginningOfConditions(codenode_it currentNotYetAdded)
    {
        if (_conditions.empty())
        {
            return currentNotYetAdded;
        }
        else
        {
            return _conditions[0];
        }
    }

private:
    void _ApplyMainCodeBlock(OneStatementNode &statementNode, IDecompileLookups &lookups)
    {
        // The if
        auto_ptr<CodeBlock> pIfBlock(new CodeBlock);
        _ProcessCodeNodesIntoStatements(*pIfBlock, _ifBegin, _ifEnd, lookups);
        SingleStatement ifBlockStatement;
        ifBlockStatement.SetSyntaxNode(pIfBlock);
        statementNode.SetStatement1(ifBlockStatement);
    }

    void _ApplyConditions(ConditionNode &conditionNode, IDecompileLookups &lookups)
    {
        // The conditions - they are all OR'd together
        auto_ptr<ConditionalExpression> pCondition(new ConditionalExpression);
        for (size_t i = 0; i < _conditions.size(); ++i)
        {
            _ApplySyntaxNodeToCodeNode(*(*_conditions[i]), *pCondition, lookups);
            if (i > 0)
            {
                pCondition->AddAndOr(false);
            }
        }
        conditionNode.SetCondition(*pCondition);
    }

    std::vector<codenode_it> _conditions;
    codenode_it _ifBegin;
    codenode_it _ifEnd;
    bool _fElse;
    codenode_it _elseBegin;
    codenode_it _elseEnd;
    bool _fWhile;
};

// Given a CodeShape vector, looks at the CodeNode iterators and turns them into CodeShapes...
void _PopulateCodeShapes(std::vector<CodeShape*> &shapes, codenode_it begin, codenode_it end, IDecompileLookups &lookups)
{
    codenode_it it = begin;

    // Keep an if/else code shape around, as these could be complex - we can't complete them in one go.
    auto_ptr<CodeTypeIfElse> pShapeIfElse(new CodeTypeIfElse);

    while (it != end)
    {
        CodeNode *pNode = *it;

        // Scan for if statement
        if (pNode->GetCode()->get_opcode() == acBT)
        {
            // This generally indicates an OR condition in an if statement.  Let's see if its target
            // points to a acBNT... that is usually a good indicator.  The Sierra games tend to have
            // the acBT pointing to the final acBNT which is used to jump over the +ve code
            code_pos posTarget = pNode->GetCode()->get_branch_target();
            if (posTarget->get_opcode() == acBNT)
            {
                // TODO: Ensure this is the same acBNT as what we choose in our if statement...
                pShapeIfElse->AddCondition(it);
            }
            else
            {
                // PHIL this is hit alot in Laura Bow
                ASSERT(FALSE);
            }
            ++it;
        }
        else if (pNode->GetCode()->get_opcode() == acBNT)
        {
            // Start of a normal if statement.
            bool fElse = false;
            bool fWhileLoop = false;
            code_pos posTarget = pNode->GetCode()->get_branch_target();
            // Try to match the target up to the beginning of another codenode.
            codenode_it itIf = it;
            codenode_it itCondition = it;
            ++itIf;
            codenode_it itIfBegin = itIf;
            codenode_it itIfEnd = end;  // Just in case we never hit the target (it's outside of our scope)
            codenode_it itElseBegin = itIf;
            codenode_it itElseEnd = end;// Just in case we never hit the target (it's outside of our scope)
            while (itIf != end)
            {
                if (_ScanCodeNodeForCodePos(*itIf, posTarget))
                {
                    // Found where this jumps to.  The end of the +ve if clause.
                    // Now back up and see if this is an else...
                    itIfEnd = itIf;
                    codenode_it itMaybeJump = itIfEnd;
                    --itMaybeJump;
                    if ((*itMaybeJump)->GetCode()->get_opcode() == acJMP)
                    {
                        --itIfEnd; // If this was a jump, don't include it in the +ve branch
                        code_pos posTarget = (*itMaybeJump)->GetCode()->get_branch_target();
                        // Now find out where it jumped to.

                        // First check if this is a while loop, by seeing if it jumps to the
                        // condition of our "if statement"
                        // NOTE: when we have an && in our while statement, we'll end up with an if and a while... oh oh
                        // then our while detection will be broken - PHIL
                        if (_ScanCodeNodeForCodePos(*pShapeIfElse->GetBeginningOfConditions(itCondition), posTarget))
                        {
                            fWhileLoop = true;
                        }
                        else
                        {
                            codenode_it itElse = itIf;
                            itElseBegin = itElse;
                            while (itElse!= end)
                            {
                                if (_ScanCodeNodeForCodePos(*itElse, posTarget))
                                {
                                    break;
                                }
                                ++itElse;
                            }
                            // NOTE: If we didn't find the target of the acJMP in our thing, then
                            // we're going to still assume it was an else, and the entire rest of our thing
                            // is the else clause.  When we have nested if statements, the acJMPs at the end of
                            // the if clauses don't jump right after the else clauses... they'll jump outside
                            // our begin/end.
                            // NOTE: We may need to revisit this logic when dealing with loops - and check to make
                            // sure the jmp wasn't to somewhere before us.
                            // This was the target branch.
                            fElse = true;
                            itElseEnd = itElse;
                        }
                    }
                    break;
                }
                ++itIf;
            }

            pShapeIfElse->AddCondition(itCondition);
            pShapeIfElse->SetIf(itIfBegin, itIfEnd);
            pShapeIfElse->SetWhile(fWhileLoop);
            it = itIfEnd;
            if (fElse)
            {
                // Give it an else too.
                pShapeIfElse->SetElse(itElseBegin, itElseEnd);
                it = itElseEnd; // This needs to point here...
            }
            shapes.push_back(pShapeIfElse.get());
            pShapeIfElse.release();
            pShapeIfElse.reset(new CodeTypeIfElse); // Put a new blank one in...
        }
        else if (pNode->GetCode()->get_opcode() == acTOSS)
        {
            auto_ptr<CodeTypeSwitch> pSwitch(new CodeTypeSwitch);
            // This generally indicates a switch statement.  Furthermore, all the children are within the switch
            // statement, unlike ifs for example.  So we don't need to explore outside this codenode's children.
            // There should be a whole bunch of children.  The first one will be a push.
            codenode_it itChild = pNode->begin();
            if (itChild != pNode->end())
            {
                Consumption cons = _GetInstructionConsumption(*(*itChild)->GetCode());
                if (cons.cStackGenerate) // Could be acPUSH, or something like acPTOS
                //if ((*itChild)->GetCode()->get_opcode() == acPUSH)
                {
                    // This is our switch pivot then.
                    pSwitch->SetSwitch(itChild);
                }
                else
                {
                    pSwitch->SetSwitch(itChild); // phil, oh well...
                    ASSERT(FALSE);
                }
                ++itChild;
            }
            while (itChild != pNode->end())
            {
                // Now we expect a series of bnt instructions finally ending in our toss.
                if ((*itChild)->GetCode()->get_opcode() == acBNT)
                {
                    codenode_it switchValue = itChild; // 1
                    code_pos posTarget = (*itChild)->GetCode()->get_branch_target();
                    ++itChild;
                    codenode_it switchBegin = itChild; // 2
                    // Scan for this target...
                    codenode_it nextValue = _FindCodeNodeWithCodePos(itChild, pNode->end(), posTarget);
                    if (nextValue != pNode->end())
                    {
                        codenode_it switchEnd = nextValue;  // 3
                        // Add the switch value
                        pSwitch->AddValue(switchValue, switchBegin, switchEnd);
                        itChild = nextValue;
                    }
                    else if (posTarget->get_opcode() == acTOSS)
                    {
                        // Aha, this was our root TOSS instruction.
                        codenode_it switchEnd = pNode->end();
                        pSwitch->AddValue(switchValue, switchBegin, switchEnd);
                        itChild = pNode->end();
                    }
                    else
                    {
                        // phil, this might be the default case
                        ASSERT(FALSE);
                    }
                }
                else if ((*itChild)->GetCode()->get_opcode() == acTOSS)
                {
                    ASSERT(FALSE); // ???
                    ++itChild;
                }
                else
                {
                    //ASSERT(FALSE); // phil fix - we hit now with my ternary hack
                    ++itChild;
                }
            }

            shapes.push_back(pSwitch.get());
            pSwitch.release();
            ++it;
        }
        else
        {
            // Normal for now...
            shapes.push_back(new CodeTypeNormal(it));
            ++it;
        }
    }

}

bool _FindNodeAndApplyLabel(char &label, code_pos target, codenode_it begin, codenode_it end)
{
    bool fRet = false;
    while (!fRet && (begin != end))
    {
        fRet = ((*begin)->GetCode() == target);
        if (fRet)
        {
            (*begin)->SetLabel(label);
            ++label;
        }
        else
        {
            fRet = _FindNodeAndApplyLabel(label, target, (*begin)->begin(), (*begin)->end());
        }
        ++begin;
    }
    return fRet;
}

// For debugging
void _ApplyBranchLabelsWorker(char &label, codenode_it begin, codenode_it end, codenode_it beginMaster, codenode_it endMaster)
{
    while (begin != end)
    {
        if (IsBranchInstruction((*begin)->GetCode()))
        {
            (*begin)->SetTargetLabel(label);
            // Find its target.
            if (!_FindNodeAndApplyLabel(label, (*begin)->GetCode()->get_branch_target(), beginMaster, endMaster))
            {
                (*begin)->SetTargetLabel('?');
            }
        }
        // Go to the children:
        _ApplyBranchLabelsWorker(label, (*begin)->begin(), (*begin)->end(), beginMaster, endMaster);
        ++begin;
    }
}
void _ApplyBranchLabels(char &label, codenode_it begin, codenode_it end)
{
    _ApplyBranchLabelsWorker(label, begin, end, begin, end);
}

void DecompileRaw(FunctionBase &func, IDecompileLookups &lookups, const BYTE *pBegin, const BYTE *pEnd, WORD wBaseOffset)
{
    lookups.EndowWithFunction(&func);

    // Take the raw data, and turn it into a list of scii instructions, and make sure the branch targets point to code_pos's
    std::list<scii> code;
    _ConvertToInstructions(code, pBegin, pEnd, wBaseOffset);

    // Now make them hierarchical.
    std::vector<CodeNode*> codeNodes;

    code_pos pos = code.end();
    --pos; // Go to last instruction
    code_pos start = code.begin();
    --start; // This is the "off the end" marker
    while (pos != start)
    {
        auto_ptr<CodeNode> pRoot(new CodeNode(pos));
        _GenerateInstructionTree(*pRoot, start, pos);
        // Insert at beginning, since we're going backwards:
        codeNodes.insert(codeNodes.begin(), pRoot.get());
        pRoot.release();
    }

    char label = 'A';
    _ApplyBranchLabels(label, codeNodes.begin(), codeNodes.end());

    // Now we have a hierarchical thing.  Separate into branch blocks.  We may need to do this over and over.
    std::vector<CodeShape*> codeShapes;

    _PopulateCodeShapes(codeShapes, codeNodes.begin(), codeNodes.end(), lookups);

    // Debug out
    auto_ptr<Comment> pComment(new Comment);
    std::stringstream ss;
    ss << "/*\n";
    std::vector<CodeShape*>::iterator itDebug = codeShapes.begin();
    while (itDebug != codeShapes.end())
    {
        (*itDebug)->Print(ss, 2);
        ++itDebug;
        ss << "------------\n";
    }
    ss << "*/\n";
    pComment->SetName(ss.str());
    lookups.SetPosition(pComment.get());
    func.GetOwnerScript()->AddComment(pComment);
    lookups.SetPosition(&func); // Make the function after, so the comment is spit out.


    // Construct the function -> for now use procedure, but really should be method or proc
    auto_ptr<FunctionSignature> pSignature(new FunctionSignature);
    _FigureOutParameters(func, *pSignature, code);
    func.AddSignature(pSignature);
    _FigureOutTempVariables(func, 2, code); // phil: 2 -> temp vars
    // Make a function and add statements!
    std::vector<CodeShape*>::iterator itCT = codeShapes.begin();
    while (itCT != codeShapes.end())
    {
        auto_ptr<SingleStatement> pStatement((*itCT)->DoIt(lookups));
        func.AddStatement(pStatement);
        ++itCT;
    }

    for_each(codeShapes.begin(), codeShapes.end(), delete_object());
    for_each(codeNodes.begin(), codeNodes.end(), delete_object());

    lookups.EndowWithFunction(NULL);
}

bool _IsPrintProcedure(WORD wScript, WORD wIndex)
{
    if (wScript == 255 && wIndex == 0)
    {
        return true; // True for space quest
    }
    return false;
}

bool IsStatementImmediateValue(const SingleStatement &statement, WORD &wValue)
{
    const PropertyValue *pValue = statement.CastSyntaxNode<PropertyValue>();
    bool fRet = (pValue && (pValue->GetType() == ValueTypeNumber));
    if (fRet)
    {
        wValue = pValue->GetNumberValue();
    }
    return fRet;
}

bool _MaybeAProperty(const std::string &selector)
{
    static g_fInited = false;
    static std::set<std::string> g_PropertyMap;
    static const std::string c_rgTendsToBeProperty[] =
    {
        "x",
        "y",
        "z",
        "view",
        "cel",
        "loop",
        "script",
        "number",
        "timer",
        "keep",
        "initialized",
        "picture",
        "style",
        "horizon",
        "controls",
        "north",
        "east",
        "west",
        "south",
        "curPic",
        "picAngle",
        "vanishingX",
        "vanishingY",
        "priority",
        "underbits",
        "heading",
        "yStep",
        "xStep",
        "signal",
        "nsTop",
        "nsBottom",
        "nsLeft",
        "nsRight",
        "lsTop",
        "lsBottom",
        "lsLeft",
        "lsRight",
        "brTop",
        "brBottom",
        "brLeft",
        "brRight",
        "cycleSpeed",
        "cycler",
        "illegalBits",
        "xLast",
        "yLast",
        "moveSpeed",
        "blocks",
        "baseSetter",
        "mover",
        "looper",
        "viewer",
        "avoider",
        "edgeHit",
        "elements",
        "size",
        "text",
        "window",
        "theItem",
        "time",
        "busy",
        "seconds",
        "lastSeconds",
        "cycles",
        "higher",
        "lower",
        "normal",
        "minimum",
        "maximum",
        "description",
        "owner",
        "said",
        "curEvent",
        "mapKeyToDir",
        "inputLineAddr",
        "prompt",
        "prevDir",
        "echo",
        "canInput",
        "alterEgo",
        "species",
    };

    if (!g_fInited)
    {
        g_fInited = true;
        g_PropertyMap.insert(&c_rgTendsToBeProperty[0], &c_rgTendsToBeProperty[ARRAYSIZE(c_rgTendsToBeProperty) - 1]);
    }
    return (g_PropertyMap.find(selector) != g_PropertyMap.end());
}

//
// Try to massage the parameters so that it makes more sense to the user.
//
void _MassageProcedureCall(ProcedureCall &proc, IDecompileLookups &lookups, code_pos pos)
{
    if (pos->get_opcode() == acCALLE)
    {
        if (_IsPrintProcedure(pos->get_first_operand(), pos->get_second_operand()))
        {
            proc.SetName("Print");

            const SingleStatement *pOne = proc.GetParameter(0);
            const SingleStatement *pTwo = proc.GetParameter(1);
            WORD wOne;
            WORD wTwo;
            if (pOne && IsStatementImmediateValue(*pOne, wOne) &&
                pTwo && IsStatementImmediateValue(*pTwo, wTwo))
            {
                // If the first two parameters are immediates, and the first one matches the current
                // script number, then assume it's a text resource.
                if (wOne == lookups.GetScriptNumber())
                {
                    std::string text = lookups.LookupTextResource(wTwo);
                    if (!text.empty())
                    {
                        auto_ptr<SingleStatement> pNewStatement(new SingleStatement);
                        auto_ptr<PropertyValue> pNewValue(new PropertyValue);
                        pNewValue->SetValue(text, ValueTypeString);
                        pNewStatement->SetSyntaxNode(pNewValue);
                        SingleStatementVector parameters;
                        parameters.push_back(pNewStatement.get());
                        pNewStatement.release();
                        proc.StealParams(parameters); // Do the swap
                        for_each(parameters.begin(), parameters.end(), delete_object());
                    }
                }
            }
        }
    }
    else if (pos->get_opcode() == acCALLK)
    {
        if (proc.GetName() == "Load")
        {
            SingleStatement *pOne = proc.GetParameter(0);
            if (pOne)
            {
                PropertyValue *pValue = pOne->CastSyntaxNode<PropertyValue>();
                if (pValue && (pValue->GetType() == ValueTypeNumber))
                {
                    static const std::string rgResourceTypes[] = 
                    {
                        "rsVIEW", "rsPIC", "rsSCRIPT", "rsTEXT", "rsSOUND", "rsMEMORY", "rsVOCAB", "rsFONT", "rsCURSOR", "rsPATCH"
                    };
                    if ((pValue->GetNumberValue() >= 0x0080) && pValue->GetNumberValue() < (0x0080 + ARRAYSIZE(rgResourceTypes)))
                    {
                        pValue->SetValue(rgResourceTypes[pValue->GetNumberValue() - 0x0080], ValueTypeToken);
                    }
                }
            }
        }
    }
}

// Returns the token string in a PropertyValue, or an empty string
std::string _PluckTokenFromStatement(SingleStatement &statement)
{
    std::string ret;
    const PropertyValue *pValue = statement.CastSyntaxNode<PropertyValue>();
    if (pValue && (pValue->GetType() == ValueTypeToken))
    {
        ret = pValue->GetStringValue();
    }
    return ret;
}

WORD _GetImmediateFromCodeNode(CodeNode &node, CodeNode *pNodePrevious = NULL)
{
    WORD w = 0;
    code_pos pos = node.GetCode();
    switch (pos->get_opcode())
    {
    case acLDI:
        w = pos->get_first_operand();
        break;

    case acPUSHI:
        w = pos->get_first_operand();
        break;

    case acPUSH0:
        w = 0;
        break;

    case acPUSH1:
        w = 1;
        break;

    case acPUSH2:
        w = 2;
        break;

    case acPUSH:
        // phil, hits too often..... ldi jmp ldi push
        //ASSERT(node.GetChildCount());
        break;

    case acDUP:
        if (pNodePrevious)
        {
            w = _GetImmediateFromCodeNode(*pNodePrevious);
        }
        else
        {
            code_pos posFlatPrevious = pos;
            --posFlatPrevious;
            // REVIEW: could crash...
            CodeNode nodeTemp(posFlatPrevious);
            w = _GetImmediateFromCodeNode(nodeTemp);
        }
        break;

    default:
        ASSERT(FALSE);
        break;
    }
    return w;
}

void _ApplySyntaxNodeToCodeNode(CodeNode &node, StatementsNode &statementsNode, IDecompileLookups &lookups, CodeNode *pPreviousNode)
{
    auto_ptr<SingleStatement> pStatement(new SingleStatement);
    pStatement->SetSyntaxNode(_CodeNodeToSyntaxNode(node, lookups, pPreviousNode));
    statementsNode.AddStatement(pStatement);
}

void _ApplySyntaxNodeToCodeNode1(CodeNode &node, OneStatementNode &statementsNode, IDecompileLookups &lookups, CodeNode *pPreviousNode)
{
    auto_ptr<SingleStatement> pStatement(new SingleStatement);
    pStatement->SetSyntaxNode(_CodeNodeToSyntaxNode(node, lookups, pPreviousNode));
    statementsNode.SetStatement1(*pStatement);
}

void _ApplySyntaxNodeToCodeNode2(CodeNode &node, TwoStatementNode &statementsNode, IDecompileLookups &lookups, CodeNode *pPreviousNode)
{
    auto_ptr<SingleStatement> pStatement(new SingleStatement);
    pStatement->SetSyntaxNode(_CodeNodeToSyntaxNode(node, lookups, pPreviousNode));
    statementsNode.SetStatement2(*pStatement);
}

// Both for variable opcodes, and acLEA
std::string _GetVariableNameFromCodePos(code_pos pos, IDecompileLookups &lookups)
{
    std::string name;
    BYTE bThing;
    WORD wIndex;
    if (pos->get_opcode() == acLEA)
    {
        bThing = (BYTE)pos->get_first_operand();
        bThing >>= 1;
        wIndex = pos->get_second_operand();
    }
    else
    {
        bThing = pos->get_opcode();
        wIndex = pos->get_first_operand();
        ASSERT(bThing >= 64);
    }
    std::stringstream ss;
    switch (bThing & 0x3)
    {
    case 0x00:
        {
            // global
            name = lookups.ReverseLookupGlobalVariableName(wIndex);
            if (name.empty())
            {
                ss << "global" << wIndex;
                name = ss.str();
            }
        }
        break;
    case 0x01:
        // local
        ss << "local" << wIndex;
        name = ss.str();
        break;
    case 0x02:
        // temp
        ss << "temp" << wIndex;
        name = ss.str();
        break;
    case 0x03:
        // param
        name = lookups.LookupParameterName(wIndex);
        break;
    }
    return name;
}

SyntaxNode *_CodeNodeToSyntaxNode(CodeNode &node, IDecompileLookups &lookups, CodeNode *pPreviousNode)
{
    SyntaxNode *pNodeRet = NULL;
    code_pos pos = node.GetCode();
    BYTE bOpcode = pos->get_opcode();
    switch (bOpcode)
    {
    case acLDI:
    case acPUSHI:
    case acPUSH0:
    case acPUSH1:
    case acPUSH2:
        {
            auto_ptr<PropertyValue> value(new PropertyValue);
            switch (bOpcode)
            {
            case acPUSH0:
                value->SetValue(0);
                break;
            case acPUSH1:
                value->SetValue(1);
                break;
            case acPUSH2:
                value->SetValue(2);
                break;
            default: // LDI and PUSHI
                value->SetValue(pos->get_first_operand());
                break;
            }
            pNodeRet = value.release();
        }
        break;

    case acPUSH:
        if (node.GetChildCount() >= 1)
        {
            if (node.GetChildCount() == 3)
            {
                // The unusual case... our hack for a ternary operator.
                auto_ptr<CppIfStatement> pTernary(new CppIfStatement);
                pTernary->MakeTernary();
                CodeNode *pConditionNode = node.Child(0);
                // phil - laura bow, script 30 has acCLASS here... ?
                ASSERT(pConditionNode->GetCode()->get_opcode() == acBNT);
                ConditionalExpression condition;
                _ApplySyntaxNodeToCodeNode(*pConditionNode, condition, lookups);
                pTernary->SetCondition(condition);
                _ApplySyntaxNodeToCodeNode1(*node.Child(1), *pTernary, lookups);
                _ApplySyntaxNodeToCodeNode2(*node.Child(2), *pTernary, lookups);
                pNodeRet = pTernary.release();
            }
            else
            {
                // Fwd to the first child (should be what gets in to the accumulator)
                pNodeRet = _CodeNodeToSyntaxNode(*node.Child(0), lookups);
            }
        }
        break;

    case acCALLK:   // kernel, # of params
    case acCALLB:   // mainproc index, # of params
    case acCALLE:   // script, index, # of params
    case acCALL:    // offset, # of params
        {
            WORD cParams = (bOpcode == acCALLE) ? pos->get_third_operand() : pos->get_second_operand();
            cParams /= 2; // bytes -> params
            cParams += 1; // +1 because there is a parameter count first.
            std::stringstream ss;
            switch (bOpcode)
            {
            case acCALLK:
                ss << lookups.LookupKernelName(pos->get_first_operand());
                break;
            case acCALLB:
                ss << "proc000_" << pos->get_first_operand();
                break;
            case acCALLE:
                ss << _GetPublicProcedureName(pos->get_first_operand(), pos->get_second_operand());
                break;
            case acCALL:
                ss << _GetProcNameFromScriptOffset(pos->get_final_postop_offset() + pos->get_first_operand());
                break;
            }
            auto_ptr<ProcedureCall> pProcCall(new ProcedureCall);
            pProcCall->SetName(ss.str());
            for (WORD i = 0; i < cParams && i < node.GetChildCount(); ++i)
            {
                if (i == 0)
                {
                    // The first one should just be the number of parameters.
                    WORD cParamsCheck = _GetImmediateFromCodeNode(*node.Child(i));
                    ASSERT((cParamsCheck + 1) == cParams);
                }
                else
                {
                    _ApplySyntaxNodeToCodeNode(*node.Child(i), *pProcCall, lookups, node.Child(i - 1));
                }
            }
            _MassageProcedureCall(*pProcCall, lookups, pos);
            pNodeRet = pProcCall.release();
        }
        break;

    case acSELF:
    case acSEND:
    case acSUPER:
        {
            auto_ptr<SendCall> sendCall(new SendCall);
            if (bOpcode == acSELF)
            {
                sendCall->SetName("self");
            }
            else if (bOpcode == acSUPER)
            {
                // TODO - actually you can send to any super class... the first operand says which
                sendCall->SetName("super");
            }
            Consumption cons = _GetInstructionConsumption(*pos);
            WORD cStackPushesLeft = cons.cStackConsume;
            WORD cAccLeft = (bOpcode == acSEND) ? 1 : 0;
            size_t i = 0;
            WORD cParamsLeft = 0;
            bool fLookingForSelector = true;
            auto_ptr<SendParam> sendParam;
            while ((cAccLeft || cStackPushesLeft) && (i < node.GetChildCount()))
            {
                CodeNode *pPreviousChild = i ? node.Child(i - 1) : NULL;
                if (cAccLeft)
                {
                    Consumption consAcc = _GetInstructionConsumption(*node.Child(i)->GetCode());
                    if (consAcc.cAccGenerate)
                    {
                        auto_ptr<SyntaxNode> pSendObject(_CodeNodeToSyntaxNode(*node.Child(i), lookups, pPreviousNode));
                        const PropertyValue *pValue = SafeSyntaxNode<const PropertyValue*>(pSendObject.get());
                        if (pValue && (pValue->GetType() == ValueTypeToken))
                        {
                            sendCall->SetName(pValue->GetStringValue());
                        }
                        else
                        {
                            SingleStatement statement;
                            statement.SetSyntaxNode(pSendObject);
                            sendCall->SetStatement1(statement);
                        }
                        --cAccLeft;
                    }
                }

                if (cStackPushesLeft)
                {
                    Consumption consAcc = _GetInstructionConsumption(*node.Child(i)->GetCode());
                    if (consAcc.cStackGenerate)
                    {
                        if (fLookingForSelector)
                        {
                            sendParam.reset(new SendParam);
                            fLookingForSelector = false;
                            WORD wValue = _GetImmediateFromCodeNode(*node.Child(i), pPreviousChild);
                            sendParam->SetName(lookups.LookupSelectorName(wValue));
                            sendParam->SetIsMethod(!_MaybeAProperty(sendParam->GetName()));
                            // TODO: handle selectors that aren't immediate values.
                        }
                        else if (cParamsLeft)
                        {
                            --cParamsLeft;
                            _ApplySyntaxNodeToCodeNode(*node.Child(i), *sendParam, lookups, pPreviousChild);
                            if (cParamsLeft == 0)
                            {
                                sendCall->AddSendParam(sendParam);
                                fLookingForSelector = true;
                            }
                        }
                        else
                        {
                            // Must be a param count
                            cParamsLeft = _GetImmediateFromCodeNode(*node.Child(i), pPreviousChild);
                            if (cParamsLeft == 0)
                            {
                                sendCall->AddSendParam(sendParam);
                                fLookingForSelector = true;
                            }
                        }
                        --cStackPushesLeft;
                    }
                }
                ++i; // Always increment i
                // TODO: warn if we didn't do anything in this loop?  Unused instruction???
            }
            sendCall->SimplifySendObject();
            pNodeRet = sendCall.release();
        }
        break;

    case acBNT:
    case acBT:
        // phil -> ternary operator is a common thing (e.g. space quest script 005)
        // How can we do things that cross branch boundaries?  We could look for this pattern:
        //      eq?
        //      bnt A
        //      *something*
        //      jmp B
        // A:   *something*
        // B:   push
        //

        if (node.GetChildCount() >= 1)
        {
            // This should have one child, which is the condition...
            pNodeRet = _CodeNodeToSyntaxNode(*node.Child(0), lookups);
        }
        break;

    case acSUB:
    case acMUL:
    case acDIV:
    case acMOD:
    case acSHR:
    case acSHL:
    case acXOR:
    case acAND:
    case acADD:
    case acEQ:
    case acGT:
    case acLT:
    case acLE:
    case acNE:
    case acGE:
    case acUGT:
    case acUGE:
    case acULT:
    case acULE:
        if (node.GetChildCount() >= 2)
        {
            auto_ptr<BinaryOp> binaryOp(new BinaryOp);
            // The child that generates the stack push should be the first statement.
            CodeNode *pCodeNodeStack = node.Child(0);
            CodeNode *pCodeNodeAcc = node.Child(1);
            Consumption cons = _GetInstructionConsumption(*pCodeNodeStack->GetCode());
            if (!cons.cStackGenerate)
            {
                // Ooops... the other one must have been the stack-generating one.
                swap(pCodeNodeStack, pCodeNodeAcc);
            }
            _ApplySyntaxNodeToCodeNode1(*pCodeNodeStack, *binaryOp, lookups);
            _ApplySyntaxNodeToCodeNode2(*pCodeNodeAcc, *binaryOp, lookups);
            binaryOp->SetName(GetBinaryOperatorForInstruction(bOpcode));
            pNodeRet = binaryOp.release();
        }
        break;

    case acBNOT:
    case acNOT:
    case acNEG:
        if (node.GetChildCount() >= 1)
        {
            auto_ptr<UnaryOp> unaryOp(new UnaryOp);
            CodeNode *pCodeNodeAcc = node.Child(0);
            Consumption cons = _GetInstructionConsumption(*pCodeNodeAcc->GetCode());
            ASSERT(cons.cAccGenerate);
            _ApplySyntaxNodeToCodeNode1(*pCodeNodeAcc, *unaryOp, lookups);
            unaryOp->SetName(GetUnaryOperatorForInstruction(bOpcode));
            pNodeRet = unaryOp.release();
        }
        break;

    case acREST:
        {
            auto_ptr<PropertyValue> pValue(new PropertyValue);
            pValue->SetValue("rest", ValueTypeToken);
            pNodeRet = pValue.release();
        }
        break;

    case acLEA:
        {
            WORD wType = pos->get_first_operand();
            auto_ptr<PropertyValue> pValue(new PropertyValue);
            pValue->SetValue(_GetVariableNameFromCodePos(pos, lookups), ValueTypePointer);
            pNodeRet = pValue.release();
            ASSERT((wType & LEA_ACC_AS_INDEX_MOD) == 0); // phil TODO
        }
        break;

    case acCLASS:
    case acPUSHSELF:
        {
            std::string className = (bOpcode == acCLASS) ? lookups.LookupClassName(pos->get_first_operand()) : "self";
            if (!className.empty())
            {
                auto_ptr<PropertyValue> value(new PropertyValue);
                value->SetValue(className, ValueTypeToken);
                pNodeRet = value.release();
            }
        }
        break;

    case acLOFSA:
    case acLOFSS:
        {
            // The first operand specifies an offset from the start of the next instruction.
            ICompiledScriptSpecificLookups::ObjectType type;
            std::string name = lookups.LookupScriptThing(pos->get_first_operand() + pos->get_final_postop_offset(), type);
            ASSERT(!name.empty());
            if (!name.empty())
            {
                auto_ptr<PropertyValue> value(new PropertyValue);
                value->SetValue(name, _ScriptObjectTypeToPropertyValueType(type));
                pNodeRet = value.release();
            }
        }
        break;

    case acATOP:    // acc to property index
    case acSTOP:    // Stack to property index
        {
            auto_ptr<Assignment> pAssignment(new Assignment);
            if (node.GetChildCount())
            {
                _ApplySyntaxNodeToCodeNode1(*node.Child(0), *pAssignment, lookups);
            }
            // Now this is a property... find out which.
            WORD wPropertyIndex = node.GetCode()->get_first_operand();
            LValue lValue;
            lValue.SetName(lookups.LookupPropertyName(wPropertyIndex));
            pAssignment->SetVariable(lValue);
            pAssignment->SetName("=");
            pNodeRet = pAssignment.release();
        }
        break;

    case acPTOS:    // property index to stack
    case acPTOA:    // property index to acc
    case acIPTOA:   // Inc prop to acc
    case acDPTOA:   // Dec prop to acc
    case acIPTOS:   // Inc prop to stack
    case acDPTOS:   // Dec prop to stack
        {
            auto_ptr<PropertyValue> pValue(new PropertyValue);
            ASSERT(node.GetChildCount() == 0);
            WORD wPropertyIndex = node.GetCode()->get_first_operand();
            pValue->SetValue(lookups.LookupPropertyName(wPropertyIndex), ValueTypeToken);
            bool fIncrement = (bOpcode == acIPTOA) || (bOpcode == acIPTOS);
            bool fDecrement = (bOpcode == acDPTOA) || (bOpcode == acDPTOS);
            if (fIncrement || fDecrement)
            {
                // Phil - TODO - copied code with below..
                auto_ptr<UnaryOp> pUnary(new UnaryOp);
                pUnary->SetName(fIncrement ? "++" : "--");
                SingleStatement statement;
                statement.SetSyntaxNode(pValue);
                pUnary->SetStatement1(statement);
                pNodeRet = pUnary.release();
            }
            else
            {
                pNodeRet = pValue.release();
            }
        }
        break;

    case acDUP:
        // How do we handle this one?
        ASSERT(pPreviousNode);  // We'll need to handle this due to bad code, but for now assert. phil
        if (pPreviousNode)
        {
            pNodeRet = _CodeNodeToSyntaxNode(*pPreviousNode, lookups);
        }
        else
        {
            // TODO: Walk backwards until we have an instruction that puts something on the stack.
            // This may not work in all cases (if we pass a branch, etc...)
        }
        break;

    case acJMP:
        // Already handled by branching code
        pNodeRet = new Comment();
        break;

    case acLINK:
        pNodeRet = new Comment();
        break;

    case acRET:
        {
            auto_ptr<ReturnStatement> pReturn(new ReturnStatement());
            if (node.GetChildCount())
            {
                _ApplySyntaxNodeToCodeNode1(*node.Child(0), *pReturn, lookups);
            }
            pNodeRet = pReturn.release();
            // TODO: Difficult to determine if the code actually meant to return anything...
            // we'll need to check if the previous thing was "intentional" or not...
            // We could have trouble with branching...
        }
        break;

    default:
        if ((bOpcode >= acLAG) && (bOpcode <= acnSPI))
        {
            // This could be a load or store operation.
            if (_IsVOStoreOperation(bOpcode))
            {
                // Store operation: Assignment (LValue/PropertyValue)
                auto_ptr<Assignment> pAssignment(new Assignment);
                pAssignment->SetName("=");

                // The variable name
                LValue lValue;
                lValue.SetName(_GetVariableNameFromCodePos(pos, lookups));
                if (_IsVOIndexed(bOpcode)) // The accumulator is used as an indexer.
                {
                    for (size_t i = 0; i < node.GetChildCount(); i++)
                    {
                        Consumption cons = _GetInstructionConsumption(*node.Child(i)->GetCode());
                        if (cons.cAccGenerate)
                        {
                            SingleStatement statement;
                            auto_ptr<SyntaxNode> pIndexerNode(_CodeNodeToSyntaxNode(*node.Child(i), lookups));
                            statement.SetSyntaxNode(pIndexerNode);
                            lValue.SetIndexer(statement);
                            break;
                        }
                    }
                }
                pAssignment->SetVariable(lValue);

                // Is assigned our child:
                ASSERT(node.GetChildCount() >= 1);
                for (size_t i = 0; i < node.GetChildCount(); i++)
                {
                    Consumption cons = _GetInstructionConsumption(*node.Child(i)->GetCode());
                    if ((_IsVOStack(bOpcode) && cons.cStackGenerate) || (!_IsVOStack(bOpcode) && cons.cAccGenerate))
                    {
                        _ApplySyntaxNodeToCodeNode1(*node.Child(i), *pAssignment, lookups);
                    }
                }

                pNodeRet = pAssignment.release();
            }
            else
            {
                // Load operation - make PropertyValue
                std::auto_ptr<PropertyValueBase> pValue;

                // Does it have an indexer?
                if (_IsVOIndexed(bOpcode))
                {
                    pValue.reset(new ComplexPropertyValue);
                    // Then it should have a child
                    if (node.GetChildCount() >= 1)
                    {
                        auto_ptr<SingleStatement> pStatement(new SingleStatement);
                        pStatement->SetSyntaxNode(_CodeNodeToSyntaxNode(*node.Child(0), lookups));
                        static_cast<ComplexPropertyValue*>(pValue.get())->SetIndexer(pStatement);
                    }
                    else
                    {
                        ASSERT(FALSE); // phil, TODO 
                    }
                }
                else
                {
                    pValue.reset(new PropertyValue);
                }

                pValue->SetValue(_GetVariableNameFromCodePos(pos, lookups), ValueTypeToken);

                // If it has an incrementer or decrementer, wrap it in a unary operator first.
                bool fIncrement = _IsVOIncremented(bOpcode);
                bool fDecrement = _IsVODecremented(bOpcode);
                if (fIncrement || fDecrement)
                {
                    auto_ptr<UnaryOp> pUnary(new UnaryOp);
                    pUnary->SetName(fIncrement ? "++" : "--");
                    SingleStatement statement;
                    statement.SetSyntaxNode(pValue);
                    pUnary->SetStatement1(statement);
                    pNodeRet = pUnary.release();
                }
                else
                {
                    pNodeRet = pValue.release();
                }
            }
        }
        break;
    }

    // Up next: acTOSS

    if (pNodeRet == NULL)
    {
        auto_ptr<Comment> pComment(new Comment());
        pComment->SetName("ERROR");
        pNodeRet = pComment.release();
    }
    return pNodeRet;
}



std::string IDecompileLookups::LookupSelectorName(WORD wIndex)
{
    return _pLookups->LookupSelectorName(wIndex);
}
std::string IDecompileLookups::LookupKernelName(WORD wIndex)
{
    return _pLookups->LookupKernelName(wIndex);
}
std::string IDecompileLookups::LookupClassName(WORD wIndex)
{
    std::string ret = _pLookups->LookupClassName(wIndex);
    if (ret.empty())
    {
        ret = _pPrivateSpecies->LookupClassName(wIndex);
    }
    return ret;
}
bool IDecompileLookups::LookupSpeciesPropertyList(WORD wIndex, std::vector<WORD> &props)
{
    bool fRet = _pLookups->LookupSpeciesPropertyList(wIndex, props);
    if (!fRet)
    {
        fRet = _pPrivateSpecies->LookupSpeciesPropertyList(wIndex, props);
    }
    return fRet;
}
bool IDecompileLookups::LookupSpeciesPropertyListAndValues(WORD wIndex, std::vector<WORD> &props, std::vector<WORD> &values)
{
    bool fRet = _pLookups->LookupSpeciesPropertyListAndValues(wIndex, props, values);
    if (!fRet)
    {
        fRet = _pPrivateSpecies->LookupSpeciesPropertyListAndValues(wIndex, props, values);
    }
    return fRet;
}
std::string IDecompileLookups::ReverseLookupGlobalVariableName(WORD wIndex)
{
    static std::string s_defaults[] = {
        "gEgo",
        "gGame",
        "gRoom",
        "gSpeed",
        "gQuitGame",
        "gCast",
        "gRegions",
        "gLocales",
        "gTimers",
        "gSounds",  // gInv, SQ
        "gInv",     // gAddToPics...
        "gAddToPics",   // something passed to OwnedBy
        "gFeatures",
        "gSFeatures",
        "gRoomNumberExit",
        "gPreviousRoomNumber",
        "gRoomNumber",
        "gDebugOnExit",
        "gScore",
        "gMaxScore",
    };


    std::string result = _pOFLookups->ReverseLookupGlobalVariableName(wIndex);
    if (result.empty())
    {
        // Supply some defaults.  These may be different for different games.
        if (wIndex < ARRAYSIZE(s_defaults))
        {
            result = s_defaults[wIndex];
        }
    }
    return result;
}
std::string IDecompileLookups::ReverseLookupPublicExportName(WORD wScript, WORD wIndex)
{
    std::string ret = _pOFLookups->ReverseLookupPublicExportName(wScript, wIndex);
    if (ret.empty())
    {
        // This will be true if there are no .sco/.scop files.
        ret = _GetPublicProcedureName(wScript, wIndex);
    }
    return ret;
}
std::string IDecompileLookups::LookupPropertyName(WORD wPropertyIndex)
{
    if (_pPropertyNames)
    {
        return _pPropertyNames->LookupPropertyName(this, wPropertyIndex);
    }
    else
    {
        return "PROPERTY-ACCESS-IN-NON-METHOD";
    }
}
std::string IDecompileLookups::LookupScriptThing(WORD wName, ICompiledScriptSpecificLookups::ObjectType &type) const
{
    return _pScriptThings->LookupObjectName(wName, type);
}
std::string IDecompileLookups::LookupParameterName(WORD wIndex)
{
    if (wIndex)
    {
        const FunctionSignature &signature = *_pFunc->GetSignatures()[0];
        size_t iRealIndex = (wIndex - 1);
        ASSERT(iRealIndex < signature.GetParams().size()); // Since it was us who analyzed the code and added the right # of params
        return signature.GetParams()[iRealIndex]->GetName();
    }
    else
    {
        return "paramTotal"; // parameter 0 is the count of params
    }
}

std::string IDecompileLookups::LookupTextResource(WORD wIndex) const
{
    std::string ret;
    if (_pTextResource)
    {
        ret = _pTextResource->Lookup(wIndex);
    }
    return ret;
}

void IDecompileLookups::SetPosition(sci::SyntaxNode *pNode)
{
    pNode->SetPosition(_fakePosition);
    _fakePosition = LineCol(_fakePosition.Line() + 1, 0);
}

// TODOs:
//
// Try to identify places where constants may be used... e.g.
// (send gEgo.onControl(FALSE)) != ctlCREEN)
//
// Give names to things which have certain things called on them... e.g.
// param1.claimed  -->  pEvent.claimed
// Or just do obvious things for changeState and handleEvent....
//
//
// Apply popular defines to things, e.g.
// Load(rsVIEW, 123);
// or, if this is possible,
// gEgo.onControl(FALSE) == ctlGREEN
// ignoreControl(ctrlGREEN);
// ignoreActors(TRUE)
//
// Assume stuff for the Print thing.... so we can do selectors....
// And for methods like "eachElementDo"
//
//
// Know what the Print procedure is for all games... store it somewhere??? e.g. SQ3 is proc255_0.
// Detect if it has two parameters, the first which matches the script number... if so, then go load
// the text resource for this, and replace that in the thing.
//
// Have a visitor pattern that looks at binary ops that are & or |, and makes any child property values be hex

// phil - features TODO
//
// How to handle/detect break statements?
// How to handle && in a while loop?
//
//
// phil -bugs
// Internal strings are doubled???? 

//
// phil BUGS in SCRIPTS
// 92,91,90 has the pprev:     
/*
    bnt--> B
      le?
        push0
        send
          pushi
          push0
          lofsa
    bnt--> C
      B:not
        le?
          pprev
          ldi
    bnt--> D
      gt?
        push
          send
            pushi
            push0
            lag
        ldi



I think this was the pattern for
(if (<= 0 findScript.state() 3)
)
*/
// We might be able to fake it by copying the tree under pprev
//	look backwards for a branch instruction, and find what the accumulator
//	would have been
//
//
//
// Script 75 has an issue in rm075::doit, where the stuff at the end doesn't work.
// It is nested ternary operators
//
// Script 52 has an LEA
//
//
// We have problems with this in the main script... 
// EXPORTED procedure #12 ()
/*
(procedure proc_1278
  1278:81 19              lag  
  127a:30 0005            bnt code_1282 
  127d:39 5b            pushi 5b // $5b dispose
  127f:76               push0 
  1280:4a 04             send 4 


        code_1282
  1282:48                 ret 
)
*/
// and with procedure 15... which has OnControl(dup)

// BUG:
// Conquests of Camelot, script 005, treasurerActions::handleEvent, lots of degenerate if statements.
//                       script 005, pouch::handleEvent, same thing... some new kind of branching structure.