#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "lofs.h"
#include "proc.h"
#include "tty.h"
#include "global.h"

//PCB表
PUBLIC	PROCESS	proc_table[NR_PROC];

//进程栈
PUBLIC	char	task_stack[STACK_SIZE_TOTAL];
//PUBLIC	char	task_stack[NR_PROC * 0x8000];/* 暂时写成这样，以后要改 */
//系统调用备份表，在系统调用中备份进程表
PUBLIC	char	task_kernel_stack[KERNEL_STACK_SIZE_TOTAL];
//PUBLIC	char	task_kernel_stack[NR_PROC * 0x8000];

//用于创建进程时读取需要的信息
PUBLIC	TASK	task_table[NR_PROC] = {{idle, STACK_SIZE_IDLE, KERNEL_STACK_SIZE_IDLE, "idle"},
					{TestA, STACK_SIZE_TESTA, KERNEL_STACK_SIZE_TESTA,"TestA"},
					{TestB, STACK_SIZE_TESTB, KERNEL_STACK_SIZE_TESTB,"TestB"},
					{(void*)USER_PROCESS_EIP, STACK_SIZE_USER, KERNEL_STACK_SIZE_USER, "UserProc"},
					{(void*)USER_PROCESS_EIP, STACK_SIZE_USER, KERNEL_STACK_SIZE_USER, "UserProc"}
					};

//中断处理处理程序表
PUBLIC	irq_handler	irq_table[NR_IRQ];



/*
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
*/
//系统调用处理程序表
PUBLIC	system_call_handler	sys_call_table[NR_SYS_CALL] = { sysGetTicks,
								sysReadKey, 
								sysHdOpen,
								sysBlock,
								sysUnBlock,
								sysHdClose,
								sysHdRead,
								sysHdWrite,
								sysHdIoctl,
								sysOpenFile, 
								sysFileRead,
								sysCloseFile,
								sysCreateProcess,
								sysExitProcess,
								sysTeletypeOutputStr,
								sysReadComboKey};
