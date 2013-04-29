#include "../lib/squares.c"
#include "../lib/cyclic.c"

void grid_write(latin_grid square, coord position, int symbol) {}

// use group theory to fill in the rest of the square from the first row
void fill_in_square(int* first_row, latin_grid square) {
  int row, col;

  for (col = 0; col < square->size; col++) {
	CELL(square, 0, col) = first_row[col];
  }
  
  for (row = 1; row < square->size; row++) {
    for (col = 0; col < square->size; col++) {
      CELL(square, row, col) =
        cyclic_multiply(row, CELL(square, 0, col), square->size);
    }
  }
}

int main() {
  
  verbose = true;
  
  int sequence_13A[13] = { 0, 2, 4, 8, 3, 6, 12, 11, 9, 5, 10, 7, 1 };
  int sequence_13B[13] = { 0, 8, 3, 6, 12, 11, 9, 5, 10, 7, 1, 2, 4 };
  int sequence_17A[17] = { 0, 3, 9, 10, 13, 5, 15, 11, 16, 14, 8, 7, 4, 12, 2, 6, 1 };
  int sequence_17B[17] = { 0, 10, 13, 5, 15, 11, 16, 14, 8, 7, 4, 12, 2, 6, 1, 3, 9 };
  int sequence_19A[19] = { 0, 2, 4, 8, 16, 13, 7, 14, 9, 18, 17, 15, 11, 3, 6, 12, 5, 10, 1 };
  int sequence_19B[19] = { 0, 8, 16, 13, 7, 14, 9, 18, 17, 15, 11, 3, 6, 12, 5, 10, 1, 2, 4 };
  
  latin_grid square13A = new_latin_grid(13);
  fill_in_square(sequence_13A, square13A);
  latin_grid square13B = new_latin_grid(13);
  fill_in_square(sequence_13B, square13B);
  latin_grid square17A = new_latin_grid(17);
  fill_in_square(sequence_17A, square17A);
  latin_grid square17B = new_latin_grid(17);
  fill_in_square(sequence_17B, square17B);
  latin_grid square19A = new_latin_grid(19);
  fill_in_square(sequence_19A, square19A);
  latin_grid square19B = new_latin_grid(19);
  fill_in_square(sequence_19B, square19B);
  
  report2(square13A, square13B);
  report2(square17A, square17B);
  report2(square19A, square19B);
  
  return 0;
}