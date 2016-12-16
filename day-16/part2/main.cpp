#include <string>
#include <algorithm>

#define INITIAL_SATE "01111010110010011"
#define DISK_LENGTH 35651584

#define swap10(ch) ch = (ch == '0' ? '1' : '0')
#define match_pair(str, i) ((str[i*2] == str[(i*2)+1]) ? '1' : '0')

std::string dragon(std::string str)
{
  std::reverse(str.begin(), str.end());
  std::for_each(str.begin(), str.end(), [](char& ch) { swap10(ch); });

  return str;
}

std::string checksum(std::string str)
{
  int len = str.length();
  std::string sum = "";
  for (int i = 0; i < len / 2; ++i) sum += match_pair(str, i);

  if (sum.length() % 2 == 0) return checksum(sum);

  return sum;
}

int main()
{
  std::string disk_data = INITIAL_SATE;
  std::string sum = "";

  while (disk_data.length() < DISK_LENGTH) {
    disk_data += '0' + dragon(disk_data);
  }
  disk_data = disk_data.substr(0, DISK_LENGTH);
  sum = checksum(disk_data);
  printf("Checksum:  %s\n", sum.c_str());
  return 0;
}
