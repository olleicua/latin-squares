#include "../lib/backtrack.c"

// row_used_X and col_used_X are arrays of bit arrays to keep track of symbol
// availability in each row/column in square X.  The nth bit of the mth element
// of this array is a 1 iff symbol n is already taken for the mth row/column.
int* row_used_A;
int* col_used_A;
int* row_used_B;
int* col_used_B;
// pair_used an array of bits to keep track of the availability of orthogonal
// pairs.  The nth bit of the mth element of this array is a 1 iff A[x,y] is n
// and B[x,y] is m for some x,y that has been filled in both A and B.
int* pair_used;

// current state
latin_grid square_A;
latin_grid square_B;
latin_grid current_square;
coord position;

// current best row completeness
int min_row_completeness_A;
int min_row_completeness_B;
int min_row_completeness_total;

// current best diagonal completeness
int min_diagonal_completeness_AB;
int min_diagonal_completeness_BA;
int min_diagonal_completeness_total;

bool is_finished(latin_grid square, coord position) {
  return (position->col >= square->size);
}

bool is_terminal(latin_grid square, coord position) {
  int mask = (1 << square->size) - 1;
  int* row_used = (square == square_B) ? row_used_B : row_used_A;
  int* col_used = (square == square_B) ? col_used_B : col_used_A;
  
  int pair_possibilities = 0;
  if (square == square_B) {
	pair_possibilities = mask &
	  pair_used[CELL(square_A, position->row, position->col)];
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
	result->row = (square == square_B) ? 0 : 1;
	result->col = position->col + 1;
  }
  return result;
}

bool is_allowed(latin_grid square, coord position, int symbol) {
  int mask = 1 << symbol;
  int* row_used = (square == square_B) ? row_used_B : row_used_A;
  int* col_used = (square == square_B) ? col_used_B : col_used_A;

  int pair_possibilities = 0;
  if (square == square_B) {
	pair_possibilities = mask &
	  pair_used[CELL(square_A, position->row, position->col)];
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
  
  int* row_used = (square == square_B) ? row_used_B : row_used_A;
  int* col_used = (square == square_B) ? col_used_B : col_used_A;

  // old value is available
  int old_symbol = CELL(square, position->row, position->col);
  set_used(row_used, position->row, old_symbol, false);
  set_used(col_used, position->col, old_symbol, false);
  // new value is not
  set_used(row_used, position->row, symbol, true);
  set_used(col_used, position->col, symbol, true);
  // adjacent pairs
  if (square == square_B) {
	set_used(pair_used, CELL(square_A, position->row, position->col),
			 old_symbol, false);
	set_used(pair_used, CELL(square_A, position->row, position->col),
			 symbol, true);
  }
  CELL(square, position->row, position->col) = symbol;
}

void print_success(latin_grid square) {
  if (square == square_B) {
	int repeats_A = row_completeness_repeats(square_A);
	int repeats_B = row_completeness_repeats(square_B);
	int repeats_total = repeats_A + repeats_B;
	int repeats_d_AB = diagonal_repeats(square_A, square_B);
	int repeats_d_BA = diagonal_repeats(square_B, square_A);
	int repeats_d_total = repeats_d_AB + repeats_d_BA;
	printf("row-completeness-repeats A: %d\n", repeats_A);
	printf("row-completeness-repeats B: %d\n", repeats_B);
	printf("row-completeness-repeats total: %d\n\n", repeats_total);
	printf("row-diagonal-repeats AB: %d\n", repeats_d_AB);
	printf("row-diagonal-repeats BA: %d\n", repeats_d_BA);
	printf("row-diagonal-repeats total: %d\n\n", repeats_d_total);
	print_latin_grid(square_A);
	print_latin_grid(square_B);
	printf(" *\n\n");
	if (repeats_A < min_row_completeness_A) {
	  min_row_completeness_A = repeats_A;
	}
	if (repeats_B < min_row_completeness_B) {
	  min_row_completeness_B = repeats_B;
	}
	if (repeats_total < min_row_completeness_total) {
	  min_row_completeness_total = repeats_total;
	}

	if (repeats_d_AB < min_diagonal_completeness_AB) {
	  min_diagonal_completeness_AB = repeats_d_AB;
	}
	if (repeats_d_BA < min_diagonal_completeness_BA) {
	  min_diagonal_completeness_BA = repeats_d_BA;
	}
	if (repeats_d_total < min_diagonal_completeness_total) {
	  min_diagonal_completeness_total = repeats_d_total;
	}
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
  printf("-- %d --\n", size);
  min_row_completeness_A = size * size;
  min_row_completeness_B = size * size;
  min_row_completeness_total = size * size;
  min_diagonal_completeness_AB = size * size;
  min_diagonal_completeness_BA = size * size;
  min_diagonal_completeness_total = size * size;

  // allocate memory
  row_used_A = malloc(size * sizeof(int));
  col_used_A = malloc(size * sizeof(int));
  row_used_B = malloc(size * sizeof(int));
  col_used_B = malloc(size * sizeof(int));
  pair_used = malloc(size * sizeof(int));
  square_A = new_latin_grid(size);
  square_B = new_latin_grid(size);
  
  // search
  normalize_grid(square_A);
  position->row = position->col = 1;
  backtrack(square_A, position);
  printf(" min row completeness repeats A %d at size %d\n",
		 min_row_completeness_A, size);
  printf(" min row completeness repeats B %d at size %d\n",
		 min_row_completeness_B, size);
  printf(" min row completeness repeats total %d at size %d\n\n",
		 min_row_completeness_total, size);
  printf(" min diagonal completeness repeats AB %d at size %d\n",
		 min_diagonal_completeness_AB, size);
  printf(" min diagonal completeness repeats BA %d at size %d\n",
		 min_diagonal_completeness_BA, size);
  printf(" min diagonal completeness repeats total %d at size %d\n\n",
		 min_diagonal_completeness_total, size);

}
