#ifndef _SHADOWMEM_H_
#define _SHADOWMEM_H_
#include "pin.H"
#include "region.h" 
#include<list>
#include<hash_map>

#define PAGE_SIZE 4096
#define VIRT2PEK(addr)      ((addr) >> 12) //PEK- page entry key
#define VIRT2OFFSET(addr)   ((addr) & 0x00000fff)
#define PEK2VIRT(PTE)       ((PTE) << 12)
#define OFFSET2VIRT(PTE, offset) (((PTE) << 12) + offset)


typedef unsigned char *  ShadowPage;
class ShadowMem{
private:
    std::hash_map<UINT32, ShadowPage> page_table_;
public:
    ~ShadowMem(){
        auto map_it = page_table_.begin();
        while (map_it != page_table_.end()){
            ShadowPage page_table = map_it->second;
            free(page_table);
            ++map_it;
        }
    }
    ShadowPage AllocateShadowPage(UINT32 page_entry_key);
    std::hash_map<UINT32, ShadowPage>::value_type GetPageTableEntry(UINT32 virtual_address);
    bool CheckTaint(UINT32 addr, UINT32 length);
    VOID RemoveTaint(UINT32 addr,UINT32 length);
    VOID AddTaint(UINT32 addr,UINT32 length);
    VOID PrintTaint(RegionInfo * regions, RegionType print_region);
    /*VOID printTaintCmpIns();*/
};


#endif