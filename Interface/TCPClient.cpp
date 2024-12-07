#include "TCPClient.h"
#include <iostream>
#include <arpa/inet.h>

TCPClient::TCPClient(const std::string& serverIP, int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw TCPException("Error creating socket");
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
        closeSocket(serverSocket);
        throw TCPException("Invalid server IP address");
    }
}

TCPClient::~TCPClient() {
    closeSocket(serverSocket);
}

void TCPClient::connectToServer() {
    if (connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw TCPException("Error connecting to server");
    }
    std::cout << "Connected to server!" << std::endl;
}

void TCPClient::send(const std::string& data) {
    if (write(serverSocket, data.c_str(), data.size()) < 0) {
        throw TCPException("Error sending data");
    }
}

std::string TCPClient::receive(int nbytes) {
    char buffer[nbytes+1];
    memset(buffer, 0, nbytes);
    ssize_t bytesRead = read(serverSocket, buffer, nbytes);
    buffer[bytesRead] = '\0';
    if (bytesRead < 0) {
        throw TCPException("Error receiving data");
    }
    return std::string(buffer);
}

void TCPClient::closeSocket(int socket) {
    if (socket > 0) {
        close(socket);
    }
}
