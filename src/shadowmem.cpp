#include "shadowmem.h"
#include<iostream>
using std::string;
using std::iostream;

bool ShadowMem::checkTaint(UINT32 addr, UINT32 length)
{
	list<UINT32>::iterator i;
	for(i = address_tainted_.begin(); i != address_tainted_.end(); i++){
		if (*i >= addr || *i < addr+length){
			return true;
		}
	}
	return false;
}
VOID ShadowMem::removeTaint(UINT32 addr,UINT32 length)
{
	list<UINT32>::iterator i;
	for(i = address_tainted_.begin(); i != address_tainted_.end(); i++){
		address_tainted_.remove(*i);
	}
	std::cout << std::hex << "\t\t\t" << addr<<"["<<length<< "] is now freed" << std::endl;
}

VOID ShadowMem::addTaint(UINT32 addr,UINT32 length)
{
	for(UINT32 i=addr; i<(addr+length); i++){
		address_tainted_.push_back(i);
	}
	std::cout << std::hex << "\t\t\t" << addr<<"["<<length<< "] is now tainted" << std::endl;
}
VOID ShadowMem::printTaint(REGIN_Tnfo * regions){
	std::cout << std::hex << "\n\t" << "*****************Tainted memory in data section*****************" << std::endl;
	int count=0;
	list<UINT32>::iterator i;
	address_tainted_.unique();
	for(i = address_tainted_.begin(); i != address_tainted_.end(); i++){
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
/*
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
}*/

