#include "inc/inc.h"


void main(void)
{
	printstring("Kernel successfully loaded.\r\n");
	process_init();

	process_start("hlt", halt);
	process_start("hlt2", halt);
	process_kill(257);
	process_start("hlt3", halt);
	process_kill(258);
	process_kill(259);
	halt();
}


void halt(void)
{
	while(true);
}
