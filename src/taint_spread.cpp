#include "taint_spread.h"
#include <asm/unistd.h>
#include "pin.H"
#include "debug.h"
#include "instruction.h"


#define __DEBUG
#ifdef __DEBUG
#define DEBUG(format,...) printf(format,##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif
extern RegionInfo* region_info;
extern ShadowMem* shadow_mem;
extern ShadowReg* shadow_reg;
VOID MemTo1Reg(VOID * ip, string assemble, VOID * addr, UINT32 size, REG w_reg_1){
	cout << hex<< "MemTo1Reg:\t"<< assemble  << endl;
	if(shadow_mem->checkTaint((UINT32)addr, size)){
		shadow_reg->taintREG(w_reg_1);
	}else{
		shadow_reg->removeREG(w_reg_1);
	}
	return;
}
VOID MemTo2Reg(VOID * ip, string assemble, VOID * addr, UINT32 size, REG w_reg_1, REG w_reg_2){
	cout << hex<< "MemTo2Reg:\t"<< assemble  << endl;
	if(shadow_mem->checkTaint((UINT32)addr, size)){
		shadow_reg->taintREG(w_reg_1);
		shadow_reg->taintREG(w_reg_2);
	}else{
		shadow_reg->removeREG(w_reg_1);
		shadow_reg->removeREG(w_reg_2);
	}
	return;
}
VOID MemTo3Reg(VOID * ip, string assemble, VOID * addr, UINT32 size, REG w_reg_1, REG w_reg_2, REG w_reg_3){
	cout << hex<< "MemTo3Reg:\t"<< assemble  << endl;
	if(shadow_mem->checkTaint((UINT32)addr, size)){
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
	cout << hex<< "ZeroReg2Mem:\t"<< assemble  << endl;
	shadow_mem->removeTaint((UINT32)addr, size);
}

VOID OneReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG r_reg_1){
	cout << hex<< "OneReg2Mem:\t"<< assemble  << endl;
	if(shadow_reg->checkREG(r_reg_1)){
		shadow_mem->addTaint((UINT32)addr, size);
	}else{
		shadow_mem->removeTaint((UINT32)addr, size);
	}
	return;
}

VOID TwoReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG r_reg_1, REG r_reg_2){
	cout << hex<< "TwoReg2Mem:\t"<< assemble  << endl;
	if(shadow_reg->checkREG(r_reg_1) || shadow_reg->checkREG(r_reg_2) ){
		shadow_mem->addTaint((UINT32)addr, size);
	}else{
		shadow_mem->removeTaint((UINT32)addr, size);
	}
	return;
}

VOID ThreeReg2Mem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG r_reg_1, REG r_reg_2, REG r_reg_3){
	cout << hex<< "TwoReg2Mem:\t"<< assemble  << endl;
	if(shadow_reg->checkREG(r_reg_1) || shadow_reg->checkREG(r_reg_2)|| shadow_reg->checkREG(r_reg_3)){
		shadow_mem->addTaint((UINT32)addr, size);
	}else{
		shadow_mem->removeTaint((UINT32)addr, size);
	}
	return;
}

VOID OneRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1){
	cout << hex<< "Reg2Reg(1):\t"<< assemble  << endl;
	if(read_reg_num == 0 ){
		shadow_reg->removeREG(reg_1); 
	}
	return;
}

VOID TwoRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1, REG reg_2){
	cout << hex<< "Reg2Reg(2):\t"<< assemble  << endl;
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
			CASE_TAINT_REG_(2)
			CASE_TAINT_REG_(1)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}else{
		switch(2 - read_reg_num){
			CASE_REMOVE_REG_(2)
			CASE_REMOVE_REG_(1)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}
	return;
}

VOID ThreeRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1, REG reg_2, REG reg_3){
	cout << hex<< "Reg2Reg(3):\t"<< assemble  << endl;
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
			CASE_TAINT_REG_(3)
			CASE_TAINT_REG_(2)
			CASE_TAINT_REG_(1)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}else{
		switch(3 - read_reg_num){
			CASE_REMOVE_REG_(3)
			CASE_REMOVE_REG_(2)
			CASE_REMOVE_REG_(1)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}
	return;
}

