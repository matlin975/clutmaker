EXE=clutmaker
CC=gcc

CFLAGS=-std=gnu11 -g

LDFLAGS = -lm

all:	main

main:	main.o command_parser.o gnuplot_i.o
	$(CC) -o $(EXE) main.o command_parser.o gnuplot_i.o $(LDFLAGS)

main.o:	main.c main.h
	$(CC) $(CFLAGS) -c main.c

command_parser.o: command_parser.c command_parser.h
	$(CC) $(CFLAGS) -c command_parser.c

gnuplot_i.o: gnuplot_i.c gnuplot_i.h
	$(CC) $(CFLAGS) -c gnuplot_i.c

.PHONY:	clean
clean:
	clear
	rm clutmaker main.o command_parser.o gnuplot_i.o output.csv output.png

mem:
	valgrind --leak-check=full ./clutmaker x^2+x x=1:1:10
