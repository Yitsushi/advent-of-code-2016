#include <string>
#include <vector>
#include <fstream>

#define ROWS 400000

#define safe_n(str, pos) (pos < 0 || pos >= str.length() ? true : (str[pos] == '.' ? true : false))
#define is_safe(a, b, c) !((!a && !b && c) || (a && !b && !c) || (!a && b && c) || (a && b && !c))
#define safe_count(str)  static_cast<unsigned int>(std::count(str.begin(), str.end(), '.'))

struct Row
{
  std::string tiles;
  unsigned int safe_tiles;
};

Row readfile(std::string filename)
{
  std::string line;
  std::fstream fin(filename, std::fstream::in);
  std::getline(fin, line);

  return {
    line,
    safe_count(line)
  };
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input_file>\n", argv[0]);
    return 1;
  }

  std::vector<Row> rows;
  rows.push_back(readfile(argv[1]));

  unsigned int total_safe_place = rows[0].safe_tiles;

  for (int i = 0; i < ROWS - 1; ++i)
  {
    Row current = {};
    for (int p = 0; p < rows[i].tiles.length(); ++p) {
      bool left   = safe_n(rows[i].tiles, p - 1),
           center = safe_n(rows[i].tiles, p),
           right  = safe_n(rows[i].tiles, p + 1);

      current.tiles += (is_safe(left, center, right) ? '.' : '^');
    }

    current.safe_tiles = safe_count(current.tiles);
    total_safe_place += current.safe_tiles;
    rows.push_back(current);
  }

  printf("safe tiles: %d\n", total_safe_place);
  return 0;
}
