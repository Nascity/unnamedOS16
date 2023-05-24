#include "inc/io.h"

void main(void)
{
	asm "sti";

	printstring("Kernel successfully loaded.\r\n");
	while(1);
}

