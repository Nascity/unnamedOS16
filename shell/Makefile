MAKEFLAGS += --silent

CC	:= bcc
LD	:= ld86

OUTDIR	:= ../bin
OBJS	:= main.o	\
	uosapi_file.o uosapi_io.o uosapi_mem.o uosapi_proc.o

CCFLAGS	:= -ansi -0c
LDFLAGS	:= -M -d


all: shell

clean:
	rm $(OBJS)


main.o:
	$(CC) $(CCFLAGS) -o main.o main.c
uosapi_file.o:
	$(CC) $(CCFLAGS) -o uosapi_file.o ../uosapi/uosapi_file.c
uosapi_io.o:
	$(CC) $(CCFLAGS) -o uosapi_io.o ../uosapi/uosapi_io.c
uosapi_mem.o:
	$(CC) $(CCFLAGS) -o uosapi_mem.o ../uosapi/uosapi_mem.c
uosapi_proc.o:
	$(CC) $(CCFLAGS) -o uosapi_proc.o ../uosapi/uosapi_proc.c


shell: $(OBJS)
	$(LD) $(LDFLAGS) -o $(OUTDIR)/shell.bin $(OBJS) -L/usr/lib/bcc -lc
	rm $(OBJS)
