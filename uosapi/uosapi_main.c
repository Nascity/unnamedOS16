#include "inc/uosapi.h"

/* ---------- internal functions ---------- */
INT
main(
	VOID
    )
{
	PTR	pCmdLine;
	PSTRING	psArgs;
	INT	iReturn;

#asm
	mov	ax, word ptr [0xFFFE]
	mov	word ptr [bp - 6], ax
#endasm
	iReturn = UosMain(pCmdLine);
#asm
	mov	ax, word ptr [bp - 10]
	mov	sp, bp
	add	sp, #0x04
	pop	bp
	retf
#endasm
}

/* ---------- Entrypoint API ---------- */
// WIP
