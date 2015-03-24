%ifdef	_BOOT_DEBUG_
	org  0100h			; 调试状态, 做成 .COM 文件, 可调试
%else
	org  07c00h			; Boot 状态, Bios 将把 Boot Sector 加载到 0:7C00 处并开始执行
%endif

;================================================================================================
%ifdef	_BOOT_DEBUG_
BaseOfStack		equ	0100h	; 调试状态下堆栈基地址(栈底, 从这个位置向低地址生长)
%else
BaseOfStack		equ	07c00h	; Boot状态下堆栈基地址(栈底, 从这个位置向低地址生长)
%endif

BaseOfLoader		equ	09000h	; LOADER.BIN 被加载到的位置 ----  段地址
OffsetOfLoader	equ	0100h	; LOADER.BIN 被加载到的位置 ---- 偏移地址
RootDirSectors		equ	1	; 根目录占用空间,这里暂时设置为1，即根目录最多存放32个文件.
SectorNoOfRootDirectory	equ	22	; Root Directory 的第一个扇区号
SecPerTrk		equ	18	;每磁道扇区数
BlockSize 		equ	512	;每块512字节
InodeSize		equ	32	;每个inode大小为32字节 
INDEX_DBIDX_COUNT	equ	4 	;4个直接索引块
INDEX_L1IDBIDX_COUNT	equ	2 	;2个一级间接索引块
INDEX_L2IDBIDX_COUNT	equ	1 	;1个二级间接索引块
;================================================================================================

LABEL_START:	
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BaseOfStack
	
	xor	ah, ah	; ┓
	xor	dl, dl	; ┣ 软驱复位
	int	13h	; ┛


;先找到RootDIrectory所在的sector也就是数据区开始的sector
	mov	ax, BaseOfLoader
	mov	es, ax			; es <- BaseOfLoader
	mov	bx, OffsetOfLoader	; bx <- OffsetOfLoader	于是, es:bx = BaseOfLoader:OffsetOfLoader
	mov	ax, 1
	mov	cl, 1
	call	ReadSector		;读取元数据到 BaseOfLoader:OffsetOfLoader指向的缓冲区中
	mov	dx, [es:bx + 19]		
	add	dx, 1
	mov	word [istart], dx 	;[istart]存放inode起始块号

	mov	ax, [es:bx + 23]		;ax<-元数据区中的数据区开始扇区号
	add	ax, 1 			;元数据中的扇区号是没有包含引导扇区的，而且从0开始算,所以要加1。
	mov	word[dstart], ax
; 下面在 A 盘的根目录寻找 LOADER.BIN
	mov	word [wSectorNo], ax

LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
	cmp	word [wRootDirSizeForLoop], 0	; ┓
	jz	LABEL_NO_LOADERBIN		; ┣ 判断根目录区是不是已经读完
	dec	word [wRootDirSizeForLoop]	; ┛ 如果读完表示没有找到 LOADER.BIN
	mov	ax, BaseOfLoader
	mov	es, ax			; es <- BaseOfLoader
	mov	bx, OffsetOfLoader	; bx <- OffsetOfLoader	于是, es:bx = BaseOfLoader:OffsetOfLoader
	mov	ax, [wSectorNo]	; ax <- Root Directory 中的某 Sector 号
	mov	cl, 1
	call	ReadSector		;读取目录扇区到BaseOfLoader:OffsetOfLoader指向的缓冲区中

	mov	si, LoaderFileName	; ds:si -> "LOADER  BIN"
	mov	di, OffsetOfLoader	; es:di -> BaseOfLoader:0100 = BaseOfLoader*10h+100
	cld
	mov	dx, 32
LABEL_SEARCH_FOR_LOADERBIN:
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
; 我们要找的 LOADER.BIN
LABEL_GO_ON:
	inc	di
	jmp	LABEL_CMP_FILENAME	;	继续循环

LABEL_DIFFERENT:
	and	di, 0FFF0h						; else ┓	di &= F0 为了让它指向本条目开头
	add	di, 10h							;          ┃
	mov	si, LoaderFileName					;          ┣ di += 10h  下一个目录条目
	jmp	LABEL_SEARCH_FOR_LOADERBIN			;          ┛

LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
	add	word [wSectorNo], 1
	jmp	LABEL_SEARCH_IN_ROOT_DIR_BEGIN

LABEL_NO_LOADERBIN:
	mov	dh, 2			; "No LOADER."
	call	DispStr			; 显示字符串
%ifdef	_BOOT_DEBUG_
	mov	ax, 4c00h		; ┓
	int	21h			; ┛没有找到 LOADER.BIN, 回到 DOS
%else
	jmp	$			; 没有找到 LOADER.BIN, 死循环在这里
%endif

