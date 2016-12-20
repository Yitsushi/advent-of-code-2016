#include <stdio.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdint.h>

typedef uint32_t ip_number;

struct Range
{
  ip_number min;
  ip_number max;

  bool operator<(const Range other) const
  {
    return min < other.min;
  }
};

std::vector<Range> readfile(std::string filename)
{
  std::vector<Range> ranges;
  std::fstream fin(filename, std::fstream::in);
  std::string line;
  while (std::getline(fin, line)) {
    ip_number min, max;
    sscanf(line.c_str(), "%u-%u", &min, &max);
    ranges.push_back(Range{min, max});
  }

  return ranges;
}

int main(int argc, const char *argv[])
{
   if (argc < 2) {
    printf("%s <filename>\n", argv[0]);
    exit(0);
  }
  std::vector<Range> ranges = readfile(argv[1]);
  std::sort(ranges.begin(), ranges.end());

  // normalize
  for (std::vector<Range>::iterator it = ranges.begin(); it != (ranges.end() - 1); ++it) {
    while (it->max >= ((it+1)->min) - 1) {
      if (it->max < (it+1)->max) {
        it->max = (it+1)->max;
      }
      ranges.erase(it + 1);

      if (it+1 == ranges.end()) { break; }
    }
    if (it+1 == ranges.end()) { break; }
  }

  uint32_t number_or_blocked_ips = 0;

  for (std::vector<Range>::iterator it = ranges.begin(); it != ranges.end(); ++it) {
    number_or_blocked_ips += it->max - it->min + 1; // because inclusive
  }

  // UINT32_MAX => max value can store => +1 means how many possible number are in there
  printf("%u\n", UINT32_MAX + 1 - number_or_blocked_ips);

  return 0;
}
