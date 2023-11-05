#ifndef __UOSAPI_H__
#define __UOSAPI_H__

/* ---------- #defines ---------- */
#define NULL	(void*)0

#define	TRUE	1
#define FALSE	0

#define TEXTMODE_SCREEN_WIDTH	80
#define TEXTMODE_SCREEN_HEIGHT	25

#define FOREGROUND_COLOR_BLACK		0x00
#define FOREGROUND_COLOR_BLUE		0x01
#define FOREGROUND_COLOR_GREEN		0x02
#define FOREGROUND_COLOR_CYAN		0x03
#define FOREGROUND_COLOR_RED		0x04
#define FOREGROUND_COLOR_MAGENTA	0x05
#define FOREGROUND_COLOR_BROWN		0x06
#define FOREGROUND_COLOR_LIGHT_GRAY	0x07

#define FOREGROUND_COLOR_DARK_GRAY	0x08
#define FOREGROUND_COLOR_LIGHT_BLUE	0x09
#define FOREGROUND_COLOR_LIGHT_GREEN	0x0A
#define FOREGROUND_COLOR_LIGHT_CYAN	0x0B
#define FOREGROUND_COLOR_LIGHT_RED	0x0C
#define FOREGROUND_COLOR_LIGHT_MAGENTA	0x0D
#define FOREGROUND_COLOR_YELLOW		0x0E
#define FOREGROUND_COLOR_WHITE		0x0F

#define BACKGROUND_COLOR_BLACK		0x00
#define BACKGROUND_COLOR_BLUE		0x10
#define BACKGROUND_COLOR_GREEN		0x20
#define BACKGROUND_COLOR_CYAN		0x30
#define BACKGROUND_COLOR_RED		0x40
#define BACKGROUND_COLOR_MAGENTA	0x50
#define BACKGROUND_COLOR_BROWN		0x60
#define BACKGROUND_COLOR_LIGHT_GRAY	0x70

#define UP_ARROW_SCAN_CODE	0x48
#define LEFT_ARROW_SCAN_CODE	0x4B
#define RIGHT_ARROW_SCAN_CODE	0x4D
#define DOWN_ARROW_SCAN_CODE	0x50
#define BACKSPACE_SCAN_CODE	0x0E
#define ENTER_SCAN_CODE		0x1C

#define GET_ASCII_CODE(ch)	((ch) & 0xFF)
#define GET_SCAN_CODE(ch)	(((ch) >> 8) & 0xFF)

#define GETSTRING_BUFFER_COUNT_LIMIT	64

#define TAB_SIZE		4

#define FILE_MAX_NAME		8
#define FILE_MAX_EXT		3

#define FILE_OPEN_WRITE		0x01
#define FILE_OPEN_READ		0x02

#define FILE_CREATE_READONLY	0x01
#define FILE_CREATE_HIDDEN	0x02
#define FILE_CREATE_SYSTEM	0x04
#define FILE_CREATE_SUBDIR	0x08

#define DIR_ENTRY_ATTRIB_READONLY	0x01
#define DIR_ENTRY_ATTRIB_HIDDEN		0x02
#define DIR_ENTRY_ATTRIB_SYSTEM		0x04
#define DIR_ENTRY_ATTRIB_SUBDIR		0x08
#define DIR_ENTRY_ATTRIB_USED		0x80

#define INVALID_KOBJIO		((KOBJIO)0xFFFF)

#include "uostypes.h"

/* ---------- Entrypoint API ---------- */
INT
UosMain(
	PTR	pCmdLine,
	);

/* ---------- Screen IO API ---------- */
VOID
PrintChar(
	CHAR	chCharacter
	);
VOID
PrintCharAttrib(
	CHAR	chCharacter,
	BYTE	bAttrib
	);
VOID
Print(
	STRING	strString
     );
VOID
PrintFormat(
	STRING	strFormat,
	...
	);
VOID
PrintAttrib(
	STRING	strString,
	BYTE	bAttrib
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
UINT
GetCursorPos(
	VOID
	);
VOID
SetCursorPos(
	UINT	uiCursorPos
	);
VOID
SetCursorPosEx(
	BYTE	bColumn,
	BYTE	bRow
	);
VOID
CursorStep(
	BOOL	bForward
	);

/* ---------- Keyboard API ---------- */
CHAR
GetChar(
	VOID
       );
BOOL
GetString(
	STRING	strBuffer,
	UINT	uiBufferCount
	);
UINT
GetCharWithScanCode(
	VOID
	);

/* ---------- Memory API ---------- */
PTR
AllocHeapMem(
	INT	iCount
	);
BOOL
FreeHeapMem(
	PTR	pAddr
	);
VOID
Memset(
	PTR	pTarget,
	BYTE	bVal,
	UINT	uiCount
      );
VOID
Memcpy(
	PTR	pTarget,
	PTR	pSource,
	UINT	uiCount
      );

/* ---------- String API ---------- */
INT
StringCompare(
	STRING	str1,
	STRING	str2
	);
PSTRING
ParseString(
	STRING	strParse
       );
VOID
FreeArgs(
	ARGS	args
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
