	org	400h
DispStr:
	mov 	ax, cs
	mov 	ds, ax
	mov 	es, ax

	mov	bp, Message		; ┓
	mov	ax, ds			; ┣ ES:BP = 串地址
	mov	es, ax			; ┛
	mov	cx, MessageLength	; CX = 串长度
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 000ch		; 页号为0(BH = 0) 黑底红字(BL = 0ch)
	mov	dl, 0
	int	10h			; int 10h

	jmp	$

;===================================================================
Message 		db	"This is a kernel!"
MessageLength		equ 	17
;===================================================================