#include "inc/inc.h"

static struct
{
	pcb_t* top;	// this pointer points to the top of the stack
	pcb_t blocks[MAX_PROCESS];
	int count;
} pcb_stack;

bool new_process_possible(void);
void context_switch(pcb_t* from, pcb_t* to);
void save_registers(pcb_t* pcb);

void pcb_stack_init(void);

// -----------------------------------------------------------------------
// Process operations

void process_init(void)
{
	// Initialize PCB stack
	pcb_stack_init();
}

void process_start(char* name, int ip)
{
	pcb_t new_pcb;

	if(!new_process_possible()) 
	{
		printstring("Process stack is full.");
		asm ("iret");
	}

	new_pcb.pid = KERNEL_SEGMENT + pcb_stack.count;
	new_pcb.name = name;
	new_pcb.ip = ip;
	new_pcb.sp = 0;

	(pcb_stack.top)++;
	pcb_stack.blocks[pcb_stack.count] = new_pcb;
	(pcb_stack.count)++;

	printstring("Process pid %x started", new_pcb.pid);
}


void process_kill(int pid)
{

}


bool new_process_possible(void)
{
	return pcb_stack.count <= MAX_PROCESS;
}

// -----------------------------------------------------------------------
// PCB operations

void pcb_stack_init(void)
{
	pcb_t kernel_block;

	kernel_block.pid = KERNEL_SEGMENT;
	kernel_block.name = "Kernel";
	kernel_block.ip = halt;
	kernel_block.sp = 0;

	pcb_stack.top = pcb_stack.blocks;
	pcb_stack.blocks[0] = kernel_block;
	pcb_stack.count = 1;
}
