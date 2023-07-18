MAKEFLAGS += --silent

CC := bcc
LD := ld86
NS := nasm

OUTDIR := bin
INCDIR := inc

CCFLAGS := -ansi -0c -I$(INCDIR)
LDFLAGS := -M -d
NSFLAGS := -fbin
QEMUFLAGS := -drive format=raw,file=$(OUTDIR)/floppy.img,if=floppy

BINS := bootloader.bin kernel.bin
OBJS := kmain.o cpu.o io.o memory.o process.o syscall.o

FLOPPY_MAKER := ../dev_utils/floppy_maker/bin/floppy_maker



all: outdir bootloader kernel floppy

clean:
	rm $(OUTDIR)/*

outdir:
	if [ ! -d "$(OUTDIR)" ]; then mkdir $(OUTDIR); fi


cpu.o: outdir
	$(CC) $(CCFLAGS) -o cpu.o cpu.c
kmain.o: outdir
	$(CC) $(CCFLAGS) -o kmain.o kmain.c
io.o: outdir
	$(CC) $(CCFLAGS) -o io.o io.c
memory.o:
	$(CC) $(CCFLAGS) -o memory.o memory.c
process.o: outdir
	$(CC) $(CCFLAGS) -o process.o process.c
syscall.o: outdir
	$(CC) $(CCFLAGS) -o syscall.o syscall.c


bootloader: outdir
	$(NS) $(NSFLAGS) -o $(OUTDIR)/bootloader.bin bootloader.asm

kernel: outdir $(OBJS)
	$(LD) $(LDFLAGS) -o $(OUTDIR)/kernel.bin $(OBJS) -L/usr/lib/bcc -lc
	rm $(OBJS) 

floppy: bootloader kernel
	if [ -f $(OUTDIR)/floppy.img ]; then rm $(OUTDIR)/floppy.img; fi
	$(FLOPPY_MAKER) $(BINS)

	truncate --size=1440K $(OUTDIR)/floppy.img

debug:
	qemu-system-i386 $(QEMUFLAGS)
