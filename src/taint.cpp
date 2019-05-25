#include "pin.H"
#include "taint_source.h"
#include "instruction.h"
#include "debug.h"
#include "taint_spread.h"
#include "region.h"
#include "shadowmem.h"
#include <fstream>
#include <iostream>
#include <list>
using std::cout;
using std::hex;
using std::cerr;

#define __DEBUG
#ifdef __DEBUG
#define DEBUG(format,...) printf(format,##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif
#define MAX_WRITE_REGS 3
#define MAX_READ_REGS 3
#define MAX_RW_REGS 4
RegionInfo* region_info;
ShadowMem* shadow_mem;
ShadowReg* shadow_reg;
extern bool outflag;

AFUNPTR Mem2Reg[] =
{
	NULL, AFUNPTR(MemTo1Reg), AFUNPTR(MemTo2Reg), AFUNPTR(MemTo3Reg)
};
AFUNPTR Reg2Mem[] =
{
	AFUNPTR(ZeroReg2Mem), AFUNPTR(OneReg2Mem), AFUNPTR(TwoReg2Mem), AFUNPTR(ThreeReg2Mem),
};
AFUNPTR Reg2Regs[] =
{
	NULL, AFUNPTR(OneRegs), AFUNPTR(TwoRegs), AFUNPTR(ThreeRegs), AFUNPTR(FourRegs),
};
//std::list<Ins_info> taintCmp;

INT32 Usage()
{
	cerr << "Ex 4" << endl;
	return -1;
}

VOID  BeforeMain() 
{     
	outflag=false;
} 
VOID  AfterMain()
{
	outflag=false;
}
VOID Instruction(INS ins) 
{
	//TAINT_Sink(ins);
	cout << hex << INS_Address(ins) << ":\t"<<INS_Disassemble(ins) << endl;
	if(INS_OperandCount(ins) <= 1){ //One Operand does not incurs data spread.
		return ;
	}
	if(INS_IsMemoryRead(ins) && INS_IsMemoryWrite(ins)){
		//cout << hex << INS_Address(ins) << ":\t"<<INS_Disassemble(ins) << endl;
		// cout << "MemoryRead && Memory Write" << endl;
	}else if(INS_IsMemoryRead(ins)){
		cout << "[Memory2Reg]:" << endl;
		UINT32 write_reg_count = INS_MaxNumRRegs(ins);
		if(write_reg_count > 0 && write_reg_count <= MAX_WRITE_REGS ){
			IARGLIST args = IARGLIST_Alloc();
			for (UINT32 i = 0; i < write_reg_count; i++){
				REG reg =  INS_RegW(ins, i);
				if()
				cout << "--write reg:" << i << ":" << REG_StringShort(reg)<< endl;
				IARGLIST_AddArguments(args, IARG_UINT32, reg, IARG_END);
			}
			cout << "--*->*"<< endl;
			INS_InsertCall(
						ins, IPOINT_BEFORE, Mem2Reg[write_reg_count],
						IARG_INST_PTR, 
						IARG_PTR, new string(INS_Disassemble(ins)),
						IARG_MEMORYREAD_EA,
						IARG_MEMORYREAD_SIZE,
						IARG_IARGLIST, args,
						IARG_END);
			IARGLIST_Free(args);
		}
	}else if(INS_HasMemoryRead2(ins)){
		// cout << hex << INS_Address(ins) << ":\t"<<INS_Disassemble(ins) << endl;
		cout << "[DoubleMemory2Reg]:" << endl;
	}else if(INS_IsMemoryWrite(ins)){ 
		cout << "[Reg2Mem]:" << endl;
		UINT32 read_reg_count = INS_MaxNumRRegs(ins);
		if(read_reg_count <= MAX_READ_REGS ){
			IARGLIST args = IARGLIST_Alloc();
			for (UINT32 i = 0; i < read_reg_count; i++)
			{
				REG reg =  INS_RegR(ins, i);
				cout << "--read reg:" << i << ":" << REG_StringShort(reg)<< endl;
				IARGLIST_AddArguments(args, IARG_UINT32, reg, IARG_END);
			}
			cout << "--*->*"<< endl;
			INS_InsertCall(
						ins, IPOINT_BEFORE, Reg2Mem[read_reg_count],
						IARG_INST_PTR, 
						IARG_PTR, new string(INS_Disassemble(ins)),
						IARG_MEMORYWRITE_EA,
						IARG_MEMORYWRITE_SIZE,
						IARG_IARGLIST, args,
						IARG_END);
			IARGLIST_Free(args);
		}
	}else {
		cout << "[RegisterOnly]:" << endl;
		UINT32 read_reg_count = INS_MaxNumRRegs(ins);
		UINT32 write_reg_count = INS_MaxNumWRegs(ins);
		if(read_reg_count + write_reg_count > 0 && read_reg_count + write_reg_count <= MAX_RW_REGS ){
			IARGLIST args = IARGLIST_Alloc();
			for(UINT32 i=0; i<read_reg_count; i++){
				REG reg =  INS_RegR(ins, i);
				cout << "--read reg:" << i << ":" << REG_StringShort(reg)<< endl;
				IARGLIST_AddArguments(args, IARG_UINT32, reg, IARG_END);
			}
			for (UINT32 i = 0; i < write_reg_count; i++)
			{
				REG reg =  INS_RegW(ins, i);
				cout << "--write reg:" << i << ":" << REG_StringShort(reg)<< endl;
				IARGLIST_AddArguments(args, IARG_UINT32, reg, IARG_END);
			}
			cout << "--*->*"<< endl;
			INS_InsertCall(
						ins, IPOINT_BEFORE, Reg2Regs[read_reg_count + write_reg_count],
						IARG_INST_PTR, 
						IARG_PTR, new string(INS_Disassemble(ins)),
						IARG_UINT32, read_reg_count,
						IARG_IARGLIST, args,
						IARG_END);
			IARGLIST_Free(args);
		}
	}

		


/*
			if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsRead(ins, 0) ){
				INS_InsertCall(
						ins, IPOINT_BEFORE, (AFUNPTR)ReadMEM,
						IARG_ADDRINT, INS_Address(ins),
						IARG_PTR, new string(INS_Disassemble(ins)),
						IARG_UINT32, INS_OperandCount(ins),
						IARG_UINT32, INS_OperandReg(ins, 0),
						IARG_MEMORYOP_EA, 0,
						IARG_REG_VALUE, REG_STACK_PTR,
						IARG_END);
			}else if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsWritten(ins, 0)){
				INS_InsertCall(
						ins, IPOINT_BEFORE, (AFUNPTR)WriteMEM,
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
							ins, IPOINT_BEFORE, (AFUNPTR)SpreadREG,
							IARG_ADDRINT, INS_Address(ins),
							IARG_PTR, new string(INS_Disassemble(ins)),
							IARG_UINT32, INS_OperandCount(ins),
							IARG_UINT32, INS_RegR(ins, 0),
							IARG_UINT32, INS_RegW(ins, 0),
							IARG_END);
				}
			}

*/
		//std::cout<<std::hex << INS_Address(ins)<<INS_Disassemble(ins) <<":"<<INS_Opcode(ins)<< std::endl;
		/*if( INS_Opcode(ins)==99 ||  INS_Opcode(ins)==787 ){

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
		}*/

} 



