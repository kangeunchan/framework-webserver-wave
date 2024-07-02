CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
LDFLAGS = -lsqlite3

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

CORE_SRC = $(wildcard $(SRC_DIR)/core/*.c)
MIDDLEWARE_SRC = $(wildcard $(SRC_DIR)/middleware/*.c)
DATABASE_SRC = $(wildcard $(SRC_DIR)/database/*.c)
TEMPLATE_SRC = $(wildcard $(SRC_DIR)/template/*.c)
UTILS_SRC = $(wildcard $(SRC_DIR)/utils/*.c)

# 헤더 파일 경로들
CORE_HEADERS = $(wildcard $(SRC_DIR)/core/*.h)
MIDDLEWARE_HEADERS = $(wildcard $(SRC_DIR)/middleware/*.h)
DATABASE_HEADERS = $(wildcard $(SRC_DIR)/database/*.h)
TEMPLATE_HEADERS = $(wildcard $(SRC_DIR)/template/*.h)
UTILS_HEADERS = $(wildcard $(SRC_DIR)/utils/*.h)

# 모든 소스 파일
SRC = $(CORE_SRC) $(MIDDLEWARE_SRC) $(DATABASE_SRC) $(TEMPLATE_SRC) $(UTILS_SRC)

# 각각의 object 파일
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# 최종 실행 파일
TARGET = $(BIN_DIR)/wave

.PHONY: all clean

all: $(TARGET)

# 실행 파일 생성
$(TARGET): $(OBJ) examples/hi.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 각각의 object 파일 생성
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# 예제 파일 object 파일 생성
examples/hello_world.o: examples/hi.c
	$(CC) $(CFLAGS) -c -o $@ $<

# 정리 작업
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) examples/hi.o
