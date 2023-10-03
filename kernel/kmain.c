#include "inc/inc.h"

void init(void);
int execute_shell(int* suc);

void main(void)
{
	int	ret;
	int	suc;

	printline("Kernel successfully loaded.");
	init();

	ret = execute_shell(&suc);
	printline("ret: %d, suc: %d", ret, suc);

	halt();
}


void init(void)
{
	syscall_init();
	if(!io_init())
	{
		printline("io_init failed.");
		halt();
	}
	process_init();
}


int execute_shell(int* suc)
{
	int	is = suc;
	int	args = "uOS16 solstice v1.0.0";
	int	ext = "sys";
	int	filename = "shell";
	int	workingdir = 1;
	asm("int 0x20");
}


void halt(void)
{
	printstring("halt");
	while(true);
}
