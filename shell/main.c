#include "../uosapi/inc/uosapi.h"
#include "inc/shell.h"
#include "inc/shelltypes.h"

/* ---------- Global Variable ---------- */
INT
iShellExitCode;

OS_VERSION_INFO
ovi;

/* ---------- UosMain ---------- */
INT
UosMain(
	PTR	pCmdLine,
       )
{
	CHAR	strCmdBuffer[COMMAND_BUFFER_SIZE];
	PSTRING	psArgs;

	psArgs = ParseString(pCmdLine);

	if (psArgs[3])
	{
		PrintFormat("%s", INVALID_ARGUMENT_ERROR_MSG);
		return -1;
	}
	ovi.strOSName = psArgs[0];
	ovi.strKernelName = psArgs[1];
	ovi.strKernelVer = psArgs[2];
	while (TRUE)
	{
		Memset(strCmdBuffer, 0, COMMAND_BUFFER_SIZE);
		PrintChar('\n');
		if (!GetCommand(strCmdBuffer))
		{
			PrintFormat(COMMAND_LONG_ERROR_MSG);
			continue;
		}
		if (!InterpretCommand(strCmdBuffer))
			break;
	}
	return iShellExitCode;
}
