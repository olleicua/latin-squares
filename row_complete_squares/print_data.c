#include "../lib/squares.c"
#include "../lib/read_row_complete.c"

void grid_write(latin_grid square, coord position, int symbol) {}

int main(int argc, char *argv[]) {
  
  if (argc <= 1) {
    printf("No order specified\n");
    return 1;
  }
  
  square_list squares = file2squares(argv[1]);
  square_list this_square;
  
  for (this_square = squares; this_square != NULL; this_square = this_square->next) {
    print_latin_grid(this_square->square);
  }
  
  return 0;
}
