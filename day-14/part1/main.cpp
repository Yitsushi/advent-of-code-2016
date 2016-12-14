#include <string>
#include <vector>
#include <openssl/md5.h>
#include <regex>

#define SALT "ngcjuoqr"

std::regex match_three_in_a_row("(.)\\1\\1");
std::regex match_five_in_a_row("(.)\\1{4}");

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

std::vector<std::string> hash_cache;
unsigned int counter = 0;

std::string nextValidKey()
{
  std::string valid_key;
  while(true) {
    std::string hash = md5hash(std::string(SALT) + std::to_string(counter));
    if (hash_cache.size() > counter) {
      hash = hash_cache.at(counter);
    } else {
      hash = md5hash(std::string(SALT) + std::to_string(counter));
      hash_cache.push_back(hash);
    }

    std::cmatch match;
    if (regex_search(hash.c_str(), match, match_three_in_a_row)) {
      char found = match[1].str().at(0);

      std::string subhash;
      for (int j = 1; j <= 1000; ++j) {
        if (hash_cache.size() > counter + j) {
          subhash = hash_cache.at(counter + j);
        } else {
          subhash = md5hash(std::string(SALT) + std::to_string(counter + j));
          hash_cache.push_back(subhash);
        }
        if (regex_search(subhash.c_str(), match, match_five_in_a_row)) {
          if (match[1].str().at(0) == found) {
            ++counter;
            return hash;
          }
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
