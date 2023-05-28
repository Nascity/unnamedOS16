#include "inc/inc.h"

void main(void)
{
	asm "sti";

	printstring("Kernel successfully loaded.\r\n");
	printstring("%d %x %c", 123, 456, 'c');

	while(1);
}

