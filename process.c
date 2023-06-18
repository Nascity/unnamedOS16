#include "inc/inc.h"

static struct
{
	pcb_t* head;
	pcb_t* tail;
	pcb_t blocks[MAX_PROCESS];
	int count;
} pcb_list;

int ticks;

void scheduler(void);
void int0x8h_handler(void);
int original_handler_cs;
int original_handler_ip;

void pcb_list_init(void);



void process_init(void)
{
	// Initialize PCB list
	pcb_list_init();

#asm
	lea	bx, _int0x8h_handler

	xor	ax, ax
	mov	ds, ax

	mov	si, #0x0020
	push	word ptr [si]
	mov	word ptr [si], bx

	inc	si
	inc	si

	push	word ptr [si]
	mov	ax, #0x0100
	mov	word ptr [si], ax
	mov	ds, ax

	pop	ax
	mov	word ptr [_original_handler_cs], ax
	pop	ax
	mov	word ptr [_original_handler_ip], ax
#endasm

	printstring("Original 0x8 handler %x:%x\r\n", original_handler_cs, original_handler_ip);
	// Set timer
	ticks = TIME_QUANTUM_TICKS;
}


void int0x8h_handler(void)
{
	asm("cli");

	printstring("INT 08H\r\n");
	scheduler();

#asm
	mov	al, #0x20
	out	#0x20, al
	iret
#endasm
}


void scheduler(void)
{
	ticks--;
	if(!ticks)
	{
		ticks = TIME_QUANTUM_TICKS;
	}

	printstring("scheduler exit!\r\n");
}

// -----------------------------------------------------------------------
// PCB operations

void pcb_list_init(void)
{
	int i;

	// Set head and tail
	pcb_list.head = &(pcb_list.blocks[0]);
	pcb_list.tail = &(pcb_list.blocks[MAX_PROCESS - 1]);

	// Initialize blocks
	for (i = 0; i < MAX_PROCESS - 1; i++)
	{
		pcb_list.blocks[i].next = &(pcb_list.blocks[i + 1]);
		pcb_list.blocks[i].state = unused;
	}
	pcb_list.blocks[MAX_PROCESS - 1].next = NULL;
	pcb_list.blocks[MAX_PROCESS - 1].state = unused;
	pcb_list.count = 0;
}
