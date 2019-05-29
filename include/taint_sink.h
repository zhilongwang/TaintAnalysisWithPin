#ifndef _TAINT_SINK_H_
#define _TAINT_SINK_H_
#include "pin.H"

bool AddCallSinkFunction(INS ins);
bool AddCmpSinkFunction(INS ins);
bool TaintSink(INS ins);
#endif