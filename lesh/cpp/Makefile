all:Lsh
Lsh: Lsh.o read_command.o
	g++ -o Lsh Lsh.o read_command.o
Lsh.o: Lsh.cpp read_command.h
	g++ -c Lsh.cpp
read_command.o:read_command.cpp read_command.h
	g++ -c read_command.cpp
clean:
	-rm read_command.o Lsh.o
