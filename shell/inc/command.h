#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "../../uosapi/inc/uosapi.h"

#define COMMAND_DEF(name)	INT name(PSTRING psArgs)

#define CAT_FAIL_MSG		"\'cat\' command has failed.\n"
#define CAT_INVALID_FILE_MSG	"is an invalid file.\n"

typedef struct	__COMMAND_ENTRY
{
	STRING	strCommandName;
	UINT	uiHelpPageNumber;
	PTR	pCommandHandler;
}		COMMAND_ENTRY, *PCOMMAND_ENTRY;

#endif
