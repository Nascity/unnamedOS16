MAKEFLAGS += --silent

CC := bcc
LD := ld86
NS := nasm

OUTDIR := bin
INCDIR := inc

CCFLAGS := -ansi -0c -I$(INCDIR)
LDFLAGS := -M -d
NSFLAGS := -fbin

BINS := bootloader.bin kernel.bin
OBJS := kmain.o io.o process.o



all: outdir bootloader kernel floppy

clean:
	rm $(OUTDIR)/*

outdir:
	if [ ! -d "$(OUTDIR)" ]; then mkdir $(OUTDIR); fi

bootloader: outdir
	$(NS) $(NSFLAGS) -o $(OUTDIR)/bootloader.bin bootloader.asm

kernel: outdir $(OBJS)
	$(LD) $(LDFLAGS) -o $(OUTDIR)/kernel.bin $(OBJS) -L/usr/lib/bcc -lc
	rm $(OBJS) 

floppy: bootloader kernel
	if [ -f floppy.img ]; then rm floppy.img; fi
	touch $(OUTDIR)/floppy.img
	for file in $(BINS); do \
		cat $(OUTDIR)/$$file >> $(OUTDIR)/floppy.img; \
	done
	truncate --size=1440K $(OUTDIR)/floppy.img



kmain.o: outdir
	$(CC) $(CCFLAGS) -o kmain.o kmain.c
io.o: outdir
	$(CC) $(CCFLAGS) -o io.o io.c
process.o: outdir
	$(CC) $(CCFLAGS) -o process.o process.c
