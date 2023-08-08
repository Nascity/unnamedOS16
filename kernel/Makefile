MAKEFLAGS += --silent

CC := bcc
LD := ld86
NS := nasm

OUTDIR := ../bin
OBJS := kmain.o io.o memory.o process.o syscall.o

CCFLAGS := -ansi -0c -I$(INCDIR)
LDFLAGS := -M -d
NSFLAGS := -fbin


all: bootloader kernel

clean:
	rm *.o


kmain.o:
	$(CC) $(CCFLAGS) -o kmain.o kmain.c
io.o:
	$(CC) $(CCFLAGS) -o io.o io.c
memory.o:
	$(CC) $(CCFLAGS) -o memory.o memory.c
process.o:
	$(CC) $(CCFLAGS) -o process.o process.c
syscall.o:
	$(CC) $(CCFLAGS) -o syscall.o syscall.c


bootloader:
	$(NS) $(NSFLAGS) -o $(OUTDIR)/bootloader.bin bootloader.asm

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o $(OUTDIR)/kernel.bin $(OBJS) -L/usr/lib/bcc -lc
	rm $(OBJS) 
