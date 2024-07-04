CC = gcc
CFLAGS = -Wall -Wextra -pthread
INCLUDES = -Isrc

SRCS = $(wildcard src/*.c src/*/*.c)
OBJS = $(SRCS:.c=.o)

TARGET = wave

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)