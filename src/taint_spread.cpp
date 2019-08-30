#include "taint_spread.h"
#include <asm/unistd.h>
#include "pin.H"
#include "debug.h"
#include "instruction.h"
using std::endl;
#ifdef __DEGUG_TRACE
#define D(x) x
#else 
#define D(x)
#endif


extern RegionInfo* region_info;
extern ShadowMem* shadow_mem;
extern ShadowReg* shadow_reg;
VOID MemTo1Reg(VOID * ip, string assemble, VOID * addr, UINT32 size, REG w_reg_1){
	D(cout << hex << "MemTo1Reg: "<<ip << "\t"<< assemble  << endl;)
	D(cout << hex << "\t\t\t--" << "read:" << addr << endl;)
	if(shadow_mem->CheckTaint((UINT32)addr, size)){
		shadow_reg->taintREG(w_reg_1);
	}else{
		shadow_reg->removeREG(w_reg_1);
	}
	return;
}
VOID MemTo2Reg(VOID * ip, string assemble, VOID * addr, UINT32 size, REG w_reg_1, REG w_reg_2){
	D(cout << hex<< "MemTo2Reg: "<<ip << "\t"<< assemble  << endl;)
	D(cout << hex << "\t\t\t--" << "read:" << addr << endl;)
	if(shadow_mem->CheckTaint((UINT32)addr, size)){
		shadow_reg->taintREG(w_reg_1);
		shadow_reg->taintREG(w_reg_2);
	}else{
		shadow_reg->removeREG(w_reg_1);
		shadow_reg->removeREG(w_reg_2);
	}
	return;
}
VOID MemTo3Reg(VOID * ip, string assemble, VOID * addr, UINT32 size, REG w_reg_1, REG w_reg_2, REG w_reg_3){
	D(cout << hex<< "MemTo3Reg: "<<ip << "\t"<< assemble  << endl;)
	D(cout << hex << "\t\t\t--" << "read:" << addr << endl;)
	if(shadow_mem->CheckTaint((UINT32)addr, size)){
		shadow_reg->taintREG(w_reg_1);
		shadow_reg->taintREG(w_reg_2);
		shadow_reg->taintREG(w_reg_3);
	}else{
		shadow_reg->removeREG(w_reg_1);
		shadow_reg->removeREG(w_reg_2);
		shadow_reg->removeREG(w_reg_3);
	}
	return;
}
VOID ZeroReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size){
	D(cout << hex<< "ZeroReg2Mem: "<<ip << "\t"<< assemble  << endl;)
	D(cout << hex << "\t\t\t--" << "write:" << addr << endl;)
	shadow_mem->RemoveTaint((UINT32)addr, size);
	return;
}

VOID OneReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG r_reg_1){
	D(cout << hex<< "OneReg2Mem: "<<ip << "\t"<< assemble  << endl;)
	D(cout << hex << "\t\t\t--" << "write:" << addr << endl;)
	if(shadow_reg->checkREG(r_reg_1)){
		shadow_mem->AddTaint((UINT32)addr, size);
	}else{
		shadow_mem->RemoveTaint((UINT32)addr, size);
	}
	return;
}

VOID TwoReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG r_reg_1, REG r_reg_2){
	D(cout << hex<< "TwoReg2Mem: "<<ip << "\t"<< assemble  << endl;)
	D(cout << hex << "\t\t\t--" << "write:" << addr << endl;)
	if(shadow_reg->checkREG(r_reg_1) || shadow_reg->checkREG(r_reg_2) ){
		shadow_mem->AddTaint((UINT32)addr, size);
	}else{
		shadow_mem->RemoveTaint((UINT32)addr, size);
	}
	return;
}

VOID ThreeReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG r_reg_1, REG r_reg_2, REG r_reg_3){
	D(cout << hex<< "TwoReg2Mem: "<<ip << "\t"<< assemble  << endl;)
	D(cout << hex << "\t\t\t--" << "write:" << addr << endl;)
	if(shadow_reg->checkREG(r_reg_1) || shadow_reg->checkREG(r_reg_2)|| shadow_reg->checkREG(r_reg_3)){
		shadow_mem->AddTaint((UINT32)addr, size);
	}else{
		shadow_mem->RemoveTaint((UINT32)addr, size);
	}
	return;
}

