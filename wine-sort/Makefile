all: main.o vinho.o
	gcc -o main main.o vinho.o

main.o: main.c vinho.h
	gcc -c main.c

vinho.o: vinho.c vinho.h
	gcc -c vinho.c

run:
	./main

clean:
	rm *.o main
