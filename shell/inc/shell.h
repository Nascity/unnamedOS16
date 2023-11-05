#ifndef __SHELL_H__
#define __SHELL_H__

#include "../../uosapi/inc/uosapi.h"

/* ---------- #defines ---------- */
#define SHELL_NAME	"unnamedShell (ush)"
#define SHELL_VERSION	"v1.0.0"

#define COMMAND_BUFFER_SIZE	64

#define SHELL_PREFIX_FORMAT	"ush> "
#define SHELL_PREFIX_LENGTH	5

#define COMMAND_LONG_ERROR_MSG		"Command too long.\n"
#define COMMAND_INVALID_ERROR_MSG	"is an invalid command.\n"
#define INVALID_ARGUMENT_ERROR_MSG	"Invalid argument count.\n"

/* ---------- command.c ---------- */
BOOL
GetCommand(
	STRING	strCmd
	);
BOOL
InterpretCommand(
	STRING	strCmd
	);

#endif
