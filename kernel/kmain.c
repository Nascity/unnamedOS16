#include "inc/inc.h"
#include "inc/uosapi.h"

void init(void);


void main(void)
{
	CHAR	buffer[40];
	KOBJIO	koio;

	printline("Kernel successfully loaded.");
	init();

	koio = OpenFile("testfile", "sys", FILE_OPEN_READ);
	ReadFile(koio, buffer, 0, 38);
	printline(buffer);

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
