// divide two members of the cyclic group of the specified size
int cyclic_divide(int a, int b, int size) {
  return (size + a - b) % size ;
}

// multiply two members of the cyclic group of the specified size
int cyclic_multiply(int a, int b, int size) {
  return (a + b) % size;
}

// calculate the greatest common divisor.  b is assumed to be smaller than a.
int gcd(int a, int b) {
  if (b == 0) {
	return 0;
  }
  if (a % b == 0) {
	return b;
  }
  return gcd(b, a % b);
}

// determine whether the given element is equivalent up to some permutation of
// the group labels, to 1
bool cyclic_equivalent_to_1(int x, int size) {
  return (x < size) && gcd(size, x) == 1;
}
