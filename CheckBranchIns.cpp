#include "pin.H"
#include <asm/unistd.h>
#include <fstream>
#include <iostream>
#include <list>
#include "include/taintlib.h"
#include "include/control.h"
#include "include/contex.h"
#define __DEBUG
#ifdef __DEBUG
#define DEBUG(format,...) printf(format,##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif
INT32 Usage()
{
	cerr << "Ex 4" << endl;
	return -1;
}
VOID ReadMem(UINT32 insAddr, std::string insDis, UINT32 OperandCount,REG reg_r, UINT32 memOp, UINT32 sp)
{
	list<UINT32>::iterator i;
	UINT32 addr = memOp;
	/*if(insAddr>=0x804848a&&insAddr<=0x81484ee){
	  std::cout << std::hex<< "************" << std::endl;
	  std::cout << std::hex << "insAddr:" << insAddr<< std::endl;
	  std::cout << std::hex << "insDis:" << insDis<< std::endl;
	  std::cout << std::hex << "OperandCount:" << OperandCount<< std::endl;
	  std::cout << std::hex << "memOp:" << memOp << std::endl;
	  std::cout << std::hex<< "sp:" << sp << std::endl;
	  std::cout << std::hex<< "reg_r:" <<reg_r << std::endl;
	  std::cout << std::hex<< "************" << std::endl;
	  }*/

	OutToX(insAddr,&insDis);
	if (OperandCount != 2)
		return;
	for(i = addressTainted.begin(); i != addressTainted.end(); i++){
		if (addr == *i){
			/****out to file
			 *****/
			OutToX(LOAD__T,insAddr, &insDis,addr,reg_r);
			taintReg(reg_r);
			return ;
		}
	}
	/* if mem != tained and reg == taint => free the reg */
	if (checkAlreadyRegTainted(reg_r)){

		OutToX(LOAD__F,insAddr, &insDis,addr,reg_r);
		removeRegTainted(reg_r);
	}
}
VOID WriteMem(UINT32 insAddr, std::string insDis, UINT32 OperandCount,REG reg_r, REG reg_0,UINT32 memOp, UINT32 sp)
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
			/*example:mov dword ptr [eax*4+0x804a080], 0xa*/
			if(insDis.find("dword ptr",0)!=string::npos){
				length=4;
			}else if(insDis.find("word ptr",0)!=string::npos){
				length=2;
			}else{
				length=1;
			}
		}  
	} 
	for(i = addressTainted.begin(); i != addressTainted.end(); i++){
		if (addr == *i){
			//std::cout << std::hex << reg_r<< std::endl;
			if (!REG_valid(reg_r) || !checkAlreadyRegTainted(reg_r))
			{
				if(REG_is_Lower8(reg_r)||REG_is_Upper8(reg_r)){
					length=1;
				}else if(REG_is_Half16(reg_r)){
					length=2;
				}else if(REG_is_Half32(reg_r)){
					length=4;
				}
				OutToX(STORE_F,insAddr, &insDis,addr,length);
				removeMemTainted(addr,length);
				return;
			}
		}
	}

	if (REG_valid(reg_r) && checkAlreadyRegTainted(reg_r)){
		if(REG_is_Lower8(reg_r)||REG_is_Upper8(reg_r)){
			length=1;
		}else if(REG_is_Half16(reg_r)){
			length=2;
		}else if(REG_is_Half32(reg_r)){
			length=4;
		}
		OutToX(STORE_T,insAddr, &insDis,addr,length);
		addMemTainted(addr,length);
	}


}
VOID spreadRegTaint(UINT32 insAddr, std::string insDis, UINT32 OperandCount,REG reg_r, REG reg_w)
{
	OutToX(insAddr,&insDis);
	if (checkAlreadyRegTainted(reg_w) && (!REG_valid(reg_r) || !checkAlreadyRegTainted(reg_r))){
		OutToX(SPREE_F,insAddr, &insDis,reg_w);
		removeRegTainted(reg_w);
	}
	else if (!checkAlreadyRegTainted(reg_w) && checkAlreadyRegTainted(reg_r)){
		OutToX(SPREE_T,insAddr, &insDis,reg_w);
		taintReg(reg_w);
	}
}
VOID mulOper(UINT32 insAddr, std::string insDis, UINT32 OperandCount,REG reg_r, REG reg_w)
{
	if (REG_valid(reg_w)||REG_is_Half32(reg_r)){
		if ((!REG_valid(reg_r)||((!checkAlreadyRegTainted(reg_r))&&(!checkAlreadyRegTainted(REG_EAX)))))
		{
			if((checkAlreadyRegTainted(reg_w)||checkAlreadyRegTainted(REG_EDX))){

				OutToX(SPREE_F,insAddr, &insDis,reg_w);
				//std::cout << "\t\t\toutput: "<< REG_StringShort(reg_w) << " | input: " << (REG_valid(reg_r) ? REG_StringShort(reg_r) : "constant") << std::endl;
				removeRegTainted(reg_w);
				removeRegTainted(REG_EDX);
			}
		}else if((checkAlreadyRegTainted(reg_r)||checkAlreadyRegTainted(REG_EAX)))
		{
			OutToX(SPREE_T,insAddr, &insDis,reg_w);
			//std::cout << "\t\t\toutput: " << REG_StringShort(reg_w) << " | input: "<< REG_StringShort(reg_r) << std::endl;
			taintReg(reg_w);
			taintReg(REG_EDX);
		}
	}
}
VOID CmpMem(UINT32 insAddr, std::string insDis, REG reg, UINT32 memOp, UINT32 sp)
{
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n***************\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	if(checkAlreadyRegTainted(reg)){
		DEBUG("TYPE1:taint_branch:%x\n",insAddr);	
		BRINFO binst={insAddr, insDis};
		taintCmp.push_back(binst);
		return;
	}
	list<UINT32>::iterator i;
	for(i = addressTainted.begin(); i != addressTainted.end(); i++){
		if (memOp == *i){
			DEBUG("TYPE2:taint_branch:%x\n",insAddr);	
			BRINFO binst={insAddr, insDis};
			taintCmp.push_back(binst);
			return;
		} 
	}
}
VOID CmpReg(UINT32 insAddr, std::string insDis, REG reg1, REG reg2, UINT32 sp)
{
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n***************\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	if(checkAlreadyRegTainted(reg1)||checkAlreadyRegTainted(reg2)){
		
		DEBUG("TYPE3:taint_branch:%x\n",insAddr);	
		BRINFO binst={insAddr, insDis};
		taintCmp.push_back(binst);
		return;
	}
}

