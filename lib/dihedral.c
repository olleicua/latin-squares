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