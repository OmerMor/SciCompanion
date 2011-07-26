#pragma once

#define TOTAL_KERNELS 				    114 // SCI0 only

#define TOTAL_OPCODES 				    128

#define TOTAL_SAID_ARGS 10

struct SaidToken
{
    char Character;
    BYTE Code;
};

extern SaidToken SaidArgs[TOTAL_SAID_ARGS];

struct KernelInfo
{
	char *Ret,*Name,*Params;
    const char *GetName() { return Name; }
};

extern KernelInfo KrnlInfo[TOTAL_KERNELS];

extern BYTE OpArgs[TOTAL_OPCODES*2];

extern char *OpcodeNames[];

enum OperandType
{
    otEMPTY =     0,
    otINT =       1,
    otVAR =       2,
    otPVAR =      3,
    otCLASS =     4,
    otPROP =      5,
    otSTRING =    6,
    otSAID =      7,
    otKERNEL =    8,
    otLABEL =     9,
    otPUBPROC =   10,
    otINT8 =      11,
    otINT16 =     12,
    otOFFS =      13,
};
extern OperandType OpArgTypes[TOTAL_OPCODES][3];

// Opcodes

#define acBNOT							0
#define acADD							1
#define acSUB							2
#define acMUL							3
#define acDIV							4
#define acMOD							5
#define acSHR							6
#define acSHL							7
#define acXOR							8
#define acAND							9
#define acOR							10
#define acNEG							11
#define acNOT							12
#define acEQ							13
#define acNE							14
#define acGT							15
#define acGE							16
#define acLT							17
#define acLE							18
#define acUGT							19
#define acUGE							20
#define acULT							21
#define acULE							22
#define acBT							23      // branch on true
#define acBNT							24      // branch on not true
#define acJMP							25      // jump
#define acLDI							26      // load immediate data into accumulator
#define acPUSH							27      // push accumulator onto stack (1 byte)
#define acPUSHI						28          // push immediate onto stack
#define acTOSS							29      // get rid of top of stack
#define acDUP							30      // dupe stack top of stack
#define acLINK							31      // add n frames to stack
#define acCALL							32      // call local proc
#define acCALLK						33          // call kernel
#define acCALLB						34          // call public proc in main script
#define acCALLE						35          // call public proc in external script
#define acRET							36      // return (value goes in acc)
#define acSEND							37      // send
#define acCLASS						40          // load address of class # to accumulator
#define acSELF							42      // send to self
#define acSUPER						43          // send to super
#define acREST							44
#define acLEA							45  // load  address of a variable into the acc
#define acSELFID						46  // puts address of self into acc
#define acINDETERMINATE                       47  // NOT A VALID OPCODE
#define acPPREV						48
#define acPTOA							49      // property index to acc
#define acATOP							50      // acc to property index
#define acPTOS							51      // property index to stack
#define acSTOP							52      // Stack to property index
#define acIPTOA						53          // Inc prop to acc
#define acDPTOA						54          // Dec prop to acc
#define acIPTOS						55          // Inc prop to stack
#define acDPTOS						56          // Dec prop to stack
#define acLOFSA						57      // Load offset (from pc) onto acc
#define acLOFSS						58      // Load offset (from pc) onto stack
#define acPUSH0						59      // push 0 onto stack
#define acPUSH1						60      // push 1 onto stack
#define acPUSH2						61      // push 2 onto stack
#define acPUSHSELF					62      // push self onto stack
#define acLAG							64  // load global to acc
#define acLAL							65
#define acLAT							66
#define acLAP							67
#define acLSG							68  // load global to stack
#define acLSL							69
#define acLST							70
#define acLSP							71
#define acLAGI							72  // index global and load to acc
#define acLALI							73
#define acLATI							74
#define acLAPI							75
#define acLSGI							76  // index global and load to stack
#define acLSLI							77
#define acLSTI							78
#define acLSPI							79
#define acSAG							80  // store acc in global
#define acSAL							81
#define acSAT							82
#define acSAP							83
#define acSSG							84  // store stack in global
#define acSSL							85
#define acSST							86
#define acSSP							87
#define acSAGI							88  // store acc in global and index?
#define acSALI							89
#define acSATI							90
#define acSAPI							91
#define acSSGI							92  // store stack in global and index?
#define acSSLI							93
#define acSSTI							94
#define acSSPI							95
#define acpAG							96
#define acpAL							97
#define acpAT							98
#define acpAP							99
#define acpSG							100
#define acpSL							101
#define acpST							102
#define acpSP							103
#define acpAGI							104
#define acpALI							105
#define acpATI							106
#define acpAPI							107
#define acpSGI							108
#define acpSLI							109
#define acpSTI							110
#define acpSPI							111
#define acnAG							112
#define acnAL							113
#define acnAT							114
#define acnAP							115
#define acnSG							116
#define acnSL							117
#define acnST							118
#define acnSP							119
#define acnAGI							120
#define acnALI							121
#define acnATI							122
#define acnAPI							123
#define acnSGI							124
#define acnSLI							125
#define acnSTI							126
#define acnSPI							127         




// Variable operands
// bit "-1" is operand size
// bit 0,1 -> type of variable
#define VO_GLOBAL           0x00
#define VO_LOCAL            0x01
#define VO_TEMP             0x02
#define VO_PARAM            0x03
// bit 2
#define VO_STACK            0x04
#define VO_ACC              0x00
// bit 3
#define VO_ACC_AS_INDEX_MOD 0x08
// bit 4,5
#define VO_LOAD             0x00 // -> to acc or stack
#define VO_STORE            0x10 // -> in variable
#define VO_INC_AND_LOAD     0x20
#define VO_DEC_AND_LOAD     0x30

// The lea instruction (load effective address into acc)
// bits 0-1  (actually bits 1-2)
#define LEA_GLOBAL           0x00 // phil -> make them static const BYTE or something.
#define LEA_LOCAL            0x01
#define LEA_TEMP             0x02
#define LEA_PARAM            0x03
#define LEA_VARIABLEMASK     0x03
// bit 3     (actually bit 4)
#define LEA_ACC_AS_INDEX_MOD 0x08

