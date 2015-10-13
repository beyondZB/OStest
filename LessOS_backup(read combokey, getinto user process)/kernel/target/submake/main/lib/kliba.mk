/home/beyond/project/OStest/LessOS/kernel/target/object/main/lib/kliba.o: /home/beyond/project/OStest/LessOS/kernel/src/main/lib/kliba.asm \
  /home/beyond/project/OStest/LessOS/kernel/include/main/sconst.inc
	mkdir -p /home/beyond/project/OStest/LessOS/kernel/target/object/main/lib
	nasm -f elf  -o /home/beyond/project/OStest/LessOS/kernel/target/object/main/lib/kliba.o -I/home/beyond/project/OStest/LessOS/kernel/include/main/ /home/beyond/project/OStest/LessOS/kernel/src/main/lib/kliba.asm
