#include "backtrack.c"

bool is_complete(latin_grid square, coord position) {
  return (position->col >= square->size) && is_latin(square);
}

bool is_terminal(latin_grid square, coord position) {
  return (position->col >= square->size);
}

coord next_coord(latin_grid square, coord position) {
  coord result = new_coord();
  result->row = position->row + 1;
  result->col = position->col;
  if (result->row >= square->size) {
	result->row = 0;
	result->col = position->col + 1;
  }
  return result;
}

bool is_allowed(latin_grid square, coord position, int symbol) {
  return true;
}

void grid_write(latin_grid square, coord position, int symbol) {
  CELL(square, position->row, position->col) = symbol;
}

int main() {
  latin_grid square;
  coord position = new_coord();
  int size;
  for (size = 1; size <= 3; size++) {
	square = new_latin_grid(size);
	normalize_grid(square);
	position->row = position->col = 1;
	backtrack(square, position);
  }
  
  return 0;
}
