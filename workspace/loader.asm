%include	"pm.inc"
%include	"load.inc"
%include	"lofs.inc"

org 	OffsetOfLoader

	jmp	LABEL_START

; GDT
;				     	段基址, 段界限, 属性
LABEL_GDT:		Descriptor	0, 0, 0              ; 空描述符
LABEL_DESC_FLAT_C:	Descriptor	0, 0fffffh, DA_CR|DA_32|DA_LIMIT_4K ;0-4G
LABEL_DESC_FLAT_RW:	Descriptor	0, 0fffffh, DA_DRW|DA_32|DA_LIMIT_4K;0-4G
LABEL_DESC_VIDEO:	Descriptor	0B8000h, 0ffffh, DA_DRW|DA_DPL3 ; 显存首地址

GdtLen		equ	$ - LABEL_GDT
GdtPtr		dw	GdtLen - 1				; 段界限
		dd	BaseOfLoaderPhyAddr + LABEL_GDT	; 基地址

; GDT 选择子
SelectorFlatC		equ	LABEL_DESC_FLAT_C	- LABEL_GDT
SelectorFlatRW		equ	LABEL_DESC_FLAT_RW	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT + SA_RPL3

BaseOfStack	equ	OffsetOfLoader
PageDirBase	equ	100000h	; 页目录开始地址: 1M
PageTblBase	equ	101000h	; 页表开始地址:   1M + 4K

LABEL_START:	
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BaseOfStack
	
	mov	dh, 0 			;"Loading Kernel"
	call	DispStrRealMode	;显示字符串

	; 得到内存数
	mov	ebx, 0			; ebx = 后续值, 开始时需为 0
	mov	di, _MemChkBuf	; es:di 指向一个地址范围描述符结构(ARDS)
.MemChkLoop:
	mov	eax, 0E820h		; eax = 0000E820h
	mov	ecx, 20			; ecx = 地址范围描述符结构的大小
	mov	edx, 0534D4150h		; edx = 'SMAP'
	int	15h			; int 15h
	jc	.MemChkFail
	;更新可用内存大小
	cmp 	dword [es:(di + 16)], 1 	;if(type == 1)
	jne	.MemSizeOk
	mov 	eax, [es:di] 		;if(BaseAddressLow + LengthLow > MemSize)
	add 	eax, [es:(di + 8)]
	cmp 	eax, [_dwMemSize]
	jb	.MemSizeOk
	mov 	[_dwMemSize], eax	;MemSize = BaseAddressLow + LengthLow
.MemSizeOk:
	add	di, 20
	inc	dword [_dwMCRNumber]	; dwMCRNumber = ARDS 的个数
	cmp	ebx, 0
	jne	.MemChkLoop
	jmp	.MemChkOK
.MemChkFail:
	mov	dword [_dwMCRNumber], 0
.MemChkOK:

;判断内存是否大于16M,若不足16M,则报错
	cmp 	dword [_dwMemSize], 1000000h
	ja	.MemEnough
	mov 	dh, 3
	call	DispStrRealMode
	jmp	$			;报错之后,死循环于此

.MemEnough:
;在A盘根目录找KERNEL.BIN
	xor	ah, ah	; ┓
	xor	dl, dl	; ┣ 软驱复位
	int	13h	; ┛


;先找到RootDIrectory所在的sector也就是数据区开始的sector
	mov	ax, BaseOfKernel
	mov	es, ax			; es <- BaseOfKernel
	mov	bx, OffsetOfKernel	; bx <- OffsetOfKernel	于是, es:bx = BaseOfKernel:OffsetOfKernel
	mov	ax, 0
	add	ax, PARTITION_BEGIN_BLOCK
	mov	cl, 1
	call	ReadSector		;读取元数据到 BaseOfKernel:OffsetOfKernel指向的缓冲区中
	mov	dx, [es:bx + 19]		
	add	dx, PARTITION_BEGIN_BLOCK
	mov	word [istart], dx 	;[istart]存放inode起始块号

	mov	ax, [es:bx + 23]		;ax<-元数据区中的数据区开始扇区号
	add	ax, PARTITION_BEGIN_BLOCK		;元数据中的扇区号是没有包含引导扇区的，而且从0开始算,所以要加1。
	mov	word[dstart], ax
