#include "inc/inc.h"

static struct
{
	pcb_t* head;
	pcb_t* exec;
	pcb_t* tail;
	pcb_t* rear;
	pcb_t blocks[MAX_PROCESS];
	int count;
} pcb_list;

int ticks;

void int0x8h_handler(void);
void scheduler(void);
void dispatch(void);
int curr_ip;

bool new_process_possible(void);

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
	mov	word ptr [si], bx

	inc	si
	inc	si

	mov	ax, #0x0100
	mov	word ptr [si], ax
	mov	ds, ax
#endasm

	// Set timer
	ticks = TIME_QUANTUM_TICKS;
}


void int0x8h_handler(void)
{
#asm
	sti
	mov	bx, sp
	mov	ax, word ptr [bx]
	mov	word ptr [_curr_ip], ax
#endasm
	scheduler();
#asm
	mov	al, #0x20
	out	#0x20, al

	; < Stack diagram >
	;
	; [EFLAGS]
	; cs
	; ip
	; bp
	; di
	; si
	pop	si
	pop	di
	pop	bp

	pop	ax
	pop	cx
	popf
	push	cx
	push	ax
	cli
	retf
#endasm
}


void scheduler(void)
{
	ticks--;

	// if ticks != 0
	if(ticks) return;

	ticks = TIME_QUANTUM_TICKS;

	// if PCB list is empty
	if(!(pcb_list.count)) return;

	dispatch();
}

void dispatch(void)
{
	
}

// -----------------------------------------------------------------------
// Process operations

bool process_start(char* name, int ip)
{
	pcb_t new_PCB;

	if(!new_process_possible()) return false;

	// create new PCB
	new_PCB.pid = ((int)(pcb_list.tail->next) - (int)(pcb_list.blocks)) / sizeof(pcb_t) + KERNEL_SEGMENT;
	new_PCB.name = name;
	new_PCB.state = PCB_WAITING;
	new_PCB.ip = ip;
	new_PCB.next = pcb_list.tail->next->next;
	new_PCB.prev = pcb_list.tail;

	// assign it to the next PCB in unused queue
	*(pcb_list.tail->next) = new_PCB;
	pcb_list.tail = pcb_list.tail->next;
	(pcb_list.count)++;

	// DEBUG
	printstring("New process PID: %d, name: %s\r\n", new_PCB.pid, new_PCB.name);

	return true;
}


bool process_kill(int pid)
{
	pcb_t* del_PCB = &(pcb_list.blocks[pid - KERNEL_SEGMENT]);

	if(del_PCB->state == PCB_UNUSED) return false;

	del_PCB->state = PCB_UNUSED;
	pcb_list.rear = del_PCB;

	if(del_PCB == pcb_list.head)
	{
		pcb_list.head = pcb_list.head->next;
	}
	else if(del_PCB == pcb_list.tail)
	{
		pcb_list.tail = pcb_list.tail->prev;
	}
	else
	{
		del_PCB->prev->next = del_PCB->next;
		del_PCB->next->prev = del_PCB->prev;

		pcb_list.rear->next = del_PCB;
		del_PCB->prev = pcb_list.rear;
		pcb_list.head->prev = del_PCB;
		del_PCB->next = pcb_list.head;

		pcb_list.rear = del_PCB;
		(pcb_list.count)--;
	}

	// DEBUG
	printstring("Killed process PID %d\r\n", pid);
}


bool new_process_possible(void)
{
	return pcb_list.count <= MAX_PROCESS;
}

// -----------------------------------------------------------------------
// PCB operations

void pcb_list_init(void)
{
	int i;

	// Set head and tail
	pcb_list.head = &(pcb_list.blocks[0]);
	pcb_list.exec = &(pcb_list.blocks[0]);
	pcb_list.tail = &(pcb_list.blocks[0]);
	pcb_list.rear = &(pcb_list.blocks[MAX_PROCESS - 1]);

	// Initialize blocks
	pcb_list.blocks[0].next = &(pcb_list.blocks[1]);
	pcb_list.blocks[0].prev = &(pcb_list.blocks[MAX_PROCESS - 1]);
	pcb_list.blocks[0].state = PCB_UNUSED;
	for (i = 1; i < MAX_PROCESS - 1; i++)
	{
		pcb_list.blocks[i].next = &(pcb_list.blocks[i + 1]);
		pcb_list.blocks[i].prev = &(pcb_list.blocks[i - 1]);
		pcb_list.blocks[i].state = PCB_UNUSED;
	}
	pcb_list.blocks[MAX_PROCESS - 1].next = &(pcb_list.blocks[0]);
	pcb_list.blocks[MAX_PROCESS - 1].prev = &(pcb_list.blocks[MAX_PROCESS - 2]);
	pcb_list.blocks[MAX_PROCESS - 1].state = PCB_UNUSED;
	pcb_list.count = 0;

	// initialize first block with kernel
	pcb_list.blocks[0].pid = KERNEL_SEGMENT;
	pcb_list.blocks[0].name = "kernel";
	pcb_list.blocks[0].state = PCB_RUNNING;
	pcb_list.blocks[0].ip = halt;
}
