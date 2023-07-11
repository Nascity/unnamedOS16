#ifndef __PROCESS_H__
#define __PROCESS_H__

#define MAX_PROCESS	64

typedef struct __pcb_t
{
	int pid;
	char* name;

	int ip;
	int sp;
} pcb_t;

void process_init(void);
void process_start(char* name, int ip);
void process_kill(int pid);

#endif