; 下面在 A 盘的根目录寻找 KERNEL.BIN
	mov	word [wSectorNo], ax

LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
	cmp	word [wRootDirSizeForLoop], 0	; ┓
	jz	LABEL_NO_KERNELBIN		; ┣ 判断根目录区是不是已经读完
	dec	word [wRootDirSizeForLoop]	; ┛ 如果读完表示没有找到 KERNEL.BIN
	mov	ax, BaseOfKernel
	mov	es, ax			; es <- BaseOfKernel
	mov	bx, OffsetOfKernel	; bx <- OffsetOfKernel	于是, es:bx = BaseOfKernel:OffsetOfKernel
	mov	ax, [wSectorNo]	; ax <- Root Directory 中的某 Sector 号
	mov	cl, 1
	call	ReadSector		;读取目录扇区到BaseOfKernel:OffsetOfKernel指向的缓冲区中

	mov	si, KERNELFileName	; ds:si -> "KERNEL  BIN"
	mov	di, OffsetOfKernel	; es:di -> BaseOfKernel:0100 = BaseOfKernel*10h+100
	cld
	mov	dx, 32
LABEL_SEARCH_FOR_KERNELBIN:
	cmp	dx, 0						; ┓循环次数控制,
	jz	LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR	; ┣如果已经读完了一个 Sector,
	dec	dx						; ┛就跳到下一个 Sector
	mov	cx, 11
LABEL_CMP_FILENAME:
	cmp	cx, 0
	jz	LABEL_FILENAME_FOUND	; 如果比较了 11 个字符都相等, 表示找到
	dec	cx
	lodsb					; ds:si -> al
	cmp	al, byte [es:di]
	jz	LABEL_GO_ON
	jmp	LABEL_DIFFERENT		; 只要发现不一样的字符就表明本 DirectoryEntry 不是
; 我们要找的 KERNEL.BIN
LABEL_GO_ON:
	inc	di
	jmp	LABEL_CMP_FILENAME	;	继续循环

LABEL_DIFFERENT:
	and	di, 0FFF0h						; else ┓	di &= F0 为了让它指向本条目开头
	add	di, 10h							;          ┃
	mov	si, KERNELFileName					;          ┣ di += 10h  下一个目录条目
	jmp	LABEL_SEARCH_FOR_KERNELBIN			;          ┛

LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
	add	word [wSectorNo], 1
	jmp	LABEL_SEARCH_IN_ROOT_DIR_BEGIN

LABEL_NO_KERNELBIN:
	mov	dh, 2			; "No KERNEL."
	call	DispStrRealMode			; 显示字符串

%ifdef	_BOOT_DEBUG_
	mov	ax, 4c00h		; ┓
	int	21h			; ┛没有找到 KERNEL.BIN, 回到 DOS
%else
	jmp	$			; 没有找到 KERNEL.BIN, 死循环在这里
%endif

