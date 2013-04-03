#include "../lib/backtrack.c"
#include "../lib/dihedral.c"

// current state
latin_grid square_A, square_B;
coord position;

// row_used_A and row_used_B are bit arrays to keep track of symbol availability
// in the top row of each square. The nth bit of this array is a 1 iff symbol n
// is already taken in that square.
long row_used_A, row_used_B;
// diff_used_A, diff_used_B, and diff_used_orthogonal are arrays of bits to keep
// track of the availability of pair differences in each square.  The nth bit of
// this array is a 1 iff the difference of n is already used in adjacent pairs
// in that square or in an orthogonal pair.
long diff_used_A, diff_used_B, diff_used_orthogonal;
int *diff_used_diagonal_AB, *diff_used_diagonal_BA;

int least_AB_repeats, least_diagonal_repeats, total_found,
  equivalent_to_v_used, equivalent_to_u_used;

int diagonal_repeat_count(int *repeats) {
  int i, result = 0;
  for (i = 0; i < square_A->size; i++) {
	if (repeats[i] < 0) {
	  printf("ASSERT FAILS! : %dth element is %d and should not be negative\n",
			 i, repeats[i]);
	  exit(0);
	}
	if (repeats[i] > 0) {
	  result += repeats[i] - 1;
	}
  }
  return result;
}

// use group theory to fill in the rest of the square from the first row
void fill_in_square(latin_grid square) {
  int row, col;
  for (row = 1; row < square->size; row++) {
	for (col = 0; col < square->size; col++) {
	  CELL(square, row, col) =
		dihedral_multiply(row, CELL(square, 0, col), square->size);
	}
  }
}

// use group theory to determine the "difference" between the specified symbol
// and the symbol in the space to the left of the specified position
int row_difference(latin_grid square, coord position, int symbol) {
  if (symbol >= square->size) {
	return square->size;
  }
  if (position->col > 0) {
	return dihedral_divide(symbol, CELL(square, position->row, position->col - 1),
						   square->size);
  }
  return square->size;
}

// use group theory to determine the "difference" between the specified symbol
// and the symbol in the specified position in square_A
int orthogonal_difference(coord position, int symbol) {
  if (symbol >= square_A->size) {
	return square_A->size;
  }
  return dihedral_divide(CELL(square_A, position->row, position->col),
						 symbol, square_A->size);
}

// use group theory to determine the "difference" between the specified symbol
// and the symbol to the left of the specified position in square_A
int diagonal_AB_difference(coord position, int symbol) {
  if (symbol >= square_A->size) {
	return square_A->size;
  }
  if (position->col > 0) {
	return dihedral_divide(CELL(square_A, position->row, position->col - 1),
						   symbol, square_A->size);
  }
  return square_A->size;
}

// use group theory to determine the "difference" between the specified symbol
// and the symbol to the right of the specified position in square_A
int diagonal_BA_difference(coord position, int symbol) {
  if (symbol >= square_A->size) {
	return square_A->size;
  }
  if (position->col < square_A->size - 1) {
	return dihedral_divide(CELL(square_A, position->row, position->col + 1),
						   symbol, square_A->size);
  }
  return square_A->size;
}

// just search the first row
bool is_finished(latin_grid square, coord position) {
  return (position->col >= square->size);
}

// backtrack if a better diagonal result exists
bool is_terminal(latin_grid square, coord position) {
  if (square == square_B) {
	int AB_repeats = diagonal_repeat_count(diff_used_diagonal_AB);
	int total_diagonal_repeats =
	  AB_repeats + diagonal_repeat_count(diff_used_diagonal_BA);
	if (AB_repeats >= least_AB_repeats &&
		total_diagonal_repeats >= least_diagonal_repeats) {
	  return true;
	}
  }
  return false;
}

coord next_coord(latin_grid square, coord position) {
  coord result = new_coord();
  result->row = position->row;
  result->col = position->col + 1;
  return result;
}

