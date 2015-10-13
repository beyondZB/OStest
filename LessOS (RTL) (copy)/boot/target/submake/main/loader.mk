/home/beyond/project/OStest/LessOS/boot/target/object/main/loader: /home/beyond/project/OStest/LessOS/boot/src/main/loader.asm \
  /home/beyond/project/OStest/LessOS/boot/include/main/pm.inc \
  /home/beyond/project/OStest/LessOS/boot/include/main/load.inc \
  /home/beyond/project/OStest/LessOS/boot/include/main/lofs.inc
	mkdir -p /home/beyond/project/OStest/LessOS/boot/target/object/main/
	nasm   -o /home/beyond/project/OStest/LessOS/boot/target/object/main/loader -I/home/beyond/project/OStest/LessOS/boot/include/main/ /home/beyond/project/OStest/LessOS/boot/src/main/loader.asm
