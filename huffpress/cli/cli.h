#ifndef HUFFPRESS_CLI_H
#define HUFFPRESS_CLI_H

#include "export.h"
#include "../huffpress.h" 

#include <vector>

namespace Huffpress {
    class HUFFPRESS_CLI_API HuffpressCLI {
    public:
        HUFFPRESS_CLI_API HuffpressCLI();
        
        HUFFPRESS_CLI_API void run();
        HUFFPRESS_CLI_API void run(const std::string& str);
        HUFFPRESS_CLI_API void run(int argc, char* argv[]);

    private:
        std::string filePath;
        Huffpress::HuffpressFile file;

        HUFFPRESS_CLI_API std::string getSelectedFilePath();
        HUFFPRESS_CLI_API std::vector<std::string> splitCommands(const std::string& line);
        HUFFPRESS_CLI_API std::vector<std::string> tokenize(const std::string& cmd);
        HUFFPRESS_CLI_API void executeCommand(const std::vector<std::string>& tokens);
        HUFFPRESS_CLI_API std::string getRemainingTokens(const std::vector<std::string>& tokens);

        HUFFPRESS_CLI_API void handleTouch(const std::string& remainingTokens);
        HUFFPRESS_CLI_API void handleRemove(const std::string& remainingTokens);
        HUFFPRESS_CLI_API void handleExists(const std::string& remainingTokens);
        HUFFPRESS_CLI_API void handleSelect(const std::string& remainingTokens);
        HUFFPRESS_CLI_API void handleDeselect();
        HUFFPRESS_CLI_API void handleRefresh();
        HUFFPRESS_CLI_API void handleCat();
        HUFFPRESS_CLI_API void handleSet(const std::string& remainingTokens);
        HUFFPRESS_CLI_API void handleDump(const std::string& remainingTokens);
        HUFFPRESS_CLI_API void handleLoad(const std::string& remainingTokens);
        HUFFPRESS_CLI_API void handleClean(const std::string& remainingTokens);
        HUFFPRESS_CLI_API void handleCommit();
        HUFFPRESS_CLI_API void handleRevert();
        HUFFPRESS_CLI_API void handleFile();
        HUFFPRESS_CLI_API void handleVersion();
        
        HUFFPRESS_CLI_API void handleSystemCommand(const std::string& remainingToken);
        
        HUFFPRESS_CLI_API void quit(int code);
        HUFFPRESS_CLI_API void showHelp();
        HUFFPRESS_CLI_API void showFileInfo(const std::string& filePath, Huffpress::HuffpressFile& file);
        
        HUFFPRESS_CLI_API double calculateCompressionEfficiency(size_t sourceSize, size_t compressedSize);
        HUFFPRESS_CLI_API bool fileExists(const std::string& path);
        HUFFPRESS_CLI_API void createOrClearFile(const std::string& filePath);
        HUFFPRESS_CLI_API void bufferedWrite(const std::string& filePath, const std::string& data, size_t bufferSize);
        HUFFPRESS_CLI_API std::string bufferedRead(const std::string& filePath, size_t bufferSize);
    };
} // Huffpress

#endif // HUFFPRESS_CLI_H
