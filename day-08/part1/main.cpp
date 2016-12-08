#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>
#include <functional>

#define ROWS 6
#define COLUMNS 50

typedef std::vector< std::string > display_t;

struct Command {
  std::string name;
  std::string arguments;

  display_t* display;

  void rect();
  void rotate();
  void execute();
};

void Command::execute()
{
  if (name == "rect") { rect(); }
  else if (name == "rotate") { rotate(); }
}

void Command::rect()
{
  int x, y;
  sscanf(arguments.c_str(), "%dx%d", &x, &y);

  for (--y; y >= 0; --y) {
    (*display)[y].replace((*display)[y].begin(), (*display)[y].begin()+x, x, '#');
  }
}

void Command::rotate()
{
  char direction;
  int coord, value;

  sscanf(arguments.c_str(), "%*s %c=%d by %d", &direction, &coord, &value);

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

std::vector<Command*> readfile(std::string filename, display_t* display)
{
  std::vector<Command*> sequences;

  std::string line;
  std::fstream fin(filename, std::fstream::in);
  while (std::getline(fin, line)) {
    Command* command = new Command();

    command->name = line.substr(0, line.find(' '));
    command->arguments = line.substr(line.find(' ') + 1);
    command->display = display;

    sequences.push_back(command);
  }

  return sequences;
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input_file>\n", argv[0]);
    return 1;
  }

  display_t display(ROWS, std::string(COLUMNS, ' '));

  std::vector<Command*> commands = readfile(argv[1], &display);

  std::for_each(commands.begin(), commands.end(), [](Command* command){ command->execute(); });
  std::copy(display.begin(), display.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

  return 0;
}
