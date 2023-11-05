#include "inc/uosapi.h"

/* ---------- internal functions ---------- */
static
VOID
_printchar_internal(
	CHAR	ch
	)
{
#asm
	push	bp
	mov	bp, sp
	push	bx

	mov	ah, #0x0e
	mov	al, byte ptr [bp + 4]
	xor	bx, bx
	int	0x10

	pop	bx
	pop	bp
#endasm
}

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
		if (strString[i] == '\n')
			PrintChar('\r');
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

static
VOID
_swap_char(
	PCHAR	c1,
	PCHAR	c2
	)
{
	CHAR	temp;

	temp = *c2;
	*c2 = *c1;
	*c1 = temp;
}

static
VOID
_set_cursor_pos(
	UINT	uiCursorInitPos,
	UINT	uiOffset
	)
{
	UINT	uiNewOffset;

	uiNewOffset = uiCursorInitPos + uiOffset;
	SetCursorPos(uiNewOffset);
}

static
VOID
InputLetter(
	STRING	strBuffer,
	UINT	uiPtr,
	CHAR	chChar
	)
{
	UINT	i;
	CHAR	chTemp;

	chTemp = strBuffer[uiPtr];
	for (i = uiPtr + 1; chTemp; i++)
		_swap_char(&chTemp, &strBuffer[i]);
	strBuffer[uiPtr] = chChar;
	strBuffer[i] = 0;
}

static
VOID
DeleteLetter(
	STRING	strBuffer,
	UINT	uiPtr,
	)
{
	UINT	i;
	
	if (!uiPtr)
		return;
	for (i = uiPtr - 1 ; strBuffer[i - 1]; i++)
		_swap_char(&strBuffer[i], &strBuffer[i + 1]);
	strBuffer[i] = 0;
}

/* ---------- Screen IO API ---------- */
VOID
PrintChar(
	CHAR	chCharacter
	)
{
	UINT	uiCursorPos;
	UINT	uiTabSize;
	UINT	i;

	if (chCharacter == '\n')
	{
		_printchar_internal('\r');
		_printchar_internal('\n');
	}
	else if (chCharacter == '\t')
	{
		uiCursorPos = GetCursorPos();
		uiTabSize = 4 - (uiCursorPos & 0xFF) % 4;
		if ((uiCursorPos & 0xFF) + uiTabSize >= TEXTMODE_SCREEN_WIDTH)
			PrintChar('\n');
		else
			for (i = 0; i < uiTabSize; i++)
				_printchar_internal(' ');
	}
	else
		_printchar_internal(chCharacter);
}

VOID
PrintCharAttrib(
	CHAR	chCharacter,
	BYTE	bAttrib
	)
{
	INT	unused;
#asm
	push	bx
	push	cx

	mov	ah, #0x09
	mov	al, byte ptr [bp + 4]
	xor	bh, bh
	mov	bl, byte ptr [bp + 6]
	mov	cx, #0x01
	int	0x10

	pop	cx
	pop	bx
#endasm
	CursorStep(TRUE);
}

VOID
Print(
	STRING	strString
     )
{
	UINT	i;

	i = 0;
	while (strString[i])
	{
		if (strString[i] == '\n')
			PrintChar('\r');
		PrintChar(strString[i]);
		i++;
	}
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
			if (strFormat[i] == '\n')
				PrintChar('\r');
			PrintChar(strFormat[i++]);
			continue;
		}
		
		i += _format(strFormat[++i], ap, FALSE);
		ap++;
	}
}

VOID
PrintAttrib(
	STRING	strString,
	BYTE	bAttrib
	)
{
	INT	i;

	i = 0;
	while (strString[i])
	{
		PrintCharAttrib(strString[i], bAttrib);
		i++;
	}
}

VOID
PrintHex(
	UINT	uiNumber,
	BOOL	bCapitalize,
	BOOL	bPrefix
	)
{
	if (bPrefix)
		PrintFormat("0x");
	if (uiNumber >= 16)
		PrintHex(uiNumber / 16, bCapitalize, FALSE);
	if (bCapitalize)
		PrintChar("0123456789ABCDEF"[uiNumber  % 16]);
	else
		PrintChar("0123456789abcdef"[uiNumber % 16]);
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
	if (iNumber >= 10)
		PrintNumber(iNumber / 10);
	PrintChar(iNumber % 10 + '0');
}

