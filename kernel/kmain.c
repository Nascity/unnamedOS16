#include "inc/inc.h"

extern struct
{
	int count;
	pcb_t* top;	// this pointer points to the top of the stack
	pcb_t blocks[MAX_PROCESS];
} pcb_stack;

kobj_mem AllocHeapMem(int count);
bool FreeHeapMem(kobj_mem komem);
void PrintMemAllocState(void);

void main(void)
{
	char* test = "testtestsets";
	char temp[16];

	printline("Kernel successfully loaded.");
	process_init();
	syscall_init();

	printline("%d %d", sizeof(date_entry), sizeof(dir_entry));

	AllocHeapMem(16);
	AllocHeapMem(16);
	printline("%d", WriteHeapMem(0, test, 11, 4));
	WriteHeapMem(1, test, 10, 1);
	printline("%d", ReadHeapMem(0, temp, 11, 8));
	printline(temp);

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

bool WriteHeapMem(kobj_mem komem, void* buffer, int offset, int count)
{
	int cnt = count;
	int off = offset;
	int buf = buffer;
	int km = komem;
	asm("int 0x24");
}

bool ReadHeapMem(kobj_mem komem, void* buffer, int offset, int count)
{
	int cnt = count;
	int off = offset;
	int buf = buffer;
	int km = komem;
	asm("int 0x25");
}

void PrintMemAllocState(void)
{
	int* ptable = pcb_stack.blocks[0].mem_alloc_table.memstate_16B;
	int i;

	for(i = 0; i < 16; i++)
	{
		printnumber(ptable[i]);
		printchar(' ');
	}
	printline("");

	ptable = pcb_stack.blocks[0].mem_alloc_table.memstate_32B;

	for(i = 0; i < 16; i++)
	{
		printnumber(ptable[i]);
		printchar(' ');
	}
	printline("");

	ptable = pcb_stack.blocks[0].mem_alloc_table.memstate_64B;

	for(i = 0; i < 16; i++)
	{
		printnumber(ptable[i]);
		printchar(' ');
	}
	printline("");

	ptable = pcb_stack.blocks[0].mem_alloc_table.memstate_128B;

	for(i = 0; i < 8; i++)
	{
		printnumber(ptable[i]);
		printchar(' ');
	}
	printline("");

	ptable = pcb_stack.blocks[0].mem_alloc_table.memstate_256B;

	for(i = 0; i < 8; i++)
	{
		printnumber(ptable[i]);
		printchar(' ');
	}
	printline("");
}
