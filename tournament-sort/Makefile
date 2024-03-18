all: main.o torneio.o
	gcc -o main main.o torneio.o

main.o: main.c torneio.h
	gcc -c main.c

torneio.o: torneio.c torneio.h
	gcc -c torneio.c

run:
	./main

clean:
	rm *.o main 
	clear
