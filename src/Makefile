# Компилятор и флаги
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

.PHONY: all clean re