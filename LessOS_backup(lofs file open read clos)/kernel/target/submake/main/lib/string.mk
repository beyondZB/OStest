/home/beyond/project/OStest/LessOS/kernel/target/object/main/lib/string.o: /home/beyond/project/OStest/LessOS/kernel/src/main/lib/string.asm
	mkdir -p /home/beyond/project/OStest/LessOS/kernel/target/object/main/lib
	nasm -f elf  -o /home/beyond/project/OStest/LessOS/kernel/target/object/main/lib/string.o -I/home/beyond/project/OStest/LessOS/kernel/include/main/ /home/beyond/project/OStest/LessOS/kernel/src/main/lib/string.asm
