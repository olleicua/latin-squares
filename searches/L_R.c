#include "../lib/backtrack.c"

bool is_finished(latin_grid square, coord position) { }

bool is_terminal(latin_grid square, coord position) { }

coord next_coord(latin_grid square, coord position) { }

bool is_allowed(latin_grid square, coord position, int symbol) { }

void set_used(int* array, int a_index, int b_index, bool val) { }

void grid_write(latin_grid square, coord position, int symbol) { }

void print_success(latin_grid square) { }

void init() { }

void gen_cyclic(latin_grid square) {
  int i, j;
  for (i = 1; i < square->size; i++) {
    for (j = 0; j < square->size; j++) {
      CELL(square, i, j) = (CELL(square, (i - 1), j) + 1) % square->size;
    }
  }  
}

void even_gen_first_row(latin_grid square) {
  int i;
  CELL(square, 0, 0) = 0;
  for (i = 1; i < square->size; i++) {
    if (i % 2 == 1) {
      CELL(square, 0, i) = (i >> 1) + 1;
    } else {
      CELL(square, 0, i) = square->size - (i >> 1);
    }
  }
}

void even_gen_first_square(latin_grid square) {
  gen_cyclic(square);
}

void even_gen_second_square(latin_grid square1, latin_grid square2) {
  int i, j;
  for (i = 0; i < (square1->size / 2); i++) {
    for (j = 0; j < square1->size; j++) {
      CELL(square2, i, j) = CELL(square1, (i * 2), j);
    }
  }
  for (j = 0; j < square1->size; j++) {
    CELL(square2, (square1->size / 2), j) =
      CELL(square1, (square1->size - 1), j);
  }
  for (i = 1; i < (square1->size / 2); i++) {
    for (j = 0; j < square1->size; j++) {
      CELL(square2, ((square1->size / 2) + i), j) =
                CELL(square1, ((i * 2) - 1), j);
    }
  }
}

void odd_gen_square1_row1(latin_grid square) {
  int i;
  int m = (square->size + 1) / 2;
  int q = (m + 1) >> 1;
  CELL(square, 0, 0) = 0;
  for (i = 1; i < (square->size - m); i++) {
    CELL(square, 0, (i * 2)) = m + i;
  }
  if (square->size > 1) {
    CELL(square, 0, (square->size - 1)) = q;
  }
  for (i = 0; i < (m - 1); i++) {
    if ((m - i) > q) {
      CELL(square, 0, ((i * 2) + 1)) = m - i;
    } else {
      CELL(square, 0, ((i * 2) + 1)) = (m - i) - 1;
    }
  }
}

void odd_gen_square2_row1(latin_grid square1, latin_grid square2) {
  int i;
  for (i = 0; i < square1->size; i++) {
    CELL(square2, 0, i) = (CELL(square1, 0, i) * 2) % square1->size;
  }
}

latin_grid square1, square2;
int a, b, c, d, e;

void loop(size) {
  printf("-- %d --\n", size);
  square1 = new_latin_grid(size);
  square2 = new_latin_grid(size);
  
  if (size % 2 == 0) {
    even_gen_first_row(square1);
    even_gen_first_square(square1);
    even_gen_second_square(square1, square2);
  } else {
    odd_gen_square1_row1(square1);
    odd_gen_square2_row1(square1, square2);
    gen_cyclic(square1);
    gen_cyclic(square2);
  }
  
  print_latin_grid(square1);
  print_latin_grid(square2);

  a = orthogonality_repeats(square1, square2);
  b = row_completeness_repeats(square1);
  c = row_completeness_repeats(square2);
  d = diagonal_repeats(square1, square2);
  e = diagonal_repeats(square2, square1);

  
  printf("a:%d, b:%d, c:%d, d:%d, e:%d\n",
	 a, b, c, d, e);
  
  printf("2a+b+c:%d, a+b+d:%d, a+c+e:%d, a+b+c+d+e:%d\n",
	 (2 * a) + b + c,
	 a + b + d,
	 a + c + e,
	 a + b + c + d + e);
}

void finish() {}
