#include "inc/inc.h"

// syscall handlers must have following structure:
// return_type handler(int return_cs, int flags, ...);
void* handlers[] =
{
	process_start,
	process_kill
};

void replace_handler(int int_number, void* handler);



void syscall_init(void)
{
	int i;

	for(i = SYSCALL_BEGIN; i <= SYSCALL_END; i++)
		replace_handler(i, handlers[i - SYSCALL_BEGIN]);

	// DEBUG
	printline("Syscall initialized.");
}


void replace_handler(int int_number, void* handler)
{
#asm
	push	bp
	mov	bp, sp

	mov	bx, word ptr [bp + 6]
	xor	ax, ax
	mov	ds, ax

	mov	si, word ptr [bp + 4]
	shl	si, #0x02
	mov	word ptr [si], bx
	add	si, #0x02
	mov	ax, #0x0100
	mov	word ptr [si], ax
	mov	ds, ax

	pop	bp
#endasm	
}
