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

struct segment {
  int x1;
  int y1;
  int x2;
  int y2;
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

segment step(coordinate* location, int units)
{
  segment current_segment = {location->x, location->y};
  int new_x = location->x + location->actual_facing.x * units;
  int new_y = location->y + location->actual_facing.y * units;

  current_segment.x2 = new_x;
  current_segment.y2 = new_y;

  location->x = new_x;
  location->y = new_y;

  return current_segment;
}

bool inRange(int n, int x1, int x2)
{
  return std::min(x1, x2) <= n && std::max(x1, x2) >= n;
}

bool historicalCrossCheck(std::vector<segment>* history, segment* current, coordinate* crossing)
{
  bool foundAtLeastOne = false;
  for(
      std::vector<segment>::iterator s = history->begin();
      s != history->end();
      ++s
  ) {
    // previous one, skip it
    if (s->x2 == current->x1 && s->y2 == current->y1) {
      continue;
    }

    if (inRange(current->x1, s->x1, s->x2) && inRange(s->y1, current->y1, current->y2)) {
      if (foundAtLeastOne && (s->y1 - current->y1) > (crossing->y - current->y1)) {
        continue;
      }
      crossing->x = current->x1;
      crossing->y = s->y1;
      foundAtLeastOne = true;
    }

    if (inRange(current->y1, s->y1, s->y2) && inRange(s->x1, current->x1, current->x2)) {
      if (foundAtLeastOne && (s->x1 - current->x1) > (crossing->x - current->x1)) {
        continue;
      }
      crossing->x = current->y1;
      crossing->y = s->x1;
      foundAtLeastOne = true;
    }
  }

  return foundAtLeastOne;
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <filename>\n", argv[0]);
    exit(0);
  }
  std::vector<movement> instructions = readfile(argv[1]);
  coordinate location = {0, 0, {0, 1}};   // Origin, facing north
  std::vector<segment> history;
  coordinate crossing = {};

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
    segment current_segment = step(&location, m->unit);
    if (historicalCrossCheck(&history, &current_segment, &crossing)) {
      printf(
          "Bang! (%d, %d) Taxi-distance: %d\n",
          crossing.x, crossing.y,
          abs(crossing.x) + abs(crossing.y)
      );
      break;
    }
    history.push_back(current_segment);
//    printf(
//        "%c => %d facing(%d, %d) location(%d, %d) segment[(%d,%d),(%d,%d)]\n",
//        m->direction, m->unit,
//        location.actual_facing.x, location.actual_facing.y,
//        location.x, location.y,
//        current_segment.x1, current_segment.y1,
//        current_segment.x2, current_segment.y2
//    );
  }

  //printf("Actual position: (%d;%d)\n", location.x, location.y);
  //printf("Taxi-distance: %d\n", abs(location.x) + abs(location.y));

  return 0;
}

