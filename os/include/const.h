#ifndef CONST_H
#define CONST_H

//EXTERN is defined as extern except in global.c
//extern 提示编译器在其他模块中寻找定义
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

//RPL
#define RPL_KRNL	SA_RPL0
#define RPL_TASK	SA_RPL1
#define RPL_USER	SA_RPL3

//8259A interrupt controller ports
#define INT_M_CTL	0x20	/* I/O port for master interrupt controller 	<Master> */
#define INT_M_CTLMASK	0x21	/* setting bits in this port disables ints 	<Master> */
#define INT_S_CTL	0xA0	/* I/O port for slave interrupt controller	<Slave> */
#define INT_S_CTLMASK	0xA1	/* setting bits in this port disables ints	<Slave> */

//8253/8254 PIT (Programmable Interval Timer)
#define TIMER0		0x40	/* I/O port for timer channel 0 */
#define TIMER_MODE	0x43	/* I/O port for timer mode control */
#define RATE_GENERATOR	0x34	/* 00-11-010-0:
				 * Counter0-LSB then MSB-rate generator-binary*/

#define TIMER_FREQ	1193182L/* clock frequency for timer in PC and AT */
#define HZ		100	/* clock frequenct (software settable on IBM-PC) */

/* Hardware interrupts */
#define	NR_IRQ		16	/* Number of IRQs */
#define	CLOCK_IRQ	0
#define	KEYBOARD_IRQ	1
#define	CASCADE_IRQ	2	/* cascade enable for 2nd AT controller */
#define	ETHER_IRQ	3	/* default ethernet interrupt vector */
#define	SECONDARY_IRQ	3	/* RS232 interrupt vector for port 2 */
#define	RS232_IRQ	4	/* RS232 interrupt vector for port 1 */
#define	XT_WINI_IRQ	5	/* xt winchester */
#define	FLOPPY_IRQ	6	/* floppy disk */
#define	PRINTER_IRQ	7
#define	AT_WINI_IRQ	14	/* at winchester */

/* tasks */
/* 注意 TASK_XXX 的定义要与 global.c 中对应 */
#define INVALID_DRIVER	-20
#define INTERRUPT	-10
#define TASK_TTY	0
#define TASK_SYS	1
#define TASK_HD		2

/* system call */
#define NR_SYS_CALL	9


#define		TRUE	1
#define		true	1
#define		FALSE	0
#define		false	0

/* AT keyboard */
/* 8042 ports */
#define KB_DATA		0x60	/* I/O port for keyboard data
					Read : Read Output Buffer
					Write: Write Input Buffer(8042 Data&8048 Command) */
#define KB_CMD		0x64	/* I/O port for keyboard command
					Read : Read Status Register
					Write: Write Input Buffer(8042 Command) */


#define	DIOCTL_GET_GEO	1

/* Hard Drive */
#define SECTOR_SIZE		512
#define SECTOR_BITS		(SECTOR_SIZE * 8)
#define SECTOR_SIZE_SHIFT	9

#define MAX_DRIVES		2
#define NR_PART_PER_DRIVE	4
#define NR_SUB_PER_PART		16
#define NR_SUB_PER_DRIVE	(NR_SUB_PER_PART * NR_PART_PER_DRIVE)
#define NR_PRIM_PER_DRIVE	(NR_PART_PER_DRIVE + 1)

#define MAX_PRIM		(MAX_DRIVES * NR_PRIM_PER_DRIVE - 1)
#define MAX_WUBPARTITIONS	(NR_SUB_PER_DRIVE * MAX_DRIVES)

/* device numbers of hard disk */
#define	MINOR_hd1a		0x10
#define	MINOR_hd2a		(MINOR_hd1a+NR_SUB_PER_PART)

#define	ROOT_DEV		MAKE_DEV(DEV_HD, MINOR_BOOT)

#define	P_PRIMARY	0
#define	P_EXTENDED	1

#define LESSOS_PART	0x99	/* LESSOS partition */
#define NO_PART		0x00	/* unused entry */
#define EXT_PART	0x05	/* extended partition */

/* major device numbers (corresponding to kernel/global.c::dd_map[]) */
#define	NO_DEV			0
#define	DEV_FLOPPY		1
#define	DEV_CDROM		2
#define	DEV_HD			3
#define	DEV_CHAR_TTY		4
#define	DEV_SCSI		5

/* make device number from major and minor numbers */
#define	MAJOR_SHIFT		8
#define	MAKE_DEV(a,b)		((a << MAJOR_SHIFT) | b)

/* separate major and minor numbers from device number */
#define	MAJOR(x)		((x >> MAJOR_SHIFT) & 0xFF)
#define	MINOR(x)		(x & 0xFF)

#endif /*CONST_H*/
