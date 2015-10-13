#define GLOBAL_VARIABLES_HERE

#include "protect.h"
#include "proto.h"
#include "proc.h"
#include "global.h"

//PCB表
PUBLIC	PROCESS	proc_table[NR_TASKS];

//进程栈
PUBLIC	char	task_stack[STACK_SIZE_TOTAL];
//系统调用备份表，在系统调用中备份进程表
PUBLIC	char	task_kernel_stack[KERNEL_STACK_SIZE_TOTAL];

//用于创建进程时读取需要的信息
PUBLIC	TASK	task_table[NR_TASKS] = {{idle, STACK_SIZE_IDLE, KERNEL_STACK_SIZE_IDLE, "idle"},
					{TestA, STACK_SIZE_TESTA, KERNEL_STACK_SIZE_TESTA,"TestA"},
					{TestB, STACK_SIZE_TESTB, KERNEL_STACK_SIZE_TESTB,"TestB"}};

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
								sysHdIoctl};
