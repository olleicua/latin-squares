// combine a value with a label to form a symbol
int ga_combine_symbol(int value, int label, int rows, int size) {
  return (value * rows) + label;
}

// determine the first coordinate of a symbol
int ga_value(int symbol, int rows, int size) {
  if (symbol == size) return size;
  return symbol / rows;
}

// determine the second coordinate of a symbol
int ga_label(int symbol, int rows, int size) {
  if (symbol == size) return size;
  return symbol % rows;
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

// determine whether the first coordinate of the symbol is equivalent to 1
bool ga_value_equivalent_to_1(int symbol, int rows, int size) {
  return gcd(symbol / rows, size) == 1;
}

// determine whether the second coordinate of the symbol is equivalent to 1
bool ga_label_equivalent_to_1(int symbol, int rows, int size) {
  return symbol % rows != 0 ;
}

// construct a three part vector [a, b, c] where a is the modular difference
// between the two symbol1's label and c is symbol2's label
int ga_difference(int symbol1, int symbol2, int rows, int size) {
  if (symbol1 == size || symbol2 == size) return size * rows;
  int cycle_size = (size / rows);
  return ((((symbol1 / rows) - (symbol2 / rows) + cycle_size) % cycle_size)
          * rows * rows) +
    ((symbol1 % rows) * rows) +
    (symbol2 % rows);
}

// generate a symbol with the same label and a value equal to the modular sum of
// the symbol's value and the cycle parameter
int ga_cycle(int symbol, int cycle, int rows, int size) {
  if (symbol == size) return size;
  return ga_combine_symbol((symbol / rows + cycle) % (size / rows),
                           symbol % rows,
                           rows,
                           size);
}
