#include "../lib/squares.c"

void grid_write(latin_grid square, coord position, int symbol) {}

void brute_search(latin_grid square, int row, int col) {
  if (col >= square->size) {
    if (is_latin(square)) {
      print_latin_grid(square);
    }
    return;
  }
  int next_row = row + 1;
  int next_col = col;
  if (next_row >= square->size) {
    next_row = 0;
    next_col = col + 1;
  }
  int symbol;
  for (symbol = 0; symbol < square->size; symbol++) {
    CELL(square, row, col) = symbol;
    brute_search(square, next_row, next_col);
  }
}

int main() {
  int size;
  for (size = 1; size <= 3; size++) {
    latin_grid square = new_latin_grid(size);
    brute_search(square, 0, 0);
  }
  
  return 0;
}
