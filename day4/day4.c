#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 5
#define LINE_LENGTH 300

struct bingo_case {
	int value;
	int marked;
};

struct bingo {
	struct bingo_case *cases[SIZE*SIZE];
};

struct file_content {
	int* numbers_to_draw;
	struct bingo **bingo;
	int number_of_numbers_to_draw;
	int number_of_bingos;
};

void show_bingo_grid(struct bingo *bingo) {
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			printf("%d ", bingo->cases[i*SIZE+j]->value);
		}
		printf("\n");
	}
	
}

void show_file_content(struct file_content *file_content) {
	printf("%d numbers to draw\n", file_content->number_of_numbers_to_draw);
	printf("%d", file_content->numbers_to_draw[0]);
	for(int i = 1; i < file_content->number_of_numbers_to_draw; i++) {
		printf(",%d", file_content->numbers_to_draw[i]);
	}

	printf("\n");
	printf("\n");

	printf("%d grids of bingo\n", file_content->number_of_bingos);
	printf("\n");
	for (int i = 0; i < file_content->number_of_bingos; i++)
	{
		show_bingo_grid(file_content->bingo[i]);
		printf("\n");
	}
}

struct file_content *init_file_content(FILE *input) {
	struct file_content *content = malloc(sizeof(struct file_content));
	content->number_of_numbers_to_draw = 0;
	content->number_of_bingos = 0;

	char line[LINE_LENGTH];
	char* num;
	
	fgets(line, LINE_LENGTH, input);

	num = strtok(line, ",");
	while(num) {
		content->number_of_numbers_to_draw++;
		num = strtok(NULL, ",");
	}

	while (fgets(line, LINE_LENGTH, input))
	{
		if (strcmp(line, "\n"))
		{
		} else {
			content->number_of_bingos++;
		}
	}
	content->numbers_to_draw = malloc(content->number_of_numbers_to_draw*sizeof(int));
	content->bingo = malloc(content->number_of_bingos*sizeof(struct bingo));

	for (int i = 0; i < content->number_of_bingos; i++)
	{
		content->bingo[i] = malloc(sizeof(struct bingo));
		for (int j = 0; j < SIZE*SIZE; j++)
		{
			content->bingo[i]->cases[j] = malloc(sizeof(struct bingo_case));
		}
		
	}

	return content;
}

void fill_file_content(struct file_content* content, FILE* input){
	char line[LINE_LENGTH];
	char* num;
	int cur=0;
	
	fgets(line, LINE_LENGTH, input);
	num = strtok(line, ",");
	while(num) {
		content->numbers_to_draw[cur] = atoi(num);
		num = strtok(NULL, ",");
		cur++;
	}

	int cur_bingo = -1;
	cur=0;

	while (fgets(line, LINE_LENGTH, input))
	{
		if (strcmp(line, "\n"))
		{
				
			num = strtok(line, " ");
			while(num) {
				content->bingo[cur_bingo]->cases[cur]->marked = 0;
				content->bingo[cur_bingo]->cases[cur]->value = atoi(num);
				num = strtok(NULL, " ");
				cur ++;
			}
		} else {
			cur_bingo++;
			cur=0;
		}
	}
}

void free_bingo(struct bingo *bingo) {
	for (int i = 0; i < SIZE*SIZE; i++)
	{
		free(bingo->cases[i]);
	}
	free(bingo);
}

void free_file_content(struct file_content *content) {
	free(content->numbers_to_draw);
	for (int i = 0; i < content->number_of_bingos; i++)
	{
		free_bingo(content->bingo[i]);
	}
	free(content->bingo);
	free(content);
}

void mark_case_in_grid(struct bingo *grid, int value) {
	for (int i = 0; i < SIZE*SIZE; i++)
	{
		if (grid->cases[i]->value == value)
		{
			grid->cases[i]->marked = 1;
		}
	}
}

int winner(struct bingo *grid) {
	for (int i = 0; i < SIZE; i++)
	{
		int win = 1;
		for (int j = 0; j < SIZE; j++)
		{
			win*=grid->cases[i*SIZE+j]->marked;
		}
		if (win)
		{
			return win;
		}
	}

	for (int j = 0; j < SIZE; j++)
	{
		int win = 1;
		for (int i = 0; i < SIZE; i++)
		{
			win*=grid->cases[i*SIZE+j]->marked;
		}
		
		if (win)
		{
			return win;
		}
	}
	
	return 0;
}

int calculate_score(struct bingo *grid) {
	int score = 0;
	for (int i = 0; i < SIZE*SIZE; i++)
	{
		if (! grid->cases[i]->marked)
		{
			score+=grid->cases[i]->value;
		}
		
	}
	return score;
	
}

int main(int argc, char** argv) {
	struct file_content *content;
	FILE* input = fopen("input", "r");
	content = init_file_content(input);

	rewind(input);
	fill_file_content(content, input);

	int winners[content->number_of_bingos];
	for (int i = 0; i < content->number_of_bingos; i++)
	{
		winners[i] = 0;
	}
	

	for(int i = 0; i < content->number_of_numbers_to_draw; i++) {
		for (int j = 0; j < content->number_of_bingos; j++)
		{
			if (!winners[j])
			{
				mark_case_in_grid(content->bingo[j], content->numbers_to_draw[i]);
				if (winner(content->bingo[j]))
				{
					winners[j] = 1;
					printf("grid %d wins at %d number (%d) !\n", j+1, i+1, content->numbers_to_draw[i]);
					int score = calculate_score(content->bingo[j]);
					printf("Score : %d\n", score);
					printf("%d*%d = %d\n", content->numbers_to_draw[i], score, score*content->numbers_to_draw[i]);
				}
			}
		}
		
	}

	free_file_content(content);
	fclose(input);
	return EXIT_SUCCESS;
}

