#include "debug.h"
#include "register.h"
string typestr[7]={"LOAD__F","LOAD__T","STORE_F","STORE_T","SPREE_F","SPREE_T","UKNOW"};
bool outflag=false;
string invalid = "invalid_rtn";
/*
*print analysis instructions
*/
void OutToX(UINT32 insAddr,string *insDis){
  if(outflag){
    ;//std::cout<<std::hex<<insAddr<<":\t" <<*insDis << std::endl;
  }
}
/*
*print analysis instructions
*/
void OutToX(TINS_TYPE type,UINT32 insAddr, string *insDis,UINT32 addr,REG reg){
  if(outflag){
    std::cout <<std::hex <<"["<<insAddr<<"]["<<typestr[type]<<"]["<<GetREGName(reg)<<"] ACCESS:["<<addr<<"] insDis:["<<*insDis<<"]"<< std::endl;
  }
}
void OutToX(TINS_TYPE type,UINT32 insAddr, string *insDis,UINT32 addr,UINT32 length){
  if(outflag){
    std::cout <<std::hex <<"["<<insAddr<<"]["<<typestr[type]<<"]["<<length<<"] ACCESS:["<<addr<<"] insDis:["<<*insDis<<"]"<< std::endl;
  }
}
void OutToX(TINS_TYPE type,UINT32 insAddr, string *insDis,REG reg){
  if(outflag){
    std::cout <<std::hex <<"["<<insAddr<<"]["<<typestr[type]<<"]["<<GetREGName(reg)<<"] insDis:["<<*insDis<<"]"<< std::endl;
  }
}

string *ADDRINT2str(ADDRINT value){
	char str[16];
    	sprintf(str,"%x",(unsigned int)value);
	return new string(str);
}
/* ===================================================================== */
const string *Target2String(ADDRINT target)
{
    string name = RTN_FindNameByAddress(target);
    if (name == "")
        return &invalid;
    else
        return new string(name);
}
