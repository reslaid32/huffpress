#include "huffpress.h"
#include <fstream>
#include <iostream>

namespace Huffpress {
    HuffpressFile* InitializeFile() {
        HuffpressFile* file = new HuffpressFile;
        return file;
    }

    HuffpressFile* InitializeFile(const std::string& data) {
        HuffpressFile* file = new HuffpressFile;
        file->byteVec = Huffman::Compress(data, file->freqMap, file->bitLength);
        file->size = file->byteVec.size();
        file->sourceChecksum = checksum(data.c_str(), data.size());
        file->compressedChecksum = checksum(reinterpret_cast<char*>(file->byteVec.data()), file->size);
        return file;
    }

    void FreeFile(HuffpressFile* file) {
        if (!file) { return; }
        delete file;
    }

    void Serialize(const HuffpressFile* file, const std::string& filePath) {
        if (!file) { return; }

        std::ofstream out(filePath, std::ios::binary);
        
        if (!out) {
            std::cerr << "Error opening file for writing" << std::endl;
            return;
        }

        out.write(file->magic, sizeof(file->magic));
        out.write(reinterpret_cast<const char*>(file->version), sizeof(file->version));
        
        size_t freqMapSize = file->freqMap.size();
        out.write(reinterpret_cast<const char*>(&freqMapSize), sizeof(freqMapSize));

        for (const auto& pair : file->freqMap) {
            out.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first)); 
            out.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
        }

        out.write(reinterpret_cast<const char*>(&file->bitLength), sizeof(file->bitLength));

        out.write(reinterpret_cast<const char*>(&file->size), sizeof(file->size));
        out.write(reinterpret_cast<const char*>(&file->sourceChecksum), sizeof(file->sourceChecksum));
        out.write(reinterpret_cast<const char*>(&file->compressedChecksum), sizeof(file->compressedChecksum));

        out.write(reinterpret_cast<const char*>(file->byteVec.data()), file->size);

        out.close();
    }

    HuffpressFile* ParseFile(const std::string& filePath) {
        std::ifstream in(filePath, std::ios::binary);

        if (!in) {
            std::cerr << "Error opening file for reading" << std::endl;
            return nullptr;
        }

        HuffpressFile* file = new HuffpressFile;

        in.read(file->magic, sizeof(file->magic));
        in.read(reinterpret_cast<char*>(file->version), sizeof(file->version));

        size_t freqMapSize;
        in.read(reinterpret_cast<char*>(&freqMapSize), sizeof(freqMapSize));

        file->freqMap.clear();
        for (size_t i = 0; i < freqMapSize; ++i) {
            char key;
            int value;
            in.read(reinterpret_cast<char*>(&key), sizeof(key));
            in.read(reinterpret_cast<char*>(&value), sizeof(value));
            file->freqMap[key] = value;
        }

        in.read(reinterpret_cast<char*>(&file->bitLength), sizeof(file->bitLength));

        in.read(reinterpret_cast<char*>(&file->size), sizeof(file->size));
        in.read(reinterpret_cast<char*>(&file->sourceChecksum), sizeof(file->sourceChecksum));
        in.read(reinterpret_cast<char*>(&file->compressedChecksum), sizeof(file->compressedChecksum));

        file->byteVec.resize(file->size);
        in.read(reinterpret_cast<char*>(file->byteVec.data()), file->size);

        in.close();
        return file;
    }

    void Modify(HuffpressFile* file, const std::string& data) {
        if (!file) { return; }
        file->byteVec = Huffman::Compress(data, file->freqMap, file->bitLength);
        file->size = file->byteVec.size();
        file->sourceChecksum = checksum(data.c_str(), data.size());
        file->compressedChecksum = checksum(reinterpret_cast<char*>(file->byteVec.data()), file->size);
    }

    void Modify(HuffpressFile* file, const Huffman::ByteVector& newByteVec, const Huffman::FreqMap& newFreqMap, size_t bitLength) {
        if (!file) { return; }
        file->byteVec = newByteVec;
        file->size = file->byteVec.size();
        file->freqMap = newFreqMap;
        std::string decompressed = Huffman::Decompress(file->byteVec, file->freqMap, bitLength);
        file->sourceChecksum = checksum(decompressed.c_str(), decompressed.size());
        file->compressedChecksum = checksum(reinterpret_cast<char*>(file->byteVec.data()), file->size);
    }
} // Huffpress