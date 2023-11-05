#include "inc/uosapi.h"

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
			return ret;
		}
		bWasWord = bIsWord;
		(*dpCmdLine)++;
	}
	return NULL;
}

static
INT
_recursive_sti(
	STRING	str,
	INT	i
	)
{
	if (str[1] >= '0' && str[1] <= '9')
		return _recursive_sti(str + 1, i * 10 + str[0] - '0');
	return i * 10 + str[0] - '0';
}

PSTRING
ParseString(
	PTR	pCmdLine
       )
{
	UINT	i;
	UINT	uiArgsCount;
	PSTRING	ret;

	uiArgsCount = _count(pCmdLine);
	ret = AllocHeapMem(sizeof(STRING) * (uiArgsCount + 1));

	i = 0;
	while (i < uiArgsCount)
	{
		ret[i] = _parse(&pCmdLine);

		if (ret[i] == NULL)
		{
			FreeArgs(ret);
			return NULL;
		}
		i++;
	}
	ret[i] = NULL;

	return ret;
}

VOID
FreeArgs(
	PSTRING	psArgs
	)
{
	UINT	i;

	for (i = 0; psArgs[i]; i++)
		FreeHeapMem(psArgs[i]);
	FreeHeapMem(psArgs);
}

INT
StringCompare(
	STRING	str1,
	STRING	str2
	)
{
	UINT	i;
	INT	diff;

	i = 0;
	while (str1[i] || str2[i])
	{
		diff = str1[i] - str2[i];
		if (diff)
			return diff;
		i++;
	}
	return 0;
}

INT
StrToInt(
	STRING	str
	)
{
	UINT	i;
	INT	sign;

	i = 0;
	sign = 1;
	if (str[0] == '-')
	{
		i++;
		sign = -1;
	}
	return sign * _recursive_sti(str + i, 0);
}
