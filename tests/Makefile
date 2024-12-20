# Compiler
CC = g++

# Sources
SOURCES = huffpress.cpp ../huffpress/huffpress.cpp ../huffpress/huffman/huffman.cpp ../huffpress/checksum/checksum.c framework/tinytest.c

# Output file
OUTPUT = bin/test

# Build target
all:
	$(CC) $(SOURCES) -o $(OUTPUT)

# Clean target
clean:
	rm -f $(OUTPUT)
