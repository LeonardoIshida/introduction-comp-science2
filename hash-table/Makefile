all: main.o hash.o hash.h
	gcc -o main main.o hash.o

main.o: main.c hash.h
	gcc -c main.c

hash.o: hash.c hash.h
	gcc -c hash.c

clean:
	rm *.o main
	clear

run:
	./main