LABEL_FILENAME_FOUND:			; 找到 KERNEL.BIN 后便来到这里继续
	mov	ax, BaseOfKernel
	mov	es, ax				; es <- BaseOfKernel
	and	di, 0FFF0h			;di &= FFF0 为了让它指向本条目开头
	add	di, 14				;目录文件中14为i-node号起始偏移地址
	mov	ax, [es:di]			;ax<-文件的i-node号
	mov	bx, InodeSize
	mul	bx
	mov	bx, BlockSize
	div	bx				;dx = 块内字节偏移, ax = inode所在inode块相对与istart的块号（0开始）
	mov 	di, dx 				;di = 块内字节偏移
	add	ax, [istart]			;ax=文件inode所在的块
	mov	cl, 1
	mov	bx, BaseOfKernel
	mov	es, bx 				; es <- BaseOfKernel
	mov	bx, OffsetOfKernel 		; bx <- OffsetOfKernel	于是, es:bx = BaseOfKernel:OffsetOfKernel
	call	ReadSector			;读取KERNEL.bin的inode所在的块到BaseOfKernel:OffsetOfKernel所指向的缓冲区

	mov	eax, [es:bx + di + 6]
	mov	[fileSize], eax	
	mov 	ax, [es:bx + di + 18]
	mov 	[dbidx], ax
	mov 	ax, [es:bx + di + 20]
	mov 	[dbidx + 2], ax
	mov 	ax, [es:bx + di + 22]
	mov 	[dbidx + 4], ax
	mov 	ax, [es:bx + di + 24]
	mov 	[dbidx + 6], ax
	
	mov	ax, [es:bx + di + 26]
	mov	[l1ibidx], ax
	mov	ax, [es:bx + di + 28]
	mov	[l1ibidx + 2], ax
	mov 	ax, [es:bx + di + 30]
	mov 	[l2idbidx], ax
	
	;开始将文件内容复制到es:bx指向的内存中
	mov	bx, BaseOfKernel
	mov	es, bx 				; es <- BaseOfKernel
	mov	bx, OffsetOfKernel 		; bx <- OffsetOfKernel	于是, es:bx = BaseOfKernel:OffsetOfKernel

	cmp 	dword [fileSize], 0
	jle	LABEL_COPY_FINISH
	push	word INDEX_DBIDX_COUNT	;[bp + 6]
	push	dbidx 				;[bp + 4]
	call	CopyContentInDirectIndex
	add	esp, 4

	cmp 	dword [fileSize], 0
	jle	LABEL_COPY_FINISH
	push	word INDEX_L1IDBIDX_COUNT	;[bp + 6]
	push	l1ibidx 				;[bp + 4]
	call	CopyContentInl1IndirectIndex
	add 	esp, 4

	cmp 	dword [fileSize], 0
	jle	LABEL_COPY_FINISH
	push	word INDEX_L2IDBIDX_COUNT	;[bp + 6]
	push 	l2idbidx 				;[bp + 4]
	call	CopyContentInl2IndirectIndex
	add 	esp, 4

LABEL_COPY_FINISH:
LABEL_FILE_LOADED:
	mov	dh, 1			; "Ready."
	call	DispStrRealMode			; 显示字符串


;进入保护模式
	; 加载 GDTR
	lgdt	[GdtPtr]

	; 关中断
	cli

	; 打开地址线A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al

	; 准备切换到保护模式
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

	; 真正进入保护模式
	jmp	dword SelectorFlatC:(BaseOfLoaderPhyAddr+LABEL_PM_START)

	jmp	$



;============================================================================
;变量
;----------------------------------------------------------------------------
wRootDirSizeForLoop	dw	RootDirSectors	; Root Directory 占用的扇区数, 在循环中会递减至零.
wSectorNo		dw	0		; 要读取的扇区号
bOdd			db	0		; 奇数还是偶数
istart			dw	0 		;inode起始块编号
dstart			dw 	0 		;数据区开始的sector
bxtmp			dw 	0 		;临时存放变量
estemp			dw  	0 		;临时存放变量


;文件inode内容
;----------------------------------------------------------------------------------------------
dbidx:			dw	0 		;四个直接块索引
			dw	0
			dw	0
			dw	0

l1ibidx:			dw	0 		;2个一级间接块索引
			dw	0

l2idbidx		dw	0 		;1个二级间接块索引

fileSize 			dd 	0 		;文件长度
;============================================================================
;字符串
;----------------------------------------------------------------------------
KERNELFileName		db	"KERNEL.BIN", 0	; KERNEL.BIN 之文件名
; 为简化代码, 下面每个字符串的长度均为 MessageLength
MessageLength		equ	14
BootMessage:		db	"Loading Kernel"; 14字节, 不够则用空格补齐. 序号 0
Message1		db	"Ready.        "; 14字节, 不够则用空格补齐. 序号 1
Message2		db	"No KERNEL     "; 14字节, 不够则用空格补齐. 序号 2
Message3		db 	"Mem not enough"
;============================================================================

;----------------------------------------------------------------------------
; 函数名: CopyContentInDirectIndex
;----------------------------------------------------------------------------
; 作用:
;	将直接索引块复制到es:bx指向空间，并且将BX指向下一个空白位置
;	堆栈传入参数：[bp+4] = blockIndex, [bp + 6] = dbidxCount

