/home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel/buffer.o: \
 /home/beyond/project/OStest/LessOS/kernel/src/main/kernel/buffer.c \
 /home/beyond/project/OStest/LessOS/kernel/include/main/buffer.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/type.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/const.h
	mkdir -p /home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel
	gcc -fno-builtin -m32 -fno-stack-protector -c -o /home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel/buffer.o -I/home/beyond/project/OStest/LessOS/kernel/include/main/ /home/beyond/project/OStest/LessOS/kernel/src/main/kernel/buffer.c
