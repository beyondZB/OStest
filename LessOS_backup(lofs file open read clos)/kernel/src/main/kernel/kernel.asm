%include "sconst.inc"

;导入函数
extern	cstart
extern	exception_handler
extern	spurious_irq
extern	kernel_main
extern	disp_str
extern	delay
extern	clock_handler
extern	kernelSchedule
extern	disp_int


;导入全局变量gdt_ptr(全局描述符表指针)
extern gdt_ptr
;导入全局变量idt_ptr(中断描述符表)
extern idt_ptr
extern p_proc_ready
extern p_proc_running
extern tss
extern disp_pos
extern k_reenter
extern irq_table
extern sys_call_table


bits	32

[SECTION .bss]
StackSpace	resb	2 * 1024
StackTop:	;栈顶

[SECTION .text]	;代码

global _start	;导出_start(程序入口)
global restart
global save
global kernelShift

;中断处理函数
global divide_error
global single_step_exception
global nmi
global breakpoint_exception
global overflow
global bounds_check
global inval_opcode
global copr_not_available
global double_fault
global copr_seg_overrun
global inval_tss
global segment_not_present
global stack_exception
global general_protection
global page_fault
global copr_error
global sys_call

;硬件中断HardWare Interruption
global  hwint00
global  hwint01
global  hwint02
global  hwint03
global  hwint04
global  hwint05
global  hwint06
global  hwint07
global  hwint08
global  hwint09
global  hwint10
global  hwint11
global  hwint12
global  hwint13
global  hwint14
global  hwint15


_start:


	;把esp从LOADER挪到KERNEL
	mov	esp, StackTop	;堆栈在bss段中

	mov	dword [disp_pos], 0
	
	sgdt	[gdt_ptr]	;cstart()中将会用到gdt_ptr
	call	cstart		;在次函数中改变了gdt_ptr,让他指向新的GDT
	lgdt	[gdt_ptr]	;将cstart()函数产生的新的GDT载入GDTR

	lidt	[idt_ptr]

	jmp	SELECTOR_KERNEL_CS:csinit
csinit:		;"这个跳转指令强制使用刚刚初始化的结构"
	
	xor	eax, eax
	mov	ax, SELECTOR_TSS
	ltr	ax


	;sti
	jmp	kernel_main

	;hlt




; 中断和异常 -- 硬件中断
; ---------------------------------
%macro  hwint_master    1
	call	save

	in	al, INT_M_CTLMASK	;'.
	or	al, (1 << %1)		; |屏蔽当前中断
	out	INT_M_CTLMASK, al	;/
	
	mov	al, EOI			;'.置EOI位
	out	INT_M_CTL, al		;/

	sti	;CPU在响应中断的过程中会自动关闭中断,这句之后就允许响应新的中断-------
	push	%1
	call	[irq_table + 4 * %1]
	pop	ecx
	cli	;关中断---------------------------------------------------------------

	in	al, INT_M_CTLMASK
	and	al, ~(1 << %1)
	out	INT_M_CTLMASK, al
	ret
%endmacro
; ---------------------------------

ALIGN   16
hwint00:                ; Interrupt routine for irq 0 (the clock).
	;inc	byte [gs:0]
	hwint_master	0

ALIGN   16
hwint01:                ; Interrupt routine for irq 1 (keyboard)
        hwint_master    1

ALIGN   16
hwint02:                ; Interrupt routine for irq 2 (cascade!)
        hwint_master    2

ALIGN   16
hwint03:                ; Interrupt routine for irq 3 (second serial)
        hwint_master    3

ALIGN   16
hwint04:                ; Interrupt routine for irq 4 (first serial)
        hwint_master    4

ALIGN   16
hwint05:                ; Interrupt routine for irq 5 (XT winchester)
        hwint_master    5

ALIGN   16
hwint06:                ; Interrupt routine for irq 6 (floppy)
        hwint_master    6

ALIGN   16
hwint07:                ; Interrupt routine for irq 7 (printer)
        hwint_master    7

