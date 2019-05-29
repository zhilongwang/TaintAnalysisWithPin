#include "taint_sink.h"
#include "rt_check.h"
#include "debug.h"
#ifdef __DEBUG_ANALYIZE
#define D(x) x
#else 
#define D(x)
#endif
AFUNPTR CheckRegOnly[] =
{
	NULL, AFUNPTR(CheckOneReg), AFUNPTR(CheckTwoReg)
};
AFUNPTR CheckMemAndReg[] =
{
	NULL, AFUNPTR(CheckOneRegOneMem),AFUNPTR(CheckTwoRegOneMem)
};
bool AddCmpSinkFunction(INS ins){
    UINT32 operands_count = INS_OperandCount(ins);
    UINT32 read_reg_count = 0;
    IARGLIST args = IARGLIST_Alloc();

    REG pin_base_reg = INS_MemoryBaseReg(ins);
    REG pin_ss_reg = REG_SEG_SS;
    REG pin_gs_reg = REG_SEG_GS;
    for(UINT32 i=0; i<operands_count; i++){
        if(INS_OperandRead(ins, i) && INS_OperandIsReg(ins, i)){
            REG reg = INS_OperandReg(ins, i);
            if( (reg!= pin_base_reg) && (reg < pin_ss_reg || reg > pin_gs_reg)){
                read_reg_count++;
                D(cout << "--read reg:" << i << ":" << REG_StringShort(reg)<< endl;)
                IARGLIST_AddArguments(args, IARG_UINT32, reg, IARG_END);
            }
        }
    }
    if(INS_IsMemoryRead(ins) && read_reg_count > 0 && read_reg_count < 3){
        D(cout << "[CheckMemAndReg]:" << endl;)
        INS_InsertCall(
            ins, IPOINT_BEFORE, CheckMemAndReg[read_reg_count],
            IARG_INST_PTR, 
            IARG_PTR, new string(INS_Disassemble(ins)),
            IARG_MEMORYREAD_EA,
		    IARG_MEMORYREAD_SIZE,
            IARG_IARGLIST, args,
            IARG_END);
        IARGLIST_Free(args);
        return true; 
    }else if(read_reg_count > 0 && read_reg_count < 3){
        D(cout << "[CheckRegOnly]:" << endl;)
        INS_InsertCall(
            ins, IPOINT_BEFORE, CheckRegOnly[read_reg_count],
            IARG_INST_PTR, 
            IARG_PTR, new string(INS_Disassemble(ins)),
            IARG_IARGLIST, args,
            IARG_END);
        IARGLIST_Free(args);
        return true; 
    }
    IARGLIST_Free(args);
    return false;
}
bool AddCallSinkFunction(INS ins){
	if ( INS_OperandReg(ins, 0) == REG_EAX){
		D(cout << "[CheckCall]:" << endl;)
		INS_InsertCall(
			ins, IPOINT_BEFORE, (AFUNPTR)CheckIndirectCall,
			IARG_INST_PTR, 
            IARG_PTR, new string(INS_Disassemble(ins)),
			IARG_UINT32, INS_OperandReg(ins, 0),
			IARG_END);
		return true;
  	}
	return false;
}
bool TaintSink(INS ins){
    bool did_add_check_fun = false;
    switch(INS_Opcode(ins)){
        case XED_ICLASS_CMP :
        case XED_ICLASS_TEST :
        case XED_ICLASS_CMPSB :
        case XED_ICLASS_CMPSW :
        case XED_ICLASS_CMPSD :
        case XED_ICLASS_CMPSQ :
            did_add_check_fun = AddCmpSinkFunction(ins);
            break;
		case XED_ICLASS_CALL_FAR:
  		case XED_ICLASS_CALL_NEAR:
		  	did_add_check_fun = AddCallSinkFunction(ins);
            break;
        default:
        ;
    }
    return did_add_check_fun;
}