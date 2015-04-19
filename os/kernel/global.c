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

PUBLIC	TASK	task_table[NR_TASKS] = {{TestA, STACK_SIZE_TESTA, KERNEL_STACK_SIZE_TESTA,"TestA"},
					{TestB, STACK_SIZE_TESTB, KERNEL_STACK_SIZE_TESTB,"TestB"}};

PUBLIC	irq_handler	irq_table[NR_IRQ];

PUBLIC	system_call_handler	sys_call_table[NR_SYS_CALL] = { sysGetTicks,
								sysReadKey, 
								sysHdIdentify,
								sysBlock,
								sysUnBlock};

