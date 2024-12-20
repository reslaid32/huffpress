@echo off

REM Compiler
set CC=g++

REM Source files
set SOURCES=main.cpp ^
    huffpress/huffpress.cpp ^
    huffpress/huffman/huffman.cpp ^
    huffpress/checksum/checksum.c

REM Output file
set OUTPUT=bin\out.exe

REM Create the bin directory if it doesn't exist
if not exist bin (
    mkdir bin
)

REM Compile
%CC% %SOURCES% -o %OUTPUT%

REM Check if the compilation succeeded
if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b %errorlevel%
) else (
    echo Compilation successful. Output: %OUTPUT%
)
