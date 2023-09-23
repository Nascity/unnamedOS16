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
	INT	openmode = wOpenMode;
	INT	ext = strExt;
	INT	name = strName;
	INT	wd = koIoWorkingDir;
	asm("int 0x26");
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
	INT	count = iCount;
	INT	offset = iOffset;
	INT	buffer = pBuffer;
	INT	koio = koIoFile;
	INT	wd = koIoWorkingDir;
	asm("int 0x28");
}

BOOL
ReadFile(
	KOBJIO	koIoFile,
	PTR	pBuffer,
	INT	iOffset,
	INT	iCount
	)
{
	INT	count = iCount;
	INT	offset = iOffset;
	INT	buffer = pBuffer;
	INT	koio = koIoFile;
	INT	wd = koIoWorkingDir;
	asm("int 0x29");
}

BOOL
CreateFile(
	STRING	strName,
	STRING	strExt,
	BYTE	bAttrib
	)
{
	INT	attrib = bAttrib;
	INT	ext = strExt;
	INT	name = strName;
	INT	working_dir = koIoWorkingDir;
	asm("int 0x2A");
}

BOOL
DeleteFile(
	STRING	strName,
	STRING	strExt
	)
{
	INT	ext = strExt;
	INT	name = strName;
	INT	working_dir = koIoWorkingDir;
	asm("int 0x2B");
}
