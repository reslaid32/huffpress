# Compiler
CC = g++

# Sources
SOURCES = main.cpp huffpress/huffpress.cpp huffpress/huffman/huffman.cpp huffpress/checksum/checksum.c

# Output file
OUTPUT = bin/main

# Build target
all:
	$(CC) $(SOURCES) -o $(OUTPUT)

# Clean target
clean:
	rm -f $(OUTPUT)
