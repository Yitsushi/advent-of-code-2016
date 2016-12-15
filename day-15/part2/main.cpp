#include <stdio.h>
#include <vector>
#include <fstream>

struct Disk {
  int number;
  int max_positions;
  int shift_value;

  bool zero_at_time(int i);
};

bool Disk::zero_at_time(int i)
{
  return (((i + number) + shift_value) % max_positions) == 0;
}

std::vector<Disk> readfile(std::string filename)
{
  std::vector<Disk> disks;

  std::string line;
  std::fstream fin(filename, std::fstream::in);
  while (std::getline(fin, line)) {
    int disk_number, max_positions, position_at_zero;

    sscanf(
        line.c_str(), "Disc #%d has %d positions; at time=0, it is at position %d.",
        &disk_number, &max_positions, &position_at_zero
    );

    disks.push_back(Disk{disk_number, max_positions, position_at_zero});
  }

  return disks;
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input_file>\n", argv[0]);
    return 1;
  }

  std::vector<Disk> disks = readfile(argv[1]);

  // add extra disk
  disks.push_back(Disk{static_cast<int>(disks.size() + 1), 11, 0});

  for(int t = 0; t >= 0; ++t) {
    if (std::all_of(disks.cbegin(), disks.cend(), [t](Disk d){ return d.zero_at_time(t); })) { 
      printf("Press the button at T+%d seconds.\n", t);
      break;
    }
  }
  return 0;
}