UINT
GetCursorPos(
	VOID
	)
{
#asm
	push	bp
	mov	bp, sp
	push	bx
	push	cx
	push	dx

	mov	ah, #0x03
	xor	bh, bh
	int	#0x10

	mov	ax, dx
	pop	dx
	pop	cx
	pop	bx
	pop	bp
#endasm
}

VOID
SetCursorPos(
	UINT	uiCursorPos
	)
{
#asm
	push	bp
	mov	bp, sp
	push	bx
	push	dx

	mov	dx, word ptr [bp + 4]
	mov	ah, #0x02
	xor	bh, bh
	int	0x10

	pop	dx
	pop	bx
	pop	bp
#endasm
}

VOID
SetCursorPosEx(
	BYTE	bColumn,
	BYTE	bRow
	)
{
#asm
	push	bp
	mov	bp, sp
	push	bx
	push	dx

	mov	dh, byte ptr [bp + 6]
	mov	dl, byte ptr [bp + 4]
	xor	bh, bh
	mov	ah, #0x02
	int	0x10

	pop	dx
	pop	bx
	pop	bp
#endasm
}

VOID
CursorStep(
	BOOL	bForward
	)
{
	UINT	uiCursorPos;

	uiCursorPos = GetCursorPos();
	if (bForward)
	{
		uiCursorPos++;
		if ((uiCursorPos & 0xFF) >= TEXTMODE_SCREEN_WIDTH)
		{
			uiCursorPos += 0x0100;
			uiCursorPos &= 0xFF00;
		}
	}
	else
	{
		uiCursorPos--;
		if ((uiCursorPos & 0xFF) >= TEXTMODE_SCREEN_WIDTH)
			uiCursorPos &= 0xFF00;
	}
	SetCursorPos(uiCursorPos);
}

/* ---------- Keyboard IO API ---------- */
CHAR
GetChar(
	VOID
       )
{
#asm
GetChar_loop:
	xor	ax, ax
	int	0x16
	xor	ah, ah
	and	al, #0xFF
	jz	GetChar_loop
#endasm
}

BOOL
GetString(
	STRING	strBuffer,
	UINT	uiBufferCount
	)
{
	UINT	i;
	UINT	ch;
	UINT	uiBufferLen;
	UINT	uiCursorInitPos;

	i = 0;
	uiBufferLen = 0;
	uiCursorInitPos = GetCursorPos();
	if (uiBufferCount >= GETSTRING_BUFFER_COUNT_LIMIT)
		uiBufferCount = GETSTRING_BUFFER_COUNT_LIMIT;
	while (uiBufferLen < uiBufferCount 
		&& GET_SCAN_CODE(ch = GetCharWithScanCode()) != ENTER_SCAN_CODE)
	{
		if (!GET_ASCII_CODE(ch))
		{
			switch (GET_SCAN_CODE(ch))
			{
			case LEFT_ARROW_SCAN_CODE:
				if (i != 0)
				{
					i--;
					CursorStep(FALSE);
				}
				break;
			case RIGHT_ARROW_SCAN_CODE:
				if (strBuffer[i])
				{
					i++;
					CursorStep(TRUE);
				}
				break;
			}
			continue;
		}
		if (GET_SCAN_CODE(ch) == BACKSPACE_SCAN_CODE)
		{
			DeleteLetter(strBuffer, i);
			if (i)
				i--;
		}
		else
			InputLetter(strBuffer, i, ch);
		if (GET_SCAN_CODE(ch) != BACKSPACE_SCAN_CODE)
		{
			i++;
			uiBufferLen++;
		}
		else if (uiBufferLen)
			uiBufferLen--;
		_set_cursor_pos(uiCursorInitPos, 0);
		Print(strBuffer);
		Print(" ");
		_set_cursor_pos(uiCursorInitPos, i);
	}
	PrintFormat("\n");
	if (uiBufferLen >= uiBufferCount)
		return FALSE;
	return TRUE;
}

UINT
GetCharWithScanCode(
	VOID
	)
{
#asm
	xor	ax, ax
	int	0x16
#endasm
}
