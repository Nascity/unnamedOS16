#include "inc/command.h"
#include "inc/shell.h"
#include "inc/shelltypes.h"

/* ---------- command defines ---------- */
COMMAND_DEF(chCat);
COMMAND_DEF(chClear);
COMMAND_DEF(chHalt);
COMMAND_DEF(chLs);
COMMAND_DEF(chKernelinfo);
COMMAND_DEF(chShellinfo);

/* ---------- global variable ---------- */
COMMAND_ENTRY
ceCommands[] =
{
	{ "cat", 2, chCat },
	{ "clear", 5, chClear },
	{ "halt", 4, chHalt },
	{ "kernelinfo", 0, chKernelinfo },
	{ "ls", 3, chLs },
	{ "shellinfo", 1, chShellinfo }
};

UINT
uiCmdsSize = sizeof(ceCommands) / sizeof(COMMAND_ENTRY);

extern
INT
iShellExitCode;

extern
OS_VERSION_INFO
ovi;

/* ---------- internal functions ---------- */
static
INT
_cmds_bsearch(
	STRING	strTarget
	)
{
	UINT	s;
	UINT	e;
	INT	iCmp;

	for (s = 0, e = uiCmdsSize - 1; s <= e;)
	{
		iCmp = StringCompare(strTarget, ceCommands[(s + e) / 2].strCommandName);

		if (!iCmp)
			return (s + e) / 2;
		if (iCmp > 0)
			s = (s + e) / 2 + 1;
		else
			e = (s + e) / 2 - 1;
	}
	return -1;
}

/* ---------- main functions ---------- */

BOOL
GetCommand(
	STRING	strCmd
	)
{
	PrintFormat(SHELL_PREFIX_FORMAT);
	return GetString(strCmd, COMMAND_BUFFER_SIZE);
}

BOOL
InterpretCommand(
	STRING	strCmd
	)
{
	PSTRING	psParse;
	INT	iCmdsIndex;
	INT	(*pHandler)(PSTRING);
	INT	iHandlerReturn;

	psParse = ParseString(strCmd);

	if (psParse[0])
	{
		if ((iCmdsIndex = _cmds_bsearch(psParse[0])) == -1)
			PrintFormat("\'%s\' %s", psParse[0], COMMAND_INVALID_ERROR_MSG);
		else
		{
			PrintChar('\n');
			pHandler = ceCommands[iCmdsIndex].pCommandHandler;
			iHandlerReturn = pHandler(&psParse[1]);
		}
	}
	FreeArgs(psParse);
	return iHandlerReturn;
}

/* ---------- Handlers internal ---------- */
static
VOID
_get_filename_ext(
	PSTRING	psFullName,
	PSTRING	psFileName,
	PSTRING	psFileExt
	)
{
	UINT	i;

	for (i = 0; (*psFullName)[i]; i++)
	{
		if ((*psFullName)[i] == '.')
		{
			(*psFullName)[i] = 0;
			break;
		}
	}
	*psFileName = *psFullName;
	*psFileExt = *psFullName + i + 1;
}

static
VOID
_ls_print_number_zero_extended(
	UINT	uiFilesize,
	UINT	uiDepth
	)
{
	if (uiDepth == 0)
		return;
	_ls_print_number_zero_extended(uiFilesize / 10, uiDepth - 1);
	PrintChar(uiFilesize % 10 + '0');
}

static
VOID
_ls_print_file_attribs(
	PDIR_ENTRY	pde
	)
{
	UINT	i;
	
	for (i = 0; (i < FILE_MAX_NAME) && pde->chName[i]; i++)
	 	PrintChar(pde->chName[i]);
	for (; i < FILE_MAX_NAME; i++)
		PrintChar(' ');
	if (pde->chExt[0])
		PrintFormat(".");
	else
		PrintChar(' ');
	for (i = 0; (i < FILE_MAX_EXT) && pde->chExt[i]; i++)
		PrintChar(pde->chExt[i]);
	for (; i < FILE_MAX_EXT; i++)
		PrintChar(' ');
	PrintFormat(" | ");
	_ls_print_number_zero_extended(pde->uiFilesize, 5);
	PrintFormat("     | ");
	_ls_print_number_zero_extended(pde->teCreation.bYear, 2);
	PrintChar('/');
	_ls_print_number_zero_extended(pde->teCreation.bMonth, 2);
	PrintChar('/');
	_ls_print_number_zero_extended(pde->teCreation.bDay, 2);
	PrintChar(' ');
	_ls_print_number_zero_extended(pde->teCreation.bHour, 2);
	PrintChar(':');
	_ls_print_number_zero_extended(pde->teCreation.bMinute, 2);
	PrintChar(':');
	_ls_print_number_zero_extended(pde->teCreation.bSecond, 2);
	PrintFormat(" | ");
	_ls_print_number_zero_extended(pde->teOpen.bYear, 2);
	PrintChar('/');
	_ls_print_number_zero_extended(pde->teOpen.bMonth, 2);
	PrintChar('/');
	_ls_print_number_zero_extended(pde->teOpen.bDay, 2);
	PrintChar(' ');
	_ls_print_number_zero_extended(pde->teOpen.bHour, 2);
	PrintChar(':');
	_ls_print_number_zero_extended(pde->teOpen.bMinute, 2);
	PrintChar(':');
	_ls_print_number_zero_extended(pde->teOpen.bSecond, 2);
	PrintFormat("  | ");
	if (pde->bAttrib & DIR_ENTRY_ATTRIB_READONLY)
	{
		PrintCharAttrib('R', 0x01);
		PrintCharAttrib('O', 0x01);
	}
	if (pde->bAttrib & DIR_ENTRY_ATTRIB_SYSTEM)
		PrintCharAttrib('S', 0x04);
	if (pde->bAttrib & DIR_ENTRY_ATTRIB_HIDDEN)
		PrintCharAttrib('H', 0x07);
	if (pde->bAttrib & DIR_ENTRY_ATTRIB_SUBDIR)
		PrintCharAttrib('D', 0x0E);
	PrintChar('\n');
}

