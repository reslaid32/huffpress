#include <stdexcept>

namespace Huffpress::Exceptions
{
    class HuffpressException : public std::exception {
    public:
        explicit HuffpressException(const std::string& message) : message_(message) {}
        const char* what() const noexcept override {
            return message_.c_str();
        }

    private:
        std::string message_;
    };

    class FileOpenException : public HuffpressException {
    public:
        explicit FileOpenException(const std::string& filePath) 
            : HuffpressException("Error opening file: " + filePath) {}
    };

    class SerializationException : public HuffpressException {
    public:
        explicit SerializationException(const std::string& message) 
            : HuffpressException("Serialization error: " + message) {}
    };

    class DeserializationException : public HuffpressException {
    public:
        explicit DeserializationException(const std::string& message) 
            : HuffpressException("Deserialization error: " + message) {}
    };
} // namespace Huffpress::Exceptions
