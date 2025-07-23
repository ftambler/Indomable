# Compiler and flags
CC = gcc
CFLAGS = -Iinclude/core -Iinclude/entities -Iinclude/systems -Iinclude/utils -Ilib/raylib/include -Ilib/cJSON -Isrc/utils -Wall -Wextra -g
LDFLAGS = -Llib/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm

# Source files
# SRC = src/main.c lib/cJSON/cJSON.c src/utils/prints.c
SRC := $(wildcard src/*.c src/core/*.c src/entities/*.c src/systems/*.c src/utils/*.c lib/cJSON/*.c)
OBJ = $(SRC:.c=.o)

# Output executable
TARGET = build/main.exe

# Default build rule
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	del /Q $(OBJ) $(TARGET)

.PHONY: all clean
