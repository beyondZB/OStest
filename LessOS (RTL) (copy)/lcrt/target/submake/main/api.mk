/home/beyond/project/OStest/LessOS/userProc/target/object/main/api.o: /home/beyond/project/OStest/LessOS/userProc/src/main/api.c \
 /home/beyond/project/OStest/LessOS/userProc/include/main/api.h \
 /home/beyond/project/OStest/LessOS/userProc/include/main/type.h \
 /home/beyond/project/OStest/LessOS/userProc/include/main/string.h
	mkdir -p /home/beyond/project/OStest/LessOS/userProc/target/object/main/
	gcc -fno-builtin -m32 -fno-stack-protector -c -o /home/beyond/project/OStest/LessOS/userProc/target/object/main/api.o -I/home/beyond/project/OStest/LessOS/userProc/include/main/ /home/beyond/project/OStest/LessOS/userProc/src/main/api.c
