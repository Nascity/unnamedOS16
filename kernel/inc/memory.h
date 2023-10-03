#ifndef __MEMORY_H__
#define __MEMORY_H__

#define HEAP_SIZE		256
#define HEAP_START		0xFE00
#define HEAP_CHUNK_SIZE		4
#define HEAP_CHUNK_COUNT	(HEAP_SIZE / HEAP_CHUNK_SIZE)

#define KERNEL_MEMORY_START_SEGMENT	0x2000

#define UNALLOCATED_SLICE	-1

typedef int kobj_mem;

typedef struct __MAT_t
{
	int chunk_state[HEAP_CHUNK_COUNT];
} MAT_t;

void MAT_init(memmgr_t* pmgr, int prealloc_size);

void* memory_alloc(int cs, int flags, int count);
bool memory_free(int cs, int flags, void* addr);

#endif
