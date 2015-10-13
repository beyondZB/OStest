#ifndef _TYPE_H_
#define _TYPE_H_

typedef signed char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef	unsigned long long u64;
typedef unsigned int	u32;
typedef unsigned short	u16;
typedef unsigned char	u8;
typedef	u8		VirtualKey;
typedef u8		bool;

typedef i64 Size;

typedef struct {
    i32 low_part;
    i32 high_part;
} i64_parts;

typedef struct {
    u32 low_part;
    u32 high_parts;
} u64_parts;

typedef void (*int_handler) ();
typedef void (*task_f) ();
typedef void (*irq_handler) (int irq);
typedef void* system_call_handler;	//任意类型的函数指针
#endif /*_TYPE_H_*/
