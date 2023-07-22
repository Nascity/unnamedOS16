#include "inc/inc.h"

extern struct
{
	int count;
	pcb_t* top;	// this pointer points to the top of the stack
	pcb_t blocks[MAX_PROCESS];
} pcb_stack;

kobj_mem AllocHeapMem(int count);

void main(void)
{
	int* ptable;

	printline("Kernel successfully loaded.");
	process_init();
	syscall_init();

	AllocHeapMem(31);
	AllocHeapMem(18);

	ptable = pcb_stack.blocks[0].mem_alloc_table.memstate_16B;

	printline("%d", FreeHeapMem((kobj_mem)2));

	printline("%d %d %d %d %d %d",
		ptable[0], ptable[1],
		ptable[2], ptable[3],
		ptable[4], ptable[5]);

	halt();
}


void halt(void)
{
	printstring("halt");
	while(true);
}

kobj_mem AllocHeapMem(int count)
{
	int cnt = count;
	asm("int 0x22");
}

bool FreeHeapMem(kobj_mem komem)
{
	kobj_mem km = komem;
	asm("int 0x23");
}
