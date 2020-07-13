EXE=clutmaker
CC=gcc

CFLAGS=-std=c99 -g

LDFLAGS = -lm

all:	main

main:	main.o queue.o list.o gnuplot_i.o
	$(CC) -o $(EXE) main.o queue.o list.o gnuplot_i.o $(LDFLAGS)

main.o:	main.c main.h
	$(CC) $(CFLAGS) -c main.c

queue.o:	queue.c queue.h util.h
	$(CC) $(CFLAGS) -c queue.c

list.o:	list.c list.h util.h
	$(CC) $(CFLAGS) -c list.c

.PHONY:	clean
clean:
	clear
	rm clutmaker main.o

mem:
	valgrind --leak-check=full ./clutmaker
