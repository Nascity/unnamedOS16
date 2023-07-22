#ifndef __PROCESS_H__
#define __PROCESS_H__

#define MAX_PROCESS		16
#define MAX_PROCESS_NAME	16

typedef struct __pcb_t
{
	int pid;
	char name[MAX_PROCESS_NAME];

	int ip;
	int sp;
	// flags
	// cs
	// ip	<--- sp
	
	MAT_t mem_alloc_table;
} pcb_t;

void process_init(void);
void process_start(int return_cs, int flags, char name[MAX_PROCESS_NAME]);
void process_kill(int return_cs, int flags, int return_val);

#endif
