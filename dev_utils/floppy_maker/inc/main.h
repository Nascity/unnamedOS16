#ifndef __MAIN_H__
#define __MAIN_H__

#define ERROR		"ERROR"
#define FAT_RECORD	"FAT_RECORD"
#define FILE_OPEN	"FILE_OPEN"
#define FILE_READ	"FILE_READ"
#define FILE_WRITE	"FILE_WRITE"
#define FILE_CLOSE	"FILE_CLOSE"
#define FLOPPY_WRITE	"FLOPPY_WRITE"

#define FLOPPY_LOCATION	"../../../kernel/bin/floppy.img"

#define UNALLOCATED	0x0000
#define END_OF_CLUSTER	0xFFFF

typedef char byte;
typedef short word;

#endif
