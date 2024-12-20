#include "huffpress/huffpress.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open file: " + filePath);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string content(size, '\0');
    if (!file.read(&content[0], size)) {
        throw std::ios_base::failure("Failed to read file: " + filePath);
    }

    return content;
}

size_t getFileSize(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file.");
    }
    return file.tellg();
}

void compareFileSizes(size_t size1, size_t size2) {
    if (size1 == size2) {
        std::cout << "Both files are the same size.\n";
        return;
    }

    size_t larger = (size1 > size2) ? size1 : size2;
    size_t smaller = (size1 < size2) ? size1 : size2;
    double percentageSmaller = 100.0 - (static_cast<double>(smaller) / larger * 100.0);

    std::cout << "The smaller file is " << percentageSmaller << "% smaller than the larger file.\n";
}

int main() {

    std::string sourceFilePath = "text.txt";
    std::string destFilePath = "text.hpf";

    Huffpress::HuffpressFile *destFile = Huffpress::InitializeFile(readFile(sourceFilePath));
    Huffpress::Serialize(destFile, destFilePath);

    size_t sourceFileSize = getFileSize(sourceFilePath), destFileSize = getFileSize(destFilePath);

    printf("Source file (%s) size: %llu bytes\n", sourceFilePath.c_str(), (unsigned long long)sourceFileSize);
    printf("Dest file (%s) size: %llu bytes\n", destFilePath.c_str(), (unsigned long long)destFileSize);
    compareFileSizes(sourceFileSize, destFileSize);

    // std::remove(sourceFilePath.c_str());
    // std::remove(destFilePath.c_str());

    return 0;
}
