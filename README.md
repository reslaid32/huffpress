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
  Huffpress::HuffpressFile file;
  file.Init("Hello, World!");
  file.Serialize("output.hpf");
  ```

### `void BufferedSerialize(const std::string& filePath, const size_t bufferSize)`
- **Description**: Serializes a Huffpress file to the specified file path using a buffer of the given size. This function writes the file in chunks to improve performance when working with large files. The file is written in the Huffpress format, including both compressed data and the header.
- **Usage**:
  ```cpp
  Huffpress::HuffpressFile file;
  file.BufferedSerialize("output.hpf", 1024);  // Buffer size is 1024 bytes
  ```

### `void SerializeToBuffer(Huffman::ByteVector& buffer)`
- **Description**: Serializes the `HuffpressFile` object into a buffer (`Huffman::ByteVector`). The buffer will contain the compressed data and header information.
- **Usage**:
  ```cpp
  Huffpress::HuffpressFile file;
  file.Init("Hello, World!");
  Huffman::ByteVector buffer;
  file.SerializeToBuffer(buffer);
  ```

### `void Parse(const std::string& filePath)`
- **Description**: Parses a Huffpress file from the specified file path. The file must be in the Huffpress format, containing both compressed data and a header.
- **Usage**:
  ```cpp
  Huffpress::HuffpressFile file;
  file.Parse("output.hpf");
  ```

### `void BufferedParse(const std::string& filePath, const size_t bufferSize)`
- **Description**: Parses a Huffpress file from the specified file path using a buffer of the given size. This function reads the file in chunks to reduce memory usage, making it more efficient for larger files. The file must be in the Huffpress format, containing both compressed data and a header.
- **Usage**:
  ```cpp
  Huffpress::HuffpressFile file;
  file.BufferedParse("output.hpf", 1024);  // Buffer size is 1024 bytes
  ```

### `void ParseFromBuffer(const Huffman::ByteVector& buffer)`
- **Description**: Parses a Huffpress file from the given buffer. The buffer must contain the compressed data and header in the correct format.
- **Usage**:
  ```cpp
  Huffpress::HuffpressFile file;
  Huffman::ByteVector buffer; // previously filled buffer
  file.ParseFromBuffer(buffer);
  ```

### `void Modify(const std::string& data)`
- **Description**: Modifies the `HuffpressFile` object by compressing the new string data and updating the file accordingly. This replaces the old compressed data.
- **Usage**:
  ```cpp
  Huffpress::HuffpressFile file;
  file.Init("Old Data");
  file.Modify("New Data");
  ```

### `void Modify(const Huffman::ByteVector& newByteVec, const Huffman::FreqMap& newFreqMap, size_t bitLength)`
- **Description**: Modifies the `HuffpressFile` object with the new byte vector, frequency map, and bit length. This method allows direct setting of the compressed data, frequency map, and the bit length. (The null value is done with: `file.Modify({}, {{}}, 0)`)
- **Usage**:
  ```cpp
  Huffpress::HuffpressFile file;
  Huffman::ByteVector newByteVec = {/* compressed data */};
  Huffman::FreqMap newFreqMap = {/* frequency map */};
  size_t bitLength = 1000; // Example bit length
  file.Modify(newByteVec, newFreqMap, bitLength);
  ```

### `std::string Decompress()`
- **Description**: Decompresses the data stored in the `HuffpressFile` object and returns the original uncompressed string.
- **Usage**:
  ```cpp
  Huffpress::HuffpressFile file;
  file.Init("Hello, World!");
  std::string decompressedData = file.Decompress();
  ```

## File Header Structure

The header for the Huffpress file contains critical information, including magic bytes, version, frequency map, checksums, and bit lengths. It ensures that the file can be validated and parsed correctly.

```cpp
struct _HuffpressFileHeader {
    char magic[3] = {'H', 'P', 'F'}; // Magic number for file identification
    uint8_t version[3];              // Version (major, minor)
    Huffman::FreqMap freqMap;        // Frequency map for Huffman encoding
    size_t bitLength;                // Bit length of the compressed data
    size_t size = 0;                 // Size of the compressed data
    checksum_t sourceChecksum;       // Checksum of the original data
    checksum_t compressedChecksum;   // Checksum of the compressed data
};
```

# HuffpressCLI Class Documentation (in [cli.h](./huffpress/cli/cli.h))

The `HuffpressCLI` class is designed to provide a command-line interface (CLI) for interacting with the `Huffpress` compression format. It allows users to run commands to manipulate Huffpress files, including actions like creating, modifying, compressing, and decompressing files. This class is intended for use with the `huffpress` compression format in a terminal or shell environment.

## Commands & Prefixes

| Command                | Description                                                                                     |
|------------------------|-------------------------------------------------------------------------------------------------|
| `help`                | Show this help message                                                                          |
| `touch <path>`        | Creates a clean selected file; if there is a file, it will overwrite it with a clean one        |
| `remove <path>`       | Deletes a file                                                                                  |
| `exists <path>`       | Shows whether the file exists                                                                   |
| `select <path>`       | Open and parse a Huffpress file                                                                 |
| `deselect`            | Close Huffpress file                                                                            |
| `cat`                 | Write the decompressed content from the selected file to the console                            |
| `set <content>`       | Load and compress the content and save to the selected file                                     |
| `dump <path>`         | Write decompressed content from the selected file to another file                               |
| `load <path>`         | Load and compress the content of the specified file and save to the selected file               |
| `clean`               | Clear the selected file                                                                         |
| `commit`              | Overwrites the selected file with all changes                                                   |
| `revert`              | Roll back changes that have not been committed                                                  |
| `refresh`             | Refresh file buffer                                                                             |
| `version`             | Write the Huffpress library version                                                             |
| `file`                | Write file info                                                                                 |
| `exit`                | Exit the program                                                                                |

| Prefix                | Description                                                                                     |
|-----------------------|-------------------------------------------------------------------------------------------------|
| `!...`               | Execute everything after `!` in the console                                                      |

## Constructor

### `HuffpressCLI()`
- **Description**: Default constructor for initializing the `HuffpressCLI` object, which prepares the interface to handle user commands.
- **Usage**:
  ```cpp
  Huffpress::HuffpressCLI cli;
  ```

## Methods

### `void run()`
- **Description**: Starts the CLI, awaiting user input. This method processes the input and executes commands interactively until the user decides to quit.
- **Usage**:
  ```cpp
  Huffpress::HuffpressCLI cli;
  cli.run();
  ```

### `void run(const std::string& str)`
- **Description**: Runs a single command from the provided string input. This method parses and executes the command without interactive input.
- **Usage**:
  ```cpp
  Huffpress::HuffpressCLI cli;
  cli.run("touch test.hpf");
  ```

### `void run(int argc, char* argv[])`
- **Description**: Executes commands passed as arguments in the command line. This method allows for batch processing or script execution.
- **Usage**:
  ```cpp
  Huffpress::HuffpressCLI cli;
  cli.run(argc, argv);
  ```