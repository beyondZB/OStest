/home/beyond/project/OStest/LessOS/lcrt/target/object/main/syscall.o: /home/beyond/project/OStest/LessOS/lcrt/src/main/syscall.asm
	mkdir -p /home/beyond/project/OStest/LessOS/lcrt/target/object/main/
	nasm -f elf  -o /home/beyond/project/OStest/LessOS/lcrt/target/object/main/syscall.o -I/home/beyond/project/OStest/LessOS/lcrt/include/main/ /home/beyond/project/OStest/LessOS/lcrt/src/main/syscall.asm
