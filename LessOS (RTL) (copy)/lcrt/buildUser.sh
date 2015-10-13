kake

ld -s -Ttext 0x700000 -m elf_i386 -o /home/beyond/project/OStest/LessOS/userProc/target/userProc-0.0.1 \
/home/beyond/project/OStest/LessOS/userProc/target/object/main/userProc.o \
/home/beyond/project/OStest/LessOS/userProc/target/object/main/api.o  \
/home/beyond/project/OStest/LessOS/userProc/target/object/main/syscall.o \
/home/beyond/project/OStest/LessOS/userProc/target/object/main/stringc.o \
/home/beyond/project/OStest/LessOS/userProc/target/object/main/string.o
