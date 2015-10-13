All: Lsh
Lsh : Lsh.o read_command.o mystring.o map.o
	gcc -o Lsh Lsh.o read_command.o mystring.o map.o
Lsh.o: Lsh.c read_command.h
	gcc -c Lsh.c
read_command.o: read_command.c read_command.h mystring.h map.h
	gcc -c read_command.c
mystring.o: mystring.c mystring.h read_command.h
	gcc -c mystring.c 
map.o: map.h mystring.h
	gcc -c map.c
clean:
	-rm read_command.o mystring.o Lsh.o map.o	
