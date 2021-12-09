CFLAGS = -Wall -Werror

all : day9

day9 : day9.o
	gcc -o day9.out day9.o

day9.o : day9.c
	gcc -c day9.c ${CFLAGS}

clean :
	rm -rf *.out *.o

