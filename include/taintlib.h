#include "pin.H"
#include <asm/unistd.h>
#include <fstream>
#include <iostream>
#include <list>

#ifndef TAINTLIB_H
#define TAINTLIB_H
#endif

#define LEN_ADDR 8

/* ===================================================================== */
/* Global Variables */
/* ===================================================================== */
enum AccessType{
	MR = 0 ,//Memory Read
	MW,	//Memory Write
};
typedef struct{


}taint_info;
typedef struct BRINFO
{
	UINT32 addr;
	string disas;	
	bool operator==(struct BRINFO b) const  
	{  
		return this->addr == b.addr;
	}  
	bool operator!=(struct BRINFO b) const  
	{  
		return this->addr != b.addr;
	} 
	bool operator>=(struct BRINFO b) const  
	{  
		return this->addr >= b.addr;
	} 
	bool operator<=(struct BRINFO b) const  
	{  
		return this->addr <= b.addr;
	} 
	bool operator>(struct BRINFO b) const  
	{  
		return this->addr > b.addr;
	} 
	bool operator<(struct BRINFO b) const  
	{  
		return this->addr < b.addr;
	} 
}BRINFO;

std::list<UINT32> addressTainted;
std::list<BRINFO> taintCmp;
bool shadowReg[100]={false};
typedef enum region_type {
	GLOBAL = 0,
	HEAP,
	STACK,
	CODE,
	NO_REGION
}region_type;

typedef struct region_info {
	region_type type;
	ADDRINT base;
	ADDRINT end;
}region_info;

char regin_type_str[5][6]={"uknow","stack"," heap"," glob","code"};

// size changes with region num
region_info regions[4];

/* ===================================================================== */
/* funcions for getting region info */
/* ===================================================================== */

void get_region_info()
{
	INT pid = PIN_GetPid();
	//printf("Pid : %d\n", pid);

	FILE *procmaps;
	char path[20];
	char line[100];

	sprintf(path, "/proc/%d/maps", pid);

	procmaps = fopen(path, "r"); 
	int timesdata = 0;
	int timescode = 0;

	while(fgets(line, 100, procmaps) != NULL) {
		//printf("%s", line);
		if (timescode == 0 && strstr(line, "r-xp")) {
			sscanf(strndup(line, 1+2*LEN_ADDR), "%x-%x", &regions[CODE].base, &regions[CODE].end);
			//printf("%x and %x\n", regions[CODE].base, regions[CODE].end);
			regions[CODE].type = CODE;
			timescode++;
		}
		if (timesdata == 0 && strstr(line, "rwxp")) {
			sscanf(strndup(line, 1+2*LEN_ADDR), "%x-%x", &regions[GLOBAL].base, &regions[GLOBAL].end);
			//printf("%x and %x\n", regions[GLOBAL].base, regions[GLOBAL].end);
			regions[GLOBAL].type = GLOBAL;
			timesdata++;
		}
		if (strstr(line, "[stack")) {
			sscanf(strndup(line, 1+2*LEN_ADDR), "%x-%x", &regions[STACK].base, &regions[STACK].end);
			regions[STACK].type = STACK;
			//printf("%x and %x\n", regions[STACK].base, regions[STACK].end);
		}
		if (strstr(line, "[heap")) {
			sscanf(strndup(line, 1+2*LEN_ADDR), "%x-%x", &regions[HEAP].base, &regions[HEAP].end);
			regions[HEAP].type = HEAP;
			//printf("%x and %x\n", regions[HEAP].base, regions[HEAP].end);
		} 
	} 
	//KNOB<BOOL>   KnobNoCompress(KNOB_MODE_WRITEONCE, "pintool","no_compress", "0", "Do not compress");

	//fprintf(trace, "%x;%x\n", regions[GLOBAL].base, regions[GLOBAL].end);
	//fprintf(trace, "%x;%x\n", regions[STACK].base, regions[STACK].end);
	//fprintf(trace, "%x;%x\n", regions[HEAP].base, regions[HEAP].end);
}
int GetReginFromAddr(VOID * poi){
	ADDRINT addr=(ADDRINT)poi;
	int type=0;
	if(addr<regions[STACK].end && addr>regions[STACK].base){
		type=1;
	}else if(addr<regions[HEAP].end && addr>regions[HEAP].base){
		type=2;
	}else if(addr<regions[GLOBAL].end && addr>regions[GLOBAL].base){
		type=3;
	}
	return type;
}

