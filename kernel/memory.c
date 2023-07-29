#include "inc/inc.h"

extern struct
{
	int count;
	pcb_t* top;
	pcb_t blocks[MAX_PROCESS];
} pcb_stack;

int find_slice(MAT_t* pMAT, int req_size);
int find_slice_16B(MAT_t* pMAT, int req_size, int direction);
int find_slice_32B(MAT_t* pMAT, int req_size, int direction);
int find_slice_64B(MAT_t* pMAT, int req_size, int direction);
int find_slice_128B(MAT_t* pMAT, int req_size, int direction);
int find_slice_256B(MAT_t* pMAT, int req_size, int direction);
int reqsize_to_segmentcount(int req_size, int slice_size);

void memory_set(void* addr, int count, int val);
void* slice_to_addr(int slice_size, int slice);
int* komem_to_addr(kobj_mem komem);
int reqsize_to_segmentcount(int req_size, int slice_size);

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
			count);

	asm("mov	ax, word ptr [bp - 6]");
	syscall_return();
}


bool memory_free(int cs, int flags, kobj_mem komem)
{
	int* state_table = pcb_stack.blocks[(cs / 0x100) - 1]
				.mem_alloc_table.memstate_16B;
	int index = komem;

	if(state_table[index] == UNALLOCATED_SLICE
		|| state_table[index] != komem)
	{
		asm("mov	ax, #0x00");
		syscall_return();
	}

	while(state_table[index] != UNALLOCATED_SLICE
		&& state_table[index] == komem)
		state_table[index++] = UNALLOCATED_SLICE;

	asm("mov	ax, #0x01");
	syscall_return();
}


bool memory_write(int cs, int flags, kobj_mem komem, void* buffer, int offset, int count)
{
	int ds = komem < _256BYTE_SLICE_START ? cs + 0x2000 : cs;
	int* target = komem_to_addr(komem);
	int seg_count = 0;
	int multiplier = 0;
	int temp = komem;

	if(target == -1)
	{
		asm("mov	ax, #0x00");
		syscall_return();
	}

	// how many segments?
	while(komem == pcb_stack.blocks[(cs / 0x100) - 1]
		.mem_alloc_table.memstate_16B[temp++])
		seg_count++;
	if(komem < _32BYTE_SLICE_COUNT && komem >= 0) multiplier = 16;
	else if(komem < _64BYTE_SLICE_COUNT) multiplier = 32;
	else if(komem < _128BYTE_SLICE_COUNT) multiplier = 64;
	else if(komem < _256BYTE_SLICE_COUNT) multiplier = 128;
	else if(komem < SLICE_COUNT) multiplier = 256;
	else
	{
		asm("mov	ax, #0x00");
		syscall_return();
	}
	if(offset + count > seg_count * multiplier)
		count -= offset + count - seg_count * multiplier + 1;
	if(seg_count == 0)
	{
		asm("mov	ax, #0x00");
		syscall_return();
	}

#asm
	mov	bx, word ptr [bp + 10]	; bx = buffer
	mov	cx, word ptr [bp + 14]	; cx = count
	mov	di, word ptr [bp + 12]	; di = target + offset
	add	di, word ptr [bp - 8]
	xor	si, si			; si = 0
memory_write_loop:
	cmp	si, cx
	jz	memory_write_ending

	mov	ax, word ptr [bp + 4]
	mov	ds, ax
	mov	dx, word ptr [bx + si]

	mov	ax, word ptr [bp - 6]
	mov	ds, ax
	mov	word ptr [di], dx

	inc	di
	inc	si
	jmp	memory_write_loop
memory_write_ending:
	mov	ax, word ptr [bp + 4]
	mov	ds, ax
	mov	ax, #0x01
	mov	sp, bp
	add	sp, #0x02
	iret
#endasm
}


