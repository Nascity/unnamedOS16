#include "inc/uosapi.h"

/* ---------- Process API ---------- */
extern
KOBJIO	koIoWorkingDir;

BOOL
CreateProcess(
	STRING	strName,
	STRING	strExt,
	STRING	strArgs,
	PBOOL	pbIsSuccessful
	)
{
	INT	is = pbIsSuccessful;
	INT	args = strArgs;
	INT	ext = strExt;
	INT	filename = strName;
	INT	working_dir = koIoWorkingDir;
	asm("int 0x20");
}
