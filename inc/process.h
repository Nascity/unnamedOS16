#ifndef __PROCESS_H__
#define __PROCESS_H__

#define MAX_PROCESS		64
#define TIME_QUANTUM_TICKS	2

#define PCB_UNUSED	0
#define PCB_WAITING	1
#define PCB_RUNNING	2

typedef int pstate_t;

typedef struct __pcb_t
{
	int pid;
	char* name;
	pstate_t state;

	int ip;
	// ax, bx, cx, dx
	// bp, sp, di, si
	// eflags
	int registers[9];

	struct __pcb_t* next;
	struct __pcb_t* prev;
} pcb_t;

void process_init(void);
bool process_start(char* name, int ip);
bool process_kill(int pid);

#endif
