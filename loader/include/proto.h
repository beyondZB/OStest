/* klib.asm */
PUBLIC void	out_byte(u16 port, u8 value);
PUBLIC u8	in_byte(u16 port);
PUBLIC void	disp_str(char * info);
PUBLIC void	disp_color_str(char * info, int color);

/* protect.c */
PUBLIC void	init_prot();
PUBLIC u32	seg2phys(u16 seg);

/* klib.c */
PUBLIC void	delay(int time);

/* kernel.asm */
void restart();

/* main.c */
void TestA();
void TestB();


/* i8259.c */
PUBLIC void spurious_irq(int irq);


/* clock.c */
PUBLIC void clock_handler(int irq); 
PUBLIC void milli_delay(int milli_sec);

/* 以下为系统调用相关 */
/* proc.c */
PUBLIC	int	sys_get_ticks();	/* sys_call */

/* syscall.asm */
PUBLIC	void	sys_call();		/* int_handler */
PUBLIC	int	get_ticks();
PUBLIC	int	system_call(u32 eax, u32 edx,u32 esi, u32 edi, u32 ecx, u32 ebx);



/* keyboard.c */
PUBLIC void keyboard_handler(int irq);
PUBLIC void init_keyboard();
