#include "inc/inc.h"

extern struct
{
	int count;
	pcb_t* top;
	pcb_t blocks[MAX_PROCESS];
} pcb_stack;

int find_slice(MAT_t* pMAT, int slice_size, int req_size);
int find_slice_16B(MAT_t* pMAT, int req_size);
int find_slice_32B(MAT_t* pMAT, int req_size);
int find_slice_64B(MAT_t* pMAT, int req_size);
int find_slice_128B(MAT_t* pMAT, int req_size);
int find_slice_256B(MAT_t* pMAT, int req_size);
int reqsize_to_segmentcount(int req_size, int slice_size);

void memory_set(void* addr, int count, int val);
void* slice_to_addr(int slice_size, int slice);

// -----------------------------------------------------------------------
// Initialization
void MAT_init(MAT_t* pmgr)
{
	memory_set(pmgr, sizeof(MAT_t), UNALLOCATED_SLICE);
}


// -----------------------------------------------------------------------
// Interaction
kobj_mem memory_alloc(int cs, int flags, int count)
{
	kobj_mem komem = find_slice(
			&(pcb_stack.blocks[(cs / 0x100) - 1]
				.mem_alloc_table),
			16,
			count);
#asm
	mov	ax, word ptr [bp - 6]	; return value
	mov	sp, bp
	add	sp, #0x02
	iret
#endasm	
}


bool memory_free(int cs, int flags, kobj_mem komem)
{
	int* state_table;
	int index;

	if(komem < 64)
	{
		state_table = pcb_stack.blocks[(cs / 0x100) - 1]
				.mem_alloc_table.memstate_16B;
		index = (int)komem;
	}
	else if(komem < 96)
	{
		state_table = pcb_stack.blocks[(cs / 0x100) - 1]
				.mem_alloc_table.memstate_32B;
		index = (int)komem - 64;
	}
	else if(komem < 112)
	{
		state_table = pcb_stack.blocks[(cs / 0x100) - 1]
				.mem_alloc_table.memstate_64B;
		index = (int)komem - 96;
	}
	else if(komem < 120)
	{
		state_table = pcb_stack.blocks[(cs / 0x100) - 1]
				.mem_alloc_table.memstate_128B;
		index = (int)komem - 112;
	}
	else if(komem < 128)
	{
		state_table = pcb_stack.blocks[(cs / 0x100) - 1]
				.mem_alloc_table.memstate_256B;
		index = (int)komem - 120;
	}
	else
	{
#asm
		mov	ax, #0x00	; return false
		mov	sp, bp
		add	sp, #0x02
		iret
#endasm
	}

	if(state_table[index] == UNALLOCATED_SLICE
		|| state_table[index] != komem)
	{
#asm
		mov	ax, #0x00	; return false
		mov	sp, bp
		add	sp, #0x02
		iret
#endasm
	}

	while(state_table[index] != UNALLOCATED_SLICE
		&& state_table[index] == komem)
		state_table[index++] = UNALLOCATED_SLICE;

#asm
	mov	ax, #0x01	; return true
	mov	sp, bp
	add	sp, #0x02
	iret
#endasm
}


int find_slice(MAT_t* pMAT, int slice_size, int req_size)
{
	switch(slice_size)
	{
	case 16: return find_slice_16B(pMAT, req_size);
	case 32: return find_slice_32B(pMAT, req_size);
	case 64: return find_slice_64B(pMAT, req_size);
	case 128: return find_slice_128B(pMAT, req_size);
	case 256: return find_slice_256B(pMAT, req_size);
	default: return -1;
	}
}
	

int find_slice_16B(MAT_t* pMAT, int req_size)
{
	int count = reqsize_to_segmentcount(req_size, 16);
	int temp = count;
	int i;
	int start_slice;
	bool found = false;

	if(req_size >= _16BYTE_SLICE_ALLOC_THRESHOLD)
		return find_slice_32B(pMAT, req_size);

	for(i = 0; i < _16BYTE_SLICE_COUNT; i++)
	{
		if(pMAT->memstate_16B[i] == UNALLOCATED_SLICE)
		{
			temp--;

			if(!found)
			{
				start_slice = i;
				found = true;
			}

			if(!temp) break;
		}
	}
	
	if(temp == 0 && found)
	{
		temp = start_slice;

		for(i = 0; i < count; i++)
			pMAT->memstate_16B[temp++] = start_slice + 0;

		return start_slice;
	}
	else return -1;
}


