#ifndef TCP_INTERFACE_H
#define TCP_INTERFACE_H

#include <string>
#include <stdexcept>

// Base interface class
class TCPInterface {
public:
    virtual ~TCPInterface() = default;

    // Method to receive data
    virtual std::string receive(int nbytes) = 0;
};

// Exception class for TCP errors
class TCPException : public std::runtime_error {
public:
    explicit TCPException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif // TCP_INTERFACE_H
