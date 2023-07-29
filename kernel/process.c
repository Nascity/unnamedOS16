#include "inc/inc.h"

struct
{
	int count;
	pcb_t* top;	// this pointer points to the top of the stack
	pcb_t blocks[MAX_PROCESS];
} pcb_stack;

bool new_process_possible(void);
void pcb_stack_init(void);

// -----------------------------------------------------------------------
// Process operations
void process_init(void)
{
	// Initialize PCB stack
	pcb_stack_init();

	// DEBUG
	printline("PCB stack initialized.");
}


void process_start(int return_cs, int flags, char name[MAX_PROCESS_NAME])
{
	pcb_t* new_pcb = pcb_stack.top + 1;
	pcb_t* old_pcb = pcb_stack.top;
	int* ap = &return_cs;
	int i;
	int new_pid = KERNEL_SEGMENT + pcb_stack.count * 0x0100;

	if(!new_process_possible()) 
	{
		printline("Process stack is full.");
		syscall_return();
	}

	old_pcb->ip = *(ap - 1);
	old_pcb->sp = ap - 1;

	new_pcb->pid = new_pid;
	for(i = 0; i < MAX_PROCESS_NAME; i++)
		new_pcb->name[i] = name[i];

	(pcb_stack.top)++;
	(pcb_stack.count)++;

	printline("Created process %s. (%x)", new_pcb->name, new_pid);
#asm
	mov	ax, word ptr [bp - 10]
	mov	ds, ax
	mov	ss, ax

	xor	bp, bp
	xor	sp, sp

	push	ax
	push	#0x00
	retf
#endasm
}


void process_kill(int return_cs, int flags, int return_val)
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
	char* kernel_name = "init";
	int i;

	// assigning information of the kernel
	kernel_block.pid = KERNEL_SEGMENT;
	for(i = 0; i < MAX_PROCESS_NAME; i++)
		kernel_block.name[i] = kernel_name[i];
	kernel_block.ip = halt;
	kernel_block.sp = 0;
	MAT_init(&(kernel_block.mem_alloc_table), kernel_block.pid);

	pcb_stack.top = pcb_stack.blocks;
	pcb_stack.blocks[0] = kernel_block;
	pcb_stack.count = 1;
}
