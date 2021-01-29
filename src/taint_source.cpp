#include "taint_source.h"
#if defined(TARGET_MAC)
#include <sys/syscall.h>
#else
#include <syscall.h>
#endif
extern bool outflag;
extern ShadowMem* shadow_mem;
extern InputHandler* input_handler;
unsigned int tryksOpen = 0;

#define TRICKS()\
 {if (tryksOpen++ == 0)return;}
 
VOID Syscall_entry(THREADID thread_id, CONTEXT *ctxt, SYSCALL_STANDARD std, void *v)
{
	
	UINT32 start, size;
	if (PIN_GetSyscallNumber(ctxt, std) == __NR_read){

		TRICKS(); /* tricks to ignore the first open */

		start = static_cast<UINT32>((PIN_GetSyscallArgument(ctxt, std, 1)));
		size  = static_cast<UINT32>((PIN_GetSyscallArgument(ctxt, std, 2)));
		input_handler->RegisterReadEnter(start);
		// shadow_mem->AddTaint(start, size);
		// outflag=true;
		cout << "[TAINT]\t\t\tbytes tainted from " << hex << "0x" << start << " to 0x" << start+size << " (via read)"<< endl;

	} 
}
VOID SysAfter( ADDRINT value, INT32 err, UINT32 gax )
{
    int error = 0;
    ADDRINT neg_one = (ADDRINT)(0-1);

    if ( err == 0 )
    {
        if ( gax != value )
            error = 1;
    }
    else
    {
        if ( value != neg_one )
            error = 3;
        if ( err != -(INT32)gax )
            error = 4;
    }

    if ( error == 0 ){
        cout << "Success: value=0x" << hex << value << ", errno=" << dec << err << endl;
    	cout << "__NR_read:" << __NR_read << endl;
    }else
    {
        cout << "Failure " << error << ": value=0x" << hex << value << ", errno=" << dec << err;
        cout << ", gax=0x" << hex << gax << endl;
    }

    cout << endl;
}
void Syscall_exit(THREADID thread_id, CONTEXT *ctxt, SYSCALL_STANDARD std, void *v)
{
	input_handler->RegisterReadExit(PIN_GetSyscallReturn(ctxt, std));
	// if (PIN_GetSyscallNumber(ctxt, std) == __NR_read){
		// cout << "sys num:" << PIN_GetSyscallNumber(ctxt, std)<< endl;
		// SysAfter(PIN_GetSyscallReturn(ctxt, std),
        //      PIN_GetSyscallErrno(ctxt, std),
        //      PIN_GetContextReg(ctxt, REG_GAX));
	// } 
}


void MainArg(ADDRINT argc, ADDRINT argv)
{
    cout << "main argc:" << "(" << argc << ")" << endl;
	for(int i=0;i<argc;i++){
		cout << "main argv["<< i <<"]:" << "(" << ((char **)argv)[i] << ")" << endl;
	}
}




