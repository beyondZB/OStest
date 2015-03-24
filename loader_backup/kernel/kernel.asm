;在loader中定义的老的GDT中KERNEL所在的段的Selector
SELECTOR_KERNEL_CS	equ	8

;导入cstart函数
extern	cstart

;导入全局变量gdt_ptr(全局描述符表指针)
extern gdt_ptr

[SECTION .bss]
StackSpace	resb	2 * 1024
StackTop:	;栈顶

[SECTION .text]	;代码

global _start	;导出_start(程序入口)

_start:


	;把esp从LOADER挪到KERNEL
	mov	esp, StackTop	;堆栈在bss段中
	
	sgdt	[gdt_ptr]	;cstart()中将会用到gdt_ptr
	call	cstart		;在次函数中改变了gdt_ptr,让他指向新的GDT
	lgdt	[gdt_ptr]	;将cstart()函数产生的新的GDT载入GDTR

	;lidt	[idt_ptr]

	jmp	SELECTOR_KERNEL_CS:csinit
csinit:		;"这个跳转指令强制使用刚刚初始化的结构"
	
	push	0
	popfd	;pop top of stack into EFLAGS(即将EFLAG清零)

	hlt

