#include <string>
#include <fstream>

typedef unsigned long long verylong;

verylong calculate_decompressed_size(std::string subsection)
{
  verylong size = 0;

  std::string instruction;
  for(std::string::iterator p = subsection.begin(); p != subsection.end(); ++p) {
    int position = p - subsection.begin();

    if (*p == '(') {
      ++p; ++position;

      int end_of_instruction = subsection.find(')', position);

      std::string instruction = subsection.substr(position, end_of_instruction - position);

      unsigned int number_or_characters = 0, repeat_number = 0;
      sscanf(instruction.c_str(), "%dx%d", &number_or_characters, &repeat_number);

      size += number_or_characters * repeat_number;
      p += number_or_characters + instruction.length() - 1;

      continue;
    }

    int end_of_data = subsection.find('(', position);
    int length = 0;
    if (end_of_data != std::string::npos) { length = end_of_data - position - 1; }
    else { length = subsection.length() - position - 1; }

    size += length;
    p += length;
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
