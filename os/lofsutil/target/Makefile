all: target/lofsutil-0.0.1
	
clean:
	rm -r target/object/main/mbr.o target/object/main/lofs.o target/object/main/task.o target/object/main/main.o target/object/main/lib/device.o target/object/main/lib/list.o target/object/main/lib/platform/std/device.o target/object/main/lib/platform/win32/device.o

target/lofsutil-0.0.1: target/object/main/mbr.o target/object/main/lofs.o target/object/main/task.o target/object/main/main.o target/object/main/lib/device.o target/object/main/lib/list.o target/object/main/lib/platform/std/device.o target/object/main/lib/platform/win32/device.o
	gcc -o target/lofsutil-0.0.1 target/object/main/mbr.o target/object/main/lofs.o target/object/main/task.o target/object/main/main.o target/object/main/lib/device.o target/object/main/lib/list.o target/object/main/lib/platform/std/device.o target/object/main/lib/platform/win32/device.o

target/object/main/mbr.o: src/main/mbr.c include/main/mbr.h include/main/type.h \
 include/main/lib/device.h include/main/type.h \
 include/main/lib/platform/std/device.h
	mkdir -p target/object/main/
	gcc -c -o "target/object/main/mbr.o" -Iinclude/main "src/main/mbr.c"

target/object/main/lofs.o: src/main/lofs.c include/main/lofs.h include/main/type.h \
 include/main/lib/device.h include/main/type.h \
 include/main/lib/platform/std/device.h
	mkdir -p target/object/main/
	gcc -c -o "target/object/main/lofs.o" -Iinclude/main "src/main/lofs.c"

target/object/main/task.o: src/main/task.c include/main/task.h include/main/type.h \
 include/main/lib/device.h include/main/type.h \
 include/main/lib/platform/std/device.h include/main/lofs.h
	mkdir -p target/object/main/
	gcc -c -o "target/object/main/task.o" -Iinclude/main "src/main/task.c"

target/object/main/main.o: src/main/main.c include/main/lib/device.h include/main/type.h \
 include/main/lib/platform/std/device.h include/main/task.h \
 include/main/type.h include/main/lib/device.h
	mkdir -p target/object/main/
	gcc -c -o "target/object/main/main.o" -Iinclude/main "src/main/main.c"

target/object/main/lib/device.o: src/main/lib/device.c include/main/lib/device.h \
 include/main/type.h include/main/lib/platform/std/device.h
	mkdir -p target/object/main/lib
	gcc -c -o "target/object/main/lib/device.o" -Iinclude/main "src/main/lib/device.c"

target/object/main/lib/list.o: src/main/lib/list.c include/main/lib/list.h include/main/type.h
	mkdir -p target/object/main/lib
	gcc -c -o "target/object/main/lib/list.o" -Iinclude/main "src/main/lib/list.c"

target/object/main/lib/platform/std/device.o: src/main/lib/platform/std/device.c \
 include/main/lib/platform/std/device.h include/main/type.h \
 include/main/lib/list.h
	mkdir -p target/object/main/lib/platform/std
	gcc -c -o "target/object/main/lib/platform/std/device.o" -Iinclude/main "src/main/lib/platform/std/device.c"

target/object/main/lib/platform/win32/device.o: src/main/lib/platform/win32/device.c
	mkdir -p target/object/main/lib/platform/win32
	gcc -c -o "target/object/main/lib/platform/win32/device.o" -Iinclude/main "src/main/lib/platform/win32/device.c"

