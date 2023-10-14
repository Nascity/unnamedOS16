#include "../uosapi/inc/uosapi.h"

INT
UosMain(
	PTR	pCmdLine,
	ARGS	args
       )
{
	INT	i;
	KOBJIO	koio;

	PrintFormat("koio: %d\n", koio = OpenFile("testfile", "sys", FILE_OPEN_WRITE));
	PrintFormat("ret: %d\n", WriteFile(koio, "teststring", 1, 10));
	PrintFormat("cf: %d\n", CreateFile("test", "txt", FILE_CREATE_READONLY));
	PrintFormat("df: %d\n", DeleteFile("testfile", "sys"));
	return 1234;
}
