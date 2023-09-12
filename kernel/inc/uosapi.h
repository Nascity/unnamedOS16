#ifndef __UOSAPI_H__
#define __UOSAPI_H__

/* ---------- #defines ---------- */
#define	TRUE	1
#define FALSE	0

#define FILE_OPEN_WRITE		0x01
#define FILE_OPEN_READ		0x02

#define FILE_CREATE_READONLY	0x01
#define FILE_CREATE_HIDDEN	0x02
#define FILE_CREATE_SYSTEM	0x04
#define FILE_CREATE_SUBDIR	0x08

#define INVALID_KOBJIO		((KOBJIO)0xFFFF)
#define INVALID_KOBJMEM		((KOBJMEM)-1)

/* ---------- typedefs ---------- */
// Basic type definition
typedef char	BOOL;
typedef char	CHAR;
typedef char	BYTE;
typedef int	INT;
typedef void*	PTR;
typedef char*	STRING;
typedef short	WORD;
#define VOID	void

// Structs
typedef struct __TIME_ENTRY
{
	BYTE	year;
	BYTE	month;
	BYTE	day;
	BYTE	hour;
	BYTE	minute;
	BYTE	second;
} TIME_ENTRY, *PTIME_ENTRY;

// Kernel object type definition
typedef int KOBJIO;
typedef int KOBJMEM;

/* ---------- Memory API ---------- */
KOBJMEM
AllocHeapMem(
	INT	iCount
	);
BOOL
FreeHeapMem(
	KOBJMEM	koMem
	);
BOOL
WriteHeapMem(
	KOBJMEM	koMem,
	PTR	pBuffer,
	INT	iOffset,
	INT	iCount
	);
BOOL
ReadHeapMem(
	KOBJMEM	koMem,
	PTR	pBuffer,
	INT	iOffset,
	INT	iCount
	);

/* ---------- File API ---------- */
KOBJIO
OpenFile(
	STRING	strName,
	STRING	strExt,
	WORD	wOpenMode
	);
BOOL
CloseFile(
	KOBJIO	koIo
	);
KOBJIO
GetWorkingDirectory(
	VOID
	);
VOID
SetWorkingDirectory(
	KOBJIO	koIoDir
	);
BOOL
WriteFile(
	KOBJIO	koIoFile,
	PTR	pBuffer,
	INT	iOffset,
	INT	iCount
	);
BOOL
ReadFile(
	KOBJIO	koIoFile,
	PTR	pBuffer,
	INT	iOffset,
	INT	iCount
	);
BOOL
CreateFile(
	STRING	strName,
	STRING	strExt,
	BYTE	bAttrib
	);
#endif
