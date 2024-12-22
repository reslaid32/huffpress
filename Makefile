# Define the output directories
OUTDIR = build
OBJDIR = $(OUTDIR)/obj
BINDIR = $(OUTDIR)/bin
TESTDIR = tests
TESTOUTDIR = $(OUTDIR)/$(TESTDIR)
TESTOBJDIR = $(TESTOUTDIR)/obj

# Set the compiler and flags
CC = clang
CXX = clang++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -L$(BINDIR)

ifeq ($(shell uname), Linux)
  LIBEXT = .so
  APPEXT = 
else
  LIBEXT = .dll
  APPEXT = .exe
endif

# Default target
# Target for building libraries only
only-lib: libraries

# Target for building all project (with tests)
all: libraries tests

# Target for building tests only
only-test: tests

# Create directories if they don't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Build libraries
libraries: $(OBJDIR) $(BINDIR)
	@echo "Building huffman library..."
	$(CXX) $(CXXFLAGS) -I./huffpress/huffman -c ./huffpress/huffman/huffman.cpp -o $(OBJDIR)/huffman.o
	$(CXX) -shared $(OBJDIR)/huffman.o -o $(BINDIR)/libhuffman$(LIBEXT)

	@echo "Building checksum library..."
	$(CXX) -c ./huffpress/checksum/checksum.c -o $(OBJDIR)/checksum.o
	$(CXX) -shared $(OBJDIR)/checksum.o -o $(BINDIR)/libchecksum$(LIBEXT)

	@echo "Building huffpress library..."
	$(CXX) $(CXXFLAGS) -I./huffpress/huffman -I./huffpress/checksum -c ./huffpress/huffpress.cpp -o $(OBJDIR)/huffpress.o
	$(CXX) -shared $(OBJDIR)/huffpress.o -o $(BINDIR)/libhuffpress$(LIBEXT) $(LDFLAGS) -lhuffman -lchecksum
	
# Build tests
tests: $(OBJDIR) $(BINDIR)
	@echo "Building tests..."

	@echo "Building framework..."
	$(CXX) $(CXXFLAGS) -c $(TESTDIR)/framework/tinytest.c -o $(OBJDIR)/unit.framework.o

	@echo "Building test executable..."
	$(CXX) $(CXXFLAGS) -I$(TESTDIR) -I./huffpress -c $(TESTDIR)/unit.cpp -o $(OBJDIR)/unit.o

	$(CXX) $(CXXFLAGS) $(OBJDIR)/unit.o $(OBJDIR)/unit.framework.o -o $(BINDIR)/unit$(APPEXT) $(LDFLAGS) -lhuffman -lchecksum -lhuffpress

# Clean up object files (optional)
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(OUTDIR)

.PHONY: all only-lib only-test libraries tests clean
