#include "pin.H"
extern "C" {
  #include "xed-interface.h"
}
#include "taint.h"
#include "taint_source.h"
#include "instruction.h"
#include "debug.h"
#include "taint_spread.h"
#include "region.h"
#include "shadowmem.h"
#include "taint_sink.h"
#include "rt_check.h"
#include <fstream>
#include <iostream>
#include <list>
using std::cout;
using std::hex;
using std::cerr;
#ifdef __DEBUG_ANALYIZE
#define D(x) x
#else 
#define D(x)
#endif

#define MAX_WRITE_REGS 3
#define MAX_READ_REGS 3
#define MAX_RW_REGS 4

RegionInfo* region_info;
ShadowMem* shadow_mem;
ShadowReg* shadow_reg;
SinkInfo* sink_info;
extern bool outflag;

AFUNPTR Mem2Reg[] =
{
	NULL, AFUNPTR(MemTo1Reg), AFUNPTR(MemTo2Reg), AFUNPTR(MemTo3Reg)
};
AFUNPTR Reg2Mem[] =
{
	AFUNPTR(ZeroReg2Mem), AFUNPTR(OneReg2Mem), AFUNPTR(TwoReg2Mem), AFUNPTR(ThreeReg2Mem),
};
AFUNPTR Reg2Reg[] =
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
	D(cout << hex << INS_Address(ins) << ":\t"<<INS_Disassemble(ins) << endl;)

	if(TaintSink(ins)){
		return ;
	}
	if(INS_OperandCount(ins) <= 1){ //One Operand does not incurs data spread.
		return ;
	}
	//print_memops(xedd);
	if(INS_IsMemoryRead(ins) && INS_IsMemoryWrite(ins)){
		//cout << hex << INS_Address(ins) << ":\t"<<INS_Disassemble(ins) << endl;
		D(cout << "MemoryRead && Memory Write" << endl;)
		D(cout << "--*->*"<< endl;)
		INS_InsertCall(
					ins, IPOINT_BEFORE, (AFUNPTR)Mem2Mem,
					IARG_INST_PTR, 
					IARG_PTR, new string(INS_Disassemble(ins)),
					IARG_MEMORYREAD_EA,
					IARG_MEMORYREAD_SIZE,
					IARG_MEMORYWRITE_EA,
					IARG_MEMORYWRITE_SIZE,
					IARG_END);
	}else if(INS_IsMemoryRead(ins)){
		D(cout << "[Memory2Reg]:" << endl;)
		UINT32 write_reg_count = INS_MaxNumWRegs(ins);
		if(write_reg_count > 0 && write_reg_count <= MAX_WRITE_REGS ){
			IARGLIST args = IARGLIST_Alloc();
			for (UINT32 i = 0; i < write_reg_count; i++){
				REG reg =  INS_RegW(ins, i);
				
				D(cout << "--write reg:" << i << ":" << REG_StringShort(reg)<< endl;)
				IARGLIST_AddArguments(args, IARG_UINT32, reg, IARG_END);
			}
			D(cout << "--*->*"<< endl;)
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
		D(cout << "[DoubleMemory2Reg]:" << endl;)
	}else if(INS_IsMemoryWrite(ins)){ 
		D(cout << "[Reg2Mem]:" << endl;)

		//using intel xed engine to get more information about the instruction. 
		// xed_decoded_inst_t* xedd = INS_XedDec(ins);
		// xed_reg_enum_t xed_seg_reg = XED_STATIC_CAST(xed_reg_enum_t,xed3_operand_get_seg0(xedd));
		// xed_reg_enum_t xed_base_reg = XED_STATIC_CAST(xed_reg_enum_t,xed3_operand_get_base0(xedd));
		// REG pin_seg_reg = (xed_seg_reg == XED_REG_INVALID)? PIN_REG_INVALID : INS_XedExactMapToPinReg(xed_seg_reg);
		// REG pin_base_reg = (xed_base_reg == XED_REG_INVALID)? PIN_REG_INVALID : INS_XedExactMapToPinReg(xed_base_reg);
		REG pin_base_reg = INS_MemoryBaseReg(ins);
		REG pin_ss_reg = REG_SEG_SS;
		REG pin_gs_reg = REG_SEG_GS;
		UINT32 base_and_seg_reg_exclusive_count = 0;
		UINT32 read_reg_count = INS_MaxNumRRegs(ins);
		if(read_reg_count <= MAX_READ_REGS ){
			IARGLIST args = IARGLIST_Alloc();
			for (UINT32 i = 0; i < read_reg_count; i++)
			{
				REG reg =  INS_RegR(ins, i);
				if(INS_RegRContain(ins, reg) && (reg!= pin_base_reg) && (reg < pin_ss_reg || reg > pin_gs_reg)){ //non implicit read operand.
					base_and_seg_reg_exclusive_count++;
					D(cout << "--read reg:" << i << ":" << REG_StringShort(reg)<< endl;)
					IARGLIST_AddArguments(args, IARG_UINT32, reg, IARG_END);
				}else{
					D(cout << "??read reg:" << i << ":" << REG_StringShort(reg)<< endl;)
				}
			}
			D(cout << "--*->*"<< endl;)
			INS_InsertCall(
						ins, IPOINT_BEFORE, Reg2Mem[base_and_seg_reg_exclusive_count],
						IARG_INST_PTR, 
						IARG_PTR, new string(INS_Disassemble(ins)),
						IARG_MEMORYWRITE_EA,
						IARG_MEMORYWRITE_SIZE,
						IARG_IARGLIST, args,
						IARG_END);
			IARGLIST_Free(args);
		}
	}else {
		D(cout << "[RegisterOnly]:" << endl;)
		UINT32 read_reg_count = INS_MaxNumRRegs(ins);
		UINT32 write_reg_count = INS_MaxNumWRegs(ins);
		if(read_reg_count + write_reg_count > 0 && read_reg_count + write_reg_count <= MAX_RW_REGS ){
			IARGLIST args = IARGLIST_Alloc();
			for(UINT32 i=0; i<read_reg_count; i++){
				REG reg =  INS_RegR(ins, i);
				D(cout << "--read reg:" << i << ":" << REG_StringShort(reg)<< endl;)
				IARGLIST_AddArguments(args, IARG_UINT32, reg, IARG_END);
			}
			for (UINT32 i = 0; i < write_reg_count; i++)
			{
				REG reg =  INS_RegW(ins, i);
				D(cout << "--write reg:" << i << ":" << REG_StringShort(reg)<< endl;)
				IARGLIST_AddArguments(args, IARG_UINT32, reg, IARG_END);
			}
			D(cout << "--*->*"<< endl;)
			INS_InsertCall(
						ins, IPOINT_BEFORE, Reg2Reg[read_reg_count + write_reg_count],
						IARG_INST_PTR, 
						IARG_PTR, new string(INS_Disassemble(ins)),
						IARG_UINT32, read_reg_count,
						IARG_IARGLIST, args,
						IARG_END);
			IARGLIST_Free(args);
		}
	}
} 



