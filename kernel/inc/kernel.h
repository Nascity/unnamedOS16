#ifndef __KERNEL_H__
#define __KERNEL_H__

#define KERNEL_SEGMENT	0x0100

#define OS_NAME		"uOS16"
#define KERNEL_NAME	"solstice"
#define KERNEL_VERSION	"v1.0.0"

#define SHELL_ARG	OS_NAME " " KERNEL_NAME " " KERNEL_VERSION
#define SHELL_NAME	"shell"
#define SHELL_EXT	"sys"

#define KERNEL_LOADED_MSG	"Kernel successfully loaded."
#define IO_INIT_FAIL_MSG	"io_init failed."
#define SHELL_FAIL_MSG		"Shell failed to execute."
#define SHELL_SUCCESS_MSG	"Shell exited with %d."
#define SYSTEM_HALT_MSG		"Halt."

void halt(void);

#endif
