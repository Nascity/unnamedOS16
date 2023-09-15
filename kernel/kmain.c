#include "inc/inc.h"
#include "../uosapi/inc/uosapi.h"

void init(void);


void main(void)
{
	KOBJIO	koio;
	CHAR	test[10];

	printline("Kernel successfully loaded.");
	init();

	CreateFile("test0", "txt", 0);
	CreateFile("test1", "txt", 0);
	CreateFile("test2", "txt", 0);

	DeleteFile("test1", "txt");
	DeleteFile("test2", "txt");

	CreateFile("test3", "txt", 0);
	CreateFile("test4", "txt", 0);
	CreateFile("test5", "txt", 0);

	halt();
}


void init(void)
{
	syscall_init();
	if(!io_init())
	{
		printline("io_init failed.");
		halt();
	}
	process_init();
}


void halt(void)
{
	printstring("halt");
	while(true);
}
