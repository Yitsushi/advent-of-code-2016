#include <string>
#include <fstream>

typedef unsigned long long verylong;

verylong calculate_decompressed_size(std::string subsection)
{
  verylong size = 0;

  std::string instruction;
  bool instruction_section = false;
  for(std::string::iterator p = subsection.begin(); p != subsection.end(); ++p) {
    if (*p == '(') { instruction_section = true; continue; }
    if (*p == ')') {
      instruction_section = false;

      ++p;

      int position = p - subsection.begin();

      unsigned int number_or_characters = 0, repeat_number = 0;
      sscanf(instruction.c_str(), "%dx%d", &number_or_characters, &repeat_number);

      std::string section = subsection.substr(position, number_or_characters);

      size += calculate_decompressed_size(section) * repeat_number;

      p += number_or_characters - 1;

      instruction = "";
      continue;
    }
    if (instruction_section) { instruction += *p; continue; }

    size += 1;
  }

  return size;
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input_file>\n", argv[0]);
    return 1;
  }

  std::string line;
  std::fstream fin(argv[1], std::fstream::in);
  std::getline(fin, line);

  verylong size = calculate_decompressed_size(line);

  printf("length: %llu\n", size);

  return 0;
}
