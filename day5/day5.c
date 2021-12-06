#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 80

struct coord
{
	int x;
	int y;
};

struct line
{
	struct coord *start;
	struct coord *end;
};


struct content
{
	int n_lines;
	struct line **lines;
};

struct grid
{
	int max_x;
	int max_y;
	int *pts;
};


struct content *get_content(FILE* input) {
	struct content *content = malloc(sizeof(content));
	content->n_lines = 0;
	char line[LINE_LENGTH];
	while (fgets(line, LINE_LENGTH, input))
	{
		content->n_lines++;
	}

	rewind(input);

	content->lines = malloc(content->n_lines*sizeof(struct line*));
	for (int i = 0; i < content->n_lines; i++)
	{
		fgets(line, LINE_LENGTH, input);
		char* start = strtok(line, "->");
		char* end = strtok(NULL, "->");

		content->lines[i] = malloc(sizeof(struct line));
		content->lines[i]->start = malloc(sizeof(struct coord));
		content->lines[i]->end = malloc(sizeof(struct coord));

		char* start_x = strtok(start, ",");
		char* start_y = strtok(NULL, ",");
		content->lines[i]->start->x = atoi(start_x);
		content->lines[i]->start->y = atoi(start_y);

		char* end_x = strtok(end, ",");
		char* end_y = strtok(NULL, ",");
		content->lines[i]->end->x = atoi(end_x);
		content->lines[i]->end->y = atoi(end_y);
	}
	
	return content;
}

void show_line(struct line *line) {
	printf("%d,%d -> %d,%d\n", line->start->x, line->start->y, line->end->x, line->end->y);
}

void show_content(struct content *content) {
	printf("%d lines\n", content->n_lines);
	for (int i = 0; i < content->n_lines; i++)
	{
		printf("[%d] ", i);
		show_line(content->lines[i]);
	}
}

struct grid *init_grid(int x, int y) {
	struct grid *grid = malloc(sizeof(grid));
	grid->pts = malloc(x*y*sizeof(int));
	grid->max_x = x;
	grid->max_y = y;
	for (int i = 0; i < x*y; i++)
	{
		grid->pts[i] = 0;
	}
	
	return grid;
}

void show_grid(struct grid *grid) {
	for (int i = 0; i < grid->max_y; i++)
	{
		for (int j = 0; j < grid->max_x; j++)
		{
			if (grid->pts[i*grid->max_y + j])
			{
				printf("%d", grid->pts[i*grid->max_y + j]);
			}
			else
			{
				printf(".");
			}
		}
		printf("\n");
	}
	
}

void vertical_line(struct grid *grid, int x, int y1, int y2) {
	int min_y, max_y;
	if (y1 < y2)
	{
		min_y = y1;
		max_y = y2;
	} else {
		min_y = y2;
		max_y = y1;
	}

	for (int i = min_y; i <= max_y; i++)
	{
		grid->pts[i*grid->max_x+x] ++;
	}
	
}

void horizontal_line(struct grid *grid, int x1, int x2, int y) {
	int min_x, max_x;
	if (x1 < x2)
	{
		min_x = x1;
		max_x = x2;
	} else {
		min_x = x2;
		max_x = x1;
	}

	for (int i = min_x; i <= max_x; i++)
	{
		if (y*grid->max_x + i >= grid->max_x*grid->max_y)
		{
			printf("/!\\ %d > %d !\n", y*grid->max_x + i, grid->max_x*grid->max_y);
			exit(EXIT_FAILURE);
		}

		grid->pts[y*grid->max_x + i] ++;
	}
	
	
}

void diagonal_line(struct grid *grid, int x1, int x2, int y1, int y2) {
	int desc;
	int max_x, min_x, max_y, min_y;
	if (x1 < x2)
	{
		min_x = x1;
		max_x = x2;
		if (y1 < y2)
		{
			min_y = y1;
			max_y = y2;
			desc = 1;
		} else {
			min_y = y2;
			max_y = y1;
			desc = 0;
		}
	} else {
		min_x = x2;
		max_x = x1;
		if (y1 < y2)
		{
			min_y = y1;
			max_y = y2;
			desc = 0;
		} else {
			min_y = y2;
			max_y = y1;
			desc = 1;
		}
	}
	
	if (desc) {
		for (int i = min_y*grid->max_x + min_x; i <= max_y*grid->max_x+max_x; i+=(grid->max_x+1))
		{
			grid->pts[i]++;
		}
	} else {
		for (int i = min_y*grid->max_x + max_x; i <= max_y*grid->max_x+min_x; i+=(grid->max_x-1))
		{
			grid->pts[i]++;
		}
	}
}

struct grid *get_grid_from_content(struct content *content) {
	int x,y;
	x = 0;
	y = 0;

	for (int i = 0; i < content->n_lines; i++)
	{
		if (content->lines[i]->end->x > x)
		{
			x = content->lines[i]->end->x;
		}
		if (content->lines[i]->end->y > y)
		{
			y = content->lines[i]->end->y;
		}
		
		if (content->lines[i]->start->x > x)
		{
			x = content->lines[i]->start->x;
		}
		if (content->lines[i]->start->y > y)
		{
			y = content->lines[i]->start->y;
		}
	}
	
	struct grid *grid = init_grid(x+1, y+1);

	for (int i = 0; i < content->n_lines; i++)
	{
		if (content->lines[i]->start->x == content->lines[i]->end->x)
		{
			vertical_line(grid, content->lines[i]->start->x, content->lines[i]->start->y, content->lines[i]->end->y);
		}
		else if (content->lines[i]->start->y == content->lines[i]->end->y)
		{
			horizontal_line(grid, content->lines[i]->start->x, content->lines[i]->end->x, content->lines[i]->end->y);
		} else
		{
			diagonal_line(grid, content->lines[i]->start->x, content->lines[i]->end->x, content->lines[i]->start->y, content->lines[i]->end->y);
		}
	}
	
	//diagonal_line(grid, 8, 0, 0, 8);
	
	return grid;
}

int overlap(struct grid *grid) {
	int overlap = 0;
	for (int i = 0; i < grid->max_x*grid->max_y; i++)
	{
		if (grid->pts[i] > 1)
		{
			overlap ++;
		}
	}
	
	return overlap;
}

void free_line(struct line *line) {
	free(line->end);
	free(line->start);
	free(line);
}

void free_content(struct content *content) {
	for (int i = 0; i < content->n_lines; i++)
	{
		free_line(content->lines[i]);
	}
	free(content->lines);
	
	free(content);
}

void free_grid(struct grid *grid) {
	free(grid->pts);
	free(grid);
}

int main(int argc, char** argv) {
	FILE* input = fopen("input", "r");

	struct content *content = get_content(input);
	struct grid *grid = get_grid_from_content(content);

	//show_content(content);
	//show_grid(grid);

	printf("%d points \n", overlap(grid));

	free_content(content);
	free_grid(grid);
	fclose(input);
	return EXIT_SUCCESS;
}

