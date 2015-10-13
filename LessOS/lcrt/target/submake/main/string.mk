/home/beyond/project/OStest/LessOS/lcrt/target/object/main/string.o: /home/beyond/project/OStest/LessOS/lcrt/src/main/string.asm
	mkdir -p /home/beyond/project/OStest/LessOS/lcrt/target/object/main/
	nasm -f elf  -o /home/beyond/project/OStest/LessOS/lcrt/target/object/main/string.o -I/home/beyond/project/OStest/LessOS/lcrt/include/main/ /home/beyond/project/OStest/LessOS/lcrt/src/main/string.asm
