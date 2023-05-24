#include "inc/io.h"

void printchar(int ch)
{
#asm
	push	bp
	mov	bp, sp

	push	ax
	push	bx
	mov	ah, #0x0e
	mov	al, byte ptr [bp + 4]
	mov	bx, #0x000c
	int	0x10
	pop	bx
	pop	ax

	pop	bp
#endasm
}

void printnumber(int num)
{
	// TODO: implement
}

void printstring(char* str)
{
	int i;

	while(*str)
		printchar(*(str++));
}
