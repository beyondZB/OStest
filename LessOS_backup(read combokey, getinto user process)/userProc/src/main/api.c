#include "api.h"

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
#define NR_SYS_CREATE_PROCESS	12
#define NR_SYS_EXIT_PROCESS	13
#define NR_SYS_TTY_OUTPUT_STR	14

void createProcess(const char* processName)
{
	system_call(NR_SYS_CREATE_PROCESS, (u32)processName, 0, 0, 0, 0);
}

int exitProcess()
{
	return system_call(NR_SYS_EXIT_PROCESS, 0, 0, 0, 0, 0);
}

void  ttyOutputStr(const char* str)
{
	system_call(NR_SYS_TTY_OUTPUT_STR, (u32)str, 0, 0, 0, 0);
}
