#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "TCPInterface.h"
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <vector>

class TCPServer : public TCPInterface {
private:
    int serverSocket;
    int clientSocket;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
public:
    std::vector<int> clientList;
    TCPServer(int port);
    ~TCPServer();

    void start();
    void send(const std::string& data, int clientSocket);
    void sendToAll(const std::string& data);
    int connectToClient();
    std::string receive(int  bufferSize) override;

private:
    void closeSocket(int socket);
};

#endif // TCP_SERVER_H
