#include "squares.c"

typedef struct _coord {
  int row;
  int col;
}* coord;

coord new_coord() {
  return malloc(sizeof(struct _coord));
}

bool is_complete(latin_grid square, coord position);
bool is_terminal(latin_grid square, coord position);
coord next_coord(latin_grid square, coord position);
bool is_allowed(latin_grid square, coord position, int symbol);
void grid_write(latin_grid square, coord position, int symbol);

void backtrack(latin_grid square, coord position) {
  
  if (is_complete(square, position)) {
	print_latin_grid(square);
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
