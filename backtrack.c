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
	}
  }
  grid_write(square, position, square->size);
}
