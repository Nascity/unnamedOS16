#include "inc/inc.h"

extern int ticks;

void main(void)
{
	asm "sti";

	int temp;

	printstring("Kernel successfully loaded.\r\n");
	printstring("%s test %d %d %d %x %c\r\n", "printstring", 123, 0, -456, 123, 'a');
	process_init();

	printstring("Halting...\r\n");
	
	while(true)
	{
		if(ticks != temp)
		{
			printnumber(ticks);
			temp = ticks;
		}
	}
}

