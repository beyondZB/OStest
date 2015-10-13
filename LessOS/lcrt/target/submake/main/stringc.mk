/home/beyond/project/OStest/LessOS/lcrt/target/object/main/stringc.o: /home/beyond/project/OStest/LessOS/lcrt/src/main/stringc.c \
 /home/beyond/project/OStest/LessOS/lcrt/include/main/string.h \
 /home/beyond/project/OStest/LessOS/lcrt/include/main/type.h
	mkdir -p /home/beyond/project/OStest/LessOS/lcrt/target/object/main/
	gcc -fno-builtin -m32 -fno-stack-protector -c -o /home/beyond/project/OStest/LessOS/lcrt/target/object/main/stringc.o -I/home/beyond/project/OStest/LessOS/lcrt/include/main/ /home/beyond/project/OStest/LessOS/lcrt/src/main/stringc.c
