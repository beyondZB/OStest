/home/beyond/project/OStest/LessOS/kernel/target/object/main/lib/device.o: /home/beyond/project/OStest/LessOS/kernel/src/main/lib/device.c \
 /home/beyond/project/OStest/LessOS/kernel/include/main/type.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/const.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/lib/device.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/lib/platform/LessOS/device.h
	mkdir -p /home/beyond/project/OStest/LessOS/kernel/target/object/main/lib
	gcc -fno-builtin -m32 -fno-stack-protector -c -o /home/beyond/project/OStest/LessOS/kernel/target/object/main/lib/device.o -I/home/beyond/project/OStest/LessOS/kernel/include/main/ /home/beyond/project/OStest/LessOS/kernel/src/main/lib/device.c
