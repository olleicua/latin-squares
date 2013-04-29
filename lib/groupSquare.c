void fill_in_square(latin_grid square) {
  int row, col;
  for (row = 1; row < square->size; row++) {
    for (col = 0; col < square->size; col++) {
      CELL(square, row, col) = (CELL(square, row - 1, col) + 1) % square->size;
    }
  }
}
