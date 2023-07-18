#include "inc/inc.h"

int get_ds(void)
{
#asm
	mov	ax, ds
	ret
#endasm
}


int get_es(void)
{
#asm
	mov	ax, es
	ret
#endasm
}


int get_ss(void)
{
#asm
	mov	ax, ss
	ret
#endasm
}


void set_ds(int ds)
{
#asm
	mov	bx, sp
	mov	ax, word ptr [bx + 2]
	mov	ds, ax
	ret
#endasm
}


void set_es(int es)
{
#asm
	mov	bx, sp
	mov	ax, word ptr [bx + 2]
	mov	es, ax
	ret
#endasm
}


void set_ss(int ss)
{
#asm
	mov	bx, sp
	mov	ax, word ptr [bx + 2]
	mov	ss, ax
	ret
#endasm
}
