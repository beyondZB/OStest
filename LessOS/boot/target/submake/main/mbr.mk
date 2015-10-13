/home/beyond/project/OStest/LessOS/boot/target/object/main/mbr: /home/beyond/project/OStest/LessOS/boot/src/main/mbr.asm \
  /home/beyond/project/OStest/LessOS/boot/include/main/load.inc \
  /home/beyond/project/OStest/LessOS/boot/include/main/lofs.inc
	mkdir -p /home/beyond/project/OStest/LessOS/boot/target/object/main/
	nasm   -o /home/beyond/project/OStest/LessOS/boot/target/object/main/mbr -I/home/beyond/project/OStest/LessOS/boot/include/main/ /home/beyond/project/OStest/LessOS/boot/src/main/mbr.asm
