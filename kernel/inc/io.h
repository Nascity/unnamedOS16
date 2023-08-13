#ifndef __IO_H__
#define __IO_H__

#define FILE_MAX_NAME		8
#define FILE_MAX_EXT		3
#define FILE_MAX_SIZE		0xFFFF

#define SECTORS_PER_HEAD	18

#define MAX_FIT_ENTRY		16
#define UNREGISTERED_FIT_ENTRY	0xFFFF

#define FILE_OPEN_WRITE		0x01
#define FILE_OPEN_READ		0x02

#define FIT_FLAGS_WRITE		0x01
#define FIT_FLAGS_READ		0x02
#define FIT_FLAGS_USED_ENTRY	0x04

#define FIT_FAT_INDEX		0
#define FIT_ROOT_INDEX		1

#define FAT_CLUSTER		0x0002
#define ROOT_CLUSTER		0x0003
#define SERIAL0_CLUSTER		0xFFFD
#define SERIAL1_CLUSTER		0xFFFE
#define INVALID_CLUSTER		0xFFFF

#define EOC	0xFFFF

// FIT index
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
	char reserved1[2];
	int filesize;
	date_entry creation_date;
	date_entry opened_date;
	char reserved2[2];
	word start_cluster;
} dir_entry;

typedef struct __file_interaction_table_entry
{
	int owner_pid;
	int start_cluster;
	int flags;
} file_interaction_table_entry;

void printchar(int ch);
void printhex(int num, bool uppercase);
void printnumber(int num);
void printstring(char* str, ...);
void printline(char* str, ...);

bool io_init(void);
kobj_io io_open(int cs, int flags, kobj_io working_dir, char name[FILE_MAX_NAME], char ext[FILE_MAX_EXT], int open_mode);
bool io_close(int cs, int flags, kobj_io koio);
bool io_write(int cs, int flags, kobj_io working_dir, kobj_io koio, void* buffer, int offset, int count);
bool io_read(int cs, int flags, kobj_io working_dir, kobj_io koio, void* buffer, int offset, int count);

#endif
