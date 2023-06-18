#ifndef __PROCESS_H__
#define __PROCESS_H__

#define MAX_PROCESS	64
#define TIME_QUANTUM_TICKS	25

typedef enum { unused, waiting, running } pstate_t;

typedef struct __pcb_t
{
	int pid;
	pstate_t state;

	// ax, bx, cx, dx
	// bp, sp, di, si
	// cs, ds, ss, es
	// ip, eflags
	int registers[14];
	struct __pcb_t* next;
} pcb_t;

void process_init(void);
bool process_start(void* entry);
bool process_kill(int pid);

#endif
