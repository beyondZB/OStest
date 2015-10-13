cd kernel
kake 
cd ../boot
kake
cd ../userProc
./buildUser.sh
cd ..
ld -s -Ttext 0x30400 -m elf_i386 \
    -o kernel/target/kernel-0.0.1 \
    kernel/target/object/main/kernel/kernel.o \
    kernel/target/object/main/kernel/start.o \
    kernel/target/object/main/lib/kliba.o \
    kernel/target/object/main/lib/klib.o \
    kernel/target/object/main/lib/string.o \
    kernel/target/object/main/kernel/i8259.o \
    kernel/target/object/main/kernel/global.o \
    kernel/target/object/main/kernel/protect.o \
    kernel/target/object/main/kernel/main.o \
    kernel/target/object/main/kernel/clock.o \
    kernel/target/object/main/kernel/proc.o \
    kernel/target/object/main/kernel/syscall.o \
    kernel/target/object/main/kernel/keyboard.o \
    kernel/target/object/main/kernel/api.o \
    kernel/target/object/main/kernel/hd.o \
    kernel/target/object/main/kernel/lofs.o \
    kernel/target/object/main/lib/stringc.o \
    kernel/target/object/main/lib/platform/LessOS/device.o \
    kernel/target/object/main/kernel/buffer.o \
    kernel/target/object/main/kernel/vkey.o \
    kernel/target/object/main/kernel/tty.o

lofsutil-0.0.1 -c hd.img
lofsutil-0.0.1 -f hd.img
cp boot/target/object/main/mbr ./
cp boot/target/object/main/loader ./LOADER.BIN
cp boot/target/object/main/findLoader ./
cp kernel/target/kernel-0.0.1 ./KERNEL.BIN
cp userProc/target/userProc-0.0.1 ./
lofsutil-0.0.1 -b mbr hd.img
lofsutil-0.0.1 -e findLoader hd.img
lofsutil-0.0.1 -p LOADER.BIN hd.img
lofsutil-0.0.1 -p KERNEL.BIN hd.img
lofsutil-0.0.1 -p a.txt hd.img
lofsutil-0.0.1 -p userProc-0.0.1 hd.img
xxd -a -g 1 -s 3072 -l 512 hd.img
