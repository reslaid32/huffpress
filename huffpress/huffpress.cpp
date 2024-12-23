#define HUFFPRESS_LIBRARY_BUILD

#include "huffpress.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>

namespace Huffpress {

    HUFFPRESS_API HuffpressFile::HuffpressFile(const std::string& data) {
        this->Init(data);
    }

    HUFFPRESS_API void HuffpressFile::Init(const std::string& data) {
        this->byteVec = Huffman::Compress(data, this->header.freqMap, this->header.bitLength);
        this->header.size = this->byteVec.size();
        this->header.sourceChecksum = checksum(data.c_str(), data.size());
        this->header.compressedChecksum = checksum(reinterpret_cast<char*>(this->byteVec.data()), this->header.size);
    }

    HUFFPRESS_API void HuffpressFile::Serialize(const std::string& filePath) {
        std::ofstream out(filePath, std::ios::binary);
        if (!out) {
            throw Exceptions::FileOpenException(filePath);
        }

        try {
            out.write(this->header.magic, sizeof(this->header.magic));

            out.write(reinterpret_cast<const char*>(this->header.version), sizeof(this->header.version));
            
            size_t freqMapSize = this->header.freqMap.size();
            out.write(reinterpret_cast<const char*>(&freqMapSize), sizeof(freqMapSize));

            for (const auto& pair : this->header.freqMap) {
                out.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first)); 
                out.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
            }

            out.write(reinterpret_cast<const char*>(&this->header.bitLength), sizeof(this->header.bitLength));

            out.write(reinterpret_cast<const char*>(&this->header.size), sizeof(this->header.size));
            out.write(reinterpret_cast<const char*>(&this->header.sourceChecksum), sizeof(this->header.sourceChecksum));
            out.write(reinterpret_cast<const char*>(&this->header.compressedChecksum), sizeof(this->header.compressedChecksum));

            out.write(reinterpret_cast<const char*>(this->byteVec.data()), this->header.size);

