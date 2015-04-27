#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"


#define NR_SYS_GET_TICKS	0
#define NR_SYS_READ_KEY		1
#define NR_SYS_HD_OPEN		2
#define NR_SYS_BLOCK		3
#define NR_SYS_UNBLOCK		4
#define NR_SYS_HD_CLOSE		5
#define NR_SYS_HD_READ		6
#define NR_SYS_HD_WRITE		7
#define NR_SYS_HD_IOCTL		8

PUBLIC int getTicks()
{
	return system_call(NR_SYS_GET_TICKS, 0, 0, 0, 0, 0);
}

PUBLIC VirtualKey readKey(){
	return (VirtualKey)( system_call(NR_SYS_READ_KEY, 0, 0, 0, 0, 0) );
}

PUBLIC void hdOpen(int device)
{
	system_call(NR_SYS_HD_OPEN, device, 0, 0, 0, 0);
}

PUBLIC void block()
{
	system_call(NR_SYS_BLOCK, 0, 0, 0, 0, 0);
}

PUBLIC void unBlock(u32 pid)
{
	system_call(NR_SYS_UNBLOCK, pid, 0, 0, 0, 0);
}

PUBLIC void hdClose(u32 device)
{
	system_call(NR_SYS_HD_CLOSE, device, 0, 0, 0, 0);
}

PUBLIC void hdRead(u32 device, u64 position, u32 count, char* buf)
{
	system_call(NR_SYS_HD_READ, device, position & 0xFFFFFFFF, (position >> 32) & 0xFFFFFFFF, count, (u32)buf);
}

PUBLIC void hdWrite(u32 device, u64 position, u32 count, char* buf)
{
	system_call(NR_SYS_HD_WRITE, device, position & 0xFFFFFFFF, (position >> 32) & 0xFFFFFFFF, count, (u32)buf);
}

PUBLIC void hdIoctl(u32 device, u32 request, char* buf)
{
	system_call(NR_SYS_HD_IOCTL, device, request, (u32)buf, 0, 0);
}

