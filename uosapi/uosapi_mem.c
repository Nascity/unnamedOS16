#include "inc/uosapi.h"

/* ---------- Memory API ---------- */
PTR
AllocHeapMem(
	INT	iCount
	)
{
	INT	unused;
#asm
	mov	si, sp
	mov	bx, word ptr [bp + 4]

	mov	ax, #0x100
	mov	ss, ax
	mov	sp, #0xC000
	mov	bp, sp

	push	si
	push	bx

	int	0x22

	add	sp, #0x02
	pop	bp
	mov	sp, bp

	mov	bx, cs
	mov	ss, bx
#endasm
}

BOOL
FreeHeapMem(
	PTR	pAddr
	)
{
	INT	unused;
#asm
	mov	si, sp
	mov	bx, word ptr [bp + 4]

	mov	ax, #0x100
	mov	ss, ax
	mov	sp, #0xC000
	mov	bp, sp

	push	si
	push	bx

	int	0x23

	add	sp, #0x02
	pop	bp
	mov	sp, bp

	mov	bx, cs
	mov	ss, bx
#endasm
}

VOID
Memcpy(
	PTR	pTarget,
	PTR	pSource,
	INT	iCount
      )
{
	INT	i;

	for (i = 0; i < iCount; i++)
		pTarget[i] = pSource[i];
}
