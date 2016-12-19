#include <stdio.h>
#include <vector>
#include <sys/time.h>

#define NUMBER_OF_ELVES 3017957

struct Elf
{
  unsigned int number;

  Elf* left;
  Elf* right;

  Elf(unsigned int n, Elf* l, Elf* r);

  Elf* kill_and_jump(int n);
  Elf* nthleft(int n);
};

Elf::Elf(unsigned int n, Elf* l, Elf* r) : number(n), left(l), right(r) {}

Elf* Elf::kill_and_jump(int n)
{
  Elf* next = nthleft(n);
  left->right = right;
  right->left = left;

  return next;
}

Elf* Elf::nthleft(int n)
{
  Elf* target = left;
  for (--n; n > 0; --n) {
    target = target->left;
  }
  return target;
}

int main()
{
  Elf* first_elf = new Elf(1, nullptr, nullptr);

  Elf* prev = first_elf;
  for (unsigned int i = 2; i <= NUMBER_OF_ELVES; ++i) {
    Elf* current = new Elf(i, nullptr, prev);
    prev->left = current;
    prev = prev->left;
  }
  prev->left = first_elf;
  first_elf->right = prev;

  // number of elves alive
  unsigned int alive = NUMBER_OF_ELVES;
  Elf* c_elf = first_elf->nthleft(alive / 2);

  while (--alive > 0) c_elf = c_elf->kill_and_jump(2 - (alive % 2));

  printf("Final: %d\n", c_elf->number);

  return 0;
}
