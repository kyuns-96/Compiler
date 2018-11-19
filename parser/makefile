parser: parser.o scanner.o MiniC.o
	gcc -c -o parser parser.o scanner.o MiniC.o

parser.o: scanner.h scanner.o parser.h parser.c
	gcc -c -o parser.o parser.c

scanner.o: scanner.h scanner.c
	gcc -c -o scanner.o scanner.c

MiniC.o: MiniC.tbl
	gcc -c -o MiniC.tbl