; ---------------------------------
%macro  hwint_slave     1
	call	save

	in	al, INT_S_CTLMASK	;'.
	or	al, (1 << (%1 - 8))	; |屏蔽当前中断
	out	INT_S_CTLMASK, al	;/

	mov	al, EOI			;置EOI位(master)
	out	INT_M_CTL, al
	
	nop
	out	INT_S_CTL, al		;置EOI位(slave)
	;一定要注意:slave和master都要置EOI

	sti	;CPU在响应中断的过程中会自动关闭中断,这句之后就允许响应新的中断-----------------------

	push	%1			;'.
	call	[irq_table + 4 * %1]	; |中断处理程序
	pop	ecx			;/

	cli	;关中断-------------------------------------------------------------------------------

	in	al, INT_S_CTLMASK	;'.
	and	al, ~(1 << (%1 - 8))	; |回复接受当前中断
	out	INT_S_CTLMASK, al	;/

	ret
%endmacro
; ---------------------------------

ALIGN   16
hwint08:                ; Interrupt routine for irq 8 (realtime clock).
        hwint_slave     8

ALIGN   16
hwint09:                ; Interrupt routine for irq 9 (irq 2 redirected)
        hwint_slave     9

ALIGN   16
hwint10:                ; Interrupt routine for irq 10
        hwint_slave     10

ALIGN   16
hwint11:                ; Interrupt routine for irq 11
        hwint_slave     11

ALIGN   16
hwint12:                ; Interrupt routine for irq 12
        hwint_slave     12

ALIGN   16
hwint13:                ; Interrupt routine for irq 13 (FPU exception)
        hwint_slave     13

ALIGN   16
hwint14:                ; Interrupt routine for irq 14 (AT winchester)
        hwint_slave     14

ALIGN   16
hwint15:                ; Interrupt routine for irq 15
        hwint_slave     15


; 中断和异常
divide_error:
	push	0xFFFFFFFF	; no err code
	push	0		; vector_no	= 0
	jmp	exception
single_step_exception:
	push	0xFFFFFFFF	; no err code
	push	1		; vector_no	= 1
	jmp	exception
nmi:
	push	0xFFFFFFFF	; no err code
	push	2		; vector_no	= 2
	jmp	exception
breakpoint_exception:
	push	0xFFFFFFFF	; no err code
	push	3		; vector_no	= 3
	jmp	exception
overflow:
	push	0xFFFFFFFF	; no err code
	push	4		; vector_no	= 4
	jmp	exception
bounds_check:
	push	0xFFFFFFFF	; no err code
	push	5		; vector_no	= 5
	jmp	exception
inval_opcode:
	push	0xFFFFFFFF	; no err code
	push	6		; vector_no	= 6
	jmp	exception
copr_not_available:
	push	0xFFFFFFFF	; no err code
	push	7		; vector_no	= 7
	jmp	exception
double_fault:
	push	8		; vector_no	= 8
	jmp	exception
copr_seg_overrun:
	push	0xFFFFFFFF	; no err code
	push	9		; vector_no	= 9
	jmp	exception
inval_tss:
	push	10		; vector_no	= A
	jmp	exception
segment_not_present:
	push	11		; vector_no	= B
	jmp	exception
stack_exception:
	push	12		; vector_no	= C
	jmp	exception
general_protection:
	push	13		; vector_no	= D
	jmp	exception
page_fault:
	push	14		; vector_no	= E
	jmp	exception
copr_error:
	push	0xFFFFFFFF	; no err code
	push	16		; vector_no	= 10h
	jmp	exception

exception:
	call	exception_handler
	add	esp, 4 * 2	;让栈顶指向EIP,堆栈中从顶向下依次是:EIP,CS,EFLAGS
	hlt

