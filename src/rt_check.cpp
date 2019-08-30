#include "rt_check.h"
#include "shadowmem.h"
#include "register.h"
#include "debug.h"
#include "instruction.h"
using std::endl;
#ifdef __DEGUG_SINK
#define D(x) x
#else 
#define D(x)
#endif


extern ShadowMem* shadow_mem;
extern ShadowReg* shadow_reg;
extern SinkInfo* sink_info;

string SinkInfo::sink_type_name[]={
            "CONTROL_DEPENDENCY_SINK",
            "INDIRECT_CALL_SINK",
            "UNDEFINED_SINK"};
VOID CheckIndirectCall(VOID * ip, string assemble, REG reg_1){
    if (shadow_reg->checkREG(reg_1)){
        D(cout << "Hicking Indirect Call" << endl;)
        sink_info->AddSinkPoint(INDIRECT_CALL_SINK,(UINT32)ip);
    }
    return;
}

VOID CheckOneReg(VOID * ip, string assemble, REG reg_1){
	D(cout << hex<< "CheckOneReg: "<<ip << "\t"<< assemble  << endl;)
    if(shadow_reg->checkREG(reg_1)){
        D(cout << "branch instruction" << endl;)
        sink_info->AddSinkPoint(CONTROL_DEPENDENCY_SINK,(UINT32)ip);
    }
    return;
}
VOID CheckTwoReg(VOID * ip, string assemble, REG reg_1, REG reg_2){
	D(cout << hex<< "CheckTwoReg: "<<ip << "\t"<< assemble  << endl;)
    if(shadow_reg->checkREG(reg_1) || shadow_reg->checkREG(reg_2) ){
        D(cout << "branch instruction" << endl;)
        sink_info->AddSinkPoint(CONTROL_DEPENDENCY_SINK,(UINT32)ip);
    }
	return;
}

VOID CheckZeroRegOneMem(VOID * ip, string assemble, VOID * addr, UINT32 size){
	D(cout << hex<< "CheckZeroRegOneMem: "<<ip << "\t"<< assemble  << endl;)
    D(cout << hex << "\t\t\t--" << "read:" << addr << endl;)
    if(shadow_mem->CheckTaint((UINT32)addr, size)){
        D(cout << "branch instruction" << endl;)
        sink_info->AddSinkPoint(CONTROL_DEPENDENCY_SINK,(UINT32)ip);
    }
	return;
}
VOID CheckOneRegOneMem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG reg_1){
	D(cout << hex<< "CheckOneRegOneMem: "<<ip << "\t"<< assemble  << endl;)
    D(cout << hex << "\t\t\t--" << "read:" << addr << endl;)
    if(shadow_mem->CheckTaint((UINT32)addr, size) || shadow_reg->checkREG(reg_1) ){
        D(cout << "branch instruction" << endl;)
        sink_info->AddSinkPoint(CONTROL_DEPENDENCY_SINK,(UINT32)ip);
    }
	return;
}
VOID CheckTwoRegOneMem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG reg_1, REG reg_2){
	D(cout << hex<< "CheckOneRegOneMem: "<<ip << "\t"<< assemble  << endl;)
    if(shadow_mem->CheckTaint((UINT32)addr, size) || shadow_reg->checkREG(reg_1) ||shadow_reg->checkREG(reg_2) ){
        D(cout << "branch instruction" << endl;)
        sink_info->AddSinkPoint(CONTROL_DEPENDENCY_SINK,(UINT32)ip);
    }
	return;
}
