#include "framework/tinytest.h"
#include "../huffpress/huffpress.h"
#include <string>
#include <chrono>
#include <cstring>
#include <iostream>

// Test 1 (00): File initialization with data
ttret_t test_initialize_file(void) {
    std::string testData = "Hello, World!";

    Huffpress::HuffpressFile file(testData);
    ttcheck(file.header.magic[0] == 'H');  // Check the magic number
    ttcheck(file.header.magic[1] == 'P');
    ttcheck(file.header.magic[2] == 'F');
    ttcheck(file.header.version[0] == 0);  // Check the version
    ttcheck(file.header.version[1] == 1);
    ttcheck(file.header.freqMap.size() > 0);  // Ensure the frequency map is not empty
    tinytestdone();
}

// Test 2 (01): Data modification
ttret_t test_modify_file(void) {
    std::string testData = "Hello, World!";
    Huffpress::HuffpressFile file(testData);
    
    std::string newData = "Goodbye, World!";
    file.Modify(newData);
    
    ttcheck(file.header.size > 0);  // The size of the data should be updated
    ttcheck(file.header.freqMap.size() > 0);  // The frequency map should be updated
    // printf("'%s' == '%s'\n", newData.c_str(), file.header.Decompress().c_str());
    ttcheck(file.Decompress().compare(newData) == 0); // Decompressed string comparing

    tinytestdone();
}

// Test 3 (02): Serialization and deserialization
ttret_t test_serialize_and_deserialize(void) {
    // auto start = std::chrono::high_resolution_clock::now();
    std::string testData = "Hello World!";
    Huffpress::HuffpressFile file(testData);
    
    // Serialization
    std::string filePath = "testfile.hpf";
    file.Serialize(filePath);
    
    // Deserialization
    Huffpress::HuffpressFile loadedFile;
    loadedFile.Parse(filePath);    
    // Check data integrity
    ttcheck(loadedFile.header.magic[0] == 'H');
    ttcheck(loadedFile.header.magic[1] == 'P');
    ttcheck(loadedFile.header.magic[2] == 'F');
    ttcheck(checksum(testData.c_str(), testData.size()) == loadedFile.header.sourceChecksum);
    ttcheck(loadedFile.Decompress().compare(testData) == 0);
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double, std::milli> duration = end - start;

    // std::cout << "Duration: " << duration.count() << " ms" << std::endl;
    
    ttcheck(std::remove(filePath.c_str()) == 0); // Remove 'testfile.hpf'
    tinytestdone();
}

// Test 4 (03): Buffered serialization and deserialization
ttret_t test_buffered_serialize_and_deserialize(void) {   
    // auto start = std::chrono::high_resolution_clock::now();
    std::string testData = "Hello World!";
    Huffpress::HuffpressFile file(testData);

    // Serialization
    const std::string filePath = "testfile.hpf";
    file.BufferedSerialize(filePath); 

    // Deserialiation
    Huffpress::HuffpressFile loadedFile;
    loadedFile.BufferedParse(filePath);

    ttcheck(loadedFile.header.magic[0] == 'H');
    ttcheck(loadedFile.header.magic[1] == 'P');
    ttcheck(loadedFile.header.magic[2] == 'F');
    ttcheck(checksum(testData.c_str(), testData.size()) == loadedFile.header.sourceChecksum);
    ttcheck(loadedFile.Decompress().compare(testData) == 0);
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double, std::milli> duration = end - start;

    // std::cout << "Duration: " << duration.count() << " ms" << std::endl;=

    ttcheck(std::remove(filePath.c_str()) == 0); // Remove 'testfile.hpf'
    ttdone();
}

// Test 5 (05): Checksum validation
ttret_t test_check_sums(void) {
    std::string testData = "Hello, World!";
    Huffpress::HuffpressFile file(testData);
    
    ttcheck(file.header.sourceChecksum != 0);  // The source checksum should not be 0
    ttcheck(file.header.compressedChecksum != 0);  // The compressed checksum should not be 0
    ttcheck(file.header.sourceChecksum == checksum(testData.c_str(), testData.size())); // The source checksum be a testData checksum

    tinytestdone();
}

// Array of test functions
ttest_t tests[] = {
    { test_initialize_file, "Test initialization"                           },
    { test_modify_file, "Test modification"                                 },
    { test_serialize_and_deserialize, "Test serialization"                  },
    { test_buffered_serialize_and_deserialize, "Test buff. serialization"   },
    { test_check_sums, "Test checksum validation"                           }
};

// Main function to run the tests
int main(void) {
    return tt_suit(tests, ttnumtests(tests));
}
