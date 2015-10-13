/home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel/syscall.o: /home/beyond/project/OStest/LessOS/kernel/src/main/kernel/syscall.asm \
  /home/beyond/project/OStest/LessOS/kernel/include/main/sconst.inc
	mkdir -p /home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel
	nasm -f elf  -o /home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel/syscall.o -I/home/beyond/project/OStest/LessOS/kernel/include/main/ /home/beyond/project/OStest/LessOS/kernel/src/main/kernel/syscall.asm
