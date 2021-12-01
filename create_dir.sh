#! /bin/bash

if [ "$#" -ne 1 ]; then
	echo "usage : $0 day_number"
	exit 1
fi

echo "Run in adventofcode2021"

root=$(pwd)

dir="$root/day$1"
short_dir="day$1"
makefile="$dir/Makefile"
src="$dir/day$1.c"

if [ -d $dir ]; then
	echo "Repository for $short_dir already exists"
else
	echo "Creating repository for $short_dir"
	mkdir $dir
fi

if [ -f "$makefile" ]; then
	echo "Makefile for $short_dir already exists"
else
	echo "Creating Makefile for $short_dir"
	touch $makefile

	echo "Filling Makefile"

	echo "CFLAGS = -Wall -Werror" >> $makefile
	echo >> $makefile

	echo "all : $short_dir" >> $makefile
	echo >> $makefile

	echo "$short_dir : $short_dir.o" >> $makefile
	echo "	gcc -o $short_dir.out $short_dir.o" >> $makefile
	echo >> $makefile

	echo "$short_dir.o : $short_dir.c" >> $makefile
	echo "	gcc -c $short_dir.c" '${CFLAGS}' >> $makefile
	echo >> $makefile

	echo "clean :">> $makefile
	echo "	rm -rf *.out *.o" >> $makefile
	echo >> $makefile
fi

if [ -f $src ]; then
	echo "Source file for $short_dir already exists"
else 
	echo "Creating source file for $short_dir"
	touch $src

	echo "Filling source file"

	echo '#include <stdlib.h>' >> $src
	echo '#include <stdio.h>' >> $src
	echo >> $src

	echo 'int main(int argc, char** argv) {' >> $src
	echo '	return EXIT_SUCCESS;' >> $src
	echo '}' >> $src
	echo >> $src

fi

exit 0
