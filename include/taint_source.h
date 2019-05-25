#ifndef _TAINT_INIT_H_
#define _TAINT_INIT_H_
#include "pin.H"
#include <asm/unistd.h>
#include "shadowmem.h"
#include<string>
#include<iostream>
using std::cout;
using std::string;

void Syscall_entry(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v);


#endif