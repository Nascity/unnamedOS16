#include "inc/inc.h"

int test(void);

void main(void)
{
	memmgr_t memmgr;

	printline("Kernel successfully loaded.");
	process_init();
	syscall_init();

	memmgr_init(&memmgr, 0x200);

	printline("Kernel memmgr info:");
	printline("16b:\t%x", memmgr.slice_16B[0].addr);
	printline("32b:\t%x", memmgr.slice_32B[0].addr);
	printline("64b:\t%x", memmgr.slice_64B[0].addr);
	printline("128b:\t%x", memmgr.slice_128B[0].addr);
	printline("256b:\t%x", memmgr.slice_256B[0].addr);

	halt();
}


void halt(void)
{
	printstring("halt");
	while(true);
}
