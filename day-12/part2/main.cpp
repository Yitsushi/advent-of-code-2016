#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <functional>
#include <ctime>

#define int_or_addr(a)  (self->memory.hasRegister(a[0]) ? self->memory.registers[a[0]] : atoi(a.c_str()) )

struct Program;
struct Memory;
struct Command;

typedef std::function<void (Program*)> commandFunction;

struct Command {
  commandFunction operation;

  std::string first, second;
};

struct Memory {
  std::map<char, int> registers;

  void init();
  bool hasRegister(char);
};

void Memory::init() {
  registers['a'] = registers['b'] = registers['d'] = 0;
  registers['c'] = 1;
}

bool Memory::hasRegister(char c)
{
  return registers.count(c) > 0;
}

struct Program {
  std::vector<Command*> lines;

  Memory memory;
  int head;

  void execute();
  void init();
  void dump();
};

void Program::init() {
  memory = Memory{};
  memory.init();
}

void Program::execute()
{
  for (;head < lines.size(); head++) {
    Command* current = lines[head];
    current->operation(this);
  }
}

void Program::dump()
{
  printf("Register a    => %d\n", memory.registers['a']);
  printf("Register b    => %d\n", memory.registers['b']);
  printf("Register c    => %d\n", memory.registers['c']);
  printf("Register d    => %d\n", memory.registers['d']);
  printf("Head position => %d\n", head);
}

Program readfile(std::string filename)
{
  Program program = {};

  std::map< std::string, std::function<commandFunction (Command*)> > operations;
  operations["cpy"] = [](Command* command) -> commandFunction {
    return [command](Program* self) { self->memory.registers[command->second[0]] = int_or_addr(command->first); };
  };
  operations["inc"] = [](Command* command) -> commandFunction {
    return [command](Program* self) { self->memory.registers[command->first[0]]++; };
  };
  operations["dec"] = [](Command* command) -> commandFunction {
    return [command](Program* self) { self->memory.registers[command->first[0]]--; };
  };
  operations["jnz"] = [](Command* command) -> commandFunction {
    return [command](Program* self) { if (int_or_addr(command->first) != 0) { self->head += atoi(command->second.c_str()) - 1; } };
  };

  std::string line;
  std::fstream fin(filename, std::fstream::in);
  while (std::getline(fin, line)) {
    Command* command = new Command();
    command->operation = operations[line.substr(0, 3)](command);
    std::string args = line.substr(4);

    int space_pos = args.find(" ");
    if (space_pos != std::string::npos) {
      command->first = args.substr(0, space_pos);
      command->second = args.substr(space_pos + 1);
    } else {
      command->first = args;
      command->second = "";
    }

    program.lines.push_back(command);
  }
  return program;
}


int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input_file>\n", argv[0]);
    return 1;
  }

  Program program = readfile(argv[1]);
  program.init();
  program.execute();
  program.dump();

  return 0;
}
