#include <vector>
#include <fstream>
#include <map>

// configuration
#define COMPARE_VALUE_HIGHER 61
#define COMPARE_VALUE_LOWER 17
// macros
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

struct Bot {
  // positive bot
  // negative output
  int number;

  std::vector<int> hand;
  bool handledDesiredCompareValues;

  Bot* low_target;
  Bot* high_target;

  // true  = handled desired compare values
  // false = not ^^
  bool addValue(int value);
  bool execute();

  int lowerValue();
  int higherValue();
};

bool Bot::addValue(int value)
{
  hand.push_back(value);
  if (hand.size() > 1) { return execute(); }
  return false;
}

bool Bot::execute()
{
  int lower = lowerValue(), higher = higherValue();
  hand.clear();
  if (lower == COMPARE_VALUE_LOWER && higher == COMPARE_VALUE_HIGHER) {
    printf("<Bot#%03d> I got a new values %d and %d\n", number, lower, higher);
    return true;
  }

  if(low_target->addValue(lower)) { return true; };
  if(high_target->addValue(higher)) { return true; };

  return false;
}

int Bot::lowerValue()
{
  if (hand.size() > 1) { return MIN(hand[0], hand[1]); }
  if (hand.size() > 0) { return hand[0]; }

  return -1;
}

int Bot::higherValue()
{
  if (hand.size() > 1) { return MAX(hand[0], hand[1]); }
  if (hand.size() > 0) { return -1; }

  return -1;
}

std::map<int, Bot> army;

Bot* getOrCreateBot(int number)
{
  std::map<int, Bot>::iterator it = army.find(number);

  if (it == army.end()) {
    army[number] = Bot{number};
    return &(army[number]);
  }

  return &(it->second);
}

void readfile(std::string filename)
{
  std::string line;
  std::fstream fin(filename, std::fstream::in);

  std::string instruction_type;
  while (std::getline(fin, line)) {
    instruction_type = line.substr(0, line.find(' '));

    if (instruction_type == "bot") {
      int bot_number = 0, low_target_number = 0, high_target_number = 0;
      char low_target[6], high_target[6];

      sscanf(
          line.c_str(), "bot %d gives low to %s %d and high to %s %d",
          &bot_number,
          low_target, &low_target_number,
          high_target, &high_target_number
      );
      // ++ because output and bot also can be number 0
      if (strncmp(low_target, "output", 6) == 0) { low_target_number++; low_target_number *= -1; }
      if (strncmp(high_target, "output", 6) == 0) { high_target_number++; high_target_number *= -1; }

      Bot* bot = getOrCreateBot(bot_number);
      bot->low_target = getOrCreateBot(low_target_number);
      bot->high_target = getOrCreateBot(high_target_number);

    } else if (instruction_type == "value") {
      int bot_number = 0, value = 0;
      sscanf(line.c_str(), "value %d goes to bot %d", &value, &bot_number);

      Bot* bot = getOrCreateBot(bot_number);
      bot->hand.push_back(value);
    }
  }
}

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <input_file>\n", argv[0]);
    return 1;
  }

  readfile(argv[1]);

  for (std::map<int, Bot>::iterator it = army.begin(); it != army.end(); ++it) {
    if (it->second.hand.size() > 1) {
      it->second.execute();
    }
  }

  return 0;
}
