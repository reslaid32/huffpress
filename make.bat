@echo off
setlocal

:: Define the output directory
set OUTDIR=build
set OBJDIR=%OUTDIR%\obj
set BINDIR=%OUTDIR%\bin
set TESTDIR=tests
set TESTOUTDIR=%OUTDIR%\%TESTDIR%
set TESTOBJDIR=%TESTOUTDIR%\obj

:: Set the compiler and flags
set CC=clang
set CXX=clang++
set CXXFLAGS=-std=c++11 -Wall -fPIC
set LDFLAGS=-L.\%BINDIR%

:: Check for command-line argument (default is "all")
set ARG=%1
if "%ARG%"=="" set ARG=only-lib

:: Default: 1 (build)
:: 1 = build
:: 2 = clean
set OP_TYPE=1

:: Create output directory if it doesn't exist
if not exist %OUTDIR% mkdir %OUTDIR%
if not exist %OBJDIR% mkdir %OBJDIR%
if not exist %BINDIR% mkdir %BINDIR%

:: Handle the 'all' case
if %ARG%=="all" (
    echo Building all...
    call :build_libraries
    call :build_tests
    goto end
)

:: Default case: 'any-lib' (build only lib)
:: Handle the 'only-lib' case
if "%ARG%"=="only-lib" (
    echo Building libraries only...
    call :build_libraries
    goto end
)

:: Handle the 'only-test' case
if "%ARG%"=="only-test" (
    echo Building tests only...
    call :build_tests
    goto end
)

:: Handle the 'clean' case
if "%ARG%"=="clean" (
    :: Set operation type to 2 (clean)
    set OP_TYPE=2
    echo Cleaning build...
    call :clean_build
    goto end
)

:: Build libraries section
:build_libraries
:: Build the huffman DLL
echo Building huffman library...
%CXX% %CXXFLAGS% -I.\huffpress\huffman -c .\huffpress\huffman\huffman.cpp -o %OBJDIR%\huffman.o
if %ERRORLEVEL% neq 0 (
    echo Failed to build huffman.o
    exit /b %ERRORLEVEL%
)
%CXX% -shared %OBJDIR%\huffman.o -o %BINDIR%\huffman.dll
if %ERRORLEVEL% neq 0 (
    echo Failed to build huffman.dll
    exit /b %ERRORLEVEL%
)

:: Build the checksum DLL
echo Building checksum library...
%CXX% -c .\huffpress\checksum\checksum.c -o %OBJDIR%\checksum.o
if %ERRORLEVEL% neq 0 (
    echo Failed to build checksum.o
    exit /b %ERRORLEVEL%
)
%CXX% -shared %OBJDIR%\checksum.o -o %BINDIR%\checksum.dll
if %ERRORLEVEL% neq 0 (
    echo Failed to build checksum.dll
    exit /b %ERRORLEVEL%
)

:: Build the huffpress DLL, linking with huffman and checksum
echo Building huffpress library...
%CXX% %CXXFLAGS% -I.\huffpress\huffman -I.\huffpress\checksum -c .\huffpress\huffpress.cpp -o %OBJDIR%\huffpress.o
if %ERRORLEVEL% neq 0 (
    echo Failed to build huffpress.o
    exit /b %ERRORLEVEL%
)
%CXX% -shared %OBJDIR%\huffpress.o -o %BINDIR%\huffpress.dll %LDFLAGS% -lhuffman -lchecksum
if %ERRORLEVEL% neq 0 (
    echo Failed to build huffpress.dll
    exit /b %ERRORLEVEL%
)

:: End of library building
goto :eof

:: Build tests section
:build_tests
echo Building tests.

:: Build the framework
echo Building framework...
%CXX% %CXXFLAGS% -c .\%TESTDIR%\framework\tinytest.c -o %OBJDIR%\unit.framework.o
if %ERRORLEVEL% neq 0 (
    echo Failed to build framework object file
    exit /b %ERRORLEVEL%
)

:: Build the test executable
echo Building test executable...
%CXX% %CXXFLAGS% -I.%TESTDIR% -I.\huffpress -c .\%TESTDIR%\unit.cpp -o %OBJDIR%\unit.o
if %ERRORLEVEL% neq 0 (
    echo Failed to build test object file
    exit /b %ERRORLEVEL%
)

:: Build the test executable, linking with the framework and the libraries
%CXX% %CXXFLAGS% %OBJDIR%\unit.o %OBJDIR%\unit.framework.o -o %BINDIR%\unit.exe %LDFLAGS% -lhuffman -lchecksum -lhuffpress
if %ERRORLEVEL% neq 0 (
    echo Failed to build test executable
    exit /b %ERRORLEVEL%
)

:: End of test building
goto :eof

:: Clean build files (removes object files, libraries, executables, and the build directory)
:clean_build
echo Removing build files...

:: Remove the entire build directory
rd /s /q %OUTDIR%

echo Clean complete.
goto :eof

:: End
:end
if %OP_TYPE% equ 1 ( echo Build complete. )
endlocal
