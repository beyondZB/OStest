#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "lofs.h"
#include "proc.h"
#include "tty.h"
#include "global.h"


PUBLIC void clock_handler(int irq)
{
//	disp_str("#");
	ticks++;
	if(k_reenter != 0){
//		disp_str("!");
		return;
	}

	p_proc_running->state = PROC_STATE_READY;
	kernelSchedule();
}

PUBLIC void kernelSchedule()
{
	//disp_int((void*)kernelSchedule);
	do{
		p_proc_ready++;
		if(p_proc_ready >= proc_table + NR_PROC){
			p_proc_ready = proc_table;
		}
	}while(p_proc_ready->state != PROC_STATE_READY);
	
//	TeletypeOutputStr(" ");
//	TeletypeOutputStr(p_proc_ready->p_name);
//	TeletypeOutputStr(" ");
/* old process dispatch */
//	p_proc_ready++;
//	if (p_proc_ready >= proc_table + NR_TASKS)
//		p_proc_ready = proc_table;
}

PUBLIC void milli_delay(int milli_sec)
{
	int t = kernelGetTicks();

	while(((kernelGetTicks() - t) * 1000 / HZ) < milli_sec) {}
}

PUBLIC void init_clock()
{
        /* 初始化 8253 PIT */
        out_byte(TIMER_MODE, RATE_GENERATOR);
        out_byte(TIMER0, (u8) (TIMER_FREQ/HZ) );
        out_byte(TIMER0, (u8) ((TIMER_FREQ/HZ) >> 8));

        put_irq_handler(CLOCK_IRQ, clock_handler);    /* 设定时钟中断处理程序 */
        enable_irq(CLOCK_IRQ);                        /* 让8259A可以接收时钟中断 */
}
