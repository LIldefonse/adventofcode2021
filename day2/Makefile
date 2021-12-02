CFLAGS = -Wall -Werror

all : day2

day2 : day2.o
	gcc -o day2.out day2.o

day2.o : day2.c
	gcc -c day2.c ${CFLAGS}

clean :
	rm -rf *.out *.o

