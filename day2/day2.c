#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LINE_LENGTH 80


struct instruction
{
	char* instruction;
	int value;
};

struct instruction* get_instruction_from_string(char* string) {
	struct instruction* instruct = malloc(sizeof(struct instruction));
	instruct->instruction = strtok(string, " ");
	instruct->value = atoi(strtok(NULL, " "));
	return instruct;
	
}

void show_instruction(struct instruction* instruct) {
	printf("%s %d\n", instruct->instruction, instruct->value);
}


int main(int argc, char** argv) {
	int depth = 0;
	int pos = 0;
	int aim = 0;

	FILE* input = fopen("input", "r");

	char line[LINE_LENGTH];
	struct instruction* instruct;

	while (fgets(line, LINE_LENGTH, input))
	{
		instruct = get_instruction_from_string(line);
		show_instruction(instruct);
		
		if ( ! strcmp(instruct->instruction, "forward"))
		{
			pos += instruct->value;
			depth += aim*instruct->value;
		} else if ( ! strcmp(instruct->instruction, "down")) {
			aim += instruct->value;
		} else if ( ! strcmp(instruct->instruction, "up")) {
			aim -= instruct->value;
		} else {
			printf("Error while reading instruction : %s\n", instruct->instruction);
			exit(EXIT_FAILURE);
		}
		
	}
	

	fclose(input);
	free(instruct);

	printf("Horizontal position : %d\tDepth : %d\nGlobal position : %d\n", pos, depth, pos*depth);
	return EXIT_SUCCESS;
}

