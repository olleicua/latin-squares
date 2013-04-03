/* members of dihedral groups are represented here by integers with the least
 * significant bit representing v and the remaining bits representing u
 * bitshifted the the left one place.  So for example:
 *    u2v1 is 5 and u3 is 6
 */

// divide two members of the dihedral group of the specified size
int dihedral_divide(int a, int b, int size) {
  int u_1 = a >> 1, v_1 = a & 1,
	u_2 = b >> 1, v_2 = b & 1,
	u_3, v_3;
  if (v_1) {
	u_3 = (size + u_1 - u_2) % (size >> 1);
	v_3 = ! v_2;
  } else {
	u_3 = (size + u_2 - u_1) % (size >> 1);
	v_3 = v_2;
  }
  return (u_3 << 1) | v_3;
}

// multiply two members of the dihedral group of the specified size
int dihedral_multiply(int a, int b, int size) {
  int u_1 = a >> 1, v_1 = a & 1,
	u_2 = b >> 1, v_2 = b & 1,
	u_3, v_3;
  if (v_1) {
	u_3 = (size + u_1 - u_2) % (size >> 1);
	v_3 = ! v_2;
  } else {
	u_3 = (u_1 + u_2) % (size >> 1);
	v_3 = v_2;
  }
  return (u_3 << 1) | v_3;
}

// calculate the greatest common divisor.  b is assumed to be smaller than a.
int gcd(int a, int b) {
  if (a % b == 0) {
	return b;
  }
  return gcd(b, a % b);
}

// determine whether the given element is equivalent up to some permutation of
// the group labels, to 1 (v1)
bool dihedral_equivalent_to_v(int x, int size) {
  return (x < size) && (x & 1);
}

// determine whether the given element is equivalent up to some permutation of
// the group labels, to 2 (u1)
bool dihedral_equivalent_to_u(int x, int size) {
  if (x & 1) {
	return false;
  }
  return (x < size) && gcd(x >> 1, size >> 1) == 1;
}
