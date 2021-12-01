CFLAGS = -Wall -Werror

all : day1

day1 : day1.o
	gcc -o day1.out day1.o

day1.o : day1.c 
	gcc -c day1.c ${CFLAGS}

clean : 
	rm -rf day1 *.o