static
BOOL
_cat_internal(
	KOBJIO	ki
	)
{
	PTR	pBuffer;
	INT	iFilePtr;

	iFilePtr = 0;
	pBuffer = AllocHeapMem(32);
	if (!pBuffer)
		return FALSE;
	while (ReadFile(ki, pBuffer, iFilePtr, 32))
	{
		PrintFormat("%s", pBuffer);
		iFilePtr += 32;
	}
	FreeHeapMem(pBuffer);
	return TRUE;
}

/* ---------- Handlers ---------- */
INT
chCat(
	PSTRING psArgs
     )
{
	UINT	i;
	STRING	strFileName;
	STRING	strFileExt;
	KOBJIO	kiFile;

	i = 0;
	while (psArgs[i])
	{
		_get_filename_ext(&psArgs[i], &strFileName, &strFileExt);
		if ((kiFile = OpenFile(strFileName, strFileExt, FILE_OPEN_READ)) != INVALID_KOBJIO)
		{
			if (!_cat_internal(kiFile))
				PrintFormat("%s", CAT_FAIL_MSG);
			CloseFile(kiFile);
		}
		else
			PrintFormat("%s.%s %s", strFileName, strFileExt, CAT_INVALID_FILE_MSG);
		i++;
	}
	return TRUE;
}

INT
chClear(
	PSTRING psArgs
       )
{
#asm
	mov	ax, #0x0003
	int	0x10
#endasm
}

INT
chHalt(
	PSTRING	psArgs
      )
{
	if (psArgs[0])
		iShellExitCode = StrToInt(psArgs[0]);
	else
		iShellExitCode = 0;
	return FALSE;
}

INT
chLs(
	PSTRING psArgs
    )
{
	DIR_ENTRY	de;
	UINT		i;
	KOBJIO		ki;
	BOOL		bAll;
	UINT		uiArgPtr;

	if (!StringCompare(psArgs[0], "-a"))
	{
		bAll = 1;
		uiArgPtr = 1;
	}
	else
	{
		bAll = 0;
		uiArgPtr = 0;
	}
	if ((ki = OpenFile(psArgs[uiArgPtr] ? psArgs[uiArgPtr] : ".", "", FILE_OPEN_READ)) == INVALID_KOBJIO)
	{
		PrintFormat("%s is an invalid directory.", psArgs[uiArgPtr]);
		return TRUE;
	}
	PrintFormat("File Name    | File Size | Creation Time     | Last Opened Time   | Attribute\n");
	for (i = 0; i < 512 / sizeof(de); i++)
	{
		ReadFile(ki, &de, i * sizeof(de), sizeof(de));
		if (de.bAttrib & DIR_ENTRY_ATTRIB_USED && (!(de.bAttrib & DIR_ENTRY_ATTRIB_HIDDEN) || bAll))
			_ls_print_file_attribs(&de);
	}
	CloseFile(ki);
	return TRUE;
}

INT
chKernelinfo(
	PSTRING	psArgs
	)
{
	PrintFormat("***** Kernel Info *****\n");
	PrintFormat("- OS name:\t\t\t%s\n", ovi.strOSName);
	PrintFormat("- Kernel name:\t\t%s\n", ovi.strKernelName);
	PrintFormat("- Kernel version:\t%s\n", ovi.strKernelVer);
	return TRUE;
}

INT
chShellinfo(
	PSTRING psArgs
	)
{
	PrintFormat("***** Shell Info *****\n");
	PrintFormat("- Shell name:\t\t%s\n", SHELL_NAME);
	PrintFormat("- Shell version:\t%s\n", SHELL_VERSION);
	return TRUE;
}