// check that the symbol, the "difference" between it and the previous symbol
// and the "difference" between it and it's orthogonal pair are all unique
bool is_allowed(latin_grid square, coord position, int symbol) {
  
  /*
  if (square == square_A) {
	if (equivalent_to_v_used == 0 &&
		dihedral_equivalent_to_v(symbol, square->size) &&
		symbol != 1) {
	  return false;
	}
	if (equivalent_to_u_used == 0 &&
		dihedral_equivalent_to_u(symbol, square->size) &&
		symbol != 2) {
	  return false;
	}
  }
  */
  
  long symbol_mask = 1 << symbol;
  long row_diff_mask = 1 << row_difference(square, position, symbol);
  
  if (square == square_A) {
	return ! ((symbol_mask & row_used_A) ||
			  ((position->col > 0) ? row_diff_mask & diff_used_A : 0));
  } else {
	long orthogonal_diff_mask = 1 << orthogonal_difference(position, symbol);
	return ! ((symbol_mask & row_used_B) ||
			  ((position->col > 0) ? row_diff_mask & diff_used_B : 0) ||
			  (orthogonal_diff_mask & diff_used_orthogonal));
  }
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

// add the symbol to the grid and to the usage arrays
void grid_write(latin_grid square, coord position, int symbol) {
  
  // TODO: clean this up
  
  int old_symbol = CELL(square, position->row, position->col);
  
  if (square == square_A) {
	// old value is available
	set_used(&row_used_A, old_symbol, false);
	
	// new value is not
	set_used(&row_used_A, symbol, true);
	
	// diffs
	if (position->col > 0) {
	  set_used(&diff_used_A, row_difference(square, position, old_symbol), false);
	  set_used(&diff_used_A, row_difference(square, position, symbol), true);
	}
	
	// dihedral equivalences
	if (dihedral_equivalent_to_v(old_symbol, square->size)) {
	  equivalent_to_v_used--;
	}
	if (dihedral_equivalent_to_u(old_symbol, square->size)) {
	  equivalent_to_u_used--;
	}
	if (dihedral_equivalent_to_v(symbol, square->size)) {
	  equivalent_to_v_used++;
	}
	if (dihedral_equivalent_to_u(symbol, square->size)) {
	  equivalent_to_u_used++;
	}
  } else {
	// old value is available
	set_used(&row_used_B, old_symbol, false);
	
	// new value is not
	set_used(&row_used_B, symbol, true);
	
	// diffs
	set_used(&diff_used_orthogonal,
			 orthogonal_difference(position, old_symbol), false);
	set_used(&diff_used_orthogonal,
			 orthogonal_difference(position, symbol), true);
	if (position->col > 0) {
	  set_used(&diff_used_B,
			   row_difference(square, position, old_symbol), false);
	  set_used(&diff_used_B,
			   row_difference(square, position, symbol), true);
	  diff_used_diagonal_AB[diagonal_AB_difference(position, old_symbol)]--;
	  diff_used_diagonal_AB[diagonal_AB_difference(position, symbol)]++;
	}
	if (position->col < square->size - 1) {
	  diff_used_diagonal_BA[diagonal_BA_difference(position, old_symbol)]--;
	  diff_used_diagonal_BA[diagonal_BA_difference(position, symbol)]++;
	}
  }
  CELL(square, position->row, position->col) = symbol;
}

void print_success(latin_grid square) {
  if (square == square_B) {
	bool new_result = false;
	int AB_repeats = diagonal_repeat_count(diff_used_diagonal_AB);
	int total_diagonal_repeats =
	  AB_repeats + diagonal_repeat_count(diff_used_diagonal_BA);
	if (AB_repeats < least_AB_repeats) {
	  least_AB_repeats = AB_repeats;
	  new_result = true;
	}
	if (total_diagonal_repeats < least_diagonal_repeats) {
	  least_diagonal_repeats = total_diagonal_repeats;
	  new_result = true;
	}
	
	if (verbose && new_result) {
	  fill_in_square(square_A);
	  fill_in_square(square_B);
	  report2(square_A, square_B);
	}
	
	total_found++;
  } else {
	coord position = new_coord();
	position->row = position->col = 0;
	backtrack(square_B, position);
  }
}

void init() {
  position = new_coord();
}

void loop(size) {
  
  // ignore orders besides 12, 16, and 20
  if (size != 12 && size != 16 && size != 20) {
	return;
  }
  
  printf("-- %d --\n", size);
  total_found = 0;
  row_used_A = row_used_B = diff_used_A = diff_used_B = diff_used_orthogonal = 0;
  diff_used_diagonal_AB = calloc(size + 1, sizeof(int));
  diff_used_diagonal_BA = calloc(size + 1, sizeof(int));
  least_AB_repeats = least_diagonal_repeats = size;
  square_A = new_latin_grid(size);
  square_B = new_latin_grid(size);
  position->row = 0;
  position->col = 0;
  grid_write(square_A, position, 0);
  position->col = 1;
  backtrack(square_A, position);
  printf("\nfound %i of size %i\n", total_found, size);
}

void finish() {}
