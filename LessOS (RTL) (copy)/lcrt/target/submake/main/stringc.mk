/home/beyond/project/OStest/LessOS/userProc/target/object/main/stringc.o: /home/beyond/project/OStest/LessOS/userProc/src/main/stringc.c \
 /home/beyond/project/OStest/LessOS/userProc/include/main/string.h \
 /home/beyond/project/OStest/LessOS/userProc/include/main/type.h
	mkdir -p /home/beyond/project/OStest/LessOS/userProc/target/object/main/
	gcc -fno-builtin -m32 -fno-stack-protector -c -o /home/beyond/project/OStest/LessOS/userProc/target/object/main/stringc.o -I/home/beyond/project/OStest/LessOS/userProc/include/main/ /home/beyond/project/OStest/LessOS/userProc/src/main/stringc.c
