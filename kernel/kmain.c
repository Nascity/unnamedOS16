#include "inc/inc.h"
#include "inc/uosapi.h"

void init(void);


void main(void)
{
	KOBJIO	koio;
	CHAR	test[10];

	printline("Kernel successfully loaded.");
	init();

	koio = OpenFile("testfile", "sys", FILE_OPEN_READ);
	ReadFile(koio, test, 0, 9);
	printline(test);
	
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