VOID FourRegs(VOID * ip, string assemble, UINT32 read_reg_num, REG reg_1, REG reg_2, REG reg_3, REG reg_4){
	cout << hex<< "Reg2Reg(1):\t"<< assemble  << endl;
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
			CASE_TAINT_REG_(4)
			CASE_TAINT_REG_(3)
			CASE_TAINT_REG_(2)
			CASE_TAINT_REG_(1)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}else{
		switch(4 - read_reg_num){
			CASE_REMOVE_REG_(4)
			CASE_REMOVE_REG_(3)
			CASE_REMOVE_REG_(2)
			CASE_REMOVE_REG_(1)
			case 0:
				break;
			default:
				cout << "error" << endl;
		}
	}
	return;
}

/*

VOID ReadMEM(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, UINT32 OperandCount,REG reg_r, UINT32 memOp, UINT32 sp)
{
	list<UINT32>::iterator i;
	UINT32 addr = memOp;
	// if(insAddr>=0x804848a&&insAddr<=0x81484ee){
	//   std::cout << std::hex<< "************" << std::endl;
	//   std::cout << std::hex << "insAddr:" << insAddr<< std::endl;
	//   std::cout << std::hex << "insDis:" << insDis<< std::endl;
	//   std::cout << std::hex << "OperandCount:" << OperandCount<< std::endl;
	//   std::cout << std::hex << "memOp:" << memOp << std::endl;
	//   std::cout << std::hex<< "sp:" << sp << std::endl;
	//   std::cout << std::hex<< "reg_r:" <<reg_r << std::endl;
	//   std::cout << std::hex<< "************" << std::endl;
	//   }

	OutToX(insAddr,&insDis);
	if (OperandCount != 2)
		return;
	
	if(shadow_mem.checkTaint(addr)){
		
			OutToX(LOAD__T,insAddr, &insDis,addr,reg_r);
			shadow_reg.taintREG(reg_r);
			return ;
	}
	//if mem != tained and reg == taint => free the reg 
	if (shadow_reg.checkREG(reg_r)){
		OutToX(LOAD__F,insAddr, &insDis,addr,reg_r);
		shadow_reg.removeREG(reg_r);
	}
}
VOID WriteMEM(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr, string insDis, UINT32 OperandCount,REG reg_r, REG reg_0,UINT32 memOp, UINT32 sp)
{
	list<UINT32>::iterator i;
	UINT32 addr = memOp;
	UINT32 length=0;
	OutToX(insAddr,&insDis);

	if (OperandCount != 2)
		return;
	if(!REG_valid(reg_r)){
		if(REG_valid(reg_0)){
			reg_r=reg_0;
		}else{
			//example:mov dword ptr [eax*4+0x804a080], 0xa
			if(insDis.find("dword ptr",0)!=string::npos){
				length=4;
			}else if(insDis.find("word ptr",0)!=string::npos){
				length=2;
			}else{
				length=1;
			}
		}  
	} 
	if(shadow_mem.checkTaint(addr)){
		//std::cout << std::hex << reg_r<< std::endl;
		if (!REG_valid(reg_r) || ! shadow_reg.checkREG(reg_r))
		{
			if(REG_is_Lower8(reg_r)||REG_is_Upper8(reg_r)){
				length=1;
			}else if(REG_is_Half16(reg_r)){
				length=2;
			}else if(REG_is_Half32(reg_r)){
				length=4;
			}
			OutToX(STORE_F,insAddr, &insDis,addr,length);
			shadow_mem.removeTaint(addr,length);
			return;
		}
	}

	if (REG_valid(reg_r) &&  shadow_reg.checkREG(reg_r)){
		if(REG_is_Lower8(reg_r)||REG_is_Upper8(reg_r)){
			length=1;
		}else if(REG_is_Half16(reg_r)){
			length=2;
		}else if(REG_is_Half32(reg_r)){
			length=4;
		}
		OutToX(STORE_T,insAddr, &insDis,addr,length);
		shadow_mem.addTaint(addr,length);
	}


}
VOID SpreadREG(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr, string insDis, UINT32 OperandCount,REG reg_r, REG reg_w)
{
	OutToX(insAddr,&insDis);
	if ( shadow_reg.checkREG(reg_w) && (!REG_valid(reg_r) || ! shadow_reg.checkREG(reg_r))){
		OutToX(SPREE_F,insAddr, &insDis,reg_w);
		shadow_reg.removeREG(reg_w);
	}
	else if (! shadow_reg.checkREG(reg_w) &&  shadow_reg.checkREG(reg_r)){
		OutToX(SPREE_T,insAddr, &insDis,reg_w);
		shadow_reg.taintREG(reg_w);
	}
}
VOID mulOper(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, UINT32 OperandCount,REG reg_r, REG reg_w)
{
	if (REG_valid(reg_w)||REG_is_Half32(reg_r)){
		if ((!REG_valid(reg_r)||((! shadow_reg.checkREG(reg_r))&&(! shadow_reg.checkREG(REG_EAX)))))
		{
			if(( shadow_reg.checkREG(reg_w)|| shadow_reg.checkREG(REG_EDX))){

				OutToX(SPREE_F,insAddr, &insDis,reg_w);
				//std::cout << "\t\t\toutput: "<< REG_StringShort(reg_w) << " | input: " << (REG_valid(reg_r) ? REG_StringShort(reg_r) : "constant") << std::endl;
				shadow_reg.removeREG(reg_w);
				shadow_reg.removeREG(REG_EDX);
			}
		}else if(( shadow_reg.checkREG(reg_r)|| shadow_reg.checkREG(REG_EAX)))
		{
			OutToX(SPREE_T,insAddr, &insDis,reg_w);
			//std::cout << "\t\t\toutput: " << REG_StringShort(reg_w) << " | input: "<< REG_StringShort(reg_r) << std::endl;
			shadow_reg.taintREG(reg_w);
			shadow_reg.taintREG(REG_EDX);
		}
	}
}
VOID CmpMem(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, REG reg, UINT32 memOp, UINT32 sp)
{
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n***************\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	if(shadow_reg.checkREG(reg)){
		DEBUG("TYPE1:taint_branch:%x\n",insAddr);	
		InsInfo binst={insAddr, insDis};
		//taintCmp.push_back(binst);
		return;
	}
	if(shadow_mem.checkTaint(memOp)){
		DEBUG("TYPE2:taint_branch:%x\n",insAddr);	
		InsInfo binst={insAddr, insDis};
		//taintCmp.push_back(binst);
		return;
	}
}
VOID CmpReg(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,  string insDis, REG reg1, REG reg2, UINT32 sp)
{
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n***************\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	if(shadow_reg.checkREG(reg1)||shadow_reg.checkREG(reg2)){
		
		DEBUG("TYPE3:taint_branch:%x\n",insAddr);	
		InsInfo binst={insAddr, insDis};
		//taintCmp.push_back(binst);
		return;
	}
}

VOID CmpImmi(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr, string insDis, REG reg, UINT32 sp)
{
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n***************\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	if(shadow_reg.checkREG(reg)){
		
		DEBUG("TYPE4:taint_branch:%x\n",insAddr);	
		InsInfo binst={insAddr, insDis};
		//taintCmp.push_back(binst);
		return;
	}
}
VOID CmpImmi2(ShadowMem &shadow_mem,ShadowReg &shadow_reg, UINT32 insAddr,string insDis, UINT32 memOp, UINT32 sp)
{
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n***************\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("memread addr:%x\n",memOp);
	if(shadow_mem.checkTaint(memOp)){
		DEBUG("TYPE5:taint_branch:%x\n",insAddr);	
		//InsInfo binst={insAddr, insDis};
		//taintCmp.push_back(binst);
		return;
	}
}*/