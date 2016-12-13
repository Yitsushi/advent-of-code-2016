#include <stdio.h>
#include <vector>

#define FAV_NUM 1358
#define TARGET_X 31
#define TARGET_Y 39

struct field;

std::vector<field> visited_cubes;

bool isWall(unsigned int x, unsigned int y)
{
  unsigned int sum = (x*x) + (3*x) + (2*x*y) + y + (y*y) + FAV_NUM;

  sum -= ((sum >> 1) & 0x55555555);
  sum = (sum & 0x33333333) + ((sum >> 2) & 0x33333333);
  sum = (((sum + (sum >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

  return ((sum % 2) != 0);
}

struct field {
  unsigned int x;
  unsigned int y;

  std::vector<field> neighbors();

  bool operator==(const field& other) const
  {
    return (x == other.x && y == other.y);
  }
};

bool visited(field cube)
{
  return std::find(visited_cubes.begin(), visited_cubes.end(), cube) != visited_cubes.end();
}

bool visited_cord(unsigned int x, unsigned int y)
{
  field pos = {x, y};
  return visited(pos);
}

std::vector<field> field::neighbors()
{
  std::vector<field> fields;

  if (!isWall(x + 1, y) && !visited_cord(x + 1, y)) fields.push_back({x+1, y});
  if (!isWall(x, y + 1) && !visited_cord(x, y + 1)) fields.push_back({x, y+1});
  if (x > 0 && !isWall(x - 1, y) && !visited_cord(x - 1, y)) fields.push_back({x-1, y});
  if (y > 0 && !isWall(x, y - 1) && !visited_cord(x, y - 1)) fields.push_back({x, y-1});

  return fields;
}

std::vector<field> move(std::vector<field> neighbors)
{
  std::vector<field> next_round;

  for (std::vector<field>::iterator it = neighbors.begin(); it != neighbors.end(); ++it) {
    // next_round.erase(std::remove(next_round.begin(), next_round.end(), *it), next_round.end());
    visited_cubes.push_back(*it);
    std::vector<field> next_neighbors = it->neighbors();

    for (field& f : next_neighbors) {
      if (std::find(next_round.begin(), next_round.end(), f) == next_round.end()) {
        next_round.push_back(f);
      }
    }
  }

  return next_round;
}

int main()
{
  int counter = 0;
  field target_cube = {TARGET_X, TARGET_Y};

  std::vector<field> neighbors;
  neighbors.push_back({1, 1});

  for (counter = 0; !visited(target_cube); ++counter) {
    neighbors = move(neighbors);
  }

  printf("Steps: %d\n", counter - 1);

  return 0;
}
