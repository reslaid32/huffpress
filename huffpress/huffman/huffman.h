#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "export.h"

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

        HUFFMAN_API HuffmanNode(char data, int freq);
    };

    struct HuffmanCompare {
        HUFFMAN_API bool operator()(HuffmanNode* left, HuffmanNode* right);
    };

    HUFFMAN_API ByteVector Compress(const std::string& text, FreqMap& freqMap, size_t& bitLength);
    HUFFMAN_API std::string Decompress(const ByteVector& compressed, const FreqMap& freqMap, size_t bitLength);

    namespace Methods {
        HUFFMAN_API HuffmanNode* BuildHuffmanTree(const FreqMap& freqMap);
        HUFFMAN_API void GenerateCodes(HuffmanNode* node, const std::string& code, std::map<Char, std::string>& huffmanCode);
        HUFFMAN_API ByteVector PackBitsToBytes(const std::string& bitString, size_t& bitLength);
        HUFFMAN_API std::string UnpackBytesToBits(const ByteVector& compressedData, size_t bitLength);
        HUFFMAN_API void FreeTree(HuffmanNode* node);
    }

    namespace Stringize {
        HUFFMAN_API std::string StringizeFreqMap(const Huffman::FreqMap& freqMap);
        HUFFMAN_API std::string StringizeByteVec(const Huffman::ByteVector& byteVec);
    }
}

#endif // HUFFMAN_H