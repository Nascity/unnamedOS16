MAKEFLAGS += --silent

CC	:= bcc
LD	:= ld86

OUTDIR	:= ../bin
OBJS	:= uosapi_main.o main.o	command.o \
	uosapi_file.o uosapi_io.o uosapi_mem.o uosapi_proc.o uosapi_str.o

CCFLAGS	:= -ansi -0cV -Ms
LDFLAGS	:= -M -d


all: shell

clean:
	rm $(OBJS)


main.o:
	$(CC) $(CCFLAGS) -o main.o main.c
command.o:
	$(CC) $(CCFLAGS) -o command.o command.c
uosapi_file.o:
	$(CC) $(CCFLAGS) -o uosapi_file.o ../uosapi/uosapi_file.c
uosapi_io.o:
	$(CC) $(CCFLAGS) -o uosapi_io.o ../uosapi/uosapi_io.c
uosapi_main.o:
	$(CC) $(CCFLAGS) -o uosapi_main.o ../uosapi/uosapi_main.c
uosapi_mem.o:
	$(CC) $(CCFLAGS) -o uosapi_mem.o ../uosapi/uosapi_mem.c
uosapi_proc.o:
	$(CC) $(CCFLAGS) -o uosapi_proc.o ../uosapi/uosapi_proc.c
uosapi_str.o:
	$(CC) $(CCFLAGS) -o uosapi_str.o ../uosapi/uosapi_str.c


shell: $(OBJS)
	$(LD) $(LDFLAGS) -o $(OUTDIR)/shell.bin $(OBJS) -L/usr/lib/bcc -lc
	rm $(OBJS)
