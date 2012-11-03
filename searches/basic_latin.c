#include "../lib/backtrack.c"

int* row_used;
int* col_used;
int min_row_completeness;

bool is_finished(latin_grid square, coord position) {
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

void set_used(int* array, int a_index, int b_index, bool val) {
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
  set_used(row_used, position->row, old_symbol, false);
  set_used(col_used, position->col, old_symbol, false);
  // new value is not
  set_used(row_used, position->row, symbol, true);
  set_used(col_used, position->col, symbol, true);
  CELL(square, position->row, position->col) = symbol;
}

void print_success(latin_grid square) {
  int repeats = row_completeness_repeats(square);
  printf("row-completeness-repeats: %d\n", repeats);
  print_latin_grid(square);
  if (repeats < min_row_completeness) {
	min_row_completeness = repeats;
  }
}

latin_grid square;
coord position;

void init() {
  position = new_coord();
}

void loop(size) {
	printf("-- %d --\n", size);
	row_used = malloc(size * sizeof(int));
	col_used = malloc(size * sizeof(int));
	min_row_completeness = size * 2;
	square = new_latin_grid(size);
	normalize_grid(square);
	position->row = position->col = 1;
	backtrack(square, position);
	printf(" min row completeness repeats %d at size %d\n", min_row_completeness, size);
}