;void copyContentInDirectIndex(FILE* device,FILE* localFile, u16 * directBlockIndex, u8 dbidxCount, int* remainingSize)
;{
;	int i;
;	for(i = 0; i < dbidxCount; i++)
;	{	
;		if(!*remainingSize)
;			break;
;		if(directBlockIndex[i])
;			copyBlockContent(device, localFile, directBlockIndex[i], remainingSize);
;	}
;}
CopyContentInDirectIndex:
	push 	bp
	mov	bp, sp
	push 	di
	push	ax
	push	cx

	mov	di, 0 				;di代表第几个直接索引
	shl 	word [bp + 6], 1
LABEL_DBIDX_LOOP:
	cmp 	di, [bp + 6]
	jz	LABEL_DBIDX_END
	cmp 	dword [fileSize], 0
	jle	LABEL_DBIDX_END
	mov	[bxtmp], bx 				;后面要用到传入的bx所以先push保存起来
	mov 	bx, [bp + 4]
	cmp 	word [ds:bx + di], 0
	jz	LABEL_DBIDX_NOTHING_TO_CP
	mov	ax, [ds:bx + di]			;将[ds:bx + di]索引指向的块复制到es:bx
	add 	ax, PARTITION_BEGIN_BLOCK
	mov	cl, 1
	mov 	bx, [bxtmp]
	call	ReadSector			;读取KERNEL.bin的inode所在的块到BaseOfKernel:OffsetOfKernel所指向的缓冲区

	push	ax
	push	bx
	push	cx
	mov	ah, 0Eh			; ┃ 每读一个扇区打一个点, 形成这样的效果:
	mov	al, '.'			; ┃
	mov	bl, 0Fh			; ┃ ....
	mov 	cx, 1
	int	10h			; ┃
	pop 	cx
	pop 	bx
	pop 	ax

	add	bx, 512				;将文件的一块复制到内存后bx+512指向新的空闲空间
	mov	ax, [fileSize]
	sub	ax, 512
	mov	[fileSize], ax			;[fileSize] -= 512
	mov	ax, [fileSize + 2]
	sbb	ax, 0
	mov 	[fileSize + 2], ax


LABEL_DBIDX_NOTHING_TO_CP:
	add 	di, 2
	jmp	LABEL_DBIDX_LOOP

LABEL_DBIDX_END:
	pop	cx
	pop	ax
	pop	di
	pop	bp
	ret


;----------------------------------------------------------------------------
; 函数名: CopyContentInl1IndirectIndex
;----------------------------------------------------------------------------
; 作用:
;	将一级间接索引块复制到es:bx指向空间，并且将BX指向下一个空白位置
;	参数：[bp + 4] = l1IndirectBlockIndex, [bp + 6] = l1idbidxCount

;void copyContentInl1IndirectIndex(FILE* device,FILE* localFile, u16 * l1IndirectBlockIndex, u8 l1idbidxCount, int* remainingSize)
;{
;	u16 buffer[BLOCK_SIZE / 2];
;	int i, j;
;	for(i = 0; i < l1idbidxCount; i++)
;	{
;		if(!* remainingSize)
;			break;
;		if(l1IndirectBlockIndex[i])
;		{
;			seekToPartationBlock(device, l1IndirectBlockIndex[i]);
;			fread(buffer, 1, BLOCK_SIZE, device);
;			for(j = 0; j < BLOCK_SIZE / 2; j++)
;			{
;				if(!* remainingSize)
;					break;
;				if(buffer[j])
;					copyContentInDirectIndex(device, localFile, buffer + j, 1, remainingSize);
;			}
;		}
;	}
;}
CopyContentInl1IndirectIndex:
	push	bp
	mov	bp, sp
	sub	esp, 512			; 辟出512个字节的堆栈区域保存一级间接索引块
	push 	di
	push 	ax

	mov 	di, 0
	shl 	word [bp + 6], 1