/* ===================================================================== */
/* funcions for Tainting */
/* ===================================================================== */
bool checkAlreadyRegTainted(REG reg)
{
	return shadowReg[reg];
}

bool taintReg(REG reg)
{
	if (shadowReg[reg] == true){
		std::cout << "\t\t\t" << REG_StringShort(reg) << " is already tainted" << std::endl;
		return false;
	}

	switch(reg){

		//case REG_RAX:  regsTainted.push_front(REG_RAX);
		case REG_EAX:  shadowReg[REG_EAX]=true; 
		case REG_AX:   shadowReg[REG_AX]=true; 
		case REG_AH:   shadowReg[REG_AH]=true;; 
		case REG_AL:   shadowReg[REG_AL]=true; 
			       break;

			       //case REG_RBX:  regsTainted.push_front(REG_RBX);
		case REG_EBX:  shadowReg[REG_EBX]=true; 
		case REG_BX:   shadowReg[REG_BX]=true; 
		case REG_BH:   shadowReg[REG_BH]=true; 
		case REG_BL:   shadowReg[REG_BL]=true; 
			       break;

			       //case REG_RCX:  regsTainted.push_front(REG_RCX); 
		case REG_ECX:  shadowReg[REG_ECX]=true; 
		case REG_CX:   shadowReg[REG_CX]=true; 
		case REG_CH:   shadowReg[REG_CH]=true; 
		case REG_CL:   shadowReg[REG_CL]=true; 
			       break;

			       //case REG_RDX:  regsTainted.push_front(REG_RDX); 
		case REG_EDX:  shadowReg[REG_EDX]=true;  
		case REG_DX:   shadowReg[REG_DX]=true; 
		case REG_DH:   shadowReg[REG_DH]=true;  
		case REG_DL:   shadowReg[REG_DL]=true;  
			       break;

			       //case REG_RDI:  regsTainted.push_front(REG_RDI); 
		case REG_EDI:  shadowReg[REG_EDI]=true;  
		case REG_DI:   shadowReg[REG_DI]=true; 
			       //case REG_DIL:  regsTainted.push_front(REG_DIL); 
			       break;

			       //case REG_RSI:  regsTainted.push_front(REG_RSI); 
		case REG_ESI:  shadowReg[REG_ESI]=true; 
		case REG_SI:   shadowReg[REG_SI]=true;  
			       //case REG_SIL:  regsTainted.push_front(REG_SIL); 
			       break;
		case REG_EFLAGS: shadowReg[REG_EFLAGS]=true; 
				 break;

		case REG_XMM0: shadowReg[REG_XMM0]=true; 
			       break;
		case REG_XMM1: shadowReg[REG_XMM1]=true; 
			       break;
		case REG_XMM2: shadowReg[REG_XMM2]=true; 
			       break;
		case REG_XMM3: shadowReg[REG_XMM3]=true; 
			       break;
		case REG_XMM4: shadowReg[REG_XMM4]=true; 
			       break;
		case REG_XMM5: shadowReg[REG_XMM5]=true; 
			       break;
		case REG_XMM6: shadowReg[REG_XMM6]=true; 
			       break;
		case REG_XMM7: shadowReg[REG_XMM7]=true; 
			       break;

		default:
			       std::cout << "\t\t\t" << REG_StringShort(reg) << " can't be tainted" << std::endl;
			       return false;
	}
	std::cout << "\t\t\t" << REG_StringShort(reg) << " is now tainted" << std::endl;
	return true;
}

