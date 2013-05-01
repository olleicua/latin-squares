// This file defines the shell of a backtracking search.
// It should be included in another file inwhich the function stubs below are
// implemented.

#include "squares.c"

bool is_finished(latin_grid square, coord position);
bool is_terminal(latin_grid square, coord position);
coord next_coord(latin_grid square, coord position);
bool is_allowed(latin_grid square, coord position, int symbol);
void grid_write(latin_grid square, coord position, int symbol);
void print_success(latin_grid square);

void backtrack(latin_grid square, coord position) {
  
  if (is_finished(square, position)) {
    print_success(square);
    return;
  }
  
  if (is_terminal(square, position)) {
    return;
  }
  
  coord next_position = next_coord(square, position);
  
  int symbol;
  for (symbol = 0; symbol < square->size; symbol++) {
    if (is_allowed(square, position, symbol)) {
      grid_write(square, position, symbol);
      backtrack(square, next_position);
      grid_write(square, position, square->size);
    }
  }
  grid_write(square, position, square->size); // square->size represents blank
  free(next_position);
}

void init();
void loop(int size);
void finish();
int main(int argc, char *argv[]) {
  
  if ((argc > 2) && (strcmp(argv[2], "--quiet") == 0)) {
    verbose = false;
  } else {
    verbose = true;
  }
  
  if ((argc > 2) && (strcmp(argv[2], "--log") == 0)) {
    logging = true;
    logfile = fopen("log", "a");
  } else {
    logging = false;
  }
  
  init();
  int size;
  int max = 3;
  if (argc > 1) {
    max = atoi(argv[1]);
  }
  for (size = 1; size <= max; size++) {
    loop(size);
  }
  finish();
  return 0;
}
