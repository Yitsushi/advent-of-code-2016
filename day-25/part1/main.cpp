#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <functional>
#include <ctime>
#include <list>

#define int_or_addr(a)  (self->memory.hasRegister(a[0]) ? self->memory.registers[a[0]] : atoi(a.c_str()) )

struct Program;
struct Memory;
struct Command;

typedef std::function<void (Program*)> commandFunction;

struct Command
{
  std::string type;
  commandFunction operation;

  std::string first, second;
  void print();
};

void Command::print()
{
  printf("%-5s %-3s %-3s\n", type.c_str(), first.c_str(), second.c_str());
}

struct Memory
{
  std::map<char, int> registers;

  void init();
  bool hasRegister(char);
};

void Memory::init() {
  registers['a'] = registers['b'] = registers['c'] = registers['d'] = 0;
}

bool Memory::hasRegister(char c)
{
  return registers.count(c) > 0;
}

struct Program
{
  std::vector<Command*> lines;

  Memory memory;
  int head;

  std::map< std::string, std::function<commandFunction (Command*)> > operations;
  std::list<int> signal_sent;

  Program();

  void execute(int signal_limit);
  void init();
  void dump();
  void dumpCodebase();
};

void Program::init() {
  memory = Memory{};
  memory.init();
}

Program::Program()
{
  operations["cpy"] = [](Command* command) -> commandFunction {
    return [command](Program* self) {
      self->memory.registers[command->second[0]] = int_or_addr(command->first);
    };
  };
  operations["inc"] = [](Command* command) -> commandFunction {
    return [command](Program* self) {
      self->memory.registers[command->first[0]]++;
    };
  };
  operations["dec"] = [](Command* command) -> commandFunction {
    return [command](Program* self) {
      self->memory.registers[command->first[0]]--;
    };
  };
  operations["jnz"] = [](Command* command) -> commandFunction {
    return [command](Program* self) {
      if (int_or_addr(command->first) != 0) { self->head += int_or_addr(command->second) - 1; }
    };
  };
  operations["tgl"] = [](Command* command) -> commandFunction {
    return [command](Program* self) {
      int address = self->head + int_or_addr(command->first);

      if (address >= self->lines.size()) { return; }

      Command* target = self->lines[address];

      printf("TGL on +%d\n", address);

      if (target->type == "dec" || target->type == "tgl") {
        target->type = "inc";
        target->operation = self->operations["inc"](target);
      } else if (target->type == "inc") {
        target->type = "dec";
        target->operation = self->operations["dec"](target);
      } else if (target->type == "jnz") {
        target->type = "cpy";
        target->operation = self->operations["cpy"](target);
      } else if (target->type == "cpy") {
        target->type = "jnz";
        target->operation = self->operations["jnz"](target);
      }
    };
  };
  operations["out"] = [](Command* command) -> commandFunction {
    return [command](Program* self) {
      self->signal_sent.push_back(int_or_addr(command->first));
    };
  };
}

void Program::execute(int signal_limit)
{
  signal_sent.clear();
  for (head = 0; head < lines.size(); head++) {
    Command* current = lines[head];
    current->operation(this);
    if (signal_sent.size() >= signal_limit) break;
  }
}

void Program::dumpCodebase()
{
  printf("----------------------\n");
  for (int i = 0; i < lines.size(); ++i) {
    printf("%2d  %s  %s %s\n", i, lines[i]->type.c_str(), lines[i]->first.c_str(), lines[i]->second.c_str());
  }
  printf("----------------------\n");
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

  std::string line;
  std::fstream fin(filename, std::fstream::in);
  while (std::getline(fin, line)) {
    Command* command = new Command();
    command->operation = program.operations[line.substr(0, 3)](command);
    command->type = line.substr(0, 3);
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
  unsigned int signal;
  for (int a = 0; a < UINT32_MAX; ++a) {
    signal = 0;
    program.memory.init();
    program.memory.registers['a'] = a;
    program.execute(16);
    printf("[a=%5d] signal sent: ", a);
    for (int s : program.signal_sent) {
      signal <<= 1;
      signal += s;
      printf("%d ", s);
    }
    printf("%d\n", signal);
    if (signal == 21845) break; // 0101010101010101 = 21845
  }
  //program.dump();

  return 0;
}
