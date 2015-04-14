#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"

PUBLIC int kernel_main()
{
	disp_str("-----\"kernel_main\" begins-----\n");
	
	TASK*		p_task		= task_table;
	PROCESS* 	p_proc		= proc_table;
	char*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	u16		selector_ldt	= SELECTOR_LDT_FIRST;
	int i;

	for(i = 0; i < NR_TASKS; i++)
	{
		strcpy(p_proc->p_name, p_task->name);		//name of the process
		p_proc->pid = i;				//pid
		p_proc->ldt_sel = selector_ldt;

		//初始化进程表（PCB）中的LDT
		//LDT中第一个描述符为GDT中的KERNEL_CS (DESC_FLAT_C)
		//LDT中第二个描述符为GDT中的KERNEL_DS (DESC_FLAT_RW)
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_USER << 5;	//change the DPL to level 3
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_USER << 5;	//change the DPL to level 3
	
		//初始化进程表(PCB)中的寄存器纪录
		//其中CS DS采用LDT中的段，gs采用GDT中的段只是RPL发生改变
		//要处理Selector的TI和RPL使其满足要求
		p_proc->regs.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_USER;
		p_proc->regs.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_USER;
		p_proc->regs.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_USER;
		p_proc->regs.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_USER;
		p_proc->regs.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_USER;
		p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_USER;
	
		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = 0x1202;	//IF = 1, IOPL = 1; bit 2 is always 1.

		p_task_stack -= p_task->stacksize;
		p_proc++;				//指向下一个进程表
		p_task++;				//指向下一个任务
		selector_ldt += 1 << 3;			//在GDT增加一个LDT描述符
	}

	k_reenter = 0;
	ticks = 0;

	p_proc_ready	= proc_table;


	init_clock();
	init_keyboard();


	restart();					//跳转到准备好的进程中
	

	while(1){}
}
PRIVATE void test_disp(int i)
{
	disp_int(i);
	disp_str(" ");
}

void TestA()
{	
	int i = 0;
	while(1){
		//ComboKey kernelReadComboKey()使用示例
		ComboKey ck = kernelReadComboKey();
		bool* decorateKeyStatus[6] = {&(ck.ctrl_l_status), &(ck.ctrl_r_status), &(ck.alt_l_status), 
						&(ck.alt_r_status), &(ck.shift_l_status), &(ck.shift_r_status)};
		char* decorateOutput[6] = {"ctrlL ", "ctrlR ", "altL ", "altR ", "siftL ", "shiftR "};

		int i = 0;
		for(i = 0; i < 6; i++){
			if(*(decorateKeyStatus[i]) == VKEY_STATE_PRESSED){
				disp_str(decorateOutput[i]);
			}
		}
		
		char out[3] = {' ', ' ', '\0'};
		out[0] = ck.key;
		disp_str(out);

		//系统调用VirtualKey readKey()使用实例
//		char out[3] = {' ', ' ', '\0'};
//		out[0] = readKey();
//		disp_str(out);
//
//		milli_delay(10);
	}
}

void TestB()
{
	int i = 0x1000;
	while(1)
	{
		disp_str("B");
		disp_int(getTicks());
		disp_str(". ");
		readKey();
		milli_delay(1000);
	}
}
