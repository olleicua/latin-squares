#include "squares.c"

int main() {
  int row, col;
  latin_grid square1 = new_latin_grid(5);
  latin_grid square2 = new_latin_grid(5);
  for (row = 0; row < 5; row++) {
	for (col = 0; col < 5; col++) {
	  CELL(square1, row, col) = row + col;
	  CELL(square2, row, col) = row * col;
	}
  }
  printf("square 1:\n");
  print_latin_grid(square1);
  printf("\n");
  printf("square 2:\n");
  print_latin_grid(square2);
  printf("\n");
  printf("square 1 has %d row repeats, square 2 has %d\n",
		 row_repeats(square1), row_repeats(square2));
  printf("square 1 has %d column repeats, square 2 has %d\n",
		 column_repeats(square1), column_repeats(square2));
  printf("square 1 has %d row completeness repeats, square 2 has %d\n",
		 row_completeness_repeats(square1), row_completeness_repeats(square2));
  printf("square 1 and square 2 have %d orthogonality repeats\n",
		 orthogonality_repeats(square1, square2));
  
  return 0;
}
