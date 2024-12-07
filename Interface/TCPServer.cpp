#include "TCPServer.h"
#include "TCPInterface.h"
#include <iostream>
#include <arpa/inet.h>
TCPServer::TCPServer(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw TCPException("Error creating socket");
    }
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        closeSocket(serverSocket);
        TCPException("Error setting socket options");
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    // Obtener la IP y puerto del servidor
    struct sockaddr_in serverInfo;
    socklen_t len = sizeof(serverInfo);
    if (getsockname(serverSocket, (struct sockaddr*)&serverInfo, &len) == 0) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(serverInfo.sin_addr), ip, INET_ADDRSTRLEN);
        std::cout << "Servidor iniciado en IP: " << ip
             << " y puerto: " << ntohs(serverInfo.sin_port) << std::endl;
    } else {
        std::cerr << "Error al obtener información del socket." << std::endl;
    }
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
    char buffer[nbytes+1];
    memset(buffer, 0, nbytes);
    ssize_t bytesRead = read(clientSocket_, buffer, nbytes);
    buffer[bytesRead] = '\0';
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
