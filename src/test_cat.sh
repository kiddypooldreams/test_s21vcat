#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

if [ $# -ne 1 ]; then
    echo -e "${RED}Usage: $0 <path_to_your_cat>${NC}"
    exit 1
fi

YOUR_CAT="$1"

# Определяем оригинальный cat (GNU на macOS)
if [[ "$(uname)" == "Darwin" ]] && command -v gcat &>/dev/null; then
    ORIG_CAT="gcat"
else
    ORIG_CAT="cat"
fi

if [ ! -x "$YOUR_CAT" ]; then
    echo -e "${RED}Error: '$YOUR_CAT' is not executable.${NC}"
    exit 1
fi

TMP_ORIG="tmp_orig.txt"
TMP_YOUR="tmp_your.txt"
TEST_FILE="test_file.txt"

# Создаем тестовый файл с разными типами строк
echo -e "# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I../../include  # Если есть заголовки в include/

# Целевой бинарник
TARGET = s21_cat

# Пути к исходникам
SRC_CAT = cat.c flag_func.c
SRC_COMMON = ../common/flag_parser.c

# Объектные файлы
OBJ_CAT = $(SRC_CAT:.c=.o)
OBJ_COMMON = $(notdir $(SRC_COMMON:.c=.o))
OBJS = $(OBJ_CAT) $(OBJ_COMMON)

# Сборка
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

# Правила для объектных файлов в текущей dir
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Правила для common-файлов (лежат в ../common/)
%.o: ../common/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Очистка
clean:
	rm -f $(OBJS) $(TARGET)

re: clean all

.PHONY: all clean re" > "$TEST_FILE"

run_test() {
    local test_name="$1"
    local flags="$2"
    local file="$3"
    
    echo -e "\n=== TEST: $test_name ==="
    echo "Command: $flags $file"

    $ORIG_CAT $flags "$file" > "$TMP_ORIG" 2>&1
    $YOUR_CAT $flags "$file" > "$TMP_YOUR" 2>&1

    if diff -q "$TMP_ORIG" "$TMP_YOUR" > /dev/null; then
        echo -e "${GREEN}PASSED${NC}"
    else
        echo -e "${RED}FAILED${NC}"
        echo "--- Expected output ---"
        cat "$TMP_ORIG"
        echo "--- Your output ---"
        cat "$TMP_YOUR"
        echo "--- Differences ---"
        diff --color "$TMP_ORIG" "$TMP_YOUR"
        exit 1
    fi
}

# Основные тесты
run_test "Basic output" "" "$TEST_FILE"
run_test "Number non-blank lines (-b)" "-b" "$TEST_FILE"
run_test "Show line ends (-e)" "-e" "$TEST_FILE"
run_test "Number all lines (-n)" "-n" "$TEST_FILE"
run_test "Squeeze blank lines (-s)" "-s" "$TEST_FILE"
run_test "Show tabs (-t)" "-t" "$TEST_FILE"

# Тест с stdin
echo -e "\n=== TEST: stdin ==="
echo "Test line" | $ORIG_CAT -n > "$TMP_ORIG"
echo "Test line" | $YOUR_CAT -n > "$TMP_YOUR"

if diff -q "$TMP_ORIG" "$TMP_YOUR" > /dev/null; then
    echo -e "${GREEN}PASSED${NC}"
else
    echo -e "${RED}FAILED${NC}"
    diff --color "$TMP_ORIG" "$TMP_YOUR"
    exit 1
fi

# GNU-specific tests
if [[ "$ORIG_CAT" == "gcat" || "$(uname)" == "Linux" ]]; then
    run_test "GNU: show $ at end (-E)" "-E" "$TEST_FILE"
    run_test "GNU: show tabs as ^I (-T)" "-T" "$TEST_FILE"
fi

# Удаляем временные файлы
rm -f "$TMP_ORIG" "$TMP_YOUR" "$TEST_FILE"

echo -e "\n${GREEN}All tests passed!${NC}"