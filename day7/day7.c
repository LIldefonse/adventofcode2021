#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 10000

struct crabs {
	int n_crabs;
	int *horizontal_positions;
	int max_pos;
};

struct crabs *init_crabs(int n_crabs) {
	struct crabs *crabs = malloc(sizeof(struct crabs));
	crabs->n_crabs = n_crabs;
	crabs->horizontal_positions = malloc(n_crabs*sizeof(int));
	crabs->max_pos = 0;
	return crabs;
}

struct crabs *get_crabs_from_context(FILE* input) {
	int n = 0;

	char line[LINE_LENGTH];
	char *str;
	fgets(line, LINE_LENGTH, input);
	str = strtok(line, ",");
	while (str)
	{
		n++;
		str = strtok(NULL, ",");
	}

	struct crabs *crabs = init_crabs(n);

	rewind(input);

	fgets(line, LINE_LENGTH, input);
	str = strtok(line, ",");
	for(int i = 0; i < n ; i++) 
	{
		crabs->horizontal_positions[i] = atoi(str);
		if (atoi(str) > crabs->max_pos)
		{
			crabs->max_pos = atoi(str);
		}
		
		str = strtok(NULL, ",");
	}
	
	return crabs;
}

int fuel_spend_in_steps(int initial, int final) {
	int diff = abs(initial - final);
	int fuel = 0;
	for (int i = 0; i <= diff; i++)
	{
		fuel += i;
	}
	return fuel;
}

int fuel_spent_to_align(int pos, struct crabs *crabs) {
	int fuel_spent = 0;
	for (int i = 0; i < crabs->n_crabs; i++)
	{
		fuel_spent += fuel_spend_in_steps(crabs->horizontal_positions[i], pos);
	}

	return fuel_spent;
	
}

int min_fuel_to_align(struct crabs *crabs) {
	int min_fuel = 0;
	int fuel;
	for (int i = 0; i <= crabs->max_pos; i++)
	{
		fuel = fuel_spent_to_align(i, crabs);
		if (!min_fuel || fuel < min_fuel)
		{
			min_fuel = fuel;
			printf("[%d] %d fuel needed\n", i, fuel);
		}
		
	}
			
	return min_fuel;

}

void show_crabs(struct crabs *crabs) {
	for (int i = 0; i < crabs->n_crabs; i++)
	{
		printf("[%d] %d\n", i+1, crabs->horizontal_positions[i]);
	}
	
	printf("%d crabs\n", crabs->n_crabs);
	printf("maximal position : %d \n", crabs->max_pos);
}

void free_crabs(struct crabs *crabs) {
	free(crabs->horizontal_positions);
	free(crabs);
}

int main(int argc, char** argv) {
	FILE* input = fopen("input", "r");

	struct crabs *crabs = get_crabs_from_context(input);
	//show_crabs(crabs);
	int fuel;
	
	fuel = min_fuel_to_align(crabs);
	printf("min fuel to align : %d\n", fuel);
	
	free_crabs(crabs);
	fclose(input);
	return EXIT_SUCCESS;
}
