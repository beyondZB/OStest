#ifndef _LESSOS_CONST_H_
#define _LESSOS_CONST_H_

//EXTERN is defined as extern except in global.c
#define EXTERN extern

//函数类型
#define PUBLIC		
#define PRIVATE static

//GDT和IDT中描述符的个数
#define GDT_SIZE 128
#define IDT_SIZE 256

//权限设置
#define PRIVILEGE_KRNL	0
#define PRIVILEGE_TASK	1
#define PRIVILEGE_USER	3

//829A interrupt controller ports
#define INT_M_CTL	0x20	/* I/O port for master interrupt controller 	<Master> */
#define INT_M_CTLMASK	0x21	/* setting bits in this port disables ints 	<Master> */
#define INT_S_CTL	0xA0	/* I/O port for slave interrupt controller	<Slave> */
#define INT_S_CTLMASK	0xA1	/* setting bits in this port disables ints	<Slave> */


#endif /*_LESSOS_CONST_H_*/