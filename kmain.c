#include "inc/inc.h"

extern int ticks;

void main(void)
{
	int temp = 0;

	printstring("Kernel successfully loaded.\r\n");
	process_init();
	
	asm ("inc	word ptr [bp - 6]");
	printnumber(temp);

	halt();
}


void halt(void)
{
	printstring("halt");
	while(true);
}
