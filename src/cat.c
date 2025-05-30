#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

#include "../common/flag_parser.h"
#include "flag_func.h"

int main(int argc, char* argv[]) {
  CatFlag flags = CatFlagsParser(argc, argv);

  if (optind == argc) {
    process_file(NULL, flags);  // Чтение из stdin
  } else {
    for (int i = optind; i < argc; i++) {
      process_file(argv[i], flags);
    }
  }

  return 0;
}
