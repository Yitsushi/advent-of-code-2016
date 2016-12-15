#include <string>
#include <vector>
#include <openssl/md5.h>

#define SALT "ngcjuoqr"

std::string md5hash(std::string current)
{
  for (int i = 0; i < 2017; ++i) {
    unsigned char sum[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)current.c_str(), current.length(), sum);

    char out[32];
    for (int n = 0; n < 16; ++n) {
      snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)sum[n]);
    }
    current = std::string(out);
  }

  return current;
}

bool threeInRow(const std::string current, char* ch)
{
  for (int i = 0; i < current.length() - 2; ++i) {
    if (current[i] == current[i+1] && current[i+1] == current[i+2]) {
      *ch = current.c_str()[i];
      return true;
    }
  }
  return false;
}

bool fiveInRowAs(const std::string current, const char ch)
{
  std::string target = std::string(5, ch);
  for (int i = 0; i < current.length() - 4; ++i) {
    if (current.substr(i, 5) == target) {
      return true;
    }
  }
  return false;
}

std::vector<std::string> hash_cache;
unsigned int counter = 0;

std::string nextValidKey()
{
  while(true) {
    std::string hash;
    if (hash_cache.size() > counter) {
      hash = hash_cache.at(counter);
    } else {
      hash = md5hash(std::string(SALT) + std::to_string(counter));
      hash_cache.push_back(hash);
    }

    char ch = 0x00;
    if (threeInRow(hash, &ch)) {
      std::string subhash;
      for (int j = 1; j <= 1000; ++j) {
        if (hash_cache.size() > counter + j) {
          subhash = hash_cache.at(counter + j);
        } else {
          subhash = md5hash(std::string(SALT) + std::to_string(counter + j));
          hash_cache.push_back(subhash);
        }
        if (fiveInRowAs(subhash.c_str(), ch)) {
          ++counter;
          return hash;
        }
      }
    }
    ++counter;
  }
}

int main()
{
  std::vector<std::string> valid_keys;

  while (valid_keys.size() < 64)
  {
    std::string key = nextValidKey();
    valid_keys.push_back(key);
  }

  printf("Last index: %d\n", counter - 1);

  return 0;
}
