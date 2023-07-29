#ifndef __INC_H__
#define __INC_H__

int get_ds(void);
int get_es(void);
int get_ss(void);

void set_ds(int ds);
void set_es(int es);
void set_ss(int ss);

void write_byte(void* offset, byte val);
void write_word(void* offset, int val);
byte read_byte(void* offset);
int read_word(void* offset);

#endif
