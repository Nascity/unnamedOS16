#ifndef __MAIN_H__
#define __MAIN_H__

#define ERROR		"ERROR"

#define FAT_RECORD	"FAT_RECORD"
#define ROOT_RECORD	"ROOT_RECORD"

#define FILE_OPEN	"FILE_OPEN"
#define FILE_READ	"FILE_READ"
#define FILE_WRITE	"FILE_WRITE"
#define FILE_CLOSE	"FILE_CLOSE"

#define FLOPPY_WRITE	"FLOPPY_WRITE"

#define FLOPPY_LOCATION	"../../../bin/floppy.img"

#define UNALLOCATED	0x0000
#define END_OF_CLUSTER	0xFFFF

#define DISABLE_4BYTE_ALIGN	__attribute__((packed))

#define DIR_ENTRY_ATTRIB_READONLY	0x01
#define DIR_ENTRY_ATTRIB_HIDDEN		0x02
#define DIR_ENTRY_ATTRIB_SYSTEM		0x04
#define DIR_ENTRY_ATTRIB_SUBDIR		0x08
#define DIR_ENTRY_ATTRIB_USED		0x80

#define ROOT_DIR_FILE_POS	1024

typedef char byte;
typedef short word;

typedef struct __date_entry
{
	byte year;
	byte month;
	byte day;
	byte hour;
	byte minute;
	byte second;
} DISABLE_4BYTE_ALIGN date_entry;

typedef struct __dir_entry
{
	char name[8];
	char ext[3];
	byte attrib;
	word filesize_high;
	word filesize_low;
	date_entry creation_date;
	date_entry opened_date;
	char reserved[2];
	word start_cluster;
} DISABLE_4BYTE_ALIGN dir_entry;

#endif
