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
	printline("Kernel successfully loaded.");
	syscall_init();
	if(!io_init())
	{
		printline("io_init failed.");
		halt();
	}
	process_init();

	printline("%d %d %d %d",
			string_n_compare("a", "bc"),
			string_n_compare("aa", "aaa"),
			string_n_compare("aa", "aa"),
			string_n_compare("asdf", "asdf"));

	halt();
}


bool string_n_compare(char* str1, char* str2, int n)
{
	int i = 0;
	bool ret = true;

	while(str1[i] && str2[i] && i <= n)
	{
		if(str1[i] != str2[i])
		{
			ret = false;
			break;
		}
		i++;
	}

	return ret;
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
