############################################################
#			LessOS Makefile			   #
############################################################


#KERNEL ENTRY POINT
#Must be the same with "KernelEntryPointPhyAddr" in load.inc
ENTRYPOINT = 0x30400

# Offset of entry point in kernel file
# It depends on ENTRYPOINT
ENTRYOFFSET	=   0x400

#Programs, flags, etc.
ASM 		= nasm
DASM 		= ndisasm
CC 		= gcc
LD 		= ld
ASMBFLAGS	= -I boot/include/
ASMKFLAGS	= -I include/ -f elf
CFLAGS		= -I include/ -c -fno-builtin -m32 -fno-stack-protector
LDFLAGS 	= -s -Ttext $(ENTRYPOINT) -m elf_i386
DASMFLAGS	= -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)

#Programs
BOOT 		= boot/mbr boot/findLoader boot/LOADER.BIN
KERNEL 		= kernel/KERNEL.BIN
OBJS 		= kernel/kernel.o kernel/start.o lib/kliba.o lib/klib.o lib/string.o\
		  kernel/i8259.o kernel/global.o kernel/protect.o kernel/main.o kernel/clock.o
DASMOUTPUT	= kernel.bin.asm

.PHONY: everything final image clean realclean  all buildImg

everything: $(BOOT) $(KERNEL) image

all: realclean $(BOOT) $(KERNEL) image clean

image: buildImg writeImg

clean:
	rm -f $(OBJS)

realclean:
	rm -f $(OBJS) $(BOOT) $(KERNEL)

disasm:
	$(DASM) $(DASMFLAGS) $(KERNEL) > $(DASMOUTPUT)

buildImg:
	dd if=/dev/zero of=b.img bs=1024 count=1440
	lofsutil-0.0.1 -c b.img
	lofsutil-0.0.1 -f b.img

writeImg:
	cp boot/mbr ./
	lofsutil-0.0.1 -b mbr b.img
	rm mbr
	cp boot/findLoader ./
	lofsutil-0.0.1 -e findLoader b.img
	rm findLoader
	cp boot/LOADER.BIN ./
	lofsutil-0.0.1 -p LOADER.BIN b.img
	rm LOADER.BIN
	cp kernel/KERNEL.BIN ./
	lofsutil-0.0.1 -p KERNEL.BIN b.img
	rm KERNEL.BIN

boot/mbr: boot/mbr.asm boot/include/load.inc boot/include/lofs.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

boot/findLoader: boot/findLoader.asm boot/include/load.inc boot/include/lofs.inc boot/include/pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

boot/LOADER.BIN: boot/loader.asm boot/include/load.inc boot/include/lofs.inc boot/include/pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $(KERNEL) $^

kernel/kernel.o: kernel/kernel.asm include/sconst.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<

kernel/start.o: kernel/start.c include/type.h include/const.h include/protect.h \
		include/proto.h include/string.h include/global.h include/proc.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/main.o: kernel/main.c include/type.h include/const.h include/protect.h \
		include/string.h include/proc.h include/proto.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/i8259.o: kernel/i8259.c include/type.h include/const.h include/protect.h include/proto.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/protect.o: kernel/protect.c include/type.h include/const.h include/protect.h \
		  include/global.h include/proto.h include/proc.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/global.o: kernel/global.c include/type.h include/const.h include/protect.h \
		 include/proto.h include/global.h include/proc.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/clock.o: kernel/clock.c include/type.h include/const.h include/protect.h \
		 include/proto.h include/global.h include/proc.h
	$(CC) $(CFLAGS) -o $@ $<
 

lib/kliba.o: lib/kliba.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

klib.o: lib/klib.c /usr/include/stdc-predef.h include/type.h \
	include/const.h include/protect.h include/proto.h include/string.h \
	include/global.h include/proc.h
	$(CC) $(CFLAGS) -o $@ $<


lib/string.o: lib/string.asm 
	$(ASM) $(ASMKFLAGS) -o $@ $<

