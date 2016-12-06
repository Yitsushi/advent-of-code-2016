#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> readfile(std::string filename)
{
  std::vector<std::string> sequences;
  std::string line;

  std::fstream fin(filename, std::fstream::in);
  while (fin >> line) {
    for (int i = 0; i < line.length(); ++i) {
      if (sequences.size() <= i) {
        sequences.push_back("");
      }
      sequences[i] += (char)line[i];
    }
  }
  return sequences;
}

char mostFrequentCharacter(const std::string str) {
  int freq[26];
  // fill with zeros :D to be sure
  for (int i = 0; i < 26; ++i) {
    freq[i] = 0;
  }

  // Count them all
  for (int i = 0; i < str.length(); ++i) {
    freq[((char)str[i]) - 'a']++;
  }

  char most_frequent = 'a';
  int min = 9999;
  for(int i = 0; i < 26; ++i) {
    if (freq[i] < min && freq[i] > 0) {
      min = freq[i];
      most_frequent = (i + 'a');
    }
  }

  return most_frequent;
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input_file>\n", argv[0]);
    return 1;
  }

  std::vector<std::string> messages = readfile(argv[1]);

  for (
      std::vector<std::string>::iterator message = messages.begin();
      message != messages.end();
      ++message
  ) {
    printf("%c", mostFrequentCharacter(*message));
  }
  printf("\n");

  return 0;
}