LABEL_L1IDBIDX_LOOP:
	cmp 	di, [bp + 6]
	jz	LABEL_L1IDBIDX_END

	cmp 	word [fileSize], 0
	jle	LABEL_L1IDBIDX_END

	mov 	[bxtmp], bx
	mov 	[estemp], es

	mov	bx, [bp + 4]
	cmp 	word [ds:bx + di], 0
	jz	LABLE_L1IDBIDX_NOTHING_TO_CP
	mov 	ax, [ds:bx + di]
	add 	ax, PARTITION_BEGIN_BLOCK
	mov	cl, 1
	mov	dx, cs
	mov 	es, dx				;es = cs
	mov 	bx, bp 				;bx = bp - 512
	sub 	bx, 512
	call	ReadSector

	mov	es, [estemp]
	mov	bx, [bxtmp]

	mov 	si, 0
LABEL_L1IDBIDX_LOOP_1:
	cmp 	si, BlockSize
	jz	LABEL_L1IDBIDX_END_1

	cmp 	word [fileSize], 0
	jle	LABEL_L1IDBIDX_END_1

	cmp 	word [bp - 512 + si], 0
	jz	LABEL_L1IDBIDX_NOTHING_TO_CP_1
	push	word 1 						;[bp + 6] = 1
	mov	dx, bp
	sub	dx, 512
	add	dx, si
	push	dx 	 	 				;[bp + 4] = bp - 512 + si
	;es:bx已到位
	call	CopyContentInDirectIndex
	add	esp, 4
	
LABEL_L1IDBIDX_NOTHING_TO_CP_1:
	add 	si, 2
	jmp 	LABEL_L1IDBIDX_LOOP_1
LABEL_L1IDBIDX_END_1:


LABLE_L1IDBIDX_NOTHING_TO_CP:
	add 	di, 2
	jmp	LABEL_L1IDBIDX_LOOP
LABEL_L1IDBIDX_END:
	pop	ax
	pop	di
	add	esp, 512
	pop	bp
	ret


;----------------------------------------------------------------------------
; 函数名: CopyContentInl2IndirectIndex
;----------------------------------------------------------------------------
; 作用:
;	将二级间接索引块复制到es:bx指向空间，并且将BX指向下一个空白位置
;	参数：[bp + 4] = l2IndirectBlockIndex, [bp + 6] = l2idbidxCount
;void copyContentInl2IndirectIndex(FILE* device,FILE* localFile, u16 * l2IndirectBlockIndex, u8 l2idbidxCount, int* remainingSize)
;{
;	u16 buffer[BLOCK_SIZE / 2];
;	int i, j;
;	for(i = 0; i < l2idbidxCount; i++)
;	{
;		if(!* remainingSize)
;			break;
;		if(l2IndirectBlockIndex[i])
;		{
;			seekToPartationBlock(device, l2IndirectBlockIndex[j]);
;			fread(buffer, 1, BLOCK_SIZE, device);
;			for(j = 0; j < BLOCK_SIZE / 2; j++)
;			{	
;				if(!* remainingSize)
;					break;
;				if(buffer[j])
;					copyContentInl1IndirectIndex(device, localFile, buffer + j, 1, remainingSize);
;			}
;		}
;	}
;}
CopyContentInl2IndirectIndex:
	push	bp
	mov	bp, sp
	sub	esp, 512			; 辟出512个字节的堆栈区域保存二级间接索引块
	push 	di
	push 	ax

	mov 	di, 0
	shl 	word [bp + 6], 1
LABEL_L2IDBIDX_LOOP:
	cmp 	di, [bp + 6]
	jz 	LABEL_L2IDBIDX_END

	cmp 	word [fileSize], 0
	jle	LABEL_L2IDBIDX_END

	mov	[bxtmp], bx
	mov	[estemp], es

	mov	bx, [bp + 4]
	cmp 	word [ds:bx + di], 0
	jz	LABEL_L2IDBIDX_NOTHING_TO_CP
	mov 	ax, [ds:bx + di]
	add 	ax, PARTITION_BEGIN_BLOCK
	mov	cl, 1
	mov	dx, cs
	mov 	es, dx				;es = cs
	mov 	bx, bp 				;bx = bp - 512
	sub 	bx, 512
	call	ReadSector

	mov 	bx, [bxtmp]
	mov	es, [estemp]


	mov 	si, 0
