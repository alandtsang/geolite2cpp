#include <iostream>
#include <cstdlib>
#include <memory>

#include <unistd.h>

#include "GeoLite2PP.h"


static void print_usage(const char *prgname)
{
  fprintf(stderr, "Usage: %s -i ipaddr\n", prgname);
  exit(EXIT_FAILURE);
}

static const char* parse_args(int argc, char** argv)
{
  int opt;
  const char* ip = NULL;

  if (argc != 3)
    print_usage(argv[0]);

  while ((opt = getopt(argc, argv, "i:")) != -1) {
    switch (opt) {
    case 'i':
      ip = optarg;
      break;
    default:
      print_usage(argv[0]);
    }
  }
  return ip;
}

int main(int argc, char** argv)
{
  std::shared_ptr<GeoLite2PP::DB> db;
  db = std::make_shared<GeoLite2PP::DB>("../data/ipdb.mmdb");

  uint16_t country, prov, isp, city;
  const char* ip = parse_args(argc, argv);

  db->get_geoinfo(ip, country, prov, isp, city);
  printf("ip=%s, country=%d, prov=%d, city=%d, isp=%d\n", ip, country, prov, city, isp);

  return 0;
}
