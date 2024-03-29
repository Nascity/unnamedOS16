#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define SYSCALL_BEGIN		0x20

#define SYSCALL_PROCESS_CREATE	0x20
#define SYSCALL_PROCESS_UNUSED	0x21
#define SYSCALL_MEMORY_ALLOC	0x22
#define SYSCALL_MEMORY_FREE	0x23
#define SYSCALL_MEMORY_WRITE	0x24
#define SYSCALL_MEMORY_READ	0x25
#define SYSCALL_IO_OPEN		0x26
#define SYSCALL_IO_CLOSE	0x27
#define SYSCALL_IO_WRITE	0x28
#define SYSCALL_IO_READ		0x29
#define SYSCALL_CREATE_FILE	0x2A
#define SYSCALL_DELETE_FILE	0x2B

#define SYSCALL_END		0x2B

void syscall_init(void);
void syscall_begin(void);
void syscall_end(void);
void syscall_return(void);

#endif