bool memory_read(int cs, int flags, kobj_mem komem, void* buffer, int offset, int count)
{
	int ds = komem < _256BYTE_SLICE_START ? cs + 0x2000 : cs;
	int* target = komem_to_addr(komem);
	int seg_count = 0;
	int multiplier = 0;
	int temp = komem;

	if(target == -1)
	{
		asm("mov	ax, #0x00");
		syscall_return();
	}

	// how many segments?
	while(komem == pcb_stack.blocks[(cs / 0x100) - 1]
		.mem_alloc_table.memstate_16B[temp++])
		seg_count++;
	if(komem < _32BYTE_SLICE_COUNT && komem >= 0) multiplier = 16;
	else if(komem < _64BYTE_SLICE_COUNT) multiplier = 32;
	else if(komem < _128BYTE_SLICE_COUNT) multiplier = 64;
	else if(komem < _256BYTE_SLICE_COUNT) multiplier = 128;
	else if(komem < SLICE_COUNT) multiplier = 256;
	else
	{
		asm("mov	ax, #0x00");
		syscall_return();
	}
	if(offset + count > seg_count * multiplier)
		count -= offset + count - seg_count * multiplier;
	if(seg_count == 0)
	{
		asm("mov	ax, #0x00");
		syscall_return();
	}

#asm
	mov	bx, word ptr [bp + 10]	; bx = buffer
	mov	cx, word ptr [bp + 14]	; cx = count
	mov	di, word ptr [bp + 12]	; di = target + offset
	add	di, word ptr [bp - 8]
	xor	si, si			; si = 0
memory_read_loop:
	cmp	si, cx
	jz	memory_read_ending

	mov	ax, word ptr [bp - 6]
	mov	ds, ax
	mov	dx, word ptr [di]

	mov	ax, word ptr [bp + 4]
	mov	ds, ax
	mov	word ptr [bx + si], dx

	inc	di
	inc	si
	jmp	memory_read_loop
memory_read_ending:
	mov	ax, word ptr [bp + 4]
	mov	ds, ax
	mov	ax, #0x01
	mov	sp, bp
	add	sp, #0x02
	iret
#endasm
}

// -----------------------------------------------------------------------
// Internal
void memory_set(void* addr, int count, int val)
{
	int i;

	for(i = 0; i < count; i++) ((char*)addr)[i] = val;
}


int find_slice(MAT_t* pMAT, int req_size)
{
	int ret;

	if(req_size < _16BYTE_SLICE_ALLOC_THRESHOLD)
		ret = find_slice_16B(pMAT, req_size, 0);
	else if(req_size < _32BYTE_SLICE_ALLOC_THRESHOLD)
		ret = find_slice_32B(pMAT, req_size, 0);
	else if(req_size < _64BYTE_SLICE_ALLOC_THRESHOLD)
		ret = find_slice_64B(pMAT, req_size, 0);
	else if(req_size < _128BYTE_SLICE_ALLOC_THRESHOLD)
		ret = find_slice_128B(pMAT, req_size, 0);
	else if(req_size < _256BYTE_SLICE_ALLOC_THRESHOLD)
		ret = find_slice_256B(pMAT, req_size, 0);
	else
		ret = INVALID_KOBJMEM;

	return ret;
}


int find_slice_16B(MAT_t* pMAT, int req_size, int direction)
{
	int count = reqsize_to_segmentcount(req_size, 16);
	int temp = count;
	int i;
	int start_slice;
	bool found = false;

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
		else
		{
			found = false;
			temp = count;
		}
	}
	
	if(temp == 0 && found)
	{
		temp = start_slice;

		for(i = 0; i < count; i++)
			pMAT->memstate_16B[temp++] =
				start_slice + _16BYTE_SLICE_START;

		return start_slice + _16BYTE_SLICE_START;
	}
	
	if(direction == 0)
		return find_slice_32B(pMAT, req_size, 1);

	return INVALID_KOBJMEM;
}


int find_slice_32B(MAT_t* pMAT, int req_size, int direction)
{
	int count = reqsize_to_segmentcount(req_size, 32);
	int temp = count;
	int i;
	int start_slice;
	bool found = false;

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
		else
		{
			found = false;
			temp = count;
		}
	}
	
	if(temp == 0 && found)
	{
		temp = start_slice;

		for(i = 0; i < count; i++)
			pMAT->memstate_32B[temp++] =
				start_slice + _32BYTE_SLICE_START;

		return start_slice + _32BYTE_SLICE_START;
	}

	if(direction == 0)
	{
		temp = find_slice_16B(pMAT, req_size, -1);
		if(temp == INVALID_KOBJMEM)
			return find_slice_64B(pMAT, req_size, 1);
		return temp;
	}
	else if(direction == -1)
		return find_slice_16B(pMAT, req_size, -1);
	else if(direction == 1)
		return find_slice_64B(pMAT, req_size, 1);

	return INVALID_KOBJMEM;
}


