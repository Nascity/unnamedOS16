#ifndef __MEMORY_H__
#define __MEMORY_H__

#define _16BYTE_SLICE_COUNT	64
#define _32BYTE_SLICE_COUNT	32
#define _64BYTE_SLICE_COUNT	16
#define _128BYTE_SLICE_COUNT	8
#define _256BYTE_SLICE_COUNT	8

#define _16BYTE_SLICE_ADDR_OFFSET	0x0000
#define _32BYTE_SLICE_ADDR_OFFSET	0x0400
#define _64BYTE_SLICE_ADDR_OFFSET	0x0800
#define _128BYTE_SLICE_ADDR_OFFSET	0x0C00

#define _16BYTE_SLICE_ALLOC_THRESHOLD	32
#define _32BYTE_SLICE_ALLOC_THRESHOLD	64
#define _64BYTE_SLICE_ALLOC_THRESHOLD	128
#define _128BYTE_SLICE_ALLOC_THRESHOLD	256
#define _256BYTE_SLICE_ALLOC_THRESHOLD	1024

#define LOWER_HEAP_START_OFFSET	0x0F000
#define UPPER_HEAP_START_OFFSET	0x20000

#define UNALLOCATED_SLICE	-1

typedef struct __MAT_t
{
	int memstate_16B[_16BYTE_SLICE_COUNT];
	int memstate_32B[_32BYTE_SLICE_COUNT];
	int memstate_64B[_64BYTE_SLICE_COUNT];
	int memstate_128B[_128BYTE_SLICE_COUNT];
	int memstate_256B[_256BYTE_SLICE_COUNT];
} MAT_t;

void MAT_init(memmgr_t* pmgr);

kobj_mem memory_alloc(int count);
bool memory_free(kobj_mem komem);

#endif