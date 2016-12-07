#include <fstream>
#include <vector>
#include <string>
#include <regex>

std::vector<std::string> readfile(std::string filename)
{
  std::vector<std::string> sequences;

  std::string line;
  std::regex match_aba("(.)(.)\\1");
  std::regex match_inside_brackets("\\[[^\\]]*(.)(.)\\1[^\\]]*\\]");

  std::fstream fin(filename, std::fstream::in);
  while (fin >> line) {
    std::smatch match;
    std::smatch match_in;
    // skip if no potential aba
    if (!regex_search(line, match_aba)) { continue; }

    // skip if no potential bab
    if (!regex_search(line, match_inside_brackets)) { continue; }

    std::vector<std::string> abas;
    bool bracket_opened = false;
    for(int i = 0; i < line.length() - 2; ++i) {
      // jump if we are hit a bracket
      if (line[i+1] == '[' || line[i+1] == ']') { continue; }
      if (line[i+2] == '[' || line[i+2] == ']') { ++i; continue; }

      if (line[i] == '[') { bracket_opened = true; continue; }
      if (line[i] == ']') { bracket_opened = false; continue; }

      if (bracket_opened) { continue; }

      if (line[i] == line[i+2] && line[i] != line[i+1]) {
        char buffer[4];
        line.copy(buffer, 3, i);
        buffer[3] = 0x00;

        abas.push_back(buffer);
      }
    }

    if (abas.size() < 1) { continue; }

    bracket_opened = false;
    bool found = false;
    for(int i = 0; i < line.length() - 2; ++i) {
      // jump if we are hit a bracket
      if (line[i+1] == '[' || line[i+1] == ']') { continue; }
      if (line[i+2] == '[' || line[i+2] == ']') { ++i; continue; }

      if (line[i] == '[') { bracket_opened = true; continue; }
      if (line[i] == ']') { bracket_opened = false; continue; }

      if (!bracket_opened) { continue; }

      if (line[i] == line[i+2] && line[i] != line[i+1] && line[i]) {
        char buffer[4] = { line[i+1], line[i], line[i+1], 0x00 };
        std::string aba = std::string(buffer);
        for (std::string str : abas) {
          if (str == aba) {
            sequences.push_back(line);
            found = true;
            break;
          }
        }
        if (found) { break; }
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
