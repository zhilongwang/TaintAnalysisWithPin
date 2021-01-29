#ifndef _TAINT_INIT_H_
#define _TAINT_INIT_H_
#include "pin.H"
#include <asm/unistd.h>
#include "shadowmem.h"
#include<string>
#include<iostream>
using std::dec;
using std::ofstream;
using std::hex;
using std::endl;
using std::cout;

#define MAX_INPUT_NUM 40
typedef struct SysReadInfo{
    UINT32 start_addr; // start address of input
    UINT32 length;      // length of input
    UINT32 mark_addr;   // the address of region we are going to track.
    UINT32 mark_length; // the length of region we are going to track.
}SysReadInfo;
typedef struct InputInfo{
    UINT32 total_input_len;
    UINT32 alreay_read_len;
    UINT32 mark_addr;
    UINT32 mark_length;
}InputInfo;

class InputHandler{
private:
    InputInfo inputinfo;
    SysReadInfo sysreadinfo[MAX_INPUT_NUM];
    unsigned int read_count = 0;
    bool waiting_exit = false;
public:
    
    bool RegisterReadEnter(UINT32 start_addr){
        sysreadinfo[read_count] = {start_addr, 0, 0, 0};
        waiting_exit = true;
    }
    bool RegisterReadExit(UINT32 read_len){
        if(waiting_exit == false){
            return false;
        }
        sysreadinfo[read_count].length = read_len;
        cout << "Program Read:" <<(char *)sysreadinfo[read_count].start_addr << endl;
        waiting_exit = false;
        read_count ++;
        return true;
    }
    /*VOID printTaintCmpIns();*/
};

void Syscall_entry(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v);
void Syscall_exit(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v);
void MainArg(ADDRINT argc, ADDRINT argv);
#endif