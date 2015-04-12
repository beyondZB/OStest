#ifndef _LESSOS_TYPE_H_
#define _LESSOS_TYPE_H_

typedef unsigned int	u32;
typedef unsigned short	u16;
typedef unsigned char	u8;

typedef void (*int_handler) ();
typedef void (*task_f) ();
typedef void (*irq_handler) (int irq);
typedef void* system_call_handler;	//任意类型的函数指针
#endif /*_LESSOS_TYPE_H_*/
