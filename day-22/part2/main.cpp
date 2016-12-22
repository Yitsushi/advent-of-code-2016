#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <unistd.h>

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define DEBUG_WAIT 10000
#define debug_dump(dc) if (dc.DebugMode > 0) { system("clear"); dc.dump({}); usleep(DEBUG_WAIT); }
#define debug_path(dc, path, mul) if (dc.DebugMode > 0) { system("clear"); dc.dump(path); usleep(DEBUG_WAIT * mul); }
#define debug_dump_visited(dc, visited) if (dc.DebugMode > 1) { system("clear"); dc.dump(visited); usleep(DEBUG_WAIT * 2); }

struct Node
{
  int x;
  int y;
  int size;
  int used;

  std::vector<Node*> connected_nodes;

  int avail();
  float used_percentage();
  bool is_blocked();
  bool is_free();

  std::vector<Node*> pathTo(Node* n, std::vector<Node *>);
  void addConnection(Node* n);

  bool operator==(Node n)
  {
    return x == n.x && y == n.y;
  }
};

struct DataCenter
{
  std::map<std::pair<int, int>, Node> nodes;
  int max_x = 0;
  int max_y = 0;
  Node* empty_node;
  Node* access_node;
  Node* target_data_node;

  int DebugMode = 0;

  int move_count = 0;

  void init(std::string filename);
  Node* FindOrCreateNode(int x, int y);
  Node* FindNode(int x, int y);
  void moveData(Node* from, Node* to);
  void dump(std::vector<Node*> visited);
};

// Glob
DataCenter dc;

int Node::avail() { return size - used; }
float Node::used_percentage() { return static_cast<float>(used) / static_cast<float>(size); }

bool Node::is_blocked() { return used_percentage() > 0.9; }
bool Node::is_free() { return used < 1; }

void Node::addConnection(Node* n)
{
  if (std::find(connected_nodes.begin(), connected_nodes.end(), n) == connected_nodes.end()) {
    connected_nodes.insert(connected_nodes.begin(), n);
  }
}

std::vector<Node*> Node::pathTo(Node* n, std::vector<Node*> blacklist)
{
  std::vector<Node*> visited;
  std::vector< std::vector<Node*> > routes = {{this}};

  while (std::find(visited.begin(), visited.end(), n) == visited.end()) {
    std::vector< std::vector<Node*> > current_routes = routes;
    routes.clear();
    for (std::vector<Node*> &r : current_routes) {
      Node* rn = r.back();
      for (Node* nc : rn->connected_nodes) {
        if (nc->is_blocked()) continue;
        if (std::find(blacklist.begin(), blacklist.end(), nc) != blacklist.end()) continue;
        if (std::find(visited.begin(), visited.end(), nc) != visited.end()) continue;

        visited.push_back(nc);
        std::vector<Node*> cr = r;
        cr.push_back(nc);
        routes.push_back(cr);
      }
    }
    debug_dump_visited(dc, visited);
  }

  for (std::vector<Node*> &r : routes) {
    if (std::find(r.begin(), r.end(), n) != r.end()) {
      r.erase(r.begin());
      return r;
    }
  }

  return std::vector<Node*>();
}

Node* DataCenter::FindNode(int x, int y)
{
  std::pair<int, int> coord = {x, y};

  if (nodes.count(coord)) {
    return &(nodes[coord]);
  }

  return nullptr;
}

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

void DataCenter::moveData(Node* from, Node* to)
{
  to->used = from->used;
  from->used = 0;
  empty_node = from;

  if (from == target_data_node) {
    target_data_node = to;
  }
  move_count++;
}

void DataCenter::dump(std::vector<Node*> visited)
{
  std::pair<int, int> position = {0, 0};
  for (int x = 0; x <= max_x; ++x) {
    position.first = x;
    printf("%2d  ", x);
    for (int y = 0; y <= max_y; ++y) {
      position.second = y;
      char display = '.';

      if (nodes[position].is_blocked()) display = '#';
      if (nodes[position].is_free()) display = '_';

      if (nodes[position] == *access_node) display = 'T';
      if (nodes[position] == *target_data_node) display = 'F';

      if (std::find(visited.begin(), visited.end(), &(nodes[position])) != visited.end()) {
        printf(COLOR_RED "%c" COLOR_RESET, display);
      } else {
        printf(COLOR_RESET "%c" COLOR_RESET, display);
      }
    }
    printf("\n");
  }
  printf("\nSteps: %d\n", move_count);
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

    if (node->is_free()) empty_node = node;
  }

  // Create connections
  for (auto &n : nodes) {
    Node* nc;
    nc = FindNode(n.second.x + 1, n.second.y    );
    if (nc != nullptr) n.second.addConnection(nc);
    nc = FindNode(n.second.x - 1, n.second.y    );
    if (nc != nullptr) n.second.addConnection(nc);
    nc = FindNode(n.second.x    , n.second.y + 1);
    if (nc != nullptr) n.second.addConnection(nc);
    nc = FindNode(n.second.x    , n.second.y - 1);
    if (nc != nullptr) n.second.addConnection(nc);
  }
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input>\n", argv[1]);
  }

  dc.init(argv[1]);

  if (argc > 2) {
    if (strcmp(argv[2], "-v") == 0) dc.DebugMode = 1;
    if (strcmp(argv[2], "-vv") == 0) dc.DebugMode = 2;
  }

  dc.access_node = dc.FindOrCreateNode(0, 0);
  dc.target_data_node = dc.FindNode(dc.max_x - 4, 5);

  std::vector<Node*> path = dc.empty_node->pathTo(dc.target_data_node, {});
  debug_path(dc, path, 100);

  for (Node* n : path) {
    dc.moveData(n, dc.empty_node);
    debug_dump(dc);
  }

  path = dc.target_data_node->pathTo(dc.access_node, {});
  debug_path(dc, path, 100);

  for (Node* n : path) {
    std::vector<Node*> subpath = dc.empty_node->pathTo(n, {dc.target_data_node});
    debug_path(dc, subpath, 20);
    for (Node* sn : subpath) {
      dc.moveData(sn, dc.empty_node);
      debug_dump(dc);
    }
    dc.moveData(dc.target_data_node, dc.empty_node);
    debug_dump(dc);
  }

  printf("Final step count: %d\n", dc.move_count);

  return 0;
}
