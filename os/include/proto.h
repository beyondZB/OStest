

/* kliba.asm */
PUBLIC void	out_byte(u16 port, u8 value);
PUBLIC u8	in_byte(u16 port);
PUBLIC void	disp_str(char * info);
PUBLIC void	disp_color_str(char * info, int color);
PUBLIC void	port_read(u16 port, void* buf, int n);
PUBLIC void	port_write(u16 port, void* buf, int n);

/* protect.c */
PUBLIC void	init_prot();
PUBLIC u32	seg2phys(u16 seg);

/* klib.c */
PUBLIC void	delay(int time);

/* kernel.asm */
void restart();
PUBLIC	void	sys_call();		
void save();
void kernelShift();		//立即切换进程

/* main.c */
void idle();
void TestA();
void TestB();


/* i8259.c */
PUBLIC void spurious_irq(int irq);


/* clock.c */
PUBLIC void clock_handler(int irq); 
PUBLIC void milli_delay(int milli_sec);
PUBLIC void kernelSchedule();

/* 以下为系统调用相关 */
/* proc.c */
PUBLIC	int	kernelGetTicks();	
PUBLIC	int	sysGetTicks();		/* sys_call */
PUBLIC 	void	kernelBlock();		//阻塞当前进程
PUBLIC	int	sysBlock();
PUBLIC	int	sysUnBlock(u32 pid);
PUBLIC	void	kernelUnBlock(u32 pid);


/* syscall.asm */
;PUBLIC	int	get_ticks();
PUBLIC	int	system_call(u32 eax, u32 edx,u32 esi, u32 edi, u32 ecx, u32 ebx);



/* keyboard.c */
PUBLIC void keyboard_handler(int irq);
PUBLIC void init_keyboard();
PUBLIC int sysReadKey();

/* api.c */
PUBLIC int getTicks();
PUBLIC VirtualKey readKey();
PUBLIC void block();
PUBLIC void unBlock(u32 pid);

/* hd.c */
PUBLIC void init_hd();
PUBLIC void kernelHdIdentify(int drive);
PUBLIC int sysHdIdentify(int drive);

