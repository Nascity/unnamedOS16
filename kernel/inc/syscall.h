#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define SYSCALL_BEGIN		0x20

#define SYSCALL_PROCESS_CREATE	0x20
#define SYSCALL_PROCESS_KILL	0x21

#define SYSCALL_END		0x21

void syscall_init(void);

#endif
