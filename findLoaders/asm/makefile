all: mbr findLoader LOADER.BIN

mbr: mbr.asm 
	nasm -o mbr mbr.asm

findLoader: findLoader.asm
	nasm -o findLoader findLoader.asm

 LOADER.BIN: loader.asm
	nasm -o LOADER.BIN loader.asm