%ifdef	_BOOT_DEBUG_
	org  0100h			; 调试状态, 做成 .COM 文件, 可调试
%else
	org  07c00h			; Boot 状态, Bios 将把 Boot Sector 加载到 0:7C00 处并开始执行
%endif

;================================================================================================
%ifdef	_BOOT_DEBUG_
BaseOfStack		equ	0100h	; 调试状态下堆栈基地址(栈底, 从这个位置向低地址生长)
%else
BaseOfStack		equ	7c00h	; Boot状态下堆栈基地址(栈底, 从这个位置向低地址生长)
%endif

BaseOfMBR		equ	9000h	; LOADER.BIN 被加载到的位置 ----  段地址
OffsetOfMBR		equ	0000h	; LOADER.BIN 被加载到的位置 ---- 偏移地址
SecPerTrk		equ	18	;每磁道扇区数
BlockSize 		equ	512	;每块512字节
PARTITION_BEGIN_BLOCK 	equ	3 	;软盘内容从第三个扇区开始，0号扇区为mbr，1-2号扇区为嵌入式扇区
EMBEDDED_SECTOR	equ 	1 	;1-2号扇区为嵌入式扇区
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

	mov	ax, BaseOfMBR
	mov	es, ax			; es <- BaseOfMBR
	mov	bx, OffsetOfMBR	; bx <- OffsetOfMBR	于是, es:bx = BaseOfMBR:OffsetOfMBR
	mov	ax, EMBEDDED_SECTOR 	;嵌入式扇区为1-2号扇区
	mov	cl, 2
	call	ReadSector		;读取嵌入式扇区到 BaseOfMBR:OffsetOfMBR指向的缓冲区中

	jmp	BaseOfMBR:OffsetOfMBR


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

times 	510-($-$$)	db	0	; 填充剩下的空间，使生成的二进制代码恰好为512字节
dw 	0xaa55				; 结束标志