# Compiler and linker
CC = g++

# Compiler flags
CFLAGS = -Wall -static

# Libraries to link
LIBS = -L/libs -lraylib -lm

# Output binary names (inside the bin directory)
TARGET = bin/main

# Source files
SRC = main.cpp

# Object files (generated from source files inside the bin folder)
OBJ = $(SRC:.cpp=.o)
OBJ_BIN = $(OBJ:%.o=bin/%.o)

# Default target
all: $(TARGET) $(QUADTREE_TEST_TARGET)

# Rule to link the object files into the final executable for main
$(TARGET): bin/main.o
	$(CC) bin/main.o $(LIBS) -static-libgcc -static-libstdc++ -o $(TARGET)

# Rule to compile .cpp files into .o files inside the bin folder
bin/%.o: %.cpp
	@mkdir -p bin  # Make sure the bin folder exists
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files (optional, but this rule is still here for reference)
clean:
	rm -rf bin

.PHONY: all clean
