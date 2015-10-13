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

#define BS	10000
PUBLIC void kernelCreateProcess(const char* processName)
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
	proc_table[i].state = PROC_STATE_READY;
	asm("sti");
}

PUBLIC u32 sysCreateProcess(const char* processName)
{
	kernelCreateProcess(processName);
	return 0;
}


PUBLIC int kernelExitProcess()
{
	asm("cli");

	//将即将结束的进程PCB设置为无效进程块
	p_proc_running->state = PROC_STATE_ILLEGAL;
	kernelShift();

}

PUBLIC u32 sysExitProcess()
{
	return (u32)kernelExitProcess();
}
