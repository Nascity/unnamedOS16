bits 16

init:
	; initialize segment registers
	mov	ax, 0x07c0
	mov	ds, ax
	mov	ax, StackSeg
	mov	ss, ax
	mov	sp, StackOffset

	; clear screen
	mov	ax, 0x0003
	int	0x10

	; clear direction flag
	cld

	; read kernel from disc
	mov	ax, KernelSeg
	mov	es, ax
	mov	ah, 0x02
	mov	al, KernelSegmentCount
	mov	bx, KernelOffset
	xor	ch, ch
	mov	cl, 0x02
	xor	dh, dh
	xor	dl, dl
	int	0x13

	; check whether any error have occured
	jc	error

	; set stack pointer to 0100:0000
	mov	ax, KernelSeg
	mov	ds, ax
	mov	ss, ax
	xor	sp, sp

	; jump to 0100:0000
	cli
	push	es
	push	bx
	retf

error:
	mov	al, ah
	add	al, 0x30
	mov	ah, 0x0e
	int	0x10
	mov	si, error_msg
error.loop:
	lodsb
	or	al, al
	jz	halt
	mov	ah, 0x0e
	int	0x10
	jmp	error.loop
halt:
	jmp	$

error_msg	db	"Kernel not found.", 0	

KernelSeg		equ	0x0100
KernelOffset		equ	0x0000
KernelSegmentCount	equ	10

StackSeg		equ	0x9000
StackOffset		equ	0x0000

times	510 - ($ - $$)	db	0
dw	0xaa55
