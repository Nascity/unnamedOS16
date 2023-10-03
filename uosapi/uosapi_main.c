#include "inc/uosapi.h"

/* ---------- internal functions ---------- */
static
ARGS
_mkargs(
	PTR	pCmdLine
       );

INT
main(
	VOID
    )
{
	PTR	pCmdLine;
	ARGS	args;
	INT	iReturn;

#asm
	mov	ax, word ptr [0xFFFE]
	mov	word ptr [bp - 6], ax
#endasm
	args = _mkargs(pCmdLine);
	iReturn = UosMain(pCmdLine, args);
#asm
	mov	ax, word ptr [bp - 10]
	mov	sp, bp
	add	sp, #0x04
	pop	bp
	retf
#endasm
}

static
BOOL
_is_space(
	CHAR	ch
	)
{
	return ch == '\n' || ch == '\r' || ch == '\v'
		|| ch == '\t' || ch == '\f' || ch == ' ';
}

static
UINT
_count(
	PTR	pCmdLine
      )
{
	UINT	i;
	UINT	uiCnt;
	BOOL	bIsWord;
	BOOL	bWasWord;

	i = 0;
	uiCnt = 0;
	bWasWord = 0;
	while (pCmdLine[i])
	{
		bIsWord = !_is_space(pCmdLine[i]);
		if (!bWasWord && bIsWord)
			uiCnt++;
		bWasWord = bIsWord;
		i++;
	}
	return uiCnt;
}

static
STRING
_parse(
	PTR	*dpCmdLine,
	PUINT	puiLength
      )
{
	UINT	i;
	BOOL	bIsWord;
	BOOL	bWasWord;
	STRING	ret;

	bWasWord = 0;
	while (**dpCmdLine)
	{
		bIsWord = !_is_space(**dpCmdLine);
		if (!bWasWord && bIsWord)
		{
			i = 0;
			while ((*dpCmdLine)[i] && !_is_space((*dpCmdLine)[i]))
				i++;
			ret = AllocHeapMem(i + 1);
			if (ret == NULL)
				return ret;
			Memcpy(ret, *dpCmdLine, i);
			ret[i] = 0;
			*dpCmdLine += i;
			*puiLength = i;
			return ret;
		}
		bWasWord = bIsWord;
		(*dpCmdLine)++;
	}
	return NULL;
}

static
ARGS
_mkargs(
	PTR	pCmdLine
       )
{
	ARGS		ag;
	ARGS_ENTRY	ae;
	UINT		i;

	ag.uiArgCount = _count(pCmdLine);
	ag.pArgs = AllocHeapMem(sizeof(ARGS_ENTRY) * ag.uiArgCount);

	i = 0;
	while (i < ag.uiArgCount)
	{
		ae.strArg = _parse(&pCmdLine, &(ae.uiArgLength));

		if (ae.strArg == NULL)
		{
			ag.uiArgCount = 0;
			FreeHeapMem(ag.pArgs);
			return ag;
		}
		Memcpy(ag.pArgs + i, &ae, sizeof(ARGS_ENTRY));
		i++;
	}

	return ag;
}

/* ---------- Entrypoint API ---------- */
// WIP
