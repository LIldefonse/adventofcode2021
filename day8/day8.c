#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 150
#define VALUE_SIZE 4
#define SIZE 10
#define N_SEGS 7

int digits[10] = {6, 2, 6, 5, 4, 5, 6, 3, 7, 6};

struct digit
{
	char code[N_SEGS];
	int value;
};


struct pattern {
	struct digit *codes[SIZE];
	struct digit *code_value[VALUE_SIZE];
};

struct pattern *init_pattern() {
	struct pattern *pattern = malloc(sizeof(struct pattern));
	for (int i = 0; i < SIZE; i++)
	{
		pattern->codes[i] = malloc(sizeof(struct digit));
	}
	for (int i = 0; i < VALUE_SIZE; i++)
	{
		pattern->code_value[i] = malloc(sizeof(struct digit));
	}
	return pattern;
}

void free_pattern(struct pattern *pattern) {
	for (int i = 0; i < SIZE; i++)
	{
		free(pattern->codes[i]);
	}
	for (int i = 0; i < VALUE_SIZE; i++)
	{
		free(pattern->code_value[i]);
	}
	free(pattern);
}

void alphabetic_order(char string[]) {
	int len = strlen(string);
	char temp;
	
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len-1; j++)
		{
			if (string[j] > string[j+1])
			{
				temp = string[j];
				string[j] = string[j+1];
				string[j+1] = temp;
			}
		}
	}
}

struct pattern *get_pattern_from_line(char* line) {
	struct pattern *pattern = init_pattern();
	char *str_code;
	char *str_value;
	str_code = strtok(line, "|");
	str_value = strtok(NULL, "|");

	char* digit;
	digit = strtok(str_code, " ");

	for (int i = 0; i < SIZE; i++)
	{
		strcpy(pattern->codes[i]->code, digit);
		alphabetic_order(pattern->codes[i]->code);
		pattern->codes[i]->value = -1;
		digit = strtok(NULL, " ");
	}

	digit = strtok(str_value, " ");
	
	for (int i = 0; i < VALUE_SIZE; i++)
	{
		strcpy(pattern->code_value[i]->code, digit);
		alphabetic_order(pattern->code_value[i]->code);
		pattern->code_value[i]->value = -1;
		digit = strtok(NULL, " ");
	}

	return pattern;
}

void show_pattern(struct pattern *pattern) {
	for (int i = 0; i < SIZE; i++)
	{
		printf("%s ", pattern->codes[i]->code);
	}
	printf("| ");
	for (int i = 0; i < VALUE_SIZE; i++)
	{
		printf("%s ", pattern->code_value[i]->code);
	}
	printf("\n");
	
}

int char_in_string(char car, char* string) {
	for (int i = 0; i < strlen(string); i++)
	{
		if (car == string[i])
		{
			return 1;
		}
		
	}
	return 0;
}

int pow_10(int n) {
	if (n == 0)
	{
		return 1;
	} else {
		return 10*pow_10(n-1);
	}
	
}

int get_value(struct digit **code_value, struct digit **decode) {
	int value = 0;
	for (int i = 0; i < VALUE_SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (! strcmp(code_value[i]->code, decode[j]->code))
			{
				if (decode[j]->value < 0)
				{
					fprintf(stderr, "Error : Value < 0\n");
					exit(EXIT_FAILURE);
				}
				
				code_value[i]->value = decode[j]->value;
				value += decode[j]->value*pow_10(VALUE_SIZE-1-i);
			}
		}
		
	}
	return value;
}

int decode_complete(struct pattern *pattern) {
	for (int i = 0; i < SIZE; i++)
	{
		if(pattern->codes[i]->value < 0) {
			//printf("Reste à décoder : %s\n", pattern->codes[i]->code);
			return 0;
		}
	}

	return 1;
}

