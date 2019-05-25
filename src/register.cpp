#include "register.h"
#include<string>
#include<iostream>
using std::string;
using std::iostream;

string GetREGName(REG reg){
  string reg_name; 
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
  return reg_name;
}

/* ===================================================================== */
/* funcions for Tainting */
/* ===================================================================== */
bool ShadowReg::checkREG(REG reg)
{
	return shadow_reg_[reg];
}

bool ShadowReg::taintREG(REG reg)
{
	if (shadow_reg_[reg] == true){
		cout << "\t\t\t" << REG_StringShort(reg) << " is already tainted" << endl;
		return false;
	}

	switch(reg){

		//case REG_RAX:  regsTainted.push_front(REG_RAX);
		case REG_EAX:  shadow_reg_[REG_EAX]=true; 
		case REG_AX:   shadow_reg_[REG_AX]=true; 
		case REG_AH:   shadow_reg_[REG_AH]=true;; 
		case REG_AL:   shadow_reg_[REG_AL]=true; 
			       break;

			       //case REG_RBX:  regsTainted.push_front(REG_RBX);
		case REG_EBX:  shadow_reg_[REG_EBX]=true; 
		case REG_BX:   shadow_reg_[REG_BX]=true; 
		case REG_BH:   shadow_reg_[REG_BH]=true; 
		case REG_BL:   shadow_reg_[REG_BL]=true; 
			       break;

			       //case REG_RCX:  regsTainted.push_front(REG_RCX); 
		case REG_ECX:  shadow_reg_[REG_ECX]=true; 
		case REG_CX:   shadow_reg_[REG_CX]=true; 
		case REG_CH:   shadow_reg_[REG_CH]=true; 
		case REG_CL:   shadow_reg_[REG_CL]=true; 
			       break;

			       //case REG_RDX:  regsTainted.push_front(REG_RDX); 
		case REG_EDX:  shadow_reg_[REG_EDX]=true;  
		case REG_DX:   shadow_reg_[REG_DX]=true; 
		case REG_DH:   shadow_reg_[REG_DH]=true;  
		case REG_DL:   shadow_reg_[REG_DL]=true;  
			       break;

			       //case REG_RDI:  regsTainted.push_front(REG_RDI); 
		case REG_EDI:  shadow_reg_[REG_EDI]=true;  
		case REG_DI:   shadow_reg_[REG_DI]=true; 
			       //case REG_DIL:  regsTainted.push_front(REG_DIL); 
			       break;

			       //case REG_RSI:  regsTainted.push_front(REG_RSI); 
		case REG_ESI:  shadow_reg_[REG_ESI]=true; 
		case REG_SI:   shadow_reg_[REG_SI]=true;  
			       //case REG_SIL:  regsTainted.push_front(REG_SIL); 
			       break;
		case REG_EFLAGS: shadow_reg_[REG_EFLAGS]=true; 
				 break;

		case REG_XMM0: shadow_reg_[REG_XMM0]=true; 
			       break;
		case REG_XMM1: shadow_reg_[REG_XMM1]=true; 
			       break;
		case REG_XMM2: shadow_reg_[REG_XMM2]=true; 
			       break;
		case REG_XMM3: shadow_reg_[REG_XMM3]=true; 
			       break;
		case REG_XMM4: shadow_reg_[REG_XMM4]=true; 
			       break;
		case REG_XMM5: shadow_reg_[REG_XMM5]=true; 
			       break;
		case REG_XMM6: shadow_reg_[REG_XMM6]=true; 
			       break;
		case REG_XMM7: shadow_reg_[REG_XMM7]=true; 
			       break;

		default:
			       cout << "\t\t\t" << REG_StringShort(reg) << " can't be tainted" << endl;
			       return false;
	}
	cout << "\t\t\t" << REG_StringShort(reg) << " is now tainted" << endl;
	return true;
}

bool ShadowReg::removeREG(REG reg)
{
	switch(reg){

		//case REG_RAX:  regsTainted.remove(REG_RAX);
		case REG_EAX:  shadow_reg_[REG_EAX]=false;
		case REG_AX:   shadow_reg_[REG_AX]=false;
		case REG_AH:   shadow_reg_[REG_AH]=false;
		case REG_AL:   shadow_reg_[REG_AL]=false;
			       break;

			       //case REG_RBX:  regsTainted.remove(REG_RBX);
		case REG_EBX:  shadow_reg_[REG_EBX]=false;
		case REG_BX:   shadow_reg_[REG_BX]=false;
		case REG_BH:   shadow_reg_[REG_BH]=false;
		case REG_BL:   shadow_reg_[REG_BL]=false;
			       break;

			       //case REG_RCX:  regsTainted.remove(REG_RCX); 
		case REG_ECX:  shadow_reg_[REG_ECX]=false;
		case REG_CX:   shadow_reg_[REG_CX]=false;
		case REG_CH:   shadow_reg_[REG_CH]=false;
		case REG_CL:   shadow_reg_[REG_CL]=false;
			       break;

			       //case REG_RDX:  regsTainted.remove(REG_RDX); 
		case REG_EDX:  shadow_reg_[REG_EDX]=false;
		case REG_DX:   shadow_reg_[REG_DX]=false;
		case REG_DH:   shadow_reg_[REG_DH]=false;
		case REG_DL:   shadow_reg_[REG_DL]=false;
			       break;

			       //case REG_RDI:  regsTainted.remove(REG_RDI); 
		case REG_EDI:  shadow_reg_[REG_EDI]=false;
		case REG_DI:   shadow_reg_[REG_DI]=false;
			       //case REG_DIL:  regsTainted.remove(REG_DIL); 
			       break;

			       //case REG_RSI:  regsTainted.remove(REG_RSI); 
		case REG_ESI:  shadow_reg_[REG_ESI]=false;
		case REG_SI:   shadow_reg_[REG_SI]=false;
			       //case REG_SIL:  regsTainted.remove(REG_SIL); 
			       break;

		case REG_EFLAGS: shadow_reg_[REG_EFLAGS]=false;
				 break;

		case REG_XMM0: shadow_reg_[REG_XMM0]=false;
			       break;
		case REG_XMM1: shadow_reg_[REG_XMM1]=false;
			       break;
		case REG_XMM2: shadow_reg_[REG_XMM2]=false;
			       break;
		case REG_XMM3: shadow_reg_[REG_XMM3]=false;
			       break;
		case REG_XMM4: shadow_reg_[REG_XMM4]=false;
			       break;
		case REG_XMM5: shadow_reg_[REG_XMM5]=false;
			       break;
		case REG_XMM6: shadow_reg_[REG_XMM6]=false;
			       break;
		case REG_XMM7: shadow_reg_[REG_XMM7]=false;
			       break;

		default:
			       return false;
	}
	cout << "\t\t\t" << REG_StringShort(reg) << " is now freed" << endl;
	return true;
}