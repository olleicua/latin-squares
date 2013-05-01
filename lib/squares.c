// Defines an API for interacting with latin squares

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "squares.h"
#define CELL(square, row, col) ((square)->grid[(row)][(col)])

bool logging;
FILE *logfile;

coord new_coord() {
  return malloc(sizeof(struct _coord));
}

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

latin_grid square_copy(latin_grid square) {
  int row, col;
  latin_grid result = new_latin_grid(square->size);
  for (row = 0; row < square->size; row++) {
    for (col = 0; col < square->size; col++) {
      CELL(result, row, col) = CELL(square, row, col);
    }
  }
  return result;
}

void grid_write(latin_grid square, coord position, int symbol);

latin_grid normalize_grid(latin_grid grid) {
  int i;
  coord pos = new_coord();
  for (i = 0; i < grid->size; i++) {
    pos->row = 0;
    pos->col = i;
    grid_write(grid, pos, i);
    pos->row = i;
    pos->col = 0;
    grid_write(grid, pos, i);
  }
  return grid;
}

void print_latin_grid(latin_grid grid) {
  int row, col;
  for (row = 0; row < grid->size; row++) {
    for (col = 0; col < grid->size; col++) {
      int symbol = CELL(grid, row, col);
      printf("%c", (symbol < 10) ? '0' + symbol : 'W' + symbol);
    }
    printf("\n");
  }
  printf("\n");
}

void log_latin_grid(latin_grid grid) {
  int row, col;
  for (row = 0; row < grid->size; row++) {
    for (col = 0; col < grid->size; col++) {
      int symbol = CELL(grid, row, col);
      fprintf(logfile, "%c", (symbol < 10) ? '0' + symbol : 'W' + symbol);
    }
    fprintf(logfile, "\n");
  }
  fprintf(logfile, "\n");
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

int diagonal_repeats(latin_grid grid1, latin_grid grid2) {
  // counts repeated pairs of the (grid1[n,m], grid2[n,m+1])
  int row, col;
  list pair_list = new_list(grid1->size * grid1->size);
  for (row = 0; row < grid1->size; row++) {
    for (col = 0; col < (grid1->size - 1); col++) {
      int pair_id = generate_pair_id(grid1->size, CELL(grid1, row, col),
                                     CELL(grid2, row, col + 1));
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

void print_bits(long array, int size) {
  int i;
  for (i = size - 1; i >= 0; i--) {
    printf("%s", (array & (1 << i)) ? "1" : "0");
  }
}

void print_bit_array(int* array, int size) {
  int i;
  for (i = 0; i < size; i++) {
    printf("(%i):", i);
    print_bits((long)array[i], size);
    printf(" ");
  }
  printf("\n");
}

bool verbose;

void report1(latin_grid square) {
  if (verbose) {
    print_latin_grid(square);
  }
  printf("b:%d\n\n", row_completeness_repeats(square));
}

void report2(latin_grid square1, latin_grid square2) {
  if (verbose) {
    print_latin_grid(square1);
    print_latin_grid(square2);
  }
  
  if (logging) {
    log_latin_grid(square1);
    log_latin_grid(square2);
  }

  int a = orthogonality_repeats(square1, square2);
  int b = row_completeness_repeats(square1);
  int c = row_completeness_repeats(square2);
  int d = diagonal_repeats(square1, square2);
  int e = diagonal_repeats(square2, square1);
  
  printf("a:%d, b:%d, c:%d, d:%d, e:%d\n",
         a, b, c, d, e);
  
  printf("2a+b+c:%d, a+b+d:%d, a+c+e:%d, a+b+c+d+e:%d\n\n",
         (2 * a) + b + c,
         a + b + d,
         a + c + e,
         a + b + c + d + e);

  if (logging) {
    fprintf(logfile, "a:%d, b:%d, c:%d, d:%d, e:%d\n",
            a, b, c, d, e);
    
    fprintf(logfile, "2a+b+c:%d, a+b+d:%d, a+c+e:%d, a+b+c+d+e:%d\n\n",
            (2 * a) + b + c,
            a + b + d,
            a + c + e,
            a + b + c + d + e);
    fflush(logfile);
  }
}
