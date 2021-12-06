#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define AGE_MAX 8
#define LINE_LENGTH 800

struct lanternfish
{
	unsigned long number_of_fish_of_age[AGE_MAX+1];
};

struct lanternfish *init_lanternfish() {
	struct lanternfish *lanternfish = malloc(sizeof(struct lanternfish));
	for (int i = 0; i < AGE_MAX; i++)
	{
		lanternfish->number_of_fish_of_age[i] = 0;
	}
	
	return lanternfish;
}

struct lanternfish *get_lanternfish_from_input(FILE* input) {
	struct lanternfish *lanternfish = init_lanternfish();
	char line[LINE_LENGTH];
	//int cur = 0;
	fgets(line, LINE_LENGTH, input);
	char* str;
	str = strtok(line, ",");
	while (str)
	{
		//printf("[%d] %d\n", ++cur, atoi(str));
		lanternfish->number_of_fish_of_age[atoi(str)]++;
		str = strtok(NULL, ",");
	}

	return lanternfish;
	
}

void show_lanterfish(struct lanternfish* lanternfish) {
	for (int i = 0; i <= AGE_MAX; i++)
	{
		printf("[Age %d] %lu\n", i, lanternfish->number_of_fish_of_age[i]);
	}
}

unsigned long number_of_lanterfishs(struct lanternfish *lanternfish) {
	unsigned long sum = 0;
	for (int i = 0; i <= AGE_MAX; i++)
	{
		sum += lanternfish->number_of_fish_of_age[i];
	}
	return sum;
}

void compute_day(struct lanternfish *lanternfish) {
	unsigned long num_of_zeros = lanternfish->number_of_fish_of_age[0];
	for (int i = 0; i < AGE_MAX ; i++)
	{
		lanternfish->number_of_fish_of_age[i] = lanternfish->number_of_fish_of_age[i+1];
	}
	lanternfish->number_of_fish_of_age[AGE_MAX] = num_of_zeros;
	lanternfish->number_of_fish_of_age[6] += num_of_zeros;
}

int main(int argc, char** argv) {
	FILE* input = fopen("input", "r");

	struct lanternfish *lanternfish = get_lanternfish_from_input(input);

	unsigned long number = number_of_lanterfishs(lanternfish);

	show_lanterfish(lanternfish);
	printf("%lu lanternfishs !\n\n", number);

	for (int i = 0; i < 256; i++)
	{
		compute_day(lanternfish);
		printf("After %d days : \n", i+1);
		show_lanterfish(lanternfish);
		number = number_of_lanterfishs(lanternfish);
		printf("%lu lanternfishs !\n\n", number);
	}
	

	free(lanternfish);
	fclose(input);
	return EXIT_SUCCESS;
}