LABEL_L2IDBIDX_LOOP_1:
	cmp 	si, BlockSize
	jz	LABEL_L1IDBIDX_END_1

	cmp 	word[fileSize], 0
	jle 	LABEL_L1IDBIDX_END_1

	cmp 	word [bp - 512 + si], 0
	jz	LABEL_L2IDBIDX_NOTHING_TO_CP_1
	push	word 1 						;[bp + 6] = 1
	mov 	dx, bp
	sub	dx, 512
	add 	dx, si
	push	bx 						;[bp + 4] = bp - 512 + si
	;es:bx已到位
	call 	CopyContentInl1IndirectIndex
	add	esp, 4

LABEL_L2IDBIDX_NOTHING_TO_CP_1:
	add	si, 2
	jmp	LABEL_L1IDBIDX_LOOP_1
LABEL_L2IDBIDX_END_1:


LABEL_L2IDBIDX_NOTHING_TO_CP:
	add 	di, 2
	jmp	LABEL_L2IDBIDX_LOOP
LABEL_L2IDBIDX_END:
	pop	ax
	pop	di
	add	esp, 512
	pop	bp
	ret



;----------------------------------------------------------------------------
; 函数名: DispStrRealMode
;----------------------------------------------------------------------------
; 作用:
;	显示一个字符串, 函数开始时 dh 中应该是字符串序号(0-based)
DispStrRealMode:
	push	es
	push	ax
	push 	bx
	push	cx
	push	dx

	mov	ax, MessageLength
	mul	dh
	add	ax, BootMessage
	mov	bp, ax			; ┓
	mov	ax, ds			; ┣ ES:BP = 串地址
	mov	es, ax			; ┛
	mov	cx, MessageLength	; CX = 串长度
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 0007h		; 页号为0(BH = 0) 黑底白字(BL = 07h)
	add 	dh, 6			;从第六行开始显示
	mov	dl, 0
	int	10h			; int 10h

	pop	dx
	pop	cx
	pop	bx
	pop	ax
	pop	es
	ret

;----------------------------------------------------------------------------
; 函数名: ReadSector
;----------------------------------------------------------------------------
; 作用:
;	从第 ax 个 Sector 开始, 将 cl 个 Sector 读入 es:bx 中
ReadSector:
	; -----------------------------------------------------------------------
	; 怎样由扇区号求扇区在磁盘中的位置 (扇区号 -> 柱面号, 起始扇区, 磁头号)
	; -----------------------------------------------------------------------
	; 设扇区号为 x
	;                           ┌ 柱面号 = y >> 1
	;       x           ┌ 商 y ┤
	; -------------- => ┤      └ 磁头号 = y & 1
	;  每磁道扇区数     │
	;                   └ 余 z => 起始扇区号 = z + 1
	push	ax
	push	bx
	push	cx
	push	dx
	push	bp

	mov	bp, sp
	sub	esp, 2			; 辟出两个字节的堆栈区域保存要读的扇区数: byte [bp-2]

	mov	byte [bp-2], cl
	push	bx			; 保存 bx
	mov	bl, SecPerTrk		; bl: 除数
	div	bl			; y 在 al 中, z 在 ah 中
	inc	ah			; z ++
	mov	cl, ah			; cl <- 起始扇区号
	mov	dh, al			; dh <- y
	shr	al, 1			; y >> 1 (其实是 y/BPB_NumHeads, 这里BPB_NumHeads=2)
	mov	ch, al			; ch <- 柱面号
	and	dh, 1			; dh & 1 = 磁头号
	pop	bx			; 恢复 bx
	; 至此, "柱面号, 起始扇区, 磁头号" 全部得到 ^^^^^^^^^^^^^^^^^^^^^^^^
	mov	dl, 0			; 驱动器号 (0 表示 A 盘)
.GoOnReading:
	mov	ah, 2			; 读
	mov	al, byte [bp-2]		; 读 al 个扇区
	int	13h
	jc	.GoOnReading		; 如果读取错误 CF 会被置为 1, 这时就不停地读, 直到正确为止

	add	esp, 2
	pop	bp
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	ret


; 从此以后的代码在保护模式下执行 ----------------------------------------------------
; 32 位代码段. 由实模式跳入 ---------------------------------------------------------
[SECTION .s32]

ALIGN	32

[BITS	32]

