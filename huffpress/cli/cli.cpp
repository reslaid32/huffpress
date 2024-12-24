#define HUFFPRESS_CLI_LIBRARY_BUILD

#include "cli.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

namespace Huffpress {
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string RESET = "\033[0m";

    HUFFPRESS_CLI_API HuffpressCLI::HuffpressCLI() : filePath_(""), file_() {}

    HUFFPRESS_CLI_API void HuffpressCLI::run_() {
        std::string line;
        std::cout << "Welcome to Huffpress CLI! Type 'exit' to quit.\n";

        while (doCliLoop_) {
            std::cout << "[" << getSelectedFilePath() << "] # ";
            std::getline(std::cin, line);

            if (line.empty()) continue;

            std::vector<std::string> commands = splitCommands(line);
            for (const std::string& cmd : commands) {
                std::vector<std::string> tokens = tokenize(cmd);
                executeCommand(tokens);
            }
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::run() {
        doCliLoop_ = true;
        run_();
    }

    // HUFFPRESS_CLI_API void HuffpressCLI::stop() {
    //     doCliLoop_ = false;
    // }

    HUFFPRESS_CLI_API void HuffpressCLI::run(const std::string& str) {
        std::vector<std::string> commands = splitCommands(str);

        for (const std::string& cmd : commands) {
            std::vector<std::string> tokens = tokenize(cmd);
            executeCommand(tokens);
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::run(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            if (std::strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
                std::string commands = argv[i + 1];
                std::vector<std::string> commandList = splitCommands(commands);
                for (const std::string& cmd : commandList) {
                    std::vector<std::string> tokens = tokenize(cmd);
                    executeCommand(tokens);
                }
            }
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::runCombine(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            if (std::strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
                std::string commands = argv[i + 1];
                std::vector<std::string> commandList = splitCommands(commands);
                for (const std::string& cmd : commandList) {
                    std::vector<std::string> tokens = tokenize(cmd);
                    executeCommand(tokens);
                }
            }
        }
        run(); // stdin
    }

    HUFFPRESS_CLI_API std::string HuffpressCLI::getSelectedFilePath() {
        if (filePath_.empty()) return RED+"None" + RESET;
        return GREEN + filePath_ + RESET;
    }

    HUFFPRESS_CLI_API std::vector<std::string> HuffpressCLI::splitCommands(const std::string& line) {
        std::vector<std::string> commands;
        std::string currentCommand;
        bool insideQuotes = false;

        for (char c : line) {
            if (c == '"' && (currentCommand.empty() || currentCommand.back() != '\\')) {
                insideQuotes = !insideQuotes; 
            }
            if (!insideQuotes && (c == '&' || c == ';' || c == '\n')) {
                if (!currentCommand.empty()) {
                    commands.push_back(currentCommand);
                    currentCommand.clear();
                }
                continue;
            }
            currentCommand.push_back(c);
        }
        if (!currentCommand.empty()) {
            commands.push_back(currentCommand);
        }
        return commands;
    }

    HUFFPRESS_CLI_API std::vector<std::string> HuffpressCLI::tokenize(const std::string& cmd) {
        std::vector<std::string> tokens;
        std::stringstream ss(cmd);
        std::string token;
        while (ss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    HUFFPRESS_CLI_API void HuffpressCLI::executeCommand(const std::vector<std::string>& tokens) {
        if (tokens.empty()) return;

        std::string command = tokens[0];
        std::string remainingTokens = getRemainingTokens(tokens);

        if (!command.empty() && command[0] == '!') {
            std::string systemCommand = command.substr(1) + " " + remainingTokens;
            handleSystemCommand(systemCommand);
            return;
        }

        if (command == "help") {
            showHelp();
        }
        else if (command == "touch") {
            handleTouch(remainingTokens);
        }
        else if (command == "remove") {
            handleRemove(remainingTokens);
        }
        else if (command == "exists") {
            handleExists(remainingTokens);
        }
        else if (command == "select") {
            handleSelect(remainingTokens);
        }
        else if (command == "deselect") {
            handleDeselect();
        }
        else if (command == "cat") {
            handleCat();
        }
        else if (command == "set") {
            handleSet(remainingTokens);
        }
        else if (command == "dump") {
            handleDump(remainingTokens);
        }
        else if (command == "load") {
            handleLoad(remainingTokens);
        }
        else if (command == "clean") {
            handleClean(remainingTokens);
        }
        else if (command == "commit") {
            handleCommit();
        }
        else if (command == "revert") {
            handleRevert();
        }
        else if (command == "refresh") {
            handleRefresh();
        }
        else if (command == "file") {
            handleFile();
        }
        else if (command == "version") {
            handleVersion();
        }
        // else if (command == "run") {
        //     handleRun();
        // }
        // else if (command == "stop") {
        //     handleStop();
        // }
        else if (command == "exit") {
            quit(EXIT_SUCCESS);
        }
        else {
            std::cout << "Unknown command: " << command << "\n";
        }
    }

    HUFFPRESS_CLI_API std::string HuffpressCLI::getRemainingTokens(const std::vector<std::string>& tokens) {
        return tokens.size() > 1 ? std::accumulate(tokens.begin() + 1, tokens.end(), std::string(),
            [](const std::string& a, const std::string& b) { return a.empty() ? b : a + " " + b; }) : "";
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleTouch(const std::string& remainingTokens) {
        try {
            createOrClearFile(remainingTokens);
            Huffpress::HuffpressFile tempFile = Huffpress::HuffpressFile();
            tempFile.Modify({}, {{}}, 0);
            tempFile.BufferedSerialize(remainingTokens);
            std::cout << "Successfully\n";
        } catch (const std::exception& e) {
            std::cout << "An error has occurred: " << e.what() << "\n";
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleRemove(const std::string& remainingTokens) {
        try {
            if (fileExists(remainingTokens)) {
                std::remove(remainingTokens.c_str());
                std::cout << "Successfully\n";
            } else {
                std::cout << "File does not exist, skip\n";
            }
        } catch (const std::exception& e) {
            std::cout << "An error has occurred: " << e.what() << "\n";
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleExists(const std::string& remainingTokens) {
        try {
            bool exists = fileExists(remainingTokens);
            std::cout << "Exists: " << exists << "\n";
        } catch (const std::exception& e) {
            std::cout << "An error has occurred: " << e.what() << "\n";
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleSelect(const std::string& remainingTokens) {
        if (fileExists(remainingTokens)) {
            try {
                filePath_ = remainingTokens;
                file_.BufferedParse(filePath_, 0x20000);
                std::cout << "File " << filePath_ << " opened!\n";
            } catch (const std::exception& e) {
                std::cout << "[HPF] Parsing error: " << e.what() << "\n";
            }
        } else {
            std::cout << "File does not exist: " << remainingTokens << "\n";
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleDeselect() {
        if (filePath_.empty()) {
            std::cout << "You do not have an open file, skip\n";
        } else {
            filePath_ = "";
            file_ = Huffpress::HuffpressFile();
            std::cout << "The file is closed!\n";
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleRefresh() {
        if (filePath_.empty()) {
            std::cout << "You do not have an open file, skip\n";
        } else {
            file_.Parse(filePath_);
            std::cout << "Successfully\n";
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleCat() {
        if (filePath_.empty()) {
            std::cout << "You do not have an open file, skip\n";
        } else {
            std::cout << file_.Decompress() << "\n";
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleSet(const std::string& remainingTokens) {
        if (filePath_.empty()) {
            std::cout << "You do not have an open file, skip\n";
        } else {
            try {
                file_.Modify(remainingTokens);
                std::cout << "Successful, commit the changes with commit\n";
            } catch (const std::exception& e) {
                std::cout << "An error has occurred: " << e.what() << "\n";
            }
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleDump(const std::string& remainingTokens) {
        if (filePath_.empty()) {
            std::cout << "You do not have an open file, skip\n";
        } else {
            try {
                if (!remainingTokens.empty()) {
                    bufferedWrite(remainingTokens, file_.Decompress(), 0x20000);
                }
                std::cout << "Successful\n";
            } catch (const std::exception& e) {
                std::cout << "An error has occurred: " << e.what() << "\n";
            }
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleLoad(const std::string& remainingTokens) {
        if (filePath_.empty()) {
            std::cout << "You do not have an open file, skip\n";
        } else {
            try {
                file_.Modify(bufferedRead(remainingTokens, 0x20000));
                std::cout << "Successful, commit the changes with commit\n";
            } catch (const std::exception& e) {
                std::cout << "An error has occurred: " << e.what() << "\n";
            }
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleClean(const std::string& remainingTokens) {
        if (filePath_.empty()) {
            std::cout << "You do not have an open file, skip\n";
        } else {
            try {
                file_.Modify({}, {{}}, 0);
                std::cout << "Successful, commit the changes with commit\n";
            } catch (const std::exception& e) {
                std::cout << "An error has occurred: " << e.what() << "\n";
            }
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleCommit() {
        if (filePath_.empty()) {
            std::cout << "You do not have an open file, skip\n";
        } else {
            try {
                file_.BufferedSerialize(filePath_, 0x20000);
                std::cout << "Successfully\n";
            } catch (const std::exception& e) {
                std::cout << "An error has occurred: " << e.what() << "\n";
            }
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleRevert() {
        if (filePath_.empty()) {
            std::cout << "You do not have an open file, skip\n";
        } else {
            try {
                file_.BufferedParse(filePath_, 0x20000);
                std::cout << "Successfully\n";
            } catch (const std::exception& e) {
                std::cout << "An error has occurred: " << e.what() << "\n";
            }
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleFile() {
        if (filePath_.empty()) {
            std::cout << "You do not have an open file, skip\n";
        } else {
            showFileInfo(filePath_, file_);
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::handleVersion() {
        printf("Library version: %d.%d.%d\n", Huffpress::Version[0], Huffpress::Version[1], Huffpress::Version[2]);
    }

    // HUFFPRESS_CLI_API void HuffpressCLI::handleRun() {
    //     if (doCliLoop_) {
    //         std::cout << "The loop has already running" << std::endl;
    //     } else {
    //         run();
    //     }
    // }

    // HUFFPRESS_CLI_API void HuffpressCLI::handleStop() {
    //     if (!doCliLoop_) {
    //         std::cout << "Loop not running" << std::endl;
    //     } else {
    //         stop();
    //     }
    // }

    HUFFPRESS_CLI_API void HuffpressCLI::handleSystemCommand(const std::string& remainingToken) {
        system(remainingToken.c_str());
    }

    HUFFPRESS_CLI_API void HuffpressCLI::quit(int code) {
        std::cout << "Exiting...\n";
        exit(code);
    }

    HUFFPRESS_CLI_API void HuffpressCLI::showHelp() {
        std::cout << "Available commands:\n";
        std::cout << "  help                - Show this help message\n";
        std::cout << "  touch <path>        - Creates a clean selected file; if there is a file, it will overwrite it with a clean one\n";
        std::cout << "  remove <path>       - Deletes a file\n";
        std::cout << "  exists <path>       - Shows whether the file exists\n";
        std::cout << "  select <path>       - Open and parse a Huffpress file\n";
        std::cout << "  deselect            - Close Huffpress file\n";
        std::cout << "  cat                 - Write the decompressed content from selected file to the console\n";
        std::cout << "  set <content>       - Load and compress the content and save to selected file\n";
        std::cout << "  dump <path>         - Write decompressed content from selected file to another file\n";
        std::cout << "  load <path>         - Load and compress the content of the specified file and save to selected file\n";
        std::cout << "  clean               - Clear selected file\n";
        std::cout << "  commit              - Overwrites the selected file with all changes\n";
        std::cout << "  revert              - Roll back changes that have not been committed\n";
        std::cout << "  refresh             - Refresh file buffer\n";
        std::cout << "  version             - Write a huffpress library version\n";
        std::cout << "  file                - Write a file info\n";
        // std::cout << "  run                 - Run console loop\n";
        // std::cout << "  stop                - Stop console loop\n";
        std::cout << "  exit                - Exit the program\n";
        std::cout << "Available prefixes:\n";
        std::cout << "  !...                - Execute everything after '!' in the console\n";
    }

    HUFFPRESS_CLI_API void HuffpressCLI::showFileInfo(const std::string& filePath, Huffpress::HuffpressFile& file) {
        std::cout << "Target: " << filePath << std::endl;
        std::cout << "  Magic: " << file.header.magic << "\n";
        printf(      "  Version: %d.%d.%d\n", file.header.version[0], file.header.version[1], file.header.version[2]);
        std::cout << "  FreqMapSize: " << file.header.freqMap.size() << "\n";
        std::cout << "  BitLength: " << file.header.bitLength << "\n";
        size_t sourceSize = file.Decompress().size();
        std::cout << "  Source size: " << sourceSize << "\n";
        std::cout << "  Compressed size " << file.byteVec.size() << " (" << file.header.size << " counting the size according to the header)\n";
        std::cout << "  Compression efficiency: " << calculateCompressionEfficiency(sourceSize, file.byteVec.size()) << "%\n";
        std::cout << "  Source checksum: " << file.header.sourceChecksum << "\n";
        std::cout << "  Compressed checksum: " << file.header.compressedChecksum << "\n";
    }

    HUFFPRESS_CLI_API double HuffpressCLI::calculateCompressionEfficiency(size_t sourceSize, size_t compressedSize) {
        return (1.0 - static_cast<double>(compressedSize) / sourceSize) * 100;
    }

    HUFFPRESS_CLI_API bool HuffpressCLI::fileExists(const std::string& path) {
        std::ifstream f(path.c_str());
        return f.good();
    }

    HUFFPRESS_CLI_API void HuffpressCLI::createOrClearFile(const std::string& filePath) {
        std::ofstream outFile(filePath, std::ios::trunc);
        if (!outFile) {
            throw std::runtime_error("Could not create or clear the file.");
        }
    }

    HUFFPRESS_CLI_API void HuffpressCLI::bufferedWrite(const std::string& filePath, const std::string& data, size_t bufferSize) {
        std::ofstream outFile(filePath, std::ios::binary);
        if (!outFile) {
            throw std::ios_base::failure("Failed to open file for writing: " + filePath);
        }

        size_t dataSize = data.size();
        size_t written = 0;

        while (written < dataSize) {
            size_t chunkSize = std::min(bufferSize, dataSize - written);
            outFile.write(data.data() + written, chunkSize);
            written += chunkSize;
        }

        outFile.close();
        if (!outFile) {
            throw std::ios_base::failure("Failed to finalize writing to file: " + filePath);
        }
    }

    HUFFPRESS_CLI_API std::string HuffpressCLI:: bufferedRead(const std::string& filePath, size_t bufferSize) {
        std::ifstream inFile(filePath, std::ios::binary);
        if (!inFile) {
            throw std::ios_base::failure("Failed to open file for reading: " + filePath);
        }

        std::string data;
        std::vector<char> buffer(bufferSize);
        
        while (inFile) {
            inFile.read(buffer.data(), bufferSize);
            data.append(buffer.data(), inFile.gcount());
        }

        inFile.close();
        if (!inFile.eof() && !inFile) {
            throw std::ios_base::failure("Error occurred while reading file: " + filePath);
        }

        return data;
    }
}