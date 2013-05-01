#include "../lib/squares.c"
#include "../lib/read_row_complete.c"
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

void grid_write(latin_grid square, coord position, int symbol) {}

int best_orthogonal, best_diagonal, best_2AD, best_2ADE, best_DE_for_A;

latin_grid temp_square2;
unsigned int rows_used;
int current_row;

void initialize(int size) {
  temp_square2 = new_latin_grid(size);
  rows_used = 0;
  current_row = 0;
}

bool is_available(int row) {
  // check whether the rowth bit of row_used is set
  return ! ((1 << row) & rows_used);
}

void choose_row(int row, latin_grid square2) {
  // put the rowth row of square2 into the next row in temp_square2
  int i;
  for (i = 0; i < square2->size; i++) {
    CELL(temp_square2, current_row, i) = CELL(square2, row, i);
  }
  current_row++;
  rows_used |= 1 << row;
}

void unchoose_row(int row, latin_grid square2) {
  // blank the current row of temp_square2 and mark the rowth row as being
  // available from square2
  current_row--;
  int i;
  for (i = 0; i < square2->size; i++) {
    CELL(temp_square2, current_row, i) = square2->size;
  }
  rows_used &= ~(1 << row);
}

bool check_metric(int metric, int* best, bool complete, latin_grid square1) {
  // check whether this metric is the best in this category
  // return true if it is worse in this category
  // print the squares if it is best in this category and complete
  //printf("m:%d b:%d\n", metric, *best);
  if (metric < *best) {
    if (complete) {
      *best = metric;
      report2(square1, temp_square2);
    }
    return false;
  }
  return true;
}

bool check_metrics(latin_grid square1) {
  // check whether this the best in any category.
  // return true if it is the worse than the best so far in every category
  // print it if the the square is complete and it is the best in some category
  bool is_complete = current_row == square1->size;
  int orthogonal = orthogonality_repeats(square1, temp_square2);
  int diagonal_AB = diagonal_repeats(square1, temp_square2) ;
  int diagonal_BA = diagonal_repeats(square1, temp_square2) ;
  int DE = diagonal_AB + diagonal_BA;
  if (orthogonal < best_orthogonal) {
    if (is_complete) {
      best_DE_for_A = DE;
      best_orthogonal = orthogonal;
      report2(square1, temp_square2);
    }
    return false;
  }
  if (orthogonal == best_orthogonal && DE < best_DE_for_A) {
    if (is_complete) {
      best_DE_for_A = DE;
      report2(square1, temp_square2);
    }
    return false;
  }
  return true;
}

void compare_permutations(latin_grid square1, latin_grid square2) {
  // search the space of permutations of the rows of the second square
  // stop if the current pair is worse in every metric than a pair we've found
  
  int i;
  for (i = 0; i < square1->size; i++) {
    //printf("row %d available? %d (%d)\n", i, is_available(i), rows_used);
    if (is_available(i)) {
      //printf("choosing row %d for row %d\n", i, current_row);
      choose_row(i, square2);
      bool bad_metrics = check_metrics(square1);
      //printf("bm:%d incomp:%d both: %d\n", (! bad_metrics), current_row < square1->size, (! bad_metrics) && current_row < square1->size);
      if ((! bad_metrics) && current_row < square1->size) {
        //printf("recurse\n");
        compare_permutations(square1, square2);
      }
      unchoose_row(i, square2);
    }
  }
}

void compare_pairs(int size, square_list squares) {
  initialize(size);
  square_list sq_ls1, sq_ls2;
  for (sq_ls1 = squares; sq_ls1 != NULL; sq_ls1 = sq_ls1->next) {
    for (sq_ls2 = sq_ls1; sq_ls2 != NULL; sq_ls2 = sq_ls2->next) {
      // iterate over each pair of squares (order doesn't matter but each square
      // should be compared to itself
      
      compare_permutations(sq_ls1->square, sq_ls2->square);
    }
  }
}

int main(int argc, char *argv[]) {
  
  // read squares from file
  if (argc <= 1) {
    printf("No order specified\n");
    return 1;
  }
  square_list squares = file2squares(argv[1]);
  if (squares == NULL) {
    return 1;
  }
  int size = squares->square->size;
  
  // initialize best metrics
  best_orthogonal = size;
  best_diagonal = size;
  best_2AD = 3 * size;
  best_2ADE = 4 * size;
  best_DE_for_A = size * size * 2;
  
  // look at all permuted combinations
  verbose = true;
  compare_pairs(size, squares);
  
  return 0;
}
