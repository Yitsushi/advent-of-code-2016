#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <unistd.h>

struct Node
{
  int x;
  int y;

  int value;

  std::vector<Node*> connected_nodes;

  bool blocked;
  bool target;

  std::vector<Node*> pathTo(Node* n);
  void addConnection(Node* n);

  void print(std::string message);

  bool operator==(Node n)
  {
    return x == n.x && y == n.y;
  }
};

struct DataCenter
{
  std::map<std::pair<int, int>, Node> nodes;

  Node* current_node;
  std::map<std::pair<int, int>, Node*> target_nodes_by_coord;
  std::vector<Node*> target_nodes;

  void init(std::string filename);
  Node* FindOrCreateNode(int x, int y);
  Node* FindNode(int x, int y);
};

// Glob
DataCenter dc;

void Node::addConnection(Node* n)
{
  if (std::find(connected_nodes.begin(), connected_nodes.end(), n) == connected_nodes.end()) {
    connected_nodes.push_back(n);
    n->addConnection(this);
  }
}

void Node::print(std::string message)
{
  printf("[%3dx%3d]{%c%c} %s\n", x, y, (blocked?'B':' '), (target?'T':' '), message.c_str());
}

std::vector<Node*> Node::pathTo(Node* n)
{
  std::vector<Node*> visited;
  std::vector< std::vector<Node*> > routes = {{this}};

  while (std::find(visited.begin(), visited.end(), n) == visited.end()) {
    std::vector< std::vector<Node*> > current_routes = routes;
    routes.clear();
    for (std::vector<Node*> &r : current_routes) {
      Node* rn = r.back();
      for (Node* nc : rn->connected_nodes) {
        if (nc->blocked) continue;
        if (std::find(visited.begin(), visited.end(), nc) != visited.end()) continue;

        visited.push_back(nc);
        std::vector<Node*> cr = r;
        cr.push_back(nc);
        routes.push_back(cr);
      }
    }
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

  Node n = Node{x, y};
  nodes[coord] = n;

  return &(nodes[coord]);
}

void DataCenter::init(std::string filename)
{
  std::string line;
  std::fstream fin(filename, std::fstream::in);
  for (int y = 0; std::getline(fin, line); ++y) {
    for (int x = 0; x < line.length(); ++x) {
      Node* node = FindOrCreateNode(x, y);
      if (line[x] == '#') node->blocked = true;
      if (line[x] == '0') {
        node->value = line[x] - '0';
        current_node = node;
        target_nodes_by_coord[{x, y}] = node;
        target_nodes.push_back(node);
      }
      if (std::atoi(&line[x]) > 0) {
        node->value = line[x] - '0';
        node->target = true;

        target_nodes_by_coord[{x, y}] = node;
        target_nodes.push_back(node);
      }
    }
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

  printf(" -- starting node = %dx%d\n", dc.current_node->x, dc.current_node->y);
  printf(" -- connection count: %lu\n", dc.current_node->connected_nodes.size());

  std::map<std::pair<Node*, Node*>, int> distance_cache;

  int shortest_length = INT_MAX;
  do {
    if (dc.target_nodes.front() != dc.current_node) continue;

    int distance = 0;
    Node* last_node = dc.target_nodes.front();
    for (Node* n: dc.target_nodes) {
      if (last_node == n) continue;

      auto distance_from_cache = distance_cache.find(std::pair<Node*, Node*>(last_node, n));
      if (distance_from_cache != distance_cache.end()) {
        distance += distance_from_cache->second;
      } else {
        int local_distance = last_node->pathTo(n).size();
        distance_cache[std::pair<Node*, Node*>(last_node, n)] = local_distance;
        distance_cache[std::pair<Node*, Node*>(n, last_node)] = local_distance;
        distance += local_distance;
      }
      last_node = n;
      if (distance > shortest_length) break;
    }
    if (distance < shortest_length) shortest_length = distance;
  } while (std::next_permutation(dc.target_nodes.begin(), dc.target_nodes.end()));


  printf("Final step count: %d\n", shortest_length);
  return 0;
}
