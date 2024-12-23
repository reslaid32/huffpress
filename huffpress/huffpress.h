#ifndef HUFFPRESS_H
#define HUFFPRESS_H

#include "export.h"

#include "checksum/checksum.h"
#include "huffman/huffman.h"

#include "exceptions.h"

namespace Huffpress {

    const uint8_t Version[3] = {0, 1, 2};

    class HUFFPRESS_API HuffpressFile
    {
    public:
        HuffpressFile() = default;
        HUFFPRESS_API HuffpressFile(const std::string& data);

        // Initialize the structure by data
        HUFFPRESS_API void Init(const std::string& data);
        // Serialize to a file
        HUFFPRESS_API void Serialize(const std::string& filePath);
        // Serialize to a file (buffered writing, default buffer size 64 KB)
        HUFFPRESS_API void BufferedSerialize(const std::string& filePath, const size_t bufferSize = 64 * 1024);
        // Serialize to a buffer
        HUFFPRESS_API void SerializeToBuffer(Huffman::ByteVector& buffer);
        // Deserialize from a file
        HUFFPRESS_API void Parse(const std::string& filePath);
        // Desrialize from a file (buffered reading, default buffer size 64 KB)
        HUFFPRESS_API void BufferedParse(const std::string& filePath, const size_t bufferSize = 64 * 1024);
        // Deserialize from a buffer
        HUFFPRESS_API void ParseFromBuffer(const Huffman::ByteVector& buffer);
        // Modify the file's data by compressing the new string data
        HUFFPRESS_API void Modify(const std::string& data);
        // Modify the file's data by directly setting the new byte vector and frequency map
        HUFFPRESS_API void Modify(const Huffman::ByteVector& newByteVec, const Huffman::FreqMap& newFreqMap, size_t bitLength);
        // Get decompressed buffer
        HUFFPRESS_API std::string Decompress();

    public:
        struct _HuffpressFileHeader {
            // Magic number for identifying Huffpress files ("HPF")
            // Used to ensure the file is in the correct format when parsing
            char magic[3] = {'H', 'P', 'F'};

            // Version of the file format (major, minor)
            // Helps in managing backward compatibility for future versions of the format
            uint8_t version[3] = {Huffpress::Version[0], Huffpress::Version[1], Huffpress::Version[2]};

            // Frequency map used for compression
            // Stores the frequency of each character in the original data for Huffman encoding
            Huffman::FreqMap freqMap;

            // Bit length of the compressed data
            // Represents the total number of bits used in the compressed byte vector
            size_t bitLength = 0;

            // Size of the byte vector (compressed data)
            // Used to track the size of the compressed data
            size_t size = 0;

            // Checksum of the original (source) data
            // Used for verifying the integrity of the source data
            checksum_t sourceChecksum = 0;

            // Checksum of the compressed (byte) data
            // Used for verifying the integrity of the compressed data
            checksum_t compressedChecksum = 0;
        };

        // File header
        _HuffpressFileHeader header;

        // The compressed byte vector
        // Stores the actual compressed data in bytes
        Huffman::ByteVector byteVec;
    };    
} // Huffpress
#endif // HUFFPRESS_H