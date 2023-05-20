void printchar(ch);

void main()
{
	printchar('N');
}

void printchar(ch)
{
#asm
	push	ax
	push	bx
	mov	ah, 0x0e
	mov	bx, sp
	mov	al, [bx + 2]
	xor	bx, bx
	int	0x10
#endasm
}