int decode(struct pattern *pattern) {

	char *in_order_values[SIZE];

	for (int i = 0; i < SIZE; i++)
	{
		in_order_values[i] = NULL;
	}
	
	
	int value;
	int len_digit;

	//show_pattern(pattern);
	int break_var = 0;
	while (!decode_complete(pattern) )
	{
		break_var++;
		if (break_var == 10)
		{
			break;
		}
		
		for (int i = 0; i < SIZE; i++)
		{
			len_digit = strlen(pattern->codes[i]->code);
			//printf("%d - %s\n", len_digit, pattern->codes[i]->code);
			int test;
			if (pattern->codes[i]->value < 0)
			{
				switch (len_digit)
				{
				case 2 :
					//printf("%s is 1\n", pattern->codes[i]->code);
					pattern->codes[i]->value = 1;
					in_order_values[1] = malloc(N_SEGS);
					strcpy(in_order_values[1], pattern->codes[i]->code);
					break;
				case 4 :
					//printf("%s is 4\n", pattern->codes[i]->code);
					pattern->codes[i]->value = 4;
					in_order_values[4] = malloc(N_SEGS);
					strcpy(in_order_values[4], pattern->codes[i]->code);
					break;
				case 3 :
					//printf("%s is 7\n", pattern->codes[i]->code);
					pattern->codes[i]->value = 7;
					in_order_values[7] = malloc(N_SEGS);
					strcpy(in_order_values[7], pattern->codes[i]->code);
					break;
				case 7 :
					//printf("%s is 8\n", pattern->codes[i]->code);
					pattern->codes[i]->value = 8;
					in_order_values[8] = malloc(N_SEGS);
					strcpy(in_order_values[8], pattern->codes[i]->code);
					break;
				case 5 :
					if (in_order_values[1])
					{
						test = 1;
						for (int j = 0; j < strlen(in_order_values[1]); j++)
						{
							if (! char_in_string(in_order_values[1][j], pattern->codes[i]->code))
							{
								//printf("%s is not in symbol %s\n", in_order_values[1], pattern->codes[i]->code);
								test = 0;
							}
						}

						if (test)
						{
							//printf("%s is 3\n", pattern->codes[i]->code);
							pattern->codes[i]->value = 3;
							in_order_values[3] = malloc(N_SEGS);
							strcpy(in_order_values[3], pattern->codes[i]->code);
							break;
						}
					}
				
					if (in_order_values[9] && in_order_values[3])
					{
						test = 1;
						for (int j = 0; j < len_digit; j++)
						{
							if (! char_in_string(pattern->codes[i]->code[j], in_order_values[9]))
							{
								//printf("%s is not in symbol %s\n", in_order_values[1], pattern->codes[i]->code);
								test = 0;
							}
						}

						if (test)
						{
							//printf("%s is 5\n", pattern->codes[i]->code);
							pattern->codes[i]->value = 5;
							in_order_values[5] = malloc(N_SEGS);
							strcpy(in_order_values[5], pattern->codes[i]->code);
							break;
						}
					}

					if (in_order_values[3] && in_order_values[5])
					{
						//printf("%s is 2\n", pattern->codes[i]->code);
						pattern->codes[i]->value = 2;
						in_order_values[2] = malloc(N_SEGS);
						strcpy(in_order_values[2], pattern->codes[i]->code);
						break;
					}
					
					break;
				case 6 :
					if (in_order_values[4])
					{
						test = 1;
						for (int j = 0; j < strlen(in_order_values[4]); j++)
						{
							if (! char_in_string(in_order_values[4][j], pattern->codes[i]->code))
							{
								//printf("%s is not in symbol %s\n", in_order_values[4], pattern->codes[i]->code);
								test = 0;
							}
						}

						if (test)
						{
							//printf("%s is 9\n", pattern->codes[i]->code);
							pattern->codes[i]->value = 9;
							in_order_values[9] = malloc(N_SEGS);
							strcpy(in_order_values[9], pattern->codes[i]->code);
							break;
						}
					}
				
					if (in_order_values[5])
					{
						test = 1;
						for (int j = 0; j < strlen(in_order_values[5]); j++)
						{
							if (! char_in_string(in_order_values[5][j], pattern->codes[i]->code))
							{
								//printf("%s is not in symbol %s\n", in_order_values[4], pattern->codes[i]->code);
								test = 0;
							}
						}

						if (test)
						{
							//printf("%s is 6\n", pattern->codes[i]->code);
							pattern->codes[i]->value = 6;
							in_order_values[6] = malloc(N_SEGS);
							strcpy(in_order_values[6], pattern->codes[i]->code);
							break;
						}
					}
				
					if (in_order_values[6] && in_order_values[9])
					{
						//printf("%s is 0\n", pattern->codes[i]->code);
						pattern->codes[i]->value = 0;
						in_order_values[0] = malloc(N_SEGS);
						strcpy(in_order_values[0], pattern->codes[i]->code);
						break;
					}
					
					break;

				default:
					break;
				}
			}
		}
	}

	value = get_value(pattern->code_value, pattern->codes);

	for (int i = 0; i < SIZE; i++)
	{
		free(in_order_values[i]);
	}
	
	printf("%d\n", value);
	return value;

}

int main(int argc, char** argv) {
	FILE *input = fopen("input", "r");
	char line[LINE_LENGTH];
	int number = 0;
	struct pattern *pattern;


	while(fgets(line, LINE_LENGTH, input)) {
		//number += decode(strtok(line, "\n"));
		pattern = get_pattern_from_line(strtok(line, "\n"));
		number += decode(pattern);
		//show_pattern(pattern);
		free_pattern(pattern);
	}

	printf("%d\n", number);

	fclose(input);
	return EXIT_SUCCESS;
}

