#include <stdio.h>
#include <iostream>
#include <string.h>
#include <openssl/sha.h>
#include <getopt.h>
#include <math.h>

std::string encodeId(std::string& ibuf) {
  int                 mid, i, len = 0;
  std::string         salt, key, hash;
  unsigned char       obuf[20];
  /*
   * TODO: This is a wide buffer to avoid 'stack smashing'. Size this right!
   */
  char                tbuf[100] = "";

  /*
   * TODO: There are strlen bugs here
   */
  mid = round(ibuf.length()/2 - 1);
  salt.append(ibuf, ibuf.length() - 5, ibuf.length());
  salt.append("613");
  salt.append(ibuf, 0, 2);
  salt.append("125");
  salt.append(ibuf, (mid - 3), 3);
  salt.append("1125");
  salt.append(ibuf, mid, 4);

  key.append(ibuf);
  key.append(salt);

  SHA1(reinterpret_cast<const unsigned char *>(key.c_str()), key.length(), obuf);
  for (i = 0; i < 20; i++) {
    len += snprintf(tbuf+len, 100, "%02x", obuf[i]);
  }

  hash = tbuf;
  return hash;
}

int main(int argc, char **argv) {
  int           c;
  std::string   hash, ibuf;
  const char *  short_opt = "s:";

  struct option long_opt[] = {
    { "string", required_argument, NULL, 's' }
  };

  while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
    switch(c) {
      case 's':
	ibuf.assign(optarg);
	break;
      default:
	fprintf(stderr, "%s: invalid option -- %c\n", argv[0], c);
	return -2;
    }
  }

  hash = encodeId(ibuf);
  std::cout << "HASH: " << hash << '\n';
  return 0;
}
