#include "inc/uosapi.h"

/* ---------- Memory API ---------- */
KOBJMEM
AllocHeapMem(
	INT	iCount
	)
{
	INT count = iCount;
	asm("int 0x22");
}

BOOL
FreeHeapMem(
	KOBJMEM	koMem
	)
{
	KOBJMEM km = koMem;
	asm("int 0x23");
}

BOOL
WriteHeapMem(
	KOBJMEM	koMem,
	PTR	pBuffer,
	INT	iOffset,
	INT	iCount
	)
{
	INT	count = iCount;
	INT	offset = iOffset;
	INT	buffer = pBuffer;
	INT	km = koMem;
	asm("int 0x24");
}

BOOL
ReadHeapMem(
	KOBJMEM	koMem,
	PTR	pBuffer,
	INT	iOffset,
	INT	iCount
	)
{
	INT	count = iCount;
	INT	offset = iOffset;
	INT	buffer = pBuffer;
	INT	km = koMem;
	asm("int 0x25");
}
