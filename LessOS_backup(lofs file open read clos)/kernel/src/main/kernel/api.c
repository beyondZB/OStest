#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "lofs.h"
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
#define NR_SYS_FILE_OPEN	9
#define NR_SYS_FILE_READ	10
#define NR_SYS_FILE_CLOSE	11


PUBLIC int getTicks()
{
	disp_color_str("@",0x3F);
	return system_call(NR_SYS_GET_TICKS, 0, 0, 0, 0, 0);
}

PUBLIC VirtualKey readKey(){
	disp_color_str("api read key@@@@", 0x3F);
	return (VirtualKey)( system_call(NR_SYS_READ_KEY, 0, 0, 0, 0, 0) );
}

PUBLIC struct hd_info* hdOpen(u32 device)
{
	disp_color_str("api hd open@@@@", 0x3F);
	return (struct hd_info*)system_call(NR_SYS_HD_OPEN, device, 0, 0, 0, 0);
}

PUBLIC void block()
{
	disp_color_str("api block@@@@", 0x3F);
	system_call(NR_SYS_BLOCK, 0, 0, 0, 0, 0);
}

PUBLIC void unBlock(u32 pid)
{
	disp_color_str("api unblock@@@@", 0x3F);
	system_call(NR_SYS_UNBLOCK, pid, 0, 0, 0, 0);
}

PUBLIC void hdClose(u32 device)
{
	disp_color_str("hd close@@@@", 0x3F);
	system_call(NR_SYS_HD_CLOSE, device, 0, 0, 0, 0);
}

PUBLIC void hdRead(u32 device, u64 position, u32 count, char* buf)
{
	disp_color_str("api hd rads@@@@", 0x3F);
	system_call(NR_SYS_HD_READ, device, position & 0xFFFFFFFF, (position >> 32) & 0xFFFFFFFF, count, (u32)buf);
}

PUBLIC void hdWrite(u32 device, u64 position, u32 count, char* buf)
{
	disp_color_str("api hd write@@@@", 0x3F);
	system_call(NR_SYS_HD_WRITE, device, position & 0xFFFFFFFF, (position >> 32) & 0xFFFFFFFF, count, (u32)buf);
}

PUBLIC void hdIoctl(u32 device, u32 request, char* buf)
{
	disp_color_str("api hd ioctl@@@@", 0x3F);
	system_call(NR_SYS_HD_IOCTL, device, request, (u32)buf, 0, 0);
}

PUBLIC int fileOpen(int deviceNo, const char* path, FILE_MODE mode)
{
	disp_color_str("api file Open@@@@@", 0x3F);
	return system_call(NR_SYS_FILE_OPEN, deviceNo, (u32)path, mode, 0, 0);
}

PUBLIC int fileRead(int fd, char* buf, int size)
{
	disp_color_str("api file read@@@@@", 0x3F);
	return system_call(NR_SYS_FILE_READ, fd, (u32)buf, size, 0, 0);
}

PUBLIC void fileClose(int fd)
{
	disp_color_str("api file close@@@@@", 0x3F);
	system_call(NR_SYS_FILE_CLOSE, (u32)fd, 0, 0, 0, 0);
}

