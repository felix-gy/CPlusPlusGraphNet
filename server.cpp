#include "Interface/TCPServer.h"
#include "config.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <set>
#include <map>
using namespace std;

TCPServer server(PORT);

set<int> processingServerList; 
set<int> clientList;
// Servidores de Procesamiento
void manageProcessingServer(int PServerSocket)
{
    string message;
    while (true) {
        message = server.receive(PServerSocket, 1);
        if (message.empty()) break;
        switch (message[0]) {
            case 'o':
                processingServerList.erase(PServerSocket);
                cout << "Processing server disconnected - "<< PServerSocket << endl;
                break;
            default:{
                cout << "Unknown message" << endl;
                break;
            }
        }
    }
}

// Cliente
void manageClient(int clientSocket)
{
    string message;
    while (true) {
        message = server.receive(clientSocket, 1);
        if (message.empty()) break;
        switch (message[0]) {
            case 'o':
                processingServerList.erase(clientSocket);
                cout << "Client disconnected - "<< clientSocket << endl;
                break;
            default:{
                cout << "Unknown message" << endl;
                break;
            }
        }
    }
}

void socketThread(int clientSocket)
{
    string message;
    message = server.receive(clientSocket, 1);
    if (message == "P") { // Para los servidores de procesamiento
        processingServerList.insert(clientSocket);
        manageProcessingServer(clientSocket);
    }
    else if (message == "C") { // Para los clientes
        clientList.insert(clientSocket);
        manageClient(clientSocket);
    }

}

int main()
{
    server.start();

    int clientSocket;
    while (true) {
        clientSocket = server.connectToClient();
        thread(socketThread, clientSocket).detach();
    }
    return 0;
}