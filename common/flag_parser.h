#ifndef FLAG_PARSER_H_
#define FLAG_PARSER_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct CatFlag {
  int numberNonBlank;   // -b
  int showEnds;         // -E
  int number;           // -n
  int squeezeBlank;     // -s
  int showTabs;         // -T
  int showNonprinting;  // -v

} CatFlag;

typedef struct GrepFlag {
  int pattern_e;      // -e (Шаблон)
  int pattern_count;  // Количество шаблонов
  const char **patterns;
  int ignore_case;   // -i (Игнорирование регистра)
  int invert_match;  // -v (Инвертирование совпадений)
  int count_lines;  // -c (Только количество совпадений)
  int files_with_matches;  // -l (Только имена файлов с совпадениями)
  int line_number;  // -n
} GrepFlag;

CatFlag CatFlagsParser(int argc, char *argv[]);
GrepFlag GrepFlagsParser(int argc, char *argv[]);

#endif
