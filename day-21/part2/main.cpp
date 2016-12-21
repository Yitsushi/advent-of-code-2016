#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <functional>

#define INITIAL_STATE "abcdefgh"
#define TARGET_STATE "fbgdceah"

struct Command
{
  std::string arguments;
  bool parsed;
  virtual std::string execute(std::string state) = 0;
};

struct SwapCommand : Command {
  int x = 0, y = 0;
  char a = 0x00, b = 0x00;
  std::string sub;

  std::string execute(std::string state)
  {
    if (!parsed) {
      sub = arguments.substr(0, arguments.find(' '));
      if (sub == "position") {
        sscanf(arguments.c_str(), "position %d with position %d", &x, &y);
      } else if (sub == "letter") {
        sscanf(arguments.c_str(), "letter %c with letter %c", &a, &b);
      }
      parsed = true;
    }
    if (sub == "letter") {
      x = state.find(a);
      y = state.find(b);
    }

    std::swap(state[x], state[y]);
    return state;
  }
};

struct RotateCommand : Command {
  std::string direction;
  int value = 0;
  char c = 0x00;

  std::string execute(std::string state)
  {

    if (!parsed) {
      direction = arguments.substr(0, arguments.find(' '));
      if (direction == "left" || direction == "right") {
        sscanf(arguments.c_str(), "%*s %d steps", &value);
      } else {
        direction = "right";
        sscanf(arguments.c_str(), "based on position of letter %c", &c);
      }
      parsed = true;
    }

    if (c != 0x00) {
      value = state.find(c);

      if (value >= 4) value++;
      value++;
    }

    int steps = value % state.length();
    if (direction == "right") steps = state.length() - steps;

    state = state.substr(steps) + state.substr(0, steps);

    return state;
  }
};

struct ReverseCommand : Command {
  int x = 0, y = 0;
  std::string execute(std::string state)
  {
    if (!parsed) {
      sscanf(arguments.c_str(), "positions %d through %d", &x, &y);
      parsed = true;
    }

    std::reverse(state.begin() + x, state.begin() + y + 1);
    return state;
  }
};

struct MoveCommand : Command {
  int from = 0, to = 0;
  std::string execute(std::string state)
  {
    if (!parsed) {
      sscanf(arguments.c_str(), "position %d to position %d", &from, &to);
      parsed = true;
    }
    char c = state[from];
    state.erase(state.begin() + from);
    state.insert(to, 1, c);
    return state;
  }
};

struct Program
{
  std::string initial;
  std::string state;
  std::vector<Command*> lines;
  int head;

  Program(std::string filename, std::string base);

  void execute(std::string target);
};

void Program::execute(std::string target)
{
  do {
    state = initial;
    for (head = 0; head < lines.size(); ++head) {
      state = lines[head]->execute(state);
    }
    if (target == state) { break; }
  } while (std::next_permutation(initial.begin(), initial.end()));
}

Program::Program(std::string filename, std::string base) : initial(base)
{
  std::map<std::string, std::function<Command* ()> > availableCommands;
  availableCommands["swap"] = []() -> Command* { return new SwapCommand(); };
  availableCommands["rotate"] = []() -> Command* { return new RotateCommand(); };
  availableCommands["reverse"] = []() -> Command* { return new ReverseCommand(); };
  availableCommands["move"] = []() -> Command* { return new MoveCommand(); };

  std::string line;
  std::fstream fin(filename, std::fstream::in);
  while (std::getline(fin, line)) {
    std::string instruction = line.substr(0, line.find(' '));
    std::string rest = line.substr(instruction.length() + 1);

    Command* command = availableCommands[instruction]();
    command->arguments = rest;

    lines.push_back(command);
  }
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input_file>\n", argv[0]);
    return 1;
  }

  Program* program = new Program(argv[1], INITIAL_STATE);
  program->execute(TARGET_STATE);

  printf("Value: %s\n", program->initial.c_str());

  return 0;
}