int find_slice_64B(MAT_t* pMAT, int req_size, int direction)
{
	int count = reqsize_to_segmentcount(req_size, 64);
	int temp = count;
	int i;
	int start_slice;
	bool found = false;

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
		else
		{
			found = false;
			temp = count;
		}
	}
	
	if(temp == 0 && found)
	{
		temp = start_slice;

		for(i = 0; i < count; i++)
			pMAT->memstate_64B[temp++] = start_slice + 96;

		return start_slice + _64BYTE_SLICE_START;
	}

	if(direction == 0)
	{
		temp = find_slice_32B(pMAT, req_size, -1);
		if(temp == INVALID_KOBJMEM)
			return find_slice_128B(pMAT, req_size, 1);
		return temp;
	}
	else if(direction == -1)
		return find_slice_32B(pMAT, req_size, -1);
	else if(direction == 1)
		return find_slice_128B(pMAT, req_size, -1);

	return INVALID_KOBJMEM;
}


int find_slice_128B(MAT_t* pMAT, int req_size, int direction)
{
	int count = reqsize_to_segmentcount(req_size, 128);
	int temp = count;
	int i;
	int start_slice;
	bool found = false;

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
		else
		{
			found = false;
			temp = count;
		}
	}
	
	if(temp == 0 && found)
	{
		temp = start_slice;

		for(i = 0; i < count; i++)
			pMAT->memstate_128B[temp++] = start_slice + _128BYTE_SLICE_START;

		return start_slice + _128BYTE_SLICE_START;
	}

	if(direction == 0)
	{
		temp = find_slice_64B(pMAT, req_size, -1);
		if(temp == INVALID_KOBJMEM)
			return find_slice_256B(pMAT, req_size, 1);
		return temp;
	}
	else if(direction == -1)
		return find_slice_64B(pMAT, req_size, -1);
	else if(direction == 1)
		return find_slice_256B(pMAT, req_size, 1);

	return INVALID_KOBJMEM;
}


int find_slice_256B(MAT_t* pMAT, int req_size, int direction)
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
		else
		{
			found = false;
			temp = count;
		}
	}
	
	if(temp == 0 && found)
	{
		temp = start_slice;

		for(i = 0; i < count; i++)
			pMAT->memstate_256B[temp++] = start_slice + _256BYTE_SLICE_START;

		return start_slice + _256BYTE_SLICE_START;
	}

	if(direction == 0)
		return find_slice_128B(pMAT, req_size, -1);

	return INVALID_KOBJMEM;
}


int* komem_to_addr(kobj_mem komem)
{
	if(komem < _32BYTE_SLICE_START && komem >= 0)
		return _16BYTE_SLICE_ADDR_OFFSET
			+ (komem - _16BYTE_SLICE_START) * 16;
	else if(komem < _64BYTE_SLICE_START)
		return _32BYTE_SLICE_ADDR_OFFSET
			+ (komem - _32BYTE_SLICE_START) * 32;
	else if(komem < _128BYTE_SLICE_START)
		return _64BYTE_SLICE_ADDR_OFFSET
			+ (komem - _64BYTE_SLICE_START) * 64;
	else if(komem < _256BYTE_SLICE_START)
		return _128BYTE_SLICE_ADDR_OFFSET
			+ (komem - _128BYTE_SLICE_START) * 128;
	else if(komem < SLICE_COUNT)
		return _256BYTE_SLICE_ADDR_OFFSET
			+ (komem - _256BYTE_SLICE_START) * 256;
	else return -1;
}


int reqsize_to_segmentcount(int req_size, int slice_size)
{
	return req_size % slice_size == 0 ?
		((req_size - (req_size % slice_size)) / slice_size) :
		((req_size - (req_size % slice_size)) / slice_size) + 1;
}