VOID Routine(RTN rtn, void * v){
	RTN_Open(rtn);
	//hook entrance and exit of main function.
	
	if(strcmp(RTN_Name(rtn).c_str(),"main")==0){
		RTN_InsertCall(rtn, IPOINT_BEFORE, AFUNPTR(BeforeMain), IARG_END);
		RTN_InsertCall(rtn, IPOINT_AFTER, AFUNPTR(AfterMain), IARG_END);
	}
	//add trace function 
	D(cout << "RTN = " << RTN_Name(rtn) << endl;)
	for ( INS ins=RTN_InsHead(rtn) ; INS_Valid(ins) ; ins=INS_Next(ins) )
	{
		Instruction(ins);

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
			D(cout << "SEC = " << SEC_Name(sec) << endl;)
			for( RTN rtn=SEC_RtnHead(sec); RTN_Valid(rtn); rtn=RTN_Next(rtn) )
				Routine(rtn,v);
		}
	}
}
/* ===================================================================== */
VOID TAINT_Init(){
	
	region_info = new RegionInfo();
	shadow_mem = new ShadowMem();
	shadow_reg = new ShadowReg();
	sink_info = new SinkInfo();
}
VOID Fini(INT32 code, VOID *v)
{
	// Write to a file since cout and cerr maybe closed by the application

	sink_info->PrintSinkInfo();
	shadow_mem->PrintTaint(region_info, GLOBAL);
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
	PIN_SetSyntaxATT(); //Sets the disassembly syntax to Intel format. PIN_SetSyntaxXED()
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