VOID CmpImmi(UINT32 insAddr, std::string insDis, REG reg, UINT32 sp)
{
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n***************\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	if(checkAlreadyRegTainted(reg)){
		
		DEBUG("TYPE4:taint_branch:%x\n",insAddr);	
		BRINFO binst={insAddr, insDis};
		taintCmp.push_back(binst);
		return;
	}
}
VOID CmpImmi2(UINT32 insAddr, std::string insDis, UINT32 memOp, UINT32 sp)
{
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n***************\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("memread addr:%x\n",memOp);
	list<UINT32>::iterator i;
	for(i = addressTainted.begin(); i != addressTainted.end(); i++){
		if (memOp == *i){
			DEBUG("TYPE5:taint_branch:%x\n",insAddr);	
			BRINFO binst={insAddr, insDis};
			taintCmp.push_back(binst);
			return;
		} 
	}
}
VOID Arg1Before(ADDRINT size)
{
	regions[HEAP].end=size;
}

VOID MallocAfter(ADDRINT ret)
{
	regions[HEAP].base=ret;
	regions[HEAP].end=ret+regions[HEAP].end;
	std::cout << std::hex<<regions[HEAP].base<<"\t"<<regions[HEAP].end<< std::endl;
}
VOID  begintrace() 
{     
	outflag=false;
} 
VOID  endtrace()
{
	outflag=false;
}
VOID RTNInstruction(RTN rtn, VOID *v) 
{
	RTN_Open(rtn);

	string name=RTN_Name(rtn);
	/*if(name.compare("mymalloc")==0){
		RTN_InsertCall(rtn, IPOINT_BEFORE, (AFUNPTR)Arg1Before,
				IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
				IARG_END);
		//RTN_InsertCall(rtn, IPOINT_AFTER, (AFUNPTR)MallocAfter,
		//               IARG_FUNCRET_EXITPOINT_VALUE, IARG_END);
		RTN_InsertCall(rtn, IPOINT_AFTER, (AFUNPTR)MallocAfter,
				IARG_REG_VALUE, REG_EAX, IARG_END);

	} */ 
	if(routine_name.compare(name)==0){
		INS InsHead=RTN_InsHead(rtn);
		INS InsTail=RTN_InsTail(rtn);
		if(INS_Valid(InsHead))
			INS_InsertPredicatedCall(InsHead, IPOINT_BEFORE, AFUNPTR(begintrace),IARG_END);
		if(INS_Valid(InsTail))
			INS_InsertPredicatedCall(InsTail, IPOINT_BEFORE, AFUNPTR(endtrace),IARG_END);         
	}
	for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)){

		//std::cout<<std::hex << INS_Address(ins)<<INS_Disassemble(ins) <<":"<<INS_Opcode(ins)<< std::endl;
		if( INS_Opcode(ins)==99 ||  INS_Opcode(ins)==787 ){

			INT32 memOperands = INS_OperandCount(ins);
			//std::cout << INS_Address(ins)<<INS_Disassemble(ins) <<":"<<INS_Opcode(ins)<<":" << memOperands << ":" << INS_OperandIsImmediate(ins,0)   << INS_OperandIsImmediate(ins,1)<< std::endl;
			if(memOperands==3){
				if (INS_MemoryOperandIsRead(ins, 0) && INS_OperandIsReg(ins, 1)){
					INS_InsertCall(
							ins, IPOINT_BEFORE, (AFUNPTR)CmpMem,
							IARG_ADDRINT, INS_Address(ins),
							IARG_PTR, new string(INS_Disassemble(ins)),
							IARG_UINT32, INS_OperandReg(ins, 1),
							IARG_MEMORYOP_EA, 0,
							IARG_END);
				}else if(INS_MemoryOperandIsRead(ins, 1) && INS_OperandIsReg(ins, 0)){
					INS_InsertCall(
							ins, IPOINT_BEFORE, (AFUNPTR)CmpMem,
							IARG_ADDRINT, INS_Address(ins),
							IARG_PTR, new string(INS_Disassemble(ins)),
							IARG_UINT32, INS_OperandReg(ins, 0),
							IARG_MEMORYOP_EA, 1,
							IARG_END);
				}else if(INS_OperandIsReg(ins, 0) && INS_OperandIsReg(ins, 1)){
					INS_InsertCall(
							ins, IPOINT_BEFORE, (AFUNPTR)CmpReg,
							IARG_ADDRINT, INS_Address(ins),
							IARG_PTR, new string(INS_Disassemble(ins)),
							IARG_UINT32, INS_OperandReg(ins, 0),
							IARG_UINT32, INS_OperandReg(ins, 1),
							IARG_END);
				}else if(INS_OperandIsImmediate(ins,0)&& INS_OperandIsReg(ins, 1)){
					INS_InsertCall(
							ins, IPOINT_BEFORE, (AFUNPTR)CmpImmi,
							IARG_ADDRINT, INS_Address(ins),
							IARG_PTR, new string(INS_Disassemble(ins)),
							IARG_UINT32, INS_OperandReg(ins, 1),
							IARG_END);
				}else if(INS_OperandIsImmediate(ins,1)&& INS_OperandIsReg(ins, 0)){
					INS_InsertCall(
							ins, IPOINT_BEFORE, (AFUNPTR)CmpImmi,
							IARG_ADDRINT, INS_Address(ins),
							IARG_PTR, new string(INS_Disassemble(ins)),
							IARG_UINT32, INS_OperandReg(ins, 0),
							IARG_END);
				}else if(INS_OperandIsImmediate(ins,0)&& INS_MemoryOperandIsRead(ins, 1)){
					INS_InsertCall(
							ins, IPOINT_BEFORE, (AFUNPTR)CmpImmi2,
							IARG_ADDRINT, INS_Address(ins),
							IARG_PTR, new string(INS_Disassemble(ins)),
							IARG_MEMORYOP_EA, 1,
							IARG_END);
				}else if(INS_OperandIsImmediate(ins,1)&& INS_MemoryOperandIsRead(ins, 0)){
					INS_InsertCall(
							ins, IPOINT_BEFORE, (AFUNPTR)CmpImmi2,
							IARG_ADDRINT, INS_Address(ins),
							IARG_PTR, new string(INS_Disassemble(ins)),
							IARG_MEMORYOP_EA, 0,
							IARG_END);
				}
			}
		}else{
			if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsRead(ins, 0) /*&& INS_OperandIsReg(ins, 0)*/){
				INS_InsertCall(
						ins, IPOINT_BEFORE, (AFUNPTR)ReadMem,
						IARG_ADDRINT, INS_Address(ins),
						IARG_PTR, new string(INS_Disassemble(ins)),
						IARG_UINT32, INS_OperandCount(ins),
						IARG_UINT32, INS_OperandReg(ins, 0),
						IARG_MEMORYOP_EA, 0,
						IARG_REG_VALUE, REG_STACK_PTR,
						IARG_END);
			}else if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsWritten(ins, 0)){
				INS_InsertCall(
						ins, IPOINT_BEFORE, (AFUNPTR)WriteMem,
						IARG_ADDRINT, INS_Address(ins),
						IARG_PTR, new string(INS_Disassemble(ins)),
						IARG_UINT32, INS_OperandCount(ins),
						IARG_UINT32, INS_OperandReg(ins, 1),
						IARG_UINT32, INS_OperandReg(ins, 0),
						IARG_MEMORYOP_EA, 0,
						IARG_REG_VALUE, REG_STACK_PTR,
						IARG_END);
			}else if (INS_OperandCount(ins) > 1 && INS_OperandIsReg(ins, 0)){
				if(INS_Mnemonic(ins)=="IMUL"&&INS_OperandCount(ins)==4){
					//trace the mulOpereration taint spread
					INS_InsertCall(
							ins, IPOINT_BEFORE, (AFUNPTR)mulOper,
							IARG_ADDRINT, INS_Address(ins),
							IARG_PTR, new string(INS_Disassemble(ins)),
							IARG_UINT32, INS_OperandCount(ins),
							IARG_UINT32, INS_RegR(ins, 0),
							IARG_UINT32, INS_RegW(ins, 0),
							IARG_END);
				}else if(INS_OperandCount(ins) > 1 && INS_OperandIsReg(ins, 0)&& INS_OperandIsReg(ins, 1)){
					//trace the taint spread via “mov reg1，reg2”
					INS_InsertCall(
							ins, IPOINT_BEFORE, (AFUNPTR)spreadRegTaint,
							IARG_ADDRINT, INS_Address(ins),
							IARG_PTR, new string(INS_Disassemble(ins)),
							IARG_UINT32, INS_OperandCount(ins),
							IARG_UINT32, INS_RegR(ins, 0),
							IARG_UINT32, INS_RegW(ins, 0),
							IARG_END);
				}
			}

		}

	}
	RTN_Close(rtn);

} 

