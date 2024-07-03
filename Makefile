CC = gcc
CFLAGS = -Wall -Wextra -I./include

SRCDIR = src
OBJDIR = obj
BINDIR = bin
EXAMPLEDIR = example

# Source files (excluding main.c)
SRCS = $(filter-out $(SRCDIR)/main.c, $(wildcard $(SRCDIR)/*/*.c))
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Library name
LIBNAME = libwave.a

# Example target
TARGET = $(EXAMPLEDIR)/main

# Default target
all: $(TARGET)

# Create directories
$(shell mkdir -p $(OBJDIR)/config $(OBJDIR)/core $(OBJDIR)/utils $(BINDIR))

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Create static library
$(LIBNAME): $(OBJS)
	ar rcs $@ $^

# Compile and link example
$(TARGET): $(EXAMPLEDIR)/main.c $(LIBNAME)
	$(CC) $(CFLAGS) $< -L. -lwave -o $@

# Clean
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(TARGET) $(LIBNAME)

# Run example
run: $(TARGET)
	$(TARGET)

.PHONY: all clean run