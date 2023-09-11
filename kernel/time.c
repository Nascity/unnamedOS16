#include "inc/inc.h"

void decode_bcd(byte* data, time_entry* pde);


bool get_time(time_entry* pde)
{
	byte data[6];
	int ret;
#asm
	mov	ah, #0x04
	int	0x1A
	jc	get_time_error
	mov	byte ptr [bp - 10], cl
	mov	byte ptr [bp - 9], dh
	mov	byte ptr [bp - 8], dl
	mov	ah, #0x02
	int	0x1A
	jc	get_time_error
	mov	byte ptr [bp - 7], ch
	mov	byte ptr [bp - 6], cl
	mov	byte ptr [bp - 5], dh
	mov	word ptr [bp - 12], #0x01
	jmp	get_time_end
get_time_error:
	xor	ax, ax
	mov	word ptr [bp - 12], ax
get_time_end:
	nop
#endasm
	decode_bcd(data, pde);
	return ret;
}


void decode_bcd(byte* data, time_entry* pde)
{
	pde->year = ((data[0] / 16) * 10) + data[0] % 16;
	pde->month = ((data[1] / 16) * 10) + data[1] % 16;
	pde->day = ((data[2] / 16) * 10) + data[2] % 16;
	pde->hour = ((data[3] / 16) * 10) + data[3] % 16;
	pde->minute = ((data[4] / 16) * 10) + data[4] % 16;
	pde->second = ((data[5] / 16) * 10) + data[5] % 16;
}
