#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "lofs.h"
#include "proc.h"
#include "tty.h"
#include "global.h"


//================================================
//		sysGetTicks
//================================================
PUBLIC int sysGetTicks()
{
	return kernelGetTicks();
}


//================================================
//		kernelGetTicks
//================================================
PUBLIC int kernelGetTicks()
{
	return ticks;
}


//================================================
//		kernelBlock
//================================================
PUBLIC void kernelBlock()
{
//	disp_str(" kernelBlock ");
	asm("cli");
	p_proc_running->state = PROC_STATE_BLOCKING;
	kernelShift();
}


//================================================
//		kernelUnBlock	
//================================================
PUBLIC void kernelUnBlock(u32 pid)
{
	proc_table[pid].state = PROC_STATE_READY;
//	disp_str("unblocked@@\n");
}



PUBLIC int sysBlock()
{
	kernelBlock();
}

PUBLIC int sysUnBlock(u32 pid)
{
	kernelUnBlock(pid);
}

#define BS	0x100000
PUBLIC int kernelCreateProcess(const char* processName)
{
	asm("cli");
	//将用户进程文件放入内存适当位置
	int fd = kernelOpenFile(0, processName, DEV_OM_READ);
	char* buf = (char*)USER_PROCESS_POS;//将用户程序拷到0x800000处
	kernelFileRead(fd, buf, BS);
	
	//移动用户程序到指定位置
	u16 e_phnum = *((u16*)(buf + 44));
	u32 e_phoff = *((u32*)(buf + 28));
	char* phoff_pos = (char*)USER_PROCESS_POS + e_phoff;

	//根据elf的数据，将各段移动到内存中的正确位置
	int i = 0;
	for(i = 0; i < e_phnum; i++)
	{
		if(*((u32*)(phoff_pos)) == 0)//p_type == 0
			continue;
		u32 p_filesz = *((u32*)(phoff_pos + 16));	//段在文件中的长度
		u32 p_offset = *((u32*)(phoff_pos + 4));	//段的第一字节在文件中的偏移
		char* poff_pos = (char*)USER_PROCESS_POS + p_offset;	//段现在内存中的偏移
		u32 p_vaddr = *((u32*)(phoff_pos + 8));		//段应该放在内存中的虚拟地址
		memcpy((char*)p_vaddr, poff_pos, p_filesz);
		phoff_pos += 32;
	}

	//找到一个非法PCB（即空PCB），为其设值
	for(i = 0; i < NR_PROC; i++){
		if(proc_table[i].state == PROC_STATE_ILLEGAL)
			break;
	}
	if(i >= NR_PROC)
		return -1;	//创建进程失败

	
//初始化PCB
	strcpy(proc_table[i].p_name, processName);
	proc_table[i].fpid = p_proc_running - proc_table;
	proc_table[i].state = PROC_STATE_READY;
	proc_table[i].ldt_sel = (5 + i) * 8;//从gdt的第五个描述符开始为ldt描述符
	proc_table[i].ttyNo = 0;

	//初始化进程表（PCB）中的LDT
	//LDT中第一个描述符为GDT中的KERNEL_CS (DESC_FLAT_C)
	//LDT中第二个描述符为GDT中的KERNEL_DS (DESC_FLAT_RW)
	memcpy(&proc_table[i].ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
	proc_table[i].ldts[0].attr1 = DA_C | PRIVILEGE_USER << 5;	//change the DPL to level 3
	memcpy(&proc_table[i].ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
	proc_table[i].ldts[1].attr1 = DA_DRW | PRIVILEGE_USER << 5;	//change the DPL to level 3

	//初始化进程表(PCB)中的寄存器纪录
	//其中CS DS采用LDT中的段，gs采用GDT中的段只是RPL发生改变
	//要处理Selector的TI和RPL使其满足要求
	proc_table[i].regs.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_USER;
	proc_table[i].regs.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_USER;
	proc_table[i].regs.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_USER;
	proc_table[i].regs.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_USER;
	proc_table[i].regs.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_USER;
	proc_table[i].regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_USER;

	proc_table[i].regs.eip = USER_PROCESS_EIP;
	proc_table[i].regs.esp = 0x1500000;
	proc_table[i].regs.eflags = 0x3202;	//IF = 1, IOPL = 3; bit 2 is always 1.
	proc_table[i].regs.task_kernel_stack = (u32)(task_kernel_stack + KERNEL_STACK_SIZE_TOTAL - i * 0x8000);//非常不安全！！！
	proc_table[i].regs.task_regs_backup = (u32)&(proc_table[i].regs_backup);

	//初始化fdp_table
	int j = 0;
	for(j = 0; j < NR_FDP; j++)
		memset(&(proc_table[i].fdp_table[j]), 0, sizeof(FILE_DESCRIPTOR*));

	asm("sti");
}


PUBLIC u32 sysCreateProcess(const char* processName)
{
	return kernelCreateProcess(processName);
}


PUBLIC int kernelExitProcess()
{
	asm("cli");

	//将即将结束的进程PCB设置为无效进程块
	p_proc_running->state = PROC_STATE_ILLEGAL;
	kernelUnBlock(p_proc_running->fpid);
	kernelShift();

}

PUBLIC u32 sysExitProcess()
{
	return (u32)kernelExitProcess();
}

PUBLIC void* kernelUserMalloc(u32 num_bytes)
{
	return (void*)0x800000;
}

PUBLIC u32 sysUserMalloc(u32 num_bytes)
{
	return (u32)kernelUserMalloc(num_bytes);
}

PUBLIC void kernelUserFree(void* ptr)
{
	return;
}

PUBLIC u32 sysUserFree(void* ptr)
{
	kernelUserFree(ptr);
	return 0;
}

