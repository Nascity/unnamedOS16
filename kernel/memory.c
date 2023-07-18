#include "inc/inc.h"

void init_16B_slice(memmgr_t* pmgr);
void init_32B_slice(memmgr_t* pmgr);
void init_64B_slice(memmgr_t* pmgr);
void init_128B_slice(memmgr_t* pmgr);
void init_256B_slice(memmgr_t* pmgr);




// -----------------------------------------------------------------------
// Initialization
void memmgr_init(memmgr_t* pmgr)
{
	init_16B_slice(pmgr);
	init_32B_slice(pmgr);
	init_64B_slice(pmgr);
	init_128B_slice(pmgr);
	init_256B_slice(pmgr);
}


void init_16B_slice(memmgr_t* pmgr)
{
	int i;

	for(i = 0; i < _16BYTE_SLICE_COUNT; i++)
	{
		pmgr->slice_16B[i].empty = true;
		pmgr->slice_16B[i].addr =
			UPPER_HEAP_START_OFFSET
			+ _16BYTE_SLICE_ADDR_OFFSET
			+ i * 16;
	}
}


void init_32B_slice(memmgr_t* pmgr)
{
	int i;

	for(i = 0; i < _32BYTE_SLICE_COUNT; i++)
	{
		pmgr->slice_32B[i].empty = true;
		pmgr->slice_32B[i].addr =
			UPPER_HEAP_START_OFFSET
			+ _32BYTE_SLICE_ADDR_OFFSET
			+ i * 32;
	}
}


void init_64B_slice(memmgr_t* pmgr)
{
	int i;

	for(i = 0; i < _64BYTE_SLICE_COUNT; i++)
	{
		pmgr->slice_64B[i].empty = true;
		pmgr->slice_64B[i].addr =
			UPPER_HEAP_START_OFFSET
			+ _64BYTE_SLICE_ADDR_OFFSET
			+ i * 64;
	}
}


void init_128B_slice(memmgr_t* pmgr)
{
	int i;

	for(i = 0; i < _128BYTE_SLICE_COUNT; i++)
	{
		pmgr->slice_128B[i].empty = true;
		pmgr->slice_128B[i].addr =
			UPPER_HEAP_START_OFFSET
			+ _128BYTE_SLICE_ADDR_OFFSET
			+ i * 128;
	}
}


void init_256B_slice(memmgr_t* pmgr)
{
	int i;

	for(i = 0; i < _256BYTE_SLICE_COUNT; i++)
	{
		pmgr->slice_256B[i].empty = true;
		pmgr->slice_256B[i].addr =
			LOWER_HEAP_START_OFFSET
			+ i * 256;
	}
}

// -----------------------------------------------------------------------
// Interaction
void* memory_alloc(int count)
{

}

