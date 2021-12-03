#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define LINE_LENGTH 80

struct record 
{
	int nb_record;
	int size;
	char **lines;
};


int bin_to_int(char* bin)
{
	return (int) strtol(bin, NULL, 2);
}

void free_record(struct record* record) {
	for (int i = 0; i < record->nb_record; i++)
	{
		free(record->lines[i]);
	}
	free(record->lines);
	free(record);
}

struct record *get_file_content(FILE* input) {
	struct record *record = malloc(sizeof(struct record));

	char line[LINE_LENGTH];
	char* str;


	fgets(line, LINE_LENGTH, input);
	str = strtok(line, "\n");
	record->size = strlen(str);
	record->nb_record = 0;
	record->lines = NULL;

	rewind(input);
	
	while (fgets(line, LINE_LENGTH, input))
	{
		record->nb_record++;
		record->lines = realloc(record->lines, record->nb_record*record->size*sizeof(record->lines));
		str = strtok(line, "\n");
		record->lines[record->nb_record-1] = malloc(record->size);
		strncpy(record->lines[record->nb_record-1], str, record->size);
	}

	return record;
}

int *get_sums(struct record *record) {
	int* sums = malloc(record->size*sizeof(int));
	
	for (int i = 0; i < record->size; i++) {
		sums[i] = 0;
	}

	for (int j = 0; j < record->nb_record; j++) {
		for (int i = 0; i < record->size; i++) {
			sums[i] += record->lines[j][i] - '0';
		}
	}
	
	return sums;
}

struct record* life_support_rating(struct record *record, int cur, int most_common)	
{
	printf("[%d] testing %d bit\n", record->nb_record, cur);
	if (record->nb_record == 1) return record;
	int* sums = get_sums(record);
	char bit_criteria;

	if (most_common) {
		if ((float) sums[cur]/record->nb_record < 0.5) {
			bit_criteria = '0';
		} else {
			bit_criteria = '1';
		}
	} else {
		if ((float) sums[cur]/record->nb_record >= 0.5) {
			bit_criteria = '0';
		} else {
			bit_criteria = '1';
		}
	}

	struct record *next_record = malloc(sizeof(struct record));
	next_record->nb_record = 0;
	next_record->size = record->size;
	next_record->lines = NULL;
	for (int i = 0; i < record->nb_record; i++) {
		if (record->lines[i][cur] == bit_criteria)
		{
			printf("[%c in %d] %s\n", bit_criteria, cur, record->lines[i]);
			next_record->nb_record++;
			next_record->lines = realloc(next_record->lines, next_record->nb_record*record->size*sizeof(next_record->lines));
			next_record->lines[next_record->nb_record-1] = malloc(record->size*sizeof(char));
			strncpy(next_record->lines[next_record->nb_record-1], record->lines[i], record->size);
			next_record->lines[next_record->nb_record-1][record->size] = '\0';
		}
		
	}
	if (cur > 0) {
		free_record(record);
	}
	cur++;
	if (cur == record->size) {
		printf("Error : too much records at last step");
		exit(1);
	}
	return life_support_rating(next_record, cur, most_common);
}

int main(int argc, char** argv) {
	FILE* input = fopen("input", "r");

	int* sums;
	

	struct record *record = get_file_content(input);
	sums = get_sums(record);

	char bin_gama_rate[record->size];
	char bin_epsilon_rate[record->size];

	for (int i = 0; i < record->size; i++)
	{
		printf("%d ", sums[i]);
		if ((float) sums[i]/record->nb_record > 0.5)
		{
			bin_gama_rate[i] = '1';
			bin_epsilon_rate[i] = '0';
		} else {
			bin_gama_rate[i] = '0';
			bin_epsilon_rate[i] = '1';
		}
		
	}
	
	printf("\n");

	int gama_rate = bin_to_int(bin_gama_rate);
	printf("gama rate : %d\n", gama_rate);
	int epsilon_rate = bin_to_int(bin_epsilon_rate);
	printf("epsilon rate : %d\n", epsilon_rate);

	printf("Answer : %d\n", epsilon_rate*gama_rate);

	struct record* oxygen_record;
	oxygen_record = life_support_rating(record, 0, 1);
	int oxygen_generator_rating = bin_to_int(oxygen_record->lines[0]);
	printf("Oxygen generator rating = %d (%s)\n", oxygen_generator_rating, oxygen_record->lines[0]);

	struct record* co2_record;
	co2_record = life_support_rating(record, 0, 0);
	int co2_scrubber_rating = bin_to_int(co2_record->lines[0]);
	printf("CO2 scrubber rating = %d (%s)\n", co2_scrubber_rating, co2_record->lines[0]);


	printf("Answer : %d\n", oxygen_generator_rating*co2_scrubber_rating);

	free(sums);
	free_record(oxygen_record);
	free_record(co2_record);
	free_record(record);
	fclose(input);
	return EXIT_SUCCESS;
}
