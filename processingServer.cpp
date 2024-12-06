#include "Interface/TCPClient.h"
#include "config.hpp"
#include <iostream>
#include <thread>
using namespace std;

TCPClient processingServer(IP_ADDRESS, PORT);

int main()
{
    processingServer.connectToServer();
    string message = "P";
    processingServer.send(message);
    message = "";
    while (true) {
        cout << "Enter message: ";
        getline(cin, message);
        if (message == "exit") {
            processingServer.send("o");
            break;
        }
    }
    return 0;
}