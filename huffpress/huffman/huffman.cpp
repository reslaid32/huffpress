#include "huffman.h"
#include <queue>
#include <vector>
#include <sstream>

namespace Huffman {

    HuffmanNode::HuffmanNode(char data, int freq) {
        this->data = data;
        this->freq = freq;
        left = right = nullptr;
    }

    bool HuffmanCompare::operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->freq > right->freq;
    }

    namespace Methods {
        HuffmanNode* BuildHuffmanTree(const Huffman::FreqMap& freqMap) {
            std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, HuffmanCompare> pq;

            for (const auto& pair : freqMap) {
                pq.push(new HuffmanNode(pair.first, pair.second));
            }

            while (pq.size() != 1) {
                HuffmanNode *left = pq.top(); pq.pop();
                HuffmanNode *right = pq.top(); pq.pop();

                int sum = left->freq + right->freq;
                HuffmanNode* node = new HuffmanNode('\0', sum);
                node->left = left;
                node->right = right;
                pq.push(node);
            }

            return pq.top();
        }

        void GenerateCodes(HuffmanNode* node, const std::string& code, std::map<char, std::string>& huffmanCode) {
            if (!node) return;

            if (!node->left && !node->right) {
                huffmanCode[node->data] = code;
            }

            GenerateCodes(node->left, code + '0', huffmanCode);
            GenerateCodes(node->right, code + '1', huffmanCode);
        }

        void FreeTree(HuffmanNode* node) {
            if (node == nullptr) return;
            FreeTree(node->left);
            FreeTree(node->right);
            delete node;
        }

        Huffman::ByteVector PackBitsToBytes(const std::string& bitString, size_t& bitLength) {
            Huffman::ByteVector compressedData;
            Huffman::Byte currentByte = 0;
            int bitCount = 0;

            bitLength = bitString.size();

            for (char bit : bitString) {
                currentByte <<= 1;
                if (bit == '1') {
                    currentByte |= 1;
                }
                bitCount++;

                if (bitCount == 8) {
                    compressedData.push_back(currentByte);
                    currentByte = 0;
                    bitCount = 0;
                }
            }

            if (bitCount > 0) {
                currentByte <<= (8 - bitCount);
                compressedData.push_back(currentByte);
            }

            return compressedData;
        }

        std::string UnpackBytesToBits(const Huffman::ByteVector& compressedData, size_t bitLength) {
            std::string bitString;

            for (Huffman::Byte byte : compressedData) {
                for (int i = 7; i >= 0; --i) {
                    bitString += (byte & (1 << i)) ? '1' : '0';
                }
            }

            return bitString.substr(0, bitLength);
        }

    }

    Huffman::ByteVector Compress(const std::string& text, FreqMap& freqMap, size_t& bitLength) {
        for (char ch : text) {
            freqMap[ch]++;
        }

        HuffmanNode* root = Methods::BuildHuffmanTree(freqMap);
        
        std::map<char, std::string> huffmanCode;
        Methods::GenerateCodes(root, std::string(), huffmanCode);

        std::string bitString;
        for (char ch : text) {
            bitString += huffmanCode[ch];
        }

        Huffman::ByteVector compressed = Methods::PackBitsToBytes(bitString, bitLength);
        Methods::FreeTree(root);
        return compressed;
    }

    std::string Decompress(const ByteVector& compressed, const FreqMap& freqMap, size_t bitLength) {
        HuffmanNode* root = Methods::BuildHuffmanTree(freqMap);
        std::string result;

        std::string bitString = Methods::UnpackBytesToBits(compressed, bitLength);
        HuffmanNode* current = root;
        for (char bit : bitString) {
            current = (bit == '0') ? current->left : current->right;

            if (!current->left && !current->right) {
                result += current->data;
                current = root;
            }
        }

        Methods::FreeTree(root);
        return result;
    }

    namespace Stringize {
        std::string StringizeFreqMap(const Huffman::FreqMap& freqMap) {
            std::stringstream ss;

            for (const auto& pair : freqMap) {
                ss << "'" << pair.first << "'" << ":" << "'" << pair.second << "'" << " "; 
            }

            return ss.str();
        }

        std::string StringizeByteVec(const Huffman::ByteVector& byteVec) {
            std::stringstream ss;

            for (size_t i = 0; i < byteVec.size(); ++i) {
                ss << byteVec[i];
            }

            return ss.str();
        }
    }
}