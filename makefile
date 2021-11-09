all: hw13.o
	gcc -o hw13 .o

hw13.o: hw13.c hw13.h
	gcc -c hw13.c

run:
	./hw13