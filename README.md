# huffpress
Compression format written in C++ (hpf) written only on self-written libraries and the standard C/C++ library

# HuffpressFile Class Documentation (in [huffpress.h](./huffpress/huffpress.h))

The `HuffpressFile` class is designed to handle Huffman compression and decompression of data. It provides various methods to initialize, modify, serialize, and deserialize Huffpress files.

## Constructor

### `HuffpressFile()`
- Default constructor for creating an empty `HuffpressFile` object.

### `HuffpressFile(const std::string& data)`
- Initializes the `HuffpressFile` object with the given string data. The data will be compressed and stored within the object.

## Methods

### `void Init(const std::string& data)`
- **Description**: Initializes the `HuffpressFile` object with the given string data. Compresses the data and prepares it for storage.
- **Usage**:
  ```cpp
  HuffpressFile file;
  file.Init("Hello, World!");
  ```

### `void Serialize(const std::string& filePath)`
- **Description**: Serializes the `HuffpressFile` object to a file at the specified path. The file contains the compressed data and the necessary headers.
- **Usage**:
  ```cpp
  HuffpressFile file;
  file.Init("Hello, World!");
  file.Serialize("output.hpf");
  ```

### `void SerializeToBuffer(Huffman::ByteVector& buffer)`
- **Description**: Serializes the `HuffpressFile` object into a buffer (`Huffman::ByteVector`). The buffer will contain the compressed data and header information.
- **Usage**:
  ```cpp
  HuffpressFile file;
  file.Init("Hello, World!");
  Huffman::ByteVector buffer;
  file.SerializeToBuffer(buffer);
  ```

### `void Parse(const std::string& filePath)`
- **Description**: Parses a Huffpress file from the specified file path. The file must be in the Huffpress format, containing both compressed data and a header.
- **Usage**:
  ```cpp
  HuffpressFile file;
  file.Parse("output.hpf");
  ```

### `void ParseFromBuffer(const Huffman::ByteVector& buffer)`
- **Description**: Parses a Huffpress file from the given buffer. The buffer must contain the compressed data and header in the correct format.
- **Usage**:
  ```cpp
  HuffpressFile file;
  Huffman::ByteVector buffer; // previously filled buffer
  file.ParseFromBuffer(buffer);
  ```

### `void Modify(const std::string& data)`
- **Description**: Modifies the `HuffpressFile` object by compressing the new string data and updating the file accordingly. This replaces the old compressed data.
- **Usage**:
  ```cpp
  HuffpressFile file;
  file.Init("Old Data");
  file.Modify("New Data");
  ```

### `void Modify(const Huffman::ByteVector& newByteVec, const Huffman::FreqMap& newFreqMap, size_t bitLength)`
- **Description**: Modifies the `HuffpressFile` object with the new byte vector, frequency map, and bit length. This method allows direct setting of the compressed data, frequency map, and the bit length.
- **Usage**:
  ```cpp
  HuffpressFile file;
  Huffman::ByteVector newByteVec = {/* compressed data */};
  Huffman::FreqMap newFreqMap = {/* frequency map */};
  size_t bitLength = 1000; // Example bit length
  file.Modify(newByteVec, newFreqMap, bitLength);
  ```

### `std::string Decompress()`
- **Description**: Decompresses the data stored in the `HuffpressFile` object and returns the original uncompressed string.
- **Usage**:
  ```cpp
  HuffpressFile file;
  file.Init("Hello, World!");
  std::string decompressedData = file.Decompress();
  ```

## File Header Structure

The header for the Huffpress file contains critical information, including magic bytes, version, frequency map, checksums, and bit lengths. It ensures that the file can be validated and parsed correctly.

```cpp
struct _HuffpressFileHeader {
    char magic[3] = {'H', 'P', 'F'}; // Magic number for file identification
    uint8_t version[3] = {0, 1, 0};  // Version (major, minor)
    Huffman::FreqMap freqMap;        // Frequency map for Huffman encoding
    size_t bitLength;                // Bit length of the compressed data
    size_t size = 0;                 // Size of the compressed data
    checksum_t sourceChecksum;       // Checksum of the original data
    checksum_t compressedChecksum;   // Checksum of the compressed data
};
```