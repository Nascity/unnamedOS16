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
#define _256BYTE_SLICE_ADDR_OFFSET	0xF000

#define _16BYTE_SLICE_START	0
#define _32BYTE_SLICE_START	64
#define _64BYTE_SLICE_START	96
#define _128BYTE_SLICE_START	112
#define _256BYTE_SLICE_START	120
#define SLICE_COUNT	128

#define _16BYTE_SLICE_ALLOC_THRESHOLD	32
#define _32BYTE_SLICE_ALLOC_THRESHOLD	64
#define _64BYTE_SLICE_ALLOC_THRESHOLD	128
#define _128BYTE_SLICE_ALLOC_THRESHOLD	256
#define _256BYTE_SLICE_ALLOC_THRESHOLD	2048

#define UNALLOCATED_SLICE	-1
#define INVALID_KOBJMEM		-1

typedef int kobj_mem;

typedef struct __MAT_t
{
	int memstate_16B[_16BYTE_SLICE_COUNT];
	int memstate_32B[_32BYTE_SLICE_COUNT];
	int memstate_64B[_64BYTE_SLICE_COUNT];
	int memstate_128B[_128BYTE_SLICE_COUNT];
	int memstate_256B[_256BYTE_SLICE_COUNT];
} MAT_t;

void MAT_init(memmgr_t* pmgr);

kobj_mem memory_alloc(int cs, int flags, int count);
bool memory_free(int cs, int flags, kobj_mem komem);

bool memory_write(int cs, int flags, kobj_mem komem, void* buffer, int offset, int count);
bool memory_read(int cs, int flags, kobj_mem komem, void* buffer, int offset, int count);

#endif
