#include <string>
#include <fstream>

typedef unsigned long long verylong;

verylong calculate_decompressed_size(std::string subsection)
{
  verylong size = 0;

  for(std::string::iterator p = subsection.begin(); p != subsection.end(); ++p) {
    int position = p - subsection.begin();

    if (*p == '(') {
      ++p; ++position;

      int end_of_instruction = subsection.find(')', position);

      std::string instruction = subsection.substr(position, end_of_instruction - position);
      position += instruction.length() + 1;

      unsigned int number_or_characters = 0, repeat_number = 0;
      sscanf(instruction.c_str(), "%dx%d", &number_or_characters, &repeat_number);

      std::string section = subsection.substr(position, number_or_characters);
      size += calculate_decompressed_size(section) * repeat_number;
      p += number_or_characters + instruction.length();

      continue;
    }

    int end_of_data = subsection.find('(', position);
    int length = 0;
    if (end_of_data != std::string::npos) { length = end_of_data - position - 1; }
    else { length = subsection.length() - position; }

    size += length;
    p += length - 1;
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
