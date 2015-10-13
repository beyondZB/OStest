/home/beyond/project/OStest/LessOS/lcrt/target/object/main/api.o: /home/beyond/project/OStest/LessOS/lcrt/src/main/api.c \
 /home/beyond/project/OStest/LessOS/lcrt/include/main/api.h \
 /home/beyond/project/OStest/LessOS/lcrt/include/main/type.h \
 /home/beyond/project/OStest/LessOS/lcrt/include/main/string.h \
 /home/beyond/project/OStest/LessOS/lcrt/include/main/lofs.h \
 /home/beyond/project/OStest/LessOS/lcrt/include/main/lib/device.h \
 /home/beyond/project/OStest/LessOS/lcrt/include/main/type.h \
 /home/beyond/project/OStest/LessOS/lcrt/include/main/lib/platform/LessOS/device.h
	mkdir -p /home/beyond/project/OStest/LessOS/lcrt/target/object/main/
	gcc -fno-builtin -m32 -fno-stack-protector -c -o /home/beyond/project/OStest/LessOS/lcrt/target/object/main/api.o -I/home/beyond/project/OStest/LessOS/lcrt/include/main/ /home/beyond/project/OStest/LessOS/lcrt/src/main/api.c
