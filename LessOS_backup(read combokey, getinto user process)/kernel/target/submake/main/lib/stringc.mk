/home/beyond/project/OStest/LessOS/kernel/target/object/main/lib/stringc.o: \
 /home/beyond/project/OStest/LessOS/kernel/src/main/lib/stringc.c \
 /home/beyond/project/OStest/LessOS/kernel/include/main/string.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/type.h \
 /home/beyond/project/OStest/LessOS/kernel/include/main/const.h
	mkdir -p /home/beyond/project/OStest/LessOS/kernel/target/object/main/lib
	gcc -fno-builtin -m32 -fno-stack-protector -c -o /home/beyond/project/OStest/LessOS/kernel/target/object/main/lib/stringc.o -I/home/beyond/project/OStest/LessOS/kernel/include/main/ /home/beyond/project/OStest/LessOS/kernel/src/main/lib/stringc.c
