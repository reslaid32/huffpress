# Define the output directories
$OUTDIR = "build"
$OBJDIR = Join-Path $OUTDIR "obj"
$BINDIR = Join-Path $OUTDIR "bin"
$TESTDIR = "tests"
$EXAMPLESDIR = "examples"

# Set the compiler and flags
# $CC = "clang"
$CXX = "clang++"
$CXXFLAGS = "-std=c++11"
$CXXWARNINGS = "-Wall"
$CXXPIC = "-fPIC" 
$CXXTARGET = "--target=x86_64-w64-mingw32"
$LDFLAGS = "-L$BINDIR"

# Check for OS (Windows or Linux)
if ($env:OS -eq "Windows_NT") {
    $LIBEXT = ".dll"
    $APPEXT = ".exe"
} else {
    $LIBEXT = ".so"
    $APPEXT = ""
}

# Default target
$ARG = $args[0]
if (-not $ARG) {
    $ARG = "only-lib"
}

# Functions
function Build-Libraries {
    Write-Host "Building libraries..."

    # Build the huffman library
    & $CXX $CXXTARGET $CXXFLAGS $CXXWARNINGS $CXXPIC -I"./huffpress/huffman" -c "./huffpress/huffman/huffman.cpp" -o "$OBJDIR\huffman.o"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build huffman.o"
        exit $LASTEXITCODE
    }
    & $CXX $CXXTARGET -shared "$OBJDIR\huffman.o" -o "$BINDIR\libhuffman$LIBEXT"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build libhuffman$LIBEXT"
        exit $LASTEXITCODE
    }

    # Build the checksum library
    & $CXX $CXXTARGET -c "./huffpress/checksum/checksum.c" -o "$OBJDIR\huffchecksum.o"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build huffchecksum.o"
        exit $LASTEXITCODE
    }
    & $CXX $CXXTARGET -shared "$OBJDIR\huffchecksum.o" -o "$BINDIR\libhuffchecksum$LIBEXT"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build libhuffchecksum$LIBEXT"
        exit $LASTEXITCODE
    }

    # Build the huffpress library
    & $CXX $CXXTARGET $CXXFLAGS $CXXWARNINGS $CXXPIC -I"./huffpress/huffman" -I"./huffpress/checksum" -c "./huffpress/huffpress.cpp" -o "$OBJDIR\huffpress.o"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build huffpress.o"
        exit $LASTEXITCODE
    }
    & $CXX $CXXTARGET -shared "$OBJDIR\huffpress.o" -o "$BINDIR\libhuffpress$LIBEXT" $LDFLAGS -lhuffman -lhuffchecksum
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build libhuffpress$LIBEXT"
        exit $LASTEXITCODE
    }

    # Build the huffpress CLI library
    & $CXX $CXXTARGET $CXXFLAGS $CXXWARNINGS $CXXPIC -I"./huffpress/huffman" -I"./huffpress/checksum" -I"./huffpress" -c "./huffpress/cli/cli.cpp" -o "$OBJDIR\huffpresscli.o"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build huffpresscli.o"
        exit $LASTEXITCODE
    }
    & $CXX $CXXTARGET -shared "$OBJDIR\huffpresscli.o" -o "$BINDIR\libhuffpresscli$LIBEXT" $LDFLAGS -lhuffman -lhuffchecksum -lhuffpress
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build libhuffpresscli$LIBEXT"
        exit $LASTEXITCODE
    }
}

function Build-Tests {
    Write-Host "Building tests..."

    # Build the framework
    & $CXX $CXXTARGET $CXXFLAGS $CXXWARNINGS $CXXPIC -c "$TESTDIR\framework\tinytest.c" -o "$OBJDIR\unit.framework.o"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build framework object file"
        exit $LASTEXITCODE
    }

    # Build the test executable
    & $CXX $CXXTARGET $CXXFLAGS $CXXWARNINGS $CXXPIC -I$TESTDIR -I"./huffpress" -c "$TESTDIR\unit.cpp" -o "$OBJDIR\unit.o"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build test object file"
        exit $LASTEXITCODE
    }

    & $CXX $CXXTARGET $CXXFLAGS $CXXWARNINGS $CXXPIC "$OBJDIR\unit.o" "$OBJDIR\unit.framework.o" -o "$BINDIR\unit$APPEXT" $LDFLAGS -lhuffman -lhuffchecksum -lhuffpress
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build test executable"
        exit $LASTEXITCODE
    }
}

function Build-Examples {
    Write-Host "Building examples..."

    & $CXX $CXXTARGET $CXXFLAGS $CXXWARNINGS $CXXPIC -I. -c "$EXAMPLESDIR\cli.cpp" -o "$OBJDIR\cli.o"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build example (cli) object file"
        exit $LASTEXITCODE
    }

    & $CXX $CXXTARGET $CXXFLAGS $CXXWARNINGS $CXXPIC "$OBJDIR\cli.o" -o "$BINDIR\cli$APPEXT" $LDFLAGS -lhuffman -lhuffchecksum -lhuffpress -lhuffpresscli
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to build example (cli) executable"
        exit $LASTEXITCODE
    }
}

function Clean-Build {
    Write-Host "Cleaning build files..."
    Remove-Item -Recurse -Force $OUTDIR
    Write-Host "Clean complete."
}

# Create directories if they don't exist
if (-not (Test-Path $OBJDIR)) {
    New-Item -ItemType Directory -Path $OBJDIR
}
if (-not (Test-Path $BINDIR)) {
    New-Item -ItemType Directory -Path $BINDIR
}

# Main logic based on arguments
switch ($ARG) {
    "only-lib" {
        Write-Host "Building libraries only..."
        Build-Libraries
    }
    "all" {
        Write-Host "Building all..."
        Build-Libraries
        Build-Tests
        Build-Examples
    }
    "only-test" {
        Write-Host "Building tests only..."
        Build-Tests
    }
    "only-examples" {
        Write-Host "Building examples only..."
        Build-Examples
    }
    "clean" {
        Write-Host "Cleaning build..."
        Clean-Build
    }
    default {
        Write-Host "Unknown option. Use one of: only-lib, all, only-test, only-examples, clean."
    }
}

Write-Host "Complete."
