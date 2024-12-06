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
    int clientSocket = 0;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
public:
    TCPServer(int port);
    ~TCPServer();

    void start();
    void send(const std::string& data, int clientSocket);
    int connectToClient();
    std::string receive(int clientSocket_, int nbytes); ;
private:
    void closeSocket(int socket);
};

#endif // TCP_SERVER_H
