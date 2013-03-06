// linked list of squares
typedef struct _square_list {
  latin_grid square;
  struct _square_list* next;
}* square_list;

square_list square_list_push(square_list list, latin_grid square) {
  square_list result = malloc(sizeof(square_list));
  result->square = square;
  result->next = list;
  return result;
}

square_list file2squares(char* filename) {
  square_list result = NULL;
  latin_grid this_square;
  FILE *f = fopen(filename, "r");
  char line[20];
  int size;
  int squares;
  sscanf(fgets(line, 20, f), "%d %d %d", &size, &size, &squares);
  
  int i, j, k;
  for (i = 0; i < squares; i++) {
	this_square = new_latin_grid(size);
	for (j = 0; j < size; j++) {
	  fgets(line, 20, f);
	  for (k = 0; k < size; k++) {
		CELL(this_square, j, k) = line[k] - 'a';
	  }
	}
	fgets(line, 20, f);
	result = square_list_push(result, this_square);
  }

  fclose(f);

  return result;
}