#include "Interface/TCPClient.h"
#include "config.hpp"
#include <iostream>
#include <thread>
#include <sstream> 
#include <iomanip>
using namespace std;

TCPClient client(IP_ADDRESS, PORT);

void readThread()
{
    string message = "";
    while (true) {
        message = client.receive(11);
        if (message.empty()) {
            cout << "message is empty" << endl;
            break;
        }
        cout << "Received: " << message << endl;
    }
}

int main()
{
    client.connectToServer();
    thread(readThread).detach();
    string message = "C";
    client.send(message);
    do  {
        cout << "Enter message: ";
        getline(cin, message);
        if (message == "exit") {
            client.send("o");
            break;
        }
        client.send(message);
    } while (true);

    return 0;
}