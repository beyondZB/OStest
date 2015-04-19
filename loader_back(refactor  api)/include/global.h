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

EXTERN	int		ticks;

extern	PROCESS		proc_table[];
extern	char		task_stack[];
extern	TASK		task_table[];
extern	irq_handler	irq_table[];