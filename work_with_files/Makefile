.PHONY: all project rebuild run
all: project

project: cipher.o
	gcc -o ./build/file.exe cipher.o

cipher.o: cipher.c
	gcc -c cipher.c

clean: 
	rm project *.o

run: 
	./build/file.exe

rebuild: project clean
