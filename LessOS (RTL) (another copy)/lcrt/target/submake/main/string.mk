/home/beyond/project/OStest/LessOS/userProc/target/object/main/string.o: /home/beyond/project/OStest/LessOS/userProc/src/main/string.asm
	mkdir -p /home/beyond/project/OStest/LessOS/userProc/target/object/main/
	nasm -f elf  -o /home/beyond/project/OStest/LessOS/userProc/target/object/main/string.o -I/home/beyond/project/OStest/LessOS/userProc/include/main/ /home/beyond/project/OStest/LessOS/userProc/src/main/string.asm