VOID OneRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1){
	D(cout << hex<< "Reg2Reg(1): "<<ip << "\t"<< assemble  << endl;)
	if(read_reg_num == 0 ){
		shadow_reg->removeREG(reg_1); 
	}
	return;
}

VOID TwoRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1, REG reg_2){
	D(cout << hex<< "Reg2Reg(2): "<<ip << "\t"<< assemble  << endl;)
	D(cout << hex << "\t\t\t--" << "read_reg_num:" << read_reg_num << endl;)
	bool source_is_tainted = false;
	switch(read_reg_num){
		CASE_CHECK_REG_(2)
		CASE_CHECK_REG_(1)
		case 0:
			break;
		default:
			cout << "error" << endl;
	}
	if(source_is_tainted){
		switch(2 - read_reg_num){
			CASE_TAINT_REG_(2,1)
			CASE_TAINT_REG_(1,2)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}else{
		switch(2 - read_reg_num){
			CASE_REMOVE_REG_(2,1)
			CASE_REMOVE_REG_(1,2)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}
	return;
}

VOID ThreeRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1, REG reg_2, REG reg_3){
	D(cout << hex<< "Reg2Reg(3): "<<ip << "\t"<< assemble  << endl;)
	D(cout << hex << "\t\t\t--" << "read_reg_num:" << read_reg_num << endl;)
	bool source_is_tainted = false;
	switch(read_reg_num){
		CASE_CHECK_REG_(3)
		CASE_CHECK_REG_(2)
		CASE_CHECK_REG_(1)
		case 0:
			break;
		default:
			cout << "error" << endl;
	}
	if(source_is_tainted){
		switch(3 - read_reg_num){
			CASE_TAINT_REG_(3,1)
			CASE_TAINT_REG_(2,2)
			CASE_TAINT_REG_(1,3)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}else{
		switch(3 - read_reg_num){
			CASE_REMOVE_REG_(3,1)
			CASE_REMOVE_REG_(2,2)
			CASE_REMOVE_REG_(1,3)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}
	return;
}

VOID FourRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1, REG reg_2, REG reg_3, REG reg_4){
	D(cout << hex<< "Reg2Reg(1): "<<ip << "\t"<< assemble  << endl;)
	D(cout << hex << "\t\t\t--" << "read_reg_num:" << read_reg_num << endl;)
	bool source_is_tainted = false;
	switch(read_reg_num){
		CASE_CHECK_REG_(4)
		CASE_CHECK_REG_(3)
		CASE_CHECK_REG_(2)
		CASE_CHECK_REG_(1)
		case 0:
			break;
		default:
			cout << "error" << endl;
	}
	if(source_is_tainted){
		switch(4 - read_reg_num){
			CASE_TAINT_REG_(4,1)
			CASE_TAINT_REG_(3,2)
			CASE_TAINT_REG_(2,3)
			CASE_TAINT_REG_(1,4)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}else{
		switch(4 - read_reg_num){
			CASE_REMOVE_REG_(4,1)
			CASE_REMOVE_REG_(3,2)
			CASE_REMOVE_REG_(2,3)
			CASE_REMOVE_REG_(1,4)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}
	return;
}
VOID Mem2Mem(VOID * ip, string assemble, VOID * read_addr, UINT32 read_size, VOID * write_addr, UINT32 write_size){
	D(cout << hex<< "Mem2Mem: "<< ip << "\t"<< assemble  << endl;)
	if(shadow_mem->CheckTaint((UINT32)read_addr, read_size)){
		shadow_mem->AddTaint((UINT32)write_addr, write_size);
	}else{
		shadow_mem->RemoveTaint((UINT32)write_addr, write_size);
	}
	return;
}

