#ifndef __UOSTYPES_H__
#define __UOSTYPES_H__

// Basic type definition
typedef char		BOOL;
typedef char		CHAR;
typedef unsigned char	BYTE;
typedef int		INT;
typedef char*		PBOOL;
typedef char*		PCHAR;
typedef int*		PINT; typedef unsigned char*	PTR;
typedef unsigned int*	PUINT;
typedef unsigned short*	PWORD;
typedef char**		PSTRING;
typedef char*		STRING;
typedef unsigned int	UINT;
typedef unsigned short	WORD;
#define VOID		void

// Kernel object type definition
typedef int		KOBJIO;

typedef struct	__TIME_ENTRY
{
	BYTE	bYear;
	BYTE	bMonth;
	BYTE	bDay;
	BYTE	bHour;
	BYTE	bMinute;
	BYTE	bSecond;
}		TIME_ENTRY, *PTIME_ENTRY;

typedef struct	__DIR_ENTRY
{
	CHAR		chName[FILE_MAX_NAME];
	CHAR		chExt[FILE_MAX_EXT];
	BYTE		bAttrib;
	CHAR		reserved1[2];
	UINT		uiFilesize;
	TIME_ENTRY	teCreation;
	TIME_ENTRY	teOpen;
	CHAR		reserved2[2];
	WORD		wStartCluster;
}		DIR_ENTRY, *PDIR_ENTRY;

#endif
