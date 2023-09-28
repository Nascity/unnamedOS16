#include "inc/inc.h"
#include "../uosapi/inc/uosapi.h"

void init(void);


void main(void)
{
	int	i;
	int	ret;

	printline("Kernel successfully loaded.");
	init();

	ret = CreateProcess("shell", "sys", "arg test", &i);
	printline("ret: %d, suc: %d", ret, i);

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
