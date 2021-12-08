CFLAGS = -Wall -Werror

all : day7

day7 : day7.o
	gcc -o day7.out day7.o

day7.o : day7.c
	gcc -c day7.c ${CFLAGS}

clean :
	rm -rf *.out *.o

