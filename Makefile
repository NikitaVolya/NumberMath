# Compiler and flags
CC = gcc
CFLAGS = -W -Wall -std=c89 -O2 -pedantic

# Directories
SRC_DIR = .
GAME_OBJ_DIR = game_objects
OUTPUT_STRATEGIES_DIR = output_strategies

# Sources and objects
SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(GAME_OBJ_DIR)/*.c) $(wildcard $(OUTPUT_STRATEGIES_DIR)/*/*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
DEP = $(OBJ:.o=.d)

# Executable
TARGET = main

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ)

# Compilation rule (with dependency file)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Include generated dependency files (if they exist)
-include $(DEP)

# Cleaning
clean:
	rm -f $(OBJ) $(DEP) $(TARGET)

.PHONY: all clean