#include "../uosapi/inc/uosapi.h"

INT
UosMain(
	PTR	pCmdLine,
	ARGS	args
       )
{
	INT	i;
	char*	test;

	for (i = 0; i < args.uiArgCount; i++)
		PrintFormat("%s\n", args.pArgs[i].strArg);
	OpenFile(test = "testfile", "sys", FILE_OPEN_READ);
	return 0;
}
