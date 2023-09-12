#include "inc/inc.h"
#include "inc/uosapi.h"

void init(void);


void main(void)
{
	KOBJIO	koio;
	CHAR	test[10];

	printline("Kernel successfully loaded.");
	init();

	CreateFile("dirtest", "", FILE_CREATE_SUBDIR);

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
