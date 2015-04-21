#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"


#define NR_SYS_GET_TICKS	0
#define NR_SYS_READ_KEY		1
#define NR_SYS_HD_IDENTIFY	2
#define NR_SYS_BLOCK		3
#define NR_SYS_UNBLOCK		4

PUBLIC int getTicks()
{
	return system_call(NR_SYS_GET_TICKS, 0, 0, 0, 0, 0);
}

PUBLIC VirtualKey readKey(){
	return (VirtualKey)( system_call(NR_SYS_READ_KEY, 0, 0, 0, 0, 0) );
}

PUBLIC void hdIdentify(int drive)
{
//	disp_str("HdIdentify!!!");
	system_call(NR_SYS_HD_IDENTIFY, drive, 0, 0, 0, 0);
}

PUBLIC void block()
{
	system_call(NR_SYS_BLOCK, 0, 0, 0, 0, 0);
}

PUBLIC void unBlock(u32 pid)
{
	system_call(NR_SYS_UNBLOCK, pid, 0, 0, 0, 0);
}