LABEL_PM_START:
	mov	ax, SelectorVideo
	mov	gs, ax

	mov	ax, SelectorFlatRW
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	ss, ax
	mov	esp, TopOfStack

	push	szMemChkTitle
	call	DispStr
	add	esp, 4

	call	DispMemInfo
	call	SetupPaging

	mov	ah, 0Fh				; 0000: 黑底    1111: 白字
	mov	al, 'P'
	mov	[gs:((80 * 0 + 39) * 2)], ax	; 屏幕第 0 行, 第 39 列。

	call	InitKernel

	;***************************************************************
	jmp	SelectorFlatC:KernelEntryPointPhyAddr			  ; 正式进入内核 *
	;***************************************************************

%include	"lib.inc"

; 显示内存信息 --------------------------------------------------------------
DispMemInfo:
	push	esi
	push	edi
	push	ecx

	mov	esi, MemChkBuf
	mov	ecx, [dwMCRNumber];for(int i=0;i<[MCRNumber];i++)//每次得到一个ARDS
.loop:				  ;{
	mov	edx, 5		  ;  for(int j=0;j<5;j++)//每次得到一个ARDS中的成员
	mov	edi, ARDStruct	  ;  {//依次显示:BaseAddrLow,BaseAddrHigh,LengthLow
.1:				  ;               LengthHigh,Type
	push	dword [esi]	  ;
	call	DispInt		  ;    DispInt(MemChkBuf[j*4]); // 显示一个成员
	pop	eax		  ;
	stosd			  ;    ARDStruct[j*4] = MemChkBuf[j*4];
	add	esi, 4		  ;
	dec	edx		  ;
	cmp	edx, 0		  ;
	jnz	.1		  ;  }
	call	DispReturn	  ;  printf("\n");
	cmp	dword [dwType], 1 ;  if(Type == AddressRangeMemory)
	jne	.2		  ;  {
	mov	eax, [dwBaseAddrLow];
	add	eax, [dwLengthLow];
	cmp	eax, [dwMemSize]  ;    if(BaseAddrLow + LengthLow > MemSize)
	jb	.2		  ;
	mov	[dwMemSize], eax  ;    MemSize = BaseAddrLow + LengthLow;
.2:				  ;  }
	loop	.loop		  ;}
				  ;
	call	DispReturn	  ;printf("\n");
	push	szRAMSize	  ;
	call	DispStr		  ;printf("RAM size:");
	add	esp, 4		  ;
				  ;
	push	dword [dwMemSize] ;
	call	DispInt		  ;DispInt(MemSize);
	add	esp, 4		  ;

	pop	ecx
	pop	edi
	pop	esi
	ret
; ---------------------------------------------------------------------------


; 启动分页机制 --------------------------------------------------------------
SetupPaging:
	; 根据内存大小计算应初始化多少PDE以及多少页表
	xor	edx, edx
	mov	eax, [dwMemSize]
	mov	ebx, 400000h	; 400000h = 4M = 4096 * 1024, 一个页表对应的内存大小
	div	ebx
	mov	ecx, eax	; 此时 ecx 为页表的个数，也即 PDE 应该的个数
	test	edx, edx
	jz	.no_remainder
	inc	ecx		; 如果余数不为 0 就需增加一个页表
.no_remainder:
	push	ecx		; 暂存页表个数

	; 为简化处理, 所有线性地址对应相等的物理地址. 并且不考虑内存空洞.

	; 首先初始化页目录
	mov	ax, SelectorFlatRW
	mov	es, ax
	mov	edi, PageDirBase	; 此段首地址为 PageDirBase
	xor	eax, eax
	mov	eax, PageTblBase | PG_P  | PG_USU | PG_RWW
.1:
	stosd
	add	eax, 4096		; 为了简化, 所有页表在内存中是连续的.
	loop	.1

	; 再初始化所有页表
	pop	eax			; 页表个数
	mov	ebx, 1024		; 每个页表 1024 个 PTE
	mul	ebx
	mov	ecx, eax		; PTE个数 = 页表个数 * 1024
	mov	edi, PageTblBase	; 此段首地址为 PageTblBase
	xor	eax, eax
	mov	eax, PG_P  | PG_USU | PG_RWW
