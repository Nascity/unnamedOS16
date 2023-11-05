#include "inc/inc.h"

void init(void);
int execute_shell(int* suc);

void main(void)
{
	int	ret;
	int	suc;

	printline(KERNEL_LOADED_MSG);
	init();

	ret = execute_shell(&suc);
	if (!suc)
		printline(SHELL_FAIL_MSG);
	else
		printline(SHELL_SUCCESS_MSG);
	halt();
}


void init(void)
{
	syscall_init();
	if(!io_init())
	{
		printline(IO_INIT_FAIL_MSG);
		halt();
	}
	process_init();
}


int execute_shell(int* suc)
{
	int	is = suc;
	int	args = SHELL_ARG;
	int	ext = SHELL_EXT;
	int	filename = SHELL_NAME;
	int	workingdir = 1;
	asm("int 0x20");
}


void halt(void)
{
	printstring(SYSTEM_HALT_MSG);
	while(true);
}
