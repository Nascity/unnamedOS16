MAKEFLAGS += --silent

OUTDIR		:= bin
MAINRELFLPMKR	:= ../../..

FLPMKRARGS	:= $(MAINRELFLPMKR)/$(OUTDIR)/bootloader.bin	\
		$(MAINRELFLPMKR)/$(OUTDIR)/kernel.bin solstice	\
		$(MAINRELFLPMKR)/$(OUTDIR)/test.bin testfile	\
		$(MAINRELFLPMKR)/$(OUTDIR)/shell.bin shell
DIRS		:= kernel shell

QEMUFLAGS	:= -drive format=raw,file=$(OUTDIR)/floppy.img,if=floppy,readonly=off
QEMUDEBUGFLAGS	:= $(QEMUFLAGS) -S -gdb tcp::8000

all: floppy

clean:
	rm $(OUTDIR)/floppy.img
	for dir in $(DIRS); do	\
	cd $$dir && make clean;	\
	cd ..;			\
	done

dirs:
	for dir in $(DIRS); do	\
	cd $$dir && make;	\
	cd ..;			\
	done

floppy: dirs
	if [ -f $(OUTDIR)/floppy.img ]; then rm $(OUTDIR)/floppy.img; fi
	touch $(OUTDIR)/floppy.img
	cd dev_utils/floppy_maker/bin && ./floppy_maker $(FLPMKRARGS)
	truncate --size=1440K $(OUTDIR)/floppy.img

debug:
	qemu-system-i386 $(QEMUFLAGS)

debug_remote:
	echo type \"target remote localhost:8000\" in gdb.
	qemu-system-i386 $(QEMUDEBUGFLAGS)

floppy_view:
	cd dev_utils/floppy_viewer/bin && ./floppy_viewer $(MAINRELFLPMKR)/$(OUTDIR)/floppy.img

floppy_raw:
	hexdump -C $(OUTDIR)/floppy.img
