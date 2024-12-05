#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "TCPInterface.h"
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

class TCPClient : public TCPInterface {
private:
    int serverSocket;
    sockaddr_in serverAddr;

public:
    TCPClient(const std::string& serverIP, int port);
    ~TCPClient();

    void connectToServer();
    void send(const std::string& data);
    std::string receive(int nbytes) override;

private:
    void closeSocket(int socket);
};

#endif // TCP_CLIENT_H
