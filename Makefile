# Define the output directories
OUTDIR = build
OBJDIR = $(OUTDIR)/obj
BINDIR = $(OUTDIR)/bin
TESTDIR = tests
EXAMPLESDIR = examples

# Set the compiler and flags
CC = clang
CXX = clang++
CXXFLAGS = -std=c++11 -Wall -fPIC
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
all: libraries tests examples

# Target for building tests only
only-test: tests
 
# Target for building examples only
only-examples: examples

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

	@echo "Building huffpress cli library..."
	$(CXX) $(CXXFLAGS) -I./huffpress/huffman -I./huffpress/checksum -I./huffpress -c ./huffpress/cli/cli.cpp -o $(OBJDIR)/huffpresscli.o
	$(CXX) -shared $(OBJDIR)/huffpresscli.o -o $(BINDIR)/libhuffpresscli$(LIBEXT) $(LDFLAGS) -lhuffman -lchecksum -lhuffpress

# Build tests
tests: $(OBJDIR) $(BINDIR)
	@echo "Building tests..."

	@echo "Building framework..."
	$(CXX) $(CXXFLAGS) -c $(TESTDIR)/framework/tinytest.c -o $(OBJDIR)/unit.framework.o

	@echo "Building test executable..."
	$(CXX) $(CXXFLAGS) -I$(TESTDIR) -I./huffpress -c $(TESTDIR)/unit.cpp -o $(OBJDIR)/unit.o

	$(CXX) $(CXXFLAGS) $(OBJDIR)/unit.o $(OBJDIR)/unit.framework.o -o $(BINDIR)/unit$(APPEXT) $(LDFLAGS) -lhuffman -lchecksum -lhuffpress

# Build examples
examples: $(OBJDIR) $(BINDIR)
	@echo "Building examples..."

	$(CXX) $(CXXFLAGS) -I. -c $(EXAMPLESDIR)/cli.cpp -o $(OBJDIR)/cli.o
	$(CXX) $(CXXFLAGS) $(OBJDIR)/cli.o -o $(BINDIR)/cli$(APPEXT) $(LDFLAGS) -lhuffman -lchecksum -lhuffpress -lhuffpresscli

# Clean up object files (optional)
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(OUTDIR)

.PHONY: all only-lib only-test only-examples libraries tests examples clean
