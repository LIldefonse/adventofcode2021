CFLAGS = -Wall -Werror -g

all : day5

day5 : day5.o
	gcc -o day5.out day5.o

day5.o : day5.c
	gcc -c day5.c ${CFLAGS}

clean :
	rm -rf *.out *.o

