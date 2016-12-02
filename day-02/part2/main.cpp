#include <fstream>
#include <vector>

std::vector< std::vector<char> > Map = {
  { ' ', ' ', '1', ' ', ' ' },
  { ' ', '2', '3', '4', ' ' },
  { '5', '6', '7', '8', '9' },
  { ' ', 'A', 'B', 'C', ' ' },
  { ' ', ' ', 'D', ' ', ' ' }
};

struct coordinate {
  int x;
  int y;
};

std::vector<std::string> readfile(std::string filename)
{
  std::vector<std::string> full_content;
  std::string line;
  std::fstream fin(filename, std::fstream::in);

  while (fin >> line) {
    full_content.push_back(line);
  }
  return full_content;
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <filename>\n", argv[0]);
    exit(0);
  }
  std::vector<std::string> lines = readfile(argv[1]);

  coordinate position = {0, 2}; // start from 5

  for(
      std::vector<std::string>::iterator line = lines.begin();
      line != lines.end();
      ++line
  ) {
    for(char& c : *line) {
      int move_x = 0, move_y = 0;
      switch(c) {
        case 'U':
          move_y = -1;
          break;
        case 'L':
          move_x = -1;
          break;
        case 'D':
          move_y = 1;
          break;
        case 'R':
          move_x = 1;
          break;
      }

      if (position.y + move_y < 0) { move_y = 0; }
      if (position.y + move_y > Map.size() - 1) { move_y = 0; }

      if (position.x + move_x < 0) { move_x = 0; }
      if (position.x + move_x > Map[position.y].size() - 1) { move_x = 0; }

      if (Map[position.y + move_y][position.x + move_x] == ' ') { move_y = 0; move_x = 0; }

      position.x += move_x;
      position.y += move_y;
    }
    printf("%c", Map[position.y][position.x]);
  }

  return 0;
}
