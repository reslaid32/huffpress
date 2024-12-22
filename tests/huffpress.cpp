#include "framework/tinytest.h"
#include "../huffpress/huffpress.h"
#include <string>

// Test 1: File initialization with data
ttret_t test_initialize_file(void) {
    std::string testData = "Hello, World!";

    Huffpress::HuffpressFile file(testData);
    ttcheck(file.magic[0] == 'H');  // Check the magic number
    ttcheck(file.magic[1] == 'P');
    ttcheck(file.magic[2] == 'F');
    ttcheck(file.version[0] == 0);  // Check the version
    ttcheck(file.version[1] == 1);
    ttcheck(file.freqMap.size() > 0);  // Ensure the frequency map is not empty
    tinytestdone();
}

// Test 2: Data modification
ttret_t test_modify_file(void) {
    std::string testData = "Hello, World!";
    Huffpress::HuffpressFile file(testData);
    
    std::string newData = "Goodbye, World!";
    file.Modify(newData);
    
    ttcheck(file.size > 0);  // The size of the data should be updated
    ttcheck(file.freqMap.size() > 0);  // The frequency map should be updated
    // printf("'%s' == '%s'\n", newData.c_str(), file.Decompress().c_str());
    ttcheck(file.Decompress().compare(newData) == 0); // Decompressed string comparing

    tinytestdone();
}

// Test 3: Serialization and deserialization
ttret_t test_serialize_and_deserialize(void) {
    std::string testData = "Hello, World!";
    Huffpress::HuffpressFile file(testData);
    
    // Serialization
    std::string filePath = "testfile.hpf";
    file.Serialize(filePath);
    
    // Deserialization
    Huffpress::HuffpressFile loadedFile;
    loadedFile.Parse(filePath);    
    // Check data integrity
    ttcheck(loadedFile.magic[0] == 'H');
    ttcheck(loadedFile.magic[1] == 'P');
    ttcheck(loadedFile.magic[2] == 'F');
    ttcheck(loadedFile.Decompress().compare(testData) == 0);
    
    ttcheck(std::remove(filePath.c_str()) == 0); // Remove 'testfile.hpf'
    tinytestdone();
}

// Test 4: Checksum validation
ttret_t test_check_sums(void) {
    std::string testData = "Hello, World!";
    Huffpress::HuffpressFile file(testData);
    
    ttcheck(file.sourceChecksum != 0);  // The source checksum should not be 0
    ttcheck(file.compressedChecksum != 0);  // The compressed checksum should not be 0
    ttcheck(file.sourceChecksum == checksum(testData.c_str(), testData.size())); // The source checksum be a testData checksum

    tinytestdone();
}

// Array of test functions
ttest_t tests[] = {
    { test_initialize_file, "Test initialization" },
    { test_modify_file, "Test modification" },
    { test_serialize_and_deserialize, "Test serialization and deserialization" },
    { test_check_sums, "Test checksum validation" }
};

// Main function to run the tests
int main(void) {
    return tt_suit(tests, ttnumtests(tests));
}