static unsigned int tryksOpen;

#define TRICKS(){if (tryksOpen++ == 0)return;}

VOID Syscall_entry(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v)
{
	unsigned int i;
	UINT32 start, size;

	if (PIN_GetSyscallNumber(ctx, std) == __NR_read){

		TRICKS(); /* tricks to ignore the first open */

		start = static_cast<UINT64>((PIN_GetSyscallArgument(ctx, std, 1)));
		size  = static_cast<UINT64>((PIN_GetSyscallArgument(ctx, std, 2)));

		for (i = 0; i < size; i++)
			addressTainted.push_back(start+i);

		outflag=true;
		std::cout << "[TAINT]\t\t\tbytes tainted from " << std::hex << "0x" << start << " to 0x" << start+size << " (via read)"<< std::endl;

	} 
}
VOID taint_init(){
	get_region_info();
}
VOID Fini(INT32 code, VOID *v)
{
	// Write to a file since cout and cerr maybe closed by the application
	printTaintMemory();
	printTaintCmpIns();
}
int main(int argc, char *argv[])
{
	PIN_InitSymbols();
	if(PIN_Init(argc, argv)){
		return Usage();
	}
	taint_init();
	PIN_SetSyntaxIntel();
	PIN_AddSyscallEntryFunction(Syscall_entry, 0);
	RTN_AddInstrumentFunction(RTNInstruction, 0);
	PIN_AddFiniFunction(Fini, 0);
	PIN_StartProgram();

	return 0;
}

