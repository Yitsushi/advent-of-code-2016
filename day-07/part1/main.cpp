#include <fstream>
#include <vector>
#include <string>
#include <regex>

std::vector<std::string> readfile(std::string filename)
{
  std::vector<std::string> sequences;

  std::string line;
  std::regex match_abba("(.)(.)\\2\\1");
  std::regex match_inside_brackets("\\[[^\\]]*(.)(.)\\2\\1[^\\]]*\\]");
  std::smatch match;

  std::regex_iterator<std::string::iterator> r_end;
  std::fstream fin(filename, std::fstream::in);
  while (fin >> line) {
    if (!regex_search(line, match_abba)) { continue; }

    if (regex_search(line, match, match_inside_brackets)) {
      if (match[1] != match[2]) { continue; }
    }

    for(int i = 0; i < line.length() - 3; ++i) {
      if (line[i] == line[i+3] && line[i+1] == line[i+2] && line[i] != line[i+1]) {
        sequences.push_back(line);
        break;
      }
    }
  }
  return sequences;
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input_file>\n", argv[0]);
    return 1;
  }

  std::vector<std::string> lines = readfile(argv[1]);

  printf("Lines: %lu\n", lines.size());

  return 0;
}
