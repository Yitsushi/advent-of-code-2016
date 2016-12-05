#include <vector>
#include <string>
#include <fstream>
#include <map>

struct Room {
  std::string name;
  std::map<char,int> letters;
  int sector;
  std::string checksum;
};

bool validateRoom(Room* room) {
  std::string generated_checksum = "";

  for(char& c : room->name) {
    if (c == '-') { continue; }

    room->letters[c]++;
  }

  std::vector<std::pair<char, int>> pairs;
  std::copy(
      room->letters.begin(),
      room->letters.end(),
      std::back_inserter(pairs)
  );
  std::sort(
      pairs.begin(),
      pairs.end(),
      [](std::pair<char, int> const & a, std::pair<char, int> const & b)
      {
        return a.second != b.second?  a.second > b.second : a.first < b.first;
      }
  );
  
  for (
      std::vector<std::pair<char,int>>::iterator letter=pairs.begin();
      letter != pairs.end();
      ++letter
  ) {
    generated_checksum += letter->first;
  }

  return generated_checksum.substr(0, 5) == room->checksum;
}

std::vector<Room*> readfile(std::string filename)
{
  std::vector<Room*> full_content;
  std::string line;
  std::fstream fin(filename, std::fstream::in);
  while (fin >> line) {
    char* room_name = nullptr;
    int sector = 0;
    char* checksum = nullptr;

    room_name = (char*)std::malloc(200 * sizeof(char));
    checksum = (char*)std::malloc(5 * sizeof(char));

    sscanf(line.c_str(), "%[^0-9]%d[%5s]", room_name, &sector, checksum);

    Room* room = new Room{};
    room->name = std::string(room_name);
    room->sector = sector;
    room->checksum = std::string(checksum);
    room->name.pop_back();

    if (!validateRoom(room)) {
      delete room;
      continue;
    }

    full_content.push_back(room);

    //printf("%s => %d\n", room->name.c_str(), room->sector);
  }
  return full_content;
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <filename>\n", argv[0]);
    exit(0);
  }
  std::vector<Room*> rooms = readfile(argv[1]);

  unsigned int sectorsum = 0;
  for (
      std::vector<Room*>::iterator room = rooms.begin();
      room != rooms.end();
      ++room
  ) {
    sectorsum += (*room)->sector;
  }

  printf("Sum of Sector Numbers: %d\n", sectorsum);

  return 0;
}
