#ifndef __PROCESS_H__
#define __PROCESS_H__

#define MAX_PROCESS_NAME	8
#define MAX_PROCESS		32

#define KERNEL_SPACE_START_SEGMENT	0x100
#define KERNEL_SPACE_END_SEGMENT	0x400
#define USER_SPACE_START_SEGMENT	0x400
#define USER_SPACE_END_SEGMENT		0xC00

#define ARG_LOAD_SP	0xFE00

typedef struct __pcb_t
{
	int pid;
	int start_seg;
	int seg_count;
	char name[MAX_PROCESS_NAME];

	MAT_t mem_alloc_table;
} pcb_t;

void process_init(void);
int process_start(int cs, int flags, int working_dir, char* filename, char* ext, char* args, bool* success);

int get_MAT_index(int cs);
void interprocess_read(int segment, char* dest, char* src, int size);
void interprocess_write(int segment, char* dest, char* src, int size);

#endif
