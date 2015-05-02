%include	"load.inc"
%include	"lofs.inc"

%ifdef	DEBUG
	org  0100h			; 调试状态, 做成 .COM 文件, 可调试
BaseOfStack	equ	0100h	; 调试状态下堆栈基地址(栈底, 从这个位置向低地址生长)
%else
	org  07c00h			; Boot 状态, Bios 将把 Boot Sector 加载到 0:7C00 处并开始执行
BaseOfStack	equ	7c00h	; Boot状态下堆栈基地址(栈底, 从这个位置向低地址生长)
%endif


LABEL_START:	
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BaseOfStack
	
	; 清屏
	mov	ax, 0600h		; AH = 6,  AL = 0h
	mov	bx, 0700h		; 黑底白字(BL = 07h)
	mov	cx, 0			; 左上角: (0, 0)
	mov	dx, 0184fh		; 右下角: (80, 50)
	int	10h			; int 10h

;	call	CheckInt13h

	mov	dh, 0			; "Booting  "
	call	DispStr			; 显示字符串
;传统13h获取硬盘参数---------------------
;	mov	ah, 8h
;	mov	dl, 80h
;	int 	13h
;
;	and	cl, 00111111b
;	mov	dl, cl
;	call	DispDecmo
;
;	mov	dl, ch
;	call	DispDecmo
;
;	mov	dl, dh
;	call	DispDecmo
;---------------------------------------

	xor	ah, ah	; ┓
	xor	dl, 80h	; ┣ 硬盘复位
	int	13h	; ┛

	mov	ax, BaseOfEmbeddedSector
	mov	es, ax			; es <- BaseOfEmbeddedSector
	mov	bx, OffsetOfEmbeddedSector	; bx <- OffsetOfEmbeddedSector	于是, es:bx = BaseOfEmbeddedSector:OffsetOfEmbeddedSector
	mov	ax, EMBEDDED_SECTOR 	;嵌入式扇区为1-2号扇区
	mov	cl, 2
	call	ReadSector		;读取嵌入式扇区到 BaseOfEmbeddedSector:OffsetOfEmbeddedSector指向的缓冲区中

	mov	dh, 1			; "Ready.   "
	call	DispStr			; 显示字符串

	jmp	BaseOfEmbeddedSector:OffsetOfEmbeddedSector

;;----------------------------------------------------------------------------
;; 函数名: CheckInt13h
;;----------------------------------------------------------------------------
;; 作用:
;;	检查是否能使用拓展功能
;CheckInt13h:
;	mov	ah, 41h
;	mov	bx, 55aah
;	mov	dl, 80
;	int	13h
;	cmp	bx, 0aa55h
;	jnz	NoSupport
;	mov	dh, 3
;	call	DispStr
;	ret
;NoSupport:
;	mov	dh, 4
;	call	DispStr
;	ret


;===========================================================================
DiskAddressPacket:
	PacketSize	db	10h
	Reserved	db	0
	BlockCount	dw	0
	BufferOff	dw	0
	BufferSeg	dw	0
	BlockNum	dq	0
;===========================================================================





;----------------------------------------------------------------------------
; 函数名: DispDecmo
;----------------------------------------------------------------------------
; 作用:
;	显示十进制整数, dl中存放要输出的数字
DispDecmo:
	push	ax
	push	bx
	push	cx
	push	dx

	mov	al, dl
	mov	cx, 3
loop1:
	mov	ah, 0
	mov	dl, 10
	div	dl
	add	ah, 48
	mov	bl, ah
	mov	bh, 0
	push	bx
	loop	loop1

	mov	cx, 3
loop2:
	pop	ax
	mov	ah, 0eh
	int	10h
	loop	loop2
	
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	ret

;----------------------------------------------------------------------------
; 函数名: DispStr
;----------------------------------------------------------------------------
; 作用:
;	显示一个字符串, 函数开始时 dh 中应该是字符串序号(0-based)
DispStr:
	push	ax
	push	bx
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
	mov	dl, 0
	int	10h			; int 10h

	pop	dx
	pop	cx
	pop	bx
	pop	ax
	ret

;----------------------------------------------------------------------------
; 函数名: ReadSector
;----------------------------------------------------------------------------
; 作用:
;	从第 ax 个 Sector 开始, 将 cl 个 Sector 读入 es:bx 中
ReadSector:
	push	ax
	push	bx
	push	cx
	push	dx
	push	si


	mov	byte [PacketSize], 	10h
	mov	byte [Reserved], 	0
	mov	byte [BlockCount], 	cl
	mov	byte [BlockCount + 1], 	0
	mov	word [BufferOff], 	bx
	mov	word [BufferSeg], 	es
	mov	word [BlockNum], 	ax
	mov	word [BlockNum + 2],	0
	mov	dword [BlockNum + 4], 	0
READFAIL:
	mov	ah, 42h			;功能号42h读，43h写
	mov	al, 0			;写操作时有效：0为无校验写，1为有校验写
	mov	dl, 80h			;驱动号
	;数据包的段地址为ds
	mov	si, DiskAddressPacket
	int	13h
	;ah==0,cf==0表示成功,若失败ah为错误码,cf==1
	jnc	READSUCC

	push	ax
	push	bx
	push	cx
	mov	ah, 0Eh		
	mov	al, '#'		
	mov	bl, 0Fh		
	int	10h		
	pop 	cx
	pop 	bx
	pop 	ax

	mov	dl, ah
	call	DispDecmo

	jmp	READFAIL		;没有成功，就反复读，直到成功
READSUCC:	
	mov	ah, 0Eh			; ┃ 每读一个扇区就在 "Booting  " 后面打一个点, 形成这样的效果:
	mov	al, '.'			; ┃
	mov	bl, 0Fh			; ┃ Booting ......
	mov 	cl, byte [bp - 2]
	mov 	ch, 0
	int	10h			; ┃

	pop	si
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	ret

;============================================================================
;字符串
;----------------------------------------------------------------------------
; 为简化代码, 下面每个字符串的长度均为 MessageLength
MessageLength		equ	9
BootMessage:		db	"Booting  "; 9字节, 不够则用空格补齐. 序号 0
Message1		db	"Ready.   "; 9字节, 不够则用空格补齐. 序号 1
Message2		db	"Head:    "
Message3		db	"NoSupport"
Message4		db	"Support  "
Message5		db	"readSucce"
Message6		db	"readFail "
;============================================================================


times 	510-($-$$)	db	0	; 填充剩下的空间，使生成的二进制代码恰好为512字节
dw 	0xaa55				; 结束标志
