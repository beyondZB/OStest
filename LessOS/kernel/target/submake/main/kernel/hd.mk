/home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel/hd.o: /home/beyond/project/OStest/LessOS/kernel/src/main/kernel/hd.c \
 /home/beyond/project/OStest/LessOS/kernel/include/main/type.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/const.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/protect.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/type.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/const.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/string.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/lofs.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/lib/device.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/lib/platform/LessOS/device.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/proc.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/global.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/tty.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/buffer.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/proto.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/lofs.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/keyboard.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/tty.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/hd.h
	mkdir -p /home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel
	gcc -fno-builtin -m32 -fno-stack-protector -c -o /home/beyond/project/OStest/LessOS/kernel/target/object/main/kernel/hd.o -I/home/beyond/project/OStest/LessOS/kernel/include/main/ /home/beyond/project/OStest/LessOS/kernel/src/main/kernel/hd.c
