#ifndef _TAINT_SPREAD_H_
#define _TAINT_SPREAD_H_
#include "pin.H"
#include "shadowmem.h"
#include "register.h"
#include "instruction.h"
#include<string>
#include<iostream>
#include<list>
using std::cout;
using std::string;
using std::hex;
#define MAX_OP 5
#define CASE_CHECK_REG_(N)                  \
		case N:                             \
			if(shadow_reg->checkREG(reg_##N)){\
				source_is_tainted = true;   \
			}   

#define CASE_TAINT_REG_(N)                      \
		case N:                                 \
				shadow_reg->taintREG(reg_##N);  

#define CASE_REMOVE_REG_(N)                     \
        case N:                                 \
				shadow_reg->removeREG(reg_##N); 

typedef struct Mem_Data_Flow{
    enum AccessType accesstype;
    UINT32 addr;
    UINT32 length;
}Mem_Data_Flow;
typedef struct Reg_Data_Flow{
    enum AccessType accesstype;
    REG reg;
}Reg_Data_Flow;


typedef struct dataflow{
    Reg_Data_Flow reg_data_flow[MAX_OP];
    Mem_Data_Flow mem_data_flow[MAX_OP];
}dataflow;

VOID MemTo1Reg(VOID * ip, string assemble, VOID * addr, UINT32 size, REG w_reg_1);
VOID MemTo2Reg(VOID * ip, string assemble, VOID * addr, UINT32 size, REG w_reg_1, REG w_reg_2);
VOID MemTo3Reg(VOID * ip, string assemble, VOID * addr, UINT32 size, REG w_reg_1, REG w_reg_2, REG w_reg_3);

VOID ZeroReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size);
VOID OneReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG r_reg_1);
VOID TwoReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG r_reg_1, REG r_reg_2);
VOID ThreeReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG r_reg_1, REG r_reg_2, REG r_reg_3);

VOID OneRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1);
VOID TwoRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1, REG reg_2);
VOID ThreeRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1, REG reg_2, REG reg_3);
VOID FourRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1, REG reg_2, REG reg_3, REG reg_4);
// VOID ReadMEM(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, UINT32 OperandCount,REG reg_r, UINT32 memOp, UINT32 sp);
// VOID WriteMEM(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, UINT32 OperandCount,REG reg_r, REG reg_0,UINT32 memOp, UINT32 sp);
// VOID SpreadREG(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, UINT32 OperandCount,REG reg_r, REG reg_w);
// VOID mulOper(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, UINT32 OperandCount,REG reg_r, REG reg_w);
// VOID CmpMem(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, REG reg, UINT32 memOp, UINT32 sp);
// VOID CmpReg(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, REG reg1, REG reg2, UINT32 sp);
// VOID CmpImmi(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, REG reg, UINT32 sp);
// VOID CmpImmi2(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, UINT32 memOp, UINT32 sp);

#endif