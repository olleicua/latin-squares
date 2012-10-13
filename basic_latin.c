#include "backtrack.c"

int* row_used;
int* col_used;

bool is_complete(latin_grid square, coord position) {
  return (position->col >= square->size);
}

bool is_terminal(latin_grid square, coord position) {
  int mask = (1 << square->size) - 1;
  int used_union = (mask & row_used[position->row]) | (mask & col_used[position->col]);
  return used_union == mask;
}

coord next_coord(latin_grid square, coord position) {
  coord result = new_coord();
  result->row = position->row + 1;
  result->col = position->col;
  if (result->row >= square->size) {
	result->row = 1;
	result->col = position->col + 1;
  }
  return result;
}

bool is_allowed(latin_grid square, coord position, int symbol) {
  int mask = 1 << symbol;
  return ! ((row_used[position->row] & mask) ||
			(col_used[position->col] & mask));
}

void set_avail(int* array, int a_index, int b_index, bool val) {
  if (val) {
	int mask = 1 << b_index;
	array[a_index] |= mask; // set the value to true
  } else {
	int mask = ~(1 << b_index);
	array[a_index] &= mask; // set the value to false
  }
}

void grid_write(latin_grid square, coord position, int symbol) {
  // old value is available
  int old_symbol = CELL(square, position->row, position->col);
  set_avail(row_used, position->row, old_symbol, false);
  set_avail(col_used, position->col, old_symbol, false);
  // new value is not
  set_avail(row_used, position->row, symbol, true);
  set_avail(col_used, position->col, symbol, true);
  CELL(square, position->row, position->col) = symbol;
}

void before_print_callback(latin_grid square) {
  printf("row-completeness-repeats: %d\n", row_completeness_repeats(square));
}

int main() {
  latin_grid square;
  coord position = new_coord();
  int size;
  for (size = 1; size <= 6; size++) {
	printf("-- %d --\n", size);
	row_used = malloc(size * sizeof(int));
	col_used = malloc(size * sizeof(int));
	square = new_latin_grid(size);
	normalize_grid(square);
	position->row = position->col = 1;
	backtrack(square, position);
  }
  
  return 0;
}
