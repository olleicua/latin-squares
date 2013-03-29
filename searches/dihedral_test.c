#include <stdlib.h>
#include <stdio.h>
#include "../lib/dihedral.c"

int main(int argc, char *argv[]) {
  int size;
  if (argc > 1) {
	size = atoi(argv[1]);
	if (size & 1) {
	  size--;
	}
  } else {
	size = 6;
  }

  int i, j;
  printf("     ");
  for (i = 0; i < size; i++) {
	printf("%3d ", i);
  }
  printf("\n\n");
  for (i = 0; i < size; i++) {
	printf("%2d   ", i);
	for (j = 0; j < size; j++) {
	  printf("%3d ", dihedral_divide(i, j, size));
	}
	printf("\n");
  }

  printf("\n     ");
  for (i = 0; i < size; i++) {
	printf("%3d ", i);
  }
  printf("\n\n");
  for (i = 0; i < size; i++) {
	printf("%2d   ", i);
	for (j = 0; j < size; j++) {
	  printf("%3d ", dihedral_multiply(i, j, size));
	}
	printf("\n");
  }
  return 0;
}