VOID Routine(RTN rtn, void * v){
	RTN_Open(rtn);
	//hook entrance and exit of main function.
	
	if(strcmp(RTN_Name(rtn).c_str(),"main")==0){
		RTN_InsertCall(rtn, IPOINT_BEFORE, AFUNPTR(BeforeMain), IARG_END);
		RTN_InsertCall(rtn, IPOINT_AFTER, AFUNPTR(AfterMain), IARG_END);
	}
	//add trace function 
	cout << "RTN = " << RTN_Name(rtn) << endl;
	for ( INS ins=RTN_InsHead(rtn) ; INS_Valid(ins) ; ins=INS_Next(ins) )
	{
		Instruction(ins);
		/*cout << ": " << INS_Disassemble(ins) << endl;
		string *disas = new string("[INFO]"+*ADDRINT2str(INS_Address(ins))+"\t" + INS_Disassemble(ins));

		if( INS_IsCall(ins) ){
			string  filePath;
			PIN_GetSourceLocation(RTN_Address(rtn), NULL, NULL, &filePath);

			if (filePath != "")
			{
				//disas = *disas + "\n" + new string(filePath);
			}
		}
		INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(docount),
				IARG_PTR, disas,
				IARG_END);	
		*/

	}
	RTN_Close(rtn);
}
static void Image(IMG img, VOID *v)
{
	if (!IMG_IsMainExecutable(img))
	{
		return;
	}
	for( SEC sec=IMG_SecHead(img); SEC_Valid(sec) ; sec=SEC_Next(sec) )
	{

		if ( SEC_IsExecutable(sec) && strcmp(SEC_Name(sec).c_str(),".text")==0 )
		{
			cout << "SEC = " << SEC_Name(sec) << endl;
			for( RTN rtn=SEC_RtnHead(sec); RTN_Valid(rtn); rtn=RTN_Next(rtn) )
				Routine(rtn,v);
		}
	}
	/*RTN mainRtn = RTN_FindByName(img, NAME("main"));
	  if (!RTN_Valid(mainRtn)){
	  RTN_Open(mainRtn);
	  RTN_InsertCall(mainRtn, IPOINT_AFTER, AFUNPTR(AfterMain), IARG_THREAD_ID, IARG_CONTEXT, IARG_END);
	  RTN_Close(mainRtn);
	  }
	  */
}

/* ===================================================================== */

VOID TAINT_Init(){
	region_info = new RegionInfo();
	shadow_mem = new ShadowMem();
	shadow_reg = new ShadowReg();
}
VOID Fini(INT32 code, VOID *v)
{
	// Write to a file since cout and cerr maybe closed by the application
	//printTaintMemory();
	//printTaintCmpIns();
}
  
  
/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int  main(int argc, char *argv[])
{

	PIN_InitSymbols();

	if( PIN_Init(argc,argv) )
	{
		return Usage();
	}
	TAINT_Init();
	PIN_SetSyntaxATT(); //Sets the disassembly syntax to Intel format. 
/*
	TraceFile.open(KnobOutputFile.Value().c_str());
	TraceFile << hex;
	TraceFile.setf(ios::showbase);

	string trace_header = string("#\n"
			"# Trace Generated By Pin\n"
			"#\n");
	TraceFile.write(trace_header.c_str(),trace_header.size());
*/
	PIN_AddSyscallEntryFunction(Syscall_entry, 0);
	IMG_AddInstrumentFunction(Image, 0);
	//TRACE_AddInstrumentFunction(Trace, 0);
	PIN_AddFiniFunction(Fini, 0);

	// Never returns

	PIN_StartProgram();

	return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
