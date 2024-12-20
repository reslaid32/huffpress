#include "framework/tinytest.h"
#include "../huffpress/huffpress.h"
#include <string>

// Test 1: File initialization with data
ttret_t test_initialize_file(void) {
    std::string testData = "Hello, World!";
    Huffpress::HuffpressFile* file = Huffpress::InitializeFile(testData);
    ttcheck(file != nullptr);  // Ensure the file was successfully initialized
    ttcheck(file->magic[0] == 'H');  // Check the magic number
    ttcheck(file->magic[1] == 'P');
    ttcheck(file->magic[2] == 'F');
    ttcheck(file->version[0] == 0);  // Check the version
    ttcheck(file->version[1] == 1);
    ttcheck(file->freqMap.size() > 0);  // Ensure the frequency map is not empty
    delete file;  // Free memory
    tinytestdone();
}

// Test 2: Data modification
ttret_t test_modify_file(void) {
    std::string testData = "Hello, World!";
    Huffpress::HuffpressFile* file = Huffpress::InitializeFile(testData);
    
    std::string newData = "Goodbye, World!";
    Huffpress::Modify(file, newData);
    
    ttcheck(file->size > 0);  // The size of the data should be updated
    ttcheck(file->freqMap.size() > 0);  // The frequency map should be updated

    delete file;  // Free memory
    tinytestdone();
}

// Test 3: Serialization and deserialization
ttret_t test_serialize_and_deserialize(void) {
    std::string testData = "Hello, World!";
    Huffpress::HuffpressFile* file = Huffpress::InitializeFile(testData);
    
    // Serialization
    std::string filePath = "testfile.hpf";
    Huffpress::Serialize(file, filePath);
    
    // Deserialization
    Huffpress::HuffpressFile* loadedFile = Huffpress::ParseFile(filePath);
    ttcheck(loadedFile != nullptr);  // Ensure the file was successfully deserialized
    
    // Check data integrity
    ttcheck(loadedFile->magic[0] == 'H');
    ttcheck(loadedFile->magic[1] == 'P');
    ttcheck(loadedFile->magic[2] == 'F');
    
    delete file;  // Free memory
    delete loadedFile;  // Free memory
    ttcheck(std::remove(filePath.c_str()) == 0); // Remove 'testfile.hpf'
    tinytestdone();
}

// Test 4: Checksum validation
ttret_t test_check_sums(void) {
    std::string testData = "Hello, World!";
    Huffpress::HuffpressFile* file = Huffpress::InitializeFile(testData);
    
    ttcheck(file->sourceChecksum != 0);  // The source checksum should not be 0
    ttcheck(file->compressedChecksum != 0);  // The compressed checksum should not be 0

    delete file;  // Free memory
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
