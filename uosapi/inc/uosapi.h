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
typedef char		BOOL;
typedef char		CHAR;
typedef unsigned char	BYTE;
typedef int		INT;
typedef int*		PINT;
typedef void*		PTR;
typedef unsigned short*	PWORD;
typedef char*		STRING;
typedef unsigned int	UINT;
typedef unsigned short	WORD;
#define VOID		void

// Structs
typedef struct __TIME_ENTRY
{
	BYTE	bYear;
	BYTE	bMonth;
	BYTE	bDay;
	BYTE	bHour;
	BYTE	bMinute;
	BYTE	bSecond;
} TIME_ENTRY, *PTIME_ENTRY;

// Kernel object type definition
typedef int KOBJIO;
typedef int KOBJMEM;

/* ---------- Screen IO API ---------- */
VOID
PrintChar(
	CHAR	chCharacter
	);
VOID
PrintFormat(
	STRING	strFormat,
	...
	);
VOID
PrintHex(
	UINT	iNumber,
	BOOL	bCapitalize,
	BOOL	bPrefix
	);
VOID
PrintNumber(
	INT	iNumber
	);

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

/* ---------- Process API ---------- */
BOOL
CreateProcess(
	STRING	strName,
	STRING	strExt,
	STRING	strArgs,
	PINT	piReturn
	);
#endif
