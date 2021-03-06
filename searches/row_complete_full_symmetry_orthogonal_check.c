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

// linked list of squares
typedef struct _square_list {
  latin_grid square;
  struct _square_list* next;
}* square_list;

square_list result_list;

square_list square_list_push(square_list list, latin_grid square) {
  square_list result = malloc(sizeof(square_list));
  result->square = square;
  result->next = list;
  return result;
}

int previous_cell(latin_grid square, coord position) {
  if (position->col > 0) {
    return CELL(square, position->row, position->col - 1);
  }
  return square->size;
}

bool is_finished(latin_grid square, coord position) {
  return (position->row >= square->size);
}

int serialize_row(latin_grid square, int row) {
  int i;
  int result = 0;
  for (i = 0; i < square->size; i++) {
    result *= square->size;
    result += CELL(square, row, i);
  }
  return result;
}

int reverse_serialize_row(latin_grid square, int row) {
  int i;
  int result = 0;
  for (i = square->size - 1; i >= 0; i--) {
    result *= square->size;
    result += square->size - CELL(square, row, i) - 1;
  }
  return result;
}

int mapping(int size, int map, int in) {
  int i;
  for (i = size - 1; i >= 0; i--) {
    if (in == map % size) {
      return i;
    }
    map /= size;
  }
  printf("OOPS!! this is not where you want to be!\n");
}

int row_map(int size, int map, int row) {
  int i;
  int result = 0;
  int multiplier = 1;
  for (i = 0; i < size; i++) {
    result += mapping(size, map, (row % size)) * multiplier;
    multiplier *= size;
    row /= size;
  }
}

bool is_shuffle_redundant(latin_grid square, int row, int target) {
  int target_row = CELL(square, row, target);
  if (CELL(square, target_row, target) >= square->size) {
    return false;
  }
  int this_row = serialize_row(square, row);
  int old_row = serialize_row(square, target);
  int new_row = row_map(square->size, this_row,
                        serialize_row(square, target_row));
  if (new_row > old_row) {
    return true;
  }
  if (new_row == old_row) {
    return is_shuffle_redundant(square, row, target + 1);
  }
  return false;
}

bool is_mirror_redundant(latin_grid square, int row) {
  
  if (CELL(square, row, square->size - 1) != square->size - 2) {
    return false;
  }
  
  if (reverse_serialize_row(square, row) > serialize_row(square, 1)) {
    return true;
  }
  
  return false;
}

bool is_terminal(latin_grid square, coord position) {
  int i;
  
  // check for row shuffling symmetry if we are at the end of a line
  if (position->col == 1 && position->row > 2) {
    for (i = 1; i < position->row; i++) {
      if (is_shuffle_redundant(square, i, 1)) {
        return true;
      }
    }
  }

  // check for mirror symmetry if we are at the end of a line
  if (position->col == 1 && position->row > 1) {
    if (is_mirror_redundant(square, position->row - 1)) {
        return true;
    }
  }
  
  // check that there are available symbols for the next square
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
  result->row = position->row;
  result->col = position->col + 1;
  if (result->col >= square->size) {
    result->row = position->row + 1;
    result->col = 1;
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
  if (verbose) {
    report1(square);
  }
  result_list = square_list_push(result_list, square_copy(square));
}

void compare_pairs(int size) {
  square_list sq_ls1, sq_ls2;
  int best_orthogonal = size * size;
  int best_diagonal = size * size;
  int best_AD = size * size * size;
  int best_ADE = size * size * size;
  int orthogonal, diagonal_AB, diagonal_BA, diagonal, AD, ADE;
  bool show_pair;
  for (sq_ls1 = result_list; sq_ls1 != NULL; sq_ls1 = sq_ls1->next) {
    for (sq_ls2 = sq_ls1->next; sq_ls2 != NULL; sq_ls2 = sq_ls2->next) {
      orthogonal = orthogonality_repeats(sq_ls1->square, sq_ls2->square);
      diagonal_AB = diagonal_repeats(sq_ls1->square, sq_ls2->square);
      diagonal_BA = diagonal_repeats(sq_ls2->square, sq_ls1->square);
      diagonal = diagonal_AB < diagonal_BA ? diagonal_AB : diagonal_BA;
      AD = orthogonal + diagonal;
      ADE = orthogonal + diagonal_AB + diagonal_BA;
      show_pair = false;
      if (orthogonal < best_orthogonal) {
        best_orthogonal = orthogonal;
        show_pair = true;
      }
      if (diagonal < best_diagonal) {
        best_diagonal = diagonal;
        show_pair = true;
      }
      if (AD < best_AD) {
        best_AD = AD;
        show_pair = true;
      }
      if (ADE < best_ADE) {
        best_ADE = ADE;
        show_pair = true;
      }
      if (show_pair) {
        printf("/-pair-\\\n");
        report2(sq_ls1->square, sq_ls2->square);
      }
    }
  }
}

latin_grid square;
coord position;

void init() {
  position = new_coord();
}

void loop(size) {
  printf("-- %d --\n", size);

  result_list = NULL;

  row_used = malloc(size * sizeof(int));
  col_used = malloc(size * sizeof(int));
  pair_used = malloc(size * sizeof(int));
  square = new_latin_grid(size);
  normalize_grid(square);
  position->row = position->col = 1;
  backtrack(square, position);
  compare_pairs(size);
}

void finish() {}
