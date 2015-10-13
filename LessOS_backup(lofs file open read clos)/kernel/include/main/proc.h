#ifndef _PROC_H
#define _PROC_H

#include "type.h"
#include "const.h"
typedef struct s_stackframe {	/* proc_ptr points here				↑ Low			*/
	u32	gs;		/* ┓stackbase					│			*/
	u32	fs;		/* ┃						│			*/
	u32	es;		/* ┃						│			*/
	u32	ds;		/* ┃						│			*/
	u32	edi;		/* ┃						│			*/
	u32	esi;		/* ┣ pushed by save()				│			*/
	u32	ebp;		/* ┃						│			*/
	u32	kernel_esp;	/* <- 'popad' will ignore it			│			*/
	u32	ebx;		/* ┃						↑栈从高地址往低地址增长*/		
	u32	edx;		/* ┃						│			*/
	u32	ecx;		/* ┃						│			*/
	u32	eax;		/* ┛						│			*/
	u32	retaddr;	/* return address for assembly code save()	│			*/
	u32	eip;		/*  ┓						│			*/
	u32	cs;		/*  ┃						│			*/
	u32	eflags;		/*  ┣ these are pushed by CPU during interrupt	│			*/
	u32	esp;		/*  ┃						│			*/
	u32	ss;		/*  ┛stacktop					┷High			*/
	u32	task_kernel_stack;
	u32	task_regs_backup;
}STACK_FRAME;

#define NR_FDP	10

/* s_proc也就是我们常说的进程表或者PCB,保存在堆栈中
   内容包括保存在栈中的寄存器,gdt中的LDT选择子,LDT中的选择子,ticks，优先级，pid,进程名*/
typedef struct s_proc {
	STACK_FRAME regs;          /* process registers saved in stack frame */

	u16 ldt_sel;               /* gdt selector giving ldt base and limit */
	DESCRIPTOR ldts[LDT_SIZE]; /* local descriptors for code and data */
	STACK_FRAME regs_backup;   /* backup for system call regs backing up*/
	FILE_DESCRIPTOR* fdp_table[NR_FDP]; /* file descriptor pointer table */

	u32 pid;                   /* process id passed in from MM */
	char p_name[16];           /* name of the process */
	u8 state;		   /* the state of process.Three value: PROC_STATE_RUNNING, PROC_STATE_READY, PROC_STATE_BLOCKING */
}PROCESS;

typedef struct s_task{
	task_f	initial_eip;
	int	stacksize;
	int	kernelStackSize;
	char	name[32];
}TASK;

//state value
#define PROC_STATE_RUNNING	0
#define PROC_STATE_READY	1
#define PROC_STATE_BLOCKING	2

// number of tasks
#define	NR_TASKS	3

/* stacks of tasks */
#define STACK_SIZE_IDLE		0x8000
#define STACK_SIZE_TESTA	0x8000
#define STACK_SIZE_TESTB	0x8000
#define KERNEL_STACK_SIZE_IDLE	0x8000
#define KERNEL_STACK_SIZE_TESTA	0x8000
#define KERNEL_STACK_SIZE_TESTB	0x8000

#define STACK_SIZE_TOTAL	(STACK_SIZE_IDLE + \
				STACK_SIZE_TESTA + \
				STACK_SIZE_TESTB)

#define KERNEL_STACK_SIZE_TOTAL	(KERNEL_STACK_SIZE_IDLE + \
				KERNEL_STACK_SIZE_TESTA + \
				KERNEL_STACK_SIZE_TESTB)

#endif	//_PROC_H
