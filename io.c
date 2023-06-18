#include "inc/inc.h"

void format(char letter, int* ap);


void printchar(int ch)
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



void printhex(int num, bool uppercase)
{
	char str[8];
	int i;
	int temp;

	//  0   1   2   3   4   5   6   7
	// '0' 'x' '0' '0' '0' '0'  0   0
	for (i = 0; i < 8; i++) str[i] = '0';
	str[1] = 'x';
	str[6] = 0;	

	for (i = 5; i >= 2; i--)
	{
		temp = (num % 16);
		if (temp < 10) str[i] = temp + '0';
		else str[i] = uppercase ? temp - 10 + 'A' : temp - 10 + 'a';
		num /= 16;

		if (num == 0) break;
	}

	printstring(str);
}

void printnumber(int num)
{
	char str[8];
	int i;

	str[7] = 0;

	for (i = 6; i >= 0; i--)
	{
		str[i] = (num % 10) + '0';
		num /= 10;

		if (num == 0) break;
	}

	printstring(&str[i]);
}

void printstring(char* str, ...)
{
	char* ap = (char*)&str + 2;

	while (*str)
	{
		if (*str != '%')
		{
			printchar(*(str++));
			continue;
		}

		// if the letter is %
		// check for next letter *(++str)
		format(*(++str), (int*)ap);
		ap += 2;
		str++;
	}
}



// parameter:	next letter of %
//		argument pointer
void format(char letter, int* ap)
{
	switch (letter)
	{
		case 'c':
			printchar(*ap);
			break;
		case 'd':
			printnumber(*ap);
			break;
		case 's':
			printstring(*ap);
		case 'X':
			printhex(*ap, true);
			break;
		case 'x':
			printhex(*ap, false);
			break;
		default:
			printchar(' ');
			break;
	}
}
