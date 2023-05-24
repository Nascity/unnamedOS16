#include "inc/io.h"

void printchar(int ch)
{
#asm
	push	ax
	push	bx
	mov	ah, #0x0e
	mov	al, [bp + 4] 
	mov	bl, #0x0c
	int	0x10
	pop	bx
	pop	ax
#endasm
}
