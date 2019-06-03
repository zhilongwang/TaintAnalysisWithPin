#ifndef _DEBUG_H_
#define _DEBUG_H_
#include "pin.H"
extern "C" {
  #include "xed-interface.h"
}
#include "instruction.h"
#include "register.h"
#include<string>
#include<iostream>
using std::string;
using std::cout;

#define __DEBUG_ANALYIZE
#define __DEGUG_TRACE
#define __DEGUG_SINK



#define GlobalTaint 0
#define OutToFile 1
#define FileName taintout.txt
#define OutToScreen 1

const string *Target2String(ADDRINT target);
string *ADDRINT2str(ADDRINT value);
void OutToX(UINT32 insAddr,string *insDis);
void OutToX(TINS_TYPE type,UINT32 insAddr, string *insDis,UINT32 addr,REG reg);
void OutToX(TINS_TYPE type,UINT32 insAddr, string *insDis,UINT32 addr,UINT32 length);
void OutToX(TINS_TYPE type,UINT32 insAddr, string *insDis,REG reg);
void print_memops(xed_decoded_inst_t* xedd);
#endif
