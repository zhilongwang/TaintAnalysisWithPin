#ifndef CONTROL_H
#define CONTROL_H
#endif

#define GlobalTaint 0
#define OutToFile 1
#define FileName taintout.txt
#define OutToScreen 1
bool outflag=false;
const string routine_name="main";

string typestr[7]={"LOAD__F","LOAD__T","STORE_F","STORE_T","SPREE_F","SPREE_T","UKNOW"};
typedef enum taint_type {
    LOAD__F = 0,
    LOAD__T,
    STORE_F,
    STORE_T,
    SPREE_F,
    SPREE_T,
    UKNOW
}taint_type;
string reg_name=""; 
string* getRegName(REG reg){
	reg_name="UNKnowReg:"+reg; 
	switch(reg){

    //case REG_RAX:  regsTainted.push_front(REG_RAX);
    case REG_EAX:  reg_name="REG_EAX"; 
    	break;
    case REG_AX:   reg_name="REG_AX"; 
    	break;
    case REG_AH:   reg_name="REG_AH"; 
    	break;
    case REG_AL:   reg_name="REG_AL"; 
         break;

    //case REG_RBX:  regsTainted.push_front(REG_RBX);
    case REG_EBX:  reg_name="REG_EBX"; 
    	break;
    case REG_BX:   reg_name="REG_BX"; 
    	break; 
    case REG_BH:   reg_name="REG_BH"; 
    	break; 
    case REG_BL:   reg_name="REG_BL"; 
         break;

    //case REG_RCX:  regsTainted.push_front(REG_RCX); 
    case REG_ECX:  reg_name="REG_ECX"; 
    	break;
    case REG_CX:   reg_name="REG_CX"; 
    	break;
    case REG_CH:   reg_name="REG_CH"; 
    	break;
    case REG_CL:   reg_name="REG_CL"; 
    	break;

    //case REG_RDX:  regsTainted.push_front(REG_RDX); 
    case REG_EDX:  reg_name="REG_EDX"; 
    	break; 
    case REG_DX:   reg_name="REG_DX"; 
    	break;
    case REG_DH:   reg_name="REG_DH"; 
    	break;
    case REG_DL:   reg_name="REG_DL"; 
    	break;

    //case REG_RDI:  regsTainted.push_front(REG_RDI); 
    case REG_EDI:  reg_name="REG_EDI"; 
    	break; 
    case REG_DI:   reg_name="REG_DI"; 
    	break;

    //case REG_RSI:  regsTainted.push_front(REG_RSI); 
    case REG_ESI:  reg_name="REG_ESI"; 
    	break;
    case REG_SI:   reg_name="REG_SI"; 
    	break;
    case REG_EFLAGS: reg_name="REG_EFLAGS"; 
    	break;

    case REG_XMM0: reg_name="REG_XMM0"; 
    	break;
    case REG_XMM1: reg_name="REG_XMM1"; 
    	break;
    case REG_XMM2: reg_name="REG_XMM2"; 
    	break;
    case REG_XMM3: reg_name="REG_XMM3"; 
    	break;
    case REG_XMM4: reg_name="REG_XMM4"; 
    	break;
    case REG_XMM5: reg_name="REG_XMM5"; 
    	break;
    case REG_XMM6: reg_name="REG_XMM6"; 
    	break;
    case REG_XMM7: reg_name="REG_XMM7"; 
    	break;
    default:
      reg_name="UNKnowReg";  
  }
  return &reg_name;
}

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
void OutToX(taint_type type,UINT32 insAddr, string *insDis,UINT32 addr,REG reg){
  if(outflag){
    std::cout <<std::hex <<"["<<insAddr<<"]["<<typestr[type]<<"]["<<*getRegName(reg)<<"] ACCESS:["<<addr<<"] insDis:["<<*insDis<<"]"<< std::endl;
  }
}
void OutToX(taint_type type,UINT32 insAddr, string *insDis,UINT32 addr,UINT32 length){
  if(outflag){
    std::cout <<std::hex <<"["<<insAddr<<"]["<<typestr[type]<<"]["<<length<<"] ACCESS:["<<addr<<"] insDis:["<<*insDis<<"]"<< std::endl;
  }
}
void OutToX(taint_type type,UINT32 insAddr, string *insDis,REG reg){
  if(outflag){
    std::cout <<std::hex <<"["<<insAddr<<"]["<<typestr[type]<<"]["<<*getRegName(reg)<<"] insDis:["<<*insDis<<"]"<< std::endl;
  }
}
