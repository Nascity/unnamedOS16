#include "inc/uosapi.h"

/* ---------- internal functions ---------- */
static
ARGS
_mkargs(
	PTR	pCmdLine
       );

INT
main(
	VOID
    )
{
	PTR	pCmdLine;
	ARGS	args;
	INT	iReturn;

#asm
	mov	ax, word ptr [bp + 8]
	mov	word ptr [bp - 6], ax
#endasm
	args = _mkargs(pCmdLine);
	iReturn = UosMain(pCmdLine, args);
#asm
	mov	ax, word ptr [bp - 10]
	mov	sp, bp
	add	sp, #0x04
	pop	bp
	retf
#endasm
}

static
ARGS
_mkargs(
	PTR	pCmdLine
       )
{

}

/* ---------- Entrypoint API ---------- */
