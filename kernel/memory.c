#include "inc/inc.h"

extern struct
{
	int count;
	int top_seg;
	pcb_t blocks[MAX_PROCESS];
} pcb_stack;

void* chunk_index_to_addr(int chunk_index);
int addr_to_chunk_index(void* addr);

// -----------------------------------------------------------------------
// Initialization
void MAT_init(MAT_t* pmgr, int prealloc_size)
{
	int i;
	int prealloc_chunk_count =
		prealloc_size % HEAP_CHUNK_SIZE == 0 ?
		prealloc_size / HEAP_CHUNK_SIZE :
		prealloc_size / HEAP_CHUNK_SIZE + 1;

	for (i = 0; i < HEAP_CHUNK_COUNT; i++)
	{
		if (i < prealloc_chunk_count)
			pmgr->chunk_state[i] = 0;
		else
			pmgr->chunk_state[i] = UNALLOCATED_SLICE;
	}
}

// -----------------------------------------------------------------------
// Interaction
void* memory_alloc(int cs, int flags, int count)
{
	int chunk_count;
	int empty_count;
	int i;
	int candidate;
	void* ret;
	MAT_t* pmgr;

	syscall_begin();
	chunk_count =
		count % HEAP_CHUNK_SIZE == 0 ?
		count / HEAP_CHUNK_SIZE :
		count / HEAP_CHUNK_SIZE + 1;
	pmgr = &(pcb_stack.blocks[get_MAT_index(cs)].mem_alloc_table);

	i = 0;
	ret = 0;
	empty_count = 0;
	candidate = 0;
	while (i < HEAP_CHUNK_COUNT)
	{
		if (pmgr->chunk_state[i] != UNALLOCATED_SLICE)
		{
			empty_count = 0;
			candidate = 0;
			i++;
			continue;
		}

		if (candidate == 0)
			candidate = i;
		empty_count++;
		if (empty_count == chunk_count)
		{
			for (i = candidate; i < candidate + empty_count; i++)
				pmgr->chunk_state[i] = candidate;
			ret = chunk_index_to_addr(candidate);
			break;
		}
		i++;
	}

	asm("mov	ax, word ptr [bp - 14]");
	syscall_end();
	syscall_return();
}


bool memory_free(int cs, int flags, void* addr)
{
	int target;

	syscall_begin();

	target = addr_to_chunk_index(addr);

	syscall_end();
	syscall_return();
}

// -----------------------------------------------------------------------
// Internal
void* chunk_index_to_addr(int chunk_index)
{
	return HEAP_START + HEAP_CHUNK_SIZE * chunk_index;
}


int addr_to_chunk_index(void* addr)
{
	// it was (int)addr - HEAP_START
	// but due to compiler bug, it had
	// to be changed.
	return ((int)addr + 0x200) / HEAP_CHUNK_SIZE;
}
