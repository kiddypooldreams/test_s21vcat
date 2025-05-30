#include "flag_parser.h"

#include "getopt.h"

CatFlag CatFlagsParser(int argc, char *argv[]) {
  struct option long_flags[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                {"number", no_argument, NULL, 'n'},
                                {"squeeze-blank", no_argument, NULL, 's'},
                                {0, 0, 0, 0}};
  CatFlag flag = {0};

  int opt;

  opt = getopt_long(argc, argv, "bnsEeTt", long_flags, 0);

  switch (opt) {
    case 'b':
      flag.numberNonBlank = 1;
      break;
    case 'e':
      flag.showEnds = 1;
      flag.showNonprinting = 1;
      break;
    case 'E':
      flag.showEnds = 1;
      break;
    case 's':
      flag.squeezeBlank = 1;
      break;
    case 'n':
      flag.number = 1;
      break;
    case 't':
      flag.showTabs = 1;
      flag.showNonprinting = 1;
      break;
    case 'T':
      flag.showTabs = 1;
      break;
    case '?':
      perror("ERROR");
      break;
  }

  return flag;
}

GrepFlag GrepFlagsParser(int argc, char *argv[]) {
  GrepFlag flag = {0};
  flag.patterns = malloc(sizeof(char *) * argc);  // Максимум — argc шаблонов
  flag.pattern_count = 0;

  int opt;
  struct option long_flags[] = {{"line-number", no_argument, NULL, 'n'},
                                {"ignore-case", no_argument, NULL, 'i'},
                                {"invert-match", no_argument, NULL, 'v'},
                                {"count", no_argument, NULL, 'c'},
                                {"regexp", required_argument, NULL, 'e'},
                                {"files-with-matches", no_argument, NULL, 'l'},
                                {NULL, 0, NULL, 0}};

  while ((opt = getopt_long(argc, argv, "e:iclnv", long_flags, NULL)) != -1) {
    switch (opt) {
      case 'i':
        flag.ignore_case = 1;
        break;
      case 'e':
        flag.patterns[flag.pattern_count++] = optarg;  // Добавляем шаблон
        break;
      case 'v':
        flag.invert_match = 1;
        break;
      case 'l':
        flag.files_with_matches = 1;
        break;
      case 'n':
        flag.line_number = 1;
        break;
      case 'c':
        flag.count_lines = 1;
        break;
      case '?':
        fprintf(stderr, "Unknown option\n");
        exit(EXIT_FAILURE);
    }
  }

  // Если -e не использовался, берем первый аргумент как шаблон
  if (flag.pattern_count == 0 && optind < argc) {
    flag.patterns[flag.pattern_count++] = argv[optind++];
  }

  return flag;
}
