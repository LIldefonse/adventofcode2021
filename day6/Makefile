CFLAGS = -Wall -Werror

all : day6

day6 : day6.o
	gcc -o day6.out day6.o

day6.o : day6.c
	gcc -c day6.c ${CFLAGS}

clean :
	rm -rf *.out *.o