int find_slice_32B(MAT_t* pMAT, int req_size)
{
	int count = reqsize_to_segmentcount(req_size, 32);
	int temp = count;
	int i;
	int start_slice;
	bool found = false;

	if(req_size >= _32BYTE_SLICE_ALLOC_THRESHOLD)
		return find_slice_64B(pMAT, req_size);

	for(i = 0; i < _32BYTE_SLICE_COUNT; i++)
	{
		if(pMAT->memstate_32B[i] == UNALLOCATED_SLICE)
		{
			temp--;

			if(!found)
			{
				start_slice = i;
				found = true;
			}

			if(!temp) break;
		}
	}
	
	if(temp == 0 && found)
	{
		temp = start_slice;

		for(i = 0; i < count; i++)
			pMAT->memstate_32B[temp++] = start_slice + 64;

		return start_slice + 64;
	}
	else return -1;
}


int find_slice_64B(MAT_t* pMAT, int req_size)
{
	int count = reqsize_to_segmentcount(req_size, 64);
	int temp = count;
	int i;
	int start_slice;
	bool found = false;

	if(req_size >= _64BYTE_SLICE_ALLOC_THRESHOLD)
		return find_slice_128B(pMAT, req_size);

	for(i = 0; i < _64BYTE_SLICE_COUNT; i++)
	{
		if(pMAT->memstate_64B[i] == UNALLOCATED_SLICE)
		{
			temp--;

			if(!found)
			{
				start_slice = i;
				found = true;
			}

			if(!temp) break;
		}
	}
	
	if(temp == 0 && found)
	{
		temp = start_slice;

		for(i = 0; i < count; i++)
			pMAT->memstate_64B[temp++] = start_slice + 96;

		return start_slice;
	}
	else return -1;
}


int find_slice_128B(MAT_t* pMAT, int req_size)
{
	int count = reqsize_to_segmentcount(req_size, 128);
	int temp = count;
	int i;
	int start_slice;
	bool found = false;

	if(req_size >= _128BYTE_SLICE_ALLOC_THRESHOLD)
		return find_slice_256B(pMAT, req_size);

	for(i = 0; i < _128BYTE_SLICE_COUNT; i++)
	{
		if(pMAT->memstate_128B[i] == UNALLOCATED_SLICE)
		{
			temp--;

			if(!found)
			{
				start_slice = i;
				found = true;
			}

			if(!temp) break;
		}
	}
	
	if(temp == 0 && found)
	{
		temp = start_slice;

		for(i = 0; i < count; i++)
			pMAT->memstate_128B[temp++] = start_slice + 112;

		return start_slice;
	}
	else return -1;
}


int find_slice_256B(MAT_t* pMAT, int req_size)
{
	int count = reqsize_to_segmentcount(req_size, 256);
	int temp = count;
	int i;
	int start_slice;
	bool found = false;

	if(req_size >= _256BYTE_SLICE_ALLOC_THRESHOLD)
		return -1;

	for(i = 0; i < _256BYTE_SLICE_COUNT; i++)
	{
		if(pMAT->memstate_256B[i] == UNALLOCATED_SLICE)
		{
			temp--;

			if(!found)
			{
				start_slice = i;
				found = true;
			}

			if(!temp) break;
		}
	}
	
	if(temp == 0 && found)
	{
		temp = start_slice;

		for(i = 0; i < count; i++)
			pMAT->memstate_256B[temp++] = start_slice + 256;

		return start_slice;
	}
	else return -1;
}


int reqsize_to_segmentcount(int req_size, int slice_size)
{
	return req_size % slice_size == 0 ?
		((req_size - (req_size % slice_size)) / slice_size) :
		((req_size - (req_size % slice_size)) / slice_size) + 1;
}

// -----------------------------------------------------------------------
// Internal
void memory_set(void* addr, int count, int val)
{
	int i;

	for(i = 0; i < count; i++) ((char*)addr)[i] = val;
}


void* slice_to_addr(int slice_size, int slice)
{
	switch(slice_size)
	{
	case 16: return _16BYTE_SLICE_ADDR_OFFSET + slice * 16;
	case 32: return _32BYTE_SLICE_ADDR_OFFSET + (slice - 64) * 32;
	case 64: return _64BYTE_SLICE_ADDR_OFFSET + (slice - 96) * 64;
	case 128: return _128BYTE_SLICE_ADDR_OFFSET + (slice - 112) * 128;
	case 256: return LOWER_HEAP_START_OFFSET + (slice - 120) * 256;
	default: return NULL;
	}
}
