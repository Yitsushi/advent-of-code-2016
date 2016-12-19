#include <stdio.h>
#include <cmath>

#define NUMBER_OF_ELVES 3017957

int main()
{
  int largest = pow(2, floor(log(NUMBER_OF_ELVES)/log(2)));

  printf("%d\n", 1 + (2 * (NUMBER_OF_ELVES - largest)));
  return 0;
}
