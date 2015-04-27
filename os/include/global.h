//global.c中有#define GLOBAL_VARIABLES_HERE,这是EXTERN会消失,应为这些变量本身就定义在global.c这个模块中,不用到其他模块寻找定义
//而在其他.c文件中#include "global.h"时,EXTERN是存在的,这会提示编译器到其他地方寻找定义,也就是找到global这个模块中.
#ifdef	GLOBAL_VARIABLES_HERE
#undef	EXTERN
#define	EXTERN
#endif

EXTERN	int		disp_pos;
EXTERN	int		k_reenter;
EXTERN	u8		gdt_ptr[6];	/* 0~15:Limit 16~47:Base */
EXTERN	DESCRIPTOR	gdt[GDT_SIZE];
EXTERN	u8		idt_ptr[6];	/* 0~15:Limit 16~47:Base */
EXTERN	GATE		idt[IDT_SIZE];

EXTERN	TSS		tss;
EXTERN	PROCESS*	p_proc_ready;
EXTERN	PROCESS*	p_proc_running;

EXTERN	int		ticks;

extern	PROCESS		proc_table[];
extern	char		task_stack[];
extern	char		task_kernel_stack[];
extern	TASK		task_table[];
extern	irq_handler	irq_table[];

EXTERN	int		readKeyBlockEip;
EXTERN	int		hdIdentifyBlockEip;
EXTERN	int		hdGetPartTableBlockEip;
