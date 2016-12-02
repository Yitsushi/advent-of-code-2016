#include <fstream>
#include <vector>
#include <math.h>

#define PI atan(1)*4

struct movement {
  char direction;
  uint unit;
};

struct facing {
  int x;
  int y;
};

struct coordinate {
  int x;
  int y;
  facing actual_facing;
};

std::vector<movement> readfile(std::string filename)
{
  std::vector<movement> instructions;

  std::string command;
  char direction;
  int unit;

  std::fstream fin(filename, std::fstream::in);
  while (fin >> command) {
    sscanf(command.c_str(), "%c%d", &direction, &unit);
    movement current;
    current.direction = direction;
    current.unit = unit;

    instructions.push_back(current);
  }

  return instructions;
}

void turn(facing* actual, char rotate)
{
  int degree = 90;
  if (rotate == 'R') {
    degree *= -1;
  }

  int new_x = (actual->x * cos(degree * PI/180)) - (actual->y * sin(degree * PI/180));
  int new_y = (actual->x * sin(degree * PI/180)) + (actual->y * cos(degree * PI/180));

  actual->x = new_x;
  actual->y = new_y;
}

void step(coordinate* location, int units)
{
  location->x += location->actual_facing.x * units;
  location->y += location->actual_facing.y * units;
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <filename>\n", argv[0]);
    exit(0);
  }
  std::vector<movement> instructions = readfile(argv[1]);
  coordinate location = {0, 0, {0, 1}};   // Origin, facing north

  printf(
      "Initial state: facing(%d, %d) location(%d, %d)\n",
      location.actual_facing.x, location.actual_facing.y,
      location.x, location.y
  );
  for(
      std::vector<movement>::iterator m = instructions.begin();
      m != instructions.end();
      ++m
  ) {
    turn(&(location.actual_facing), m->direction);
    step(&location, m->unit);
    printf(
        "%c => %d facing(%d, %d) location(%d, %d)\n",
        m->direction, m->unit,
        location.actual_facing.x, location.actual_facing.y,
        location.x, location.y
    );
  }

  printf("Actual position: (%d;%d)\n", location.x, location.y);
  printf("Taxi-distance: %d\n", abs(location.x) + abs(location.y));

  return 0;
}

