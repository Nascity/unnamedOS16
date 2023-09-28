#include "inc/inc.h"

struct
{
	int count;
	int top_seg;
	pcb_t blocks[MAX_PROCESS];
} pcb_stack;

extern bool get_file_size(int working_directory_FIT_index, int FIT_index, int* filesize);
extern int get_file_cluster(int FIT_index);
extern void calculate_chs_tuple(int cluster, int* cl, int* dh);

bool new_process_possible(int process_segment_count);
void pcb_stack_init(void);

int io_open_wrapper(int working_dir, char* name, char* ext);
bool io_close_wrapper(int koio);

bool load_process(int file_cluster, int segment, int count);
void init_pcb_t(int process_seg_count, char* filename);
void process_args_copy(int sp, char* args);
void process_setup_stack(int ip, int cmdline, int stack_top);
int process_farjump(int cs, int ip, int bp);

// -----------------------------------------------------------------------
// Process operations
void process_init(void)
{
	pcb_stack_init();
}


bool new_process_possible(int process_segment_count)
{
	if (pcb_stack.count == 0)
		return true;
	return (pcb_stack.blocks[pcb_stack.count - 1].start_seg
		+ (pcb_stack.blocks[pcb_stack.count - 1].seg_count
			+ process_segment_count) * 0x20)
		<= USER_SPACE_END_SEGMENT
		&& pcb_stack.count < MAX_PROCESS;
}


// -----------------------------------------------------------------------
// PCB operations
void pcb_stack_init(void)
{
	pcb_stack.count = 0;
	pcb_stack.top_seg = USER_SPACE_START_SEGMENT;
}


// -----------------------------------------------------------------------
// Syscalls
int process_start(int cs, int flags, int working_dir, char* filename, char* ext, char* args, bool* success)
{
	int process_base_file;
	int process_base_file_size;
	int process_seg_count;
	int process_stack_offset;

	*success = 1;
	process_base_file = io_open_wrapper(working_dir, filename, ext);
	if(process_base_file == 0xFFFF
		|| !get_file_size(working_dir, process_base_file, &process_base_file_size))
	{
		*success = 0;
		asm("mov	ax, #0x00");
		syscall_return();
	}
	process_seg_count = process_base_file_size % 512 == 0 ?
				process_base_file_size / 512
				: process_base_file_size / 512 + 1;
	if(!new_process_possible(process_seg_count)
		|| !load_process(get_file_cluster(process_base_file),
			pcb_stack.top_seg,
			process_seg_count)
		|| !io_close_wrapper(process_base_file))
	{
		*success = 0;
		asm("mov	ax, #0x00");
		syscall_return();
	}

	init_pcb_t(process_seg_count, filename);

	process_stack_offset = -(pcb_stack.top_seg - process_seg_count * 512 - USER_SPACE_START_SEGMENT);
	process_args_copy(-(pcb_stack.top_seg - USER_SPACE_START_SEGMENT), args);
	process_setup_stack(((char*)process_farjump) + 0x23, -(pcb_stack.top_seg - USER_SPACE_START_SEGMENT), process_stack_offset);
	process_farjump(USER_SPACE_START_SEGMENT,
			-process_stack_offset,
			process_stack_offset - 6);
	syscall_return();
}


// -----------------------------------------------------------------------
// io.c wrappers
int io_open_wrapper(int working_dir, char* name, char* ext)
{
	int m = FILE_OPEN_READ;
	int e = ext;
	int n = name;
	int w = working_dir;
	asm("int 0x26");
}


bool io_close_wrapper(int koio)
{
	int ki = koio;
	asm("int 0x27");
}


// -----------------------------------------------------------------------
// Extras
bool load_process(int file_cluster, int segment, int count)
{
	int es = segment;
	int cnt = count;
	int cl;
	int dh;
	int ret;

	calculate_chs_tuple(file_cluster, &cl, &dh);
#asm
	mov	ax, word ptr [bp - 6]
	mov	es, ax

	mov	ah, #0x02
	mov	al, byte ptr [bp - 8]
	xor	bx, bx
	xor	ch, ch
	mov	cl, byte ptr [bp - 10]
	mov	dh, byte ptr [bp - 12]
	xor	dl, dl
	int	0x13

	jc	load_process_carry
	xor	ax, ax
	inc	ax
	jmp	load_process_end
load_process_carry:
	xor	ax, ax
load_process_end:
	mov	word ptr [bp - 14], ax
#endasm
	return ret;
}


void init_pcb_t(int process_seg_count, char* filename)
{
	pcb_t* ptarget = &(pcb_stack.blocks[pcb_stack.count]);
	int i;

	ptarget->pid = 0x100 * (pcb_stack.count + 1);
	ptarget->start_seg = pcb_stack.top_seg;
	ptarget->seg_count = process_seg_count;
	for (i = 0; i < FILE_MAX_NAME && filename[i]; i++)
		ptarget->name[i] = filename[i];
	MAT_init(&(ptarget->mem_alloc_table));
	pcb_stack.top_seg += process_seg_count * 512;
}


void process_args_copy(int sp, char* args)
{
	int i;
	int ch;	// bp - 8

	for (i = 0; args[i]; i++, sp++)
	{
		ch = args[i];
#asm
		;	si = sp
		mov	si, word ptr [bp + 4]

		;	bl = args[i]
		mov	bl, byte ptr [bp - 8]

		mov	ax, #0x400
		mov	ds, ax

		;	[0400h:si] = args[i]
		mov	byte ptr [si], bl

		mov	ax, #0x100
		mov	ds, ax
#endasm
	}
}


void process_setup_stack(int ip, int cmdline, int stack_top)
{
	int unused;
#asm
	mov	di, word ptr [bp + 4]	; di = ip
	mov	bx, word ptr [bp + 6]	; bx = cmdline
	mov	si, word ptr [bp + 8]	; si = stack_top

	mov	ax, #0x400
	mov	ds, ax

	sub	si, #0x02
	mov	word ptr [si], bx
	sub	si, #0x02
	mov	word ptr [si], #0x100
	sub	si, #0x02
	mov	word ptr [si], di

	mov	ax, #0x100
	mov	ds, ax
#endasm
}


int process_farjump(int cs, int ip, int bp)
{
	int unused;
#asm
	mov	dx, bp

	mov	cx, word ptr [bp + 4]
	mov	ax, cx
	mov	cx, word ptr [bp + 6]
	mov	bx, cx
	mov	cx, word ptr [bp + 8]
	mov	bp, cx
	mov	sp, bp

	mov	ds, ax
	mov	ss, ax
	push	dx

	pushf
	push	ax
	push	bx
	retf

process_farjump_return:
	mov	sp, bp
	sub	sp, #0x06
	mov	cx, #0x100
	mov	ds, cx
	mov	ss, cx
#endasm
}