;==========================================================
;		save
;==========================================================
save:
	pushad		;'.
	push	ds	; |
	push	es	; |保存原寄存器值
	push	fs	; |这里也就是在保存进程表中的寄存器信息
	push	gs	;/
	mov	si, ss	
	mov	ds, si	
	mov	es, si	

	mov	esi, esp	;esi = 进程表的起始地址

	inc	dword [k_reenter]		;k_reenter++;
	cmp	dword [k_reenter], 0		;if(k_reenter == 0)
	jne	.1				;{
	mov	esp, StackTop			;	切换到内核栈

	push	restart				;	push	restart
	jmp	[esi + RETADR - P_STACKBASE]	;	return

.1:						;}else{ 已经在内核栈中,不需要切换
	push	restart_reenter			;	push	restart_reenter
	jmp	[esi + RETADR - P_STACKBASE]	;	return
						;}

;==========================================================
;		restart
;==========================================================
restart:					;若不是重入中断则切换进程
;	push	dword restart
;	call	disp_int
;	sub	esp, 4
	mov	esp, [p_proc_ready]		;p_proc_ready在global.h声明,存放就绪进程的进程表地址,这条语句将esp转到就绪进程的进程表
	mov	[p_proc_running], esp		;接下来就绪进程将要运行,所以现在运行进程指针p_proc_running指向就绪进程
	lldt	[esp + P_LDT_SEL]		;载入进程表中的ldt选择子
	lea	eax, [esp + P_STACKTOP]		
	mov	dword [tss + TSS3_S_SP0], eax	;将tts中的sp0设置为就绪进程进程表的最高地址,下次进程进入内核态的时候esp就会指向这个进程表

restart_reenter:				;若是重入终端则不切换进程
	dec	dword [k_reenter]
	pop	gs	;'.
	pop	fs	; |
	pop	es	; |恢复寄存器值
	pop	ds	; |
	popad		;/

	add	esp, 4	;跳过指向retstart或restart_reenter的指针

	iretd				;跳转到指定进程


;==========================================================
;		kernelShift
;==========================================================
kernelShift:
	cli;------------------------------------关中断------------------------------------------------------------
	push	eax		;保存eax的值
	mov	eax, [esp + 4]	;取得返回地址
	mov	[dest], eax	;保存返回地址
	pop	eax		;恢复eax的值
	add	esp, 4		;由于kernelBlock之后会进行进程切换并不会返回,为了保证堆栈正确,将eip从栈中去除

	;写进程表,保留现场
	mov	[tempebx], ebx
	mov	ebx, [p_proc_running]

	mov	[tempeax], eax	;保存eax的值
	mov	ax, gs
	mov	[ebx + GSREG], ax
	mov	ax, fs
	mov	[ebx + FSREG], ax
	mov	ax, es
	mov	[ebx + ESREG], ax
	mov	ax, ds
	mov	[ebx + DSREG], ax
	mov	[ebx + EDIREG], edi
	mov	[ebx + ESIREG], esi
	mov	[ebx + EBPREG], ebp
	lea	eax, [ebx + RETADR]
	mov	[ebx + KERNELESPREG], eax	;指向进程表中的RETADR位置
	mov	eax, [tempebx]			;取出ebx的值到eax中
	mov	[ebx + EBXREG], eax
	mov	[ebx + EDXREG], edx
	mov	[ebx + ECXREG], ecx
	mov	eax, [tempeax]
	mov	[ebx + EAXREG], eax
	mov	dword [ebx + RETADR], 0		;用不到
	mov	eax, [dest]
	mov	[ebx + EIPREG], eax
	mov	ax, cs
	mov	[ebx + CSREG], ax
	;取得eflags的值
	pushfd
	pop	eax
	mov	[ebx + EFLAGSREG], eax
	mov	[ebx + ESPREG], esp
	mov	eax, ss
	mov	[ebx + SSREG], eax

	call	kernelSchedule	;选择将要调度的进程

	;切换进程(不用restart的原因是restart中会dec[p_reenter])
	mov	esp, [p_proc_ready]		;p_proc_ready在global.h声明,存放就绪进程的进程表地址,这条语句将esp转到就绪进程的进程表
	mov	[p_proc_running], esp		;接下来就绪进程将要运行,所以现在运行进程指针p_proc_running指向就绪进程
	lldt	[esp + P_LDT_SEL]		;载入进程表中的ldt选择子
	lea	eax, [esp + P_STACKTOP]		
	mov	dword [tss + TSS3_S_SP0], eax	;将tts中的sp0设置为就绪进程进程表的最高地址,下次进程进入内核态的时候esp就会指向这个进程表

	pop	gs	;'.
	pop	fs	; |
	pop	es	; |恢复寄存器值
	pop	ds	; |
	popad		;/

	add	esp, 4	;跳过指向retstart或restart_reenter的指针

	;下面这条sti实际上是没有意义的,因为实际上iretd回恢复eflags,这是IF会被覆盖
	;sti;-----------------由于不是中断处理,cpu不会在退出时,自己开中断,最后时刻开中断,之后好让中断可以unblock已经block的进程--------------
	iretd				;跳转到指定进程


