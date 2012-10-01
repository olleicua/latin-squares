#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _list {
  char* list;
  int size;
  int max_size;
}* list;

list new_list(int max_size) {
  list list = malloc(sizeof(struct _list));
  list->size = 0;
  list->max_size = max_size;
  list->list = malloc(max_size * sizeof(char));
  return list;
}

list push(list list, char symbol) {
  if (list->size < list->max_size) {
	list->list[list->size] = symbol;
	list->size++;
  }
  return list;
}

typedef struct _latin_grid {
  char** grid;
  int size;
}* latin_grid;

latin_grid new_latin_grid(int size) {
  int row, col;
  latin_grid latin_grid = malloc(sizeof(struct _latin_grid));
  latin_grid->size = size;
  latin_grid->grid = malloc(size * sizeof(char *));
  for (row = 0; row < size; row++) {
	latin_grid->grid[row] = malloc(size * sizeof(char));
	for (col = 0; col < size; col++) {
	  latin_grid->grid[row][col] = 1;
	}	
  }
  return latin_grid;
}

void print_latin_grid(latin_grid grid) {
  int row, col;
  for (row = 0; row < grid->size; row++) {
	for (col = 0; col < grid->size; col++) {
	  printf("%d", grid->grid[row][col]);
	}
	printf("\n");
  }
}

bool is_latin(latin_grid grid) {
  return (row_repeats(grid) == 0) && (column_repeats(grid) == 0);
}

bool is_row_complete(latin_grid grid) {
  return row_completeness_repeats(grid) == 0;
}

bool is_orthogonal(latin_grid grid1, latin_grid grid2) {
  return orthogonality_repeats(grid1, grid2) == 0;
}

int row_repeats(latin_grid grid) {
  int row, col;
  int count = 0;
  list row_list = new_list(grid->size);
  for (row = 0; row < grid->size; row++) {
	row_list->size = 0;
	for (col = 0; col < grid->size; col++) {
	  push(row_list, grid->grid[row][col]);
	}
	count += repeats(row_list, grid->size);
  }
  return count;
}

int column_repeats(latin_grid grid) {
  int row, col;
  int count = 0;
  list column_list = new_list(grid->size);
  for (col = 0; col < grid->size; col++) {
	column_list->size = 0;
	for (row = 0; row < grid->size; row++) {
	  push(column_list, grid->grid[row][col]);
	}
	count += repeats(column_list, grid->size);
  }
  return count;
}

int row_completeness_repeats(latin_grid grid) {
  int row, col;
  list pair_list = new_list(grid->size * grid->size);
  for (row = 0; row < grid->size; row++) {
	for (col = 0; col < (grid->size - 1); col++) {
	  int pair_id = (grid->grid[row][col] * grid->size) +
		grid->grid[row][col + 1];
	  push(pair_list, pair_id);
	}
  }
  return repeats(pair_list, grid->size * grid->size);
}

int orthogonality_repeats(latin_grid grid1, latin_grid grid2) {
  int row, col;
  list pair_list = new_list(grid1->size * grid1->size);
  for (row = 0; row < grid1->size; row++) {
	for (col = 0; col < grid1->size; col++) {
	  int pair_id = (grid1->grid[row][col] * grid1->size) +
		grid2->grid[row][col];
	  push(pair_list, pair_id);
	}
  }
  return repeats(pair_list, grid1->size * grid1->size);
}

int repeats(list list, int space) {
  int i;
  int count = 0;
  bool usage[space];
  for (i = 0; i < space; i++) {
	usage[i] = false;
  }
  for (i = 0; i < list->size; i++) {
	if (list->list[i] < space) {
	  if (usage[list->list[i]]) {
		count++;
	  } else {
		usage[list->list[i]] = true;
	  }
	}
  }
  return count;
}

int main() {
  latin_grid square = new_latin_grid(5);  
  latin_grid square2 = new_latin_grid(5);  
  print_latin_grid(square);
  printf("\n");
  print_latin_grid(square2);
  //printf("\n%d, %d, %d, %d, %d\n", row_repeats(square), column_repeats(square), is_latin(square), row_completeness_repeats(square), orthogonality_repeats(square, square2));
  printf("\n%d\n", orthogonality_repeats(square, square2));
  
  return 0;
}
