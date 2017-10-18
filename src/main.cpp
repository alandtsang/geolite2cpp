#include <iostream>
#include <memory>

#include <unistd.h>

#include "GeoLite2PP.h"

static void print_usage(const char *prgname) {
  fprintf(stderr, "Usage: %s -i ipaddr\n", prgname);
  exit(EXIT_FAILURE);
}

static const char *parse_args(int argc, char **argv) {
  int opt;
  const char *ip = NULL;

  if (argc != 3)
    print_usage(argv[0]);

  while ((opt = getopt(argc, argv, "i:")) != -1) {
    switch (opt) {
      case 'i':ip = optarg;
        break;
      default:print_usage(argv[0]);
    }
  }
  return ip;
}

bool get_ipdb_path(std::string &ipdb_path) {
  std::size_t pos = ipdb_path.rfind("/");
  if (pos != std::string::npos) {
    ipdb_path.erase(ipdb_path.begin() + pos, ipdb_path.end());

    pos = ipdb_path.rfind("/");
    if (pos != std::string::npos) {
      ipdb_path.erase(ipdb_path.begin() + pos, ipdb_path.end());
    } else {
      return false;
    }
  } else {
    return false;
  }

  ipdb_path.append("/data/ipdb.mmdb");

  return true;
}

int main(int argc, char **argv) {
  const char *ip = parse_args(argc, argv);
  uint16_t country, prov, isp, city;

  char *exe_path = get_current_dir_name();
  std::string ipdb_path(exe_path);
  if (!get_ipdb_path(ipdb_path)) {
    std::cout << "Error: ipdb path was wrong!\n";
    return -1;
  }

  std::shared_ptr<GeoLite2PP::DB> db;
  db = std::make_shared<GeoLite2PP::DB>(ipdb_path);

  db->get_geoinfo(ip, country, prov, isp, city);
  std::cout << "ip=" << ip << ", country=" << country << ", prov=" << prov
            << ", city=" << city << ", isp=" << isp
            << std::endl;

  return 0;
}
