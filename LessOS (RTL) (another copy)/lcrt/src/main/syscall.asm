
INT_VECTOR_SYS_CALL	equ 0x90

global	system_call

bits 32
[section .text]

;get_ticks:
;	mov	eax, _NR_get_ticks
;	int	INT_VECTOR_SYS_CALL
;	ret

;=================================================================================
;	int system_call(u32 eax, u32 edx,u32 esi, u32 edi, u32 ecx, u32 ebx)
;=================================================================================
;穿进来6个参数,用对应的寄存器保存之后,int到系统服务中断
system_call:
	push	ebp
	mov	ebp, esp

	;由于后面要用寄存器传递参数,所以先要保存寄存器(eax做返回值不用保存)
	push	ebx
	push	ecx
	push	edi
	push	esi
	push	edx

	;用寄存器传递参数
	mov	eax, [ebp + 8]
	mov	edx, [ebp + 12]
	mov	esi, [ebp + 16]
	mov	edi, [ebp + 20]
	mov	ecx, [ebp + 24]
	mov	ebx, [ebp + 28]
	
	int	INT_VECTOR_SYS_CALL
	

	pop	edx
	pop	esi
	pop	edi
	pop	ecx
	pop	ebx

	pop	ebp
	ret
