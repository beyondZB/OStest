#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"



//================================================
//		sysGetTicks
//================================================
PUBLIC int sysGetTicks()
{
	return kernelGetTicks();
}


//================================================
//		kernelGetTicks
//================================================
PUBLIC int kernelGetTicks()
{
	return ticks;
}


//================================================
//		kernelBlock
//================================================
PUBLIC void kernelBlock()
{
	p_proc_running->state = PROC_STATE_BLOCKING;
	kernelShift();
}


//================================================
//		kernelUnBlock	
//================================================
PUBLIC void kernelUnBlock(u32 pid)
{
	proc_table[pid].state = PROC_STATE_READY;
//	disp_str("unblocked@@\n");
}



PUBLIC int sysBlock()
{
	kernelBlock();
}

PUBLIC int sysUnBlock(u32 pid)
{
	kernelUnBlock(pid);
}


