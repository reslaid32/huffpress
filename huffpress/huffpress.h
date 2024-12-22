#include "checksum/checksum.h"
#include "huffman/huffman.h"

namespace Huffpress {

    class HuffpressFile
    {
    public:
        HuffpressFile() = default;
        HuffpressFile(const std::string& data);

        // Initialize the structure by data
        void Init(const std::string& data);
        // Serialize to a file
        void Serialize(const std::string& filePath);
        // Serialize to a buffer
        void SerializeToBuffer(Huffman::ByteVector& buffer);
        // Deserialize from a file
        void Parse(const std::string& filePath);
        // Deserialize from a buffer
        void ParseFromBuffer(const Huffman::ByteVector& buffer);
        // Modify the file's data by compressing the new string data
        void Modify(const std::string& data);
        // Modify the file's data by directly setting the new byte vector and frequency map
        void Modify(const Huffman::ByteVector& newByteVec, const Huffman::FreqMap& newFreqMap, size_t bitLength);
        // Get decompressed buffer
        std::string Decompress();

    public:
        // Magic number for identifying Huffpress files ("HPF")
        // Used to ensure the file is in the correct format when parsing
        char magic[3] = {'H', 'P', 'F'};

        // Version of the file format (major, minor)
        // Helps in managing backward compatibility for future versions of the format
        uint8_t version[3] = {0, 1, 0};

        // Frequency map used for compression
        // Stores the frequency of each character in the original data for Huffman encoding
        Huffman::FreqMap freqMap;

        // Bit length of the compressed data
        // Represents the total number of bits used in the compressed byte vector
        size_t bitLength;

        // Size of the byte vector (compressed data)
        // Used to track the size of the compressed data
        size_t size = 0;

        // The compressed byte vector
        // Stores the actual compressed data in bytes
        Huffman::ByteVector byteVec;

        // Checksum of the original (source) data
        // Used for verifying the integrity of the source data
        checksum_t sourceChecksum;

        // Checksum of the compressed (byte) data
        // Used for verifying the integrity of the compressed data
        checksum_t compressedChecksum;
    };
} // Huffpress