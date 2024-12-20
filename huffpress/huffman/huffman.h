#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <map>
#include <vector>
#include <string>
#include <cstdint>

namespace Huffman {
    using Char = char;
    using Int = std::int32_t;
    using Byte = std::uint8_t;
    using ByteVector = std::vector<Byte>;
    using FreqMap = std::map<Char, Int>;

    struct HuffmanNode {
        char data;
        int freq;
        HuffmanNode *left, *right;

        HuffmanNode(char data, int freq);
    };

    struct HuffmanCompare {
        bool operator()(HuffmanNode* left, HuffmanNode* right);
    };

    ByteVector Compress(const std::string& text, FreqMap& freqMap, size_t& bitLength);

    std::string Decompress(const ByteVector& compressed, const FreqMap& freqMap, size_t bitLength);

    namespace Methods {
        HuffmanNode* BuildHuffmanTree(const FreqMap& freqMap);

        void GenerateCodes(HuffmanNode* node, const std::string& code, std::map<Char, std::string>& huffmanCode);

        ByteVector PackBitsToBytes(const std::string& bitString, size_t& bitLength);

        std::string UnpackBytesToBits(const ByteVector& compressedData, size_t bitLength);

        void FreeTree(HuffmanNode* node);
    }

    namespace Stringize {
        std::string StringizeFreqMap(const Huffman::FreqMap& freqMap);
        std::string StringizeByteVec(const Huffman::ByteVector& byteVec);
    }
}

#endif // HUFFMAN_H