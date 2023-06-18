#include "inc/inc.h"

void main(void)
{
	asm "sti";

	printstring("Kernel successfully loaded.\r\n");
	process_init();

	printstring("Halting...\r\n");
	while(1);
}

