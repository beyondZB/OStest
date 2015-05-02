#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"

PUBLIC	PROCESS	proc_table[NR_TASKS];

PUBLIC	char	task_stack[STACK_SIZE_TOTAL];
PUBLIC	char	task_kernel_stack[KERNEL_STACK_SIZE_TOTAL];

PUBLIC	TASK	task_table[NR_TASKS] = {{idle, STACK_SIZE_IDLE, KERNEL_STACK_SIZE_IDLE, "idle"},
					{TestA, STACK_SIZE_TESTA, KERNEL_STACK_SIZE_TESTA,"TestA"},
					{TestB, STACK_SIZE_TESTB, KERNEL_STACK_SIZE_TESTB,"TestB"}};

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
*/
PUBLIC	system_call_handler	sys_call_table[NR_SYS_CALL] = { sysGetTicks,
								sysReadKey, 
								sysHdOpen,
								sysBlock,
								sysUnBlock,
								sysHdClose,
								sysHdRead,
								sysHdWrite,
								sysHdIoctl};
