#ifndef _TYPE_H_
#define _TYPE_H_

typedef	unsigned long long u64;
typedef unsigned int	u32;
typedef unsigned short	u16;
typedef unsigned char	u8;
typedef	u8		VirtualKey;
typedef u8		bool;

typedef void (*int_handler) ();
typedef void (*task_f) ();
typedef void (*irq_handler) (int irq);
typedef void* system_call_handler;	//任意类型的函数指针
#endif /*_TYPE_H_*/
