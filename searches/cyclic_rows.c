#include "../lib/backtrack.c"
#include "../lib/groupSquare.c"

// row_used is a bit arrays to keep track of symbol availability in the top row.
// The nth bit of this array is a 1 iff symbol n is already taken.
long row_used;
// diff_used an array of bits to keep track of the availability of adjacent pair
// differences.  The nth bit of this array is a 1 iff the difference of n is
// already used.
long diff_used;

int total_found;

latin_grid first_found;
latin_grid second_found;

int difference(latin_grid square, coord position, int symbol) {
  if (symbol >= square->size) {
	return square->size;
  }
  if (position->col > 0) {
	return (square->size +
			(symbol - CELL(square, position->row, position->col - 1)))
	  % square->size;
  }
  return square->size;
}

bool is_finished(latin_grid square, coord position) {
  return (position->col >= square->size);
}

bool is_terminal(latin_grid square, coord position) {
  // no static check
  return false;
}

coord next_coord(latin_grid square, coord position) {
  coord result = new_coord();
  result->row = position->row;
  result->col = position->col + 1;
  return result;
}

bool is_allowed(latin_grid square, coord position, int symbol) {
  long symbol_mask = 1 << symbol;
  long diff_mask = 1 << difference(square, position, symbol);
  
  return ! ((symbol_mask & row_used) ||
			((position->col > 0) ? diff_mask & diff_used : 0));
}

void set_used(long* array, int index, bool val) {
  if (val) {
	long mask = 1 << index;
	array[0] |= mask; // set the value to true
  } else {
	long mask = ~(1 << index);
	array[0] &= mask; // set the value to false
  }
}

void grid_write(latin_grid square, coord position, int symbol) {

  // old value is available
  int old_symbol = CELL(square, position->row, position->col);
  set_used(&row_used, old_symbol, false);

  // new value is not
  set_used(&row_used, symbol, true);

  // diffs
  if (position->col > 0) {
	set_used(&diff_used, difference(square, position, old_symbol), false);
	set_used(&diff_used, difference(square, position, symbol), true);
  }
  CELL(square, position->row, position->col) = symbol;
}

void print_success(latin_grid square) {
  if (verbose) {
	fill_in_square(square);
	print_latin_grid(square);
  }
  
  if (first_found == NULL) {
	first_found = square_copy(square);
  } else {
	if (second_found == NULL) {
	  second_found = square_copy(square);
	}
  }
  
  total_found++;
}

latin_grid square;
coord position;

void init() {
  position = new_coord();
}

void loop(size) {
  int _size = size * 2; // ignore odd orders
  printf("-- %d --\n", _size);
  first_found = NULL;
  second_found = NULL;
  total_found = 0;
  row_used = 0;
  diff_used = 0;
  square = new_latin_grid(_size);
  position->row = 0;
  position->col = 0;
  grid_write(square, position, 0);
  position->col = 1;
  backtrack(square, position);
  printf("\nfound %i of size %i\n", total_found, _size);
  if (first_found != NULL && second_found != NULL) {
	printf("\ncomparing squares:\n");
	report2(first_found, second_found);
  }
}

void finish() {}
