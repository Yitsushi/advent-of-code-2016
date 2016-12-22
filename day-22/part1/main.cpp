#include <fstream>
#include <string>
#include <vector>
#include <map>

struct Node
{
  int x;
  int y;
  int size;
  int used;

  std::vector<Node> connected_nodes;

  int avail();

  bool operator==(Node n)
  {
    return x == n.x && y == n.y;
  }
};

int Node::avail()
{
  return size - used;
}

struct DataCenter
{
  std::map<std::pair<int, int>, Node> nodes;
  int max_x = 0;
  int max_y = 0;

  void init(std::string filename);
  Node* FindOrCreateNode(int x, int y);
};

Node* DataCenter::FindOrCreateNode(int x, int y)
{
  std::pair<int, int> coord = {x, y};

  if (nodes.count(coord)) {
    return &(nodes[coord]);
  }

  max_x = x > max_x ? x : max_x;
  max_y = y > max_y ? x : max_y;

  Node n = Node{x, y};
  nodes[coord] = n;

  return &(nodes[coord]);
}

void DataCenter::init(std::string filename)
{
  std::string line;
  std::fstream fin(filename, std::fstream::in);
  while (std::getline(fin, line)) {
    if (line[0] != '/') { continue; }

    int x = 0, y = 0, size = 0, used = 0;

    sscanf(
        line.c_str(), "/dev/grid/node-x%d-y%d %dT %dT %*dT %*d%%",
        &x, &y, &size, &used
    );

    Node* node = FindOrCreateNode(x, y);
    node->size = size;
    node->used = used;
  }
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input>\n", argv[1]);
  }

  DataCenter dc = {};
  dc.init(argv[1]);

  int counter = 0;
  for (auto &n : dc.nodes) {
    for (auto &vn : dc.nodes) {
      if (n.second == vn.second) { continue; }
      if (n.second.used == 0) { continue; }
      if (n.second.used <= vn.second.avail()) counter++;
    }
  }

  printf("Pairs: %d\n", counter);

  return 0;
}
