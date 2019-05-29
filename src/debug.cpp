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
void print_memops(xed_decoded_inst_t* xedd) {
    unsigned int i, memops = xed_decoded_inst_number_of_memory_operands(xedd);
    printf("Memory Operands\n");
    
    for( i=0;i<memops ; i++)   {
        xed_bool_t r_or_w = 0;
        xed_reg_enum_t seg;
        xed_reg_enum_t base;
        xed_reg_enum_t indx;
        printf("  %u ",i);
        if ( xed_decoded_inst_mem_read(xedd,i)) {
            printf("   read ");
            r_or_w = 1;
        }
        if (xed_decoded_inst_mem_written(xedd,i)) {
            printf("written ");
            r_or_w = 1;
        }
        if (!r_or_w) {
            printf("   agen "); // LEA instructions
        }
        seg = xed_decoded_inst_get_seg_reg(xedd,i);
        if (seg != XED_REG_INVALID) {
            printf("SEG= %s ", xed_reg_enum_t2str(seg));
        }
        base = xed_decoded_inst_get_base_reg(xedd,i);
        if (base != XED_REG_INVALID) {
            printf("BASE= %3s/%3s ",
                   xed_reg_enum_t2str(base),
                   xed_reg_class_enum_t2str(xed_reg_class(base)));
        }
        indx = xed_decoded_inst_get_index_reg(xedd,i);
        if (i == 0 && indx != XED_REG_INVALID) {
            printf("INDEX= %3s/%3s ",
                   xed_reg_enum_t2str(indx),
                   xed_reg_class_enum_t2str(xed_reg_class(indx)));
            if (xed_decoded_inst_get_scale(xedd,i) != 0) {
                // only have a scale if the index exists.
                printf("SCALE= %u ",
                       xed_decoded_inst_get_scale(xedd,i));
            }
        }
        if (xed_operand_values_has_memory_displacement(xedd))
        {
            xed_uint_t disp_bits =
                xed_decoded_inst_get_memory_displacement_width(xedd,i);
            if (disp_bits)
            {
                xed_int64_t disp;
                printf("DISPLACEMENT_BYTES= %u ", disp_bits);
                disp = xed_decoded_inst_get_memory_displacement(xedd,i);
                printf("0x" XED_FMT_LX16 " base10=" XED_FMT_LD, disp, disp);
            }
        }
        printf(" ASZ%u=%u\n",
               i,
               xed_decoded_inst_get_memop_address_width(xedd,i));
    }
    printf("  MemopBytes = %u\n",
           xed_decoded_inst_get_memory_operand_length(xedd,0));
}
