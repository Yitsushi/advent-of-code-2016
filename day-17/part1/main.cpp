#include <string>
#include <vector>
#include <openssl/md5.h>
#include <climits>

#define KEY "udskfozm"

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

#define door_is_open(hash, door) (hash[door] > 0x61 && hash[door] < 0x67)
#define MIN(a, b) (a < b ? a : b)

unsigned int current_shortest_route = UINT_MAX;

std::string md5hash(const std::string current);

struct Coordinate {
    unsigned int x;
    unsigned int y;

    std::string path;
    bool done;

    std::vector<Coordinate> rush();
};

std::vector<Coordinate> Coordinate::rush()
{
  std::vector<Coordinate> moves;
  std::vector<Coordinate> final_moves;

  if (x > 3 || y > 3) return moves;
  if (path.length() > current_shortest_route) return moves;

  std::string hash = md5hash(KEY + path);

  if (door_is_open(hash, UP))    {
    moves.push_back({x - 1, y, path + 'U'});
  }
  if (door_is_open(hash, DOWN))  {
    moves.push_back({x + 1, y, path + 'D'});
  }
  if (door_is_open(hash, LEFT))  {
    moves.push_back({x, y - 1, path + 'L'});
  }
  if (door_is_open(hash, RIGHT)) {
    moves.push_back({x, y + 1, path + 'R'});
  }

  for (std::vector<Coordinate>::iterator it = moves.begin(); it != moves.end(); ++it) {
    if (it->x == 3 && it->y == 3) {
      it->done = true;
      current_shortest_route = MIN(current_shortest_route, it->path.length());
      final_moves.push_back(*it);
      continue;
    }

    std::vector<Coordinate> possibilities = it->rush();
    for (std::vector<Coordinate>::iterator sit = possibilities.begin(); sit != possibilities.end(); ++sit) {
      if (sit->done == true) { final_moves.push_back(*sit); }
    }
  }

  return final_moves;
}

std::string md5hash(const std::string current)
{
  unsigned char sum[MD5_DIGEST_LENGTH];
  MD5((unsigned char*)current.c_str(), current.length(), sum);

  char out[32];
  for (int n = 0; n < 16; ++n) {
    snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)sum[n]);
  }

  return std::string(out);
}

int main()
{
  Coordinate me = {};

  std::vector<Coordinate> path_list = me.rush();

  //for (std::vector<Coordinate>::iterator it = path_list.begin(); it != path_list.end(); ++it) {
  //  printf("%s\n", it->path.c_str());
  //}

  std::sort(
      path_list.begin(),
      path_list.end(),
      [](Coordinate s1, Coordinate s2) { return s1.path.length() < s2.path.length(); }
  );

  printf("Shortest path: %s\n", path_list.front().path.c_str());
  return 0;
}
