#ifndef _SHADOWMEM_H_
#define _SHADOWMEM_H_
#include "pin.H"
#include "region.h" 
#include<list>

class ShadowMem{
private:
    std::list<UINT32> address_tainted_;
public:

    bool checkTaint(UINT32 addr, UINT32 length);
    VOID removeTaint(UINT32 addr,UINT32 length);
    VOID addTaint(UINT32 addr,UINT32 length);
    VOID printTaint(REGIN_Tnfo * regions);
    /*VOID printTaintCmpIns();*/

};


#endif