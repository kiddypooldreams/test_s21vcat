#include "flag_func.h"

#include <ctype.h>
#include <string.h>

#include "../common/flag_parser.h"

void process_file(const char *filename, CatFlag flags) {
    FILE *fp = filename ? fopen(filename, "rb") : stdin;
    if (!fp) {
        perror(filename ? filename : "stdin");
        return;
    }

    int c, prev = '\n';
    int line_num = 1;
    int empty_lines = 0;
    int pending_newline = 0;

    // Флаг -e включает -v
    if (flags.showEnds) {
        flags.showNonprinting = 1;
    }

    while ((c = fgetc(fp)) != EOF) {
        // Обработка squeeze-blank (-s)
        if (flags.squeezeBlank && c == '\n' && prev == '\n') {
            if (++empty_lines > 1) continue;
        } else {
            empty_lines = 0;
        }

        // Нумерация строк (-b, -n)
        if (prev == '\n') {
            if (flags.numberNonBlank) {
                if (c != '\n') printf("%6d\t", line_num++);
            } else if (flags.number) {
                printf("%6d\t", line_num++);
            }
        }

        // Обработка вывода символов
        if (c == '\n') {
            if (flags.showEnds) {
                putchar('$');
            }
            putchar('\n');
            pending_newline = 0;
        } else if (c == '\t' && flags.showTabs) {
            printf("^I");
            pending_newline = 0;
        } else if (flags.showNonprinting) {
            // Обработка непечатаемых символов
            if (c < 32 && c != '\t' && c != '\n') {
                printf("^%c", c + 64);
            } else if (c == 127) {
                printf("^?");
            } else if (c >= 128 && c < 160) {
                printf("M-^%c", c - 64);
            } else if (c >= 160 && c < 255) {
                printf("M-%c", c - 128);
            } else if (c == 255) {
                printf("M-^?");
            } else {
                putchar(c);
            }
            pending_newline = 0;
        } else {
            putchar(c);
            pending_newline = (c != '\n');
        }

        prev = c;
    }

    if (pending_newline && !(flags.squeezeBlank && prev == '\n')) {
        putchar('\n');
    }

    if (filename) fclose(fp);
}