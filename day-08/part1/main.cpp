#include <string>
#include <vector>
#include <fstream>

#define ROWS 6
#define COLUMNS 50

struct Command {
  std::string name;
  std::string arguments;
};

typedef std::vector< std::string > display_t;

std::vector<Command*> readfile(std::string filename)
{
  std::vector<Command*> sequences;

  std::string line;
  std::fstream fin(filename, std::fstream::in);
  while (std::getline(fin, line)) {
    Command* command = new Command();

    command->name = line.substr(0, line.find(' '));
    command->arguments = line.substr(line.find(' ') + 1);

    sequences.push_back(command);
  }

  return sequences;
}

void rect(display_t* display, std::string arguments)
{
  int x, y;
  sscanf(arguments.c_str(), "%dx%d", &x, &y);

  x--; y--;

  printf(" rect -> %s\n", arguments.c_str());
  for (; y >= 0; --y) {
    for (int i = 0; i <= x; ++i) {
      (*display)[y][i] = '#';
    }
  }
}

void rotate(display_t* display, std::string arguments)
{
  char direction;
  int coord, value;

  sscanf(arguments.c_str(), "%*s %c=%d by %d", &direction, &coord, &value);

  printf("%s[%d] -> %d\n", (direction == 'y' ? "row" : "column"), coord, value);
  if (direction == 'y') {
    std::string chop = (*display)[coord].substr(COLUMNS - value);
    (*display)[coord] = chop + (*display)[coord].substr(0, COLUMNS - value);
  } else {
    char column[ROWS];
    for (int i = 0; i < ROWS; ++i) {
      column[i] = (*display)[i].at(coord);
    }
    for (int i = 0; i < ROWS; ++i) {
      (*display)[(i + value) % ROWS][coord] = column[i];
    }
  }
}

void display_me(display_t display)
{
  for (std::string s : display) {
    printf("%s\n", s.c_str());
  }
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input_file>\n", argv[0]);
    return 1;
  }

  std::vector<Command*> commands = readfile(argv[1]);
  display_t display;

  for (int i = 0; i < ROWS; ++i) {
    display.push_back(std::string(COLUMNS, ' '));
  }

  for (Command* command : commands) {
    if (command->name == "rect") { rect(&display, command->arguments); }
    else if (command->name == "rotate") { rotate(&display, command->arguments); }
    display_me(display);
  }


  return 0;
}