            out.close();
        } catch (const std::exception& e) {
            throw Exceptions::SerializationException(e.what());
        }
    }

    HUFFPRESS_API void HuffpressFile::BufferedSerialize(const std::string& filePath, const size_t bufferSize) {
        std::ofstream out(filePath, std::ios::binary);
        if (!out) {
            throw Exceptions::FileOpenException(filePath);
        }

        try {
            std::vector<char> buffer(bufferSize);
            size_t bufferPos = 0;

            auto writeBuffer = [&out, &buffer, &bufferPos]() {
                if (bufferPos > 0) {
                    out.write(buffer.data(), bufferPos);
                    bufferPos = 0;
                }
            };

            std::memcpy(buffer.data(), this->header.magic, sizeof(this->header.magic));
            bufferPos += sizeof(this->header.magic);
            writeBuffer();

            std::memcpy(buffer.data(), this->header.version, sizeof(this->header.version));
            bufferPos += sizeof(this->header.version);
            writeBuffer();

            size_t freqMapSize = this->header.freqMap.size();
            std::memcpy(buffer.data(), &freqMapSize, sizeof(freqMapSize));
            bufferPos += sizeof(freqMapSize);
            writeBuffer();

            for (const auto& pair : this->header.freqMap) {
                std::memcpy(buffer.data(), &pair.first, sizeof(pair.first));
                bufferPos += sizeof(pair.first);
                std::memcpy(buffer.data() + bufferPos, &pair.second, sizeof(pair.second));
                bufferPos += sizeof(pair.second);
                writeBuffer();
            }

            std::memcpy(buffer.data(), &this->header.bitLength, sizeof(this->header.bitLength));
            bufferPos += sizeof(this->header.bitLength);
            writeBuffer();

            std::memcpy(buffer.data(), &this->header.size, sizeof(this->header.size));
            bufferPos += sizeof(this->header.size);
            writeBuffer();

            std::memcpy(buffer.data(), &this->header.sourceChecksum, sizeof(this->header.sourceChecksum));
            bufferPos += sizeof(this->header.sourceChecksum);
            writeBuffer();

            std::memcpy(buffer.data(), &this->header.compressedChecksum, sizeof(this->header.compressedChecksum));
            bufferPos += sizeof(this->header.compressedChecksum);
            writeBuffer();

            std::memcpy(buffer.data(), this->byteVec.data(), this->header.size);
            bufferPos += this->header.size;
            writeBuffer();

            out.close();
        } catch (const std::exception& e) {
            throw Exceptions::SerializationException(e.what());
        }
    }

    HUFFPRESS_API void HuffpressFile::SerializeToBuffer(Huffman::ByteVector& buffer) {
        try {
            buffer.clear();

            buffer.insert(buffer.end(), this->header.magic, this->header.magic + sizeof(this->header.magic));
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(this->header.version), reinterpret_cast<const char*>(this->header.version) + sizeof(this->header.version));

            size_t freqMapSize = this->header.freqMap.size();
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&freqMapSize), reinterpret_cast<const char*>(&freqMapSize) + sizeof(freqMapSize));

            for (const auto& pair : this->header.freqMap) {
                buffer.insert(buffer.end(), reinterpret_cast<const char*>(&pair.first), reinterpret_cast<const char*>(&pair.first) + sizeof(pair.first));
                buffer.insert(buffer.end(), reinterpret_cast<const char*>(&pair.second), reinterpret_cast<const char*>(&pair.second) + sizeof(pair.second));
            }

            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&this->header.bitLength), reinterpret_cast<const char*>(&this->header.bitLength) + sizeof(this->header.bitLength));

            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&this->header.size), reinterpret_cast<const char*>(&this->header.size) + sizeof(this->header.size));
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&this->header.sourceChecksum), reinterpret_cast<const char*>(&this->header.sourceChecksum) + sizeof(this->header.sourceChecksum));
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&this->header.compressedChecksum), reinterpret_cast<const char*>(&this->header.compressedChecksum) + sizeof(this->header.compressedChecksum));

            buffer.insert(buffer.end(), this->byteVec.begin(), this->byteVec.end());
        } catch (const std::exception& e) {
            throw Exceptions::SerializationException(e.what());
        }
    }

    HUFFPRESS_API void HuffpressFile::Parse(const std::string& filePath) {
        std::ifstream in(filePath, std::ios::binary);
        if (!in) {
            throw Exceptions::FileOpenException(filePath);
        }
        try {
            in.read(this->header.magic, sizeof(this->header.magic));
            in.read(reinterpret_cast<char*>(this->header.version), sizeof(this->header.version));

            size_t freqMapSize;
            in.read(reinterpret_cast<char*>(&freqMapSize), sizeof(freqMapSize));

            this->header.freqMap.clear();
            for (size_t i = 0; i < freqMapSize; ++i) {
                char key;
                int value;
                in.read(reinterpret_cast<char*>(&key), sizeof(key));
                in.read(reinterpret_cast<char*>(&value), sizeof(value));
                this->header.freqMap[key] = value;
            }

            in.read(reinterpret_cast<char*>(&this->header.bitLength), sizeof(this->header.bitLength));

            in.read(reinterpret_cast<char*>(&this->header.size), sizeof(this->header.size));
            in.read(reinterpret_cast<char*>(&this->header.sourceChecksum), sizeof(this->header.sourceChecksum));
            in.read(reinterpret_cast<char*>(&this->header.compressedChecksum), sizeof(this->header.compressedChecksum));

            this->byteVec.resize(this->header.size);
            in.read(reinterpret_cast<char*>(this->byteVec.data()), this->header.size);

            in.close();
        } catch (const std::exception& e) {
            throw Exceptions::DeserializationException(e.what());
        }
    }

    HUFFPRESS_API void HuffpressFile::BufferedParse(const std::string& filePath, const size_t bufferSize) {
        std::ifstream in(filePath, std::ios::binary);
        if (!in) {
            throw Exceptions::FileOpenException(filePath);
        }

        try {
            std::vector<char> buffer(bufferSize);
            size_t bufferPos = 0;

            auto readBuffer = [&in, &buffer, &bufferPos, bufferSize]() {
                if (bufferPos == 0 || bufferPos == bufferSize) {
                    in.read(buffer.data(), bufferSize);
                    bufferPos = 0;
                }
            };

            readBuffer();
            std::memcpy(this->header.magic, buffer.data() + bufferPos, sizeof(this->header.magic));
            bufferPos += sizeof(this->header.magic);

            readBuffer();
            std::memcpy(this->header.version, buffer.data() + bufferPos, sizeof(this->header.version));
            bufferPos += sizeof(this->header.version);

            size_t freqMapSize;
            readBuffer();
            std::memcpy(&freqMapSize, buffer.data() + bufferPos, sizeof(freqMapSize));
            bufferPos += sizeof(freqMapSize);

            this->header.freqMap.clear();
            for (size_t i = 0; i < freqMapSize; ++i) {
                char key;
                int value;
                readBuffer();
                std::memcpy(&key, buffer.data() + bufferPos, sizeof(key));
                bufferPos += sizeof(key);
                readBuffer();
                std::memcpy(&value, buffer.data() + bufferPos, sizeof(value));
                bufferPos += sizeof(value);
                this->header.freqMap[key] = value;
            }

            readBuffer();
            std::memcpy(&this->header.bitLength, buffer.data() + bufferPos, sizeof(this->header.bitLength));
            bufferPos += sizeof(this->header.bitLength);

            readBuffer();
            std::memcpy(&this->header.size, buffer.data() + bufferPos, sizeof(this->header.size));
            bufferPos += sizeof(this->header.size);

            readBuffer();
            std::memcpy(&this->header.sourceChecksum, buffer.data() + bufferPos, sizeof(this->header.sourceChecksum));
            bufferPos += sizeof(this->header.sourceChecksum);

            readBuffer();
            std::memcpy(&this->header.compressedChecksum, buffer.data() + bufferPos, sizeof(this->header.compressedChecksum));
            bufferPos += sizeof(this->header.compressedChecksum);

            this->byteVec.resize(this->header.size);
            readBuffer();
            std::memcpy(this->byteVec.data(), buffer.data() + bufferPos, this->header.size);
            bufferPos += this->header.size;

            in.close();
        } catch (const std::exception& e) {
            throw Exceptions::DeserializationException(e.what());
        }
    }
        
    HUFFPRESS_API void HuffpressFile::ParseFromBuffer(const Huffman::ByteVector& buffer) {
        try {
            size_t offset = 0;

            std::memcpy(this->header.magic, buffer.data() + offset, sizeof(this->header.magic));
            offset += sizeof(this->header.magic);

            std::memcpy(this->header.version, buffer.data() + offset, sizeof(this->header.version));
            offset += sizeof(this->header.version);

            size_t freqMapSize;
            std::memcpy(&freqMapSize, buffer.data() + offset, sizeof(freqMapSize));
            offset += sizeof(freqMapSize);

            this->header.freqMap.clear();
            for (size_t i = 0; i < freqMapSize; ++i) {
                char key;
                int value;
                std::memcpy(&key, buffer.data() + offset, sizeof(key));
                offset += sizeof(key);

                std::memcpy(&value, buffer.data() + offset, sizeof(value));
                offset += sizeof(value);

                this->header.freqMap[key] = value;
            }

            std::memcpy(&this->header.bitLength, buffer.data() + offset, sizeof(this->header.bitLength));
            offset += sizeof(this->header.bitLength);

            std::memcpy(&this->header.size, buffer.data() + offset, sizeof(this->header.size));
            offset += sizeof(this->header.size);

            std::memcpy(&this->header.sourceChecksum, buffer.data() + offset, sizeof(this->header.sourceChecksum));
            offset += sizeof(this->header.sourceChecksum);

            std::memcpy(&this->header.compressedChecksum, buffer.data() + offset, sizeof(this->header.compressedChecksum));
            offset += sizeof(this->header.compressedChecksum);

            this->byteVec.resize(this->header.size);
            std::memcpy(this->byteVec.data(), buffer.data() + offset, this->header.size);
        } catch (const std::exception& e) {
            throw Exceptions::DeserializationException(e.what());
        }
    }

    HUFFPRESS_API void HuffpressFile::Modify(const std::string& data) {
        this->header.freqMap.clear();
        this->byteVec = Huffman::Compress(data, this->header.freqMap, this->header.bitLength);
        this->header.size = this->byteVec.size();
        this->header.sourceChecksum = checksum(data.c_str(), data.size());
        this->header.compressedChecksum = checksum(reinterpret_cast<char*>(this->byteVec.data()), this->header.size);
    }

    HUFFPRESS_API void HuffpressFile::Modify(const Huffman::ByteVector& newByteVec, const Huffman::FreqMap& newFreqMap, size_t bitLength) {
        this->byteVec = newByteVec;
        this->header.size = this->byteVec.size();
        this->header.freqMap = newFreqMap;
        std::string decompressed = Huffman::Decompress(this->byteVec, this->header.freqMap, bitLength);
        this->header.sourceChecksum = checksum(decompressed.c_str(), decompressed.size());
        this->header.compressedChecksum = checksum(reinterpret_cast<char*>(this->byteVec.data()), this->header.size);
    }

    HUFFPRESS_API std::string HuffpressFile::Decompress() {
        return Huffman::Decompress(this->byteVec, this->header.freqMap, this->header.bitLength);
    }
} // Huffpress