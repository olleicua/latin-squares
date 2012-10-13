typedef struct _list {
  int* list;
  int size;
  int max_size;
}* list;

typedef struct _coord {
  int row;
  int col;
}* coord;

typedef struct _latin_grid {
  int** grid;
  int size;
}* latin_grid;

list new_list(int max_size);
list push(list list, int symbol);
latin_grid new_latin_grid(int size);
void print_latin_grid(latin_grid grid);
bool is_latin(latin_grid grid);
bool is_row_complete(latin_grid grid);
bool is_orthogonal(latin_grid grid1, latin_grid grid2);
int row_repeats(latin_grid grid);
int column_repeats(latin_grid grid);
int row_completeness_repeats(latin_grid grid);
int orthogonality_repeats(latin_grid grid1, latin_grid grid2);
int repeats(list list, int space);
int generate_pair_id(int size, int symbol1, int symbol2);
