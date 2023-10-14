#include "inc/uosapi.h"

/* ---------- File API ---------- */
KOBJIO	koIoWorkingDir = 1;

KOBJIO
OpenFile(
	STRING	strName,
	STRING	strExt,
	WORD	wOpenMode
	)
{
	INT	wd = koIoWorkingDir;
	INT	unused;
#asm
	mov	si, sp
	mov	bx, word ptr [bp + 4]
	mov	cx, word ptr [bp + 6]
	mov	dx, word ptr [bp + 8]
	mov	di, word ptr [bp - 6]

	mov	ax, #0x100
	mov	ss, ax
	mov	sp, #0xC000
	mov	bp, sp

	push	si
	push	dx
	push	cx
	push	bx
	push	di

	int	0x26

	add	sp, #0x08
	pop	bp
	mov	sp, bp

	mov	bx, cs
	mov	ss, bx
#endasm
}

BOOL
CloseFile(
	KOBJIO	koIo
	)
{
	INT	ki = koIo;
	asm("int 0x27");
}

KOBJIO
GetWorkingDirectory(
	VOID
	)
{
	return koIoWorkingDir;
}

VOID
SetWorkingDirectory(
	KOBJIO	koIoDir
	)
{
	koIoWorkingDir = koIoDir;
}

BOOL
WriteFile(
	KOBJIO	koIoFile,
	PTR	pBuffer,
	INT	iOffset,
	INT	iCount
	)
{
	INT	wd = koIoWorkingDir;
	INT	unused;
#asm
	mov	si, sp
	mov	ax, word ptr [bp + 10]
	mov	es, ax
	mov	bx, word ptr [bp + 4]
	mov	cx, word ptr [bp + 6]
	mov	dx, word ptr [bp + 8]
	mov	di, word ptr [bp - 6]

	mov	ax, #0x100
	mov	ss, ax
	mov	sp, #0xC000
	mov	bp, sp

	push	si
	mov	ax, es
	push	ax
	push	dx
	push	cx
	push	bx
	push	di

	int	0x28

	add	sp, #0x0A
	pop	bp
	mov	sp, bp

	mov	bx, cs
	mov	ss, bx
#endasm
}

BOOL
ReadFile(
	KOBJIO	koIoFile,
	PTR	pBuffer,
	INT	iOffset,
	INT	iCount
	)
{
	INT	wd = koIoWorkingDir;
	INT	unused;
#asm
	mov	si, sp
	mov	ax, word ptr [bp + 10]
	mov	es, ax
	mov	bx, word ptr [bp + 4]
	mov	cx, word ptr [bp + 6]
	mov	dx, word ptr [bp + 8]
	mov	di, word ptr [bp - 6]

	mov	ax, #0x100
	mov	ss, ax
	mov	sp, #0xC000
	mov	bp, sp

	push	si
	mov	ax, es
	push	ax
	push	dx
	push	cx
	push	bx
	push	di

	int	0x29

	add	sp, #0x0A
	pop	bp
	mov	sp, bp

	mov	bx, cs
	mov	ss, bx
#endasm
}

BOOL
CreateFile(
	STRING	strName,
	STRING	strExt,
	BYTE	bAttrib
	)
{
	INT	wd = koIoWorkingDir;
	INT	unused;
#asm
	mov	si, sp
	mov	di, word ptr [bp - 6]
	mov	bx, word ptr [bp + 4]
	mov	cx, word ptr [bp + 6]
	mov	dx, word ptr [bp + 8]

	mov	ax, #0x100
	mov	ss, ax
	mov	sp, #0xC000
	mov	bp, sp

	push	si
	push	dx
	push	cx
	push	bx
	push	di

	int	0x2A

	add	sp, #0x08
	pop	bp
	mov	sp, bp

	mov	bx, cs
	mov	ss, bx
#endasm
}

BOOL
DeleteFile(
	STRING	strName,
	STRING	strExt
	)
{
	INT	wd = koIoWorkingDir;
	INT	unused;
#asm
	mov	si, sp
	mov	di, word ptr [bp - 6]
	mov	bx, word ptr [bp + 4]
	mov	cx, word ptr [bp + 6]

	mov	ax, #0x100
	mov	ss, ax
	mov	sp, #0xC000
	mov	bp, sp

	push	si
	push	cx
	push	bx
	push	di

	int	0x2B

	add	sp, #0x06
	pop	bp
	mov	sp, bp

	mov	bx, cs
	mov	ss, bx
#endasm
}
