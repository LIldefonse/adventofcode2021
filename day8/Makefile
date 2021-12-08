CFLAGS = -Wall -Werror -g

all : day8

day8 : day8.o
	gcc -o day8.out day8.o

day8.o : day8.c
	gcc -c day8.c ${CFLAGS}

clean :
	rm -rf *.out *.o