;---------------用于零时存储值------------------------------------------
	dest	dd	0	;用于存储返回地址,用于给进程表的eip赋值
	tempeax	dd	0
	tempebx	dd	0
;-----------------------------------------------------------------------



;================================================================================================
;				START	sys_call
;================================================================================================

;-------------------------------------------------
;copy %1 byte from ds:esi to es:edi
%macro	blockcpy	1
	push	ecx
	push	eax
	push	esi
	push	edi
	pushfd

	mov	ecx, %1
	cld

	rep 	movsb

	popfd
	pop	edi
	pop	esi
	pop	eax
	pop	ecx
%endmacro
;--------------------------------------------------

sys_call:	;syscall现在不调用save,因为syscall不应该处理重入问题,且应该将栈切换到进程内核栈
	sub	esp, 4	;跳过retaddr
	pushad		;'.
	push	ds	; |
	push	es	; |保存原寄存器值
	push	fs	; |这里也就是在保存进程表中的寄存器信息
	push	gs	;/
	mov	[tempesi], esi
	mov	si, ss	
	mov	ds, si	
	mov	es, si	

	mov	esi, esp	;esi = 进程表的起始地址

	mov	esp, [esi + TASKKERNELSTACK - P_STACKBASE]	;切换到内核栈
;以上相当于save的内容
	;将进程表中的内容保存到进程表备份中
	push	edi
	mov	edi, [esi + TASKREGSBACKUP - P_STACKBASE]
	
	blockcpy	TASKREGSBACKUP - P_STACKBASE + 4

	pop	edi
;以上保存进程现场,即写进程表和备份进程表

	;sti;-----------------------------开中断----------------------------------------
	;穿参数给相应的系统调用
	push	ebx
	push	ecx
	push	edi
	push	dword [tempesi]
	push	edx	

	call	[sys_call_table + eax * 4]

	add	esp, 20					;恢复堆栈

	
	cli;----------------------------关中断-----------------------------------------

;以下退出系统调用中断,恢复进程表,并依据进程表,还原现场
	mov	edi, [p_proc_running]				
	mov	[edi + EAXREG - P_STACKBASE], eax	;将存放返回值的eax存入进程表中,以便恢复进程的时候eax中存放着正确的返回值
;	mov	[edi + TASKKERNELSTACK - P_STACKBASE], esp	;将进程内核栈的指针esp保存起来
	mov	esi, [edi + TASKREGSBACKUP - P_STACKBASE]
;	mov	[esi + TASKKERNELSTACK - P_STACKBASE],	esp	;将进程内核栈的指针保存到备份进程表
	mov	[esi + EAXREG - P_STACKBASE], eax

	blockcpy	TASKREGSBACKUP - P_STACKBASE + 4

;以下相当于restart的内容
	mov	esp, [p_proc_running]

	pop	gs	;'.
	pop	fs	; |
	pop	es	; |恢复寄存器值
	pop	ds	; |
	popad		;/

	add	esp, 4	;跳过指向retstart或restart_reenter的指针

	iretd				;跳转到指定进程
;-------------------由于堆栈变化,临时存储寄存器esi的值---------------------
	tempesi	dd	0
	sysCallStr db	"sys_call@@@@@@@@@",0
;--------------------------------------------------------------------------
;==============================================================================================================
;					END	sys_call
;==============================================================================================================
