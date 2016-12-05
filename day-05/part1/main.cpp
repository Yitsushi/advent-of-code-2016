#include <string>
#include <openssl/md5.h>

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("%s <roomid>\n", argv[0]);
    return 1;
  }

  std::string door_id = std::string(argv[1]);
  std::string password = "";

  printf("........\r");
  for (int counter = 0; password.length() < 8; counter++) {
    std::string current = door_id + std::to_string(counter);

    unsigned char sum[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)current.c_str(), current.length(), sum);

    char out[32];

    for (int n = 0; n < 16; ++n) {
      snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int)sum[n]);
    }

    std::string hash = std::string(out);

    printf("%c", hash[5]);
    if (hash.substr(0, 5) == "00000") {
      password += hash[5];
    } else {
      printf("%c", 0x08);
    }
  }
  printf("\n\nPassword: %s\n", password.c_str());

  return 0;
}
