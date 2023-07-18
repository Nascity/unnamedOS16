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

#define LOWER_HEAP_START_OFFSET	0x0F000
#define UPPER_HEAP_START_OFFSET	0x20000

typedef struct __slice_t
{
	bool empty;
	void* addr;
} slice_t;

typedef struct __memmgr_t
{
	slice_t slice_16B[_16BYTE_SLICE_COUNT];
	slice_t slice_32B[_32BYTE_SLICE_COUNT];
	slice_t slice_64B[_64BYTE_SLICE_COUNT];
	slice_t slice_128B[_128BYTE_SLICE_COUNT];
	slice_t slice_256B[_256BYTE_SLICE_COUNT];
} memmgr_t;

void memmgr_init(memmgr_t* pmgr);

void* memory_alloc(int count);
bool memory_free(void* addr);

#endif
