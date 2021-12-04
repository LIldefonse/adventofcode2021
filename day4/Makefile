CFLAGS = -Wall -Werror -g

all : day4

day4 : day4.o
	gcc -o day4.out day4.o

day4.o : day4.c
	gcc -c day4.c ${CFLAGS}

clean :
	rm -rf *.out *.o