bool removeRegTainted(REG reg)
{
	switch(reg){

		//case REG_RAX:  regsTainted.remove(REG_RAX);
		case REG_EAX:  shadowReg[REG_EAX]=false;
		case REG_AX:   shadowReg[REG_AX]=false;
		case REG_AH:   shadowReg[REG_AH]=false;
		case REG_AL:   shadowReg[REG_AL]=false;
			       break;

			       //case REG_RBX:  regsTainted.remove(REG_RBX);
		case REG_EBX:  shadowReg[REG_EBX]=false;
		case REG_BX:   shadowReg[REG_BX]=false;
		case REG_BH:   shadowReg[REG_BH]=false;
		case REG_BL:   shadowReg[REG_BL]=false;
			       break;

			       //case REG_RCX:  regsTainted.remove(REG_RCX); 
		case REG_ECX:  shadowReg[REG_ECX]=false;
		case REG_CX:   shadowReg[REG_CX]=false;
		case REG_CH:   shadowReg[REG_CH]=false;
		case REG_CL:   shadowReg[REG_CL]=false;
			       break;

			       //case REG_RDX:  regsTainted.remove(REG_RDX); 
		case REG_EDX:  shadowReg[REG_EDX]=false;
		case REG_DX:   shadowReg[REG_DX]=false;
		case REG_DH:   shadowReg[REG_DH]=false;
		case REG_DL:   shadowReg[REG_DL]=false;
			       break;

			       //case REG_RDI:  regsTainted.remove(REG_RDI); 
		case REG_EDI:  shadowReg[REG_EDI]=false;
		case REG_DI:   shadowReg[REG_DI]=false;
			       //case REG_DIL:  regsTainted.remove(REG_DIL); 
			       break;

			       //case REG_RSI:  regsTainted.remove(REG_RSI); 
		case REG_ESI:  shadowReg[REG_ESI]=false;
		case REG_SI:   shadowReg[REG_SI]=false;
			       //case REG_SIL:  regsTainted.remove(REG_SIL); 
			       break;

		case REG_EFLAGS: shadowReg[REG_EFLAGS]=false;
				 break;

		case REG_XMM0: shadowReg[REG_XMM0]=false;
			       break;
		case REG_XMM1: shadowReg[REG_XMM1]=false;
			       break;
		case REG_XMM2: shadowReg[REG_XMM2]=false;
			       break;
		case REG_XMM3: shadowReg[REG_XMM3]=false;
			       break;
		case REG_XMM4: shadowReg[REG_XMM4]=false;
			       break;
		case REG_XMM5: shadowReg[REG_XMM5]=false;
			       break;
		case REG_XMM6: shadowReg[REG_XMM6]=false;
			       break;
		case REG_XMM7: shadowReg[REG_XMM7]=false;
			       break;

		default:
			       return false;
	}
	std::cout << "\t\t\t" << REG_StringShort(reg) << " is now freed" << std::endl;
	return true;
}
VOID removeMemTainted(UINT32 addr,UINT32 length)
{
	for(UINT32 i=addr;i<(addr+length);i++){
		addressTainted.remove(i);
	}
	std::cout << std::hex << "\t\t\t" << addr<<"["<<length<< "] is now freed" << std::endl;
}

VOID addMemTainted(UINT32 addr,UINT32 length)
{
	for(UINT32 i=addr;i<(addr+length);i++){
		addressTainted.push_back(i);
	}
	std::cout << std::hex << "\t\t\t" << addr<<"["<<length<< "] is now tainted" << std::endl;
}
VOID printTaintMemory(){
	std::cout << std::hex << "\n\t" << "*****************Tainted memory in data section*****************" << std::endl;
	int count=0;
	list<UINT32>::iterator i;
	addressTainted.unique();
	for(i = addressTainted.begin(); i != addressTainted.end(); i++){
		if(*i>=regions[GLOBAL].base&&*i<=regions[GLOBAL].end){
			std::cout << std::hex<<"\t"<< *i;
			count++;
			if(count==8){
				count=0;
				std::cout << std::endl;
			}
		}
	}
	std::cout << std::hex << "\n\t" << "*****************Tainted memory in data section*****************" << std::endl;
}
VOID printTaintCmpIns(){
	std::cout << std::hex << "\n\t" << "*****************Tainted Cmp Instruction Address*****************" << std::endl;
	list<BRINFO>::iterator i;
	taintCmp.sort();
	taintCmp.unique();
	for(i = taintCmp.begin(); i != taintCmp.end(); i++){
		if(i->addr >= regions[CODE].base&& i->addr <= regions[CODE].end){
			std::cout << std::hex<<"\t"<< i->addr << i->disas << std::cout << std::endl;
		}
	}
	std::cout << std::hex << "\n\t" << "*****************Tainted Cmp Instruction Address*****************" << std::endl;
}

