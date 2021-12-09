#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 150

struct grid_case {
	int value;
	struct grid_case *up;
	struct grid_case *down;
	struct grid_case *right;
	struct grid_case *left;
	int id;
};

struct grid {
	int x_max;
	int y_max;
	struct grid_case **values;
};

struct grid *init_grid(int x, int y) {
	printf("Creating a %dx%d grid\n", x, y);
	struct grid *grid = malloc(sizeof(struct grid));
	grid->x_max = x;
	grid->y_max = y;
	grid->values = malloc(x*y*sizeof(struct grid_case *));
	for (int i = 0; i < x*y; i++)
	{
		grid->values[i] = malloc(sizeof(struct grid_case));
		grid->values[i]->down = NULL;
		grid->values[i]->up = NULL;
		grid->values[i]->right = NULL;
		grid->values[i]->left = NULL;
		grid->values[i]->id = i;
	}
	
	return grid;
}

void free_grid(struct grid *grid) {
	for (int i = 0; i < grid->x_max*grid->y_max; i++)
	{
		free(grid->values[i]);
	}
	free(grid->values);
	free(grid);
}

void show_grid(struct grid *grid) {
	for (int i = 0; i < grid->y_max; i++)
	{
		for (int j = 0; j < grid->x_max; j++)
		{
			printf("%d", grid->values[i*grid->x_max+j]->value);
		}
		printf("\n");
	}
	
}

struct grid *get_grid_from_context(FILE *context) {
	char line[LINE_LENGTH];
	char *str;
	int x = 0;
	int y = 0;

	while (fgets(line, LINE_LENGTH, context))
	{
		if (!x)
		{
			str = strtok(line, "\n");
			x = strlen(str);
		}
		y++;
	}
	
	struct grid *grid = init_grid(x, y);
	rewind(context);
	char car;
	for (int i = 0; i < y; i++)
	{
		fgets(line, LINE_LENGTH, context);
		str = strtok(line, "\n");
		for (int j = 0; j < x; j++)
		{
			car = str[j];
			grid->values[i*x+j]->value = car - '0';
			if (i)
			{
				grid->values[i*x+j]->up = grid->values[(i-1)*x+j];
				grid->values[(i-1)*x+j]->down = grid->values[i*x+j];
			}
			if (j)
			{
				grid->values[i*x+j]->left = grid->values[i*x+j-1];
				grid->values[i*x+j-1]->right = grid->values[i*x + j];
			}
		}
	}
	
	return grid;

}

int is_low_point(struct grid_case *cas) {
	if (cas->down && cas->down->value <= cas->value)
	{
		return 0;
	}
	if (cas->up && cas->up->value <= cas->value)
	{
		return 0;
	}
	if (cas->left && cas->left->value <= cas->value)
	{
		return 0;
	}
	if (cas->right && cas->right->value <= cas->value)
	{
		return 0;
	}
	return 1;
}

int is_value_in_tab(int val, int tab[], int tab_size) {
	for(int i = 0; i < tab_size; i++) {
		if (tab[i] == val)
		{
			return 1;
		}
		
	}
	return 0;
}

int basin_size(struct grid_case *cas, int id_in_bassin[], int cur){
	int bas_size;
	if (!cas)
	{
		return 0;
	}
	if (cas->value == 9)
	{
		return 0;
	}
	if (is_value_in_tab(cas->id, id_in_bassin, cur))
	{
		return 0;
	}
	
	id_in_bassin[cur] = cas->id;
	bas_size = 1;
	bas_size += basin_size(cas->up, id_in_bassin, cur+bas_size);
	bas_size += basin_size(cas->down, id_in_bassin, cur+bas_size);
	bas_size += basin_size(cas->right, id_in_bassin, cur+bas_size);
	bas_size += basin_size(cas->left, id_in_bassin, cur+bas_size);

	return bas_size;
}

int get_number_of_low_points(struct grid *grid)
{
	int n = 0;
	for (int i = 0; i < grid->x_max*grid->y_max; i++)
	{
		if (is_low_point(grid->values[i]))
		{
			n++;
		}
	}
	return n;
}

int *get_bassins_size(struct grid *grid)
{
	int n_low_points;
	n_low_points = get_number_of_low_points(grid);
	printf("Nomber of low points : %d \n", n_low_points);
	int *bassins_size = malloc(sizeof(int)*n_low_points);
	int cur = 0;
	for (int i = 0; i < grid->x_max*grid->y_max; i++)
	{
		if (is_low_point(grid->values[i]))
		{
			printf("[%d] %d is a low point\n", i, grid->values[i]->value);
			int id_in_bassin[grid->x_max*grid->y_max];
			bassins_size[cur] = basin_size(grid->values[i], id_in_bassin, 0);
			printf("Bassin size for %d : %d\n", grid->values[i]->id, bassins_size[cur]);
			cur++;
		}
	}
	return bassins_size;
}

int get_risk_level(struct grid *grid) {
	int sum = 0;
	for (int i = 0; i < grid->x_max*grid->y_max; i++)
	{
		if (is_low_point(grid->values[i]))
		{
			printf("[%d] %d is a low point\n", i, grid->values[i]->value);
			sum += grid->values[i]->value + 1;
		}
	}
	return sum;
}

void pop_from_list(int val, int *list, int size) {
	for (int i = 0; i < size; i++)
	{
		if (list[i] == val)
		{
			list[i] = -1;
			return;
		}
	}
}

int max_for_list(int *list, int size) {
	int max = -1;
	for (int i = 0; i < size; i++)
	{
		if (max < list[i]) {
			max = list[i];
		}
	}

	return max;
	
}

int main(int argc, char** argv) {
	FILE *input = fopen("input", "r");
	struct grid *grid = get_grid_from_context(input);
	show_grid(grid);
	int n_bassins = get_number_of_low_points(grid);
	int *basins_size = get_bassins_size(grid);
	int max_1, max_2, max_3;
	max_1 = max_for_list(basins_size, n_bassins);
	pop_from_list(max_1, basins_size, n_bassins);
	max_2 = max_for_list(basins_size, n_bassins);
	pop_from_list(max_2, basins_size, n_bassins);
	max_3 = max_for_list(basins_size, n_bassins);
	printf("%d*%d*%d = %d\n", max_1, max_2, max_3, max_1*max_2*max_3);
	free(grid);
	free(basins_size);
	fclose(input);
	return EXIT_SUCCESS;
}
