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

void writeFile(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open file for writing: " + filePath);
    }

    if (!file.write(content.c_str(), content.size())) {
        throw std::ios_base::failure("Failed to write to file: " + filePath);
    }

    file.close();
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

    std::string sourceFilePath = "text.txt", destFilePath = "text.hpf", decompressedDestFilePath = "text.hpf.txt";

    Huffpress::HuffpressFile destFile(readFile(sourceFilePath)), decompressedDestFile;
    destFile.Serialize(destFilePath);

    size_t sourceFileSize = getFileSize(sourceFilePath), destFileSize = getFileSize(destFilePath), decompressedDestFileSize;

    printf("Source file (%s) size: %llu bytes\n", sourceFilePath.c_str(), (unsigned long long)sourceFileSize);
    printf("Dest file (%s) size: %llu bytes\n", destFilePath.c_str(), (unsigned long long)destFileSize);
    compareFileSizes(sourceFileSize, destFileSize);

    printf("\n\n");

    decompressedDestFile.Parse(destFilePath);

    writeFile(decompressedDestFilePath, decompressedDestFile.Decompress());
    decompressedDestFileSize = getFileSize(decompressedDestFilePath);

    printf("Source file (%s) size: %llu bytes\n", sourceFilePath.c_str(), (unsigned long long)sourceFileSize);
    printf("Decompressed dest file (%s) size %llu bytes\n", decompressedDestFilePath.c_str(), (unsigned long long)decompressedDestFileSize);
    compareFileSizes(sourceFileSize, decompressedDestFileSize);

    // std::remove(sourceFilePath.c_str());
    // std::remove(destFilePath.c_str());
    // std::remove(decompressedDestFilePath.c_str())

    return 0;
}
