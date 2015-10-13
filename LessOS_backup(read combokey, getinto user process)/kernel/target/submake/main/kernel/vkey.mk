/home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel/vkey.o: /home/beyond/project/OStest/LessOS/kernel/src/main/kernel/vkey.c \
 /home/beyond/project/OStest/LessOS/kernel/include/main/vkey.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/type.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/const.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/const.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/keyboard.h
	mkdir -p /home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel
	gcc -fno-builtin -m32 -fno-stack-protector -c -o /home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel/vkey.o -I/home/beyond/project/OStest/LessOS/kernel/include/main/ /home/beyond/project/OStest/LessOS/kernel/src/main/kernel/vkey.c
