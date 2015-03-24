	.file	"findLoader.c"
	.section	.rodata
	.align 8
.LC0:
	.string	"usage: findLoder <the device> <the file you want to find and copy in the img> <file name you want to copy to on your host>"
.LC1:
	.string	"rb"
.LC2:
	.string	"%s has been copied to %s."
.LC3:
	.string	"%s is not exist.\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movl	%edi, -100(%rbp)
	movq	%rsi, -112(%rbp)
	cmpl	$4, -100(%rbp)
	je	.L2
	movl	$.LC0, %edi
	call	puts
	movl	$1, %eax
	jmp	.L6
.L2:
	movq	-112(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movl	$.LC1, %esi
	movq	%rax, %rdi
	call	fopen
	movq	%rax, -88(%rbp)
	leaq	-80(%rbp), %rdx
	movq	-88(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	getMetaData
	movw	$0, -90(%rbp)
	movq	-112(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rdx
	leaq	-90(%rbp), %rcx
	leaq	-80(%rbp), %rsi
	movq	-88(%rbp), %rax
	movq	%rax, %rdi
	call	findFile
	testb	%al, %al
	je	.L4
	movzwl	-90(%rbp), %eax
	movzwl	%ax, %ecx
	leaq	-48(%rbp), %rdx
	leaq	-80(%rbp), %rsi
	movq	-88(%rbp), %rax
	movq	%rax, %rdi
	call	getInode
	movq	-112(%rbp), %rax
	addq	$24, %rax
	movq	(%rax), %rdx
	leaq	-48(%rbp), %rcx
	movq	-88(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	copyFileToLocal
	movq	-112(%rbp), %rax
	addq	$24, %rax
	movq	(%rax), %rdx
	movq	-112(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	jmp	.L5
.L4:
	movq	-112(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
.L5:
	movl	$10, %edi
	call	putchar
	movl	$0, %eax
.L6:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.globl	getInode
	.type	getInode, @function
getInode:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	%ecx, %eax
	movw	%ax, -28(%rbp)
	movq	-16(%rbp), %rax
	movzwl	20(%rax), %eax
	movzwl	%ax, %edx
	movq	-8(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	seekToPartationBlock
	movzwl	-28(%rbp), %eax
	sall	$5, %eax
	movslq	%eax, %rcx
	movq	-8(%rbp), %rax
	movl	$1, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	fseek
	movq	-24(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, %rcx
	movl	$1, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread
	movq	-24(%rbp), %rax
	leaq	1(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$1, %esi
	call	fread
	movq	-24(%rbp), %rax
	leaq	2(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-24(%rbp), %rax
	leaq	4(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-24(%rbp), %rax
	leaq	8(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	movq	-24(%rbp), %rax
	leaq	12(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	movq	-24(%rbp), %rax
	leaq	16(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	movq	-24(%rbp), %rax
	leaq	20(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	movl	$4, %edx
	movl	$2, %esi
	call	fread
	movq	-24(%rbp), %rax
	leaq	28(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	movl	$2, %edx
	movl	$2, %esi
	call	fread
	movq	-24(%rbp), %rax
	leaq	32(%rax), %rdi
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	getInode, .-getInode
	.globl	getMetaData
	.type	getMetaData, @function
getMetaData:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$544, %rsp
	movq	%rdi, -536(%rbp)
	movq	%rsi, -544(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-536(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	seekToPartationBlock
	movq	-544(%rbp), %rax
	movq	-536(%rbp), %rdx
	movq	%rdx, %rcx
	movl	$1, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread
	movq	-544(%rbp), %rax
	leaq	2(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	4(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	8(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	10(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	12(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	14(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	16(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	18(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	20(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	22(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	24(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-544(%rbp), %rax
	leaq	26(%rax), %rdi
	movq	-536(%rbp), %rax
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$2, %esi
	call	fread
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L9
	call	__stack_chk_fail
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	getMetaData, .-getMetaData
	.globl	seekToPartationBlock
	.type	seekToPartationBlock, @function
seekToPartationBlock:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, %eax
	movw	%ax, -12(%rbp)
	movzwl	-12(%rbp), %eax
	addl	$1, %eax
	sall	$9, %eax
	movslq	%eax, %rcx
	movq	-8(%rbp), %rax
	movl	$0, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	fseek
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	seekToPartationBlock, .-seekToPartationBlock
	.globl	findFile
	.type	findFile, @function
findFile:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$88, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%rdx, -88(%rbp)
	movq	%rcx, -96(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movq	-80(%rbp), %rax
	movzwl	24(%rax), %eax
	movzwl	%ax, %edx
	movq	-72(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	seekToPartationBlock
	movl	$0, -52(%rbp)
	jmp	.L12
.L15:
	movq	-72(%rbp), %rdx
	leaq	-48(%rbp), %rax
	movq	%rdx, %rcx
	movl	$1, %edx
	movl	$14, %esi
	movq	%rax, %rdi
	call	fread
	movb	$0, -34(%rbp)
	leaq	-48(%rbp), %rdx
	movq	-88(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L13
	movq	-72(%rbp), %rdx
	movq	-96(%rbp), %rax
	movq	%rdx, %rcx
	movl	$1, %edx
	movl	$2, %esi
	movq	%rax, %rdi
	call	fread
	movl	$1, %eax
	jmp	.L16
.L13:
	movq	-72(%rbp), %rax
	movl	$1, %edx
	movl	$2, %esi
	movq	%rax, %rdi
	call	fseek
	addl	$1, -52(%rbp)
.L12:
	cmpl	$31, -52(%rbp)
	jle	.L15
	movl	$0, %eax
.L16:
	movq	-24(%rbp), %rbx
	xorq	%fs:40, %rbx
	je	.L17
	call	__stack_chk_fail
.L17:
	addq	$88, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	findFile, .-findFile
	.section	.rodata
.LC4:
	.string	"wb"
	.text
	.globl	copyFileToLocal
	.type	copyFileToLocal, @function
copyFileToLocal:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-40(%rbp), %rax
	movl	$.LC4, %esi
	movq	%rax, %rdi
	call	fopen
	movq	%rax, -8(%rbp)
	movq	-32(%rbp), %rax
	movl	8(%rax), %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	testl	%eax, %eax
	je	.L19
	movq	-32(%rbp), %rax
	leaq	20(%rax), %rdx
	leaq	-12(%rbp), %rcx
	movq	-8(%rbp), %rsi
	movq	-24(%rbp), %rax
	movq	%rcx, %r8
	movl	$4, %ecx
	movq	%rax, %rdi
	call	copyContentInDirectIndex
.L19:
	movl	-12(%rbp), %eax
	testl	%eax, %eax
	je	.L20
	movq	-32(%rbp), %rax
	leaq	28(%rax), %rdx
	leaq	-12(%rbp), %rcx
	movq	-8(%rbp), %rsi
	movq	-24(%rbp), %rax
	movq	%rcx, %r8
	movl	$2, %ecx
	movq	%rax, %rdi
	call	copyContentInl1IndirectIndex
.L20:
	movl	-12(%rbp), %eax
	testl	%eax, %eax
	je	.L21
	movq	-32(%rbp), %rax
	leaq	32(%rax), %rdx
	leaq	-12(%rbp), %rcx
	movq	-8(%rbp), %rsi
	movq	-24(%rbp), %rax
	movq	%rcx, %r8
	movl	$1, %ecx
	movq	%rax, %rdi
	call	copyContentInl2IndirectIndex
.L21:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	copyFileToLocal, .-copyFileToLocal
	.globl	copyContentInl2IndirectIndex
	.type	copyContentInl2IndirectIndex, @function
copyContentInl2IndirectIndex:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$576, %rsp
	movq	%rdi, -536(%rbp)
	movq	%rsi, -544(%rbp)
	movq	%rdx, -552(%rbp)
	movl	%ecx, %eax
	movq	%r8, -568(%rbp)
	movb	%al, -556(%rbp)
	movl	$0, -520(%rbp)
	jmp	.L23
.L31:
	movq	-568(%rbp), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jne	.L24
	jmp	.L22
.L24:
	movl	-520(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	movq	-552(%rbp), %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	testw	%ax, %ax
	je	.L26
	movl	-516(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	movq	-552(%rbp), %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %edx
	movq	-536(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	seekToPartationBlock
	movq	-536(%rbp), %rdx
	leaq	-512(%rbp), %rax
	movq	%rdx, %rcx
	movl	$512, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread
	movl	$0, -516(%rbp)
	jmp	.L27
.L30:
	movq	-568(%rbp), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jne	.L28
	jmp	.L26
.L28:
	movl	-516(%rbp), %eax
	cltq
	movzwl	-512(%rbp,%rax,2), %eax
	testw	%ax, %ax
	je	.L29
	movl	-516(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	leaq	-512(%rbp), %rax
	addq	%rax, %rdx
	movq	-568(%rbp), %rcx
	movq	-544(%rbp), %rsi
	movq	-536(%rbp), %rax
	movq	%rcx, %r8
	movl	$1, %ecx
	movq	%rax, %rdi
	call	copyContentInl1IndirectIndex
.L29:
	addl	$1, -516(%rbp)
.L27:
	cmpl	$255, -516(%rbp)
	jle	.L30
.L26:
	addl	$1, -520(%rbp)
.L23:
	movzbl	-556(%rbp), %eax
	cmpl	-520(%rbp), %eax
	jg	.L31
.L22:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	copyContentInl2IndirectIndex, .-copyContentInl2IndirectIndex
	.globl	copyContentInl1IndirectIndex
	.type	copyContentInl1IndirectIndex, @function
copyContentInl1IndirectIndex:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$576, %rsp
	movq	%rdi, -536(%rbp)
	movq	%rsi, -544(%rbp)
	movq	%rdx, -552(%rbp)
	movl	%ecx, %eax
	movq	%r8, -568(%rbp)
	movb	%al, -556(%rbp)
	movl	$0, -520(%rbp)
	jmp	.L33
.L41:
	movq	-568(%rbp), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jne	.L34
	jmp	.L32
.L34:
	movl	-520(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	movq	-552(%rbp), %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	testw	%ax, %ax
	je	.L36
	movl	-520(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	movq	-552(%rbp), %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %edx
	movq	-536(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	seekToPartationBlock
	movq	-536(%rbp), %rdx
	leaq	-512(%rbp), %rax
	movq	%rdx, %rcx
	movl	$512, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread
	movl	$0, -516(%rbp)
	jmp	.L37
.L40:
	movq	-568(%rbp), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jne	.L38
	jmp	.L36
.L38:
	movl	-516(%rbp), %eax
	cltq
	movzwl	-512(%rbp,%rax,2), %eax
	testw	%ax, %ax
	je	.L39
	movl	-516(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	leaq	-512(%rbp), %rax
	addq	%rax, %rdx
	movq	-568(%rbp), %rcx
	movq	-544(%rbp), %rsi
	movq	-536(%rbp), %rax
	movq	%rcx, %r8
	movl	$1, %ecx
	movq	%rax, %rdi
	call	copyContentInDirectIndex
.L39:
	addl	$1, -516(%rbp)
.L37:
	cmpl	$255, -516(%rbp)
	jle	.L40
.L36:
	addl	$1, -520(%rbp)
.L33:
	movzbl	-556(%rbp), %eax
	cmpl	-520(%rbp), %eax
	jg	.L41
.L32:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	copyContentInl1IndirectIndex, .-copyContentInl1IndirectIndex
	.globl	copyContentInDirectIndex
	.type	copyContentInDirectIndex, @function
copyContentInDirectIndex:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movl	%ecx, %eax
	movq	%r8, -56(%rbp)
	movb	%al, -44(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L43
.L47:
	movq	-56(%rbp), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jne	.L44
	jmp	.L42
.L44:
	movl	-4(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	testw	%ax, %ax
	je	.L46
	movl	-4(%rbp), %eax
	cltq
	leaq	(%rax,%rax), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %edx
	movq	-56(%rbp), %rcx
	movq	-32(%rbp), %rsi
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	copyBlockContent
.L46:
	addl	$1, -4(%rbp)
.L43:
	movzbl	-44(%rbp), %eax
	cmpl	-4(%rbp), %eax
	jg	.L47
.L42:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	copyContentInDirectIndex, .-copyContentInDirectIndex
	.globl	copyBlockContent
	.type	copyBlockContent, @function
copyBlockContent:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$72, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%rdi, -88(%rbp)
	movq	%rsi, -96(%rbp)
	movl	%edx, %eax
	movq	%rcx, -112(%rbp)
	movw	%ax, -100(%rbp)
	movq	%rsp, %rax
	movq	%rax, %rbx
	movq	-112(%rbp), %rax
	movl	(%rax), %eax
	movl	$512, %edx
	cmpl	$512, %eax
	cmovg	%edx, %eax
	movl	%eax, -68(%rbp)
	movzwl	-100(%rbp), %edx
	movq	-88(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	seekToPartationBlock
	movl	-68(%rbp), %eax
	movslq	%eax, %rdx
	subq	$1, %rdx
	movq	%rdx, -64(%rbp)
	movslq	%eax, %rdx
	movq	%rdx, %r14
	movl	$0, %r15d
	movslq	%eax, %rdx
	movq	%rdx, %r12
	movl	$0, %r13d
	cltq
	movl	$16, %edx
	subq	$1, %rdx
	addq	%rdx, %rax
	movl	$16, %ecx
	movl	$0, %edx
	divq	%rcx
	imulq	$16, %rax, %rax
	subq	%rax, %rsp
	movq	%rsp, %rax
	addq	$0, %rax
	movq	%rax, -56(%rbp)
	movl	-68(%rbp), %eax
	movslq	%eax, %rsi
	movq	-56(%rbp), %rax
	movq	-88(%rbp), %rdx
	movq	%rdx, %rcx
	movl	$1, %edx
	movq	%rax, %rdi
	call	fread
	movl	-68(%rbp), %eax
	movslq	%eax, %rsi
	movq	-56(%rbp), %rax
	movq	-96(%rbp), %rdx
	movq	%rdx, %rcx
	movl	$1, %edx
	movq	%rax, %rdi
	call	fwrite
	movq	-112(%rbp), %rax
	movl	(%rax), %eax
	subl	-68(%rbp), %eax
	movl	%eax, %edx
	movq	-112(%rbp), %rax
	movl	%edx, (%rax)
	movq	%rbx, %rsp
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	copyBlockContent, .-copyBlockContent
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