LABEL_FILENAME_FOUND:			; 找到 LOADER.BIN 后便来到这里继续
	mov	ax, BaseOfLoader
	mov	es, ax				; es <- BaseOfLoader
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
	mov	bx, BaseOfLoader
	mov	es, bx 				; es <- BaseOfLoader
	mov	bx, OffsetOfLoader 		; bx <- OffsetOfLoader	于是, es:bx = BaseOfLoader:OffsetOfLoader
	call	ReadSector			;读取loader.bin的inode所在的块到BaseOfLoader:OffsetOfLoader所指向的缓冲区

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
	mov	bx, BaseOfLoader
	mov	es, bx 				; es <- BaseOfLoader
	mov	bx, OffsetOfLoader 		; bx <- OffsetOfLoader	于是, es:bx = BaseOfLoader:OffsetOfLoader

	cmp 	word [fileSize], 0
	jle	LABEL_COPY_FINISH
	push	word INDEX_DBIDX_COUNT	;[bp + 6]
	push	dbidx 				;[bp + 4]
	call	CopyContentInDirectIndex
	add	esp, 4

	cmp 	word [fileSize], 0
	jle	LABEL_COPY_FINISH
	push	word INDEX_L1IDBIDX_COUNT	;[bp + 6]
	push	l1ibidx 				;[bp + 4]
	call	CopyContentInl1IndirectIndex
	add 	esp, 4

	cmp 	word [fileSize], 0
	jle	LABEL_COPY_FINISH
	push	word INDEX_L2IDBIDX_COUNT	;[bp + 6]
	push 	l2idbidx 				;[bp + 4]
	call	CopyContentInl2IndirectIndex
	add 	esp, 4

LABEL_COPY_FINISH:
LABEL_FILE_LOADED:
	mov	dh, 1			; "Ready."
	call	DispStr			; 显示字符串

%ifdef	_BOOT_DEBUG_
	mov	ax, 4c00h		; ┓
	int	21h			; ┛没有找到 LOADER.BIN, 回到 DOS
%else
	jmp	$			; 没有找到 LOADER.BIN, 死循环在这里
%endif

; *****************************************************************************************************
	jmp	BaseOfLoader:OffsetOfLoader	; 这一句正式跳转到已加载到内存中的 LOADER.BIN 的开始处
						; 开始执行 LOADER.BIN 的代码
						; Boot Sector 的使命到此结束
; *****************************************************************************************************


;============================================================================
;变量
;----------------------------------------------------------------------------
wRootDirSizeForLoop	dw	RootDirSectors	; Root Directory 占用的扇区数, 在循环中会递减至零.
wSectorNo		dw	0		; 要读取的扇区号
bOdd			db	0		; 奇数还是偶数
istart			dw	0 		;inode起始块编号
dstart			dw 	0 		;数据区开始的sector

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
LoaderFileName		db	"LOADER.BIN", 0	; LOADER.BIN 之文件名
; 为简化代码, 下面每个字符串的长度均为 MessageLength
MessageLength		equ	9
BootMessage:		db	"Booting  "; 9字节, 不够则用空格补齐. 序号 0
Message1		db	"Ready.   "; 9字节, 不够则用空格补齐. 序号 1
Message2		db	"No LOADER"; 9字节, 不够则用空格补齐. 序号 2
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
LABEL_DBIDX_LOOP:
	mov 	ax, [bp + 6]
	mov 	dl, 2
	mul	dl
	cmp 	di, ax
	jz	LABEL_DBIDX_END
	cmp 	dword [fileSize], 0
	jle	LABEL_DBIDX_END
	push	bx 				;后面要用到传入的bx所以先push保存起来
	mov 	bx, [bp + 4]
	cmp 	word [ds:bx + di], 0
	jz	LABEL_DBIDX_NOTHING_TO_CP

	mov	ax, [ds:bx + di]			;将[ds:bx + di]索引指向的块复制到es:bx
	inc 	ax
	mov	cl, 1
	pop 	bx
	call	ReadSector			;读取loader.bin的inode所在的块到BaseOfLoader:OffsetOfLoader所指向的缓冲区
	add	bx, 512				;将文件的一块复制到内存后bx+512指向新的空闲空间
	mov	ax, [fileSize]
	sub	ax, 512
	mov	[fileSize], ax			;[fileSize] -= 512

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
LABEL_L1IDBIDX_LOOP:
	mov 	ax, [bp + 6]
	mov 	dl, 2
	mul	dl
	cmp 	di, ax
	jz	LABEL_L1IDBIDX_END

	cmp 	word [fileSize], 0
	jle	LABEL_L1IDBIDX_END

	push 	bx
	push 	es

	mov	bx, [bp + 4]
	cmp 	word [ds:bx + di], 0
	jz	LABLE_L1IDBIDX_NOTHING_TO_CP
	mov 	ax, [ds:bx + di]
	inc 	ax
	mov	cl, 1
	mov	dx, cs
	mov 	es, dx				;es = cs
	mov 	bx, bp 				;bx = bp - 512
	sub 	bx, 512
	call	ReadSector

	pop	es
	pop	bx

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
LABEL_L2IDBIDX_LOOP:
	mov 	ax, [bp + 6]
	mov 	dl, 2
	mul	dl
	cmp 	di, ax
	jz 	LABEL_L2IDBIDX_END

	cmp 	word [fileSize], 0
	jle	LABEL_L2IDBIDX_END

	push	bx
	push	es

	mov	bx, [bp + 4]
	cmp 	word [ds:bx + di], 0
	jz	LABEL_L2IDBIDX_NOTHING_TO_CP
	mov 	ax, [ds:bx + di]
	inc 	ax
	mov	cl, 1
	mov	dx, cs
	mov 	es, dx				;es = cs
	mov 	bx, bp 				;bx = bp - 512
	sub 	bx, 512
	call	ReadSector

	pop 	bx
	pop	es


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
; 函数名: DispStr
;----------------------------------------------------------------------------
; 作用:
;	显示一个字符串, 函数开始时 dh 中应该是字符串序号(0-based)
DispStr:
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
	mov	bx, 000ch		; 页号为0(BH = 0) 黑底红字(BL = 0ch)
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

times 	510-($-$$)+302	db	0	; 填充剩下的空间，使生成的二进制代码恰好为512字节
dw 	0xaa55				; 结束标志