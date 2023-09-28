#include "inc/uosapi.h"

/* ---------- internal functions ---------- */
static
VOID
_printstring(
	STRING	strString
	)
{
	INT	i;

	i = 0;
	while (strString[i])
	{
		PrintChar(strString[i]);
		i++;
	}
}

static
INT
_format(
	CHAR	chFormatChar,
	PWORD	pArgPtr,
	INT	iUnused
       )
{
	switch (chFormatChar)
	{
		case 'c':
			PrintChar(*pArgPtr);
			return 1;
		case 'd':
			PrintNumber(*pArgPtr);
			return 1;
		case 's':
			_printstring(*pArgPtr);
			return 1;
		case 'X':
			PrintHex(*pArgPtr, TRUE, iUnused);
			if (iUnused)
				return 2;
			else return 1;
		case 'x':
			PrintHex(*pArgPtr, FALSE, iUnused);
			if (iUnused)
				return 2;
			else return 1;
		case '#':
			return _format(chFormatChar + 1, pArgPtr, TRUE);
		case '%':
			PrintChar('%');
			return 1;
		default:
			PrintChar(' ');
			return 1;
	}
}

/* ---------- Screen IO API ---------- */
VOID
PrintChar(
	CHAR	chCharacter
	)
{
#asm
	push	bp
	mov	bp, sp
	push	ax
	push	bx

	mov	ah, #0x0e
	mov	al, byte ptr [bp + 4]
	xor	bx, bx
	int	0x10

	pop	bx
	pop	ax
	pop	bp
#endasm
}

VOID
PrintFormat(
	STRING	strFormat,
	...
	)
{
	PWORD	ap = (PWORD)&strFormat + 1;
	INT	i;

	i = 0;
	while (strFormat[i])
	{
		if (strFormat[i] != '%')
		{
			PrintChar(strFormat[i++]);
			continue;
		}
		
		i += _format(strFormat[++i], ap, FALSE);
		ap++;
	}
}

VOID
PrintHex(
	UINT	iNumber,
	BOOL	bCapitalize,
	BOOL	bPrefix
	)
{
	if (bPrefix)
		PrintFormat("0x");
	if (iNumber >= 16)
		PrintHex(iNumber / 16, bCapitalize, FALSE);
	if (bCapitalize)
		PrintChar("0123456789ABCDEF"[iNumber  % 16]);
	else
		PrintChar("0123456789abcdef"[iNumber % 16]);
}

VOID
PrintNumber(
	INT	iNumber
	)
{
	if (iNumber < 0)
	{
		PrintChar('-');
		iNumber *= -1;
	}
	if (iNumber == 0xFFFF)
	{
		PrintFormat("-32768");
		return;
	}
	if (iNumber > 10)
		PrintNumber(iNumber / 10);
	PrintChar(iNumber % 10 + '0');
}
