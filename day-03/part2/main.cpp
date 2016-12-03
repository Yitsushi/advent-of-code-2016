#include <fstream>
#include <vector>

#define is_triangle(a, b, c) (a + b > c && a + c > b && b + c > a)

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <filename>\n", argv[0]);
    exit(0);
  }

  std::fstream fin(argv[1], std::fstream::in);

  int a1, a2, a3,
      b1, b2, b3,
      c1, c2, c3;

  while (fin
      >> a1 >> a2 >> a3
      >> b1 >> b2 >> b3
      >> c1 >> c2 >> c3
  ) {
    if (is_triangle(a1, b1, c1)) printf("%d;%d;%d\n", a1, b1, c1);
    if (is_triangle(a2, b2, c2)) printf("%d;%d;%d\n", a2, b2, c2);
    if (is_triangle(a3, b3, c3)) printf("%d;%d;%d\n", a3, b3, c3);
  }

  return 0;
}

