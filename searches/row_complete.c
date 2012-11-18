#include "../lib/backtrack.c"

// row_used and col_used are arrays of bit arrays to keep track of symbol
// availability in each row/column.  The nth bit of the mth element of this
// array is a 1 iff symbol n is already taken for the mth row/column
int* row_used;
int* col_used;
// pair_used an array of bits to keep track of the availability of adjacent
// pairs.  The nth bit of the mth element of this array is a 1 iff n already
// follows m somewhere on the grid.
int* pair_used;

int previous_cell(latin_grid square, coord position) {
  if (position->col > 0) {
	return CELL(square, position->row, position->col - 1);
  }
  return square->size;
}

bool is_finished(latin_grid square, coord position) {
  return (position->col >= square->size);
}

bool is_terminal(latin_grid square, coord position) {
  int mask = (1 << square->size) - 1;
  
  int pair_possibilities = 0;
  if (position->col >= 1) {
	pair_possibilities = mask & pair_used[previous_cell(square, position)];
  }
  int used_union = (mask & row_used[position->row]) |
	(mask & col_used[position->col]) | pair_possibilities;
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
  
  int pair_possibilities = 0;
  if (position->col > 0) {
	pair_possibilities = mask & pair_used[previous_cell(square, position)];
  }
  
  return ! ((row_used[position->row] & mask) ||
			(col_used[position->col] & mask) ||
			pair_possibilities);
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
  // adjacent pairs
  if (position->col > 0) {
	set_used(pair_used, previous_cell(square, position), old_symbol, false);
	set_used(pair_used, previous_cell(square, position), symbol, true);
  }
  CELL(square, position->row, position->col) = symbol;
}

void print_success(latin_grid square) {
  report1(square);
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
  pair_used = malloc(size * sizeof(int));
  square = new_latin_grid(size);
  normalize_grid(square);
  position->row = position->col = 1;
  backtrack(square, position);
}

void finish() {}
