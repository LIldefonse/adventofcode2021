CFLAGS = -Wall -Werror -g

all : day3

day3 : day3.o
	gcc -o day3.out day3.o

day3.o : day3.c
	gcc -c day3.c ${CFLAGS}

clean :
	rm -rf *.out *.o

