#include "TCPServer.h"
#include <iostream>

TCPServer::TCPServer(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw TCPException("Error creating socket");
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        closeSocket(serverSocket);
        throw TCPException("Error binding socket");
    }
}

TCPServer::~TCPServer() {
    closeSocket(serverSocket);
}

void TCPServer::start() {
    if (listen(serverSocket, 1) < 0) {
        throw TCPException("Error listening on socket");
    }
    std::cout << "Server is listening..." << std::endl;
}

int TCPServer::connectToClient() {
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket < 0) {
        throw TCPException("Error accepting client connection");
    }
    std::cout << "Client connected!" << std::endl;
    return clientSocket;
}

void TCPServer::send(const std::string& data, int clientSocket_) {
    if (write(clientSocket_, data.c_str(), data.size()) < 0) {
        throw TCPException("Error sending data");
    }
}

std::string TCPServer::receive(int clientSocket_, int nbytes) {
    char buffer[nbytes];
    memset(buffer, 0, nbytes);
    ssize_t bytesRead = read(clientSocket_, buffer, nbytes);
    if (bytesRead < 0) {
        throw TCPException("Error receiving data");
    }
    return std::string(buffer);
}

void TCPServer::closeSocket(int socket) {
    if (socket > 0) {
        close(socket);
    }
}
