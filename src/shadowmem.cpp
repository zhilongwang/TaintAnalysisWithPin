#include "shadowmem.h"

#include "debug.h"
#include<string>
#include<iostream>
using std::string;
using std::iostream;
using std::endl;
using std::cerr;
#ifdef __DEBUG_ANALYIZE
#define D(x) x
#else 
#define D(x)
#endif
std::hash_map<UINT32, ShadowPage>::value_type ShadowMem::GetPageTableEntry(UINT32 virtual_address){
	std::hash_map<UINT32, ShadowPage>::iterator it = page_table_.find(VIRT2PEK(virtual_address));
	if(it == page_table_.end()){
		ShadowPage shadow_page = (ShadowPage)malloc(PAGE_SIZE * sizeof(unsigned char));
		if(shadow_page == NULL){
			cerr<< "allocate page table failed" << endl;
		}
		memset(shadow_page, 0, PAGE_SIZE * sizeof(unsigned char));
		page_table_.insert({VIRT2PEK(virtual_address), shadow_page});
		it = page_table_.find(VIRT2PEK(virtual_address));
	}
	auto page_entry = *it;
	return page_entry;
}

bool ShadowMem::CheckTaint(UINT32 addr, UINT32 length)
{
	auto page_entry = GetPageTableEntry(addr);
	ShadowPage shadow_page = page_entry.second;
	for(UINT32 i=addr; i<(addr+length); i++){
		if( shadow_page[VIRT2OFFSET(i)] == 1){
			return true;
		}
	}
	return false;
}

VOID ShadowMem::RemoveTaint(UINT32 addr,UINT32 length)
{
	auto page_entry = GetPageTableEntry(addr);
	ShadowPage shadow_page = page_entry.second;
	for(UINT32 i=addr; i<(addr+length); i++){
		shadow_page[VIRT2OFFSET(i)] = 0;
	}
	D(std::cout << std::hex << "\t\t\t--" << addr<<"["<<length<< "] is now freed" << std::endl;)
}

VOID ShadowMem::AddTaint(UINT32 addr,UINT32 length)
{
	auto page_entry = GetPageTableEntry(addr);
	ShadowPage shadow_page = page_entry.second;
	for(UINT32 i=addr; i<(addr+length); i++){
		shadow_page[VIRT2OFFSET(i)] = 1;
	}
	D(std::cout << std::hex << "\t\t\t--" << addr<<"["<<length<< "] is now tainted" << std::endl;)
}
VOID ShadowMem::PrintTaint(RegionInfo * region_info, RegionType print_region){
	std::cout << std::hex << "\n\t" << "*****************Tainted memory in data section*****************" << std::endl;
	Region * regions = region_info->GetRegions();
	auto map_it = page_table_.begin();
	UINT32 format_control = 0;
	//std::cout << std::hex<<"\tbase:end:"<< regions[print_region].base << "::" << regions[print_region].end << std::endl;;
	while (map_it != page_table_.end()){
		UINT32 page_entry_key = map_it->first;
		ShadowPage page_table = map_it->second;
		//std::cout << std::hex<<"\tpage_table:"<< (UINT32)page_entry_key << "::" << std::endl;;
		for(UINT32 offset = 0; offset < PAGE_SIZE; offset++){
			if(OFFSET2VIRT(page_entry_key, offset) >= regions[print_region].base && OFFSET2VIRT(page_entry_key, offset) <= regions[print_region].end && page_table[offset] == 1){
				std::cout << std::hex<<"\t"<< OFFSET2VIRT(page_entry_key, offset);
				if(++format_control == 8){
						std::cout << "\n" ;
						format_control = 0;
				}
			
			}
		}
		++map_it;
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

