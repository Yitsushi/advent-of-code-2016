#include <stdio.h>
#include <vector>
#include <fstream>

#define dzero(t, n, s, m) ((((t + n) + s) % m) == 0)
// Disks with initial values
#define d1(t) dzero(t, 1, 15, 17)
#define d2(t) dzero(t, 2, 2, 3)
#define d3(t) dzero(t, 3, 4, 19)
#define d4(t) dzero(t, 4, 2, 13)
#define d5(t) dzero(t, 5, 2, 7)
#define d6(t) dzero(t, 6, 0, 5)
#define d7(t) dzero(t, 7, 0, 11)

int main(int argc, const char *argv[])
{
  for(int i = 0; i >= 0; ++i) {
    if (d1(i) && d2(i) && d3(i) && d4(i) && d5(i) && d6(i) && d7(i)) {
      printf("%d\n", i);
      break;
    }
  }
  return 0;
}
