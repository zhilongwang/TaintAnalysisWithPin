#include "taint_source.h"
extern bool outflag;
extern ShadowMem* shadow_mem;

unsigned int tryksOpen;

#define TRICKS()\
 {if (tryksOpen++ == 0)return;}
 
VOID Syscall_entry(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v)
{
	UINT32 start, size;

	if (PIN_GetSyscallNumber(ctx, std) == __NR_read){

		//TRICKS(); /* tricks to ignore the first open */

		start = static_cast<UINT64>((PIN_GetSyscallArgument(ctx, std, 1)));
		size  = static_cast<UINT64>((PIN_GetSyscallArgument(ctx, std, 2)));

		shadow_mem->AddTaint(start, size);

		outflag=true;
		std::cout << "[TAINT]\t\t\tbytes tainted from " << std::hex << "0x" << start << " to 0x" << start+size << " (via read)"<< std::endl;

	} 
}