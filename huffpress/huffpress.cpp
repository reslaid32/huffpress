#include "huffpress.h"
#include "exceptions.h"
#include <fstream>
#include <iostream>
#include <cstring>

namespace Huffpress {

    HuffpressFile::HuffpressFile(const std::string& data) {
        this->Init(data);
    }

    void HuffpressFile::Init(const std::string& data) {
        this->byteVec = Huffman::Compress(data, this->freqMap, this->bitLength);
        this->size = this->byteVec.size();
        this->sourceChecksum = checksum(data.c_str(), data.size());
        this->compressedChecksum = checksum(reinterpret_cast<char*>(this->byteVec.data()), this->size);
    }

    void HuffpressFile::Serialize(const std::string& filePath) {
        std::ofstream out(filePath, std::ios::binary);
        if (!out) {
            throw Exceptions::FileOpenException(filePath);
        }

        try {
            out.write(this->magic, sizeof(this->magic));
            out.write(reinterpret_cast<const char*>(this->version), sizeof(this->version));
            
            size_t freqMapSize = this->freqMap.size();
            out.write(reinterpret_cast<const char*>(&freqMapSize), sizeof(freqMapSize));

            for (const auto& pair : this->freqMap) {
                out.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first)); 
                out.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
            }

            out.write(reinterpret_cast<const char*>(&this->bitLength), sizeof(this->bitLength));

            out.write(reinterpret_cast<const char*>(&this->size), sizeof(this->size));
            out.write(reinterpret_cast<const char*>(&this->sourceChecksum), sizeof(this->sourceChecksum));
            out.write(reinterpret_cast<const char*>(&this->compressedChecksum), sizeof(this->compressedChecksum));

            out.write(reinterpret_cast<const char*>(this->byteVec.data()), this->size);

            out.close();
        } catch (const std::exception& e) {
            throw Exceptions::SerializationException(e.what());
        }
    }

    void HuffpressFile::SerializeToBuffer(Huffman::ByteVector& buffer) {
        try {
            buffer.clear();

            buffer.insert(buffer.end(), this->magic, this->magic + sizeof(this->magic));
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(this->version), reinterpret_cast<const char*>(this->version) + sizeof(this->version));

            size_t freqMapSize = this->freqMap.size();
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&freqMapSize), reinterpret_cast<const char*>(&freqMapSize) + sizeof(freqMapSize));

            for (const auto& pair : this->freqMap) {
                buffer.insert(buffer.end(), reinterpret_cast<const char*>(&pair.first), reinterpret_cast<const char*>(&pair.first) + sizeof(pair.first));
                buffer.insert(buffer.end(), reinterpret_cast<const char*>(&pair.second), reinterpret_cast<const char*>(&pair.second) + sizeof(pair.second));
            }

            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&this->bitLength), reinterpret_cast<const char*>(&this->bitLength) + sizeof(this->bitLength));

            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&this->size), reinterpret_cast<const char*>(&this->size) + sizeof(this->size));
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&this->sourceChecksum), reinterpret_cast<const char*>(&this->sourceChecksum) + sizeof(this->sourceChecksum));
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&this->compressedChecksum), reinterpret_cast<const char*>(&this->compressedChecksum) + sizeof(this->compressedChecksum));

            buffer.insert(buffer.end(), this->byteVec.begin(), this->byteVec.end());
        } catch (const std::exception& e) {
            throw Exceptions::SerializationException(e.what());
        }
    }

    void HuffpressFile::Parse(const std::string& filePath) {
        std::ifstream in(filePath, std::ios::binary);
        if (!in) {
            throw Exceptions::FileOpenException(filePath);
        }
        try {
            in.read(this->magic, sizeof(this->magic));
            in.read(reinterpret_cast<char*>(this->version), sizeof(this->version));

            size_t freqMapSize;
            in.read(reinterpret_cast<char*>(&freqMapSize), sizeof(freqMapSize));

            this->freqMap.clear();
            for (size_t i = 0; i < freqMapSize; ++i) {
                char key;
                int value;
                in.read(reinterpret_cast<char*>(&key), sizeof(key));
                in.read(reinterpret_cast<char*>(&value), sizeof(value));
                this->freqMap[key] = value;
            }

            in.read(reinterpret_cast<char*>(&this->bitLength), sizeof(this->bitLength));

            in.read(reinterpret_cast<char*>(&this->size), sizeof(this->size));
            in.read(reinterpret_cast<char*>(&this->sourceChecksum), sizeof(this->sourceChecksum));
            in.read(reinterpret_cast<char*>(&this->compressedChecksum), sizeof(this->compressedChecksum));

            this->byteVec.resize(this->size);
            in.read(reinterpret_cast<char*>(this->byteVec.data()), this->size);

            in.close();
        } catch (const std::exception& e) {
            throw Exceptions::DeserializationException(e.what());
        }
    }
    
    void HuffpressFile::ParseFromBuffer(const Huffman::ByteVector& buffer) {
        try {
            size_t offset = 0;

            std::memcpy(this->magic, buffer.data() + offset, sizeof(this->magic));
            offset += sizeof(this->magic);

            std::memcpy(this->version, buffer.data() + offset, sizeof(this->version));
            offset += sizeof(this->version);

            size_t freqMapSize;
            std::memcpy(&freqMapSize, buffer.data() + offset, sizeof(freqMapSize));
            offset += sizeof(freqMapSize);

            this->freqMap.clear();
            for (size_t i = 0; i < freqMapSize; ++i) {
                char key;
                int value;
                std::memcpy(&key, buffer.data() + offset, sizeof(key));
                offset += sizeof(key);

                std::memcpy(&value, buffer.data() + offset, sizeof(value));
                offset += sizeof(value);

                this->freqMap[key] = value;
            }

            std::memcpy(&this->bitLength, buffer.data() + offset, sizeof(this->bitLength));
            offset += sizeof(this->bitLength);

            std::memcpy(&this->size, buffer.data() + offset, sizeof(this->size));
            offset += sizeof(this->size);

            std::memcpy(&this->sourceChecksum, buffer.data() + offset, sizeof(this->sourceChecksum));
            offset += sizeof(this->sourceChecksum);

            std::memcpy(&this->compressedChecksum, buffer.data() + offset, sizeof(this->compressedChecksum));
            offset += sizeof(this->compressedChecksum);

            this->byteVec.resize(this->size);
            std::memcpy(this->byteVec.data(), buffer.data() + offset, this->size);
        } catch (const std::exception& e) {
            throw Exceptions::DeserializationException(e.what());
        }
    }

    void HuffpressFile::Modify(const std::string& data) {
        this->byteVec = Huffman::Compress(data, this->freqMap, this->bitLength);
        this->size = this->byteVec.size();
        this->sourceChecksum = checksum(data.c_str(), data.size());
        this->compressedChecksum = checksum(reinterpret_cast<char*>(this->byteVec.data()), this->size);
    }

    void HuffpressFile::Modify(const Huffman::ByteVector& newByteVec, const Huffman::FreqMap& newFreqMap, size_t bitLength) {
        this->byteVec = newByteVec;
        this->size = this->byteVec.size();
        this->freqMap = newFreqMap;
        std::string decompressed = Huffman::Decompress(this->byteVec, this->freqMap, bitLength);
        this->sourceChecksum = checksum(decompressed.c_str(), decompressed.size());
        this->compressedChecksum = checksum(reinterpret_cast<char*>(this->byteVec.data()), this->size);
    }

    std::string HuffpressFile::Decompress() {
        return Huffman::Decompress(this->byteVec, this->freqMap, this->bitLength);
    }
} // Huffpress