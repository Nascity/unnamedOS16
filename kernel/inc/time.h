#ifndef __TIME_H__
#define __TIME_H__

typedef struct __time_entry
{
	byte year;
	byte month;
	byte day;
	byte hour;
	byte minute;
	byte second;
} time_entry;

bool get_time(time_entry* pde);

#endif
