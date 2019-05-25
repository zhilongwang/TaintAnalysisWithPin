#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_
#include<string>
using std::string;

typedef enum TINS_TYPE{
    LOAD__F = 0,
    LOAD__T,
    STORE_F,
    STORE_T,
    SPREE_F,
    SPREE_T,
    UKNOW
}TINS_TYPE;

enum AccessType{
	MR = 0 ,//Memory Read
	MW,	//Memory Write
    MRW, //Memory RW
	RR, //Register Read
	RW, //Register Write
};

typedef struct InsInfo
{
	UINT32 addr;
	string disas;	
	bool operator==(struct InsInfo b) const  
	{  
		return this->addr == b.addr;
	}  
	bool operator!=(struct InsInfo b) const  
	{  
		return this->addr != b.addr;
	} 
	bool operator>=(struct InsInfo b) const  
	{  
		return this->addr >= b.addr;
	} 
	bool operator<=(struct InsInfo b) const  
	{  
		return this->addr <= b.addr;
	} 
	bool operator>(struct InsInfo b) const  
	{  
		return this->addr > b.addr;
	} 
	bool operator<(struct InsInfo b) const  
	{  
		return this->addr < b.addr;
	} 
}InsInfo;


#endif