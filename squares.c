#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "squares.h"
#define CELL(square, row, col) ((square)->grid[(row)][(col)])

list new_list(int max_size) {
  list list = malloc(sizeof(struct _list));
  list->size = 0;
  list->max_size = max_size;
  list->list = malloc(max_size * sizeof(int));
  return list;
}

list push(list list, int symbol) {
  if (list->size < list->max_size) {
	list->list[list->size] = symbol;
	list->size++;
  }
  return list;
}

latin_grid new_latin_grid(int size) {
  int row, col;
  latin_grid latin_grid = malloc(sizeof(struct _latin_grid));
  latin_grid->size = size;
  latin_grid->grid = malloc(size * sizeof(int *));
  for (row = 0; row < size; row++) {
	latin_grid->grid[row] = malloc(size * sizeof(int));
	for (col = 0; col < size; col++) {
	  CELL(latin_grid, row, col) = size;
	}	
  }
  return latin_grid;
}

latin_grid normalize_grid(latin_grid grid) {
  int i;
  for (i = 0; i < grid->size; i++) {
	CELL(grid, i, 0) = i;
	CELL(grid, 0, i) = i;
  }
}

void print_latin_grid(latin_grid grid) {
  int row, col;
  for (row = 0; row < grid->size; row++) {
	for (col = 0; col < grid->size; col++) {
	  printf("%d", CELL(grid, row, col));
	}
	printf("\n");
  }
  printf("\n");
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
	  push(row_list, CELL(grid, row, col));
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
	  push(column_list, CELL(grid, row, col));
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
	  int pair_id = (CELL(grid, row, col) * grid->size) +
		CELL(grid, row, col + 1);
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
	  int pair_id = generate_pair_id(grid1->size, CELL(grid1, row, col),
									 CELL(grid2, row, col));
	  push(pair_list, pair_id);
	}
  }
  return repeats(pair_list, grid1->size * grid1->size);
}

int generate_pair_id(int size, int symbol1, int symbol2) {
  if (symbol1 >= size || symbol2 >= size) {
	return size * size;
  }
  return (symbol1 * size) + symbol2;
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
