MAKEFLAGS += --silent

AS := as86
CC := bcc
NS := nasm

CCFLAGS := -3S
NSFLAGS := -fbin

OUTDIR := ./bin



all: outdir bins

clean:
	rm $(OUTDIR)/*.s $(BINDIR)/*.bin



outdir:
	if [ ! -d "$(OUTDIR)" ]; then mkdir $(OUTDIR); fi

bins: kmain bootloader

kmain:
	$(CC) $(CCFLAGS) -o $(OUTDIR)/kmain.s kmain.c
	$(AS) -b $(OUTDIR)/kmain.bin $(OUTDIR)/kmain.s

bootloader:
	$(NS) $(NSFLAGS) -o $(OUTDIR)/bootloader.bin bootloader.asm
