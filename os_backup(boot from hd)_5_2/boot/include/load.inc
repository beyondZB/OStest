BaseOfEmbeddedSector	equ	8000h	; findloader 被加载到的位置 ----  段地址
OffsetOfEmbeddedSector	equ	0400h	; findloader 被加载到的位置 ---- 偏移地址(同时也是栈基址，有1k大小的栈)

BaseOfLoader 		equ	8080h	; LOADER.BIN 被加载到的位置 ----  段地址
OffsetOfLoader		equ	0800h	; LOADER.BIN 被加载到的位置 ---- 偏移地址(同事也是栈基址，有2k大小的栈)

BaseOfLoaderPhyAddr	equ	BaseOfLoader*10h ; LOADER.BIN 被加载到的位置 ---- 物理地址

BaseOfKernel		equ	8FC0h	; KERNEL.BIN 被加载到的位置 ----  段地址
OffsetOfKernel		equ	0h	; KERNEL.BIN 被加载到的位置 ---- 偏移地址

BaseOfKernelPhyAddr		equ	BaseOfKernel * 10h
KernelEntryPointPhyAddr	equ	030400h	; 注意：1、必须与 MAKEFILE 中参数 -Ttext 的值相等!!
					;       2、这是个地址而非仅仅是个偏移，如果 -Ttext 的值为 0x400400，则它的值也应该是 0x400400。
