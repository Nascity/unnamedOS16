#include "inc/io.h"

void main()
{
	asm "sti";

	printchar('H');

	while(1);
}

