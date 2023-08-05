#ifndef __IO_H__
#define __IO_H__

#define FILE_MAX_NAME	8
#define FILE_MAX_EXT	3

#define STDIN_KOBJIO	0x0000
#define STDOUT_KOBJIO	0x0001
#define STDERR_KOBJIO	0x0002
#define ROOTDIR_KOBJIO	0x0003
#define SERIAL0_KOBJIO	0xFFFD
#define SERIAL1_KOBJIO	0xFFFE
#define INVALID_KOBJIO	0xFFFF

typedef int kobj_io;

typedef struct __date_entry
{
	byte year;
	byte month;
	byte day;
	byte hour;
	byte minute;
	byte second;
} date_entry;

typedef struct __dir_entry
{
	char name[FILE_MAX_NAME];
	char ext[FILE_MAX_EXT];
	byte attrib;
	int fliesize_high;
	int filesize_low;
	date_entry creation_date;
	date_entry opened_date;
	char reserved[2];
	word start_cluster;
} dir_entry;

void printchar(int ch);
void printhex(int num, bool uppercase);
void printnumber(int num);
void printstring(char* str, ...);
void printline(char* str, ...);

kobj_io io_open(int cs, int flags, char name[FILE_MAX_NAME], int open_mode);
bool io_close(int cs, int flags, kobj_io koio);
bool io_write(int cs, int flags, kobj_io koio, void* buffer, int offset, int count);
bool io_read(int cs, int flags, kobj_io koio, void* buffer, int offset, int count);

#endif
