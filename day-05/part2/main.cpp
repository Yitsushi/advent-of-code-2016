#include <string>
#include <openssl/md5.h>
#include <stdlib.h>

static const char alphanum[] =
  "0123456789"
  "abcdefghijklmnopqrstuvwxyz";

int alphanumLength = sizeof(alphanum) - 1;

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <roomid>\n", argv[0]);
    return 1;
  }

  std::string door_id = std::string(argv[1]);
  std::string password = "........";

  for (int counter = 0; password.find('.') != std::string::npos; counter++) {
    std::string current = door_id + std::to_string(counter);

    unsigned char sum[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)current.c_str(), current.length(), sum);

    char out[32];

    for (int n = 0; n < 16; ++n) {
      snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)sum[n]);
    }

    std::string hash = std::string(out);

    if (hash.substr(0, 5) == "00000") {
      char c = hash[5];
      int num = atoi(&c);

      char as_char = std::to_string(num).c_str()[0];

      if (c == as_char && num < 8) {
        if (password[num] == '.') {
          password[num] = hash[6];
        }
      }
    }
    std::string displayPass = password.c_str();
    for (int j = 0; displayPass[j] != '\0'; j++) {
      if (displayPass[j] == '.')
        displayPass[j] = alphanum[rand() % alphanumLength];
    }
    std::replace( displayPass.begin(), displayPass.end(), '.', '%');
    printf("\r%s", displayPass.c_str());
  }
  printf("\n\nPassword: %s\n", password.c_str());

  return 0;
}
