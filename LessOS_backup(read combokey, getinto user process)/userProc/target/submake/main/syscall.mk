/home/beyond/project/OStest/LessOS/userProc/target/object/main/syscall.o: /home/beyond/project/OStest/LessOS/userProc/src/main/syscall.asm
	mkdir -p /home/beyond/project/OStest/LessOS/userProc/target/object/main/
	nasm -f elf  -o /home/beyond/project/OStest/LessOS/userProc/target/object/main/syscall.o -I/home/beyond/project/OStest/LessOS/userProc/include/main/ /home/beyond/project/OStest/LessOS/userProc/src/main/syscall.asm
