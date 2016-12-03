#include <fstream>

#define is_triangle(a, b, c) (a + b > c && a + c > b && b + c > a)

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <filename>\n", argv[0]);
    exit(0);
  }

  std::fstream fin(argv[1], std::fstream::in);

  int a, b, c;

  while (fin >> a >> b >> c) {
    if (is_triangle(a, b, c)) printf("%d;%d;%d\n", a, b, c);
  }
  return 0;
}