.2:
	stosd
	add	eax, 4096		; 每一页指向 4K 的空间
	loop	.2

	mov	eax, PageDirBase
	mov	cr3, eax
	mov	eax, cr0
	or	eax, 80000000h
	mov	cr0, eax
	jmp	short .3
.3:
	nop

	ret
; 分页机制启动完毕 ----------------------------------------------------------

; InitKernel ---------------------------------------------------------------------------------
; 将 KERNEL.BIN 的内容经过整理对齐后放到新的位置
; 遍历每一个 Program Header，根据 Program Header 中的信息来确定把什么放进内存，放到什么位置，以及放多少。
; --------------------------------------------------------------------------------------------
InitKernel:
        xor   esi, esi
        mov   cx, word [BaseOfKernelPhyAddr+2Ch]	;`. ecx <- pELFHdr->e_phnum
        movzx ecx, cx                               			;/
        mov   esi, [BaseOfKernelPhyAddr + 1Ch]  	; esi <- pELFHdr->e_phoff
        add   esi, BaseOfKernelPhyAddr		;esi<-OffsetOfKernel+pELFHdr->e_phoff
.Begin:
        mov   eax, [esi + 0]
        cmp   eax, 0                      			; PT_NULL
        jz    .NoAction
        push  dword [esi + 010h]			;size 	;`.
        mov   eax, [esi + 04h]				; |
        add   eax, BaseOfKernelPhyAddr			; | memcpy((void*)(pPHdr->p_vaddr),
        push  eax					;src	; |      uchCode + pPHdr->p_offset,
        push  dword [esi + 08h]			;dst  	; |      pPHdr->p_filesz;
        call  MemCpy					; |
        add   esp, 12 					;/
.NoAction:
        add   esi, 020h 				; esi += pELFHdr->e_phentsize
        dec   ecx
        jnz   .Begin

        ret
; InitKernel ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

; SECTION .data1 之开始 ---------------------------------------------------------------------------------------------
[SECTION .data1]

ALIGN	32

LABEL_DATA:
; 实模式下使用这些符号
; 字符串
_szMemChkTitle:	db "BaseAddrL BaseAddrH LengthLow LengthHigh   Type", 0Ah, 0
_szRAMSize:	db "RAM size:", 0
_szReturn:	db 0Ah, 0
;; 变量
_dwMCRNumber:	dd 0	; Memory Check Result
_dwDispPos:	dd (80 * 9 + 0) * 2	; 屏幕第 6 行, 第 0 列。
_dwMemSize:	dd 0
_ARDStruct:	; Address Range Descriptor Structure
  _dwBaseAddrLow:		dd	0
  _dwBaseAddrHigh:		dd	0
  _dwLengthLow:			dd	0
  _dwLengthHigh:		dd	0
  _dwType:			dd	0
_MemChkBuf:	times	256	db	0
;
;; 保护模式下使用这些符号
szMemChkTitle		equ	BaseOfLoaderPhyAddr + _szMemChkTitle
szRAMSize		equ	BaseOfLoaderPhyAddr + _szRAMSize
szReturn		equ	BaseOfLoaderPhyAddr + _szReturn
dwDispPos		equ	BaseOfLoaderPhyAddr + _dwDispPos
dwMemSize		equ	BaseOfLoaderPhyAddr + _dwMemSize
dwMCRNumber		equ	BaseOfLoaderPhyAddr + _dwMCRNumber
ARDStruct		equ	BaseOfLoaderPhyAddr + _ARDStruct
	dwBaseAddrLow	equ	BaseOfLoaderPhyAddr + _dwBaseAddrLow
	dwBaseAddrHigh	equ	BaseOfLoaderPhyAddr + _dwBaseAddrHigh
	dwLengthLow	equ	BaseOfLoaderPhyAddr + _dwLengthLow
	dwLengthHigh	equ	BaseOfLoaderPhyAddr + _dwLengthHigh
	dwType		equ	BaseOfLoaderPhyAddr + _dwType
MemChkBuf		equ	BaseOfLoaderPhyAddr + _MemChkBuf


; 堆栈就在数据段的末尾
StackSpace:	times	1024	db	0
TopOfStack	equ	BaseOfLoaderPhyAddr + $	; 栈顶
; SECTION .data1 之结束 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
